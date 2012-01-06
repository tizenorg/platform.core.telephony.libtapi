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
#include "uts_tel_set_sms_preferred_bearer.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_sms_preferred_bearer_01, 1},
	{utc_tel_set_sms_preferred_bearer_02, 2},
	{utc_tel_set_sms_preferred_bearer_03, 3},
	{utc_tel_set_sms_preferred_bearer_04, 4},
	{utc_tel_set_sms_preferred_bearer_05, 5},
	{utc_tel_set_sms_preferred_bearer_06, 6},
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
	//G_MAIN_CLOSE();
	tel_deinit();
	return;
}

/**
 * @brief This tests tel_set_sms_preferred_bearer() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_PREFERRED_BEARER_01()
 * @param [in] BearerType = TAPI_NETTEXT_BEARER_PS_PREFERRED
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_preferred_bearer_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	int pRequestId = -1;
	printf("SET PREFERRED BEARER TO PS MODE.\n");

	returnStatus = tel_set_sms_preferred_bearer(TAPI_NETTEXT_BEARER_PS_PREFERRED, &pRequestId);

	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_preferred_bearer pass\n");
	else
		printf("utc_tel_set_sms_preferred_bearer fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_set_sms_preferred_bearer() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_PREFERRED_BEARER_02()
 * @param [in] BearerType = TAPI_NETTEXT_BEARER_CS_PREFERRED
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_preferred_bearer_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	int pRequestId = -1;
	printf("SET PREFERRED BEARER TO CS MODE.\n");

	returnStatus = tel_set_sms_preferred_bearer(TAPI_NETTEXT_BEARER_CS_PREFERRED, &pRequestId);

	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_preferred_bearer_02 pass\n");
	else
		printf("utc_tel_set_sms_preferred_bearer_02 fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_set_sms_preferred_bearer() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_PREFERRED_BEARER_03()
 * @param [in] BearerType = TAPI_NETTEXT_NO_PREFERRED_BEARER
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_preferred_bearer_03()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	int pRequestId = -1;
	printf("SET PREFERRED BEARER to No Preferred Mode.\n");

	returnStatus = tel_set_sms_preferred_bearer(TAPI_NETTEXT_NO_PREFERRED_BEARER, &pRequestId);

	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_preferred_bearer_03 pass\n");
	else
		printf("utc_tel_set_sms_preferred_bearer_03 fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_set_sms_preferred_bearer() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_PREFERRED_BEARER_04()
 * @param [in]  BearerType = TAPI_NETTEXT_NO_PREFERRED_BEARER + 1
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_preferred_bearer_04()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	int pRequestId = -1;
	printf("SET PREFERRED BEARER TO INVALID MODE.\n");

	returnStatus = tel_set_sms_preferred_bearer(TAPI_NETTEXT_NO_PREFERRED_BEARER + 1, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_preferred_bearer_04 pass\n");
	else
		printf("utc_tel_set_sms_preferred_bearer_04 fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_set_sms_preferred_bearer() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_PREFERRED_BEARER_05()
 * @param [in] BearerType = TAPI_NETTEXT_BEARER_PS_ONLY - 1
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_preferred_bearer_05()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	int pRequestId = -1;
	printf("SET PREFERRED BEARER TO INVALID MODE.\n");

	returnStatus = tel_set_sms_preferred_bearer(TAPI_NETTEXT_BEARER_PS_ONLY - 1, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_preferred_bearer_05 pass\n");
	else
		printf("utc_tel_set_sms_preferred_bearer_05 fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_set_sms_preferred_bearer() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_PREFERRED_BEARER_05()
 * @param [in]  BearerType = TAPI_NETTEXT_BEARER_PS_PREFERRED
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_set_sms_preferred_bearer_06()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	returnStatus = tel_set_sms_preferred_bearer(TAPI_NETTEXT_BEARER_PS_PREFERRED, NULL);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_set_sms_preferred_bearer_06 pass\n");
	else
		printf("utc_tel_set_sms_preferred_bearer_06 fail\n");

	printf("\n");
	return;
}
/** @} */
