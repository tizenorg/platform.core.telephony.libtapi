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
#include "uts_tel_download_sat_event.h"

static void startup();
static void cleanup();

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tel_download_sat_event_01, 1},
	{utc_tel_download_sat_event_02, 2},
	{NULL, 0}
};


static void startup()
{
    tet_infoline("utc_tel_download_sat_event Test Start\n");
	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_download_sat_event Test Finished\n");
	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
    return;
}


/**
 * @test		This case tests BOOL sat_engine_get_main_menu_title(sat_engine_main_menu_title_t* main_menu_title);
 * @brief		This function is called by passing main_menu_title = NULL
 * @return		TAPI_API_INVALID_PTR
 * @param[out]	main_menu_title
 */
void utc_tel_download_sat_event_01()
{
	int reqId=-1;
	int ret_status;

	/* Calling tel_download_sat_event API */
	ret_status = tel_download_sat_event(NULL,&reqId);

	tet_printf("ret_status[%d]\n",ret_status);

	TET_CHECK(ret_status == TAPI_API_INVALID_PTR);
	if(ret_status == TAPI_API_INVALID_PTR)
		tet_printf("utc_tel_download_sat_event_01 pass\n");
	return;
}


/**
 * @test		This case tests BOOL sat_engine_get_main_menu_title(sat_engine_main_menu_title_t* main_menu_title);
 * @brief		This function is called by passing NULL request ID
 * @return		TAPI_API_INVALID_PTR
 * @param[out]	main_menu_title
 */
void utc_tel_download_sat_event_02()
{
	TelSatEventDownloadReqInfo_t  sat_event_info;
	int reqId=-1;

	/* Test SAT Event Download Envelope Command - Invalid input (NULL pointer) Handling */

	int ret_status;

	sat_event_info.eventDownloadType=5;//Idle Screen Available Event
	sat_event_info.u.bIdleScreenAvailable==TRUE;

	/* Calling tel_download_sat_event API */
	ret_status = tel_download_sat_event(&sat_event_info,NULL);

	tet_printf("ret_status[%d]\n",ret_status);
	
	TET_CHECK(ret_status == TAPI_API_INVALID_PTR);
	if(ret_status == TAPI_API_INVALID_PTR)
		tet_printf("utc_tel_download_sat_event_02 pass\n");
	return;
}
