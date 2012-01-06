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
* @addtogroup	SIM
*/

/**
* @ingroup	SIM
* @addtogroup	UTS_TEL_GET_SIM_PB_INIT_INFO Unit
*/

/**
* @ingroup	UTS_TEL_GET_SIM_PB_INIT_INFO Unit
* @addtogroup	UTS_TEL_GET_SIM_PB_INIT_INFO_ Uts_Tel_Get_Sim_Pb_Init_Info
* @{
*/

/**
* @file uts_tel_get_sim_pb_init_info.c
* @brief This is a suit of unit test cases to test tel_get_sim_pb_init_info() API function
* @author Gautam Nand Sinha (gnand.sinha@samsung.com)
* @version Initial Creation Version 0.1
* @date 2010.06.29
*/
#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>
#include <string.h>
	
#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_get_sim_pb_init_info.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_pb_init_info_01, 1},
	{utc_tel_get_sim_pb_init_info_02, 2},
	{utc_tel_get_sim_pb_init_info_03, 3},
	{utc_tel_get_sim_pb_init_info_04, 4},
	{NULL, 0}
};


static void startup()
{
    tet_infoline("utc_tel_get_sim_pb_init_info Test Start\n");

	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_get_sim_pb_init_info Test Finished\n");

	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("cleanup fail. tel_deinit() failed");
	}
    return;
}


void utc_tel_get_sim_pb_init_info_01()
{
    int err,init_completed = 0;
    TelSimPbList_t pb_list = {0,}; 
    int first_index = 0;
    err = tel_get_sim_pb_init_info(&init_completed, &pb_list,&first_index );
	if(err == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_get_sim_pb_init_info_01 has passed and the data in err-->%d", err);
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_get_sim_pb_init_info_01 has failed and the data in err-->%d", err);
		tet_result(TET_FAIL);
	}
	return;       
}


void utc_tel_get_sim_pb_init_info_02()
{
    int err;
    TelSimPbList_t pb_list = {0,}; 
    int first_index = 0;
    err = tel_get_sim_pb_init_info(NULL, &pb_list,&first_index );
    if(err != TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_get_sim_pb_init_info_02 has passed and the data in err-->%d", err);
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_get_sim_pb_init_info_02 has failed and the data in err-->%d", err);
		tet_result(TET_FAIL);
	}
	return;
}

void utc_tel_get_sim_pb_init_info_03()
{
    int err;
    
    int first_index = 0;
    err = tel_get_sim_pb_init_info(NULL, NULL,&first_index );
    if(err != TAPI_API_SUCCESS)
    {
		tet_printf("utc_tel_get_sim_pb_init_info_03 has passed and the data in err-->%d", err);
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_get_sim_pb_init_info_03 has failed and the data in err-->%d", err);
		tet_result(TET_FAIL);
	}
	return;      
}

void utc_tel_get_sim_pb_init_info_04()
{
    int err,init_completed = 0;
   
    err = tel_get_sim_pb_init_info(&init_completed, NULL,NULL );
	if(err != TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_get_sim_pb_init_info_04 has passed and the data in err-->%d", err);
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_get_sim_pb_init_info_04 has failed and the data in err-->%d", err);
		tet_result(TET_FAIL);
	}
	return;   
}

/** @} */
