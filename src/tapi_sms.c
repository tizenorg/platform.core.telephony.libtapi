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

#include "tapi_sms.h"

static gboolean __tapi_check_sim_sms_data(const TelSmsSimDataInfo *data)
{
	if (data == NULL)
		return FALSE;

	switch (data->status) {
	case TEL_SMS_STATUS_MT_READ:
	case TEL_SMS_STATUS_MT_UNREAD:
	case TEL_SMS_STATUS_MO_SENT:
	case TEL_SMS_STATUS_MO_NOT_SENT:
	case TEL_SMS_STATUS_MO_DELIVERED:
	case TEL_SMS_STATUS_MO_DELIVERY_NOT_CONFIRMED:
	case TEL_SMS_STATUS_REPLACED:
		break;
	default:
		return FALSE;
	}

	if (data->data.tpdu_length > TEL_SMS_SMDATA_SIZE_MAX)
		return FALSE;

	return TRUE;
}

void on_sms_property_change_handler(TelephonySms *sms,
	GVariant *changed_properties, const gchar *invalidated_properties,
	gpointer user_data)
{

	TapiEvtCbData *evt_cb_data = NULL;
	TelHandle *handle = user_data;
	GVariantIter *iter;
	const gchar *key;
	GVariant *value;

	if (handle == NULL)
		return;

	if (g_variant_n_children(changed_properties) == 0)
		return;

	dbg("Properties Changed:");

	g_variant_get(changed_properties, "a{sv}", &iter);
	while (g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		char *evt_id;

		dbg("%s", key);

		evt_id = g_strdup_printf("%s:%s", TELEPHONY_SMS_INTERFACE, key);

		evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);

		if (evt_cb_data) {
			if (!g_strcmp0("SimMemoryStatus", key)
				|| !g_strcmp0("InitStatus", key)) {
				gboolean status = g_variant_get_boolean(value);
				dbg("status[%d]", status);
				EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &status);
			}
		}
		g_free(evt_id);
	}
	g_variant_iter_free(iter);
}

void on_sms_signal_emit_handler(TelephonySms *sms, gchar *sender_name,
		gchar *signal_name, GVariant *parameters, gpointer user_data)
{
	TapiEvtCbData *evt_cb_data = NULL;
	TelHandle *handle = user_data;
	char *evt_id = NULL;

	dbg("Entry");

	if (handle == NULL)
		return;

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_SMS_INTERFACE,
					signal_name);

	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "IncomingMsg")) {
		TelSmsDatapackageInfo incoming_msg;
		GVariant *sca;
		char *sca_number = NULL;
		char *tpdu = NULL;
		guchar *decoded_tpdu = NULL;

		GVariantIter *iter = NULL;
		GVariant *key_value;
		const gchar *key;

		memset(&incoming_msg, 0, sizeof(TelSmsDatapackageInfo));

		g_variant_get(parameters, "(@a{sv}s)", &sca, &tpdu);
		g_variant_get(sca, "a{sv}", &iter);
		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "ton") == 0) {
				incoming_msg.sca.ton = g_variant_get_byte(key_value);
			}
			else if (g_strcmp0(key, "npi") == 0) {
				incoming_msg.sca.npi = g_variant_get_byte(key_value);
			}
			else if (g_strcmp0(key, "number") == 0) {
				g_strlcpy(incoming_msg.sca.number,
					g_variant_get_string(key_value, NULL),
					TEL_SMS_SCA_LEN_MAX + 1);
			}
		}
		g_variant_iter_free(iter);

		dbg("TON: [%d] NPI: [%d] SCA: [%s]",
			incoming_msg.sca.ton, incoming_msg.sca.npi,
			incoming_msg.sca.number);

		decoded_tpdu = g_base64_decode(tpdu, &incoming_msg.tpdu_length);
		if (!decoded_tpdu) {
			err("base64 decoding of tpdu failed");
			goto out;
		}
		else if (incoming_msg.tpdu_length > TEL_SMS_SMDATA_SIZE_MAX) {
			err("invalid tpdu length received: [%d]", incoming_msg.tpdu_length);
			g_free(decoded_tpdu);
			goto out;
		}

		memcpy((gchar*)incoming_msg.tpdu, (gchar*)decoded_tpdu,
				TEL_SMS_SMDATA_SIZE_MAX);

		dbg("Signal [%s] TPDU length: [%d]",
			signal_name, incoming_msg.tpdu_length);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &incoming_msg);

		g_free(decoded_tpdu);
out:
		g_variant_unref(sca);
		g_free(sca_number);
		g_free(tpdu);
	}
	else if (!g_strcmp0(signal_name, "IncomingCbMsg")) {
		TelSmsCbMsgInfo cb_msg;
		char *pdu = NULL;
		memset(&cb_msg, 0, sizeof(TelSmsCbMsgInfo));

		g_variant_get(parameters, "(is)", &cb_msg.cb_type, &pdu);

		cb_msg.cb_data = g_base64_decode(pdu, &cb_msg.length);
		if (cb_msg.cb_data) {
			dbg("signal[%s] cb_type[%d] cb_data[%s] cb_data_len[%d]",
				signal_name, cb_msg.cb_type, pdu, cb_msg.length);
		} else {
			dbg("signal[%s] cb_type[%d] cb_data_len[%d]",
				signal_name, cb_msg.cb_type, cb_msg.length);
		}

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &cb_msg);
		g_free(pdu);
		g_free(cb_msg.cb_data);
	}
	else if (!g_strcmp0(signal_name, "IncomingEtwsMsg")) {
		TelSmsEtwsMsgInfo etws_msg;
		char *pdu = NULL;
		memset(&etws_msg, 0, sizeof(TelSmsEtwsMsgInfo));

		g_variant_get(parameters, "(is)", &etws_msg.etws_type, &pdu);
		etws_msg.etws_data = g_base64_decode(pdu, &etws_msg.length);

		if (etws_msg.etws_data) {
			dbg("signal[%s] etws_type[%d]  etws_data[%s] etws_data_len[%d]",
				signal_name, etws_msg.etws_type, pdu, etws_msg.length);
		} else {
			dbg("signal[%s] etws_type[%d]  etws_data_len[%d]",
				signal_name, etws_msg.etws_type, etws_msg.length);
		}

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &etws_msg);

		g_free(pdu);
		g_free(etws_msg.etws_data);

	}
	else {
		err("Unsupported signal [%s]", signal_name);
	}

	g_free(evt_id);
}

static void on_response_sms_send(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_send_finish(handle->sms_proxy, (int *)&result,
			res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_send(TelHandle *handle,
			const TelSmsSendInfo *info,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariantBuilder sca_builder;
	GVariant *sca;
	char *encoded_tpdu;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && info != NULL &&
			info->send_data.tpdu_length <= TEL_SMS_SMDATA_SIZE_MAX &&
			callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	g_variant_builder_init(&sca_builder, G_VARIANT_TYPE("a{sv}"));

	g_variant_builder_add(&sca_builder, "{sv}",
		"ton", g_variant_new_byte(info->send_data.sca.ton));
	g_variant_builder_add(&sca_builder, "{sv}",
		"npi", g_variant_new_byte(info->send_data.sca.npi));
	g_variant_builder_add(&sca_builder, "{sv}",
		"number", g_variant_new_string(info->send_data.sca.number));

	sca = g_variant_builder_end(&sca_builder);

	encoded_tpdu = g_base64_encode(info->send_data.tpdu, info->send_data.tpdu_length);

	telephony_sms_call_send(handle->sms_proxy,
		info->more_msgs,
		sca, encoded_tpdu, NULL,
		on_response_sms_send, rsp_cb_data);

	g_free(encoded_tpdu);
	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_read_sms_in_sim(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSmsSimDataInfo data;
	TelSmsMsgStatus status;
	GVariant *sca = NULL;
	unsigned int pdu_len;
	char *encoded_pdu = NULL;
	guchar *pdu = NULL;

	GVariantIter *iter = NULL;
	GVariant *key_value;
	const gchar *key;

	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_read_in_sim_finish(handle->sms_proxy,
		(int *)&result, (int *)&status, &sca, &encoded_pdu, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SMS_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		goto out;
	}

	pdu = g_base64_decode(encoded_pdu, &pdu_len);
	if (pdu_len > TEL_SMS_SMDATA_SIZE_MAX) {
		err("PDU length is wrong");
		RESP_CALLBACK_CALL(rsp_cb_data, TEL_SMS_RESULT_FAILURE, NULL);
		goto out;
	}

	data.status = status;
	data.data.tpdu_length = pdu_len;
	memcpy(data.data.tpdu, pdu, pdu_len);

	g_variant_get(sca, "a{sv}", &iter);
	while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
		if (g_strcmp0(key, "ton") == 0) {
			data.data.sca.ton = g_variant_get_byte(key_value);
		}
		else if (g_strcmp0(key, "npi") == 0) {
			data.data.sca.npi = g_variant_get_byte(key_value);
		}
		else if (g_strcmp0(key, "number") == 0) {
			g_strlcpy(data.data.sca.number,
				g_variant_get_string(key_value, NULL),
				TEL_SMS_SCA_LEN_MAX + 1);
		}
	}
	g_variant_iter_free(iter);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &data);

out:
	g_variant_unref(sca);
	g_free(encoded_pdu);
	g_free(pdu);
}

EXPORT_API TelReturn tapi_sms_read_sms_in_sim(TelHandle *handle,
		unsigned int index, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL &&
			index < TEL_SMS_GSM_MSG_NUM_MAX &&
			callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_read_in_sim(handle->sms_proxy,
		index, NULL,
		on_response_sms_read_sms_in_sim, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_write_sms_in_sim(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	unsigned int index;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_write_in_sim_finish(handle->sms_proxy,
			(int *)&result, &index, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SMS_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &index);
	}
}

EXPORT_API TelReturn tapi_sms_write_sms_in_sim(TelHandle *handle,
	const TelSmsSimDataInfo *wdata,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariantBuilder sca_builder;
	GVariant *sca;
	char *encoded_tpdu;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL &&
			__tapi_check_sim_sms_data(wdata) && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	g_variant_builder_init(&sca_builder, G_VARIANT_TYPE("a{sv}"));

	g_variant_builder_add(&sca_builder, "{sv}",
		"ton", g_variant_new_byte(wdata->data.sca.ton));
	g_variant_builder_add(&sca_builder, "{sv}",
		"npi", g_variant_new_byte(wdata->data.sca.npi));
	g_variant_builder_add(&sca_builder, "{sv}",
		"number", g_variant_new_string(wdata->data.sca.number));

	sca = g_variant_builder_end(&sca_builder);

	encoded_tpdu = g_base64_encode(wdata->data.tpdu, wdata->data.tpdu_length);

	telephony_sms_call_write_in_sim(handle->sms_proxy,
		wdata->status, sca, encoded_tpdu,
		NULL,
		on_response_sms_write_sms_in_sim, rsp_cb_data);

	g_free(encoded_tpdu);
	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_delete_sms_in_sim(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_delete_in_sim_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_delete_sms_in_sim(TelHandle *handle,
	unsigned int index,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL &&
			callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_delete_in_sim(handle->sms_proxy, index, NULL,
			on_response_sms_delete_sms_in_sim, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_get_count(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	TelSmsStoredMsgCountInfo info;
	GError *error = NULL;

	GVariant *var;
	GVariantIter *iter;
	unsigned int index;
	gint i = 0;

	dbg("Entry");

	memset(&info, 0, sizeof(TelSmsStoredMsgCountInfo));

	telephony_sms_call_get_count_finish(handle->sms_proxy,
			(int *)&result, &info.total_count, &info.used_count,
			&var, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SMS_RESULT_SUCCESS) {
		g_variant_unref(var);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if (info.used_count == 0) {
		g_variant_unref(var);
		RESP_CALLBACK_CALL(rsp_cb_data, result, &info);
		return;
	}

	if (info.used_count > TEL_SMS_GSM_MSG_NUM_MAX) {
		err("Invalid result!");
		g_variant_unref(var);
		result = TEL_SMS_RESULT_FAILURE;
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	g_variant_get(var, "au", &iter);

	dbg("Fetching index ...");
	while (g_variant_iter_next(iter, "u", &index)) {
		dbg("index - %d", index);
		info.index_list[i] = index;
		i++;
	}

	RESP_CALLBACK_CALL(rsp_cb_data, result, &info);

	g_variant_iter_free(iter);
	g_variant_unref(var);
}

EXPORT_API TelReturn tapi_sms_get_count(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_get_count(handle->sms_proxy, NULL,
			on_response_sms_get_count, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_set_cb_config(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_set_cb_config_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_set_cb_config(TelHandle *handle,
	const TelSmsCbConfigInfo *cb_conf,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariant *mids;
	GVariantBuilder builder;
	unsigned int i;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL &&
			cb_conf != NULL &&
			cb_conf->msg_id_range_cnt < TEL_SMS_GSM_CBMI_LIST_SIZE_MAX,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	g_variant_builder_init(&builder, G_VARIANT_TYPE("aa{sv}"));
	for (i = 0; i < cb_conf->msg_id_range_cnt; i++) {
		g_variant_builder_open(&builder, G_VARIANT_TYPE("a{sv}"));

		g_variant_builder_add(&builder, "{sv}",
			"from_msg_id",
			g_variant_new_uint16(cb_conf->msg_ids[i].from_msg_id));
		g_variant_builder_add(&builder, "{sv}",
			"to_msg_id",
			g_variant_new_uint16(cb_conf->msg_ids[i].to_msg_id));
		g_variant_builder_add(&builder, "{sv}",
			"selected",
			g_variant_new_boolean(cb_conf->msg_ids[i].selected));

		g_variant_builder_close(&builder);
	}
	mids = g_variant_builder_end(&builder);

	telephony_sms_call_set_cb_config(handle->sms_proxy,
			cb_conf->cb_enabled, cb_conf->msg_id_range_cnt,
			mids, NULL,
			on_response_sms_set_cb_config, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_get_cb_config(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	TelSmsCbConfigInfo config;
	GError *error = NULL;

	GVariant *var;
	GVariantIter *iter = NULL, *iter_row = NULL;
	GVariant *key_value;
	const gchar *key;

	guint count = 0;

	dbg("Entry");

	memset(&config, 0, sizeof(TelSmsCbConfigInfo));

	telephony_sms_call_get_cb_config_finish(handle->sms_proxy,
			(int *)&result, &config.cb_enabled,
			&config.msg_id_range_cnt, &var,
			res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SMS_RESULT_SUCCESS) {
		g_variant_unref(var);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if (config.msg_id_range_cnt == 0) {
		g_variant_unref(var);
		RESP_CALLBACK_CALL(rsp_cb_data, result, &config);
		return;
	}

	if (g_variant_n_children(var) != config.msg_id_range_cnt
		|| config.msg_id_range_cnt > TEL_SMS_GSM_CBMI_LIST_SIZE_MAX) {
		err("Invalid result!");
		g_variant_unref(var);
		result = TEL_SMS_RESULT_FAILURE;
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	g_variant_get(var, "aa{sv}", &iter);
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "from_msg_id") == 0) {
				config.msg_ids[count].from_msg_id =
					g_variant_get_uint16(key_value);
			}
			else if (g_strcmp0(key, "to_msg_id") == 0) {
				config.msg_ids[count].to_msg_id =
					g_variant_get_uint16(key_value);
			}
			else if (g_strcmp0(key, "number") == 0) {
				config.msg_ids[count].selected =
					g_variant_get_boolean(key_value);
			}
		}
		count++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &config);

	g_variant_unref(var);
}

EXPORT_API TelReturn tapi_sms_get_cb_config(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_get_cb_config(handle->sms_proxy, NULL,
			on_response_sms_get_cb_config, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_get_parameters(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	TelSmsParamsInfoList param_list;
	GError *error = NULL;

	GVariant *var;
	GVariantIter *iter = NULL, *iter_row = NULL;
	GVariant *key_value;
	const gchar *key;

	guint count = 0;

	TelSmsParamsInfo *params;

	dbg("Entry");

	telephony_sms_call_get_parameters_finish(handle->sms_proxy,
		(int *)&result, &param_list.count, &var, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SMS_RESULT_SUCCESS) {
		g_variant_unref(var);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if (param_list.count == 0) {
		g_variant_unref(var);
		RESP_CALLBACK_CALL(rsp_cb_data, result, &param_list);
		return;
	}

	if (g_variant_n_children(var) != param_list.count) {
		err("Invalid result!");
		g_variant_unref(var);
		result = TEL_SMS_RESULT_FAILURE;
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	param_list.params =
		g_malloc0(sizeof(TelSmsParamsInfo) * param_list.count);

	params = param_list.params;

	g_variant_get(var, "aa{sv}", &iter);
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "index") == 0) {
				params[count].index = g_variant_get_uint32(key_value);
			}
			else if (g_strcmp0(key, "sca") == 0) {
				GVariantIter *iter2 = NULL;
				GVariant *key_value2;
				const gchar *key2;

				g_variant_get(key_value, "a{sv}", &iter2);
				while (g_variant_iter_loop(iter2, "{sv}", &key2, &key_value2)) {
					if (g_strcmp0(key, "ton") == 0) {
						params[count].sca.ton =
							g_variant_get_byte(key_value2);
					}
					else if (g_strcmp0(key2, "npi") == 0) {
						params[count].sca.npi =
							g_variant_get_byte(key_value2);
					}
					else if (g_strcmp0(key2, "number") == 0) {
						g_strlcpy(params[count].sca.number,
							g_variant_get_string(key_value2, NULL),
							TEL_SMS_SCA_LEN_MAX + 1);
					}
				}
				g_variant_iter_free(iter2);
			}
			else if (g_strcmp0(key, "vp") == 0) {
				params[count].vp = g_variant_get_boolean(key_value);
			}
		}
		count++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &param_list);

	g_variant_unref(var);
	g_free(param_list.params);
}

EXPORT_API TelReturn tapi_sms_get_parameters(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_get_parameters(handle->sms_proxy, NULL,
			on_response_sms_get_parameters, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_set_parameters(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_set_parameters_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_set_parameters(TelHandle *handle,
	const TelSmsParamsInfo *params,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariantBuilder sca_builder;
	GVariant *sca;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL &&
			params != NULL && params->index > 0,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	g_variant_builder_init(&sca_builder, G_VARIANT_TYPE("a{sv}"));

	g_variant_builder_add(&sca_builder, "{sv}",
		"ton", g_variant_new_byte(params->sca.ton));
	g_variant_builder_add(&sca_builder, "{sv}",
		"npi", g_variant_new_byte(params->sca.npi));
	g_variant_builder_add(&sca_builder, "{sv}",
		"number", g_variant_new_string(params->sca.number));

	sca = g_variant_builder_end(&sca_builder);

	telephony_sms_call_set_parameters(handle->sms_proxy, params->index,
			sca, params->vp, NULL,
			on_response_sms_set_parameters, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_send_deliver_report(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_send_deliver_report_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_send_deliver_report(TelHandle *handle,
	const TelSmsDeliverReportInfo *dr_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariantBuilder sca_builder;
	GVariant *sca;
	char *encoded_tpdu;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && dr_info != NULL &&
			dr_info->report <= TEL_SMS_DELIVERY_REPORT_UNSPECIFIED_ERROR &&
			dr_info->data.tpdu_length <= TEL_SMS_SMDATA_SIZE_MAX &&
			callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	g_variant_builder_init(&sca_builder, G_VARIANT_TYPE("a{sv}"));

	g_variant_builder_add(&sca_builder, "{sv}",
		"ton", g_variant_new_byte(dr_info->data.sca.ton));
	g_variant_builder_add(&sca_builder, "{sv}",
		"npi", g_variant_new_byte(dr_info->data.sca.npi));
	g_variant_builder_add(&sca_builder, "{sv}",
		"number", g_variant_new_string(dr_info->data.sca.number));

	sca = g_variant_builder_end(&sca_builder);

	encoded_tpdu = g_base64_encode(dr_info->data.tpdu, dr_info->data.tpdu_length);
	telephony_sms_call_send_deliver_report(handle->sms_proxy,
			dr_info->report, sca, encoded_tpdu, NULL,
			on_response_sms_send_deliver_report, rsp_cb_data);

	g_free(encoded_tpdu);
	g_variant_unref(sca);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_set_sca(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_set_sca_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_set_sca(TelHandle *handle, TelSmsSca *sca,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariantBuilder sca_builder;
	GVariant *var_sca;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && sca != NULL &&
			callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	g_variant_builder_init(&sca_builder, G_VARIANT_TYPE("a{sv}"));

	g_variant_builder_add(&sca_builder, "{sv}",
		"ton", g_variant_new_byte(sca->ton));
	g_variant_builder_add(&sca_builder, "{sv}",
		"npi", g_variant_new_byte(sca->npi));
	g_variant_builder_add(&sca_builder, "{sv}",
		"number", g_variant_new_string(sca->number));

	var_sca = g_variant_builder_end(&sca_builder);

	telephony_sms_call_set_sca(handle->sms_proxy, var_sca, NULL,
			on_response_sms_set_sca, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_get_sca(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSmsSca sca;
	GVariant *var_sca;

	GVariantIter *iter = NULL;
	GVariant *key_value;
	const gchar *key;

	GError *error = NULL;
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;

	dbg("Entry");

	telephony_sms_call_get_sca_finish(handle->sms_proxy,
			(int *)&result, &var_sca, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SMS_RESULT_SUCCESS) {
		g_variant_unref(var_sca);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	g_variant_get(var_sca, "a{sv}", &iter);
	while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
		if (g_strcmp0(key, "ton") == 0) {
			sca.ton = g_variant_get_byte(key_value);
		}
		else if (g_strcmp0(key, "npi") == 0) {
			sca.npi = g_variant_get_byte(key_value);
		}
		else if (g_strcmp0(key, "number") == 0) {
			g_strlcpy(sca.number,
				g_variant_get_string(key_value, NULL),
				TEL_SMS_SCA_LEN_MAX + 1);
		}
	}
	g_variant_iter_free(iter);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &sca);

	g_variant_unref(var_sca);
}

EXPORT_API TelReturn tapi_sms_get_sca(TelHandle *handle,
			TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_get_sca(handle->sms_proxy, NULL,
			on_response_sms_get_sca, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_set_memory_status(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_set_memory_status_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_set_memory_status(TelHandle *handle,
			gboolean available, TapiResponseCb callback,
			void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_set_memory_status(handle->sms_proxy, available,
			NULL, on_response_sms_set_memory_status, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sms_set_message_status(GObject *source_object,
			GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSmsResult result = TEL_SMS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_sms_call_set_message_status_finish(handle->sms_proxy,
			(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_sms_set_message_status(TelHandle *handle,
			TelSmsStatusInfo *info, TapiResponseCb callback,
			void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL &&
			info != NULL && info->index > 0 &&
			info->status <= TEL_SMS_STATUS_REPLACED,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sms_call_set_message_status(handle->sms_proxy, info->index,
			info->status, NULL, on_response_sms_set_message_status,
			rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_sms_get_init_status(TelHandle *handle,
			gboolean *init_status)
{
	g_return_val_if_fail(handle != NULL && init_status != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	*init_status = telephony_sms_get_init_status(handle->sms_proxy);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_sms_get_sim_memory_status(TelHandle *handle,
			gboolean *memory_full)
{
	g_return_val_if_fail(handle != NULL && memory_full != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	*memory_full = telephony_sms_get_sim_memory_status(handle->sms_proxy);

	return TEL_RETURN_SUCCESS;
}
