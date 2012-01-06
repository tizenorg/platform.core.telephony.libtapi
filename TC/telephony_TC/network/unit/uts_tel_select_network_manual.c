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
#include <ITapiNetwork.h>

#include "uts_tel_select_network_manual.h"
#include "tel_tapi_nw_common.h"

TS_UINT subscription_id = 0, subscription_id1 = 0;

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_select_network_manual_01, 1},
	{utc_tel_select_network_manual_02, 2},
	{NULL, 0}
};

void callback(TelTapiEvent_t *event)
{
	if (event->EventClass != TAPI_EVENT_CLASS_NETWORK)
		return;

	if (event->EventType == TAPI_EVENT_NETWORK_SELECT_CNF) {
		printf("\n$$$$$$ TAPI_EVENT_NETWORK_SELECT_CNF $$$$$$ \n");
	}
}


/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module
 */
static void startup()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	tet_infoline("utc_tel_select_network_manual, Test Start\n");

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS) {
		printf("tel_init Failed api_err = %d \n", api_err);
	}
	api_err = tel_register_event(TAPI_EVENT_NETWORK_SELECT_CNF, &subscription_id1, (TelAppCallback) &callback, NULL);
	if (api_err != TAPI_API_SUCCESS) {
		printf("TapiCommonRegisterEvent Failed api_err = %d \n", api_err);
	}
	printf("after TapiCommonRegisterEvent sub id is %d api_err is %d\n", subscription_id1, api_err);

	tel_register_app_name("org.samsung.test");
	return;
}

static void cleanup()
{
	tet_infoline("utc_tel_select_network_manual, Test Finished\n");
	tel_deregister_event(subscription_id1);
	tel_deinit();
	return;
}


/*
  Unit Test Case functions
*/

/**
 * @brief This tests tel_select_network_manual() API in normal conditions
 * @par ID:
 * utc_tel_select_network_manual_01
 * @code

		pSystemId -> SystemType = TAPI_NETWORK_SYSTEM_GSM;
		pSystemId -> ServiceType = TAPI_NETWORK_SERVICE_TYPE_COMBINED;
		pSystemId -> SysIdFormat = TAPI_NETWORK_SYSID_FORMAT_PLMN;
		pSystemId -> NetworkMode = TAPI_NETWORK_MODE_AUTOMATIC;
		pSystemId -> SysId = 12345;
		pSystemId -> bPlmnForbidden = FALSE;
		pSystemId -> bHome = TRUE;
		pSystemId -> bEquivalentPlmn = TRUE;
		pSystemId -> bNetworkIdentityAvail = TRUE;
		pSystemId -> bNetworkTimeAvail = TRUE;
		pSystemId -> bNetworkTimeZoneAvail = TRUE;
		pSystemId -> bNetworkDlsAvail = TRUE;
		memcpy(pSystemId -> szSystemName, "XO", 2);
		memcpy(pSystemId -> szSpn, "Vodafone", 9);
		pSystemId -> NetworkTimeZone.Year = 1970;
		pSystemId -> NetworkTimeZone.Month = 01;
		pSystemId -> NetworkTimeZone.Day = 01;
		pSystemId -> NetworkTimeZone.Hour = 01;
		pSystemId -> NetworkTimeZone.Minute = 01;
		pSystemId -> NetworkTimeZone.Second = 01;
		pSystemId -> NetworkTimeZone.DaylightSave = TAPI_NETWORK_DLS_ADJ_NO_ADJUSTMENT;
		pSystemId -> bMncIncludesPcsDigit = TRUE;
		ret_val = tel_select_network_manual(pSystemId, &pRequestId);
		ret_val = tel_select_network_manual(&pRequestId);
* @endcode
* @param [out] pRequestId
* @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
*/
void utc_tel_select_network_manual_01()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;
	unsigned int Plmn = 40405;
	int pRequestId;
	pRequestId = -1;

	tet_infoline("utc_tel_select_network_manual_01 Test\n");
	/* Selection of Network done*/
	/*************************/
	ret_val = tel_select_network_manual(Plmn, &pRequestId);

	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_select_network_manual_01 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_select_network_manual() API in abnormal conditions
 * @par ID:
 * utc_tel_select_network_manual_02
 * @code

		pRequestId = NULL;
		ret_val = tel_select_network_manual(&pRequestId);
* @endcode
* @param [out] pRequestId
* @return TAPI_API_INVALID_PTR
 */
void utc_tel_select_network_manual_02()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	unsigned int Plmn = 1;

	tet_infoline("utc_tel_select_network_manual_02 Test\n");

	ret_val = tel_select_network_manual(Plmn, NULL);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);

	tet_infoline("utc_tel_select_network_manual_02 is passed\n");
	tet_result(TET_PASS);

	return;
}

void utc_tel_select_network_manual_03()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	unsigned int Plmn = -1;

	tet_infoline("utc_tel_select_network_manual_02 Test\n");

	ret_val = tel_select_network_manual(Plmn, NULL);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);

	tet_infoline("utc_tel_select_network_manual_03 is passed\n");
	tet_result(TET_PASS);

	return;
}

/** @} */
