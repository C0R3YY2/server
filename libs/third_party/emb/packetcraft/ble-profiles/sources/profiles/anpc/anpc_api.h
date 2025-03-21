/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Alert Notification profile client.
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
 *
 *  Copyright (c) 2019-2021 Packetcraft, Inc.  All rights reserved.
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
#ifndef ANPC_API_H
#define ANPC_API_H

#include "att_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup ALERT_NOTIFICATION_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief Enumeration of handle indexes of characteristics to be discovered */
enum
{
  ANPC_ANS_SNAC_HDL_IDX,          /*!< Supported new alert category */
  ANPC_ANS_NA_HDL_IDX,            /*!< New alert */
  ANPC_ANS_NA_CCC_HDL_IDX,        /*!< New alert CCC descriptor */
  ANPC_ANS_SUAC_HDL_IDX,          /*!< Supported unread alert category */
  ANPC_ANS_UAS_HDL_IDX,           /*!< Unread alert status */
  ANPC_ANS_UAS_CCC_HDL_IDX,       /*!< Unread alert status CCC descriptor */
  ANPC_ANS_ANCP_HDL_IDX,          /*!< Alert notification control point */
  ANPC_ANS_HDL_LIST_LEN           /*!< Handle list length */
};

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Alert Notification service.
 *          Parameter pHdlList must point to an array of length \ref ANPC_ANS_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AnpcAnsDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Send a command to the alert notification control point.
 *
 *  \param  connId    Connection identifier.
 *  \param  handle    Attribute handle.
 *  \param  command   Control point command.
 *  \param  catId     Alert category ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AnpcAnsControl(dmConnId_t connId, uint16_t handle, uint8_t command, uint8_t catId);

/*************************************************************************************************/
/*!
 *  \brief  Process a value received in an ATT read response, notification, or indication
 *          message.  Parameter pHdlList must point to an array of length \ref ANPC_ANS_HDL_LIST_LEN.
 *          If the ATT handle of the message matches a handle in the handle list the value
 *          is processed, otherwise it is ignored.
 *
 *  \param  pHdlList  Characteristic handle list.
 *  \param  pMsg      ATT callback message.
 *
 *  \return \ref ATT_SUCCESS if handle is found, \ref ATT_ERR_NOT_FOUND otherwise.
 */
/*************************************************************************************************/
uint8_t AnpcAnsValueUpdate(uint16_t *pHdlList, attEvt_t *pMsg);

/*! \} */    /* ALERT_NOTIFICATION_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* ANPC_API_H */
