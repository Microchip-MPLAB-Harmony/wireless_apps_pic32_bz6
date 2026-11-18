/*******************************************************************************
  Secure Key Store - Porting Interface (PRIVATE)

  File Name:
    sks_port.h

  Summary:
    Everything a platform port provides to the module:
      - compiler macros,
      - flash, crypto and device-security interfaces.

  Description:
    Each interface is implemented by exactly one backend file:
      Flash  : port/sks_port_flash_harmony.c (MCC/Harmony NVM PLIB)
      Crypto : port/sks_port_crypto_rom.c    (device ROM cryptographic API)
      Device : port/sks_port_device_rot.c    (device security fuses)

    The crypto and device backends are device-family specific; the backends
    include the MCC-generated "device.h" for the register definitions.
    No backend produces console output.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#if !defined(SKS_MODULE_SOURCE)
#error "Private to secure_key_store: include sks.h"
#endif

#ifndef SKS_PORT_H
#define SKS_PORT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// *****************************************************************************
// Section: Compiler Abstraction
// *****************************************************************************

#if defined(__GNUC__) || defined(__clang__)   /* XC32, arm-none-eabi-gcc, Clang */
#define SKS_PACKED                  __attribute__((packed))
#define SKS_ALIGNED(n)              __attribute__((aligned(n)))
#else
#error "secure_key_store: define SKS_PACKED and SKS_ALIGNED for this compiler"
#endif

#define SKS_STATIC_ASSERT(cond, msg)    _Static_assert(cond, msg)

// *****************************************************************************
// Section: Flash Interface
// All offsets are relative to the start of the key region; the backend alone
// knows where the region lives.  Geometry comes from sks_config.h
// (SKS_FLASH_PAGE_SIZE = erase unit, SKS_FLASH_PROGRAM_UNIT = program unit).
// All functions block until the flash operation has finished.
// *****************************************************************************

/** Copy 'len' bytes from region offset 'offset' into 'buf'.
 *  Returns true on success. */
bool sks_port_flash_read(uint32_t offset, void *buf, uint32_t len);

/** Erase the flash page at region offset 'offset'
 *  (SKS_FLASH_PAGE_SIZE-aligned).  Returns true on success. */
bool sks_port_flash_erase_page(uint32_t offset);

/** Program 'len' bytes from 'data' at region offset 'offset', one
 *  SKS_FLASH_PROGRAM_UNIT at a time.  'offset' must be program-unit aligned
 *  and 'data' 4-byte aligned and located in RAM; a trailing partial unit is
 *  not programmed.  The target area must be erased.  Returns true on
 *  success. */
bool sks_port_flash_program(uint32_t offset, const void *data, uint32_t len);

// *****************************************************************************
// Section: Crypto Interface
// AES-256-GCM with the device Storage Root Key, random numbers and crypto
// engine bring-up.  Every function returns SKS_* result codes only; vendor
// codes are mapped inside the backend.
// All buffers passed in must remain valid until the function returns
// (crypto hardware may access them by DMA).
// *****************************************************************************

/** AES-GCM IV (nonce) size in bytes. NIST SP 800-38D 5.2.1.1 recommends 96-bit. */
#define SKS_IV_SIZE                 12U

/** AES-GCM authentication tag size in bytes (128-bit full tag). */
#define SKS_TAG_SIZE                16U

/** Enable the crypto engine clock once; it is never disabled.
 *  Repeated calls are no-ops.  Returns SKS_OK. */
int sks_port_crypto_platform_init(void);

/** Initialise the random number generator.
 *  Returns SKS_OK or SKS_ERR_TRNG. */
int sks_port_crypto_init(void);

/** Fill 'buf' with 'len' random bytes.
 *  Returns SKS_OK or SKS_ERR_TRNG. */
int sks_port_crypto_random(uint8_t *buf, size_t len);

/** AES-256-GCM encrypt 'keyLen' bytes of 'plaintextKey' with the SRK.
 *  'aad' is authenticated but not encrypted.  Produces 'keyLen' bytes of
 *  ciphertext and a SKS_TAG_SIZE tag.
 *  Returns SKS_OK or SKS_ERR_CRYPTO. */
int sks_port_crypto_wrap(const uint8_t *plaintextKey,
                         uint16_t       keyLen,
                         const uint8_t *aad,
                         size_t         aadLen,
                         const uint8_t  iv[SKS_IV_SIZE],
                         uint8_t       *ciphertext,
                         uint8_t        tag[SKS_TAG_SIZE]);

/** AES-256-GCM decrypt and verify with the SRK.  On any failure the output
 *  buffer is zeroized before returning.
 *  Returns SKS_OK, SKS_ERR_TAG_VERIFY or SKS_ERR_CRYPTO. */
int sks_port_crypto_unwrap(const uint8_t *ciphertext,
                           uint16_t       keyLen,
                           const uint8_t *aad,
                           size_t         aadLen,
                           const uint8_t  iv[SKS_IV_SIZE],
                           const uint8_t  tag[SKS_TAG_SIZE],
                           uint8_t       *plaintextKeyOut);

// *****************************************************************************
// Section: Device Security Interface
// *****************************************************************************

/** Decoded device security state. */
typedef struct
{
    bool rootKeyLocked;   /* Storage Root Key fused and locked        */
    bool debugLocked;     /* Debug access locked                      */
    bool uuidLocked;      /* UUID locked                              */
    bool bootKeyLocked;   /* Secure-boot key locked                   */
} sks_port_device_info_t;

/** Read and decode the device security state (one hardware read).
 *  Returns SKS_OK, SKS_ERR_INVALID_PARAM (info == NULL) or
 *  SKS_ERR_INVALID_STATE (security fuse reload timed out). */
int sks_port_device_read(sks_port_device_info_t *info);

#endif /* SKS_PORT_H */

/*******************************************************************************
 End of File
*******************************************************************************/
