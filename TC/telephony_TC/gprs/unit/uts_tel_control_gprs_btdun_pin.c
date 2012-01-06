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

//add includes


#include "uts_tel_control_gprs_btdun_pin.h"

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

extern int expected_event;
extern bool async_response;

static void startup()
{
	TapiResult_t ret_status;

	ret_status = tel_init();

	if (ret_status != TAPI_API_SUCCESS) {
		printf("TelTapiInit() Failed ret_status = %d \n", ret_status);
	}

	register_all_events();
	tel_register_app_name("org.samsung.test");
}

static void cleanup()
{
	TapiResult_t ret_status;

	deregister_all();
	ret_status = tel_deinit();

	if (ret_status != TAPI_API_SUCCESS) {
		printf("TelTapiDeInit() Failed ret_status = %d \n", ret_status);
	}
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_00
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_00()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_00 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	/* NORMAL CASE */
	bt_control.signal = GPRS_DATA_SIGNAL_DTR;
	bt_control.status = GPRS_SIGNAL_STATUS_OFF;

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_00 pass");
	expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	printf("- The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n", expected_event);

	G_MAIN_LOOP_RUN(10);
	TET_CHECK_TEMP(true, async_response);
	tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_00 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_01
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_01()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_01 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	/* NORMAL CASE */
	bt_control.signal = GPRS_DATA_SIGNAL_RTS;
	bt_control.status = GPRS_SIGNAL_STATUS_ON;

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_01 pass");
	expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	printf("- The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n", expected_event);

	G_MAIN_LOOP_RUN(10);
	TET_CHECK_TEMP(true, async_response);
	tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_01 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_02
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_02()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_02 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	/* BAD CASE - GPRS_DATA_SIGNAL_RI not allowed */
	bt_control.signal = GPRS_DATA_SIGNAL_RI;
	bt_control.status = GPRS_SIGNAL_STATUS_ON;

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_02 pass");
	expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	printf("- The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n", expected_event);

	G_MAIN_LOOP_RUN(10);
	TET_CHECK_TEMP(false, async_response);
	tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_02 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_03
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_03()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_03 START    ###########");

	TapiResult_t ret_status;
	//int request_id=0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	bt_control.signal = GPRS_DATA_SIGNAL_RI;
	bt_control.status = GPRS_SIGNAL_STATUS_ON;

	printf("- gprs btdunpincontrol null case check\n");

	ret_status = tel_control_gprs_btdun_pin(bt_control, NULL);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_INVALID_PTR, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_03 pass");
	//expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	//printf("The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_03 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_04
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_04()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_04 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	bt_control.signal = GPRS_SIGNAL_DCD - 1;
	bt_control.status = GPRS_SIGNAL_STATUS_ON;

	printf("- gprs btdunpincontrol invalid input case check\n");

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_04 pass");
	//expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	//printf("The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_04 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_05
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_05()
{

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_05 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	bt_control.signal = GPRS_SIGNAL_DCD;
	bt_control.status = GPRS_SIGNAL_STATUS_OFF - 1;

	printf("- gprs btdunpincontrol invalid input case check\n");

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_05 pass");
	//expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	//printf("The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_05 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_06
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_06()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_06 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	bt_control.signal = GPRS_DATA_SIGNAL_RI + 1;
	bt_control.status = GPRS_SIGNAL_STATUS_ON;

	printf("- gprs btdunpincontrol invalid input case check\n");

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_06 pass");
	//expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	//printf("The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_06 ENDS    ###########\n");
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_BTDUNPINCONTROL_GET_07
 * @param [in]  pincontrol
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  pincontrol
 * @param [out]  pRequestID
 * @return TapiResult_t
 */
void utc_tel_control_gprs_btdun_pin_07()
{
	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_07 START    ###########");

	TapiResult_t ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;
	async_response = false;//initialize

	memset(&bt_control, 0, sizeof(tapi_ps_btdun_pincontrol));

	bt_control.signal = 0x03;
	bt_control.status = GPRS_SIGNAL_STATUS_ON;

	printf("- gprs btdunpincontrol invalid input case check\n");

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id);

	printf("- return value = %d\n", ret_status);

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_control_gprs_btdun_pin_07 pass");
	//expected_event = TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI;
	//printf("The expected_event is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);
	//tet_result(TET_PASS);

	TEST_DEBUG("########     TEST utc_tel_control_gprs_btdun_pin_07 ENDS    ###########\n");
}

/** @} */
