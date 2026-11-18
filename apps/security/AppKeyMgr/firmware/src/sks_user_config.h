/*******************************************************************************
  Secure Key Store - Project Configuration (MCC/Harmony, PIC32CX2051BZ62132)

  File Name:
    sks_user_config.h

  Summary:
    Platform values consumed by secure_key_store (see sks_config.h).

  Description:
    BZ6 key region = last 4 KB page of Program Flash (PFM page 511,
    0x011FF000 - 0x011FFFFF).  On BZ6 PDS lives in Boot Flash
    (0x00807000), so - unlike BZ3, where the key page sits directly below
    PDS at 0x0107B000 - the key page is placed at the very end of Program
    Flash.

    SKS_REGION_BASE_ADDR / SKS_REGION_SIZE must match the 'securekey'
    MEMORY region (.secureKeyStorage section) in PIC32CX2051BZ62132.ld.
    The Harmony flash backend checks them against the linker symbols
    __secure_key_storage_start / __secure_key_storage_end at run time and
    refuses to erase or program flash on a mismatch.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef SKS_USER_CONFIG_H
#define SKS_USER_CONFIG_H

/** Base address of the key region (last NVM page of Program Flash).
 *  Must match SECURE_KEY_STORAGE_ORIGIN in PIC32CX2051BZ62132.ld. */
#define SKS_REGION_BASE_ADDR        0x011FF000UL

/** Size of the key region - one NVM page (4096 B).
 *  Must match SECURE_KEY_STORAGE_LENGTH in PIC32CX2051BZ62132.ld. */
#define SKS_REGION_SIZE             0x1000U

/** Flash erase unit (NVM page, NVM_FLASH_PAGESIZE). */
#define SKS_FLASH_PAGE_SIZE         4096U

/** Flash program unit (NVM row, NVM_FLASH_ROWSIZE). */
#define SKS_FLASH_PROGRAM_UNIT      1024U

/** Hardware key index of the Storage Root Key (SRK). */
#define SKS_SRK_KEY_ID              0U

#endif /* SKS_USER_CONFIG_H */

/*******************************************************************************
 End of File
*******************************************************************************/
