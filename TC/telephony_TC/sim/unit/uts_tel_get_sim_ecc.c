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
* @addtogroup UTS_TAPI_SIM_GET_ECC_INFO tel_get_sim_ecc
*
* @brief Unit test code for tel_get_sim_ecc()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_ecc(TelSimEccData_t* pSimEccInfo, int *pCount);
* @par Test function description:
* 	This function is a synchronous API used to get ECC data.
* @par Important Notes:
* 	None
* @param [out] pSimEccInfo
* 	This parameter returns the SIM emergency call code information like ecc length, service type etc.
* @param [out] pCount
* 	Specifies number of ECC records.
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
* @file		uts_tel_get_sim_ecc.c
* @brief	Test for the tel_get_sim_ecc() API that is used to get ecc info(Card type)
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include "tet_tapi_util.h"
#include "uts_tel_get_sim_ecc.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_ecc_01, 1},
	{utc_tel_get_sim_ecc_02, 2},
	{utc_tel_get_sim_ecc_03, 3},
	{NULL, 0}
};

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("uts_tel_get_sim_ecc Test Start\n");
	 tel_register_app_name("org.samsung.test");
	return;
}


static void cleanup()
{
	tet_infoline("uts_tel_get_sim_ecc Test Finished\n");
	return;
}

/**
* @brief tel_get_sim_ecc() API : Normal opertion
* @par ID: UTC_TEL_GET_SIM_ECC_01
* @param [out] &ecc_info
* @param [out] &ecc_rec_count
* @return TAPI_API_SUCCESS
*/
void utc_tel_get_sim_ecc_01()
{
	TET_STAMP_START("Normal test");
	int  ecc_rec_count = 0;
	TelSimEccData_t  ecc_info;
	memset(&ecc_info, 0, sizeof(ecc_info));

	TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_ecc(&ecc_info, &ecc_rec_count));

	tet_printf("EEC count [%d]\n",ecc_rec_count);

	int i;
	for (i=0 ; i<ecc_rec_count ; i++)
	{
		tet_printf("[UeccInfo %d] [bUsed = %d] [szEccAlphaIdentifier = 0x%x",
		i, ecc_info.UeccInfo[i].bUsed, ecc_info.UeccInfo[i].szEccAlphaIdentifier);
	}

  /*
        tet_printf("ECC1 [%s]\n",ecc_info.EccInfo.szEcc1);
        tet_printf("ECC2 [%s]\n",ecc_info.EccInfo.szEcc2);
        tet_printf("ECC3 [%s]\n",ecc_info.EccInfo.szEcc3);
        tet_printf("ECC4 [%s]\n",ecc_info.EccInfo.szEcc4);
        tet_printf("ECC5 [%s]\n",ecc_info.EccInfo.szEcc5);
*/
        TET_STAMP_PASS();
	return;
}

/**
* @brief tel_get_sim_ecc() API : Negative condition(First parameter is Null)
* @par ID: UTC_TEL_GET_SIM_ECC_02
* @param [out] &ecc_info = NULL
* @param [out] &ecc_rec_count
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_ecc_02()
{
    int ecc_rec_count = 0;
    TET_STAMP_START("GET ECC INFO WITH NULL OUT PARAM");

    /* Calling TAPI GET ECC INFORMATION API */
    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_ecc(NULL, &ecc_rec_count));

    TET_STAMP_PASS();
    return;
}

/**
* @brief tel_get_sim_ecc() API : Negative condition(Second parameter is Null)
* @par ID: UTC_TEL_GET_SIM_ECC_03
* @param [out] &ecc_info
* @param [out] &ecc_rec_count = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_ecc_03()
{
    TelSimEccData_t  ecc_info;
    memset(&ecc_info, 0, sizeof(ecc_info));

    TET_STAMP_START("GET ECC INFO WITH NULL OUT PARAM");

    /* Calling TAPI GET ECC INFORMATION API */
    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_ecc(&ecc_info,NULL));

    TET_STAMP_PASS();
    return;
}

/** @} */
