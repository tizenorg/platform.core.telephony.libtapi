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
#include "uts_tel_send_ss_ussd_request.h"


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

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_send_ss_ussd_request_01, 1},
	{utc_tel_send_ss_ussd_request_02, 2},
	{utc_tel_send_ss_ussd_request_03, 3},
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
		if(tel_register_event(TAPI_EVENT_SS_USSD_CNF, &subscription_id, (TelAppCallback)ss_utc_callback,NULL) == TAPI_API_SUCCESS)
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
	if(gStartupSuccess == TRUE)
		tel_deregister_event(subscription_id);
	tel_deinit();
	return;
}


/**
 * @brief This tests TelTapiUssdSendRequest() API in normal conditions
 * @par ID:
 * UTC_TAPI_USSD_SEND_REQUEST_001
 * @param [in] forward_type TAPI_CS_FORWARD_TYPE_VOICE_EV
 * @param [in] Condition  TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV
 * @param [out] RequestId
 * @return TAPI_API_SUCCESS
*/

void utc_tel_send_ss_ussd_request_01()
{
	TET_STAMP_START("Positive Test");
	TapiResult_t ret_status = TAPI_API_SUCCESS;
	TelSsUssdMsgInfo_t pAppMsg;
	int pRequestId;

	char ussd_str[25] = {"*123*0#"};
	strncpy(pAppMsg.szUssdString, ussd_str, strlen(ussd_str));
	pAppMsg.UssdStringLength = strlen(pAppMsg.szUssdString);

	ret_status = tel_send_ss_ussd_request(&pAppMsg, &pRequestId);
	tet_printf("%s",TET_RESULT_TO_STRING(ret_status));

	TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(TET_FAIL, exit, TAPI_API_SUCCESS, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_PASS);
exit:
	return;
}

/**
 * @brief Null testing of the pAppMsg for TelTapiUssdSendRequest() API
 * @par ID:
 * UTC_TAPI_USSD_SEND_REQUEST_002
 * @param [in] pAppMsg NULL
 * @param [out] RequestId
 * @return TAPI_API_INVALID_INPUT
*/

void utc_tel_send_ss_ussd_request_02()
{
	TET_STAMP_START("Test for null pAppMsg");

	TapiResult_t ret_status = TAPI_API_SUCCESS;
	TelSsUssdMsgInfo_t *pAppMsg=NULL;
	int pRequestId;

	ret_status = tel_send_ss_ussd_request(pAppMsg, &pRequestId);
	tet_printf("ret_status %d",ret_status);
	tet_infoline("aaa");
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR,ret_status);

	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing of the request ID for TelTapiUssdSendRequest() API
 * @par ID:
 * UTC_TAPI_USSD_SEND_REQUEST_003
 * @param [in] pAppMsg
 * @code
	pAppMsg.Dcs  = 0x0f;
	pAppMsg.IndType = TAPI_USSD_IND_NOTIFY;
	char ussd_str[25] = {"*123*0#"};
	strcpy(pAppMsg.szUssdString,ussd_str);
	pAppMsg.UssdStringLength = strlen(ussd_str);
* @endcode
 * @param [out] RequestId NULL
 * @return TAPI_API_INVALID_INPUT
*/


void utc_tel_send_ss_ussd_request_03()
{
	TET_STAMP_START("Test for null pRequestId");

	TapiResult_t ret_status = TAPI_API_SUCCESS;
	TelSsUssdMsgInfo_t pAppMsg;

	char ussd_str[25] = {"*123*0#"};
	strncpy(pAppMsg.szUssdString, ussd_str, strlen(ussd_str));
	pAppMsg.UssdStringLength = strlen(ussd_str);

	ret_status = tel_send_ss_ussd_request(&pAppMsg, NULL);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR,ret_status);
	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

