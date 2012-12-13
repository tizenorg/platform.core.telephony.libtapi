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


/**
 * @ingroup	TAPI
 * @defgroup	SMS
 * @{
 *
 * SMS APIs allow a client to accomplish the following features: @n
 * - Send, Recieve, Save, Delete, Read short messages  @n
 * - Set and Get information like Service Center Address, Cell Broadcast configuration,Preferred Bearer, SMS parameters @n
 * - Retrieve information like Current Memory selected, NetTextCount related to the messages @n
 * - Set delivery report @n
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

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_default type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(i)", &result);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
}

static void on_response_read_msg(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	TelSmsData_t	readMsg = {0,};

	GVariant *dbus_result = NULL;
	const char *sca = NULL;
	const char *tpdu = NULL;
	gsize length;
	guchar *decoded_sca = NULL;
	guchar *decoded_tpdu = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_read_msg type_format(%s)", g_variant_get_type_string(dbus_result));

//	readMsg.SimIndex = 0xFFFFFFFF;
	g_variant_get (dbus_result, "(iiisis)", &result,
			&readMsg.SimIndex,
			&readMsg.MsgStatus,
			&sca,
			&readMsg.SmsData.MsgLength,
			&tpdu);

	decoded_sca = g_base64_decode(sca, &length);
	if (decoded_sca) {
		memcpy(&(readMsg.SmsData.Sca[0]), decoded_sca, TAPI_SMS_ENCODED_SCA_LEN_MAX);
		g_free(decoded_sca);
	}

	decoded_tpdu = g_base64_decode(tpdu, &length);
	if (decoded_tpdu) {
		memcpy(&(readMsg.SmsData.szData[0]), decoded_tpdu, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);
		g_free(decoded_tpdu);
	}

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &readMsg, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
}

static void on_response_write_msg(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int index = 0;

	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_write_msg type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(ii)", &result, &index);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &index, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
}

static void on_response_delete_msg(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int index = -1;

	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_delete_msg type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(ii)", &result, &index);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &index, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
}

static void on_response_get_msg_count(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSmsStoredMsgCountInfo_t storedMsgCnt = {0,};

	GVariant *dbus_result = NULL;
	const char *indexList = NULL;
	gsize length;
	guchar *decoded_indexList = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_get_msg_count type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(iiis)", &result,
								&storedMsgCnt.TotalCount,
								&storedMsgCnt.UsedCount,
								&indexList);

	decoded_indexList = g_base64_decode(indexList, &length);
	if (decoded_indexList) {
		memcpy(&(storedMsgCnt.IndexList[0]), decoded_indexList, TAPI_NETTEXT_GSM_SMS_MSG_NUM_MAX);
		g_free(decoded_indexList);
	}

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &storedMsgCnt, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}

}

static void on_response_get_sca(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSmsAddressInfo_t scaInfo = {0,};

	GVariant *dbus_result = NULL;
	const char *sca = NULL;
	gsize length;
	guchar *decoded_sca = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_get_sca type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(iiiis)", &result,
					&scaInfo.Ton,
					&scaInfo.Npi,
					&scaInfo.DialNumLen,
					&sca);

	decoded_sca = g_base64_decode(sca, &length);
	if (decoded_sca) {
		memcpy(&(scaInfo.szDiallingNum[0]), decoded_sca, TAPI_SIM_SMSP_ADDRESS_LEN + 1);
		g_free(decoded_sca);
	}

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &scaInfo, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
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

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_get_cb_config type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(iiiiiaa{sv})", &result,
					&cbConfig.Net3gppType,
					&cbConfig.CBEnabled,
					&cbConfig.MsgIdMaxCount,
					&cbConfig.MsgIdRangeCount,
					&iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "FromMsgId")) {
				cbConfig.MsgIDs[i].Net3gpp.FromMsgId = g_variant_get_uint16(value);
			}
			if (!g_strcmp0(key, "ToMsgId")) {
				cbConfig.MsgIDs[i].Net3gpp.ToMsgId = g_variant_get_uint16(value);
			}
			if (!g_strcmp0(key, "CBCategory")) {
				cbConfig.MsgIDs[i].Net3gpp2.CBCategory = g_variant_get_uint16(value);
			}
			if (!g_strcmp0(key, "CBLanguage")) {
				cbConfig.MsgIDs[i].Net3gpp2.CBLanguage = g_variant_get_uint16(value);
			}
			if (!g_strcmp0(key, "Selected")) {
				cbConfig.MsgIDs[i].Net3gpp.Selected = g_variant_get_byte(value);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &cbConfig, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
}

static void on_response_get_sms_params(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSmsParams_t paramInfo = {0,};

	GVariant *dbus_result = NULL;
	const char *alphaId = NULL;
	const char *destDialNum = NULL;
	const char *scaDialNum = NULL;
	gsize length;
	guchar *decoded_alphaId = NULL;
	guchar *decoded_destDialNum = NULL;
	guchar *decoded_scaDialNum = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_get_sms_params type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(iiiisiiiisiiisiii)", &result,
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

	decoded_alphaId = g_base64_decode(alphaId, &length);
	if (decoded_alphaId) {
		memcpy(&(paramInfo.szAlphaId[0]), decoded_alphaId, TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1);
		g_free(decoded_alphaId);
	}

	decoded_destDialNum = g_base64_decode(destDialNum, &length);
	if (decoded_destDialNum) {
		memcpy(&(paramInfo.TpDestAddr.szDiallingNum[0]), decoded_destDialNum, TAPI_SIM_SMSP_ADDRESS_LEN + 1);
		g_free(decoded_destDialNum);
	}

	decoded_scaDialNum = g_base64_decode(scaDialNum, &length);
	if (decoded_scaDialNum) {
		memcpy(&(paramInfo.TpSvcCntrAddr.szDiallingNum[0]), decoded_scaDialNum, TAPI_SIM_SMSP_ADDRESS_LEN + 1);
		g_free(decoded_scaDialNum);
	}

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &paramInfo, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}

}

static void on_response_get_sms_param_cnt(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int recordCount = 0;

	GVariant *dbus_result = NULL;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data) {
			if (evt_cb_data->cb_fn) {
				evt_cb_data->cb_fn(evt_cb_data->handle, -1, NULL, evt_cb_data->user_data);
			}

			free(evt_cb_data);
		}

		if (error)
			g_error_free(error);

		return;
	}

	dbg("on_response_get_sms_param_cnt type_format(%s)", g_variant_get_type_string(dbus_result));

	g_variant_get (dbus_result, "(ii)", &result,
			&recordCount);

	if (evt_cb_data) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &recordCount, evt_cb_data->user_data);
		}

		free(evt_cb_data);
	}
}

/**
 *
 * This function enables the applications to send SMS to the network.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]	       pDataPackage - SMS-SUBMIT TPDU or SMS-COMMAND, and its length have to be passed in this structure.
 *				tapi_sms_more_to_send_t parameter denotes whether the sms is short or concatenated.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer		tapi_sms_datapackage, tapi_sms_more_to_send_t.
 */
EXPORT_API int tel_send_sms(struct tapi_handle *handle,
		const TelSmsDatapackageInfo_t *pDataPackage,
		int bMoreMsgToSend,
		tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_sca = NULL;
	gchar *encoded_tpdu = NULL;

	int emergency_mode = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pDataPackage ,TAPI_API_INVALID_PTR);
#if 0
	if (vconf_get_int("db/telephony/emergency", &emergency_mode) != 0) {
		err("[FAIL]GET db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
#endif

	if (emergency_mode) {
		dbg("emergency mode on");
		return TAPI_API_OPERATION_FAILED;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_sca = g_base64_encode(&(pDataPackage->Sca[0]), TAPI_SMS_ENCODED_SCA_LEN_MAX);
	if (encoded_sca == NULL) {
		dbg("g_base64_encode: Failed to Enocde the SCA");
		return TAPI_API_OPERATION_FAILED;
	}

	encoded_tpdu = g_base64_encode(&(pDataPackage->szData[0]), TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);
	if (encoded_tpdu == NULL) {
		dbg("g_base64_encode: Failed to Enocde the TPDU");
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(sisi)", encoded_sca,
							pDataPackage->MsgLength,
							encoded_tpdu,
							bMoreMsgToSend);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SendMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, 120000, handle->ca,
		on_response_default, evt_cb_data);

	g_free(encoded_sca);
	g_free(encoded_tpdu);

	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to read sms from the preferred storage.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be read
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark		tapi_sms_selmem_set is to be called always to select the
 *				preferred memory before calling this api.
 * @Refer
 */
EXPORT_API int tel_read_sms_in_sim(struct tapi_handle *handle, int index, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	if( (index < 0) || (index > TAPI_NETTEXT_MAX_INDEX) ) {
		err("Invalid Input -Read SMS %d",index);

		return TAPI_API_INVALID_INPUT;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"ReadMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_read_msg, evt_cb_data);

	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to write/save sms to the preferred storage.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be saved, msg_status will denote the status
 *				of the message whether Sent, Unsent, Read, Unread, Unknown.
 *				pDataPackage- the SMS-SUBMIT PDU or SMS-DELIVER,SMS-STATUSREPORT
 *				being passed to be saved in memory.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @Refer		tapi_sms_datapackage, tapi_sms_status_type.
 */
EXPORT_API int tel_write_sms_in_sim(struct tapi_handle *handle, const TelSmsData_t *pWriteData, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_sca = NULL;
	gchar *encoded_tpdu = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pWriteData ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_sca = g_base64_encode(&(pWriteData->SmsData.Sca[0]), TAPI_SMS_ENCODED_SCA_LEN_MAX);
	if (encoded_sca == NULL) {
		dbg("g_base64_encode: Failed to Enocde the SCA");
		return TAPI_API_OPERATION_FAILED;
	}

	encoded_tpdu = g_base64_encode(&(pWriteData->SmsData.szData[0]), TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);
	if (encoded_tpdu == NULL) {
		dbg("g_base64_encode: Failed to Enocde the TPDU");
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(isis)", pWriteData->MsgStatus,
							encoded_sca,
							pWriteData->SmsData.MsgLength,
							encoded_tpdu);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SaveMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_write_msg, evt_cb_data);

	g_free(encoded_sca);
	g_free(encoded_tpdu);

	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to delete sms to the preferred storage.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be deleted. if index is -1, all sms in the sim are deleted.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark		tapi_sms_selmem_set has to be called always before calling this API
 * @Refer
 */
EXPORT_API int tel_delete_sms_in_sim(struct tapi_handle *handle, int index, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	if ((index < -1) || (index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"DeleteMsg", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_delete_msg, evt_cb_data);

	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to get the count of the messages stored in the memory
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]	None
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *              Its value can be any value from 0 to 255 if the API is returned successfully
 *	            -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface	Asynchronous
 * @remark		The requested memory details come in response API
 * @Refer	    None
 */
EXPORT_API int tel_get_sms_count(struct tapi_handle *handle, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetMsgCount", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_get_msg_count, evt_cb_data);

	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to get the service center address from the SIM storage
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		pSCA - the service center address that is to be set in the SIM storage
 *					    refer to 3GPP TS 23.040:9.1.2.5 Address fields
 *				index - the index that is to be set in the SIM storage
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_sms_sms_addr_info
 */
EXPORT_API int tel_get_sms_sca(struct tapi_handle *handle, int index, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	if ((index < 0) || (index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSca", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_get_sca, evt_cb_data);

	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to set the service center address in the SIM storage
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		pSCA - the service center address that is to be set in the SIM storage
 *					    refer to 3GPP TS 23.040:9.1.2.5 Address fields
 *				index - the index that is to be set in the SIM storage
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark
 * @Refer	       tapi_sms_sms_addr_info
 */
EXPORT_API int tel_set_sms_sca(struct tapi_handle *handle, const TelSmsAddressInfo_t *pSCA, int index, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_sca = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pSCA ,TAPI_API_INVALID_PTR);

	if ((index < 0) || (index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_sca = g_base64_encode(&(pSCA->szDiallingNum[0]), TAPI_SIM_SMSP_ADDRESS_LEN);
	if (encoded_sca == NULL) {
		dbg("g_base64_encode: Failed to Enocde the SCA");
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(iiiis)", index,
							pSCA->Ton,
							pSCA->Npi,
							pSCA->DialNumLen,
							encoded_sca);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetSca", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	g_free(encoded_sca);


	return TAPI_API_SUCCESS;
}


/**
 *
 * This function enables the applications to set the configuration for cell broadcast messages
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       None
 */
EXPORT_API int tel_get_sms_cb_config(struct tapi_handle *handle, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetCbConfig", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_get_cb_config, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables the applications to set the configuration for cell broadcast messages
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		tapi_sms_cb_config the configuration details to be set
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_sms_cb_config
 */
EXPORT_API int tel_set_sms_cb_config(struct tapi_handle *handle, const TelSmsCbConfig_t *pCBConfig, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_cbConfig = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pCBConfig ,TAPI_API_INVALID_PTR);

	if ((pCBConfig->Net3gppType > 2) || (pCBConfig->MsgIdRangeCount < 0)) {
		err("Invalid Input -3gppType(%d)",pCBConfig->Net3gppType);
		err("Invalid Input -MsgIdRangeCount(%d)",pCBConfig->MsgIdRangeCount);

		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_cbConfig = g_base64_encode((const guchar *)&(pCBConfig->MsgIDs[0]), TAPI_NETTEXT_GSM_SMS_CBMI_LIST_SIZE_MAX*5);
	if (encoded_cbConfig == NULL) {
		dbg("g_base64_encode: Failed to Enocde the CB Config");
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(iiiis)", pCBConfig->Net3gppType,
							pCBConfig->CBEnabled,
							pCBConfig->MsgIdMaxCount,
							pCBConfig->MsgIdRangeCount,
							encoded_cbConfig);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetCbConfig", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	g_free(encoded_cbConfig);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables the applications to set the phone memory status whether full or available
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		memory_status whether full or available
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_sms_memstatus_type
 */
EXPORT_API int tel_set_sms_memory_status(struct tapi_handle *handle, int memoryStatus, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	if ((memoryStatus < TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE) || (memoryStatus > TAPI_NETTEXT_PDA_MEMORY_STATUS_FULL)) {
		err("Invalid Input -MemoryStatus Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", memoryStatus);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetMemStatus", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function is used to get SMS preferred bearer on which SMS has to be transmitted. This is a synchronous function
 *
 * @return		TRUE in case of success and FALSE in case of error .
 * @param[in]        None
 *
 * @param[out]	requestId-Unique identifier for a particular request
 *                          Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_get_sms_preferred_bearer(struct tapi_handle *handle, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetPrefBearer", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables the applications to set the preferred bearer
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		service option ie the bearer type to be set
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_sms_bearer_type
 */
EXPORT_API int tel_set_sms_preferred_bearer(struct tapi_handle *handle, TelSmsBearerType_t BearerType, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	if ((TAPI_NETTEXT_BEARER_PS_ONLY > BearerType) || (TAPI_NETTEXT_NO_PREFERRED_BEARER < BearerType)) {
		err("Invalid Input -PrefBearer Set Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", BearerType);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetPrefBearer", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables the applications to set the deliver report for an incoming message(MT).
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]	       pDataPackage - SMS-DELIVER-REPORT, and its length have to be passed in this structure.
 * @param[in]		RPCause - the result cause
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Synchronous.
 * @remark
 * @Refer	       tapi_sms_deliver_report
 */
EXPORT_API int tel_send_sms_deliver_report(struct tapi_handle *handle,
		const TelSmsDatapackageInfo_t *pDataPackage,
		TelSmsResponse_t RPCause,
		tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_sca = NULL;
	gchar *encoded_tpdu = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pDataPackage ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_sca = g_base64_encode(&(pDataPackage->Sca[0]), TAPI_SMS_ENCODED_SCA_LEN_MAX);
	if (encoded_sca == NULL) {
		dbg("g_base64_encode: Failed to Enocde the SCA");
		return TAPI_API_OPERATION_FAILED;
	}

	encoded_tpdu = g_base64_encode(&(pDataPackage->szData[0]), TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);
	if (encoded_tpdu == NULL) {
		dbg("g_base64_encode: Failed to Enocde the TPDU");
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(sisi)", encoded_sca,
							pDataPackage->MsgLength,
							encoded_tpdu,
							RPCause);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetDeliveryReport", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	g_free(encoded_sca);
	g_free(encoded_tpdu);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables the application to set the status of a message
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be changed
 *				msgStatus -SMS message status
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_sms_memory_type,tapi_sms_status_type
 */
EXPORT_API int tel_set_sms_message_status(struct tapi_handle *handle, int index, TelSmsMsgStatus_t msgStatus, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	if ((index < 0) || (index > TAPI_NETTEXT_MAX_INDEX) ||
		(msgStatus > TAPI_NETTEXT_STATUS_RESERVED)) {
		err("Invalid Input -MsgStatus Set Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", index, msgStatus);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetMsgStatus", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables to application to get the sms parameters
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		record index of the record in which the sms parameters are stored in the EFsmsp file
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark		Requested details come in response API
 * @Refer
 */
EXPORT_API int tel_get_sms_parameters(struct tapi_handle *handle, int index, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	if ((index < 0) || (index > TAPI_NETTEXT_MAX_INDEX)) {
		err("Invalid Input -SMS Param Get Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", index);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSmsParams", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_get_sms_params, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables to application to get the sms parameters
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		tapi_smsp_set_param_t sms parameters to be set in the EFsmsp file
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark
 * @Refer	       tapi_smsp_set_param_t
 */
EXPORT_API int tel_set_sms_parameters(struct tapi_handle *handle, const TelSmsParams_t *pSmsSetParameters, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	gchar *encoded_alphaId = NULL;
	gchar *encoded_destDialNum = NULL;
	gchar *encoded_scaDialNum = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pSmsSetParameters ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	encoded_alphaId = g_base64_encode((guchar *)&(pSmsSetParameters->szAlphaId[0]), TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1);
	if (encoded_alphaId == NULL) {
		dbg("g_base64_encode: Failed to Enocde the AlphaId");
		return TAPI_API_OPERATION_FAILED;
	}

	encoded_destDialNum = g_base64_encode(&(pSmsSetParameters->TpDestAddr.szDiallingNum[0]), TAPI_SIM_SMSP_ADDRESS_LEN + 1);
	if (encoded_destDialNum == NULL) {
		dbg("g_base64_encode: Failed to Enocde the DestAddr.DiallingNum");
		return TAPI_API_OPERATION_FAILED;
	}

	encoded_scaDialNum = g_base64_encode(&(pSmsSetParameters->TpSvcCntrAddr.szDiallingNum[0]), TAPI_SIM_SMSP_ADDRESS_LEN + 1);
	if (encoded_scaDialNum == NULL) {
		dbg("g_base64_encode: Failed to Enocde the ScaAddr.DiallingNum");
		return TAPI_API_OPERATION_FAILED;
	}

	param = g_variant_new("(iiisiiiisiiisiii)", pSmsSetParameters->RecordIndex,
									pSmsSetParameters->RecordLen,
									pSmsSetParameters->AlphaIdLen,
									encoded_alphaId,
									pSmsSetParameters->ParamIndicator,
									pSmsSetParameters->TpDestAddr.DialNumLen,
									pSmsSetParameters->TpDestAddr.Ton,
									pSmsSetParameters->TpDestAddr.Npi,
									encoded_destDialNum,
									pSmsSetParameters->TpSvcCntrAddr.DialNumLen,
									pSmsSetParameters->TpSvcCntrAddr.Ton,
									pSmsSetParameters->TpSvcCntrAddr.Npi,
									encoded_scaDialNum,
									pSmsSetParameters->TpProtocolId,
									pSmsSetParameters->TpDataCodingScheme,
									pSmsSetParameters->TpValidityPeriod);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"SetSmsParams", param, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_default, evt_cb_data);

	g_free(encoded_alphaId);
	g_free(encoded_destDialNum);
	g_free(encoded_scaDialNum);

	return TAPI_API_SUCCESS;
}

/**
 *
 * This function enables to application to get the sms parameter count from the EFsmsp file.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark		Requested details come in response API
 * @Refer
 */
EXPORT_API int tel_get_sms_parameter_count(struct tapi_handle *handle, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(callback ,TAPI_API_INVALID_PTR);

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
		DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_SMS_INTERFACE,
		"GetSmsParamCnt", NULL, NULL,
		G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
		on_response_get_sms_param_cnt, evt_cb_data);

	return TAPI_API_SUCCESS;
}

#if 0
/**
 *
 *  This API is used to send an SMS message to the network. This API allows keeping
 *  the dedicated link at lower layers by passing information as more messages to send in parameters.
 * This will enable not to release if a dedicated connection is used for transmission.
 *
 * @return		TRUE in case of success and FALSE in case of error .
 * @param[in]		TelSmsMsgInfo_t - SMS_SUBMIT/ACK/CANCEL and its length have to be passed in this structure.
 *
 *				unsigned int MoreMsgToSend will be set to TRUE when there are more than one message to be
 *                         send or Multiple segmented concatenated message to be send, otherwise FALSE. This flag
 *                         indicates OEM that more messages to send.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 *
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSmsMsgInfo_t.
 */
EXPORT_API int tel_send_sms_msg(struct tapi_handle *handle, const TelSmsMsgInfo_t *pSmsMsgInfo, unsigned int MoreMsgToSend, tapi_response_cb callback, void* user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;
	int emergency_mode = 0;

	if (vconf_get_int("db/telephony/emergency", &emergency_mode) != 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL]GET db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
	if (emergency_mode) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "emergency mode on");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(pSmsMsgInfo ,TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pSmsMsgInfo, sizeof(TelSmsMsgInfo_t));
	g_array_append_vals(in_param2, &MoreMsgToSend, sizeof(unsigned int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SEND_EX, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}
#endif

/**
 *
 * This function enables the applications to check the device status
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 *
 * @param[out]	Device status whether Ready or Not Ready
 * @Interface		Synchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_check_sms_device_status(struct tapi_handle *handle, int *pReadyStatus)
{
	GError *error;
	GVariant *smsReady = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pReadyStatus, TAPI_API_INVALID_PTR);

	memset(pReadyStatus, 0, sizeof(int));

	error = NULL;
	smsReady = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE , handle->path,
			DBUS_TELEPHONY_SMS_INTERFACE, "GetSmsReadyStatus", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

	if(!smsReady){
		dbg( "error to get SMS ready_status(%s)", error->message);
		g_error_free (error);
		return TAPI_API_OPERATION_FAILED;
	}

	dbg("get SMS ready_status type_format(%s)", g_variant_get_type_string(smsReady));

	g_variant_get(smsReady, "(b)", pReadyStatus);

	dbg("************SMS_device_status (%d)", *pReadyStatus);

	return TAPI_API_SUCCESS;
}

#if 0
/**
 *
 * This function enables to application to set that the device is ready to recieve messages from network.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 * @param[out]	None
 * @Interface		Synchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_set_sms_device_status(struct tapi_handle *handle)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance ");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_DEVICEREADY,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}
#endif
//eof
