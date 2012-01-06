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


#include "uts_tapi_display_get_icon_info.h"



void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tapi_display_get_icon_info_00, 1},
	{utc_tapi_display_get_icon_info_01, 2},
	{utc_tapi_display_get_icon_info_02, 3},
	{utc_tapi_display_get_icon_info_03, 4},
	{utc_tapi_display_get_icon_info_04, 5},
	{NULL, 0}
	};

extern int expected_event;
extern bool async_response;

static void startup()
{
	TapiResult_t  ret_status ;


	ret_status = tel_init();

	if(ret_status != TAPI_API_SUCCESS)	
		{	
			printf("TelTapiInit() Failed ret_status = %d \n",ret_status);	
		}

	register_all_events();

	tel_register_app_name("org.samsung.test");
}

static void cleanup()
{
	TapiResult_t  ret_status ;

	deregister_all();	
	ret_status = tel_deinit();

	
	if(ret_status != TAPI_API_SUCCESS)	
	{	
		printf("TelTapiDeInit() Failed ret_status = %d \n",ret_status);	
	}
}





/**
 * @brief This tests 
 * @par ID: utc_tapi_display_get_icon_info_00
 * @param [in]  tapi_display_get_icon_req_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
*/	


void utc_tapi_display_get_icon_info_00()
{


		TEST_DEBUG("\n TEST utc_tapi_display_get_icon_info_00 \n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		
		ret_status = tapi_display_get_icon_info(TAPI_DISPLAY_GET_ICON_RSSI);
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_SUCCESS == ret_status)
		{
			printf("/n/******************  get icon  info api successful **************************\\n");
		}
		else
		{
			printf("/n/******************    get icon  info api   case  failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
		tet_infoline("utc_tapi_display_get_icon_info_00 pass");
		expected_event = TAPI_EVENT_DISPLAY_ICON_INFO_IND;
		printf("The expected_event is TAPI_EVENT_DISPLAY_ICON_INFO_IND(%d)\n",expected_event);

		printf("\n\n value of asyn_response %d",async_response);
		G_MAIN_LOOP_RUN(10);
		tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_00 ENDS    ###########\n");


}



/**
 * @brief This tests 
 * @par ID: utc_tapi_display_get_icon_info_01
 * @param [in]  tapi_display_get_icon_req_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
*/	


void utc_tapi_display_get_icon_info_01()
{


		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_01 START    ###########\n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		
		ret_status = tapi_display_get_icon_info(TAPI_DISPLAY_GET_ICON_ACT);
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_SUCCESS == ret_status)
		{
			printf("/n/******************  get icon  info api successful **************************\\n");
		}
		else
		{
			printf("/n/******************    get icon  info api   case  failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
		tet_infoline("utc_tapi_display_get_icon_info_01 pass");
		expected_event = TAPI_EVENT_DISPLAY_ICON_INFO_IND;
		printf("The expected_event is TAPI_EVENT_DISPLAY_ICON_INFO_IND(%d)\n",expected_event);

		printf("\n\n value of asyn_response %d",async_response);
		G_MAIN_LOOP_RUN(10);
		tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_01 ENDS    ###########\n");

}



/**
 * @brief This tests 
 * @par ID: utc_tapi_display_get_icon_info_02
 * @param [in]  tapi_display_get_icon_req_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
*/	


void utc_tapi_display_get_icon_info_02()
{

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_02 START    ###########\n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		
		ret_status = tapi_display_get_icon_info(TAPI_DISPLAY_GET_ICON_ALL);
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_SUCCESS == ret_status)
		{
			printf("/n/******************  get icon  info api successful **************************\\n");
		}
		else
		{
			printf("/n/******************    get icon  info api   case  failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
		tet_infoline("utc_tapi_display_get_icon_info_02 pass");
		expected_event = TAPI_EVENT_DISPLAY_ICON_INFO_IND;
		printf("The expected_event is TAPI_EVENT_DISPLAY_ICON_INFO_IND(%d)\n",expected_event);

		printf("\n\n value of asyn_response %d",async_response);
		G_MAIN_LOOP_RUN(10);
		tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_02 ENDS    ###########\n");



}





/**
 * @brief This tests 
 * @par ID: utc_tapi_display_get_icon_info_03
 * @param [in]  tapi_display_get_icon_req_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
*/	


void utc_tapi_display_get_icon_info_03()
{
		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_03 START    ###########\n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		
		ret_status = tapi_display_get_icon_info(TAPI_DISPLAY_GET_ICON_RSSI - 1);
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_INVALID_INPUT == ret_status)
		{
			printf("/n/******************  get icon  info api invalid input case successful **************************\\n");
		}
		else
		{
			printf("/n/******************    get icon  info api invalid input case failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
		tet_infoline("utc_tapi_display_get_icon_info_03 pass");
expected_event = TAPI_EVENT_DISPLAY_ICON_INFO_IND;
printf("The expected_event is TAPI_EVENT_DISPLAY_ICON_INFO_IND(%d)\n",expected_event);

G_MAIN_LOOP_RUN(10);
tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_03 ENDS    ###########\n");


}





/**
 * @brief This tests 
 * @par ID: utc_tapi_display_get_icon_info_04
 * @param [in]  tapi_display_get_icon_req_type_t
 * @code     	
 * @endcode
 * @return TapiResult_t
*/	


void utc_tapi_display_get_icon_info_04()
{
		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_04 START    ###########\n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		
		ret_status = tapi_display_get_icon_info(TAPI_DISPLAY_GET_ICON_ALL + 1);
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_INVALID_INPUT == ret_status)
		{
			printf("/n/******************  get icon  info api invalid input case successful **************************\\n");
		}
		else
		{
			printf("/n/******************    get icon  info api invalid input case failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_INVALID_INPUT, ret_status);
		tet_infoline("utc_tapi_display_get_icon_info_04 pass");
	expected_event = TAPI_EVENT_DISPLAY_ICON_INFO_IND;
	printf("The expected_event is TAPI_EVENT_DISPLAY_ICON_INFO_IND(%d)\n",expected_event);

G_MAIN_LOOP_RUN(10);
	tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_display_get_icon_info_04 ENDS    ###########\n");



}





/** @} */
