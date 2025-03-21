/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller privacy interface file.
 *
 *  Copyright (c) 2013-2018 Arm Ltd. All Rights Reserved.
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

#ifndef LCTR_API_PRIV_H
#define LCTR_API_PRIV_H

#include "lctr_api.h"
#include "lmgr_api_priv.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 *  \addtogroup LL_LCTR_API_PRIV
 *  \{
 */

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Slave advertising task messages for \a LCTR_DISP_PRIV dispatcher. */
enum
{
  /* Privacy events */
  LCTR_PRIV_MSG_RESET,                  /*!< HCI reset event. */
  LCTR_PRIV_MSG_RES_PRIV_ADDR_TIMEOUT,  /*!< Resolvable private address timeout event. */
  LCTR_PRIV_MSG_ADDR_RES_NEEDED,        /*!< Address resolution needed. */
  LCTR_PRIV_MSG_REFRESH_PRIV_ADDR       /*!< Refresh private addresses. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Address resolution pending message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                    /*!< Message header. */
  bool              peer;                   /*!< true if RPA is a peer's RPA. */
  uint8_t           peerAddrType;           /*!< Peer identity address type. */
  uint64_t          peerIdentityAddr;       /*!< Peer identity address. */
  uint64_t          rpa;                    /*!< Resolvable private address. */
} lctrAddrResNeededMsg_t;

/*! \brief      Address resolution pending message. */
typedef union
{
  lctrMsgHdr_t            hdr;              /*!< Message header. */
  lctrAddrResNeededMsg_t  addrResNeeded;    /*!< Address resolution needed. */
} LctrPrivMsg_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrPrivInit(void);

/* Control */
void LctrPrivSetResPrivAddrTimeout(uint32_t timeout);
void LctrRefreshPrivateAddrs(void);
void LctrRefreshPrivateAddr(uint8_t peerAddrType, uint64_t peerIdentityAddr);

/*! \} */    /* LL_LCTR_API_PRIV */

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_PRIV_H */
