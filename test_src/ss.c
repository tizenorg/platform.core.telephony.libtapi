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
#include <tapi_ss.h>
#include <tapi_events.h>

#include "menu.h"
#include "ss.h"

#define CHECK_RESULT(result) \
{\
	if (result < TEL_SS_RESULT_SUCCESS || result > TEL_SS_RESULT_OPERATION_NOT_PERMITTED) { \
		msg("API failed with result : %d", result); \
		return; \
	} else { \
		msg("Result[%d] :[%s] ", result, dbg_ss_result[result]); \
	} \
	if (result != TEL_SS_RESULT_SUCCESS) \
		return; \
}

static char* dbg_ss_result[] = {
	"TEL_SS_RESULT_SUCCESS",
	"TEL_SS_RESULT_FAILURE",
	"TEL_SS_RESULT_INVALID_PARAMETER",
	"TEL_SS_RESULT_MEMORY_FAILURE",
	"TEL_SS_RESULT_OPERATION_NOT_SUPPORTED",
	"TEL_SS_RESULT_SERVICE_NOT_AVAILABLE",
	"TEL_SS_RESULT_OPERATION_NOT_ALLOWED",
	"TEL_SS_RESULT_INCORRECT_OPERATION",
	"TEL_SS_RESULT_INVALID_PASSWORD",
	"TEL_SS_RESULT_PASSWORD_BLOCKED",
	"TEL_SS_RESULT_OPERATION_NOT_PERMITTED"
};

static char data_ss_set_barring_class[MENU_DATA_SIZE + 1] = "10";
static char data_ss_set_barring_enable[MENU_DATA_SIZE + 1] = "0";
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
static char data_ss_set_waiting_enable[MENU_DATA_SIZE + 1] = "0";

static char data_ss_get_waiting_status_class[MENU_DATA_SIZE + 1] = "16";

static char data_ss_set_cli_status_type[MENU_DATA_SIZE + 1] = "1";
static char data_ss_set_cli_device_status[MENU_DATA_SIZE + 1] = "0";

static char data_ss_get_cli_status_type[MENU_DATA_SIZE + 1] = "0";

static char data_ss_ussd_request_type[MENU_DATA_SIZE + 1] = "1";
static char data_ss_ussd_request_string[MENU_DATA_SIZE + 1] = "";

static void on_noti_ss_ussd(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSsUssdNoti *noti = data;

	if (g_strcmp0(noti_id, TEL_NOTI_SS_USSD) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_SS_USSD);
	} else {
		msg("Wrong Event Recieved : (%s)", noti_id);
		return;
	}

	if (noti) {
		msg(" USSD Noti Response - Status : [%d],  String : [%s]", noti->status, noti->str);
	} else {
		msg("USSD Noti data is NULL.");
	}
}

static void on_response_ss_set_barring(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_response_ss_set_barring :");
	CHECK_RESULT(result);
}

static int run_ss_set_barring(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsBarringInfo info;

	msg("call tapi_ss_set_barring()");

	memset(&info, 0, sizeof(TelSsBarringInfo));

	info.class = atoi(data_ss_set_barring_class);
	info.type = atoi(data_ss_set_barring_type);
	info.enable = atoi(data_ss_set_barring_enable);
	if ( g_strlcpy(info.pwd, data_ss_set_barring_password, TEL_SS_BARRING_PASSWORD_LEN_MAX + 1) > TEL_SS_BARRING_PASSWORD_LEN_MAX) {
		msg ("Entered Password exceeds maximum permissible length(4). Exiting!");
		return 0;
	}

	msg("barring class : (%d)", info.class);
	msg("barring type : (%d)", info.type);
	msg("barring enable : (%d)", info.enable);
	msg("barring password : (%s)", info.pwd);

	result = tapi_ss_set_barring(handle, &info, on_response_ss_set_barring, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_get_barring_status(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSsBarringResp *resp = data;
	unsigned int iter;

	msg("on_response_ss_get_barring_status :");
	CHECK_RESULT(result);

	if (resp) {
		msg(" No. of records = %u", resp->record_num);
		for (iter = 0; iter < resp->record_num; ++iter) {
			msg("Record : [%d] - Class : [%d], Enable : [%d], Type : [%d]",
				iter,
				resp->records[iter].class,
				resp->records[iter].enable,
				resp->records[iter].type);
		}
	}
	else {
		msg("Response data is NULL");
	}
}

static int run_ss_get_barring_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsBarringGetInfo info;

	msg("call tapi_ss_get_barring_status()");

	memset(&info, 0, sizeof(TelSsBarringGetInfo));

	info.class = atoi(data_ss_get_barring_status_class);
	info.type = atoi(data_ss_get_barring_status_type);

	msg("barring class : (%d)", info.class);
	msg("barring type : (%d)", info.type);

	result = tapi_ss_get_barring_status(handle, &info, on_response_ss_get_barring_status, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_change_barring_password(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_response_ss_change_barring_password :");
	CHECK_RESULT(result);
}

static int run_ss_change_barring_password(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsBarringPwdInfo info;

	msg("call tapi_ss_change_barring_password()");

	if ( g_strlcpy(info.old_pwd, data_ss_change_barring_password_old, TEL_SS_BARRING_PASSWORD_LEN_MAX + 1) > TEL_SS_BARRING_PASSWORD_LEN_MAX) {
		msg ("Entered Password exceeds maximum permissible length(4). Exiting!");
		return 0;
	}
	if ( g_strlcpy(info.new_pwd, data_ss_change_barring_password_new, TEL_SS_BARRING_PASSWORD_LEN_MAX + 1) > TEL_SS_BARRING_PASSWORD_LEN_MAX) {
		msg ("Entered Password exceeds maximum permissible length(4). Exiting!");
		return 0;
	}

	msg("old barring password : (%s)", info.old_pwd);
	msg("new barring password : (%s)", info.new_pwd);

	result = tapi_ss_change_barring_password(handle, &info, on_response_ss_change_barring_password, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_set_forward(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_response_ss_set_forward :");
	CHECK_RESULT(result);
}

static int run_ss_set_forward(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsForwardInfo info;

	msg("call tapi_ss_set_forwarding()");

	memset(&info, 0, sizeof(TelSsForwardInfo));

	info.class = atoi(data_ss_set_forward_class);
	info.mode = atoi(data_ss_set_forward_mode);
	info.condition = atoi(data_ss_set_forward_condition);
	if (g_strlcpy(info.number, data_ss_set_forward_number, TEL_SS_NUMBER_LEN_MAX + 1) > TEL_SS_NUMBER_LEN_MAX) {
		msg("Entered Number exceeds maximum permissible length (82). Exiting!");
		return 0;
	}
	info.wait_time = atoi(data_ss_set_forward_timer);

	msg("forward class : (%d)", info.class);
	msg("forward mode : (%d)", info.mode);
	msg("forward condition : (%d)", info.condition);
	msg("forward number : (%s)", info.number);
	msg("forward wait_time : (%u)", info.wait_time);

	result = tapi_ss_set_forwarding(handle, &info, on_response_ss_set_forward, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_get_forward_status(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSsForwardingResp *resp = data;
	unsigned int iter;

	msg("on_response_ss_get_forward_status :");
	CHECK_RESULT(result);

	if (resp) {
		msg(" No. of records = %u", resp->record_num);
		for (iter = 0; iter < resp->record_num; ++iter) {
			msg(" Record : [%u] - Class : [%d], Enable : [%d], Condition : [%d], Number : [%s], Wait_Time : [%d]",
					iter,
					resp->records[iter].class,
					resp->records[iter].enable,
					resp->records[iter].condition,
					resp->records[iter].number,
					resp->records[iter].wait_time);
			msg("");
		}
	} else {
		msg("Response data is NULL");
	}
}

static int run_ss_get_forward_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsForwardGetInfo info;

	msg("call tapi_ss_get_forwarding_status()");

	info.class = atoi(data_ss_get_forward_status_class);
	info.condition = atoi(data_ss_get_forward_status_condition);

	msg("forward class : (%d)", info.class);
	msg("forward condition : (%d)", info.condition);

	result = tapi_ss_get_forwarding_status(handle, &info, on_response_ss_get_forward_status, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_set_waiting(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_response_ss_set_waiting :");
	CHECK_RESULT(result);
}

static int run_ss_set_waiting(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsWaitingInfo info;

	msg("call tapi_ss_set_waiting()");

	memset(&info, 0, sizeof(TelSsWaitingInfo));

	info.class = atoi(data_ss_set_waiting_class);
	info.enable= atoi(data_ss_set_waiting_enable);

	msg("waiting class : (%d)", info.class);
	msg("waiting enable : (%d)", info.enable);

	result = tapi_ss_set_waiting(handle, &info, on_response_ss_set_waiting, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_get_waiting_status(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSsWaitingResp *resp = data;
	unsigned int iter;

	msg("on_response_ss_get_waiting_status :");
	CHECK_RESULT(result);

	if (resp) {
		msg(" No. of records = %d", resp->record_num);
		for (iter = 0; iter < resp->record_num; ++iter) {
			msg(" Record : [%u] - Class : [%d], Enable : [%d]",
				iter,
				resp->records[iter].class,
				resp->records[iter].enable);
		}
	} else {
		msg("Response data is NULL");
	}
}

static int run_ss_get_waiting_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsClass class;

	msg("call tapi_ss_get_waiting_status()");

	class = atoi(data_ss_get_waiting_status_class);

	msg("waiting class : (%d)", class);

	result = tapi_ss_get_waiting_status(handle, class, on_response_ss_get_waiting_status, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_set_cli(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_response_ss_set_cli :");
	CHECK_RESULT(result);
}

static int run_ss_set_cli(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsCliInfo info;

	msg("call tapi_ss_set_cli()");

	info.type= atoi(data_ss_set_cli_status_type);

	msg("cli type : (%d)", info.type);

	switch (info.type) {
		case TEL_SS_CLI_CLIP:
			info.status.clip = atoi(data_ss_set_cli_device_status);
			msg("CLIP status : (%d)", info.status.clip);
			break;
		case TEL_SS_CLI_CLIR:
			info.status.clir = atoi(data_ss_set_cli_device_status);
			msg("CLIR status : (%d)", info.status.clir);
			break;
		case TEL_SS_CLI_COLP:
			info.status.colp = atoi(data_ss_set_cli_device_status);
			msg("COLP status : (%d)", info.status.colp);
			break;
		case TEL_SS_CLI_COLR:
			info.status.colr = atoi(data_ss_set_cli_device_status);
			msg("COLR status : (%d)", info.status.colr);
			break;
		case TEL_SS_CLI_CDIP:
			info.status.cdip = atoi(data_ss_set_cli_device_status);
			msg("CDIP status : (%d)", info.status.cdip);
			break;
		case TEL_SS_CLI_CNAP:
			info.status.cnap = atoi(data_ss_set_cli_device_status);
			msg("CNAP status : (%d)", info.status.cnap);
	}

	result = tapi_ss_set_cli(handle, &info, on_response_ss_set_cli, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_get_cli_status(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSsCliResp *resp = data;

	msg("on_response_ss_get_cli_status :");
	CHECK_RESULT(result);

	if (resp) {
		switch (resp->type) {
			case TEL_SS_CLI_CLIP:
				msg(" CLIP Response - Device : (%d), Network : (%d)",
					resp->status.clip.dev_status,
					resp->status.clip.net_status);
				break;
			case TEL_SS_CLI_CLIR:
				msg(" CLIR Response - Device : (%d), Network : (%d)",
				resp->status.clir.dev_status,
				resp->status.clir.net_status);
				break;
			case TEL_SS_CLI_COLP:
				msg(" COLP Response - Device : (%d), Network : (%d)",
					resp->status.colp.dev_status,
					resp->status.colp.net_status);
				break;
			case TEL_SS_CLI_COLR:
				msg(" COLR Response - Device : (%d), Network : (%d)",
					resp->status.colr.dev_status,
					resp->status.colr.net_status);
				break;
			case TEL_SS_CLI_CDIP:
				msg(" CDIP Response - Device : (%d), Network : (%d)",
					resp->status.cdip.dev_status,
					resp->status.cdip.net_status);
				break;
			case TEL_SS_CLI_CNAP:
				msg(" CNAP Response - Device : (%d), Network : (%d)",
					resp->status.cnap.dev_status,
					resp->status.cnap.net_status);
		}
	} else {
		msg("Response data is NULL");
	}
}

static int run_ss_get_cli_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsCliType type;

	msg("call tapi_ss_get_cli_status()");

	type = atoi(data_ss_get_cli_status_type);

	msg("cli type : (%d)", type);

	result = tapi_ss_get_cli_status(handle, type, on_response_ss_get_cli_status, NULL);
	CHECK_RT(result);

	return 0;
}

static void on_response_ss_send_ussd_request(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSsUssdResp *resp = data;

	msg("on_response_ss_send_ussd_request :");
	CHECK_RESULT(result);

	if (resp) {
		msg(" Type : [%d] Status : [%d], String : [%s]",
				resp->type,
				resp->status,
				resp->str);
	} else {
		msg("No Callback data present.");
	}
}

static int run_ss_send_ussd_request(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelSsUssdInfo info;

	msg("call tapi_ss_send_ussd_request()");

	memset(&info, 0, sizeof(TelSsUssdInfo));

	info.type = atoi(data_ss_ussd_request_type);
	info.str = (unsigned char *) &data_ss_ussd_request_string;

	msg("ussd type : (%d)", info.type);
	msg("ussd string : (%s)", info.str);

	result = tapi_ss_send_ussd_request(handle, &info, on_response_ss_send_ussd_request, NULL);
	CHECK_RT(result);

	return 0;
}

static struct menu_data menu_ss_set_barring[] = {
	{ "1", "class (0=All Teleservices bearer, \n" \
			"\t 10=All Teleservices, 11=Voice, 12=All Data Teleservices, 13=Fax, 16=SMS, 17=VGCS, 18=VBS, 19=All Tele except SMS \n" \
			"\t 20=All Bearer, 21=All async, 22=All sync, 24=All CS sync, 25=All CS async, 26=All DEDI PS, 27=All DEDI PAD, 28=All Data CDA\n" \
			"\t 89=Aux voice, 99=All GPRS bearer", NULL, NULL, data_ss_set_barring_class},
	{ "2", "enable (0=No, 1=Yes)", NULL, NULL, data_ss_set_barring_enable},
	{ "3", "type (0=BOAC, 1=BOIC, 2=BOIC Not HC 3=BAIC, 4=BIC Roam, 5=NS, 6=AB, 7=AOB, 8=AIB)", NULL, NULL, data_ss_set_barring_type},
	{ "4", "password", NULL, NULL, data_ss_set_barring_password},
	{ "5", "run", NULL, run_ss_set_barring, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_barring_status[] = {
	{ "1", "class (0=All Teleservices bearer, \n" \
			"\t 10=All Teleservices, 11=Voice, 12=All Data Teleservices, 13=Fax, 16=SMS, 17=VGCS, 18=VBS, 19=All Tele except SMS \n" \
			"\t 20=All Bearer, 21=All async, 22=All sync, 24=All CS sync, 25=All CS async, 26=All DEDI PS, 27=All DEDI PAD, 28=All Data CDA\n" \
			"\t 89=Aux voice, 99=All GPRS bearer", NULL, NULL, data_ss_get_barring_status_class},
	{ "2", "type (0=BOAC, 1=BOIC, 2=BOIC Not HC 3=BAIC, 4=BIC Roam, 5=NS, 6=AB, 7=AOB, 8=AIB)", NULL, NULL, data_ss_get_barring_status_type},
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
	{ "1", "class (0=All Teleservices bearer, \n" \
			"\t 10=All Teleservices, 11=Voice, 12=All Data Teleservices, 13=Fax, 16=SMS, 17=VGCS, 18=VBS, 19=All Tele except SMS \n" \
			"\t 20=All Bearer, 21=All async, 22=All sync, 24=All CS sync, 25=All CS async, 26=All DEDI PS, 27=All DEDI PAD, 28=All Data CDA\n" \
			"\t 89=Aux voice, 99=All GPRS bearer", NULL, NULL, data_ss_set_forward_class},
	{ "2", "mode (0=Disable, 1=Enable, 2=Register, 3=Deregister)", NULL, NULL, data_ss_set_forward_mode},
	{ "3", "condition (0=CFU, 1=CFB, 2=CFNRY, 3=CFNRC, 4=ALL, 5=ALL_CFC)", NULL, NULL, data_ss_set_forward_condition},
	{ "4", "number", NULL, NULL, data_ss_set_forward_number},
	{ "5", "wait time (1-30 seconds)", NULL, NULL, data_ss_set_forward_timer},
	{ "6", "run", NULL, run_ss_set_forward, NULL},

	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_forward_status[] = {
	{ "1", "class (0=All Teleservices bearer, \n" \
			"\t 10=All Teleservices, 11=Voice, 12=All Data Teleservices, 13=Fax, 16=SMS, 17=VGCS, 18=VBS, 19=All Tele except SMS \n" \
			"\t 20=All Bearer, 21=All async, 22=All sync, 24=All CS sync, 25=All CS async, 26=All DEDI PS, 27=All DEDI PAD, 28=All Data CDA\n" \
			"\t 89=Aux voice, 99=All GPRS bearer", NULL, NULL, data_ss_get_forward_status_class},
	{ "2", "condition (0=CFU, 1=CFB, 2=CFNRY, 3=CFNRC, 4=ALL, 5=ALL_CFC)", NULL, NULL, data_ss_get_forward_status_condition},
	{ "3", "run", NULL, run_ss_get_forward_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_set_waiting[] = {
	{ "1", "class (0=All Teleservices bearer, \n" \
			"\t 10=All Teleservices, 11=Voice, 12=All Data Teleservices, 13=Fax, 16=SMS, 17=VGCS, 18=VBS, 19=All Tele except SMS \n" \
			"\t 20=All Bearer, 21=All async, 22=All sync, 24=All CS sync, 25=All CS async, 26=All DEDI PS, 27=All DEDI PAD, 28=All Data CDA\n" \
			"\t 89=Aux voice, 99=All GPRS bearer", NULL, NULL, data_ss_set_waiting_class},
	{ "2", "enable (0=Deactivate, 1=Activate)", NULL, NULL, data_ss_set_waiting_enable},
	{ "3", "run", NULL, run_ss_set_waiting, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_waiting_status[] = {
	{ "1", "class (0=All Teleservices bearer, \n" \
			"\t 10=All Teleservices, 11=Voice, 12=All Data Teleservices, 13=Fax, 16=SMS, 17=VGCS, 18=VBS, 19=All Tele except SMS \n" \
			"\t 20=All Bearer, 21=All async, 22=All sync, 24=All CS sync, 25=All CS async, 26=All DEDI PS, 27=All DEDI PAD, 28=All Data CDA\n" \
			"\t 89=Aux voice, 99=All GPRS bearer", NULL, NULL, data_ss_get_waiting_status_class},
	{ "2", "run", NULL, run_ss_get_waiting_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_set_cli[] = {
	{ "1", "type (0=clir, 1=clip, 2=colp, 3=colr, 4=cdip, 5=cnap)", NULL, NULL, data_ss_set_cli_status_type},
	{ "2", "device status ( CLIR : 0=Default, 1=Invocation, 2=Suppression, \n" \
			"\t Others : 0=Disable, 1=Enable)", NULL, NULL, data_ss_set_cli_device_status},
	{ "3", "run", NULL, run_ss_set_cli, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_get_cli_status[] = {
	{ "1", "type (0=CLIP, 1=CLIR, 2=COLP, 3=COLR, 4=CDIP, 5=CNAP)", NULL, NULL, data_ss_get_cli_status_type},
	{ "2", "run", NULL, run_ss_get_cli_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_ss_send_ussd_request[] = {
	{ "1", "type (0=INIT, 1=RSP, 2=REL)", NULL, NULL, data_ss_ussd_request_type},
	{ "2", "ussd string", NULL, NULL, data_ss_ussd_request_string},
	{ "3", "run", NULL, run_ss_send_ussd_request, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_ss[] = {
	{ "1", "tapi_ss_set_barring", menu_ss_set_barring, NULL, NULL},
	{ "2", "tapi_ss_get_barring_status", menu_ss_get_barring_status, NULL, NULL},
	{ "3", "tapi_ss_change_barring_password", menu_ss_change_barring_password, NULL, NULL},
	{ "4", "tapi_ss_set_forwarding", menu_ss_set_forward, NULL, NULL},
	{ "5", "tapi_ss_get_forwarding_status", menu_ss_get_forward_status, NULL, NULL},
	{ "6", "tapi_ss_set_waiting", menu_ss_set_waiting, NULL, NULL},
	{ "7", "tapi_ss_get_waiting_status", menu_ss_get_waiting_status, NULL, NULL},
	{ "8", "tapi_ss_set_cli", menu_ss_set_cli, NULL, NULL},
	{ "9", "tapi_ss_get_cli_status", menu_ss_get_cli_status, NULL, NULL},
	{ "10", "tapi_ss_send_ussd_request", menu_ss_send_ussd_request, NULL, NULL},
	{ NULL, NULL, },
};

void register_ss_event(TelHandle *handle)
{
	TelReturn ret;

	ret = tapi_register_event_id (handle, TEL_NOTI_SS_USSD, on_noti_ss_ussd, NULL);
	if (ret != TEL_RETURN_SUCCESS) {
		msg("TEL_NOTI_SS_USSD - register event failed: [%d]", ret);
	}
}
