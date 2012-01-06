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




#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_set_sim_language.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tel_set_sim_language_01, 1},
	{utc_tel_set_sim_language_02, 2},
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

	case TAPI_EVENT_SIM_SET_LANGUAGE_CNF:
        {
		tet_printf("\n[SIM APP]***************TAPI_EVENT_SIM_SET_LANGUAGE_CNF*****************************\n");
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
    tet_infoline("utc_tel_set_sim_language Test Start\n");
	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if ( TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_SET_LANGUAGE_CNF,&subscription_id,(TelAppCallback)sim_app_callback, NULL) ) 
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
    tet_infoline("utc_tel_set_sim_langauge Test Finished\n");

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


void utc_tel_set_sim_language_01()
{
	TET_STAMP_START("utc_tel_set_sim_language_01");

    TelSimLanguagePreferenceCode_t language = 0x00;
	TET_CHECK(TAPI_API_INVALID_PTR,tel_set_sim_language(language,NULL));
	TET_STAMP_PASS();
	return;
}


void utc_tel_set_sim_language_02()
{
	TET_STAMP_START("utc_tel_set_sim_language_02");

    TelSimLanguagePreferenceCode_t language = 0x00;
    int req_id = 0;

	gAsyncResult = FALSE;
				
	TET_CHECK(TAPI_API_SUCCESS,tel_set_sim_language(language,&req_id));
							
	G_MAIN_LOOP_RUN(2);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}

/** @} */
