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

#include "tapi_private.h"
#include "tapi.h"
#include "tapi_ss.h"

#include <tel_ss.h>

void on_ss_signal_emit_handler(TelephonySs *ss, gchar *sender_name,
					gchar *signal_name, GVariant *parameters,
					gpointer user_data)
{
	TapiEvtCbData *evt_cb_data = NULL;
	TelHandle *handle = user_data;
	char *evt_id = NULL;

	dbg("Entry");

	if (handle == NULL) {
		err("invalid handle");
		return;
	}

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_SS_INTERFACE,
					signal_name);

	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		err("invalid evt_id[%s]", evt_id);
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "NotifyUssd")) {
		TelSsUssdNoti ussd_noti;
		memset(&ussd_noti, 0, sizeof(TelSsUssdNoti));
		g_variant_get(parameters, "(is)", &ussd_noti.status, &ussd_noti.str);

		dbg("signal[%s] status[%d] str[%s]", signal_name,
					ussd_noti.status, ussd_noti.str);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &ussd_noti);
		g_free(ussd_noti.str);
	} else {
		err("Unsupported signal %s", signal_name);
	}

	g_free(evt_id);
}

static gboolean __tapi_check_ss_class(TelSsClass ss_class)
{
	switch (ss_class) {
	case TEL_SS_CLASS_ALL_TELE_BEARER:
	case TEL_SS_CLASS_ALL_TELE:
	case TEL_SS_CLASS_VOICE:
	case TEL_SS_CLASS_ALL_DATA_TELE:
	case TEL_SS_CLASS_FAX:
	case TEL_SS_CLASS_SMS:
	case TEL_SS_CLASS_VGCS:
	case TEL_SS_CLASS_VBS:
	case TEL_SS_CLASS_ALL_TELE_EXPT_SMS:
	case TEL_SS_CLASS_ALL_BEARER:
	case TEL_SS_CLASS_ALL_ASYNC:
	case TEL_SS_CLASS_ALL_SYNC:
	case TEL_SS_CLASS_ALL_CS_SYNC:
	case TEL_SS_CLASS_ALL_CS_ASYNC:
	case TEL_SS_CLASS_ALL_DEDI_PS:
	case TEL_SS_CLASS_ALL_DEDI_PAD:
	case TEL_SS_CLASS_ALL_DATA_CDA:
	case TEL_SS_CLASS_ALL_GPRS_BEARER:
	case TEL_SS_CLASS_AUX_VOICE:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_barring_type(TelSsBarringType type)
{
	switch (type) {
	case TEL_SS_CB_TYPE_BAOC:
	case TEL_SS_CB_TYPE_BOIC:
	case TEL_SS_CB_TYPE_BOIC_NOT_HC:
	case TEL_SS_CB_TYPE_BAIC:
	case TEL_SS_CB_TYPE_BIC_ROAM:
	case TEL_SS_CB_TYPE_NS:
	case TEL_SS_CB_TYPE_AB:
	case TEL_SS_CB_TYPE_AOB:
	case TEL_SS_CB_TYPE_AIB:
		return TRUE;
	}

	return FALSE;
}
static gboolean __tapi_check_barring_pwd(const char *pwd)
{
	if (pwd == NULL || (strlen(pwd) > TEL_SS_BARRING_PASSWORD_LEN_MAX))
		return FALSE;
	else
		return TRUE;
}
static gboolean __tapi_check_forwarding_mode(TelSsForwardMode mode)
{
	switch (mode) {
	case TEL_SS_CF_MODE_DISABLE:
	case TEL_SS_CF_MODE_ENABLE:
	case TEL_SS_CF_MODE_REGISTER:
	case TEL_SS_CF_MODE_DEREGISTER:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_forwarding_condition(TelSsForwardCondition condition)
{
	switch (condition) {
	case TEL_SS_CF_COND_CFU:
	case TEL_SS_CF_COND_CFB:
	case TEL_SS_CF_COND_CFNRY:
	case TEL_SS_CF_COND_CFNRC:
	case TEL_SS_CF_COND_ALL:
	case TEL_SS_CF_COND_ALL_CFC:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_forwarding_number(const char *number)
{
	if (number == NULL || (strlen(number) > TEL_SS_NUMBER_LEN_MAX))
		return FALSE;
	else
		return TRUE;
}

static gboolean __tapi_check_and_get_cli_status(const TelSsCliInfo *cli_info, gint *status)
{
	switch (cli_info->type) {
	case TEL_SS_CLI_CLIR:
		if (cli_info->status.clir <= TEL_CLIR_STATUS_SUPPRESSION) {
			*status = cli_info->status.clir;
			return TRUE;
		}
	break;
	case TEL_SS_CLI_CLIP:
		if (cli_info->status.clip == TEL_SS_CLI_DISABLE || cli_info->status.clip == TEL_SS_CLI_ENABLE) {
			*status = cli_info->status.clip;
			return TRUE;
		}
	break;
	case TEL_SS_CLI_COLP:
		if (cli_info->status.colp == TEL_SS_CLI_DISABLE || cli_info->status.colp == TEL_SS_CLI_ENABLE) {
			*status = cli_info->status.colp;
			return TRUE;
		}
	break;
	case TEL_SS_CLI_COLR:
		if (cli_info->status.colr == TEL_SS_CLI_DISABLE || cli_info->status.colr == TEL_SS_CLI_ENABLE) {
			*status = cli_info->status.colr;
			return TRUE;
		}
	break;
	case TEL_SS_CLI_CDIP:
		if (cli_info->status.cdip == TEL_SS_CLI_DISABLE || cli_info->status.cdip == TEL_SS_CLI_ENABLE) {
			*status = cli_info->status.cdip;
			return TRUE;
		}
	break;
	case TEL_SS_CLI_CNAP:
		if (cli_info->status.cnap == TEL_SS_CLI_DISABLE || cli_info->status.cnap == TEL_SS_CLI_ENABLE) {
			*status = cli_info->status.cnap;
			return TRUE;
		}
	}

	return FALSE;
}

static gboolean __tapi_check_cli_type(TelSsCliType type)
{
	switch (type) {
	case TEL_SS_CLI_CLIR:
	case TEL_SS_CLI_CLIP:
	case TEL_SS_CLI_COLP:
	case TEL_SS_CLI_COLR:
	case TEL_SS_CLI_CDIP:
	case TEL_SS_CLI_CNAP:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_ussd_type(TelSsUssdType type)
{
	switch (type) {
	case TEL_SS_USSD_TYPE_USER_INIT:
	case TEL_SS_USSD_TYPE_USER_RSP:
	case TEL_SS_USSD_TYPE_USER_REL:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_ussd_string(unsigned char *string)
{
	if (string == NULL)
		return FALSE;

	return TRUE;
}

static void on_response_ss_set_barring(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_set_barring_finish(handle->ss_proxy, (int *)&result, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_ss_set_barring(TelHandle *handle,
	const TelSsBarringInfo *barring_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& barring_info && __tapi_check_ss_class(barring_info->class)
		&& __tapi_check_barring_type(barring_info->type)
		&& __tapi_check_barring_pwd(barring_info->pwd),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_set_barring(handle->ss_proxy,
		barring_info->class, barring_info->enable,
		barring_info->type, barring_info->pwd, NULL,
		on_response_ss_set_barring, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_get_barring_status(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;

	GVariant *record_list;
	TelSsBarringResp barring_resp = {0, };

	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_get_barring_status_finish(handle->ss_proxy,
		(int *)&result, &(barring_resp.record_num),
		&record_list, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SS_RESULT_SUCCESS) {
		g_variant_unref(record_list);
		err("Failed to get barring status: %d", result);

		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if ((barring_resp.record_num != 0)
			&& (g_variant_n_children(record_list)
			== barring_resp.record_num)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Barring status list count: [%d]", barring_resp.record_num);
		barring_resp.records =
			g_malloc0(sizeof(TelSsBarringInfoRecord)*barring_resp.record_num);

		g_variant_get(record_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "class") == 0) {
					barring_resp.records[i].class = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "enable") == 0) {
					barring_resp.records[i].enable = g_variant_get_boolean(key_value);
				}

				if (g_strcmp0(key, "type") == 0) {
					barring_resp.records[i].type = g_variant_get_int32(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("Barring status list count is NOT valid - Count: [%d]", barring_resp.record_num);
		barring_resp.record_num = 0;
	}
	g_variant_unref(record_list);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &barring_resp);

	g_free(barring_resp.records);
}

EXPORT_API TelReturn tapi_ss_get_barring_status(TelHandle *handle,
	const TelSsBarringGetInfo *get_barring_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& get_barring_info && __tapi_check_ss_class(get_barring_info->class)
		&& __tapi_check_barring_type(get_barring_info->type),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_get_barring_status(handle->ss_proxy,
		get_barring_info->class, get_barring_info->type, NULL,
		on_response_ss_get_barring_status, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_change_barring_password(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_change_barring_password_finish(handle->ss_proxy, (int *)&result, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_ss_change_barring_password(TelHandle *handle,
	const TelSsBarringPwdInfo *barring_pwd_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& barring_pwd_info && __tapi_check_barring_pwd(barring_pwd_info->new_pwd)
		&& __tapi_check_barring_pwd(barring_pwd_info->old_pwd),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_change_barring_password(handle->ss_proxy,
		barring_pwd_info->old_pwd, barring_pwd_info->new_pwd, NULL,
		on_response_ss_change_barring_password, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_set_forwarding(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_set_forwarding_finish(handle->ss_proxy, (int *)&result, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_ss_set_forwarding(TelHandle *handle,
	const TelSsForwardInfo *forwarding_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& forwarding_info && __tapi_check_ss_class(forwarding_info->class)
		&& __tapi_check_forwarding_mode(forwarding_info->mode)
		&& __tapi_check_forwarding_condition(forwarding_info->condition)
		&& __tapi_check_forwarding_number(forwarding_info->number),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_set_forwarding(handle->ss_proxy,
		forwarding_info->class, forwarding_info->mode, forwarding_info->condition,
		forwarding_info->number, forwarding_info->wait_time, NULL,
		on_response_ss_set_forwarding, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_get_forwarding_status(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *record_list;
	TelSsForwardingResp forwarding_resp = {0, };

	GError *error = NULL;
	TelSsResult result = TEL_SS_RESULT_FAILURE;

	dbg("Entry");

	telephony_ss_call_get_forwarding_status_finish(handle->ss_proxy,
		(int *)&result, &(forwarding_resp.record_num),
		&record_list, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SS_RESULT_SUCCESS) {
		g_variant_unref(record_list);
		err("Failed to get forwarding status: %d", result);

		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if ((forwarding_resp.record_num != 0)
			&& (g_variant_n_children(record_list)
			== forwarding_resp.record_num)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Forwarding status list count: [%d]", forwarding_resp.record_num);
		forwarding_resp.records =
			g_malloc0(sizeof(TelSsForwardingInfoRecord)*forwarding_resp.record_num);

		g_variant_get(record_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "class") == 0) {
					forwarding_resp.records[i].class = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "enable") == 0) {
					forwarding_resp.records[i].enable = g_variant_get_boolean(key_value);
				}

				if (g_strcmp0(key, "condition") == 0) {
					forwarding_resp.records[i].condition = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "number") == 0) {
					g_strlcpy(forwarding_resp.records[i].number,
						g_variant_get_string(key_value, NULL),
						TEL_SS_NUMBER_LEN_MAX + 1);
				}

				if (g_strcmp0(key, "wait_time") == 0) {
					forwarding_resp.records[i].wait_time = g_variant_get_int32(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("Forwarding status list count is NOT valid - Count: [%d]",
			forwarding_resp.record_num);
		forwarding_resp.record_num = 0;
	}
	g_variant_unref(record_list);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &forwarding_resp);

	g_free(forwarding_resp.records);
}

EXPORT_API TelReturn tapi_ss_get_forwarding_status(TelHandle *handle,
	const TelSsForwardGetInfo *get_forwarding_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& get_forwarding_info && __tapi_check_ss_class(get_forwarding_info->class)
		&& __tapi_check_forwarding_condition(get_forwarding_info->condition),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_get_forwarding_status(handle->ss_proxy,
		get_forwarding_info->class, get_forwarding_info->condition, NULL,
		on_response_ss_get_forwarding_status, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_set_waiting(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_set_waiting_finish(handle->ss_proxy, (int *)&result, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_ss_set_waiting(TelHandle *handle,
	const TelSsWaitingInfo *waiting_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& waiting_info && __tapi_check_ss_class(waiting_info->class),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_set_waiting(handle->ss_proxy,
		waiting_info->class, waiting_info->enable, NULL,
		on_response_ss_set_waiting, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_get_waiting_status(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *record_list;
	TelSsWaitingResp waiting_resp = {0, };

	GError *error = NULL;
	TelSsResult result = TEL_SS_RESULT_FAILURE;

	dbg("Entry");

	telephony_ss_call_get_waiting_status_finish(handle->ss_proxy,
		(int *)&result, &(waiting_resp.record_num),
		&record_list, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SS_RESULT_SUCCESS) {
		g_variant_unref(record_list);
		err("Failed to get waiting status: %d", result);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if ((waiting_resp.record_num != 0)
			&& (g_variant_n_children(record_list)
			== waiting_resp.record_num)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Waiting status list count: [%d]", waiting_resp.record_num);
		waiting_resp.records =
			g_malloc0(sizeof(TelSsWaitingInfo)*waiting_resp.record_num);

		g_variant_get(record_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "class") == 0) {
					waiting_resp.records[i].class = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "enable") == 0) {
					waiting_resp.records[i].enable = g_variant_get_boolean(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("Waiting status list count is NOT valid - Count: [%d]",
			waiting_resp.record_num);
		waiting_resp.record_num = 0;
	}
	g_variant_unref(record_list);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &waiting_resp);

	g_free(waiting_resp.records);
}

EXPORT_API TelReturn tapi_ss_get_waiting_status(TelHandle *handle,
	TelSsClass class,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle
		&& callback
		&& __tapi_check_ss_class(class),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_get_waiting_status(handle->ss_proxy,
		class, NULL,
		on_response_ss_get_waiting_status, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_set_cli(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_set_cli_finish(handle->ss_proxy, (int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_ss_set_cli(TelHandle *handle,
	const TelSsCliInfo *cli_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	int cli_status;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle
		&& callback && cli_info
		&& __tapi_check_and_get_cli_status(cli_info, &cli_status),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_set_cli(handle->ss_proxy,
		cli_info->type, cli_status, NULL,
		on_response_ss_set_cli, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_get_cli_status(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	TelSsCliResp cli_resp = {0, };
	int net_status;
	int dev_status;
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_get_cli_status_finish(handle->ss_proxy,
		(int *)&result, (int *)(&(cli_resp.type)), &net_status, &dev_status,
		res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SS_RESULT_SUCCESS) {
		err("Failed to get CLI status: %d", result);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	switch (cli_resp.type) {
	case TEL_SS_CLI_CLIR:
		cli_resp.status.clir.net_status = (TelSsClirNetworkStatus)net_status;
		cli_resp.status.clir.dev_status = (TelSsClirDeviceStatus)dev_status;
	break;
	case TEL_SS_CLI_CLIP:
		cli_resp.status.clip.net_status = (TelSsCliNetworkStatus)net_status;
		cli_resp.status.clip.dev_status = (TelSsCliDeviceStatus)dev_status;
	break;
	case TEL_SS_CLI_COLP:
		cli_resp.status.colp.net_status = (TelSsCliNetworkStatus)net_status;
		cli_resp.status.colp.dev_status = (TelSsCliDeviceStatus)dev_status;
	break;
	case TEL_SS_CLI_COLR:
		cli_resp.status.colr.net_status = (TelSsCliNetworkStatus)net_status;
		cli_resp.status.colr.dev_status = (TelSsCliDeviceStatus)dev_status;
	break;
	case TEL_SS_CLI_CDIP:
		cli_resp.status.cdip.net_status = (TelSsCliNetworkStatus)net_status;
		cli_resp.status.cdip.dev_status = (TelSsCliDeviceStatus)dev_status;
	break;
	case TEL_SS_CLI_CNAP:
		cli_resp.status.cnap.net_status = (TelSsCliNetworkStatus)net_status;
		cli_resp.status.cnap.dev_status = (TelSsCliDeviceStatus)dev_status;
	break;
	default :
		result = TEL_SS_RESULT_FAILURE;
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	RESP_CALLBACK_CALL(rsp_cb_data, result, &cli_resp);
}

EXPORT_API TelReturn tapi_ss_get_cli_status(TelHandle *handle,
	TelSsCliType type,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& __tapi_check_cli_type(type), TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_get_cli_status(handle->ss_proxy,
		type, NULL, on_response_ss_get_cli_status, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_ss_send_ussd_request(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSsResult result = TEL_SS_RESULT_FAILURE;
	TelSsUssdResp ussd_resp = {0, };
	GError *error = NULL;

	dbg("Entry");

	telephony_ss_call_send_ussd_request_finish(handle->ss_proxy,
		(int *)&result, (int *)(&(ussd_resp.type)), (int *)(&(ussd_resp.status)),
		(gchar **)&(ussd_resp.str), res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SS_RESULT_SUCCESS) {
		g_free(ussd_resp.str);
		err("USSD request failed: %d", result);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	RESP_CALLBACK_CALL(rsp_cb_data, result, &ussd_resp);

	g_free(ussd_resp.str);
}

EXPORT_API TelReturn tapi_ss_send_ussd_request(TelHandle *handle,
	const TelSsUssdInfo *ussd_request,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
		&& ussd_request && __tapi_check_ussd_type(ussd_request->type)
		&& __tapi_check_ussd_string(ussd_request->str),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_ss_call_send_ussd_request(handle->ss_proxy,
		ussd_request->type, (const gchar *)ussd_request->str, NULL,
		on_response_ss_send_ussd_request, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}
/*	EOF  */
