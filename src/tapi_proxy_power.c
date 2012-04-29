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

#include "tel_cs_conn.h"
#include "TelDefines.h"
#include "TelPower.h"
#include "TapiUtility.h"
#include "TelErr.h"
#include "ITapiPower.h"
#include "tapi_proxy_internal.h"

/**
 *
 * Turn Off or Turn On the Modem.
 *
 * @param[in]		tapi_power_phone_cmd_t  Turn ON or OFF.
 * @param[out]	NONE
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks		None.
 * @see			tapi_power_phone_cmd_t, TapiResult_t.
 */
EXPORT_API int tel_process_power_command(tapi_power_phone_cmd_t cmd)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	if ((cmd < TAPI_PHONE_POWER_OFF) || (cmd > TAPI_PHONE_POWER_RESET)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input [%d]", cmd);

		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &cmd, sizeof(tapi_power_phone_cmd_t));

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Power Service Request: Power Off /On/Reset..[ %d ]", cmd);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_POWER, TAPI_CS_POWER_PHONE_POWER_ON_OFF, in_param1, in_param2,
			in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err	= g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/**
 *
 * Enter in to or Leave from Flight Mode.
 *
 * @param[in]	Enable (if 1, Enable and if 0, Disable)
 * @param[out]	NONE
 * @return		TapiResult_t	  API result code. Either Success or some Error Code.
 * @exception	In case of exceptions return value contains appropriate error code.
 * @remarks		None
 * @see			None.
 */
EXPORT_API int tel_set_flight_mode(tapi_power_flight_mode_type_t mode)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	/*	 check for the RPC link.... */
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	if ((mode < TAPI_POWER_FLIGHT_MODE_ENTER) || (mode >= TAPI_POWER_FLIGHT_MODE_MAX)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input [%d]", mode);

		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &mode, sizeof(tapi_power_flight_mode_type_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_POWER, TAPI_CS_POWER_FLIGHT_MODE, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err	= g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/**
 *
 * Reset the Modem.
 *
 * @param[in]		NONE
 * @param[out]	NONE
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks		None.
 * @see			TapiResult_t.
 */
EXPORT_API int tel_reset_modem(void)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_POWER, TAPI_CS_POWER_REBOOT, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err	= g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/**
 *
 * Enforce ramdump of phone.
 *
 * @param[in]	NONE
 * @param[out]	NONE
 * @return		TapiResult_t	  API result code. Either Success or some Error Code.
 * @exception	In case of exceptions return value contains appropriate error code.
 * @remarks		None
 * @see			None.
 */
EXPORT_API int tel_enforce_ramdump_of_modem(void)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Enterance.");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_POWER, TAPI_CS_POWER_RAMDUMP, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_COMMON_ERR_: Phone Service Down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/*	EOF	*/
