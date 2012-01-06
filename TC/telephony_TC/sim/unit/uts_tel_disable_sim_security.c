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
* @addtogroup UTS_TAPI_SIM_DISABLE_PIN tel_disable_sim_security
*
* @brief Unit test code for tel_disable_sim_security()
* @par Test function Prototype:
* 	TapiResult_t tel_disable_sim_security	(const TelSimPinData_t* pPinData, int *pRequestId);
* @par Test function description:
* 	This function disables the PIN based on the Pin type and pin data passed from Application.
* @par Important Notes:
*   For PIN2, disabling applies only for 3G USIMs and when PIN2 in NOT mapped to CHV2 of GSM application.
*	Based on Pin status (PIN/UPIN/PUK) is blocked, an unsolicited event TAPI_EVENT_SIM_STATUS_IND
*	is published to Applications.
* @param [in] pPinData
* 	#TelSimPinData_t type of user entered Pin disable data and pin type
* @code
typedef struct
{
    TelSimPinType_t	PinType;	// Pin type
    char		szPinCode[TAPI_SIM_PIN_LENGTH+1]; //PIN code
} TelSimPinData_t;

typedef enum
{
    TAPI_SIM_PTYPE_PIN1	= 0x00,		// PIN 1
    TAPI_SIM_PTYPE_PIN2	= 0x01,		// PIN 2
    TAPI_SIM_PTYPE_PUK1	= 0x02,		// Puk 1
    TAPI_SIM_PTYPE_PUK2	= 0x03,		// Puk 2
    TAPI_SIM_PTYPE_UPIN	= 0x04,		// Universal PIN
    TAPI_SIM_PTYPE_ADM	= 0x05		// Administrator
} TelSimPinType_t;
* @endcode
* @param [out] pRequestId
*   - Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_DISABLE_PIN_CNF
*	- TAPI_EVENT_SIM_ENABLE_PIN_CNF
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
* @file		uts_tel_disable_sim_security.c
* @brief	Test for the tel_disable_sim_security() API that is used to disable PIN
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_disable_sim_security.h"
#include "sim_common.h"
#include "async_util.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_disable_sim_security_01, 1},
	{utc_tel_disable_sim_security_02, 2},
	{utc_tel_disable_sim_security_03, 3},
	{utc_tel_disable_sim_security_04, 4},
	{utc_tel_disable_sim_security_05, 5},
	{utc_tel_disable_sim_security_06, 6},
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


static int sim_pin_process_for_condition(TelSimPinType_t wish_pin, TelSimPinStatus_t wish_condition)
{
    int pRequestId=0;
	int tapiRet = TAPI_API_SUCCESS;
	TelSimPinStatus_t pin_status = TAPI_SIM_PIN_STATUS_UNKNOWN;
	TelSimSecPw_t sec_data;
	memset(&sec_data, 0x00, sizeof(TelSimSecPw_t));
	
	tet_printf("pin[%d]:0-pin1, 1-pin2, wish_condition[%d]: 0-dis, 1- enabled, 2-blocked(puk)",wish_pin, wish_condition);

	tapiRet = tel_get_sim_security_status(wish_pin,&pin_status);
	if(tapiRet != TAPI_API_SUCCESS)
	{
		tet_printf("tel_get_sim_security_status is Failed with tapiRet[%d]", tapiRet);
		return -1;
	}

	if(pin_status == TAPI_SIM_PIN_STATUS_DISABLED )
	{
		tet_printf("current pin status is TAPI_SIM_PIN_STATUS_DISABLED");
	}
	else if(pin_status == TAPI_SIM_PIN_STATUS_ENABLED)
	{
		tet_printf("current pin status is TAPI_SIM_PIN_STATUS_ENABLED");
	}
	else
	{
		tet_printf("current pin status is unhandled status[%d]",pin_status );
		return -1;
	}

	if(pin_status == wish_condition )
	{
		tet_printf("current pin status is same with wished condition");
		return 0;
	}
	else
	{
		tet_printf("current pin status is different with wished condition");	
		sec_data.type = wish_pin;

		if(wish_pin == TAPI_SIM_PTYPE_PIN1 )
		{
			sec_data.pw_len = strlen(g_si.pin1);
			sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
			memcpy(sec_data.pw,g_si.pin1,sec_data.pw_len);
		}
		else if(wish_pin == TAPI_SIM_PTYPE_PIN2)
		{
			sec_data.pw_len = strlen(g_si.pin2);
			sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
			memcpy(sec_data.pw,g_si.pin2,sec_data.pw_len);
		}

		gAsyncResult = FALSE;
		
		if(wish_condition == TAPI_SIM_PIN_STATUS_DISABLED)
		{
			tet_printf("make pin status with wished condition(TAPI_SIM_PIN_STATUS_DISABLED)");

			tapiRet = tel_disable_sim_security(&sec_data,&pRequestId);
		}
		else if(wish_condition == TAPI_SIM_PIN_STATUS_ENABLED)
		{
			tet_printf("make pin status with wished condition(TAPI_SIM_PIN_STATUS_ENABLED)");		
			tapiRet = tel_enable_sim_security(&sec_data,&pRequestId);
		}
		else
		{
			tet_printf("Unhandled wish_condition[%d]", wish_condition);
			return -1;
		}
		
		if(tapiRet != TAPI_API_SUCCESS)
		{
			tet_printf("tapi return[%d] when en/disabling pin for condition",tapiRet);
			return -1;
		}
		else
		{
			G_MAIN_LOOP_RUN(3);
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
    tet_printf("\n***************[SIM]Callback data **********************\n");
    tet_printf("[SIM APP]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM APP]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    switch(sim_event->EventType)
    {
        case TAPI_EVENT_SIM_ENABLE_SEC_CNF:
        {
            tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_ENABLE_SEC_CNF***************************\n");
            tet_printf("SIM Enable PIN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            gAsyncResult = TRUE;
        }
        break;

        case TAPI_EVENT_SIM_DISABLE_SEC_CNF:
        {
            tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_DISABLE_SEC_CNF***************************\n");
            tet_printf("SIM Disable PIN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            gAsyncResult = TRUE;
        }
        break;

        default:
        {
            gSimEventStatus = sim_event->Status;
            gAsyncResult = FALSE;
            tet_printf("[SIM]Undhandled event type [%d]\n",sim_event->EventType);
            tet_printf("[SIM]Undhandled event state [%d]\n",sim_event->Status);
            break;
	}
    }//end switch

    G_MAIN_LOOP_CLOSE();
    return 0;
}


static void startup()
{
	tet_infoline("utc_tel_disable_sim_security Test Start\n");

	sim_pin_info_setup();

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_DISABLE_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
        tel_register_event(TAPI_EVENT_SIM_ENABLE_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);

		gStartupSuccess = TRUE;

	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	tel_register_app_name("org.samsung.test");
 	return;
}

static void cleanup()
{
	tet_infoline("utc_tel_disable_sim_security Test Finished\n");

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
* @brief tel_disable_sim_security() API : Negative condition(First parameter is Null)
* @par ID: UTC_TEL_DIsABLE_SIM_SECURITY_01
* @param [in] &pPinData = NULL
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_disable_sim_security_01()
{
    TET_STAMP_START("NULL parameter test 1");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;

    TET_CHECK(TAPI_API_INVALID_PTR, tel_disable_sim_security(NULL,&pRequestId));
    TET_STAMP_PASS();
    return;
}

/**
* @brief tel_disable_sim_security() API : Negative condition(Second parameter is Null)
* @par ID: UTC_TEL_DIsABLE_SIM_SECURITY_02
* @param [in] &pPinData.szPinCode = PIN1_NORMAL
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_disable_sim_security_02()
{
    TET_STAMP_START("NULL parameter test 2");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimSecPw_t sec_data;
	memset(&sec_data, 0x00, sizeof(TelSimSecPw_t));
	sec_data.type = TAPI_SIM_PTYPE_PIN1;
	sec_data.pw_len = strlen(g_si.pin1);
	sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
	memcpy(sec_data.pw,g_si.pin1,sec_data.pw_len);	

    TET_CHECK(TAPI_API_INVALID_PTR, tel_disable_sim_security(&sec_data,NULL));
    TET_STAMP_PASS();
    return;
}

/**
* @brief tel_disable_sim_security() API : Low boundary test
* @par ID: UTC_TEL_DIsABLE_SIM_SECURITY_03
* @param [in] &pPinData.PinType = TAPI_SIM_PTYPE_PIN1-1; //<---- Invalid value
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_disable_sim_security_03()
{
    TET_STAMP_START("Invalid Input test 1");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int pRequestId = 0;
	TelSimSecPw_t sec_data;
	memset(&sec_data, 0x00, sizeof(TelSimSecPw_t));
	sec_data.type = TAPI_SIM_PTYPE_PIN1 -1;
	sec_data.pw_len = strlen(g_si.pin1);
	sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
	memcpy(sec_data.pw,g_si.pin1,sec_data.pw_len);

    TET_CHECK(TAPI_API_INVALID_INPUT, tel_disable_sim_security(&sec_data,&pRequestId));
    TET_STAMP_PASS();
    return;
}

/**
* @brief tel_disable_sim_security() API : Upper boundary test
* @par ID: UTC_TEL_DIsABLE_SIM_SECURITY_04
* @param [in] &pPinData.PinType = TAPI_SIM_PTYPE_ADM+1; //<---- Invalid value
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_disable_sim_security_04()
{
    TET_STAMP_START("Invalid Input test 2");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int pRequestId = 0;
	TelSimSecPw_t sec_data;
	memset(&sec_data, 0x00, sizeof(TelSimSecPw_t));
	sec_data.type = TAPI_SIM_PTYPE_PIN1 +1;
	sec_data.pw_len = strlen(g_si.pin1);
	sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
	memcpy(sec_data.pw,g_si.pin1,sec_data.pw_len);

    TET_CHECK(TAPI_API_INVALID_INPUT, tel_disable_sim_security(&sec_data,&pRequestId));
    TET_STAMP_PASS();
    return;
}


/**
* @brief tel_disable_sim_security() API : Invalid parameter(PIN1_INCORRECT)
* @par ID: UTC_TEL_DIsABLE_SIM_SECURITY_05
* @param [in] &pPinData.szPinCode = PIN1_INCORRECT; //<---- Invalid value
* @param [out] &pRequestId = NULL
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event received is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_DISABLE_PIN_CNF (TAPI_SIM_PIN_INCORRECT_PASSWORD)
*/
void utc_tel_disable_sim_security_05()
{
    TET_STAMP_START("PIN1_INCORRECT test");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	// Step 1 : make proper condition
    tet_printf("Step 1 : enable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_ENABLED))
	{
		tet_printf("condition set is failed!!");
		return;
	}

	// Step 2 : disable pin
    tet_printf("Step 2 : disable pin");
	int pRequestId = 0;
	int tapiRet = TAPI_API_SUCCESS;
	TelSimSecPw_t sec_data;
	memset(&sec_data, 0x00, sizeof(TelSimSecPw_t));
	sec_data.type = TAPI_SIM_PTYPE_PIN1;
	sec_data.pw_len = strlen(g_si.wpin1);
	sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
	memcpy(sec_data.pw,g_si.wpin1,sec_data.pw_len);

    gAsyncResult = FALSE;

	tapiRet = tel_disable_sim_security(&sec_data,&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);

    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(3);

    tet_printf("after real tc api - wait stop");

    TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
    TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_SIM_PIN_INCORRECT_PASSWORD, gSimEventStatus); // If gSimEventStatus != TAPI_SIM_PIN_INCORRECT_PASSWORD, then TET_FAIL and return
    TET_STAMP_PASS();
    tet_printf("finish case");	
	return;
}


/**
* @brief tel_disable_sim_security() API : Normal operation
* @par ID: UTC_TEL_DIsABLE_SIM_SECURITY_06
* @param [in] &pPinData
	pPinData.szPinCode = PIN1_NORMAL
	pPinData.PinType=TAPI_SIM_PTYPE_PIN1
* @param [out] &pRequestId = NULL
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event received is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_DISABLE_PIN_CNF (TAPI_SIM_PIN_OPERATION_SUCCESS)
*/
void utc_tel_disable_sim_security_06()
{
    TET_STAMP_START("PIN1_NORMAL test");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	// Step 1 : make proper condition
    tet_printf("Step 1 : enable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_ENABLED))
	{
		tet_printf("condition set is failed!!");
		return;
	}

	// Step 2 : disable pin
    tet_printf("Step 2 : disable pin");
	int pRequestId = 0;
	int tapiRet = TAPI_API_SUCCESS;
	TelSimSecPw_t sec_data;
	memset(&sec_data, 0x00, sizeof(TelSimSecPw_t));
	sec_data.type = TAPI_SIM_PTYPE_PIN1;
	sec_data.pw_len = strlen(g_si.pin1);
	sec_data.pw = (unsigned char*)malloc(sec_data.pw_len);
	memcpy(sec_data.pw,g_si.pin1,sec_data.pw_len);

    gAsyncResult = FALSE;

	tapiRet = tel_disable_sim_security(&sec_data,&pRequestId);
	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,tapiRet);

    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(3);

    tet_printf("after real tc api - wait stop");

    TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
    TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_SIM_PIN_OPERATION_SUCCESS, gSimEventStatus); // If gSimEventStatus != TAPI_SIM_PIN_INCORRECT_PASSWORD, then TET_FAIL and return
    TET_STAMP_PASS();
    tet_printf("finish case");	
	return;
}

/** @} */
