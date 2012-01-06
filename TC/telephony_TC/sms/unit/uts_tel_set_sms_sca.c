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
#include <assert.h>
#include <glib.h>
#include <ITapiNetText.h>

#include <TapiCommon.h>
#include <gconf/gconf-client.h>

#include <tet_api.h>
#include "tet_macro.h"

#include "sms_callback_util.h"
#include "uts_tel_set_sms_sca.h"
#include "sms_common.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_sms_sca_01, 1},
	{utc_tel_set_sms_sca_02, 2},
	{utc_tel_set_sms_sca_03, 3},
	{utc_tel_set_sms_sca_04, 4},
	{utc_tel_set_sms_sca_05, 5},
	{utc_tel_set_sms_sca_06, 6},
	{NULL, 0}
};

TelSmsAddressInfo_t *pSCA;
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

	pSCA = (TelSmsAddressInfo_t*) malloc(sizeof(TelSmsAddressInfo_t));
	memset(pSCA, 0, sizeof(pSCA));

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

		printf("TapiCommonRegisterEvent subscription_id is %d gStartupSuccess is %d\n", subscription_id, gStartupSuccess);
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
 * @brief This tests tel_set_sms_sca() API in normal conditions
 * @par ID:
 * UTC_TEL_SET_SERV_CENTER_ADDR_01
 * @param [in] pSCA
 * @param [in] Index = 0
 * @code
 sprintf (scaNum,"%d", VALID_SCA_NUM);
 scaNum_len = strlen(scaNum);
 scaNum[scaNum_len] = '\0';
 pSCA->DialNumLen = scaNum_len;
 pSCA->Ton = SIM_TON_NATIONAL;
 pSCA->Npi = TAPI_SIM_NPI_ISDN_TEL;
 memcpy(pSCA->szDiallingNum,scaNum,scaNum_len);
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS
 */
void utc_tel_set_sms_sca_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	char scaNum[TAPI_SIM_SMSP_ADDRESS_LEN + 1];
	int scaNum_len = 0;
	int Index = 0;
	int i = 0;
	TS_BYTE packet[500];

	sprintf(scaNum, "%s", VALID_SCA_NUM);
	scaNum_len = strlen(scaNum);
	scaNum[scaNum_len] = '\0';
	scaNum_len = scaNum_len - 1;

	printf("Sca Number :%s\n", scaNum);
	printf("sca  number length : %d \n", scaNum_len);

	Index = SmsUtilEncodeAddrField(packet, scaNum, scaNum_len, 0x01, 0x01);

	printf("Index =%d\n", Index);

	for (i = 0; i < Index; i++)
		printf("[%02x]", packet[i]);

	pSCA->DialNumLen = Index;

	memcpy(pSCA->szDiallingNum, packet, Index);

	printf("\nSET VALID SERVICE CENTER ADDRESS\n");
	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(pSCA, 0, &pRequestId);

	gmain_loop_for_events(5000);

	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_sca_01 pass\n");

	return;
}


/**
 * @brief This tests tel_set_sms_sca() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_SERV_CENTER_ADDR_02
 * @param [in] pSCA
 * @param [in] Index = 0
 * @code
 sprintf (scaNum,"%d", INVALID_SCA_ADDR);
 scaNum_len = strlen(scaNum);
 scaNum[scaNum_len] = '\0';
 pSCA->DialNumLen = scaNum_len;
 pSCA->Ton = SIM_TON_NATIONAL;
 pSCA->Npi = TAPI_SIM_NPI_ISDN_TEL;
 memcpy(pSCA->szDiallingNum,scaNum,scaNum_len);
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_sca_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	char scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int scaNum_len = 0;
	int Index = 0;
	int i = 0;
	TS_BYTE packet[500];

	sprintf(scaNum, "%s", INVALID_SCA_ADDR);
	scaNum_len = strlen(scaNum);
	scaNum[scaNum_len] = '\0';
	scaNum_len = scaNum_len - 1;

	printf("Sca Number :%s\n", scaNum);
	printf("sca  number length : %d \n", scaNum_len);

	Index = SmsUtilEncodeAddrField(packet, scaNum, scaNum_len, 0x01, 0x01);

	printf("Index =%d\n", Index);

	for (i = 0; i < Index; i++)
		printf("[%02x]", packet[i]);

	pSCA->DialNumLen = Index;

	memcpy(pSCA->szDiallingNum, packet, Index);

	printf("\nSET INVALID SERVICE CENTER ADDRESS\n");
	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(pSCA, Index, &pRequestId);
	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	if (Index != 0) {
		TET_CHECK(returnStatus == TAPI_API_NOT_SUPPORTED);
		if (returnStatus == TAPI_API_NOT_SUPPORTED)
			printf("utc_tel_set_sms_sca_02 pass\n");
	}
	else {
		TET_CHECK(returnStatus == TAPI_API_SUCCESS);
		if (returnStatus == TAPI_API_SUCCESS)
			printf("utc_tel_set_sms_sca_02 pass\n");
	}
	return;

}

/**
 * @brief This tests tel_set_sms_sca() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_SERV_CENTER_ADDR_03()
 * @param [in] pSCA
 * @param [in] Index = 0
 * @code
 sprintf (scaNum,"%d", VALID_SCA_NUM);
 scaNum_len = strlen(scaNum);
 scaNum[scaNum_len] = '\0';
 pSCA->DialNumLen = -1;
 pSCA->Ton = SIM_TON_NATIONAL;
 pSCA->Npi = TAPI_SIM_NPI_ISDN_TEL;
 memcpy(pSCA->szDiallingNum,scaNum,scaNum_len);
 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_sms_sca_03()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	char scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int scaNum_len = 0;
	int Index = 0;
	int i = 0;
	TS_BYTE packet[500];

	sprintf(scaNum, "%s", VALID_SCA_NUM);
	scaNum_len = strlen(scaNum);
	scaNum[scaNum_len] = '\0';
	scaNum_len = scaNum_len - 1;

	printf("Sca Number :%s\n", scaNum);
	printf("sca  number length : %d \n", scaNum_len);

	Index = SmsUtilEncodeAddrField(packet, scaNum, scaNum_len, 0x01, 0x01);

	printf("Index =%d\n", Index);

	for (i = 0; i < Index; i++)
		printf("[%02x]", packet[i]);

	pSCA->DialNumLen = -1;

	memcpy(pSCA->szDiallingNum, packet, Index);

	printf("\nSET INVALID SERVICE CENTER ADDRESS LENGTH\n");
	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(pSCA, Index, &pRequestId);
	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_sca_03 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_sca() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_SERV_CENTER_ADDR_04()
 * @param [in] pSCA
 * @param [in] Index = 0
 * @code
 sprintf (scaNum,"%d", VALID_SCA_NUM);
 scaNum_len = strlen(scaNum);
 scaNum[scaNum_len] = '\0';
 pSCA->DialNumLen = -1;
 pSCA->Ton = SIM_TON_NATIONAL;
 pSCA->Npi = TAPI_SIM_NPI_ISDN_TEL;
 memcpy(pSCA->szDiallingNum,scaNum,scaNum_len);
 * @endcode
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_set_sms_sca_04()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	//int  pRequestId = -1;
	char scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int scaNum_len = 0;
	int Index = 0;

	sprintf(scaNum, "%s", VALID_SCA_NUM);
	scaNum_len = strlen(scaNum); //shlee, temporary divide the length by 2
	scaNum[scaNum_len] = '\0';

	pSCA->DialNumLen = scaNum_len;
	pSCA->Ton = SIM_TON_NATIONAL;
	pSCA->Npi = TAPI_SIM_NPI_ISDN_TEL;

	memcpy(pSCA->szDiallingNum, scaNum, scaNum_len);

	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(pSCA, Index, NULL);
	printf("The return value is %d\n", returnStatus);
	//printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_set_sms_sca_04 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_sca() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_SERV_CENTER_ADDR_05()
 * @param [in] pSCA  = NULL
 * @param [in] Index = 0
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_set_sms_sca_05()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;

	int Index = 0;
	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(NULL, Index, &pRequestId);
	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_set_sms_sca_05 pass\n");

	return;
}

/**
 * @brief This tests tel_set_sms_sca() API in abnormal conditions
 * @par ID:
 * UTC_TEL_SET_SERV_CENTER_ADDR_06()
 * @param [in] pSCA  = NULL
 * @param [in] Index = 0
 * @param [out] pRequestId = NULL
 * @return TAPI_API_INVALID_PTR
 */

void utc_tel_set_sms_sca_06()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int Index = 0;
	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(NULL, Index, NULL);
	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tel_set_sms_sca_06 pass\n");

	return;
}

/** @} */
