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


#include <ITapiCall.h>
#include <ITapiSs.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>

#include <pthread.h>
#include "assert.h"
#include "glib.h"

#include "TapiCommon.h"
#include <gconf/gconf-client.h>
#include "string.h"

#include "tet_tapi_util.h"
#include <tet_api.h>

#include "uts_tel_set_ss_waiting.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_ss_waiting_01, 1},
	{utc_tel_set_ss_waiting_02, 2},
	{utc_tel_set_ss_waiting_03, 3},
	{utc_tel_set_ss_waiting_04, 4},
	{utc_tel_set_ss_waiting_05, 5},
	{utc_tel_set_ss_waiting_06, 6},
	{utc_tel_set_ss_waiting_07, 7},
	{NULL, 0}
};


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;
	
	gStartupSuccess = FALSE;
	if(tel_init() == TAPI_API_SUCCESS)
	{
		if(tel_register_event(TAPI_EVENT_SS_WAITING_CNF, &subscription_id, (TelAppCallback)ss_utc_callback,NULL) == TAPI_API_SUCCESS)
		{
			gStartupSuccess = TRUE;
			tel_register_app_name("org.samsung.test");
			if(tel_get_app_name(&dname))
				printf("Dbus Get app name error\n");
			else 
				printf("Dbus Name is %s\n",dname.name);
				
			tel_check_service_ready(&bStatus);
			printf("Telephony service  %d\n",bStatus);
		}
		else
		{
			tet_infoline("startup fail. tel_register_event()");
		}
	}else {
		tet_infoline("startup fail. tel_init() failed");
	}

	return;
}

static void cleanup()
{
	if(gStartupSuccess == TRUE)
		tel_deregister_event(subscription_id);
	tel_deinit();
	return;
}

/**
 * @brief This tests TelTapiSsSetCallWaiting() API in normal conditions
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_001
 * @param [in] waitInfo
 * @code
 *    	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
 *	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_ACTIVATE;
* @endcode
* @param [out] RequestId
* @return TAPI_API_SUCCESS
*/

void utc_tel_set_ss_waiting_01()
{
	TET_STAMP_START("This tests TelTapiSsSetCallWaiting() API in normal conditions");
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	TelSsWaitingInfo_t waitInfo;

	int request_id=0;
	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_ACTIVATE;

	ret_status = tel_set_ss_waiting(&waitInfo, &request_id);

	TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(TET_FAIL, exit, TAPI_API_SUCCESS, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_PASS);
exit:
	return;
}

/**
 * @brief Lower boundary testing of the call  type for TelTapiSsSetCallWaiting() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_002
 * @param [in] waitInfo
 * @code
 *   	waitInfo.CallType = (TelSsCallType_t)-1;
 *	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_ACTIVATE;
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_waiting_02()
{
	TET_STAMP_START("Lower boundary testing of the call  type for TelTapiSsSetCallWaiting() API");

	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	TelSsWaitingInfo_t waitInfo;
	int request_id=0;

	waitInfo.CallType = (TelSsCallType_t)-1;
	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_ACTIVATE;

	ret_status = tel_set_ss_waiting(&waitInfo, &request_id);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}


/**
 * @brief Upper boundary testing of the call  type for TelTapiSsSetCallWaiting() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_003
 * @param [in] waitInfo
 * @code
*     	waitInfo.CallType = (TelSsCallType_t)(TAPI_CALL_TYPE_ALL_TELE+1);
*	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_ACTIVATE;
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_waiting_03()
{
	TET_STAMP_START("Upper boundary testing of the call  type for TelTapiSsSetCallWaiting() API");
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	TelSsWaitingInfo_t waitInfo;
	waitInfo.CallType = (TelSsCallType_t)(TAPI_CALL_TYPE_ALL_TELE+1);
	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_ACTIVATE;
	int request_id=0;

	ret_status = tel_set_ss_waiting(&waitInfo, &request_id);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}


/**
 * @brief Lower boundary testing of the waiting operation mode for TelTapiSsSetCallWaiting() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_004
 * @param [in] waitInfo
 * @code
 *   	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
*	waitInfo.Mode =(TelSsCallWaitOperationMode_t)-1;
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_waiting_04()
{
	TET_STAMP_START("Lower boundary testing of the waiting operation mode for TelTapiSsSetCallWaiting() API");
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	TelSsWaitingInfo_t waitInfo;
	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	waitInfo.Mode =(TelSsCallWaitOperationMode_t)-1;
	int request_id=0;

	ret_status = tel_set_ss_waiting(&waitInfo, &request_id);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Upper boundary testing of the waiting operation mode for TelTapiSsSetCallWaiting() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_005
 * @param [in] waitInfo
 * @code
 *    	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
 *	waitInfo.Mode =(TelSsCallWaitOperationMode_t)(TAPI_SS_CW_DEACTIVATE+1);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_waiting_05()
{
	TET_STAMP_START("Upper boundary testing of the waiting operation mode for TelTapiSsSetCallWaiting() API");
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	TelSsWaitingInfo_t waitInfo;
	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	waitInfo.Mode =(TelSsCallWaitOperationMode_t)(TAPI_SS_CW_DEACTIVATE+1);
	int request_id=0;

	ret_status = tel_set_ss_waiting(&waitInfo, &request_id);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing of the waitInfo for TelTapiSsSetCallWaiting() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_006
 * @param [in] waitInfo NULL
* @param [out] RequestId
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_set_ss_waiting_06()
{
	TET_STAMP_START("Null testing of the waitInfo for TelTapiSsSetCallWaiting() API");
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	int request_id=0;

	ret_status = tel_set_ss_waiting(NULL, &request_id);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing of the request ID for TelTapiSsSetCallWaiting() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_WAITING_007
 * @param [in] waitInfo
  * @code
 * waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
* waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_DEACTIVATE;
* @param [out] RequestId NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_set_ss_waiting_07()
{
	TET_STAMP_START("Null testing of the request ID for TelTapiSsSetCallWaiting() API");
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	TelSsWaitingInfo_t waitInfo;
	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	waitInfo.Mode =(TelSsCallWaitOperationMode_t)TAPI_SS_CW_DEACTIVATE;

	ret_status = tel_set_ss_waiting(&waitInfo, NULL);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}


/** @} */


