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
* @addtogroup UTS_TAPI_SIM_SAP_SET_TRANSPORT_PROTOCOL tel_req_sap_transport_protocol
*
* @brief Unit test code for tel_req_sap_transport_protocol()
* @par Test function Prototype:
* 	TapiResult_t tel_req_sap_transport_protocol(TelSimSapProtocol_t  Protocol, int* pRequestId);
* @par Test function description:
* 	This API is used to transfer APDU from SIM using SAP.
* @par Important Notes:
* 	None
* @param [in] Protocol 		SAP transfer protocol type
*	@code
	typedef enum
	{
		TAPI_SIM_SAP_PROTOCOL_T0,  // T = 0
		TAPI_SIM_SAP_PROTOCOL_T1   // T = 1
	}TelSimSapProtocol_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimSapResultCode_t. \n
* 	- TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF
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
* @file		uts_tel_req_sap_transport_protocol.c
* @brief	Test for the tel_req_sap_transport_protocol() API that is used to set transport protocol
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_req_sap_transport_protocol.h"



/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sap_transport_protocol_01, 1},
	{utc_tel_req_sap_transport_protocol_02, 2},
	{utc_tel_req_sap_transport_protocol_03, 3},
	{utc_tel_req_sap_transport_protocol_04, 4},
	{utc_tel_req_sap_transport_protocol_05, 5},
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


static int sim_sap_process_for_condition(int wish_condition)
{
	TelSimSapConnect_t pConnectionReq;
	int pRequestId=0;
	int tapiRet = TAPI_API_SUCCESS;
	
	tet_printf("sap wish condition[%d]:0-disconnect, 1-connect",wish_condition);

	if(wish_condition == 1)
	{
		pConnectionReq.MsgId = TAPI_SIM_SAP_CONNECT_REQ;
	}
	else
	{
		pConnectionReq.MsgId = TAPI_SIM_SAP_DISCONNECT_REQ;
	}

	pConnectionReq.MaxMsgSize = 10; //temp for testing only - not clear
		
	gAsyncResult = FALSE;
		
	tapiRet = tel_req_sap_connection(&pConnectionReq,&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
		
	G_MAIN_LOOP_RUN(2);

	if(gAsyncResult == TRUE)
		return 0;
	else
		return -1;

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
			TelSimSapConnect_t	*sap_conn_resp = (TelSimSapConnect_t*)sim_event->pData;
		
			tet_printf("[SIM APP]SIM SAP connect request event status  = [%x]\n", sim_event->Status);
		//		tet_printf("[SIM APP] message id [%x]\n", sap_conn_resp->MsgId);
		//		tet_printf("[SIM APP]connection status [%x]\n ", sap_conn_resp->ConnectionStatus);
		//		tet_printf("[SIM APP]max message size [%d]\n", sap_conn_resp->MaxMsgSize);
		
			bReceived = TRUE;
			gAsyncResult = TRUE;
		}
		break;

		case TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF:
		{
			tet_printf("\n[GOT THE EVENT]*****TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF******\n");
			TelSimSapProtocol_t *sap_result_code = (TelSimSapProtocol_t*)sim_event->pData;

			tet_printf("[SIM APP]SIM SAP set protocol event status  = [%x]\n", sim_event->Status);
		//	tet_printf("[SIM APP]SAP SET protocol  result [%x]\n", *sap_result_code );
			bReceived = TRUE;
			gAsyncResult = TRUE;
		}
		break;

        default:
        {
		bReceived = TRUE;
		gAsyncResult = FALSE;
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
    tet_infoline("utc_tel_req_sap_transport_protocol Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_SAP_CONNECT_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);			

		gStartupSuccess = TRUE;
	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_req_sap_transport_protocol Test Finished\n");

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
* @brief tel_req_sap_transport_protocol() API : Positive condition(Set protocol TAPI_SIM_SAP_PROTOCOL_T0)
* @par ID: UTC_TEL_REQ_SAP_TRANSPORT_PROTOCOL_01
* @param [in] Protocol = TAPI_SIM_SAP_PROTOCOL_T0
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimSapResultCode_t. \n
* 	- TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF
*/
void utc_tel_req_sap_transport_protocol_01()
{
	TET_STAMP_START("Normal operation : TAPI_SIM_SAP_PROTOCOL_T0");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimSapProtocol_t Protocol=TAPI_SIM_SAP_PROTOCOL_T0;
	int pRequestId=0;
	int tapiRet = TAPI_API_SUCCESS;
#if 0 // we assume sap connect on.

	// Step 1 : make proper condition
    tet_printf("Step 1 : sap connect");

	if( 0 != sim_sap_process_for_condition(1))
	{
		tet_printf("condition set is failed!!");
		return;
	}
#endif
    gAsyncResult = FALSE;

	tapiRet = tel_req_sap_transport_protocol(Protocol,&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);

    G_MAIN_LOOP_RUN(3);

    TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE);
    TET_RESULT_EQUAL_GOTO(TET_PASS,exit,gAsyncResult,TRUE);

exit:
#if 0 // we assume sap connect on.

	// Step 3 : make default condition
    tet_printf("Step 3 : disconnect sap");

	if( 0 != sim_sap_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
	}
#endif	
    gAsyncResult = FALSE;
	return;
}

/**
* @brief tel_req_sap_transport_protocol() API : Positive condition(Set protocol TAPI_SIM_SAP_PROTOCOL_T1)
* @par ID: UTC_TEL_REQ_SAP_TRANSPORT_PROTOCOL_02
* @param [in] Protocol = TAPI_SIM_SAP_PROTOCOL_T1
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimSapResultCode_t. \n
* 	- TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF
*/
void utc_tel_req_sap_transport_protocol_02()
{
	TET_STAMP_START("Normal operation : TAPI_SIM_SAP_PROTOCOL_T1");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimSapProtocol_t Protocol=TAPI_SIM_SAP_PROTOCOL_T1;
	int pRequestId=0;
	int tapiRet = TAPI_API_SUCCESS;
#if 0 // we assume sap connect on.	
	// Step 1 : make proper condition
	tet_printf("Step 1 : sap connect");
	
	if( 0 != sim_sap_process_for_condition(1))
	{
		tet_printf("condition set is failed!!");
		return;
	}
#endif	
	gAsyncResult = FALSE;
	
	tapiRet = tel_req_sap_transport_protocol(Protocol,&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
	
	G_MAIN_LOOP_RUN(2);
	
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE);
	TET_RESULT_EQUAL_GOTO(TET_PASS,exit,gAsyncResult,TRUE);
	
exit:
#if 0 // we assume sap connect on.
	// Step 3 : make default condition
	tet_printf("Step 3 : disconnect sap");
	
	if( 0 != sim_sap_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
	}
#endif	
	gAsyncResult = FALSE;
	return;
}


/**
* @brief tel_req_sap_transport_protocol() API : Negative condition(Low boundary test)
* @par ID: UTC_TEL_REQ_SAP_TRANSPORT_PROTOCOL_03
* @param [in] Protocol = TAPI_SIM_SAP_PROTOCOL_T0-1;// Low boundary
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_req_sap_transport_protocol_03()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT");

	TelSimSapProtocol_t Protocol=(TelSimSapProtocol_t)TAPI_SIM_SAP_PROTOCOL_T0-1;
	int pRequestId=0;

	TapiResult_t error = tel_req_sap_transport_protocol(Protocol,&pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, error);
	TET_STAMP_PASS();
}

/**
* @brief tel_req_sap_transport_protocol() API : Negative condition(Upper boundary test)
* @par ID: UTC_TEL_REQ_SAP_TRANSPORT_PROTOCOL_04
* @param [in] Protocol = TAPI_SIM_SAP_PROTOCOL_T1+1;// Upper boundary
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_req_sap_transport_protocol_04()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT");

	TelSimSapProtocol_t Protocol=(TelSimSapProtocol_t)TAPI_SIM_SAP_PROTOCOL_T1+1;
	int pRequestId=0;

	TapiResult_t error = tel_req_sap_transport_protocol(Protocol,&pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, error);
	TET_STAMP_PASS();
}

/**
* @brief tel_req_sap_transport_protocol() API : Negative condition(Null parameter)
* @par ID: UTC_TEL_REQ_SAP_TRANSPORT_PROTOCOL_04
* @param [in] Protocol = TAPI_SIM_SAP_PROTOCOL_T1
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_req_sap_transport_protocol_05()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR");

	TelSimSapProtocol_t Protocol= TAPI_SIM_SAP_PROTOCOL_T1;
	TapiResult_t error = tel_req_sap_transport_protocol(Protocol,NULL);
	TET_CHECK(TAPI_API_INVALID_PTR, error);
	TET_STAMP_PASS();
}

/** @} */
