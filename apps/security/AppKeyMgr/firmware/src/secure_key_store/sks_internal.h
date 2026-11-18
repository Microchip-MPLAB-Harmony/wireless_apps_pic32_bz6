/*******************************************************************************
  Secure Key Store - Internal API (PRIVATE)

  File Name:
    sks_internal.h

  Summary:
    Slot and page access used inside the module.

  Description:
    All offsets are relative to the start of the key region.  The key region
    is exactly one flash page.

    The flash controller must be initialised by the platform before any SKS
    function is used (MCC/Harmony: SYS_Initialize()).
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#if !defined(SKS_MODULE_SOURCE)
#error "Private to secure_key_store: include sks.h"
#endif

#ifndef SKS_INTERNAL_H
#define SKS_INTERNAL_H

#include <stdint.h>
#include <stdbool.h>
#include "sks_layout.h"

/*******************************************************************************
  Function:
    bool SKS_ReadSlot(uint8_t slotIndex, SKS_Slot_t *slotOut)

  Summary:
    Read one slot from flash.  Does not validate the header - the caller
    inspects slotOut->header after the call.

  Returns:
    true  - Read completed.
    false - slotIndex out of range, NULL pointer, or flash error.
*******************************************************************************/
bool SKS_ReadSlot(uint8_t slotIndex, SKS_Slot_t *slotOut);


/*******************************************************************************
  Function:
    bool SKS_WriteSlot(uint8_t slotIndex, const SKS_Slot_t *slotIn)

  Summary:
    Write one slot back to flash using a page shadow write.

  Description:
    Read-Modify-Erase-Write over the key page, so that only the target slot
    changes and all other slots are preserved:
      1. Reads the full page into the shared page buffer.
      2. Patches the buffer at the slot's offset with slotIn's bytes.
      3. Erases the page.
      4. Writes the page back from the buffer.
    The page buffer is zeroized before the function returns.

    The caller performs all data transformation (e.g. encryption) before
    calling; this function does not inspect slotIn.

  Returns:
    true  - All shadow-write operations succeeded.
    false - slotIndex out of range, NULL pointer, or flash error.
*******************************************************************************/
bool SKS_WriteSlot(uint8_t slotIndex, const SKS_Slot_t *slotIn);


/*******************************************************************************
  Batch page access

  Summary:
    Load the key page into a shared RAM buffer, modify any number of slots
    in RAM, then rewrite the page once.

  Description:
    Typical sequence:
      SKS_PageLoad();                       // flash -> RAM image
      buf = SKS_PageBuffer();               // patch slots at SKS_SLOT_OFFSET(n)
      SKS_PageCommit();                     // erase, program, read-back verify
      SKS_PageClear();                      // ALWAYS - the image may hold keys

    The buffer is shared with SKS_WriteSlot(), which zeroizes it when it
    returns: do not call SKS_WriteSlot() between SKS_PageLoad() and
    SKS_PageCommit().  Not reentrant.
*******************************************************************************/

/** Pointer to the shared, 4-byte aligned page buffer (SKS_FLASH_PAGE_SIZE B). */
uint8_t *SKS_PageBuffer(void);

/** Read the key page into the page buffer.  On failure the buffer is
 *  zeroized and false is returned. */
bool SKS_PageLoad(void);

/** Erase the key page, program it from the page buffer and compare the
 *  flash content with the buffer.  Returns false on a flash error or a
 *  read-back mismatch (no retry).  Does not clear the buffer. */
bool SKS_PageCommit(void);

/** Zeroize the page buffer. */
void SKS_PageClear(void);

#endif /* SKS_INTERNAL_H */

/*******************************************************************************
 End of File
*******************************************************************************/
