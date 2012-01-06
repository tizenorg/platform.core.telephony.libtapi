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

#include "uts_tel_select_sat_menu.h"

static void startup();
static void cleanup();

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tel_select_sat_menu_01, 1},
	{utc_tel_select_sat_menu_02, 2},
	{NULL, 0}
};


static void startup()
{
    tet_infoline("tel_select_sat_menu Test Start\n");
	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("tel_select_sat_menu Test Finished\n");
	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
    return;
}


/**
 * @test		This case tests BOOLuts_tapi_sat_menu_selection_envelope(sat_engine_main_menu_title_t* main_menu_title);
 * @brief		This function sends the UI display status to sat engine for specific command.
 * @return		TRUE.
 * @param[out]	main_menu_title
 */
void utc_tel_select_sat_menu_01()
{
	/* Test SAT Menu Selection Envelope Command - Successful case */

	int ret_status = TAPI_API_SUCCESS;
	int reqId = -1;

	/* Calling tel_select_sat_menu API */
	ret_status = tel_select_sat_menu(NULL,&reqId);
	
	TET_CHECK(ret_status == TAPI_API_INVALID_PTR);
}


/**
 * @test		This case tests BOOLuts_tapi_sat_menu_selection_envelope(sat_engine_main_menu_title_t* main_menu_title);
 * @brief		This function is called by passing main_menu_title = NULL
 * @return		FALSE
 * @param[out]	main_menu_title
 */
void utc_tel_select_sat_menu_02()
{
	TelSatMenuSelectionReqInfo_t menu_selection_info;

	menu_selection_info.itemIdentifier =1;
	int ret_status = TAPI_API_SUCCESS;

	/* Calling tel_select_sat_menu API */
	ret_status = tel_select_sat_menu(&menu_selection_info,NULL);
	TET_CHECK(ret_status == TAPI_API_INVALID_PTR);
}




