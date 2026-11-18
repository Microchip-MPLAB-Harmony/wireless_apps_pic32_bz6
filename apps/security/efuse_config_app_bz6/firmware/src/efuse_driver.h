/*******************************************************************************
  eFuse Driver Header File

  Company:
    Microchip Technology Inc.

  File Name:
    efuse_driver.h

  Summary:
    eFuse driver interface for PIC32CX-BZ3/BZ6 devices.

  Description:
    Provides functions to read and program eFuse registers including:
    - Anti-Rollback Counter (255-bit thermometer counter)
    - UUID (128-bit Unique Identifier)
    - Debug Lock
    - Patch Lock
    - Device ID (16-bit)
    - Security Configuration (SECCFG)
    - Secure Boot Key (P-256 or P-384 public key)
    - Storage Root Key (256-bit)

    Supported Devices:
    - PIC32CX-BZ3 / WBZ35x (Buckland) - define DEVICE_BZ3
    - PIC32CX-BZ6 / BZ6               - define DEVICE_BZ6

    WARNING: eFuse programming is IRREVERSIBLE!
*******************************************************************************/

#ifndef EFUSE_DRIVER_H
#define EFUSE_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// ============================================================================
// Device Detection - Auto-detect from DFP if not manually defined
// ============================================================================
#if !defined(DEVICE_BZ3) && !defined(DEVICE_BZ6)
    // Try to auto-detect from DFP definitions
    #if defined(__WBZ351__) || defined(__WBZ350__) || defined(__PIC32CX1012BZ25048__) || defined(__PIC32CX1012BZ25032__) || defined(__PIC32CX1012BZ24032__)
        #define DEVICE_BZ3
    #elif defined(__WBZ451__) || defined(__WBZ450__) || defined(__PIC32CX2051BZ62132__) || defined(__PIC32CX5109BZ61132__) || defined(__32WM_BZ6204__) || defined(__PIC32WM_BZ6204__)
        #define DEVICE_BZ6
    #else
        // Default to BZ3 if cannot detect
        #warning "Device type not detected, defaulting to DEVICE_BZ3. Define DEVICE_BZ3 or DEVICE_BZ6 explicitly."
        #define DEVICE_BZ3
    #endif
#endif

// Validate only one device is selected
#if defined(DEVICE_BZ3) && defined(DEVICE_BZ6)
    #error "Both DEVICE_BZ3 and DEVICE_BZ6 defined. Please define only one."
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Debug Lock Levels
#define DEBUG_LOCK_NONE                  0x00
#define DEBUG_LOCK_LEVEL1                0x01
#define DEBUG_LOCK_LEVEL2                0x02
#define DEBUG_LOCK_LEVEL3                0x03

// Patch Lock in SECCFG bits [15:14], OTP byte 1 bits [7:6].
// Matches ROT_SECCFG_PATCH_LCK in the BZ6 DFP and the BZ3 patch.
#define EFUSE_SECCFG_PATCH_LOCK_Pos       14U
#define EFUSE_SECCFG_PATCH_LOCK_Msk       (0x03UL << EFUSE_SECCFG_PATCH_LOCK_Pos)

// eFuse Operation Status
typedef enum {
    EFUSE_STATUS_SUCCESS = 0,
    EFUSE_STATUS_ERROR,
    EFUSE_STATUS_ALREADY_PROGRAMMED,
    EFUSE_STATUS_INVALID_PARAM,
    EFUSE_STATUS_PROGRAM_TIMEOUT,
    EFUSE_STATUS_RELOAD_TIMEOUT,
    EFUSE_STATUS_VERIFY_ERROR,
    EFUSE_STATUS_CONTROLLER_BUSY,
    EFUSE_STATUS_TIMEBASE_ERROR,
    EFUSE_STATUS_KEY_NOT_CONFIGURED,
    EFUSE_STATUS_NO_KEY_DATA
} EFUSE_STATUS;

// UUID Structure (128-bit)
typedef struct {
    uint32_t word0;
    uint32_t word1;
    uint32_t word2;
    uint32_t word3;
} EFUSE_UUID_t;

// Physical Secure Boot Key slot (48 bytes). P-256 uses the final 32 bytes,
// preceded by 16 zero bytes; P-384 uses all 48 bytes.
typedef struct {
    uint32_t word[12];
} EFUSE_SecureBootKey_t;

// Storage Root Key Structure (256-bit = 32 bytes = 8 x 32-bit words)
typedef struct {
    uint32_t word[8];
} EFUSE_StorageRootKey_t;

// Security Configuration Structure
typedef struct {
    uint8_t debugLock;          // DEBUG_LOCK bits [7:6] - 0-3
    bool uuidLocked;            // UUID_LOCK bits [5:4] - non-zero = locked
    bool rootKeyLocked;         // ROOT_KEY_LOCK bits [9:8] - non-zero = locked
    bool bootKeyLocked;         // BOOT_KEY_LOCK bits [11:10] - non-zero = locked
    bool patchLocked;           // PATCH_LCK bits [15:14] - non-zero = locked
    bool addBootKey;            // ADD_BOOT_KEY bit [16] - raw read-back; used as Y parity by this programmer
    uint32_t rawValue;          // Full 32-bit SECCFG register value
} EFUSE_SECCFG_t;

// ============================================================================
// Function Prototypes - Read Operations
// ============================================================================
// Read APIs refresh the holding registers and leave output unchanged on failure.
EFUSE_STATUS EFUSE_ReadAntiRollback(uint8_t *value);
EFUSE_STATUS EFUSE_ReadUUID(EFUSE_UUID_t *uuid);
// SUCCESS only after a completed physical OTP reload; output is unchanged on failure.
EFUSE_STATUS EFUSE_ReadSECCFG(EFUSE_SECCFG_t *seccfg);
// Latched timeout/clock fault or current controller busy state. No register writes.
EFUSE_STATUS EFUSE_GetControllerStatus(void);
// Completed physical reload; output is unchanged on a reload/controller error.
EFUSE_STATUS EFUSE_ReadSecureBootKey(EFUSE_SecureBootKey_t *bootKey);
// Compare the full slot (including P-256 zero prefix) and parity with the compiled key.
// Placeholder/missing-coordinate check only; validate the public point offline.
bool EFUSE_IsConfiguredSecureBootKeyReady(void);
// Rejects unconfigured keys and blank OTP; verifies X, zero prefix and parity.
EFUSE_STATUS EFUSE_VerifyHardcodedKey(void);
bool EFUSE_SecureBootKeyMatchesConfigured(const EFUSE_SecureBootKey_t *readback, const EFUSE_SECCFG_t *seccfg);
// Curve selected by the compiled public-key length, not inferred from OTP data.
const char *EFUSE_ConfiguredSecureBootCurve(void);
EFUSE_STATUS EFUSE_ReadStorageRootKey(EFUSE_StorageRootKey_t *rootKey);
bool EFUSE_IsSecureBootKeyValid(const EFUSE_SecureBootKey_t *bootKey);
bool EFUSE_IsStorageRootKeyValid(const EFUSE_StorageRootKey_t *rootKey);

// ============================================================================
// Function Prototypes - Program Operations (IRREVERSIBLE!)
// ============================================================================
EFUSE_STATUS EFUSE_ProgramAntiRollback(uint8_t value);
EFUSE_STATUS EFUSE_ProgramUUID(const EFUSE_UUID_t *uuid);
// Rejects transitions requiring a bit to clear; programs missing DEBUG_LCK bits
// and verifies reloaded SECCFG. An unchanged level succeeds without programming.
EFUSE_STATUS EFUSE_ProgramDebugLock(uint8_t level);
// Programs UUID_LCK [5:4] to 0b11 and verifies reloaded SECCFG.
EFUSE_STATUS EFUSE_LockUUID(void);
EFUSE_STATUS EFUSE_LockPatch(void);  // Permanently locks Patch Code and Patch_CFGn
// Input starts with big-endian X: 32 or 48 bytes per the compiled configuration.
// Programs reversed X at 0x0036 for P-256 (0x0026 for P-384) and verifies
// the full 48-byte slot.
EFUSE_STATUS EFUSE_ProgramSecureBootKey(const EFUSE_SecureBootKey_t *bootKey);
// Programs/verifies the compiled-in key and parity, without automatically locking.
EFUSE_STATUS EFUSE_ProgramHardcodedKey(void);
// Accepts 0/1, programs only a missing 1, and verifies reloaded SECCFG.
// A programmed 1 cannot be cleared to match parity 0; returns VERIFY_ERROR.
EFUSE_STATUS EFUSE_ProgramAddBootKey(uint8_t parityBit);
// Requires a match with the compiled-in key/parity before any lock write.
// Targets BOOT_KEY_LCK=0b11 and verifies the entire reloaded SECCFG.
EFUSE_STATUS EFUSE_LockSecureBootKey(void);
EFUSE_STATUS EFUSE_GenerateStorageRootKey(void);                                  // Generate SRK via TRNG
EFUSE_STATUS EFUSE_ProgramGeneratedSRK(void);                                     // Program TRNG-generated SRK + auto-lock

// ============================================================================
// Function Prototypes - Utility Functions
// ============================================================================
void EFUSE_ReverseKeyBytes(uint8_t *data, uint8_t size);

// ============================================================================
// Function Prototypes - Debug/Diagnostic Operations
// ============================================================================
// Refresh only; callers must check status before using the holding registers.
EFUSE_STATUS EFUSE_DumpRawRegisters(uint16_t startAddr, uint16_t endAddr);

#ifdef __cplusplus
}
#endif

#endif // EFUSE_DRIVER_H
