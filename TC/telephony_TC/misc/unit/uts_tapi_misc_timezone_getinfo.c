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

#include "uts_tapi_misc_timezone_getinfo.h"


static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tapi_misc_timezone_getinfo_001, 1},
	{NULL, 0}
};


static int gStartupSuccess;

void alarm_received(int sgi)
{
	gAsyncResult = FALSE;
	printf("Signal [SIGALARM] Received. Close g_main_loop()");
//	G_MAIN_LOOP_CLOSE();
}


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module */

static void startup()
{
	gStartupSuccess =  tel_init();
	if(gStartupSuccess != TAPI_API_SUCCESS)
	{
		printf("tel_init() Failed gStartupSuccess = %d \n",gStartupSuccess);
	}

	register_all_events();

}

static void cleanup()
{
	if (gStartupSuccess == TAPI_API_SUCCESS)
		{
		//	 G_MAIN_CLOSE();
		}
	deregister_all();
	tel_deinit();
	return;

}




/**
 * @brief This tests
 * @par ID:
 *  UTC_TAPI_MISC_TIMEZONE_GETINFO_001
 * @param [in]  *timezone_info
 * @code
 * @endcode
 * @param [out]  *timezone_info
 * @return int
*/


void utc_tapi_misc_timezone_getinfo_001()
{

	int  returnStatus = TAPI_API_SUCCESS;
	gmain_loop_for_tapi_events(5000);
	returnStatus = tel_get_misc_timezone_info();
	gmain_loop_for_tapi_events(5000);

	printf ("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		printf("utc_tapi_misc_timezone_getinfo_001 pass\n");


}


/** @} */
