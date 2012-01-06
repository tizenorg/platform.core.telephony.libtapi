/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
* @ingroup TELEPHONY_API
* @addtogroup TAPI_API_SIM_SAT SIM/SAT
*/

/**
* @ingroup  TAPI_API_SIM_SAT
* @addtogroup UTS_SIM_SAT Unit
*/

/**
* @ingroup  UTS_SIM_SAT
* @addtogroup UTS_TAPI_SIM_SAP_CONNECT_REQUEST tel_req_sap_connection
*
* @brief Unit test code for tel_req_sap_connection()
* @par Test function Prototype:
* 	TapiResult_t tel_req_sap_connection( TelSimSapConnect_t *pConnectionReq, int* pRequestId);
* @par Test function description:
* 	This API is used to request for an external SAP connection
* @par Important Notes:
* 	None
* @param [in] pConnectionReq    This enum gives the SAP ATR response data information
*	@code
	typedef struct
	{
		TelSimSapMsgId_t	MsgId;	// connect or disconnect request /response
		TelSimSapConnectionStatus_t	ConnectionStatus;	// used only in response from tapi
		unsigned short	MaxMsgSize;	// request from app or set by lower layer
	}TelSimSapConnect_t;

	 // This enum gives the SAP message Ids between SAP client and SAP server.
	typedef enum
	{
		TAPI_SIM_SAP_CONNECT_REQ        = 0x00,
		TAPI_SIM_SAP_CONNECT_RESP       = 0x01,
		TAPI_SIM_SAP_DISCONNECT_REQ     = 0x02,
		TAPI_SIM_SAP_DISCONNECT_RESP    = 0x03,
		TAPI_SIM_SAP_DISCONNECT_IND     = 0x04,
		TAPI_SIM_SAP_TRANSFER_APDU_REQ  = 0x05,
		TAPI_SIM_SAP_TRANSFER_APDU_RESP = 0x06,
		TAPI_SIM_SAP_TRANSFER_ATR_REQ   = 0x07,
		TAPI_SIM_SAP_TRANSFER_ATR_RESP  = 0x08,
		TAPI_SIM_SAP_POWER_SIM_OFF_REQ  = 0x09,
		TAPI_SIM_SAP_POWER_SIM_OFF_RESP = 0x0A,
		TAPI_SIM_SAP_POWER_SIM_ON_REQ   = 0x0B,
		TAPI_SIM_SAP_POWER_SIM_ON_RESP  = 0x0C,
		TAPI_SIM_SAP_RESET_SIM_REQ      = 0x0D,
		TAPI_SIM_SAP_RESET_SIM_RESP     = 0x0E,
		TAPI_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ  = 0x0F,
		TAPI_SIM_SAP_TRANSFER_CARD_READER_STATUS_RESP = 0x10,
		TAPI_SIM_SAP_STATUS_IND         = 0x11,
		TAPI_SIM_SAP_ERROR_RESP         = 0x12,
		TAPI_SIM_SAP_SET_TRANSPORT_PROTOCOL_REQ = 0x13,
		TAPI_SIM_SAP_SET_TRANSPORT_PROTOCOL_RESP = 0x14
	}TelSimSapMsgId_t

	// This enum gives the SAP connection status information. This is used for Async Response Message
	typedef enum
	{
		TAPI_SIM_SAP_CONNECTION_STATUS_OK = 0x00,
		TAPI_SIM_SAP_CONNECTION_STATUS_UNABLE_TO_ESTABLISH ,
		TAPI_SIM_SAP_CONNECTION_STATUS_NOT_SUPPORT_MAX_SIZE,
		TAPI_SIM_SAP_CONNECTION_STATUS_TOO_SMALL_MAX_SIZE
	}TelSimSapConnectionStatus_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* 	The event associated is below and the event data is #TelSimSapConnect_t.
* 	- TAPI_EVENT_SIM_SAP_CONNECT_CNF
* @pre
*	SAP supported sim used to testing
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_req_sap_connection.c
* @brief	Test for the tel_req_sap_connection() API that is used to sap connect request
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_req_sap_connection.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sap_connection_01, 1},
	{utc_tel_req_sap_connection_02, 2},
	{NULL, 0}
};


/*
  G_MAIN Utility ////////////////////////////////////////////////////////////////////////////
*/
// Prototype : User must add function body
gboolean g_timeout_callback (void *data);

// GMainLoop global handler
GMainLoop *nw_context_loop = NULL;

void G_MAIN_LOOP_CLOSE()
{
    g_main_loop_quit(nw_context_loop);
}

int G_MAIN_LOOP_RUN(int sec)
{
	//GMainLoop *mainloop;
	nw_context_loop = g_main_loop_new (NULL, FALSE);
	g_timeout_add ( (sec * 1000), g_timeout_callback, NULL);
	g_main_loop_run (nw_context_loop);

	return 0;
}
// End of  G_MAIN Utility//////////////////////////////////////////////////////////////////////


/*
  User Defined Handler
*/
// Global variable for user data
static BOOL gAsyncResult; //user defined data : Async callback is called or not

// G_MAIN Utility : time out callback function
gboolean g_timeout_callback (void *data)
{
	gAsyncResult = FALSE;
	tet_printf("[SIM APP][TIME OUT] g_timeout called .Close g_main_roop()");
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}

static int  sim_app_callback  (TelTapiEvent_t *sim_event)
{
    tet_printf("\n\n\n***************[SIM APP]Callback data **********************\n");
    tet_printf("[SIM APP]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM APP]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    BOOL bReceived = FALSE;

    switch(sim_event->EventType)
    {
 	case TAPI_EVENT_SIM_SAP_CONNECT_CNF:
	{
		tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_SAP_CONNECT_CNF:******\n");
		TelSimSapConnect_t  *sap_conn_resp = (TelSimSapConnect_t*)sim_event->pData;

		tet_printf("[SIM APP]SIM SAP connect request event status  = [%x]\n", sim_event->Status);
//		tet_printf("[SIM APP] message id [%x]\n", sap_conn_resp->MsgId);
//		tet_printf("[SIM APP]connection status [%x]\n ", sap_conn_resp->ConnectionStatus);
//		tet_printf("[SIM APP]max message size [%d]\n", sap_conn_resp->MaxMsgSize);

		bReceived = TRUE;
		gAsyncResult = TRUE;
	}
	break;

	case TAPI_EVENT_SIM_SAP_CONNECT_STATUS_CNF:
	{
		tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_SAP_CONNECT_STAT_CNF:******\n");
		tet_printf("[SIM APP]SIM SAP connect event status = [%x]\n", sim_event->Status);

		TelSimSapStatusInfo_t *sap_conn_status = (TelSimSapStatusInfo_t*)sim_event->pData;
		tet_printf("[SIM APP] connection status info [%x]\n", *sap_conn_status);
		bReceived = TRUE;
		gAsyncResult = FALSE;
	}
	break;

        default:
        {
		bReceived = TRUE;
		gAsyncResult = FALSE;
		tet_printf("Info : TAPI_EVENT_SIM_SAP_CONNECT_CNF = %d", TAPI_EVENT_SIM_SAP_CONNECT_CNF);
		tet_printf("[SIM APP]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
		tet_printf("[SIM APP]Undhandled event state [%d]\n",sim_event->Status);
	}
	break;
    }//end switch

    if (bReceived == TRUE)
      G_MAIN_LOOP_CLOSE();

    return 0;
}



/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

static void startup()
{
	tet_infoline("utc_tel_req_sap_connection Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_SAP_CONNECT_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
			gStartupSuccess = TRUE;
		else
			tet_infoline("startup fail. tel_register_event() failed");
	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");
	return;
}


static void cleanup()
{
    tet_infoline("utc_tel_req_sap_connection Test Finished\n");

    if (gStartupSuccess == TRUE)
	{
		tel_deregister_event(subscription_id);
	}
	tel_deinit();

    return;
}

/*
  Unit Test Case functions
*/

/**
* @brief tel_req_sap_connection() API : Negative condition(Second Parameter is Null)
* @par ID: UTC_TEL_REQ_SAP_CONNECTION_01
* @param [in] &pConnectionReq
* @param [out]&sim_data = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_connection_01()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR 1");

	TelSimSapConnect_t pConnectionReq;
	pConnectionReq.MsgId = TAPI_SIM_SAP_CONNECT_REQ;
	pConnectionReq.MaxMsgSize = 10; //temp for testing only - not clear

    TET_CHECK(TAPI_API_INVALID_PTR,tel_req_sap_connection(&pConnectionReq,NULL));
    TET_STAMP_PASS();
    return;
}


/**
* @brief tel_req_sap_connection() API : Negative condition(First Parameter is Null)
* @par ID: UTC_TEL_REQ_SAP_CONNECTION_02
* @param [in] &pConnectionReq = NULL
* @param [out]&sim_data
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_connection_02()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR 2");

	int pRequestId=0;
	TET_CHECK(TAPI_API_INVALID_PTR,tel_req_sap_connection(NULL,&pRequestId));
	TET_STAMP_PASS();
	return;
}

/** @} */
