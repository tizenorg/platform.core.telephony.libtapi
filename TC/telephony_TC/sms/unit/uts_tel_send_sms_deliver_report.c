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

#include "sms_common.h"
#include "sms_callback_util.h"
#include "uts_tel_send_sms_deliver_report.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_send_sms_deliver_report_01, 1},
	{utc_tel_send_sms_deliver_report_02, 2},
	{utc_tel_send_sms_deliver_report_03, 3},
	{utc_tel_send_sms_deliver_report_04, 4},
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

	gStartupSuccess = tel_register_event(TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF, &subscription_id, tapi_sms_callback, (void *)TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF);
	gStartupSuccess = tel_register_event(TAPI_EVENT_NETTEXT_INCOM_IND, &subscription_id, tapi_sms_callback, (void *)TAPI_EVENT_NETTEXT_INCOM_IND);

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

		printf("after TapiCommonRegisterEvent subscription_id1 is %d gStartupSuccess is %d\n", subscription_id, gStartupSuccess);
	}

	printf("\n## TEST START ##\n");
}

static void cleanup()
{
	if (gStartupSuccess == TAPI_API_SUCCESS)
		tel_deregister_event(subscription_id);
	tel_deinit();

	printf("\n## TEST FINISH ##\n");
	return;
}

void EncodeSmsDeliveryReportTpdu(TelSmsDatapackageInfo_t *pDataPackage)
{
	TPDU_SMS_DELIVER_REPORT deliver_report;
	unsigned char packet[TAPI_NETTEXT_MSG_SIZE_MAX] = {0,};
	char *SCA = VALID_SCA_NUM;

	TS_BYTE tp_dcs;
	TS_BYTE mask = 0x69;
	int index = 0, ScIndex = 0, sca_len;
	int i;

	ScIndex = SmsUtilEncodeAddrField(pDataPackage->Sca, SCA, strlen(SCA), 0x01, 0x01);
	sca_len = strlen(SCA);

	/* TP-MTI , TP-UDHI */
	deliver_report.msg_type = SMS_TPDU_DELIVER_REPORT;
	deliver_report.udhi = FALSE;

	packet[index] = deliver_report.msg_type; //DELIVER REPORT
	packet[index] |= deliver_report.udhi ? 0x40 : 0; //bit 6

	index++;

	/* TP -PI */
	deliver_report.paraInd = mask;
	packet[index] = deliver_report.paraInd;

	if ((mask != 0xFF) && (mask != 0)) {
		index++;
		/* TP-PID */
		if (mask & 0x01) {
			deliver_report.pId = 0x65;
			packet[index++] = deliver_report.pId;
		}

		/* TP-DCS */
		if (mask & 0x02) {
			deliver_report.dcs.bCompressed = FALSE;
			deliver_report.dcs.bmsg_class_set = TRUE;
			deliver_report.dcs.coding_group_type = TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;
			deliver_report.dcs.alphabet_type = TAPI_NETTEXT_ALPHABET_DEFAULT;
			deliver_report.dcs.class_type = TAPI_NETTEXT_CLASS_2;

			SmsUtilEncodeDCS(&tp_dcs, &deliver_report.dcs);
			packet[index++] = tp_dcs;
		}
		/* TP-UDL */
		if (mask & 0x04) {
			deliver_report.udl = 0x00;
			packet[index++] = deliver_report.udl;

			/* TP-UD */

			if (deliver_report.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT) {
				//SmsUtilPackGSMCode( packet + index, msg, msg_len);
			}
		}
	}
	printf("EncodeSmsDeliveryReportTpdu: Send packet to phonseSever[%d] : ", index);

	for (i = 0; i < index; i++) {
		printf("[%02x]", packet[i]);
		if (i % 10 == 9)
			printf("\n");
	}
	printf("\n");

	memcpy(pDataPackage->szData, packet, index);
	pDataPackage->MsgLength = index;
	printf("- msgLength:%d\n", pDataPackage->MsgLength);
	printf("- SCA Number:[%s], SCA Length:%d\n", SCA, sca_len);
	printf("- SCA Addr:");

	for (i = 0; i < sca_len; i++) {
		printf("%02x ",  pDataPackage->Sca[i]);
		if (i % 10 == 9)
			printf("  ");
	}
	printf("\n\n");

	return;
}

/**
 * @brief This tests tel_send_sms_deliver_report() API in normal conditions
 * @par ID:
 * UTC_TEL_SEND_DELIVERY_REPORT_01
 * @param [in] pDataPackage = valid
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_send_sms_deliver_report_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = 1;
	TelSmsDatapackageInfo_t deliveryReportDataPackage = {{0,},0,{0,},};

	printf("/******************************************\n");
	printf("* Please send a message to tesing target  *\n");
	printf("* within 60 seconds !!!                   *\n");
	printf("*******************************************\n");

	EncodeSmsDeliveryReportTpdu(&deliveryReportDataPackage);
	returnStatus = tel_send_sms_deliver_report(&deliveryReportDataPackage, TAPI_NETTEXT_SENDSMS_SUCCESS, &pRequestId);

	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	/* return TAPI_API_OEM_PLUGIN_FAILURE is ok. because call tel_send_sms_deliver_report() without tel_send_sms() */
	TET_CHECK(returnStatus == TAPI_API_OEM_PLUGIN_FAILURE);
	if (returnStatus == TAPI_API_OEM_PLUGIN_FAILURE)
		printf("utc_tel_send_sms_deliver_report_01 pass\n");
	else
		printf("utc_tel_send_sms_deliver_report_01 fail\n");

	return;
}

/**
 * @brief This tests tel_send_sms_deliver_report() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SEND_DELIVERY_REPORT_02
 * @param [in] pDataPackage = valid
 * @param [out] pRequestId  = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_send_sms_deliver_report_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TelSmsDatapackageInfo_t deliveryReportDataPackage;
	EncodeSmsDeliveryReportTpdu(&deliveryReportDataPackage);

	returnStatus = tel_send_sms_deliver_report(&deliveryReportDataPackage, TAPI_NETTEXT_SENDSMS_SUCCESS, NULL);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_send_sms_deliver_report_02 pass\n");
	else
		printf("utc_tel_send_sms_deliver_report_02 fail\n");

	return;
}

/**
 * @brief This tests tel_send_sms_deliver_report() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SEND_DELIVERY_REPORT_03
 * @param [in] pDataPackage = NULL
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_send_sms_deliver_report_03()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	returnStatus = tel_send_sms_deliver_report(NULL, TAPI_NETTEXT_SENDSMS_SUCCESS, &pRequestId);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_send_sms_deliver_report_03 pass\n");
	else
		printf("utc_tel_send_sms_deliver_report_03 fail\n");

	return;
}

/**
 * @brief This tests tel_write_sms_in_sim() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SEND_DELIVERY_REPORT_04
 * @param [in]  pDataPackage = NULL
 * @param [out] pRequestId  = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_send_sms_deliver_report_04()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	returnStatus = tel_send_sms_deliver_report(NULL, TAPI_NETTEXT_SENDSMS_SUCCESS, NULL);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_send_sms_deliver_report_04 pass\n");
	else
		printf("utc_tel_send_sms_deliver_report_04 fail\n");

	return;
}
/** @} */
