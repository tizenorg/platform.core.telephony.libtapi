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
* @ingroup TELEPHONY_API
* @addtogroup TAPI_API_SIM_SAT SIM/SAT
*/

/**
* @ingroup  TAPI_API_SIM_SAT
* @addtogroup UTS_SIM_SAT Unit
*/

/**
* @ingroup  UTS_SIM_SAT
* @addtogroup UTS_TAPI_SIM_GET_LANGUAGE_PREF_INFO tel_get_sim_language
*
* @brief Unit test code for tel_get_sim_language()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_language(TelSimLanguageInfo_t* pSimLpInfo);
* @par Test function description:
*	This API is used to get the language preference information.
* @par Important Notes:
* 	None
* \param [out] pSimLpInfo                      
* 	This parameter returns the language preference information.
* @par Async Response Message:
* 	None
* @pre
*	None
* @post
*	None
* @return
 * \return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tapi_sim_get_sim_language_pref_info.c
* @brief	Test for the tel_get_sim_language() API that is used to get Lock information
* @author	
* @version	Initial Creation V0.1
* @date		2009.12.17
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_get_sim_language.h"
#include "sim_common.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_language_01, 1},
	{utc_tel_get_sim_language_02, 2},
	{NULL, 0}
};


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("tel_get_sim_language Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("tel_get_sim_language Test Finished\n");
	if (tel_deinit() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_deinit() failed");
	}
	return;
}


/*
  Unit Test Case functions
*/

/**
* @brief tel_get_sim_language() API : Positive condition
* @par ID: UTC_TEL_GET_SIM_LANGUAGE_01
* @param [out] pSimLpInfo = valid parameter
* @return TAPI_API_SUCCESS
*/
void utc_tel_get_sim_language_01()
{
	TET_STAMP_START("Normal operation");
	TelSimLanguageInfo_t simLpInfo;
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_language(&simLpInfo));
	TET_STAMP_PASS();
	return;
}

/**
* @brief TelTapiSimGetLockInfo() API : Negative condition(NULL)
* @par ID: UTC_TEL_GET_SIM_LANGUAGE_02
* @param [out] pRequestId = NULL
* @return 
*/
void utc_tel_get_sim_language_02()
{
	TET_STAMP_START("Null parameter test");
	TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_language(NULL));
	TET_STAMP_PASS();
}
/** @} */
