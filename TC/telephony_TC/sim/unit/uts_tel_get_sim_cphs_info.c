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
* @addtogroup UTS_TAPI_SIM_GET_CPHS_INFO tel_get_sim_cphs_info
*
* @brief Unit test code for tel_get_sim_cphs_info()
* @par Test function Prototype:
* 	int tel_get_sim_cphs_info(TelSimCphsLocalInfo_t *cphs);
* @par Test function description:
* 	This function is used to get CPHS specific Information
* @par Important Notes:
* 	None
* @param [out] cphs This parameter returns the CPHS specific information
* @code
* typedef struct
* {
*	int b_used; // used or not
*	TelSimCphsInfo_t cphsinfo;	// cphs information
*	TelSimCphsCustomerServiceProfileInfo_t csp;	// customer service profile
*	TelSimCphsInformationNum_t infn;	// information numbers
*	TelSimCphsOperatorName_t opname;	// operator name
*	TelSimCphsOperatorShortName_t opshortform;	//operator short form
*	TelSimDynamicFlagsInfo_t dflagsinfo; // Dynamic flags
*	TelSimDynamic2FlagsInfo_t d2flagsinfo; // Dynamic2 flags
* }TelSimCphsLocalInfo_t;
* @endcode
* @par Async Response Message:
* 	None
* @pre
*	None
* @post
*	None
* @return
* Return Type : (int)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_get_sim_cphs_info.c
* @brief	Test for the tel_get_sim_cphs_info() API that is used to get CPHS specific information
* @author
* @version	Initial Creation V0.1
* @date		2011.08.29
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include "tet_tapi_util.h"
#include "uts_tel_get_sim_cphs_info.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_cphs_info_01, 1},
	{utc_tel_get_sim_cphs_info_02, 2},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("uts_tel_get_sim_cphs_info Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("uts_tel_get_sim_cphs_info Test Finished\n");
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
* @brief tel_get_sim_cphs_info() API : Positive condition(Get CPHS specific Information)
* @par ID: UTC_TEL_GET_SIM_CPHS_INFO_01
* @param [out] &cphs
* @return TAPI_API_SUCCESS
* @par Expected Result:
*  - CPHS information
*/
void utc_tel_get_sim_cphs_info_01()
{
    TET_STAMP_START("Normal operation");
    TelSimCphsLocalInfo_t cphs;

    TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_cphs_info(&cphs));

	if( cphs.b_used == 0 || cphs.b_used == 1)
	{
		tet_printf("b_used[%d]",cphs.b_used);
		TET_STAMP_PASS();
		return;
	}
	else
	{
		TET_STAMP_FAIL();
		return;
	}
	return;
}

/**
* @brief tel_get_sim_cphs_info() API : Negative condition(Null Parameter test)
* @par ID: UTC_TEL_GET_SIM_CPHS_INFO_02
* @param [out] &cphs = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_cphs_info_02()
{
    TET_STAMP_START("null pointer operation");
    TelSimCphsLocalInfo_t cphs;

    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_cphs_info(NULL));
    TET_STAMP_PASS();
    return;
}

/** @} */
