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
#include "TelNetwork.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiNetwork.h"

static int _convert_systemtype_to_act(int type)
{
	switch (type) {
		case TAPI_NETWORK_SYSTEM_GSM:
			return 0x1;
			break;

		case TAPI_NETWORK_SYSTEM_GPRS:
			return 0x2;
			break;

		case TAPI_NETWORK_SYSTEM_EGPRS:
			return 0x3;
			break;

		case TAPI_NETWORK_SYSTEM_PCS1900:
			break;

		case TAPI_NETWORK_SYSTEM_UMTS:
			return 0x4;
			break;

		case TAPI_NETWORK_SYSTEM_GSM_AND_UMTS:
			return 0x5;
			break;

		case TAPI_NETWORK_SYSTEM_IS95A:
			return 0x11;
			break;

		case TAPI_NETWORK_SYSTEM_IS95B:
			return 0x12;
			break;

		case TAPI_NETWORK_SYSTEM_CDMA_1X:
			return 0x13;
			break;

		case TAPI_NETWORK_SYSTEM_EVDO_REV_0:
			return 0x14;
			break;

		case TAPI_NETWORK_SYSTEM_1X_EVDO_REV_0_HYBRID:
			return 0x15;
			break;

		case TAPI_NETWORK_SYSTEM_EVDO_REV_A:
			return 0x16;
			break;

		case TAPI_NETWORK_SYSTEM_1X_EVDO_REV_A_HYBRID:
			return 0x17;
			break;

		case TAPI_NETWORK_SYSTEM_EVDV:
			return 0x18;
			break;

		case TAPI_NETWORK_SYSTEM_LTE:
			return 0x21;
			break;

		default:
			break;
	}

	return 0xFF;
}

static int _convert_act_to_systemtype(int act)
{
	switch (act) {
		case 0x1:
			return TAPI_NETWORK_SYSTEM_GSM;
			break;

		case 0x2:
			return TAPI_NETWORK_SYSTEM_GPRS;
			break;

		case 0x3:
			return TAPI_NETWORK_SYSTEM_EGPRS;
			break;

		case 0x4:
			return TAPI_NETWORK_SYSTEM_UMTS;
			break;

		case 0x5:
			return TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;
			break;

		case 0x11:
			return TAPI_NETWORK_SYSTEM_IS95A;
			break;

		case 0x12:
			return TAPI_NETWORK_SYSTEM_IS95B;
			break;

		case 0x13:
			return TAPI_NETWORK_SYSTEM_CDMA_1X;
			break;

		case 0x14:
			return TAPI_NETWORK_SYSTEM_EVDO_REV_0;
			break;

		case 0x15:
			return TAPI_NETWORK_SYSTEM_1X_EVDO_REV_0_HYBRID;
			break;

		case 0x16:
			return TAPI_NETWORK_SYSTEM_EVDO_REV_A;
			break;

		case 0x17:
			return TAPI_NETWORK_SYSTEM_1X_EVDO_REV_A_HYBRID;
			break;

		case 0x18:
			return TAPI_NETWORK_SYSTEM_EVDV;
			break;

		case 0x21:
			return TAPI_NETWORK_SYSTEM_LTE;
			break;

		default:
			break;
	}

	return TAPI_NETWORK_SYSTEM_NO_SRV;
}

static void on_response_search_network(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int i;

	TelNetworkPlmnList_t list;

	GVariant *dbus_result = NULL;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;

	memset(&list, 0, sizeof(TelNetworkPlmnList_t));

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

	g_variant_get (dbus_result, "(aa{sv}i)", &iter, &result);

	list.networks_count = g_variant_iter_n_children(iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "plmn")) {
				list.network_list[i].plmn_id = atoi(g_variant_get_string(value, NULL));
				strncpy(list.network_list[i].plmn, g_variant_get_string(value, NULL), TAPI_NETWORK_PLMN_LEN_MAX);
			}
			if (!g_strcmp0(key, "act")) {
				list.network_list[i].access_technology = _convert_act_to_systemtype(g_variant_get_int32(value));
			}
			if (!g_strcmp0(key, "type")) {
				list.network_list[i].type_of_plmn = g_variant_get_int32(value);
			}
			if (!g_strcmp0(key, "name")) {
				strncpy(list.network_list[i].network_name, g_variant_get_string(value, NULL), 40);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_network_selection_mode(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	int mode = 0;

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

	g_variant_get (dbus_result, "(ii)", &mode, &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &mode, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

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

static void on_response_get_network_preferred_plmn(GDBusConnection *conn, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int i;

	TelNetworkPreferredPlmnList_t list;

	GVariant *dbus_result = NULL;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;

	memset(&list, 0, sizeof(TelNetworkPreferredPlmnList_t));

	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if (!dbus_result) {
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, -1, &list, evt_cb_data->user_data);
		}

		if (error)
			g_error_free(error);

		free(evt_cb_data);
		return;
	}

	g_variant_get (dbus_result, "(aa{sv}i)", &iter, &result);

	list.NumOfPrefPlmns = g_variant_iter_n_children(iter);

	if (list.NumOfPrefPlmns == 0) {
		dbg("num_of_.. = 0");
		if (evt_cb_data->cb_fn) {
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
		}

		free(evt_cb_data);
		return;
	}

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "plmn")) {
				strncpy(list.PrefPlmnRecord[i].Plmn, g_variant_get_string(value, NULL), 6);
			}
			if (!g_strcmp0(key, "act")) {
				list.PrefPlmnRecord[i].SystemType = _convert_act_to_systemtype(g_variant_get_int32(value));
			}
			if (!g_strcmp0(key, "index")) {
				list.PrefPlmnRecord[i].Index = g_variant_get_int32(value);
			}
			if (!g_strcmp0(key, "name")) {
				strncpy(list.PrefPlmnRecord[i].network_name, g_variant_get_string(value, NULL), 40);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_network_band(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	int band = 0;
	int mode = 0;

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

	g_variant_get (dbus_result, "(iii)", &band, &mode, &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &band, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_network_mode(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	int mode = 0;

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

	g_variant_get (dbus_result, "(ii)", &mode, &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &mode, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_network_service_domain(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	int domain = 0;

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

	g_variant_get (dbus_result, "(ii)", &domain, &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &domain, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_network_serving(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	TelNetworkServing_t data;

	GVariant *dbus_result;
	char *plmn;

	memset(&data, 0, sizeof(TelNetworkServing_t));

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

	g_variant_get (dbus_result, "(isii)",
			&data.act,
			&plmn,
			&data.lac,
			&result);

	data.act = _convert_act_to_systemtype(data.act);

	if (plmn) {
		snprintf(data.plmn, 7, "%s", plmn);
		free(plmn);
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &data, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_neighboring_cell_info(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int geran_index=0, umts_index=0;

	TelNetworkNeighboringCellInfo_t list;

	GVariant *dbus_result = NULL;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	gint v0, v1,v2,v3,v4;

	memset(&list, 0, sizeof(TelNetworkNeighboringCellInfo_t));

	dbg("enter");
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

	g_variant_get (dbus_result, "(aa{sv}i)", &iter, &result);

	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			g_variant_get(value, "(iiiii)", &v0,&v1,&v2,&v3,&v4);
			if (!g_strcmp0(key, "geran")) {
				list.geran_list[geran_index].cell_id	= v0;
				list.geran_list[geran_index].lac		= v1;
				list.geran_list[geran_index].bcch		= v2;
				list.geran_list[geran_index].bsic		= v3;
				list.geran_list[geran_index].rxlev		= v4;
				geran_index++;
			}
			else if (!g_strcmp0(key, "umts")) {
				list.umts_list[umts_index].cell_id	= v0;
				list.umts_list[umts_index].lac		= v1;
				list.umts_list[umts_index].arfcn	= v2;
				list.umts_list[umts_index].psc		= v3;
				list.umts_list[umts_index].rscp		= v4;
				umts_index++;
			}
		}
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	list.geran_list_count = geran_index;
	list.umts_list_count = umts_index;
	dbg("geran_list_count=%d, umts_list_count=%d", geran_index, umts_index);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_search_network(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"Search", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, 180000, handle->ca,
			on_response_search_network, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_selection_mode(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetSelectionMode", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_network_selection_mode, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_select_network_automatic(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(isi)",
			0, /* Automatic */
			"",
			0);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetSelectionMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, 180000, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_select_network_manual(struct tapi_handle *handle, const char *plmn, int type, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(isi)",
			1, /* Manual */
			plmn,
			_convert_systemtype_to_act(type));

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetSelectionMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, 180000, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_preferred_plmn(
		TapiHandle *handle,
		TelNetworkPreferredPlmnOp_t operation,
		TelNetworkPreferredPlmnInfo_t *info,
		tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;
	int act = 0;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	switch (info->SystemType) {
		case TAPI_NETWORK_SYSTEM_GSM:
			act = 1;
			break;

		case TAPI_NETWORK_SYSTEM_UMTS:
			act = 4;
			break;

		case TAPI_NETWORK_SYSTEM_GPRS:
			act = 2;
			break;

		case TAPI_NETWORK_SYSTEM_EGPRS:
			act = 3;
			break;

		case TAPI_NETWORK_SYSTEM_GSM_AND_UMTS:
			act = 4;
			break;

		default:
			act = 4;
			break;
	}

	param = g_variant_new("(iiis)",
			operation,
			info->Index,
			act,
			info->Plmn);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetPreferredPlmn", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_preferred_plmn(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetPreferredPlmn", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			(GAsyncReadyCallback) on_response_get_network_preferred_plmn, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_band(TapiHandle *handle, TelNetworkBandPreferred_t mode, TelNetworkBand_t band, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)",
			band,
			mode);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetBand", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_band(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetBand", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_network_band, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_mode(TapiHandle *handle, int mode, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)",
			mode);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetMode", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_network_mode, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_service_domain(TapiHandle *handle, TelNetworkServiceDomain_t domain, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", domain);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetServiceDomain", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_service_domain(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetServiceDomain", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_network_service_domain, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_cancel_network_manual_search(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SearchCancel", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_serving(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetServingNetwork", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_network_serving, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_neighboring_cell_info(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetNgbrCellInfo", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, handle->ca,
			on_response_get_neighboring_cell_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

