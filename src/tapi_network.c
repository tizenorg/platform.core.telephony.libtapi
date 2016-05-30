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
#include "TelNetwork.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiNetwork.h"

static int _convert_systemtype_to_act(int type)
{
	switch (type) {
	case TAPI_NETWORK_SYSTEM_GSM:
		return 0x1;

	case TAPI_NETWORK_SYSTEM_GPRS:
		return 0x2;

	case TAPI_NETWORK_SYSTEM_EGPRS:
		return 0x3;

	case TAPI_NETWORK_SYSTEM_PCS1900:
		break;

	case TAPI_NETWORK_SYSTEM_UMTS:
		return 0x4;

	case TAPI_NETWORK_SYSTEM_GSM_AND_UMTS:
		return 0x5;

	case TAPI_NETWORK_SYSTEM_IS95A:
		return 0x11;

	case TAPI_NETWORK_SYSTEM_IS95B:
		return 0x12;

	case TAPI_NETWORK_SYSTEM_CDMA_1X:
		return 0x13;

	case TAPI_NETWORK_SYSTEM_EVDO_REV_0:
		return 0x14;

	case TAPI_NETWORK_SYSTEM_1X_EVDO_REV_0_HYBRID:
		return 0x15;

	case TAPI_NETWORK_SYSTEM_EVDO_REV_A:
		return 0x16;

	case TAPI_NETWORK_SYSTEM_1X_EVDO_REV_A_HYBRID:
		return 0x17;

	case TAPI_NETWORK_SYSTEM_EVDO_REV_B:
		return 0x18;

	case TAPI_NETWORK_SYSTEM_1X_EVDO_REV_B_HYBRID:
		return 0x19;

	case TAPI_NETWORK_SYSTEM_EVDV:
		return 0x1A;

	case TAPI_NETWORK_SYSTEM_EHRPD:
		return 0x1B;

	case TAPI_NETWORK_SYSTEM_LTE:
		return 0x21;

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

	case 0x2:
		return TAPI_NETWORK_SYSTEM_GPRS;

	case 0x3:
		return TAPI_NETWORK_SYSTEM_EGPRS;

	case 0x4:
		return TAPI_NETWORK_SYSTEM_UMTS;

	case 0x5:
		return TAPI_NETWORK_SYSTEM_GSM_AND_UMTS;

	case 0x11:
		return TAPI_NETWORK_SYSTEM_IS95A;

	case 0x12:
		return TAPI_NETWORK_SYSTEM_IS95B;

	case 0x13:
		return TAPI_NETWORK_SYSTEM_CDMA_1X;

	case 0x14:
		return TAPI_NETWORK_SYSTEM_EVDO_REV_0;

	case 0x15:
		return TAPI_NETWORK_SYSTEM_1X_EVDO_REV_0_HYBRID;

	case 0x16:
		return TAPI_NETWORK_SYSTEM_EVDO_REV_A;

	case 0x17:
		return TAPI_NETWORK_SYSTEM_1X_EVDO_REV_A_HYBRID;

	case 0x18:
		return TAPI_NETWORK_SYSTEM_EVDO_REV_B;

	case 0x19:
		return TAPI_NETWORK_SYSTEM_1X_EVDO_REV_B_HYBRID;

	case 0x1A:
		return TAPI_NETWORK_SYSTEM_EVDV;

	case 0x1B:
		return TAPI_NETWORK_SYSTEM_EHRPD;

	case 0x21:
		return TAPI_NETWORK_SYSTEM_LTE;

	default:
		break;
	}

	return TAPI_NETWORK_SYSTEM_NO_SRV;
}

/* LCOV_EXCL_START */
static void on_signal_registration_status(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	TelNetworkRegistrationStatus_t noti;

	g_variant_get(param, "(iiib)", &noti.cs, &noti.ps, &noti.type, &noti.is_roaming);

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_strength(TapiHandle *handle, GVariant *param,
		char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_signal_strength noti;

	g_variant_get(param, "(i)", &noti.dbm);

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_cell_info(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_cell_info noti;

	memset(&noti, 0, sizeof(struct tel_noti_network_cell_info));
	g_variant_get(param, "(ii)", &noti.lac, &noti.cell_id);

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_change(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_change noti;
	char *plmn = NULL;
	int act;
	memset(&noti, 0, sizeof(struct tel_noti_network_change));

	g_variant_get(param, "(is)", &act, &plmn);

	noti.act = _convert_act_to_systemtype(act);

	if (plmn) {
		snprintf(noti.plmn, 7, "%s", plmn);
		g_free(plmn);
	}

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_time_info(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_time_info noti;
	char *plmn = NULL;

	memset(&noti, 0, sizeof(struct tel_noti_network_time_info));
	g_variant_get(param, "(iiiiiiiiibs)", &noti.year, &noti.month, &noti.day,
			&noti.hour, &noti.minute, &noti.second,
			&noti.wday, &noti.gmtoff, &noti.dstoff, &noti.isdst,
			&plmn);

	if (plmn) {
		snprintf(noti.plmn, 7, "%s", plmn);
		g_free(plmn);
	}

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_identity(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_identity noti;
	char *plmn = NULL, *s_name = NULL, *f_name = NULL;

	memset(&noti, 0, sizeof(struct tel_noti_network_identity));

	g_variant_get(param, "(sss)", &plmn, &s_name, &f_name);

	if (plmn) {
		snprintf(noti.plmn, 7, "%s", plmn);
		g_free(plmn);
	}
	if (s_name) {
		snprintf(noti.short_name, 17, "%s", s_name);
		g_free(s_name);
	}
	if (f_name) {
		snprintf(noti.full_name, 33, "%s", f_name);
		g_free(f_name);
	}

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_emergency_callback_mode(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_emergency_callback_mode noti;

	memset(&noti, 0, sizeof(struct tel_noti_network_emergency_callback_mode));

	g_variant_get(param, "(i)", &noti.mode);

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_default_data_subscription(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_default_data_subs noti;

	memset(&noti, 0, sizeof(struct tel_noti_network_default_data_subs));

	g_variant_get(param, "(i)", &noti.default_subs);

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_default_subscription(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	struct tel_noti_network_default_subs noti;

	memset(&noti, 0, sizeof(struct tel_noti_network_default_subs));

	g_variant_get(param, "(i)", &noti.default_subs);

	TAPI_INVOKE_NOTI_CALLBACK(&noti);
}

static void on_signal_cell_id(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int cell_id;
	g_variant_get(param, "(i)", &cell_id);
	TAPI_INVOKE_NOTI_CALLBACK(&cell_id);
}

static void on_signal_lac(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int lac;
	g_variant_get(param, "(i)", &lac);
	TAPI_INVOKE_NOTI_CALLBACK(&lac);
}

static void on_signal_tac(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int tac;
	g_variant_get(param, "(i)", &tac);
	TAPI_INVOKE_NOTI_CALLBACK(&tac);
}

static void on_signal_system_id(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int system_id;
	g_variant_get(param, "(i)", &system_id);
	TAPI_INVOKE_NOTI_CALLBACK(&system_id);
}

static void on_signal_network_id(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int network_id;
	g_variant_get(param, "(i)", &network_id);
	TAPI_INVOKE_NOTI_CALLBACK(&network_id);
}

static void on_signal_bs_id(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int bs_id;
	g_variant_get(param, "(i)", &bs_id);
	TAPI_INVOKE_NOTI_CALLBACK(&bs_id);
}

static void on_signal_bs_latitude(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int bs_latitude;
	g_variant_get(param, "(i)", &bs_latitude);
	TAPI_INVOKE_NOTI_CALLBACK(&bs_latitude);
}

static void on_signal_bs_longitude(TapiHandle *handle, GVariant *param,
	char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	int bs_longitude;
	g_variant_get(param, "(i)", &bs_longitude);
	TAPI_INVOKE_NOTI_CALLBACK(&bs_longitude);
}
/* LCOV_EXCL_STOP */

static struct signal_map signals[] = {
	{ "RegistrationStatus", on_signal_registration_status },
	{ "SignalStrength", on_signal_strength },
	{ "CellInfo", on_signal_cell_info },
	{ "Change", on_signal_change },
	{ "TimeInfo", on_signal_time_info },
	{ "Identity", on_signal_identity },
	{ "EmergencyCallbackMode", on_emergency_callback_mode },
	{ "DefaultDataSubscription", on_signal_default_data_subscription },
	{ "DefaultSubscription", on_signal_default_subscription },
	{ "CellId", on_signal_cell_id },
	{ "Lac", on_signal_lac },
	{ "Tac", on_signal_tac },
	{ "SystemId", on_signal_system_id },
	{ "NetworkId", on_signal_network_id },
	{ "BsId", on_signal_bs_id },
	{ "BsLatitude", on_signal_bs_latitude },
	{ "BsLongitude", on_signal_bs_longitude },
};

void _process_network_event(const gchar *sig, GVariant *param,
	TapiHandle *handle, char *noti_id, struct tapi_evt_cb *evt_cb_data)
{
	unsigned int i;
	TAPI_RETURN_IF_FAIL(evt_cb_data);

	for (i = 0; i < sizeof(signals) / sizeof(struct signal_map); i++) {
		dbg("Received (%s)(%s) signal from telephony", handle->cp_name, sig);
		if (!g_strcmp0(sig, signals[i].signal_name)) {
			signals[i].callback(handle, param, noti_id, evt_cb_data);
			return;
		}
	}

	dbg("not handled NETWORK noti[%s]", sig);
}

static gboolean _check_plmn(const char *plmn)
{
	unsigned int plmn_len = 0;

	if (plmn == NULL) {
		err("PLMN is NULL");
		return FALSE;
	}

	plmn_len = strlen(plmn);
	if (plmn_len < 5 || plmn_len > 6) {
		err("PLMN length(%d) is invalid", plmn_len);
		return FALSE;
	}

	return TRUE;
}

static gboolean _check_operation(TelNetworkPreferredPlmnOp_t operation)
{
	switch (operation) {
	case TAPI_NETWORK_PREF_PLMN_ADD:
	case TAPI_NETWORK_PREF_PLMN_EDIT:
	case TAPI_NETWORK_PREF_PLMN_DELETE:
		return TRUE;
	default:
		/*Do Nothing*/
		err("Default Case executed. Unknown PLMN Op");
		break;
	}
	err("operation %d is not supported", operation);
	return FALSE;
}

static gboolean _check_network_mode(int mode)
{
	if (mode == TAPI_NETWORK_MODE_AUTO) {
		return TRUE;
	} else {
		if (mode & TAPI_NETWORK_MODE_GSM)
			mode &= ~TAPI_NETWORK_MODE_GSM;

		if (mode & TAPI_NETWORK_MODE_WCDMA)
			mode &= ~TAPI_NETWORK_MODE_WCDMA;

		if (mode & TAPI_NETWORK_MODE_1XRTT)
			mode &= ~TAPI_NETWORK_MODE_1XRTT;

		if (mode & TAPI_NETWORK_MODE_LTE)
			mode &= ~TAPI_NETWORK_MODE_LTE;

		if (mode & TAPI_NETWORK_MODE_EVDO)
			mode &= ~TAPI_NETWORK_MODE_EVDO;

		if (mode == 0)
			return TRUE;
	}

	err("mode %d is not supported", mode);
	return FALSE;
}

static gboolean _check_emergency_callback_mode(TelNetworkEmergencyCallbackMode_t mode)
{
	switch (mode) {
	case TAPI_NETWORK_EMERGENCY_CALLBACK_MODE_ENTER:
	case TAPI_NETWORK_EMERGENCY_CALLBACK_MODE_EXIT:
		return TRUE;

	default:
		/*Do Nothing*/
		err("Default Case executed. Unknown Emergency callback mode");
		break;
	}
	err("emergency callback mode %d is not supported", mode);
	return FALSE;
}

static gboolean _check_roaming_preference(TelNetworkPrefNetType_t roam_pref)
{
	switch (roam_pref) {
	case TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY:
	case TAPI_NETWORK_PREF_NET_TYPE_AFFILIATED:
	case TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC:
	case TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A:
	case TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_B:
	case TAPI_NETWORK_PREF_NET_TYPE_ROAM_DOMESTIC:
	case TAPI_NETWORK_PREF_NET_TYPE_ROAM_INTERNATIONAL:
	case TAPI_NETWORK_PREF_NET_TYPE_ROAM_DUAL:
	case TAPI_NETWORK_PREF_NET_TYPE_BLANK:
		return TRUE;

	default:
		/*Do Nothing*/
		err("Default Case executed. Unknown roaming preference");
		break;
	}

	err("roam_pref %d is not supported", roam_pref);
	return FALSE;
}

static void on_response_search_network(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
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

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(aa{sv}i)", &iter, &result);

	list.networks_count = g_variant_iter_n_children(iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "plmn")) {
				strncpy(list.network_list[i].plmn, g_variant_get_string(value, NULL), TAPI_NETWORK_PLMN_LEN_MAX);
				list.network_list[i].plmn_id = atoi(g_variant_get_string(value, NULL));
			} else if (!g_strcmp0(key, "act")) {
				list.network_list[i].access_technology = _convert_act_to_systemtype(g_variant_get_int32(value));
			} else if (!g_strcmp0(key, "type")) {
				list.network_list[i].type_of_plmn = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "name")) {
				strncpy(list.network_list[i].network_name, g_variant_get_string(value, NULL), 40);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &list);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_network_selection_mode(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	int mode = 0;

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(ii)", &mode, &result);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &mode);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_default_set(GObject *source_object, GAsyncResult *res,
		gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)", &result);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, NULL);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_network_preferred_plmn(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
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

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(aa{sv}i)", &iter, &result);

	list.NumOfPrefPlmns = g_variant_iter_n_children(iter);

	if (list.NumOfPrefPlmns == 0) {
		dbg("num_of_.. = 0");
		if (evt_cb_data->cb_fn)
			evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);

		g_free(evt_cb_data);
		return;
	}

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "plmn"))
				strncpy(list.PrefPlmnRecord[i].Plmn, g_variant_get_string(value, NULL), 6);
			else if (!g_strcmp0(key, "act"))
				list.PrefPlmnRecord[i].SystemType = _convert_act_to_systemtype(g_variant_get_int32(value));
			else if (!g_strcmp0(key, "index"))
				list.PrefPlmnRecord[i].Index = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "name"))
				strncpy(list.PrefPlmnRecord[i].network_name, g_variant_get_string(value, NULL), 40);
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &list);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_network_mode(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	int mode = 0;

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(ii)", &mode, &result);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &mode);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_network_serving(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	TelNetworkServing_t data;

	GVariantIter *iter;
	GVariant *value = NULL;
	const gchar *key = NULL;
	GVariant *dbus_result;
	char *plmn;
	int v0, v4, v5;
	char v1, v2, v3, v6, v7;

	memset(&data, 0, sizeof(TelNetworkServing_t));

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(a{sv}i)", &iter, &result);

	while (g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		if (!g_strcmp0(key, "serving")) {
			g_variant_get(value, "(is)", &v0, &plmn);
			data.act = _convert_act_to_systemtype(v0);
			if (plmn) {
				snprintf(data.plmn, 7, "%s", plmn);
				g_free(plmn);
			}
		} else if (!g_strcmp0(key, "g_serving")) {
			g_variant_get(value, "(i)", &v0);
			data.info.lac = v0;
		} else if (!g_strcmp0(key, "c_serving")) {
			g_variant_get(value, "(iuuuiiuu)", &v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7);
			data.info.cdma_info.carrier = v0;
			data.info.cdma_info.system_id = v1;
			data.info.cdma_info.network_id = v2;
			data.info.cdma_info.base_station_id = v3;
			data.info.cdma_info.base_station_latitude = v4;
			data.info.cdma_info.base_station_longitude = v5;
			data.info.cdma_info.registration_zone = v6;
			data.info.cdma_info.pilot_offset = v7;
		}
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_neighboring_cell_info(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	int geran_index = 0, umts_index = 0;

	TelNetworkNeighboringCellInfo_t list;

	GVariant *dbus_result = NULL;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	gint v0, v1, v2, v3, v4, v5;

	memset(&list, 0, sizeof(TelNetworkNeighboringCellInfo_t));

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(aa{sv}i)", &iter, &result);

	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "serving")) {
				g_variant_get(value, "(iii)", &v0, &v1, &v2);
				list.serving.act = _convert_act_to_systemtype(v0);
				list.serving.mcc = v1;
				list.serving.mnc = v2;
			} else if (!g_strcmp0(key, "g_serving")) {
				g_variant_get(value, "(iiiii)", &v0, &v1, &v2, &v3, &v4);
				list.serving.cell.geran.cell_id = v0;
				list.serving.cell.geran.lac = v1;
				list.serving.cell.geran.bcch = v2;
				list.serving.cell.geran.bsic = v3;
				list.serving.cell.geran.rxlev = v4;
			} else if (!g_strcmp0(key, "u_serving")) {
				g_variant_get(value, "(iiiii)", &v0, &v1, &v2, &v3, &v4);
				list.serving.cell.umts.cell_id = v0;
				list.serving.cell.umts.lac = v1;
				list.serving.cell.umts.arfcn = v2;
				list.serving.cell.umts.psc = v3;
				list.serving.cell.umts.rscp = v4;
			} else if (!g_strcmp0(key, "l_serving")) {
				g_variant_get(value, "(iiiii)", &v0, &v1, &v2, &v3, &v4);
				list.serving.cell.lte.cell_id = v0;
				list.serving.cell.lte.lac = v1;
				list.serving.cell.lte.earfcn = v2;
				list.serving.cell.lte.tac = v3;
				list.serving.cell.lte.rssi = v4;
			} else if (!g_strcmp0(key, "c_serving")) {
				g_variant_get(value, "(uuuuii)", &v0, &v1, &v2, &v3, &v4, &v5);
				list.serving.cell.cdma.system_id = v0;
				list.serving.cell.cdma.network_id = v1;
				list.serving.cell.cdma.base_station_id = v2;
				list.serving.cell.cdma.reference_pn = v3;
				list.serving.cell.cdma.base_station_latitude = v4;
				list.serving.cell.cdma.base_station_longitude = v5;
			} else if (!g_strcmp0(key, "geran")) {
				g_variant_get(value, "(iiiii)", &v0, &v1, &v2, &v3, &v4);
				list.geran_list[geran_index].cell_id = v0;
				list.geran_list[geran_index].lac = v1;
				list.geran_list[geran_index].bcch = v2;
				list.geran_list[geran_index].bsic = v3;
				list.geran_list[geran_index].rxlev = v4;
				geran_index++;
			} else if (!g_strcmp0(key, "umts")) {
				g_variant_get(value, "(iiiii)", &v0, &v1, &v2, &v3, &v4);
				list.umts_list[umts_index].cell_id = v0;
				list.umts_list[umts_index].lac	 = v1;
				list.umts_list[umts_index].arfcn = v2;
				list.umts_list[umts_index].psc = v3;
				list.umts_list[umts_index].rscp = v4;
				umts_index++;
			}
		}
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	list.geran_list_count = geran_index;
	list.umts_list_count = umts_index;
	dbg("act=%d, count(geran:%d, umts:%d)", list.serving.act, geran_index, umts_index);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &list);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_select_network(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;
	GVariant *dbus_result;

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);
	g_variant_get(dbus_result, "(i)", &result);

	/* Map result received from libtcore to TapiResult_t */
	if (result == 0) {
		result = TAPI_API_SUCCESS;
	} else {
		switch (result) {
		case 0x50000001:
			result = TAPI_API_NETWORK_PLMN_NOT_ALLOWED;
		break;

		case 0x50000002:
			result = TAPI_API_NETWORK_ROAMING_NOT_ALLOWED;
		break;

		default:
			/*Do Nothing*/
		break;
		}
	}

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, NULL);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_network_roaming_preference(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	int result = -1;

	GVariant *dbus_result;
	TelNetworkPrefNetType_t roam_pref;

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(ii)", &roam_pref, &result);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &roam_pref);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

EXPORT_API int tel_search_network(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	msg("[%s] network_search requested", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"Search", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_UNRESTRICTED_TIMEOUT, handle->ca,
			on_response_search_network, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_selection_mode(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetSelectionMode", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_network_selection_mode, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_select_network_automatic(struct tapi_handle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(isi)",
			0, /* Automatic */
			"",
			0);

	msg("[%s] automatic selection requested", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetSelectionMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_UNRESTRICTED_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_select_network_manual(struct tapi_handle *handle, const char *plmn, int type, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && _check_plmn(plmn), TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(isi)",
			1, /* Manual */
			plmn,
			_convert_systemtype_to_act(type));

	msg("[%s] manual selection requested plmn:[%s] type:[%d]", handle->cp_name, plmn ? plmn : "", type);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetSelectionMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_UNRESTRICTED_TIMEOUT, handle->ca,
			on_response_select_network, evt_cb_data);

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

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && info != NULL &&
							_check_operation(operation) &&
							_check_plmn((const char *)&info->Plmn), TAPI_API_INVALID_PTR);

	dbg("Func Entrance");

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

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
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_preferred_plmn(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetPreferredPlmn", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_network_preferred_plmn, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_mode(TapiHandle *handle, int mode, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL &&
								_check_network_mode(mode), TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)",
			mode);

	msg("[%s] requested mode:[0x%x]", handle->cp_name, mode);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_UNRESTRICTED_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("[%s] Func Entrance", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetMode", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_network_mode, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_cancel_network_manual_search(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	msg("[%s] network search cancel requested", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SearchCancel", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_serving(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("[%s] Func Entrance", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetServingNetwork", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_network_serving, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_neighboring_cell_info(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("[%s] Func Entrance", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetNgbrCellInfo", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_neighboring_cell_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_default_data_subscription(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	msg("[%s] Func Entrance", handle->cp_name);

	/* DBUS call */
	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetDefaultDataSubscription", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_emergency_callback_mode(TapiHandle *handle, TelNetworkEmergencyCallbackMode_t mode, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL &&
								_check_emergency_callback_mode(mode), TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", mode);

	msg("[%s] emergency callback mode :[%d]", handle->cp_name, mode);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetEmergencyCallbackMode", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_network_roaming_preference(TapiHandle *handle, TelNetworkPrefNetType_t roam_pref, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL &&
								_check_roaming_preference(roam_pref), TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", roam_pref);

	msg("[%s] roam_pref:[%d]", handle->cp_name, roam_pref);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetRoamingPreference", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;

}

EXPORT_API int tel_get_network_roaming_preference(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle != NULL && callback != NULL, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("[%s] Func Entrance", handle->cp_name);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetRoamingPreference", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_network_roaming_preference, evt_cb_data);

	return TAPI_API_SUCCESS;

}

EXPORT_API int tel_get_network_default_data_subscription(TapiHandle *handle, TelNetworkDefaultDataSubs_t *default_subscription)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int subs = 0;
	int result = 0;
	TapiResult_t ret = TAPI_API_OPERATION_FAILED;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(default_subscription, TAPI_API_INVALID_PTR);

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetDefaultDataSubscription", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			&gerr);
	if (sync_gv) {
		g_variant_get(sync_gv, "(ii)", &subs, &result);

		if (result == 0)
			*default_subscription = subs;
		else
			*default_subscription = TAPI_NETWORK_DEFAULT_DATA_SUBS_UNKNOWN;

		msg("'default' Data Subscription: [%d]", subs);

		g_variant_unref(sync_gv);
		ret = TAPI_API_SUCCESS;
	} else {
		err("Get Data Subscription failed: [%s]", gerr->message);
		if (strstr(gerr->message, "AccessDenied"))
			ret = TAPI_API_ACCESS_DENIED;
		g_error_free(gerr);
	}

	return ret;
}

EXPORT_API int tel_set_network_default_subscription(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(callback, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("[%s] Set 'default' Subscription (for CS)", handle->cp_name);

	/* DBUS call */
	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"SetDefaultSubscription", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_default_set, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_network_default_subscription(TapiHandle *handle, TelNetworkDefaultSubs_t *default_subscription)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int subs = 0;
	int result = 0;
	TapiResult_t ret = TAPI_API_OPERATION_FAILED;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(default_subscription, TAPI_API_INVALID_PTR);

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, handle->path, DBUS_TELEPHONY_NETWORK_INTERFACE,
			"GetDefaultSubscription", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			&gerr);
	if (sync_gv) {
		g_variant_get(sync_gv, "(ii)", &subs, &result);

		if (result == 0)
			*default_subscription = subs;
		else
			*default_subscription = TAPI_NETWORK_DEFAULT_SUBS_UNKNOWN;
		dbg("'default' Subscription (for CS): [%d]", subs);

		g_variant_unref(sync_gv);
		ret = TAPI_API_SUCCESS;
	} else {
		err("Get 'default' Subscription (for CS) failed: [%s]", gerr->message);
		if (strstr(gerr->message, "AccessDenied"))
			ret = TAPI_API_ACCESS_DENIED;
		g_error_free(gerr);
	}

	return ret;
}
