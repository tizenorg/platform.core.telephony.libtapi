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

/** 
 * @ingroup Telephony_API TelephonyAPI
 * @addtogroup POWER power
 */

/**
 * @ingroup  POWER power
 * @addtogroup UTS_POWER Unit
 */

/**
 * @ingroup  UTS_POWER Unit
 * @addtogroup UTS_TAPI_POWER_FLIGHT_MODE tel_set_flight_mode
 * @{
 */

/**
 * @file    uts_tel_set_flight_mode.c
 * @brief	This is a suite of unit test cases to test Telephony power API function
 * @author	Sagar Aggarwal  (a.sagar@samsung.com)
 * @version	Initial Creation
 * @date		[2009-05-13]
 */

#include "uts_tel_set_flight_mode.h"
#include "ITapiPower.h"

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

extern int expected_event;
extern bool async_response;

static void startup() {
	TapiResult_t ret_status;

	ret_status = tel_init();

	if (ret_status != TAPI_API_SUCCESS) {
		printf("TelTapiInit() Failed ret_status = %d \n", ret_status);
	}

	register_all_events();
	tel_register_app_name("org.samsung.test");

}

static void cleanup() {
	TapiResult_t ret_status;

	deregister_all();
	ret_status = tel_deinit();

	if (ret_status != TAPI_API_SUCCESS) {
		printf("TelTapiDeInit() Failed ret_status = %d \n", ret_status);
	}
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_POWER_FLIGHT_MODE_00
 * @param [in]  tel_set_flight_mode_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
 */

void utc_tel_set_flight_mode_00() {

	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_00 START    ###########\n");

	TapiResult_t ret_status;

	async_response = false;//initialize

	ret_status = tel_set_flight_mode(TAPI_POWER_FLIGHT_MODE_ENTER);

	printf("/n return value = %d\n", ret_status);

	if (TAPI_API_SUCCESS == ret_status) {
		printf(
				"/n/******************  set flight mode api successful **************************\\n");
	} else {
		printf(
				"/n/******************     set flight mode api  case  failed **************************\\n");
	}

	TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status)
		;
	tet_infoline("utc_tel_set_flight_mode_00 pass");
	expected_event = TAPI_EVENT_POWER_FLIGHT_MODE_RESP;
	printf("The expected_event is TAPI_EVENT_POWER_FLIGHT_MODE_RESP(%d)\n",
			expected_event);

	printf("\n\n value of asyn_response %d", async_response);
	G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	tet_result(TET_PASS);

	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_00 ENDS    ###########\n");

}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_POWER_FLIGHT_MODE_01
 * @param [in]  tel_set_flight_mode_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
 */

void utc_tel_set_flight_mode_01() {

	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_01 START    ###########\n");

	TapiResult_t ret_status;

	async_response = false;//initialize

	ret_status = tel_set_flight_mode(TAPI_POWER_FLIGHT_MODE_LEAVE);

	printf("/n return value = %d\n", ret_status);

	if (TAPI_API_SUCCESS == ret_status) {
		printf(
				"/n/******************  set flight mode api successful **************************\\n");
	} else {
		printf(
				"/n/******************     set flight mode api  case  failed **************************\\n");
	}

	TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status)
		;
	tet_infoline("utc_tel_set_flight_mode_01 pass");
	expected_event = TAPI_EVENT_POWER_FLIGHT_MODE_RESP;
	printf("The expected_event is TAPI_EVENT_POWER_FLIGHT_MODE_RESP(%d)\n",
			expected_event);

	printf("\n\n value of asyn_response %d", async_response);
	G_MAIN_LOOP_RUN(10);
	//	TET_CHECK_TEMP(true, async_response);	
	tet_result(TET_PASS);

	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_01 ENDS    ###########\n");

}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_POWER_FLIGHT_MODE_02
 * @param [in]  tel_set_flight_mode_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
 */

void utc_tel_set_flight_mode_02() {

	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_02 START    ###########\n");

	TapiResult_t ret_status;

	async_response = false;//initialize

	ret_status = tel_set_flight_mode(TAPI_POWER_FLIGHT_MODE_MAX);

	printf("/n return value = %d\n", ret_status);

	if (TAPI_API_INVALID_INPUT == ret_status) {
		printf(
				"/n/******************  set flight mode api invalid input case successful **************************\\n");
	} else {
		printf(
				"/n/******************     set flight mode api  invalid input case  failed **************************\\n");
	}

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status)
		;
	tet_infoline("utc_tel_set_flight_mode_02 pass");
	//expected_event = TAPI_EVENT_POWER_FLIGHT_MODE_RESP;
	//printf("The expected_event is TAPI_EVENT_POWER_FLIGHT_MODE_RESP(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);
	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_02 ENDS    ###########\n");

}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_POWER_FLIGHT_MODE_03
 * @param [in]  tel_set_flight_mode_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
 */

void utc_tel_set_flight_mode_03() {

	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_03 START    ###########\n");

	TapiResult_t ret_status;

	async_response = false;//initialize

	ret_status = tel_set_flight_mode(TAPI_POWER_FLIGHT_MODE_ENTER - 1);

	printf("/n return value = %d\n", ret_status);

	if (TAPI_API_INVALID_INPUT == ret_status) {
		printf(
				"/n/******************  set flight mode api invalid input case successful **************************\\n");
	} else {
		printf(
				"/n/******************     set flight mode api  invalid input case  failed **************************\\n");
	}

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status)
		;
	tet_infoline("utc_tel_set_flight_mode_03 pass");
	//expected_event = TAPI_EVENT_POWER_FLIGHT_MODE_RESP;
	//printf("The expected_event is TAPI_EVENT_POWER_FLIGHT_MODE_RESP(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);
	TEST_DEBUG("\n\n\n\n\n########     TEST utc_tel_set_flight_mode_03 ENDS    ###########\n");

}

/** @} */
