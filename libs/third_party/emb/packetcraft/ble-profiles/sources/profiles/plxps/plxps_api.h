/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Pulse Oximeter profile sensor.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
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
#ifndef PLXPS_API_H
#define PLXPS_API_H

#include "app_hw.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PULSE_OXIMETER_PROFILE
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Configurable parameters */
typedef struct
{
  wsfTimerTicks_t     period;     /*!< Continuous Measurement timer expiration period in ms */
} plxpsCfg_t;

/*! \brief Pulse Oximeter continuous measurement structure */
typedef appPlxCm_t plxpCm_t;

/*! \brief Pulse Oximeter spot check measurement structure */
typedef appPlxScm_t plxpScm_t;

/*************************************************************************************************/
/*!
 *  \brief  Initialize the Pulse Oximeter profile sensor.
 *
 *  \param  handlerId   DM connection identifier.
 *  \param  pCfg        Configuration parameters.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpsInit(wsfHandlerId_t handlerId, plxpsCfg_t *pCfg);

/*************************************************************************************************/
/*!
 *  \brief  This function is called by the application when a message that requires
 *          processing by the pulse oximeter profile sensor is received.
 *
 *  \param  pMsg     Event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpsProcMsg(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
*  \brief  Handle a button press.
*
*  \param  connId    Connection identifier.
*  \param  btn       Button press.
*
*  \return None.
*/
/*************************************************************************************************/
void PlxpsBtn(dmConnId_t connId, uint8_t btn);

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for pulse oximeter service.
 *
 *  \param  connId      DM connection identifier.
 *  \param  handle      ATT handle.
 *  \param  operation   ATT operation.
 *  \param  offset      Write offset.
 *  \param  len         Write length.
 *  \param  pValue      Value to write.
 *  \param  pAttr       Attribute to write.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t PlxpsWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                        uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr);

/*************************************************************************************************/
/*!
 *  \brief  Set the supported features of the pulse oximeter sensor.
 *
 *  \param  feature       Feature bitmask.
 *  \param  measStatus    Measurement status.
 *  \param  sensorStatus  Sensor status.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpsSetFeature(uint16_t feature, uint16_t measStatus, uint32_t sensorStatus);

/*************************************************************************************************/
/*!
 *  \brief  Set the CCCD index used by the application for pulse oximeter service characteristics.
 *
 *  \param  plxscCccIdx Pulse Oximeter spot check CCCD index.
 *  \param  plxcCccIdx  Pulse Oximeter continuous CCCD index.
 *  \param  racpCccIdx  Record access control point CCCD index.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpsSetCccIdx(uint8_t plxscCccIdx, uint8_t plxcCccIdx, uint8_t racpCccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Start periodic pulse oximeter measurement.  This function starts a timer to perform
 *          periodic measurements.
 *
 *  \param  connId      DM connection identifier.
 *  \param  timerEvt    WSF event designated by the application for the timer.
 *  \param  plxmCccIdx  Index of pulse oximeter CCC descriptor in CCC descriptor handle table.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpsMeasStart(dmConnId_t connId, uint8_t timerEvt, uint8_t plxmCccIdx);

/*************************************************************************************************/
/*!
 *  \brief  Stop periodic pulse oximeter measurement.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlxpsMeasStop(void);

/*************************************************************************************************/
/*!
 *  \brief  Send a spot check measurement indication.
 *
 *  \param  connId      Connection ID.
 *  \param  pMeas       Pointer to pulse oximeter spot check measurement.
 *
 *  \return None.
 */
/*************************************************************************************************/
void plxpsSendSpotCheckMeas(dmConnId_t connId, plxpScm_t *pMeas);

/*************************************************************************************************/
/*!
 *  \brief  Send a Continuous measurement notification.
 *
 *  \param  connId      Connection ID.
 *  \param  pMeas       Pointer to Pulse Oximiter continuous measurement.
 *
 *  \return None.
 */
/*************************************************************************************************/
void plxpsSendContinuousMeas(dmConnId_t connId, plxpCm_t *pMeas);

/*! \} */    /* PULSE_OXIMETER_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* PLXPS_API_H */
