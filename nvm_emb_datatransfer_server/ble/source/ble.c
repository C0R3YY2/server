////////////////////////////////////////////////////////////////////////////////
///
/// @file       ble.c
///
/// @project    T9305
///
/// @brief      Bluetooth LE management file.
///
/// @classification  Confidential
///
////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////
///
/// @copyright Copyright (c) 2021-2024, EM Microelectronic-Marin SA
/// @cond
///
/// All rights reserved.
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// 1. Redistributions of source code must retain the above copyright notice,
/// this list of conditions and the following disclaimer.
/// 2. Redistributions in binary form must reproduce the above copyright notice,
/// this list of conditions and the following disclaimer in the documentation
/// and/or other materials provided with the distribution.
///
////////////////////////////////////////////////////////////////////////////////
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
/// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
/// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
/// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
/// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
/// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
/// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
/// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
/// POSSIBILITY OF SUCH DAMAGE.
/// @endcond
////////////////////////////////////////////////////////////////////////////////

#include "ble.h"

#include <app_api.h>
#include <app_main.h>
#include <att_api.h>
#include <att_defs.h>
#include <att_handler.h>
#include <dm_api.h>
#include <dm_handler.h>
#include <emdt_svc.h>
#include <emdtp_server.h>
#include <gatt/gatt_api.h>
#include <hci_api.h>
#include <hci_handler.h>
#include <l2c_api.h>
#include <l2c_handler.h>
#include <ll_init_api.h>
#include <macros.h>
#include <my_app_signals.h>
#include <pal_cfg.h>
#include <pal_sys.h>
#include <smp_api.h>
#include <smp_handler.h>
#include <string.h>
#include <svc_ch.h>
#include <svc_core.h>
#include <svc_dis.h>
#include <util/bstream.h>
#include <wsf_buf.h>
#include <wsf_heap.h>
#include <wsf_msg.h>
#include <wsf_os.h>
#include <wsf_timer.h>
#include <wsf_types.h>
#include <wsf_math.h>

#ifdef PRINT_LTK
#include <printf.h>
#endif

/******************************************************************************\
 *  Configurable Parameters
\******************************************************************************/

// The 'app' prefix in this file refer to the application framework (AF), not
// the user application.

/** @brief Maximum number of connections.
 *  @note  Must be smaller than LL_MAX_CONN.
 */
#define BLE_CONN_MAX    (3u)

// Sanity check on the BLE_CONN_MAX definition.
_Static_assert(BLE_CONN_MAX <= LL_MAX_CONN,
               "BLE_CONN_MAX cannot be larger than LL_MAX_CONN.");

/** @brief   Maximum notifications per connection event.
 *  @warning If this number is too high, the available airtime for multiple connections will be poor.
 */
#define NUM_MAX_PACKETS_PER_INTERVAL (3u)

/** @brief ATT notification payload size for the Data Characteristic of EMDTS service */
#define ATT_EMDTS_PAYLOAD_SIZE 10

/** @brief ATT Desired MTU */
#define ATT_ACCEPTABLE_MTU          (23u)

/** @brief Link layer runtime configuration - TX buffers. */
#define LL_RT_CFG_NUM_TX_BUFS       (NUM_MAX_PACKETS_PER_INTERVAL * BLE_CONN_MAX)
/** @brief Link layer runtime configuration - RX buffers. */
#define LL_RT_CFG_NUM_RX_BUFS       (3u)

// Sanity check on the number of runtime buffers.
_Static_assert((LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS) <= UINT8_MAX,
               "Number of runtime buffers has to be less than 255.");

/** @brief Link layer runtime configuration - Maximum number of pending advertising reports. */
#define LL_RT_CFG_MAX_ADV_REPORTS   (3u)

/** @brief Advertising data, discoverable mode. */
static const uint8_t gcAdvDataDisc[] =
{
    // Flags.
    2,                          // Length.
    DM_ADV_TYPE_FLAGS,          // AD type.
    DM_FLAG_LE_GENERAL_DISC |
    DM_FLAG_LE_BREDR_NOT_SUP,

    // TX Power.
    2,                          // Length.
    DM_ADV_TYPE_TX_POWER,       // AD type.
    0,

    // Service UUID list.
    17,                         // Length.
    DM_ADV_TYPE_128_UUID,       // AD type.
    UUID_EM_DATA_TRANSFER_SERVICE // 128-bit UUID for proprietary service
};

/** @brief Scan data, discoverable mode. */
static const uint8_t gcScanDataDisc[] =
{
    // Device name.
    18,                     // Length.
    DM_ADV_TYPE_LOCAL_NAME, // AD type.
    'E', 'M', ' ', 'E', 'M', 'B', ' ', 'D', 'T', 'S', ' ', 'S', 'e', 'r', 'v', 'e', 'r',
};

/** @brief Configurable parameters for advertising (application framework). */
static const appAdvCfg_t gcAfAdvCfg =
{
    .advDuration = {   0,  0,  0 }, // Advertising durations in ms (0 = infinite) for state 1, state 2 and state 3.
    .advInterval = { 160,  0,  0 }  // Advertising intervals in 0.625 ms units for state 1, state 2 and state 3.
};

/** @brief Configurable parameters for peripheral (application framework). */
static const appSlaveCfg_t gcAfSlaveCfg =
{
    .connMax = BLE_CONN_MAX,        // Maximum connections.
};

/** @brief Configurable parameters for connection parameter update (application framework). */
static const appUpdateCfg_t gcAfUpdateCfg =
{
    .idlePeriod      = 0,                   // Connection idle period in ms before attempting
                                            // connection parameter update; set to zero to disable.
    .connIntervalMin = 640,                 // Minimum connection interval in 1.25ms units.
    .connIntervalMax = 800,                 // Maximum connection interval in 1.25ms units.
    .connLatency     = 0,                   // Connection latency.
    .supTimeout      = 900,                 // Supervision timeout in 10ms units.
    .maxAttempts     = 5                    // Number of update attempts before giving up.
};

/** @brief ATT configurable parameters (increase MTU). */
static const attCfg_t gcAttCfg =
{
    .discIdleTimeout = 15,                    // ATT server service discovery connection idle timeout in seconds
    .mtu             = ATT_ACCEPTABLE_MTU,    // acceptable ATT MTU
    .transTimeout    = ATT_MAX_TRANS_TIMEOUT, // transaction timeout in seconds
    .numPrepWrites   = 4                      // number of queued prepare writes supported by server
};

/** @brief Configurable parameters for security (application framework). */
static const appSecCfg_t gcAfSecCfg =
{
    .auth        = DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG, // Authentication and bonding flags.
    .iKeyDist    = 0,                                   // Initiator key distribution flags.
    .rKeyDist    = (uint8_t)DM_KEY_DIST_LTK,            // Responder key distribution flags.
    .oob         = false,                               // true if Out-of-band pairing data is present.
    .initiateSec = true                                 // true to initiate security upon connection.
};

/** @brief SMP security parameter configuration. */
static const smpCfg_t gcSmpCfg =
{
    .attemptTimeout    = 500,                   // 'Repeated attempts' timeout in msec.
    .ioCap             = SMP_IO_NO_IN_NO_OUT,   // I/O Capability.
    .minKeyLen         = 16,                    // Minimum encryption key length. Note: while this value can be reduced to 7, it is recommended to keep at 16 to prevent key entropy reduction attacks.
    .maxKeyLen         = 16,                    // Maximum encryption key length.
    .maxAttempts       = 1,                     // Attempts to trigger 'repeated attempts' timeout.
    .auth              = 0,                     // Device authentication requirements.
    .maxAttemptTimeout = 64000,                 // Maximum repeated attempts timeout in msec.
    .attemptDecTimeout = 64000,                 // Time msec before attemptExp decreases.
    .attemptExp        = 2                      // Repeated attempts multiplier exponent.
};

/** @brief Buffer Pool runtime configuration.
 *  @details Buffers will be allocated with the first possible match.
 */
static const wsfBufPoolDesc_t gcBuffPoolDesc[] =
{
    { .len = 16,                        .num = 4                                             },
    { .len = 32,                        .num = 5 + 2 * BLE_CONN_MAX                          },
    { .len = 64,                        .num = 2 * BLE_CONN_MAX                              },
    { .len = 112,                       .num = BLE_CONN_MAX                                  },
    { .len = ATT_ACCEPTABLE_MTU + 24,   .num = LL_RT_CFG_NUM_RX_BUFS + LL_RT_CFG_NUM_TX_BUFS },
    { .len = 272,                       .num = LL_RT_CFG_MAX_ADV_REPORTS                     },
    { .len = 320,                       .num = 1                                             }
};

/******************************************************************************\
 *  ATT Server Configuration.
\******************************************************************************/

/** @brief Enumeration of client characteristic configuration descriptors. */
enum
{
    BLE_GATT_SC_CCC_IDX,    // GATT service, service changed characteristic.
    EMDTSVC_DATA_CHARAC_CCC_IDX, // EM Data Transfer service, Data characteristic.
    BLE_NUM_CCC_IDX
};

/** @brief Client characteristic configuration descriptors settings, indexed by above enumeration. */
static const attsCccSet_t gcCccSet[BLE_NUM_CCC_IDX] = {
    [BLE_GATT_SC_CCC_IDX]         = { .handle     = GATT_SC_CH_CCC_HDL,
                                      .valueRange = ATT_CLIENT_CFG_INDICATE,
                                      .secLevel   = DM_SEC_LEVEL_NONE },
    [EMDTSVC_DATA_CHARAC_CCC_IDX] = { .handle     = EMDTSVC_DATA_CHARAC_CCC_HDL,
                                      .valueRange = ATT_CLIENT_CFG_NOTIFY,
                                      .secLevel   = DM_SEC_LEVEL_NONE }
};

/******************************************************************************\
 *  EM Data Transfer Profile Server Configuration.
\******************************************************************************/

/** @brief EM data transfer profile server configuration. */
static const EmdtpServer_Cfg_t gcEmdtpServerCfg =
{
    .dataCharCccIdx = EMDTSVC_DATA_CHARAC_CCC_IDX
};

/******************************************************************************\
 *  Type definition
\******************************************************************************/

/** @brief Connection control block structure. */
typedef struct
{
    dmConnId_t connId; /**< Connection ID */
    uint8_t    charToSend; /**< Letter */
} BLE_ConnCb_t;

/******************************************************************************\
 *  Global variables
\******************************************************************************/

/** @brief WSF handler ID. */
wsfHandlerId_t gBLE_SubsysHandlerId;

/** @brief Persistent BB runtime configuration. */
static BbRtCfg_t gBbRtCfg;

/** @brief Persistent LL runtime configuration. */
static LlRtCfg_t gLlRtCfg;

/** @brief Connection control block. */
static BLE_ConnCb_t gConnCb[BLE_CONN_MAX];

/******************************************************************************\
 *  Private functions declarations
\******************************************************************************/

static void BLE_SetupAdv(void);

static void BLE_StartAdv(void);

static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg);

/******************************************************************************\
 *  Private functions definitions
\******************************************************************************/

#ifdef PRINT_LTK
/**
 * @brief Print the Long-Term Key.
 * @param connId Connection ID.
 */
static void BLE_PrintLTK(dmConnId_t connId)
{
    extern uint8_t *SmpDmGetLtk(dmConnId_t connId);
    uint8_t *pLtk = SmpDmGetLtk(connId);

    printf("LTK: ");

    if (pLtk != NULL)
    {
        for (uint8_t i = 0; i < SEC_CMAC_KEY_LEN; i++)
        {
            printf("%02X", pLtk[i]);
        }
    }
    else
    {
        printf("INVALID");
    }

    printf("\r\n");
}
#endif // PRINT_LTK

/**
 * @brief Device Manager event callback.
 * @param pEvt DM event.
 */
static void BLE_DmCback(dmEvt_t *pEvt)
{
    dmEvt_t *pMsg;
    uint16_t len;

    len = DmSizeOfEvt(pEvt);

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
        memcpy(pMsg, pEvt, len);
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief ATT event callback.
 * @param pEvt ATT event.
 */
static void BLE_AttCback(attEvt_t *pEvt)
{
    attEvt_t *pMsg;

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t *) (pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief ATTS client characteristic configuration callback.
 * @param pEvt ATTS CCC event.
 */
static void BLE_AttsCccCback(attsCccEvt_t *pEvt)
{
    attsCccEvt_t  *pMsg;
    appDbHdl_t    dbHdl;

    // If CCC not set from initialization and there's a device record and currently bonded.
    if ((pEvt->handle != ATT_HANDLE_NONE) &&
        ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE) &&
        AppCheckBonded((dmConnId_t)pEvt->hdr.param))
    {
        // Store value in device database.
        AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
    }

    // Alloc a message and send it the BLE_SubsysHandler() function.
    if ((pMsg = WsfMsgAlloc(sizeof(attsCccEvt_t))) != NULL)
    {
        memcpy(pMsg, pEvt, sizeof(attsCccEvt_t));
        WsfMsgSend(gBLE_SubsysHandlerId, pMsg);
    }
}

/**
 * @brief Init a connection control block.
 * @param pConnCb The connection control block to be initialized.
 */
static void BLE_InitConnCb(BLE_ConnCb_t *pConnCb)
{
    pConnCb->charToSend = 'A';
}

/**
 * @brief Get the next free connection control block.
 * @return A pointer to the next free connection control block.
 * @retval NULL No more free connection control block.
 */
static BLE_ConnCb_t *BLE_GetNextFreeConnCb(void)
{
    BLE_ConnCb_t *pConnCb = NULL;

    for (uint32_t i = 0u; i < ARRAY_DIM(gConnCb); i++)
    {
        if (gConnCb[i].connId == DM_CONN_ID_NONE)
        {
            pConnCb = &gConnCb[i];
            break;
        }
    }

    return pConnCb;
}

/**
 * @brief Allocate a connection control block.
 * @param connId The ID of the connection for which a control block is to be assigned.
 * @return A pointer to the allocated connection control block.
 * @retval NULL The connection control block could not be allocated.
 */
static BLE_ConnCb_t *BLE_AllocConnCb(dmConnId_t connId)
{
    BLE_ConnCb_t *pConnCb;

    pConnCb = BLE_GetNextFreeConnCb();

    if (pConnCb != NULL)
    {
        pConnCb->connId = connId;
    }

    return pConnCb;
}

/**
 * @brief Free a connection control block.
 * @param pConnCb A pointer to a previously allocated connection control block.
 */
static void BLE_FreeConnCb(BLE_ConnCb_t *pConnCb)
{
    pConnCb->connId = DM_CONN_ID_NONE;
}

/**
 * @brief Get the connection block associated with a connection ID.
 * @param connId The connection ID for which to retrieve a control block.
 */
static BLE_ConnCb_t *BLE_GetConnCbWithConnId(dmConnId_t connId)
{
    BLE_ConnCb_t *pConnCb = NULL;

    for (uint32_t i = 0u; i < ARRAY_DIM(gConnCb); i++)
    {
        if (gConnCb[i].connId == connId)
        {
            pConnCb = &gConnCb[i];
            break;
        }
    }

    return pConnCb;
}

/**
 * @brief Update the character to send.
 * @param connId The connection ID for which to retrieve a control block.
 */
static void BLE_UpdateCharToSend(BLE_ConnCb_t *pConnCb)
{
    pConnCb->charToSend++;
    if (pConnCb->charToSend > 'Z')
    {
        // Reset character to 'A'.
        pConnCb->charToSend = 'A';
    }
}

/**
 * @brief Queue a notification in the stack.
 * @param pConnCb A pointer to a previously allocated connection control block.
 */
static void BLE_QueueNotification(BLE_ConnCb_t *pConnCb)
{
    // Notifications enabled for data transfer characteristic and connection is open.
    if ((AttsCccEnabled(pConnCb->connId, EMDTSVC_DATA_CHARAC_CCC_IDX)) &&
        (DM_CONN_ID_NONE != pConnCb->connId))
    {
        uint16_t len;
        uint8_t *pBuff;

        // Send ATT_EMDTS_PAYLOAD_SIZE characters in the notification (limited by ATT MTU).
        len = MIN(ATT_EMDTS_PAYLOAD_SIZE, AttGetMtu(pConnCb->connId) - ATT_VALUE_NTF_LEN);

        pBuff = AttMsgAlloc(len, ATT_PDU_VALUE_NTF);

        if (pBuff != NULL)
        {
            // Example dummy data here, this is where actual data could go.
            memset(pBuff, pConnCb->charToSend, len);
            AttsHandleValueNtfZeroCpy(pConnCb->connId, EMDTSVC_DATA_CHARAC_VALUE_HDL, len, pBuff);
            // Update the data to be sent for this connection.
            BLE_UpdateCharToSend(pConnCb);
        }
    }
}

/**
 * @brief EM Data Transfer Profile Server callback.
 * @param pEvt EMDTPS event.
 * @param pUserData User-defined parameter passed during configuration @ref EmdtpServer_Cfg_t.
 */
static void BLE_EmdtpServerCback(EmdtpServer_Event_t *pEvt, void *pUserData)
{
    switch (pEvt->type)
    {
        case EMDTPSERVER_NOTIFICATION_STARTED_EVENT:
        {
            BLE_ConnCb_t *pConnCb;
            dmConnId_t connId;

            connId = (dmConnId_t)pEvt->params.notifStarted.connId;
            pConnCb = BLE_GetConnCbWithConnId(connId);

            BLE_QueueNotification(pConnCb);
            break;
        }

        case EMDTPSERVER_NOTIFICATION_STOPPED_EVENT:
            break;

        case EMDTPSERVER_NOTIFICATION_QUEUED_EVENT:
            if (pEvt->params.notifQueued.attStatus == ATT_SUCCESS)
            {
                BLE_ConnCb_t *pConnCb;
                dmConnId_t connId;

                connId = (dmConnId_t)pEvt->params.notifQueued.connId;
                pConnCb = BLE_GetConnCbWithConnId(connId);

                BLE_QueueNotification(pConnCb);
            }
            break;

        default:
            break;
    }
}

/**
 * @brief BLE subsystem event handler.
 * @param event Unused.
 * @param pEvt Pointer to the event.
 */
static void BLE_SubsysHandler(wsfEventMask_t event, wsfMsgHdr_t *pEvt)
{
    // unused
    (void)(event);

    if (pEvt != NULL)
    {
        // Process ATT messages.
        if ((pEvt->event >= ATT_CBACK_START) && (pEvt->event <= ATT_CBACK_END))
        {
            // Process server-related ATT messages.
            AppServerProcAttMsg(pEvt);
        }
        // Process DM messages.
        else if ((pEvt->event >= DM_CBACK_START) && (pEvt->event <= DM_CBACK_END))
        {
            // Process advertising and connection-related messages.
            AppSlaveProcDmMsg((dmEvt_t*)pEvt);

            // Process security-related messages.
            AppSlaveSecProcDmMsg((dmEvt_t*)pEvt);
        }
        else
        {
            // no specific message to process
        }

        // Perform profiles, subsystems and other tasks related operations.
        BLE_ProcSubsysMsg(pEvt);
    }
}

/**
 * @brief Process messages from profiles, subsystems and other tasks.
 * @param pMsg Pointer to message.
 */
static void BLE_ProcSubsysMsg(wsfMsgHdr_t *pMsg)
{
    switch (pMsg->event)
    {
        // External signals and timer signals.
        case BLE_START_ADV_SIG:
            BLE_SetupAdv();
            BLE_StartAdv();
            break;

        // Device manager events.
        case DM_RESET_CMPL_IND:
            AttsCalculateDbHash();
            DmSecGenerateEccKeyReq();
            break;

        case DM_ADV_SET_START_IND:
            break;

        case DM_ADV_SET_STOP_IND:
            break;

        case DM_ADV_START_IND:
            break;

        case DM_ADV_STOP_IND:
            break;

        case DM_CONN_OPEN_IND:
        {
            dmConnId_t connId = (dmConnId_t)pMsg->param;
            BLE_ConnCb_t *pConnCb;

            pConnCb = BLE_AllocConnCb(connId);

            if (pConnCb != NULL)
            {
                BLE_InitConnCb(pConnCb);
            }
            break;
        }

        case DM_CONN_CLOSE_IND:
        {
            dmConnId_t connId = (dmConnId_t)pMsg->param;
            BLE_ConnCb_t *pConnCb;

            pConnCb = BLE_GetConnCbWithConnId(connId);
            BLE_FreeConnCb(pConnCb);

            break;
        }

        case DM_SEC_PAIR_CMPL_IND:
            DmSecGenerateEccKeyReq();
#ifdef PRINT_LTK
            BLE_PrintLTK((dmConnId_t)pMsg->param);
#endif // PRINT_LTK
            break;

        case DM_SEC_PAIR_FAIL_IND:
            DmSecGenerateEccKeyReq();
            break;

        case DM_SEC_ENCRYPT_IND:
            break;

        case DM_SEC_ENCRYPT_FAIL_IND:
            break;

        case DM_SEC_AUTH_REQ_IND:
            AppHandlePasskey(&((dmEvt_t *)pMsg)->authReq);
            break;

        case DM_SEC_ECC_KEY_IND:
            DmSecSetEccKey(&((dmEvt_t *)pMsg)->eccMsg.data.key);
            break;

        case DM_SEC_COMPARE_IND:
            AppHandleNumericComparison(&((dmEvt_t *)pMsg)->cnfInd);
            break;

        case DM_PRIV_CLEAR_RES_LIST_IND:
            break;

        case DM_HW_ERROR_IND:
            break;

        // ATT server events.
        case ATTS_HANDLE_VALUE_CNF:
            EmdtpServer_ProcMsg(pMsg);
            break;

        case ATTS_CCC_STATE_IND:
            EmdtpServer_ProcMsg(pMsg);
            break;

        default:
            break;
    }
}

/**
 * @brief Set up advertising.
 */
static void BLE_SetupAdv(void)
{
    // Set advertising and scan response data for discoverable mode.
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(gcAdvDataDisc), (uint8_t*)gcAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(gcScanDataDisc), (uint8_t*)gcScanDataDisc);

    // Set advertising and scan response data for connectable mode.
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
    AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);

    // Set advertising mode.
    AppSetAdvType(DM_ADV_CONN_UNDIRECT);
}

/**
 * @brief Start advertising.
 */
static void BLE_StartAdv(void)
{
    // Start advertising; automatically set connectable/discoverable mode and bondable mode.
    AppAdvStart(APP_MODE_AUTO_INIT);
}

/**
 * @brief Buffer initialization.
 * @details gcBuffPoolDesc variable can be modified to suit the user's needs.
 */
static void BLE_InitBuffer(void)
{
    uint32_t memUsed;

    const uint8_t numPools = sizeof(gcBuffPoolDesc) / sizeof(gcBuffPoolDesc[0]);

    memUsed = WsfBufInit(numPools, (wsfBufPoolDesc_t *)&gcBuffPoolDesc[0]);

    WsfHeapAlloc(memUsed);
}

/**
 * @brief Get the current Link Layer runtime configuration.
 * @param[out] llRtCfg returned Link Layer runtime configuration.
 */
static void BLE_GetCurrentLlRtConfig(LlRtCfg_t *llRtCfg)
{
    LlGetDefaultRunTimeCfg(llRtCfg);
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_LL_PARAM,
                         &llRtCfg->maxAdvSets,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, maxAdvSets)));

#if BT_VER >= LL_VER_BT_CORE_SPEC_5_0
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BLE_PHY,
                         &llRtCfg->phy2mSup,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, phy2mSup)));
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_CTE,
                         &llRtCfg->cteUnused,
                         (sizeof(LlRtCfg_t) - offsetof(LlRtCfg_t, cteUnused)));
#endif
}

/**
 * @brief Get the current Baseband runtime configuration.
 * @param[out] bbRtCfg returned Baseband runtime configuration.
 */
static inline void BLE_GetCurrentBbRtConfig(BbRtCfg_t *bbRtCfg)
{
    PalBbLoadCfg((PalBbCfg_t*)bbRtCfg);
}

/**
 * @brief Link Layer initialization function.
 * @param[in] llRtCfg Link Layer runtime configuration.
 * @param[in] bbRtCfg Baseband runtime configuration.
 */
static void BLE_InitLl(LlRtCfg_t *llRtCfg, BbRtCfg_t *bbRtCfg)
{
    uint32_t memUsed;

    BLE_GetCurrentBbRtConfig(bbRtCfg);

    BLE_GetCurrentLlRtConfig(llRtCfg);

    // Custom LL configuration.

    llRtCfg->maxConn       = BLE_CONN_MAX;
    llRtCfg->maxAclLen     = ATT_ACCEPTABLE_MTU + L2C_HDR_LEN;
    llRtCfg->numTxBufs     = LL_RT_CFG_NUM_TX_BUFS;
    llRtCfg->numRxBufs     = LL_RT_CFG_NUM_RX_BUFS;
    llRtCfg->maxAdvReports = LL_RT_CFG_MAX_ADV_REPORTS;

    LlInitRtCfg_t llCfg =
    {
        .pBbRtCfg     = bbRtCfg,
        .wlSizeCfg    = 1, // Whitelist size.
        .rlSizeCfg    = 1, // Resolvable list size.
        .plSizeCfg    = 1, // Periodic list size.
        .pLlRtCfg     = llRtCfg,
        .pFreeMem     = WsfHeapGetFreeStartAddress(),
        .freeMemAvail = WsfHeapCountAvailable()
    };

    bdAddr_t bdAddr;

    memUsed = LlInit(&llCfg);
    WsfHeapAlloc(memUsed);

    // Load the Bluetooth address and use it as the seed for the PRAND.
    (void)PalCfgLoadData((uint8_t)PAL_CFG_ID_BD_ADDR, bdAddr, sizeof(bdAddr_t));
    LlSetBdAddr((uint8_t*)bdAddr);
}

/******************************************************************************\
 *  Protected functions definitions
\******************************************************************************/

void BLE_Init(void)
{
    wsfHandlerId_t handlerId;

    PalSysInit();

    // Initialize the Link Layer.
    BLE_InitLl(&gLlRtCfg, &gBbRtCfg);

    // Initialize the buffers.
    BLE_InitBuffer();

    // Initialize OS timer.
    WsfTimerInit();

    // Initialize the stack modules and the handlers.

    // Security Module.
    SecInit();
    SecAesInit();
    SecCmacInit();
    SecEccInit();

    // Host Controller Interface.
    handlerId = WsfOsSetNextHandler(HciHandler);
    HciHandlerInit(handlerId);
    HciSetMaxRxAclLen(WSF_MAX(ATT_ACCEPTABLE_MTU + L2C_HDR_LEN, SMP_PUB_KEY_MSG_LEN + L2C_HDR_LEN));
    // Note, ACL queue flow control is offset by one such that flow control is
    // released on each connection event. If the low watermark is set to 0, flow
    // control does not get released in the same connection event the queue is
    // emptied on.
    HciSetAclQueueWatermarks(NUM_MAX_PACKETS_PER_INTERVAL + 1u, 1u);

    // Device Manager.
    handlerId = WsfOsSetNextHandler(DmHandler);
    DmDevVsInit(0);
    DmConnInit();
    DmAdvInit();
    DmConnSlaveInit();
    DmSecInit();
    DmSecLescInit();
    DmPrivInit();
    DmHandlerInit(handlerId);
    // Register Callbacks for DM.
    DmRegister(BLE_DmCback);
    DmConnRegister(DM_CLIENT_ID_APP, BLE_DmCback);

    // Logical Link Control and Adaptation Layer Protocol (L2CAP).
    handlerId = WsfOsSetNextHandler(L2cSlaveHandler);
    L2cSlaveHandlerInit(handlerId);
    L2cInit();
    L2cSlaveInit();

    // Attribute Protocol.
    handlerId = WsfOsSetNextHandler(AttHandler);
    AttHandlerInit(handlerId);
    pAttCfg = (attCfg_t *)&gcAttCfg;
    AttsInit();
    AttsIndInit();
    // Register Callbacks for ATT.
    AttRegister(BLE_AttCback);
    AttConnRegister(AppServerConnCback);
    AttsCccRegister(BLE_NUM_CCC_IDX, (attsCccSet_t*)gcCccSet, BLE_AttsCccCback);

    // Security Manager.
    handlerId = WsfOsSetNextHandler(SmpHandler);
    SmpHandlerInit(handlerId);
    pSmpCfg = (smpCfg_t*)&gcSmpCfg;
    SmprInit();
    SmprScInit();

    // Application Framework (AF).
    handlerId = WsfOsSetNextHandler(AppHandler);
    AppHandlerInit(handlerId);
    AppSlaveInit();
    AppServerInit();
    pAppAdvCfg = (appAdvCfg_t*)&gcAfAdvCfg;
    pAppSlaveCfg = (appSlaveCfg_t*)&gcAfSlaveCfg;
    pAppSecCfg = (appSecCfg_t*)&gcAfSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t*)&gcAfUpdateCfg;

    // User defined handler for BLE events.
    handlerId = WsfOsSetNextHandler(BLE_SubsysHandler);
    gBLE_SubsysHandlerId = handlerId;

    // Initialize attribute server database and register callbacks.
    SvcCoreGattCbackRegister(GattReadCback, GattWriteCback);
    SvcCoreAddGroup();

    // Add Device Information Service in ATT Database.
    SvcDisAddGroup();

    // Proprietary profile and service.
    EmdtpServer_Init((EmdtpServer_Cfg_t *)&gcEmdtpServerCfg, BLE_EmdtpServerCback);

    // Add proprietary service in ATT Database.
    EmdtSvc_AddGroup();

    // Set Service Changed CCCD index.
    GattSetSvcChangedIdx(BLE_GATT_SC_CCC_IDX);

    // Reset the device.
    DmDevReset();
}
