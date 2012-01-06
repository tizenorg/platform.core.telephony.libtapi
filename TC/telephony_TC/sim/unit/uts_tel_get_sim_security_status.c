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
* @addtogroup UTS_TAPI_SIM_GET_PIN_STATUS tel_get_sim_security_status
*
* @brief Unit test code for tel_get_sim_security_status()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_security_status(TelSimPinStatus_t *pPinStatus, TelSimPinType_t* pPinType);
* @par Test function description:
* 	This function gets the current PIN (PIN1 or UPIN) status.
* @par Important Notes:
* 	None
* @param [out] pPinStatus
* 	This parameter returns the current Pin status.
* @code
typedef enum
{
	TAPI_SIM_PIN_STATUS_DISABLED		= 0x00,
	TAPI_SIM_PIN_STATUS_ENABLED		= 0x01,
	TAPI_SIM_PIN_STATUS_BLOCKED		= 0x02,
	TAPI_SIM_PIN_STATUS_PUK_BLOCKED		= 0x03,
	TAPI_SIM_PIN_STATUS_UNKNOWN		= 0xFF
} TelSimPinStatus_t;
* @endcode
* @param [out] pPinType
* 	This parameter returns the current Pin (Pin1 or upin) type.
* @code
typedef enum
{
	TAPI_SIM_PTYPE_PIN1		= 0x00,
	TAPI_SIM_PTYPE_PIN2		= 0x01,
	TAPI_SIM_PTYPE_PUK1		= 0x02,
	TAPI_SIM_PTYPE_PUK2		= 0x03,
	TAPI_SIM_PTYPE_UPIN		= 0x04,
	TAPI_SIM_PTYPE_ADM		= 0x05
} TelSimPinType_t;
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
* @file		uts_tel_get_sim_security_status.c
* @brief	Test for the tel_get_sim_security_status() API that is used to get pin1 status
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_get_sim_security_status.h"
#include "sim_common.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_security_status_01, 1},
	{utc_tel_get_sim_security_status_02, 2},
	{utc_tel_get_sim_security_status_03, 3},
	{NULL, 0}
};


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static void startup()
{
	tet_infoline("utc_tel_get_sim_security_status Test Start\n");
	if (tel_init() != TAPI_API_SUCCESS)
	{
		tet_infoline("startup fail. tel_init() failed");
	}
	return;
}


static void cleanup()
{
	tet_infoline("utc_tel_get_sim_security_status Test Finished\n");
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
* @brief tel_get_sim_security_status() API : Positive condition(Get PIN1 status)
* @par ID: UTC_TEL_SET_SIM_SECURITY_STATUS_01
* @param [out] pPinStatus
* @param [in] pPinType
* @return TAPI_API_SUCCESS
* @par Expected Result:
*	pPinStatus is one of TelSimPinStatus_t type value and pPinType is one of pPinType value
*/
void utc_tel_get_sim_security_status_01()
{
	TET_STAMP_START("Normal operation - get pin1 status");
	TelSimPinStatus_t pin1_status = TAPI_SIM_PIN_STATUS_UNKNOWN;
	TelSimPinType_t pPinType = TAPI_SIM_PTYPE_PIN1;

	TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_security_status(pPinType, &pin1_status));
	tet_printf ("PIN1 status is %d\n", pin1_status);
	if ( (pin1_status>=TAPI_SIM_PIN_STATUS_DISABLED && pin1_status<=TAPI_SIM_PIN_STATUS_PUK_BLOCKED)||(pin1_status==TAPI_SIM_PIN_STATUS_UNKNOWN))
	{
		if ( pPinType >= TAPI_SIM_PTYPE_PIN1 && pPinType <=TAPI_SIM_PTYPE_ADM )
		{
			TET_STAMP_PASS();
			return;
		}
	}
	TET_STAMP_FAIL();
	return;
}


/**
* @brief tel_get_sim_security_status() API : Positive condition(Get PIN2 status)
* @par ID: UTC_TEL_SET_SIM_SECURITY_STATUS_02
* @param [out] pPinStatus
* @param [in] pPinType
* @return TAPI_API_SUCCESS
* @par Expected Result:
*	pPinStatus is one of TelSimPinStatus_t type value and pPinType is one of pPinType value
*/
void utc_tel_get_sim_security_status_02()
{
	TET_STAMP_START("Normal operation - get pin2 status");
	TelSimPinStatus_t pin2_status = TAPI_SIM_PIN_STATUS_UNKNOWN;
        TelSimPinType_t pPinType = TAPI_SIM_PTYPE_PIN2;

	TET_CHECK(TAPI_API_SUCCESS, tel_get_sim_security_status(pPinType, &pin2_status));
	tet_printf ("PIN1 status is %d\n", pin2_status);
	if ( (pin2_status>=TAPI_SIM_PIN_STATUS_DISABLED && pin2_status<=TAPI_SIM_PIN_STATUS_PUK_BLOCKED)||(pin2_status==TAPI_SIM_PIN_STATUS_UNKNOWN))
	{
		if ( pPinType >= TAPI_SIM_PTYPE_PIN1 && pPinType <=TAPI_SIM_PTYPE_ADM )
		{
			TET_STAMP_PASS();
			return;
		}
	}
	TET_STAMP_FAIL();
	return;
}


/**
* @brief tel_get_sim_security_status() API : Negative condition(Second parameter is NULL)
* @par ID: UTC_TEL_SET_SIM_SECURITY_STATUS_03
* @param [out] pPinStatus = NULL
* @param [in] pPinType
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_security_status_03()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR");
    TelSimPinType_t pPinType = TAPI_SIM_PTYPE_PIN1;

    TET_CHECK(TAPI_API_INVALID_PTR,tel_get_sim_security_status(pPinType, NULL));
    TET_STAMP_PASS();
}

/**
* @brief tel_get_sim_security_status() API : Negative condition(first parameter is invalid)
* @par ID: UTC_TEL_SET_SIM_SECURITY_STATUS_04
* @param [out] pPinStatus
* @param [in] pPinType = NULL
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_get_sim_security_status_04()
{
    TET_STAMP_START("TAPI_API_INVALID_PTR 2");
    TelSimPinStatus_t pin1_status = TAPI_SIM_PIN_STATUS_UNKNOWN;
    TelSimPinType_t pPinType = TAPI_SIM_PTYPE_SIM+1;

    TET_CHECK(TAPI_API_INVALID_INPUT,tel_get_sim_security_status(pPinType, &pin1_status));
    TET_STAMP_PASS();
}


/** @} */
