/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
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

#ifndef LE_VECTOR_KERNEL_H
#define LE_VECTOR_KERNEL_H

#include "gfx/legato/vector/legato_vector.h"

#ifdef __cplusplus
extern "C" {
#endif

struct leVector_KernelContext
{
    leColor color;
    leReal_i16 alpha;
    enum leAntialiasingMode aaMode;
};

struct leVector_Kernel
{
    struct leVector_KernelContext* context;
    struct leVector2 origin;
    struct leRect scanArea;
    struct lePoint scanPoint;
    struct leVector2 pixelCenter;
    struct leVector2 testPoint;
    uint32_t hitCount;
    void (*evaluatePoint)(struct leVector_Kernel*);
    uint8_t (*shadeFragment)(struct leVector_Kernel*);
};

void leVector_Kernel_Exec(struct leVector_Kernel* krn);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // LE_VECTOR_KERNEL_H