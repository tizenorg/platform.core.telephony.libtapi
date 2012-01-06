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
* @addtogroup UTS_TAPI_SIM_CHANGE_PIN tel_change_sim_pins
*
* @brief Unit test code for tel_change_sim_pins()
* @par Test function Prototype:
* 	TapiResult_t tel_change_sim_pins	(TelSimPinType_t PinType , const char *pOldPinData, const char *pNewPinData, int *pRequestId);
* @par Test function description:
* 	This function changes PIN code based on the pin type passed along with old pin data and new pin data.
* @par Important Notes:
* 	Based on Pin status (PIN/UPIN/PUK) is blocked, an unsolicited event TAPI_EVENT_SIM_STATUS_IND is published to Applications
* @param [in]
*	PinType  Pin type information.
* @code
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
* @param [in]
*	pOldPinData  Old pin code, entered by the user. (Null terminated string)
* @param [in]
*	pNewPinData  New pin code, entered by the user. (Null terminated string)
* @param [out] pRequestId
* \n  Unique identifier for a particular request.
* \n  Request_id value can be any value from 0 to 255 if the API is returned successfully
* \n  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_DISABLE_PIN_CNF
*	- TAPI_EVENT_SIM_ENABLE_PIN_CNF
* 	- TAPI_EVENT_SIM_CHANGE_PIN_CNF
*	- TAPI_EVENT_SIM_UNBLOCK_PIN_CNF
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
* @file		uts_tel_change_sim_pins.c
* @brief	Test for the tel_change_sim_pins() API that is used to change pin1 code
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_change_sim_pins.h"
#include "sim_common.h"
#include "async_util.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_change_sim_pins_01, 1},
	{utc_tel_change_sim_pins_02, 2},
	{utc_tel_change_sim_pins_03, 3},
	{utc_tel_change_sim_pins_04, 4},
	{utc_tel_change_sim_pins_05, 5},
	{utc_tel_change_sim_pins_06, 6},
	{utc_tel_change_sim_pins_07, 7},
	{utc_tel_change_sim_pins_08, 8},
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
    tet_printf("\n\n\n***************[SIM APP]Callback data **********************\n");
    tet_printf("[SIM APP]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM APP]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    switch(sim_event->EventType)
    {
	    case TAPI_EVENT_SIM_ENABLE_SEC_CNF: //preet: changed TAPI_EVENT_SIM_ENABLE_PIN_CNF to TAPI_EVENT_SIM_ENABLE_SEC_CNF
        {
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_ENABLE_SEC_CNF***************************\n");
            tet_printf("SIM Enable PIN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            if(sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
                gAsyncResult = TRUE;
            else
                gAsyncResult = FALSE;
        }
        break;

	    case TAPI_EVENT_SIM_DISABLE_SEC_CNF: 	////preet: changed TAPI_EVENT_SIM_DISABLE_PIN_CNF to TAPI_EVENT_SIM_DISABLE_SEC_CNF
        {
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_DISABLE_SEC_CNF***************************\n");
            tet_printf("SIM Disable PIN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            if(sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
                  gAsyncResult = TRUE;
            else
                gAsyncResult = FALSE;
        }
        break;

	    case TAPI_EVENT_SIM_CHANGE_PINS_CNF: 	//preet: changed TAPI_EVENT_SIM_CHANGE_PIN_CNF to TAPI_EVENT_SIM_CHANGE_PINS_CNF:
    	{
            tet_printf("\n[GOT THE EVENT]*****TAPI_EVENT_SIM_CHANGE_PINS_CNF:******\n");
            tet_printf("SIM Change PIN event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            gAsyncResult = TRUE;
        }
        break;

        default:
        {
            gSimEventStatus = sim_event->Status;
            gAsyncResult = FALSE;
            tet_printf("[SIM APP]Undhandled event type [0x%x]\n",sim_event->EventType);
            tet_printf("[SIM APP]Undhandled event state [%d]\n",sim_event->Status);
            break;
	}
    }//end switch

    G_MAIN_LOOP_CLOSE();
    return 0;
}


static void startup()
{
    // describe code if you need...
    tet_infoline("utc_tel_change_sim_pins Test Start\n");

	sim_pin_info_setup();

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		tel_register_event(TAPI_EVENT_SIM_DISABLE_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_CHANGE_PINS_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_VERIFY_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
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
	tet_infoline("utc_tel_change_sim_pins Test Finished\n");

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
* @brief tel_change_sim_pins() API : Negative condition(First parameter is Null)
* @par ID: UTC_TEL_CHANGE_SIM_PINS_01
* @param [in] PinType = TAPI_SIM_PTYPE_PIN1
* @param [in] *pOldPinData = NULL
* @param [in] *pNewPinData = PIN1_CHANGED
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_change_sim_pins_01()
{
    TET_STAMP_START("tel_change_sim_pins(),Null parameter check 1");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;

    TelSimSecPw_t pNewPinData;
    pNewPinData.type = TAPI_SIM_PTYPE_PIN1;
    pNewPinData.pw = (unsigned char*)g_si.pin1;
    pNewPinData.pw_len = strlen(g_si.pin1);

    TET_CHECK(TAPI_API_INVALID_PTR, tel_change_sim_pins(NULL, &pNewPinData, &pRequestId));
    TET_STAMP_PASS();
	return;
}

/**
* @brief tel_change_sim_pins() API : Negative condition(Second parameter is Null)
* @par ID: UTC_TEL_CHANGE_SIM_PINS_02
* @param [in] PinType = TAPI_SIM_PTYPE_PIN1
* @param [in] *pOldPinData = PIN1_NORMAL
* @param [in] *pNewPinData = NULL
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_change_sim_pins_02()
{
    TET_STAMP_START("tel_change_sim_pins(),Null parameter check 2");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;

    TelSimSecPw_t pOldPinData;
    pOldPinData.type = TAPI_SIM_PTYPE_PIN1;
    pOldPinData.pw = (unsigned char*)g_si.pin1;
    pOldPinData.pw_len = strlen(g_si.pin1);

    TET_CHECK(TAPI_API_INVALID_PTR, tel_change_sim_pins(&pOldPinData,NULL,&pRequestId));
    TET_STAMP_PASS();
	return;
}

/**
* @brief tel_change_sim_pins() API : Negative condition(Third parameter is Null)
* @par ID: UTC_TEL_CHANGE_SIM_PINS_03
* @param [in] PinType = TAPI_SIM_PTYPE_PIN1
* @param [in] *pOldPinData = PIN1_NORMAL
* @param [in] *pNewPinData = PIN1_CHANGED
* @param [out] NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_change_sim_pins_03()
{
    TET_STAMP_START("tel_change_sim_pins(), Null parameter check3");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    TelSimSecPw_t pOldPinData;
    TelSimSecPw_t pNewPinData;

    pOldPinData.type = TAPI_SIM_PTYPE_PIN1;
    pOldPinData.pw_len = strlen(g_si.pin1);
	pOldPinData.pw = (unsigned char*)malloc(pOldPinData.pw_len);
	memcpy(pOldPinData.pw,g_si.pin1,pOldPinData.pw_len);
	
    pNewPinData.type = TAPI_SIM_PTYPE_PIN1;
    pNewPinData.pw_len = strlen(g_si.pin1);
	pNewPinData.pw = (unsigned char*)malloc(pNewPinData.pw_len);
	memcpy(pNewPinData.pw,g_si.pin1,pNewPinData.pw_len);

    TET_CHECK(TAPI_API_INVALID_PTR, tel_change_sim_pins(&pOldPinData, &pNewPinData,NULL));
    TET_STAMP_PASS();
	return;
}


/**
* @brief tel_change_sim_pins() API : Low boundary test
* @par ID: UTC_TEL_CHANGE_SIM_PINS_04
* @param [in] PinType = (TelSimPinType_t)TAPI_SIM_PTYPE_PIN1-1;//<-- Invalid pintype
* @param [in] *pOldPinData = PIN1_NORMAL
* @param [in] *pNewPinData = PIN1_CHANGED
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_change_sim_pins_04()
{
    TET_STAMP_START("invalid pin type 1");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;

    TelSimSecPw_t pOldPinData;
    TelSimSecPw_t pNewPinData;
	
    pOldPinData.type = TAPI_SIM_PTYPE_PIN1 -1; //INVALID PINTYPE
    pOldPinData.pw_len = strlen(g_si.pin1);
	pOldPinData.pw = (unsigned char*)malloc(pOldPinData.pw_len);
	memcpy(pOldPinData.pw,g_si.pin1,pOldPinData.pw_len);
	
    pNewPinData.type = TAPI_SIM_PTYPE_PIN1 -1; //INVALID PINTYPE
    pNewPinData.pw_len = strlen(g_si.pin1);
	pNewPinData.pw = (unsigned char*)malloc(pNewPinData.pw_len);
	memcpy(pNewPinData.pw,g_si.pin1,pNewPinData.pw_len);

    TET_CHECK(TAPI_API_INVALID_INPUT, tel_change_sim_pins (&pOldPinData, &pNewPinData,&pRequestId));
    TET_STAMP_PASS();
	return;
}

/**
* @brief tel_change_sim_pins() API : Invalid pin type 
* @par ID: UTC_TEL_CHANGE_SIM_PINS_05
* @param [in] PinType = (TelSimPinType_t)TAPI_SIM_PTYPE_SIM+1;//<-- Invalid pintype
* @param [in] *pOldPinData = PIN2_NORMAL
* @param [in] *pNewPinData = PIN2_CHANGED
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
*/
void utc_tel_change_sim_pins_05()
{
    TET_STAMP_START("Invalid pin type ");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;

    TelSimSecPw_t pOldPinData;
    TelSimSecPw_t pNewPinData;
	
    pOldPinData.type = TAPI_SIM_PTYPE_SIM +1; //INVALID PINTYPE
    pOldPinData.pw_len = strlen(g_si.pin1);
	pOldPinData.pw = (unsigned char*)malloc(pOldPinData.pw_len);
	memcpy(pOldPinData.pw,g_si.pin1,pOldPinData.pw_len);
	
    pNewPinData.type = TAPI_SIM_PTYPE_SIM +1; //INVALID PINTYPE
    pNewPinData.pw_len = strlen(g_si.pin1);
	pNewPinData.pw = (unsigned char*)malloc(pNewPinData.pw_len);
	memcpy(pNewPinData.pw,g_si.pin1,pNewPinData.pw_len);

    TET_CHECK(TAPI_API_INVALID_INPUT,tel_change_sim_pins (&pOldPinData, &pNewPinData,&pRequestId));
    TET_STAMP_PASS();
	return;
}

/**
* @brief tel_change_sim_pins() API : Positive condition(Change Pin1 coce)
* @par ID: UTC_TEL_CHANGE_SIM_PINS_06
* @param [in] PinType = TAPI_SIM_PTYPE_PIN1
* @param [in] *pOldPinData = PIN1_NORMAL
* @param [in] *pNewPinData = PIN1_CHANGED
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event received is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_CHANGE_PIN_CNF (TAPI_SIM_PIN_OPERATION_SUCCESS)
*/
void utc_tel_change_sim_pins_06()
{
    TET_STAMP_START("Change pin1 code successfully : Before test, you set correct PIN1_NORMAL value on 'tet_tapi_util.h'");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int tapiRet = TAPI_API_SUCCESS;
	int pRequestId=0;

	// Step 1 : enable pin
    tet_printf("Step 1 : enable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_ENABLED))
	{
		tet_printf("condition set is failed!!");
		return;
	}

    // Step 2 : now change chv
    tet_printf("Step 2 : now change chv");
    TelSimSecPw_t pOldPinData;
    TelSimSecPw_t pNewPinData;
    pOldPinData.type = TAPI_SIM_PTYPE_PIN1;
    pOldPinData.pw_len = strlen(g_si.pin1);
	pOldPinData.pw = (unsigned char*)malloc(pOldPinData.pw_len);
	memcpy(pOldPinData.pw,g_si.pin1,pOldPinData.pw_len);

    pNewPinData.type = TAPI_SIM_PTYPE_PIN1;
    pNewPinData.pw_len = strlen(g_si.pin1);
	pNewPinData.pw = (unsigned char*)malloc(pNewPinData.pw_len);
	memcpy(pNewPinData.pw,g_si.pin1,pNewPinData.pw_len);

	tapiRet = tel_change_sim_pins(&pOldPinData, &pNewPinData,&pRequestId);
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,TAPI_API_SUCCESS,tapiRet);

    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(3);

    tet_printf("after real tc api - wait stop");


	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE); // If gAsyncResult != FALSE, then TET_FAIL and return
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,TAPI_SIM_PIN_OPERATION_SUCCESS, gSimEventStatus); // If gSimEventStatus != TAPI_SIM_PIN_INCORRECT_PASSWORD, then TET_FAIL and return
	TET_STAMP_PASS();
	
exit:
	// Step 3 : make default condition
	tet_printf("Step 3 : disable pin");
	
	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_DISABLED))
	{
		tet_printf("condition set is failed!!");
	}
	return;
}


/**
* @brief tel_change_sim_pins() API : Negative condition(Incorrect pin code)
* @par ID: UTC_TEL_CHANGE_SIM_PINS_07
* @param [in] PinType = TAPI_SIM_PTYPE_PIN1
* @param [in] *pOldPinData = PIN1_INCORRECT
* @param [in] *pNewPinData = PIN1_CHANGED
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event received is below and the event data is #TelSimPinInfo_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_CHANGE_PIN_CNF (TAPI_SIM_PIN_INCORRECT_PASSWORD)
*/
void utc_tel_change_sim_pins_07()
{
    TET_STAMP_START("Change pin1 code Fail : Before test, you set incorrect PIN1_INCORRECT value on 'tet_tapi_util.h'");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int tapiRet = TAPI_API_SUCCESS;
	int pRequestId=0;

	// Step 1 : make proper condition
    tet_printf("Step 1 : enable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_ENABLED))
	{
		tet_printf("condition set is failed!!");
		return;
	}

     // Step 2 : now change chv
    tet_printf("Step 2 : now change chv");
    TelSimSecPw_t pInvalidPinData;
    TelSimSecPw_t pNewPinData;
    pInvalidPinData.type = TAPI_SIM_PTYPE_PIN1;
    pInvalidPinData.pw_len = strlen(g_si.wpin1);
	pInvalidPinData.pw = (unsigned char*)malloc(pInvalidPinData.pw_len);
	memcpy(pInvalidPinData.pw,g_si.wpin1,pInvalidPinData.pw_len);

    pNewPinData.type = TAPI_SIM_PTYPE_PIN1;
    pNewPinData.pw_len = strlen(g_si.wpin1);
	pNewPinData.pw = (unsigned char*)malloc(pNewPinData.pw_len);
	memcpy(pNewPinData.pw,g_si.wpin1,pNewPinData.pw_len);

    gAsyncResult = FALSE;

	tapiRet = tel_change_sim_pins(&pInvalidPinData, &pNewPinData,&pRequestId);
	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,TAPI_API_SUCCESS,tapiRet);

    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(3);

    tet_printf("after real tc api - wait stop");


    TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE); // If gAsyncResult != FALSE, then TET_FAIL and return
    TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,TAPI_SIM_PIN_INCORRECT_PASSWORD, gSimEventStatus); // If gSimEventStatus != TAPI_SIM_PIN_INCORRECT_PASSWORD, then TET_FAIL and return
    TET_STAMP_PASS();

exit:
	// Step 3 : make default condition
    tet_printf("Step 3 : disable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_DISABLED))
	{
		tet_printf("condition set is failed!!");
	}
	return;
}


/**
* @brief tel_change_sim_pins() API : Negative condition(Disable pin and try to change pin1 code)
* @par ID: UTC_TEL_CHANGE_SIM_PINS_08
* @pre
*	If PIN Enable, try to diable PIN. \n
*	(At the test function 'utc_tel_change_sim_pins_08()' check and disable PIN1 function is coded)
* @param [in] PinType = TAPI_SIM_PTYPE_PIN1
* @param [in] *pOldPinData = PIN1_NORMAL
* @param [in] *pNewPinData = PIN1_CHANGED
* @param [out] &pRequestId
* @return TAPI_API_INVALID_OPERATION
*/
void utc_tel_change_sim_pins_08()
{
    TET_STAMP_START("Disable pin and try to change");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int pRequestId=0;

	// Step 1 : make proper condition
    tet_printf("Step 1 : enable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_DISABLED))
	{
		tet_printf("condition set is failed!!");
		return;		
	}

    // Step 2 : now change chv
    tet_printf("Step 2 : now change chv");
    TelSimSecPw_t pOldPinData;
    TelSimSecPw_t pNewPinData;
    pOldPinData.type = TAPI_SIM_PTYPE_PIN1;
    pOldPinData.pw_len = strlen(g_si.pin1);
	pOldPinData.pw = (unsigned char*)malloc(pOldPinData.pw_len);
	memcpy(pOldPinData.pw,g_si.pin1,pOldPinData.pw_len);

    pNewPinData.type = TAPI_SIM_PTYPE_PIN1;
    pNewPinData.pw_len = strlen(g_si.pin1);
	pNewPinData.pw = (unsigned char*)malloc(pNewPinData.pw_len);
	memcpy(pNewPinData.pw,g_si.pin1,pNewPinData.pw_len);

    TET_CHECK(TAPI_API_INVALID_OPERATION,tel_change_sim_pins(&pOldPinData, &pNewPinData,&pRequestId));
    TET_STAMP_PASS();
	return;
}

/** @} */
