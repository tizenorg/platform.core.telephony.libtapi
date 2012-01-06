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
* @ingroup	TELEPHONY_FRAMEWORK_API
* @addtogroup	MMS
*/

/**
* @ingroup	SOUND
* @addtogroup	UTS_TEL_GET_SOUND_MUTE_STATUS Unit
*/

/**
* @ingroup	UTS_TEL_GET_SOUND_MUTE_STATUS Unit
* @addtogroup	UTS_TEL_GET_SOUND_MUTE_STATUS_ Uts_Tel_Get_Sound_Mute_Status
* @{
*/

/**
* @file uts_tel_get_sound_mute_status.c
* @brief This is a suit of unit test cases to test tel_get_sound_mute_status() API function
* @author Gautam Nand Sinha (gnand.sinha@samsung.com)
* @version Initial Creation Version 0.1
* @date 2008.09.08
*/

#include "uts_tel_get_sound_mute_status.h"


static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;


static TapiResult_t gStartupSuccess;

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module */

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;

	gStartupSuccess =  tel_init();
	if(gStartupSuccess != TAPI_API_SUCCESS)
	{
		printf("tel_init() Failed gStartupSuccess = %d \n",gStartupSuccess);
	}
		register_all_events();
	tel_register_app_name("org.samsung.test");
	if(tel_get_app_name(&dname))
		printf("Dbus Get app name error\n");
	else 
		printf("Dbus Name is %s\n",dname.name);
		
	tel_check_service_ready(&bStatus);
	printf("Telephony service  %d\n",bStatus);

}

static void cleanup()
{
	deregister_all();
	tel_deinit();
	return;

}

struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sound_mute_status_000, 1},
	{utc_tel_get_sound_mute_status_001, 2},
	{utc_tel_get_sound_mute_status_002, 3},
	{NULL, 0}
};





/**
 * @brief This tests
 * @par ID:
 *  utc_tel_get_sound_mute_status_000
 * @param [out]  pRequestId
 * @code
 * @endcode
 * @return TapiResult_t
*/


void utc_tel_get_sound_mute_status_000()
{


	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	tel_set_sound_mute_status(TAPI_SOUND_MIC_MUTE,&pRequestId);
        returnStatus = tel_get_sound_mute_status(&pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);
	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);

	
	if(returnStatus == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_set_sound_mute_status_000 pass\n");
         tet_result(TET_PASS);
         }
          else
          {
           tet_infoline("utc_tel_set_sound_mute_status_000 has failed\n");
           tet_result(TET_FAIL);
           }



}


/**
 * @brief This tests
 * @par ID:
 *  utc_tel_get_sound_mute_status_001
 * @param [out]  pRequestId
 * @code
 * @endcode
 * @return TapiResult_t
*/



void utc_tel_get_sound_mute_status_001()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	tel_set_sound_mute_status(TAPI_SOUND_MIC_UNMUTE,&pRequestId);
        returnStatus = tel_get_sound_mute_status(&pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);
	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);

	if(returnStatus == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_set_sound_mute_status_001 pass\n");
         tet_result(TET_PASS);
         }
          else
          {
           tet_infoline("utc_tel_set_sound_mute_status_001 has failed\n");
           tet_result(TET_FAIL);
           }


}




/**
 * @brief This tests
 * @par ID:
 *  utc_tel_get_sound_mute_status_002
 * @param [out]  pRequestId
 * @code
 * @endcode
 * @return TapiResult_t
*/


void utc_tel_get_sound_mute_status_002()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
        returnStatus = tel_get_sound_mute_status(NULL);
	gmain_loop_for_tapi_events(TIME_DELAY);

	tet_printf ("The return value is %d\n", returnStatus);

	if(returnStatus == TAPI_API_INVALID_PTR)
    {
		 tet_infoline("utc_tel_set_sound_mute_status_002 pass\n");
         tet_result(TET_PASS);
     }
     else
     {
           tet_infoline("utc_tel_set_sound_mute_status_002 has failed\n");
           tet_result(TET_FAIL);
      }


}

/** @} */

