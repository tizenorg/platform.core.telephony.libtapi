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
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>

#include <tapi_common.h>
#include <ITapiNetwork.h>
#include <TapiUtility.h>

#include "menu.h"
#include "network.h"

static char data_net_set_plmn_mode_manual_act[MENU_DATA_SIZE + 1] = "4";
static char data_net_set_plmn_mode_manual_plmn[MENU_DATA_SIZE + 1] = "45001";
static char data_net_set_preferred_plmn_mode[MENU_DATA_SIZE + 1] = "1";
static char data_net_set_preferred_plmn_plmn[MENU_DATA_SIZE + 1] = "45001";
static char data_net_set_preferred_plmn_type[MENU_DATA_SIZE + 1] = "4";
static char data_net_set_preferred_plmn_ef_index[MENU_DATA_SIZE + 1] = "0";

static char data_net_set_mode_mode[MENU_DATA_SIZE + 1] = "8";
static char data_net_set_emergency_callback_mode_mode[MENU_DATA_SIZE + 1] = "1";
static char data_net_set_roaming_preference_roam_pref[MENU_DATA_SIZE + 1] = "1";

static void on_prop_signal_dbm(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *sig_dbm = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_SIGNALSTRENGTH_DBM);

	msg(" - sig_dbm = %d", *sig_dbm);
}

static void on_prop_signal_level(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *sig_level = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_SIGNALSTRENGTH_LEVEL);

	msg(" - sig_level = %d", *sig_level);
}

static void on_prop_lac(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *lac = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_LAC);

	msg(" - lac = %d", *lac);
}

static void on_prop_tac(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *tac = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_TAC);

	msg(" - tac = %d", *tac);
}

static void on_prop_plmn(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_PLMN);

	msg(" - plmn = %s", (char *)data);
}

static void on_prop_cellid(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *cell_id = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_CELLID);

	msg(" - cell_id = %d", *cell_id);
}

static void on_prop_service_type(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *service_type = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_SERVICE_TYPE);

	msg(" - service_type = %d", *service_type);
}

static void on_prop_act(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *act = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_ACT);

	msg(" - access technology = %d", *act);
}

static void on_prop_ps_type(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *ps_type = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_PS_TYPE);

	msg(" - ps_type = %d", *ps_type);
}

static void on_prop_circuit_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *circuit_status = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_CIRCUIT_STATUS);

	msg(" - circuit_status = %d", *circuit_status);
}

static void on_prop_packet_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *packet_status = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_PACKET_STATUS);

	msg(" - packet_status = %d", *packet_status);
}

static void on_prop_roaming_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	guchar *roaming_status = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_ROAMING_STATUS);

	msg(" - roaming_status = %d", *roaming_status);
}

static void on_prop_name_option(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *option = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_NAME_OPTION);

	msg(" - name_option = %d", *option);
}

static void on_prop_network_name(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_NETWORK_NAME);

	msg(" - network_name = %s", (char *)data);
}

static void on_prop_spn_name(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_SPN_NAME);

	msg(" - spn_name = %s", (char *)data);
}

static void on_prop_ims_voice_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *status = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_IMS_VOICE_SUPPORT_STATUS);

	if (status != NULL)
		msg(" - ims_voice_support_status = %d", *status);
}

static void on_noti_volte_enable(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *status = data;

	msg("");
	msgp("property(%s) receive !!", TAPI_PROP_NETWORK_VOLTE_ENABLE);

	if (status != NULL)
		msg(" - volte_enable = %d", *status);
}

static void on_noti_registration_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelNetworkRegistrationStatus_t *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_REGISTRATION_STATUS);

	msg(" - circuit status = %d", noti->cs);
	msg(" - packet status = %d", noti->ps);
	msg(" - service type = %d", noti->type);
	msg(" - is roaming = %d", noti->is_roaming);
}

static void on_noti_cell_info(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_cell_info *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_CELLINFO);

	msg(" - lac = %d", noti->lac);
	msg(" - cell_id = %d", noti->cell_id);
}

static void on_noti_change(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_change *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_CHANGE);

	msg(" - plmn = %s", noti->plmn);
	msg(" - act = %d", noti->act);
}

static void on_noti_time_info(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_time_info *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_TIMEINFO);

	msg(" - plmn = %s", noti->plmn);
	msg(" - year = %d", noti->year);
	msg(" - month = %d", noti->month);
	msg(" - day = %d", noti->day);
	msg(" - hour = %d", noti->hour);
	msg(" - minute = %d", noti->minute);
	msg(" - second = %d", noti->second);
	msg(" - wday = %d", noti->wday);
	msg(" - gmtoff = %d", noti->gmtoff);
	msg(" - dstoff = %d", noti->dstoff);
	msg(" - isdst = %d", noti->isdst);
}

static void on_noti_identity(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_identity *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_IDENTITY);

	msg(" - plmn = %s", noti->plmn);
	msg(" - short_name = %s", noti->short_name);
	msg(" - full_name = %s", noti->full_name);
}

static void on_noti_signal_strength(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_signal_strength *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_SIGNALSTRENGTH);

	msg(" - dbm = %d", noti->dbm);
}

static void on_noti_default_data_subs(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_default_data_subs *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_DEFAULT_DATA_SUBSCRIPTION);

	if (noti)
		msg(" - 'default' Data Subscription = %s",
			((noti->default_subs == TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM1) ? "SIM 1"
			: ((noti->default_subs == TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM2) ? "SIM 2"
			: "Unknown")));
}

static void on_noti_default_subs(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_default_subs *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_DEFAULT_SUBSCRIPTION);

	if (noti)
		msg(" - 'default' Subscription = %s",
			((noti->default_subs == TAPI_NETWORK_DEFAULT_SUBS_SIM1) ? "SIM 1"
			: ((noti->default_subs == TAPI_NETWORK_DEFAULT_SUBS_SIM2) ? "SIM 2"
			: "Unknown")));
}

static void on_noti_emergency_callback_mode(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	struct tel_noti_network_emergency_callback_mode *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_NETWORK_EMERGENCY_CALLBACK_MODE);

	msg(" - mode = %d", noti->mode);
}

static void on_net_search(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkPlmnList_t *list = data;
	int i = 0;

	msg("");
	msgb("tel_search_network() response receive");
	msg(" - result = 0x%x", result);

	if (!list) {
		msg(" - failed");
		return;
	}

	msg(" - count = %d", list->networks_count);

	for (i = 0; i < list->networks_count; i++) {
		msg(" - [%d] plmn=%s, act=%d, type=%d, name=%s",
				i,
				list->network_list[i].plmn,
				list->network_list[i].access_technology,
				list->network_list[i].type_of_plmn,
				list->network_list[i].network_name);
	}
}

static void on_net_set_plmn_mode_automatic(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_select_network_automatic() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_set_plmn_mode_manual(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_select_network_manual() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_get_plmn_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	int *mode = data;

	msg("");
	msgb("tel_get_network_selection_mode() response receive");
	msg(" - result = 0x%x", result);

	if (data)
		msg(" - mode = %d", *mode);
}

static void on_net_set_preferred_plmn(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_preferred_plmn() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_get_preferred_plmn(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkPreferredPlmnList_t *info = data;
	unsigned int i = 0;

	msg("");
	msgb("tel_get_preferred_plmn() response receive");
	msg(" - result = 0x%x", result);

	if (!info) {
		msg(" - failed");
		return;
	}

	msg(" - count = %d", info->NumOfPrefPlmns);

	for (i = 0; i < info->NumOfPrefPlmns; i++) {
		msg(" - [%d] plmn=%s, type=%d, ef_index=%d, name=%s",
				i,
				info->PrefPlmnRecord[i].Plmn,
				info->PrefPlmnRecord[i].SystemType,
				info->PrefPlmnRecord[i].Index,
				info->PrefPlmnRecord[i].network_name);
	}
}

static void on_net_set_cancel_manual_search(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_cancel_network_manual_search() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_get_serving_network(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkServing_t *serving_info = data;

	msg("");
	msgb("tel_get_network_serving() response receive");
	msg(" - result = 0x%x", result);

	if (!serving_info) {
		msg(" - failed");
		return;
	}

	msg(" - plmn = %s", serving_info->plmn);
	msg(" - access technology = %d", serving_info->act);

	if (serving_info->act >= TAPI_NETWORK_SYSTEM_IS95A && serving_info->act <= TAPI_NETWORK_SYSTEM_EHRPD) {
		msg(" - carrier = %d", serving_info->info.cdma_info.carrier);
		msg(" - system_id = %d", serving_info->info.cdma_info.system_id);
		msg(" - network_id = %d", serving_info->info.cdma_info.network_id);
		msg(" - base_station_id = %d", serving_info->info.cdma_info.base_station_id);
		msg(" - base_station_latitude = %d", serving_info->info.cdma_info.base_station_latitude);
		msg(" - base_station_longitude = %d", serving_info->info.cdma_info.base_station_longitude);
		msg(" - registration_zone = %d", serving_info->info.cdma_info.registration_zone);
		msg(" - pilot_offset = %d", serving_info->info.cdma_info.pilot_offset);
	} else {
		msg(" - lac = %d", serving_info->info.lac);
	}
}

static void on_net_set_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_network_mode() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_get_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	int *mode = data;

	msg("");
	msgb("tel_get_network_mode() response receive");
	msg(" - result = 0x%x", result);

	if (data)
		msg(" - mode = %d", *mode);
}

static void on_net_get_neighboring_cell_info(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkNeighboringCellInfo_t *info = data;
	int i, j;

	msg("");
	msgb("tel_get_neighboring_cell_info() response receive");
	msg(" - result = 0x%x", result);

	if (!data)
		return;

	msg("info=%p", info);

	msg("act=%d", info->serving.act);
	msg("mcc=%d", info->serving.mcc);
	msg("mnc=%d\n", info->serving.mnc);

	if (info->serving.act <= TAPI_NETWORK_SYSTEM_EGPRS) {
		msg("geran.cell_id=%d", info->serving.cell.geran.cell_id);
		msg("geran.lac=%d", info->serving.cell.geran.lac);
		msg("geran.bcch=%d", info->serving.cell.geran.bcch);
		msg("geran.bsic=%d", info->serving.cell.geran.bsic);
		msg("geran.rxlev=%d\n", info->serving.cell.geran.rxlev);
	} else if (info->serving.act <= TAPI_NETWORK_SYSTEM_GSM_AND_UMTS) {
		msg("umts.cell_id=%d", info->serving.cell.umts.cell_id);
		msg("umts.lac=%d", info->serving.cell.umts.lac);
		msg("umts.arfcn=%d", info->serving.cell.umts.arfcn);
		msg("umts.psc=%d", info->serving.cell.umts.psc);
		msg("umts.rscp=%d\n", info->serving.cell.umts.rscp);
	} else if (info->serving.act == TAPI_NETWORK_SYSTEM_LTE) {
		msg("lte.cell_id=%d", info->serving.cell.lte.cell_id);
		msg("lte.lac=%d", info->serving.cell.lte.lac);
		msg("lte.earfcn=%d", info->serving.cell.lte.earfcn);
		msg("lte.tac=%d", info->serving.cell.lte.tac);
		msg("lte.rssi=%d\n", info->serving.cell.lte.rssi);
	} else if (info->serving.act >= TAPI_NETWORK_SYSTEM_IS95A && info->serving.act <= TAPI_NETWORK_SYSTEM_EHRPD) {
		msg("cdma.system_id=%d", info->serving.cell.cdma.system_id);
		msg("cdma.network_id=%d", info->serving.cell.cdma.network_id);
		msg("cdma.base_station_id=%d", info->serving.cell.cdma.base_station_id);
		msg("cdma.reference_pn=%d", info->serving.cell.cdma.reference_pn);
		msg("cdma.base_station_latitude=%d\n", info->serving.cell.cdma.base_station_latitude);
		msg("cdma.base_station_longitude=%d\n", info->serving.cell.cdma.base_station_longitude);
	}

	msg("geran_list_count=%d", info->geran_list_count);
	for (i = 0; i < info->geran_list_count; i++) {
		msg("[%d] cell_id=%d, lac=%d, bcch=%d, bsic=%d, rxlev=%d, ", i,
			info->geran_list[i].cell_id, info->geran_list[i].lac, info->geran_list[i].bcch,
			info->geran_list[i].bsic, info->geran_list[i].rxlev);
	}

	msg("umts_list_count=%d", info->umts_list_count);
	for (j = 0; j < info->umts_list_count; j++) {
		msg("[%d] cell_id=%d, lac=%d, arfcn=%d, psc=%d, rscp=%d, ", j,
			info->umts_list[j].cell_id, info->umts_list[j].lac, info->umts_list[j].arfcn,
			info->umts_list[j].psc, info->umts_list[j].rscp);
	}
}

static void on_net_set_default_data_subscription(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_network_default_data_subscription() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_set_default_subscription(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_net_set_default_subscription() - result : 0x%x", result);
}

static void on_net_set_emergency_callback_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_network_emergency_callback_mode() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_set_roaming_preference(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_network_roaming_preference() response receive");
	msg(" - result = 0x%x", result);
}

static void on_net_get_roaming_preference(TapiHandle *handle, int result, void *data, void *user_data)
{
	int *roam_pref = data;

	msg("");
	msgb("tel_get_network_roaming_preference() response receive");
	msg(" - result = 0x%x", result);

	if (data)
		msg(" - roaming preference = %d", *roam_pref);
}

static int run_net_search(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_search_network()");

	result = tel_search_network(handle, on_net_search, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_plmn_mode_automatic(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_select_network_automatic()");

	result = tel_select_network_automatic(handle, on_net_set_plmn_mode_automatic, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_plmn_mode_manual(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	const char *plmn;
	int act;

	msg("call tel_select_network_manual()");

	plmn = data_net_set_plmn_mode_manual_plmn;
	act = atoi(data_net_set_plmn_mode_manual_act);

	result = tel_select_network_manual(handle, plmn, act, on_net_set_plmn_mode_manual, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_plmn_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_network_selection_mode()");

	result = tel_get_network_selection_mode(handle, on_net_get_plmn_mode, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_preferred_plmn(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int operation;
	TelNetworkPreferredPlmnInfo_t info;

	msg("call tel_set_network_preferred_plmn()");

	memset(&info, 0, sizeof(TelNetworkPreferredPlmnInfo_t));

	operation = atoi(data_net_set_preferred_plmn_mode);
	info.Index = atoi(data_net_set_preferred_plmn_ef_index);
	info.SystemType = atoi(data_net_set_preferred_plmn_type);
	strncpy(info.Plmn, data_net_set_preferred_plmn_plmn, 6);

	result = tel_set_network_preferred_plmn(handle, operation, &info, on_net_set_preferred_plmn, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_preferred_plmn(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_network_preferred_plmn()");

	result = tel_get_network_preferred_plmn(handle, on_net_get_preferred_plmn, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_cancel_manual_search(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_cancel_network_manual_search()");

	result = tel_cancel_network_manual_search(handle, on_net_set_cancel_manual_search, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_serving_network(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_network_serving()");

	result = tel_get_network_serving(handle, on_net_get_serving_network, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int mode;

	msg("call tel_set_network_mode()");

	mode = atoi(data_net_set_mode_mode);

	result = tel_set_network_mode(handle, mode, on_net_set_mode, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_network_mode()");

	result = tel_get_network_mode(handle, on_net_get_mode, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_neighboring_cell_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_network_neighboring_cell_info()");

	result = tel_get_network_neighboring_cell_info(handle, on_net_get_neighboring_cell_info, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_default_data_subscription(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_set_network_default_data_subscription()");

	result = tel_set_network_default_data_subscription(handle, on_net_set_default_data_subscription, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_default_data_subscription(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelNetworkDefaultDataSubs_t default_subs = TAPI_NETWORK_DEFAULT_DATA_SUBS_UNKNOWN;
	int result = 0;

	msg("call tel_get_network_default_data_subscription()");

	result = tel_get_network_default_data_subscription(handle, &default_subs);
	if (result != TAPI_API_SUCCESS) {
		msg("api call failed with[0x%x]", result);
		return 0;
	}

	msg("'default' Data Subscription = %s",
			((default_subs == TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM1) ? "SIM 1"
			: ((default_subs == TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM2) ? "SIM 2"
			: "Unknown")));

	return 0;
}


static int run_net_set_default_subscription(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;

	msg("call tel_set_network_default_subscription()");

	result = tel_set_network_default_subscription(handle, on_net_set_default_subscription, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return result;
}

static int run_net_get_default_subscription(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelNetworkDefaultSubs_t default_subs = TAPI_NETWORK_DEFAULT_SUBS_UNKNOWN;

	msg("call tel_get_network_default_subscription()");

	result = tel_get_network_default_subscription(handle, &default_subs);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
		return 0;
	}

	msg("'default' Subscription for CS (Voice) = %s",
			((default_subs == TAPI_NETWORK_DEFAULT_SUBS_SIM1) ? "SIM 1"
			: ((default_subs == TAPI_NETWORK_DEFAULT_SUBS_SIM2) ? "SIM 2"
			: "Unknown")));

	return result;
}

static int run_net_set_emergency_callback_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int mode;

	msg("call tel_set_network_emergency_callback_mode()");

	mode = atoi(data_net_set_emergency_callback_mode_mode);

	result = tel_set_network_emergency_callback_mode(handle, mode, on_net_set_emergency_callback_mode, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_set_roaming_preference(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int domain;

	msg("call tel_set_network_roaming_preference()");

	domain = atoi(data_net_set_roaming_preference_roam_pref);

	result = tel_set_network_roaming_preference(handle, domain, on_net_set_roaming_preference, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int run_net_get_roaming_preference(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_network_roaming_preference()");

	result = tel_get_network_roaming_preference(handle, on_net_get_roaming_preference, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

static int show_properties(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int lac = -1,
		tac = -1,
		cellid = -1,
		svc_type = -1,
		act = -1,
		ps_type = -1,
		cs = -1,
		ps = -1,
		roam = -1,
		sig_dbm = -1,
		sig_level = -1,
		option = -1,
		ims_voice_status = -1;
	char *plmn = NULL,
		*n_name = NULL,
		*s_name = NULL;

	tel_get_property_int(handle, TAPI_PROP_NETWORK_LAC	, &lac);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_TAC, &tac);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_CELLID, &cellid);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_SERVICE_TYPE, &svc_type);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_ACT, &act);
	tel_get_property_string(handle, TAPI_PROP_NETWORK_PLMN, &plmn);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_PS_TYPE, &ps_type);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_CIRCUIT_STATUS, &cs);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_PACKET_STATUS, &ps);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_SIGNALSTRENGTH_DBM, &sig_dbm);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_SIGNALSTRENGTH_LEVEL, &sig_level);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_ROAMING_STATUS, &roam);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_NAME_OPTION, &option);
	tel_get_property_string(handle, TAPI_PROP_NETWORK_NETWORK_NAME, &n_name);
	tel_get_property_string(handle, TAPI_PROP_NETWORK_SPN_NAME, &s_name);
	tel_get_property_int(handle, TAPI_PROP_NETWORK_IMS_VOICE_SUPPORT_STATUS, &ims_voice_status);

	msg("  [lac]: 0x%08X   [tac]: 0x%08X   [cellid]: 0x%08X\n"
		"  [service_type]: %d    [act]: %d            [plmn]: \"%s\"\n"
		"  [ps_type]: %d         [cs_status]: %d      [ps_status]: %d\n"
		"  [sig_dbm]: %d         [sig_level]: %d      [roaming_status]: %d\n"
		"  [option]: %d  [network_name]: \"%s\"    [spn_name]: \"%s\"\n"
		"  [ims_voice_status]: %d",
		lac, tac, cellid,
		svc_type, act, plmn,
		ps_type, cs, ps,
		sig_dbm, sig_level, roam,
		option, n_name, s_name, ims_voice_status);

	g_free(plmn);
	g_free(n_name);
	g_free(s_name);

	return 0;
}

static struct menu_data menu_net_search[] = {
	{ "1", "run", NULL, run_net_search, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_plmn_mode_automatic[] = {
	{ "1", "run", NULL, run_net_set_plmn_mode_automatic, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_plmn_mode_manual[] = {
	{ "1", "act", NULL, NULL, data_net_set_plmn_mode_manual_act},
	{ "2", "plmn (45001)", NULL, NULL, data_net_set_plmn_mode_manual_plmn},
	{ "3", "run", NULL, run_net_set_plmn_mode_manual, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_plmn_mode[] = {
	{ "1", "run", NULL, run_net_get_plmn_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_preferred_plmn[] = {
	{ "1", "mode (1=add, 2=edit, 3=delete)", NULL, NULL, data_net_set_preferred_plmn_mode},
	{ "2", "plmn (45001)", NULL, NULL, data_net_set_preferred_plmn_plmn},
	{ "3", "type (1=GSM, 2=GPRS, 3=EGPRS, 4=UMTS, ...)", NULL, NULL, data_net_set_preferred_plmn_type},
	{ "4", "ef_index (0, ...)", NULL, NULL, data_net_set_preferred_plmn_ef_index},
	{ "5", "run", NULL, run_net_set_preferred_plmn, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_preferred_plmn[] = {
	{ "1", "run", NULL, run_net_get_preferred_plmn, NULL},
	{ NULL, NULL, },
};


static struct menu_data menu_net_set_cancel_manual_search[] = {
	{ "1", "run", NULL, run_net_set_cancel_manual_search, NULL},
	{ NULL, NULL, },
};


static struct menu_data menu_net_get_serving_network[] = {
	{ "1", "run", NULL, run_net_get_serving_network, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_mode_3gpp[] = {
	{ "1", "mode (1=GSM, 2=WCDMA, 4=CDMA, 8=LTE)", NULL, NULL, data_net_set_mode_mode},
	{ "3", "run", NULL, run_net_set_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_mode_3gpp2[] = {
	{ "1", "mode (4=CDMA, 8=LTE)", NULL, NULL, data_net_set_mode_mode},
	{ "3", "run", NULL, run_net_set_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_mode[] = {
	{ "1", "run", NULL, run_net_get_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_neighboring_cell_info[] = {
	{ "1", "run", NULL, run_net_get_neighboring_cell_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_default_data_subscription[] = {
	{ "1", "run", NULL, run_net_set_default_data_subscription, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_default_data_subscription[] = {
	{ "1", "run", NULL, run_net_get_default_data_subscription, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_default_subscription[] = {
	{ "1", "run", 0, run_net_set_default_subscription, 0},
	{ 0, 0, },
};

static struct menu_data menu_net_get_default_subscription[] = {
	{ "1", "run", 0, run_net_get_default_subscription, 0},
	{ 0, 0, },
};

static struct menu_data menu_net_set_emergency_callback_mode[] = {
	{ "1", "mode (0=ENTER, 1=EXIT)", NULL, NULL, data_net_set_emergency_callback_mode_mode},
	{ "2", "run", NULL, run_net_set_emergency_callback_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_roaming_preference[] = {
	{ "1", "roaming preference (1=AUTOMATIC, 2=AUTOMATIC_A, 3=AUTOMATIC_B, 4=HOME_ONLY, 5=ROAM_ONLY)", NULL, NULL, data_net_set_roaming_preference_roam_pref},
	{ "2", "run", NULL, run_net_set_roaming_preference, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_roaming_preference[] = {
	{ "1", "run", NULL, run_net_get_roaming_preference, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_net_3gpp[] = {
	{ "1", "Search", menu_net_search, NULL, NULL},
	{ "2a", "SET PLMN Selection mode (Automatic)", menu_net_set_plmn_mode_automatic, NULL, NULL},
	{ "2s", "SET PLMN Selection mode (Manual)", menu_net_set_plmn_mode_manual, NULL, NULL},
	{ "2g", "GET PLMN Selection mode", menu_net_get_plmn_mode, NULL, NULL},
	{ "5s", "SET Preferred PLMN", menu_net_set_preferred_plmn, NULL, NULL},
	{ "5g", "GET Preferred PLMN", menu_net_get_preferred_plmn, NULL, NULL},
	{ "6", "SET Cancel manual search", menu_net_set_cancel_manual_search, NULL, NULL},
	{ "7", "GET Serving network", menu_net_get_serving_network, NULL, NULL},
	{ "8s", "SET Mode", menu_net_set_mode_3gpp, NULL, NULL},
	{ "8g", "GET Mode", menu_net_get_mode, NULL, NULL},
	{ "9g", "GET Neighboring Cell Info", menu_net_get_neighboring_cell_info, NULL, NULL},
	{ "10", "SET Default Data Subscription", menu_net_set_default_data_subscription, NULL, NULL},
	{ "11", "GET Default Data Subscription", menu_net_get_default_data_subscription, NULL, NULL},
	{ "12", "SET Default Subscription (for Voice)", menu_net_set_default_subscription, NULL, NULL},
	{ "13", "GET Default Subscription (for Voice)", menu_net_get_default_subscription, NULL, NULL},
	{ "*", "<Properties>", NULL, show_properties, NULL },
	{ NULL, NULL, },
};

struct menu_data menu_net_3gpp2[] = {
	{ "1s", "SET PLMN Selection mode (Automatic)", menu_net_set_plmn_mode_automatic, NULL, NULL},
	{ "1g", "GET PLMN Selection mode", menu_net_get_plmn_mode, NULL, NULL},
	{ "4s", "SET Preferred PLMN", menu_net_set_preferred_plmn, NULL, NULL},
	{ "4g", "GET Preferred PLMN", menu_net_get_preferred_plmn, NULL, NULL},
	{ "5", "GET Serving network", menu_net_get_serving_network, NULL, NULL},
	{ "6s", "SET Mode", menu_net_set_mode_3gpp2, NULL, NULL},
	{ "6g", "GET Mode", menu_net_get_mode, NULL, NULL},
	{ "7g", "GET Neighboring Cell Info", menu_net_get_neighboring_cell_info, NULL, NULL},
	{ "8", "SET Emergency Callback Mode", menu_net_set_emergency_callback_mode, NULL, NULL},
	{ "9s", "SET Roaming Preference", menu_net_set_roaming_preference, NULL, NULL},
	{ "9g", "GET Roaming Preference", menu_net_get_roaming_preference, NULL, NULL},
	{ "*", "<Properties>", NULL, show_properties, NULL },
	{ NULL, NULL, },
};

struct menu_data menu_net[] = { { "1", "3GPP(GSM/WCDMA/LTE)", menu_net_3gpp, NULL, NULL }, {
		"2", "3GPP2(CDMA)", menu_net_3gpp2, NULL, NULL }, { NULL, NULL , }, };

void register_network_event(TapiHandle *handle)
{
	int ret;

	/* Signal */
	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_REGISTRATION_STATUS,
			on_noti_registration_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_CELLINFO, on_noti_cell_info, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_CHANGE, on_noti_change, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_TIMEINFO, on_noti_time_info, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_IDENTITY, on_noti_identity, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_SIGNALSTRENGTH, on_noti_signal_strength, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_DEFAULT_DATA_SUBSCRIPTION, on_noti_default_data_subs, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_DEFAULT_SUBSCRIPTION, on_noti_default_subs, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_NOTI_NETWORK_EMERGENCY_CALLBACK_MODE, on_noti_emergency_callback_mode, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	/* Property */
	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_SIGNALSTRENGTH_DBM, on_prop_signal_dbm, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_SIGNALSTRENGTH_LEVEL, on_prop_signal_level, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_LAC, on_prop_lac, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_TAC, on_prop_tac, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_PLMN, on_prop_plmn, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_CELLID, on_prop_cellid, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_SERVICE_TYPE, on_prop_service_type, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_ACT, on_prop_act, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_PS_TYPE, on_prop_ps_type, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_CIRCUIT_STATUS, on_prop_circuit_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_PACKET_STATUS, on_prop_packet_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_ROAMING_STATUS, on_prop_roaming_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_NAME_OPTION, on_prop_name_option, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_NETWORK_NAME, on_prop_network_name, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_SPN_NAME, on_prop_spn_name, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_IMS_VOICE_SUPPORT_STATUS, on_prop_ims_voice_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle,
			TAPI_PROP_NETWORK_VOLTE_ENABLE, on_noti_volte_enable, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);
}
