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
* @ingroup TELEPHONY_API TelephonyAPI
* @addtogroup CONFIG Configuration
*/

/**
* @ingroup  CONFIG Configuration
* @addtogroup UTS_CONFIG Unit
*/

/**
* @ingroup  UTS_CONFIG
* @addtogroup UTS_TAPI_CFG_GET_TTY_MODE tel_get_cfg_vocoder_option
* @brief Unit test cases for tel_get_cfg_vocoder_option()
* @par Test function Prototype:
* 	TapiResult_t   tel_get_cfg_vocoder_option( int *pRequestID) 
* @par Test function description:
* It requests to get tty(text telephone) mode.(CDMA only)	 
* @par Important Notes:
*	None
* @param [out] pRequestId 
* - Unique identifier for a particular request.
* - request_id value can be any value from 0 to 255 if the API is returned successfully
* - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message
* The event associated is TAPI_EVENT_CFG_GET_TTY_MODE_CNF
* @pre
*	None
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully. 
* - Refer TapiResult_t for failure and error code 
* @{
*/

/**
* @file	uts_tel_get_cfg_vocoder_option.c
* @brief	This is a suite of unit test cases to test tel_get_cfg_vocoder_option() API function
* @author	Gautam Nand Sinha  (gnand.sinha@samsung.com)
* @version	Initial Creation V0.1
* @date	2009.12.16
*/



#include "uts_tel_get_cfg_vocoder_option.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tel_get_cfg_vocoder_option_000, 1},
	{utc_tel_get_cfg_vocoder_option_001, 2},
        {utc_tel_get_cfg_vocoder_option_002, 3},
        {utc_tel_get_cfg_vocoder_option_003, 4},
        {utc_tel_get_cfg_vocoder_option_004, 5},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for 
   initialize and finalize test module
*/
static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

static void startup()
{
	
	if(tel_init() != TAPI_API_SUCCESS)
	{
          printf("TelTapiInit() Failed \n")
         }
          register_all_events();
		
	return;						
}

static void cleanup()
{
	deregister_all();
	tel_deinit();
	return;
}



void utc_tel_get_cfg_vocoder_option_000()
{
	
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	int request_id=0;
	gmain_loop_for_tapi_events(TIME_DELAY);
        tel_set_cfg_vocoder_option( TAPI_CFG_VOC_OPTION_DEFAULT);
        ret_status = tel_get_cfg_vocoder_option();
        gmain_loop_for_tapi_events(TIME_DELAY);

	if(ret_status  == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_get_cfg_vocoder_option_000 pass\n");
         tet_result(TET_PASS);
         }
          else
          {
           tet_infoline("utc_tel_get_cfg_vocoder_option_000 has failed\n");
           tet_result(TET_FAIL);
           }
}



void utc_tel_get_cfg_vocoder_option_001()
{
	
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	int request_id=0;
	gmain_loop_for_tapi_events(TIME_DELAY);
        tel_set_cfg_vocoder_option( TAPI_CFG_VOC_OPTION_EVRC);
        ret_status = tel_get_cfg_vocoder_option();
        gmain_loop_for_tapi_events(TIME_DELAY);

	if(ret_status  == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_get_cfg_vocoder_option_001 pass\n");
         tet_result(TET_PASS);
         }
          else
          {
           tet_infoline("utc_tel_get_cfg_vocoder_option_001 has failed\n");
           tet_result(TET_FAIL);
           }
}

void utc_tel_get_cfg_vocoder_option_002()
{
	
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	int request_id=0;
	gmain_loop_for_tapi_events(TIME_DELAY);
        tel_set_cfg_vocoder_option( TAPI_CFG_VOC_OPTION_13K);
        ret_status = tel_get_cfg_vocoder_option();
        gmain_loop_for_tapi_events(TIME_DELAY);

	if(ret_status  == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_get_cfg_vocoder_option_002 pass\n");
         tet_result(TET_PASS);
         }
          else
          {
           tet_infoline("utc_tel_get_cfg_vocoder_option_002 has failed\n");
           tet_result(TET_FAIL);
           }
}

void utc_tel_get_cfg_vocoder_option_003()
{
	
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	int request_id=0;
	gmain_loop_for_tapi_events(TIME_DELAY);
        tel_set_cfg_vocoder_option( TAPI_CFG_VOC_OPTION_8K);
        ret_status = tel_get_cfg_vocoder_option();
        gmain_loop_for_tapi_events(TIME_DELAY);

	if(ret_status  == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_get_cfg_vocoder_option_003 pass\n");
         tet_result(TET_PASS);
         }
          else
          {
           tet_infoline("utc_tel_get_cfg_vocoder_option_003 has failed\n");
           tet_result(TET_FAIL);
           }
}

void utc_tel_get_cfg_vocoder_option_004()
{
	
	TapiResult_t   ret_status = TAPI_API_SUCCESS;
	int request_id=0;
	gmain_loop_for_tapi_events(TIME_DELAY);
        tel_set_cfg_vocoder_option( -1);
        ret_status = tel_get_cfg_vocoder_option(); 
        gmain_loop_for_tapi_events(TIME_DELAY);

	if(ret_status  == TAPI_API_SUCCESS)
        {
	 tet_infoline("utc_tel_get_cfg_vocoder_option_004 has failed\n");
         tet_result(TET_FAIL);
         }
          else
          {
           tet_infoline("utc_tel_get_cfg_vocoder_option_004 has passed\n");
           tet_result(TET_FAIL);
           }
}


/** @} */
