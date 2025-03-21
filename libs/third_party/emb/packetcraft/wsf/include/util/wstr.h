/*************************************************************************************************/
/*!
 *  \file   wstr.h
 *
 *  \brief  String manipulation functions.
 *
 *  Copyright (c) 2014-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2020 Packetcraft, Inc.  All rights reserved.
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

#ifndef WSTR_H
#define WSTR_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WSF_UTIL_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! HEX format '0x' or '0X' check */
#define WSTR_IS_HEX_FORMAT(c) (((char)(c)[0] == '0') && \
                               (((char)(c)[1] == 'x') || ((char)(c)[1] == 'X')))

/*! Binary format '0b' check */
#define WSTR_IS_BIN_FORMAT(c)  (((char)(c)[0] == '0') && ((char)(c)[1] == 'b'))

/*************************************************************************************************/
/*!
 *  \brief  Copies a string up to a given length.
 *
 *  \param  pBuf    Pointer to buffer to copy to.
 *  \param  pData   Pointer to the string to copy.
 *  \param  n       Size of pBuf in bytes.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WstrnCpy(char *pBuf, const char *pData, uint8_t n);

/*************************************************************************************************/
/*!
 *  \brief  Byte by byte reverse and copy a buffer.
 *
 *  \param  pBuf1   Buffer to hold reversed copy.
 *  \param  pBuf2   Buffer to copy.
 *  \param  len     Size of pBuf1 and pBuf2 in bytes.
 */
/*************************************************************************************************/
void WStrReverseCpy(uint8_t *pBuf1, const uint8_t *pBuf2, uint16_t len);

/*************************************************************************************************/
/*!
 *  \brief  Byte by byte reverse a buffer.
 *
 *  \param  pBuf   Buffer to reverse.
 *  \param  len    size of pBuf in bytes.
 */
/*************************************************************************************************/
void WStrReverse(uint8_t *pBuf, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Format a hex value.
 *
 *  \param  pBuf    Storage for string representation of value.
 *  \param  val     Value.
 *  \param  len     Length of value, in bits.
 */
/*************************************************************************************************/
void WStrFormatHex(char *pBuf, uint32_t val, uint8_t len);

/*************************************************************************************************/
/*!
 *  \brief  Convert a formatted string to int array, zero out space after the string length.
 *
 *  \param  pStr    Pointer to the string to convert.
 *  \param  pBuf    Pointer to destination buffer.
 *  \param  len     Size of pBuf in bytes.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WStrHexToArray(const char *pStr, uint8_t *pBuf, uint16_t len);

/*! \} */    /* WSF_UTIL_API */

#ifdef __cplusplus
}
#endif

#endif /* WSTR_H */
