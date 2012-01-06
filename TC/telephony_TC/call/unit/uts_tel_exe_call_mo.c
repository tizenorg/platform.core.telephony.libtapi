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



#include "uts_tel_exe_call_mo.h"
#include "TelCall.h"
/* Initialize TCM data structures */

/*
* TapiResult_t    tel_exe_call_mo (const TelCallSetupParams_t* pParams,unsigned int  * pCallHandle, int * );

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
 * @brief This tests tel_exe_call_mo() API in normal conditions
 * @par ID:
 * UTC_TEL_EXE_CALL_MO_01
 * @param [in] pParams = &setup_call_info
 * @param [out] pCallHandle = &active_call_handle
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_SUCCESS
*/
void utc_tel_exe_call_mo_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	unsigned int active_call_handle = -1;
	TelCallSetupParams_t setup_call_info;
	int  RequestId = -1;
	char normal[16] = {0};
	strncpy(normal, MOB_NUM1, 16);


	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;
	strncpy(setup_call_info.szNumber, normal,16);
	tet_printf("called number = %s\n", setup_call_info.szNumber);
	CALL_TEST_DEBUG("called number = %s\n", setup_call_info.szNumber);

	//setup_call_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW; 		//preet: TelCallSetupParams_t dont have these members
        //setup_call_info.pCugInfo = NULL;
	//setup_call_info.pUusInfo = NULL;
	ret_status =  tel_exe_call_mo(&setup_call_info, &active_call_handle, &RequestId);

	tet_printf ("setup_call_handle_01 = %d\t request_id_01 =%d\n", active_call_handle,RequestId);
	CALL_TEST_DEBUG ("setup_call_handle_01 = %d\t request_id_01 =%d\n", active_call_handle,RequestId);

	TET_CHECK(TAPI_API_SUCCESS, ret_status);
	tet_infoline("utc_tel_exe_call_mo_01 pass");
	tet_result(TET_PASS);
	CALL_TEST_DEBUG("i am out of 1st test case \n");

	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	RequestId = -1;
	tel_release_call_all(&RequestId);
	expected_event = TAPI_EVENT_CALL_END_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	printf("utc_tel_exe_call_mo_01 pass");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_exe_call_mo() API in abnormal conditions
 * @par ID:
 * UTC_TEL_EXE_CALL_MO_02
 * @param [in] pParams = NULL
 * @param [out] pCallHandle = &call_handle
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_exe_call_mo_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	unsigned int call_handle;
	int  RequestId = -1;

	ret_status =  tel_exe_call_mo(NULL, &call_handle, &RequestId);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	tet_printf ("setup_call_handle_02 = %d\t request_id_02 =%d\n", call_handle,RequestId);

	printf("utc_tel_exe_call_mo_02 pass");
	tet_result(TET_PASS);

	return;
}


/**
 * @brief This tests tel_exe_call_mo() API in abnormal conditions
 * @par ID:
 * UTC_TEL_EXE_CALL_MO_03
 * @param [in] pParams = &setup_call_info
 * @param [out] pCallHandle = NULL
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_exe_call_mo_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status;
	TelCallSetupParams_t setup_call_info;
	int  RequestId = -1;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;   //VOICE CALL
	strncpy(setup_call_info.szNumber, normal, sizeof(normal));
	ret_status =  tel_exe_call_mo(&setup_call_info, NULL, &RequestId);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	tet_printf (" request_id_03 =%d\n",RequestId);

	printf("utc_tel_exe_call_mo_03 pass");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_exe_call_mo() API in abnormal conditions
 * @par ID:
 * UTC_TEL_EXE_CALL_MO_04
 * @param [in] pParams = &setup_call_info
 * @param [out] pCallHandle = &active_call_handle
 * @param [out] pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_exe_call_mo_04()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	unsigned int call_handle = INVALID_CALL_HANDLE;
	TelCallSetupParams_t setup_call_info;
	int  RequestId = -1;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;   //VOICE CALL
	strncpy(setup_call_info.szNumber, normal, sizeof(normal));
	ret_status =  tel_exe_call_mo(&setup_call_info, &call_handle, NULL);

	tet_printf ("setup_call_handle_04 = %d\t request_id_04 =%d\n", call_handle,RequestId);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	printf("utc_tel_exe_call_mo_04 pass");
	tet_result(TET_PASS);

	return;
}

