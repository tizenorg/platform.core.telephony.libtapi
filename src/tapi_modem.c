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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tapi_common.h"
#include "TapiUtility.h"

#include "TelMisc.h"
#include "TelPower.h"
#include "ITapiModem.h"
#include "ITapiPS.h"

#include "common.h"
#include "tapi_log.h"

static void on_response_default_set(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_version(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelMiscVersionInformation data;
	char *sw = NULL, *hw = NULL, *rf = NULL, *product = NULL;

	GVariant *dbus_result;

	memset(&data, 0, sizeof(TelMiscVersionInformation));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(issss)", &result,
			&sw, &hw, &rf, &product);

	dbg("product code [%s]", product);

	g_strlcpy((gchar *)data.szSwVersion, sw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data.szHwVersion, hw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data.szRfCalDate, rf, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data.szProductCode, product, TAPI_MISC_PRODUCT_CODE_LEN_MAX);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_serial_number(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	char *sn = NULL;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(is)", &result, &sn);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, sn, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_imei(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	char *imei = NULL;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(is)", &result, &imei);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, imei, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_set_dun_pin_ctrl(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(i)", &result);
	dbg("dun pin ctrl result(%d)", result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, 0, evt_cb_data->user_data);
	}

	free(evt_cb_data);
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

	dbg("Func Entrance");

	if (cmd > TAPI_PHONE_POWER_RESET)
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", cmd);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"SetPower", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
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

	dbg("Func Entrance");

	if (mode == TAPI_POWER_FLIGHT_MODE_ENTER) {
		b_mode = TRUE;
	}
	else if (mode == TAPI_POWER_FLIGHT_MODE_LEAVE) {
		b_mode = FALSE;
	}
	else {
		return TAPI_API_INVALID_INPUT;
	};

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(b)", b_mode);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"SetFlightMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_misc_me_version(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetVersion", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_version, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API TelMiscVersionInformation *tel_get_misc_me_version_sync(TapiHandle *handle)
{
	GVariant *dbus_result;
	int result = -1;
	TelMiscVersionInformation *data = NULL;
	char *sw = NULL, *hw = NULL, *rf = NULL, *product = NULL;

	dbg("Func Entrance");

	if (!handle)
		return NULL;

	dbus_result = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetVersion", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL);

	g_variant_get (dbus_result, "(issss)", &result,
			&sw, &hw, &rf, &product);

	dbg("product code[%s]", product);

	data = calloc(sizeof(TelMiscVersionInformation), 1);
	if (!data)
		return NULL;

	g_strlcpy((gchar *)data->szSwVersion, sw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data->szHwVersion, hw, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data->szRfCalDate, rf, MAX_VERSION_LEN);
	g_strlcpy((gchar *)data->szProductCode, product, TAPI_MISC_PRODUCT_CODE_LEN_MAX);

	return data;
}

EXPORT_API int tel_get_misc_me_imei(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetIMEI", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_imei, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API char *tel_get_misc_me_imei_sync(TapiHandle *handle)
{
	GVariant *dbus_result;
	char *imei = NULL;
	int result = 0;

	dbg("Func Entrance");

	if (!handle)
		return NULL;

	dbus_result = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetIMEI", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL);

	g_variant_get (dbus_result, "(is)", &result, &imei);

	return imei;
}

EXPORT_API int tel_get_misc_me_sn(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetSerialNumber", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_serial_number, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API char *tel_get_misc_me_sn_sync(TapiHandle *handle)
{
	GVariant *dbus_result;
	char *sn = NULL;
	int result = 0;

	dbg("Func Entrance");

	if (!handle)
		return NULL;

	dbus_result = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"GetSerialNumber", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL);

	g_variant_get (dbus_result, "(is)", &result, &sn);

	return sn;
}

EXPORT_API int tel_check_modem_power_status(TapiHandle *handle, int *result)
{
	return tel_get_property_int(handle, TAPI_PROP_MODEM_POWER, result);
}

EXPORT_API int tel_control_modem_dun_pin_ctrl(TapiHandle *handle, tapi_ps_btdun_pincontrol *pincontrol, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection || !pincontrol){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ib)", pincontrol->signal, pincontrol->status);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_MODEM_INTERFACE,
			"SetDunPinCtrl", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_set_dun_pin_ctrl, evt_cb_data);

	return TAPI_API_SUCCESS;
}
