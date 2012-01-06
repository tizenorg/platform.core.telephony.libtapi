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

/**
* @ingroup	TELEPHONY_FRAMEWORK_API
* @addtogroup	CALL
*/

/**
* @ingroup	CALL
* @addtogroup	UTS_TEL_DEFLECT_CALL Unit
*/

/**
* @ingroup	UTS_TEL_DEFLECT_CALL Unit
* @addtogroup	UTS_TEL_DEFLECT_CALL_ Uts_Tel_Deflect_Call
* @{
*/

/**
* @file uts_tel_deflect_call.c
* @brief This is a suit of unit test cases to test tel_deflect_call() API function
* @author Gautam Nand Sinha (gnand.sinha@samsung.com)
* @version Initial Creation Version 0.1
* @date 2010.06.29
*/

#include "uts_tel_deflect_call.h"


/* Initialize TCM data structures */





/*
*    TapiResult_t	tel_deflect_call(unsigned int call_handle, const TelCallDeflectDstInfo_t *info, int *pRequestID)
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
 * @brief This tests tel_deflect_call() API in normal conditions
 * @par ID:
 * UTC_TEL_DEFLECT_CALL_01
 * @param [in] CallHandle = callhandle
 * @param [out] info = &info
 * @param [out] pCallStatus = &CallStatus
* @return TAPI_API_SUCCESS
*/

void utc_tel_deflect_call_01()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;
	TS_UINT call_handle;
        TelCallDeflectDstInfo_t info;
	TelCallStatus_t CallStatus;
	TelCallSetupParams_t setup_call_info;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;   //VOICE CALL
	strncpy(setup_call_info.szNumber, normal, 13);
	tet_printf("called number = %s\n", setup_call_info.szNumber);

	//setup_call_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW;

	tel_exe_call_mo(&setup_call_info, &call_handle, &RequestId);

	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	RequestId = -1;

	ret_status  = tel_deflect_call (call_handle,&info,&RequestId);

	tet_printf ("bMoCall = %d \n pNumber = %s\n CallType = %d\n allActiveState=%d\n CallState =%d\n bConferenceState =%d\n", CallStatus.bMoCall, CallStatus.pNumber, CallStatus.CallType, CallStatus.CallActiveState, CallStatus.CallState, CallStatus.bConferenceState);

	
        if(ret_status == TAPI_API_SUCCESS)
        { 
	 printf("utc_tel_deflect_call_01 pass\n");
	 tet_result(TET_PASS);
        }
         else
          { 
	   printf("utc_tel_deflect_call_01 pass\n");
	   tet_result(TET_FAIL);
           }
         

	
}

/**
 * @brief This tests tel_deflect_call() API in normal conditions
 * @par ID:
 * UTC_TEL_DEFLECT_CALL_02
 * @param [in] CallHandle = callhandle
 * @param [out] info = NULL
 * @param [out] pCallStatus = &CallStatus
* @return TAPI_API_INVALID_CALL_HANDLE
*/

void utc_tel_deflect_call_02()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;
	TS_UINT call_handle;
        TelCallDeflectDstInfo_t info;
	TelCallStatus_t CallStatus;
	TelCallSetupParams_t setup_call_info;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;   //VOICE CALL
	strncpy(setup_call_info.szNumber, normal, 13);
	tet_printf("called number = %s\n", setup_call_info.szNumber);

	//setup_call_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW;

	tel_exe_call_mo(&setup_call_info, &call_handle, &RequestId);

	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	RequestId = -1;

	ret_status  = tel_deflect_call (call_handle,NULL,&RequestId);

	tet_printf ("bMoCall = %d \n pNumber = %s\n CallType = %d\n allActiveState=%d\n CallState =%d\n bConferenceState =%d\n", CallStatus.bMoCall, CallStatus.pNumber, CallStatus.CallType, CallStatus.CallActiveState, CallStatus.CallState, CallStatus.bConferenceState);

	
        if(ret_status != TAPI_API_SUCCESS)
        { 
	 printf("utc_tel_deflect_call_02 pass\n");
	 tet_result(TET_PASS);
        }
         else
          { 
	   printf("utc_tel_deflect_call_02 pass\n");
	   tet_result(TET_FAIL);
           }
         

	
}


/**
 * @brief This tests tel_deflect_call() API in normal conditions
 * @par ID:
 * UTC_TEL_DEFLECT_CALL_03
 * @param [in] CallHandle = callhandle
 * @param [out] info = NULL
 * @param [out] pCallStatus = NULL
* @return TAPI_API_INVALID_CALL_HANDLE
*/

void utc_tel_deflect_call_03()
{
	CALL_TEST_DEBUG("\n########     TEST START    ###########\n");
	TapiResult_t  ret_status ;
	int  RequestId = -1;
	TS_UINT call_handle;
        TelCallDeflectDstInfo_t info;
	TelCallStatus_t CallStatus;
	TelCallSetupParams_t setup_call_info;
	char normal[16] = MOB_NUM1;

	memset(&setup_call_info, 0, sizeof(TelCallSetupParams_t));
	setup_call_info.CallType = TAPI_CALL_TYPE_VOICE;   //VOICE CALL
	strncpy(setup_call_info.szNumber, normal, 13);
	tet_printf("called number = %s\n", setup_call_info.szNumber);

	//setup_call_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW;

	tel_exe_call_mo(&setup_call_info, &call_handle, &RequestId);

	expected_event = TAPI_EVENT_CALL_CONNECTED_IND;
	gmain_loop_for_tapi_events(TIME_DELAY1);

	RequestId = -1;

	ret_status  = tel_deflect_call (call_handle,NULL,NULL);

	tet_printf ("bMoCall = %d \n pNumber = %s\n CallType = %d\n allActiveState=%d\n CallState =%d\n bConferenceState =%d\n", CallStatus.bMoCall, CallStatus.pNumber, CallStatus.CallType, CallStatus.CallActiveState, CallStatus.CallState, CallStatus.bConferenceState);

	
        if(ret_status != TAPI_API_SUCCESS)
        { 
	 printf("utc_tel_deflect_call_03 pass\n");
	 tet_result(TET_PASS);
        }
         else
          { 
	   printf("utc_tel_deflect_call_03 pass\n");
	   tet_result(TET_FAIL);
           }
         

	
}



