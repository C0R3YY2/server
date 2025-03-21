/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Wireless Data Exchange profile application interface.
 *
 *  Copyright (c) 2019-2023 Packetcraft, Inc.  All rights reserved.
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

#ifndef WDXC_API_H
#define WDXC_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup WIRELESS_DATA_EXCHANGE_PROFILE
 *  \{ */

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! \brief WDXC enumeration of handle indexes of characteristics to be discovered */
enum
{
  WDXC_DC_HDL_IDX,              /*!< WDX Device Configuration */
  WDXC_DC_CCC_HDL_IDX,          /*!< WDX Device Configuration CCC descriptor */
  WDXC_FTC_HDL_IDX,             /*!< WDX File Transfer Control */
  WDXC_FTC_CCC_HDL_IDX,         /*!< WDX File Transfer Control CCC descriptor */
  WDXC_FTD_HDL_IDX,             /*!< WDX File Transfer Data */
  WDXC_FTD_CCC_HDL_IDX,         /*!< WDX File Transfer Data CCC descriptor */
  WDXC_AU_HDL_IDX,              /*!< WDX Authenticationa */
  WDXC_AU_CCC_HDL_IDX,          /*!< WDX Authentication CCC descriptor */
  WDXC_HDL_LIST_LEN             /*!< WDX handle list length. */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/


/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/**************************************************************************************************
  Callback Function Datatypes
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  WDXC File Transfer Data callback.
 *
 *  \param  connId    Connection ID.
 *  \param  handle    Handle of the file.
 *  \param  len       length of pData in bytes.
 *  \param  pData     File data.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void WdxcFtdCallback_t(dmConnId_t connId, uint16_t handle, uint16_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  WDXC File Transfer Control callback.
 *
 *  \param  connId    Connection ID.
 *  \param  handle    Handle of the file.
 *  \param  op        Control operation.
 *  \param  status    Status of operation.
 *
 *  \return None.
 */
/*************************************************************************************************/
typedef void WdxcFtcCallback_t(dmConnId_t connId, uint16_t handle, uint8_t op, uint8_t status);

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the WDXC.
 *
 *  \param  pFtdCallback      Application Callback for File Transfer Data.
 *  \param  pFtcCallback      Application Callback for File Transfer Control.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcInit(WdxcFtdCallback_t *pFtdCallback, WdxcFtcCallback_t *pFtcCallback);

/*************************************************************************************************/
/*!
 *  \brief  Called by file system to notify the WDXC a non-blocking flash operation is complete.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFileSysOpComplete(void);

/*************************************************************************************************/
/*!
 *  \brief  Called by application to notify the WDXC of DM and ATT Events.
 *
 *  \param  pEvt   Pointer to the Event
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcProcMsg(wsfMsgHdr_t *pEvt);

/*************************************************************************************************/
/*!
 *  \brief  Perform service and characteristic discovery for Wireless Data Exchange service.
 *          Parameter pHdlList must point to an array of length \ref WDXC_HDL_LIST_LEN.
 *          If discovery is successful the handles of discovered characteristics and
 *          descriptors will be set in pHdlList.
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcWdxsDiscover(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Set service and characteristic handles .
 *
 *  \param  connId    Connection identifier.
 *  \param  pHdlList  Characteristic handle list.
 *
 *  \return None.
 *
 *  \note   This function can be used to restore handles after restablishing a bond with a device.
 */
/*************************************************************************************************/
void WdxcWdxsSetHandles(dmConnId_t connId, uint16_t *pHdlList);

/*************************************************************************************************/
/*!
 *  \brief  Perform File Discovery
 *
 *  \param  connId        Connection ID.
 *  \param  pFileInfo     Buffer to hold information about files
 *  \param  maxFiles      Size of pFileInfo in number of \ref wsfEfsFileInfo_t objects
 *
 *  \note   When file discovery completes, the ftcCallback() will be called with operator equal to
 *          \ref WDX_FTC_OP_EOF and file handle equal to \ref WDX_FLIST_HANDLE.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcDiscoverFiles(dmConnId_t connId, wsfEfsFileInfo_t *pFileInfo, uint8_t maxFiles);

/*************************************************************************************************/
/*!
 *  \brief  Transmit a file to a peer device.
 *
 *  \param  connId          Connection ID.
 *  \param  srcFileHandle   EFS handle of file to transmit.
 *  \param  dstFileHandle   EFS handle of destination file.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcPutFile(dmConnId_t connId, uint16_t srcFileHandle, uint16_t dstFileHandle);

/*************************************************************************************************/
/*!
 *  \brief  Request a file from a peer device.
 *
 *  \param  connId          Connection ID.
 *  \param  srcFileHandle   EFS handle of file to request.
 *  \param  dstFileHandle   EFS handle of destination file.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcGetFile(dmConnId_t connId, uint16_t srcFileHandle, uint16_t dstFileHandle);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to start a stream of a given file handle on the given connection.
 *
 *  \param  connId    Connection ID.
 *  \param  fileHdl   Handle of the file.
 *
 *  \return none.
 */
/*************************************************************************************************/
void WdxcStreamStart(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Stop the active stream.
 *
 *  \param  connId    Connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcStreamStop(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to abort a wdx operation.
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to abort operation on peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendAbort(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to erase a file.
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to erase on peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendEraseFile(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to verify a file.
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to verify on peer device
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendVerifyFile(dmConnId_t connId, uint16_t fileHdl);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to put a block of data into a file on the peer device
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file on peer device
 *  \param  offset          The offset from the beginning of the file in bytes
 *  \param  len             The number of bytes to put
 *  \param  fileSize        The size of the file in bytes
 *  \param  type            reserved
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendPutReq(dmConnId_t connId, uint16_t fileHdl, uint32_t offset,
                       uint32_t len, uint32_t fileSize, uint8_t type);

/*************************************************************************************************/
/*!
 *  \brief  Send a request to perform a get a block of data from a file on the peer device
 *
 *  \param  connId          Connection ID.
 *  \param  fileHdl         Handle of file to verify on peer device
 *  \param  offset          The offset from the beginning of the file in bytes
 *  \param  len             The number of bytes to get
 *  \param  type            reserved
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtcSendGetReq(dmConnId_t connId, uint16_t fileHdl, uint32_t offset, uint32_t len, uint8_t type);

/*************************************************************************************************/
/*!
 *  \brief  Send a data block to the peer device
 *
 *  \param  connId          Connection ID.
 *  \param  len             Size of pData in bytes
 *  \param  pData           Data to put to the file
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcFtdSendBlock(dmConnId_t connId, uint32_t len, uint8_t *pData);

/*************************************************************************************************/
/*!
 *  \brief  Send a Device Control Set request - disconnect request.
 *
 *  \param  connId          Connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcDcSendDisconnect(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Send a Device Control Set request - disconnect and reset request.
 *
 *  \param  connId          Connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcDcSetDisconnectAndReset(dmConnId_t connId);

/*************************************************************************************************/
/*!
 *  \brief  Send a Device Control Set request - disconnect, configure FOTA, and reset request.
 *
 *  \param  connId          Connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WdxcDcSetDisconnectCfgFotaAndReset(dmConnId_t connId);

/*! \} */    /* WIRELESS_DATA_EXCHANGE_PROFILE */

#ifdef __cplusplus
};
#endif

#endif /* WDXC_MAIN_H */
