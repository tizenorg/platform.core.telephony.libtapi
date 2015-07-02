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
#include <ITapiPhonebook.h>
#include <TapiUtility.h>

#include "menu.h"
#include "sim.h"
#include "phonebook.h"

#define CHECK_RT(x) do { \
	if ((x) != 0) { \
		msg("api call failed with[%d]", x); \
		return 0; \
	} \
} while (0)

static const char *dbg_pb_type_name[] = { "TAPI_SIM_PB_FDN", "TAPI_SIM_PB_ADN", "TAPI_SIM_PB_SDN",
		"TAPI_SIM_PB_3GSIM", "TAPI_SIM_PB_AAS", "TAPI_SIM_PB_GAS", };
static const char *dbg_pb_adf_field_name[] = { "NO VALUE 0", "TAPI_PB_3G_NAME", "TAPI_PB_3G_NUMBER",
		"TAPI_PB_3G_ANR1", "TAPI_PB_3G_ANR2", "TAPI_PB_3G_ANR3", "TAPI_PB_3G_EMAIL1",
		"TAPI_PB_3G_EMAIL2", "TAPI_PB_3G_EMAIL3", "TAPI_PB_3G_EMAIL4", "TAPI_PB_3G_SNE",
		"TAPI_PB_3G_GRP", "TAPI_PB_3G_PBC" };
static const char *dbg_pb_ton_name[] = { "TAPI_SIM_TON_UNKNOWN", "TAPI_SIM_TON_INTERNATIONAL",
		"TAPI_SIM_TON_NATIONAL", "TAPI_SIM_TON_NETWORK_SPECIFIC", "TAPI_SIM_TON_DEDICATED_ACCESS",
		"TAPI_SIM_TON_ALPHA_NUMERIC", "TAPI_SIM_TON_ABBREVIATED_NUMBER",
		"TAPI_SIM_TON_RESERVED_FOR_EXT", };
static const char *dbg_pb_dcs_name[] = { "TAPI_SIM_TEXT_ENC_ASCII", "TAPI_SIM_TEXT_ENC_GSM7BIT",
		"TAPI_SIM_TEXT_ENC_UCS2", "TAPI_SIM_TEXT_ENC_HEX", };

static char data_pb_type[MENU_DATA_SIZE + 1] = {};
static char data_pb_index[MENU_DATA_SIZE + 1] = {};
static char data_pb_name[MENU_DATA_SIZE + 1] = {};
static char data_pb_dcs[MENU_DATA_SIZE + 1] = {};
static char data_pb_number[MENU_DATA_SIZE + 1] = {};
static char data_pb_ton[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr1[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr1_ton[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr2[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr2_ton[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr3[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr3_ton[MENU_DATA_SIZE + 1] = {};
static char data_pb_email1[MENU_DATA_SIZE + 1] = {};
static char data_pb_email2[MENU_DATA_SIZE + 1] = {};
static char data_pb_email3[MENU_DATA_SIZE + 1] = {};
static char data_pb_email4[MENU_DATA_SIZE + 1] = {};
static char data_pb_group_index[MENU_DATA_SIZE + 1] = {};
static char data_pb_pb_control[MENU_DATA_SIZE + 1] = {};


static void on_noti_pb_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSimPbStatus_t *status = data;
	msgb("event(%s) receive !!", TAPI_NOTI_PB_STATUS);
	if (!status)
		return;

	msg("init[%d] fdn[%d] adn[%d] sdn[%d] usim[%d] aas[%d] gas[%d]",
		status->init_completed,
		status->pb_list.b_fdn,
		status->pb_list.b_adn,
		status->pb_list.b_sdn,
		status->pb_list.b_3g,
		status->pb_list.b_aas,
		status->pb_list.b_gas);
}

static void on_noti_pb_contact_change(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSimPbContactChangeInfo_t *contact_change = data;
	msgb("event(%s) receive !!", TAPI_NOTI_PB_CONTACT_CHANGE);
	if (!contact_change)
		return;

	msg("phonebook_type[%d] index[%d] operation[%d]",
		contact_change->pb_type,
		contact_change->index,
		contact_change->operation);
}

static int run_pb_get_sim_pb_init_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int rt = 0;
	int o_status = 0;
	TelSimPbList_t list;

	msg("run_pb_get_sim_pb_init_info service!");
	memset(&list, 0, sizeof(TelSimPbList_t));

	rt = tel_get_sim_pb_init_info(handle, &o_status, &list);
	CHECK_RT(rt);

	msg("status[%d]", o_status);
	msg("fdn[%d]", list.b_fdn);
	msg("adn[%d]", list.b_adn);
	msg("sdn[%d]", list.b_sdn);
	msg("3g usim[%d]", list.b_3g);
	msg("aas[%d]", list.b_aas);
	msg("gas[%d]", list.b_gas);
	return 0;
}

static void on_pb_get_sim_pb_count(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSimPbAccessResult_t access_rt = result;
	TelSimPbStorageInfo_t *ps = data;

	msg("on_pb_get_sim_pb_count received");
	msg("access_rt[%d]", access_rt);
	if (access_rt == TAPI_SIM_PB_SUCCESS) {
		if (ps->StorageFileType <= TAPI_SIM_PB_GAS)
			msg("StorageFileType[%d][%s]", ps->StorageFileType, dbg_pb_type_name[ps->StorageFileType]);
		else
			msg("StorageFileType[%d]", ps->StorageFileType);

		msg("TotalRecordCount[%d]", ps->TotalRecordCount);
		msg("UsedRecordCount[%d]", ps->UsedRecordCount);
	}
}

static int run_pb_get_sim_pb_count(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelSimPbType_t pb_type = TAPI_SIM_PB_UNKNOWNN;
	int rt = 0;

	pb_type = data_pb_type[0] - '0';
	msg("request pb [%d]", pb_type);
	rt = tel_get_sim_pb_count(handle, pb_type, on_pb_get_sim_pb_count, NULL);
	CHECK_RT(rt);
	return 0;
}

static void on_pb_get_sim_pb_meta_info(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSimPbAccessResult_t access_rt = result;
	TelSimPbEntryInfo_t *pe = data;

	msg("on_pb_get_sim_pb_meta_info received");
	msg("access_rt[%d]", access_rt);
	if (access_rt == TAPI_SIM_PB_SUCCESS) {
		if (pe->StorageFileType <= TAPI_SIM_PB_GAS)
			msg("StorageFileType[%d][%s]", pe->StorageFileType, dbg_pb_type_name[pe->StorageFileType]);
		else
			msg("StorageFileType[%d]", pe->StorageFileType);

		msg("PbIndexMin[%d]", pe->PbIndexMin);
		msg("PbIndexMax[%d]", pe->PbIndexMax);
		msg("PbNumLenMax[%d]", pe->PbNumLenMax);
		msg("PbTextLenMax[%d]", pe->PbTextLenMax);
		msg("PbUsedCount[%d]", pe->PbUsedCount);
	}

}

static int run_pb_get_sim_pb_meta_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelSimPbType_t pb_type = TAPI_SIM_PB_UNKNOWNN;
	int rt = 0;

	pb_type = data_pb_type[0] - '0';
	msg("request pb [%d]", pb_type);
	rt = tel_get_sim_pb_meta_info(handle, pb_type, on_pb_get_sim_pb_meta_info, NULL);
	CHECK_RT(rt);
	return 0;
}

static void on_pb_get_sim_pb_usim_meta_info(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSimPbAccessResult_t access_rt = result;
	TelSimPbCapabilityInfo_t *capa = data;
	int i = 0;

	msg("on_pb_get_sim_pb_3g_meta_info received");
	msg("access_rt[%d]", access_rt);
	if (access_rt == TAPI_SIM_PB_SUCCESS) {
		for (i = 0; i < capa->FileTypeCount; i++) {
			if (capa->FileTypeInfo[i].field_type <= TAPI_PB_3G_PBC)
				msg("capa->FileTypeInfo[%d].field_type[%d][%s]", i, capa->FileTypeInfo[i].field_type, dbg_pb_adf_field_name[capa->FileTypeInfo[i].field_type]);
			else
				msg("capa->FileTypeInfo[%d].field_type[%d]", i, capa->FileTypeInfo[i].field_type);

			msg("capa->FileTypeInfo[%d].index_max[%d]", i, capa->FileTypeInfo[i].index_max);
			msg("capa->FileTypeInfo[%d].text_max[%d]", i, capa->FileTypeInfo[i].text_max);
			msg("capa->FileTypeInfo[%d].used_count[%d]", i, capa->FileTypeInfo[i].used_count);
		}
	}
}

static int run_pb_get_sim_pb_usim_meta_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int rt = 0;

	msg("call 3g meta info");
	rt = tel_get_sim_pb_usim_meta_info(handle, on_pb_get_sim_pb_usim_meta_info, NULL);
	CHECK_RT(rt);
	return 0;
}

static void on_pb_read_sim_pb_record(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSimPbAccessResult_t access_rt = result;
	TelSimPbRecord_t *pr = data;

	msg("on_pb_read_sim_pb_record received");
	msg("access_rt[%d]", access_rt);
	if (access_rt == TAPI_SIM_PB_SUCCESS) {
		if (pr->phonebook_type <= TAPI_SIM_PB_GAS)
			msg("phonebook_type[%d][%s]", pr->phonebook_type, dbg_pb_type_name[pr->phonebook_type]);
		 else
			msg("phonebook_type[%d]", pr->phonebook_type);

		msg("index[%d]", pr->index);
		msg("next_index[%d]", pr->next_index);
		msg("name[%s]", pr->name);
		if (pr->dcs <= TAPI_SIM_TEXT_ENC_HEX)
			msg("dcs[%d][%s]", pr->dcs, dbg_pb_dcs_name[pr->dcs]);
		else
			msg("dcs[%d]", pr->dcs);

		msg("number[%s]", pr->number);

		if (pr->ton <= TAPI_SIM_TON_RESERVED_FOR_EXT)
			msg("ton[%d][%s]", pr->ton, dbg_pb_ton_name[pr->ton]);
		else
			msg("ton[%d]", pr->ton);

		if (pr->phonebook_type == TAPI_SIM_PB_3GSIM) {
			msg("anr1[%s]", pr->anr1);
			if (pr->anr1_ton <= TAPI_SIM_TON_RESERVED_FOR_EXT)
				msg("anr1_ton[%d][%s]", pr->anr1_ton, dbg_pb_ton_name[pr->anr1_ton]);
			else
				msg("anr1_ton[%d]", pr->anr1_ton);

			msg("anr2[%s]", pr->anr2);
			if (pr->anr2_ton <= TAPI_SIM_TON_RESERVED_FOR_EXT)
				msg("anr2_ton[%d][%s]", pr->anr2_ton, dbg_pb_ton_name[pr->anr2_ton]);
			else
				msg("anr2_ton[%d]", pr->anr2_ton);

			msg("anr3[%s]", pr->anr3);
			if (pr->anr3_ton <= TAPI_SIM_TON_RESERVED_FOR_EXT)
				msg("anr3_ton[%d][%s]", pr->anr3_ton, dbg_pb_ton_name[pr->anr3_ton]);
			else
				msg("anr3_ton[%d]", pr->anr3_ton);

			msg("email1[%s]", pr->email1);
			msg("email2[%s]", pr->email2);
			msg("email3[%s]", pr->email3);
			msg("email4[%s]", pr->email4);
			msg("group_index[%d]", pr->group_index);
			msg("pb_control[%d]", pr->pb_control);
		}
	}
}

static int run_pb_read_sim_pb_record(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelSimPbType_t pb_type = TAPI_SIM_PB_UNKNOWNN;
	int rt = 0;
	int local_index = 0;

	pb_type = atoi(data_pb_type);
	local_index = atoi(data_pb_index);

	msg("request pb [%d], local_index[%d]", pb_type, local_index);
	rt = tel_read_sim_pb_record(handle, pb_type, local_index, on_pb_read_sim_pb_record, NULL);
	CHECK_RT(rt);
	return 0;
}

static void on_pb_update_sim_pb_record(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSimPbAccessResult_t access_rt = result;

	msg("on_pb_update_sim_pb_record received");
	msg("access_rt[%d]", access_rt);
}

static int run_pb_update_sim_pb_record(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int rt = 0;
	TelSimPbRecord_t pb;

	memset(&pb, 0, sizeof(TelSimPbRecord_t));

	pb.phonebook_type = atoi(data_pb_type);
	pb.index = atoi(data_pb_index);
	snprintf((char *)pb.name, strlen(data_pb_name)+1, "%s", data_pb_name);
	pb.dcs = atoi(data_pb_dcs);
	snprintf((char *)pb.number, strlen(data_pb_number)+1, "%s", data_pb_number);
	pb.ton = atoi(data_pb_ton);

	if (pb.phonebook_type == TAPI_SIM_PB_3GSIM) {
		snprintf((char *)pb.anr1, strlen(data_pb_anr1) + 1, "%s", data_pb_anr1);
		pb.anr1_ton = data_pb_anr1_ton[0] - '0';
		snprintf((char *)pb.anr2, strlen(data_pb_anr2) + 1, "%s", data_pb_anr2);
		pb.anr2_ton = data_pb_anr2_ton[0] - '0';
		snprintf((char *)pb.anr3, strlen(data_pb_anr3) + 1, "%s", data_pb_anr3);
		pb.anr3_ton = data_pb_anr3_ton[0] - '0';
		snprintf((char *)pb.email1, strlen(data_pb_email1) + 1, "%s", data_pb_email1);
		snprintf((char *)pb.email2, strlen(data_pb_email2) + 1, "%s", data_pb_email2);
		snprintf((char *)pb.email3, strlen(data_pb_email3) + 1, "%s", data_pb_email3);
		snprintf((char *)pb.email4, strlen(data_pb_email4) + 1, "%s", data_pb_email4);
		pb.group_index = data_pb_group_index[0] - '0';
		pb.pb_control = data_pb_pb_control[0] - '0';
	}

	if (pb.phonebook_type <= TAPI_SIM_PB_GAS)
		msg("phonebook_type[%d][%s]", pb.phonebook_type, dbg_pb_type_name[pb.phonebook_type]);
	else
		msg("phonebook_type[%d]", pb.phonebook_type);

	rt = tel_update_sim_pb_record(handle, &pb, on_pb_update_sim_pb_record, NULL);
	CHECK_RT(rt);
	return 0;
}

static void on_pb_delete_sim_pb_record(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSimPbAccessResult_t access_rt = result;

	msg("on_pb_delete_sim_pb_record received");
	msg("access_rt[%d]", access_rt);
}

static int run_pb_delete_sim_pb_record(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelSimPbType_t pb_type = TAPI_SIM_PB_UNKNOWNN;
	int rt = 0;
	int local_index = 0;

	pb_type = atoi(data_pb_type);
	local_index = atoi(data_pb_index);

	msg("request pb [%d], local_index[%d]", pb_type, local_index);
	rt = tel_delete_sim_pb_record(handle, pb_type, local_index, on_pb_delete_sim_pb_record, NULL);
	CHECK_RT(rt);
	return 0;
}

static struct menu_data menu_pb_get_sim_pb_init_info[] = {
	{ "1", "run", NULL, run_pb_get_sim_pb_init_info, NULL },
	{ NULL, NULL, },
};

/*static struct menu_data menu_pb_select_sim_pb[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim, 4:aas, 5:gas", NULL, NULL, data_pb_type},
	{ "2", "run", NULL, run_pb_select_sim_pb, NULL },
	{ NULL, NULL, },
};*/

static struct menu_data menu_pb_get_sim_pb_count[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim, 4:aas, 5:gas", NULL, NULL, data_pb_type },
	{ "2", "run", NULL, run_pb_get_sim_pb_count, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_get_sim_pb_meta_info[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim, 4:aas, 5:gas", NULL, NULL, data_pb_type },
	{ "2", "run", NULL, run_pb_get_sim_pb_meta_info, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_get_sim_pb_usim_meta_info[] = {
	{ "1", "run", NULL, run_pb_get_sim_pb_usim_meta_info, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_read_sim_pb_record[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim, 4:aas, 5:gas", NULL, NULL, data_pb_type },
	{ "2", "index", NULL, NULL, data_pb_index },
	{ "3", "run", NULL, run_pb_read_sim_pb_record, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_update_sim_pb_record[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim, 4:aas, 5:gas", NULL, NULL, data_pb_type },
	{ "2", "index", NULL, NULL, data_pb_index },
	{ "3", "name", NULL, NULL, data_pb_name },
	{ "4", "dcs", NULL, NULL, data_pb_dcs },
	{ "5", "number", NULL, NULL, data_pb_number },
	{ "6", "ton", NULL, NULL, data_pb_ton },
	{ "7", "anr1", NULL, NULL, data_pb_anr1 },
	{ "8", "anr1_ton", NULL, NULL, data_pb_anr1_ton },
	{ "9", "anr2", NULL, NULL, data_pb_anr2 },
	{ "10", "anr2_ton", NULL, NULL, data_pb_anr2_ton },
	{ "11", "anr3", NULL, NULL, data_pb_anr3 },
	{ "12", "anr3_ton", NULL, NULL, data_pb_anr3_ton },
	{ "13", "email1", NULL, NULL, data_pb_email1 },
	{ "14", "email2", NULL, NULL, data_pb_email2 },
	{ "15", "email3", NULL, NULL, data_pb_email3 },
	{ "16", "email4", NULL, NULL, data_pb_email4 },
	{ "17", "group_index", NULL, NULL, data_pb_group_index },
	{ "18", "pb_control", NULL, NULL, data_pb_pb_control },
	{ "19", "run", NULL, run_pb_update_sim_pb_record, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_delete_sim_pb_record[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim, 4:aas, 5:gas", NULL, NULL, data_pb_type },
	{ "2", "index", NULL, NULL, data_pb_index },
	{ "3", "run", NULL, run_pb_delete_sim_pb_record, NULL },
	{ NULL, NULL, },
};

struct menu_data menu_phonebook_common[] = {
	{ "1", "phonebook init info", menu_pb_get_sim_pb_init_info, NULL, NULL },
	{ "2", "get pb count", menu_pb_get_sim_pb_count, NULL, NULL },
	{ "3", "get pb meta info", menu_pb_get_sim_pb_meta_info, NULL, NULL },
	{ "4", "get usim pb meta info", menu_pb_get_sim_pb_usim_meta_info, NULL, NULL },
	{ "5", "read record", menu_pb_read_sim_pb_record, NULL, NULL },
	{ "6", "add/update record", menu_pb_update_sim_pb_record, NULL, NULL },
	{ "7", "delete record", menu_pb_delete_sim_pb_record, NULL, NULL },
	{ NULL, NULL, },
};

struct menu_data menu_phonebook[] = {
	{"1", "3GPP(WCDMA/GSM/LTE)" , menu_phonebook_common, NULL, NULL},
	{"2", "3GPP2(CDMA)" , menu_phonebook_common, NULL, NULL},
	{ NULL, NULL, },
};


void register_phonebook_event(TapiHandle *handle)
{
	int ret;

	/* PHONEBOOK */
	ret = tel_register_noti_event(handle, TAPI_NOTI_PB_STATUS, on_noti_pb_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_PB_CONTACT_CHANGE, on_noti_pb_contact_change, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);
}
