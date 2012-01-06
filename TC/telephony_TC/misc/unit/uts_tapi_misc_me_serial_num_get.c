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

void utc_tapi_misc_me_serial_num_get_001() {

	int returnStatus;
	TelMiscSNIndexType_t index_type = TAPI_MISC_ME_MEID;
	TelMiscSNInformation sn_info;
	int case_result = 1;

	gmain_loop_for_tapi_events(10000);
	returnStatus = tel_get_misc_me_sn(index_type, &sn_info);
	// G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(10000);

	printf("The return value is %d\n", returnStatus);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);

	if (returnStatus == TAPI_API_SUCCESS)
		printf("utc_tapi_misc_me_serial_num_get_001 request successful\n");
	else
		case_result = 0;

	expected_event = TAPI_EVENT_MISC_SERIAL_NUMBER_IND;
	gmain_loop_for_tapi_events(10000);

	if (case_result)
		tet_result(TET_PASS);

	printf("\n\n\n\n\n########     TEST utc_tapi_misc_me_serial_num_get_001 ENDS    ###########\n");
}

/**
 * @brief This tests
 * @par ID:
 * @param [in]  timezone_valid
 * @code
 * @endcode
 * @param [out]  timezone_valid
 * @return int
 */

void utc_tapi_misc_me_serial_num_get_002() {

	int returnStatus;
	int index_type = -1;
	int case_result = 1;
	TelMiscSNInformation sn_info;

	gmain_loop_for_tapi_events(5000);
	returnStatus = tel_get_misc_me_sn(index_type, &sn_info);
	// G_MAIN_LOOP_RUN(5);
	gmain_loop_for_tapi_events(5000);

	printf("The return value is %d\n", returnStatus);

	if (returnStatus == TAPI_API_SUCCESS) {
		tet_result(TET_FAIL);
		printf("\nTEST utc_tapi_misc_me_serial_num_get_002 Failed    \n");
	}

	tet_result(TET_PASS);

	printf("\n\n\n\n\n########     TEST utc_tapi_misc_me_serial_num_get_002 ENDS    ###########\n");
}
/** @} */
