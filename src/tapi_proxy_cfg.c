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

#include <stdio.h>
#include <dlog.h>

#include <TelDefines.h>

#include <ITapiCfg.h>
#include <TapiUtility.h>
#include <TelErr.h>

#include "tel_cs_conn.h"
#include "tapi_proxy_internal.h"


/******************************************************************************
 ******************************************************************************
 *
 *						P R O X Y   C A L  L   A P I's
 *
 ******************************************************************************
 ******************************************************************************/

/**
 * Set default configuration to modem.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_cfg_default_configuration(TelDefaultCfg_t bType)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	if ((TAPI_CFG_DEFAULT_FULL > bType) || (TAPI_CFG_DEFAULT_CUSTOM < bType)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -Default Configuration");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &bType, sizeof(TelDefaultCfg_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_DEFAULTCONFGURATION_EXE,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

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
 * get A-Key.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_akey(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/* check for the RPC link.... */
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_A_KEY,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * verify A-Key.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_verify_cfg_akey(const TelCfgAKeyInfo_t *pAKey_info)
{
	TS_BOOL ret = FALSE;
	int app_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "A-key:[%s]", pAKey_info->A_key);

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, pAKey_info, sizeof(TelCfgAKeyInfo_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_VERIFY_A_KEY,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		app_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		app_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", app_err);

	return app_err;
}

/**
 * get MSL_info.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_msl_info(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_MSL_INFO,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get vocoder option.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_vocoder_option(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_VOCODER_OPTION,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * set vocoder option.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_cfg_vocoder_option(TelCfgVocOption_t option)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if ((TAPI_CFG_VOC_OPTION_DEFAULT <= option) && (option <= TAPI_CFG_VOC_OPTION_RESERVED)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

		TAPI_GLIB_INIT_PARAMS();
		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &option, sizeof(TelCfgVocOption_t));

		TAPI_LIB_DEBUG(LEVEL_INFO, "option:[%d]", option);

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_SET_VOCODER_OPTION,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err	= g_array_index(out_param1, int, 0);
		}
		/*	RPC API failed, return FALSE to APP	*/
		else {
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, " Invalid Input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get hidden menu access.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_hidden_menu_access(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_HIDDEN_MENU_ACCESS,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * set hidden menu access.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_cfg_hidden_menu_access(TelCfgHiddenMenuAccessType_t type)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if ((TAPI_CFG_HIDDEN_MENU_DISABLE <= type) && (type <= TAPI_CFG_HIDDEN_MENU_ENABLE)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

		TAPI_GLIB_INIT_PARAMS();
		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &type, sizeof(TelCfgHiddenMenuAccessType_t));

		TAPI_LIB_DEBUG(LEVEL_INFO, "type:[%d]", type);

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_SET_HIDDEN_MENU_ACCESS,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err	= g_array_index(out_param1, int, 0);
		}
		/*	RPC API failed, return FALSE to APP	*/
		else {
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get current carrier.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_serving_carrier(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_CURRENT_CARRIER,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * set current carrier.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_cfg_serving_carrier(TelCfgCurrentCarrier_t Carrier)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if ((TAPI_CFG_CARRIER_TEST <= Carrier) && (Carrier <= TAPI_CFG_CARRIER_MTS)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

		TAPI_GLIB_INIT_PARAMS();
		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &Carrier, sizeof(TelCfgCurrentCarrier_t));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Carrier:[%d]", Carrier);

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_SET_CURRENT_CARRIER,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err	= g_array_index(out_param1, int, 0);
		}
		/*	RPC API failed, return FALSE to APP	*/
		else {
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, " Invalid Input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get sio mode.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_sio_mode(void) {
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_SIO_MODE,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * set sio mode.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_cfg_sio_mode(TelCfgSIOMode_t Mode)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if ((TAPI_CFG_SIO_U1_DIAG_USB2_GPS <= Mode) && (Mode <= TAPI_CFG_SIO_U1_HFK_USB2_DIAG)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

		TAPI_GLIB_INIT_PARAMS();
		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &Mode, sizeof(TelCfgSIOMode_t));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Mode:[%d]", Mode);

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_SET_SIO_MODE,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err	= g_array_index(out_param1, int, 0);
		}
		/*	RPC API failed, return FALSE to APP	*/
		else {
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, " Invalid Input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get activation date.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_activated_date_of_phone(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_ACTIVATION_DATE,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get reconditioned status and date.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_reconditioned_data_of_phone(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_RECONDITIONED_DATE,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * get tty mode. CDMA only
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_get_cfg_tty_mode(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_GET_TTY_MODE,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err	= g_array_index(out_param1, int, 0);
	}
	/*	RPC API failed, return FALSE to APP	*/
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**
 * Set tty mode. CDMA only
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_cfg_tty_mode(TelCfgTTYMode_t Mode)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if ((Mode >= TAPI_CFG_TTY_MODE_DISABLE) && (Mode <= TAPI_CFG_TTY_MODE_ENABLE_FULL)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

		TAPI_GLIB_INIT_PARAMS();
		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &Mode, sizeof(TelCfgTTYMode_t));

		TAPI_LIB_DEBUG(LEVEL_INFO, "TTY Mode:[%d]", Mode);

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_CFG, TAPI_CS_CFG_SET_TTY_MODE,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err	= g_array_index(out_param1, int, 0);
		}
		/*	RPC API failed, return FALSE to APP	*/
		else {
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, " Invalid Input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

//eof
