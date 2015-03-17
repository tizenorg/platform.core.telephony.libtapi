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
#include <ITapiModem.h>
#include <TapiUtility.h>

#include "menu.h"
#include "modem.h"

static char data_modem_set_flight_mode_mode[MENU_DATA_SIZE + 1] = "1";
static char data_modem_set_power_mode[MENU_DATA_SIZE + 1] = "1";

static void on_noti_modem_power(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *status = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_MODEM_POWER);

	if (!status)
		return;

	msg(" - status = 0x%x", *status);
}

static void on_modem_get_version(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelMiscVersionInformation *info = data;

	msg("");
	msgb("tel_get_misc_me_version() response receive");
	msg(" - result = 0x%x", result);

	if (!info)
		return;

	msg(" - sw version = %s", info->szSwVersion);
	msg(" - hw version = %s", info->szHwVersion);
	msg(" - RfCal Date = %s", info->szRfCalDate);
	msg(" - Product Code = %s", info->szProductCode);
	msg(" - Model ID = %s", info->szModelId);
	msg(" - Prl Version = %s", info->szPrlVersion);
	msg(" - ERI Version = %s", info->szEriVersion);
}

static void on_modem_get_serial_number(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelMiscSNInformation *sn = data;

	msg("");
	msgb("tel_get_misc_me_sn() response receive");
	msg(" - result = 0x%x", result);

	if (!sn)
		return;

	msg(" - esn number = %s", sn->szEsn);
	msg(" - meid number = %s", sn->szMeid);
	msg(" - imei number = %s", sn->szImei);
	msg(" - imeisv number = %s", sn->szImeiSv);

}

static void on_modem_get_imei(TapiHandle *handle, int result, void *data, void *user_data)
{
	char *imei = data;

	msg("");
	msgb("tel_get_misc_me_imei() response receive");
	msg(" - result = 0x%x", result);

	if (!imei)
		return;

	msg(" - IMEI = %s", imei);
}

static void on_modem_get_flight_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	gboolean *mode = data;

	msg("");
	msgb("tel_get_flight_mode() response receive");
	msg(" - result = 0x%x", result);

	if (data)
		msg(" - mode = %d", *mode);
}

static void on_modem_set_flight_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_set_flight_mode() response receive");
	msg(" - result = 0x%x", result);
}

static void on_modem_set_power(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tel_process_power_command() response receive");
	msg(" - result = 0x%x", result);
}

static int run_modem_get_version(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_misc_me_version()");

	result = tel_get_misc_me_version(handle, on_modem_get_version, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_modem_get_version_sync(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelMiscVersionInformation *info;

	msg("call tel_get_misc_me_version_sync()");

	info = tel_get_misc_me_version_sync(handle);
	if (!info) {
		msg("failed.");
		return 0;
	}

	msg(" - sw version = %s", info->szSwVersion);
	msg(" - hw version = %s", info->szHwVersion);
	msg(" - RfCal Date = %s", info->szRfCalDate);
	msg(" - Product Code = %s", info->szProductCode);
	msg(" - Model ID = %s", info->szModelId);
	msg(" - Prl Version = %s", info->szPrlVersion);
	msg(" - ERI Version = %s", info->szEriVersion);

	free(info);

	return 0;
}

static int run_modem_get_serial_number(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_misc_me_sn()");

	result = tel_get_misc_me_sn(handle, on_modem_get_serial_number, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_modem_get_serial_number_sync(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelMiscSNInformation *sn;

	msg("call tel_get_misc_me_sn_sync()");

	sn = tel_get_misc_me_sn_sync(handle);
	if (!sn) {
		msg("failed.");
		return 0;
	}

	msg(" - esn number = [%s]", sn->szEsn);
	msg(" - meid number = [%s]", sn->szMeid);
	msg(" - imei number = [%s]", sn->szImei);
	msg(" - imeisv number = [%s]", sn->szImeiSv);

	free(sn);

	return 0;
}

static int run_modem_get_imei(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_misc_me_imei()");

	result = tel_get_misc_me_imei(handle, on_modem_get_imei, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_modem_get_imei_sync(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	char *imei;

	msg("call tel_get_misc_me_imei_sync()");

	imei = tel_get_misc_me_imei_sync(handle);
	if (!imei) {
		msg("failed.");
		return 0;
	}

	msg(" - imei = [%s]", imei);

	free(imei);

	return 0;
}

static int run_modem_set_power(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int mode;

	msg("call tel_process_power_command()");

	mode = atoi(data_modem_set_power_mode);

	result = tel_process_power_command(handle, mode, on_modem_set_power, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_modem_set_flight_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int mode;

	msg("call tel_set_flight_mode()");

	mode = atoi(data_modem_set_flight_mode_mode);

	result = tel_set_flight_mode(handle, mode, on_modem_set_flight_mode, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_modem_get_flight_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_flight_mode()");

	result = tel_get_flight_mode(handle, on_modem_get_flight_mode, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return 0;
}

static int run_modem_check_power(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	int status = 0;

	msg("call tel_check_modem_power_status()");

	result = tel_check_modem_power_status(handle, &status);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
		return 0;
	}

	msg(" - status = %d", status);

	return 0;
}

static struct menu_data menu_modem_get_version[] = {
	{ "1", "run", NULL, run_modem_get_version, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_version_sync[] = {
	{ "1", "run", NULL, run_modem_get_version_sync, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_serial_number[] = {
	{ "1", "run", NULL, run_modem_get_serial_number, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_serial_number_sync[] = {
	{ "1", "run", NULL, run_modem_get_serial_number_sync, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_imei[] = {
	{ "1", "run", NULL, run_modem_get_imei, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_imei_sync[] = {
	{ "1", "run", NULL, run_modem_get_imei_sync, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_flight_mode[] = {
	{ "1", "run", NULL, run_modem_get_flight_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_set_flight_mode[] = {
	{ "1", "mode (1=ON, 2=OFF)", NULL, NULL, data_modem_set_flight_mode_mode},
	{ "2", "run", NULL, run_modem_set_flight_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_set_power[] = {
	{ "1", "mode (0=ON, 1=OFF, 2=RESET, 3=LOW)", NULL, NULL, data_modem_set_power_mode},
	{ "2", "run", NULL, run_modem_set_power, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_check_power[] = {
	{ "1", "run (sync api)", NULL, run_modem_check_power, NULL},
	{ "-", "(-1=unknown, 0=on, 1=off, 2=rst, 3=low, 4=err)", NULL, NULL, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_modem[] = {
	{ "1", "tel_get_misc_me_version", menu_modem_get_version, NULL, NULL},
	{ "1s", "tel_get_misc_me_version_sync", menu_modem_get_version_sync, NULL, NULL},
	{ "2", "tel_get_misc_me_sn", menu_modem_get_serial_number, NULL, NULL},
	{ "2s", "tel_get_misc_me_sn_sync", menu_modem_get_serial_number_sync, NULL, NULL},
	{ "3", "tel_get_misc_me_imei", menu_modem_get_imei, NULL, NULL},
	{ "3s", "tel_get_misc_me_imei_sync", menu_modem_get_imei_sync, NULL, NULL},
	{ "4", "tel_get_flight_mode", menu_modem_get_flight_mode, NULL, NULL},
	{ "4s", "tel_set_flight_mode", menu_modem_set_flight_mode, NULL, NULL},
	{ "5", "tel_process_power_command", menu_modem_set_power, NULL, NULL},
	{ "6", "tel_check_modem_power_status", menu_modem_check_power, NULL, NULL},
	{ NULL, NULL, },
};

void register_modem_event(TapiHandle *handle)
{
	int ret;

	ret = tel_register_noti_event(handle, TAPI_NOTI_MODEM_POWER, on_noti_modem_power, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}
}
