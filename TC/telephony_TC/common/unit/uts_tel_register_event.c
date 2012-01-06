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
#include "tapi_common.h"
#include "tet_macro.h"
#include "uts_tel_register_event.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tapi_register_event_01, 1},
//	{utc_tapi_register_event_02, 2},
//	{utc_tapi_register_event_03, 3},
	{utc_tapi_register_event_04, 4},
	{utc_tapi_register_event_05, 5},
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
	nw_context_loop = g_main_loop_new (NULL, FALSE);
	g_timeout_add ( (sec * 1000), g_timeout_callback, NULL);
	g_main_loop_run (nw_context_loop);

	return 0;
}

// Global variable for user data
static int gAsyncResult; //user defined data : Async callback is called or not

// G_MAIN Utility : time out callback function
gboolean g_timeout_callback (void *data)
{
	gAsyncResult = FALSE;
	tet_printf("[SIM APP][TIME OUT] g_timeout called .Close g_main_roop()");
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static int gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

static void startup()
{
    tet_infoline("utc_tapi_register_event Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{

		gStartupSuccess = TRUE;

	} else {
		tet_infoline("startup fail. tel_init() failed");
	}

    return;
}


static void cleanup()
{
    tet_infoline("utc_tapi_update_register_event Test Finished\n");

    if (gStartupSuccess == TRUE)
	{
		tel_deregister_event(subscription_id);
	}
	tel_deinit();

    return;
}

int register_app_callback(TelTapiEvent_t *Event)
{
		tet_printf("utc_tapi_register_event_01  - call back called\n");
		return TRUE;
}



void utc_tapi_register_event_01()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;

	returnStatus = tel_register_event(TAPI_EVENT_CLASS_DATA,&subscription_id,(TelAppCallback)register_app_callback,NULL);

	tet_printf ("The return value is %d\n", returnStatus);

	TET_CHECK_EQUAL(returnStatus , TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		tet_printf("utc_tapi_register_event_01 pass\n");

}

/*
void utc_tapi_register_event_02()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;

	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_register_event(TAPI_EVENT_CALL_SETUP_CNF - 1,&subscription_id,(TelAppCallback)register_app_callback,NULL);
	gmain_loop_for_tapi_events(TIME_DELAY);

	printf ("The return value is %d\n", returnStatus);

	TET_CHECK_EQUAL(returnStatus,TAPI_EVENT_CLASS_UNKNOWN);
	if(returnStatus == TAPI_EVENT_CLASS_UNKNOWN)
		printf("utc_tapi_register_event_02 pass\n");

}


void utc_tapi_register_event_03()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;

	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_register_event((int)NULL,&subscription_id,(TelAppCallback)register_app_callback,NULL);
	gmain_loop_for_tapi_events(TIME_DELAY);

	printf ("The return value is %d\n", returnStatus);
	if(returnStatus == TAPI_API_SUCCESS)
		tet_result(TET_FAIL);
	
	tet_result(TET_PASS);
	printf("utc_tapi_register_event_03 pass\n");

}
*/
void utc_tapi_register_event_04()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;

	returnStatus = tel_register_event(TAPI_EVENT_CALL_SETUP_CNF,NULL,(TelAppCallback)register_app_callback,NULL);
	tet_printf ("The return value is %d\n", returnStatus);
	if(returnStatus == TAPI_API_SUCCESS)
		tet_result(TET_FAIL);
	
	tet_result(TET_PASS);
	tet_printf("utc_tapi_register_event_04 pass\n");

}

void utc_tapi_register_event_05()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;

	returnStatus = tel_register_event(TAPI_EVENT_CALL_SETUP_CNF ,&subscription_id,(TelAppCallback)NULL,NULL);

	tet_printf ("The return value is %d\n", returnStatus);
	if(returnStatus == TAPI_API_SUCCESS)
		tet_result(TET_FAIL);
	
	tet_result(TET_PASS);
	tet_printf("utc_tapi_register_event_05 pass\n");

}

/** @} */









