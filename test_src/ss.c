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
#include <ITapiSs.h>
#include <TapiUtility.h>

#include "menu.h"
#include "ss.h"

static char data_ss_set_barring_class[MENU_DATA_SIZE + 1] = "16";
static char data_ss_set_barring_mode[MENU_DATA_SIZE + 1] = "0";
static char data_ss_set_barring_type[MENU_DATA_SIZE + 1] = "1";
static char data_ss_set_barring_password[MENU_DATA_SIZE + 1] = "1111";

static char data_ss_get_barring_status_class[MENU_DATA_SIZE + 1] = "16";
static char data_ss_get_barring_status_type[MENU_DATA_SIZE + 1] = "1";

static char data_ss_change_barring_password_old[MENU_DATA_SIZE + 1] = "1111";
static char data_ss_change_barring_password_new[MENU_DATA_SIZE + 1] = "1234";

static char data_ss_set_forward_class[MENU_DATA_SIZE + 1] = "16";
static char data_ss_set_forward_mode[MENU_DATA_SIZE + 1] = "1";
static char data_ss_set_forward_condition[MENU_DATA_SIZE + 1] = "5";
static char data_ss_set_forward_timer[MENU_DATA_SIZE + 1] = "5";
static char data_ss_set_forward_number[MENU_DATA_SIZE + 1] = "01030018655";

static char data_ss_get_forward_status_class[MENU_DATA_SIZE + 1] = "16";
static char data_ss_get_forward_status_condition[MENU_DATA_SIZE + 1] = "1";

static char data_ss_set_waiting_class[MENU_DATA_SIZE + 1] = "16";
static char data_ss_set_waiting_mode[MENU_DATA_SIZE + 1] = "0";

static char data_ss_get_waiting_status_class[MENU_DATA_SIZE + 1] = "16";

static char data_ss_get_cli_status_type[MENU_DATA_SIZE + 1] = "1";
static char data_ss_set_cli_status_type[MENU_DATA_SIZE + 1] = "1";
static char data_ss_set_cli_status[MENU_DATA_SIZE + 1] = "1";

static char data_ss_ussd_request_type[MENU_DATA_SIZE + 1] = "1";
static char data_ss_ussd_request_string[MENU_DATA_SIZE + 1] = "";

static void on_noti_ss_ussd(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsUssdMsgInfo_t *noti = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SS_USSD);

	if (!noti)
		return;

	msg(" - Type = 0x%x", noti->Type);
	msg(" - Length = 0x%x", noti->Length);
	msg(" - szString = %s", noti->szString);
}

static void on_noti_ss_release_complete(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsRelCompMsgInfo_t *info = data;
	int i = 0;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SS_RELEASE_COMPLETE);


	msg(" - info->RelCompMsgLen = 0x%x", info->RelCompMsgLen);
	msg(" - info->szRelCompMsg = ");
	for ( i=0; i<info->RelCompMsgLen; i++ ) {
		msg("%d : [%x]", i, info->szRelCompMsg[i]);
	}
}

static void on_noti_ss_forwarding_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsForwardNoti_t *noti = data;
	int i =0;

	msg("SS Forwarding Status Noti");

	for ( i=0; i<noti->record_num; i++ ) {
		msg("class : %d", noti->record[i].Class);
		msg("status : %d", noti->record[i].Status);
		msg("condition : %d", noti->record[i].ForwardCondition);
		msg("number present : %d", noti->record[i].bCallForwardingNumberPresent);
		msg("no reply time : %d", noti->record[i].NoReplyWaitTime);
		msg("ton : %d", noti->record[i].Ton);
		msg("npi : %d", noti->record[i].Npi);
		msg("number : %s", noti->record[i].szCallForwardingNumber);
		msg("");
	}

}

static void on_noti_ss_barring_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsBarringNoti_t *noti = data;
	int i =0;

	msg("SS Barring Status Noti");

	for ( i=0; i<noti->record_num; i++ ) {
		msg("class : %d", noti->record[i].Class);
		msg("status : %d", noti->record[i].Status);
		msg("type : %d", noti->record[i].Flavour);
		msg("");
	}

}

static void on_noti_ss_waiting_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsWaitingNoti_t *noti = data;
	int i =0;

	msg("SS Waiting Status Noti");

	for ( i=0; i<noti->record_num; i++ ) {
		msg("class : %d", noti->record[i].Class);
		msg("status : %d", noti->record[i].Status);
		msg("");
	}
}

static void on_noti_ss_sups_info(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsInfo_t *noti = data;

	msg("SS SUPS Info Noti");

	msg("SS error : %d", noti->Cause);
	msg("status : %d", noti->SsType);
	msg("");
}

static void on_ss_barring(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSsBarringResp_t *resp = data;
	int i = 0;

	msg("");
	msgb("tel_set_ss_barring() or tel_get_ss_barring_status() response receive");
	msg(" - result = 0x%x", result);

	if (!resp)
		return;

	msg(" - record_num = %d", resp->record_num);

	for (i = 0; i < resp->record_num; i++) {
		msg(" - [%d] Class=%d, Status=%d, Flavour=%d",
				i,
				resp->record[i].Class,
				resp->record[i].Status,
				resp->record[i].Flavour);
	}
}

static void on_ss_change_barring_password(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_change_ss_barring_password() response receive");
	msg(" - result = 0x%x", result);
}

static void on_ss_forward(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSsForwardResp_t *resp = data;
	int i;

	msg("");
	msgb("tel_set_ss_forward() or tel_get_ss_forward_status() response receive");
	msg(" - result = 0x%x", result);

	if (!resp)
		return;

	msg(" - record_num = %d", resp->record_num);

	for (i = 0; i < resp->record_num; i++) {
		msg(" - [%d] Class=%d, Status=%d, ForwardCondition=%d, NoReplyWaitTime=%d, bCallForwardingNumberPresend=%d, szCallForwardingNumber=[%s]",
				i,
				resp->record[i].Class,
				resp->record[i].Status,
				resp->record[i].ForwardCondition,
				resp->record[i].NoReplyWaitTime,
				resp->record[i].bCallForwardingNumberPresent,
				resp->record[i].szCallForwardingNumber);
	}
}

static void on_ss_waiting(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSsWaitingResp_t *resp = data;
	int i;

	msg("");
	msgb("tel_set_ss_waiting() or tel_get_ss_waiting_status() response receive");
	msg(" - result = 0x%x", result);

	if (!resp)
		return;

	msg(" - record_num = %d", resp->record_num);

	for (i = 0; i < resp->record_num; i++) {
		msg(" - [%d] Class=%d, Status=%d",
				i,
				resp->record[i].Class,
				resp->record[i].Status);
	}
}

static void on_ss_set_cli_status(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_ss_cli_status() response receive");
	msg(" - result = 0x%x", result);
	return;
}

static void on_ss_get_cli_status(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSsCliResp_t *resp = data;

	msg("");
	msgb("tel_get_ss_cli_status() response receive");
	msg(" - result = 0x%x", result);

	if (!resp)
		return;

	msg(" - type = %d", resp->Type);
	msg(" - status = %d", resp->Status);
}

static void on_ss_send_ussd_request(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelSsUssdResp_t *resp = data;

	msg("");
	msgb("tel_send_ss_ussd_request() response receive");
	msg(" - result = 0x%x", result);

	if (!resp)
		return;

	msg(" - type = %d", resp->Type);
	msg(" - status = %d", resp->Status);
	msg(" - length = %d", resp->Length);
	msg(" - string = [%s]", resp->szString);
}

static int run_ss_set_barring(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsBarringInfo_t info;

	msg("call tel_set_ss_barring()");

	memset(&info, 0, sizeof(TelSsBarringInfo_t));
	info.Class = atoi(data_ss_set_barring_class);
	info.Mode = atoi(data_ss_set_barring_mode);
	info.Type = atoi(data_ss_set_barring_type);
	g_strlcpy(info.szPassword, data_ss_set_barring_password, TAPI_SS_GSM_BARR_PW_LEN_MAX+1);

	msg("info.szPassword - %s data_ss_set_barring_password- %s\n", info.szPassword, data_ss_set_barring_password);
	msg("Call barring Mode [%d]\n", info.Mode);

	result = tel_set_ss_barring(handle, &info, on_ss_barring, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_get_barring_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsClass_t class;
	TelSsBarringType_t type;

	msg("call tel_get_ss_barring_status()");

	class = atoi(data_ss_get_barring_status_class);
	type = atoi(data_ss_get_barring_status_type);

	result = tel_get_ss_barring_status(handle, class, type, on_ss_barring, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_change_barring_password(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_change_ss_barring_password()");

	result = tel_change_ss_barring_password(handle,
			data_ss_change_barring_password_old,
			data_ss_change_barring_password_new,
			data_ss_change_barring_password_new,
			on_ss_change_barring_password, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_set_forward(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsForwardInfo_t info;
	int len;

	msg("call tel_set_ss_forward()");

	len = strlen(data_ss_set_forward_number);
	if (len >= TAPI_CALL_DIALDIGIT_LEN_MAX - 1) {
		msg("failed. forward_number too long");
		return 0;
	}

	memset(&info, 0, sizeof(TelSsForwardInfo_t));
	info.Class = atoi(data_ss_set_forward_class);
	info.Condition = atoi(data_ss_set_forward_condition);
	info.NoReplyConditionTimer = atoi(data_ss_set_forward_timer);
	info.Mode = atoi(data_ss_set_forward_mode);
	info.Ton = 0x01;
	info.Npi = 0x01;
	strncpy((char*)info.szPhoneNumber, data_ss_set_forward_number, len);
	info.szPhoneNumber[len] = '\0';

	result = tel_set_ss_forward(handle, &info, on_ss_forward, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_get_forward_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsClass_t class;
	TelSsForwardWhen_t condition;

	msg("call tel_get_ss_forward_status()");

	class = atoi(data_ss_get_forward_status_class);
	condition = atoi(data_ss_get_forward_status_condition);

	result = tel_get_ss_forward_status(handle, class, condition, on_ss_forward, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_set_waiting(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsWaitingInfo_t info;

	msg("call tel_set_ss_waiting()");

	memset(&info, 0, sizeof(TelSsWaitingInfo_t));
	info.Class = atoi(data_ss_set_waiting_class);
	info.Mode = atoi(data_ss_set_waiting_mode);

	result = tel_set_ss_waiting(handle, &info, on_ss_waiting, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_get_waiting_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsClass_t class;

	msg("call tel_get_ss_waiting_status()");

	class = atoi(data_ss_get_waiting_status_class);

	result = tel_get_ss_waiting_status(handle, class, on_ss_waiting, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_set_cli_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsCliType_t type;
	TelSsCliStatus_t status;

	msg("call tel_set_ss_cli_status()");

	type = atoi(data_ss_set_cli_status_type);
	status  = atoi(data_ss_set_cli_status);

	result = tel_set_ss_cli_status(handle, type, status, on_ss_set_cli_status, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}


static int run_ss_get_cli_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsCliType_t type;

	msg("call tel_get_ss_cli_status()");

	type = atoi(data_ss_get_cli_status_type);

	result = tel_get_ss_cli_status(handle, type, on_ss_get_cli_status, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_ss_send_ussd_request(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelSsUssdMsgInfo_t info;

	msg("call tel_send_ss_ussd_request()");

	memset(&info, 0, sizeof(TelSsUssdMsgInfo_t));
	info.Type = atoi(data_ss_ussd_request_type);
	info.Dcs = 0x0f;
	info.Length = strlen(data_ss_ussd_request_string);
	if (info.Length > TAPI_SS_USSD_DATA_SIZE_MAX - 1) {
		msg("failed. string is too long.");
		return 0;
	}
	strncpy(info.szString, data_ss_ussd_request_string, TAPI_SS_USSD_DATA_SIZE_MAX - 1);

	result = tel_send_ss_ussd_request(handle, &info, on_ss_send_ussd_request, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static struct menu_data menu_ss_set_barring[] = {
	{ "1", "class", NULL, NULL, data_ss_set_barring_class},
	{ "-", "(16=All teleservices, 17=All voice, 18=All data teleservices", NULL, NULL, NULL},
	{ "-", " 19=FAX, 22=SMS, 23=VGCS, 24=VBS, 25=ALL_TELE_EXPT_SMS, ...)", NULL, NULL, NULL},
	{ "2", "mode", NULL, NULL, data_ss_set_barring_mode},
	{ "-", "(0=Activate, 1=Deactivate)", NULL, NULL, NULL},
	{ "3", "type", NULL, NULL, data_ss_set_barring_type},
	{ "-", "(1=All outgoing calls, 2=Outgoing international calls, 3=BOIC_NOT_HC", NULL, NULL, NULL},
	{ "-", " 4=All incoming calls, 5=BIC_ROAM, 6=AB, 7=AOB, 8=AIB, 9=BIC_NOT_SIM)", NULL, NULL, NULL},
	{ "4", "password", NULL, NULL, data_ss_set_barring_password},
	{ "5", "run", NULL, run_ss_set_barring, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_barring_status[] = {
	{ "1", "class", NULL, NULL, data_ss_get_barring_status_class},
	{ "-", "(16=All teleservices, 17=All voice, 18=All data teleservices", NULL, NULL, NULL},
	{ "-", " 19=FAX, 22=SMS, 23=VGCS, 24=VBS, 25=ALL_TELE_EXPT_SMS, ...)", NULL, NULL, NULL},
	{ "2", "type", NULL, NULL, data_ss_get_barring_status_type},
	{ "-", "(1=All outgoing calls, 2=Outgoing international calls, 3=BOIC_NOT_HC", NULL, NULL, NULL},
	{ "-", " 4=All incoming calls, 5=BIC_ROAM, 6=AB, 7=AOB, 8=AIB, 9=BIC_NOT_SIM)", NULL, NULL, NULL},
	{ "3", "run", NULL, run_ss_get_barring_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_change_barring_password[] = {
	{ "1", "old password", NULL, NULL, data_ss_change_barring_password_old},
	{ "2", "new password", NULL, NULL, data_ss_change_barring_password_new},
	{ "3", "run", NULL, run_ss_change_barring_password, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_set_forward[] = {
	{ "1", "class", NULL, NULL, data_ss_set_forward_class},
	{ "-", "(16=All teleservices, 17=All voice, 18=All data teleservices", NULL, NULL, NULL},
	{ "-", " 19=FAX, 22=SMS, 23=VGCS, 24=VBS, 25=ALL_TELE_EXPT_SMS, ...)", NULL, NULL, NULL},
	{ "2", "mode", NULL, NULL, data_ss_set_forward_mode},
	{ "-", "(0=Deactivate, 1=Activate, 2=Register, 3=Deregister)", NULL, NULL, NULL},
	{ "3", "condition", NULL, NULL, data_ss_set_forward_condition},
	{ "-", "(1=CFU, 2=CFB, 3=CFNRy, 4=CFNRc, 5=CF_ALL, 6=CFC)", NULL, NULL, NULL},
	{ "4", "timer", NULL, NULL, data_ss_set_forward_timer},
	{ "-", "(5/10/15/20/25/30 secs)", NULL, NULL, NULL},
	{ "5", "number", NULL, NULL, data_ss_set_forward_number},
	{ "6", "run", NULL, run_ss_set_forward, NULL},

	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_forward_status[] = {
	{ "1", "class", NULL, NULL, data_ss_get_forward_status_class},
	{ "-", "(16=All teleservices, 17=All voice, 18=All data teleservices", NULL, NULL, NULL},
	{ "-", " 19=FAX, 22=SMS, 23=VGCS, 24=VBS, 25=ALL_TELE_EXPT_SMS, ...)", NULL, NULL, NULL},
	{ "2", "condition", NULL, NULL, data_ss_get_forward_status_condition},
	{ "-", "(1=CFU, 2=CFB, 3=CFNRy, 4=CFNRc, 5=CF_ALL, 6=CFC)", NULL, NULL, NULL},
	{ "3", "run", NULL, run_ss_get_forward_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_set_waiting[] = {
	{ "1", "class", NULL, NULL, data_ss_set_waiting_class},
	{ "-", "(16=All teleservices, 17=All voice, 18=All data teleservices", NULL, NULL, NULL},
	{ "-", " 19=FAX, 22=SMS, 23=VGCS, 24=VBS, 25=ALL_TELE_EXPT_SMS, ...)", NULL, NULL, NULL},
	{ "2", "mode", NULL, NULL, data_ss_set_waiting_mode},
	{ "-", "(0=Activate, 1=Deactivate)", NULL, NULL, NULL},
	{ "3", "run", NULL, run_ss_set_waiting, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_waiting_status[] = {
	{ "1", "class", NULL, NULL, data_ss_get_waiting_status_class},
	{ "-", "(16=All teleservices, 17=All voice, 18=All data teleservices", NULL, NULL, NULL},
	{ "-", " 19=FAX, 22=SMS, 23=VGCS, 24=VBS, 25=ALL_TELE_EXPT_SMS, ...)", NULL, NULL, NULL},
	{ "2", "run", NULL, run_ss_get_waiting_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_set_cli_status[] = {
	{ "1", "type", NULL, NULL, data_ss_set_cli_status_type},
	{ "-", "(1=CLIP, 2=CLIR, 3=COLP, 4=COLR, 5=CDIP, 6=CNAP)", NULL, NULL, NULL},
	{ "2", "mode", NULL, NULL, data_ss_set_cli_status},
	{ "-", "(1=SS_CLI_STATUS_NOT_PROVISONED, 2=SS_CLI_STATUS_PROVISIONED ,)", NULL, NULL, NULL},
	{ "-", "(3=SS_CLI_STATUS_ACTIVATED, 4=SS_CLI_STATUS_UNKOWN ,)", NULL, NULL, NULL},
	{ "-", "(5 =SS_CLI_STATUS_TEMP_RESTRICTED (Only for CLIR) , 6=SS_CLI_STATUS_TEMP_ALLOWED (Only for CLIR) ,)", NULL, NULL, NULL},
	{ "3", "run", NULL, run_ss_set_cli_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_cli_status[] = {
	{ "1", "type", NULL, NULL, data_ss_get_cli_status_type},
	{ "-", "(1=CLIP, 2=CLIR, 3=COLP, 4=COLR, 5=CDIP, 6=CNAP)", NULL, NULL, NULL},
	{ "2", "run", NULL, run_ss_get_cli_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_send_ussd_request[] = {
	{ "1", "type", NULL, NULL, data_ss_ussd_request_type},
	{ "-", "(1=INIT, 2=RSP, 3=REL)", NULL, NULL, NULL},
	{ "2", "string", NULL, NULL, data_ss_ussd_request_string},
	{ "3", "run", NULL, run_ss_send_ussd_request, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_ss_3gpp[] = {
	{ "1", "tel_set_ss_barring", menu_ss_set_barring, NULL, NULL},
	{ "2", "tel_get_ss_barring_status", menu_ss_get_barring_status, NULL, NULL},
	{ "3", "tel_change_ss_barring_password", menu_ss_change_barring_password, NULL, NULL},
	{ "4", "tel_set_ss_forward", menu_ss_set_forward, NULL, NULL},
	{ "5", "tel_get_ss_forward_status", menu_ss_get_forward_status, NULL, NULL},
	{ "6", "tel_set_ss_waiting", menu_ss_set_waiting, NULL, NULL},
	{ "7", "tel_get_ss_waiting_status", menu_ss_get_waiting_status, NULL, NULL},
	{ "8", "tel_set__cli_status", menu_ss_set_cli_status, NULL, NULL},
	{ "9", "tel_get_ss_cli_status", menu_ss_get_cli_status, NULL, NULL},
	{ "10", "tel_send_ss_ussd_request", menu_ss_send_ussd_request, NULL, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_ss[] = {
	{ "1", "3GPP(GSM/WCDMA/LTE)", menu_ss_3gpp, NULL, NULL},
	{ "2", "3GPP2(CDMA) Not Supported", NULL, NULL, NULL},
	{ NULL, NULL, },
};

void register_ss_event(TapiHandle *handle)
{
	int ret;

	ret = tel_register_noti_event(handle, TAPI_NOTI_SS_USSD, on_noti_ss_ussd, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_SS_RELEASE_COMPLETE, on_noti_ss_release_complete, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_SS_FORWARD_STATUS, on_noti_ss_forwarding_status, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_SS_BARRING_STATUS, on_noti_ss_barring_status, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_SS_WAITING_STATUS, on_noti_ss_waiting_status, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_SS_INFO, on_noti_ss_sups_info, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}
}
