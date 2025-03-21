/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Controller HCI transport API.
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

#ifndef CHCI_API_H
#define CHCI_API_H

#include "wsf_types.h"
#include "wsf_os.h"
#include "cfg_mac.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief    Protocols that source and sink messages. */
enum
{
  CHCI_TR_PROT_BLE = 0,                   /*!< BLE protocol. */
  CHCI_TR_PROT_BOOT = 1,                  /*!< Boot protocol. */
  CHCI_TR_PROT_15P4 = 2,                  /*!< 802.15.4 protocol */
  CHCI_TR_PROT_CAL = 3,                   /*!< Radio Calibration protocol */
  CHCI_TR_PROT_CODEC = 4,                 /*!< CODEC Test protocol */
  CHCI_TR_PROT_NUM                        /*!< Number of protocols. */
};

/*! \brief    Type of message. */
enum
{
  CHCI_TR_TYPE_CMD  = 0,                  /*!< Command message (receive only). */
  CHCI_TR_TYPE_ACL,                       /*!< ACL data message (send or receive). */
  CHCI_TR_TYPE_EVT,                       /*!< Event message (send only). */
  CHCI_TR_TYPE_ISO,                       /*!< ISO data message (send or receive). */
  CHCI_TR_TYPE_NUM                        /*!< Number of types. */
};

/*! \brief      Error codes. */
enum
{
  CHCI_TR_CODE_INVALID_DATA     = 0xA0,   /*!< Invalid data received. */
  CHCI_TR_CODE_INVALID_DATA_LEN = 0xA1,   /*!< Invalid data length. */
  CHCI_TR_CODE_OUT_OF_MEMORY    = 0xA2    /*!< Out of memory. */
};

/*! \brief      802.15.4 protocol command type. */
#define CHCI_15P4_CMD_TYPE      0x80

/*! \brief      802.15.4 protocol data type. */
#define CHCI_15P4_DATA_TYPE     0x81

/*! \brief      Radio calibration command type. */
#define CHCI_CAL_CMD_TYPE       0xF1

/*! \brief      Radio calibration signal type. */
#define CHCI_CAL_SIG_TYPE       0xF2

/*! \brief      Radio calibration event type. */
#define CHCI_CAL_EVT_TYPE       0xF4

/*! \brief      802.15.4 protocol header length. */
#define CHCI_15P4_HDR_LEN       3

/*! \brief      Radio calibration header length. */
#define CHCI_CAL_HDR_LEN        3

/*! \brief      Message received callback. */
typedef void (*ChciTrRecvCback_t)(uint8_t type, uint8_t *pBuf);

/*! \brief      Message send complete callback.  */
typedef void (*ChciTrSendCompleteCback_t)(uint8_t type, uint8_t *pBuf);

/*! \brief      Service callback. */
typedef bool (*ChciTrServiceCback_t)(uint8_t *pType, uint16_t *pLen, uint8_t **pBuf);

/*! \brief      Send hardware error callback. */
typedef void (*ChciTrSendHwErrorCback_t)(uint8_t code);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the transport handler.
 *
 *  \param  handlerId       Handler ID.
 *  \param  maxAclLen       Maximum ACL data length.
 *  \param  maxIsoSduLen       Maximum ISO data length.
 */
/*************************************************************************************************/
void ChciTrHandlerInit(wsfHandlerId_t handlerId, uint16_t maxAclLen, uint16_t maxIsoSduLen);

/*************************************************************************************************/
/*!
 *  \brief      Controller HCI transport message dispatch handler.
 *
 *  \param      event       WSF event.
 *  \param      pMsg        WSF message.
 */
/*************************************************************************************************/
void ChciTrHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  Set callbacks for a protocol.
 *
 *  \param  prot                Protocol.
 *  \param  recvCback           Message received callback.
 *  \param  sendCompleteCback   Message send complete callback.
 *  \param  serviceCback        Service callback.
 */
/*************************************************************************************************/
void ChciTrSetCbacks(uint8_t prot, ChciTrRecvCback_t recvCback, ChciTrSendCompleteCback_t sendCompleteCback,
    ChciTrServiceCback_t serviceCback);

/*************************************************************************************************/
/*!
 *  \brief  Set send hardware error callback.
 *
 *  \param  sendHwErrorCback    Send hardware error callback.
 */
/*************************************************************************************************/
void ChciTrSetSendHwErrorCback(ChciTrSendHwErrorCback_t sendHwErrorCback);

/*************************************************************************************************/
/*!
 *  \brief  Flag protocol for needing service.
 *
 *  \param  prot                Protocol.
 */
/*************************************************************************************************/
void ChciTrNeedsService(uint8_t prot);

#ifdef __cplusplus
};
#endif

#endif /* CHCI_API_H */
