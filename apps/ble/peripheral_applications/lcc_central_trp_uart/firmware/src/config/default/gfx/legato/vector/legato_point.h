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

#ifndef LE_POINT_H
#define LE_POINT_H

#include "gfx/legato/common/legato_math.h"

#ifdef __cplusplus
extern "C" {
#endif

void lePoint_Negate(const struct lePoint* v,
                    struct lePoint* o);

leBool lePoint_Equals(const struct lePoint* l,
                      const struct lePoint* r);

void lePoint_Add(const struct lePoint* l,
                 const struct lePoint* r,
                 struct lePoint* o);

void lePoint_Subtract(const struct lePoint* l,
                      const struct lePoint* r,
                      struct lePoint* o);

void lePoint_Scale(const struct lePoint* v,
                   float f,
                   struct lePoint* o);

extern const struct lePoint LE_POINT_ZERO;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_POINT_H */
