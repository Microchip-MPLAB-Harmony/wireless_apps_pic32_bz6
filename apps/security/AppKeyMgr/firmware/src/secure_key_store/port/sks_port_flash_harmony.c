/*******************************************************************************
  Secure Key Store - Flash Backend for MCC/Harmony (NVM PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    sks_port_flash_harmony.c

  Summary:
    Implements the flash interface of sks_port.h on top of the
    MCC-generated NVM PLIB.

  Description:
    Blocking model: every erase/program polls NVM_IsBusy() until the NVM
    controller signals completion, then checks NVM_ErrorGet() before
    returning.  Region offsets are translated to absolute flash addresses
    with SKS_REGION_BASE_ADDR (sks_user_config.h).

    Guards:
      - Before any erase/program, the configured region
        (SKS_REGION_BASE_ADDR, SKS_REGION_SIZE) is compared with the region
        reserved by the linker script (__secure_key_storage_start /
        __secure_key_storage_end, .secureKeyStorage section).  On a mismatch
        nothing is erased or programmed, so a stale configuration can never
        wipe application code.
      - Every request is bounds-checked against the key region and checked
        for the alignment the NVM controller requires.

    Precondition: NVM_Initialize() has been performed by SYS_Initialize()
    (MCC NVM PLIB, initialization.c) before any SKS function is used.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

#define SKS_MODULE_SOURCE

#include <stddef.h>
#include <stdint.h>
#include "peripheral/nvm/plib_nvm.h"
#include "../sks.h"
#include "sks_port.h"

SKS_STATIC_ASSERT(SKS_FLASH_PAGE_SIZE == NVM_FLASH_PAGESIZE,
                  "SKS_FLASH_PAGE_SIZE must equal NVM_FLASH_PAGESIZE");
SKS_STATIC_ASSERT(SKS_FLASH_PROGRAM_UNIT == NVM_FLASH_ROWSIZE,
                  "SKS_FLASH_PROGRAM_UNIT must equal NVM_FLASH_ROWSIZE");
SKS_STATIC_ASSERT((SKS_REGION_BASE_ADDR % SKS_FLASH_PAGE_SIZE) == 0U,
                  "SKS_REGION_BASE_ADDR must be NVM page aligned");
SKS_STATIC_ASSERT((SKS_REGION_BASE_ADDR >= NVM_FLASH_START_ADDRESS) &&
                  ((SKS_REGION_BASE_ADDR + SKS_REGION_SIZE) <=
                   (NVM_FLASH_START_ADDRESS + NVM_FLASH_SIZE)),
                  "The key region must lie inside Program Flash");

// *****************************************************************************
// Section: Linker-Reserved Key Region
// *****************************************************************************

/* Boundaries of the .secureKeyStorage section (device linker script). */
extern const uint8_t __secure_key_storage_start[];
extern const uint8_t __secure_key_storage_end[];

// *****************************************************************************
// Section: Local Helpers
// *****************************************************************************

/* True when SKS_REGION_BASE_ADDR / SKS_REGION_SIZE describe exactly the
 * region the linker script reserved. */
static bool sks_flash_regionMatchesLinker(void)
{
    uint32_t ldStart = (uint32_t)(uintptr_t)__secure_key_storage_start;
    uint32_t ldEnd   = (uint32_t)(uintptr_t)__secure_key_storage_end;

    return (ldStart == SKS_REGION_BASE_ADDR) &&
           ((ldEnd - ldStart) == (uint32_t)SKS_REGION_SIZE);
}

/* True when [offset, offset + len) lies inside the key region. */
static bool sks_flash_rangeOk(uint32_t offset, uint32_t len)
{
    return (len <= (uint32_t)SKS_REGION_SIZE) &&
           (offset <= ((uint32_t)SKS_REGION_SIZE - len));
}

/* Wait until the NVM controller has finished the current operation. */
static void sks_flash_waitIdle(void)
{
    while (NVM_IsBusy() == true)
    {
        /* Spin - blocking poll model */
    }
}

/* Wait until the NVM controller finishes the current operation.
 * Returns true when idle without error. */
static bool sks_flash_waitReady(void)
{
    sks_flash_waitIdle();

    return (NVM_ErrorGet() == NVM_ERROR_NONE);
}

// *****************************************************************************
// Section: Porting Interface Implementation
// *****************************************************************************

/*******************************************************************************
  Function:
    bool sks_port_flash_read(uint32_t offset, void *buf, uint32_t len)
*******************************************************************************/
bool sks_port_flash_read(uint32_t offset, void *buf, uint32_t len)
{
    if ((buf == NULL) || (sks_flash_rangeOk(offset, len) == false))
    {
        return false;
    }

    /* NVM_Read is a synchronous memcpy - no busy-wait required */
    return NVM_Read((uint32_t *)buf, len, SKS_REGION_BASE_ADDR + offset);
}

/*******************************************************************************
  Function:
    bool sks_port_flash_erase_page(uint32_t offset)
*******************************************************************************/
bool sks_port_flash_erase_page(uint32_t offset)
{
    uint32_t address = SKS_REGION_BASE_ADDR + offset;

    if (((offset % SKS_FLASH_PAGE_SIZE) != 0U) ||
        (offset >= (uint32_t)SKS_REGION_SIZE) ||
        (sks_flash_regionMatchesLinker() == false))
    {
        return false;
    }

    /* Wait for any prior operation to complete before starting a new one */
    sks_flash_waitIdle();

    if (NVM_PageErase(address) == false)
    {
        return false;
    }

    /* Block until the erase is done, then check for errors */
    return sks_flash_waitReady();
}

/*******************************************************************************
  Function:
    bool sks_port_flash_program(uint32_t offset, const void *data, uint32_t len)
*******************************************************************************/
bool sks_port_flash_program(uint32_t offset, const void *data, uint32_t len)
{
    const uint8_t *writePtr       = (const uint8_t *)data;
    uint32_t       rowAddr        = SKS_REGION_BASE_ADDR + offset;
    uint32_t       bytesRemaining = len;

    if ((data == NULL) ||
        ((((uintptr_t)data) & 3U) != 0U) ||
        ((offset % SKS_FLASH_PROGRAM_UNIT) != 0U) ||
        (sks_flash_rangeOk(offset, len) == false) ||
        (sks_flash_regionMatchesLinker() == false))
    {
        return false;
    }

    while (bytesRemaining >= SKS_FLASH_PROGRAM_UNIT)
    {
        /* Wait for any prior operation to complete before starting a new one */
        sks_flash_waitIdle();

        /* MISRA-safe cast: data is word-aligned (checked above); the NVM
         * controller only reads the source buffer (RAM). */
        if (NVM_RowWrite((uint32_t *)(uintptr_t)writePtr, rowAddr) == false)
        {
            return false;
        }

        /* Block until the write is done, then check for errors */
        if (sks_flash_waitReady() == false)
        {
            return false;
        }

        writePtr       += SKS_FLASH_PROGRAM_UNIT;
        rowAddr        += SKS_FLASH_PROGRAM_UNIT;
        bytesRemaining -= SKS_FLASH_PROGRAM_UNIT;
    }

    return true;
}

/*******************************************************************************
 End of File
*******************************************************************************/
