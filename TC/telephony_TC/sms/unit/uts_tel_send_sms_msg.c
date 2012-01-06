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

#include "tet_api.h"
#include "tet_macro.h"

#include <TapiCommon.h>
#include <gconf/gconf-client.h>

#include "sms_callback_util.h"
#include "uts_tel_send_sms_msg.h"
#include "sms_common.h"

#define BOOL int

unsigned int SmsMsgId = 0;

static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_send_sms_msg_01, 1},
	{utc_tel_send_sms_msg_02, 2},
	{utc_tel_send_sms_msg_03, 3},
	{utc_tel_send_sms_msg_04, 4},
	{NULL, 0}
};

TelSmsMsgInfo_t *pDataPackage;
static TapiResult_t gStartupSuccess;
unsigned int subscription_id;

void EncodeCdmaSmsSubmitTpdu(char* diallingNum, int dialNumLen, char* msgTxt, int msg_len);
void alarm_received(int sgi)
{
	gAsyncResult = FALSE;
	printf("Signal [SIGALARM] Received. Close g_main_loop()");
	G_MAIN_LOOP_CLOSE();
}

/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module
 */

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;

	pDataPackage = malloc(sizeof(TelSmsMsgInfo_t));
	memset(pDataPackage, 0, sizeof(TelSmsMsgInfo_t));

	gStartupSuccess = tel_init();
	if (gStartupSuccess != TAPI_API_SUCCESS) {
		printf("tel_init() Failed gStartupSuccess = %d \n", gStartupSuccess);
	}

	gStartupSuccess = tel_register_event(TAPI_EVENT_NETTEXT_SENTSTATUS_CNF, &subscription_id, tapi_sms_callback, NULL);

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

		printf("TapiCommonRegisterEvent subscription_id is %d gStartupSuccess is %d\n", subscription_id, gStartupSuccess);
	}
}

static void cleanup()
{
	free(pDataPackage);
	if (gStartupSuccess == TAPI_API_SUCCESS)
		tel_deregister_event(subscription_id);
	tel_deinit();
	return;
}

/////////////////////


BOOL EncodeCdmaSmsParamTeleservice(int teleservice_id, ParamTeleserv_t *ParamTeleserv)
{
	BOOL ret = FALSE;

	if (ParamTeleserv == NULL) {
		printf("[EncodeCdmaSmsParamTeleservice] NULL pointer  \n");
		return ret;
	}

	switch (teleservice_id) {
		case TELESERVICEID_CPT95:
		case TELESERVICEID_CMT95:
		case TELESERVICEID_VMN95:
		case TELESERVICEID_WAP:
			ret = TRUE;
			break;

		default:
			ret = FALSE;
	}

	if (ret == TRUE) {
		ParamTeleserv->param_id = PARAMID_TELESERVICE_ID;
		ParamTeleserv->param_len = 2;
		ParamTeleserv->teleserv_id = teleservice_id;
	}

	return ret;
}

BOOL EncodeCdmaSmsParamDestAddress(int NumberPlan, char *diallingNum, ParamAddress_t *ParamAddr)
{
	int length = 0;

	if (ParamAddr == NULL || diallingNum == NULL) {
		printf("[EncodeCdmaSmsParamAddress] NULL pointer  \n");
		return FALSE;
	}

	length = strlen(diallingNum); //remove CR

	ParamAddr->param_id = PARAMID_ADDRESS;
	ParamAddr->len = length;
	ParamAddr->digit = DIGITMODE_4BITDTMF;
	ParamAddr->num = NUMMODE_NONE_DATANETWORK;
	ParamAddr->numtype = NUMBER_TYPE_UNKNOWN;
	ParamAddr->numplan = NumberPlan;

	strncpy((char *) ParamAddr->CHARi, diallingNum, length);

	length += 5;
	ParamAddr->param_len = length;

	return TRUE;
}

BOOL EncodeCdmaSmsParamCallbackAddress(int NumberPlan, char *diallingNum, ParamCallback_t *ParamCallback)
{
	int length = 0;

	if (ParamCallback == NULL || diallingNum == NULL) {
		printf("[EncodeCdmaSmsParamAddress] NULL pointer  \n");
		return FALSE;
	}

	length = strlen(diallingNum); //remove CR

	ParamCallback->param_id = PARAMID_CALLBACK;
	ParamCallback->digit = DIGITMODE_4BITDTMF;
	ParamCallback->numtype = NUMBER_TYPE_UNKNOWN;
	ParamCallback->numplan = NumberPlan;
	ParamCallback->len = length;
	strncpy((char *) ParamCallback->CHARi, diallingNum, length);

	length += 4;
	ParamCallback->param_len = length;

	return TRUE;
}

BOOL EncodeCdmaSmsParamMsgId(int type, ParamMsgId_t *ParamMsg)
{
	BOOL ret = FALSE;

	if (ParamMsg == NULL) {
		printf("[EncodeCdmaSmsParamMsgId] NULL pointer  \n");
		return ret;
	}

	ParamMsg->param_id = PARAMID_MESSAGE_ID;
	ParamMsg->param_len = 3;
	ParamMsg->type = type;
	ParamMsg->msg_id = SmsMsgId++;

	ret = TRUE;

	return ret;
}

BOOL EncodeCdmaSmsParamUserData(char *SendTxt, ParamUserData_t *ParamUser)
{
	BOOL ret = FALSE;
	int length = 0;

	if (ParamUser == NULL || SendTxt == NULL) {
		printf("[EncodeCdmaSmsParamUserData] NULL pointer  \n");
		return ret;
	}

	length = strlen(SendTxt);

	ParamUser->param_id = PARAMID_USER_DATA;
	ParamUser->encoding = MSGENCODING_7BIT_ASCII;
	strncpy((char *) ParamUser->CHARi, SendTxt, length);

	ParamUser->len = length;
	length += 2;
	;
	ParamUser->param_len = length;

	ret = TRUE;

	return ret;
}
////////////////////

void EncodeCdmaSmsSubmitTpdu(char *diallingNum, int dialNumLen, char *msgTxt, int msg_len)
{
	ParamTeleserv_t ParamTeleserv = { 0, };
	ParamAddress_t ParamAddr = { 0, };
	ParamCallback_t Paramcallback = { 0, };
	ParamMsgId_t ParamMsg = { 0, };
	ParamUserData_t ParamUser = { 0, };
	int i;
	char CallbackNum[] = "1117908499";

	printf("EncodeCdmaSmsSubmitTpdu Func Enter\n");

	memset(pDataPackage, 0, sizeof(TelSmsMsgInfo_t));
	pDataPackage->MsgType = TAPI_NETTEXT_MESSAGETYPE_SUBMIT;
	EncodeCdmaSmsParamTeleservice(TELESERVICEID_CMT95, &ParamTeleserv);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_TELESERVICE_MASK;
	memcpy(&pDataPackage->MsgData.OutSubmit.TeleService, &ParamTeleserv.teleserv_id, sizeof(unsigned short));
	printf("teleservice at TEST APP=%x\n", pDataPackage->MsgData.OutSubmit.TeleService);

	diallingNum[dialNumLen] = '\0';
	EncodeCdmaSmsParamDestAddress(NUMBER_PLAN_TELEPHONY, diallingNum, &ParamAddr);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_ADDRESS_MASK;
	pDataPackage->MsgData.OutSubmit.DstAddr.Digit = ParamAddr.digit;
	pDataPackage->MsgData.OutSubmit.DstAddr.NumberMode = ParamAddr.num;
	pDataPackage->MsgData.OutSubmit.DstAddr.NumberType = ParamAddr.numtype;
	pDataPackage->MsgData.OutSubmit.DstAddr.NumberPlan = ParamAddr.numplan;
	pDataPackage->MsgData.OutSubmit.DstAddr.szAddrLength = ParamAddr.len;
	memcpy(pDataPackage->MsgData.OutSubmit.DstAddr.szAddress, ParamAddr.CHARi, ParamAddr.len);

	EncodeCdmaSmsParamMsgId(MESSAGETYPE_SUBMIT, &ParamMsg);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_MESSAGE_ID_MASK;
	pDataPackage->MsgData.OutSubmit.MsgId = ParamMsg.type;
	pDataPackage->MsgType = ParamMsg.type;

	EncodeCdmaSmsParamCallbackAddress(NUMBER_PLAN_UNKNOWN, CallbackNum, &Paramcallback);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_CALLBACK_MASK;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.Digit = Paramcallback.digit;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.NumberType = Paramcallback.numtype;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.NumberPlan = Paramcallback.numplan;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.szAddrLength = Paramcallback.len;
	memcpy(pDataPackage->MsgData.OutSubmit.CallBackNumber.szAddress, Paramcallback.CHARi, Paramcallback.len);

	msgTxt[msg_len - 1] = '\0';
	EncodeCdmaSmsParamUserData(msgTxt, &ParamUser);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_USER_DATA_MASK;
	pDataPackage->MsgData.OutSubmit.MsgEncoding = ParamUser.encoding;
	pDataPackage->MsgData.OutSubmit.MsgLength = ParamUser.len;
	memcpy(pDataPackage->MsgData.OutSubmit.szData, ParamUser.CHARi, ParamUser.len);

	printf("\n=============== Test App Message =================\n");
	printf("0000: ");
	for (i = 0; i < sizeof(TelSmsMsgInfo_t); i++) {
		printf("[%2x] ", *(((unsigned char *) pDataPackage) + i));
		if ((i+1)%32 == 0) {
			printf("\n%04x: ", i+1);
		}
	}
	printf("\n==============================================\n");
}

void PrepareCDMAMessage()
{
	int msg_len = 0;
	char buf[512];
	char message[512];
	char diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	int diallingNum_len = 0;

	memset(buf, 0, sizeof(buf));
	memset(diallingNum, 0, sizeof(diallingNum));
	memset(message, 0, sizeof(message));

	strcpy(message, "This is a test message");
	strncpy(diallingNum, RECEIPIENT, TAPI_NETTEXT_ADDRESS_LEN_MAX);

	diallingNum_len = strlen(diallingNum);
	printf("dialling num %s and dialling num len is %d\n", diallingNum, diallingNum_len);
	diallingNum[diallingNum_len] = 0;
	diallingNum_len = diallingNum_len - 1;

	msg_len = strlen(message);

	printf("==========================\n");
	printf("To :%s\n", diallingNum);
	printf("Message: %s\nMsg Length:%d\n", message, msg_len);
	printf("Dialling number Length : %d\n", diallingNum_len);
	printf("==========================\n");

	EncodeCdmaSmsSubmitTpdu(diallingNum, diallingNum_len, message, msg_len);
}

/**
 * @brief This tests tel_send_sms_msg() API in normal conditions
 * @par ID:
 * UTC_TEL_SEND_SMS_01
 * @param [in] pDataPackage   = valid
 * @param [in] bMoreMsgToSend = FALSE
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_send_sms_msg_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	PrepareCDMAMessage();
	returnStatus = tel_send_sms_msg(pDataPackage, 0, &pRequestId);
	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_send_sms_msg_01 pass\n");
	else
		printf("utc_tel_send_sms_msg_01 fail\n");

	printf("\n");
	return;
}

void utc_tel_send_sms_msg_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TS_BOOL bMoreMsgToSend = FALSE;
	int pRequestId = -1;

	returnStatus = tel_send_sms_msg(NULL, bMoreMsgToSend, &pRequestId);
	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_send_sms_msg_02 pass\n");
	else
		printf("utc_tel_send_sms_msg_02 fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_send_sms_msg() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SEND_SMS_03
 * @param [in]  pDataPackage = valid
 * @param [in]  bMoreMsgToSend = FALSE
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_send_sms_msg_03()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TS_BOOL bMoreMsgToSend = FALSE;

	returnStatus = tel_send_sms_msg(pDataPackage, bMoreMsgToSend, NULL);
	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_send_sms_msg_03 pass\n");
	else
		printf("utc_tel_send_sms_msg_03 fail\n");

	printf("\n");
	return;
}

/**
 * @brief This tests tel_send_sms() API in normal conditions
 * @par ID:
 * UTC_TEL_SEND_SMS_04
 * @param [in] pDataPackage   = valid
 * @param [in] bMoreMsgToSend = TRUE
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_send_sms_msg_04()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TS_BOOL bMoreMsgToSend = TRUE;
	int pRequestId = -1;

	PrepareCDMAMessage();
	returnStatus = tel_send_sms_msg(pDataPackage, bMoreMsgToSend, &pRequestId);

	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_send_sms_msg_04 pass\n");
	else
		printf("utc_tel_send_sms_msg_04 fail\n");

	printf("\n");
	return;
}
/** @} */
