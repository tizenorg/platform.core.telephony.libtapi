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
#include <tapi_sim.h>
#include <tapi_events.h>

#include "menu.h"
#include "sim.h"

#define CHECK_SIM_RESULT(x) \
{ \
	if (x >= TEL_SIM_RESULT_SUCCESS && x <= TEL_SIM_RESULT_OPERATION_NOT_PERMITTED) { \
		msg("result[%d][%s]", x, dbg_sim_result[x]); \
	} else { \
		msg("API failed with result[%d]", x); \
	} \
	if (x != TEL_SIM_RESULT_SUCCESS) \
		return; \
} \

static char *dbg_sim_result[] = {
	"TEL_SIM_RESULT_SUCCESS",
	"TEL_SIM_RESULT_FAILURE",
	"TEL_SIM_RESULT_INVALID_PARAMETER",
	"TEL_SIM_RESULT_MEMORY_FAILURE",
	"TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED",
	"TEL_SIM_RESULT_CARD_ERROR",
	"TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED",
	"TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION",
	"TEL_SIM_RESULT_INCORRECT_PASSWORD",
	"TEL_SIM_RESULT_PIN_REQUIRED",
	"TEL_SIM_RESULT_PUK_REQUIRED",
	"TEL_SIM_RESULT_PERM_BLOCKED",
	"TEL_SIM_RESULT_SQN_FAILURE",
	"TEL_SIM_RESULT_OPERATION_NOT_PERMITTED",
};

static char data_sim_set_language[MENU_DATA_SIZE + 1] = { 0x01,}; //0x01 = English

static char data_sim_set_mw_profile_id[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mw_indicator_status[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mw_voice_cnt[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mw_fax_cnt[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mw_email_cnt[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mw_other_cnt[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mw_video_cnt[MENU_DATA_SIZE + 1] = {};

static char data_sim_set_mb_profile_id[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mb_type[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mb_alpha_id[MENU_DATA_SIZE + 1] = {};
static char data_sim_set_mb_number[MENU_DATA_SIZE + 1] = {};

static char data_sim_req_authentication_type[MENU_DATA_SIZE + 1] = { 0x01,}; //SIM_AUTH_TYPE_GSM
static char data_sim_req_authentication_autn_data[MENU_DATA_SIZE + 1] = { 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, };
static char data_sim_req_authentication_rand_data[MENU_DATA_SIZE + 1] = { 200, 200, 200,
	200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, 200, };

static char data_sim_verify_pins_type[MENU_DATA_SIZE + 1] = {};
static char data_sim_verify_pins_pw[MENU_DATA_SIZE + 1] = {};

static char data_sim_verify_puks_type[MENU_DATA_SIZE + 1] = {};
static char data_sim_verify_puks_puk_pw[MENU_DATA_SIZE + 1] = {};
static char data_sim_verify_puks_pin[MENU_DATA_SIZE + 1] = {};

static char data_sim_change_pins_type[MENU_DATA_SIZE + 1] = {};
static char data_sim_change_pins_old_pw[MENU_DATA_SIZE + 1] = {};
static char data_sim_change_pins_new_pw[MENU_DATA_SIZE + 1] = {};

static char data_sim_facility_type[MENU_DATA_SIZE + 1] = {};
static char data_sim_facility_pw[MENU_DATA_SIZE + 1] = {};
static char data_sim_apdu[MENU_DATA_SIZE + 1] = {};

static int _sim_str_to_hex(char a)
{
	int ret;

	if (a >= '0' && a <= '9')
		ret = a - '0';
	else if (a >= 'a' && a <= 'f')
		ret = a - 'a' + 10;
	else if (a >= 'A' && a <= 'F')
		ret = a - 'A' + 10;
	else
		ret = -1;
	return ret;
}

/* Event Callback */
static void on_noti_sim_status(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSimCardStatusInfo *status = data;

	msgb("event(%s) received !!", TEL_NOTI_SIM_STATUS);

	if (!status) {
		msg("sim card status notification data is null");
		return;
	}
	msg("status[%d]", status->status);
	msg("change_status[%d]", status->change_status);
}

/* Requests and Responses */
static int run_sim_get_init_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimCardStatusInfo sim_status_info;

	msg("call tapi_sim_get_init_info()");

	memset(&sim_status_info, 0, sizeof(TelSimCardStatusInfo));

	rt = tapi_sim_get_init_info(handle, &sim_status_info);
	CHECK_RT(rt);

	msg("status[%d]", sim_status_info.status);
	msg("change_status[%d]", sim_status_info.change_status);

	return 0;
}

static int run_sim_get_card_type(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimCardType sim_type = 0;

	msg("call tapi_sim_get_type()");

	rt = tapi_sim_get_type(handle, &sim_type);
	CHECK_RT(rt);

	msg("sim card type[%d]", sim_type);

	return 0;
}

static int run_sim_get_imsi(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimImsiInfo imsi;

	msg("call tapi_sim_get_imsi()");

	memset(&imsi, 0, sizeof(TelSimImsiInfo));

	rt = tapi_sim_get_imsi(handle, &imsi);
	CHECK_RT(rt);

	msg("mcc[%s]", imsi.mcc);
	msg("mnc[%s]", imsi.mnc);
	msg("msin[%s]", imsi.msin);

	return 0;
}

static int run_sim_get_ecc(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimEccList ecc_list;
	unsigned int i =0;

	msg("call tapi_sim_get_ecc()");

	memset(&ecc_list, 0, sizeof(TelSimEccList));

	rt = tapi_sim_get_ecc(handle, &ecc_list);
	CHECK_RT(rt);

	msg("ecc_count[%d]",ecc_list.count);

	for(i = 0; i < ecc_list.count; i++) {
		msg("[%d].name[%s] number[%s] category[%d]", i+1, ecc_list.list[i].name,
			ecc_list.list[i].number, ecc_list.list[i].category);
	}

	return 0;
}

static void on_sim_get_iccid(TelHandle *handle, int result, void *data, void *user_data)
{
	gchar *iccid = data;

	msg("tapi_sim_get_iccid() response received");
	CHECK_SIM_RESULT(result);

	if (!iccid) {
		msg("get sim iccid response data is null");
		return;
	}

	msg("iccid[%s]", iccid);
}

static int run_sim_get_iccid(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_iccid()");

	rt = tapi_sim_get_iccid(handle, on_sim_get_iccid, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_language(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimLanguagePreferenceCode *language = data;

	msg("tapi_sim_get_language() response received");
	CHECK_SIM_RESULT(result);

	if (!language) {
		msg("get sim language response data is null");
		return;
	}

	msg("language[%d]", *language);
}

static int run_sim_get_language(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_language()");

	rt = tapi_sim_get_language(handle, on_sim_get_language, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_set_language(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sim_set_language() response received");
	CHECK_SIM_RESULT(result);
}

static int run_sim_set_language(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelSimLanguagePreferenceCode language;
	TelReturn rt = 0;

	msg("call tapi_sim_set_language()");

	language = atoi(data_sim_set_language);
	msg("Language selected: [%d]", language);

	rt = tapi_sim_set_language(handle, language, on_sim_set_language, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_cf_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimCfisList *cf_info = data;
	unsigned int i = 0;

	msg("tapi_sim_get_callforwarding_info() response received");
	CHECK_SIM_RESULT(result);

	if (!cf_info) {
		msg("get cf info response data is null");
		return;
	}

	msg("cf_info.profile_count[%d]", cf_info->profile_count);

	for(i = 0; i < cf_info->profile_count; i++) {
		msg("[%d].profile_id[%d] ", i+1, cf_info->cf[i].profile_id);
		if(cf_info->cf[i].indication[0])
			msg(" Voice type ");
		if(cf_info->cf[i].indication[1])
			msg(" Fax type ");
		if(cf_info->cf[i].indication[2])
			msg(" Email type ");
		if(cf_info->cf[i].indication[3])
			msg(" Other type ");
		if(cf_info->cf[i].indication[4])
			msg(" Video type ");
	}
}

static int run_sim_get_cf_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_callforwarding_info()");

	rt = tapi_sim_get_callforwarding_info(handle, on_sim_get_cf_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_set_mw_info(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sim_set_messagewaiting_info() response received");
	CHECK_SIM_RESULT(result);
}

static int run_sim_set_mw_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimMwis mw_info;
	unsigned char indicator_status = 0;
	unsigned char voice_count, fax_count, email_count, other_count, video_count;
	unsigned char mask = 0x01;
	int i = 0;

	msg("call tapi_sim_set_messagewaiting_info()");

	memset(&mw_info, 0, sizeof(TelSimMwis));

	mw_info.profile_id = atoi(data_sim_set_mw_profile_id);
	indicator_status = atoi(data_sim_set_mw_indicator_status);
	voice_count = atoi(data_sim_set_mw_voice_cnt);
	fax_count = atoi(data_sim_set_mw_fax_cnt);
	email_count = atoi(data_sim_set_mw_email_cnt);
	other_count = atoi(data_sim_set_mw_other_cnt);
	video_count = atoi(data_sim_set_mw_video_cnt);

	msg("profile id[%d]", mw_info.profile_id);
	msg("indicator status[%d]", indicator_status);
	msg("voice count[%d]", voice_count);
	msg("fax count[%d]", fax_count);
	msg("email count[%d]", email_count);
	msg("other count[%d]", other_count);
	msg("video count[%d]", video_count);

	mw_info.count_valid = TRUE;

	if (indicator_status) {
		for (i = 0; i < 5; i++) {
			switch (indicator_status & mask) {
			case TEL_SIM_MWIS_VOICE:
				mw_info.msg_waiting[0].indication = TRUE;
				mw_info.msg_waiting[0].count = voice_count;
			break;
			case TEL_SIM_MWIS_FAX:
				mw_info.msg_waiting[1].indication = TRUE;
				mw_info.msg_waiting[1].count = fax_count;
			break;
			case TEL_SIM_MWIS_EMAIL:
				mw_info.msg_waiting[2].indication = TRUE;
				mw_info.msg_waiting[2].count = email_count;
			break;
			case TEL_SIM_MWIS_OTHER:
				mw_info.msg_waiting[3].indication = TRUE;
				mw_info.msg_waiting[3].count = other_count;
			break;
			case TEL_SIM_MWIS_VIDEO:
				mw_info.msg_waiting[4].indication = TRUE;
				mw_info.msg_waiting[4].count = video_count;
			break;
			default:
				msg("MWIS is NONE");
			}
			mask = mask << 1;
		}
	}

	rt = tapi_sim_set_messagewaiting_info(handle, &mw_info, on_sim_set_mw_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_mw_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimMwisList *mw_info = data;
	unsigned int i = 0;

	msg("tapi_sim_get_messagewaiting_info() response received");
	CHECK_SIM_RESULT(result);

	if (!mw_info) {
		msg("get msg waiting info response data is null");
		return;
	}

	msg("mw_info.profile_count: [%d]", mw_info->profile_count);

	for(i = 0; i < mw_info->profile_count; i++) {
		msg("[%d].profile_id[%d] count_valid[%d] ", i+1, mw_info->mw[i].profile_id, mw_info->mw[i].count_valid);
		if(mw_info->mw[i].msg_waiting[0].indication)
			msg(" Voice type and count:[%d]", mw_info->mw[i].msg_waiting[0].count);
		if(mw_info->mw[i].msg_waiting[1].indication)
			msg(" Fax type and count:[%d]", mw_info->mw[i].msg_waiting[1].count);
		if(mw_info->mw[i].msg_waiting[2].indication)
			msg(" Email type and count:[%d]", mw_info->mw[i].msg_waiting[2].count);
		if(mw_info->mw[i].msg_waiting[3].indication)
			msg(" Other type and count:[%d]", mw_info->mw[i].msg_waiting[3].count);
		if(mw_info->mw[i].msg_waiting[4].indication)
			msg(" Video type and count:[%d]", mw_info->mw[i].msg_waiting[4].count);
	}
}

static int run_sim_get_mw_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_messagewaiting_info()");

	rt = tapi_sim_get_messagewaiting_info(handle, on_sim_get_mw_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_set_mb_info(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sim_set_mailbox_info() response received");
	CHECK_SIM_RESULT(result);
}

static int run_sim_set_mb_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimMailBoxNumber mb_info;

	msg("call tapi_sim_set_mailbox_info()");

	memset(&mb_info, 0, sizeof(TelSimMailBoxNumber));

	mb_info.profile_id = atoi(data_sim_set_mb_profile_id);
	mb_info.mb_type = atoi(data_sim_set_mb_type);
	mb_info.alpha_id_len = strlen(data_sim_set_mb_alpha_id);
	if (mb_info.alpha_id_len > TEL_SIM_ALPHA_ID_LEN_MAX) {
		msg("Alpha id input too long");
		return 0;
	}
	memcpy(mb_info.alpha_id, data_sim_set_mb_alpha_id, mb_info.alpha_id_len);

	if (strlen(data_sim_set_mb_number) > TEL_SIM_MBDN_NUM_LEN_MAX) {
		msg("Mail box number input too long");
		return 0;
	}
	memcpy(mb_info.number, data_sim_set_mb_number, TEL_SIM_MBDN_NUM_LEN_MAX);

	msg("profile id[%d]", mb_info.profile_id);
	msg("mb type[%d]", mb_info.mb_type);
	msg("alpha id[%s]", mb_info.alpha_id);
	msg("number[%s]", mb_info.number);

	rt = tapi_sim_set_mailbox_info(handle, &mb_info, on_sim_set_mb_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_mb_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimMailboxList *mb_list = data;
	unsigned int i = 0;

	msg("tapi_sim_get_mailbox_info() response received");
	CHECK_SIM_RESULT(result);

	if (!mb_list) {
		msg("get mailbox info response data is null");
		return;
	}

	msg("alpha id max len: [%d]", mb_list->alpha_id_max_len);
	msg("mb list count: [%d]", mb_list->count);

	for (i = 0; i < mb_list->count; i++) {
		msg("[%d].profile_id[%d] mb_type[%d] alpha_id length[%d] alpha_id[%s] number[%s]", i+1,
			mb_list->list[i].profile_id, mb_list->list[i].mb_type, mb_list->list[i].alpha_id_len,
			mb_list->list[i].alpha_id, mb_list->list[i].number);
	}
}

static int run_sim_get_mb_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_mailbox_info()");

	rt = tapi_sim_get_mailbox_info(handle, on_sim_get_mb_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_msisdn(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimMsisdnList *msisdn_list = data;
	unsigned int i =0;

	msg("tapi_sim_get_msisdn() response received");
	CHECK_SIM_RESULT(result);

	if (!msisdn_list) {
		msg("get msisdn response data is null");
		return;
	}

	msg("msisdn list count[%d]", msisdn_list->count);

	for (i = 0; i < msisdn_list->count; i++)
		msg("[%d].alpha_id[%s] number[%s]", i+1, msisdn_list->list[i].alpha_id, msisdn_list->list[i].num);
}

static int run_sim_get_msisdn(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_msisdn()");

	rt = tapi_sim_get_msisdn(handle, on_sim_get_msisdn, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_spn(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimSpn *spn_info = data;

	msg("tapi_sim_get_spn() response received");
	CHECK_SIM_RESULT(result);

	if (!spn_info) {
		msg("get spn response data is null");
		return;
	}

	msg("spn display condition[%d]", spn_info->display_condition);
	msg("spn[%s]", spn_info->spn);
}

static int run_sim_get_spn(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_spn()");

	rt = tapi_sim_get_spn(handle, on_sim_get_spn, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_cphs_netname(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimCphsNetName *cphs_net = data;

	msg("tapi_sim_get_cphs_netname() response received");
	CHECK_SIM_RESULT(result);

	if (!cphs_net) {
		msg("sim get cphs netname response data is null");
		return;
	}

	msg("cphs full name[%s]", cphs_net->full_name);
	msg("cphs short name[%s]", cphs_net->short_name);
}

static int run_sim_get_cphs_netname(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_cphs_netname()");

	rt = tapi_sim_get_cphs_netname(handle, on_sim_get_cphs_netname, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_sp_display_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimSpPlmnList *sp_info = data;
	unsigned int i = 0;

	msg("tapi_sim_get_sp_display_info() response received");
	CHECK_SIM_RESULT(result);

	if (!sp_info) {
		msg("sim get sp display info response data is null");
		return;
	}

	msg("sp info count[%d]", sp_info->count);

	for(i = 0; i < sp_info->count; i++)
		msg("[%d].index[%d] plmn[%s]", i+1, sp_info->list[i].index, sp_info->list[i].plmn);
}

static int run_sim_get_sp_display_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_get_sp_display_info()");

	rt = tapi_sim_get_sp_display_info(handle, on_sim_get_sp_display_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_req_authentication(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimAuthenticationResponse *auth_resp = data;
	unsigned int i = 0;

	msg("tapi_sim_req_authentication() response received");
	CHECK_SIM_RESULT(result);

	if (!auth_resp) {
		msg("sim authentication response data is null");
		return;
	}

	msg("auth_type[%d]", auth_resp->auth_type);
	msg("auth_result[%d]", auth_resp->detailed_result);

	msg("resp_length[%d]", auth_resp->resp_length);
	for( i = 0; i < auth_resp->resp_length; i++)
		msg("[%d].resp_data[0x%2x]", i+1, auth_resp->resp_data[i]);

	msg("authentication_key_length[%d]", auth_resp->authentication_key_length);
	for( i = 0; i < auth_resp->authentication_key_length; i++)
		msg("[%d].authentication_key[0x%2x]", i+1, auth_resp->authentication_key[i]);

	msg("cipher_length[%d]", auth_resp->cipher_length);
	for( i = 0; i < auth_resp->cipher_length; i++)
		msg("[%d].cipher_data[0x%2x]", i+1, auth_resp->cipher_data[i]);

	msg("integrity_length[%d]", auth_resp->integrity_length);
	for( i = 0; i < auth_resp->integrity_length; i++)
		msg("[%d].integrity_data[0x%2x]", i+1, auth_resp->integrity_data[i]);
}

static int run_sim_req_authentication(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimAuthenticationData auth_data;

	msg("call tapi_sim_req_authentication()");

	memset(&auth_data, 0, sizeof(TelSimAuthenticationData));

	auth_data.auth_type = atoi(data_sim_req_authentication_type);
	auth_data.autn_length = strlen(data_sim_req_authentication_autn_data);
	memcpy(auth_data.autn_data, data_sim_req_authentication_autn_data, auth_data.autn_length);
	auth_data.rand_length = strlen(data_sim_req_authentication_rand_data);
	memcpy(auth_data.rand_data, data_sim_req_authentication_rand_data, auth_data.rand_length);

	msg("auth_type[%d]", auth_data.auth_type);
	msg("autn_data[%s] autn_length[%d]", auth_data.autn_data, auth_data.autn_length);
	msg("rand_data[%s] rand_length[%d]", auth_data.rand_data, auth_data.rand_length);

	rt = tapi_sim_req_authentication(handle, &auth_data, on_sim_req_authentication, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_verify_pins(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimSecPinResult *verify_pin_resp = data;

	msg("tapi_sim_verify_pins() response received");
	CHECK_SIM_RESULT(result);

	if (!verify_pin_resp) {
		msg("sim verify pins response data is null");
		return;
	}

	msg("pin type[%d]", verify_pin_resp->pin_type);
	msg("No of tries left[%d]", verify_pin_resp->retry_count);
}

static int run_sim_verify_pins(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimSecPinPw pin_data;

	msg("call tapi_sim_verify_pins()");

	memset(&pin_data, 0, sizeof(TelSimSecPinPw));

	pin_data.pin_type = atoi(data_sim_verify_pins_type);
	pin_data.pw = (char *)data_sim_verify_pins_pw;

	msg("pin type[%d]", pin_data.pin_type);
	msg("password[%s]", pin_data.pw);

	rt = tapi_sim_verify_pins(handle, &pin_data, on_sim_verify_pins, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_verify_puks(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimSecPukResult *verify_puk_resp = data;

	msg("tapi_sim_verify_puks() response received");
	CHECK_SIM_RESULT(result);

	if (!verify_puk_resp) {
		msg("sim verify puks response data is null");
		return;
	}

	msg("puk type[%d]", verify_puk_resp->puk_type);
	msg("No of tries left[%d]", verify_puk_resp->retry_count);
}

static int run_sim_verify_puks(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimSecPukPw puk_data;

	msg("call tapi_sim_verify_puks()");

	memset(&puk_data, 0, sizeof(TelSimSecPukPw));

	puk_data.puk_type = atoi(data_sim_verify_puks_type);
	puk_data.puk_pw = (char *)data_sim_verify_puks_puk_pw;
	puk_data.new_pin_pw = (char *)data_sim_verify_puks_pin;

	msg("puk_type[%d]", puk_data.puk_type);
	msg("puk password[%s]", puk_data.puk_pw);
	msg("new pin password[%s]", puk_data.new_pin_pw);

	rt = tapi_sim_verify_puks(handle, &puk_data, on_sim_verify_puks, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_change_pins(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimSecPinResult *change_pin_resp = data;

	msg("tapi_sim_change_pins() response received");
	CHECK_SIM_RESULT(result);

	if (!change_pin_resp) {
		msg("sim change pins response data is null");
		return;
	}

	msg("pin type[%d]", change_pin_resp->pin_type);
	msg("No of tries left[%d]", change_pin_resp->retry_count);
}

static int run_sim_change_pins(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimSecChangePinPw pin_data;

	msg("call tapi_sim_change_pins()");

	memset(&pin_data, 0, sizeof(TelSimSecChangePinPw));

	pin_data.pin_type = atoi(data_sim_change_pins_type);
	pin_data.old_pw = (char *)data_sim_change_pins_old_pw;
	pin_data.new_pw = (char *)data_sim_change_pins_new_pw;

	msg("pin type[%d]", pin_data.pin_type);
	msg("old password[%s]", pin_data.old_pw);
	msg("new password[%s]", pin_data.new_pw);

	rt = tapi_sim_change_pins(handle, &pin_data, on_sim_change_pins, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_enable_or_disable_facility(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimFacilityResult *sim_facility_resp = data;

	msg("tapi_sim_enable/disable_facility() response received");
	CHECK_SIM_RESULT(result);

	if (!sim_facility_resp) {
		msg("sim enable/disable facility response data is null");
		return;
	}

	msg("lock_type[%d]", sim_facility_resp->type);
	msg("No of tries left[%d]", sim_facility_resp->retry_count);
}

static int run_sim_enable_facility(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimFacilityPw pw;

	msg("call tel_%s()", menu->title);

	memset(&pw, 0, sizeof(TelSimFacilityPw));

	pw.lock_type = atoi(data_sim_facility_type);
	pw.pw = (char *)data_sim_facility_pw;

	msg("lock_type[%d]", pw.lock_type);
	msg("password[%s]", pw.pw);

	rt = tapi_sim_enable_facility(handle, &pw, on_sim_enable_or_disable_facility, NULL);

	CHECK_RT(rt);

	return 0;
}

static int run_sim_disable_facility(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimFacilityPw pw;

	msg("call tel_%s()", menu->title);

	memset(&pw, 0, sizeof(TelSimFacilityPw));

	pw.lock_type = atoi(data_sim_facility_type);
	pw.pw = (char *)data_sim_facility_pw;

	msg("lock_type[%d]", pw.lock_type);
	msg("password[%s]", pw.pw);

	rt = tapi_sim_disable_facility(handle, &pw, on_sim_enable_or_disable_facility, NULL);

	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_facility(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimFacilityInfo *facility_info = data;

	msg("tapi_sim_get_facility() response received");
	CHECK_SIM_RESULT(result);

	if (!facility_info) {
		msg("get sim facility response data is null");
		return;
	}

	msg("lock_type[%d]", facility_info->type);
	msg("sim facility status[%d]", facility_info->f_status);
}

static int run_sim_get_facility(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimLockType lock_type;

	msg("call tapi_sim_get_facility()");

	lock_type = atoi(data_sim_facility_type);
	msg("lock_type[%d]", lock_type);

	rt = tapi_sim_get_facility(handle, lock_type, on_sim_get_facility, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_get_lock_info(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimLockInfo *lock_info = data;

	msg("tapi_sim_get_lock_info() response received");
	CHECK_SIM_RESULT(result);

	if (!lock_info) {
		msg("lock info response data is null");
		return;
	}

	msg("lock_type[%d]", lock_info->lock_type);
	msg("lock_status[%d]", lock_info->lock_status);
	msg("No of tries left[%d]", lock_info->retry_count);
}

static int run_sim_get_lock_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimLockType lock_type;

	msg("call tapi_sim_get_lock_info()");

	lock_type = atoi(data_sim_facility_type);
	msg("lock_type[%d]", lock_type);

	rt = tapi_sim_get_lock_info(handle, lock_type, on_sim_get_lock_info, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_request_apdu(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimApduResp *apdu_resp = data;
	unsigned int i = 0;

	msg("tapi_sim_req_apdu() response received");
	CHECK_SIM_RESULT(result);

	if (!apdu_resp) {
		msg("apdu response data is null");
		return;
	}

	msg("apdu_len[%d]", apdu_resp->apdu_resp_len);

	for(i = 0; i < apdu_resp->apdu_resp_len; i++)
		msg("[%d]`s byte is [0x%x]",i, apdu_resp->apdu_resp[i]);
}

static int run_sim_request_apdu(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;
	TelSimApdu apdu_data;
	char result_apdu[TEL_SIM_APDU_LEN_MAX +1] = {0,};
	unsigned int i, j;
	unsigned int len;

	memset(&apdu_data, 0, sizeof(TelSimApdu));

	msg("call tapi_sim_req_apdu()");

	len = strlen(data_sim_apdu);

	for (i = 0, j = 0; i < len; i += 2, j++)
		result_apdu[j] = (_sim_str_to_hex(data_sim_apdu[i]) << 4) | (_sim_str_to_hex(data_sim_apdu[i+1]));

	apdu_data.apdu_len = len/2;
	if (apdu_data.apdu_len > TEL_SIM_APDU_LEN_MAX) {
		msg("Input too long");
		return 0;
	}
	memcpy(apdu_data.apdu, result_apdu, apdu_data.apdu_len);

	for (i = 0; i < apdu_data.apdu_len; i++)
		msg("[%02d] = 0x%02x\n", i, apdu_data.apdu[i]);

	rt = tapi_sim_req_apdu(handle, &apdu_data, on_sim_request_apdu, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_sim_request_atr(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSimAtr *atr_resp = data;
	unsigned int i = 0;

	msg("tapi_sim_req_atr() response received");
	CHECK_SIM_RESULT(result);

	if (!atr_resp) {
		msg("atr response data is null");
		return;
	}

	msg("atr_len[%d]", atr_resp->atr_len);

	for(i = 0; i < atr_resp->atr_len; i++)
		msg("[%d]`s byte is [0x%x]", i, atr_resp->atr[i]);
}

static int run_sim_request_atr(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sim_req_atr()");

	rt = tapi_sim_req_atr(handle, on_sim_request_atr, NULL);
	CHECK_RT(rt);

	return 0;
}

/* SIM SubMenu */
static struct menu_data menu_sim_get_init_info[] = {
	{ "1", "run", NULL, run_sim_get_init_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_sim_get_card_type[] = {
	{ "1", "run", NULL, run_sim_get_card_type, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_sim_get_imsi[] = {
	{ "1", "run", NULL, run_sim_get_imsi, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_sim_get_ecc[] = {
	{ "1", "run", NULL, run_sim_get_ecc, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_sim_get_iccid[] = {
	{ "1", "run", NULL, run_sim_get_iccid, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_sim_set_language[] = {
	{"1", "select language (0=GERMAN 1=ENGLISH 2=ITALIAN 3=FRENCH 4=SPANISH 5=DUTCH 6=SWEDISH" \
		"7=DANISH 8=PORTUGUESE 9=FINNISH 10=NORWEGIAN 11=GREEK 12=TURKISH 13=HUNGARIAN 14=POLISH" \
		"15=KOREAN 16=CHINESE 17=RUSSIAN 18=JAPANESE 255=LANG_UNSPECIFIED)", NULL, NULL, data_sim_set_language},
	{"2", "run", NULL, run_sim_set_language, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_language[] = {
	{"1", "run", NULL, run_sim_get_language, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_cf_info[] = {
	{"1", "run", NULL, run_sim_get_cf_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_set_mw_info[] = {
	{"1", "profile id", NULL, NULL, data_sim_set_mw_profile_id},
	{"2", "Indicator status", NULL, NULL, data_sim_set_mw_indicator_status},
	{"3", "voice count", NULL, NULL, data_sim_set_mw_voice_cnt},
	{"4", "fax count", NULL, NULL, data_sim_set_mw_fax_cnt},
	{"6", "email count", NULL, NULL, data_sim_set_mw_email_cnt},
	{"7", "other count", NULL, NULL, data_sim_set_mw_other_cnt},
	{"8", "video count", NULL, NULL, data_sim_set_mw_video_cnt},
	{"9", "run", NULL, run_sim_set_mw_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_mw_info[] = {
	{"1", "run", NULL, run_sim_get_mw_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_set_mb_info[] = {
	{"1", "profile id", NULL, NULL, data_sim_set_mb_profile_id},
	{"2", "mailbox type (0=VOICE 1=FAX 2=EMAIL 3=OTHER 4=VIDEO)", NULL, NULL, data_sim_set_mb_type},
	{"3", "alpha id", NULL, NULL, data_sim_set_mb_alpha_id},
	{"4", "number", NULL, NULL, data_sim_set_mb_number},
	{"5", "run", NULL, run_sim_set_mb_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_mb_info[] = {
	{"1", "run", NULL, run_sim_get_mb_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_msisdn[] = {
	{"1", "run", NULL, run_sim_get_msisdn, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_spn[] = {
	{"1", "run", NULL, run_sim_get_spn, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_cphs_netname[] = {
	{"1", "run", NULL, run_sim_get_cphs_netname, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_sp_display_info[] = {
	{"1", "run", NULL, run_sim_get_sp_display_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_req_authentication[] = {
	{"1", "sim auth type (0=GSM 1=GSM_CTX 2=3G_CTX 3=GBA 4=GBA_NAF 5=IMS_AKA)",
		NULL, NULL, data_sim_req_authentication_type},
	{"2", "autn_data", NULL, NULL, data_sim_req_authentication_autn_data},
	{"3", "rand_data", NULL, NULL, data_sim_req_authentication_rand_data},
	{"4", "run", NULL, run_sim_req_authentication, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_verify_pins[] = {
	{"1", "pin type(0=PIN1, 1=PIN2)", NULL, NULL, data_sim_verify_pins_type},
	{"2", "password", NULL, NULL, data_sim_verify_pins_pw},
	{"3", "run", NULL, run_sim_verify_pins, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_verify_puks[] = {
	{"1", "puk type(0=PUK1, 1=PUK2)", NULL, NULL, data_sim_verify_puks_type},
	{"2", "puk password", NULL, NULL, data_sim_verify_puks_puk_pw},
	{"3", "pin", NULL, NULL, data_sim_verify_puks_pin},
	{"4", "run", NULL, run_sim_verify_puks, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_change_pins[] = {
	{"1", "pin type(0=PIN1, 1=PIN2)", NULL, NULL, data_sim_change_pins_type},
	{"2", "old_pw", NULL, NULL, data_sim_change_pins_old_pw},
	{"3", "new_pw", NULL, NULL, data_sim_change_pins_new_pw},
	{"4", "run", NULL, run_sim_change_pins, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_enable_facility[] = {
	{"1", "lock type(0=Phone-Sim, 1=Phone-First-Sim, 2=simlock, 3=fdnlock, 4=network, 5=network subset," \
		"6=service provider 7=corporate", NULL, NULL, data_sim_facility_type},
	{"2", "password", NULL, NULL, data_sim_facility_pw},
	{"3", "run", NULL, run_sim_enable_facility, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_disable_facility[] = {
	{"1", "lock type(0=Phone-Sim, 1=Phone-First-Sim, 2=simlock, 3=fdnlock, 4=network, 5=network subset," \
		"6=service provider 7=corporate", NULL, NULL, data_sim_facility_type},
	{"2", "password", NULL, NULL, data_sim_facility_pw},
	{"3", "run", NULL, run_sim_disable_facility, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_facility[] = {
	{"1", "lock type(0=Phone-Sim, 1=Phone-First-Sim, 2=simlock, 3=fdnlock, 4=network, 5=network subset," \
		"6=service provider 7=corporate", NULL, NULL, data_sim_facility_type},
	{"2", "run", NULL, run_sim_get_facility, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_get_lock_info[] = {
	{"1", "lock type(0=Phone-Sim, 1=Phone-First-Sim, 2=simlock, 3=fdnlock, 4=network, 5=network subset," \
		"6=service provider 7=corporate", NULL, NULL, data_sim_facility_type},
	{"2", "run", NULL, run_sim_get_lock_info, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_request_apdu[] = {
	{"1", "apdu", NULL, NULL, data_sim_apdu},
	{"2", "run", NULL, run_sim_request_apdu, NULL},
	{NULL, NULL},
};

static struct menu_data menu_sim_request_atr[] = {
	{"1", "run", NULL, run_sim_request_atr, NULL},
	{NULL, NULL},
};

/* SIM Menu */
struct menu_data menu_sim[] = {
	{"1g", "Get sim init info", menu_sim_get_init_info, NULL, NULL},
	{"2g", "Get sim card type", menu_sim_get_card_type, NULL, NULL},
	{"3g", "Get imsi", menu_sim_get_imsi, NULL, NULL},
	{"4g", "Get ecc", menu_sim_get_ecc, NULL, NULL},
	{"5g", "Get iccid", menu_sim_get_iccid, NULL, NULL},
	{"6s", "Set sim language", menu_sim_set_language, NULL, NULL},
	{"6g", "Get sim language", menu_sim_get_language, NULL, NULL},
	{"7g", "Get call forwarding info", menu_sim_get_cf_info, NULL, NULL},
	{"8s", "Set msg waiting info", menu_sim_set_mw_info, NULL, NULL},
	{"8g", "Get msg waiting info", menu_sim_get_mw_info, NULL, NULL},
	{"9s", "Set mailbox info", menu_sim_set_mb_info, NULL, NULL},
	{"9g", "Get mailbox info", menu_sim_get_mb_info, NULL, NULL},
	{"10g", "Get msisdn", menu_sim_get_msisdn, NULL, NULL},
	{"11g", "Get spn", menu_sim_get_spn, NULL, NULL},
	{"12g", "Get cphs network name", menu_sim_get_cphs_netname, NULL, NULL},
	{"13g", "Get sim service provider info", menu_sim_get_sp_display_info, NULL, NULL},
	{"14", "sim authentication", menu_sim_req_authentication, NULL, NULL},
	{"15", "verify pins", menu_sim_verify_pins, NULL, NULL},
	{"16", "verify puks", menu_sim_verify_puks, NULL, NULL},
	{"17", "change pins", menu_sim_change_pins, NULL, NULL},
	{"18", "disable_sim_facility", menu_sim_disable_facility, NULL, NULL},
	{"19", "enable_sim_facility", menu_sim_enable_facility, NULL, NULL},
	{"20g", "Get sim facility info", menu_sim_get_facility, NULL, NULL},
	{"21g", "Get sim lock info", menu_sim_get_lock_info, NULL, NULL},
	{"22", "request sim apdu", menu_sim_request_apdu, NULL, NULL},
	{"23", "request sim atr", menu_sim_request_atr, NULL, NULL},
	{ NULL, NULL, },
};

/* Register Events */
void register_sim_event(TelHandle *handle)
{
	TelReturn ret;

	ret = tapi_register_event_id(handle, TEL_NOTI_SIM_STATUS, on_noti_sim_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_SIM_STATUS - event register failed [%d]", ret);
}
