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

#include "uts_tapi_misc_NAM_info_set.h"

static void startup();
static void cleanup();

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

static TapiResult_t gStartupSuccess;

/* Startup & Clean function : It is not test code, only for
 initialize and finalize test module */

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
 * @param [in]  *timezone_info
 * @code
 * @endcode
 * @param [out]  *timezone_info
 * @return TapiResult_t
 */

void utc_tapi_misc_NAM_info_set_001() {
	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TelMiscNamInfo_t *nam_info = { 0, };
	/*
	 nam_info->nam_info_mask = ;
	 nam_info->nam_total = ;
	 nam_info->nam_current_index = ;
	 nam_info->nam_req_index = ;
	 nam_info->nam_auto = ;
	 memcpy (,&nam_info->nam_mcc,TAPI_MISC_MCC_LEN_MAX);
	 memcpy (,&nam_info->nam_mnc,TAPI_MISC_MCC_LEN_MAX);
	 memcpy (,&nam_info->nam_min,TAPI_MISC_MIN_LEN_MAX);
	 memcpy (,&nam_info->nam_mdn,TAPI_MISC_MDN_LEN_MAX);
	 memcpy (,&nam_info->nam_name,TAPI_MISC_NAME_LEN_MAX);
	 */
	gmain_loop_for_tapi_events(10000);
	returnStatus = tel_set_misc_nam_info(&nam_info);

	//G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(10000);
	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS)
		;

	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tapi_misc_NAM_info_set_001 pass\n");

}

/**
 * @brief This tests
 * @par ID:
 * @param [in]  *timezone_info
 * @code
 * @endcode
 * @param [out]  *timezone_info
 * @return TapiResult_t
 */
void utc_tapi_misc_NAM_info_set_002() {

	TapiResult_t returnStatus = TAPI_API_SUCCESS;
	TelMiscNamInfo_t *nam_info = { 0, };

	gmain_loop_for_tapi_events(10000);
	returnStatus = tel_set_misc_nam_info(NULL);
	//G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(10000);
	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR)
		;
	if (returnStatus == TAPI_API_INVALID_PTR)
		printf("utc_tapi_misc_NAM_info_set_002 pass\n");

}
/** @} */
