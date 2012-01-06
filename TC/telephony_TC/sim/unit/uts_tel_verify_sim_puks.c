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
* @addtogroup UTS_TAPI_SIM_UNBLOCK_PIN tel_verify_sim_puks
*
* @brief Unit test code for tel_verify_sim_puks()
* @par Test function Prototype:
* 	TapiResult_t tel_verify_sim_puks(const TelSimPinData_t * pPukPinData, const TelSimPinData_t * pNewPinData, int *pRequestId);
* @par Test function description:
* 	This function performs unblock PIN operation based on PUK information passed along with
* 	unblock information entered by the user.
* @par Important Notes:
* 	Based on Pin status (PIN/UPIN/PUK) is blocked, an unsolicited event TAPI_EVENT_SIM_STATUS_IND
*	is published to Applications.
* @param [in] pPukPinData  Specifies unblocking PIN password
*	@code
	typedef struct
	{
		TelSimPinType_t	PinType;	// Pin type
		char	szPinCode[TAPI_SIM_PIN_LENGTH+1]; // PIN code
	} TelSimPinData_t;
*	@endcode
* @param [in] pNewPinData  Specifies PIN password.
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* ( Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_UNBLOCK_PIN_CNF(TAPI_SIM_PIN_OPERATION_SUCCESS)
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
* @file		uts_tel_verify_sim_puks.c
* @brief	Test for the tel_verify_sim_puks() API that is used to unblock PIN
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/


#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_verify_sim_puks.h"
#include "async_util.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_verify_sim_puks_01, 1},
	{utc_tel_verify_sim_puks_02, 2},
	{utc_tel_verify_sim_puks_03, 3},
	{utc_tel_verify_sim_puks_04, 4},
	{utc_tel_verify_sim_puks_05, 5},
	{NULL, 0}
};

/*
  User Defined Handler
*/
// Global variable for user data
static BOOL gAsyncResult; //user defined data : Async callback is called or not

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
//	gAsyncResult = FALSE;
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
			tet_printf("wait for condition async event resp - start");				
			G_MAIN_LOOP_RUN(5);
			tet_printf("wait for condition async event resp - stop");				
		}

		tet_printf("gAsyncResult[%d]", gAsyncResult);				

		if(gAsyncResult == TRUE)
		{
			return 0;
		}
		else
		{
			return -1;
		}
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
	case   TAPI_EVENT_SIM_VERIFY_PUK_CNF:
   	{
		tet_printf("[SIM]***************** TAPI_EVENT_SIM_VERIFY_PUK_CNF***************************");
		TelSimPinOperationResult_t opResult = (TelSimPinOperationResult_t)sim_event->Status;
   	   	TelSimPinInfo_t *pPinInfo;
		pPinInfo = (TelSimPinInfo_t *)sim_event->pData;
		tet_printf("SIM Unblock PIN event status = [%x]", opResult);

	   	if(opResult == TAPI_EVENT_SIM_VERIFY_PUK_CNF)
	   	{
	   		if(pPinInfo->PinType == TAPI_SIM_PTYPE_PIN1)
	   			tet_printf("Unblock PIN1 Success!\n");
	   		else if(pPinInfo->PinType == TAPI_SIM_PTYPE_PIN2)
	   			tet_printf("Unblock PIN2 Success!\n");
	   		else if(pPinInfo->PinType == TAPI_SIM_PTYPE_PUK1)
	   			tet_printf("Unblock PUK1 Success!\n");
	   		else if(pPinInfo->PinType == TAPI_SIM_PTYPE_PUK2)
	   			tet_printf("Unblock PUK2 Success!\n");

	   	}
	   	else
	   	{
	   		if(pPinInfo->PinType == TAPI_SIM_PTYPE_PIN1)
    			{
    				tet_printf("PIN1 Verification Failed! - PIN Required \n");
    				//tet_printf("Remainint attempts [%d]\n", pPinInfo->RemainingAttempts);
    			}
			else if(pPinInfo->PinType == TAPI_SIM_PTYPE_PUK1)
			{
				tet_printf("PIN1 Verification Failed! - PUK Required \n");
    				//tet_printf("Remainint attempts [%d]\n", pPinInfo->RemainingAttempts);
			}
    			else if(pPinInfo->PinType == TAPI_SIM_PTYPE_PIN2)
    			{
    				tet_printf("PIN2 Verification Failed! - PIN2 Required \n");
    				//tet_printf("Remainint attempts [%d]\n", pPinInfo->RemainingAttempts);
    			}
			else if(pPinInfo->PinType == TAPI_SIM_PTYPE_PUK1)
			{
				tet_printf("PIN2 Verification Failed! - PUK2 Required \n");
    				//tet_printf("Remainint attempts [%d]\n", pPinInfo->RemainingAttempts);
			}


	   	}
			bReceived = TRUE;
			gAsyncResult = TRUE;
  	}
	break;

	    case TAPI_EVENT_SIM_ENABLE_SEC_CNF: //preet: changed TAPI_EVENT_SIM_ENABLE_PIN_CNF to TAPI_EVENT_SIM_ENABLE_SEC_CNF
        {
            tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_ENABLE_SEC_CNF***************************\n");
            tet_printf("SIM Enable PIN event status = [%x]\n", sim_event->Status);

            if(sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
                gAsyncResult = TRUE;
            else
                gAsyncResult = FALSE;

			tet_printf("gAsyncResult after enpin[%d]",gAsyncResult);
        }
        break;

	    case TAPI_EVENT_SIM_DISABLE_SEC_CNF: 	////preet: changed TAPI_EVENT_SIM_DISABLE_PIN_CNF to TAPI_EVENT_SIM_DISABLE_SEC_CNF
        {
            tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_DISABLE_SEC_CNF***************************\n");
            tet_printf("SIM Disable PIN event status = [%x]\n", sim_event->Status);

            if(sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
                  gAsyncResult = TRUE;
            else
                gAsyncResult = FALSE;
        }
        break;


        default:
        {
		bReceived = TRUE;
		gAsyncResult = FALSE;
		tet_printf("Info : TAPI_EVENT_SIM_VERIFY_PUK_CNF = %d", TAPI_EVENT_SIM_VERIFY_PUK_CNF);
		tet_printf("[SIM]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
		tet_printf("[SIM]Undhandled event state [%d]\n",sim_event->Status);
	}
	break;
    }//end switch

    if (bReceived == TRUE)
      G_MAIN_LOOP_CLOSE();

    return 0;
}


static void startup()
{
    tet_infoline("utc_tel_verify_sim_puks Test Start\n");

	sim_pin_info_setup();

	if ( tel_init() == TAPI_API_SUCCESS)
	{

		tel_register_event(TAPI_EVENT_SIM_VERIFY_PUK_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_ENABLE_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);
		tel_register_event(TAPI_EVENT_SIM_DISABLE_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL);		

		gStartupSuccess = TRUE;

	} else {
		tet_infoline("startup fail. tel_init() failed");
	}
	 tel_register_app_name("org.samsung.test");
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_verify_sim_puks Test Finished\n");
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
* @brief tel_verify_sim_puks() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_01
* @param [in] &pPukPinData = NULL
* @param [in] &pNewPinData
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verify_sim_puks_01()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR 1");
    int pRequestId=0;

    TelSimSecPw_t pPukdata;
    TelSimSecPw_t pNewPindata;
  
    tet_printf("THE PUK CODE IS %s\n", g_si.puk1);
    tet_printf("NEW PIN1 CODE %s\n", g_si.pin1);

    pPukdata.type = TAPI_SIM_PTYPE_PUK1;   // 0x00
    pNewPindata.type = TAPI_SIM_PTYPE_PIN1;   // 0x00

	pPukdata.pw= (unsigned char*)g_si.puk1;
	pNewPindata.pw = (unsigned char*)g_si.pin1;
	pPukdata.pw_len=strlen(g_si.puk1);
	pNewPindata.pw_len=strlen(g_si.pin1);
    TET_CHECK(TAPI_API_INVALID_PTR, tel_verify_sim_puks(NULL,&pNewPindata,&pRequestId));
    TET_STAMP_PASS();
}

/**
* @brief tel_verify_sim_puks() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_02
* @param [in] &pPukPinData
* @param [in] &pNewPinData = NULL
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verify_sim_puks_02()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR 2");
    int pRequestId=0;

    TelSimSecPw_t pPukdata;
    TelSimSecPw_t pNewPindata;

    tet_printf("THE PUK CODE IS %s\n", g_si.puk1);
    tet_printf("NEW PIN1 CODE %s\n", g_si.pin1);

    pPukdata.type = TAPI_SIM_PTYPE_PUK1;   // 0x00
    pNewPindata.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
	pPukdata.pw=(unsigned char*)g_si.puk1;
	pNewPindata.pw = (unsigned char*)g_si.pin1;

	pPukdata.pw_len=strlen(g_si.puk1);
	pNewPindata.pw_len=strlen(g_si.pin1);
    TET_CHECK(TAPI_API_INVALID_PTR, tel_verify_sim_puks(&pPukdata,NULL,&pRequestId));
    TET_STAMP_PASS();
}

/**
* @brief tel_verify_sim_puks() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_03
* @param [in] &pPukPinData
* @param [in] &pNewPinData
* @param [out]&pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verify_sim_puks_03()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR 3");

    TelSimSecPw_t pPukdata;
    TelSimSecPw_t pNewPindata;

    tet_printf("THE PUK CODE IS %s\n", g_si.puk1);
    tet_printf("NEW PIN1 CODE %s\n", g_si.pin1);

    pPukdata.type = TAPI_SIM_PTYPE_PUK1;   // 0x00
    pNewPindata.type = TAPI_SIM_PTYPE_PIN1;   // 0x00

	pPukdata.pw=(unsigned char*)g_si.puk1;
	pNewPindata.pw = (unsigned char*)g_si.pin1;

	pPukdata.pw_len=strlen(g_si.puk1);
	pNewPindata.pw_len=strlen(g_si.pin1);
    TET_CHECK(TAPI_API_INVALID_PTR, tel_verify_sim_puks(&pPukdata,&pNewPindata,NULL));
    TET_STAMP_PASS();
}

/**
* @brief tel_verify_sim_puks() API : Positive condition(Try to unblock PIN1 code)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_04
* @param [in] &pPukPinData
* @param [in] &pNewPinData
* @code
    TelSimPinData_t pPukdata;
    TelSimPinData_t pNewPindata;

    strcpy(init_puk_val, PUK1_CODE);
    strcpy(init_pin_val, PIN1_NORMAL);

    pPukdata.PinType = TAPI_SIM_PTYPE_PUK1;
    pNewPindata.PinType = TAPI_SIM_PTYPE_PIN1;

    memcpy(pPukdata.szPinCode,init_puk_val,length);
    memcpy(pNewPindata.szPinCode,init_pin_val,length);
* @endcode
* @param [out]&pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* ( Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_UNBLOCK_PIN_CNF(TAPI_SIM_PIN_OPERATION_SUCCESS)
*/
void utc_tel_verify_sim_puks_04()
{
    TET_STAMP_START("Normal operation TAPI_SIM_PTYPE_PIN1");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;

    TelSimSecPw_t pPukdata;
    TelSimSecPw_t pNewPindata;

	// Step 1 : make proper condition
    tet_printf("Step 1 : enable pin");

	if( 0 != sim_pin_process_for_condition(TAPI_SIM_PTYPE_PIN1,TAPI_SIM_PIN_STATUS_ENABLED))
	{
		tet_printf("condition set is failed!!");
		return;
	}

	// Step 2 : verify puk
    tet_printf("Step 2 : verify puk");

    tet_printf("THE PUK CODE IS %s\n", g_si.puk1);
    tet_printf("NEW PIN1 CODE %s\n", g_si.pin1);

    pPukdata.type = TAPI_SIM_PTYPE_PUK1;
    pNewPindata.type = TAPI_SIM_PTYPE_PIN1;
	pPukdata.pw = (unsigned char*)g_si.puk1;
	pNewPindata.pw = (unsigned char*)g_si.pin1;

	pPukdata.pw_len=strlen(g_si.puk1);
	pNewPindata.pw_len=strlen(g_si.pin1);

    gAsyncResult = FALSE;

    TapiResult_t err = tel_verify_sim_puks(&pPukdata,&pNewPindata,&pRequestId);

	TET_RESULT_NOT_EQUAL(TET_FAIL,TAPI_API_SUCCESS,err);

    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(10);

    tet_printf("after real tc api - wait stop");

	TET_RESULT_NOT_EQUAL_GOTO(TET_FAIL,exit,gAsyncResult,TRUE); // If gAsyncResult != FALSE, then TET_FAIL and return
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
* @brief tel_verify_sim_puks() API : Positive condition(Try to unblock PIN2 code)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_05
* @param [in] &pPukPinData
* @param [in] &pNewPinData
* @code
    TelSimPinData_t pPukdata;
    TelSimPinData_t pNewPindata;

    strcpy(init_puk_val, PUK2_CODE);
    strcpy(init_pin_val, PIN2_NORMAL);

    pPukdata.PinType = TAPI_SIM_PTYPE_PUK2;
    pNewPindata.PinType = TAPI_SIM_PTYPE_PIN2;

    memcpy(pPukdata.szPinCode,init_puk_val,length);
    memcpy(pNewPindata.szPinCode,init_pin_val,length);
* @endcode
* @param [out]&pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* ( Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_UNBLOCK_PIN_CNF(TAPI_SIM_PIN_OPERATION_SUCCESS)
*/
void utc_tel_verify_sim_puks_05()
{
    TET_STAMP_START("Normal operation : TAPI_SIM_PTYPE_PIN2");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int pRequestId=0;
	TelSimSecPw_t puk_data = {0,};
	TelSimSecPw_t new_pin_data = {0,};
	
	puk_data.type = TAPI_SIM_PTYPE_PUK2;   // 0x00
	puk_data.pw_len = strlen(g_si.puk2);
	puk_data.pw = (unsigned char*)malloc(puk_data.pw_len);
	memcpy(puk_data.pw,g_si.puk2,puk_data.pw_len);
	
	new_pin_data.type = TAPI_SIM_PTYPE_PIN2;   // 0x00
	new_pin_data.pw_len = strlen(g_si.pin2);
	new_pin_data.pw = (unsigned char*)malloc(new_pin_data.pw_len);
	memcpy(new_pin_data.pw,g_si.pin2,new_pin_data.pw_len);

    gAsyncResult = FALSE;

    TapiResult_t err = tel_verify_sim_puks(&puk_data,&new_pin_data,&pRequestId);
    TET_CHECK(TAPI_API_SUCCESS, err);

    tet_printf("after real tc api - wait start");

    G_MAIN_LOOP_RUN(10);

    tet_printf("after real tc api - wait stop");

    TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
    TET_STAMP_PASS();
}

/** @} */
