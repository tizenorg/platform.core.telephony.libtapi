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
#include "string.h"
#include <malloc.h>

#include <sys/types.h>
#include <signal.h>

#include <pthread.h>
#include "assert.h"
#include "glib.h"
#include <ITapiNetText.h>
#include <tet_api.h>
#include "tet_macro.h"


#include "TapiCommon.h"
#include <gconf/gconf-client.h>

#include "sms_callback_util.h"
#include "uts_tel_write_sms_in_sim.h"



static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_write_sms_in_sim_01, 1},
	{utc_tel_write_sms_in_sim_02, 2},
	{utc_tel_write_sms_in_sim_03, 3},
	{utc_tel_write_sms_in_sim_04, 4},
	{utc_tel_write_sms_in_sim_05, 5},
	{utc_tel_write_sms_in_sim_06, 6},
	{utc_tel_write_sms_in_sim_07, 7},
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

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;

	gStartupSuccess = tel_init();
	if (gStartupSuccess != TAPI_API_SUCCESS) {
		printf("tel_init() Failed gStartupSuccess = %d \n", gStartupSuccess);
	}
	gStartupSuccess = tel_register_event(TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF, &subscription_id, tapi_sms_callback, NULL);

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

		printf("TapiCommonRegisterEvent subscription_id1 is %d gStartupSuccess is %d\n", subscription_id, gStartupSuccess);
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
 * @brief This tests tel_write_sms_in_sim() API in normal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_01
 * @param [in] MsgStatus    = TAPI_NETTEXT_STATUS_READ
 * @param [in] pDataPackage
 * @code
 pDataPackage->SimIndex = 0;
 pDataPackage->MsgLength = pdu_len;
 memcpy(pDataPackage->szData, pPDU, pDataPackage->MsgLength);
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_write_sms_in_sim_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	char *scaNum = "8210911111";
	char *pPDU = "This is a Test SMS";
	int pdu_len = strlen(pPDU);
	TelSmsData_t WriteData;

	memset(&WriteData, 0, sizeof(TelSmsData_t));
	memcpy(WriteData.SmsData.Sca, scaNum, strlen(scaNum));
	WriteData.SmsData.MsgLength = pdu_len;
	memcpy(WriteData.SmsData.szData, pPDU, pdu_len);
	WriteData.MsgStatus = TAPI_NETTEXT_STATUS_READ;

	returnStatus = tel_write_sms_in_sim(&WriteData, &pRequestId);

	gmain_loop_for_events(5000);
	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_write_sms_in_sim_01 pass\n");
	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in abnormal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_02
 * @param [in] MsgStatus   = TAPI_NETTEXT_STATUS_READ - 1
 * @param [in] pDataPackage
 * @code
 pDataPackage->SimIndex = 2;
 pDataPackage->MsgLength = pdu_len;
 memcpy(pDataPackage->szData, pPDU, pDataPackage->MsgLength);
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_write_sms_in_sim_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	char *scaNum = "8210911111";
	char *pPDU = "This is a Test SMS";
	int pdu_len = strlen(pPDU);

	TelSmsData_t WriteData;

	memset(&WriteData, 0, sizeof(TelSmsData_t));
	memcpy(WriteData.SmsData.Sca, scaNum, strlen(scaNum));
	WriteData.SmsData.MsgLength = pdu_len;
	memcpy(WriteData.SmsData.szData, pPDU, pdu_len);
	WriteData.MsgStatus = TAPI_NETTEXT_STATUS_READ - 1;

	returnStatus = tel_write_sms_in_sim(&WriteData, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_write_sms_in_sim_02 pass\n");
	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in abnormal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_03
 * @param [in] MsgStatus   = TAPI_NETTEXT_STATUS_RESERVED + 1
 * @param [in] pDataPackage
 * @code
 pDataPackage->SimIndex = 0;
 pDataPackage->MsgLength = pdu_len;
 memcpy(pDataPackage->szData, pPDU, pDataPackage->MsgLength);
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_write_sms_in_sim_03()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	char *scaNum = "8210911111";
	char *pPDU = "This is a Test SMS";
	int pdu_len = strlen(pPDU);

	TelSmsData_t WriteData;

	memset(&WriteData, 0, sizeof(TelSmsData_t));
	memcpy(WriteData.SmsData.Sca, scaNum, strlen(scaNum));
	WriteData.SmsData.MsgLength = pdu_len;
	memcpy(WriteData.SmsData.szData, pPDU, pdu_len);
	WriteData.MsgStatus = TAPI_NETTEXT_STATUS_RESERVED + 1;

	returnStatus = tel_write_sms_in_sim(&WriteData, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_write_sms_in_sim_03 pass\n");
	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in normal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_04
 * @param [in] MsgStatus   = TAPI_NETTEXT_STATUS_SENT
 * @param [in] pDataPackage
 * @code
 pDataPackage->SimIndex = 1;
 pDataPackage->MsgLength = pdu_len;
 memcpy(pDataPackage->szData, pPDU, pDataPackage->MsgLength);
 memcpy(pDataPackage->pSCA, pSCA, strlen(pSCA));
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_write_sms_in_sim_04()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	char *scaNum = "8210911111";
	char *pPDU = "This is a Test SMS";
	int pdu_len = strlen(pPDU);

	TelSmsData_t WriteData;

	memset(&WriteData, 0, sizeof(TelSmsData_t));
	memcpy(WriteData.SmsData.Sca, scaNum, strlen(scaNum));
	WriteData.SmsData.MsgLength = pdu_len;
	memcpy(WriteData.SmsData.szData, pPDU, pdu_len);
	WriteData.MsgStatus = TAPI_NETTEXT_STATUS_SENT;

	returnStatus = tel_write_sms_in_sim(&WriteData, &pRequestId);
	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_write_sms_in_sim_04 pass\n");
	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in abnormal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_05
 * @param [in] MsgStatus    = TAPI_NETTEXT_STATUS_READ
 * @param [in] pDataPackage = NULL
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_write_sms_in_sim_05()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	returnStatus = tel_write_sms_in_sim(NULL, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_write_sms_in_sim_05 pass\n");
	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in abnormal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_06
 * @param [in] MsgStatus   = TAPI_NETTEXT_STATUS_READ
 * @param [in] pDataPackage
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_write_sms_in_sim_06()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	char *scaNum = "8210911111";
	char *pPDU = "This is a Test SMS";
	int pdu_len = strlen(pPDU);

	TelSmsData_t WriteData;

	memset(&WriteData, 0, sizeof(TelSmsData_t));
	memcpy(WriteData.SmsData.Sca, scaNum, strlen(scaNum));
	WriteData.SmsData.MsgLength = pdu_len;
	memcpy(WriteData.SmsData.szData, pPDU, pdu_len);
	WriteData.MsgStatus = TAPI_NETTEXT_STATUS_SENT;

	returnStatus = tel_write_sms_in_sim(&WriteData, NULL);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_write_sms_in_sim_06 pass\n");
	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in abnormal conditions
 * @par ID:
 * UTC_TEL_WRITE_SMS_07
 * @param [in] MsgStatus    = TAPI_NETTEXT_STATUS_READ
 * @param [in] pDataPackage = NULL
 * @param [out] pRequestId  = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_write_sms_in_sim_07()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	returnStatus = tel_write_sms_in_sim(NULL, NULL);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_write_sms_in_sim_07 pass\n");
	return;
}

/** @} */
