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
* @addtogroup UTS_TAPI_SIM_LOCK_ENABLE_PERSONALISATION tel_enable_sim_personalization
*
* @brief Unit test code for tel_enable_sim_personalization()
* @par Test function Prototype:
* 	TapiResult_t tel_enable_sim_personalization(TelSimLockPersonalisationInfo_t *PersonalisationPwd, int *pRequestId);
* @par Test function description:
* 	This API is used to enable the network personalisation feature.
* @par Important Notes:
* 	None
* @param [in] PersonalisationPwd
* 	The information required to disable network personalisation.
*	@code
	typedef struct
	{
		TelSimPersType_t PersonalisationLock;
		UINT8	Password_length;
		UINT8 	Personalisation_password[TAPI_SEC_LOCK_PERSONALISATION_PASSWD_LEN];
	}TelSimLockPersonalisationInfo_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is TAPI_EVENT_SIM_ENABLE_PERSONALIZATION_CNF
* @pre
*	None
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tapi_sim_lock_personalization.c
* @brief
	Test for the tel_disable_sim_personalization() API that is used to disable the network personalisation feature \n
	Test for the tel_enable_sim_personalization() API that is used to enable the network personalisation feature
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_get_sim_personalization_status.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_personalization_status01, 1},
	{utc_tel_get_sim_personalization_status02, 2},
	{utc_tel_get_sim_personalization_status03, 3},
	{utc_tel_get_sim_personalization_status04, 4},
	{utc_tel_get_sim_personalization_status05, 5},
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
		case TAPI_EVENT_SIM_PERS_STATUS_CNF:
	    	{
			tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_PERS_STATUS_CNF******\n");
			tet_printf("[SIM APP]SIM personalization status event status  = [0x%x]\n", sim_event->Status);
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
    tet_infoline("uts_tel_get_sim_personalization_status Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_PERS_STATUS_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("uts_tel_get_sim_personalization_status Test Finished\n");

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
* @brief TelTapiSimGetLockPersonalisationSatus() API : Negative condition(invalid input parameterl)
* @par ID: UTC_TEL_GET_SIM_PERSONALIZATION_STATUS01
* @param [in] PersType =  TAPI_SIM_PERS_CP+1 
* @param [out]&pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_get_sim_personalization_status01()
{
	TET_STAMP_START("utc_tel_get_sim_personalization_status01 -TAPI_API_INVALID_INPUT ");
	int pRequestId=0;
	TelSimPersType_t PersType = TAPI_SIM_PERS_CP+1 ;
	TET_CHECK(TAPI_API_INVALID_INPUT,tel_get_sim_personalization_status (PersType, &pRequestId));
	TET_STAMP_PASS();
	return;
}


void utc_tel_get_sim_personalization_status02()
{
	TET_STAMP_START("utc_tel_get_sim_personalization_status02 ");
	int pRequestId=0;
	TelSimPersType_t PersType= TAPI_SIM_PERS_NET;

	gAsyncResult = FALSE;
					
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_personalization_status (PersType, &pRequestId));
								
								tet_printf("after real tc api - wait start");
								
								G_MAIN_LOOP_RUN(5);
								
								tet_printf("after real tc api - wait stop");

	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}




void utc_tel_get_sim_personalization_status03()
{
	TET_STAMP_START("utc_tel_get_sim_personalization_status03 ");
	int pRequestId=0;
	TelSimPersType_t PersType=TAPI_SIM_PERS_NS ;
	
	gAsyncResult = FALSE;
						
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_personalization_status (PersType, &pRequestId));
									
									tet_printf("after real tc api - wait start");
									
									G_MAIN_LOOP_RUN(5);
									
									tet_printf("after real tc api - wait stop");

	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}



void utc_tel_get_sim_personalization_status04()
{
	TET_STAMP_START("utc_tel_get_sim_personalization_status04 ");
	int pRequestId=0;
	TelSimPersType_t PersType= TAPI_SIM_PERS_SP ;
	
	gAsyncResult = FALSE;
						
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_personalization_status (PersType, &pRequestId));
									
									tet_printf("after real tc api - wait start");
									
									G_MAIN_LOOP_RUN(5);
									
									tet_printf("after real tc api - wait stop");

	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


void utc_tel_get_sim_personalization_status05()
{
	TET_STAMP_START("utc_tel_get_sim_personalization_status05 ");
	int pRequestId=0;
	TelSimPersType_t PersType= TAPI_SIM_PERS_CP ;
	
	gAsyncResult = FALSE;
						
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_personalization_status (PersType, &pRequestId));
									
									tet_printf("after real tc api - wait start");
									
									G_MAIN_LOOP_RUN(5);
									
									tet_printf("after real tc api - wait stop");

	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}

