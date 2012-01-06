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
#include "uts_tel_check_service_ready.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_check_service_ready_01, 1},
	{utc_tel_check_service_ready_02, 2},
	{NULL, 0}
};
#if 0
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
#endif

/*
  User Defined Handler
*/


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("uts_tel_get_sim_imsi Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("uts_tel_get_sim_imsi Test Finished\n");
	if (tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
	return;
}

void utc_tel_check_service_ready_01()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int bstatus;

	returnStatus = tel_check_service_ready(&bstatus);

	tet_printf ("The return value is %d\n", returnStatus);

	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_result(TET_PASS);
		tet_printf("utc_tel_check_service_ready_01 success\n");
	}
	else
	{
		tet_result(TET_FAIL);	
		tet_printf("utc_tel_check_service_ready_01 failed\n");
	}
}



void utc_tel_check_service_ready_02()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	
	returnStatus = tel_check_service_ready(NULL);

	tet_printf ("The return value is %d\n", returnStatus);

	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_result(TET_FAIL);
		tet_printf("utc_tel_check_service_ready_02 failed\n");
	}
	tet_result(TET_PASS);
//returnStatus = tel_deinit(); //Deinitializin already deinitialized tapi
}

/** @} */








