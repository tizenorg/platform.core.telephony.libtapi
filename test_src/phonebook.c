/*
 * libtapi
 * Telephony test application
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
#include <tapi_phonebook.h>

#include "menu.h"
#include "sim.h"
#include "phonebook.h"

#define CHECK_PB_RESULT(x) \
{ \
	if (x >= TEL_PB_RESULT_SUCCESS && x <= TEL_PB_RESULT_ACCESS_RESTRICTED) { \
		msg("result[%d][%s]", x, dbg_pb_result[x]); \
	} else { \
		msg("Api failed with result: [%d]", x); \
	} \
	if (x != TEL_PB_RESULT_SUCCESS) \
		return; \
}

#define MSG_PB_TYPE(x) \
{ \
	if (x <= TEL_PB_USIM) { \
		msg("pb_type[%d][%s]", x, dbg_pb_type_name[x]); \
	} else { \
		msg("pb_type[%d]", x); \
	} \
}

static char *dbg_pb_type_name[] = { "TEL_PB_FDN", "TEL_PB_ADN", "TEL_PB_SDN",
		"TEL_PB_USIM", };

static char *dbg_pb_result[] = {"TEL_PB_RESULT_SUCCESS", "TEL_PB_RESULT_FAILURE",
	"TEL_PB_RESULT_INVALID_PARAMETER", "TEL_PB_RESULT_MEMORY_FAILURE",
	"TEL_PB_RESULT_OPERATION_NOT_SUPPORTED", "TEL_PB_RESULT_UNKNOWN_FAILURE",
	"TEL_PB_RESULT_INVALID_INDEX", "TEL_PB_RESULT_NOT_INITIALIZED",
	"TEL_PB_RESULT_PIN2_REQUIRED", "TEL_PB_RESULT_PUK2_REQUIRED",
	"TEL_PB_RESULT_ACCESS_RESTRICTED", };

static char data_pb_type[MENU_DATA_SIZE + 1] = {};
static char data_pb_index[MENU_DATA_SIZE + 1] = {};
static char data_pb_name[MENU_DATA_SIZE + 1] = {};
static char data_pb_number[MENU_DATA_SIZE + 1] = {};
static char data_pb_sne[MENU_DATA_SIZE + 1] = {};
static char data_pb_group_name[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr_count[MENU_DATA_SIZE + 1] = "0";
static char data_pb_anr1_num[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr1_desc[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr1_aas[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr2_num[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr2_desc[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr2_aas[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr3_num[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr3_desc[MENU_DATA_SIZE + 1] = {};
static char data_pb_anr3_aas[MENU_DATA_SIZE + 1] = {};
static char data_pb_email_count[MENU_DATA_SIZE + 1] = "0";
static char data_pb_email1[MENU_DATA_SIZE + 1] = {};
static char data_pb_email2[MENU_DATA_SIZE + 1] = {};
static char data_pb_email3[MENU_DATA_SIZE + 1] = {};
static char data_pb_email4[MENU_DATA_SIZE + 1] = {};
static char data_pb_hidden[MENU_DATA_SIZE + 1] = "0";

static void on_noti_pb_status(TelHandle *handle, const char *event_id, void *data, void *user_data)
{
	gboolean *status = data;
	msgb("event(%s) received !!", TEL_NOTI_PB_STATUS);
	if (!status) {
		msg("Phonebook status notification data is null");
		return;
	}
	msg("pb_status[%s]", (*status == TRUE) ? "PB INIT DONE" : "PB INIT NOT DONE");
}

static int run_pb_get_sim_pb_init_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	gboolean init_completed = 0;
	TelPbList list;

	msg("call tapi_pb_get_sim_pb_init_info()");

	memset(&list, 0, sizeof(TelPbList));

	rt = tapi_pb_get_sim_pb_init_info(handle, &init_completed, &list);
	CHECK_RT(rt);

	msg("status[%d]", init_completed);
	msg("fdn[%d]", list.fdn);
	msg("adn[%d]", list.adn);
	msg("sdn[%d]", list.sdn);
	msg("3g usim[%d]", list.usim);

	return 0;
}

static void on_pb_get_sim_pb_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelPbInfo *pb_info = data;

	msg("tapi_pb_get_sim_pb_info() response received");
	CHECK_PB_RESULT(result);

	if (!pb_info) {
		msg("sim pb info response data is null");
		return;
	}

	MSG_PB_TYPE(pb_info->pb_type);
	if (pb_info->pb_type > TEL_PB_USIM) {
		msg("Invalid pb type [%d]", pb_info->pb_type);
		return;
	}

	if (pb_info->pb_type == TEL_PB_USIM) {
		msg("max_count[%d]", pb_info->info_u.usim.max_count);
		msg("used_count[%d]", pb_info->info_u.usim.used_count);
		msg("max_text_len[%d]", pb_info->info_u.usim.max_text_len);
		msg("max_anr_count[%d]", pb_info->info_u.usim.max_anr_count);
		msg("max_anr_len[%d]", pb_info->info_u.usim.max_anr_len);
		msg("max_email_count[%d]", pb_info->info_u.usim.max_email_count);
		msg("max_email_len[%d]", pb_info->info_u.usim.max_email_len);
		msg("max_sne_len[%d]", pb_info->info_u.usim.max_sne_len);
		msg("max_gas_len[%d]", pb_info->info_u.usim.max_gas_len);
		msg("max_aas_len[%d]", pb_info->info_u.usim.max_aas_len);
	} else {
		msg("max_count[%d]", pb_info->info_u.sim.max_count);
		msg("used_count[%d]", pb_info->info_u.sim.used_count);
		msg("max_num_len[%d]", pb_info->info_u.sim.max_num_len);
		msg("max_text_len[%d]", pb_info->info_u.sim.max_text_len);
	}
}

static int run_pb_get_sim_pb_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelPbType pb_type = atoi(data_pb_type);
	TelReturn rt = 0;

	msg("call tapi_pb_get_sim_pb_info()");

	MSG_PB_TYPE(pb_type);

	rt = tapi_pb_get_sim_pb_info(handle, pb_type, on_pb_get_sim_pb_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_pb_read_sim_pb_record(TelHandle *handle, int result, void *data, void *user_data)
{
	TelPbReadRecord *pb_rec = data;
	unsigned char i;

	msg("tapi_pb_read_sim_pb_record() response received");
	CHECK_PB_RESULT(result);

	if (!pb_rec) {
		msg("pb read record response data is null");
		return;
	}

	MSG_PB_TYPE(pb_rec->pb_type);
	if (pb_rec->pb_type > TEL_PB_USIM) {
		msg("Invalid pb type [%d]", pb_rec->pb_type);
		return;
	}

	msg("index[%d]", pb_rec->index);
	msg("next_index[%d]", pb_rec->next_index);

	if (pb_rec->pb_type != TEL_PB_USIM) {
		msg("name[%s]", pb_rec->rec_u.sim.name);
		msg("number[%s]", pb_rec->rec_u.sim.number);
		return;
	}

	msg("name[%s]", pb_rec->rec_u.usim.name);
	msg("number[%s]", pb_rec->rec_u.usim.number);
	msg("sne[%s]", pb_rec->rec_u.usim.sne);
	msg("grp_name[%s]", pb_rec->rec_u.usim.grp_name);

	msg("anr_count[%d]", pb_rec->rec_u.usim.anr_count);
	for(i = 0; i < pb_rec->rec_u.usim.anr_count; i++)
		msg("[%d]anr_num[%s] anr_desc[%d] anr_aas[%s]", i, pb_rec->rec_u.usim.anr[i].number,
			pb_rec->rec_u.usim.anr[i].description, pb_rec->rec_u.usim.anr[i].aas);

	msg("email_count[%d]", pb_rec->rec_u.usim.email_count);
	for(i = 0; i < pb_rec->rec_u.usim.email_count; i++)
		msg("email[%d][%s]", i, pb_rec->rec_u.usim.email[i]);
	msg("hidden[%d]", pb_rec->rec_u.usim.hidden);
}

static int run_pb_read_sim_pb_record(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelPbRecordInfo record;
	TelReturn rt = 0;

	msg("call tapi_pb_read_sim_pb_record()");

	memset(&record, 0, sizeof(TelPbRecordInfo));

	record.pb_type = atoi(data_pb_type);
	record.index = atoi(data_pb_index);

	MSG_PB_TYPE(record.pb_type);
	msg("pb_index[%d]", record.index);

	rt = tapi_pb_read_sim_pb_record(handle, &record, on_pb_read_sim_pb_record, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_pb_update_sim_pb_record(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_pb_update_sim_pb_record() response received");
	CHECK_PB_RESULT(result);
}

static int run_pb_update_sim_pb_record(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelPbUpdateRecord record;
	TelReturn rt = 0;

	msg("call tapi_pb_update_sim_pb_record()");

	memset(&record, 0, sizeof(TelPbUpdateRecord));

	record.pb_type = atoi(data_pb_type);
	record.index = atoi(data_pb_index);

	MSG_PB_TYPE(record.pb_type);
	if (record.pb_type > TEL_PB_USIM) {
		msg("Invalid pb type [%d]", record.pb_type);
		return 0;
	}

	msg("pb_index[%d]", record.index);

	if (record.pb_type == TEL_PB_USIM) {
		g_strlcpy(record.rec_u.usim.name, data_pb_name, TEL_PB_TEXT_MAX_LEN+1);
		g_strlcpy(record.rec_u.usim.number, data_pb_number, TEL_PB_NUMBER_MAX_LEN+1);
		g_strlcpy(record.rec_u.usim.sne, data_pb_sne, TEL_PB_TEXT_MAX_LEN+1);
		g_strlcpy(record.rec_u.usim.grp_name, data_pb_group_name, TEL_PB_TEXT_MAX_LEN+1);

		record.rec_u.usim.anr_count = data_pb_anr_count[0] - '0';
		switch (record.rec_u.usim.anr_count) {
		case 3:
			g_strlcpy(record.rec_u.usim.anr[2].number, data_pb_anr3_num, TEL_PB_NUMBER_MAX_LEN+1);
			record.rec_u.usim.anr[2].description = data_pb_anr3_desc[0] - '0';
			g_strlcpy(record.rec_u.usim.anr[2].aas, data_pb_anr3_aas, TEL_PB_TEXT_MAX_LEN+1);
		case 2:	/* FALL THROUGH */
			g_strlcpy(record.rec_u.usim.anr[1].number, data_pb_anr2_num, TEL_PB_NUMBER_MAX_LEN+1);
			record.rec_u.usim.anr[1].description = data_pb_anr1_desc[0] - '0';
			g_strlcpy(record.rec_u.usim.anr[1].aas, data_pb_anr2_aas, TEL_PB_TEXT_MAX_LEN+1);
		case 1:	/* FALL THROUGH */
			g_strlcpy(record.rec_u.usim.anr[0].number, data_pb_anr1_num, TEL_PB_NUMBER_MAX_LEN+1);
			record.rec_u.usim.anr[0].description= data_pb_anr1_desc[0] - '0';
			g_strlcpy(record.rec_u.usim.anr[0].aas, data_pb_anr1_aas, TEL_PB_TEXT_MAX_LEN+1);
		default:
		break;
		}

		record.rec_u.usim.email_count = data_pb_email_count[0] - '0';
		switch (record.rec_u.usim.email_count) {
		case 4:
			g_strlcpy((char*)&record.rec_u.usim.email[3], data_pb_email4, TEL_PB_TEXT_MAX_LEN+1);
		case 3:
			g_strlcpy((char*)&record.rec_u.usim.email[2], data_pb_email3, TEL_PB_TEXT_MAX_LEN+1);
		case 2:
			g_strlcpy((char*)&record.rec_u.usim.email[1], data_pb_email2, TEL_PB_TEXT_MAX_LEN+1);
		case 1:
			g_strlcpy((char*)&record.rec_u.usim.email[0], data_pb_email1, TEL_PB_TEXT_MAX_LEN+1);
		default:
		break;
		}

		record.rec_u.usim.hidden = data_pb_hidden[0] - '0';
	}
	else {
		g_strlcpy(record.rec_u.sim.name, data_pb_name, TEL_PB_TEXT_MAX_LEN+1);
		g_strlcpy(record.rec_u.sim.number, data_pb_number, TEL_PB_NUMBER_MAX_LEN+1);
	}

	rt = tapi_pb_update_sim_pb_record(handle, &record, on_pb_update_sim_pb_record, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_pb_delete_sim_pb_record(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_pb_delete_sim_pb_record() response received");
	CHECK_PB_RESULT(result);
}

static int run_pb_delete_sim_pb_record(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelPbRecordInfo record;
	TelReturn rt = 0;

	msg("call tapi_pb_delete_sim_pb_record()");

	memset(&record, 0, sizeof(TelPbRecordInfo));

	record.pb_type = atoi(data_pb_type);
	record.index = atoi(data_pb_index);

	MSG_PB_TYPE(record.pb_type);
	msg("pb_index[%d]", record.index);

	rt = tapi_pb_delete_sim_pb_record(handle, &record, on_pb_delete_sim_pb_record, NULL);
	CHECK_RT(rt);

	return 0;
}

static struct menu_data menu_pb_get_sim_pb_init_info[] = {
	{ "1", "run", NULL, run_pb_get_sim_pb_init_info, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_get_sim_pb_info[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim", NULL, NULL, data_pb_type },
	{ "2", "run", NULL, run_pb_get_sim_pb_info,	NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_read_sim_pb_record[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim", NULL, NULL, data_pb_type },
	{ "2", "index", NULL, NULL, data_pb_index },
	{ "3", "run", NULL, run_pb_read_sim_pb_record,	NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_update_sim_pb_record[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim", NULL, NULL, data_pb_type },
	{ "2", "index", NULL, NULL, data_pb_index },
	{ "3", "name", NULL, NULL, data_pb_name },
	{ "4", "number", NULL, NULL, data_pb_number },
	{ "5", "sne", NULL, NULL, data_pb_sne },
	{ "6", "group_name", NULL, NULL, data_pb_group_name },
	{ "7", "anr_count", NULL, NULL, data_pb_anr_count },
	{ "8", "anr1_num", NULL, NULL, data_pb_anr1_num },
	{ "9", "anr1_desc", NULL, NULL, data_pb_anr1_desc },
	{ "10", "anr1_aas", NULL, NULL, data_pb_anr1_aas },
	{ "11", "anr2_num", NULL, NULL, data_pb_anr2_num },
	{ "12", "anr2_desc", NULL, NULL, data_pb_anr2_desc },
	{ "13", "anr2_aas", NULL, NULL, data_pb_anr2_aas },
	{ "14", "anr3_num", NULL, NULL, data_pb_anr3_num },
	{ "15", "anr3_desc", NULL, NULL, data_pb_anr3_desc },
	{ "16", "anr3_aas", NULL, NULL, data_pb_anr3_aas },
	{ "17", "email_count", NULL, NULL, data_pb_email_count },
	{ "18", "email1", NULL, NULL, data_pb_email1 },
	{ "19", "email2", NULL, NULL, data_pb_email2 },
	{ "20", "email3", NULL, NULL, data_pb_email3 },
	{ "21", "email4", NULL, NULL, data_pb_email4 },
	{ "22", "hidden", NULL, NULL, data_pb_hidden },
	{ "23", "run", NULL, run_pb_update_sim_pb_record, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_pb_delete_sim_pb_record[] = {
	{ "1", "type- 0:fdn, 1:adn, 2:sdn, 3:usim", NULL, NULL, data_pb_type },
	{ "2", "index", NULL, NULL, data_pb_index },
	{ "3", "run", NULL, run_pb_delete_sim_pb_record, NULL },
	{ NULL, NULL, },
};

struct menu_data menu_phonebook[] = {
	{ "1", "phonebook init info", menu_pb_get_sim_pb_init_info, NULL, NULL },
	{ "2", "get pb info", menu_pb_get_sim_pb_info, NULL, NULL },
	{ "3", "read record", menu_pb_read_sim_pb_record, NULL, NULL },
	{ "4", "add/update record", menu_pb_update_sim_pb_record, NULL, NULL },
	{ "5", "delete record", menu_pb_delete_sim_pb_record, NULL, NULL },
	{ NULL, NULL, },
};

void register_phonebook_event(TelHandle *handle)
{
	TelReturn ret;

	ret = tapi_register_event_id(handle, TEL_NOTI_PB_STATUS, on_noti_pb_status, NULL);
	if (ret != TEL_RETURN_SUCCESS) {
		msg("TEL_NOTI_PB_STATUS - event register failed(%d)", ret);
	}
}
