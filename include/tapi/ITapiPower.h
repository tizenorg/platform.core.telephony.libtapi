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
 * @addtogroup	POWER_TAPI	POWER
 * @{
 *
 * These APIs allow an application to accomplish the following services: @n
 * - Process power command @n
 * - Reset the phone power (on / off), @n
 * - Process airplane mode @n
 */

#ifndef _ITAPI_POWER_H_
#define _ITAPI_POWER_H_

#include <TelDefines.h>
#include <TelUtility.h>
#include <TelPower.h>

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
 * @param [in] cmd
 * - This index value is the power command as #tapi_power_phone_cmd_t.
 *
 * @param [out]
 *   - None
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
int tel_process_power_command(tapi_power_phone_cmd_t cmd);

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
 * @param [in] mode
 * - This mode value is the flight flag #tapi_power_flight_mode_type_t.
 *
 * @param [out]
 *   - None
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_POWER_FLIGHT_MODE_RESP  and the event data is #tapi_power_flight_mode_resp_type_t.
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
int tel_set_flight_mode(tapi_power_flight_mode_type_t mode);

/**
 *
 * @brief  This API is used to reset modem
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
 * @param [in]
 * - None
 *
 * @param [out]
 *   - None
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
int tel_reset_modem(void);

/**
 *
 * @brief  This API is used to get the ramdump file of modem. Modem's ramdump file is created under /opt/media/ramdump/
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
 * @param [in]
 * - None
 *
 * @param [out]
 *   - None
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
int tel_enforce_ramdump_of_modem(void);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_POWER_H_ */

/**
 * @}
 */
