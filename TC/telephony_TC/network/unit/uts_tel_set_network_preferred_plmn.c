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
#include "uts_tel_set_network_preferred_plmn.h"

TS_UINT subscription_id = 0, subscription_id1 = 0;

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_network_preferred_plmn_01, 1},
	//{utc_tel_set_network_preferred_plmn_02, 2},
	//{utc_tel_set_network_preferred_plmn_03, 3},
	{utc_tel_set_network_preferred_plmn_04, 4},
	{utc_tel_set_network_preferred_plmn_05, 3},
	{utc_tel_set_network_preferred_plmn_06, 6},
	{utc_tel_set_network_preferred_plmn_07, 7},
	{utc_tel_set_network_preferred_plmn_08, 8},
	{utc_tel_set_network_preferred_plmn_09, 9},
	{utc_tel_set_network_preferred_plmn_10, 10},
	{utc_tel_set_network_preferred_plmn_11, 11},
	//{utc_tel_set_network_preferred_plmn_12, 12},
	//{utc_tel_set_network_preferred_plmn_13, 13},
	{utc_tel_set_network_preferred_plmn_14, 14},
	{utc_tel_set_network_preferred_plmn_15, 15},
	{utc_tel_set_network_preferred_plmn_16, 16},
	{utc_tel_set_network_preferred_plmn_17, 17},
	{utc_tel_set_network_preferred_plmn_18, 18},
	{utc_tel_set_network_preferred_plmn_19, 19},
	{utc_tel_set_network_preferred_plmn_20, 20},
	{utc_tel_set_network_preferred_plmn_21, 21},
	//{utc_tel_set_network_preferred_plmn_22, 22},
	//{utc_tel_set_network_preferred_plmn_23, 23},
	{utc_tel_set_network_preferred_plmn_24, 24},
	{utc_tel_set_network_preferred_plmn_25, 25},
	{utc_tel_set_network_preferred_plmn_26, 26},
	{utc_tel_set_network_preferred_plmn_27, 27},
	{utc_tel_set_network_preferred_plmn_28, 28},
	{utc_tel_set_network_preferred_plmn_29, 29},
	{utc_tel_set_network_preferred_plmn_30, 30},
	{utc_tel_set_network_preferred_plmn_31, 31},
	//{utc_tel_set_network_preferred_plmn_32, 32},
	//{utc_tel_set_network_preferred_plmn_33, 33},
	{utc_tel_set_network_preferred_plmn_34, 34},
	{utc_tel_set_network_preferred_plmn_35, 35},
	{utc_tel_set_network_preferred_plmn_36, 36},
	{utc_tel_set_network_preferred_plmn_37, 37},
	{utc_tel_set_network_preferred_plmn_38, 38},
	{utc_tel_set_network_preferred_plmn_39, 39},
	{utc_tel_set_network_preferred_plmn_40, 40},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module
 */
static void startup()
{
	int api_err = TAPI_API_SUCCESS;
	tet_infoline("utc_tel_set_network_preferred_plmn, Test Start\n");

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS) {
		printf("tel_init Failed api_err = %d \n", api_err);
	}
	api_err = tel_register_event(TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF, &subscription_id1, (TelAppCallback) tapi_callback, NULL);
	if (api_err != TAPI_API_SUCCESS) {
		printf("TapiCommonRegisterEvent Failed api_err = %d \n", api_err);
	}
	printf("after TapiCommonRegisterEvent sub id is %d api_err is %d\n", subscription_id1, api_err);

	tel_register_app_name("org.samsung.test");
	return;
}

static void cleanup()
{
	tet_infoline("utc_tel_set_network_preferred_plmn, Test Finished\n");
	tel_deregister_event(subscription_id1);
	tel_deinit();
	return;
}

/*
  Unit Test Case functions
*/

/**
 * @brief This tests tel_set_network_preferred_plmn() API for invalid inputs
 * @par ID:
 * utc_tel_set_network_preferred_plmn_01
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

		Operation = TAPI_NETWORK_PREF_PLMN_ADD;
		PreffPlmnInfo.Index = 244;
		memcpy(PreffPlmnInfo.Plmn,"40411",5);
		PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV;
		pRequestId = -1;

* @endcode
* @param [out] pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_set_network_preferred_plmn_01()
{
	int ret_val = TAPI_API_SUCCESS;
	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 244;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV; /*Test for invalid value*/
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_01 Test\n");
	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_01 is passed\n");
	tet_result(TET_PASS);
	return;
}

#if 0
/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_02
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 245;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_02()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 245;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_02 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_02 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_03
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 246;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_03()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 246;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_03 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_03 is passed\n");
	tet_result(TET_PASS);

	return;
}
#endif

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_04
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 247;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_04()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 247;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_04 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_04 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_05
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 248;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_05()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 248;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_05 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_05 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_06
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 249;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_06()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 249;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_06 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_06 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_07
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 250;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_07()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 250;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_07 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_07 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_08
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 251;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_08()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 251;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_08 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_08 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_09
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 252;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_09()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 252;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_09 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_09 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_10
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 PreffPlmnInfo.Index = 253;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_10()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	PreffPlmnInfo.Index = 253;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_10 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_10 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid inputs
 * @par ID:
 * utc_tel_set_network_preferred_plmn_11
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 244;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_11()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 244;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV; /*Test for invalid value*/
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_11 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_11 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_12
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

		Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
		PreffPlmnInfo.Index = 245;
		memcpy(PreffPlmnInfo.Plmn,"40411",5);
		PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
		pRequestId = -1;

* @endcode
* @param [out] pRequestId
* @return TAPI_API_INVALID_INPUT
 */
#if 0
void utc_tel_set_network_preferred_plmn_12()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 245;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_12 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_12 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_13
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 246;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_13()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 246;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_13 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_13 is passed\n");
	tet_result(TET_PASS);

	return;
}
#endif

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_14
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 247;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_14()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 247;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_14 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_14 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_15
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 248;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_15()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 248;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_15 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_15 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_16
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 249;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_16()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 249;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_16 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_16 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_17
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 250;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_17()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 250;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_17 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_17 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_18
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 251;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_18()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 251;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_18 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_18 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_19
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 252;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_19()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 252;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_19 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_19 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_20
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
 PreffPlmnInfo.Index = 253;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_20()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_EDIT;
	PreffPlmnInfo.Index = 253;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_20 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_20 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid inputs
 * @par ID:
 * utc_tel_set_network_preferred_plmn_21
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 244;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_21()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 244;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV; /*Test for invalid value*/
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_21 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_21 is passed\n");
	tet_result(TET_PASS);

	return;
}

#if 0
/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_22
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 245;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_22()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 245;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_22 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_22 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_23
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 246;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_23()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 246;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_23 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_23 is passed\n");
	tet_result(TET_PASS);

	return;
}
#endif

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_24
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 247;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_24()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 247;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_24 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_24 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_25
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 248;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_25()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 248;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_25 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_25 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_26
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 249;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_26()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 249;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_26 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_26 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_26
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 250;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_27()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 250;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_27 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_27 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_28
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 251;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_28()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 251;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_28 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_28 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_29
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 252;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */
void utc_tel_set_network_preferred_plmn_29()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 252;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_29 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_29 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API in normal conditions
 * @par ID:
 * utc_tel_set_network_preferred_plmn_30
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
 PreffPlmnInfo.Index = 253;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_30()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = TAPI_NETWORK_PREF_PLMN_DELETE;
	PreffPlmnInfo.Index = 253;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_30 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_30 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_31
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_31()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_NO_SRV;/*Invalid input*/
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_31 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_31 is passed\n");
	tet_result(TET_PASS);

	return;
}

#if 0
/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_32
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_32()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_CDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_32 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_32 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_33
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_preferred_plmn_33()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn,"40411",5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_TDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_33 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_33 is passed\n");
	tet_result(TET_PASS);

	return;
}
#endif

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_34
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_34()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_34 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_34 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_35
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_35()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_35 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_35 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_36
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_36()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_EGPRS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_36 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_36 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_37
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_37()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_PCS1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_37 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_37 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_38
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_38()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_38 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_38 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_39
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_39()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_39 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_39 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_preferred_plmn() API with invalid input
 * @par ID:
 * utc_tel_set_network_preferred_plmn_40
 * @param [in] Operation
 * @param [in] PreffPlmnInfo
 * @code

 Operation = -1;
 PreffPlmnInfo.Index = 23;
 memcpy(PreffPlmnInfo.Plmn,"40411",5);
 PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */
void utc_tel_set_network_preferred_plmn_40()
{
	int ret_val = TAPI_API_SUCCESS;

	TelNetworkPrefferedPlmnOp_t Operation;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	int pRequestId;

	Operation = -1;/*invalid input*/
	PreffPlmnInfo.Index = 23;/*Some value between 0 and 255*/
	memcpy(PreffPlmnInfo.Plmn, "40411", 5);
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_HSDPA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_preferred_plmn_40 Test\n");

	ret_val = tel_set_network_preferred_plmn(Operation, &PreffPlmnInfo, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_preferred_plmn_40 is passed\n");
	tet_result(TET_PASS);

	return;
}
/** @} */
