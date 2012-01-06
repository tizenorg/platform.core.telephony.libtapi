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

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>
#include "tapi_common.h"
#include "tet_macro.h"
#include "uts_tel_deinit.h"
#include <glib.h>
/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tapi_deinit_01, 1},
	{NULL, 0}
};

static void startup()
{
}

static void cleanup()
{
}

void utc_tapi_deinit_01()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;

	if(tel_init() == TAPI_API_SUCCESS){
		returnStatus = tel_deinit();
	}

	tet_printf ("The return value is %d\n", returnStatus);

	TET_CHECK_EQUAL(returnStatus ,TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		tet_printf("utc_tapi_deinit_01 pass\n");
}

/** @} */












