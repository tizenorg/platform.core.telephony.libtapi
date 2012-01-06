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
#include "uts_tel_set_network_band.h"

TS_UINT subscription_id = 0, subscription_id1 = 0;

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_set_network_band_01, 1},
	{utc_tel_set_network_band_02, 2},
	{utc_tel_set_network_band_03, 3},
	{utc_tel_set_network_band_04, 4},
	{utc_tel_set_network_band_05, 5},
	{utc_tel_set_network_band_06, 6},
	{utc_tel_set_network_band_07, 7},
	{utc_tel_set_network_band_08, 8},
	{utc_tel_set_network_band_09, 9},
	{utc_tel_set_network_band_10, 10},
	{utc_tel_set_network_band_11, 11},
	{utc_tel_set_network_band_12, 12},
	{utc_tel_set_network_band_13, 13},
	{utc_tel_set_network_band_14, 14},
	{utc_tel_set_network_band_15, 15},
	{utc_tel_set_network_band_16, 16},
	{utc_tel_set_network_band_17, 17},
	{utc_tel_set_network_band_18, 18},
	{utc_tel_set_network_band_19, 19},
	{utc_tel_set_network_band_20, 20},
	{utc_tel_set_network_band_21, 21},
	{utc_tel_set_network_band_22, 22},
	{utc_tel_set_network_band_23, 23},
	{utc_tel_set_network_band_24, 24},
	{utc_tel_set_network_band_25, 25},
	{utc_tel_set_network_band_26, 26},
	{utc_tel_set_network_band_27, 27},
	{utc_tel_set_network_band_28, 28},
	{utc_tel_set_network_band_29, 29},
	{utc_tel_set_network_band_30, 30},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
 */
static void startup()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;
	tet_infoline("utc_tel_set_network_band, Test Start\n");

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS) {
		printf("tel_init Failed api_err = %d \n", api_err);
	}

	api_err = tel_register_event(TAPI_EVENT_NETWORK_SETNWBAND_CNF, &subscription_id1, tapi_callback, NULL);
	if (api_err != TAPI_API_SUCCESS) {
		printf("TapiCommonRegisterEvent Failed api_err = %d \n", api_err);
	}
	printf("after TapiCommonRegisterEvent sub id is %d api_err is %d\n", subscription_id1, api_err);

	tel_register_app_name("org.samsung.test");
	return;
}

static void cleanup()
{
	tet_infoline("utc_tel_set_network_band, Test Finished\n");
	tel_deregister_event(subscription_id1);
	tel_deinit();
	return;
}


/*
  Unit Test Case functions
*/

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_01
 * @param [in] BandMode
 * @param [in] Band
 * @code

		BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
		Band = TAPI_NETWORK_BAND_TYPE_ANY;
		pRequestId = -1;

* @endcode
* @param [out] pRequestId
* @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
*/
void utc_tel_set_network_band_01()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_ANY;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_01 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_01 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_02
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_GSM850;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_02()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int *pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_GSM850;
	pRequestId = NULL;

	tet_infoline("utc_tel_set_network_band_02 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, pRequestId);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);

	tet_infoline("utc_tel_set_network_band_02 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_03
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_03()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_03 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_03 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_04
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_GSM1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_04()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_GSM1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_04 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_04 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_05
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_05()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int *pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_GSM;
	pRequestId = NULL;

	tet_infoline("utc_tel_set_network_band_05 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, pRequestId);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);

	tet_infoline("utc_tel_set_network_band_05 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_06
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_06()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_06 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_06 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_07
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA850;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_07()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA850;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_07 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_07 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_08
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_08()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_08 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_08 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_09
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA2100;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_09()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA2100;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_09 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_09 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_10
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 Band = -1;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_10()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	Band = -1;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_10 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_10 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_11
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_ANY;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_11()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_ANY;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_11 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_11 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_12
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_GSM850;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_12()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int *pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_GSM850;
	pRequestId = NULL;

	tet_infoline("utc_tel_set_network_band_12 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, pRequestId);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);

	tet_infoline("utc_tel_set_network_band_12 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_13
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_13()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_13 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_13 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_14
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_GSM1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_14()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_GSM1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_14 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_14 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_15
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_15()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int *pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_GSM;
	pRequestId = NULL;

	tet_infoline("utc_tel_set_network_band_15 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, pRequestId);
	TET_CHECK(TAPI_API_INVALID_PTR, ret_val);

	tet_infoline("utc_tel_set_network_band_15 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_16
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_16()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_16 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_SUCCESS, ret_val);

	tet_infoline("utc_tel_set_network_band_16 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_17
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA850;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_17()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA850;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_17 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_17 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_18
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_18()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_18 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_18 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API in normal conditions
 * @par ID:
 * utc_tel_set_network_band_19
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA2100;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_19()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA2100;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_19 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_19 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_20
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
 Band = -1;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_20()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = TAPI_NETWORK_BAND_MODE_ONLY;
	Band = -1;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_20 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_20 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_21
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_ANY;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_21()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_ANY;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_21 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_21 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_22
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_GSM850;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_INVALID_INPUT
 */

void utc_tel_set_network_band_22()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_GSM850;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_22 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_22 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_23
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_23()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_23 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_23 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_24
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_GSM1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_24()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_GSM1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_24 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_24 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_25
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_GSM;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_25()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_GSM;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_25 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_25 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_26
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_26()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_26 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_26 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_27
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA850;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_27()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA850;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_27 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_27 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_28
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA1900;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_28()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA1900;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_28 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_28 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when one of the argument is invalid
 * @par ID:
 * utc_tel_set_network_band_29
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = TAPI_NETWORK_BAND_TYPE_WCDMA2100;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_29()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = TAPI_NETWORK_BAND_TYPE_WCDMA2100;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_29 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_29 is passed\n");
	tet_result(TET_PASS);

	return;
}

/**
 * @brief This tests tel_set_network_band() API when both the arguments are invalid
 * @par ID:
 * utc_tel_set_network_band_30
 * @param [in] BandMode
 * @param [in] Band
 * @code

 BandMode = -1;
 Band = -1;
 pRequestId = -1;

 * @endcode
 * @param [out] pRequestId
 * @return TAPI_API_SUCCESS - indicating that the operation has completed successfully.
 */

void utc_tel_set_network_band_30()
{
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	TelNetworkBandPreferred_t BandMode;
	TelNetworkBand_t Band;
	int pRequestId;

	BandMode = -1;
	Band = -1;
	pRequestId = -1;

	tet_infoline("utc_tel_set_network_band_30 Test\n");

	ret_val = tel_set_network_band(BandMode, Band, &pRequestId);
	TET_CHECK(TAPI_API_INVALID_INPUT, ret_val);

	tet_infoline("utc_tel_set_network_band_30 is passed\n");
	tet_result(TET_PASS);

	return;
}

/** @} */
