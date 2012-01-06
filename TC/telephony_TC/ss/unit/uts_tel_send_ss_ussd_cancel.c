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
#include "uts_tel_send_ss_ussd_cancel.h"
#include <ITapiCall.h>
#include <ITapiSs.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include "TapiCommon.h"
#include "tet_tapi_util.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_send_ss_ussd_cancel_01, 1},
	{utc_tel_send_ss_ussd_cancel_02, 2},
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
		if(tel_register_event(TAPI_EVENT_SS_USSD_CANCEL_CNF, &subscription_id, (TelAppCallback)ss_utc_callback,NULL) == TAPI_API_SUCCESS)
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
 * @brief This tests TelTapiUssdSendCancel() API in normal conditions
 * @par ID:
 * UTC_TAPI_USSD_SEND_CANCEL_001
 * @param [out] RequestId
 * @return TAPI_API_SUCCESS
*/

void utc_tel_send_ss_ussd_cancel_01()
{
	TET_STAMP_START("Positive Test");
	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId;

	ret_status = tel_send_ss_ussd_cancel(&pRequestId);
//	tet_printf("%s",TET_RESULT_TO_STRING(ret_status));

//	TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(TET_FAIL, exit, TAPI_API_SUCCESS, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_PASS);
	return;
}


/**
 * @brief This tests TelTapiUssdSendCancel() API in normal conditions
 * @par ID:
 * UTC_TAPI_USSD_SEND_CANCEL_002
 * @param [out] RequestId
 * @return TAPI_API_SUCCESS
*/

void utc_tel_send_ss_ussd_cancel_02()
{
	TET_STAMP_START("Test with Null Parameter");
	TapiResult_t ret_status = TAPI_API_SUCCESS;

	ret_status = tel_send_ss_ussd_cancel(NULL);
	tet_printf("ret_status %d",ret_status);
//	tet_infoline("aaa");
	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR,ret_status);

	G_MAIN_LOOP_RUN(7);
	tet_result(TET_FAIL);
	exit:
		return;
}
