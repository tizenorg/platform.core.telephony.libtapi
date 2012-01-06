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
* @addtogroup UTS_TEL_DISABLE_PERSONALISATION tel_disable_sim_personalization
*
* @brief Unit test code for tel_disable_sim_personalization()
* @par Test function Prototype:
* 	 int tel_disable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id)
* @par Test function description:
* 	This API is used to disable the network personalisation feature.
* @par Important Notes:
* 	None
* @param [in] TelSimPersPw_t
* 	The information required to disable network personalisation.
*	@code
	typedef struct
	{
			TelSimPersType_t	type;
			unsigned char*	pw;
			unsigned int		pw_len;
	} TelSimPersPw_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is TAPI_EVENT_SIM_DISABLE_PERS_CNF
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
* @file		uts_tel_disable_sim_personalization.c
* @brief
	Test for the tel_disable_sim_personalization() API that is used to disable the network personalisation feature \n
	Test for the tel_enable_sim_personalization() API that is used to enable the network personalisation feature
* @author	
* @version	Initial Creation V0.1
* @date		2010.10.05
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_disable_sim_personalization.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_disable_sim_personalization01, 1},
	{utc_tel_disable_sim_personalization02, 2},
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
		case TAPI_EVENT_SIM_DISABLE_PERS_CNF:
	    	{
			tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_DISABLE_PERS_CNF******\n");
			tet_printf("[SIM]SIM personalization status event status  = [0x%x]\n", sim_event->Status);
			bReceived = TRUE;
			gAsyncResult = TRUE;
		}
		break;

        default:
        {
			bReceived = TRUE;
			gAsyncResult = FALSE;
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
    tet_infoline("utc_tel_disable_sim_personalization_status Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_DISABLE_PERS_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("utc_tel_disable_sim_personalization_status Test Finished\n");

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
void utc_tel_disable_sim_personalization01()
{
	TET_STAMP_START("utc_tel_disable_sim_personalization01 -TAPI_API_INVALID_PTR");
	TET_CHECK(TAPI_API_INVALID_PTR,tel_disable_sim_personalization (NULL, NULL));
	TET_STAMP_PASS();
	return;
}


void utc_tel_disable_sim_personalization02()
{
	TET_STAMP_START("utc_tel_disable_sim_personalization02 ");
	int pRequestId=0;
	TelSimPersPw_t lock_pers = {0,};
	lock_pers.type = TAPI_SIM_PERS_NET;
	lock_pers.pw_len =	8;
	lock_pers.pw = (unsigned char*)malloc(8);
	memcpy(lock_pers.pw,"00000000",8);

	gAsyncResult = FALSE;
					
	TET_CHECK(TAPI_API_SUCCESS,tel_disable_sim_personalization(&lock_pers, &pRequestId));
								
	G_MAIN_LOOP_RUN(2);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;

}

