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


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>

#include <sys/types.h>
#include <signal.h>

#include <pthread.h>
#include <assert.h>
#include <glib.h>
#include <ITapiNetText.h>

#include <tet_api.h>
#include "tet_macro.h"

#include <TapiCommon.h>
#include <gconf/gconf-client.h>

#include "sms_callback_util.h"
#include "uts_tel_set_sms_message_status.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_sms_message_status_01, 1},
	{utc_tel_set_sms_message_status_02, 2},
	{utc_tel_set_sms_message_status_03, 3},
	{utc_tel_set_sms_message_status_04, 4},
	{utc_tel_set_sms_message_status_05, 5},
	{utc_tel_set_sms_message_status_06, 6},
	{utc_tel_set_sms_message_status_07, 7},
	{utc_tel_set_sms_message_status_08, 8},
	{NULL, 0}
};

static TapiResult_t gStartupSuccess;
unsigned int subscription_id;

void alarm_received(int sgi)
{
	gAsyncResult = FALSE;
	printf("Signal [SIGALARM] Received. Close g_main_loop()");
	G_MAIN_LOOP_CLOSE();
}

/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module */

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;

	gStartupSuccess = tel_init();
	if (gStartupSuccess != TAPI_API_SUCCESS) {
		printf("tel_init() Failed gStartupSuccess = %d \n", gStartupSuccess);
	}
	gStartupSuccess = tel_register_event(TAPI_EVENT_NETTEXT_SET_REQUEST_CNF, &subscription_id, tapi_sms_callback, NULL);

	if (gStartupSuccess != TAPI_API_SUCCESS) {
		printf("TapiCommonRegisterEvent Failed gStartupSuccess = %d \n", gStartupSuccess);
	}
	else {
		tel_register_app_name("org.samsung.test");
		if (tel_get_app_name(&dname))
			printf("Dbus Get app name error\n");
		else
			printf("Dbus Name is %s\n", dname.name);

		tel_check_service_ready(&bStatus);
		printf("Telephony service  %d\n", bStatus);

		printf("after TapiCommonRegisterEvent subscription_id is %d gStartupSuccess is %d\n", subscription_id, gStartupSuccess);
	}
}

static void cleanup()
{
	if (gStartupSuccess == TAPI_API_SUCCESS)
		tel_deregister_event(subscription_id);
	tel_deinit();
	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_01
 * @param [in] Index     = 0
 * @param [in] msgStatus = TAPI_NETTEXT_STATUS_READ
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_message_status_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(0, TAPI_NETTEXT_STATUS_READ, &pRequestId);
	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_message_status_01 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_02
 * @param [in]  Index     = 0
 * @param [in]  msgStatus = TAPI_NETTEXT_STATUS_UNREAD
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_message_status_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(1, TAPI_NETTEXT_STATUS_UNREAD, &pRequestId);
	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_message_status_02 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_03
 * @param [in] Index     = 1
 * @param [in] msgStatus = TAPI_NETTEXT_STATUS_SENT
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_message_status_03()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(1, TAPI_NETTEXT_STATUS_SENT, &pRequestId);
	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_message_status_03 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_04
 * @param [in] Index     = -1
 * @param [in] msgStatus = TAPI_NETTEXT_STATUS_UNSENT
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_message_status_04()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(-1, TAPI_NETTEXT_STATUS_UNSENT, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_message_status_04 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_05
 * @param [in] Index     = 256
 * @param [in] msgStatus = TAPI_NETTEXT_STATUS_RESERVED
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_message_status_05()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(256, TAPI_NETTEXT_STATUS_RESERVED, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_message_status_05 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_06
 * @param [in] Index     = 1
 * @param [in] msgStatus = TAPI_NETTEXT_STATUS_RESERVED +1
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_message_status_06()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(1, TAPI_NETTEXT_STATUS_RESERVED + 1, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_message_status_06 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_07
 * @param [in] Index     = 1
 * @param [in] msgStatus = TAPI_NETTEXT_STATUS_READ - 1
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_message_status_07()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(1, TAPI_NETTEXT_STATUS_READ - 1, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_message_status_07 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_message_status() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_MESSAGE_STATUS_08
 * @param [in] Index       = 1
 * @param [in] msgStatus   = TAPI_NETTEXT_STATUS_DELIVERED
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_set_sms_message_status_08()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	/* CALLING TAPI SET MSG STATUS API */

	returnStatus = tel_set_sms_message_status(1, TAPI_NETTEXT_STATUS_DELIVERED, NULL);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_set_sms_message_status_08 pass\n");

	return;
}
/** @} */
