/*******************************************************************************
  eFuse Driver Source File

  Company:
    Microchip Technology Inc.

  File Name:
    efuse_driver.c

  Summary:
    eFuse driver implementation for PIC32CX-BZ3/BZ6 devices.

  Description:
    Implements eFuse read and program operations using ROT_REGS.
    Supports both BZ3 (WBZ35x) and BZ6 device families.

    Register Reference (from Datasheet Section 13):
    - EFUSE_CON (offset 0xC08):
      Bit 16: EN_OTP_LDO - Enable OTP LDO for programming
      Bit 7:  EN_PGM - Start programming
      Bit 6:  EN_LD_ALL - Load all eFuse to holding registers
      Bit 5:  EN_LD - Load single eFuse byte
      Bit 1:  PGM_MODE - Programming mode enable
      Bit 0:  PGM_1BIT - 1-bit programming mode

    - EFUSE_RWDATA (offset 0xC04):
      Bits 27:16: ADDR[11:0] - eFuse address
      Bits 7:0:   DATA[7:0] - eFuse data

    WARNING: eFuse programming is IRREVERSIBLE!
*******************************************************************************/

#include "efuse_driver.h"
#include "boot_key_config.h"
#include "definitions.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>  // For memcpy (used in EFUSE_ProgramSecureBootKey)

// TRNG support for SRK generation (like HUT)
#include "config/default/peripheral/rom_api/include/sxsymcrypt/trng.h"
#include "config/default/peripheral/rom_api/include/sxsymcrypt/statuscodes.h"
#include "config/default/peripheral/rom_api/api_table.h"

// Only full X || Y for P-256 or P-384 is accepted. An inferred array length
// prevents a short initializer from being silently padded as a P-384 key.
typedef char EFUSE_PublicKey_must_be_64_or_96_bytes[
    (sizeof(publicBootKey) == 64U || sizeof(publicBootKey) == 96U) ? 1 : -1];
#define EFUSE_SBK_COORD_BYTES (sizeof(publicBootKey) / 2U)
#define EFUSE_SBK_PARITY_INDEX (sizeof(publicBootKey) - 1U)
// HUT places P-256 in the final 32 bytes of the 48-byte boot-key slot.
// P-384 uses the complete slot.
#define EFUSE_SBK_SLOT_OFFSET (48U - EFUSE_SBK_COORD_BYTES)

const char *EFUSE_ConfiguredSecureBootCurve(void)
{
    return EFUSE_SBK_COORD_BYTES == 32U ? "ECDSA P-256" : "ECDSA P-384";
}

// ============================================================================
// Device-Specific Includes and Definitions
// ============================================================================
#ifdef DEVICE_BZ3
    // BZ3 / WBZ35x specific - uses WBZ351_DFP headers
    // ROT_REGS, DSU_REGS already defined in definitions.h
#endif

#ifdef DEVICE_BZ6
    // BZ6 specific - uses PIC32CX-BZ6_DFP headers
    // ROT_REGS, DSU_REGS already defined in definitions.h
#endif

// ============================================================================
// EFUSE_CON Register Bit Definitions (from Datasheet DS70005541E, Section 13.8.1)
// ============================================================================
#define EFUSE_CON_EN_OTP_LDO_Pos    16
#define EFUSE_CON_EN_OTP_LDO_Msk    (0x01UL << EFUSE_CON_EN_OTP_LDO_Pos)  // Bit 16

#define EFUSE_CON_EN_PGM_Pos        7
#define EFUSE_CON_EN_PGM_Msk        (0x01UL << EFUSE_CON_EN_PGM_Pos)      // Bit 7

#define EFUSE_CON_EN_LD_ALL_Pos     6
#define EFUSE_CON_EN_LD_ALL_Msk     (0x01UL << EFUSE_CON_EN_LD_ALL_Pos)   // Bit 6

#define EFUSE_CON_EN_LD_Pos         5
#define EFUSE_CON_EN_LD_Msk         (0x01UL << EFUSE_CON_EN_LD_Pos)       // Bit 5

#define EFUSE_CON_PGM_MODE_Pos      1
#define EFUSE_CON_PGM_MODE_Msk      (0x01UL << EFUSE_CON_PGM_MODE_Pos)    // Bit 1

#define EFUSE_CON_PGM_1BIT_Pos      0
#define EFUSE_CON_PGM_1BIT_Msk      (0x01UL << EFUSE_CON_PGM_1BIT_Pos)    // Bit 0

// ============================================================================
// EFUSE_RWDATA Register Bit Definitions (from Datasheet DS70005541E, Section 13.8.2)
// ============================================================================
#define EFUSE_RWDATA_ADDR_Pos       16      // Bits 27:16 for address
#define EFUSE_RWDATA_ADDR_Msk       (0x0FFFUL << EFUSE_RWDATA_ADDR_Pos)

#define EFUSE_RWDATA_DATA_Pos       0       // Bits 7:0 for data
#define EFUSE_RWDATA_DATA_Msk       (0xFFUL << EFUSE_RWDATA_DATA_Pos)

// ============================================================================
// eFuse Programming Addresses (from Datasheet Figure 13-1)
// ============================================================================
#define EFUSE_PROG_ADDR_SECCFG              0x0000
#define EFUSE_PROG_ADDR_ANTI_ROLLBACK       0x0004
#define EFUSE_PROG_ADDR_LIFECYCLE           0x0024  // Life cycle counter (15-bit)
#define EFUSE_PROG_ADDR_SECURE_BOOT_KEY     0x0026  // Secure Boot Key (384-bit = 48 bytes)
#define EFUSE_PROG_ADDR_STORAGE_ROOT_KEY    0x0056  // Storage Root Key (256-bit = 32 bytes)
#define EFUSE_PROG_ADDR_UUID                0x0170

// Debug flag - set to 1 to enable debug output
#ifndef EFUSE_DEBUG_ENABLED
#define EFUSE_DEBUG_ENABLED  0
#endif

// ============================================================================
// Storage Root Key (SRK) - TRNG-generated at boot (like HUT)
// ============================================================================
static uint8_t s_storageRootKey[32];
static uint8_t s_storageRootKeyValid = 0;

#if EFUSE_DEBUG_ENABLED
#include <stdio.h>
#include "peripheral/sercom/usart/plib_sercom0_usart.h"
static void Debug_Print(const char *msg)
{
    SERCOM0_USART_Write((uint8_t *)msg, strlen(msg));
    while(!SERCOM0_USART_TransmitComplete());
}
static void Debug_PrintHex32(uint32_t val)
{
    char buf[12];
    sprintf(buf, "0x%08lX", (unsigned long)val);
    Debug_Print(buf);
}
#endif

// Forward declarations of internal functions
static void RotEfuseProgram(uint16_t addr, uint8_t data);
static void RotEfuseProgramBit(uint16_t bitAddr, uint8_t bitValue);

/*******************************************************************************
  Function:
    void EFUSE_ReverseKeyBytes(uint8_t *data, uint8_t size)

  Summary:
    Reverses the byte order of a data array (for EFUSE little-endian storage).

  Description:
    Per eference implementation, the public key bytes must be reversed
    before programming to EFUSE. This matches the reverse_order() function in
    write_key().

  Parameters:
    data - Pointer to byte array to reverse in-place
    size - Number of bytes in the array

  Remarks:
    This function modifies the data in-place.
*******************************************************************************/
void EFUSE_ReverseKeyBytes(uint8_t *data, uint8_t size)
{
    uint8_t tmp;
    for(uint8_t i = 0; i < size / 2; i++)
    {
        tmp = data[size - 1 - i];
        data[size - 1 - i] = data[i];
        data[i] = tmp;
    }
}

// Console task context only. These are application guard times, not silicon
// maximum programming/read times; confirm them on both families before release.
#define EFUSE_PROGRAM_TIMEOUT_MS 100U
#define EFUSE_RELOAD_TIMEOUT_MS  100U
#define EFUSE_STALLED_CLOCK_POLLS 1000000UL
#define EFUSE_ACTIVE_Msk (EFUSE_CON_EN_PGM_Msk | EFUSE_CON_EN_LD_ALL_Msk | EFUSE_CON_EN_LD_Msk)

// A timed-out operation has an uncertain outcome. Retain its cause and block
// further controller writes in this session, even if the busy bit later clears.
static EFUSE_STATUS s_controllerFault = EFUSE_STATUS_SUCCESS;

EFUSE_STATUS EFUSE_GetControllerStatus(void)
{
    if(s_controllerFault != EFUSE_STATUS_SUCCESS)
    {
        return s_controllerFault;
    }
    if((ROT_REGS->ROT_EFUSE_CON & EFUSE_ACTIVE_Msk) != 0)
    {
        return EFUSE_STATUS_CONTROLLER_BUSY;
    }
    return EFUSE_STATUS_SUCCESS;
}

static bool RotEfuseClockAvailable(void)
{
    // Do not enter a tick-based wait before the scheduler or with ticks masked.
    return xTaskGetCurrentTaskHandle() != NULL && __get_IPSR() == 0U &&
           __get_PRIMASK() == 0U && __get_BASEPRI() == 0U &&
           (SysTick->CTRL & (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk)) ==
           (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

static EFUSE_STATUS RotEfuseWait(uint32_t mask, uint32_t timeoutMs,
                                EFUSE_STATUS timeoutStatus)
{
    TickType_t start = xTaskGetTickCount();
    TickType_t last = start;
    // One extra tick ensures the guard does not expire early at a tick boundary.
    TickType_t timeout = pdMS_TO_TICKS(timeoutMs) + 1U;
    uint32_t stalled = 0U;
    while((ROT_REGS->ROT_EFUSE_CON & mask) != 0)
    {
        TickType_t now = xTaskGetTickCount();
        if((TickType_t)(now - start) >= timeout)
        {
            s_controllerFault = timeoutStatus;
            return timeoutStatus;
        }
        if(now != last)
        {
            last = now;
            stalled = 0U;
        }
        else if(++stalled >= EFUSE_STALLED_CLOCK_POLLS)
        {
            // This separate guard diagnoses a stopped clock, not elapsed time.
            s_controllerFault = EFUSE_STATUS_TIMEBASE_ERROR;
            return s_controllerFault;
        }
    }
    return EFUSE_STATUS_SUCCESS;
}

// Refresh holding registers with a bounded wait; no output is used on failure.
static EFUSE_STATUS RotEfuseLoadChecked(void)
{
    EFUSE_STATUS status = EFUSE_GetControllerStatus();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(!RotEfuseClockAvailable())
    {
        return EFUSE_STATUS_TIMEBASE_ERROR;
    }
    // No active operation: clear manual LDO enable before starting a load.
    ROT_REGS->ROT_EFUSE_RWDATA = 0U;
    ROT_REGS->ROT_EFUSE_CON = EFUSE_CON_EN_LD_ALL_Msk;
    return RotEfuseWait(EFUSE_CON_EN_LD_ALL_Msk, EFUSE_RELOAD_TIMEOUT_MS,
                       EFUSE_STATUS_RELOAD_TIMEOUT);
}

static EFUSE_STATUS RotEfuseProgramChecked(uint16_t addr, uint8_t data)
{
    EFUSE_STATUS status = EFUSE_GetControllerStatus();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(!RotEfuseClockAvailable())
    {
        return EFUSE_STATUS_TIMEBASE_ERROR;
    }
    // Preserve the BZ6 manual-LDO byte programming sequence.
    ROT_REGS->ROT_EFUSE_CON = 0U;
    ROT_REGS->ROT_EFUSE_CON = EFUSE_CON_EN_OTP_LDO_Msk;
    for(volatile uint32_t i = 0U; i < 1000U; i++);

    // Byte offset is carried in ADDR[11:3]; programming data is in DATA[7:0].
    ROT_REGS->ROT_EFUSE_RWDATA = ((uint32_t)addr << 19) | data;
    ROT_REGS->ROT_EFUSE_CON = EFUSE_CON_EN_OTP_LDO_Msk |
                              EFUSE_CON_PGM_MODE_Msk | EFUSE_CON_EN_PGM_Msk;
    status = RotEfuseWait(EFUSE_CON_EN_PGM_Msk, EFUSE_PROGRAM_TIMEOUT_MS,
                         EFUSE_STATUS_PROGRAM_TIMEOUT);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        // Datasheet: control/data may not change while EN_PGM is set.
        // Leave the operation untouched, retain the error, and do not reload.
        return status;
    }
    // Disable the BZ6 OTP LDO only after confirmed programming completion.
    ROT_REGS->ROT_EFUSE_CON = 0U;
    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    static void RotEfuseProgram(uint16_t addr, uint8_t data)

  Summary:
    Programs a single byte to eFuse memory.

  Description:
    Programs the specified data byte at the given eFuse address.
    WARNING: This operation is IRREVERSIBLE!

    Per Datasheet Section 13.3.1 Programming Sequence:
    1. Write EFUSE_CON.PGM_MODE = 0
    2. Write EFUSE_CON.EN_LD = 0, EN_LD_ALL = 0
    3. Write EFUSE_CON.EN_OTP_LDO = 1 (enable OTP LDO to 1.5V)
    4. Write EFUSE_RWDATA with address and data
    5. Set EFUSE_CON.PGM_MODE, EFUSE_CON.EN_PGM (and PGM_1BIT if needed)
    6. Wait for EN_PGM to clear
    7. Write EFUSE_CON.EN_OTP_LDO = 0

  Parameters:
    addr - eFuse byte address (0x0000 - 0x01FF)
    data - Byte value to program

  Remarks:
    eFuse bits can only be changed from 0 to 1, never from 1 to 0.
*******************************************************************************/
static void RotEfuseProgram(uint16_t addr, uint8_t data)
{
    if(EFUSE_GetControllerStatus() != EFUSE_STATUS_SUCCESS)
    {
        return;
    }

#if EFUSE_DEBUG_ENABLED
    Debug_Print("\r\n[DBG] eFuse byte programming started (data hidden)\r\n");
#endif

    // Step 1-2: Clear control register
    ROT_REGS->ROT_EFUSE_CON = 0;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] EFUSE_CON after clear: ");
    Debug_PrintHex32(ROT_REGS->ROT_EFUSE_CON);
    Debug_Print("\r\n");
#endif

    // Step 3: Enable OTP LDO
    ROT_REGS->ROT_EFUSE_CON = EFUSE_CON_EN_OTP_LDO_Msk;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] EFUSE_CON after EN_OTP_LDO: ");
    Debug_PrintHex32(ROT_REGS->ROT_EFUSE_CON);
    Debug_Print("\r\n");
#endif

    // Small delay for LDO to stabilize
    for(volatile uint32_t i = 0; i < 1000; i++);

    // Step 4: Set address and data
    //  #define ADDR_TO_BYTE_ADDR_OFFSET(x) (((uint32_t)x)<<19)
    // EFUSERWDATA = (ADDR_TO_BYTE_ADDR_OFFSET(addr_offset + i) | data[i]);
    // So byte address is shifted left by 19 bits, data is in bits [7:0]
    uint32_t rwdata = ((uint32_t)addr << 19) | (data & 0xFF);
    ROT_REGS->ROT_EFUSE_RWDATA = rwdata;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] eFuse programming command prepared (data hidden)\r\n");
#endif

    // Step 5: Start programming - set EN_OTP_LDO + PGM_MODE + EN_PGM
    // Keep EN_OTP_LDO enabled during programming!
    uint32_t con_val = EFUSE_CON_EN_OTP_LDO_Msk | EFUSE_CON_PGM_MODE_Msk | EFUSE_CON_EN_PGM_Msk;
    ROT_REGS->ROT_EFUSE_CON = con_val;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] EFUSE_CON for program: ");
    Debug_PrintHex32(con_val);
    Debug_Print("\r\n");
    Debug_Print("[DBG] EFUSE_CON readback: ");
    Debug_PrintHex32(ROT_REGS->ROT_EFUSE_CON);
    Debug_Print("\r\n");
#endif

    // Step 6: Wait for program to complete (EN_PGM bit clears when done)
    uint32_t timeout = 100000;
    while((ROT_REGS->ROT_EFUSE_CON & EFUSE_CON_EN_PGM_Msk) != 0 && timeout > 0)
    {
        timeout--;
    }

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] EFUSE_CON after wait: ");
    Debug_PrintHex32(ROT_REGS->ROT_EFUSE_CON);
    if(timeout == 0) Debug_Print(" (TIMEOUT!)");
    Debug_Print("\r\n");
#endif

    // Step 7: Disable OTP LDO
    ROT_REGS->ROT_EFUSE_CON = 0;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] Program complete\r\n");
#endif
}

/*******************************************************************************
  Function:
    static void RotEfuseProgramBit(uint16_t bitAddr, uint8_t bitValue)

  Summary:
    Programs a single BIT to eFuse memory (1-bit mode).

  Description:
    Per datasheet Section 13.3.1 Note:
    "Lifecycle counter and anti-rollback programming values to be programmed
     in 1-bit operation mode."

    In 1-bit mode:
    - ADDR[11:0] is the bit address
    - DATA[0] is the bit value to program

  Parameters:
    bitAddr  - eFuse bit address (0x0000 - 0x0FFF)
    bitValue - Bit value to program (0 or 1)
*******************************************************************************/
static void RotEfuseProgramBit(uint16_t bitAddr, uint8_t bitValue)
{
    if(EFUSE_GetControllerStatus() != EFUSE_STATUS_SUCCESS)
    {
        return;
    }

#if EFUSE_DEBUG_ENABLED
    Debug_Print("\r\n[DBG] eFuse bit programming started (data hidden)\r\n");
#endif

    // Step 1-2: Clear control register
    ROT_REGS->ROT_EFUSE_CON = 0;

    // Step 3: Enable OTP LDO
    ROT_REGS->ROT_EFUSE_CON = EFUSE_CON_EN_OTP_LDO_Msk;

    // Small delay for LDO to stabilize
    for(volatile uint32_t i = 0; i < 1000; i++);

    // Step 4: Set bit address (bits 27:16) and bit data (bit 0)
    // In 1-bit mode: ADDR[11:0] is full bit address, DATA[0] is bit value
    uint32_t rwdata = ((uint32_t)bitAddr << EFUSE_RWDATA_ADDR_Pos) | (bitValue & 0x01);
    ROT_REGS->ROT_EFUSE_RWDATA = rwdata;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] eFuse programming command prepared (data hidden)\r\n");
#endif

    // Step 5: Start programming with PGM_1BIT = 1
    // Set EN_OTP_LDO + PGM_MODE + PGM_1BIT + EN_PGM all at once
    uint32_t con_val = EFUSE_CON_EN_OTP_LDO_Msk |
                       EFUSE_CON_PGM_MODE_Msk |
                       EFUSE_CON_PGM_1BIT_Msk |
                       EFUSE_CON_EN_PGM_Msk;
    ROT_REGS->ROT_EFUSE_CON = con_val;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] EFUSE_CON for 1-bit program: ");
    Debug_PrintHex32(con_val);
    Debug_Print("\r\n");
    Debug_Print("[DBG] EFUSE_CON readback: ");
    Debug_PrintHex32(ROT_REGS->ROT_EFUSE_CON);
    Debug_Print("\r\n");
#endif

    // Step 6: Wait for program to complete
    uint32_t timeout = 100000;
    while((ROT_REGS->ROT_EFUSE_CON & EFUSE_CON_EN_PGM_Msk) != 0 && timeout > 0)
    {
        timeout--;
    }

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] EFUSE_CON after wait: ");
    Debug_PrintHex32(ROT_REGS->ROT_EFUSE_CON);
    if(timeout == 0) Debug_Print(" (TIMEOUT!)");
    Debug_Print("\r\n");
#endif

    // Step 7: Disable OTP LDO
    ROT_REGS->ROT_EFUSE_CON = 0;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ReadAntiRollback(uint8_t *value)

  Summary:
    Reads the Anti-Rollback Counter value from holding register.

  Parameters:
    value - Pointer to store the counter value (0-255)

  Returns:
    EFUSE_STATUS_SUCCESS or EFUSE_STATUS_INVALID_PARAM
*******************************************************************************/
EFUSE_STATUS EFUSE_ReadAntiRollback(uint8_t *value)
{
    if(value == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    EFUSE_STATUS status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // Read from ROT_ANTIROLLBACKCOUNTER[0] holding register
    *value = (uint8_t)(ROT_REGS->ROT_ANTIROLLBACKCOUNTER[0] & 0xFF);

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ReadUUID(EFUSE_UUID_t *uuid)

  Summary:
    Reads the 128-bit UUID from holding registers.

  Parameters:
    uuid - Pointer to UUID structure to store the value

  Returns:
    EFUSE_STATUS_SUCCESS or EFUSE_STATUS_INVALID_PARAM
*******************************************************************************/
EFUSE_STATUS EFUSE_ReadUUID(EFUSE_UUID_t *uuid)
{
    if(uuid == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    EFUSE_STATUS status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // Read UUID from ROT_UUID[] holding registers (ROT offset 0x170)
    // Note: eFuse raw address is 0x1D8, but ROT holding register is at 0x170
    uuid->word0 = ROT_REGS->ROT_UUID[0];
    uuid->word1 = ROT_REGS->ROT_UUID[1];
    uuid->word2 = ROT_REGS->ROT_UUID[2];
    uuid->word3 = ROT_REGS->ROT_UUID[3];

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ReadSECCFG(EFUSE_SECCFG_t *seccfg)

  Summary:
    Reads the Security Configuration from holding register.

  Parameters:
    seccfg - Pointer to SECCFG structure to store the values

  Returns:
    EFUSE_STATUS_SUCCESS or EFUSE_STATUS_INVALID_PARAM
*******************************************************************************/
EFUSE_STATUS EFUSE_ReadSECCFG(EFUSE_SECCFG_t *seccfg)
{
    if(seccfg == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    EFUSE_STATUS status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // Read from ROT_SECCFG holding register
    uint32_t regVal = ROT_REGS->ROT_SECCFG;
    seccfg->rawValue = regVal;

    // Extract DEBUG_LOCK field (bits 7:6 per DFP)
    seccfg->debugLock = (uint8_t)((regVal & ROT_SECCFG_DEBUG_LCK_Msk) >> ROT_SECCFG_DEBUG_LCK_Pos);

    // Extract UUID_LOCK field (bits 5:4 per DFP) - locked if non-zero
    seccfg->uuidLocked = ((regVal & ROT_SECCFG_UUID_LCK_Msk) != 0);

    // Extract ROOT_KEY_LOCK field (bits 9:8) - locked if non-zero
    seccfg->rootKeyLocked = ((regVal & ROT_SECCFG_ROOT_KEY_LCK_Msk) != 0);

    // Extract BOOT_KEY_LOCK field (bits 11:10) - locked if non-zero
    seccfg->bootKeyLocked = ((regVal & ROT_SECCFG_BOOT_KEY_LCK_Msk) != 0);

    // Extract PATCH_LCK bits [15:14], matching the BZ3 patch.
    seccfg->patchLocked = ((regVal & EFUSE_SECCFG_PATCH_LOCK_Msk) != 0);

    // Extract the raw ADD_BOOT_KEY bit (bit 16); this programmer uses it as Y parity.
    seccfg->addBootKey = ((regVal & ROT_SECCFG_ADD_BOOT_KEY_Msk) != 0);

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramAntiRollback(uint8_t value)

  Summary:
    Programs the Anti-Rollback Counter.

  Description:
    WARNING: This operation is IRREVERSIBLE!

    The Anti-Rollback Counter is a 255-bit THERMOMETER counter (decoded to 8-bit).
    Per datasheet Section 13.3.1 Note:
    "Lifecycle counter and anti-rollback programming values to be programmed
     in 1-bit operation mode."

    For a thermometer counter, to set value N, bits 0 through N-1 must be set to 1.
    Example: value=3 means bits 0,1,2 are set (binary: ...000111)

    The eFuse base address for anti-rollback is 0x0004 (byte address).
    In 1-bit mode, the bit address = (byte_address * 8) + bit_position

  Parameters:
    value - Counter value to program (0-255)

  Returns:
    EFUSE_STATUS_SUCCESS or EFUSE_STATUS_ERROR
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramAntiRollback(uint8_t value)
{
    // Read current value first
    uint8_t currentVal;
    EFUSE_STATUS status = EFUSE_ReadAntiRollback(&currentVal);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

#if EFUSE_DEBUG_ENABLED
    char buf[32];
    Debug_Print("\r\n[DBG] ProgramAntiRollback: current=");
    sprintf(buf, "%d", currentVal);
    Debug_Print(buf);
    Debug_Print(", target=");
    sprintf(buf, "%d", value);
    Debug_Print(buf);
    Debug_Print("\r\n");
#endif

    // eFuse bits can only go 0->1, so new value must be >= current
    if(value < currentVal)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    // If value already matches, nothing to do
    if(value == currentVal)
    {
        return EFUSE_STATUS_SUCCESS;
    }

    // Anti-rollback is a thermometer counter at eFuse address 0x0004
    // Base bit address = 0x0004 * 8 = 0x0020 (32 in decimal)
    // To set counter to value N, we need to program bits 0 through N-1
    // But we only need to program bits from currentVal to value-1 (already set bits stay set)

    uint16_t baseBitAddr = EFUSE_PROG_ADDR_ANTI_ROLLBACK * 8;  // 0x0004 * 8 = 0x20

    for(uint8_t i = currentVal; i < value; i++)
    {
        uint16_t bitAddr = baseBitAddr + i;
#if EFUSE_DEBUG_ENABLED
        Debug_Print("[DBG] Programming bit ");
        sprintf(buf, "%d", i);
        Debug_Print(buf);
        Debug_Print(" at addr ");
        sprintf(buf, "0x%04X", bitAddr);
        Debug_Print(buf);
        Debug_Print("\r\n");
#endif
        RotEfuseProgramBit(bitAddr, 1);
    }

    // Small delay for eFuse to settle
    for(volatile uint32_t i = 0; i < 10000; i++);

    // Reload and verify
    status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    uint8_t readBack = (uint8_t)(ROT_REGS->ROT_ANTIROLLBACKCOUNTER[0] & 0xFF);

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] Read back value: ");
    sprintf(buf, "%d", readBack);
    Debug_Print(buf);
    Debug_Print("\r\n");
#endif

    if(readBack != value)
    {
        return EFUSE_STATUS_VERIFY_ERROR;
    }

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramUUID(const EFUSE_UUID_t *uuid)

  Summary:
    Programs the 128-bit UUID.

  Description:
    WARNING: This operation is IRREVERSIBLE!
    Programs UUID using BYTE MODE (matching HUT application approach).
    Does NOT automatically lock - call EFUSE_LockUUID() after verification.

  Parameters:
    uuid - Pointer to UUID structure containing the value to program

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_INVALID_PARAM, or EFUSE_STATUS_ALREADY_PROGRAMMED
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramUUID(const EFUSE_UUID_t *uuid)
{
    if(uuid == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    // Check if UUID is already locked
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(seccfg.uuidLocked)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }

    status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // Program 16 bytes (128 bits) of UUID using BYTE MODE (like HUT application)
    // UUID is at eFuse address 0x0170
    uint8_t *uuidBytes = (uint8_t *)uuid;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("\r\n[DBG] Programming UUID using BYTE MODE (HUT-compatible)\r\n");
#endif

    for(uint8_t i = 0; i < 16; i++)
    {
        uint16_t byteAddr = EFUSE_PROG_ADDR_UUID + i;  // 0x0170 + i
        RotEfuseProgram(byteAddr, uuidBytes[i]);

#if EFUSE_DEBUG_ENABLED
        if(i < 4 || i >= 12)  // Print first 4 and last 4 bytes only
        {
            char buf[64];
            snprintf(buf, sizeof(buf), "[DBG] Byte[%02d] addr=0x%04X data=0x%02X\r\n", i, byteAddr, uuidBytes[i]);
            Debug_Print(buf);
        }
        else if(i == 4)
        {
            Debug_Print("[DBG] ... (bytes 4-11 omitted) ...\r\n");
        }
#endif
    }

    // eFuse load all to update holding registers
    status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // NOTE: UUID Lock is NOT set automatically
    // User must call EFUSE_LockUUID() separately after verifying the value

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] UUID programming complete (NOT locked - verify then lock separately)\r\n");
#endif

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramDebugLock(uint8_t level)

  Summary:
    Programs the Debug Lock level.

  Description:
    WARNING: This operation is IRREVERSIBLE!

    Debug Lock Levels:
    - 0: No lock (default)
    - 1: Level 1 lock
    - 2: Level 2 lock
    - 3: Level 3 lock (most restrictive)

    Only transitions that retain every programmed bit are permitted:
    00 -> 01/10/11, 01 -> 11, and 10 -> 11. An unchanged level is a no-op.
    Success requires reloaded SECCFG to match the expected configuration.

  Parameters:
    level - Debug lock level (0-3)

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_INVALID_PARAM,
    EFUSE_STATUS_VERIFY_ERROR, or a controller error.
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramDebugLock(uint8_t level)
{
    if(level > 3)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    // Read current debug lock level
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // Numeric ordering is insufficient: 01 -> 10 would require clearing bit 6.
    // Reject every target that omits an already-programmed field bit.
    if(((uint32_t)seccfg.debugLock & ~(uint32_t)level) != 0U)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    // If already at requested level, nothing to do
    if(level == seccfg.debugLock)
    {
        return EFUSE_STATUS_SUCCESS;
    }

    uint32_t target = (uint32_t)level << ROT_SECCFG_DEBUG_LCK_Pos;
    uint32_t expected = seccfg.rawValue | target;
    // DEBUG_LCK [7:6] is in OTP byte 0. Request only the missing field bits.
    uint8_t missing = (uint8_t)(target & ~seccfg.rawValue);
    status = RotEfuseProgramChecked(0x00U, missing);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // Verify the requested level and preservation of every unrelated field.
    return seccfg.rawValue == expected ? EFUSE_STATUS_SUCCESS : EFUSE_STATUS_VERIFY_ERROR;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_LockUUID(void)

  Summary:
    Sets both UUID lock bits without programming UUID data.

  Description:
    WARNING: This operation is IRREVERSIBLE!
    Programs UUID_LCK [5:4] to 0b11, completing any missing lock bit.
    Locks the UUID field even if it hasn't been programmed.
    Reloads OTP and verifies both bits and all unrelated SECCFG fields.

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_ALREADY_PROGRAMMED, or an operation error.
*******************************************************************************/
EFUSE_STATUS EFUSE_LockUUID(void)
{
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    if((seccfg.rawValue & ROT_SECCFG_UUID_LCK_Msk) == ROT_SECCFG_UUID_LCK_Msk)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }

    uint32_t expected = seccfg.rawValue | ROT_SECCFG_UUID_LCK_Msk;
    // UUID_LCK [5:4] is in SECCFG byte 0. Request only missing OTP bits.
    uint8_t missing = (uint8_t)(ROT_SECCFG_UUID_LCK_Msk & ~seccfg.rawValue);
    status = RotEfuseProgramChecked(EFUSE_PROG_ADDR_SECCFG, missing);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    return seccfg.rawValue == expected ? EFUSE_STATUS_SUCCESS : EFUSE_STATUS_VERIFY_ERROR;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_LockPatch(void)

  Summary:
    Permanently locks Patch Code and Patch_CFGn programming.

  Description:
    Programs SECCFG.PATCH_LCK [15:14] to 0b11 to match the BZ3 patch.
    Uses the existing byte-mode eFuse programming sequence at SECCFG byte 1.
    WARNING: This operation is IRREVERSIBLE!

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_ALREADY_PROGRAMMED, or EFUSE_STATUS_ERROR.
*******************************************************************************/
EFUSE_STATUS EFUSE_LockPatch(void)
{
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if((seccfg.rawValue & EFUSE_SECCFG_PATCH_LOCK_Msk) == EFUSE_SECCFG_PATCH_LOCK_Msk)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }
    uint32_t expected = seccfg.rawValue | EFUSE_SECCFG_PATCH_LOCK_Msk;
    uint8_t missing = (uint8_t)((EFUSE_SECCFG_PATCH_LOCK_Msk & ~seccfg.rawValue) >> 8);
    status = RotEfuseProgramChecked(EFUSE_PROG_ADDR_SECCFG + 1U, missing);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    // Both lock bits must be set and every unrelated SECCFG bit preserved.
    return seccfg.rawValue == expected ? EFUSE_STATUS_SUCCESS : EFUSE_STATUS_VERIFY_ERROR;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ReadSecureBootKey(EFUSE_SecureBootKey_t *bootKey)

  Summary:
    Reads the 384-bit Secure Boot Key from holding registers.

  Description:
    The Secure Boot Key is a public key used for firmware authentication.
    It is stored across multiple holding registers:
    - ROT_LIFECYCLECTR_BOOTKEY (offset 0x24): bits 31:16 contain key bytes 1-2
    - ROT_SECUREBOOTKEY_3_46[0-10] (offset 0x28-0x50): key bytes 3-46
    - ROT_SECUREBOOTKEY_47_48_STORAGE_KEY (offset 0x54): bits 15:0 contain key bytes 47-48

  Parameters:
    bootKey - Pointer to structure to store the 384-bit (48 byte) key

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_INVALID_PARAM, or a controller/reload error.
*******************************************************************************/
EFUSE_STATUS EFUSE_ReadSecureBootKey(EFUSE_SecureBootKey_t *bootKey)
{
    if(bootKey == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    EFUSE_STATUS status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // 384-bit Secure Boot Key is stored as 48 bytes across registers:
    //   ROT_LIFECYCLECTR_BOOTKEY[31:16] = key bytes 0-1 (16 bits)
    //   ROT_SECUREBOOTKEY_3_46[0..10]   = key bytes 2-45 (11 x 32 bits = 44 bytes)
    //   ROT_SECUREBOOTKEY_47_48_STORAGE_KEY[15:0] = key bytes 46-47 (16 bits)
    // Total: 2 + 44 + 2 = 48 bytes = 384 bits
    //
    // Reconstruct 12 little-endian words in physical OTP byte order.

    uint8_t keyBytes[48];
    uint32_t reg_tmp;

    // Bytes 0-1: from ROT_LIFECYCLECTR_BOOTKEY bits [31:16]
    reg_tmp = ROT_REGS->ROT_LIFECYCLECTR_BOOTKEY;
    keyBytes[0] = (reg_tmp >> 16) & 0xFF;
    keyBytes[1] = (reg_tmp >> 24) & 0xFF;

    // Bytes 2-45: from ROT_SECUREBOOTKEY_3_46[0..10] (44 bytes)
    for(int i = 0; i < 11; i++)
    {
        reg_tmp = ROT_REGS->ROT_SECUREBOOTKEY_3_46[i];
        keyBytes[2 + i*4 + 0] = (reg_tmp) & 0xFF;
        keyBytes[2 + i*4 + 1] = (reg_tmp >> 8) & 0xFF;
        keyBytes[2 + i*4 + 2] = (reg_tmp >> 16) & 0xFF;
        keyBytes[2 + i*4 + 3] = (reg_tmp >> 24) & 0xFF;
    }

    // Bytes 46-47: from ROT_SECUREBOOTKEY_47_48_STORAGE_KEY bits [15:0]
    reg_tmp = ROT_REGS->ROT_SECUREBOOTKEY_47_48_STORAGE_KEY;
    keyBytes[46] = (reg_tmp) & 0xFF;
    keyBytes[47] = (reg_tmp >> 8) & 0xFF;

    // Pack 48 bytes into 12 x 32-bit words (little-endian)
    for(int i = 0; i < 12; i++)
    {
        bootKey->word[i] = ((uint32_t)keyBytes[i*4 + 3] << 24) |
                           ((uint32_t)keyBytes[i*4 + 2] << 16) |
                           ((uint32_t)keyBytes[i*4 + 1] << 8) |
                           ((uint32_t)keyBytes[i*4 + 0]);
    }

    return EFUSE_STATUS_SUCCESS;
}

// Compare stored X-coordinate bytes in OTP order with the compiled-in key.
// Word extraction is explicit so verification does not depend on host endianness.
// Reject the customer placeholder and missing coordinates. This is a
// configuration check, not an elliptic-curve point validation; validate the
// customer public point with the signing tools before building the utility.
bool EFUSE_IsConfiguredSecureBootKeyReady(void)
{
    uint8_t xPresent = 0U;
    uint8_t yPresent = 0U;
    for(size_t i = 0U; i < EFUSE_SBK_COORD_BYTES; i++)
    {
        xPresent |= publicBootKey[i];
        yPresent |= publicBootKey[EFUSE_SBK_COORD_BYTES + i];
    }
    return xPresent != 0U && yPresent != 0U;
}

static bool RotEfuseHardcodedKeyMatches(const EFUSE_SecureBootKey_t *readback)
{
    for(uint8_t i = 0U; i < 48U; i++)
    {
        uint8_t actual = (uint8_t)(readback->word[i / 4U] >> ((i % 4U) * 8U));
        uint8_t expected = i < EFUSE_SBK_SLOT_OFFSET ? 0U :
                           publicBootKey[EFUSE_SBK_COORD_BYTES - 1U -
                                         (i - EFUSE_SBK_SLOT_OFFSET)];
        if(actual != expected)
        {
            return false;
        }
    }
    return true;
}

bool EFUSE_SecureBootKeyMatchesConfigured(const EFUSE_SecureBootKey_t *readback, const EFUSE_SECCFG_t *seccfg)
{
    return readback != NULL && seccfg != NULL &&
           EFUSE_IsConfiguredSecureBootKeyReady() &&
           EFUSE_IsSecureBootKeyValid(readback) &&
           RotEfuseHardcodedKeyMatches(readback) &&
           seccfg->addBootKey == ((publicBootKey[EFUSE_SBK_PARITY_INDEX] & 0x01U) != 0U);
}

EFUSE_STATUS EFUSE_VerifyHardcodedKey(void)
{
    if(!EFUSE_IsConfiguredSecureBootKeyReady())
    {
        return EFUSE_STATUS_KEY_NOT_CONFIGURED;
    }
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    EFUSE_SecureBootKey_t readback;
    status = EFUSE_ReadSecureBootKey(&readback);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(!EFUSE_IsSecureBootKeyValid(&readback))
    {
        return EFUSE_STATUS_NO_KEY_DATA;
    }
    return EFUSE_SecureBootKeyMatchesConfigured(&readback, &seccfg) ?
           EFUSE_STATUS_SUCCESS : EFUSE_STATUS_VERIFY_ERROR;
}

// Input bytes are already in physical OTP order. Verify compatibility before
// any write, stop on the first controller error, and compare all 48 bytes after
// a completed physical reload. Programming never automatically locks the key.
static EFUSE_STATUS RotEfuseProgramSecureBootKeyChecked(const uint8_t *storedKey)
{
    EFUSE_SecureBootKey_t before;
    EFUSE_STATUS status = EFUSE_ReadSecureBootKey(&before);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    for(uint8_t i = 0U; i < 48U; i++)
    {
        uint8_t actual = (uint8_t)(before.word[i / 4U] >> ((i % 4U) * 8U));
        if((actual & (uint8_t)~storedKey[i]) != 0U)
        {
            // Another key has already set a bit that the requested key needs clear.
            return EFUSE_STATUS_VERIFY_ERROR;
        }
    }
    for(uint8_t i = 0U; i < 48U; i++)
    {
        uint8_t actual = (uint8_t)(before.word[i / 4U] >> ((i % 4U) * 8U));
        uint8_t missing = storedKey[i] & (uint8_t)~actual;
        if(missing == 0U)
        {
            continue;
        }
        status = RotEfuseProgramChecked(EFUSE_PROG_ADDR_SECURE_BOOT_KEY + i, missing);
        if(status != EFUSE_STATUS_SUCCESS)
        {
            return status;
        }
    }
    EFUSE_SecureBootKey_t after;
    status = EFUSE_ReadSecureBootKey(&after);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    for(uint8_t i = 0U; i < 48U; i++)
    {
        uint8_t actual = (uint8_t)(after.word[i / 4U] >> ((i % 4U) * 8U));
        if(actual != storedKey[i])
        {
            return EFUSE_STATUS_VERIFY_ERROR;
        }
    }
    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    bool EFUSE_IsSecureBootKeyValid(const EFUSE_SecureBootKey_t *bootKey)

  Summary:
    Checks if a Secure Boot Key is valid (not all zeros).

  Description:
    Per datasheet: "If the secure boot key has all values set to '0',
    it is considered invalid and no secure boot key is programmed."

  Parameters:
    bootKey - Pointer to boot key structure to check

  Returns:
    true if key is valid (at least one non-zero word), false otherwise
*******************************************************************************/
bool EFUSE_IsSecureBootKeyValid(const EFUSE_SecureBootKey_t *bootKey)
{
    if(bootKey == NULL)
    {
        return false;
    }

    // Check if any word is non-zero
    for(int i = 0; i < 12; i++)
    {
        if(bootKey->word[i] != 0)
        {
            return true;
        }
    }

    return false;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ReadStorageRootKey(EFUSE_StorageRootKey_t *rootKey)

  Summary:
    Reads the 256-bit Storage Root Key from holding registers.

  Description:
    The Storage Root Key is spread across multiple ROT holding registers:
    - ROT_SECUREBOOTKEY_47_48_STORAGE_KEY[31:16] (offset 0x54): bytes 0-1
    - ROT_STORAGE_KEY_3_30[0-6] (offset 0x58-0x70): bytes 2-29
    - ROT_STORAGE_KEY_FFCFG[15:0] (offset 0x74): bytes 30-31

  Parameters:
    rootKey - Pointer to structure to store the 256-bit (32 byte) key

  Returns:
    EFUSE_STATUS_SUCCESS or EFUSE_STATUS_INVALID_PARAM
*******************************************************************************/
EFUSE_STATUS EFUSE_ReadStorageRootKey(EFUSE_StorageRootKey_t *rootKey)
{
    if(rootKey == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    EFUSE_STATUS status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    // 256-bit Storage Root Key is stored as 32 bytes across registers:
    //   ROT_SECUREBOOTKEY_47_48_STORAGE_KEY[31:16] = bytes 0-1 (16 bits)
    //   ROT_STORAGE_KEY_3_30[0..6]                 = bytes 2-29 (7 x 32 bits = 28 bytes)
    //   ROT_STORAGE_KEY_FFCFG[15:0]                = bytes 30-31 (16 bits)
    // Total: 2 + 28 + 2 = 32 bytes = 256 bits

    uint8_t keyBytes[32];
    uint32_t reg_tmp;

    // Bytes 0-1: from ROT_SECUREBOOTKEY_47_48_STORAGE_KEY bits [31:16]
    reg_tmp = ROT_REGS->ROT_SECUREBOOTKEY_47_48_STORAGE_KEY;
    keyBytes[0] = (reg_tmp >> 16) & 0xFF;
    keyBytes[1] = (reg_tmp >> 24) & 0xFF;

    // Bytes 2-29: from ROT_STORAGE_KEY_3_30[0..6] (28 bytes)
    for(int i = 0; i < 7; i++)
    {
        reg_tmp = ROT_REGS->ROT_STORAGE_KEY_3_30[i];
        keyBytes[2 + i*4 + 0] = (reg_tmp) & 0xFF;
        keyBytes[2 + i*4 + 1] = (reg_tmp >> 8) & 0xFF;
        keyBytes[2 + i*4 + 2] = (reg_tmp >> 16) & 0xFF;
        keyBytes[2 + i*4 + 3] = (reg_tmp >> 24) & 0xFF;
    }

    // Bytes 30-31: from ROT_STORAGE_KEY_FFCFG bits [15:0]
    reg_tmp = ROT_REGS->ROT_STORAGE_KEY_FFCFG;
    keyBytes[30] = (reg_tmp) & 0xFF;
    keyBytes[31] = (reg_tmp >> 8) & 0xFF;

    // Pack 32 bytes into 8 x 32-bit words (little-endian)
    for(int i = 0; i < 8; i++)
    {
        rootKey->word[i] = ((uint32_t)keyBytes[i*4 + 3] << 24) |
                           ((uint32_t)keyBytes[i*4 + 2] << 16) |
                           ((uint32_t)keyBytes[i*4 + 1] << 8) |
                           ((uint32_t)keyBytes[i*4 + 0]);
    }

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    bool EFUSE_IsStorageRootKeyValid(const EFUSE_StorageRootKey_t *rootKey)

  Summary:
    Checks if a Storage Root Key is valid (not all zeros).

  Description:
    If all values are '0', the key is considered not programmed.

  Parameters:
    rootKey - Pointer to root key structure to check

  Returns:
    true if key is valid (at least one non-zero word), false otherwise
*******************************************************************************/
bool EFUSE_IsStorageRootKeyValid(const EFUSE_StorageRootKey_t *rootKey)
{
    if(rootKey == NULL)
    {
        return false;
    }

    // Check if any word is non-zero
    for(int i = 0; i < 8; i++)
    {
        if(rootKey->word[i] != 0)
        {
            return true;
        }
    }

    return false;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramSecureBootKey(const EFUSE_SecureBootKey_t *bootKey)

  Summary:
    Programs X for the configured P-256 or P-384 curve.

  Description:
    WARNING: This operation is IRREVERSIBLE!
    Programs the public key used for secure boot firmware authentication.

  Parameters:
    bootKey - Big-endian X in the first 32 or 48 bytes; other input bytes are unused

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_INVALID_PARAM, or EFUSE_STATUS_ALREADY_PROGRAMMED
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramSecureBootKey(const EFUSE_SecureBootKey_t *bootKey)
{
    if(bootKey == NULL)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }
    uint8_t xPresent = 0U;
    const uint8_t *input = (const uint8_t *)bootKey;
    for(size_t i = 0U; i < EFUSE_SBK_COORD_BYTES; i++)
    {
        xPresent |= input[i];
    }
    if(xPresent == 0U)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(seccfg.bootKeyLocked)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }
    // Input begins with a big-endian X of the configured coordinate length.
    // HUT programs P-256 at 0x0036, after 16 zero bytes in the OTP slot.
    uint8_t storedKey[48] = {0};
    memcpy(storedKey + EFUSE_SBK_SLOT_OFFSET, bootKey, EFUSE_SBK_COORD_BYTES);
    EFUSE_ReverseKeyBytes(storedKey + EFUSE_SBK_SLOT_OFFSET, EFUSE_SBK_COORD_BYTES);
    return RotEfuseProgramSecureBootKeyChecked(storedKey);
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramHardcodedKey(void)

  Summary:
    Programs the configured P-256 or P-384 public key (like write_key).

  Description:
    WARNING: This operation is IRREVERSIBLE!
    Programs the hardcoded publicBootKey[] array to EFUSE.
    Checks ADD_BOOT_KEY parity compatibility before writing key bytes and
    verifies all 48 stored X bytes and that bit after programming.
    This matches approach exactly:
    1. Copy hardcoded key to local buffer
    2. Reverse byte order
    3. Program at offset 0x0026 using byte mode

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_ALREADY_PROGRAMMED if locked,
    EFUSE_STATUS_VERIFY_ERROR for a key/parity mismatch, or a controller error.
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramHardcodedKey(void)
{
    if(!EFUSE_IsConfiguredSecureBootKeyReady())
    {
        return EFUSE_STATUS_KEY_NOT_CONFIGURED;
    }
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(seccfg.bootKeyLocked)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }
    // Reject incompatible parity before writing any X-coordinate byte.
    uint8_t parity = publicBootKey[EFUSE_SBK_PARITY_INDEX] & 0x01U;
    if(parity == 0U && seccfg.addBootKey)
    {
        return EFUSE_STATUS_VERIFY_ERROR;
    }
    uint8_t storedKey[48] = {0};
    memcpy(storedKey + EFUSE_SBK_SLOT_OFFSET, publicBootKey, EFUSE_SBK_COORD_BYTES);
    EFUSE_ReverseKeyBytes(storedKey + EFUSE_SBK_SLOT_OFFSET, EFUSE_SBK_COORD_BYTES);
    status = RotEfuseProgramSecureBootKeyChecked(storedKey);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    status = EFUSE_ProgramAddBootKey(parity);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    return EFUSE_VerifyHardcodedKey();
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramAddBootKey(uint8_t parityBit)

  Summary:
    Programs the ADD_BOOT_KEY parity bit (LSB of Y coordinate).

  Description:
    WARNING: This operation is IRREVERSIBLE!

    
    - If the reloaded bit already matches: succeed without programming.
    - If parityBit = 1 and the bit is clear: program 0x01 at address 0x02,
      reload, and verify the entire expected SECCFG.
    - If parityBit = 0 and the bit is set: return VERIFY_ERROR; OTP cannot clear it.

    This is the LSB of the Y coordinate in compressed public key format.
    Must be programmed before BOOT_KEY_LOCK.

  Parameters:
    parityBit - LSB of Y coordinate (0 or 1)

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_ALREADY_PROGRAMMED,
    EFUSE_STATUS_INVALID_PARAM, EFUSE_STATUS_VERIFY_ERROR, or a controller error.
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramAddBootKey(uint8_t parityBit)
{
    if(parityBit > 1U)
    {
        return EFUSE_STATUS_INVALID_PARAM;
    }

    // Check if boot key is already locked
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(seccfg.bootKeyLocked)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }

    // Even a no-write result requires completed physical read-back above.
    if(seccfg.addBootKey == (parityBit != 0U))
    {
        return EFUSE_STATUS_SUCCESS;
    }

    if(parityBit == 0U)
    {
        return EFUSE_STATUS_VERIFY_ERROR;
    }

    uint32_t expected = seccfg.rawValue | ROT_SECCFG_ADD_BOOT_KEY_Msk;
    // SECCFG bit 16 is OTP byte 2, bit 0. Request only that missing bit.
    status = RotEfuseProgramChecked(0x02U, 0x01U);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

    return seccfg.rawValue == expected ? EFUSE_STATUS_SUCCESS : EFUSE_STATUS_VERIFY_ERROR;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_LockSecureBootKey(void)

  Summary:
    Sets the BOOT_KEY_LOCK bits to lock the Secure Boot Key.

  Description:
    WARNING: This operation is IRREVERSIBLE!
    After locking, the Secure Boot Key cannot be modified.
    Requires the stored key and parity to match the compiled-in key first.
    Requests only missing BOOT_KEY_LCK bits at address 0x01 to reach 0x03,
    then reloads and verifies SECCFG without changing other fields.

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_ALREADY_PROGRAMMED,
    EFUSE_STATUS_VERIFY_ERROR, or a controller error.
*******************************************************************************/
EFUSE_STATUS EFUSE_LockSecureBootKey(void)
{
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    // An already complete lock needs no write; it does not establish key identity.
    if((seccfg.rawValue & ROT_SECCFG_BOOT_KEY_LCK_Msk) == ROT_SECCFG_BOOT_KEY_LCK_Msk)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }
    status = EFUSE_VerifyHardcodedKey();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    uint32_t expected = seccfg.rawValue | ROT_SECCFG_BOOT_KEY_LCK_Msk;
    uint8_t missing = (uint8_t)((ROT_SECCFG_BOOT_KEY_LCK_Msk & ~seccfg.rawValue) >> 8);
    status = RotEfuseProgramChecked(0x01U, missing);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    return seccfg.rawValue == expected ? EFUSE_STATUS_SUCCESS : EFUSE_STATUS_VERIFY_ERROR;
}




/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_GenerateStorageRootKey(void)

  Summary:
    Generates a random 256-bit Storage Root Key using TRNG (like HUT).

  Description:
    Uses the hardware TRNG to generate a cryptographically secure random key.
    Call this function once at boot/init time. The generated key is stored
    internally and can be programmed later with EFUSE_ProgramGeneratedSRK().

    This matches HUT's HCI_ManuGenerateRootKey() function.

  Returns:
    EFUSE_STATUS_SUCCESS if TRNG succeeded, EFUSE_STATUS_ERROR if TRNG failed
*******************************************************************************/
EFUSE_STATUS EFUSE_GenerateStorageRootKey(void)
{
    struct sx_trng trng;
    int ret;

    s_storageRootKeyValid = 0;

#if EFUSE_DEBUG_ENABLED
    Debug_Print("\r\n[DBG] Generating Storage Root Key via TRNG...\r\n");
#endif

    // Enable crypto clock
    CRYPTO_CLK_ENABLE();

    // Initialize TRNG
    ret = sx_trng_init(&trng, NULL);
    if(ret == SX_OK)
    {
        // Keep trying until we get random data (may return SX_ERR_HW_PROCESSING)
        ret = SX_ERR_HW_PROCESSING;
        while(ret == SX_ERR_HW_PROCESSING)
        {
            ret = sx_trng_get(&trng, (char *)s_storageRootKey, sizeof(s_storageRootKey));
        }
    }

    // Disable TRNG and crypto clock
    CRM_TRNG_DISABLE();
    CRYPTO_CLK_DISABLE();

    if(ret == SX_OK)
    {
        s_storageRootKeyValid = 1;
#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] TRNG success! SRK generated (32 bytes)\r\n");
#endif
        return EFUSE_STATUS_SUCCESS;
    }
    else
    {
#if EFUSE_DEBUG_ENABLED
        char buf[32];
        sprintf(buf, "[DBG] TRNG FAILED! Error: %d\r\n", ret);
        Debug_Print(buf);
#endif
        return EFUSE_STATUS_ERROR;
    }
}

/*******************************************************************************
  Function:
    bool EFUSE_IsStorageRootKeyGenerated(void)

  Summary:
    Checks if a Storage Root Key has been generated via TRNG.

  Returns:
    true if EFUSE_GenerateStorageRootKey() succeeded, false otherwise
*******************************************************************************/
bool EFUSE_IsStorageRootKeyGenerated(void)
{
    return (s_storageRootKeyValid != 0);
}



/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_ProgramGeneratedSRK(void)

  Summary:
    Programs the TRNG-generated SRK and locks it (PRODUCTION MODE - like HUT).

  Description:
    WARNING: This operation is IRREVERSIBLE!
    This function exactly matches HUT behavior:
    1. Check if TRNG-generated key is valid (call EFUSE_GenerateStorageRootKey first)
    2. Check if already locked - if yes, return ALREADY_PROGRAMMED
    3. Program 32 bytes of SRK
    4. Immediately lock (set ROOT_KEY_LCK)

    For testing where you want to verify before locking, use:
    - EFUSE_ProgramStorageRootKey() with a provided key
    - EFUSE_LockStorageRootKey() separately

  Returns:
    EFUSE_STATUS_SUCCESS, EFUSE_STATUS_ERROR (TRNG not valid), or
    EFUSE_STATUS_ALREADY_PROGRAMMED
*******************************************************************************/
EFUSE_STATUS EFUSE_ProgramGeneratedSRK(void)
{
    // Check if TRNG-generated key is valid
    if(!s_storageRootKeyValid)
    {
#if EFUSE_DEBUG_ENABLED
        Debug_Print("\r\n[DBG] ERROR: No valid TRNG-generated SRK!\r\n");
        Debug_Print("[DBG] Call EFUSE_GenerateStorageRootKey() first.\r\n");
#endif
        return EFUSE_STATUS_ERROR;
    }

    // Check if root key is already locked
    EFUSE_SECCFG_t seccfg;
    EFUSE_STATUS status = EFUSE_ReadSECCFG(&seccfg);
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }
    if(seccfg.rootKeyLocked)
    {
        return EFUSE_STATUS_ALREADY_PROGRAMMED;
    }

    status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

#if EFUSE_DEBUG_ENABLED
    Debug_Print("\r\n[DBG] Programming TRNG-generated SRK WITH AUTO-LOCK (HUT mode)\r\n");
#endif

    // Step 1: Program 32 bytes (like HUT)
    for(uint8_t i = 0; i < 32; i++)
    {
        uint16_t byteAddr = EFUSE_PROG_ADDR_STORAGE_ROOT_KEY + i;  // 0x0056 + i
        RotEfuseProgram(byteAddr, s_storageRootKey[i]);
    }

    // Step 2: Immediately lock (like HUT)
    // ROOT_KEY_LCK = 1
    uint8_t seccfg_byte1 = (ROT_REGS->ROT_SECCFG >> 8) & 0xFF;
    seccfg_byte1 |= 0x01;
    RotEfuseProgram(0x01, seccfg_byte1);

    // EFUSE Load All
    status = RotEfuseLoadChecked();
    if(status != EFUSE_STATUS_SUCCESS)
    {
        return status;
    }

#if EFUSE_DEBUG_ENABLED
    Debug_Print("[DBG] TRNG-generated SRK programmed AND LOCKED (HUT mode)\r\n");
#endif

    return EFUSE_STATUS_SUCCESS;
}

/*******************************************************************************
  Function:
    EFUSE_STATUS EFUSE_DumpRawRegisters(uint16_t startAddr, uint16_t endAddr)

  Summary:
    Reads raw eFuse register contents for debugging.

  Description:
    Triggers eFuse load and reads ROT holding registers. The caller provides
    a callback function to handle the output since this driver doesn't have
    direct access to console I/O.

    ROT register base is 0x44003000. The eFuse address maps to offsets.

  Parameters:
    startAddr - Starting eFuse byte address (e.g., 0x0000)
    endAddr   - Ending eFuse byte address (e.g., 0x007A)

  Note:
    This function triggers the eFuse load. Reading is done in app.c where
    Console functions are available.
*******************************************************************************/
EFUSE_STATUS EFUSE_DumpRawRegisters(uint16_t startAddr, uint16_t endAddr)
{
    (void)startAddr;
    (void)endAddr;
    return RotEfuseLoadChecked();
}
