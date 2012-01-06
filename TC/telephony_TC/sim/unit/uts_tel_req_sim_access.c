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
* @ingroup	TELEPHONY_FRAMEWORK_API
* @addtogroup	SIM
*/

/**
* @ingroup	SIM
* @addtogroup	UTS_TEL_REQ_SIM_ACCESS Unit
*/

/**
* @ingroup	UTS_TEL_REQ_SIM_ACCESS Unit
* @addtogroup	UTS_TEL_REQ_SIM_ACCESS_ Uts_Tel_Req_Sim_Access
* @{
*/

/**
* @file uts_tel_req_sim_access.c
* @brief This is a suit of unit test cases to test tel_req_sim_access() API function
* @author Gautam Nand Sinha (gnand.sinha@samsung.com)
* @version Initial Creation Version 0.1
* @date 2010.06.29
*/


#include "uts_tel_req_sim_access.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sim_access_01, 1},
	{utc_tel_req_sim_access_02, 2},
	{utc_tel_req_sim_access_03, 3},
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
static TelSimPinOperationResult_t gSimEventStatus;

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
        case TAPI_EVENT_SIM_RSIM_ACCESS_CNF:
        {
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_RSIM_ACCESS_CNF (%d) ***************************\n", TAPI_EVENT_SIM_GET_MAILBOX_CNF);
            tet_printf("SIM rsim access event status = [%x]\n", sim_event->Status);

            gSimEventStatus = sim_event->Status;
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

        default:
        {
            gSimEventStatus = sim_event->Status;
            gAsyncResult = FALSE;
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
    tet_infoline("utc_tel_req_sim_access Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_RSIM_ACCESS_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("utc_tel_req_sim_access Test Finished\n");
    if (gStartupSuccess == TRUE)
	{
		tel_deregister_event(subscription_id);
	}
	tel_deinit();
    return;
}


void utc_tel_req_sim_access_01()
{
	TET_STAMP_START("utc_tel_req_sim_access_01");

    int request=0;
    TelSimRSimReq_t sim = {0,};
    sim.file_id = 0x6F60;
    sim.rsim_cmd = TAPI_SIM_GET_RESPONSE;

	gAsyncResult = FALSE;
					
	TET_CHECK(TAPI_API_SUCCESS,tel_req_sim_access(&sim,&request));
								
	G_MAIN_LOOP_RUN(2);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


void utc_tel_req_sim_access_02()
{
	TET_STAMP_START("utc_tel_req_sim_access_02");

    TelSimRSimReq_t sim = {0,};
    sim.file_id = 0x6F60;
    sim.rsim_cmd = TAPI_SIM_GET_RESPONSE;
	TET_CHECK(TAPI_API_INVALID_PTR,tel_req_sim_access(&sim,NULL));
	TET_STAMP_PASS();
	return;
}


void utc_tel_req_sim_access_03()
{
	TET_STAMP_START("utc_tel_req_sim_access_03");

    int requestId = 0;
	TET_CHECK(TAPI_API_INVALID_PTR,tel_req_sim_access(NULL,&requestId));
	TET_STAMP_PASS();
	return;
}

/** @} */
