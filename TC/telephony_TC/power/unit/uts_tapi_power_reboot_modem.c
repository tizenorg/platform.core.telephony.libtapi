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
* @addtogroup UTS_TAPI_POWER_FLIGHT_MODE tapi_power_flight_mode
* @{
*/



//add includes


#include "uts_tapi_power_reboot_modem.h"



void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;


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




void utc_tapi_power_reboot_modem_01()
{

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_reboot_modem_01 START    ###########\n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		
		ret_status = tel_reset_modem();
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_SUCCESS == ret_status)
		{
			printf("/n/******************  reboot modem api successful **************************\\n");
		}
		else
		{
			printf("/n/******************   reboot modem api  case  failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
		tet_infoline("utc_tapi_power_reboot_modem pass");

		G_MAIN_LOOP_RUN(10);
		//TET_CHECK_TEMP(true, async_response);	
		tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_power_reboot_modem 01 ENDS    ###########\n");


}

void utc_tapi_power_reboot_modem_02()
{

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_reboot_modem_02 START    ###########\n");
		
		TapiResult_t  ret_status ;

		async_response = false;//initialize
		tel_init();	
	//Calling rebooting of Modem after DeInitializing of Tapi 	
		ret_status = tel_reset_modem();
		
		printf("/n return value = %d\n",ret_status);
		
		if(TAPI_API_SUCCESS != ret_status)
		{
			printf("/n/******************  reboot modem invalid api successful **************************\\n");
		}
		else
		{
			printf("/n/******************   reboot modem api  case  failed **************************\\n");
		}

		TET_CHECK_EQUAL(TAPI_API_SUCCESS, ret_status);
		tet_infoline("utc_tapi_power_reboot_modem pass");

		G_MAIN_LOOP_RUN(10);
		//TET_CHECK_TEMP(true, async_response);	
		tet_result(TET_PASS);

		TEST_DEBUG("\n\n\n\n\n########     TEST utc_tapi_power_reboot_modem 01 ENDS    ###########\n");


}
