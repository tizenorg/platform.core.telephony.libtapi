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
* @addtogroup UTS_TAPI_SIM_VERIFY_PIN tel_verifiy_sim_pins
*
* @brief Unit test code for tel_verifiy_sim_pins()
* @par Test function Prototype:
* 	TapiResult_t tel_verifiy_sim_pins(const TelSimPinData_t *pPinData, int *pRequestId);
* @par Test function description:
* 	This function performs PIN verification. This function performs PIN verification based
* 	on the pin type passed along with pin data
* @par Important Notes:
* 	This API is used only Boot time. You will used other API set(tel_change_sim_pins(), etc) when boot finished
* @param [in] pPinData  PIN code, entered by the user.
*	@code
	typedef struct
	{
		TelSimPinType_t	pinType; // Pin type
		char szPinCode[TAPI_SIM_PIN_LENGTH+1];	// PIN code
	} TelSimPinData_t;
	typedef enum
	{
		TAPI_SIM_PTYPE_PIN1,
		TAPI_SIM_PTYPE_PIN2,
		TAPI_SIM_PTYPE_PUK1,
		TAPI_SIM_PTYPE_PUK2,
		TAPI_SIM_PTYPE_UPIN,
		TAPI_SIM_PTYPE_ADM
	} TelSimPinType_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPinInfo_t. \n
* ( Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_VERIFY_PIN_CNF(API_SIM_PIN_OPERATION_SUCCESS)
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
* @file		uts_tel_verifiy_sim_pins.c
* @brief	Test for the tel_verifiy_sim_pins() API that is used to verify pin code
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_verifiy_sim_pins.h"
#include "async_util.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_verifiy_sim_pins_01, 1},
	{utc_tel_verifiy_sim_pins_02, 2},
	{utc_tel_verifiy_sim_pins_03, 3},
	{utc_tel_verifiy_sim_pins_04, 4},
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
	gAsyncResult = FALSE;
	tet_printf("[TIME OUT] g_timeout called .Close g_main_roop()");
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}
// End of  G_MAIN Utility//////////////////////////////////////////////////////////////////////


static int  sim_app_callback  (TelTapiEvent_t *sim_event)
{
    tet_printf("\n***************[SIM APP]Callback data **********************\n");
    tet_printf("[SIM]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    BOOL bReceived = FALSE;

    switch(sim_event->EventType)
    {
	case TAPI_EVENT_SIM_VERIFY_SEC_CNF:
	{
		tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_VERIFY_SEC_CNF***************************\n");

		//TelSimPinInfo_t *pPinInfo = (TelSimPinInfo_t*)sim_event->pData;
		tet_printf("SIM Verify PIN event status = [%x]\n", sim_event->Status);

		if(sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
		{
			tet_printf("PIN2 Vefication Success!");

		}
		else
		{
			tet_printf("PIN1 Verification Failed! - PIN Required \n");
			//tet_printf("Remainint attempts [%d]\n", pPinInfo->RemainingAttempts);
		}
			bReceived = TRUE;
			gAsyncResult = TRUE;
	}
	break;

	case TAPI_EVENT_SIM_ENABLE_SEC_CNF:
        {
		tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_ENABLE_SEC_CNF***************************\n");
		//TelSimPinInfo_t *pPinInfo = (TelSimPinInfo_t*)sim_event->pData;

		tet_printf("SIM Enable PIN event status = [%x]\n", sim_event->Status);
		bReceived = TRUE;
		if(sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
			gAsyncResult = TRUE;
        	else
			gAsyncResult = FALSE;


        }
	break;

	case TAPI_EVENT_SIM_DISABLE_SEC_CNF:
        {
		tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_DISABLE_SEC_CNF***************************\n");
		//TelSimPinInfo_t *pPinInfo = (TelSimPinInfo_t*)sim_event->pData;

		tet_printf("SIM Disable PIN event status = [%x]\n", sim_event->Status);

		bReceived = TRUE;
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
		tet_printf("Info : TAPI_EVENT_SIM_DISABLE_SEC_CNF = %d", TAPI_EVENT_SIM_DISABLE_SEC_CNF);
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
    tet_infoline("utc_tel_verifiy_sim_pins Test Start\n");

	sim_pin_info_setup();
	
	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if ( (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_VERIFY_SEC_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) ) || (TAPI_API_SUCCESS == tel_register_event(TAPI_SIM_PIN_OPERATION_SUCCESS,&subscription_id,(TelAppCallback)sim_app_callback,NULL) ) )
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
    tet_infoline("utc_tel_verifiy_sim_pins Test Finished\n");

   // Disable pin with correct PWD
    TelSimSecPw_t pPinData;
    int pRequestId=0;
    pPinData.type=TAPI_SIM_PTYPE_PIN1;

    pPinData.pw = (unsigned char*)g_si.pin1;
	pPinData.pw_len =strlen(g_si.pin1) ;
    tel_disable_sim_security(&pPinData,&pRequestId);

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
* @brief tel_verifiy_sim_pins() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_01
* @param [in] &pPinData
* @param [out]&pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verifiy_sim_pins_01()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR");
    TelSimSecPw_t pPinData;
    pPinData.type=TAPI_SIM_PTYPE_PIN1;
	pPinData.pw = (unsigned char*)g_si.pin1;
	pPinData.pw_len =strlen(g_si.pin1) ;
    TET_CHECK(TAPI_API_INVALID_PTR, tel_verifiy_sim_pins(&pPinData, NULL));
    TET_STAMP_PASS();
}

/**
* @brief tel_verifiy_sim_pins() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_02
* @param [in] &pPinData = NULL
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verifiy_sim_pins_02()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR");
    int RequestId;
    TelSimSecPw_t pPinData;
    pPinData.type=TAPI_SIM_PTYPE_PIN1;
	pPinData.pw = (unsigned char*)g_si.pin1;
	pPinData.pw_len =strlen(g_si.pin1) ;

    TET_CHECK(TAPI_API_INVALID_PTR, tel_verifiy_sim_pins(NULL,&RequestId));
    TET_STAMP_PASS();
}

/**
* @brief tel_verifiy_sim_pins() API : Negative condition(Low boundary test)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_03
* @param [in] &pPinData.PinType = (TelSimPinType_t)TAPI_SIM_PTYPE_PIN1-1;
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verifiy_sim_pins_03()
{
    TET_STAMP_START("TAPI_API_INVALID_INPUT");
    int RequestId;
    TelSimSecPw_t pPinData = {0,};
    pPinData.type = (TelSimPinType_t)TAPI_SIM_PTYPE_PIN1-1;
	pPinData.pw = (unsigned char*)g_si.pin1;
	pPinData.pw_len =strlen(g_si.pin1) ;
    TET_CHECK(TAPI_API_INVALID_INPUT, tel_verifiy_sim_pins(&pPinData, &RequestId));
    TET_STAMP_PASS();
}

/**
* @brief tel_verifiy_sim_pins() API : Negative condition(Upper boundary test)
* @par ID: UTC_TEL_VERIFY_SIM_PUKS_04
* @param [in] &pPinData.PinType = (TelSimPinType_t)TAPI_SIM_PTYPE_SIM+1;
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_verifiy_sim_pins_04()
{
    TET_STAMP_START("TAPI_API_INVALID_INPUT");
    int RequestId;
    TelSimSecPw_t pPinData;
    pPinData.type = (TelSimPinType_t)TAPI_SIM_PTYPE_SIM+1;
	pPinData.pw = (unsigned char*)g_si.pin1;
	pPinData.pw_len =strlen(g_si.pin1) ;

    TET_CHECK(TAPI_API_INVALID_INPUT, tel_verifiy_sim_pins(&pPinData, &RequestId));
    TET_STAMP_PASS();
}

/** @} */
