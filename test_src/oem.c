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
#include <TelOem.h>
#include <ITapiOem.h>

#include <TapiUtility.h>

#include "menu.h"
#include "oem.h"

#define RAW_DATA_MAX_LEN 512

/* Type definition for Common Header */
typedef struct {
	short len;
	char msg_seq;
	char ack_seq;
	char main_cmd;
	char sub_cmd;
	char cmd_type;
	unsigned char raw_data[RAW_DATA_MAX_LEN];
} __attribute__ ((packed)) ipc_type;

typedef struct {
	char magic;
	int calltime;
} __attribute__ ((packed)) call_time_access_t;

static char data_oem_send_data_option[MENU_DATA_SIZE + 1] = "1";
static char data_oem_send_data_sync_option[MENU_DATA_SIZE + 1] = "1";
static char data_oem_send_data_async_option[MENU_DATA_SIZE + 1] = "1";
static char data_oem_thermal_max[MENU_DATA_SIZE + 1] = "40";
static char data_oem_thermal_min[MENU_DATA_SIZE + 1] = "30";
static char data_at_command[MENU_DATA_SIZE + 1] = "AT+CMUT=1";
static char data_oem_external_cmd_send_option[MENU_DATA_SIZE + 1] = "1";
static char data_access_call_time_option[MENU_DATA_SIZE + 1] = "1";
static char data_oem_ims_data[MENU_DATA_SIZE + 1] = "1";

static void on_noti_oemdata(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelOemData_t *pdata = data;
	ipc_type *noti = NULL;

	if (!pdata) {
		msg("Invalid Data");
		return;
	}

	msg("id:[0x%x] data_len:[%d] [%s]", pdata->oem_id, pdata->data_len, noti_id);
	noti = (ipc_type *)pdata->data;
	if (!noti) {
		msg("Invalid Data");
		return;
	}

	menu_print_dump(noti->len, noti);
}

static void on_response_oemdata(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelOemData_t *info = data;

	msg("");
	msgb("on_response_oemdata() response receive");
	msg(" - result = 0x%x", result);

	if (!info) {
		msg(" - failed");
		return;
	}

	msg(" - oem_id = 0x%x", info->oem_id);
	msg(" - data_len = %d", info->data_len);
	menu_print_dump(info->data_len, info->data);
}

static void on_response_oem_send_external_command(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelOemData_t *info = data;

	msgb("on_response_oem_send_external_command() response receive");
	msg("result = 0x%x", result);

	if (!info) {
		msg(" - failed");
		return;
	}

	msg(" - oem_id = 0x%x", info->oem_id);
	msg(" - data_len = %d", info->data_len);
	msg(" - data = %s", info->data);
}

static void on_response_access_call_time(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelOemData_t *info = data;
	call_time_access_t *response = NULL;
	msgb("on_response_access_call_time() response receive");
	msg("result = 0x%x", result);

	if (!info) {
		msg(" - failed");
		return;
	}

	msg(" - oem_id = 0x%x", info->oem_id);
	msg(" - data_len = %d", info->data_len);

	response = (call_time_access_t *)info->data;
	if (!response) {
		msg("Wrong response data");
	} else {
		msg(" - result = %d", response->magic);
		msg(" - call time = %d", response->calltime);
	}

}

static int run_oem_send_data(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	ipc_type data;
	int result;
	int operation;
	int oem_id = 0;
	operation = atoi(data_oem_send_data_option);
	msg("call run_oem_send_data() operation:%d ", operation);

	data.msg_seq = 0xFF;
	data.ack_seq = 0;
	if (operation == 1) {
		data.main_cmd = 0x0b;
		data.sub_cmd = 0x01;
		data.cmd_type = 0x03;
		data.raw_data[0] = 0x04;
		data.raw_data[1] = 0x00;
		data.raw_data[2] = 0x10;
		data.len = 0x0A;

		oem_id = 0x1001;
	} else {
		data.main_cmd = 0x0b;
		data.sub_cmd = 0x02;
		data.cmd_type = 0x03;
		data.raw_data[0] = 0x04;
		data.len = 0x08;

		oem_id = 0x1001;
	}
	menu_print_dump(data.len, &data);

	result = tel_send_oem_data(handle, oem_id, &data, data.len);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_send_data_sync(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelOemData_t info = {0};
	ipc_type data;
	int oem_id = 0;
	int result;

	msg("call run_oem_send_data() ");

	/* Header - 7 bytes */
	data.len = 7;
	data.msg_seq = 0xFF;
	data.ack_seq = 0x00;
	data.main_cmd = 0x0A;
	data.sub_cmd = 0x2F;
	data.cmd_type = 0x02;

	oem_id = 0x1009;
	menu_print_dump(data.len, &data);

	result = tel_send_oem_data_sync(handle, oem_id, &data, data.len, &info);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	msg(" - oem_id = 0x%x", info.oem_id);
	msg(" - data_len = %d", info.data_len);
	menu_print_dump(info.data_len, info.data);
	msg("");

	if (info.data_len)
		g_free(info.data);

	return 0;
}

static int run_oem_send_data_async(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	ipc_type data;
	int oem_id = 0;
	int result;

	msg("call run_oem_send_data() ");

	/* Header - 7 bytes */
	data.len = 7;
	data.msg_seq = 0xFF;
	data.ack_seq = 0x00;
	data.main_cmd = 0x0A;
	data.sub_cmd = 0x2F;
	data.cmd_type = 0x02;

	oem_id = 0x1009;
	menu_print_dump(data.len, &data);

	result = tel_send_oem_data_async(handle, oem_id, &data, data.len, on_response_oemdata, NULL);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_get_thermal_data(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	ipc_type data;
	int oem_id = 0;
	int result;

	msg("call run_oem_send_data() ");

	/* Header - 7 bytes */
	data.len = 10;
	data.msg_seq = 0xFF;
	data.ack_seq = 0x00;
	data.main_cmd = 0x0A;
	data.sub_cmd = 0x2E;
	data.cmd_type = 0x02;

	data.raw_data[0] = 0x00;
	data.raw_data[1] = 0x00;
	data.raw_data[2] = 0x00;

	oem_id = 0x1009;
	menu_print_dump(data.len, &data);

	result = tel_send_oem_data(handle, oem_id, &data, data.len);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_set_thermal_noti(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	ipc_type data;
	int oem_id = 0;
	int result;
	int min_temp, max_temp;

	max_temp = atoi(data_oem_thermal_max);
	min_temp = atoi(data_oem_thermal_min);

	msg("call run_oem_send_data() min[%d],max[%d] ", min_temp, max_temp);

	/* Header - 7 bytes */
	data.len = 10;
	data.msg_seq = 0xFF;
	data.ack_seq = 0x00;
	data.main_cmd = 0x0A;
	data.sub_cmd = 0x2E;
	data.cmd_type = 0x03;

	/* payload - 3 byte */
	data.raw_data[0] = 0x00;
	data.raw_data[1] = (unsigned char)max_temp;
	data.raw_data[2] = (unsigned char)min_temp;

	oem_id = 0x1009;
	menu_print_dump(data.len, &data);

	result = tel_send_oem_data(handle, oem_id, &data, data.len);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_send_external_command_data(TapiHandle *handle)
{
	int oem_id = 0x2000;
	int result;

	result = tel_send_oem_data(handle, oem_id, data_at_command, strlen(data_at_command));

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_send_external_command_sync(TapiHandle *handle)
{
	int oem_id = 0x2000;
	TelOemData_t info = {0};
	int result;

	result = tel_send_oem_data_sync(handle, oem_id, data_at_command, strlen(data_at_command), &info);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	msg("oem_id = 0x%x", info.oem_id);
	msg("data_len = %d", info.data_len);
	msg("data = %s", info.data);

	if (info.data_len)
		g_free(info.data);

	return 0;
}

static int run_oem_send_external_command_async(TapiHandle *handle)
{
	int oem_id = 0x2000;
	int result;

	result = tel_send_oem_data_async(handle, oem_id, data_at_command, strlen(data_at_command), on_response_oem_send_external_command, NULL);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_send_external_command(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int option = atoi(data_oem_external_cmd_send_option);

	switch (option) {
	case 1:
		run_oem_send_external_command_data(handle);
		break;
	case 2:
		run_oem_send_external_command_sync(handle);
		break;
	case 3:
		run_oem_send_external_command_async(handle);
		break;
	default:
		msg("wrong option: calling run_oem_send_external_command_data");
		run_oem_send_external_command_data(handle);
	}

	return 0;
}

static int run_oem_access_call_time(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int option = atoi(data_access_call_time_option);
	int oem_id = 0x1008;
	int result;
	call_time_access_t req_data = {0};

	switch (option) {
	case 1: /* Get */
		req_data.magic = 0;
		break;
	case 2:
		req_data.magic = 1;
		req_data.calltime = 1405389618;
		break;
	default:
		msg("wrong option: calling run_oem_send_external_command_data");
		return 0;
	}
	msg("magic=%d, time=%d", req_data.magic, req_data.calltime);

	result = tel_send_oem_data_async(handle, oem_id, &req_data, sizeof(call_time_access_t), on_response_access_call_time, NULL);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	return 0;
}

static int run_oem_ims_command(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelOemData_t info = {0};
	ipc_type data;
	int oem_id = 0;
	int result;

	msg("call run_oem_ims_command() ");

	/* Header - 7 bytes */
	data.len = 8;
	data.msg_seq = 0xFF;
	data.ack_seq = 0x00;
	data.main_cmd = 0x16;
	data.sub_cmd = 0x05;
	data.cmd_type = 0x02;

	/* payload - 1 byte */
	data.raw_data[0] = 0x01;

	oem_id = 0x100A;
	menu_print_dump(data.len, &data);

	result = tel_send_oem_data_sync(handle, oem_id, &data, data.len, &info);

	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);
	else
		msg("success. (result = %d)", result);

	msg(" - oem_id = 0x%x", info.oem_id);
	msg(" - data_len = %d", info.data_len);
	menu_print_dump(info.data_len, info.data);
	msg("");
	if (info.data_len)
		g_free(info.data);

	return 0;
}

static struct menu_data menu_oem_send_data[] = {
	{ "1", "option", NULL, NULL, data_oem_send_data_option},
	{ "2", "run", NULL, run_oem_send_data, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_send_data_sync[] = {
	{ "1", "option", NULL, NULL, data_oem_send_data_sync_option},
	{ "2", "run", NULL, run_oem_send_data_sync, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_send_data_async[] = {
	{ "1", "option", NULL, NULL, data_oem_send_data_async_option},
	{ "2", "run", NULL, run_oem_send_data_async, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_thermal_get[] = {
	{ "1", "run", NULL, run_oem_get_thermal_data, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_thermal_set_noti[] = {
	{ "1", "MAX threshold (Default:40)", NULL, NULL, data_oem_thermal_max},
	{ "2", "MIN threshold (Default:30)", NULL, NULL, data_oem_thermal_min},
	{ "3", "run", NULL, run_oem_set_thermal_noti, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_send_external_command[] = {
	{ "1", "option: 1->send data 2->send sync 3->send async", NULL, NULL, data_oem_external_cmd_send_option},
	{ "2", "Enter AT command(default: AT+CMUT=1)", NULL, NULL, data_at_command},
	{ "3", "run", NULL, run_oem_send_external_command, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_call_time_access[] = {
	{ "1", "option: 1->Get 2->Set", NULL, NULL, data_access_call_time_option},
	{ "2", "run", NULL, run_oem_access_call_time, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_oem_ims[] = {
	{ "1", "option: 1->Get Option", NULL, NULL, data_oem_ims_data},
	{ "2", "run", NULL, run_oem_ims_command, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_oem[] = {
	{ "1", "SEND oem data", menu_oem_send_data, NULL, NULL},
	{ "2", "Get CP Thermal data", menu_oem_thermal_get, NULL, NULL},
	{ "3", "Set CP Thermal noti", menu_oem_thermal_set_noti, NULL, NULL},
	{ "4", "SEND oem data sync", menu_oem_send_data_sync, NULL, NULL},
	{ "5", "SEND oem data async", menu_oem_send_data_async, NULL, NULL},
	{ "6", "SEND oem external command", menu_oem_send_external_command, NULL, NULL},
	{ "7", "SEND call time access", menu_oem_call_time_access, NULL, NULL},
	{ "8", "SEND ims command", menu_oem_ims, NULL, NULL},
	{ NULL, NULL, },
};

void register_oem_event(TapiHandle *handle)
{
	int ret;

	ret = tel_register_noti_event(handle, TAPI_NOTI_OEM_DATA, on_noti_oemdata, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);
}
