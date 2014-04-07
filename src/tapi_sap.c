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

#include "tel_sap.h"
#include "tapi_sap.h"

void on_sap_signal_emit_handler(TelephonySap *sap, gchar *sender_name,
							gchar *signal_name, GVariant *parameters,
							gpointer user_data)
{
	TapiEvtCbData *evt_cb_data = NULL;
	TelHandle *handle = user_data;
	char *evt_id = NULL;

	dbg("Entry");

	if (handle == NULL) {
		err("handle is null");
		return;
	}

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_SAP_INTERFACE,
					signal_name);

	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		warn("evt_cb_data is null for the signal[%s]", signal_name);
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "Status")) {
		TelSapCardStatus status;

		g_variant_get(parameters, "(i)", &status);
		dbg("signal[%s] status[%d]", signal_name, status);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &status);
	} else {
		err("Unsupported signal %s", signal_name);
	}
	g_free(evt_id);
}

static void on_response_sap_req_connect(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	unsigned int max_msg_size;
	GError *error = NULL;

	dbg("Entry");

	telephony_sap_call_req_connect_finish(handle->sap_proxy,
			(int *)&result, &max_msg_size, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &max_msg_size);
}

EXPORT_API TelReturn tapi_sap_req_connect(TelHandle *handle,
			unsigned int max_msg_size,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sap_call_req_connect(handle->sap_proxy, max_msg_size, NULL,
			on_response_sap_req_connect, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sap_req_disconnect(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	GError *error = NULL;

	dbg("Entry");

	telephony_sap_call_req_disconnect_finish(handle->sap_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sap_req_disconnect(TelHandle *handle,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sap_call_req_disconnect(handle->sap_proxy, NULL,
			on_response_sap_req_disconnect, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sap_get_atr(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	TelSapAtr atr;
	char *content = NULL;
	GError *error = NULL;

	guchar *binary_content;

	dbg("Entry");

	telephony_sap_call_get_atr_finish(handle->sap_proxy,
			(int *)&result, &content, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SAP_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		g_free(content);
		return;
	}

	binary_content = g_base64_decode(content, &atr.atr_len);

	if (atr.atr_len > TEL_SAP_ATR_LEN_MAX) {
		err("ATR length is too long");
		RESP_CALLBACK_CALL(rsp_cb_data,
				TEL_SAP_RESULT_FAILURE_NO_REASON, NULL);
		g_free(content);
		g_free(binary_content);
		return;
	}

	memcpy(atr.atr, binary_content, atr.atr_len);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &atr);
	g_free(content);
	g_free(binary_content);
}

EXPORT_API TelReturn tapi_sap_get_atr(TelHandle *handle,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sap_call_get_atr(handle->sap_proxy, NULL,
			on_response_sap_get_atr, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sap_req_transfer_apdu(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	TelSapApduResp rsp;
	char *pdu = NULL;
	GError *error = NULL;

	guchar *binary_pdu;

	dbg("Entry");

	telephony_sap_call_req_transfer_apdu_finish(handle->sap_proxy,
			(int *)&result, &pdu, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SAP_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		g_free(pdu);
		return;
	}

	binary_pdu = g_base64_decode(pdu, &rsp.apdu_resp_len);

	if (rsp.apdu_resp_len > TEL_SAP_APDU_RESP_LEN_MAX) {
		err("Req Transfer APDU response pdu length is too long");
		RESP_CALLBACK_CALL(rsp_cb_data,
				TEL_SAP_RESULT_FAILURE_NO_REASON, NULL);
		g_free(pdu);
		g_free(binary_pdu);
		return;
	}

	memcpy(rsp.apdu_resp, binary_pdu, rsp.apdu_resp_len);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &rsp);
	g_free(pdu);
	g_free(binary_pdu);
}

EXPORT_API TelReturn tapi_sap_req_transfer_apdu(TelHandle *handle,
			TelSapApdu *apdu, TapiResponseCb callback,
			void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	char *encoded_apdu;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && apdu != NULL &&
			apdu->apdu_len < TEL_SAP_APDU_LEN_MAX &&
			callback != NULL, TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	encoded_apdu = g_base64_encode(apdu->apdu, apdu->apdu_len);
	telephony_sap_call_req_transfer_apdu(handle->sap_proxy,
			encoded_apdu, NULL,
			on_response_sap_req_transfer_apdu, rsp_cb_data);

	g_free(encoded_apdu);
	return TEL_RETURN_SUCCESS;
}

static void on_response_sap_req_transport_protocol(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	GError *error = NULL;

	dbg("Entry");

	telephony_sap_call_req_transport_protocol_finish(handle->sap_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sap_req_transport_protocol(TelHandle *handle,
			TelSimSapProtocol protocol,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL &&
			protocol <= TEL_SIM_SAP_PROTOCOL_T1 && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sap_call_req_transport_protocol(handle->sap_proxy, protocol,
			NULL, on_response_sap_req_transport_protocol, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sap_req_power_operation(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	GError *error = NULL;

	dbg("Entry");

	telephony_sap_call_req_power_operation_finish(handle->sap_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sap_req_power_operation(TelHandle *handle,
			TelSapPowerMode mode,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && mode <= TEL_SAP_SIM_RESET_REQ &&
			callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sap_call_req_power_operation(handle->sap_proxy, mode,
			NULL, on_response_sap_req_power_operation, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sap_get_card_reader_status(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSapResult result = TEL_SAP_RESULT_FAILURE_NO_REASON;
	int status;
	GError *error = NULL;

	dbg("Entry");

	telephony_sap_call_get_card_reader_status_finish(handle->sap_proxy,
			(int *)&result, (int *)&status, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &status);
}

EXPORT_API TelReturn tapi_sap_get_cardreader_status(TelHandle *handle,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sap_call_get_card_reader_status(handle->sap_proxy, NULL,
			on_response_sap_get_card_reader_status, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}
