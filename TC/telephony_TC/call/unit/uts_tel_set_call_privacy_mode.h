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


#ifndef	__UTS_TEL_SET_CALL_PRIVACY_MODE_H_
#define   __UTS_TEL_SET_CALL_PRIVACY_MODE_H_

#include "tapi_callback.h"
#include <tet_api.h>
#include "call_common.h"
#include <ITapiCall.h>

extern TS_UINT g_CallHandle;
extern TS_BOOL call_status;

	static void utc_tel_set_call_privacy_mode_01(void);
	static void utc_tel_set_call_privacy_mode_02(void);
	static void utc_tel_set_call_privacy_mode_03(void);
	static void utc_tel_set_call_privacy_mode_04(void);
	static void utc_tel_set_call_privacy_mode_05(void);
	static void utc_tel_set_call_privacy_mode_06(void);
	static void utc_tel_set_call_privacy_mode_07(void);
	static void utc_tel_set_call_privacy_mode_08(void);
	static void utc_tel_set_call_privacy_mode_09(void);
	static void utc_tel_set_call_privacy_mode_10(void);
	static void utc_tel_set_call_privacy_mode_11(void);

	static void startup();

	static void cleanup();


struct tet_testlist tet_testlist[] = {
	{utc_tel_set_call_privacy_mode_01, 1},
	{utc_tel_set_call_privacy_mode_02, 2},
	{utc_tel_set_call_privacy_mode_03, 3},
	{utc_tel_set_call_privacy_mode_04, 4},
	{utc_tel_set_call_privacy_mode_05, 5},
	{utc_tel_set_call_privacy_mode_06, 6},
	{utc_tel_set_call_privacy_mode_07, 7},
	{utc_tel_set_call_privacy_mode_08, 8},
	{utc_tel_set_call_privacy_mode_09, 9},
	{utc_tel_set_call_privacy_mode_10, 10},
	{utc_tel_set_call_privacy_mode_11, 11},
	{NULL, 0}
	};

#endif

/**
 * @}
 */
