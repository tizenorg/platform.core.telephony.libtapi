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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tet_api.h>
#include <ITapiSat.h>
#include "tet_macro.h"

#include "uts_tel_get_sat_main_menu_info.h"

static void startup();
static void cleanup();

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sat_main_menu_info_01, 1},
	{utc_tel_get_sat_main_menu_info_02, 2},		
	{NULL, 0}
};


static void startup()
{
    tet_infoline("utc_tel_get_sat_main_menu_info Test Start\n");
	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_get_sat_main_menu_info Test Finished\n");
	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
    return;
}


void utc_tel_get_sat_main_menu_info_01()
{

    int ret_status=TAPI_API_SUCCESS;
	tet_printf("Get SAT Main MenuInfo 01\n");
     
	/* Calling TelTapiSatGetMainMenuInfo API */
	ret_status = tel_get_sat_main_menu_info (NULL);
	tet_printf("The Return Status of TelTapiSatGetMainMenuInfo 01() is: %d\n", ret_status);

	TET_CHECK(ret_status == TAPI_API_INVALID_PTR);
	if(ret_status == TAPI_API_INVALID_PTR)
		tet_printf("utc_tel_get_sat_main_menu_info_01 pass\n");
	return;
}

void utc_tel_get_sat_main_menu_info_02()
{

    int ret_status=TAPI_API_SUCCESS;
	tet_printf("Get SAT Main MenuInfo 02\n");

	TelSatSetupMenuInfo_t main_menu ={0,};
     
	/* Calling TelTapiSatGetMainMenuInfo API */
	ret_status = tel_get_sat_main_menu_info (&main_menu);
	tet_printf("The Return Status of tel_get_sat_main_menu_info 02() is: %d\n", ret_status);

	TET_CHECK(ret_status == TAPI_API_SUCCESS);
	if(ret_status == TAPI_API_SUCCESS)
		tet_printf("utc_tel_get_sat_main_menu_info_02 pass\n");
	return;
}
