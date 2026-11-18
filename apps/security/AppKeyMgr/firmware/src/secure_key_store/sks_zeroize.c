/*******************************************************************************
  SKS Secure Zeroize — Implementation

  File Name:
    sks_zeroize.c
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2024 Microchip Technology Inc. and its subsidiaries.
*******************************************************************************/
// DOM-IGNORE-END

#define SKS_MODULE_SOURCE

#include <stdint.h>
#include <stddef.h>
#include "sks.h"   /* SKS_SecureZeroize() is part of the public API */

/*******************************************************************************
  Function:
    void SKS_SecureZeroize(void *buf, size_t len)

  Notes:
    The volatile qualifier on 'p' prevents the compiler from treating the
    loop as dead code (the buffer is not read again by the caller after
    zeroization, so a plain memset can be silently removed under -O2/-O3).
    The volatile write forces the compiler to generate actual store
    instructions for every byte.
*******************************************************************************/
void SKS_SecureZeroize(void *buf, size_t len)
{
    volatile uint8_t *p = (volatile uint8_t *)buf;
    size_t            i;

    for (i = 0U; i < len; i++)
    {
        p[i] = 0U;
    }
}

/*******************************************************************************
 End of File
*******************************************************************************/
