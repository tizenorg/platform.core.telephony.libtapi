/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Ja-young Gu <jygu@samsung.com>
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
 * @addtogroup	MODEM_TAPI	POWER
 * @{
 *
 * These APIs allow an application to accomplish the following services: @n
 * - Process power command @n
 * - Reset the phone power (on / off), @n
 * - Process airplane mode @n
 */

#ifndef _ITAPI_MODEM_H_
#define _ITAPI_MODEM_H_

#include <tapi_common.h>
#include <TelDefines.h>
#include <TelPower.h>
#include <TelMisc.h>
#include "ITapiPS.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @brief  This API is used to turn power off or on the modem.
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [in] cmd
 * - This index value is the power command as #tapi_power_phone_cmd_t.
 *
 * @param [in] callback
 *   - To register callback function for result of this function.
 *
 * @param [in] user_data
 *   - user data for user specification
 *
 * @par Async Response Message:
 * - None
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 */
int tel_process_power_command(TapiHandle *handle, tapi_power_phone_cmd_t cmd, tapi_response_cb callback, void *user_data);

/**
 *
 * @brief  This API is used to enter or leave airplane mode
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [in] mode
 * - This mode value is the flight flag #tapi_power_flight_mode_type_t.
 *
 * @param [in] callback
 *   - To register callback function for result of this function.
 *
 * @param [in] user_data
 *   - user data for user specification
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_MODEM_FLIGHT_MODE_RESP  and the event data is #tapi_power_flight_mode_resp_type_t.
 * Asynchronous return status is always TAPI_API_SUCCESS.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 */
int tel_set_flight_mode(TapiHandle *handle, tapi_power_flight_mode_type_t mode, tapi_response_cb callback, void *user_data);

int tel_get_flight_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 *
 * @brief  This function is invoked to Get ME version information.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [in] callback
 *   - To register callback function for result of this function.
 *
 * @param [in] user_data
 *   - user data for user specification
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_misc_me_version(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 *
 * @brief This function is to get misc me version
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @par Async Response Message:
 * - Na
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type ( TelMiscVersionInformation * ) \n
 * - Misc Version Information
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */


TelMiscVersionInformation *tel_get_misc_me_version_sync(TapiHandle *handle);


/**
 *
 * @brief  This function is invoked to Get ME IMEI/ESN/MEID for each phone type.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [in] callback
 *   - To register callback function for result of this function.
 *
 * @param [in] user_data
 *   - user data for user specification
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_misc_me_sn(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 *
 * @brief This function is to get misc me Sereal Number
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (TapiHandle *handle, or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *  - None.
 *
 * @warning
 * 
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (char*) \n
 * - Sereal Number string
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */

char *tel_get_misc_me_sn_sync(TapiHandle *handle);

/**
 *
 * @brief This function is to get misc me imei
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [in] callback
 *   - To register callback function for result of this function.
 *
 * @param [in] user_data
 *   - user data for user specification
 *
 * @par Async Response Message:
 * - IMEI
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */

int tel_get_misc_me_imei(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 *
 * @brief This function is to get misc me imei
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (TapiHandle *handle, or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *  - None.
 *
 * @warning
 * 
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (char*) \n
 * - IMEI string
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */

char *tel_get_misc_me_imei_sync(TapiHandle *handle);

/**
 *
 * @brief This function is to check modem power status
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (TapiHandle *handle, or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *  - None.
 *
 * @warning
 *
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [out] result
 * - result of checking modem power status.
 *
 * @par Async Response Message:
 * - 
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (TapiHandle *handle, int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (TapiHandle *handle, Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */

int tel_check_modem_power_status(TapiHandle *handle, int *result);

/**
 *
 * @brief This function is to control modem dun pin
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (TapiHandle *handle, or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *  - None.
 *
 * @warning
 *
 *
 * @param [in] handle
 * - handle from tel_init()
 *
 * @param [in] pincontrol
 * - pin control inforamation
 *
 * @param [in] callback
 * - To register callback function for result of this function.
 *
 * @param [in] user_data
 * - user data for user specification
 *
 * @par Async Response Message:
 * - None.
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (TapiHandle *handle, int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (TapiHandle *handle, Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */

int tel_control_modem_dun_pin_ctrl(TapiHandle *handle, tapi_ps_btdun_pincontrol *pincontrol, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_MODEM_H_ */

/**
 * @}
 */
