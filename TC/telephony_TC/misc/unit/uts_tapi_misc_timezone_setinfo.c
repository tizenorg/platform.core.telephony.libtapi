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

#include "uts_tapi_misc_timezone_setinfo.h"


static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tapi_misc_timezone_setinfo_001, 1},
	{utc_tapi_misc_timezone_setinfo_002, 2},
	{NULL, 0}
};


static TapiResult_t gStartupSuccess;

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

	gStartupSuccess =  TelTapiInit();
	if(gStartupSuccess != TAPI_API_SUCCESS)
	{
		printf("TelTapiInit() Failed gStartupSuccess = %d \n",gStartupSuccess);
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
	TelTapiDeInit();
	return;

}




/**
 * @brief This tests
 * @par ID:
 *  UTC_TAPI_MISC_TIMEZONE_SETINFO_001
 * @param [in]  *timezone_info
 * @code
 * @endcode
 * @param [out]  *timezone_info
 * @return TapiResult_t
*/


void utc_tapi_misc_timezone_setinfo_001()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	tapi_misc_time_zone_info_type timezone_info = {0,};
	timezone_info.daylight_valid=1;
	timezone_info.year=10;
	timezone_info.month=1;
	timezone_info.day=1;
	timezone_info.hour=1;
	timezone_info.minute=1;
	timezone_info.second=1;
	timezone_info.time_zone=36;
	timezone_info.time_zone_valid=TAPI_MISC_NITZ_RTC_BKUP_PHONE;
	timezone_info.daylight_adjust=TAPI_MISC_NITZ_NOADJUST;
	timezone_info.day_of_week =TAPI_MISC_NITZ_DOW_FRI;
	
	gmain_loop_for_tapi_events(5000);
	returnStatus = tapi_misc_timezone_setinfo(&timezone_info);
	gmain_loop_for_tapi_events(5000);

	printf ("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		printf("utc_tapi_misc_timezone_setinfo_001 pass\n");


}



void utc_tapi_misc_timezone_setinfo_002()
{

	TapiResult_t  returnStatus = 1;
	TelTapiDeInit();
	gmain_loop_for_tapi_events(5000);
	//De-initializing Tapi for invalid test case
	returnStatus = tapi_misc_timezone_setinfo(NULL);
	gmain_loop_for_tapi_events(5000);

	printf ("The return value is %d\n", returnStatus);
	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_result(TET_FAIL);
	}
	tet_result(TET_PASS);
}

/** @} */
