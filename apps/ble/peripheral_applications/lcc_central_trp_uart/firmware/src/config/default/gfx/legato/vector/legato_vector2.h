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

#ifndef LE_VECTOR2_H
#define LE_VECTOR2_H

#include "gfx/legato/core/legato_real_i16.h"
#include "gfx/legato/core/legato_real_u8.h"
#include "gfx/legato/vector/legato_point.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct leVector2
{
    leReal_i16 x;
    leReal_i16 y;
} leVector2;

void leVector2_FromPoint(const struct lePoint* pnt,
                         struct leVector2* o);

void leVector2_ToPoint(const struct leVector2* vec,
                       struct lePoint* o);

void leVector2_Negate(const struct leVector2* v,
                      struct leVector2* o);

leBool leVector2_Equals(const struct leVector2* l,
                        const struct leVector2* r);

void leVector2_Add(const struct leVector2* l,
                   const struct leVector2* r,
                   struct leVector2* o);

void leVector2_Subtract(const struct leVector2* l,
                        const struct leVector2* r,
                        struct leVector2* o);

void leVector2_Multiply(const struct leVector2* l,
                        const struct leVector2* r,
                        struct leVector2* o);

void leVector2_Divide(const struct leVector2* l,
                      const struct leVector2* r,
                      struct leVector2* o);

void leVector2_Translate(const struct leVector2* v,
                         leReal_i16 x,
                         leReal_i16 y,
                         struct leVector2* o);

void leVector2_TranslateAlongVector(const struct leVector2* v,
                                    const struct leVector2* dir,
                                    leReal_i16 mag,
                                    struct leVector2* o);

void leVector2_Scale(const struct leVector2* v,
                     leReal_i16 mag,
                     struct leVector2* o);

void leVector2_Normalize(const struct leVector2* v,
                         struct leVector2* o);

leReal_i16 leVector2_Length(const struct leVector2* v);

leReal_u8 leVector2_SquaredLength(const struct leVector2* v);


void leVector2_Rotate(const struct leVector2* v,
                      leReal_i16 rad,
                      struct leVector2* o);

leReal_i16 leVector2_Dot(const struct leVector2* l,
                         const struct leVector2* r);

void leVector2_ProjectOnto(const struct leVector2* l,
                           const struct leVector2* r,
                           struct leVector2* o);

leReal_i16 leVector2_AngleBetween(const struct leVector2* l,
                                  const struct leVector2* r);

leReal_i16 leVector2_AngleOf(const struct leVector2* v);

void leVector2_CounterClockwiseNormal(const struct leVector2* v,
                                      struct leVector2* o);

void leVector2_ClockwiseNormal(const struct leVector2* v,
                               struct leVector2* o);

void leVector2_NormalizedDirectionVector(const struct leVector2* s,
                                         const struct leVector2* t,
                                         struct leVector2* o);

void leVector2_DirectionVector(const struct leVector2* s,
                               const struct leVector2* t,
                               struct leVector2* o);

extern const struct leVector2 LE_VECTOR2_ZERO;
extern const struct leVector2 LE_VECTOR2_UNIT;
extern const struct leVector2 LE_VECTOR2_UNIT_X;
extern const struct leVector2 LE_VECTOR2_UNIT_Y;
extern const struct leVector2 LE_VECTOR2_NEG_UNIT;
extern const struct leVector2 LE_VECTOR2_NEG_UNIT_X;
extern const struct leVector2 LE_VECTOR2_NEG_UNIT_Y;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_VECTOR2_H */
