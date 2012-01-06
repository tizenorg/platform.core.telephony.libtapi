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


#include "uts_tel_answer_call.h"

/* Initialize TCM data structures */

/*
*  tapi_api_err_t tel_exe_call_mo(tel_exe_call_mo_info_t const * const ptr_call_setup_info,
*		tapi_call_handle *ptr_call_handle);
*/
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
 * @brief This tests tel_answer_call() API in normal conditions MO call.
 * @par ID:
 * UTC_TEL_ANSWER_CALL_01
 * @param [in] CallHandle = g_CallHandle
 * @param [in] AnsType = TEL_ANSWER_CALL_ACCEPT
 * @param [out] pRequestId = &RequestId
 * @return TAPI_API_SUCCESS
*/

/***** TESTING FOR VALID SET OF INPUTS*****/

void utc_tel_answer_call_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;
	CALL_TEST_DEBUG("\n /**************************************** Give a MT Call within 20 secs ****************************/\n");
	gmain_loop_for_tapi_events(TIME_DELAY1);
	ret_status = tel_answer_call(g_CallHandle, TAPI_CALL_ANSWER_ACCEPT, &RequestId);
	g_CallHandle = -1;

	TET_CHECK(TAPI_API_SUCCESS, ret_status);
	expected_event = TAPI_EVENT_CALL_ANSWER_CNF;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	TET_CHECK(TAPI_API_SUCCESS, ret_status);
	printf("utc_tel_answer_call_01 pass");
	tet_result(TET_PASS);
	tel_release_call_all(&RequestId);
	gmain_loop_for_tapi_events(TIME_DELAY1);

	return;
}

/**
 * @brief This tests tel_answer_call() API in normal conditions in MT call.
 * @par ID:
 * UTC_TEL_ANSWER_CALL_02
 * @param [in] CallHandle = g_CallHandle
 * @param [in] AnsType = TAPI_CALL_ANSWER_REJECT
 * @param [out] pRequestId = &RequestId
 * @return TAPI_API_SUCCESS
*/

void utc_tel_answer_call_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;

	CALL_TEST_DEBUG("\n /**************************************** Give a MT Call within 20 secs /****************************/\n");

	gmain_loop_for_tapi_events(TIME_DELAY1);

	ret_status = tel_answer_call(g_CallHandle, TAPI_CALL_ANSWER_REJECT, &RequestId);
	g_CallHandle = -1;
	gmain_loop_for_tapi_events(TIME_DELAY1);
	expected_event = TAPI_EVENT_CALL_ANSWER_CNF;
	TET_CHECK(TAPI_API_SUCCESS, ret_status);
	printf("utc_tel_answer_call_02 pass");
	tel_release_call_all(&RequestId);
	tet_result(TET_PASS);
	gmain_loop_for_tapi_events(TIME_DELAY1);

	return;
}

/**
 * @brief This tests tel_answer_call() API in abnormal conditions
 * @par ID:
 * UTC_TEL_ANSWER_CALL_03
 * @param [in] CallHandle = g_CallHandle
 * @param [in] AnsType = TEL_ANSWER_CALL_ACCEPT
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
*/

void utc_tel_answer_call_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;

	ret_status = tel_answer_call(g_CallHandle, TAPI_CALL_ANSWER_ACCEPT, NULL);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	printf("utc_tel_answer_call_03 pass");
	tet_result(TET_PASS);

	return;
}

