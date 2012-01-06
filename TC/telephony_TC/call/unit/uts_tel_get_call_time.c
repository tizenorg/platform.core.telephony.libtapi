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

#include <TapiCommon.h>
#include "uts_tel_get_call_time.h"

/* Initialize TCM data structures */

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

extern int expected_event;

static void startup()
{
	TapiResult_t  ret_status ;
	tapi_dbus_connection_name dname;
	int bStatus;
	ret_status = tel_init();

	if(ret_status != TAPI_API_SUCCESS)
	{
		printf("tel_init() Failed ret_status = %d \n",ret_status);
	}


	register_all_events();
	tel_register_app_name("org.samsung.test");
	if(tel_get_app_name(&dname))
		printf("Dbus Get app name error\n");
	else 
		printf("Dbus Name is %s\n",dname.name);
		
	tel_check_service_ready(&bStatus);
	printf("Telephpny service  %d\n",bStatus);

	g_CallHandle = -1;
}

static void cleanup()
{
	deregister_all();

	tel_deinit();
}

/**
 * @brief This tests () API in normal conditions
 * @par ID:
 * UTC_TEL_GET_CALL_TIME_01
 * @param [out] status 
 * @return TAPI_API_SUCCESS
*/

/***** TESTING FOR VALID SET OF INPUTS*****/

void utc_tel_get_call_time_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	TS_WORD req_mask = 1;
       	int *pRequestID;
	ret_status = tel_get_call_time (req_mask, &pRequestID); 
	if(ret_status !=TAPI_API_SUCCESS)
	{
		tet_result(TET_FAIL);
	}	
	printf("utc_tel_get_call_time_01 pass");
	tet_result(TET_PASS);

	return;
}

void utc_tel_get_call_time_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	TS_WORD req_mask = 1;
	ret_status = tel_get_call_time (req_mask, NULL); 
	if(ret_status ==TAPI_API_SUCCESS)
	{
		tet_result(TET_FAIL);
	}	
	printf("utc_tel_get_call_time_02 pass");
	tet_result(TET_PASS);

	return;
}
