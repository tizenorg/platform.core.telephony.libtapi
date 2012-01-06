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

#include "uts_tapi_misc_NAM_info_get.h"

static void startup();
static void cleanup();

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

static TapiResult_t gStartupSuccess;

static void startup() {
	gStartupSuccess = tel_init();
	if (gStartupSuccess != TAPI_API_SUCCESS) {
		printf("TelTapiInit() Failed gStartupSuccess = %d \n", gStartupSuccess);
	}
	register_all_events();
}

static void cleanup() {
	if (gStartupSuccess == TAPI_API_SUCCESS) {
		//		 G_MAIN_CLOSE();
	}
	deregister_all();
	tel_deinit();
	return;
}

/**
 * @brief This tests
 * @par ID:
 * @param [in]  timezone_valid
 * @code
 * @endcode
 * @param [out]  timezone_valid
 * @return TapiResult_t
 */

void utc_tapi_misc_NAM_info_get_001() {

	TapiResult_t returnStatus;
	TS_WORD nam_info_mask = 0x0008;// Auto NAM
	TelMiscNamIndex_t nam_index = TAPI_MISC_NAM_1;
	int case_result = 1;

	gmain_loop_for_tapi_events(10000);
	returnStatus = tel_get_misc_nam_info(nam_info_mask, nam_index);
	// G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(10000);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS)
		;

	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tapi_misc_NAM_info_get_001 request successful\n");
	else
		case_result = 0;

	//expected_event = TAPI_EVENT_MISC_GET_NAM_INFO_CNF;
	gmain_loop_for_tapi_events(100);

	if (case_result)
		tet_result(TET_PASS);

	printf("\n\n\n\n\n########     TEST utc_tapi_misc_NAM_info_get_001 ENDS    ###########\n");
}

/**
 * @brief This tests
 * @par ID:
 * @param [in]  timezone_valid
 * @code
 * @endcode
 * @param [out]  timezone_valid
 * @return TapiResult_t
 */

void utc_tapi_misc_NAM_info_get_002() {

	TapiResult_t returnStatus;
	TS_WORD nam_info_mask = -1;
	TelMiscNamIndex_t nam_index = TAPI_MISC_NAM_1;
	int case_result = 1;

	gmain_loop_for_tapi_events(5000);
	returnStatus = tel_get_misc_nam_info(NULL, nam_index);
	// G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(5000);

	printf("The return value is %d\n", returnStatus);
	//printf ("The pRequestId value is %d\n",flag);

	if (returnStatus == TAPI_API_SUCCESS) {
		tet_result(TET_FAIL);
		tet_printf("utc_tapi_misc_Nam_info_get_002 failed\n");
	}
	tet_result(TET_PASS);

	printf("\n\n\n\n\n########     TEST utc_tapi_misc_NAM_info_get_002 ENDS    ###########\n");
}

/**
 * @brief This tests
 * @par ID:
 * @param [in]  timezone_valid
 * @code
 * @endcode
 * @param [out]  timezone_valid
 * @return TapiResult_t
 */

void utc_tapi_misc_NAM_info_get_003() {

	TapiResult_t returnStatus;
	TS_WORD nam_info_mask = 0x0008;// Auto NAM
	int nam_index = -1;
	int case_result = 1;

	gmain_loop_for_tapi_events(5000);
	returnStatus = tel_get_misc_nam_info(nam_info_mask, nam_index);
	// G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(5000);

	printf("The return value is %d\n", returnStatus);
	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT)
		;

	if (returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tapi_misc_NAM_info_get_003 request successful\n");
	else
		case_result = 0;

	if (case_result)
		tet_result(TET_PASS);

	printf("\n\n\n\n\n########     TEST utc_tapi_misc_NAM_info_get_003 ENDS    ###########\n");
}
/** @} */
