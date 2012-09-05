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
#include "TelSim.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiSap.h"

static void on_response_sap_connect(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapConnectionStatus_t result = TAPI_SIM_SAP_CONNECTION_STATUS_OK;
	gint max_msg_size = 0;

	dbg("Func Entrance");

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(ii)", &result, &max_msg_size);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, (void*)&max_msg_size, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_connect(TapiHandle *handle, int max_msg_size, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	dbg("Func Entrance w/ max_msg_size[%d]",max_msg_size);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", max_msg_size);
	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "Connect", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_connect, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_disconnect(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapResultCode_t result = TAPI_SIM_SAP_RESULT_CODE_OK;

	dbg("Func Entrance");

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_disconnect(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

		g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "Disconnect", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_disconnect, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_connection_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapStatusInfo_t result = TAPI_SIM_SAP_STATUS_UNKNOWN;

	dbg("Func Entrance");

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_connection_status(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "GetStatus", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_connection_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_transfer_atr(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar rt_i;
	int i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapResultCode_t result = TAPI_SIM_SAP_RESULT_CODE_OK;
	TelSapAtrInfo_t r_atr;

	dbg("Func Entrance");
	memset(&r_atr, 0, sizeof(TelSapAtrInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	if (dbus_result) {
		/*	dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));*/
		g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
		inner_gv = g_variant_get_variant(param_gv);

		g_variant_get(inner_gv, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			r_atr.atr_data[i] = rt_i;
			i++;
		}
		r_atr.atr_len = (int) i;
		g_variant_iter_free(iter);
		g_variant_unref(inner_gv);
		g_variant_unref(param_gv);
		/*		for(i=0; i < (int)r_atr.atr_len; i++)
		 dbg("r_atr[%d][0x%02x]",i, r_atr.atr_data[i]);*/
	} else {
		result = TAPI_SIM_SAP_RESULT_CODE_DATA_NOT_AVAILABLE;
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, (void*)&r_atr, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_transfer_atr(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "GetATR", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_transfer_atr, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_transfer_apdu(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar rt_i;
	int i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapResultCode_t result = TAPI_SIM_SAP_RESULT_CODE_OK;
	TelSapApduData_t r_apdu;

	dbg("Func Entrance");
	memset(&r_apdu, 0, sizeof(TelSapApduData_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	if (dbus_result) {
		/*	dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));*/
		g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
		inner_gv = g_variant_get_variant(param_gv);

		g_variant_get(inner_gv, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			r_apdu.apdu_data[i] = rt_i;
			i++;
		}
		r_apdu.apdu_len = (int) i;
		g_variant_iter_free(iter);
		g_variant_unref(inner_gv);
		g_variant_unref(param_gv);
		/*		for(i=0; i < (int)r_apdu.apdu_len; i++)
		 dbg("apdu[%d][0x%02x]",i, r_apdu.apdu_data[i]);*/
	}
	else {
		result = TAPI_SIM_SAP_RESULT_CODE_DATA_NOT_AVAILABLE;
		dbg( "g_dbus_conn failed. error (%s)", error->message);
		g_error_free(error);
	}
	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, (void*)&r_apdu, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_transfer_apdu(TapiHandle *handle, TelSapApduData_t *apdu_data, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariantBuilder *builder = NULL;
	GVariant *param = NULL;
	GVariant *inner_gv = NULL;
	int i = 0;

	dbg("Func Entrance");
	TAPI_RETURN_VAL_IF_FAIL(apdu_data, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
	for (i = 0; i < apdu_data->apdu_len; i++) {
		dbg("apdu_data->apdu_data[%d][0x%02x]", i,apdu_data->apdu_data[i]);
		g_variant_builder_add(builder, "y", apdu_data->apdu_data[i]);
	}
	inner_gv = g_variant_builder_end(builder);
	param = g_variant_new("(v)", inner_gv);
	/*g_variant_builder_unref (builder);*/

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "TransferAPDU", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_transfer_apdu, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_transport_protocol(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapResultCode_t result = TAPI_SIM_SAP_RESULT_CODE_OK;

	dbg("Func Entrance");

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_transport_protocol(TapiHandle *handle, TelSimSapProtocol_t protocol, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	dbg("Func Entrance w/ protocol[%d]", protocol);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", protocol);
	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "SetProtocol", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_transport_protocol, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_power_operation(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapResultCode_t result = TAPI_SIM_SAP_RESULT_CODE_OK;

	dbg("Func Entrance");

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_power_operation(TapiHandle *handle, TelSimSapPowerMode_t power_mode, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gint mode = 0;
	dbg("Func Entrance w/ power_mode[%d]", power_mode);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	switch (power_mode) {
		case TAPI_SIM_SAP_POWER_SIM_ON_REQ:
			mode = 0;
			break;
		case TAPI_SIM_SAP_POWER_SIM_OFF_REQ:
			mode =1;
			break;
		case TAPI_SIM_SAP_RESET_SIM_REQ:
			mode = 2;
			break;
		default:
			dbg("not handed value[%d]", power_mode);
			break;
	}

	param = g_variant_new("(i)", mode);
	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "SetPower", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_power_operation, evt_cb_data);

	return TAPI_API_SUCCESS;
}

static void on_response_sap_cardreader_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimSapResultCode_t result = TAPI_SIM_SAP_RESULT_CODE_OK;
	TelSimSapCardStatus_t reader_status = TAPI_SIM_SAP_CARD_STATUS_UNKNOWN;

	dbg("Func Entrance");

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(ii)", &result, &reader_status);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, (void*)&reader_status, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_req_sap_cardreader_status(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAP_INTERFACE, "GetCardReaderStatus", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_sap_cardreader_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}
