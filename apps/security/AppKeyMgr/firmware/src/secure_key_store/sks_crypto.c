/*******************************************************************************
  Secure Key Store - Crypto Operations

  File Name:
    sks_crypto.c

  Description:
    Implements SKS_PlatformInit(), SKS_CryptoOps_Init(), SKS_EncryptSlot(),
    SKS_EncryptAllSlots(), SKS_DecryptSlot() and SKS_ReadDeviceSecurity().

    All large local buffers (SKS_Slot_t, plaintext copy, ciphertext staging,
    IV, tag, header copy) are declared static - they never live on the task
    stack.  The functions are therefore not reentrant; call them
    sequentially from a single task.  Every buffer that can hold key
    material is zeroized before the functions return.

    Hardware access goes through the porting interfaces only; every value
    returned to the caller is an SKS_* result code.  No console output.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#define SKS_MODULE_SOURCE

#include <stddef.h>
#include <string.h>
#include "sks.h"
#include "sks_internal.h"
#include "port/sks_port.h"

// *****************************************************************************
// Section: File-scope Buffers (never on the stack)
// The crypto DMA reads/writes these buffers directly; the packed slot types
// are given 4-byte alignment explicitly.
// *****************************************************************************

/** Full slot read buffer - reused across Encrypt and Decrypt calls. */
static SKS_Slot_t sks_slotBuf SKS_ALIGNED(4);

/** Staging buffer for the plaintext key (zeroized after each use). */
static uint8_t sks_plainBuf[SKS_MAX_APP_KEY_SIZE] SKS_ALIGNED(4);

/** Staging buffer for the ciphertext output of the wrap operation. */
static uint8_t sks_cipherBuf[SKS_MAX_APP_KEY_SIZE] SKS_ALIGNED(4);

/** 12-byte IV generated fresh for each encryption. */
static uint8_t sks_iv[SKS_IV_SIZE] SKS_ALIGNED(4);

/** 16-byte GCM authentication tag. */
static uint8_t sks_tag[SKS_TAG_SIZE] SKS_ALIGNED(4);

/** In-RAM copy of the header used as AAD.
 *  Modified (state/algo updated) before being fed to the wrap operation. */
static SKS_SlotHeader_t sks_aadHeader SKS_ALIGNED(4);

// *****************************************************************************
// Section: Local Helpers
// *****************************************************************************

/* Classify a slot read from flash.
 * Returns SKS_OK for a valid PLAIN slot, SKS_ERR_EMPTY_SLOT,
 * SKS_ERR_ALREADY_ENC, or SKS_ERR_INVALID_STATE. */
static int sks_checkPlainSlot(const SKS_Slot_t *slot)
{
    uint16_t keyLen;

    if (slot->header.magic == SKS_SLOT_MAGIC_ERASED)
    {
        return SKS_ERR_EMPTY_SLOT;
    }
    if (slot->header.magic != SKS_SLOT_MAGIC)
    {
        return SKS_ERR_INVALID_STATE;
    }
    if ((SKS_SlotState_t)slot->header.state == SKS_SLOT_STATE_PROVISIONED_ENC)
    {
        return SKS_ERR_ALREADY_ENC;
    }
    if ((SKS_SlotState_t)slot->header.state != SKS_SLOT_STATE_PROVISIONED_PLAIN)
    {
        return SKS_ERR_INVALID_STATE;
    }

    keyLen = slot->header.keyLen;
    if ((keyLen == 0U) || (keyLen > (uint16_t)SKS_MAX_APP_KEY_SIZE))
    {
        return SKS_ERR_INVALID_STATE;
    }

    return SKS_OK;
}

/* Wrap a validated PLAIN slot in place (RAM only).
 *   - plaintext copied to sks_plainBuf (zeroized here on every path)
 *   - header with state = ENC / encAlgo = AES-256-GCM is the AAD
 *   - fresh random IV
 *   - on success the slot holds header, ciphertext (0xFF tail), IV and tag
 * Returns SKS_OK, SKS_ERR_TRNG or SKS_ERR_CRYPTO (slot unchanged on error). */
static int sks_wrapSlotInPlace(SKS_Slot_t *slot)
{
    uint16_t keyLen = slot->header.keyLen;
    int      r;

    memcpy(sks_plainBuf, slot->keyPayload, (size_t)keyLen);

    memcpy(&sks_aadHeader, &slot->header, sizeof(SKS_SlotHeader_t));
    sks_aadHeader.state   = (uint8_t)SKS_SLOT_STATE_PROVISIONED_ENC;
    sks_aadHeader.encAlgo = (uint8_t)SKS_ENCALGO_AES256GCM;

    /* Per NIST SP 800-38D 8.2, the (SRK, IV) pair must never repeat. */
    r = sks_port_crypto_random(sks_iv, (size_t)SKS_IV_SIZE);
    if (r != SKS_OK)
    {
        SKS_SecureZeroize(sks_plainBuf, sizeof(sks_plainBuf));
        return SKS_ERR_TRNG;
    }

    r = sks_port_crypto_wrap(sks_plainBuf, keyLen,
                             (const uint8_t *)&sks_aadHeader, sizeof(SKS_SlotHeader_t),
                             sks_iv,
                             sks_cipherBuf,
                             sks_tag);

    /* Zeroize plaintext immediately - regardless of the result. */
    SKS_SecureZeroize(sks_plainBuf, sizeof(sks_plainBuf));

    if (r != SKS_OK)
    {
        SKS_SecureZeroize(sks_cipherBuf, sizeof(sks_cipherBuf));
        return SKS_ERR_CRYPTO;
    }

    /* Replace the slot content with the wrapped record */
    slot->header = sks_aadHeader;
    memcpy(slot->keyPayload, sks_cipherBuf, (size_t)keyLen);
    if ((size_t)keyLen < (size_t)SKS_MAX_APP_KEY_SIZE)
    {
        memset(slot->keyPayload + keyLen,
               0xFFU,
               (size_t)SKS_MAX_APP_KEY_SIZE - (size_t)keyLen);
    }
    memcpy(slot->iv,      sks_iv,  SKS_IV_SIZE);
    memcpy(slot->authTag, sks_tag, SKS_TAG_SIZE);

    SKS_SecureZeroize(sks_cipherBuf, sizeof(sks_cipherBuf));

    return SKS_OK;
}

// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************

/*******************************************************************************
  Function:
    int SKS_PlatformInit(void)
*******************************************************************************/
int SKS_PlatformInit(void)
{
    return sks_port_crypto_platform_init();
}

/*******************************************************************************
  Function:
    int SKS_CryptoOps_Init(void)
*******************************************************************************/
int SKS_CryptoOps_Init(void)
{
    return sks_port_crypto_init();
}

/*******************************************************************************
  Function:
    int SKS_EncryptSlot(uint8_t slotIndex)
*******************************************************************************/
int SKS_EncryptSlot(uint8_t slotIndex)
{
    int r;

    /* Step 1: Read the slot from flash. */
    if (!SKS_ReadSlot(slotIndex, &sks_slotBuf))
    {
        r = SKS_ERR_FLASH;
    }
    else
    {
        /* Step 2: Only a valid PLAIN slot can be wrapped. */
        r = sks_checkPlainSlot(&sks_slotBuf);

        /* Step 3: Wrap in RAM, then page shadow write. */
        if (r == SKS_OK)
        {
            r = sks_wrapSlotInPlace(&sks_slotBuf);
        }

        if ((r == SKS_OK) && !SKS_WriteSlot(slotIndex, &sks_slotBuf))
        {
            r = SKS_ERR_FLASH;
        }
    }

    /* sks_slotBuf may hold the plaintext key - wipe it on every path. */
    SKS_SecureZeroize(&sks_slotBuf, sizeof(sks_slotBuf));

    return r;
}

/*******************************************************************************
  Function:
    int SKS_EncryptAllSlots(uint8_t *wrappedCount)
*******************************************************************************/
int SKS_EncryptAllSlots(uint8_t *wrappedCount)
{
    uint8_t    *pageBuf;
    SKS_Slot_t *slot;
    uint8_t     count = 0U;
    uint8_t     s;
    int         r     = SKS_OK;

    if (wrappedCount == NULL)
    {
        return SKS_ERR_INVALID_PARAM;
    }
    *wrappedCount = 0U;

    /* Step 1: Load the whole key page into RAM. */
    if (!SKS_PageLoad())
    {
        return SKS_ERR_FLASH;
    }
    pageBuf = SKS_PageBuffer();

    /* Step 2: Wrap every PLAIN slot in the RAM image.
     * Slot offsets are multiples of the (4-byte aligned) slot size, so each
     * slot in the 4-byte aligned page buffer is 4-byte aligned for the DMA. */
    for (s = 0U; (s < (uint8_t)SKS_MAX_SLOTS) && (r == SKS_OK); s++)
    {
        slot = (SKS_Slot_t *)(void *)(pageBuf + SKS_SLOT_OFFSET(s));

        if (sks_checkPlainSlot(slot) != SKS_OK)
        {
            continue;   /* EMPTY, WRAPPED or invalid - unchanged */
        }

        r = sks_wrapSlotInPlace(slot);
        if (r == SKS_OK)
        {
            count++;
        }
    }

    /* Step 3: All-or-nothing - one page rewrite with read-back verify, only
     * when every wrap succeeded and at least one slot was wrapped. */
    if ((r == SKS_OK) && (count > 0U))
    {
        if (SKS_PageCommit())
        {
            *wrappedCount = count;
        }
        else
        {
            r = SKS_ERR_FLASH;
        }
    }

    SKS_PageClear();

    return r;
}

/*******************************************************************************
  Function:
    int SKS_DecryptSlot(uint8_t slotIndex, ...)
*******************************************************************************/
int SKS_DecryptSlot(uint8_t   slotIndex,
                    uint8_t  *plaintextKeyOut,
                    uint16_t  bufLen,
                    uint16_t *keyLenOut)
{
    uint16_t keyLen = 0U;
    int      r      = SKS_OK;

    if ((plaintextKeyOut == NULL) || (keyLenOut == NULL))
    {
        return SKS_ERR_INVALID_PARAM;
    }

    /* Step 1: Read the wrapped slot from flash. */
    if (!SKS_ReadSlot(slotIndex, &sks_slotBuf))
    {
        r = SKS_ERR_FLASH;
    }
    /* Step 2: Validate the slot is WRAPPED and belongs to slotIndex. */
    else if (sks_slotBuf.header.magic == SKS_SLOT_MAGIC_ERASED)
    {
        r = SKS_ERR_EMPTY_SLOT;
    }
    else if ((sks_slotBuf.header.magic != SKS_SLOT_MAGIC) ||
             ((SKS_SlotState_t)sks_slotBuf.header.state != SKS_SLOT_STATE_PROVISIONED_ENC) ||
             (sks_slotBuf.header.slotIndex != slotIndex))
    {
        /* Not wrapped, or a record copied/moved from another slot.  The
         * slot index is part of the authenticated header, so it cannot be
         * changed without failing tag verification. */
        r = SKS_ERR_INVALID_STATE;
    }
    else
    {
        keyLen = sks_slotBuf.header.keyLen;
        if ((keyLen == 0U) || (keyLen > (uint16_t)SKS_MAX_APP_KEY_SIZE))
        {
            r = SKS_ERR_INVALID_STATE;
        }
        else if (bufLen < keyLen)
        {
            r = SKS_ERR_BUF_TOO_SMALL;
        }
        else
        {
            /* Step 3: AES-256-GCM decrypt + tag verify.
             * AAD = the full slot header as stored in flash (state = ENC),
             * exactly as used during wrapping.  The unwrap operation
             * zeroizes plaintextKeyOut on any failure. */
            r = sks_port_crypto_unwrap(sks_slotBuf.keyPayload,
                                       keyLen,
                                       (const uint8_t *)&sks_slotBuf.header,
                                       sizeof(SKS_SlotHeader_t),
                                       sks_slotBuf.iv,
                                       sks_slotBuf.authTag,
                                       plaintextKeyOut);
            if (r != SKS_OK)
            {
                r = (r == SKS_ERR_TAG_VERIFY) ? SKS_ERR_TAG_VERIFY : SKS_ERR_CRYPTO;
            }
        }
    }

    SKS_SecureZeroize(&sks_slotBuf, sizeof(sks_slotBuf));

    if (r == SKS_OK)
    {
        *keyLenOut = keyLen;
    }

    return r;

    /* CALLER MUST: SKS_SecureZeroize(plaintextKeyOut, *keyLenOut) after use */
}

/*******************************************************************************
  Function:
    int SKS_ReadDeviceSecurity(SKS_DeviceSecurity_t *out)
*******************************************************************************/
int SKS_ReadDeviceSecurity(SKS_DeviceSecurity_t *out)
{
    sks_port_device_info_t info;
    int                    r;

    if (out == NULL)
    {
        return SKS_ERR_INVALID_PARAM;
    }

    r = sks_port_device_read(&info);
    if (r != SKS_OK)
    {
        return r;
    }

    out->rootKeyLocked = info.rootKeyLocked;
    out->debugLocked   = info.debugLocked;
    out->uuidLocked    = info.uuidLocked;
    out->bootKeyLocked = info.bootKeyLocked;

    return SKS_OK;
}

/*******************************************************************************
 End of File
*******************************************************************************/
