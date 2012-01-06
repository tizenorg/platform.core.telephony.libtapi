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
#include "uts_tel_get_sms_sca.h"
#include "sms_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;
struct tet_testlist tet_testlist[] = { { utc_tel_get_sms_sca_01, 1 }, { utc_tel_get_sms_sca_02, 2 }, { NULL, 0 } };

TelSmsAddressInfo_t *pSCA;
/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module
 */
static void startup()
{
#if 0
	char scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int scaNum_len = 0;
	int Index = 0;
	int i = 0;
	int pRequestId = -1;
	TS_BYTE packet[500];
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	pSCA = (TelSmsAddressInfo_t*) malloc(sizeof(TelSmsAddressInfo_t));
	memset(pSCA, 0, sizeof(pSCA));

	sprintf(scaNum, "%d", VALID_SCA_NUM);
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
#endif

	tel_init();
	tel_register_app_name("org.samsung.test");

#if 0
	printf("\nSET INVALID SERVICE CENTER ADDRESS\n");
	/* Calling TAPI SMS SET SCA API */
	returnStatus = tel_set_sms_sca(pSCA, Index, &pRequestId);
	printf("The return value is %d\n", returnStatus);
	printf("The pRequestId value is %d\n", pRequestId);
	if (returnStatus == TAPI_API_SUCCESS)
	printf("Set SCA settings successful\n");
#endif
}

static void cleanup()
{
	tel_deinit();

	//free(pSCA);
}

/**
 * @brief This tests tel_get_sms_sca() API in normal conditions
 * @par ID:
 * UTC_TEL_GET_SERV_CENTER_ADDR_01
 * @param [in] Index = 0
 * @param [out] pSCA
 * @return TAPI_API_SUCCESS
 */
void utc_tel_get_sms_sca_01()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TelSmsAddressInfo_t *pSCA;
	int req;

	pSCA = (TelSmsAddressInfo_t *) malloc(sizeof(TelSmsAddressInfo_t));

	returnStatus = tel_get_sms_sca(0, &req);

	printf("The return value is %d\n", returnStatus);

#if 0
	TS_BYTE SCA[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	char decode_sca_addr[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	int sca_len = -1;

	/* Decode the service center address */
	printf("Service center addr returned from TAPI SCA address %s returnStatus %d\n", pSCA->szDiallingNum, returnStatus);
	SmsUtilDecodeAddrField(decode_sca_addr, &(pSCA->szDiallingNum[0]), &(pSCA->Ton), &(pSCA->Npi));
	sca_len = strlen(decode_sca_addr);

	memcpy(SCA, decode_sca_addr, sca_len);
	SCA[sca_len] = '\0';

	printf("Decode SCA Address =========\n");
	printf("SCA Number :%s, SCA Length :%d \n", SCA, sca_len);
	printf("========================\n");
	printf("decodeScaAddr :%s\n", decode_sca_addr);
#endif

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_set_sms_sca_01 pass\n");

	return;
}

/**
 * @brief This tests tel_get_sms_sca() API in abnormal conditions
 * @par ID:
 * UTC_TEL_GET_SERV_CENTER_ADDR_02
 * @param [in]  Index = 0
 * @param [out] pSCA = NULL
 * @return TAPI_API_INVALID_PTR
 */
void utc_tel_get_sms_sca_02()
{
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	int req;

	returnStatus = tel_get_sms_sca(-1, &req);

	printf("The return value is %d\n", returnStatus);
	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_set_sms_sca_02 pass\n");

	return;
}
/** @} */
