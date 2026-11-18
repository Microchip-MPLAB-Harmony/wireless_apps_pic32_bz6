/*******************************************************************************
  Secure Key Store - On-Flash Slot Layout (PRIVATE)

  File Name:
    sks_layout.h

  Summary:
    Slot structures, sizes, magic/state values and offsets of the key region.

  Description:
    Header (32 bytes):
      [0..3]   magic          - SKS_SLOT_MAGIC (0x4B455931 "KEY1") always
      [4]      formatVersion  - SKS_SLOT_FORMAT_VERSION
      [5]      slotIndex      - physical slot index
      [6]      keyId          - application key identifier
      [7]      keyType        - SKS_KEY_TYPE_* (sks.h)
      [8]      state          - SKS_SlotState_t (0xA5 PLAIN / 0x5A ENC / 0xFF EMPTY)
      [9]      encAlgo        - SKS_ENCALGO_AES256GCM (0x01) once wrapped
      [10..11] keyLen         - key length in bytes (LE uint16)
      [12..31] reserved[20]   - 0xFF

    The GCM authentication tag covers the full header as Associated Data
    (AAD), so no separate CRC is needed.

    Slot (124 bytes):  header(32) | keyPayload(64) | iv(12) | authTag(16)
    State detection:   header.state byte
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#if !defined(SKS_MODULE_SOURCE)
#error "Private to secure_key_store: include sks.h"
#endif

#ifndef SKS_LAYOUT_H
#define SKS_LAYOUT_H

#include <stdint.h>
#include "sks.h"
#include "port/sks_port.h"   /* SKS_PACKED, SKS_STATIC_ASSERT, SKS_IV_SIZE, SKS_TAG_SIZE */

// *****************************************************************************
// Section: Constants
// *****************************************************************************

/** Magic written to header.magic for all states (PLAIN and ENC).
 *  ASCII "KEY1". State is tracked separately via header.state. */
#define SKS_SLOT_MAGIC              0x4B455931UL

/** Magic word of an erased (EMPTY) slot. */
#define SKS_SLOT_MAGIC_ERASED       0xFFFFFFFFUL

/** Format version stored in header.formatVersion. */
#define SKS_SLOT_FORMAT_VERSION     0x01U

/** encAlgo value of an unwrapped slot. */
#define SKS_ENCALGO_NONE            0x00U

/** encAlgo value set in header.encAlgo when a slot is wrapped. */
#define SKS_ENCALGO_AES256GCM       0x01U

// *****************************************************************************
// Section: Slot Sizing
// *****************************************************************************

#define SKS_HEADER_SIZE              32U

/**
 * Fixed-field size inside SKS_SlotHeader_t:
 *   magic(4) + formatVersion(1) + slotIndex(1) + keyId(1) + keyType(1)
 *   + state(1) + encAlgo(1) + keyLen(2) = 12 bytes.
 */
#define SKS_HEADER_FIXED_FIELD_SIZE  12U

/** reserved[] array size inside SKS_SlotHeader_t = 32 - 12 = 20 B. */
#define SKS_HEADER_RESERVED_SIZE     (SKS_HEADER_SIZE - SKS_HEADER_FIXED_FIELD_SIZE)

/** Raw sum of all per-slot byte fields (header + key + IV + tag). */
#define SKS_SLOT_RAW_SIZE            (SKS_HEADER_SIZE          \
                                      + SKS_MAX_APP_KEY_SIZE    \
                                      + SKS_IV_SIZE             \
                                      + SKS_TAG_SIZE)

/** Slot size rounded up to 4-byte word boundary for flash alignment. */
#define SKS_SLOT_SIZE                (((SKS_SLOT_RAW_SIZE) + 3U) & ~3U)

/** Byte offset of slot n from the start of the key region. */
#define SKS_SLOT_OFFSET(n)          ((uint32_t)(n) * (uint32_t)SKS_SLOT_SIZE)

// *****************************************************************************
// Section: Types
// *****************************************************************************

/** Slot lifecycle state stored in header.state. */
typedef enum
{
    SKS_SLOT_STATE_EMPTY             = 0xFFU, /* erased - no key provisioned  */
    SKS_SLOT_STATE_PROVISIONED_PLAIN = 0xA5U, /* plaintext key present        */
    SKS_SLOT_STATE_PROVISIONED_ENC   = 0x5AU, /* AES-GCM wrapped              */
    SKS_SLOT_STATE_INVALID           = 0x00U, /* corrupted / unexpected value */
} SKS_SlotState_t;

/** Per-key header (32 bytes).
 *  The entire header is fed as Associated Data (AAD) during AES-GCM
 *  encryption.  Any modification of these bytes causes tag verification
 *  to fail during decryption - no separate CRC is needed. */
typedef struct SKS_PACKED
{
    uint32_t magic;          /* [0..3]   SKS_SLOT_MAGIC (always "KEY1")          */
    uint8_t  formatVersion;  /* [4]      SKS_SLOT_FORMAT_VERSION                 */
    uint8_t  slotIndex;      /* [5]      0 .. SKS_MAX_SLOTS-1                    */
    uint8_t  keyId;          /* [6]      application key identifier              */
    uint8_t  keyType;        /* [7]      SKS_KEY_TYPE_*                          */
    uint8_t  state;          /* [8]      SKS_SlotState_t                         */
    uint8_t  encAlgo;        /* [9]      SKS_ENCALGO_AES256GCM (0x01) after wrap */
    uint16_t keyLen;         /* [10..11] key length in bytes (LE)                */
    uint8_t  reserved[SKS_HEADER_RESERVED_SIZE]; /* [12..31] 0xFF (20 bytes)     */
} SKS_SlotHeader_t; /* 32 bytes */

/** Slot (124 bytes).  Header + payload + iv + authTag.
 *  For PLAIN:   keyPayload = plaintext, iv = 0xFF..., authTag = 0xFF...
 *  For WRAPPED: keyPayload = ciphertext, iv = random nonce, authTag = GCM tag */
typedef struct SKS_PACKED
{
    SKS_SlotHeader_t header;                           /* 32 B              */
    uint8_t          keyPayload[SKS_MAX_APP_KEY_SIZE]; /* 64 B              */
    uint8_t          iv[SKS_IV_SIZE];                  /* 12 B              */
    uint8_t          authTag[SKS_TAG_SIZE];            /* 16 B              */
} SKS_Slot_t; /* 124 bytes */

SKS_STATIC_ASSERT(sizeof(SKS_SlotHeader_t) == SKS_HEADER_SIZE,
                  "SKS_SlotHeader_t packed size != SKS_HEADER_SIZE (32)");
SKS_STATIC_ASSERT(sizeof(SKS_Slot_t) == SKS_SLOT_RAW_SIZE,
                  "SKS_Slot_t packed size != SKS_SLOT_RAW_SIZE (124)");
SKS_STATIC_ASSERT((SKS_SLOT_RAW_SIZE & 3U) == 0U,
                  "SKS_SLOT_RAW_SIZE is not 4-byte aligned");
SKS_STATIC_ASSERT(SKS_HEADER_SIZE >= SKS_HEADER_FIXED_FIELD_SIZE,
                  "SKS_HEADER_SIZE is too small for the fixed header fields");

// *****************************************************************************
// Section: Consistency With The Public Limits (sks.h) And The Region
// *****************************************************************************

SKS_STATIC_ASSERT(SKS_MAX_SLOTS == (SKS_REGION_SIZE / SKS_SLOT_SIZE),
                  "sks.h SKS_MAX_SLOTS does not match the private slot layout");
SKS_STATIC_ASSERT(SKS_MAX_SLOTS >= 1U,
                  "SKS_REGION_SIZE is smaller than one slot");
SKS_STATIC_ASSERT(SKS_MAX_SLOTS <= 255U,
                  "Slot index is a uint8_t: at most 255 slots");
SKS_STATIC_ASSERT((SKS_REGION_SIZE % SKS_FLASH_PAGE_SIZE) == 0U,
                  "SKS_REGION_SIZE must be a whole number of flash pages");
SKS_STATIC_ASSERT((SKS_FLASH_PAGE_SIZE % SKS_FLASH_PROGRAM_UNIT) == 0U,
                  "SKS_FLASH_PAGE_SIZE must be a whole number of program units");

#endif /* SKS_LAYOUT_H */

/*******************************************************************************
 End of File
*******************************************************************************/
