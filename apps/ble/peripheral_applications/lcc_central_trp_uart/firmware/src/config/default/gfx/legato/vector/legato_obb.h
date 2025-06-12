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

#ifndef LE_OBB_H
#define LE_OBB_H

#include "gfx/legato/vector/legato_linef.h"

#ifdef __cplusplus
extern "C" {
#endif

struct leAABB;

typedef struct leOBB
{
    struct leVector2 origin;
    struct leVector2 extents;
    leReal_i16 angle;
} leOBB;

leResult leOBB_FromLine(const struct leLineF* ln,
                        leReal_i16 height,
                        struct leOBB* obb);

leBool leOBB_ContainsPoint(const struct leOBB* obb,
                           const struct leVector2* pnt);

leResult leOBB_AABB(const struct leOBB* obb,
                    struct leAABB* aabb);

leResult leOBB_Points(const struct leOBB* obb,
                      struct leVector2* pnts);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_OBB_H */
