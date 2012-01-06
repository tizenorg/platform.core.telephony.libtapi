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
* @addtogroup UTS_TAPI_SIM_GET_IMSI_INFO tel_get_sim_imsi
*
* @brief Unit test code for tel_get_sim_imsi()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_imsi(TelSimImsiInfo_t* pSimImsiInfo);
* @par Test function description:
* 	This function is used to get ISMI Information
* @par Important Notes:
* 	None
* @param [out] pSimImsiInfo This parameter returns the IMSI information
* @code
typedef struct
{
	BOOL	bValid;			// Valid(ACTIVATE) IMSI (TRUE) or Deactivated (FALSE)
	char	szMcc[TAPI_SIM_MCC_CODE_LEN+1];	// mobile country code
	char	szMnc[TAPI_SIM_MNC_CODE_LEN+1];	// mobile network code
	char	szMsin[TAPI_SIM_MSIN_CODE_LEN+1]; // mobile station identification number
} TelSimImsiInfo_t;
* @endcode
* @par Async Response Message:
* 	None
* @pre
*	None
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_get_sim_imsi.c
* @brief	Test for the tel_get_sim_imsi() API that is used to get imsi information
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include "tet_tapi_util.h"
#include "uts_tel_get_sim_imsi.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_imsi_01, 1},
	{utc_tel_get_sim_imsi_02, 2},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("uts_tel_get_sim_imsi Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("uts_tel_get_sim_imsi Test Finished\n");
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
* @brief tel_get_sim_imsi() API : Positive condition(Get IMSI Information)
* @par ID: UTC_TEL_GET_SIM_IMSI_01
* @param [out] &sim_imsi_info
* @return TAPI_API_SUCCESS
* @par Expected Result:
*  - sim_imsi_info.szMcc != NULL // mobile country code is returned
*  - sim_imsi_info.szMnc != NULL // mobile network code is returned
*  - sim_imsi_info.szMsin != NULL // mobile station identification number is returned
*/
void utc_tel_get_sim_imsi_01()
{
    TET_STAMP_START("Normal operation");
    TelSimImsiInfo_t sim_imsi_info;

    TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_imsi(&sim_imsi_info));

	if( sim_imsi_info.szMcc != NULL
		&& sim_imsi_info.szMnc != NULL
		&& sim_imsi_info.szMsin != NULL)
	{
		tet_printf("[SIM APP] IMSI [mcc,mnc,msin]= ");
		tet_printf(" [%s]",sim_imsi_info.szMcc);
		tet_printf(" [%s]",sim_imsi_info.szMnc);
		tet_printf(" [%s]",sim_imsi_info.szMsin);
		TET_STAMP_PASS();
		return;
	}
	else
	{
		tet_printf("[SIM APP] IMSI error [mcc,mnc,msin]=[%x,%x,%x]\n",
					sim_imsi_info.szMcc, sim_imsi_info.szMnc,
					sim_imsi_info.szMsin);
		TET_STAMP_FAIL();
		return;
	}
	return;
}

/**
* @brief tel_get_sim_imsi() API : Negative condition(Null Parameter test)
* @par ID: UTC_TEL_GET_SIM_IMSI_02
* @param [out] &sim_imsi_info = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_imsi_02()
{
    TET_STAMP_START("null pointer operation");

    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_imsi(NULL));
    TET_STAMP_PASS();
    return;
}

/** @} */
