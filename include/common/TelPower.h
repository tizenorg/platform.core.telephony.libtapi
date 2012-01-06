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
 * @internal
 * @ingroup		TelephonyAPI
 * @addtogroup 	POWER_TAPI	POWER
 * @{
 *
 * These APIs allow an application to accomplish the following services: @n
 * - Get current phone power status,  @n
 * - Reset the phone power (on / off), @n
 * - Get the Display Icon Information. @n
 */

#ifndef _TEL_POWER_H_
#define _TEL_POWER_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/

#include <TelDefines.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 MACROS
 ==================================================================================================*/

/*==================================================================================================
 ENUMS
 ==================================================================================================*/

/**
 * This defines the phone power reset commands.
 */
typedef enum {
	TAPI_PHONE_POWER_OFF = 0, TAPI_PHONE_POWER_ON, TAPI_PHONE_POWER_RESET,
} tapi_power_phone_cmd_t;

/**
 * This defines the phone power status values.
 */
typedef enum {
	TAPI_PHONE_POWER_STATUS_OFF = 0, TAPI_PHONE_POWER_STATUS_ON
} tapi_power_phone_power_status_t;

/**
 * Use to Enter or Leave Flight Mode.
 *
 */
typedef enum {
	TAPI_POWER_FLIGHT_MODE_ENTER = 0x01, /**< ONLINE OFF */
	TAPI_POWER_FLIGHT_MODE_LEAVE, /**< ONLINE ON */
	TAPI_POWER_FLIGHT_MODE_MAX
} tapi_power_flight_mode_type_t;

typedef enum {
	TAPI_POWER_FLIGHT_MODE_RESP_ON = 0x01, /**< Flight Mode On Success */
	TAPI_POWER_FLIGHT_MODE_RESP_OFF, /**< Flight Mode Off Success */
	TAPI_POWER_FLIGHT_MODE_RESP_FAIL, /**< Flight Mode Request Fail */
	TAPI_POWER_FLIGHT_MODE_RESP_MAX
} tapi_power_flight_mode_resp_type_t;

/**
 * This enum defines the phone battery status levels.
 *
 * 0x01 : Power Off Level                     => PhoneLevel = power off
 * 0x02 : Critical-Low Battery Level          => PhoneLevel = 0
 * 0x03 : Low Battery Level                   => PhoneLevel = 1
 * 0x04 : Normal Level                        => PhoneLevel = 2,3,4
 */
typedef enum {
	TAPI_POWER_BATT_STAT_POWER_OFF_LEVEL = 0x01, /**< Power Off Level */
	TAPI_POWER_BATT_STAT_CRIT_LOW_LEVEL = 0x02, /**< Critical-Low Battery Level */
	TAPI_POWER_BATT_STAT_LOW_LEVEL = 0x03, /**< Low Battery Level */
	TAPI_POWER_BATT_STAT_NORMAL_LEVEL = 0x04 /**< Normal Level */
} tapi_power_battery_status_level_t;

/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif	/* _TEL_POWER_H_ */

/**
 * @}
 */

