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
#include "TelSms.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiNetText.h"

static void on_response_default(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result = NULL;
	int data = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_default type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_read_msg(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	TelSmsData_t readMsg = {0,};
	GVariant *dbus_result = NULL;

	GVariant *sca = 0, *tpdu = 0;
	int i = 0;
	GVariantIter *iter = 0;
	GVariant *inner_gv = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_read_msg type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(iiii@vi@v)", &result,
			&readMsg.SimIndex,
			&readMsg.MsgStatus,
			&readMsg.SmsData.format,
			&sca,
			&readMsg.SmsData.MsgLength,
			&tpdu);

	inner_gv = g_variant_get_variant(sca);
	g_variant_get(inner_gv, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &readMsg.SmsData.Sca[i])) {
		i++;
		if (i >= TAPI_SIM_SMSP_ADDRESS_LEN)
			break;
	}

	g_variant_iter_free(iter);

	inner_gv = g_variant_get_variant(tpdu);
	g_variant_get(inner_gv, "ay", &iter);
	i = 0;
	while (g_variant_iter_loop(iter, "y", &readMsg.SmsData.szData[i])) {
		i++;
		if (i >= TAPI_NETTEXT_SMDATA_SIZE_MAX + 1)
			break;
	}
	g_variant_iter_free(iter);
	g_variant_unref(sca);
	g_variant_unref(tpdu);
	g_variant_unref(inner_gv);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &readMsg, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_write_msg(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int sms_index = 0;

	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_write_msg type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(ii)", &result, &sms_index);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &sms_index, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_delete_msg(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int del_index = -1;

	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_delete_msg type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(ii)", &result, &del_index);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &del_index, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_get_msg_count(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSmsStoredMsgCountInfo_t storedMsgCnt = {0,};

	GVariant *dbus_result = NULL;
	int i = 0;
	int idx = 0;
	GVariantIter *iter = NULL;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_get_msg_count type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(iiiai)", &result,
		&storedMsgCnt.TotalCount,
		&storedMsgCnt.UsedCount,
		&iter);

	i = 0;
	while (g_variant_iter_loop(iter, "i", &idx)) {
		storedMsgCnt.IndexList[i] = idx;
		i++;
		if (i >= TAPI_NETTEXT_SMS_MSG_NUM_MAX)
			break;
	}

	g_variant_iter_free(iter);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &storedMsgCnt, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_get_sca(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSmsAddressInfo_t scaInfo = {0,};

	GVariant *dbus_result = NULL;

	GVariant *num = 0;
	int i = 0;
	GVariantIter *iter = 0;
	GVariant *inner_gv = 0;


	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_get_sca type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(iiii@v)", &result,
		&scaInfo.Ton,
		&scaInfo.Npi,
		&scaInfo.DialNumLen,
		&num);

	inner_gv = g_variant_get_variant(num);
	g_variant_get(inner_gv, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &scaInfo.szDiallingNum[i])) {
		i++;
		if (i >= TAPI_SIM_SMSP_ADDRESS_LEN + 1)
			break;
	}
	g_variant_iter_free(iter);
	g_variant_unref(num);
	g_variant_unref(inner_gv);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &scaInfo, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_get_cb_config(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int i;
	TelSmsCbConfig_t cbConfig = {0,};

	GVariant *dbus_result = NULL;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_get_cb_config type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(iiiiiaa{sv})", &result,
			&cbConfig.Net3gppType,
			&cbConfig.CBEnabled,
			&cbConfig.MsgIdMaxCount,
			&cbConfig.MsgIdRangeCount,
			&iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "FromMsgId"))
				cbConfig.MsgIDs[i].Net3gpp.FromMsgId = g_variant_get_uint16(value);
			else if (!g_strcmp0(key, "ToMsgId"))
				cbConfig.MsgIDs[i].Net3gpp.ToMsgId = g_variant_get_uint16(value);
			else if (!g_strcmp0(key, "CBCategory"))
				cbConfig.MsgIDs[i].Net3gpp2.CBCategory = g_variant_get_uint16(value);
			else if (!g_strcmp0(key, "CBLanguage"))
				cbConfig.MsgIDs[i].Net3gpp2.CBLanguage = g_variant_get_uint16(value);
			else if (!g_strcmp0(key, "Selected"))
				cbConfig.MsgIDs[i].Net3gpp.Selected = g_variant_get_byte(value);
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &cbConfig, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_get_sms_params(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSmsParams_t paramInfo = {0,};

	GVariant *dbus_result = NULL;

	GVariant *alphaId = 0, *destDialNum = 0, *scaDialNum = 0;
	int i = 0;
	GVariantIter *iter = 0;
	GVariant *inner_gv = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_get_sms_params type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(iiii@viiii@viii@viii)", &result,
								&paramInfo.RecordIndex,
								&paramInfo.RecordLen,
								&paramInfo.AlphaIdLen,
								&alphaId,
								&paramInfo.ParamIndicator,
								&paramInfo.TpDestAddr.DialNumLen,
								&paramInfo.TpDestAddr.Ton,
								&paramInfo.TpDestAddr.Npi,
								&destDialNum,
								&paramInfo.TpSvcCntrAddr.DialNumLen,
								&paramInfo.TpSvcCntrAddr.Ton,
								&paramInfo.TpSvcCntrAddr.Npi,
								&scaDialNum,
								&paramInfo.TpProtocolId,
								&paramInfo.TpDataCodingScheme,
								&paramInfo.TpValidityPeriod);

	inner_gv = g_variant_get_variant(alphaId);
	g_variant_get(inner_gv, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &paramInfo.szAlphaId[i])) {
		i++;
		if (i >= TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1)
			break;
	}

	g_variant_iter_free(iter);

	inner_gv = g_variant_get_variant(destDialNum);
	g_variant_get(inner_gv, "ay", &iter);
	i = 0;
	while (g_variant_iter_loop(iter, "y", &paramInfo.TpDestAddr.szDiallingNum[i])) {
		i++;
		if (i >= TAPI_SIM_SMSP_ADDRESS_LEN + 1)
			break;
	}

	g_variant_iter_free(iter);

	inner_gv = g_variant_get_variant(scaDialNum);
	g_variant_get(inner_gv, "ay", &iter);
	i = 0;
	while (g_variant_iter_loop(iter, "y", &paramInfo.TpSvcCntrAddr.szDiallingNum[i])) {
		i++;
		if (i >= TAPI_SIM_SMSP_ADDRESS_LEN + 1)
			break;
	}

	g_variant_iter_free(iter);
	g_variant_unref(alphaId);
	g_variant_unref(destDialNum);
	g_variant_unref(scaDialNum);
	g_variant_unref(inner_gv);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &paramInfo, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

static void on_response_get_sms_param_cnt(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int recordCount = 0;

	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_ERROR(error);

	dbg("on_response_get_sms_param_cnt type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get(dbus_result, "(ii)", &result,
			&recordCount);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &recordCount, evt_cb_data->user_data);

		g_free(evt_cb_data);
	}
	g_variant_unref(dbus_result);
}

EXPORT_API int tel_send_sms(struct tapi_handle *handle,
		const TelSmsDatapackageInfo_t *pDataPackage,
		int bMoreMsgToSend,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	GVariant *sca = 0, *packet_sca = 0;
	GVariant *tpdu = 0, *packet_tpdu = 0;
	GVariantBuilder b;
	int i;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pDataPackage, TAPI_API_INVALID_PTR);

	if (pDataPackage->Sca[0] > TAPI_SIM_SMSP_ADDRESS_LEN) {
		err("Invalid SCA length:[%d]", pDataPackage->Sca[0]);
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_SIM_SMSP_ADDRESS_LEN; i++)
		g_variant_builder_add(&b, "y", pDataPackage->Sca[i]);
	sca = g_variant_builder_end(&b);
	packet_sca = g_variant_new("v", sca);

	g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_NETTEXT_SMDATA_SIZE_MAX + 1; i++)
		g_variant_builder_add(&b, "y", pDataPackage->szData[i]);
	tpdu = g_variant_builder_end(&b);
	packet_tpdu = g_variant_new("v", tpdu);

	param = g_variant_new("(i@vi@vi)", pDataPackage->format, packet_sca,
		pDataPackage->MsgLength, packet_tpdu, bMoreMsgToSend);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SendMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, 120000, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_read_sms_in_sim(struct tapi_handle *handle, int read_index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	if ((read_index < 0) || (read_index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Input -Read SMS %d", read_index);

		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", read_index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"ReadMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_read_msg, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_write_sms_in_sim(struct tapi_handle *handle, const TelSmsData_t *pWriteData, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	GVariant *sca = 0, *packet_sca = 0;
	GVariant *tpdu = 0, *packet_tpdu = 0;
	GVariantBuilder b;
	int i;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pWriteData, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	 g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_SIM_SMSP_ADDRESS_LEN; i++)
		g_variant_builder_add(&b, "y", pWriteData->SmsData.Sca[i]);
	sca = g_variant_builder_end(&b);
	packet_sca = g_variant_new("v", sca);

	 g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_NETTEXT_SMDATA_SIZE_MAX + 1; i++)
		g_variant_builder_add(&b, "y", pWriteData->SmsData.szData[i]);
	tpdu = g_variant_builder_end(&b);
	packet_tpdu = g_variant_new("v", tpdu);

	param = g_variant_new("(ii@vi@v)", pWriteData->SmsData.format,
		pWriteData->MsgStatus,
		packet_sca,
		pWriteData->SmsData.MsgLength,
		packet_tpdu);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SaveMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_write_msg, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_delete_sms_in_sim(struct tapi_handle *handle, int del_index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	if ((del_index < -1) || (del_index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", del_index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"DeleteMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_delete_msg, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sms_count(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetMsgCount", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_msg_count, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sms_sca(struct tapi_handle *handle, int sca_index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	if ((sca_index < 0) || (sca_index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", sca_index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSca", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sca, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sms_sca(struct tapi_handle *handle, const TelSmsAddressInfo_t *pSCA, int sca_index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	GVariant *sca = 0, *packet_sca = 0;
	GVariantBuilder b;
	int i;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pSCA, TAPI_API_INVALID_PTR);

	if ((sca_index < 0) || (sca_index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	 g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_SIM_SMSP_ADDRESS_LEN + 1; i++)
		g_variant_builder_add(&b, "y", pSCA->szDiallingNum[i]);
	sca = g_variant_builder_end(&b);
	packet_sca = g_variant_new("v", sca);

	param = g_variant_new("(iiii@v)", sca_index,
							pSCA->Ton,
							pSCA->Npi,
							pSCA->DialNumLen,
							packet_sca);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetSca", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sms_cb_config(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetCbConfig", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_cb_config, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sms_cb_config(struct tapi_handle *handle, const TelSmsCbConfig_t *pCBConfig, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	GVariant *msgId = 0, *packet_msgId = 0;
	GVariantBuilder b;
	int i = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pCBConfig, TAPI_API_INVALID_PTR);

	if ((pCBConfig->Net3gppType > 2) || (pCBConfig->MsgIdRangeCount < 0)) {
		err("Invalid Input -3gppType(%d)", pCBConfig->Net3gppType);
		err("Invalid Input -MsgIdRangeCount(%d)", pCBConfig->MsgIdRangeCount);

		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_variant_builder_init(&b, G_VARIANT_TYPE("aa{sv}"));

	for (i = 0; i < pCBConfig->MsgIdRangeCount; i++) {
		g_variant_builder_open(&b, G_VARIANT_TYPE("a{sv}"));
		if (pCBConfig->Net3gppType == TAPI_NETTEXT_CB_MSG_GSM) {
			g_variant_builder_add(&b, "{sv}", "FromMsgId", g_variant_new_uint16(pCBConfig->MsgIDs[i].Net3gpp.FromMsgId));
			g_variant_builder_add(&b, "{sv}", "ToMsgId", g_variant_new_uint16(pCBConfig->MsgIDs[i].Net3gpp.ToMsgId));
		} else if (pCBConfig->Net3gppType == TAPI_NETTEXT_CB_MSG_UMTS) {
			g_variant_builder_add(&b, "{sv}", "CBCategory", g_variant_new_uint16(pCBConfig->MsgIDs[i].Net3gpp2.CBCategory));
			g_variant_builder_add(&b, "{sv}", "CBLanguage", g_variant_new_uint16(pCBConfig->MsgIDs[i].Net3gpp2.CBLanguage));
		} else {
			dbg("Unknown 3gpp type");
			return FALSE;
		}
		g_variant_builder_add(&b, "{sv}", "Selected", g_variant_new_byte(pCBConfig->MsgIDs[i].Net3gpp.Selected));
		g_variant_builder_close(&b);
	}

	msgId = g_variant_builder_end(&b);
	packet_msgId = g_variant_new("v", msgId);

	param = g_variant_new("(iiii@v)", pCBConfig->Net3gppType,
							pCBConfig->CBEnabled,
							pCBConfig->MsgIdMaxCount,
							pCBConfig->MsgIdRangeCount,
							packet_msgId);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetCbConfig", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sms_memory_status(struct tapi_handle *handle, int memoryStatus, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	if ((memoryStatus < TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE) || (memoryStatus > TAPI_NETTEXT_PDA_MEMORY_STATUS_FULL)) {
		err("Invalid Input -MemoryStatus Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", memoryStatus);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetMemStatus", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_send_sms_deliver_report(struct tapi_handle *handle,
		const TelSmsDatapackageInfo_t *pDataPackage,
		TelSmsResponse_t RPCause,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_sca = NULL;
	gchar *encoded_tpdu = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pDataPackage, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_sca = g_base64_encode((const guchar*)(pDataPackage->Sca), TAPI_SIM_SMSP_ADDRESS_LEN);
	if (NULL == encoded_sca) {
		dbg("g_base64_encode: Failed to encode SCA");
		g_free(evt_cb_data);
		return TAPI_API_OPERATION_FAILED;
	}

	encoded_tpdu = g_base64_encode((const guchar*)(pDataPackage->szData), TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);
	if (NULL == encoded_sca) {
		dbg("g_base64_encode: Failed to encode TPDU");
		g_free(encoded_sca);
		g_free(evt_cb_data);
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(isisi)", pDataPackage->format, encoded_sca,
							pDataPackage->MsgLength,
							encoded_tpdu,
							RPCause);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetDeliveryReport", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_default, evt_cb_data);

	g_free(encoded_sca);
	g_free(encoded_tpdu);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sms_message_status(struct tapi_handle *handle, int set_index, TelSmsMsgStatus_t msgStatus, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	if ((set_index < 0) || (set_index > TAPI_NETTEXT_MAX_INDEX) ||
		(msgStatus > TAPI_NETTEXT_STATUS_RESERVED)) {
		err("Invalid Input -MsgStatus Set Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", set_index, msgStatus);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetMsgStatus", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sms_parameters(struct tapi_handle *handle, int get_index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	if ((get_index < 0) || (get_index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Input -SMS Param Get Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", get_index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSmsParams", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sms_params, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sms_parameters(struct tapi_handle *handle, const TelSmsParams_t *pSmsSetParameters, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	GVariant *alphaId = 0, *packet_alphaId = 0;
	GVariant *destDialNum = 0, *packet_destDialNum = 0;
	GVariant *scaDialNum = 0, *packet_scaDialNum = 0;
	GVariantBuilder b;
	int i;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pSmsSetParameters, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	 g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1; i++)
		g_variant_builder_add(&b, "y", pSmsSetParameters->szAlphaId[i]);
	alphaId = g_variant_builder_end(&b);
	packet_alphaId = g_variant_new("v", alphaId);

	 g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_SIM_SMSP_ADDRESS_LEN + 1; i++)
		g_variant_builder_add(&b, "y", pSmsSetParameters->TpDestAddr.szDiallingNum[i]);
	destDialNum = g_variant_builder_end(&b);
	packet_destDialNum = g_variant_new("v", destDialNum);

	 g_variant_builder_init(&b, G_VARIANT_TYPE("ay"));
	for (i = 0; i < TAPI_SIM_SMSP_ADDRESS_LEN + 1; i++)
		g_variant_builder_add(&b, "y", pSmsSetParameters->TpSvcCntrAddr.szDiallingNum[i]);
	scaDialNum = g_variant_builder_end(&b);
	packet_scaDialNum = g_variant_new("v", scaDialNum);

	param = g_variant_new("(iii@viiii@viii@viii)", pSmsSetParameters->RecordIndex,
			pSmsSetParameters->RecordLen,
			pSmsSetParameters->AlphaIdLen,
			packet_alphaId,
			pSmsSetParameters->ParamIndicator,
			pSmsSetParameters->TpDestAddr.DialNumLen,
			pSmsSetParameters->TpDestAddr.Ton,
			pSmsSetParameters->TpDestAddr.Npi,
			packet_destDialNum,
			pSmsSetParameters->TpSvcCntrAddr.DialNumLen,
			pSmsSetParameters->TpSvcCntrAddr.Ton,
			pSmsSetParameters->TpSvcCntrAddr.Npi,
			packet_scaDialNum,
			pSmsSetParameters->TpProtocolId,
			pSmsSetParameters->TpDataCodingScheme,
			pSmsSetParameters->TpValidityPeriod);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetSmsParams", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sms_parameter_count(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSmsParamCnt", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sms_param_cnt, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_check_sms_device_status(struct tapi_handle *handle, int *pReadyStatus)
{
	GError *error;
	GVariant *smsReady = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pReadyStatus, TAPI_API_INVALID_PTR);

	memset(pReadyStatus, 0, sizeof(int));

	error = NULL;
	smsReady = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSmsReadyStatus", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, NULL, &error);
	if (!smsReady) {
		dbg("error to get SMS ready_status(%s)", error->message);
		g_error_free(error);
		return TAPI_API_OPERATION_FAILED;
	}

	dbg("get SMS ready_status type_format(%s)", g_variant_get_type_string(smsReady));

	g_variant_get(smsReady, "(b)", pReadyStatus);
	g_variant_unref(smsReady);

	msg("************SMS_device_status (%d)", *pReadyStatus);

	return TAPI_API_SUCCESS;
}
