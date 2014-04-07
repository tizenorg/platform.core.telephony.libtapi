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
#include <tapi_modem.h>
#include <tapi_events.h>

#include "menu.h"
#include "modem.h"

static char *dbg_modem_power_status[] = {"OFF", "ON", "RESET", };

static char *dbg_modem_status[] = {"TEL_MODEM_POWER_OFF", "TEL_MODEM_POWER_ON",
		"TEL_MODEM_POWER_ERROR", };

static char *dbg_modem_result[] = {"TEL_MODEM_RESULT_SUCCESS", "TEL_MODEM_RESULT_FAILURE",
		"TEL_MODEM_RESULT_INVALID_PARAMETER", "TEL_MODEM_RESULT_MEMORY_FAILURE",
		"TEL_MODEM_RESULT_OPERATION_NOT_SUPPORTED", "TEL_MODEM_RESULT_UNKNOWN_FAILURE", };

static char data_modem_set_flight_mode[MENU_DATA_SIZE + 1] = "1";
static char data_modem_set_power_mode[MENU_DATA_SIZE + 1] = "1";

static void on_noti_modem_power_status(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	gboolean *status = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_MODEM_POWER_STATUS);

	if (!status)
		return;

	msg("modem power status[%s]", (*status == TRUE) ? "ON" : "OFF");
}

static void on_noti_modem_flight_mode_status(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	gboolean *status = data;

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_MODEM_FLIGHT_MODE_STATUS);

	if (!status)
		return;

	msg("modem flight mode status[%s]", (*status == TRUE) ? "ON" : "OFF");
}

static int run_modem_get_version(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelModemVersion version;
	TelReturn rt = 0;

	msg("call tapi_modem_get_version()");

	memset(&version, 0, sizeof(TelModemVersion));

	rt = tapi_modem_get_version(handle, &version);
	CHECK_RT(rt);

	msg("sw version: [%s]", version.software_version);
	msg("hw version: [%s]", version.hardware_version);
	msg("RfCal Date: [%s]", version.calibration_date);
	msg("Product Code: [%s]", version.product_code);

	return 0;
}

static int run_modem_get_imei(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	char imei[TEL_MODEM_IMEI_LENGTH_MAX + 1] = {};
	TelReturn rt = 0;

	msg("call tapi_modem_get_imei()");

	rt = tapi_modem_get_imei(handle, imei);
	CHECK_RT(rt);

	msg("imei: [%s]", imei);

	return 0;
}

static void on_modem_set_power_status(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_modem_set_power_status() response received");
	if (result >= TEL_MODEM_RESULT_SUCCESS && result <= TEL_MODEM_RESULT_UNKNOWN_FAILURE) {
		msg("access_rt[%d][%s]", result, dbg_modem_result[result]);
	} else {
		msg("access_rt[%d]", result);
	}
}

static int run_modem_set_power_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelModemPowerStatus mode;
	TelReturn rt = 0;

	msg("call tapi_modem_set_power_status()");

	mode = atoi(data_modem_set_power_mode);

	if (mode <= TEL_MODEM_POWER_ERROR) {
		msg("Requested power mode: [%d][%s]", mode, dbg_modem_power_status[mode]);
	} else {
		msg("Requested power mode: [%d]", mode);
	}

	rt = tapi_modem_set_power_status(handle, mode, on_modem_set_power_status, NULL);
	CHECK_RT(rt);

	return 0;
}

static int run_modem_get_power_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelModemPowerStatus status;
	TelReturn rt = 0;

	msg("call tapi_modem_get_power_status()");

	rt = tapi_modem_get_power_status(handle, &status);
	CHECK_RT(rt);

	if (status <= TEL_MODEM_POWER_ERROR) {
		msg("status[%d][%s]", status, dbg_modem_status[status]);
	} else {
		msg("status[%d]", status);
	}

	return 0;
}

static void on_modem_set_flight_mode(TelHandle *handle, int result, void *data, void *user_data)
{
	msgb("tapi_modem_set_flight_mode() response received");
	if (result >= TEL_MODEM_RESULT_SUCCESS && result <= TEL_MODEM_RESULT_UNKNOWN_FAILURE) {
		msg("access_rt[%d][%s]", result, dbg_modem_result[result]);
	} else {
		msg("access_rt[%d]", result);
	}
}

static int run_modem_set_flight_mode(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	gboolean enable;
	TelReturn rt = 0;

	msg("call tapi_modem_set_flight_mode()");

	enable = atoi(data_modem_set_flight_mode);
	msg("Requested flight mode: [%s]", ((enable == 1) ? "ON" : "OFF"));

	rt = tapi_modem_set_flight_mode(handle, enable, on_modem_set_flight_mode, NULL);
	CHECK_RT(rt);

	return 0;
}

static int run_modem_get_flight_mode(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	gboolean enable;
	TelReturn rt = 0;

	msg("call tapi_modem_get_flight_mode()");

	rt = tapi_modem_get_flight_mode(handle, &enable);
	CHECK_RT(rt);

	msg("Flight mode status: [%s]", ((enable == 1) ? "ON" : "OFF"));

	return 0;
}

static struct menu_data menu_modem_get_version[] = {
	{ "1", "run", NULL, run_modem_get_version, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_imei[] = {
	{ "1", "run", NULL, run_modem_get_imei, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_set_power_status[] = {
	{ "1", "mode (0=OFF, 1=ON, 2=RESET)", NULL, NULL, data_modem_set_power_mode},
	{ "2", "run", NULL, run_modem_set_power_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_power_status[] = {
	{ "1", "run", NULL, run_modem_get_power_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_set_flight_mode[] = {
	{ "1", "mode (0=OFF, 1=ON)", NULL, NULL, data_modem_set_flight_mode},
	{ "2", "run", NULL, run_modem_set_flight_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_modem_get_flight_mode[] = {
	{ "1", "run", NULL, run_modem_get_flight_mode, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_modem[] = {
	{ "1g", "Get Modem Version", menu_modem_get_version, NULL, NULL},
	{ "2g", "Get IMEI", menu_modem_get_imei, NULL, NULL},
	{ "3s", "Set Modem Power Status", menu_modem_set_power_status, NULL, NULL},
	{ "3g", "Get Modem Status", menu_modem_get_power_status, NULL, NULL},
	{ "4s", "Set Flight Mode", menu_modem_set_flight_mode, NULL, NULL},
	{ "4g", "Get Flight Mode", menu_modem_get_flight_mode, NULL, NULL},
	{ NULL, NULL, },
};

void register_modem_event(TelHandle *handle)
{
	TelReturn ret;

	/* Event register can fail only in Invalid input parameter case
	 * Assuming we pass valid input parameters
	 */
	ret = tapi_register_event_id(handle, TEL_NOTI_MODEM_POWER_STATUS, on_noti_modem_power_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_MODEM_POWER_STATUS - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_MODEM_FLIGHT_MODE_STATUS, on_noti_modem_flight_mode_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_MODEM_FLIGHT_MODE_STATUS - register event failed: [%d]", ret);
}
