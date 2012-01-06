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
* @addtogroup UTS_TAPI_SIM_GET_MAILBOX_INFO tel_get_sim_mailbox_info
*
* @brief Unit test code for tel_get_sim_mailbox_info()
* @par Test function Prototype:
* 	int tel_get_sim_mailbox_info(TelSimMailboxNumbers_s *mbox);
* @par Test function description:
* 	This function is used to get mailbox number Information
* @par Important Notes:
* 	None
* @param [out] mbox This parameter returns the mailbox information
* @code
* typedef struct
* {
* 	int b_cphs; // current data is from CPHS SIM or not
* 	TelSimDialingNumberInfo_t voice_line1; // voice line1 mailbox number
* 	TelSimDialingNumberInfo_t voice_line2; // voice line2 mailbox number (only available in CPHS)
* 	TelSimDialingNumberInfo_t video; // video mailbox number
* 	TelSimDialingNumberInfo_t fax; // fax mailbox number
* 	TelSimDialingNumberInfo_t email; // email mailbox number
* }TelSimMailboxNumbers_s;
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
* @file		uts_tel_get_sim_mailbox_info.c
* @brief	Test for the tel_get_sim_mailbox_info() API that is used to get mailbox information
* @author
* @version	Initial Creation V0.1
* @date		2011.08.29
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include "tet_tapi_util.h"
#include "uts_tel_get_sim_mailbox_info.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_mailbox_info_01, 1},
	{utc_tel_get_sim_mailbox_info_02, 2},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("uts_tel_get_sim_mailbox_info Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("uts_tel_get_sim_mailbox_info Test Finished\n");
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
* @brief tel_get_sim_mailbox_info() API : Positive condition(Get mailbox Information)
* @par ID: UTC_TEL_GET_SIM_MAILBOX_INFO_01
* @param [out] &mailbox
* @return TAPI_API_SUCCESS
* @par Expected Result:
*  - mailbox number
*/
void utc_tel_get_sim_mailbox_info_01()
{
    TET_STAMP_START("Normal operation");
    TelSimMailboxNumbers_s mailbox;

    TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_mailbox_info(&mailbox));

	if( mailbox.b_cphs == 0 || mailbox.b_cphs == 1)
	{
		tet_printf("b_cphs[%d]",mailbox.b_cphs);
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
* @brief tel_get_sim_mailbox_info() API : Negative condition(Null Parameter test)
* @par ID: UTC_TEL_GET_SIM_MAILBOX_INFO_02
* @param [out] &mailbox = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_mailbox_info_02()
{
    TET_STAMP_START("null pointer operation");
    TelSimMailboxNumbers_s mailbox;

    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_mailbox_info(NULL));
    TET_STAMP_PASS();
    return;
}

/** @} */
