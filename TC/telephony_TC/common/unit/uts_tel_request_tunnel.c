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
#include <TapiCommon.h>
#include "tet_macro.h"
#include "uts_tel_request_tunnel.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tapi_request_tunnel_01, 1},
	{utc_tapi_request_tunnel_02, 2},
	{NULL, 0}
};


static void startup()
{
 
}

static void cleanup()
{

}

void utc_tapi_request_tunnel_01()
{

	int  ret = TAPI_API_SUCCESS;
	int req_id;

	ret = tel_request_tunnel(NULL, &req_id);

	tet_printf ("tel_request_tunnel return=%d\n", ret);

	TET_CHECK_NOT_EQUAL(ret , TAPI_API_SUCCESS);
}


void utc_tapi_request_tunnel_02()
{
	int  ret = TAPI_API_SUCCESS;
	int req_id;
	TelTunnelInfo_t tunnel_info;

	ret = tel_request_tunnel(&tunnel_info, NULL);

	tet_printf ("tel_request_tunnel return=%d\n", ret);

	TET_CHECK_NOT_EQUAL(ret , TAPI_API_SUCCESS);

}

/** @} */
