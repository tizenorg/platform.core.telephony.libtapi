/*
 * libslp-tapi
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd. All rights reserved.
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

#include "common.h"
#include "tapi_log.h"
#include "ITapiGps.h"

static void on_response_default_set(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
		}

		if (error)
			g_error_free(error);

		free(evt_cb_data);
		return;
	}


	g_variant_get (dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}


EXPORT_API int tel_set_gps_frequency_aiding(TapiHandle *handle, unsigned char state, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(y)", state);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_GPS_INTERFACE,
			"SetFrequencyAiding", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_confirm_gps_measure_pos(TapiHandle *handle, unsigned char *data, unsigned int data_len)
{
	GVariant *param = NULL;
	GVariant *rst = NULL;
	GError *gerr = 0;
	gchar *encoded_data = NULL;
	gint result;

	dbg("Func Entrance");

	if (!handle || !handle->dbus_connection || !data){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	msg("call tel_confirm_gps_measure_pos(). data_len=%d", data_len);
	encoded_data = g_base64_encode((const guchar*)data, data_len);

	param = g_variant_new("(s)", encoded_data);

	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE , handle->path,
			DBUS_TELEPHONY_GPS_INTERFACE, "ConfirmMeasurePos", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1, 0, &gerr);

	g_free(encoded_data);

	if(!rst){
		dbg( "error to gps measure pos confirm(%s)", gerr->message);
		g_error_free (gerr);
		return TAPI_API_OPERATION_FAILED;
	}
	dbg("send gps user confirm format(%s)", g_variant_get_type_string(rst));

	g_variant_get(rst, "(i)", &result);
	dbg("result (%d)", result);

	return TAPI_API_SUCCESS;
}

