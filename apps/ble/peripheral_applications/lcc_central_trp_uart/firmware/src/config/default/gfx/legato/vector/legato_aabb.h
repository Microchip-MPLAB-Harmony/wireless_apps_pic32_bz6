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

#ifndef LE_AABB_H
#define LE_AABB_H

#include "gfx/legato/vector/legato_linef.h"

#ifdef __cplusplus
extern "C" {
#endif

struct leOBB;
struct leRect;

typedef struct leAABB
{
    struct leVector2 origin;
    struct leVector2 extents;
} leAABB;

leBool leAABB_ContainsPoint(const struct leAABB* aabb,
                            const struct leVector2* pnt);

void leAABB_EncloseOBB(const struct leOBB* obb,
                       struct leAABB* abb);

void leAABB_ToRect(const struct leAABB* aabb,
                   struct leRect* rct);

void leAABB_Combine(const struct leAABB* l,
                    const struct leAABB* r,
                    struct leAABB* res);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_AABB_H */
