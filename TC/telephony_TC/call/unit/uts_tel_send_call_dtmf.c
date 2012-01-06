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



#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
#include<unistd.h>

#include "uts_tel_send_call_dtmf.h"



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
*   TapiResult_t	tel_send_call_dtmf (const char * pDtmfString, int *pRequestId);
*/


/**
 * @brief This tests tel_send_call_dtmf () API in normal condition (MO call)
 * @par ID:
 * UTC_TEL_SEND_CALL_DTMF_01
 * @param [in] pDtmfString = DTMF_STRING
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_SUCCESS
*/

void utc_tel_send_call_dtmf_01()
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
	ret_status =  tel_exe_call_mo(&setup_call_info, &call_handle, &RequestId);

	TET_CHECK(TAPI_API_SUCCESS, ret_status);

	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	ret_status = tel_send_call_dtmf(DTMF_STRING , &RequestId);

	TET_CHECK(TAPI_API_SUCCESS, ret_status);

	expected_event = TAPI_EVENT_CALL_SEND_DTMF_CNF;
	gmain_loop_for_tapi_events(TIME_DELAY1);
	printf("utc_tel_send_call_dtmf_func_01 pass");
	tet_result(TET_PASS);

	tel_release_call_all(&RequestId);

	expected_event = TAPI_EVENT_CALL_END_IND;
    gmain_loop_for_tapi_events(TIME_DELAY1);

	return;
}

/**
 * @brief This tests tel_send_call_dtmf () API in abnormal conditions
 * @par ID:
 * UTC_TEL_SEND_CALL_DTMF_03
 * @param [in] pDtmfString = DTMF_STRING
 * @param [out] pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_send_call_dtmf_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;

	ret_status = tel_send_call_dtmf (DTMF_STRING , NULL);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	printf("utc_tel_send_call_dtmf_func_03 pass");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_send_call_dtmf () API in abnormal conditions
 * @par ID:
 * UTC_TEL_SEND_CALL_DTMF_04
 * @param [in] pDtmfString = NULL
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_send_call_dtmf_04()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;

	int  RequestId = -1;
	ret_status = tel_send_call_dtmf (NULL , &RequestId);

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	printf("utc_tel_send_call_dtmf_func_04 pass");
	tet_result(TET_PASS);

	return;
}

