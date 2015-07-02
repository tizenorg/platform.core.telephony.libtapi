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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tapi_common.h"
#include "TapiUtility.h"

#include "TelMisc.h"
#include "TelPower.h"
#include "ITapiModem.h"

#include "common.h"
#include "tapi_log.h"

static void on_response_default_set(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_version(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelMiscVersionInformation data;
	char *sw = NULL, *hw = NULL, *rf = NULL, *product = NULL, *prl = NULL, *eri = NULL;

	GVariant *dbus_result;

	memset(&data, 0, sizeof(TelMiscVersionInformation));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);


	g_variant_get(dbus_result, "(issssss)", &result,
			&sw, &hw, &rf, &product, &prl, &eri);

	dbg("product code [%s]", product);

	g_strlcpy((gchar *)data.szSwVersion, sw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data.szHwVersion, hw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data.szRfCalDate, rf, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data.szProductCode, product, TAPI_MISC_PRODUCT_CODE_LEN_MAX);
	g_strlcpy((gchar *)data.szPrlVersion, prl, TAPI_MISC_PRL_ERI_VER_LEN_MAX);
	g_strlcpy((gchar *)data.szEriVersion, eri, TAPI_MISC_PRL_ERI_VER_LEN_MAX);

	g_free(sw);
	g_free(hw);
	g_free(rf);
	g_free(product);
	g_free(prl);
	g_free(eri);

	if (evt_cb_data->cb_fn) {
		if (result != 0)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
		else
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);
	}

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_serial_number(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelMiscSNInformation data;
	char *sn = NULL, *meid = NULL, *imei = NULL, *imeisv = NULL;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	g_variant_get(dbus_result, "(issss)", &result,
			&sn, &meid, &imei, &imeisv);

	g_strlcpy((gchar *)data.szEsn, sn, TAPI_MISC_ME_SN_LEN_MAX);
	g_strlcpy((gchar *)data.szMeid, meid, TAPI_MISC_ME_SN_LEN_MAX);
	g_strlcpy((gchar *)data.szImei, imei, TAPI_MISC_ME_SN_LEN_MAX);
	g_strlcpy((gchar *)data.szImeiSv, imeisv, TAPI_MISC_ME_SN_LEN_MAX);


	g_free(sn);
	g_free(meid);
	g_free(imei);
	g_free(imeisv);

	if (evt_cb_data->cb_fn) {
		if (result != 0)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
		else
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);
	}

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_imei(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	char *imei = NULL;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	g_variant_get(dbus_result, "(is)", &result, &imei);

	if (evt_cb_data->cb_fn) {
		if (result != 0)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
		else
			evt_cb_data->cb_fn(evt_cb_data->handle, result, imei, evt_cb_data->user_data);
	}
	g_free(imei);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_device_info(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelMiscDeviceInfo_t data;
	char *vendor_name = NULL;
	char *device_name = NULL;
	GVariant *dbus_result;

	memset(&data, 0, sizeof(TelMiscDeviceInfo_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	g_variant_get(dbus_result, "(iss)", &result,
			&vendor_name, &device_name);

	dbg("vendor_name [%s] device_name [%s] , result[%d]", vendor_name, device_name, result);

	if (!result) {
		g_strlcpy((gchar *)data.vendor_name, vendor_name, TAPI_MISC_ME_DEVICE_NAME_LEN_MAX+1);
		g_strlcpy((gchar *)data.device_name, device_name, TAPI_MISC_ME_DEVICE_NAME_LEN_MAX+1);

		g_free(vendor_name);
		g_free(device_name);
	}

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_flight_mode(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	gboolean mode = FALSE;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	g_variant_get(dbus_result, "(bi)", &mode, &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &mode, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

/**
 *
 * Turn Off or Turn On the Modem.
 *
 * @param[in]	tapi_power_phone_cmd_t  Turn ON or OFF.
 * @param[out]	NONE
 * @return		TapiResult_t API result code. Either Success or some Error Code.
 * @exception	In case of exceptions return value contains appropriate error code.
 * @remarks		None.
 * @see			tapi_power_phone_cmd_t, TapiResult_t.
 */
EXPORT_API int tel_process_power_command(TapiHandle *handle, tapi_power_phone_cmd_t cmd, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	msg("[%s] tel_process_power_command:[%d] (0:ON,1:OFF,2:RST,3:LOW)", handle->cp_name, cmd);

	if (cmd > TAPI_PHONE_POWER_MAX)
		return TAPI_API_INVALID_INPUT;
	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", cmd);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"SetPower", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * Enter in to or Leave from Flight Mode.
 *
 * @param[in]	Enable (if 1, Enable and if 0, Disable)
 * @param[out]	NONE
 * @return		TapiResult_t API result code. Either Success or some Error Code.
 * @exception	In case of exceptions return value contains appropriate error code.
 * @remarks		None
 * @see			None.
 */
EXPORT_API int tel_set_flight_mode(TapiHandle *handle, tapi_power_flight_mode_type_t mode, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gboolean b_mode;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	msg("[%s] requested flight mode:[%d] (1:enter, 2:leave)", handle->cp_name, mode);

	if (mode == TAPI_POWER_FLIGHT_MODE_ENTER)
		b_mode = TRUE;
	else if (mode == TAPI_POWER_FLIGHT_MODE_LEAVE)
		b_mode = FALSE;
	else
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(b)", b_mode);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"SetFlightMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, 180000, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_flight_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetFlightMode", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_flight_mode, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_misc_me_version(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetVersion", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_version, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API TelMiscVersionInformation *tel_get_misc_me_version_sync(TapiHandle *handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	int result = -1;
	TelMiscVersionInformation *data = NULL;
	char *sw = NULL, *hw = NULL, *rf = NULL, *product = NULL, *prl = NULL, *eri = NULL;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, NULL);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetVersion", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_SHORT_TIMEOUT, handle->ca, &error);

	if (error) {
		warn("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
	}

	if (!dbus_result)
		return NULL;

	g_variant_get(dbus_result, "(issssss)", &result,
			&sw, &hw, &rf, &product, &prl, &eri);
	g_variant_unref(dbus_result);

	if (result != 0)
		goto OUT;

	dbg("product code[%s]", product);

	data = calloc(1, sizeof(TelMiscVersionInformation));
	if (!data)
		goto OUT;

	g_strlcpy((gchar *)data->szSwVersion, sw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data->szHwVersion, hw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data->szRfCalDate, rf, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data->szProductCode, product, TAPI_MISC_PRODUCT_CODE_LEN_MAX);
	g_strlcpy((gchar *)data->szPrlVersion, prl, TAPI_MISC_PRL_ERI_VER_LEN_MAX);
	g_strlcpy((gchar *)data->szEriVersion, eri, TAPI_MISC_PRL_ERI_VER_LEN_MAX);

OUT:
	g_free(sw);
	g_free(hw);
	g_free(rf);
	g_free(product);
	g_free(prl);
	g_free(eri);

	return data;
}

EXPORT_API int tel_get_misc_me_imei(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	sdbg("[%s] Func Entrance", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetIMEI", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_imei, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API char *tel_get_misc_me_imei_sync(TapiHandle *handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	char *imei = NULL;
	int result = 0;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, NULL);

	sdbg("[%s] Func Entrance", handle->cp_name);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetIMEI", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_SHORT_TIMEOUT, handle->ca, &error);

	if (error) {
		warn("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
	}

	if (!dbus_result)
		return NULL;

	g_variant_get(dbus_result, "(is)", &result, &imei);
	g_variant_unref(dbus_result);

	if (result != 0) {
		g_free(imei);
		return NULL;
	}

	return imei;
}

EXPORT_API int tel_get_misc_me_sn(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	sdbg("[%s] Func Entrance", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetSerialNumber", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_serial_number, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API TelMiscSNInformation *tel_get_misc_me_sn_sync(TapiHandle *handle)
{
	GVariant *dbus_result;
	GError *error = NULL;
	char *sn = NULL, *meid = NULL, *imei = NULL, *imeisv = NULL;
	TelMiscSNInformation *data = NULL;
	int result = 0;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, NULL);

	sdbg("[%s] Func Entrance", handle->cp_name);

	dbus_result = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetSerialNumber", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_SHORT_TIMEOUT, handle->ca, &error);
	if (error) {
		warn("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
	}

	if (!dbus_result)
		return NULL;

	g_variant_get(dbus_result, "(issss)", &result, &sn, &meid, &imei, &imeisv);
	g_variant_unref(dbus_result);

	if (result != 0)
		goto OUT;

	data = calloc(1, sizeof(TelMiscSNInformation));
	if (!data)
		goto OUT;

	g_strlcpy((gchar *)data->szEsn, sn, TAPI_MISC_ME_SN_LEN_MAX);
	g_strlcpy((gchar *)data->szMeid, meid, TAPI_MISC_ME_SN_LEN_MAX);
	g_strlcpy((gchar *)data->szImei, imei, TAPI_MISC_ME_SN_LEN_MAX);
	g_strlcpy((gchar *)data->szImeiSv, imeisv, TAPI_MISC_ME_SN_LEN_MAX);

OUT:
	g_free(sn);
	g_free(meid);
	g_free(imei);
	g_free(imeisv);

	return data;
}

EXPORT_API int tel_check_modem_power_status(TapiHandle *handle, int *result)
{
	int ret = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(result, TAPI_API_INVALID_PTR);

	ret = tel_get_property_int(handle, TAPI_PROP_MODEM_POWER, result);
	if (ret == TAPI_API_ACCESS_DENIED) {
		err("Access denied");
		return ret;
	} else if (ret != TAPI_API_SUCCESS) {
		err("failed. (ret=%d)", ret);
		return ret;
	}

	msg("[%s] modem_power_status = %d (0=on,1=off,2=rst,3=low)", handle->cp_name, *result);

	return ret;
}
EXPORT_API int tel_get_device_info(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetDeviceInfo", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_device_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}
