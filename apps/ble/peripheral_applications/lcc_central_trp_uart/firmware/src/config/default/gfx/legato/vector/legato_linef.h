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

#ifndef LE_LINE_H
#define LE_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gfx/legato/common/legato_common.h"
#include "gfx/legato/vector/legato_vector2.h"

typedef struct leLineF
{
    struct leVector2 p0;
    struct leVector2 p1;
} leLineF;

leReal_i16 leLineF_Length(const struct leLineF* ln);

leReal_i16 leLineF_Angle(const struct leLineF* ln);

void leLineF_Center(const struct leLineF* ln,
                    struct leVector2* pnt);

leReal_i16 leLineF_PointDistance(const struct leLineF* ln,
                                 const struct leVector2* pnt);

void leLineF_ClosestPoint(const struct leLineF* ln,
                          const struct leVector2* pnt,
                          struct leVector2* res);

leBool leLineF_PointSideQuery(const struct leLineF* ln,
                              const struct leVector2* pnt,
                              const struct leVector2* sign);

leResult leLineF_Normal(const struct leLineF* ln,
                        struct leVector2* nrm);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_LINE_H */
