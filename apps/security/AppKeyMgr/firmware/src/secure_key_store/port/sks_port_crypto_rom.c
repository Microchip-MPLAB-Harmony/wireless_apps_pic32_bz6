/*******************************************************************************
  Secure Key Store - Crypto Backend: Device ROM Cryptographic API (PIC32CX-BZ6)

  File Name:
    sks_port_crypto_rom.c

  Summary:
    Implements the crypto interface of sks_port.h with the cryptographic
    functions exposed by the device ROM API table (AES-256-GCM with the
    hardware Storage Root Key, TRNG).

  Description:
    - The ROM table offsets, prototypes and structure layouts come from the
      MCC-generated driver/security/ headers.
    - The SRK is referenced by hardware key index SKS_SRK_KEY_ID via
      CRM_KEYREF_LOAD_BY_ID() - its value never crosses into CPU-accessible
      memory.
    - The crypto engine clock is enabled once by
      sks_port_crypto_platform_init() (SKS_PlatformInit()) and never
      disabled.
    - All buffers passed to the CRM_AEAD_* functions must remain in stable
      memory until CRM_AEAD_WAIT() returns (the hardware DMA accesses them
      directly).
    - CRM_AEAD_CREATE_AESGCM_ENC/DEC() can return CRM_ERR_RETRY if the hardware
      engine is momentarily busy.  A spin-retry loop handles this.
    - Library status codes never leave this file: every function returns an
      SKS_* code.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#define SKS_MODULE_SOURCE

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../sks.h"
#include "sks_port.h"
#include "driver/security/cryptosym/aead_api.h"
#include "driver/security/cryptosym/keyref_api.h"
#include "driver/security/cryptosym/trng_api.h"
#include "driver/security/cryptosym/statuscodes.h"
#include "driver/security/cryptosym/internal.h"
#include "driver/security/api_table.h"   /* CRYPTO_CLK_ENABLE() */

// *****************************************************************************
// Section: File-scope State
// *****************************************************************************

/** TRNG context - kept at file scope (never on the stack). */
static struct crm_trng sks_trngCtx;

/** Tracks whether sks_port_crypto_init() has succeeded. */
static bool sks_trngInitialised = false;

/** Tracks whether the crypto engine clock has been enabled. */
static bool sks_clockEnabled = false;

// *****************************************************************************
// Section: Local Helpers
// *****************************************************************************

/* Map a failing unwrap status to an SKS code. */
static int sks_rom_unwrapFailure(int status)
{
    return (status == CRM_ERR_INVALID_TAG) ? SKS_ERR_TAG_VERIFY : SKS_ERR_CRYPTO;
}

// *****************************************************************************
// Section: Porting Interface Implementation
// *****************************************************************************

/*******************************************************************************
  Function:
    int sks_port_crypto_platform_init(void)
*******************************************************************************/
int sks_port_crypto_platform_init(void)
{
    /* Enable the crypto engine clock exactly once; it is never disabled. */
    if (!sks_clockEnabled)
    {
        CRYPTO_CLK_ENABLE()   /* macro already ends with ";" */
        sks_clockEnabled = true;
    }

    return SKS_OK;
}

/*******************************************************************************
  Function:
    int sks_port_crypto_init(void)
*******************************************************************************/
int sks_port_crypto_init(void)
{
    /* Precondition: the crypto engine clock was enabled once by
     * SKS_PlatformInit() (APP_Initialize) and is never disabled. */
    if (CRM_TRNG_INIT(&sks_trngCtx, NULL) != CRM_OK)   /* NULL = hardware defaults */
    {
        return SKS_ERR_TRNG;
    }

    sks_trngInitialised = true;
    return SKS_OK;
}

/*******************************************************************************
  Function:
    int sks_port_crypto_random(uint8_t *buf, size_t len)
*******************************************************************************/
int sks_port_crypto_random(uint8_t *buf, size_t len)
{
    int      status;
    uint32_t retries = 0U;

    if (!sks_trngInitialised)
    {
        return SKS_ERR_TRNG;
    }

    /* CRM_TRNG_GET returns a non-zero code while the TRNG FIFO does not yet
     * hold enough entropy.  Retry up to SKS_IV_GEN_MAX_RETRIES times. */
    do
    {
        status = CRM_TRNG_GET(&sks_trngCtx, (char *)buf, len);
        retries++;
    }
    while ((status != CRM_OK) && (retries < (uint32_t)SKS_IV_GEN_MAX_RETRIES));

    return (status == CRM_OK) ? SKS_OK : SKS_ERR_TRNG;
}

/*******************************************************************************
  Function:
    int sks_port_crypto_wrap(...)
*******************************************************************************/
int sks_port_crypto_wrap(const uint8_t *plaintextKey,
                         uint16_t       keyLen,
                         const uint8_t *aad,
                         size_t         aadLen,
                         const uint8_t  iv[SKS_IV_SIZE],
                         uint8_t       *ciphertext,
                         uint8_t        tag[SKS_TAG_SIZE])
{
    struct crmaead   aead;
    struct crmkeyref srkRef;
    int             status;
    uint32_t        retries = 0U;

    /* Load the SRK by hardware key index - a reference only, the SRK value
     * never crosses into CPU-accessible memory. */
    srkRef = CRM_KEYREF_LOAD_BY_ID(SKS_SRK_KEY_ID);

    /* Step 1: AES-GCM encryption context (retry while the engine is busy). */
    do
    {
        status = CRM_AEAD_CREATE_AESGCM_ENC(&aead, &srkRef, (const char *)iv);
        retries++;
    }
    while ((status == CRM_ERR_RETRY) && (retries < (uint32_t)SKS_HW_MAX_RETRIES));

    /* Step 2: header as AAD (authenticated, not encrypted). */
    if (status == CRM_OK)
    {
        status = CRM_AEAD_FEED_AAD(&aead, (const char *)aad, aadLen);
    }

    /* Step 3: plaintext -> ciphertext. */
    if (status == CRM_OK)
    {
        status = CRM_AEAD_CRYPT(&aead,
                               (const char *)plaintextKey, (size_t)keyLen,
                               (char *)ciphertext);
    }

    /* Step 4: 16-byte authentication tag. */
    if (status == CRM_OK)
    {
        status = CRM_AEAD_PRODUCE_TAG(&aead, (char *)tag);
    }

    /* Step 5: block until the hardware DMA finishes. */
    if (status == CRM_OK)
    {
        status = CRM_AEAD_WAIT(&aead);
    }

    return (status == CRM_OK) ? SKS_OK : SKS_ERR_CRYPTO;
}

/*******************************************************************************
  Function:
    int sks_port_crypto_unwrap(...)
*******************************************************************************/
int sks_port_crypto_unwrap(const uint8_t *ciphertext,
                           uint16_t       keyLen,
                           const uint8_t *aad,
                           size_t         aadLen,
                           const uint8_t  iv[SKS_IV_SIZE],
                           const uint8_t  tag[SKS_TAG_SIZE],
                           uint8_t       *plaintextKeyOut)
{
    struct crmaead   aead;
    struct crmkeyref srkRef;
    int             status;
    uint32_t        retries = 0U;

    /* Load the SRK by hardware key index. */
    srkRef = CRM_KEYREF_LOAD_BY_ID(SKS_SRK_KEY_ID);

    /* Step 1: AES-GCM decryption context (retry while the engine is busy). */
    do
    {
        status = CRM_AEAD_CREATE_AESGCM_DEC(&aead, &srkRef, (const char *)iv);
        retries++;
    }
    while ((status == CRM_ERR_RETRY) && (retries < (uint32_t)SKS_HW_MAX_RETRIES));

    if (status != CRM_OK)
    {
        return sks_rom_unwrapFailure(status);
    }

    /* Step 2: the same header bytes as AAD that were used for wrapping. */
    status = CRM_AEAD_FEED_AAD(&aead, (const char *)aad, aadLen);
    if (status != CRM_OK)
    {
        return sks_rom_unwrapFailure(status);
    }

    /* Step 3: ciphertext -> plaintextKeyOut. */
    status = CRM_AEAD_CRYPT(&aead,
                           (const char *)ciphertext, (size_t)keyLen,
                           (char *)plaintextKeyOut);

    /* Step 4: submit the stored tag for verification. */
    if (status == CRM_OK)
    {
        status = CRM_AEAD_VERIFY_TAG(&aead, (const char *)tag);
    }

    /* Step 5: block until the hardware finishes; CRM_ERR_INVALID_TAG if the
     * tag did not verify. */
    if (status == CRM_OK)
    {
        status = CRM_AEAD_WAIT(&aead);
    }

    if (status != CRM_OK)
    {
        /* NIST SP 800-38D 7.2: output is invalid if the tag fails. */
        SKS_SecureZeroize(plaintextKeyOut, (size_t)keyLen);
        return sks_rom_unwrapFailure(status);
    }

    return SKS_OK;
}

/*******************************************************************************
 End of File
*******************************************************************************/
