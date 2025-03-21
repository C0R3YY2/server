/*************************************************************************************************/
/*!
*  \file
*
*  \brief  Cycling Speed and Cadence Profile API.
*
*  Copyright (c) 2016-2018 Arm Ltd. All Rights Reserved.
*
*  Copyright (c) 2019 Packetcraft, Inc.  All rights reserved.
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

#ifndef CSCP_API_H
#define CSCP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup CYCLING_SPEED_AND_CADENCE_PROFILE
 *  \{ */

/**************************************************************************************************
Macros
**************************************************************************************************/

/** \name Cycling Speed Measurement Parameter Types
 *
 */
/**@{*/
#define CSCP_SM_PARAM_WHEEL_REVOLUTIONS             0     /* Cumulative Wheel Revolutions */
#define CSCP_SM_PARAM_LAST_WHEEL_EVT_TIME           1     /* Last Wheel Event Time */
#define CSCP_SM_PARAM_CRANK_REVOLUTIONS             2     /* Cumulative Crank Revolutions */
#define CSCP_SM_PARAM_LAST_CRANK_TIME               3     /* Last Crank Event Time */
/**@}*/

/**************************************************************************************************
Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
*  \brief  Set a cycling speed measurement parameter.
*
*  \param  type   Parameter identifier
*  \param  value  Measurement value.
*
*  \return none
*/
/*************************************************************************************************/
void CscpsSetParameter(uint8_t type, uint32_t value);

/*************************************************************************************************/
/*!
*  \brief  Notifies the collector of a Cycle Speed Measurement.
*
*  \param  connId  Connection ID
*
*  \return none
*/
/*************************************************************************************************/
void CscpsSendSpeedMeasurement(dmConnId_t connId);

/*************************************************************************************************/
/*!
*  \brief  Set the sensor location attribute.
*
*  \param  location   Sensor Location.
*
*  \return none
*/
/*************************************************************************************************/
void CscpsSetSensorLocation(uint8_t location);

/*************************************************************************************************/
/*!
*  \brief  Set the features attribute.
*
*  \param  features   Features bitmask.
*
*  \return none
*/
/*************************************************************************************************/
void CscpsSetFeatures(uint16_t features);

/*! \} */    /* CYCLING_SPEED_AND_CADENCE_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* CSCP_API_H */
