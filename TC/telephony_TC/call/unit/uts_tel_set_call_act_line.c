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



#include "uts_tel_set_call_act_line.h"


/* Initialize TCM data structures */


/*
*   TapiResult_t tel_set_call_act_line	(TelCallActiveLine_t active_line  ,int *pRequestId);
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
	TapiResult_t  ret_status ;
	int  RequestId = -1;
	ret_status = tel_set_call_act_line(TAPI_CALL_ACTIVE_LINE1 ,&RequestId);
	deregister_all();

	tel_deinit();

}



/**
 * @brief This tests tel_set_call_act_line() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_CALL_ACT_LINE_01
 * @param [in] CallHandle = TAPI_CALL_ACTIVE_LINE1
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_SUCCESS
*/


void utc_tel_set_call_act_line_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;

	ret_status = tel_set_call_act_line(TAPI_CALL_ACTIVE_LINE1 ,&RequestId);

	expected_event = TAPI_EVENT_CALL_SET_LINEID_CNF;
	if (ret_status >= 0)
	{
		TET_CHECK(TAPI_API_SUCCESS, ret_status);

		expected_event = TAPI_EVENT_CALL_SET_LINEID_CNF;
		printf("utc_tel_set_call_act_line_01 pass\n");
		tet_result(TET_PASS);
	}
	return;
}


/**
 * @brief This tests tel_set_call_act_line() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_CALL_ACT_LINE_02
 * @param [in] CallHandle = TAPI_CALL_ACTIVE_LINE2
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_SUCCESS
*/

void utc_tel_set_call_act_line_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;

	ret_status = tel_set_call_act_line(TAPI_CALL_ACTIVE_LINE2 ,&RequestId);

	expected_event = TAPI_EVENT_CALL_SET_LINEID_CNF;
	if (ret_status >= 0)
	{
		TET_CHECK(TAPI_API_SUCCESS, ret_status);

		expected_event = TAPI_EVENT_CALL_SET_LINEID_CNF;
		printf("utc_tel_set_call_act_line_02 pass\n");
		tet_result(TET_PASS);
	}
	return;
}

/**
 * @brief This tests tel_set_call_act_line() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_CALL_ACT_LINE_03
 * @param [in] CallHandle = TAPI_CALL_ACTIVE_LINE2+1
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_call_act_line_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;

	ret_status = tel_set_call_act_line(TAPI_CALL_ACTIVE_LINE2+1 ,&RequestId);


	TET_CHECK(TAPI_API_INVALID_INPUT, ret_status);

	printf("utc_tel_set_call_act_line_03 pass\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_call_act_line() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_CALL_ACT_LINE_04
 * @param [in] CallHandle = TAPI_CALL_ACTIVE_LINE1-1
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_call_act_line_04()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;

	ret_status = tel_set_call_act_line(TAPI_CALL_ACTIVE_LINE1-1 ,&RequestId);


	TET_CHECK(TAPI_API_INVALID_INPUT, ret_status);

	printf("utc_tel_set_call_act_line_04 pass\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_call_act_line() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_CALL_ACT_LINE_05
 * @param [in] CallHandle = TAPI_CALL_ACTIVE_LINE1-1
 * @param [out] pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_set_call_act_line_05()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;

	ret_status = tel_set_call_act_line(TAPI_CALL_ACTIVE_LINE1 ,NULL);


	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	printf("utc_tel_set_call_act_line_05 pass\n");
	tet_result(TET_PASS);

	return;
}





