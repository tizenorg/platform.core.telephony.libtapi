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


#include <vconf.h>

#include <TapiUtility.h>
#include <TelErr.h>
#include <ITapiMisc.h>

#include "tel_cs_conn.h"
#include "tapi_proxy_internal.h"


static int _tel_check_tapi_state()
{
	int vconf_ret = 0;
	int tapi_state = 0;

	vconf_ret = vconf_get_int(VCONFKEY_TELEPHONY_TAPI_STATE, (int *) &tapi_state);

	if (vconf_ret != 0 || tapi_state != 1) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "FAIL TO GET VCONFKEY_TELEPHONY_TAPI_STATE or SERVICE NOT READY");
		return TAPI_API_SERVICE_NOT_READY;
	}

	return 0;
}

TS_BOOL __tel_check_misc_valid(tapi_misc_time_zone_info_type *zone_info)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (0x00 > zone_info->time_zone_valid || zone_info->time_zone_valid > 0x04)
		return FALSE;

	if (1 > zone_info->month || zone_info->month > 12)
		return FALSE;
	if (1 > zone_info->day || zone_info->day > 31)
		return FALSE;
	if (0 > (int) zone_info->hour || (int) zone_info->hour > 23)
		return FALSE;
	if ((int) zone_info->minute > 59)
		return FALSE;
	if ((int) zone_info->second > 59)
		return FALSE;

	//no need check  (TS_UINT8)zone_info->time_zone

	if (0x00 > zone_info->daylight_adjust || zone_info->daylight_adjust > 0x02)
		return FALSE;
	if (0x00 > zone_info->day_of_week || zone_info->day_of_week > 0x06)
		return FALSE;

	return TRUE;
}

EXPORT_API int tel_get_misc_timezone_mode(int* bTimezone_valid)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_TIMEZONE_GETMODE,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		*bTimezone_valid = g_array_index(out_param3,int ,0);
		api_err = TAPI_API_SUCCESS;
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_get_misc_timezone_info(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_TIMEZONE_GETINFO,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = TAPI_API_SUCCESS;
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_set_misc_timezone_info(tapi_misc_time_zone_info_type *timezone_info)
{
	TS_BOOL bValid = FALSE;
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(timezone_info, TAPI_API_MISC_INPUTPARM_ERROR);

	//check validity of variable.
	bValid = __tel_check_misc_valid(timezone_info);
	if (bValid == FALSE) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "TAPI_API_MISC_VALIDITY_ERROR");
		return TAPI_API_MISC_VALIDITY_ERROR;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, timezone_info, sizeof(tapi_misc_time_zone_info_type));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_TIMEZONE_SETINFO,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = TAPI_API_SUCCESS;
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_confirm_misc_timezone_Info(tapi_misc_time_zone_info_type *timezone_info)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(timezone_info, TAPI_API_MISC_INPUTPARM_ERROR);
	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, timezone_info, sizeof(tapi_misc_time_zone_info_type));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_TIMEZONE_CONFIRMINFO,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = TAPI_API_SUCCESS;
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_get_misc_me_version(TelMiscVersionInformation* ver_info)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_ME_VERSION_GET,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);

		if (api_err == TAPI_API_SUCCESS) {
			*ver_info = g_array_index(out_param2, TelMiscVersionInformation, 0);

			TAPI_LIB_DEBUG(LEVEL_DEBUG, "ver_mask[0x%02x]", ver_info->ver_mask);
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "szHwVersion[%s]", ver_info->szHwVersion);
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "szSwVersion[%s]", ver_info->szSwVersion);
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "szRfCalDate[%s]", ver_info->szRfCalDate);
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "szProductCode[%s]", ver_info->szProductCode);
		}
		else {
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "api_err[%d]", api_err);
		}
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "tapi_send_request_internal fail");
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_get_misc_me_sn(TelMiscSNIndexType_t sn_index, TelMiscSNInformation* sn_info)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &sn_index, sizeof(TelMiscSNIndexType_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_SERIAL_NUMBER_GET,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);

		if (api_err == TAPI_API_SUCCESS) {
			*sn_info = g_array_index(out_param2, TelMiscSNInformation, 0);

			TAPI_LIB_DEBUG(LEVEL_DEBUG, "sn_index[%d]", sn_info->sn_index);
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "sn_len[%d]", sn_info->sn_len);
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "szNumber[%s]", sn_info->szNumber);
		}
		else {
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "api_err[%d]", api_err);
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_get_misc_nam_info(unsigned short nam_info_mask, TelMiscNamIndex_t nam_index)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &nam_info_mask, sizeof(unsigned short));
	g_array_append_vals(in_param2, &nam_index, sizeof(TelMiscNamIndex_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_NAM_INFO_GET,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = TAPI_API_SUCCESS;
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_set_misc_nam_info(TelMiscNamInfo_t *nam_info)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, nam_info, sizeof(TelMiscNamInfo_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_MISC, TAPI_CS_MISC_NAM_INFO_SET,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = TAPI_API_SUCCESS;
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_set_misc_emergency_mode(int mode) {
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance with request mode[%d]", mode);

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	if (mode != 0 && mode != 1)
		return TAPI_API_INVALID_INPUT;

	if (vconf_set_int("db/telephony/emergency", mode) != 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL]UPDATE db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_misc_emergency_mode(int *mode) {
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	if (vconf_get_int("db/telephony/emergency", mode) != 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL]GET db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "emergency mode[%d]", *mode);
	return TAPI_API_SUCCESS;
}
