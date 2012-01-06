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
#include <gconf/gconf-client.h>
#include "string.h"

#include "tet_tapi_util.h"
#include "uts_tel_set_ss_forward.h"
#include "ss_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_ss_forward_01, 1},
	{utc_tel_set_ss_forward_02, 2},
	{utc_tel_set_ss_forward_03, 3},
	{utc_tel_set_ss_forward_04, 4},
	{utc_tel_set_ss_forward_05, 5},
	{utc_tel_set_ss_forward_06, 6},
	{utc_tel_set_ss_forward_07, 7},
	{utc_tel_set_ss_forward_08, 8},
	{utc_tel_set_ss_forward_09, 9},
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
		if(tel_register_event(TAPI_EVENT_SS_FORWARD_CNF, &subscription_id, (TelAppCallback)ss_utc_callback,NULL) == TAPI_API_SUCCESS)
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
	TelSsForwardInfo_t pForwardInfo;
	int len;

	int pRequestId=0;
    	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ERASURE_EV;
    	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_ALL_TELE;
    	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 0;

    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strncpy(pForwardInfo.szPhoneNumber, cf_number, TAPI_CALL_DIALDIGIT_LEN_MAX);

    	tel_set_ss_forward(&pForwardInfo,&pRequestId);
	G_MAIN_LOOP_RUN(5);
	printf("CF ERASE\n");
	if(gStartupSuccess == TRUE)
		tel_deregister_event(subscription_id);
	tel_deinit();
	return;
}


/**
 * @brief This tests TelTapiSsSetCallForwarding() API in normal conditions
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_001
 * @param [in] pForwardInfo
 * @code
     	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_REGISTRATION_EV;
    	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 0;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_SUCCESS
*/

void utc_tel_set_ss_forward_01()
{
	TET_STAMP_START("This tests TelTapiSsSetCallForwarding() API in normal conditions");
	TapiResult_t ret_status = TAPI_API_SUCCESS;
	TelSsForwardInfo_t pForwardInfo;
	int len;

	int pRequestId=0;
    	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_REGISTRATION_EV;
    	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 0;

    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strncpy(pForwardInfo.szPhoneNumber, cf_number, TAPI_CALL_DIALDIGIT_LEN_MAX);

	printf("CF REGISTRATION\n");
    	ret_status =  tel_set_ss_forward(&pForwardInfo,&pRequestId);

	TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(TET_FAIL, exit, TAPI_API_SUCCESS, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_PASS);
exit:
	return;
}

/**
 * @brief Lower boundary testing of the forward mode for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_002
 * @param [in] pForwardInfo
 * @code
     	pForwardInfo.Mode = (TelSsForwardMode_t)-1;
    	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 10;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_forward_02()
{
	TET_STAMP_START("Invalid forward mode -1");
	//invalid mode
	TapiResult_t ret_status = TAPI_API_SUCCESS;

	int pRequestId=0;
	TelSsForwardInfo_t pForwardInfo;
	pForwardInfo.Mode = (TelSsForwardMode_t)-1;//TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
	pForwardInfo.NoReplyConditionTimer = 10;

	int len;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);

    	memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

	ret_status = tel_set_ss_forward(&pForwardInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Upper boundary testing of the forward mode for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_003
 * @param [in] pForwardInfo
 * @code
     	pForwardInfo.Mode = (TelSsForwardMode_t)(TAPI_CALL_FORWARD_MODE_ERASURE_EV+1);
    	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 10;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_forward_03()
{
	//invalid mode
	TET_STAMP_START("Invalid forward mode +1");
 	TapiResult_t ret_status = TAPI_API_SUCCESS;

	TelSsForwardInfo_t pForwardInfo;
	pForwardInfo.Mode = (TelSsForwardMode_t)(TAPI_CALL_FORWARD_MODE_ERASURE_EV+1);//TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
	pForwardInfo.NoReplyConditionTimer = 10;
	int pRequestId=0;
	int len;
	char cf_number[20]= {FWD_NUM};
	len = strlen(cf_number);

	memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

	ret_status =  tel_set_ss_forward(&pForwardInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Lower boundary testing of the forward type for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_004
 * @param [in] pForwardInfo
 * @code
	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = (TelSsForwardType_t)-1;
   	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 10;

    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_forward_04()
{
	//invalid type
	TET_STAMP_START("Invalid forward type -1");
    TapiResult_t ret_status = TAPI_API_SUCCESS;
int pRequestId=0;
    TelSsForwardInfo_t pForwardInfo;
    pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
    pForwardInfo.Type = (TelSsForwardType_t)-1;//TAPI_CS_FORWARD_TYPE_VOICE_EV;
    pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    pForwardInfo.NoReplyConditionTimer = 10;

    int len;
    char cf_number[20]= {FWD_NUM};
    len = strlen(cf_number);

    memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

    ret_status =  tel_set_ss_forward(&pForwardInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Upper boundary testing of the forward type for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_005
 * @param [in] pForwardInfo
 * @code
	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = (TelSsForwardType_t)(TAPI_CS_FORWARD_TYPE_ALL_TELE +1);
   	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    	pForwardInfo.NoReplyConditionTimer = 10;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_forward_05()
{
	//invalid type
	TET_STAMP_START("Invalid forward type +1");
    TapiResult_t ret_status = TAPI_API_SUCCESS;
int pRequestId=0;
    TelSsForwardInfo_t pForwardInfo;
    pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
    pForwardInfo.Type = (TelSsForwardType_t)(TAPI_CS_FORWARD_TYPE_ALL_TELE +1);
    pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    pForwardInfo.NoReplyConditionTimer = 10;

    int len;
    char cf_number[20]= {FWD_NUM};
    len = strlen(cf_number);

    memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

    ret_status =  tel_set_ss_forward(&pForwardInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Lower boundary testing of the forward condition for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_006
 * @param [in] pForwardInfo
 * @code
	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = (TelSsForwardWhen_t)-1;
    	pForwardInfo.NoReplyConditionTimer = 10;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_forward_06()
{
	//invalid condition
	TET_STAMP_START("Invalid forward condition -1");
    TapiResult_t ret_status = TAPI_API_SUCCESS;
int pRequestId=0;
    TelSsForwardInfo_t pForwardInfo;
    pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
    pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    pForwardInfo.Condition = (TelSsForwardWhen_t)-1;
    pForwardInfo.NoReplyConditionTimer = 10;

    int len;
    char cf_number[20]= {FWD_NUM};
    len = strlen(cf_number);

    memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

    ret_status =  tel_set_ss_forward(&pForwardInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Upper boundary testing of the forward condition for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_007
 * @param [in] pForwardInfo
 * @code
	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = (TelSsForwardWhen_t)(TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV+1);
    	pForwardInfo.NoReplyConditionTimer = 10;
    	char cf_number[20]= {FWD_NUM};
    	len = strlen(cf_number);
    	strcpy(pForwardInfo.szPhoneNumber,cf_number);
* @endcode
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_set_ss_forward_07()
{
	//invalid condition
	TET_STAMP_START("Invalid Forward Condition +1");
    TapiResult_t ret_status = TAPI_API_SUCCESS;
int pRequestId=0;
    TelSsForwardInfo_t pForwardInfo;
    pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
    pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    pForwardInfo.Condition = (TelSsForwardWhen_t)(TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV+1);
    pForwardInfo.NoReplyConditionTimer = 10;

    int len;
    char cf_number[20]= {FWD_NUM};
    len = strlen(cf_number);

    memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

    ret_status =  tel_set_ss_forward(&pForwardInfo,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}


/**
 * @brief Null test of the Request ID for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_008
 * @param [in] pForwardInfo
 * @code
 pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
    pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
    pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
    pForwardInfo.NoReplyConditionTimer = 10;

    int len;
    char cf_number[20]= {FWD_NUM};
    len = strlen(cf_number);
    memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);
 * @endcode
* @param [out] RequestId NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_set_ss_forward_08()
{
	//null out param
	TET_STAMP_START("Null Request ID");
	TapiResult_t ret_status = TAPI_API_SUCCESS;
	TelSsForwardInfo_t pForwardInfo;
	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
	pForwardInfo.NoReplyConditionTimer = 10;

	int len;
	char cf_number[20]= {FWD_NUM};
	len = strlen(cf_number);
	memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);

	ret_status =  tel_set_ss_forward(&pForwardInfo,NULL);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}


/**
 * @brief Null test of the forward info for TelTapiSsSetCallForwarding() API
 * @par ID:
 * UTC_TAPI_SS_SET_CALL_FORWARDING_009
 * @param [in] pForwardInfo
 * @endcode
* @param [out] RequestId NULL
* @return TAPI_API_INVALID_PTR
*/

void utc_tel_set_ss_forward_09()
{
	//null out param
	TET_STAMP_START("Null Forward Info");
	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId=0;

	ret_status =  tel_set_ss_forward(NULL,&pRequestId);
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/** @} */
