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

#ifndef LE_REALI16_H
#define LE_REALI16_H

#include "gfx/legato/common/legato_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t leReal_i16;
typedef uint32_t leReal_u8;

#define LE_REAL_I16_ONE                ((leReal_i16)0x10000)
#define LE_REAL_I16_TWO                ((leReal_i16)0x20000)
#define LE_REAL_I16_ONE_OVER_TWO       ((leReal_i16)0x8000)
#define LE_REAL_I16_ONE_OVER_FOUR      ((leReal_i16)0x4000)
#define LE_REAL_I16_ONE_OVER_THREE     ((leReal_i16)0x5555)
#define LE_REAL_I16_ONE_OVER_FIVE      ((leReal_i16)0x3333)
#define LE_REAL_I16_ONE_OVER_SIX       ((leReal_i16)0x2AAA)
#define LE_REAL_I16_ONE_OVER_NINE      ((leReal_i16)0x1C71)
#define LE_REAL_I16_ONE_OVER_16        ((leReal_i16)0x1000)
#define LE_REAL_I16_ONE_OVER_25        ((leReal_i16)0xA3D)
#define LE_REAL_I16_MINUS_ONE          ((leReal_i16)0xFFFF0000)
#define LE_REAL_I16_PI                 ((leReal_i16)0x3243F)
#define LE_REAL_I16_PI_OVER_TWO        ((leReal_i16)0x1921F)
#define LE_REAL_I16_PI_OVER_FOUR       ((leReal_i16)0xC90F)
#define LE_REAL_I16_TWO_PI             ((leReal_i16)0x6487E)
#define LE_REAL_I16_THREE_PI_OVER_TWO  ((leReal_i16)0x4B65F)
#define LE_REAL_I16_FOUR_PI            ((leReal_i16)0xC90FD)
#define LE_REAL_I16_MINUS_PI           ((leReal_i16)0xFFFCDBC1)
#define LE_REAL_I16_MINUS_TWO_PI       ((leReal_i16)0xFFF9B782)
#define LE_REAL_I16_ONE_OVER_PI        ((leReal_i16)0x517C)
#define LE_REAL_I16_RADIANS            ((leReal_i16)0x477)
#define LE_REAL_I16_DEGREES            ((leReal_i16)0x394BB8)
#define LE_REAL_I16_360DEGREES         ((leReal_i16)0x1680000)
#define LE_REAL_I16_180DEGREES         ((leReal_i16)0xB40000)
#define LE_REAL_I16_90DEGREES          ((leReal_i16)0x5A0000)
#define LE_REAL_I16_EPSILON            ((leReal_i16)0x8)
#define LE_REAL_I16_FRACTION_BITS      16
#define LE_REAL_I16_FRACTION_SCL       (1 << LE_REAL_I16_FRACTION_BITS)
#define LE_REAL_I16_255                (255 << LE_REAL_I16_FRACTION_BITS)
#define LE_REAL_I16_MAX                (0x7FFFFFFF)
#define LE_REAL_I16_MIN                (0x1)

#define LE_REAL_I16_FROM_INT(i)    ((leReal_i16)((i) << LE_REAL_I16_FRACTION_BITS))
#define LE_REAL_I16_FROM_NEGINT(i) (~((((~i) + 1) << LE_REAL_I16_FRACTION_BITS) + 1))
#define LE_REAL_I16_TO_INT(r)      ((r) >> LE_REAL_I16_FRACTION_BITS)
#define LE_REAL_I16_TO_NEGINT(r)   (~((((~i) + 1) >> LE_REAL_I16_FRACTION_BITS) + 1))
#define LE_REAL_I16_FROM_FLOAT(f)  ((int32_t)(f * LE_REAL_I16_FRACTION_SCL))
#define LE_REAL_I16_TO_FLOAT(r)    ((float)r / LE_REAL_I16_FRACTION_SCL)
//#define LE_REAL_I16_FROM_U8(u)     ((u) << (LE_REAL_I16_FRACTION_BITS - LE_REAL_U8_FRACTION_BITS))
#define LE_REAL_I16_TO_U8(u)       ((u) >> (LE_REAL_I16_FRACTION_BITS - LE_REAL_U8_FRACTION_BITS))
#define LE_REAL_I16_EQUALS(l, r)   (((l) >= (r) - LE_REAL_I16_EPSILON) && ((l) <= (r) + LE_REAL_I16_EPSILON))
#define LE_REAL_I16_MULTIPLY(l, r) ((leReal_i16)((((int64_t)(l) * (int64_t)(r)) / LE_REAL_I16_FRACTION_SCL)))
#define LE_REAL_I16_DIVIDE(l, r)   ((leReal_i16)(((int64_t)(l) * LE_REAL_I16_FRACTION_SCL) / (r)))
#define LE_REAL_I16_NEGATE(r)      (~(r) + 1)

leReal_i16 leReal_i16_Min(leReal_i16 l, leReal_i16 r);

leReal_i16 leReal_i16_Max(leReal_i16 l, leReal_i16 r);

leReal_i16 leReal_i16_FromInt(int32_t i);

int32_t leReal_i16_ToInt(leReal_i16 rl);

leReal_i16 leReal_i16_Round(leReal_i16 rl);

leReal_i16 leReal_i16_Floor(leReal_i16 rl);

leReal_i16 leReal_i16_Ceiling(leReal_i16 rl);

leReal_i16 leReal_i16_Middle(leReal_i16 rl);

leReal_i16 leReal_i16_AbsoluteValue(leReal_i16 rl);

leReal_u8  leReal_i16_Square(leReal_i16 rl);

leReal_i16 leReal_i16_SquareRoot(leReal_i16 rl);

leReal_i16 leReal_i16_InverseSquareRoot(leReal_i16 rl);

leReal_i16 leReal_i16_Sin(leReal_i16 rad);

leReal_i16 leReal_i16_Cos(leReal_i16 rad);

leReal_i16 leReal_i16_Acos(leReal_i16 val);

leReal_i16 leReal_i16_Atan(leReal_i16 val);

leReal_i16 leReal_i16_Atan2(leReal_i16 y, leReal_i16 x);

leReal_i16 leReal_i16_FromU8(leReal_u8 u);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* LE_REALI16_H */
