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
#include "uts_tel_change_ss_barring_password.h"

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
#include "string.h"

#include "tet_tapi_util.h"
#include <ss_common.h>

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_change_ss_barring_password_01, 1},
	{utc_tel_change_ss_barring_password_02, 2},
	{utc_tel_change_ss_barring_password_03, 3},
	{utc_tel_change_ss_barring_password_04, 4},
	{utc_tel_change_ss_barring_password_05, 5},
	{utc_tel_change_ss_barring_password_06, 6},
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
		if(tel_register_event(TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF, &subscription_id, (TelAppCallback)ss_utc_callback,NULL) == TAPI_API_SUCCESS)
		{
			gStartupSuccess = TRUE;
			tel_register_app_name("org.samsung.test");
			if(tel_get_app_name(&dname))
				printf("Dbus Get app name error\n");
			else 
				printf("Dbus Name is %s\n",dname.name);

			tel_check_service_ready(&bStatus);
			printf("Telephpny service  %d\n",bStatus);

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
 * @brief This tests TelTapiSsChangeBarPassword() API in normal conditions
 * @par ID:
 * UTC_TAPI_SS_CHANGE_BAR_PASSWORD_001
 * @param [in] oldPwd "0000"
 * @param [in] newPwd "1111"
* @param [out] RequestId
* @return TAPI_API_SUCCESS
*/

void utc_tel_change_ss_barring_password_01()
{
	TET_STAMP_START("Positive Test");


	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId;

	char *oldPwd=OLD_PWD;
	char *newPwd=OLD_PWD;
	char *newPwdAgain=OLD_PWD;

	ret_status = tel_change_ss_barring_password(oldPwd,newPwd,newPwdAgain, &pRequestId);

	TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(TET_FAIL, exit, TAPI_API_SUCCESS, ret_status );
	G_MAIN_LOOP_RUN(5);

	tet_result(TET_PASS);
exit:
	return;
}


/**
 * @brief Error handling of the old password for TelTapiSsChangeBarPassword() API
 * @par ID:
 * UTC_TAPI_SS_CHANGE_BAR_PASSWORD_002
 * @param [in] oldPwd "00000"  //Long password
 * @param [in] newPwd "1111"
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_change_ss_barring_password_02()
{
	TET_STAMP_START("Invalid long old password");


	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId;
	char *oldPwd="00000";
	char *newPwd="1111";
        char *newPwdAgain="1111";

	ret_status = tel_change_ss_barring_password(oldPwd,newPwd,newPwdAgain, &pRequestId);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Error handling of the new password for TelTapiSsChangeBarPassword() API
 * @par ID:
 * UTC_TAPI_SS_CHANGE_BAR_PASSWORD_003
 * @param [in] oldPwd "0000"
 * @param [in] newPwd "11111"   //Long password
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_change_ss_barring_password_03()
{
	TET_STAMP_START("Invalid long new password");


	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId;
	char *oldPwd="0000";
	char *newPwd="11111";
	char *newPwdAgain="11111";

	ret_status = tel_change_ss_barring_password(oldPwd,newPwd,newPwdAgain, &pRequestId);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_INPUT, ret_status);

	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing of the old password for TelTapiSsChangeBarPassword() API
 * @par ID:
 * UTC_TAPI_SS_CHANGE_BAR_PASSWORD_004
 * @param [in] oldPwd NULL
 * @param [in] newPwd "1111"
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_change_ss_barring_password_04()
{
	TET_STAMP_START("Null old password");


	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId;
	char *oldPwd=NULL;
	char *newPwd="1111";
	char *newPwdAgain="1111";

	ret_status = tel_change_ss_barring_password(oldPwd,newPwd,newPwdAgain, &pRequestId);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status);

	G_MAIN_LOOP_RUN(7);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing of the new password for TelTapiSsChangeBarPassword() API
 * @par ID:
 * UTC_TAPI_SS_CHANGE_BAR_PASSWORD_005
 * @param [in] oldPwd "0000"
 * @param [in] newPwd NULL
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_change_ss_barring_password_05()
{
	TET_STAMP_START("Null new password");


	TapiResult_t ret_status = TAPI_API_SUCCESS;
	int pRequestId;
	char *oldPwd="0000";
	char *newPwd=NULL;
	char *newPwdAgain=NULL;

	ret_status = tel_change_ss_barring_password(oldPwd,newPwd,newPwdAgain, &pRequestId);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}

/**
 * @brief Null testing of the request ID for TelTapiSsChangeBarPassword() API
 * @par ID:
 * UTC_TAPI_SS_CHANGE_BAR_PASSWORD_006
 * @param [in] oldPwd "0000"
 * @param [in] newPwd "1111"
* @param [out] RequestId
* @return TAPI_API_INVALID_INPUT
*/

void utc_tel_change_ss_barring_password_06()
{
	TET_STAMP_START("Null RequstId");


	TapiResult_t ret_status = TAPI_API_SUCCESS;
	char *oldPwd="0000";
	char *newPwd="1111";
        char *newPwdAgain="1111";

	ret_status = tel_change_ss_barring_password(oldPwd,newPwd,newPwdAgain, NULL);

	TET_RESULT_IF_EQUAL_THEN_GOTO(TET_PASS, exit, TAPI_API_INVALID_PTR, ret_status);

	G_MAIN_LOOP_RUN(5);

	tet_result(TET_FAIL);
exit:
	return;
}



/** @} */
