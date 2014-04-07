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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tapi_log.h"
#include "tapi.h"
#include "tapi_private.h"

#include "tapi_gps.h"

void on_gps_signal_emit_handler(TelephonyGps *gps, gchar *sender_name,
							gchar *signal_name, GVariant *parameters,
							gpointer user_data)
{
	TapiEvtCbData *evt_cb_data = NULL;
	TelHandle *handle = user_data;
	char *evt_id = NULL;

	dbg("Entry");

	if (handle == NULL)
		return;

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_GPS_INTERFACE,
					signal_name);

	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "AssistData")
		|| !g_strcmp0(signal_name, "MeasurePosition")) {
		TelGpsDataInfo gps_data = {0, };
		char *data = NULL;

		g_variant_get(parameters, "(s)", &data);

		/* Decode data */
		gps_data.data = g_base64_decode(data, &gps_data.data_len);
		dbg("signal[%s] data_len[%s]", signal_name, gps_data.data_len);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &gps_data);

		g_free(data);
		g_free(gps_data.data);
	} else if (!g_strcmp0(signal_name, "ResetAssistData")) {
		dbg("signal[%s]", signal_name);
		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, NULL);
	} else {
		err("Unhandled signal [%s]", signal_name);
	}

	g_free(evt_id);
}

EXPORT_API TelReturn tapi_gps_confirm_measure_pos(TelHandle *handle,
			TelGpsDataInfo *gps_data)
{
	TelGpsResult result;
	GError *error = NULL;
	gchar *encoded_data = NULL;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && gps_data != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	encoded_data = g_base64_encode((const guchar *)gps_data->data, gps_data->data_len);

	telephony_gps_call_confirm_measure_pos_sync(handle->gps_proxy, encoded_data,
			(int *)&result, NULL, &error);

	g_free(encoded_data);

	if (error != NULL) {
		err("dbus error: %d (%s)", error->code, error->message);
		g_error_free(error);
		return TEL_RETURN_FAILURE;
	}

	if (result != TEL_GPS_RESULT_SUCCESS) {
		err("confirm gps measure pos fail: %d", result);
		return TEL_RETURN_FAILURE;
	}

	return TEL_RETURN_SUCCESS;
}

static void on_response_gps_set_frequency_aiding(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelGpsResult result = TEL_GPS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_gps_call_set_frequency_aiding_finish(handle->gps_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_gps_set_frequency_aiding(TelHandle *handle,
			gboolean state,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_gps_call_set_frequency_aiding(handle->gps_proxy, state, NULL,
			on_response_gps_set_frequency_aiding, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}
