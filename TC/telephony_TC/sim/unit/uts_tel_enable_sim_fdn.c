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
* @addtogroup UTS_TAPI_SIM_ENABLE_FDN tel_enable_sim_fdn
*
* @brief Unit test code for tel_enable_sim_fdn()
* @par Test function Prototype:
* 	TapiResult_t tel_enable_sim_fdn(const char* pPinData, int *pRequestId);
* @par Test function description:
* 	This function enables the FDN Mode
* @par Important Notes:
* 	An unsolicited event TAPI_EVENT_SIM_FDN_STATUS_IND is notified to indicate
* 	whenever FDN status is changed(disabled/enabled)
* @param [in] pPinData
* 	User entered PIN2 data. (NULL terminated string)
* @param [out] pRequestId
*   - Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure. * @par Async Response Message:
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_DISABLE_FDN_CNF
*	- TAPI_EVENT_SIM_ENABLE_FDN_CNF
* 	- TAPI_EVENT_SIM_STATUS_IND
* @pre
*	- Before test script compile, you set correct PIN1_NORMAL, PIN2_NORMAL, PIN1_CHANGED, PIN2_CHANGED value on 'tet_tapi_util.h'
*	- Before test script compile, you set incorrect PIN1_INCORRECT value on 'tet_tapi_util.h'
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_enable_sim_fdn.c
* @brief	Test for the tel_enable_sim_fdn() API that is used to enable FDN mode
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/


#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_enable_sim_fdn.h"
#include "sim_common.h"
#include "async_util.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_enable_sim_fdn_01, 1},
	{utc_tel_enable_sim_fdn_02, 2},
	{utc_tel_enable_sim_fdn_03, 3},
	{utc_tel_enable_sim_fdn_04, 4},

	{NULL, 0}
};

/*
  User Defined Handler
*/
// Global variable for user data
static BOOL gAsyncResult; //user defined data : Async callback is called or not
static TelSimPinOperationResult_t gSimEventStatus;

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

extern sim_info_t g_si;


/*
  G_MAIN Utility ////////////////////////////////////////////////////////////////////////////
*/
// Prototype : User must add function body
gboolean g_timeout_callback (void *data);

// GMainLoop global handler
GMainLoop *sim_loop = NULL;

void G_MAIN_LOOP_CLOSE()
{
    g_main_loop_quit(sim_loop);
}

int G_MAIN_LOOP_RUN(int sec)
{
	//GMainLoop *mainloop;
	sim_loop = g_main_loop_new (NULL, FALSE);
	g_timeout_add ( (sec * 1000), g_timeout_callback, NULL);
	g_main_loop_run (sim_loop);

	return 0;
}

gboolean g_timeout_callback (void *data)
{
	gAsyncResult = FALSE;
	tet_printf("[TIME OUT] g_timeout called .Close g_main_roop()");
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}
// End of  G_MAIN Utility//////////////////////////////////////////////////////////////////////


static int sim_fdn_process_for_condition(int wish_condition)
{
    int pRequestId=0;
	int tapiRet = TAPI_API_SUCCESS;
	int fdn_status = 0; //dis(0)/enable(1)
	int pin2_len = 0;
	char* pin2 = NULL;
	
	tet_printf("wish_condition[%d]: 0-dis, 1- enabled",wish_condition);

	tapiRet = tel_get_sim_fdn_status(&fdn_status);
	if(tapiRet != TAPI_API_SUCCESS)
	{
		tet_printf("tel_get_sim_fdn_status is Failed with tapiRet[%d]", tapiRet);
		return -1;
	}

	if(fdn_status == 0 )
	{
		tet_printf("current fdn status is disable");
	}
	else if(fdn_status == 1)
	{
		tet_printf("current fdn status is enable");
	}
	else
	{
		tet_printf("current fdn status is unhandled status[%d]",fdn_status );
		return -1;
	}

	if(fdn_status == wish_condition )
	{
		tet_printf("current fdn status is same with wished condition");
		return 0;
	}
	else
	{
		tet_printf("current fdn status is different with wished condition");

		pin2_len = strlen(g_si.pin2);
		pin2 = (char*)malloc(pin2_len);
		memcpy(pin2,g_si.pin2,pin2_len);

		gAsyncResult = FALSE;
			
		if(wish_condition == 0)
		{
			tapiRet = tel_disable_sim_fdn((const unsigned char*)pin2,&pin2_len,&pRequestId);
		}
		else if(wish_condition == 1)
		{
			tapiRet = tel_enable_sim_fdn((const unsigned char*)pin2,&pin2_len,&pRequestId);
		}
		else
		{
			tet_printf("Unhandled wish_condition[%d]", wish_condition);
			return -1;
		}
			
		if(tapiRet != TAPI_API_SUCCESS)
		{
			tet_printf("tapi return[%d] when en/disabling fdn for condition",tapiRet);
			return -1;
		}
		else
		{
			G_MAIN_LOOP_RUN(2);
		}

		if(gAsyncResult == TRUE)
			return 0;
		else
			return -1;
	}
	return 0;
}


static int  sim_app_callback  (TelTapiEvent_t *sim_event)
{
    tet_printf("\n***************[SIM APP]Callback data **********************\n");
    tet_printf("[SIM]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    BOOL bReceived = FALSE;

    switch(sim_event->EventType)
      {
        case TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF:
        {
            tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF (%d) ***************************\n", TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF);
            tet_printf("SIM Enable FDN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

        case TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF:
        {
            tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF (%d) ***************************\n", TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF);
            tet_printf("SIM Disable FDN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

        default:
        {
            gSimEventStatus = sim_event->Status;
            gAsyncResult = FALSE;
            tet_printf("[SIM]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
            tet_printf("[SIM APP]Undhandled event state [%d]\n",sim_event->Status);
            break;
	}
    }//end switch

    if (bReceived == TRUE)
      G_MAIN_LOOP_CLOSE();
    return 0;
}


static void startup()
{
    tet_infoline("utc_tel_enable_sim_fdn Test Start\n");

	sim_pin_info_setup();

	if (tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		gStartupSuccess = TRUE;

	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_enable_sim_fdn Test Finished\n");

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
* @brief tel_enable_sim_fdn() API : Negative condition(First parameter is Null)
* @par ID: UTC_TEL_ENABLE_SIM_FDN_01
* @param [in] *pPinData = NULL
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_enable_sim_fdn_01()
{
	TET_STAMP_START("NULL parameter test 1");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);
	
	int pRequestId=0;
	int len = 0;
	
	TET_CHECK(TAPI_API_INVALID_PTR, tel_enable_sim_fdn(NULL,&len, &pRequestId));
	TET_STAMP_PASS();
	return;
}
	
/**
* @brief tel_enable_sim_fdn() API : Negative condition(Second parameter is Null)
* @par ID: UTC_TEL_ENABLE_SIM_FDN_02
* @param [in] *pPinData = PIN1_NORMAL
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_enable_sim_fdn_02()
{
	TET_STAMP_START("NULL parameter test 2");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);
	
	int pin2_len = strlen(g_si.pin2);
	char* pPinData = (char*)malloc(pin2_len);
	memcpy(pPinData, g_si.pin2, pin2_len);

	TET_CHECK(TAPI_API_INVALID_PTR, tel_enable_sim_fdn((const unsigned char*)pPinData, &pin2_len, NULL));
	TET_STAMP_PASS();
	return;
}

	
void utc_tel_enable_sim_fdn_03()
{
	TET_STAMP_START("PIN2_INCORRECT test");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);
	
	int tapiRet = TAPI_API_SUCCESS;
	int pRequestId = 0;
	int pin2_len = 0;
	char* pPinData = NULL;
		
	tet_printf("Step 1 : disable fdn");
	if( 0 != sim_fdn_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
		return;
	}
	
	tet_printf("Step 2 : enable fdn with incorrect pin2");
	pPinData = (char*)malloc(pin2_len);
	pin2_len = strlen(g_si.wpin2);
	memcpy(pPinData, g_si.wpin2, pin2_len);
	
	gAsyncResult = FALSE;
	
	tapiRet = tel_enable_sim_fdn((const unsigned char*)pPinData, &pin2_len, &pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
	
	tet_printf("after real tc api - wait start");
	
	G_MAIN_LOOP_RUN(2);
	
	tet_printf("after real tc api - wait stop");

	
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE); // If gAsyncResult != FALSE, then TET_FAIL and return
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,TAPI_SIM_PIN_INCORRECT_PASSWORD, gSimEventStatus); // If gSimEventStatus != TAPI_SIM_PIN_INCORRECT_PASSWORD, then TET_FAIL and return
	TET_STAMP_PASS();
		
exit:
	// Step 3 : make default condition
	tet_printf("Step 3 : disable fdn");
		
	if( 0 != sim_fdn_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
	}
	return;
}

	
void utc_tel_enable_sim_fdn_04()
{
	TET_STAMP_START("PIN2_CORRECT test");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);
	
	int tapiRet = TAPI_API_SUCCESS;
	int pRequestId = 0;
	int pin2_len = 0;
	char* pPinData = NULL;
		
	tet_printf("Step 1 : disable fdn");
	if( 0 != sim_fdn_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
		return;
	}
	
	tet_printf("Step 2 : enable fdn with correct pin2");
	pPinData = (char*)malloc(pin2_len);
	pin2_len = strlen(g_si.pin2);
	memcpy(pPinData, g_si.pin2, pin2_len);

	gAsyncResult = FALSE;
	
	tapiRet = tel_enable_sim_fdn((const unsigned char*)pPinData, &pin2_len, &pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);
	
	tet_printf("after real tc api - wait start");
	
	G_MAIN_LOOP_RUN(2);
	
	tet_printf("after real tc api - wait stop");

	
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE); // If gAsyncResult != FALSE, then TET_FAIL and return
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,TAPI_SIM_PIN_OPERATION_SUCCESS, gSimEventStatus); // If gSimEventStatus != TAPI_SIM_PIN_INCORRECT_PASSWORD, then TET_FAIL and return
	TET_STAMP_PASS();
		
exit:
	// Step 3 : make default condition
	tet_printf("Step 3 : disable fdn");
		
	if( 0 != sim_fdn_process_for_condition(0))
	{
		tet_printf("condition set is failed!!");
	}
	return;
}

/** @} */
