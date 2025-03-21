/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Internal link layer controller slave connection interface file.
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

#ifndef LCTR_INT_CONN_SLAVE_H
#define LCTR_INT_CONN_SLAVE_H

#include "lctr_int_conn.h"
#include "bb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

extern LctrLlcpHdlr_t lctrSlvLlcpSmTbl[LCTR_LLCP_SM_TOTAL];

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* State machine */
void lctrSlvConnExecuteSm(lctrConnCtx_t *pConnCtx, uint8_t event);
void lctrSlvLlcpExecuteSm(lctrConnCtx_t *pCtx, uint8_t event);
bool lctrSlvLlcpExecuteConnUpdSm(lctrConnCtx_t *pCtx, uint8_t event);
void lctrConnStatelessEventHandler(lctrConnCtx_t *pCtx, uint8_t event);
bool lctrLlcpExecuteCommonSm(lctrConnCtx_t *pCtx, uint8_t event);
void lctrLlcpStatelessEventHandler(lctrConnCtx_t *pCtx, uint8_t event);

/* Status */
bool lctrSlvCheckEncOverrideConnParam(lctrConnCtx_t *pCtx);
bool lctrSlvCheckEncOverrideCommonParam(lctrConnCtx_t *pCtx);
bool lctrSlvCheckConnUpdInstant(lctrConnCtx_t *pCtx);
bool lctrSlvCheckEncOverridePowerControl(lctrConnCtx_t *pCtx);

/* Builder */
void lctrSlvConnBuildOp(lctrConnCtx_t *pCtx);

/* ISR */
void lctrSlvConnBeginOp(BbOpDesc_t *pOp);
void lctrSlvConnCleanupOp(BbOpDesc_t *pOp);
void lctrSlvConnEndOp(BbOpDesc_t *pOp);
void lctrSlvConnTxCompletion(BbOpDesc_t *pOp, uint8_t status);
void lctrSlvConnRxCompletion(BbOpDesc_t *pOp, uint8_t *pRxBuf, uint8_t status);
void lctrSlvConnAbortOp(BbOpDesc_t *pOp);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_INT_CONN_SLAVE_H */
