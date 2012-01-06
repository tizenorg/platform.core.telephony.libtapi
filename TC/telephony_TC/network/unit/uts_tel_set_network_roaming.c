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

#include "tel_tapi_nw_common.h"
#include "uts_tel_set_network_roaming.h"

TS_UINT subscription_id = 0, subscription_id1 = 0;

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_network_roaming_01, 1},
	{utc_tel_set_network_roaming_02, 2},
	{utc_tel_set_network_roaming_03, 3},
	{utc_tel_set_network_roaming_04, 4},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module
 */
void callback(TelTapiEvent_t *event)
{
	if (event->EventClass != TAPI_EVENT_CLASS_NETWORK)
		return;

	if (event->EventType == TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF) {
		printf("\n$$$$$$ TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF $$$$$$ \n");
	}

}

static void startup()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;
	tet_infoline("utc_tel_set_network_roaming, Test Start\n");

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS) {
		printf("tel_init Failed api_err = %d \n", api_err);
	}

	api_err = tel_register_event(TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF, &subscription_id1, (TelAppCallback) &callback, NULL);
	if (api_err != TAPI_API_SUCCESS) {
		printf("TapiCommonRegisterEvent Failed api_err = %d \n", api_err);
	}
	printf("after TapiCommonRegisterEvent sub id is %d api_err is %d\n", subscription_id1, api_err);

	tel_register_app_name("org.samsung.test");
	return;
}

static void cleanup()
{
	tet_infoline("utc_tel_set_network_roaming, Test Finished\n");
	tel_deregister_event(subscription_id1);
	tel_deinit();
	return;
}


/*
 Unit Test Case functions
 */

/**
 * @brief This tests tel_set_network_roaming() API in normal conditions
 * @par ID:
 * utc_tel_tapi_nw_set_network_mode_func_01
 * @param [in] NwMode
 * @code

 RoamingMode = TAPI_NETWORK_MODE_EMERGENCY;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_roaming_01()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkRoamingMode_t RoamingMode;
	int pRequestId;

	RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC;
	RoamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_ON;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_roaming_01 Test\n");

	ret_val = tel_set_network_roaming(&RoamingMode, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_roaming_01 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_roaming() API in normal conditions
 * @par ID:
 * utc_tel_tapi_nw_set_network_mode_func_02
 * @param [in] NwMode
 * @code

 //RoamingMode = TAPI_NETWORK_MODE_AUTOMATIC;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_roaming_02()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkRoamingMode_t RoamingMode;
	int pRequestId;

	RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A;
	RoamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_ON;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_roaming_02 Test\n");

	ret_val = tel_set_network_roaming(&RoamingMode, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_roaming_02 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_roaming() API in normal conditions
 * @par ID:
 * utc_tel_set_network_roaming_03
 * @param [in] NwMode
 * @code

 //NwMode = TAPI_NETWORK_MODE_CDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_roaming_03()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkRoamingMode_t RoamingMode;
	int pRequestId;

	RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_ROAM_ONLY;
	RoamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_NONE + 1;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_roaming_03 Test\n");

	ret_val = tel_set_network_roaming(&RoamingMode, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_tapi_nw_set_network_mode_func_03 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_roaming() API in normal conditions
 * @par ID:
 * utc_tel_set_network_roaming_04
 * @param [in] NwMode
 * @code

		//RoamingMode = TAPI_NETWORK_MODE_GSM;
		pRequestId = -1;

* @endcode
* @param [out] pRequestId
* @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
*/
void utc_tel_set_network_roaming_04()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkRoamingMode_t RoamingMode;
	RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY;
	RoamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_ON;

	//RoamingMode = TAPI_NETWORK_MODE_GSM;

	tet_infoline("utc_tel_set_network_roaming_04 Test\n");

	ret_val = tel_set_network_roaming(&RoamingMode, NULL);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);
	if (ret_val == TAPI_API_INVALID_PTR) {
		tet_infoline("utc_tel_set_network_roaming_04 is passed\n");
		tet_result(TET_PASS);
	}

	return;
}

/** @} */
