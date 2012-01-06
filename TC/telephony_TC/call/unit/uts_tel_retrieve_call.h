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


#ifndef	__UTS_TEL_RETRIEVE_CALL_H_
#define   __UTS_TEL_RETRIEVE_CALL_H_

/*  Includes */


#include "tapi_callback.h"
#include <tet_api.h>
#include "call_common.h"

extern TS_UINT g_CallHandle;
extern TS_BOOL call_status;



/**
* @test   This test case tests  TapiResult_t	tel_retrieve_call ( unsigned int  CallHandle, int *pRequestId) ;
* @brief  This API is used to test MT call
* @return  tapi_api_err_t
*/


	static void utc_tel_retrieve_call_01(void);

	static void utc_tel_retrieve_call_02(void);

	static void utc_tel_retrieve_call_03(void);


	static void startup();

	static void cleanup();


struct tet_testlist tet_testlist[] = {
	{utc_tel_retrieve_call_01, 1},
	{utc_tel_retrieve_call_02, 2},
	{utc_tel_retrieve_call_03, 3},
	{NULL, 0}
	};

#endif

/**
 * @}
 */
