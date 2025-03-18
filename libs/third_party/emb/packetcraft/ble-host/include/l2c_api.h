/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  L2CAP subsystem API.
 *
 *  Copyright (c) 2009-2018 Arm Ltd. All Rights Reserved.
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

#ifndef L2C_API_H
#define L2C_API_H

#include "dm_api.h"
#include "l2c_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup STACK_L2CAP_API
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/** \name L2CAP Control Callback Events
 * Control callback message events
 */
/**@{*/
#define L2C_CTRL_FLOW_ENABLE_IND          0         /*!< Data flow enabled */
#define L2C_CTRL_FLOW_DISABLE_IND         1         /*!< Data flow disabled */
/**@}*/

/*! \brief Invalid channel registration ID for connection oriented channels */
#define L2C_COC_REG_ID_NONE               0

/*! \brief Invalid channel ID for connection oriented channels */
#define L2C_COC_CID_NONE                  0

/*! \brief Invalid signal identifier */
#define L2C_SIGNAL_ID_INVALID             0

/** \name L2CAP COC Channel Roles
 * Connection oriented channel initiator/acceptor role
 */
/**@{*/
#define L2C_COC_ROLE_NONE                 0x00U     /*!< No role (unallocated) */
#define L2C_COC_ROLE_INITIATOR            0x01U     /*!< Channel initiator */
#define L2C_COC_ROLE_ACCEPTOR             0x02U     /*!< Channel acceptor */
/**@}*/

/** \name L2CAP COC Data Confirm Codes
 * Connection oriented channel data confirm status values
 */
/**@{*/
#define L2C_COC_DATA_SUCCESS              0         /*!< Data request successful */
#define L2C_COC_DATA_ERR_MEMORY           1         /*!< Out of memory */
#define L2C_COC_DATA_ERR_OVERFLOW         2         /*!< Transaction overflow */
/**@}*/

/** \name L2CAP COC Callback Events
 * Connection oriented channel callback events.
 */
/**@{*/
#define L2C_COC_CBACK_START               0x70      /*!< L2C callback event starting value */
/*! \brief COC callback events */
enum
{
  L2C_COC_CONNECT_IND = L2C_COC_CBACK_START,        /*!< Channel connect indication */
  L2C_COC_DISCONNECT_IND,                           /*!< Channel disconnect indication */
  L2C_COC_EN_CONNECT_IND,                           /*!< Received enhanced connection indication */
  L2C_COC_EN_RECONFIG_IND,                          /*!< Received enhanced reconfiguration indication */
  L2C_COC_DATA_IND,                                 /*!< Received data indication */
  L2C_COC_DATA_CNF                                  /*!< Transmit data confirm */
};

#define L2C_COC_CBACK_CBACK_END           L2C_COC_DATA_CNF  /*!< L2C callback event ending value */
/**@}*/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief Connection oriented channel registration ID */
typedef uint16_t l2cCocRegId_t;

/*! \brief Connection oriented channel registration structure */
typedef struct
{
  uint16_t        psm;                    /*!< Protocol service multiplexer */
  uint16_t        mps;                    /*!< Maximum receive PDU fragment size */
  uint16_t        mtu;                    /*!< Maximum receive data packet size */
  uint16_t        credits;                /*!< Data packet receive credits for this channel */
  bool            authoriz;               /*!< true if authorization is required */
  uint8_t         secLevel;               /*!< Channel minimum security level requirements */
  uint8_t         role;                   /*!< Channel initiator/acceptor role */
} l2cCocReg_t;

/*! \brief Connection oriented channel connect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  uint16_t              cid;              /*!< Local channel ID */
  uint16_t              peerMtu;          /*!< Data packet MTU peer can receive */
  uint16_t              psm;              /*!< Connected PSM */
} l2cCocConnectInd_t;

/*! \brief Connection oriented channel disconnect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  uint16_t              cid;              /*!< Local channel ID */
  uint16_t              result;           /*!< Connection failure result code */
} l2cCocDisconnectInd_t;

/*! \brief Connection oriented channel data indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  uint16_t              cid;              /*!< Local channel ID */
  uint8_t               *pData;           /*!< Pointer to packet data */
  uint16_t              dataLen;          /*!< packet data length */
} l2cCocDataInd_t;

/*! \brief Connection oriented channel disconnect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  uint16_t              cid;              /*!< Local channel ID */
} l2cCocDataCnf_t;

/*! \brief Enhanced connection oriented channel connect indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  uint16_t              mps;              /*!< Data packet MPS peer can receive */
  uint16_t              mtu;              /*!< Data packet MTU peer can receive */
  bool                  req;              /*!< true if indicating a request, else a response. */
  uint8_t               cidLen;           /*!< Number of channels in cidList */
  uint16_t              cidList[L2C_MAX_EN_CHAN]; /*!< Local channel ID list */
} l2cCocEnConnectInd_t;

/*! \brief Enhanced connection oriented channel reconfiguration indication structure */
typedef struct
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  uint16_t              mps;              /*!< Data packet MPS */
  uint16_t              mtu;              /*!< Data packet MTU */
  bool                  req;              /*!< true if indicating a request, else a response. */
  uint8_t               cidLen;           /*!< Number of channels in cidList */
  uint16_t              cidList[L2C_MAX_EN_CHAN]; /*!< Local channel ID list */
} l2cCocEnReconfigInd_t;

/*!
 * \brief Connection oriented channel event structure
 *
 * Connection oriented channel callback header parameters:
 *
 * \param hdr.event     Callback event
 * \param hdr.param     DM connection ID
 * \param hdr.status    Event status (L2C_COC_DATA_CNF only)
 */
typedef union
{
  wsfMsgHdr_t           hdr;              /*!< Header structure */
  l2cCocConnectInd_t    connectInd;       /*!< Channel connect indication */
  l2cCocDisconnectInd_t disconnectInd;    /*!< Channel disconnect indication */
  l2cCocDataInd_t       dataInd;          /*!< Received data indication */
  l2cCocDataCnf_t       dataCnf;          /*!< Transmit data confirm */
  l2cCocEnConnectInd_t  enConnectInd;     /*!< Enhanced channel connect indication */
  l2cCocEnReconfigInd_t enReconfigInd;    /*!< Enhanced channel reconfigure indication */
} l2cCocEvt_t;

/*! \brief Configurable parameters */
typedef struct
{
  uint16_t            reqTimeout;         /*!< Request timeout in seconds */
} l2cCfg_t;

/*! \} */    /* STACK_L2CAP_API */

/**************************************************************************************************
  Global Variables;
**************************************************************************************************/

/*! \addtogroup STACK_INIT
 *  \{ */

/** \name L2CAP Configuration Structure
 * Pointer to structure containing initialization details of the L2CAP Subsystem.  To be configured
 * by Application.
 */
/**@{*/
/*! \brief Configuration pointer */
extern l2cCfg_t *pL2cCfg;
/**@}*/

/*! \} */    /* STACK_INIT */

/**************************************************************************************************
  Callback Function Types
**************************************************************************************************/

/*! \addtogroup STACK_L2CAP_API
 *  \{ */

/*************************************************************************************************/
/*!
 *  \brief  This callback function sends a received L2CAP packet to the client.
 *
 *  \param  handle    The connection handle.
 *  \param  len       The length of the L2CAP payload data in pPacket.
 *  \param  pPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*l2cDataCback_t)(uint16_t handle, uint16_t len, uint8_t *pPacket);

/*************************************************************************************************/
/*!
 *  \brief  This callback function sends control messages to the client.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*l2cCtrlCback_t)(wsfMsgHdr_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  This callback function sends data and other events to connection oriented
 *          channels clients.
 *
 *  \param  pMsg    Pointer to message structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void (*l2cCocCback_t)(l2cCocEvt_t *pMsg);

/*************************************************************************************************/
/*!
 *  \brief  This callback function asks clients of connection oriented channels if a given number
 *          of channels can be created on the PSM.
 *
 *  \param  connId    DM connection ID.
 *  \param  numChans  number of channels requested.
 *
 *  \return number of channels permitted by client.
 */
/*************************************************************************************************/
typedef uint8_t (*l2cCocAcceptCb_t)(dmConnId_t connId, uint8_t numChans);

/*************************************************************************************************/
/*!
 *  \brief  This callback function is used for authorization of connection oriented channels.
 *
 *  \param  connId    DM connection ID.
 *  \param  regId     The registration instance requiring authorization.
 *  \param  psm       The PSM of the registration instance.
 *
 *  \return L2C_CONN_SUCCESS if authorization is successful, any other value for failure.
 */
/*************************************************************************************************/
typedef uint16_t (*l2cCocAuthorCback_t)(dmConnId_t connId, l2cCocRegId_t regId, uint16_t psm);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/** \name L2CAP Initialization
 * Initialization and registration functions
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C for operation as a Bluetooth LE master.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cMasterInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C for operation as a Bluetooth LE slave.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cSlaveInit(void);

/**@}*/

/** \name L2CAP CID Functions
 * Register and send data over a CID
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  called by the L2C client, such as ATT or SMP, to register for the given CID.
 *
 *  \param  cid       channel identifier.
 *  \param  dataCback Callback function for L2CAP data received for this CID.
 *  \param  ctrlCback Callback function for control events for this CID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cRegister(uint16_t cid, l2cDataCback_t dataCback, l2cCtrlCback_t ctrlCback);


/*************************************************************************************************/
/*!
 *  \brief  Send an L2CAP data packet on the given CID.
 *
 *  \param  cid          The channel identifier.
 *  \param  handle       The connection handle.  The client receives this handle from DM.
 *  \param  len          The length of the payload data in pPacket.
 *  \param  pL2cPacket   A buffer containing the packet.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDataReq(uint16_t cid, uint16_t handle, uint16_t len, uint8_t *pL2cPacket);

/*************************************************************************************************/
/*!
*  \brief  Build and send a signaling packet.
*
*  \param  handle      The connection handle.
*  \param  code        Type of command.
*  \param  len         Length of the parameter.
*  \param  pParam      parameters of command to send.
*
*  \return None.
*/
/*************************************************************************************************/
void L2cDmSigReq(uint16_t handle, uint8_t code, uint16_t len, uint8_t *pParam);

/**@}*/

/** \name L2CAP COC Functions
 * Connection Oriented Channels Functions
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize L2C connection oriented channel subsystem.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocInit(void);

/*************************************************************************************************/
/*!
 *  \brief  Register to use a connection oriented channel, as either a channel acceptor,
 *          initiator, or both.  If registering as channel acceptor then the PSM is specified.
 *          After registering a connection can be established by the client using this
 *          registration instance.
 *
 *  \param  cback   Client callback function.
 *  \param  pReg    Registration parameter structure.
 *
 *  \return Registration instance ID or L2C_COC_REG_ID_NONE if registration failed.
 */
/*************************************************************************************************/
l2cCocRegId_t L2cCocRegister(l2cCocCback_t cback, l2cCocReg_t *pReg);

/*************************************************************************************************/
/*!
 *  \brief  Deregister and deallocate a connection oriented channel registration instance.
 *          This function should only be called if there are no active channels using this
 *          registration instance.
 *
 *  \param  regId   Registration instance ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDeregister(l2cCocRegId_t regId);

/*************************************************************************************************/
/*!
 *  \brief  Set the channel accept callback.
 *
 *  \param  regId   Registration instance ID.
 *  \param  cback   Client callback function.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocSetAcceptCback(l2cCocRegId_t regId, l2cCocAcceptCb_t cback);

/*************************************************************************************************/
/*!
 *  \brief  Initiate a connection to the given peer PSM.
 *
 *  \param  connId    DM connection ID.
 *  \param  regId     The associated registration instance.
 *  \param  psm       Peer PSM.
 *
 *  \return Local CID or L2C_COC_CID_NONE none if failure.
 */
/*************************************************************************************************/
uint16_t L2cCocConnectReq(dmConnId_t connId, l2cCocRegId_t regId, uint16_t psm);

/*************************************************************************************************/
/*!
 *  \brief  Disconnect the channel for the given CID.
 *
 *  \param  cid       Channel ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDisconnectReq(uint16_t cid);

/*************************************************************************************************/
/*!
 *  \brief  Send an L2CAP data packet on the given connection oriented CID.
 *
 *  \param  cid        The local channel identifier.
 *  \param  len        The length of the payload data in pPacket.
 *  \param  pPayload   Packet payload data.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocDataReq(uint16_t cid, uint16_t len, uint8_t *pPayload);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to open enhanced credit based channels.
 *
 *  \param  connId    DM connection ID.
 *  \param  regId     The associated registration instance.
 *  \param  psm       The protocol slave multiplexer.
 *  \param  credits   The initial number of credits for each CID channel.
 *  \param  numChan   The number of channels to create - L2C_MAX_EN_CHAN max.
 *
 *  \return false if unable make request, else true.
 */
/*************************************************************************************************/
bool L2cCocEnhancedConnectReq(dmConnId_t connId, l2cCocRegId_t regId, uint16_t psm,
                              uint16_t credits, uint8_t numChan);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to reconfigure enhanced credit based channels.
 *
 *  \param  connId    DM connection ID.
 *  \param  mtu       The maximum transmission unit of each source CID channel.
 *  \param  mps       The maximum payload size on each source CID channel.
 *  \param  numChan   The number of channels to create (1 to L2C_MAX_EN_CHAN).
 *  \param  pChanList A list of local CID to reconfigure (L2C_MAX_EN_CHAN channels, set unused to 0).
 *
 *  \return false if unable make request, else true.
 */
 /*************************************************************************************************/
bool L2cCocEnhancedReconfigReq(dmConnId_t connId, uint16_t mtu, uint16_t mps,
                               uint8_t numChan, uint16_t *pChanList);

/*************************************************************************************************/
/*!
 *  \brief  Register an authorization callback.
 *
 *  \param  callback  The callback function.
 *
 *  \note   If the authorization callback function returns L2C_CONN_FAIL_PEND_AUTHORIZ, then the
 *          higher layer shall subsequently call L2cCocAuthorize to authorize the pending request.
 */
 /*************************************************************************************************/
void L2cCocRegisterAuthorCback(l2cCocAuthorCback_t callback);

/*************************************************************************************************/
/*!
 *  \brief  Called to authorize a pending enhanced channel requests on a PSM.
 *
 *  \param  connId      Connection ID.
 *  \param  psm         The PSM.
 *  \param  authorized  true if channel creation is authorized, else false.
 *
 *  \note   This function shall be called to authorize a channel req after L2C_CONN_FAIL_PEND_AUTHORIZ
 *          is returned by the authorization callback function (see L2cCocRegisterAuthorCback).
 */
 /*************************************************************************************************/
void L2cCocAuthorize(dmConnId_t connId, uint16_t psm, bool authorized);

/*************************************************************************************************/
/*!
 *  \brief  Called to notify the L2CAP a connection is authentic during a pending enhanced
 *          channel requests.
 *
 *  \param  connId      Connection ID.
 *  \param  authentic   true if channel creation is authorized, else false.
 *
 *  \note   If authenPendSupp was set true in the call to L2cCocChanPendingInit, then this function
 *          shall be called by the application after the SMP security procedure.  This function
 *          will complete a pending enhanced channel request on the given channel.
 */
 /*************************************************************************************************/
void L2cCocAuthenticate(dmConnId_t connId, bool authentic);

/*************************************************************************************************/
/*!
 *  \brief  Initialize enhanced channel request auth pending.
 *
 *  \param  authenPendSupp  true if authentication pending is supported.
 *
 *  \note   If setting authenPendSupp true, L2cCocAuthenticate shall be called from the application
 *          after pairing or encryption is complete to complete channel requests that are pending
 *          authentication.
 */
 /*************************************************************************************************/
void L2cCocChanPendingInit(bool authenPendSupp);

/*************************************************************************************************/
/*!
 *  \brief  For testing purposes only.
 *
 *  \param  result    Result code
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocErrorTest(uint16_t result);

/*************************************************************************************************/
/*!
 *  \brief  For testing purposes only.
 *
 *  \param  cid       The local channel identifier.
 *  \param  credits   Credits to send.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cCocCreditSendTest(uint16_t cid, uint16_t credits);

/**@}*/

/** \name L2CAP Connection Parameter Update Functions
 *
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  This function is called by DM to send an L2CAP
 *          connection update request.
 *
 *  \param  handle      The connection handle.
 *  \param  pConnSpec   Pointer to the connection specification structure.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmConnUpdateReq(uint16_t handle, hciConnSpec_t *pConnSpec);

/*************************************************************************************************/
/*!
 *  \brief  For internal use only.  This function is called by DM to send an L2CAP
 *          connection update response.
 *
 *  \param  identifier  Identifier value previously passed from L2C to DM.
 *  \param  handle      The connection handle.
 *  \param  result      Connection update response result.
 *
 *  \return None.
 */
/*************************************************************************************************/
void L2cDmConnUpdateRsp(uint8_t identifier, uint16_t handle, uint16_t result);

/**@}*/

/*! \} */    /*! STACK_L2CAP_API */

#ifdef __cplusplus
};
#endif

#endif /* L2C_API_H */
