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


#include "uts_tel_deactivate_gprs_pdp.h"



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
		printf("TelTapiInit() Failed ret_status = %d\n", ret_status);	
	}

	register_all_events();
	tel_register_app_name("org.samsung.test");
}

static void cleanup()
{
	TLOG_HEADER("CLEANUP");

	TapiResult_t ret_status ;

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
 * @par ID: UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_00
 * @param [in]  *net_stop_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_stop_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
*/	
void utc_tel_deactivate_gprs_pdp_01()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);

	TapiResult_t ret_status ;
	tapi_ps_net_stop_req_t end_req;
	int request_id = 0;

	async_response = false;//initialize

	if (TAPI_API_SUCCESS == temp_gprs_session_activate())
	{
		TLOG_INFO(" - temp gprs mode started succesfully");

		memset(&end_req, 0, sizeof(tapi_ps_net_stop_req_t));
		end_req.cont_id = CONTEXT_ID;

		ret_status = tel_deactivate_gprs_pdp(&end_req, &request_id);
		if (TAPI_API_SUCCESS == ret_status)
		{
			TLOG_INFO(" - gprs activate session ended succesfully");
		}
		else
		{
			TLOG_INFO(" - gprs active session not ended");
		}

		TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
		tet_infoline("utc_tel_deactivate_gprs_pdp_00 pass");
		expected_event = TAPI_EVENT_PS_PDP_DEACT_RSP;

		TLOG_INFO(" - The expected_event is TAPI_EVENT_PS_PDP_DEACT_RSP(%d)\n", expected_event);

		G_MAIN_LOOP_RUN(80);
		TET_CHECK_TEMP(true, async_response);	
		tet_result(TET_PASS);
	}
	else
	{
		TLOG_INFO(" - temp gprs mode not started");
	}

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}
#endif

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_01
 * @param [in]  *net_stop_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_stop_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_deactivate_gprs_pdp_01()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);
	TLOG_INFO(" - temp gprs session end api NULL case");

	TapiResult_t ret_status;
	int request_id = 0;

	async_response = false;//initialize

	ret_status = tel_deactivate_gprs_pdp(NULL, &request_id);
	if (TAPI_API_INVALID_PTR == ret_status)
	{
		printf("/n/******************  gprs end api null case pass **************************\\n");
	}
	else
	{
		printf("/n/******************  gprs end api null case failed  **************************\\n");
	}


	TET_CHECK_EQUAL(TAPI_API_INVALID_PTR, ret_status);
	tet_infoline("utc_tel_deactivate_gprs_pdp_01 pass");
	//expected_event = TAPI_EVENT_PS_PDP_DEACT_RSP;
	//printf("The expected_event is TAPI_EVENT_PS_PDP_DEACT_RSP(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);	
	//tet_result(TET_PASS);

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_02
 * @param [in]  *net_stop_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_stop_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_deactivate_gprs_pdp_02()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);

	TapiResult_t  ret_status ;
	tapi_ps_net_stop_req_t end_req;

	async_response = false;//initialize


	printf("/n/****************** temp  gprs session end api Null case **************************\\n");

	end_req.cont_id = CONTEXT_ID;

	ret_status = tel_deactivate_gprs_pdp(&end_req, NULL);
	if (TAPI_API_INVALID_PTR == ret_status)
	{
		printf("/n/******************  gprs end api null case pass **************************\\n");
	}
	else
	{
		printf("/n/******************  gprs end api null case failed  **************************\\n");
	}


	TET_CHECK_EQUAL(TAPI_API_INVALID_PTR, ret_status);
	tet_infoline("utc_tel_deactivate_gprs_pdp_02 pass");
	//expected_event = TAPI_EVENT_PS_PDP_DEACT_RSP;
	//printf("The expected_event is TAPI_EVENT_PS_PDP_DEACT_RSP(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);	
	//tet_result(TET_PASS);

	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_04
 * @param [in]  *net_stop_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_stop_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_deactivate_gprs_pdp_03()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);

	TapiResult_t  ret_status ;
	tapi_ps_net_stop_req_t end_req;
	int request_id=0;

	async_response = false;//initialize


	printf("/n/****************** temp  gprs session end api invalid enum case **************************\\n");

	end_req.cont_id = CONTEXT_ID;
	end_req.pdp_info.auth_type = TAPI_PDP_AUTH_MAX;	

	ret_status = tel_deactivate_gprs_pdp(&end_req,&request_id);
	if (TAPI_API_INVALID_INPUT == ret_status)
	{
		printf("/n/******************  gprs end api invalid enum case pass **************************\\n");
	}
	else
	{
		printf("/n/******************  gprs end api invalid enum case failed  **************************\\n");
	}


	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_deactivate_gprs_pdp_04 pass");
	//expected_event = TAPI_EVENT_PS_PDP_DEACT_RSP;
	//printf("The expected_event is TAPI_EVENT_PS_PDP_DEACT_RSP(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);	
	//tet_result(TET_PASS);


	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}

/**
 * @brief This tests 
 * @par ID: UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_05
 * @param [in]  *net_stop_req_info
 * @param [in]  pRequestID
 * @code     	
 * @endcode
 * @param [out]  *net_stop_req_info
 * @param [out]  pRequestID
 * @return TapiResult_t
 */	
void utc_tel_deactivate_gprs_pdp_04()
{
	TLOG_HEADER("TEST START [%s]", __FUNCTION__);

	TapiResult_t  ret_status ;
	tapi_ps_net_stop_req_t end_req;
	int request_id=0;

	async_response = false;//initialize


	printf("/n/****************** temp  gprs session not started to check api **************************\\n");

	end_req.cont_id = CONTEXT_ID;
	end_req.pdp_info.auth_type = TAPI_PDP_AUTH_NONE-1;	

	ret_status = tel_deactivate_gprs_pdp(&end_req, &request_id);
	if (TAPI_API_SUCCESS != ret_status)
	{
		printf("/n/******************  gprs end api invalid server case pass **************************\\n");
	}
	else
	{
		printf("/n/******************  gprs end api invalid server case failed  **************************\\n");
	}


	TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
	tet_infoline("utc_tel_deactivate_gprs_pdp_05 pass");
	//expected_event = TAPI_EVENT_PS_PDP_DEACT_RSP;
	//printf("The expected_event is TAPI_EVENT_PS_PDP_DEACT_RSP(%d)\n",expected_event);

	//G_MAIN_LOOP_RUN(10);
	//TET_CHECK_TEMP(true, async_response);	
	//tet_result(TET_PASS);


	TLOG_HEADER("TEST END [%s]\n", __FUNCTION__);
}
/** @} */
