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
* @addtogroup UTS_TAPI_SIM_GET_MSISDN tel_get_sim_msisdn
*
* @brief Unit test code for tel_get_sim_msisdn()
* @par Test function Prototype:
* 	int tel_get_sim_msisdn(TelSimSubscriberInfo_t *subscriber);
* @par Test function description:
* 	This function is used to get SIM MSISDN Information
* @par Important Notes:
* 	None
* @param [out] subscriber This parameter returns the MSISDN information
* @code
* typedef struct
* {
* 	char num[TAPI_SIM_XDN_DIALING_NUMBER_LEN+1]; // MSISDN number. If not exist, Null string will be returned
* 	char name[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN+1]; // MSISDN name. If not exist, Null string will be returned
* }TelSimSubscriberInfo_t;
*
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
* @file		uts_tel_get_sim_msisdn.c
* @brief	Test for the tel_get_sim_msisdn() API that is used to get MSISDN information
* @author
* @version	Initial Creation V0.1
* @date		2011.09.13
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include "tet_tapi_util.h"
#include "uts_tel_get_sim_msisdn.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_msisdn_01, 1},
	{utc_tel_get_sim_msisdn_02, 2},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("uts_tel_get_sim_msisdn Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("uts_tel_get_sim_msisdn Test Finished\n");
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
* @brief tel_get_sim_msisdn() API : Positive condition(Get MSISDN Information)
* @par ID: UTC_TEL_GET_SIM_MSISDN_01
* @param [out] &msisdn
* @return TAPI_API_SUCCESS
* @par Expected Result:
*  - msisdn information
*/
void utc_tel_get_sim_msisdn_01()
{
    TET_STAMP_START("Normal operation");
    TelSimSubscriberInfo_t msisdn;
	int api_ret = 0;
	api_ret = tel_get_sim_msisdn(&msisdn);

	if (api_ret == TAPI_API_SUCCESS) {
		TET_STAMP_PASS();
		return;
	} else {
		TET_STAMP_FAIL();
		return;
	}
	return;
}

/**
* @brief tel_get_sim_msisdn() API : Negative condition(Null Parameter test)
* @par ID: UTC_TEL_GET_SIM_MSISDN_02
* @param [out] &cphs = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_msisdn_02()
{
    TET_STAMP_START("null pointer operation");
    TelSimSubscriberInfo_t msisdn;

    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_msisdn(NULL));
    TET_STAMP_PASS();
    return;
}

/** @} */
