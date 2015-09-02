/*
 * libslp-tapi
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
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
 * @file ITapiModem.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_MODEM
 * @{
 */

#ifndef _ITAPI_MODEM_H_
#define _ITAPI_MODEM_H_

#include <tapi_common.h>
#include <TelPower.h>
#include <TelMisc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Turns the modem on/off.
 * @details <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] cmd The index value, which is the power command #tapi_power_phone_cmd_t
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it provides an error code (Refer #TapiResult_t).
 * @post callback will be invoked and data is NULL.
 */
int tel_process_power_command(TapiHandle *handle, tapi_power_phone_cmd_t cmd, tapi_response_cb callback, void *user_data);

/**
 * @brief Switches the flight mode on/off.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] mode The mode value, which is the flight flag #tapi_power_flight_mode_type_t
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification

 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully, \n
 *         else it provides an error code (Refer #TapiResult_t).
 * @post callback will be invoked and #tapi_power_flight_mode_resp_type_t will be stored in result and data is NULL.
 */
int tel_set_flight_mode(TapiHandle *handle, tapi_power_flight_mode_type_t mode, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the flight mode
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @post callback will be invoked and 1(ON) or 0(OFF) will be stored in data on success case.
 */
int tel_get_flight_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets ME version information.
 *
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.\n
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation is completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 * @post callback will be invoked and #TelMiscVersionInformation will be stored in data on success case.
 */
int tel_get_misc_me_version(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the Misc ME version.
 *
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.\n
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @return The return type ( TelMiscVersionInformation * ) \n
 *         The Misc Version Information.
 */
TelMiscVersionInformation *tel_get_misc_me_version_sync(TapiHandle *handle);

/**
 * @brief Gets the ME ESN/MEID for each phone type.
 *
 * @details <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation is completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 * @post callback will be invoked and #TelMiscSNInformation will be stored in data on success case.
 */
int tel_get_misc_me_sn(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the Misc ME Serial Number.
 *
 * @details <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> Embedded call application. \n
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @return The return Type (char*) \n
 *         The Serial Number string.
 */
TelMiscSNInformation *tel_get_misc_me_sn_sync(TapiHandle *handle);

/**
 * @brief Gets the Misc ME IMEI.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation is completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 * @post callback will be invoked and IMEI value(char *) will be stored in data on success case.
 */
int tel_get_misc_me_imei(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the Misc ME IMEI.
 *
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 *          <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @return The return type (char*) \n
 *         The IMEI string.
 */
char *tel_get_misc_me_imei_sync(TapiHandle *handle);

/**
 * @brief Checks the modem power status.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @param[in] handle The handle from tel_init()
 * @param[out] result The result of checking the modem power status
 *
 * @return The return type (int) \n
 *         @c 0 indicates that the operation is completed successfully, \n
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t).
 * @post #tapi_power_phone_power_status_t will be stored in result.
 */
int tel_check_modem_power_status(TapiHandle *handle, int *result);

/**
 * @brief Gets device vendor name and device name of cellular dongle.
 *
 * @details This function provides vendor and device name of cellular dongle.
 *          Result can be delivered with only cellular dongle insertion.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation is completed successfully. \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 * @post callback will be invoked and #TelMiscDeviceInfo_t will be stored in data on success case.
 */
int tel_get_device_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_MODEM_H_ */

/**
 * @}
 */
