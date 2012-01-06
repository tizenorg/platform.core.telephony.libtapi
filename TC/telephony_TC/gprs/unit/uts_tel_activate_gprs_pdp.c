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


#include "uts_tel_activate_gprs_pdp.h"

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

extern int expected_event;
extern bool async_response;

static void startup()
{
	TLOG_HEADER("STARTUP");

	TapiResult_t ret_status;

	ret_status = tel_init();
	if (ret_status != TAPI_API_SUCCESS)	
	{	
		printf("TelTapiInit() Failed ret_status = %d \n", ret_status);
	}

	register_all_events();

	tel_register_app_name("org.samsung.test");

	TLOG_INFO(" - GPRS active session deactivate");

	ret_status = temp_gprs_session_deactivate(CONTEXT_ID);
	if (ret_status == TAPI_API_SUCCESS)
	{
		TLOG_INFO("- GPRS active session ended successfully");
	}
	else
	{
		TLOG_INFO("- GPRS active session not ended");
	}
}

static void cleanup()
{
	TLOG_HEADER("CLEANUP");

	TapiResult_t  ret_status;

	deregister_all();

	ret_status = tel_deinit();
	if (ret_status != TAPI_API_SUCCESS)	
	{	
		printf("TelTapiDeInit() Failed ret_status = %d \n", ret_status);	
	}
}


#if 0
/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_START_00
 * @param [in]  *net_start_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_start_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_activate_gprs_pdp_00()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);
	TLOG_INFO(" - setting the valid gprs mode");

	TapiResult_t ret_status ;
	tapi_ps_net_start_req_t start_req;
	int request_id = 0;

	async_response = false;//initialize

	memset(&start_req, 0, sizeof(tapi_ps_net_start_req_t));
	start_req.cont_id = CONTEXT_ID;
	strncpy(start_req.pdp_info.apn , APN_ADDRESS, TAPI_PDP_APN_LEN_MAX);

	ret_status = tel_activate_gprs_pdp(&start_req, &request_id);
	if (TAPI_API_SUCCESS == ret_status)
	{
		TLOG_INFO(" - GPRS mode started successfully");
	}
	else
	{
		TLOG_INFO(" - GPRS mode not started");
	}

	TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
	tet_infoline("utc_tel_activate_gprs_pdp_00 pass");

	expected_event = TAPI_EVENT_PS_PDP_ACT_RSP;
	TLOG_INFO(" - expected_event is TAPI_EVENT_PS_PDP_ACT_RSP(%d)", expected_event);
	TLOG_INFO(" - ... waiting ... ");

	G_MAIN_LOOP_RUN(80);
	TET_CHECK_TEMP(true, async_response);	
	tet_result(TET_PASS);

	ret_status = temp_gprs_session_deactivate(CONTEXT_ID);
	if(TAPI_API_SUCCESS == ret_status)
	{
		TLOG_INFO(" - GPRS active session ended succesfully");
	}
	else
	{
		TLOG_INFO(" - GPRS active session not ended");
	}

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}
#endif

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_START_01
 * @param [in]  *net_start_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_start_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_activate_gprs_pdp_00()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);
	TLOG_INFO(" - setting the invalid(net_start_req_info) gprs mode");

	TapiResult_t ret_status ;
	tapi_ps_net_start_req_t start_req;
	int request_id = 0;

	async_response = false;//initialize

	memset(&start_req, 0, sizeof(tapi_ps_net_start_req_t));
	start_req.cont_id = CONTEXT_ID;
	strncpy(start_req.pdp_info.apn , APN_ADDRESS, TAPI_PDP_APN_LEN_MAX);

	ret_status = tel_activate_gprs_pdp(NULL, &request_id);
	if(TAPI_API_INVALID_PTR == ret_status)
	{
		TLOG_INFO(" - GPRS mode start api invalid NULL case pass");
	}
	else
	{
		TLOG_INFO(" - GPRS mode NULL check failed");
	}

	TET_CHECK_EQUAL(TAPI_API_INVALID_PTR, ret_status);
	tet_infoline("utc_tel_activate_gprs_pdp_01 pass");

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_START_02
 * @param [in]  *net_start_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_start_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_activate_gprs_pdp_01()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);
	TLOG_INFO(" - setting the invalid(pRequestID) gprs mode");

	TapiResult_t ret_status ;
	tapi_ps_net_start_req_t start_req;
	//int request_id=0;
	async_response = false;//initialize

	memset(&start_req, 0, sizeof(tapi_ps_net_start_req_t));

	start_req.cont_id = CONTEXT_ID;
	strncpy(start_req.pdp_info.apn , APN_ADDRESS, TAPI_PDP_APN_LEN_MAX);

	ret_status = tel_activate_gprs_pdp(&start_req, NULL);
	if (TAPI_API_INVALID_PTR == ret_status)
	{
		TLOG_INFO(" - GPRS mode started api invalid NULL case pass");
	}
	else
	{
		TLOG_INFO(" - GPRS mode NULL check failed");
	}

	TET_CHECK_EQUAL(TAPI_API_INVALID_PTR, ret_status);
	tet_infoline("utc_tel_activate_gprs_pdp_02 pass");

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}

/*
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_START_03
 * @param [in]  *net_start_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_start_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_activate_gprs_pdp_02()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);
	TLOG_INFO(" - setting the invalid(auth_type) gprs mode");

	TapiResult_t ret_status;
	tapi_ps_net_start_req_t start_req;
	int request_id = 0;

	async_response = false;//initialize

	memset(&start_req, 0, sizeof(tapi_ps_net_start_req_t));
	start_req.cont_id = CONTEXT_ID;
	strncpy(start_req.pdp_info.apn , APN_ADDRESS, TAPI_PDP_APN_LEN_MAX);
	start_req.pdp_info.auth_type = TAPI_PDP_AUTH_NONE - 1;

	ret_status = tel_activate_gprs_pdp(&start_req, &request_id);
	if (TAPI_API_INVALID_INPUT == ret_status)
	{
		TLOG_INFO(" - GPRS mode start api invalid input cass pass");
	}
	else
	{
		TLOG_INFO(" - GPRS mode invalid enum check failed");
	}

	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_activate_gprs_pdp_03 pass");

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}

/** @} */
