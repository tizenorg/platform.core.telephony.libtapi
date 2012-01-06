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
* @addtogroup UTS_TAPI_SIM_PSAP_TRANSFER_APDU tel_req_sap_transfer_apdu
*
* @brief Unit test code for tel_req_sap_transfer_apdu()
* @par Test function Prototype:
* 	TapiResult_t tel_req_sap_transfer_apdu(TelSimSapApduData_t *pApduData,  int* pRequestId);
* @par Test function description:
* 	This API is used to transfer APDU from SIM using SAP.
* @par Important Notes:
* 	None
* @param [in] pApduData    SAP APDU data
*	@code
	typedef struct
	{
		unsigned char   ApduLength;
		unsigned char   Apdu[TAPI_SIM_APDU_MAX_LEN];
	}TelSimSapApduData_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimSapApduData_t. \n
* 	- TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF
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
* @file		uts_tel_req_sap_transfer_apdu.c
* @brief	Test for the tel_req_sap_transfer_apdu() API that is used to transfer APDU
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_req_sap_transfer_apdu.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sap_transfer_apdu_01, 1},
	{utc_tel_req_sap_transfer_apdu_02, 2},
	{utc_tel_req_sap_transfer_apdu_03, 3},
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

    	case TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF:
    	{
		tet_printf("\n[GOT THE EVENT]*****TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF******\n");

		int i = 0;
		TelSimSapApduData_t *sap_transfer_apdu = (TelSimSapApduData_t*)sim_event->pData;

		tet_printf("[SIM APP]SIM SAP trasfer APDU event status  = [%x]\n", sim_event->Status);
	//	tet_printf(" [SIM APP]APDU  length [%x]\n", sap_transfer_apdu->ApduLength);
	//	tet_printf(" [SIM APP]APDU  data ---> \n" );
	//	for(i =0 ; i<sap_transfer_apdu->ApduLength;  i++ )
	//		tet_printf(" [%x]", sap_transfer_apdu->Apdu[i]);

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

		for(i =0 ; i<sap_transfer_atr->AtrLength; i++ )
			tet_printf(" [SIM APP][%x]", sap_transfer_atr->AtrData[i]);

		bReceived = TRUE;
		gAsyncResult = TRUE;
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
    tet_infoline("utc_tel_req_sap_transfer_apdu Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_SAP_CONNECT_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);

		gStartupSuccess = TRUE;
	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_req_sap_transfer_apdu Test Finished\n");

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
* @brief tel_req_sap_transfer_apdu() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_REQ_SAP_TRANSFER_APDU_01
* @param [in] pApduData
* @param [out]&sim_data = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_transfer_apdu_01()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int file_id = TAPI_SIM_EFILE_ICCID;

	/* for selecting EF ICCID */
	TelSimSapApduData_t  apdu_data;
	int pRequestId=0;
	apdu_data.ApduLength = 7;
	apdu_data.Apdu[0] = 0xA0; // class
	apdu_data.Apdu[1] = 0xA4; // instruction
	apdu_data.Apdu[2] = 0; // p1 = parameter 1
	apdu_data.Apdu[3] = 0; // p2 = parameter 2
	apdu_data.Apdu[4] = 2; // p3 - parameter 3

	memcpy(&apdu_data.Apdu[5], &file_id, sizeof(unsigned short));

	TapiResult_t error = tel_req_sap_transfer_apdu(&apdu_data,NULL);
	TET_CHECK(TAPI_API_INVALID_PTR, error);
	TET_STAMP_PASS();
	
}

/**
* @brief tel_req_sap_transfer_apdu() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_REQ_SAP_TRANSFER_APDU_02
* @param [in] pApduData = NULL
* @param [out]&sim_data
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_transfer_apdu_02()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;
	TapiResult_t error = tel_req_sap_transfer_apdu(NULL,&pRequestId);
	TET_CHECK(TAPI_API_INVALID_PTR, error);
	TET_STAMP_PASS();
}

/**
* @brief tel_req_sap_transfer_apdu() API : Positive condition(Request to transfer Apdu)
* @par ID: UTC_TEL_REQ_SAP_TRANSFER_APDU_03
* @param [in] pApduData
*	@code
	int file_id = TAPI_SIM_EFILE_ICCID;

	// for selecting EF ICCID
	TelSimSapApduData_t  apdu_data;
	int pRequestId=0;
	apdu_data.ApduLength = 7;
	apdu_data.Apdu[0] = 0xA0; // class
	apdu_data.Apdu[1] = 0xA4; // instruction
	apdu_data.Apdu[2] = 0;    // p1 = parameter 1
	apdu_data.Apdu[3] = 0;    // p2 = parameter 2
	apdu_data.Apdu[4] = 2;    // p3 - parameter 3
	memcpy(&apdu_data.Apdu[5], &file_id, sizeof(unsigned short));
*	@endcode
* @param [out]&sim_data = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_transfer_apdu_03()
{
	TET_STAMP_START("Normal operation ");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int file_id = TAPI_SIM_EFILE_ICCID;
	int tapiRet = TAPI_API_SUCCESS;
	int pRequestId=0;
	
	/* for selecting EF ICCID */
	TelSimSapApduData_t  apdu_data;

	apdu_data.ApduLength = 7;
	apdu_data.Apdu[0] = 0xA0; // class
	apdu_data.Apdu[1] = 0xA4; // instruction
	apdu_data.Apdu[2] = 0; // p1 = parameter 1
	apdu_data.Apdu[3] = 0; // p2 = parameter 2
	apdu_data.Apdu[4] = 2; // p3 - parameter 3

	memcpy(&apdu_data.Apdu[5], &file_id, sizeof(unsigned short));
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
	
	tapiRet = tel_req_sap_transfer_apdu(&apdu_data,&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
		
	G_MAIN_LOOP_RUN(3);
		
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

/** @} */
