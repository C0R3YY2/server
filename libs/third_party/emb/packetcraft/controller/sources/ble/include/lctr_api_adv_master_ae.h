/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller extended scanning master interface file.
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

#ifndef LCTR_API_ADV_MASTER_AE_H
#define LCTR_API_ADV_MASTER_AE_H

#include "lctr_api.h"
#include "../lctr/lctr_int_adv_slave_ae.h"
#include "lmgr_api_adv_master.h"
#include "wsf_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Constants
**************************************************************************************************/

/*! \brief      Master scan and initiate task messages for \a LCTR_DISP_SCAN dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_EXT_SCAN_MSG_RESET               = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Scan events */
  LCTR_EXT_SCAN_MSG_DISCOVER_ENABLE,    /*!< Scan enable API event. */
  LCTR_EXT_SCAN_MSG_DISCOVER_DISABLE,   /*!< Scan disable API event. */
  LCTR_EXT_SCAN_MSG_TERMINATE,          /*!< Scan BOD terminated event. */
  LCTR_EXT_SCAN_MSG_TOTAL               /*!< Total number of scan events. */
};

/*! \brief      Scanner create sync task messages for \a LCTR_DISP_PER_CREATE_SYNC dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_CREATE_SYNC_MSG_RESET            = LCTR_MSG_RESET,   /*!< Reset API message. */
  LCTR_CREATE_SYNC_MSG_CREATE,          /*!< Create sync create API event. */
  LCTR_CREATE_SYNC_MSG_CANCEL,          /*!< Create sync cancel sync API event. */
  LCTR_CREATE_SYNC_MSG_FAILED,          /*!< Create sync failed event. */
  LCTR_CREATE_SYNC_MSG_DONE,            /*!< Create sync done event. */
  LCTR_CREATE_SYNC_MSG_TERMINATE,       /*!< Create sync scanning BOD terminate event. */
  LCTR_CREATE_SYNC_MSG_TOTAL            /*!< Total number of Create sync events. */
};

/*! \brief      Scanner transfer sync task messages for \a LCTR_DISP_TRANFER_SYNC dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_TRANSFER_SYNC_MSG_RESET      = LCTR_MSG_RESET,                   /*!< Reset API message. */
  LCTR_TRANSFER_SYNC_MSG_START      = LCTR_CREATE_SYNC_MSG_CREATE,      /*!< Transfer sync start event. */
  LCTR_TRANSFER_SYNC_MSG_CANCEL     = LCTR_CREATE_SYNC_MSG_CANCEL,      /*!< Transfer sync cancel sync API event. */
  LCTR_TRANSFER_SYNC_MSG_FAILED     = LCTR_CREATE_SYNC_MSG_FAILED,      /*!< Transfer sync failed event. */
  LCTR_TRANSFER_SYNC_MSG_DONE       = LCTR_CREATE_SYNC_MSG_DONE,        /*!< Transfer sync done event. */
  LCTR_TRANSFER_SYNC_MSG_TERMINATE  = LCTR_CREATE_SYNC_MSG_TERMINATE,   /*!< Transfer sync scanning BOD terminate event. */
  LCTR_TRANSFER_SYNC_MSG_TOTAL      = LCTR_CREATE_SYNC_MSG_TOTAL        /*!< Total number of Transfer sync events. */
};

/*! \brief      Scanner periodic synchronous task messages for \a LCTR_DISP_PER_SCAN dispatcher. */
enum
{
  /* Broadcast events */
  LCTR_PER_SCAN_MSG_RESET               = LCTR_MSG_RESET,   /*!< Reset API message. */
  /* Advertising events */
  LCTR_PER_SCAN_MSG_SYNC_ESTD,          /*!< Periodic scanning synchronous established event. */
  LCTR_PER_SCAN_MSG_TERMINATE,          /*!< Periodic scanning terminate API event. */
  LCTR_PER_SCAN_MSG_TERMINATE_DONE,     /*!< Periodic scanning terminate done event. */
  LCTR_PER_SCAN_SUP_TIMEOUT,            /*!< Periodic synchronous supervision timeout event. */
  LCTR_PER_SCAN_MSG_TOTAL               /*!< Total number of periodic scanning events. */

};

/*! \brief      Scan PHYs. */
enum
{
  LCTR_SCAN_PHY_1M,                     /*!< LE 1M PHY scanner. */
  LCTR_INIT_PHY_2M,                     /*!< LE 2M PHY initiator (not valid for scanning). */
  LCTR_SCAN_PHY_CODED,                  /*!< LE Coded PHY scanner. */
  LCTR_SCAN_PHY_TOTAL,                  /*!< Total number of scanning PHYs. */
  LCTR_SCAN_PHY_ALL     = 0xFF          /*!< All PHY scanners. */
};

/*! \brief      DBAF Relevant Test Fields. */
enum
{
  LCTR_DBAF_RESOLVABLE_TAG = 0,
  LCTR_DBAF_ADVMODE        = 6,
  LCTR_DBAF_RSSI           = 7,
  LCTR_DBAF_PATH_LOSS      = 8,
  LCTR_DBAF_ADVA           = 9,
  LCTR_DBAF_EXACTLY_N_MIN  = 17,
  LCTR_DBAF_EXACTLY_N_MAX  = 24,
  LCTR_DBAF_AT_LEAST_N_MIN = 33,
  LCTR_DBAF_AT_LEAST_N_MAX = 40,
  LCTR_DBAF_AT_MOST_N_MIN  = 49,
  LCTR_DBAF_AT_MOST_N_MAX  = 56
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Extended scan enable message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           filtDup;            /*!< Filter out duplicate advertisers. */
  uint32_t          durMs;              /*!< Duration in milliseconds. */
  uint32_t          perMs;              /*!< Period in milliseconds. */
} lctrExtScanEnableMsg_t;

/*! \brief      Extended initiate message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           filterPolicy;       /*!< Scan filter policy. */
  uint8_t           ownAddrType;        /*!< Address type used by this device. */
  uint8_t           peerAddrType;       /*!< Address type used for peer device. */
  uint64_t          peerAddr;           /*!< Address of peer device. */
  uint8_t           initPhys;           /*!< Initiating PHYs. */
} lctrExtInitiateMsg_t;

/*! \brief      Extended scan message data. */
typedef union
{
  lctrMsgHdr_t           hdr;           /*!< Message header. */
  lctrExtScanEnableMsg_t enable;        /*!< Extended scan enable message data. */
  lctrExtInitiateMsg_t   init;          /*!< Extended initiate message data. */
} LctrExtScanMsg_t;

/*! \brief      Periodic create sync message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint8_t           filterPolicy;       /*!< Filter Policy. */
  uint8_t           repDisabled;        /*!< Reporting disabled. */
  uint8_t           advSID;             /*!< Advertising SID. */
  uint8_t           advAddrType;        /*!< Advertiser Address Type. */
  uint64_t          advAddr;            /*!< Advertiser Address. */
  uint16_t          skip;               /*!< Skip. */
  uint16_t          syncTimeOut;        /*!< Synchronization Timeout. */
  uint8_t           dupFilterEnable;    /*!< Duplicate filtering enable. */
  uint8_t           syncCteType;        /*!< CTE type filter bitfield. */
} lctrPerCreateSyncMsg_t;

/*! \brief      Periodic transfer sync message. */
typedef struct
{
  lctrMsgHdr_t      hdr;                /*!< Message header. */
  uint16_t          id;                 /*!< ID. */
  uint8_t           bSyncInfo[LL_SYNC_INFO_LEN];    /*!< SyncInfo bytes. */
  uint16_t          connHandle;         /*!< Connection handle. */
  uint16_t          ceRef;              /*!< Reference connection event counter. */
  uint16_t          ceRcvd;             /*!< Connection event counter when LL_PERIODIC_SYNC_IND was received. */
  uint16_t          lastPECounter;      /*!< Last PA event counter. */
  uint8_t           advSID;             /*!< Advertising SID. */
  uint8_t           advAddrType;        /*!< Advertiser Address Type. */
  uint8_t           scaB;               /*!< Sleep clock accuracy of the device sending LL_PERIODIC_SYNC_IND. */
  uint8_t           rxPhy;              /*!< PHY used for periodic advertising scan. */
  uint64_t          advAddr;            /*!< Advertiser Address. */
  uint16_t          syncConnEvtCounter; /*!< Connection event counter when the contents of the PDU is determined. */
  bool              pawrFlag;           /*!< true if PAwR is enabled. */
  uint32_t          rspAA;              /*!< Access address to be used when transmitting a response packet to advertiser. */
  uint8_t           numSubevents;       /*!< Number of subevents. */
  uint8_t           subeventInter;      /*!< Time in 1.25 ms units from the start of one subevent to the start of the next subevent. */
  uint8_t           rspSlotDelay;       /*!< Time in 1.25 ms units from the start of one subevent to the first response slot. */
  uint8_t           rspSlotSpacing;     /*!< Time in 0.625 ms units from the start of one response slot to the start of the next response slot. */
} lctrPerTransferSyncMsg_t;

/*! \brief      Periodic Advertising message data. */
typedef union
{
  lctrMsgHdr_t              hdr;            /*!< Message header. */
  lctrPerCreateSyncMsg_t    createSync;     /*!< Periodic create sync message data. */
  lctrPerTransferSyncMsg_t  transferSync;   /*!< Periodic transfer sync message data. */
} LctrPerScanMsg_t;

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*! \brief      Periodic scan message data. */
extern LctrPerScanMsg_t *pLctrMstPerScanMsg;

extern uint8_t *lctrPerAdvRspDataBufTbl[LL_MAX_PAWR_RSP_SLOTS][LL_MAX_PER_SCAN];
/*! \brief      Response observation data buffers to receive AUX_SYNC_SUBEVENT_RSP PDUs. */
extern uint8_t  *lctrPerAdvWithRspDataBufTbl[LL_MAX_PAWR_RSP_SLOTS][LL_MAX_PER_SCAN];


extern void (*pLctrFreePawrAdvSet)(lctrAdvSet_t *);
extern lctrPerAdvWithRspDataBuf_t *pLctrPawrRspDataBufTbl[LL_MAX_PAWR_ADV_SETS];
#if (LL_ENABLE_RSPOBS)
extern uint8_t *lctrPerSyncRspDataBufTbl[LL_MAX_PAWR_ADV_SETS];
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Initialization */
void LctrMstExtScanInit(void);
void LctrMstExtScanDefaults(void);
void LctrMstPerCreateSyncInit(void);
void LctrMstPerTransferSyncInit(void);
void LctrMstPerScanInit(void);

/* Status */
bool LctrMstExtScanValidateParam(void);

/* Control */
void LctrMstExtScanSetScanPhy(uint8_t scanPhy);
void LctrMstExtScanClearScanPhy(uint8_t scanPhy);
void LctrMstExtScanSetParam(uint8_t scanPhy, uint8_t ownAddrType, uint8_t scanFiltPolicy, const LlExtScanParam_t *pParam);
bool LctrMstExtScanIsEnabled(uint8_t scanPhy);
bool LctrMstPerIsSyncPending(void);
bool LctrMstPerIsSyncDisabled(void);
bool LctrMstPerIsSync(uint8_t advSID, uint8_t advAddrType, uint64_t advAddr);
uint8_t lctrMstPerGetNumPerScanCtx(void);
uint64_t LctrGetPerScanChanMap(uint16_t handle);
bool lctrMstPerIsSyncHandleValid(uint16_t syncHandle);
void LctrPastInit(void);
uint8_t LctrPeriodicAdvSyncTransfer(uint16_t connHandle, uint16_t serviceData, uint16_t syncHandle);
void LctrMstPerSetReceiveEnable(uint16_t syncHandle, uint8_t enable);
bool LctrPeriodicAdvSetImediateSkip(uint16_t syncHandle, uint16_t skip);

#ifdef __cplusplus
};
#endif

#endif /* LCTR_API_ADV_MASTER_AE_H */
