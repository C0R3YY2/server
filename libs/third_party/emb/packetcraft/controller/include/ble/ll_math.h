/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link Layer math utilities.
 *
 *  Copyright (c) 2013-2019 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2024 Packetcraft, Inc.  All rights reserved.
 *  Packetcraft, Inc. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/
#ifndef LL_MATH_H
#define LL_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wsf_types.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief      Binary divide with 1,000 divisor (n[max]=7,999). */
#define LL_MATH_DIV_10E3(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(536871)) >> 29))

/*! \brief      Binary divide with 1,000,000 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_10E6(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(4295)) >> 32))

/*! \brief      Binary divide with 10 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_10(n)       ((uint32_t)(((uint64_t)(n) * UINT64_C(419431)) >> 22))

/*! \brief      Binary divide with 27 divisor (n[max]=55,295). */
#define LL_MATH_DIV_27(n)       (((n) * UINT32_C(77673)) >> 21)

/*! \brief      Binary divide with 37 divisor (n[max]=75,776). */
#define LL_MATH_DIV_37(n)       (((n) * UINT32_C(56680)) >> 21)

/*! \brief      Binary modulo 37. */
#define LL_MATH_MOD_37(n)       ((n) - (LL_MATH_DIV_37(n) * 37))

/*! \brief      Binary divide with 1250 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_1250(n)     ((uint32_t)(((uint64_t)(n) * UINT64_C(1717987)) >> 31))

/*! \brief      Binary divide with 625 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_625(n)      ((uint32_t)(((uint64_t)(n) * UINT64_C(1717987)) >> 30))

/*! \brief      Binary divide with 30 divisor (n[max]=0xFFFFFFFF). */
#define LL_MATH_DIV_30(n)       ((uint32_t)(((uint64_t)(n) * UINT64_C(286331154)) >> 33))

/*! \brief      Binary divide with 300 divisor (n[max]=0x3FFFFFFF). */
#define LL_MATH_DIV_300(n)      ((uint32_t)(((uint64_t)(n) * UINT64_C(14660155038)) >> 42))

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      ECC service callback. */
typedef void (*LlMathEccServiceCback_t)(uint8_t op);

/*! \brief      ECC operations. */
enum
{
  LL_MATH_ECC_OP_GENERATE_P256_KEY_PAIR,   /*!< Generate P-256 key pair. */
  LL_MATH_ECC_OP_GENERATE_DH_KEY           /*!< Generate Diffie-Hellman key. */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Generate random number.
 *
 *  \return 32-bit random number.
 */
/*************************************************************************************************/
uint32_t LlMathRandNum(void);

/*************************************************************************************************/
/*!
 *  \brief  Return the number of bits which are set.
 *
 *  \param  num   Input parameter.
 *
 *  \return Number of bits which are set.
 */
/*************************************************************************************************/
uint8_t LlMathGetNumBitsSet(uint64_t num);

/*************************************************************************************************/
/*!
 *  \brief  Return result of a division.
 *
 *  \param  nu32        Numerator of size 32 bits.
 *  \param  de32        Denominator of size 32 bits.
 *
 *  \return Result of a division.
 */
/*************************************************************************************************/
uint32_t LlMathDivideUint32(uint32_t nu32, uint32_t de32);

/*************************************************************************************************/
/*!
 *  \brief  Return result of a division, rounding up.
 *
 *  \param  nu32        Numerator of size 32 bits.
 *  \param  de32        Denominator of size 32 bits.
 *
 *  \return Result of a division.
 */
/*************************************************************************************************/
uint32_t LlMathDivideUint32RoundUp(uint32_t nu32, uint32_t de32);

/*************************************************************************************************/
/*!
 *  \brief  Reverses bytes of a 32-bit word
 *
 *  \param  in        Input word to be byte-reversed
 *
 *  \return Byte-reversed word.
 */
/*************************************************************************************************/
static inline uint32_t lctrReverseBytesWord(uint32_t in)
{
  uint32_t u0 = in & 0xFF;
  uint32_t u1 = (in >> 8) & 0xFF;
  uint32_t u2 = (in >> 16) & 0xFF;
  uint32_t u3 = (in >> 24) & 0xFF;

  return (u0 <<  24) | (u1 << 16) | (u2 << 8) | (u3);
}

#ifdef __cplusplus
};
#endif

#endif /* LL_API_H */
