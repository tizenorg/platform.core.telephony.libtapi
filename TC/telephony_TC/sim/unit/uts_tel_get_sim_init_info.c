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
* @addtogroup	UTS_TEL_GET_SIM_INIT_INFO Unit
*/

/**
* @ingroup	UTS_TEL_GET_SIM_INIT_INFO Unit
* @addtogroup	UTS_TEL_GET_SIM_INIT_INFO_ Uts_Tel_Get_Sim_Init_Info
* @{
*/

/**
* @file uts_tel_get_sim_init_info.c
* @brief This is a suit of unit test cases to test tel_get_sim_init_info() API function
* @author Gautam Nand Sinha (gnand.sinha@samsung.com)
* @version Initial Creation Version 0.1
* @date 2010.06.29
*/


#include "uts_tel_get_sim_init_info.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_init_info_01, 1},
	{utc_tel_get_sim_init_info_02, 2},
	{NULL, 0}
};



static void startup()
{
    tet_infoline("utc_tel_get_sim_init_info Test Start\n");
	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_get_sim_init_info Test Finished\n");
	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
    return;
}


void utc_tel_get_sim_init_info_01()
{
	TET_STAMP_START("utc_tel_get_sim_init_info_01");

    int changed=0,err;
    TelSimCardStatus_t sim_status = 0x00 ;
    err = tel_get_sim_init_info(&sim_status,&changed);
	if(err == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_get_sim_init_info_01 has passed and the data in err-->%d", err);
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_get_sim_init_info_01 has failed and the data in err-->%d", err);
		tet_result(TET_FAIL);
	}
	return;
}


void utc_tel_get_sim_init_info_02()
{
	TET_STAMP_START("utc_tel_get_sim_init_info_02");

	TET_CHECK(TAPI_API_INVALID_PTR,tel_get_sim_init_info(NULL,NULL));
	TET_STAMP_PASS();
	return;
}

/** @} */
