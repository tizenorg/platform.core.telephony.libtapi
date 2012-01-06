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


#include <tet_api.h>
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
//#include "tapi_utility.h"
#include <gconf/gconf-client.h>
//#include "tapi_proxy_utility.h"
#include "string.h"

#include "tet_tapi_util.h"
#include "uts_tel_set_ss_barring.h"
#include <ss_common.h>

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_ss_barring_01, 1},
	{utc_tel_set_ss_barring_02, 2},
	{utc_tel_set_ss_barring_03, 3},
	{utc_tel_set_ss_barring_04, 4},
	{utc_tel_set_ss_barring_05, 5},
	{utc_tel_set_ss_barring_06, 6},
	{utc_tel_set_ss_barring_07, 7},
	{utc_tel_set_ss_barring_08, 8},
	{utc_tel_set_ss_barring_09, 9},
	{utc_tel_set_ss_barring_10, 10},
	{utc_tel_set_ss_barring_11, 11},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static int gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;
	
	gStartupSuccess = FALSE;
	if(tel_init() == TAPI_API_SUCCESS)
	{
		if(tel_register_event(TAPI_EVENT_SS_BARRING_CNF, &subscription_id, (TelAppCallback)ss_utc_callback,NULL) == TAPI_API_SUCCESS)
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
			tet_infoline("startup fail. tel_register_event()");
	}else {
		tet_infoline("startup fail. tel_init() failed");
	}

	return;
}

static void cleanup()
{
	//Erase the service registration
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};
	int pRequestId=0;
	char bar_pwd[5]=BARRING_PWD;//{'0','0','0','0',};
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_DEACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL;
	pBarringInfo.CallType = TAPI_CALL_TYPE_ALL_TELE;
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	tel_set_ss_barring(&pBarringInfo,&pRequestId);
	G_MAIN_LOOP_RUN(5);

	if(gStartupSuccess == TRUE)
		tel_deregister_event(subscription_id);
	tel_deinit();
	return;
}


/**
 * @brief This tests TelTapiSsSetCallBarring() API in normal conditions
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_001
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5]="0000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_DEACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_SUCCESS
*/

void utc_tel_set_ss_barring_01()
{
	TET_STAMP_START("Positive Test");

	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};
	int pRequestId=0;
	char bar_pwd[5]=BARRING_PWD;//{'0','0','0','0',};
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_DEACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strncpy(pBarringInfo.szPassword,bar_pwd,4);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(TET_FAIL, exit, TAPI_API_SUCCESS, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_PASS);
exit:
	return;
}


/**
 * @brief Lower boundary testing of the barring mode for TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_002
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = (TelSsCallBarOperationMode_t)-1;//TAPI_SS_CALL_BAR_DEACTIVATE+1
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_02()
{
	TET_STAMP_START("Invalid Operation Mode -1");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[5] =BARRING_PWD;
	pBarringInfo.Mode = (TelSsCallBarOperationMode_t)-1;//TAPI_SS_CALL_BAR_DEACTIVATE+1
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strncpy(pBarringInfo.szPassword,bar_pwd,4);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Upper boundary testing of the barring mode for TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_003
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = (TelSsCallBarOperationMode_t)-1;//TAPI_SS_CALL_BAR_DEACTIVATE+1
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_03()
{
	TET_STAMP_START("Invalid Operation Mode +1");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[5] =BARRING_PWD;
	pBarringInfo.Mode = (TelSsCallBarOperationMode_t)(TAPI_SS_CALL_BAR_DEACTIVATE+1);
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;}

/**
 * @brief Lower boundary testing of the call type for TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_004
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = (TelSsCallType_t)-1;
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_04()
{
	TET_STAMP_START("Invalid Call Type -1");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[5] =BARRING_PWD;
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = (TelSsCallType_t)-1;
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;}

/**
 * @brief Upper boundary testing of the call type for TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_005
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = (TelSsCallType_t)TAPI_CALL_TYPE_ALL_TELE+1;
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_05()
{
	TET_STAMP_START("Invalid Call Type +1");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[5] =BARRING_PWD;
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = (TelSsCallType_t)TAPI_CALL_TYPE_ALL_TELE+1;
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Lower boundary testing of the barring type for TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_006
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = (TelSsCallBarType_t)-1;
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_06()
{
	TET_STAMP_START("Invalid Barring Type -1");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[5] =BARRING_PWD;
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = (TelSsCallBarType_t)-1;//TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Upper boundary testing of the barring type for TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_007
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = (TelSsCallBarType_t)(TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1);
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/


void utc_tel_set_ss_barring_07()
{
	TET_STAMP_START("Invalid Barring Type +1");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[5] =BARRING_PWD;
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = (TelSsCallBarType_t)(TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1);
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Error handling for the invaild long password of TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_008
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[6] ="00000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_08()
{
	 TET_STAMP_START("Invalid short password");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char bar_pwd[6] ="000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strncpy(pBarringInfo.szPassword,bar_pwd,4);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Error handling for the invaild long password of TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_009
 * @param [in] pBarringInfo
 * @code
	char bar_pwd[6] ="00000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strcpy(pBarringInfo.szPassword,bar_pwd);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_09()
{
	TET_STAMP_START("Null RequestId");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	char bar_pwd[5] ="0000";
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,NULL);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status );
	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing for pBarringInfo of TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_010
 * @param [in] pBarringInfo NULL
 * @param [out] RequestId
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_set_ss_barring_10()
{
	TET_STAMP_START("Null BarringInfo");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;

	int pRequestId=0;

	ret_status = tel_set_ss_barring(NULL,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status );
	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing for the barring password of TelTapiSsSetCallBarring() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_BARRING_011
 * @param [in] pBarringInfo
  * @code
	char *bar_pwd = NULL;
	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
* @endcode
 * @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_barring_11()
{
	TET_STAMP_START("Test for null password");
	TapiResult_t  ret_status = TAPI_API_SUCCESS;
	TelSsCallBarringInfo_t  pBarringInfo;// = {0,};

	int pRequestId=0;
	char *bar_pwd = "\0";

	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM+1
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;//TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV+1
	strncpy(pBarringInfo.szPassword, bar_pwd, TAPI_SS_GSM_BARR_PW_LEN_MAX);

	ret_status = tel_set_ss_barring(&pBarringInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status );
	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/** @} */
