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

#include "gprs_common.h"

#ifndef  _UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_
#define  _UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_

void utc_tel_deactivate_gprs_pdp_01();
void utc_tel_deactivate_gprs_pdp_02();
void utc_tel_deactivate_gprs_pdp_03();
void utc_tel_deactivate_gprs_pdp_04();

static void startup();
static void cleanup();

struct tet_testlist tet_testlist[] = {
	{utc_tel_deactivate_gprs_pdp_01, 1},
	{utc_tel_deactivate_gprs_pdp_02, 2},
	{utc_tel_deactivate_gprs_pdp_03, 3},
	{utc_tel_deactivate_gprs_pdp_04, 4},
	{NULL, 0}
	};

#endif  //_UTC_TAPI_GPRS_PDP_DEACTIVATION_SET_
