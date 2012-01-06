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
* @addtogroup UTS_TEL_GET_SIM_TYPE tel_get_sim_type
*
* @brief Unit test code for tel_get_sim_type()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_type(TelSimCardType_t *pCardInfo);
* @par Test function description:
* 	This function is a synchronous API which gets the Card Type whether SIM Card is a GSM SIM or a 3G SIM.
* @par Important Notes:
* 	None
* @param [in]
* 	None
* @param [out] pCardInfo
* This parameter returns the SIM card type information like whether it is GSM SIM card or USIM or it is of unknown type.
*  - TAPI_SIM_CARD_TYPE_UNKNOWN
*  - TAPI_SIM_CARD_TYPE_GSM
*  - TAPI_SIM_CARD_TYPE_USIM
* @par Async Response Message
* 	None
* @pre
*	Before test, prepare GSM SIM and USIM cards.
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_get_sim_type.c
* @brief	Test for the tel_get_sim_type() API that is used to get SIM Card type
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include "uts_tel_get_sim_type.h"
#include "tet_tapi_util.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_type_01, 1},
	{utc_tel_get_sim_type_02, 2},
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
* @brief tel_get_sim_type() API : Normal condition
* @par ID: UTC_TEL_GET_SIM_TYPE_01
* @param [out] &cardInfo
* @return TAPI_API_SUCCESS
* @par Expected Result:
* - Out paramter's value satisfied below
*@n -#cardInfo >= 0 and #cardInfo <= 2 (TAPI_SIM_CARD_TYPE_UNKNOWN, TAPI_SIM_CARD_TYPE_GSM, TAPI_SIM_CARD_TYPE_USIM)
*/
void utc_tel_get_sim_type_01()
{
	TET_STAMP_START("utc_tel_get_sim_type_01");
	TelSimCardType_t cardInfo;

    TET_STAMP_START("SIM Type check test");

    /* Calling TAPI LAGUAGE INDICATOR  API */
    TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_type(&cardInfo));

    tet_printf("The card type is %d\n", cardInfo);
    tet_printf("\t0x00 : UNKNOWN\n\t0x01 : 2GGSM\n\t0x02 : 3G\n");

    if (cardInfo >= 0 && cardInfo <= 2)
        TET_STAMP_PASS();
    else
        TET_STAMP_FAIL();
    return;
}

/**
* @brief tel_get_sim_type() API : Negative condition(First parameter is Null)
* @par ID: UTC_TEL_GET_SIM_TYPE_02
* @param [out] &cardInfo = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_type_02()
{
	TET_STAMP_START("utc_tel_get_sim_type_02");

    /* Calling TAPI LAGUAGE INDICATOR  API */
    TET_CHECK( TAPI_API_INVALID_PTR,tel_get_sim_type(NULL));
    TET_STAMP_PASS();
    return;
}
/** @} */

