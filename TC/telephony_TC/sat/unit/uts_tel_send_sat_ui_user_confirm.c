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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <tet_api.h>
#include <ITapiSat.h>
#include "tet_macro.h"

#include "uts_tel_send_sat_ui_user_confirm.h"

static void startup();
static void cleanup();

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;

struct tet_testlist tet_testlist[] = {
	{utc_tel_send_sat_ui_user_confirm_01, 1},
	{utc_tel_send_sat_ui_user_confirm_02, 2},		
	{NULL, 0}
};

static void startup()
{
    tet_infoline("utc_tel_send_sat_ui_user_confirm Test Start\n");
	if ( tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
    return;
}


static void cleanup()
{
    tet_infoline("utc_tel_send_sat_ui_user_confirm Test Finished\n");
	if ( tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
    return;
}

void utc_tel_send_sat_ui_user_confirm_01()
{
	TapiResult_t ret_status = TAPI_API_SUCCESS;

	ret_status = tel_send_sat_ui_user_confirm (NULL);

	tet_printf("utc_tel_send_sat_ui_user_confirm_01 return=%d\n", ret_status);
	TET_CHECK(ret_status == TAPI_API_INVALID_PTR);
	return;
}

void utc_tel_send_sat_ui_user_confirm_02()
{
	TapiResult_t ret_status = TAPI_API_SUCCESS;
	TelSatUiUserConfirmInfo_t userConfirmData = {0,};

	userConfirmData.dataLen = -1;

	ret_status = tel_send_sat_ui_user_confirm(&userConfirmData);

	tet_printf("utc_tel_send_sat_ui_user_confirm_02 return=%d\n", ret_status);
	TET_CHECK(ret_status == TAPI_API_INVALID_INPUT);
	return;
}
