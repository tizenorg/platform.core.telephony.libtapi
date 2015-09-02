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
#include <string.h>
#include <stdlib.h>

#include "tapi_common.h"
#include "TapiUtility.h"
#include "TelSs.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiSs.h"

static void on_response_get_ss_barring_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1, i = 0;
	TelSsBarringResp_t resp;

	GVariant *dbus_result = 0, *value = 0;
	GVariantIter *iter = 0;
	GVariantIter *iter_row = 0;
	const gchar *key = 0;

	memset(&resp, '\0', sizeof(TelSsBarringResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(aa{sv}i)", &iter, &result);

	resp.record_num = g_variant_iter_n_children(iter);

	if (TAPI_SS_RECORD_NUM_MAX < resp.record_num)
		resp.record_num = TAPI_SS_RECORD_NUM_MAX;

	while (g_variant_iter_next(iter, "a{sv}", &iter_row) && (i < resp.record_num)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "ss_class"))
				resp.record[i].Class = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "ss_status"))
				resp.record[i].Status = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "barring_mode"))
				resp.record[i].Flavour = g_variant_get_int32(value);
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &resp);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_change_ss_barring_password(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)",  &result);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, NULL);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_ss_forward_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1, i = 0;
	TelSsForwardResp_t resp;

	GVariant *dbus_result = 0, *value = 0;
	GVariantIter *iter = 0;
	GVariantIter *iter_row = 0;
	const gchar *key = 0;


	memset(&resp, '\0', sizeof(TelSsForwardResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(aa{sv}i)", &iter, &result);

	resp.record_num = g_variant_iter_n_children(iter);

	if (TAPI_SS_RECORD_NUM_MAX < resp.record_num)
		resp.record_num = TAPI_SS_RECORD_NUM_MAX;

	while (g_variant_iter_next(iter, "a{sv}", &iter_row) && (i < resp.record_num)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "ss_class"))
				resp.record[i].Class = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "ss_status"))
				resp.record[i].Status = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "forwarding_mode"))
				resp.record[i].ForwardCondition = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "number_present"))
				resp.record[i].bCallForwardingNumberPresent = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "no_reply_time"))
				resp.record[i].NoReplyWaitTime = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "type_of_number"))
				resp.record[i].Ton = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "number_plan_identity"))
				resp.record[i].Npi = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "forwarding_number"))
				strncpy((char *)resp.record[i].szCallForwardingNumber,
					g_variant_get_string(value, 0), TAPI_CALL_DIALDIGIT_LEN_MAX);
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &resp);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_ss_waiting_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1, i = 0;
	TelSsWaitingResp_t resp;

	GVariant *dbus_result = 0, *value = 0;
	GVariantIter *iter = 0;
	GVariantIter *iter_row = 0;
	const gchar *key = 0;


	memset(&resp, '\0', sizeof(TelSsWaitingResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(aa{sv}i)", &iter, &result);

	resp.record_num = g_variant_iter_n_children(iter);

	if (TAPI_SS_RECORD_NUM_MAX < resp.record_num)
		resp.record_num = TAPI_SS_RECORD_NUM_MAX;

	while (g_variant_iter_next(iter, "a{sv}", &iter_row) && (i < resp.record_num)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "ss_class"))
				resp.record[i].Class = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "ss_status"))
				resp.record[i].Status = g_variant_get_int32(value);
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &resp);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_set_ss_cli_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int data = 0;

	GVariant *dbus_result = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)",  &result);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_ss_cli_status(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	TelSsCliResp_t resp;

	GVariant *dbus_result = 0;

	memset(&resp, '\0', sizeof(TelSsCliResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)",  &result, &resp.Type, &resp.Status);
	msg("Type(%d), Status(%d)", resp.Type, resp.Status);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &resp);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_send_ss_ussd_request(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = 0;
	GDBusConnection *conn = 0;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	char *tmp_str = 0;
	TelSsUssdResp_t resp;

	GVariant *dbus_result = 0;

	memset(&resp, '\0', sizeof(TelSsUssdResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(iiiiis)",  &result, &resp.Type, &resp.Status, &resp.Dcs, &resp.Length, &tmp_str);

	if (!result) {
		memcpy(resp.szString, tmp_str, resp.Length);
		g_free(tmp_str);
	}

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &resp);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

EXPORT_API int tel_set_ss_barring(TapiHandle *handle, TelSsBarringInfo_t *info, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;
	const gchar *method = NULL;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(info, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(iis)", info->Class, info->Type, info->szPassword);

	if (info->Mode == TAPI_SS_CB_MODE_ACTIVATE) {
		msg("[ check ] barring activate");
		method = "ActivateBarring";

	} else {
		msg("[ check ] barring deactivate");
		method = "DeactivateBarring";

	}

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			method, param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_barring_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_ss_barring_status(TapiHandle *handle, TelSsClass_t class, TelSsBarringType_t type, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", class, type);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"GetBarringStatus", param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_barring_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to change call barring password.
 *
 * @param	pOldPassword[IN] of type char* Specifies the Old Call Barring Password.
 * @param	pNewPassword[IN] of type char* Specifies the New Call Barring Password.
 * @param	pNewPasswordAgain[IN] of type char* Specifies the New Call Barring Password for Confirm.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_change_ss_barring_password(TapiHandle *handle,
		const char *old_password,
		const char *new_password,
		const char *new_password_again,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	msg("barring_password change request");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(old_password, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(new_password, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(new_password_again, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(sss)", old_password, new_password, new_password_again);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"ChangeBarringPassword", param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_change_ss_barring_password, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to Activate or Deactivate the call forwarding feature.
 *
 * @param	forwardInfo[IN] of type TelSsForwardInfo_t which specifies the Teleservice and CF flavour, Forwarded to number.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_forward(TapiHandle *handle, const TelSsForwardInfo_t *info, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;
	const gchar *method = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(info, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(iiiiis)", info->Class, info->Condition,
			info->NoReplyConditionTimer, info->Ton, info->Npi, info->szPhoneNumber);

	switch (info->Mode) {
	case TAPI_SS_CF_MODE_DISABLE_EV: {
		msg("[ check ] forwarding deactivate");
		method = "DeactivateForwarding";
	}
	break;

	case TAPI_SS_CF_MODE_ENABLE_EV: {
		msg("[ check ] forwarding activate");
		method = "ActivateForwarding";
	}
	break;

	case TAPI_SS_CF_MODE_REGISTRATION_EV: {
		msg("[ check ] forwarding registration");
		method = "RegisterForwarding";
	}
	break;

	case TAPI_SS_CF_MODE_ERASURE_EV: {
		msg("[ check ] forwarding deregistration");
		method = "DeregisterForwarding";
	}
	break;

	default:
		err("[ error ] invalid input : (0x%x)", info->Mode);
		return TAPI_API_INVALID_INPUT;
	}

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			method, param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_forward_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to interrogate the status of call forwarding feature.
 *
 * @param	forwardType[IN] of type TelSsForwardType_t which specifies the telecommunication service.
 * @param	condition[IN]	is of type TelSsForwardWhen_t specified the CF flavour
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_get_ss_forward_status(TapiHandle *handle, TelSsClass_t class, TelSsForwardWhen_t condition, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", class, condition);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"GetForwardingStatus", param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_forward_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to Activate or dectivate call waiting feature.
 *
 * @param	waitInfo[IN] of type TelSsWaitingInfo_t which specifies the telecommunication service and mode.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_waiting(TapiHandle *handle, const TelSsWaitingInfo_t *info, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;
	const gchar *method = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(info, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", info->Class);

	if (info->Mode == TAPI_SS_CW_MODE_ACTIVATE) {
		msg("[ check ] waiting activate");
		method = "ActivateWaiting";
	} else {
		msg("[ check ] waiting deactivate");
		method = "DeactivateWaiting";
	}

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			method, param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_waiting_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to interrogate the status of call waiting feature.
 *
 * @param	CallType[IN] of type TelSsCallType_t which specifies the telecommunication service .
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/

EXPORT_API int tel_get_ss_waiting_status(TapiHandle *handle, const TelSsClass_t class, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", class);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"GetWaitingStatus", param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_waiting_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to set the status of CLI services feature.
 *
 * @param	CliType[IN] of type TelSsCliType_t which specifies the CLI service .
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_cli_status(TapiHandle *handle, TelSsCliType_t type, TelSsCliStatus_t status, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	msg("set cli type:[%d] status:[%d]", type, status);

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", type, status);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"SetCLIStatus", param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_set_ss_cli_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to interrogate the status of CLI services feature.
 *
 * @param	CliType[IN] of type TelSsCliType_t which specifies the CLI service .
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_get_ss_cli_status(TapiHandle *handle, TelSsCliType_t type, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	dbg("Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", type);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"GetCLIStatus", param, 0,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_ss_cli_status, evt_cb_data);

	return TAPI_API_SUCCESS;
}

/**********************************************************************************************
 *
 * This function is used to send a USSD string and also the User response for a NW intiated USSD.
 *
 * @param	pAppMsg[IN] of type TelSsUssdMsgInfo_t which specifies the USSD String,length of the string, format of the string.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark	TelSsUssdMsgInfo_t ->IndType is not used while calling this function, This is used for sending the response.
 *			Limo standard defines only one structure for Request and response.
 *
 ***********************************************************************************************/
EXPORT_API int tel_send_ss_ussd_request(TapiHandle *handle, const TelSsUssdMsgInfo_t *info, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = 0;
	GVariant *param = 0;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(info, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(iiis)", info->Type, info->Dcs, info->Length, info->szString);
	msg("ussd type:[%d] length:[%d]", info->Type, info->Length);
	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_SS_INTERFACE,
			"SendUSSD", param, 0,
			G_DBUS_CALL_FLAGS_NONE, G_MAXINT, handle->ca,
			on_response_send_ss_ussd_request, evt_cb_data);

	return TAPI_API_SUCCESS;
}
