/*******************************************************************************
  Secure Key Store - Configuration

  File Name:
    sks_config.h

  Summary:
    Build-time configuration of secure_key_store.  Included by sks.h.

  Description:
    Platform values (key region and flash geometry) come from
    "sks_user_config.h", which the project supplies on its include path
    (MCC/Harmony: firmware/src/sks_user_config.h).

    Required in sks_user_config.h:
      SKS_REGION_BASE_ADDR    Absolute start address of the key region.
      SKS_REGION_SIZE         Size of the key region in bytes (one flash page).
      SKS_FLASH_PAGE_SIZE     Flash erase unit in bytes.
      SKS_FLASH_PROGRAM_UNIT  Flash program unit in bytes.
    Optional:
      SKS_SRK_KEY_ID          Hardware key index of the SRK (default 0).
      SKS_HW_MAX_RETRIES      Crypto engine busy retries (default 100).
      SKS_IV_GEN_MAX_RETRIES  TRNG not-ready retries (default 100).

    Slot layout: 32-byte header, 64-byte max key, 124-byte slot stride,
    33 slots per 4 KB region (see sks_layout.h).

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

#ifndef SKS_CONFIG_H
#define SKS_CONFIG_H

// *****************************************************************************
// Section: Platform Values (supplied by the project)
// *****************************************************************************

#include "sks_user_config.h"

#ifndef SKS_REGION_BASE_ADDR
#error "sks_user_config.h must define SKS_REGION_BASE_ADDR"
#endif

#ifndef SKS_REGION_SIZE
#error "sks_user_config.h must define SKS_REGION_SIZE"
#endif

#ifndef SKS_FLASH_PAGE_SIZE
#error "sks_user_config.h must define SKS_FLASH_PAGE_SIZE"
#endif

#ifndef SKS_FLASH_PROGRAM_UNIT
#error "sks_user_config.h must define SKS_FLASH_PROGRAM_UNIT"
#endif

/** Hardware key index of the Storage Root Key used for wrapping. */
#ifndef SKS_SRK_KEY_ID
#define SKS_SRK_KEY_ID              0U
#endif

// *****************************************************************************
// Section: Retry Limits
// *****************************************************************************

/** Maximum retries when the hardware crypto engine reports busy. */
#ifndef SKS_HW_MAX_RETRIES
#define SKS_HW_MAX_RETRIES          100U
#endif

/** Maximum retries when the TRNG is not ready. */
#ifndef SKS_IV_GEN_MAX_RETRIES
#define SKS_IV_GEN_MAX_RETRIES      100U
#endif

#endif /* SKS_CONFIG_H */

/*******************************************************************************
 End of File
*******************************************************************************/
