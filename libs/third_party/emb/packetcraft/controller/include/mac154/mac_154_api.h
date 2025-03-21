/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      802.15.4 MAC interface file.
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

#ifndef MAC_154_API_H
#define MAC_154_API_H

#include "wsf_types.h"
#include "mac_154_defs.h"
#include "mac_154_cfg.h"

/*! \addtogroup MAC_154_API
 *  \{ */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief  Version number. */
#define MAC_154_VER_NUM        0x00000000      /*!< Default value. Auto-generated by builder. */

/*! \brief      Message header length. */
#define MAC_154_MSG_HDR_LEN    3               /*!< TBD */

/*! brief       Command CHCI host to controller */
#define MAC_154_CMD_MLME_ASSOC_REQ            0x01     /*!< TBD */
#define MAC_154_CMD_MLME_ASSOC_RSP            0x02     /*!< TBD */
#define MAC_154_CMD_MLME_DISASSOC_REQ         0x03     /*!< TBD */
#define MAC_154_CMD_MLME_GET_REQ              0x04     /*!< TBD */
#define MAC_154_CMD_MLME_ORPHAN_RSP           0x05     /*!< TBD */
#define MAC_154_CMD_MLME_RESET_REQ            0x06     /*!< TBD */
#define MAC_154_CMD_MLME_RX_ENABLE_REQ        0x07     /*!< TBD */
#define MAC_154_CMD_MLME_SCAN_REQ             0x08     /*!< TBD */
#define MAC_154_CMD_MLME_SET_REQ              0x09     /*!< TBD */
#define MAC_154_CMD_MLME_START_REQ            0x0A     /*!< TBD */
#define MAC_154_CMD_MLME_POLL_REQ             0x0B     /*!< TBD */
#define MAC_154_CMD_MCPS_PURGE_REQ            0x0C     /*!< TBD */

/*! brief       Event CHCI controller to host */
#define MAC_154_EVT_MLME_ASSOC_CFM            0x81     /*!< TBD */
#define MAC_154_EVT_MLME_ASSOC_IND            0x82     /*!< TBD */
#define MAC_154_EVT_MLME_DISASSOC_CFM         0x83     /*!< TBD */
#define MAC_154_EVT_MLME_DISASSOC_IND         0x84     /*!< TBD */
#define MAC_154_EVT_MLME_BEACON_NOTIFY_IND    0x85     /*!< TBD */
#define MAC_154_EVT_MLME_GET_CFM              0x86     /*!< TBD */
#define MAC_154_EVT_MLME_ORPHAN_IND           0x87     /*!< TBD */
#define MAC_154_EVT_MLME_RESET_CFM            0x88     /*!< TBD */
#define MAC_154_EVT_MLME_RX_ENABLE_CFM        0x89     /*!< TBD */
#define MAC_154_EVT_MLME_SCAN_CFM             0x8A     /*!< TBD */
#define MAC_154_EVT_MLME_COMM_STATUS_IND      0x8B     /*!< TBD */
#define MAC_154_EVT_MLME_SET_CFM              0x8C     /*!< TBD */
#define MAC_154_EVT_MLME_START_CFM            0x8D     /*!< TBD */
#define MAC_154_EVT_MLME_POLL_CFM             0x8E     /*!< TBD */
#define MAC_154_EVT_MCPS_PURGE_CFM            0x8F     /*!< TBD */
#define MAC_154_EVT_MCPS_DATA_CFM             0x90     /*!< TBD */
#define MAC_154_EVT_MLME_POLL_IND             0x91     /*!< TBD */

/*! brief       Data CHCI host to controller */
#define MAC_154_DATA_MCPS_DATA_REQ            0x01     /*!< TBD */

/*! brief       Data CHCI controller to host */
#define MAC_154_DATA_MCPS_DATA_IND            0x81     /*!< TBD */

/*! \brief  Receive start flag. */
#define MAC_154_RX_START                      0x01     /*!< TBD */

/*! \brief  Receive stop flag. */
#define MAC_154_RX_STOP                       0x02     /*!< TBD */

/**************************************************************************************************
  Data Types
**************************************************************************************************/
/*! \brief  HCI header */
typedef struct
{
  uint8_t  code;  /*!< Code field. */
  uint16_t len;   /*!< Parameter length. */
} Mac154Hdr_t;

/*! \brief  Callback for a raw frame message. */
typedef void (*Mac154RawFrameFn_t)(uint8_t mpduLen, uint8_t *pMpdu, uint8_t linkQuality, uint32_t timestamp);

/*! \brief  Callbacks which can be used to intercept NHLE messages. */
typedef bool (*Mac154DataFn_t)(uint8_t *pBuf);

/*! \brief  Callbacks which can be used to intercept NHLE messages. */
typedef bool (*Mac154EvtFn_t)(uint8_t *pBuf);

/*! \brief      Pending operation function. */
typedef uint8_t (*Mac154PendingOpFn_t)(void *);

/*! \brief  MAC PIB. */
typedef struct
{
  uint64_t extAddr;                   /*!< TBD */
  uint64_t coordExtAddr;              /*!< TBD */
  uint32_t responseWaitTime;          /*!< TBD */
  uint16_t vsCRCOverride;             /*!< TBD */
  uint16_t vsFctlOverride;            /*!< TBD */
  uint16_t coordShortAddr;            /*!< TBD */
  uint16_t panId;                     /*!< TBD */
  uint16_t shortAddr;                 /*!< TBD */
  uint16_t transactionPersistenceTime;/*!< TBD */
  uint16_t maxFrameTotalWaitTime;     /*!< TBD */
  uint8_t  deviceType;                /*!< TBD */
  uint8_t  disableCCA;                /*!< TBD */
  uint8_t  rxEnabled;                 /*!< TBD */
  uint8_t  vsRawRx;                   /*!< TBD */
  uint8_t  ackWaitDuration;           /*!< TBD */
  uint8_t  associatedPanCoord;        /*!< TBD */
  uint8_t  associationPermit;         /*!< TBD */
  uint8_t  autoRequest;               /*!< TBD */
  uint8_t  minBE;                     /*!< TBD */
  uint8_t  maxBE;                     /*!< TBD */
  uint8_t  maxCSMABackoffs;           /*!< TBD */
  uint8_t  maxFrameRetries;           /*!< TBD */
  uint8_t  promiscuousMode;           /*!< TBD */
  uint8_t  rxOnWhenIdle;              /*!< TBD */
  uint8_t  securityEnabled;           /*!< TBD */
  uint8_t  bsn;                       /*!< TBD */
  uint8_t  dsn;                       /*!< TBD */
  uint8_t  beaconPayloadLength;       /*!< TBD */
  uint8_t  beaconPayload[MAC_154_aMaxBeaconPayloadLength]; /*!< TBD */
} Mac154Pib_t;

/*! \brief  MAC PHY PIB. */
typedef struct
{
  uint8_t chan;      /*!< TBD */
  int8_t  txPower;   /*!< TBD */
} Mac154PhyPib_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Register raw frame callback
 *
 *  \param      rawFrameCback   Raw frame callback handler
 *
 *  \note       Called at a low level, so passed frame must be dealt with expediently
 */
/*************************************************************************************************/
void Mac154RegisterRawFrameCback(Mac154RawFrameFn_t rawFrameCback);

/*************************************************************************************************/
/*!
 *  \brief      Invoke a command handler.
 *
 *  \param      pHdr  Command header.
 *  \param      pBuf  Command buffer.
 */
/*************************************************************************************************/
bool Mac154InvokeCmdHandler(Mac154Hdr_t *pHdr, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Register Event callback.
 *
 *  \param      evtCback    Event callback handler
 */
/*************************************************************************************************/
void Mac154RegisterEvtCback(Mac154EvtFn_t evtCback);

/*************************************************************************************************/
/*!
 *  \brief      Invoke data handler.
 *
 *  \param      pHdr  Data header.
 *  \param      pBuf  Data buffer.
 */
/*************************************************************************************************/
bool Mac154InvokeDataHandler(Mac154Hdr_t *pHdr, uint8_t *pBuf);

/*************************************************************************************************/
/*!
 *  \brief      Register Data callback
 *
 *  \param      dataCback   Data callback handler
 */
/*************************************************************************************************/
void Mac154RegisterDataCback(Mac154DataFn_t dataCback);

/*************************************************************************************************/
/*!
 *  \brief      Initialize 802.15.4.
 */
/*************************************************************************************************/
void Mac154Init(void);

/*************************************************************************************************/
/*!
 *  \brief      Reset MAC 802.15.4.
 */
/*************************************************************************************************/
void Mac154Reset(void);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC set extended address.
 *
 *  \param      extAddr   Extended address.
 */
/*************************************************************************************************/
void Mac154SetExtAddr(uint64_t extAddr);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC get PIB.
 *
 *  \return     Pointer to PIB.
 */
/*************************************************************************************************/
Mac154Pib_t *Mac154GetPIB(void);

/*************************************************************************************************/
/*!
 *  \brief      802.15.4 MAC get PHY PIB.
 *
 *  \return     Pointer to PHY PIB.
 */
/*************************************************************************************************/
Mac154PhyPib_t *Mac154GetPhyPIB(void);

/*************************************************************************************************/
/*!
 *  \brief      Determine whether rx is enabled
 *
 *  \return     true if rx is enabled, false otherwise
 *
 *  \note       Based on the two values: PIB rx on when idle and rx enable flag set through
 *              MLME-RX-ENABLE.req. Note PIB rx on when idle is considered a "conflicting
 *              responsibility" ([109,10-13])
 */
/*************************************************************************************************/
bool Mac154IsRxEnabled(void);

#ifdef __cplusplus
};
#endif

/*! \} */    /* MAC_154_API */

#endif /* MAC_154_API_H */
