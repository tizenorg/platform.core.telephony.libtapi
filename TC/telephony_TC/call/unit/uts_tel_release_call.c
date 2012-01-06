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
#include <ITapiSs.h>

#include "uts_tel_release_call.h"


extern TS_UINT g_CallHandle;

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
	g_CallHandle = -1;

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
	g_CallHandle = -1;

	deregister_all();

	tel_deinit();
}

/**
TapiResult_t	tel_release_call ( unsigned int  CallHandle, int *pRequestId);
*/


/**
 * @brief This tests tel_release_call() API in normal conditions (MO CALL)
 * @par ID:
 * UTC_TEL_RELEASE_CALL_01
 * @param [in] CallHandle = active_call_handle
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_SUCCESS
*/


void utc_tel_release_call_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	unsigned int active_call_handle = 0;
	TelCallSetupParams_t setup_call_info;
	int  RequestId = -1;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;
	strncpy(setup_call_info.szNumber, normal, 13);
	tet_printf("called number = %s\n", setup_call_info.szNumber);

	//setup_call_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW;	//preet: TelCallSetupParams_t dont ahve these members
    	//setup_call_info.pCugInfo = NULL;
	//setup_call_info.pUusInfo = NULL;

	CALL_TEST_DEBUG("MAKE A MO CALL TO %s",setup_call_info.szNumber);
	ret_status =  tel_exe_call_mo(&setup_call_info, &g_CallHandle, &RequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_status);
	expected_event = TAPI_EVENT_CALL_SETUP_CNF;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	CALL_TEST_DEBUG("RELEASE A CALL %s\n",setup_call_info.szNumber);
	ret_status = tel_release_call (g_CallHandle,&RequestId );

	expected_event = TAPI_EVENT_CALL_END_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	TET_CHECK(TAPI_API_SUCCESS, ret_status);

	tet_infoline("utc_tel_release_call_01");
	tet_result(TET_PASS);

	return;
}


/**
 * @brief This tests tel_release_call() API in abnormal conditions
 * @par ID:
 * UTC_TEL_RELEASE_CALL_02
 * @param [in] CallHandle = INVALID_CALL_HANDLE
 * @param [out] pRequestId = &RequestId
* @return TAPI_API_INVALID_CALL_HANDLE
*/

void utc_tel_release_call_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	unsigned int call_handle = -1;//INVALID_CALL_HANDLE;
	int  RequestId = -1;

	ret_status = tel_release_call (call_handle ,&RequestId);

	printf ("utc_tel_release_call_02  RequestId = %d\n", RequestId);

	TET_CHECK(TAPI_API_COULD_NOT_GET_CALL_CTXT, ret_status);

	tet_infoline("utc_tel_release_call_02");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_release_call() API in abnormal conditions (MT CALL)
 * @par ID:
 * UTC_TEL_RELEASE_CALL_03
 * @param [in] CallHandle = g_CallHandle
 * @param [out] pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/


void utc_tel_release_call_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	unsigned int call_handle;

	ret_status = tel_release_call (call_handle,NULL );

	TET_CHECK(TAPI_API_INVALID_PTR, ret_status);

	tet_infoline("utc_tel_release_call_03");
	tet_result(TET_PASS);

	return;
}

