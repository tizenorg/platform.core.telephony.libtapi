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
* @file    uts_tapi_power_get_phone_power_status.h
* @brief	This is a suite of unit test cases to test Telephony power API function
* @author	Sagar Aggarwal  (a.sagar@samsung.com)
* @version	Initial Creation 
* @date		[2009-05-13]
*/



#include "power_common.h"
#include <TapiCommon.h>

#ifndef  _UTC_TAPI_POWER_STATUS_
#define  _UTC_TAPI_POWER_STATUS_



void utc_tapi_power_get_phone_power_status_00();

void utc_tapi_power_get_phone_power_status_01();


static void startup();

static void cleanup();

struct tet_testlist tet_testlist[] = {
	{utc_tapi_power_get_phone_power_status_00, 1},
	{utc_tapi_power_get_phone_power_status_01, 2},
	{NULL, 0}
	};



#endif  //_UTC_TAPI_POWER_STATUS_
