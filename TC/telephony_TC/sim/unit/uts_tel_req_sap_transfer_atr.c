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
* @addtogroup UTS_TAPI_SIM_SAP_TRANSFER_ATR_REQUEST tel_req_sap_transfer_atr
*
* @brief Unit test code for tel_req_sap_transfer_atr()
* @par Test function Prototype:
* 	TapiResult_t tel_req_sap_transfer_atr(int* pRequestId);
* @par Test function description:
* 	This API is used to request for an external SAP connection.
* @par Important Notes:
* 	None
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimSapAtrInfo_t. \n
* 	- TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF
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
* @file		uts_tel_req_sap_transfer_atr_request.c
* @brief	Test for the tel_req_sap_transfer_atr() API that is used to transfer atr request
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_req_sap_transfer_atr.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sap_transfer_atr_01, 1},
	{utc_tel_req_sap_transfer_atr_02, 2},
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
		
	G_MAIN_LOOP_RUN(5);

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

	case TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF:
	{
		tet_printf("[SIM APP]*********** SAP TRANSFER ATR *************\n");

		int i = 0;
		TelSimSapAtrInfo_t *sap_transfer_atr = (TelSimSapAtrInfo_t*)sim_event->pData;

		tet_printf("[SIM APP]SIM SAP trasfer ATR event status  = [%x]\n", sim_event->Status);

		tet_printf(" [SIM APP]ATR result [%x]\n", sap_transfer_atr->AtrResult);
		tet_printf(" [SIM APP]ATR length [%x]\n", sap_transfer_atr->AtrLength);
		tet_printf(" [SIM APP]ATR data ---> \n" );

//		for(i =0 ; i<sap_transfer_atr->AtrLength; i++ )
//			tet_printf(" [SIM APP][%x]", sap_transfer_atr->AtrData[i]);

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
    tet_infoline("utc_tel_req_sap_transfer_atr Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_SAP_CONNECT_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL); 		
			
		gStartupSuccess = TRUE;
	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");

    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_req_sap_transfer_atr Test Finished\n");
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
* @brief tel_req_sap_transfer_atr() API : Positive condition(Request to transfer Atr)
* @par ID: UTC_TEL_REQ_SAP_TRANSFER_ATR_01
* @param [out]&pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimSapAtrInfo_t. \n
* 	- TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF
*/
void utc_tel_req_sap_transfer_atr_01()
{
	TET_STAMP_START("Normal operation");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int pRequestId=0;
	int tapiRet = TAPI_API_SUCCESS;

	// Step 1 : make proper condition
    tet_printf("Step 1 : sap connect");

	if( 0 != sim_sap_process_for_condition(1))
	{
		tet_printf("condition set is failed!!");
		return;
	}


    tet_printf("Step 2 : sap atr");

    gAsyncResult = FALSE;

	tapiRet = tel_req_sap_transfer_atr(&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
	
    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(3);

    tet_printf("after real tc api - wait stop");

	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE);
	TET_RESULT_EQUAL_GOTO(TET_PASS,exit,gAsyncResult,TRUE);
	
exit:
#if 0 // we use connection status until finishing sap related api test.
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
* @brief tel_req_sap_transfer_atr() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_REQ_SAP_TRANSFER_ATR_02
* @param [out]&pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_transfer_atr_02()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR 1");

	TapiResult_t error = tel_req_sap_transfer_atr(NULL);
	TET_CHECK(TAPI_API_INVALID_PTR, error);
	TET_STAMP_PASS();
}

/** @} */
