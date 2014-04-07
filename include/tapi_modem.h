/*
 * libtapi
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd. All rights reserved.
 * Copyright (c) 2013 Intel Corporation. All rights reserved.
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

#ifndef __TAPI_MODEM_H__
#define __TAPI_MODEM_H__

/**
*  @addtogroup TAPI_MODEM
*  @{
*
*  @file tapi_modem.h
*  @brief TAPI Modem Interface
*/

#include "tapi.h"
#include <tel_modem.h>
#include <tel_return.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief Changes modem power status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] status Power status to set
 *  @param[in] callback Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelModemResult values:\n
 *  <b>TEL_MODEM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_MODEM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_MODEM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_modem_set_power_status(TelHandle *handle, TelModemPowerStatus status,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Toggles flight mode status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] enable <b>TRUE</b>: Turn Flight mode ON\n <b>FALSE</b>: Turn Flight mode OFF
 *  @param[in] callback Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelModemResult values:\n
 *  <b>TEL_MODEM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_MODEM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_MODEM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_modem_set_flight_mode(TelHandle *handle, gboolean enable,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Fetches flight mode status
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] enable <b>TRUE</b>: Flight mode is ON\n <b>FALSE</b>: Flight mode is OFF
 */
TelReturn tapi_modem_get_flight_mode(TelHandle *handle, gboolean *enable);

/**
 *  @brief Fetches modem version information
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] version Version Information (\ref TelModemVersion)
 */
TelReturn tapi_modem_get_version(TelHandle *handle, TelModemVersion *version);

/**
 *  @brief Fetches device IMEI number
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] imei NULL terminated device IMEI number
 */
TelReturn tapi_modem_get_imei(TelHandle *handle, char imei[TEL_MODEM_IMEI_LENGTH_MAX + 1]);

/**
 *  @brief Fetches current modem status
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] status Modem status (\ref TelModemPowerStatus)
 */
TelReturn tapi_modem_get_power_status(TelHandle *handle, TelModemPowerStatus *status);

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* __TAPI_MODEM_H__ */
