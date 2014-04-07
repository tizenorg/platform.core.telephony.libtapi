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
#include <tapi_gps.h>
#include <tapi_events.h>

#include "menu.h"
#include "gps.h"

static char *dbg_gps_result[] = {
	"TEL_GPS_RESULT_SUCCESS",
	"TEL_GPS_RESULT_FAILURE",
	"TEL_GPS_RESULT_INVALID_PARAMETER",
	"TEL_GPS_RESULT_MEMORY_FAILURE",
	"TEL_GPS_RESULT_OPERATION_NOT_SUPPORTED",
	"TEL_GPS_RESULT_OPERATION_NOT_PERMITTED",
	"TEL_GPS_RESULT_UNKNOWN_FAILURE",
};

static char data_gps_set_frequency_aiding[MENU_DATA_SIZE + 1] = "1";

/* Utilities */
static void _gps_hex_dump(char *pad, int size, const void *data)
{
	char buf[255] = {0, };
	char hex[4] = {0, };
	int i;
	unsigned char *p;

	if (size <= 0) {
		msg("%sno data", pad);
		return;
	}
	p = (unsigned char *)data;

	snprintf(buf, 255, "%s%04X: ", pad, 0);
	for (i = 0; i<size; i++) {
		snprintf(hex, 4, "%02X ", p[i]);
		strcat(buf, hex);

		if ((i + 1) % 8 == 0) {
			if ((i + 1) % 16 == 0) {
				msg("%s", buf);
				memset(buf, 0, 255);
				snprintf(buf, 255, "%s%04X: ", pad, i + 1);
			}
			else {
				strcat(buf, "  ");
			}
		}
	}

	msg("%s", buf);
}

static void _gps_print_dump(int data_len, void *data)
{
	if(!data)
		return;

	msg("");
	msg("  \tlen=%d", data_len);
	_gps_hex_dump("        ", data_len, data);

	msg("");
}

/* Event Callback */
static void on_noti_gps(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	msg("");
	msgb("event(%s) received !!", noti_id);

	if (!g_strcmp0(noti_id, TEL_NOTI_GPS_ASSIST_DATA)
		|| !g_strcmp0(noti_id, TEL_NOTI_GPS_MEASURE_POSITION)) {
		TelGpsDataInfo *gps_data = NULL;

		if(!data) {
			msg("Gps event data received is null");
			return;
		}

		gps_data = (TelGpsDataInfo *)data;
		_gps_print_dump(gps_data->data_len, gps_data->data);
	} else if (!g_strcmp0(noti_id, TEL_NOTI_GPS_RESET_ASSIST_DATA)) {
		/* Reset Assist Data */
	} else {
		msg("Unhandled event received");
	}
}

/* Requests and Responses */
static void on_gps_set_frequency_aiding(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msgb("tapi_gps_set_frequency_aiding() response received");

	if (result >= TEL_GPS_RESULT_SUCCESS && result <= TEL_GPS_RESULT_UNKNOWN_FAILURE) {
		msg("result[%d][%s]", result, dbg_gps_result[result]);
	} else {
		msg("Api failed with result[%d]", result);
	}
}

static int run_gps_set_frequency_aiding(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt;
	gboolean state;

	msg("call tapi_gps_set_frequency_aiding()");

	state = atoi(data_gps_set_frequency_aiding);
	msg("state[%s]", (state == 1) ? "Enable" : "Disable");

	rt = tapi_gps_set_frequency_aiding(handle, state, on_gps_set_frequency_aiding, NULL);
	CHECK_RT(rt);

	return 0;
}

static int run_gps_confirm_measure_position(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt;
	TelGpsDataInfo gps_data;

	msg("call tapi_gps_confirm_measure_pos()");

	memset(&gps_data, 0, sizeof(TelGpsDataInfo));

	rt = tapi_gps_confirm_measure_pos(handle, &gps_data);
	CHECK_RT(rt);

	_gps_print_dump(gps_data.data_len, gps_data.data);

	g_free(gps_data.data);
	return 0;
}

/* Sub-Menu */
static struct menu_data menu_gps_set_frequency_aiding[] = {
	{ "1", "state (0=Disable, 1=Enable)", NULL, NULL, data_gps_set_frequency_aiding},
	{ "2", "run", NULL, run_gps_set_frequency_aiding, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_gps_confirm_measure_position[] = {
	{ "1", "run", NULL, run_gps_confirm_measure_position, NULL},
	{ NULL, NULL, },
};

/* Menu */
struct menu_data menu_gps[] = {
	{ "1", "Set Frequency Aiding", menu_gps_set_frequency_aiding, NULL, NULL},
	{ "2", "Confirm Measure Position", menu_gps_confirm_measure_position, NULL, NULL},
	{ NULL, NULL, },
};

/* Register Events */
void register_gps_event(TelHandle *handle)
{
	TelReturn ret;

	ret = tapi_register_event_id(handle, TEL_NOTI_GPS_ASSIST_DATA, on_noti_gps, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_GPS_ASSIST_DATA - Event register failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_GPS_MEASURE_POSITION, on_noti_gps, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_GPS_MEASURE_POSITION - Event register failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_GPS_RESET_ASSIST_DATA, on_noti_gps, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_GPS_RESET_ASSIST_DATA - Event register failed: [%d]", ret);
}
