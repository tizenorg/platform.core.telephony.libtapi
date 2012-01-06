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

#include "uts_tel_send_ui_display_status.h"

static void startup();
static void cleanup();

/* Initialize TCM data structures */
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;


struct tet_testlist tet_testlist[] = {
	{utc_tel_send_ui_display_status_01, 1},
	{utc_tel_send_ui_display_status_02, 2},
	{utc_tel_send_ui_display_status_03, 3},
	{NULL, 0}
};


static void startup()
{
    tet_infoline("utc_tel_send_ui_display_status Test Start\n");
	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_send_ui_display_status Test Finished\n");
	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
    return;
}


/**
 * @test		This case tests TapiResult_t TelTapiSatSendUiDisplayStatus(int command_id, sat_engine_ui_display_status status);
 * @brief 		This function is called by passing invalid command_id = -1
 * @return		False
 * @param[in]	command_id = -1
 * @param[in]	status = TAPI_SAT_DISPLAY_SUCCESS
 */
void utc_tel_send_ui_display_status_01()
{
	int ret_val= TAPI_API_SUCCESS;

	ret_val = tel_send_ui_display_status(-1,TAPI_SAT_DISPLAY_SUCCESS);

	TET_CHECK(ret_val == TAPI_API_SAT_INVALID_COMMAND_ID);
}

 /**
  * @test		This case tests TapiResult_t TelTapiSatSendUiDisplayStatus(int command_id, sat_engine_ui_display_status status);
  * @brief 		This function is called by passing sinvalid status = -1
  * @return		False
  * @param[in]	command_id = 1
  * @param[in]	status = -1
 */
void utc_tel_send_ui_display_status_02()
{
	int  ret_val = TAPI_API_SUCCESS;

	ret_val = tel_send_ui_display_status(1,TAPI_SAT_DISPLAY_SUCCESS-1);

	TET_CHECK(ret_val == TAPI_API_INVALID_INPUT);
}

 /**
  * @test		This case tests TapiResult_t TelTapiSatSendUiDisplayStatus(int command_id, sat_engine_ui_display_status status);
  * @brief 		This function is called by invalid status = 2
  * @return		False
  * @param[in]	command_id = 1
  * @param[in]	status = 3
 */
void utc_tel_send_ui_display_status_03()
{
	 int  ret_val = TAPI_API_SUCCESS;

	ret_val = tel_send_ui_display_status(1,TAPI_SAT_DISPLAY_FAIL+1);

	TET_CHECK(ret_val == TAPI_API_INVALID_INPUT);
}










