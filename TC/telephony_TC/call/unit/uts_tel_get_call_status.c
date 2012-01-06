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


#include "uts_tel_get_call_status.h"


/* Initialize TCM data structures */





/*
*    TapiResult_t	tel_get_call_status ( unsigned int  CallHandle, TelCallStatus_t * pCallStatus);
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
	int RequestId;
	tel_release_call_all(&RequestId);
	expected_event = TAPI_EVENT_CALL_END_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);
	deregister_all();

	tel_deinit();

}


/**
 * @brief This tests tel_get_call_status() API in normal conditions
 * @par ID:
 * UTC_TEL_GET_CALL_STATUS_01
 * @param [in] CallHandle = callhandle
 * @param [out] pCallStatus = &CallStatus
* @return TAPI_API_SUCCESS
*/

void utc_tel_get_call_status_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;
	TS_UINT call_handle;
	TelCallStatus_t CallStatus;
	TelCallSetupParams_t setup_call_info;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;   //VOICE CALL
	strncpy(setup_call_info.szNumber, normal, 13);
	tet_printf("called number = %s\n", setup_call_info.szNumber);

	//setup_call_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW;

	ret_status=tel_exe_call_mo(&setup_call_info, &call_handle, &RequestId);
	tet_printf ("call_handle = %d\n",call_handle);
	TET_CHECK(TAPI_API_SUCCESS, ret_status);
	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	RequestId = -1;
	tet_printf ("call_handle = %d\n",call_handle);
	ret_status  = tel_get_call_status (call_handle, &CallStatus);

	tet_printf ("bMoCall = %d \n pNumber = %s\n CallType = %d\n allActiveState=%d\n CallState =%d\n bConferenceState =%d\n", CallStatus.bMoCall, CallStatus.pNumber, CallStatus.CallType, CallStatus.CallActiveState, CallStatus.CallState, CallStatus.bConferenceState);

	TET_CHECK(TAPI_API_SUCCESS, ret_status);

	printf("utc_tel_get_call_status_01 pass\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_get_call_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_GET_CALL_STATUS_02
 * @param [in] CallHandle = callhandle
 * @param [out] pCallStatus = NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_get_call_status_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;

	ret_status  = tel_get_call_status (VALID_CALL_HANDLE, NULL);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);
	printf("utc_tel_get_call_status_02 pass\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_get_call_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_GET_CALL_STATUS_03
 * @param [in] CallHandle = INVALID_CALL_HANDLE
 * @param [out] pCallStatus = &CallStatus
* @return TAPI_API_INVALID_CALL_HANDLE
*/
void utc_tel_get_call_status_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	TelCallStatus_t CallStatus;

	ret_status  = tel_get_call_status (INVALID_CALL_HANDLE, &CallStatus);

	TET_CHECK(TAPI_API_INVALID_CALL_HANDLE, ret_status);
	printf("utc_tel_get_call_status_03 pass\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_get_call_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_GET_CALL_STATUS_04
 * @param [in] CallHandle = INVALID_CALL_HANDLE
 * @param [out] pCallStatus = NULL
 * @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_call_status_04()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;

	ret_status  = tel_get_call_status (INVALID_CALL_HANDLE, NULL);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);
	printf("utc_tel_get_call_status_04 pass\n");
	tet_result(TET_PASS);

	return;
}



