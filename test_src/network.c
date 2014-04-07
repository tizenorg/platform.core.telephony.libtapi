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
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>
#include <glib-object.h>

#include <tapi.h>
#include <tapi_events.h>
#include <tapi_network.h>

#include "menu.h"
#include "network.h"

#define CHECK_NETWORK_RESULT(x) \
{ \
	if (x >= TEL_NETWORK_RESULT_SUCCESS && x <= TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED) { \
		msg("result[%d][%s]", x, dbg_network_result[x]); \
	} else { \
		msg("API failed with result[%d]", x); \
	} \
	if (x != TEL_NETWORK_RESULT_SUCCESS) \
		return; \
}

static char *dbg_network_result[] = {"TEL_NETWORK_RESULT_SUCCESS", "TEL_NETWORK_RESULT_FAILURE",
		"TEL_NETWORK_RESULT_INVALID_PARAMETER", "TEL_NETWORK_RESULT_MEMORY_FAILURE",
		"TEL_NETWORK_RESULT_OPERATION_NOT_PERMITTED", "TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED", };

static char data_network_act[MENU_DATA_SIZE + 1] = "";
static char data_network_plmn[MENU_DATA_SIZE + 1] = "45001";
static char data_network_plmn_index[MENU_DATA_SIZE + 1] = "1";
static char data_network_mode[MENU_DATA_SIZE + 1] = "";


/* Notification Callbacks */
static void on_noti_registration_status(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelNetworkRegStatusInfo *reg_info = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_NETWORK_REGISTRATION_STATUS);

	if (!reg_info) {
		msg("Network registration status noti data is null");
		return;
	}

	msg("circuit status[%d]", reg_info->cs_status);
	msg("packet status[%d]", reg_info->ps_status);
	msg("access technology[%d]", reg_info->act);
}

static void on_noti_cell_info(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelNetworkCellInfo *cell_info = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_NETWORK_CELL_INFO);

	if (!cell_info) {
		msg("Network cell info noti data is null");
		return;
	}

	msg("lac[%d]", cell_info->lac);
	msg("cell_id[%d]", cell_info->cell_id);
	msg("rac[%d]", cell_info->rac);
}

static void on_noti_identity(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelNetworkIdentityInfo *id_info = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_NETWORK_IDENTITY);

	if (!id_info) {
		msg("Network identity noti data is null");
		return;
	}

	msg("plmn[%s]", id_info->plmn);
	msg("short_name[%s]", id_info->short_name);
	msg("long_name[%s]", id_info->long_name);
}

static void on_noti_rssi(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	unsigned int *rssi = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_NETWORK_RSSI);

	if (!rssi) {
		msg("Network Rssi noti data is null");
		return;
	}

	msg("RSSI[%d]", *rssi);
}

static void on_noti_time_info(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelNetworkNitzInfoNoti *time_info = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_NETWORK_TIME_INFO);

	if (!time_info) {
		msg("Time info noti data is null");
		return;
	}

	msg("plmn[%s]", time_info->plmn);
	msg("year[%d]", time_info->year);
	msg("month[%d]", time_info->month);
	msg("day[%d]", time_info->day);
	msg("hour[%d]", time_info->hour);
	msg("minute[%d]", time_info->minute);
	msg("second[%d]", time_info->second);
	msg("GMT offset[%d]", time_info->gmtoff);
	msg("DST offset[%d]", time_info->dstoff);
	msg("is DST[%d]", time_info->isdst);
}

static int run_net_get_registration_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelNetworkRegistrationInfo reg_info;
	TelReturn rt = 0;

	msg("call tapi_network_get_registration_info()");

	memset(&reg_info, 0, sizeof(TelNetworkRegistrationInfo));

	rt = tapi_network_get_registration_info(handle, &reg_info);
	CHECK_RT(rt);

	msg("cs_status[%d]", reg_info.reg_status_info.cs_status);
	msg("ps_status[%d]", reg_info.reg_status_info.ps_status);
	msg("act[%d]", reg_info.reg_status_info.act);
	msg("lac[%d]", reg_info.cell_info.lac);
	msg("cell_id[%d]", reg_info.cell_info.cell_id);
	msg("rac[%d]", reg_info.cell_info.rac);

	return 0;
}

static void on_net_get_net_id_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkIdentityInfo *id_info = data;

	msg("tapi_network_get_identity_info() response received");
	CHECK_NETWORK_RESULT(result);

	if (!id_info) {
		msg("network identity info data is null");
		return;
	}

	msg("plmn[%s]", id_info->plmn);
	msg("short_name[%s]", id_info->short_name);
	msg("long_name[%s]", id_info->long_name);
}

static int run_net_get_net_id_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_get_identity_info()");

	rt = tapi_network_get_identity_info(handle, on_net_get_net_id_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_search(TelHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkPlmnList *plmn_list = data;
	unsigned int i = 0;

	msgb("tapi_network_search() response received");
	CHECK_NETWORK_RESULT(result);

	if (!plmn_list) {
		msg("plmn list data is null");
		return;
	}

	msg("plmn_list count[%d]", plmn_list->count);

	for (i = 0; i < plmn_list->count; i++) {
		msg("[%d] plmn_status[%d] act[%d] plmn[%s] short_name[%s] long_name[%s]",
			i+1, plmn_list->network_list[i].plmn_status, plmn_list->network_list[i].act,
			plmn_list->network_list[i].network_identity.plmn,
			plmn_list->network_list[i].network_identity.short_name,
			plmn_list->network_list[i].network_identity.long_name);
	}
}

static int run_net_search(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_search()");

	rt = tapi_network_search(handle, on_net_search, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_cancel_network_search(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_network_cancel_search() response received");
	CHECK_NETWORK_RESULT(result);
}

static int run_net_cancel_network_search(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_cancel_search()");

	rt = tapi_network_cancel_search(handle, on_net_cancel_network_search, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_select_network_automatic(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_network_select_automatic() response received");
	CHECK_NETWORK_RESULT(result);
}

static int run_net_select_network_automatic(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_select_automatic()");

	rt = tapi_network_select_automatic(handle, on_net_select_network_automatic, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_select_network_manual(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_network_select_manual() response received");
	CHECK_NETWORK_RESULT(result);
}

static int run_net_select_network_manual(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelNetworkSelectManualInfo select_info;
	TelReturn rt = 0;

	msg("call tapi_network_select_manual()");

	memset(&select_info, 0, sizeof(TelNetworkSelectManualInfo));

	select_info.act = atoi(data_network_act);
	select_info.plmn = (char *)data_network_plmn;

	msg("act[%d]", select_info.act);
	msg("plmn[%s]", select_info.plmn);

	rt = tapi_network_select_manual(handle, &select_info, on_net_select_network_manual, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_get_network_selection_mode(TelHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkSelectionMode *mode = data;

	msgb("tapi_network_get_selection_mode() response received");
	CHECK_NETWORK_RESULT(result);

	if (!mode) {
		msg("network selection mode data is null");
		return;
	}

	msg("network selection mode: [%d]", *mode);
}

static int run_net_get_network_selection_mode(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_get_selection_mode()");

	rt = tapi_network_get_selection_mode(handle, on_net_get_network_selection_mode, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_set_preferred_plmn(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_network_set_preferred_plmn() response received");
	CHECK_NETWORK_RESULT(result);
}

static int run_net_set_preferred_plmn(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelNetworkPreferredPlmnInfo info;

	msg("call tapi_network_set_preferred_plmn()");

	memset(&info, 0, sizeof(TelNetworkPreferredPlmnInfo));

	info.index = atoi(data_network_plmn_index);
	info.plmn = (char *)data_network_plmn;
	info.act = atoi(data_network_act);

	msg("plmn_index[%d]", info.index);
	msg("plmn[%s]", info.plmn);
	msg("act[%d]", info.act);

	rt = tapi_network_set_preferred_plmn(handle, &info, on_net_set_preferred_plmn, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_get_preferred_plmn(TelHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkPreferredPlmnList *plmn_list = data;
	unsigned int i = 0;

	msgb("tapi_network_get_preferred_plmn() response received");
	CHECK_NETWORK_RESULT(result);

	if (!plmn_list) {
		msg("preferred plmn list data is null");
		return;
	}

	msg("preferred PLMN count[%d]", plmn_list->count);

	for(i = 0; i < plmn_list->count; i++) {
		msg("[%d] index[%d] plmn[%s] act[%d]", i+1, plmn_list->list[i].index,
			plmn_list->list[i].plmn, plmn_list->list[i].act);
	}
}

static int run_net_get_preferred_plmn(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_get_preferred_plmn()");

	rt = tapi_network_get_preferred_plmn(handle, on_net_get_preferred_plmn, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_set_network_mode(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_network_set_mode() response received");
	CHECK_NETWORK_RESULT(result);
}

static int run_net_set_network_mode(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelNetworkMode mode;
	TelReturn rt = 0;

	msg("call tapi_network_set_mode()");

	mode = atoi(data_network_mode);
	msg("requested mode[%d]", mode);

	rt = tapi_network_set_mode(handle, mode, on_net_set_network_mode, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_get_network_mode(TelHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkMode *mode = data;

	msgb("tapi_network_get_mode() response received");
	CHECK_NETWORK_RESULT(result);

	if (!mode) {
		msg("network mode data is null");
		return;
	}

	msg("mode[%d]", *mode);
}

static int run_net_get_network_mode(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_get_mode()");

	rt = tapi_network_get_mode(handle, on_net_get_network_mode, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_net_get_neighboring_cell_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelNetworkNeighbourCellInfo *cell_info = data;
	unsigned int i = 0;

	msgb("tapi_network_get_neighboring_cell_info() response received");
	CHECK_NETWORK_RESULT(result);

	if (!cell_info) {
		msg("neighbouring cell info data is null");
		return;
	}

	msg("gsm_list_count[%d]", cell_info->gsm_list_count);

	for(i = 0; i < cell_info->gsm_list_count; i++) {
		msg("[%d] cell_id[%d] lac[%d] bcch[%d] bsic[%d] rxlev[%d]",
			i+1, cell_info->gsm_list[i].cell_id, cell_info->gsm_list[i].lac,
			cell_info->gsm_list[i].bcch, cell_info->gsm_list[i].bsic,
			cell_info->gsm_list[i].rxlev);
	}

	msg("umts_list_count[%d]", cell_info->umts_list_count);

	for(i = 0; i < cell_info->umts_list_count; i++) {
		msg("[%d] cell_id[%d] lac[%d] arfcn[%d] psc[%d] rscp[%d]",
			i+1, cell_info->umts_list[i].cell_id, cell_info->umts_list[i].lac,
			cell_info->umts_list[i].arfcn, cell_info->umts_list[i].psc,
			cell_info->umts_list[i].rscp);
	}
}

static int run_net_get_neighboring_cell_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_network_get_neighboring_cell_info()");

	rt = tapi_network_get_neighboring_cell_info(handle, on_net_get_neighboring_cell_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static struct menu_data menu_net_get_registration_info[] = {
	{ "1", "run", NULL, run_net_get_registration_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_net_id_info[] = {
	{ "1", "run", NULL, run_net_get_net_id_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_search[] = {
	{ "1", "run", NULL, run_net_search, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_cancel_network_search[] = {
	{ "1", "run", NULL, run_net_cancel_network_search, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_select_network_automatic[] = {
	{ "1", "run", NULL, run_net_select_network_automatic, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_select_network_manual[] = {
	{ "1", "act (0=UNKNOWN 1=GSM 2=GPRS 3=EGPRS 4=UMTS 5=GSM/UMTS 6=HSDPA 7=HSUPA 8=HSPA 9=LTE)",
			NULL, NULL, data_network_act},
	{ "2", "plmn (45001)", NULL, NULL, data_network_plmn},
	{ "3", "run", NULL, run_net_select_network_manual, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_network_selection_mode[] = {
	{ "1", "run", NULL, run_net_get_network_selection_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_preferred_plmn[] = {
	{ "1", "index (starts from 1)", NULL, NULL, data_network_plmn_index},
	{ "2", "plmn (45001)", NULL, NULL, data_network_plmn},
	{ "3", "act (0=UNKNOWN 1=GSM 2=GPRS 3=EGPRS 4=UMTS 5=GSM/UMTS 6=HSDPA 7=HSUPA 8=HSPA 9=LTE)",
			NULL, NULL, data_network_act},
	{ "4", "run", NULL, run_net_set_preferred_plmn, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_preferred_plmn[] = {
	{ "1", "run", NULL, run_net_get_preferred_plmn, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_set_network_mode[] = {
	{ "1", "network mode (0=AUTO 1=2G 2=3G 3=LTE)", NULL, NULL, data_network_mode},
	{ "2", "run", NULL, run_net_set_network_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_network_mode[] = {
	{ "1", "run", NULL, run_net_get_network_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_net_get_neighboring_cell_info[] = {
	{ "1", "run", NULL, run_net_get_neighboring_cell_info, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_net[] = {
	{ "1g", "Get Network Registration Info", menu_net_get_registration_info, NULL, NULL},
	{ "2g", "Get Network Identity Info", menu_net_get_net_id_info, NULL, NULL},
	{ "3", "Network Search ", menu_net_search, NULL, NULL},
	{ "4", "Cancel Network Search", menu_net_cancel_network_search, NULL, NULL},
	{ "5", "Select Network Automatic", menu_net_select_network_automatic, NULL, NULL},
	{ "6", "Select Network Manual", menu_net_select_network_manual, NULL, NULL},
	{ "7g", "Get Network Selection mode", menu_net_get_network_selection_mode, NULL, NULL},
	{ "8s", "Set Preferred PLMN", menu_net_set_preferred_plmn, NULL, NULL},
	{ "8g", "Get Preferred PLMN", menu_net_get_preferred_plmn, NULL, NULL},
	{ "9s", "Set Network mode", menu_net_set_network_mode, NULL, NULL},
	{ "9g", "Get Network mode", menu_net_get_network_mode, NULL, NULL},
	{ "10g", "Get Neighboring Cell Info", menu_net_get_neighboring_cell_info, NULL, NULL},
	{ NULL, NULL, },
};

void register_network_event(TelHandle *handle)
{
	TelReturn ret;

	/* Network */
	ret = tapi_register_event_id(handle, TEL_NOTI_NETWORK_REGISTRATION_STATUS, on_noti_registration_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_NETWORK_REGISTRATION_STATUS - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_NETWORK_CELL_INFO, on_noti_cell_info, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_NETWORK_CELL_INFO - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_NETWORK_IDENTITY, on_noti_identity, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_NETWORK_IDENTITY - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_NETWORK_RSSI, on_noti_rssi, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_NETWORK_SIGNAL_STRENGTH - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_NETWORK_TIME_INFO, on_noti_time_info, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_NETWORK_TIME_INFO - register event failed: [%d]", ret);
}
