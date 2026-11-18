/*******************************************************************************
  Secure Key Store - Slot and Page Access

  Company:
    Microchip Technology Inc.

  File Name:
    sks_store.c

  Summary:
    Slot read/write, batch page access and key provisioning.

  Description:
    Implements SKS_ReadSlot, SKS_WriteSlot, the batch page functions
    (SKS_PageLoad / SKS_PageCommit / SKS_PageClear) and the public
    SKS_ProvisionSlots() on top of the flash porting interface
    (port/sks_port.h).  All offsets are relative to the start of the key
    region.

    The page buffer is static (file scope, never on the stack) and is
    zeroized after every operation; the functions are not reentrant.

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

// *****************************************************************************
// Section: Included Files
// *****************************************************************************

#include <stddef.h>
#include <string.h>
#include "sks.h"
#include "sks_internal.h"
#include "port/sks_port.h"

// *****************************************************************************
// Section: File-scope Buffers (never on the stack)
// *****************************************************************************

/* The page functions below handle exactly one flash page. */
SKS_STATIC_ASSERT(SKS_REGION_SIZE == SKS_FLASH_PAGE_SIZE,
                  "secure_key_store supports a key region of exactly one flash page");

/* Shared page buffer: one flash page, word-aligned.  Used by the page shadow
 * write, the batch page functions and SKS_ProvisionSlots().  It can hold
 * plaintext keys, so it is zeroized after every operation. */
static uint8_t sks_pageBuf[SKS_FLASH_PAGE_SIZE] SKS_ALIGNED(4);

/* Chunk size used to compare flash with sks_pageBuf after programming. */
#define SKS_VERIFY_CHUNK_SIZE       64U

SKS_STATIC_ASSERT((SKS_FLASH_PAGE_SIZE % SKS_VERIFY_CHUNK_SIZE) == 0U,
                  "SKS_FLASH_PAGE_SIZE must be a multiple of SKS_VERIFY_CHUNK_SIZE");

// *****************************************************************************
// Section: Local Helpers
// *****************************************************************************

/* Erase the key page and program it from sks_pageBuf. */
static bool sks_store_writePage(void)
{
    if (sks_port_flash_erase_page(0U) == false)
    {
        return false;
    }

    return sks_port_flash_program(0U, sks_pageBuf, SKS_FLASH_PAGE_SIZE);
}

// *****************************************************************************
// Section: Internal API Implementations
// *****************************************************************************

/*******************************************************************************
  Function:
    bool SKS_ReadSlot(uint8_t slotIndex, SKS_Slot_t *slotOut)
*******************************************************************************/
bool SKS_ReadSlot(uint8_t slotIndex, SKS_Slot_t *slotOut)
{
    if ((slotIndex >= (uint8_t)SKS_MAX_SLOTS) || (slotOut == NULL))
    {
        return false;
    }

    return sks_port_flash_read(SKS_SLOT_OFFSET(slotIndex),
                               (void *)slotOut,
                               (uint32_t)SKS_SLOT_SIZE);
}

/*******************************************************************************
  Function:
    bool SKS_WriteSlot(uint8_t slotIndex, const SKS_Slot_t *slotIn)
*******************************************************************************/
bool SKS_WriteSlot(uint8_t slotIndex, const SKS_Slot_t *slotIn)
{
    bool ok;

    if ((slotIndex >= (uint8_t)SKS_MAX_SLOTS) || (slotIn == NULL))
    {
        return false;
    }

    /* Read the page, patch the slot, erase, write back */
    ok = SKS_PageLoad();
    if (ok)
    {
        memcpy(sks_pageBuf + SKS_SLOT_OFFSET(slotIndex), slotIn, SKS_SLOT_SIZE);
        ok = sks_store_writePage();
    }

    /* The page may contain plaintext keys - wipe it on every path. */
    SKS_PageClear();

    return ok;
}

/*******************************************************************************
  Function:
    uint8_t *SKS_PageBuffer(void)
*******************************************************************************/
uint8_t *SKS_PageBuffer(void)
{
    return sks_pageBuf;
}

/*******************************************************************************
  Function:
    bool SKS_PageLoad(void)
*******************************************************************************/
bool SKS_PageLoad(void)
{
    if (sks_port_flash_read(0U, sks_pageBuf, SKS_FLASH_PAGE_SIZE) == false)
    {
        SKS_PageClear();
        return false;
    }

    return true;
}

/*******************************************************************************
  Function:
    bool SKS_PageCommit(void)
*******************************************************************************/
bool SKS_PageCommit(void)
{
    static uint8_t verifyBuf[SKS_VERIFY_CHUNK_SIZE] SKS_ALIGNED(4);

    uint32_t offset;
    bool     ok;

    /* Step 1: erase the page and program it from the RAM image */
    ok = sks_store_writePage();

    /* Step 2: read back and compare with the RAM image */
    for (offset = 0U; ok && (offset < SKS_FLASH_PAGE_SIZE); offset += SKS_VERIFY_CHUNK_SIZE)
    {
        ok = sks_port_flash_read(offset, verifyBuf, SKS_VERIFY_CHUNK_SIZE);

        if (ok && (memcmp(verifyBuf, sks_pageBuf + offset, SKS_VERIFY_CHUNK_SIZE) != 0))
        {
            ok = false;
        }
    }

    SKS_SecureZeroize(verifyBuf, sizeof(verifyBuf));

    return ok;
}

/*******************************************************************************
  Function:
    void SKS_PageClear(void)
*******************************************************************************/
void SKS_PageClear(void)
{
    SKS_SecureZeroize(sks_pageBuf, SKS_FLASH_PAGE_SIZE);
}

// *****************************************************************************
// Section: Public API
// *****************************************************************************

/*******************************************************************************
  Function:
    int SKS_ProvisionSlots(const SKS_KeyDesc_t *keys, uint8_t count,
                           uint8_t *provisionedCount)
*******************************************************************************/
int SKS_ProvisionSlots(const SKS_KeyDesc_t *keys,
                       uint8_t              count,
                       uint8_t             *provisionedCount)
{
    SKS_Slot_t *slot;
    uint8_t     written = 0U;
    uint8_t     i;
    uint8_t     j;

    if (provisionedCount == NULL)
    {
        return SKS_ERR_INVALID_PARAM;
    }
    *provisionedCount = 0U;

    /* ------------------------------------------------------------------
     * Step 1: Validate every descriptor - nothing is written on error.
     * ------------------------------------------------------------------ */
    if ((keys == NULL) || (count == 0U))
    {
        return SKS_ERR_INVALID_PARAM;
    }

    for (i = 0U; i < count; i++)
    {
        if ((keys[i].slotIndex >= (uint8_t)SKS_MAX_SLOTS) ||
            (keys[i].key == NULL) ||
            (keys[i].keyLen == 0U) ||
            (keys[i].keyLen > (uint16_t)SKS_MAX_APP_KEY_SIZE))
        {
            return SKS_ERR_INVALID_PARAM;
        }

        for (j = 0U; j < i; j++)
        {
            if (keys[j].slotIndex == keys[i].slotIndex)
            {
                return SKS_ERR_INVALID_PARAM;   /* duplicate target slot */
            }
        }
    }

    /* ------------------------------------------------------------------
     * Step 2: Build an unwrapped slot in the RAM image of the page for
     * every target slot that is EMPTY.  Slots in use are skipped.
     * ------------------------------------------------------------------ */
    if (!SKS_PageLoad())
    {
        return SKS_ERR_FLASH;
    }

    for (i = 0U; i < count; i++)
    {
        slot = (SKS_Slot_t *)(void *)(sks_pageBuf + SKS_SLOT_OFFSET(keys[i].slotIndex));

        if (slot->header.magic != SKS_SLOT_MAGIC_ERASED)
        {
            continue;   /* already provisioned, wrapped or invalid - unchanged */
        }

        /* All unset fields (reserved, iv, authTag, payload tail) = 0xFF */
        memset(slot, 0xFFU, sizeof(SKS_Slot_t));

        slot->header.magic         = SKS_SLOT_MAGIC;
        slot->header.formatVersion = (uint8_t)SKS_SLOT_FORMAT_VERSION;
        slot->header.slotIndex     = keys[i].slotIndex;
        slot->header.keyId         = keys[i].keyId;
        slot->header.keyType       = keys[i].keyType;
        slot->header.state         = (uint8_t)SKS_SLOT_STATE_PROVISIONED_PLAIN;
        slot->header.encAlgo       = (uint8_t)SKS_ENCALGO_NONE;
        slot->header.keyLen        = keys[i].keyLen;

        memcpy(slot->keyPayload, keys[i].key, (size_t)keys[i].keyLen);

        written++;
    }

    /* ------------------------------------------------------------------
     * Step 3: One page rewrite with read-back verify (only if needed).
     * ------------------------------------------------------------------ */
    if (written > 0U)
    {
        if (!SKS_PageCommit())
        {
            SKS_PageClear();
            return SKS_ERR_FLASH;
        }
    }

    SKS_PageClear();

    *provisionedCount = written;
    return SKS_OK;
}

/*******************************************************************************
 End of File
*******************************************************************************/
