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


#ifndef	__UTS_TEL_GET_CALL_ACT_LINE_H_
#define   __UTS_TEL_GET_CALL_ACT_LINE_H_

/*  Includes */

#include "tapi_callback.h"
#include <tet_api.h>
#include "call_common.h"


extern TS_UINT g_CallHandle;
extern TS_BOOL call_status;

	static void utc_tel_get_call_act_line_01(void);

	static void utc_tel_get_call_act_line_02(void);

	static void startup();

	static void cleanup();


struct tet_testlist tet_testlist[] = {
	{utc_tel_get_call_act_line_01, 1},
	{utc_tel_get_call_act_line_02, 2},
	{NULL, 0}
	};

#endif

/**
 * @}
 */
