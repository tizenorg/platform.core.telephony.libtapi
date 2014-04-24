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

#include <string.h>

#include "tapi_log.h"
#include "tapi_private.h"

#include "tapi_network.h"

static gboolean __tapi_check_plmn(char *plmn)
{
	if (plmn == NULL)
		return FALSE;

	if (strlen(plmn) < 5 || strlen(plmn) > 6)
		return FALSE;

	return TRUE;
}

static gboolean __tapi_check_act(TelNetworkAct act)
{
	switch(act) {
	case TEL_NETWORK_ACT_UNKNOWN:
	case TEL_NETWORK_ACT_GSM:
	case TEL_NETWORK_ACT_GPRS:
	case TEL_NETWORK_ACT_EGPRS:
	case TEL_NETWORK_ACT_UMTS:
	case TEL_NETWORK_ACT_GSM_AND_UMTS:
	case TEL_NETWORK_ACT_HSDPA:
	case TEL_NETWORK_ACT_HSUPA:
	case TEL_NETWORK_ACT_HSPA:
	case TEL_NETWORK_ACT_LTE:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_network_mode(TelNetworkMode mode)
{
	switch(mode) {
	case TEL_NETWORK_MODE_AUTO:
	case TEL_NETWORK_MODE_2G:
	case TEL_NETWORK_MODE_3G:
	case TEL_NETWORK_MODE_LTE:
		return TRUE;
	}

	return FALSE;
}

void on_network_signal_emit_handler(TelephonyNetwork *network,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data)
{
	TapiEvtCbData *evt_cb_data;
	TelHandle *handle = user_data;
	char *evt_id;

	if (handle == NULL || signal_name == NULL)
		return;

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_NETWORK_INTERFACE,
					signal_name);

	/*
	 * If an event callback is registered process
	 * g-signal event
	 */
	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		dbg("Application not registered on event %s",
							evt_id);
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "RegistrationStatus")) {
		TelNetworkRegStatusInfo reg_status;

		g_variant_get(parameters, "(iii)", &reg_status.cs_status,
					&reg_status.ps_status, &reg_status.act);

		dbg("%s cs_status(%d), ps_status(%d), act(%d)", signal_name,
			reg_status.cs_status, reg_status.ps_status, reg_status.act);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &reg_status);
	} else if (!g_strcmp0(signal_name, "CellInfo")) {
		TelNetworkCellInfo cell_info;

		g_variant_get(parameters, "(uuu)", &cell_info.lac,
					&cell_info.cell_id, &cell_info.rac);

		dbg("%s lac(%d), cell_id(%d), rac(%d)", signal_name,
			cell_info.lac, cell_info.cell_id, cell_info.rac);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &cell_info);
	} else if (!g_strcmp0(signal_name, "Identity")) {
		TelNetworkIdentityInfo identity = {NULL, NULL, NULL};

		g_variant_get(parameters, "(sss)", &identity.plmn,
					&identity.short_name, &identity.long_name);

		dbg("%s plmn(%s), short_name(%s), long_name(%s)", signal_name,
			identity.plmn, identity.short_name, identity.long_name);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &identity);
		g_free(identity.plmn);
		g_free(identity.short_name);
		g_free(identity.long_name);
	} else if (!g_strcmp0(signal_name, "Rssi")) {
		unsigned int rssi;
		g_variant_get(parameters, "(u)", &rssi);

		dbg("%s rssi(%u)", signal_name, rssi);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &rssi);
	} else if (!g_strcmp0(signal_name, "TimeInfo")) {
		TelNetworkNitzInfoNoti time_info = {0, 0, 0, 0, 0, 0, 0, FALSE, 0, NULL};

		g_variant_get(parameters, "(uuuuuuibis)", &time_info.year, &time_info.month,
			&time_info.day, &time_info.hour, &time_info.minute, &time_info.second,
			&time_info.gmtoff, &time_info.isdst, &time_info.dstoff, &time_info.plmn);

		dbg("%s year(%u), month(%u), day(%u), hour(%u), minute(%u), second(%u), \
			gmtoff(%d), isdst(%s), dstoff(%d), plmn(%s)", signal_name,
			time_info.year, time_info.month, time_info.day, time_info.hour,
			time_info.minute, time_info.second, time_info.gmtoff,
			time_info.isdst ? "TRUE":"FALSE", time_info.dstoff, time_info.plmn);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &time_info);
		g_free(time_info.plmn);
	} else
		err("Unhandled Signal: %s", signal_name);

	g_free(evt_id);
}

EXPORT_API TelReturn tapi_network_get_registration_info(TelHandle *handle,
	TelNetworkRegistrationInfo *reg_info)
{
	dbg("Entry");

	g_return_val_if_fail(handle != NULL && reg_info != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	reg_info->reg_status_info.cs_status =
			telephony_network_get_cs_status(handle->network_proxy);

	reg_info->reg_status_info.ps_status =
			telephony_network_get_ps_status(handle->network_proxy);

	reg_info->reg_status_info.act =
			telephony_network_get_act(handle->network_proxy);

	reg_info->cell_info.lac =
			telephony_network_get_lac(handle->network_proxy);

	reg_info->cell_info.cell_id =
			telephony_network_get_cell_id(handle->network_proxy);

	reg_info->cell_info.rac =
			telephony_network_get_rac(handle->network_proxy);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_get_identity_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	TelNetworkIdentityInfo id_info;
	GError *error = NULL;

	dbg("Entry");

	memset(&id_info, 0, sizeof(id_info));
	telephony_network_call_get_identity_info_finish(handle->network_proxy,
		(int *)&result, &id_info.plmn, &id_info.short_name,
		&id_info.long_name, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_NETWORK_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &id_info);
	}

	g_free(id_info.plmn);
	g_free(id_info.short_name);
	g_free(id_info.long_name);
}

EXPORT_API TelReturn tapi_network_get_identity_info(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_get_identity_info(handle->network_proxy,
		NULL,
		on_response_network_get_identity_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_search(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *nw_list;
	TelNetworkPlmnList plmn_list;

	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	memset(&plmn_list, 0, sizeof(TelNetworkPlmnList));

	/* Setting back Proxy default timeout to DEFAULT value */
	g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
		TAPI_DBUS_TIMEOUT_DEFAULT);


	telephony_network_call_search_finish(handle->network_proxy,
			(int *)&result, &plmn_list.count, &nw_list, res, &error);
	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_NETWORK_RESULT_SUCCESS) {
		g_variant_unref(nw_list);

		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if ((plmn_list.count != 0)
			&& (g_variant_n_children(nw_list) == plmn_list.count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Network search list count: [%d]", plmn_list.count);
		plmn_list.network_list =
			g_malloc0(sizeof(TelNetworkInfo)*plmn_list.count);

		g_variant_get(nw_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "plmn_status") == 0) {
					plmn_list.network_list[i].plmn_status =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "act") == 0) {
					plmn_list.network_list[i].act =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "network_identity") == 0) {
					GVariantIter *iter2 = NULL;
					GVariant *key_value2;
					const gchar *key2;

					g_variant_get(key_value, "a{sv}", &iter2);
					while (g_variant_iter_loop(iter2, "{sv}", &key2, &key_value2)) {
						if (g_strcmp0(key2, "plmn") == 0) {
							plmn_list.network_list[i].network_identity.plmn =
								(gchar *)g_variant_get_string(key_value2, NULL);
						}
						else if (g_strcmp0(key2, "short_name") == 0) {
							plmn_list.network_list[i].network_identity.short_name =
								(gchar *)g_variant_get_string(key_value2, NULL);
						}
						else if (g_strcmp0(key2, "long_name") == 0) {
							plmn_list.network_list[i].network_identity.long_name =
								(gchar *)g_variant_get_string(key_value2, NULL);
						}
					}
					g_variant_iter_free(iter2);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("Network search list count is NOT valid - Count: [%d]",
			plmn_list.count);
		plmn_list.count = 0;
	}
	g_variant_unref(nw_list);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &plmn_list);

	/* Free resource */
	g_free(plmn_list.network_list);
}

EXPORT_API TelReturn tapi_network_search(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* Setting Proxy default timeout as 180 secs */
	g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
		TAPI_DBUS_TIMEOUT_MAX);

	telephony_network_call_search(handle->network_proxy,
		NULL,
		on_response_network_search, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_cancel_search(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_network_call_cancel_search_finish(handle->network_proxy,
		(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_network_cancel_search(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_cancel_search(handle->network_proxy,
		NULL,
		on_response_network_cancel_search, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_select_automatic(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	/* Setting back Proxy default timeout to DEFAULT value */
	g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
		TAPI_DBUS_TIMEOUT_DEFAULT);

	telephony_network_call_select_automatic_finish(handle->network_proxy,
		(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_network_select_automatic(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* Setting Proxy default timeout as 180 secs */
	g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
		TAPI_DBUS_TIMEOUT_MAX);

	telephony_network_call_select_automatic(handle->network_proxy,
		NULL,
		on_response_network_select_automatic, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_select_manual(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	/* Setting back Proxy default timeout to DEFAULT value */
	g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
		TAPI_DBUS_TIMEOUT_DEFAULT);

	telephony_network_call_select_manual_finish(handle->network_proxy,
		(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_network_select_manual(TelHandle *handle,
	const TelNetworkSelectManualInfo *select_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && select_info != NULL
		&& __tapi_check_plmn(select_info->plmn)
		&& __tapi_check_act(select_info->act) && callback != NULL,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* Setting Proxy default timeout as 180 secs */
	g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
		TAPI_DBUS_TIMEOUT_MAX);

	telephony_network_call_select_manual(handle->network_proxy,
		select_info->plmn, select_info->act, NULL,
		on_response_network_select_manual, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_get_selection_mode(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	TelNetworkSelectionMode mode;
	GError *error = NULL;

	dbg("Entry");

	telephony_network_call_get_selection_mode_finish(handle->network_proxy,
		(int *)&result, (int *)&mode, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_NETWORK_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &mode);
	}
}

EXPORT_API TelReturn tapi_network_get_selection_mode(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_get_selection_mode(handle->network_proxy,
		NULL,
		on_response_network_get_selection_mode, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_set_preferred_plmn(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_network_call_set_preferred_plmn_finish(handle->network_proxy,
		(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_network_set_preferred_plmn(TelHandle *handle,
	TelNetworkPreferredPlmnInfo *info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && info != NULL
		&& __tapi_check_plmn(info->plmn)
		&& __tapi_check_act(info->act)
		&& info->index > 0 && callback != NULL,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_set_preferred_plmn(handle->network_proxy,
		info->index, info->plmn, info->act, NULL,
		on_response_network_set_preferred_plmn, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_get_preferred_plmn(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *pref_plmn_list;
	TelNetworkPreferredPlmnList plmn_list;

	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	memset(&plmn_list, 0, sizeof(TelNetworkPreferredPlmnList));

	telephony_network_call_get_preferred_plmn_finish(handle->network_proxy,
		(int *)&result, &plmn_list.count, &pref_plmn_list, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_NETWORK_RESULT_SUCCESS) {
		g_variant_unref(pref_plmn_list);
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if ((plmn_list.count != 0)
			&& (g_variant_n_children(pref_plmn_list) == plmn_list.count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Preferred PLMN list count: [%d]", plmn_list.count);
		plmn_list.list =
			g_malloc0(sizeof(TelNetworkPreferredPlmnInfo)*plmn_list.count);

		g_variant_get(pref_plmn_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "index") == 0) {
					plmn_list.list[i].index =
						g_variant_get_uint32(key_value);
				}
				else if (g_strcmp0(key, "plmn") == 0) {
					plmn_list.list[i].plmn =
						(gchar *)g_variant_get_string(key_value, NULL);
				}
				else if (g_strcmp0(key, "act") == 0) {
					plmn_list.list[i].act =
						g_variant_get_int32(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("Preferred PLMN list count is NOT valid - Count: [%d]",
			plmn_list.count);
		plmn_list.count = 0;
	}
	g_variant_unref(pref_plmn_list);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &plmn_list);

	/* Free resource */
	g_free(plmn_list.list);
}

EXPORT_API TelReturn tapi_network_get_preferred_plmn(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_get_preferred_plmn(handle->network_proxy,
		NULL,
		on_response_network_get_preferred_plmn, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_set_mode(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_network_call_set_mode_finish(handle->network_proxy,
		(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_network_set_mode(TelHandle *handle,
	TelNetworkMode mode, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL
		&& __tapi_check_network_mode(mode)
		&& callback != NULL,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_set_mode(handle->network_proxy,
		mode, NULL,
		on_response_network_set_mode, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_get_mode(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	TelNetworkMode mode;
	GError *error = NULL;

	dbg("Entry");

	telephony_network_call_get_mode_finish(handle->network_proxy,
		(int *)&result, (int *)&mode, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_NETWORK_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &mode);
	}
}

EXPORT_API TelReturn tapi_network_get_mode(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_get_mode(handle->network_proxy,
		NULL,
		on_response_network_get_mode, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_network_get_neighboring_cell_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelNetworkNeighbourCellInfo cell_info;
	GVariant *gsm_var, *umts_var;


	TelNetworkResult result = TEL_NETWORK_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	memset(&cell_info, 0, sizeof(TelNetworkNeighbourCellInfo));

	telephony_network_call_get_ngbr_cell_info_finish(handle->network_proxy,
		(int *)&result, &cell_info.gsm_list_count, &gsm_var,
		&cell_info.umts_list_count, &umts_var,
		res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_NETWORK_RESULT_SUCCESS) {
		g_variant_unref(gsm_var);
		g_variant_unref(umts_var);

		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	if ((cell_info.gsm_list_count != 0)
			&& (g_variant_n_children(gsm_var) == cell_info.gsm_list_count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("GSM Cell list count: [%d]", cell_info.gsm_list_count);
		cell_info.gsm_list =
			g_malloc0(sizeof(TelNetworkGsmNeighbourCellInfo));

		g_variant_get(gsm_var, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "cell_id") == 0) {
					cell_info.gsm_list->cell_id =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "lac") == 0) {
					cell_info.gsm_list->lac =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "bcch") == 0) {
					cell_info.gsm_list->bcch =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "bsic") == 0) {
					cell_info.gsm_list->bsic =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "rxlev") == 0) {
					cell_info.gsm_list->rxlev =
						g_variant_get_int32(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("GSM Cell list count is NOT valid - Count: [%d]",
			cell_info.gsm_list_count);
		cell_info.gsm_list_count = 0;
	}
	g_variant_unref(gsm_var);

	if ((cell_info.umts_list_count != 0)
			&& (g_variant_n_children(umts_var) == cell_info.umts_list_count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("GSM Cell list count: [%d]", cell_info.umts_list_count);
		cell_info.umts_list =
			g_malloc0(sizeof(TelNetworkUmtsNeighbourCellInfo));

		g_variant_get(umts_var, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "cell_id") == 0) {
					cell_info.umts_list->cell_id =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "lac") == 0) {
					cell_info.umts_list->lac =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "arfcn") == 0) {
					cell_info.umts_list->arfcn =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "psc") == 0) {
					cell_info.umts_list->psc =
						g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "rscp") == 0) {
					cell_info.umts_list->rscp =
						g_variant_get_int32(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("GSM Cell list count is NOT valid - Count: [%d]",
			cell_info.umts_list_count);
		cell_info.umts_list_count = 0;
	}
	g_variant_unref(umts_var);

	RESP_CALLBACK_CALL(rsp_cb_data, result, &cell_info);

	/* Free resource */
	g_free(cell_info.gsm_list);
	g_free(cell_info.umts_list);
}

EXPORT_API TelReturn tapi_network_get_neighboring_cell_info(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	g_return_val_if_fail(handle != NULL && callback != NULL,
			TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_network_call_get_ngbr_cell_info(handle->network_proxy,
		NULL,
		on_response_network_get_neighboring_cell_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}
