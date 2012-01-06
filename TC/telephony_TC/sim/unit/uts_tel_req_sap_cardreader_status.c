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
* @addtogroup UTS_TAPI_SIM_SAP_CARD_READER_STATUS tel_req_sap_cardreader_status
*
* @brief Unit test code for tel_req_sap_cardreader_status()
* @par Test function Prototype:
* 	TapiResult_t tel_req_sap_cardreader_status( int* pRequestId);
* @par Test function description:
* 	This API is used to get the card reader status.
* @par Important Notes:
* 	None
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimCardReaderStatus_t. \n
* 	- TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF
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
* @file		uts_tel_req_sap_cardreader_status.c
* @brief	Test for the tel_req_sap_cardreader_status() API that is used to get SAP card reader status
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_req_sap_cardreader_status.h"
#include "sim_common.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sap_cardreader_status_01, 1},
	{utc_tel_req_sap_cardreader_status_02, 2},
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
    	case TAPI_EVENT_SIM_SAP_CONNECT_NOTI:
    	{
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_SAP_CONNECT_NOTI***************************\n");
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;


    	case TAPI_EVENT_SIM_SAP_CONNECT_CNF:
    	{
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_SAP_CONNECT_CNF***************************\n");
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

    	case TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF:
    	{
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF***************************\n");
            TelSimCardReaderStatus_t *sap_card_reader_status = (TelSimCardReaderStatus_t*)sim_event->pData;

            tet_printf(" [SIM APP]card reader result [%x]\n ", sap_card_reader_status->CardReaderResult);
            tet_printf(" [SIM APP]card reader status bitwise encoded data [%d]\n", sap_card_reader_status->CardReaderStatus);

            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

        default:
        {
            bReceived = TRUE;
            gAsyncResult = FALSE;
            //tet_printf("Info : TAPI_EVENT_SIM_READ_FILE_CNF = %d", TAPI_EVENT_SIM_READ_FILE_CNF);
            tet_printf("[SIM APP]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
            tet_printf("[SIM APP]Undhandled event state [%d]\n",sim_event->Status);
            break;
	}
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
    tet_infoline("utc_tel_req_sap_cardreader_status Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_SAP_CONNECT_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);		

		gStartupSuccess = TRUE;
	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_req_sap_cardreader_status Test Finished\n");
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
* @brief tel_req_sap_cardreader_status() API : Normal condition(Request card reader status)
* @par ID: UTC_TEL_REQ_SAP_CARDREADER_STATUS_01
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimCardReaderStatus_t. \n
* 	- TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF
*/
void utc_tel_req_sap_cardreader_status_01()
{
    tet_infoline("TRANSFER SAP READER STATUS\n");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

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
		
	tapiRet = tel_req_sap_cardreader_status(&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
			
	tet_printf("after real tc api - wait start");
			
	G_MAIN_LOOP_RUN(3);
			
	tet_printf("after real tc api - wait stop");
			
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE);
	TET_RESULT_EQUAL_GOTO(TET_PASS,exit,gAsyncResult,TRUE);
			
exit:
	// Step 3 : make default condition
#if 0 // we assume sap connect on.

	tet_printf("Step 3 : disconnect sap");
			
	if( 0 != sim_sap_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
	}
#endif	
	return;
}


/**
* @brief tel_req_sap_cardreader_status() API : Negative condition(NULL Parameter test)
* @par ID: UTC_TEL_REQ_SAP_CARDREADER_STATUS_02
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sap_cardreader_status_02()
{
    TET_STAMP_START("TRANSFER SAP READER STATUS WITH INVALID SIM SESSION\n");
    TapiResult_t error = tel_req_sap_cardreader_status(NULL);
    TET_CHECK(TAPI_API_INVALID_PTR,error);
    TET_STAMP_PASS();

	tel_reset_modem();

    return;
}

/** @} */
