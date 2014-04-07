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
#include <tapi_call.h>
#include <tapi_events.h>
#include "menu.h"
#include "call.h"

#define CHECK_RESULT(result) \
{\
	if (result < TEL_CALL_RESULT_SUCCESS || result > TEL_CALL_RESULT_OPERATION_NOT_PERMITTED) { \
		msg("API failed with result : %d", result); \
		return; \
	} else { \
		msg("Result[%d] :[%s] ", result, dbg_call_result[result]); \
	} \
	if (result != TEL_CALL_RESULT_SUCCESS) \
		return;\
}

static char* dbg_call_result[] = {
	"TEL_CALL_RESULT_SUCCESS",
	"TEL_CALL_RESULT_FAILURE",
	"TEL_CALL_RESULT_INVALID_PARAMETER",
	"TEL_CALL_RESULT_MEMORY_FAILURE",
	"TEL_CALL_RESULT_OPERATION_NOT_SUPPORTED",
	"TEL_CALL_RESULT_FDN_RESTRICTED",
	"TEL_CALL_RESULT_NETWORK_BUSY",
	"TEL_CALL_RESULT_SERVICE_NOT_ALLOWED",
	"TEL_CALL_RESULT_OPERATION_NOT_PERMITTED"
	};

static char data_call_type[MENU_DATA_SIZE + 1] = "0";
static char data_call_ecc[MENU_DATA_SIZE + 1] = "0";
static char data_call_number[MENU_DATA_SIZE + 1] = "+821099777347";

static char data_answer_type[MENU_DATA_SIZE + 1] = "1";

static char data_call_id[MENU_DATA_SIZE + 1] = "1";
static char data_end_type[MENU_DATA_SIZE + 1] = "1";

static char data_dtmf_string[MENU_DATA_SIZE + 1] = "1";

static char data_call_active_line_type[MENU_DATA_SIZE + 1] = "1";

static char data_vol_device[MENU_DATA_SIZE + 1] = "0";
static char data_vol_level[MENU_DATA_SIZE + 1] = "1";

static char data_call_sound_path[MENU_DATA_SIZE + 1] = "0";
static char data_sound_ex_volume[MENU_DATA_SIZE + 1] = "0";

static char data_mute_status[MENU_DATA_SIZE + 1] = "0";

static char data_sound_recording_status[MENU_DATA_SIZE + 1] = "0";

static char data_sound_equalization_mode[MENU_DATA_SIZE + 1] = "0";
static char data_sound_direction[MENU_DATA_SIZE + 1] = "0";
/* To be implemented later
static char data_sound_equalization_parameter[MENU_DATA_SIZE + 1] = "0";  */

static void on_noti_call_status_idle(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusIdleNoti *noti_data = data;

	if (g_strcmp0(noti_id, TEL_NOTI_VOICE_CALL_STATUS_IDLE) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_VOICE_CALL_STATUS_IDLE);
	} else if (g_strcmp0(noti_id, TEL_NOTI_VIDEO_CALL_STATUS_IDLE) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_VIDEO_CALL_STATUS_IDLE);
	} else {
		msg("Unregistered Event recieved!!");
		return;
	}

	if (noti_data) {
		msg(" id = %d", noti_data->call_id);
		msg(" cause = %d", noti_data->cause);
	} else {
		msg("Call Noti data is NULL.");
	}


}

static void on_noti_call_status(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int *call_id = data;

	msg("Event (%s) recieved !!", noti_id);

	if (call_id) {
		msg("call id = %d", *call_id);
	} else {
		msg("Call Noti data is NULL.");
	}
}

static void on_noti_call_sound_ringback_tone(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallSoundRingbackToneNoti *noti_data = data;

	if (g_strcmp0(noti_id, TEL_NOTI_CALL_SOUND_RINGBACK_TONE) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_CALL_SOUND_RINGBACK_TONE);
	} else {
		msg("Unregistered Event recieved!!");
		return;
	}

	if (noti_data) {
		msg(" Sound Ring-Back Tone - %s", (*noti_data == TEL_CALL_SOUND_RINGBACK_TONE_START) ? "START" : "END");
	} else {
		msg("Call Sound Ringback Noti data is NULL");
	}
}

static void on_noti_call_sound_wbamr(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallSoundWbamrNoti *noti_data = data;

	if (g_strcmp0(noti_id, TEL_NOTI_CALL_SOUND_WBAMR) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_CALL_SOUND_WBAMR);
	} else {
		msg("Unregistered Event recieved!!");
		return;
	}

	if (noti_data) {
		msg(" AMR-WB status - %s", (*noti_data == TEL_CALL_SOUND_WBAMR_STATUS_OFF) ? "OFF" : "ON");
 	} else {
		msg("Call Sound AMR-WB Noti data is NULL");
	}
}

static void on_noti_call_sound_equalization(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallSoundEqualizationNoti *noti_data = data;

	if (g_strcmp0(noti_id, TEL_NOTI_CALL_SOUND_EQUALIZATION) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_CALL_SOUND_EQUALIZATION);
	} else {
		msg("Unregistered Event recieved!!");
		return;
	}

	if (noti_data) {
		msg(" mode = %d", noti_data->mode);
		msg(" direction = %d", noti_data->direction);
	} else {
		msg("Call Sound Equalization Noti data is NULL");
	}
}

static void on_noti_call_sound_clock_status(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	gboolean *clock_status = data;

	if (g_strcmp0(noti_id, TEL_NOTI_CALL_SOUND_CLOCK_STATUS) == 0) {
		msg("Event (%s) received !!", TEL_NOTI_CALL_SOUND_CLOCK_STATUS);
	} else {
		msg("Unregistered Event recieved!!");
		return;
	}

	if (clock_status) {
		msg("clock_status[%d]", *clock_status);
	} else {
		msg("Call Sound Clock Status notification data is null");
	}
}

static void on_dial_call(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_dial_call :");
	CHECK_RESULT(result);
}

static int run_call_dial(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelCallDial call_dial_info;
	TelReturn result;

	memset(&call_dial_info, '\0', sizeof(TelCallDial));

	call_dial_info.call_type = atoi(data_call_type);
	call_dial_info.ecc = atoi(data_call_ecc);
	memcpy(call_dial_info.number, data_call_number, strlen(data_call_number));

	msg("call type : (%d)", call_dial_info.call_type);
	msg("emergency call category : (%d)", call_dial_info.ecc);
	msg("phone number : (%s)", call_dial_info.number);

	msg("call tapi_call_dial()");

	result = tapi_call_dial(handle, &call_dial_info, on_dial_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_answer_call(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_answer_call :");
	CHECK_RESULT(result);
}

static int run_call_answer(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelCallAnswerType ans_type;

	ans_type = atoi(data_answer_type);

	msg("answer type : (%d)", ans_type);

	msg("call tapi_call_answer()");

	result = tapi_call_answer(handle, ans_type, on_answer_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_end_call(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_end_call :");
	CHECK_RESULT(result);
}

static int run_call_end(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelCallEnd call_end_info;

	memset(&call_end_info, '\0', sizeof(TelCallEnd));

	call_end_info.call_id = atoi(data_call_id);
	call_end_info.end_type = atoi(data_end_type);

	msg("call id : (%d)", call_end_info.call_id);
	msg("call end type : (%d)", call_end_info.end_type);

	msg("call tapi_call_end()");

	result = tapi_call_end(handle, &call_end_info, on_end_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_send_dtmf_call(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_send_DTMF_call :");
	CHECK_RESULT(result);
}

static int run_call_send_dtmf(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	const char *str = data_dtmf_string;

	msg("DTMF string : (%s)", str);

	msg("call tapi_call_send_dtmf()");

	result = tapi_call_send_dtmf(handle, str, on_send_dtmf_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_hold_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_hold_call :");
	CHECK_RESULT(result);
}

static int run_call_hold(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_call_hold()");

	result = tapi_call_hold(handle, on_hold_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_active_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_active_call :");
	CHECK_RESULT(result);
}

static int run_call_active(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_call_active()");

	result = tapi_call_active(handle, on_active_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_swap_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_swap_call :");
	CHECK_RESULT(result);
}

static int run_call_swap(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_call_swap()");

	result = tapi_call_swap(handle, on_swap_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_join_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_join_call :");
	CHECK_RESULT(result);
}

static int run_call_join(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_call_join()");

	result = tapi_call_join(handle, on_join_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_split_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_split_call :");
	CHECK_RESULT(result);
}

static int run_call_split(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	int call_id;

	call_id = atoi(data_call_id);

	msg("split call Id : (%d)", call_id);

	msg("call tapi_call_split()");

	result = tapi_call_split(handle, call_id, on_split_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_transfer_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_transfer_call :");
	CHECK_RESULT(result);
}

static int run_call_transfer(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_call_transfer()");

	result = tapi_call_transfer(handle, on_transfer_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_deflect_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_deflect_call :");
	CHECK_RESULT(result);
}

static int run_call_deflect(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	const char *number = data_call_number;

	msg("call deflect number : (%s)", number);

	msg("call tapi_call_deflect()");

	result = tapi_call_deflect(handle, number, on_deflect_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_set_active_line_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_set_active_line_call :");
	CHECK_RESULT(result);
}

static int run_call_set_active_line(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	TelCallActiveLine active_line;

	active_line = atoi(data_call_active_line_type);

	msg("active line type : (%d)", active_line);

	msg("call tapi_call_set_active_line()");

	result = tapi_call_set_active_line(handle, active_line, on_set_active_line_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_get_active_line_call(TelHandle * handle, int result, void *data, void *user_data)
{
	if (result < TEL_CALL_RESULT_SUCCESS || result > TEL_CALL_RESULT_OPERATION_NOT_PERMITTED) {
		msg("Unidentified Result : %d", result);
	} else {
		msg("on_get_active_line_call() - result[%d] :[%s] ", result, dbg_call_result[result]);
		if (result == TEL_CALL_RESULT_SUCCESS){
			TelCallActiveLine *act = data;
			msg("on_get_active_line_call() - active_line : %d", *act);
		}
	}
}

static int run_call_get_active_line(MManager * mm, struct menu_data * menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_call_get_active_line()");

	result = tapi_call_get_active_line(handle, on_get_active_line_call, 0);
	CHECK_RT(result);
	return 0;
}

static int run_call_get_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;
	unsigned int call_id = 0;
	TelCallStatus info;

	memset(&info, '\0', sizeof(TelCallStatus));

	call_id = atoi(data_call_id);

	msg("call id : (%d)", call_id);

	msg("call tapi_call_get_status()");

	result = tapi_call_get_status(handle, call_id, &info);
	CHECK_RT(result);

	msg("id = %d", info.call_id);
	msg("type = %d", info.call_type);
	msg("state = %d", info.call_state);
	msg("mo_call = %d", info.mo_call);
	msg("multiparty = %d", info.mpty);
	msg("cli_validity = %d", info.cli_validity);
	msg("number = %s", info.number);
	msg("cni_validity= %d", info.cni_validity);
	msg("forward = %d", info.forward);
	msg("active_line = %d", info.active_line);

	return 0;
}


static int run_call_get_status_all(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;
	unsigned int iter;
	TelCallStatusList call_status_list;

	memset(&call_status_list, '\0', sizeof(TelCallStatusList));

	msg("call tapi_call_get_status_all()");

	result = tapi_call_get_status_all(handle, &call_status_list);
	CHECK_RT(result);
	msg("Total Call Results are [%d]", call_status_list.count);

	if (call_status_list.count<= TEL_CALL_MAX_CALLS_COUNT) {
		for (iter = 0; iter < call_status_list.count; iter++) {
			msg("Call No. = %d", iter+1);
			msg("id = %d", call_status_list.status[iter].call_id);
			msg("type = %d", call_status_list.status[iter].call_type);
			msg("state = %d", call_status_list.status[iter].call_state);
			msg("mo_call = %d", call_status_list.status[iter].mo_call);
			msg("multiparty = %d", call_status_list.status[iter].mpty);
			msg("cli_validity = %d", call_status_list.status[iter].cli_validity);
			msg("number = %s", call_status_list.status[iter].number);
			msg("cni_validity= %d", call_status_list.status[iter].cni_validity);
			msg("name = %s", call_status_list.status[iter].name);
			msg("forward = %d", call_status_list.status[iter].forward);
			msg("active_line = %d", call_status_list.status[iter].active_line);
			msg("");
		}
	} else {
			msg("Call status list count exceeded max count : [%d]", call_status_list.count);
	}
	return 0;
}

static void on_set_volume_info_call(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_set_volume_info_call :");
	CHECK_RESULT(result);
}

static int run_call_set_volume_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;
	TelCallVolumeInfo data;

	memset(&data, '\0', sizeof(TelCallVolumeInfo));

	data.device = atoi(data_vol_device);
	data.volume = atoi(data_vol_level);

	msg("device : (%d)", data.device);
	msg("volume : (%d)", data.volume);

	msg("call tapi_call_set_volume_info()");

	result = tapi_call_set_volume_info(handle, &data, on_set_volume_info_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_get_volume_info_call(TelHandle *handle, int result, void *data, void *user_data)
{
	if (result < TEL_CALL_RESULT_SUCCESS || result > TEL_CALL_RESULT_OPERATION_NOT_PERMITTED) {
		msg("Unidentified Result : %d", result);
	} else {
		msg("on_get_volume_info_call() - result[%d] :[%s] ", result, dbg_call_result[result]);
		if (result == TEL_CALL_RESULT_SUCCESS){
			int *volume = data;
			msg("on_get_volume_info_call() - volume : %d", *volume);
		}
	}
}

static int run_call_get_volume_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;
	TelCallSoundDevice device;

	device = atoi(data_vol_device);

	msg("call sound device : (%d)", device);

	msg("call tapi_call_get_volume_info()");

	result = tapi_call_get_volume_info(handle, device, on_get_volume_info_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_set_sound_path_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_set_sound_path_call :");
	CHECK_RESULT(result);
}

static int run_call_set_sound_path(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;
	TelCallSoundPathInfo sound_path_info;

	memset(&sound_path_info, '\0', sizeof(TelCallSoundPathInfo));

	sound_path_info.path = atoi(data_call_sound_path);
	sound_path_info.ex_volume = atoi(data_sound_ex_volume);

	msg("sound path : (%d)", sound_path_info.path);
	msg("sound ex-volume : (%d)", sound_path_info.ex_volume);

	msg("call tapi_call_set_sound_path()");

	result = tapi_call_set_sound_path(handle, &sound_path_info, on_set_sound_path_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_set_mute_call(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("on_set_mute_call :");
	CHECK_RESULT(result);
}

static int run_call_set_mute(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;
	gboolean status = 0;

	status = atoi(data_mute_status);

	msg("mute status : (%d)", status);

	msg("call tapi_call_set_mute()");

	result = tapi_call_set_mute(handle, status, on_set_mute_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_get_mute_status_call(TelHandle *handle, int result, void *data, void *user_data)
{
	if (result < TEL_CALL_RESULT_SUCCESS || result > TEL_CALL_RESULT_OPERATION_NOT_PERMITTED) {
		msg("Unidentified Result : %d", result);
	} else {
		msg("on_get_mute_status_call() - result[%d] :[%s] ", result, dbg_call_result[result]);
		if(result == TEL_CALL_RESULT_SUCCESS) {
			gboolean *mute = data;
			msg("on_get_mute_status_call() - status : %d", *mute);
		}
	}
}

static int run_call_get_mute_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;

	msg("call tapi_call_get_mute_status()");

	result = tapi_call_get_mute_status(handle, on_get_mute_status_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_set_sound_recording_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_set_sound_recording_call :");
	CHECK_RESULT(result);
}

static int run_call_set_sound_recording(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result = 0;
	TelCallSoundRecording sound_rec;

	sound_rec = atoi(data_sound_recording_status);

	msg("sound_recording_status : (%d)", sound_rec);

	msg("call tapi_call_set_sound_recording()");

	result = tapi_call_set_sound_recording(handle, sound_rec, on_set_sound_recording_call, 0);
	CHECK_RT(result);
	return 0;
}

static void on_set_sound_equalization_call(TelHandle * handle, int result, void *data, void *user_data)
{
	msg("on_set_sound_equalization_call :");
	CHECK_RESULT(result);
}

static int run_call_set_sound_equalization(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelCallSoundEqualization sound_eq;
	TelReturn result;

	memset(&sound_eq, '\0', sizeof(TelCallSoundEqualization));

	sound_eq.mode = atoi(data_sound_equalization_mode);
	sound_eq.direction = atoi(data_sound_direction);
/*  To be implemented later
	memcpy(sound_eq.parameter, data_sound_equalization_parameter, strlen(data_sound_equalization_parameter)); */

	msg("sound eq mode : (%d)", sound_eq.mode);
	msg("sound eq direction : (%d)", sound_eq.direction);

	msg("call tapi_call_set_sound_equalization()");

	result = tapi_call_set_sound_equalization(handle, &sound_eq, on_set_sound_equalization_call, 0);
	CHECK_RT(result);
	return 0;
}


static struct menu_data menu_call_dial[] = {
	{ "1", "type (0=VOICE, 1=DATA, 2=Emergency)", NULL, NULL, data_call_type},
	{ "2", "type (0=DEFAULT, 1=POLICE, 2=AMBULANCE, 4=FIREBRIGADE, 8=MARINEGUARD, 16=MOUNTAINRESCUE, 32=MANUAL_ECALL, 64=AUTO_ECALL)", NULL, NULL, data_call_ecc} ,
	{ "3", "number", NULL, NULL, data_call_number},
	{ "4", "run", 0, run_call_dial, 0},
	{ NULL, NULL, },
};

static struct menu_data menu_call_answer[] = {
	{ "1", "type (0=ACCEPT, 1=REJECT, 2=REPLACE, 3=HOLD&ACCEPT)", NULL, NULL, data_answer_type},
	{ "2", "run", NULL, run_call_answer, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_end[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "type (0=END, 1=END ALL, 2=END ACTIVE ALL, 3=END HOLD ALL)", NULL, NULL, data_end_type},
	{ "3", "run", NULL, run_call_end, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_send_dtmf[] = {
	{ "1", "Dtmf Character (0-9, A-D, '*', and '#')", NULL, NULL, data_dtmf_string},
	{ "2", "run", NULL, run_call_send_dtmf, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_hold[] = {
	{ "1", "run", NULL, run_call_hold, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_active[] = {
	{ "1", "run", NULL, run_call_active, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_swap[] = {
	{ "1", "run", NULL, run_call_swap, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_join[] = {
	{ "1", "run", NULL, run_call_join, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_split[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_split, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_transfer[] = {
	{ "1", "run", 0, run_call_transfer, 0},
	{ 0, 0, },
};

static struct menu_data menu_call_deflect[] = {
	{ "1", "deflect number", NULL, NULL, data_call_number},
	{ "2", "run", NULL, run_call_deflect, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_active_line[] = {
	{ "1", "type (0=LINE1, 1=LINE2 )", NULL, NULL, data_call_active_line_type},
	{ "2", "run", NULL, run_call_set_active_line, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_active_line[] = {
	{ "1", "run", NULL, run_call_get_active_line, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_status[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_get_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_status_all[] = {
	{ "1", "run", NULL, run_call_get_status_all, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_volume_info[] = {
	{ "1", "device (0=RECIEVER, 16=SPEAKER PHONE, 32=HFK, 48=HEADSET, 64=BLUETOOTH, 160=EC)", NULL, NULL, data_vol_device},
	{ "2", "volume(1 - 100) ", NULL, NULL, data_vol_level},
	{ "3", "run", NULL, run_call_set_volume_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_volume_info[] = {
	{ "1", "device (0=RECIEVER, 16=SPEAKER PHONE, 32=HFK, 48=HEADSET, 64=BLUETOOTH, 160=EC)", NULL, NULL, data_vol_device},
	{ "2", "run", NULL, run_call_get_volume_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_sound_path[] = {
	{ "1", "path (0=HANDSET, 1=HEADSET, 2=HANDSFREE, 3=BLUETOOTH, 4=STEREO_BLUETOOTH, 5=SPEAKER_PHONE, 6=HEADSET_3_5PI, 7=BLUETOOTH_NSEC_OFF, 8=MIC2, 9=MIC1, 10=HEADSET_HAC)", NULL, NULL, data_call_sound_path},
	{ "2", "ex_volume (0=OFF 1=ON)", NULL, NULL, data_sound_ex_volume},
	{ "3", "run", NULL, run_call_set_sound_path, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_mute[] = {
	{ "1", "status (0=MUTE_OFF, 1=MUTE_ON)", NULL, NULL, data_mute_status},
	{ "2", "run", NULL, run_call_set_mute, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_mute_status[] = {
	{ "1", "run", NULL, run_call_get_mute_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_sound_recording[] = {
	{ "1", "recording (0=STOP, 1=START, 2=PAUSE)", NULL, NULL, data_sound_recording_status},
	{ "2", "run", NULL, run_call_set_sound_recording, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_sound_equalization[] = {
	{ "1", "mode (0=OFF, 1=ON, 2=FLAG_OFF, 3=FLAG_ON, 4=SOFT1, 5=SOFT2)", NULL, NULL, data_sound_equalization_mode},
	{ "2", "direction (0=LEFT, 1=RIGHT)", NULL, NULL, data_sound_direction} ,
/* To be implemented Later
	{ "3", "parameter enter max six values", 0, 0, data_sound_equalization_parameter},*/
	{ "3", "run", NULL, run_call_set_sound_equalization, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_call[] = {
	{ "1", "tapi_call_dial", menu_call_dial, NULL, NULL},
	{ "2", "tapi_call_answer", menu_call_answer, NULL, NULL},
	{ "3", "tapi_call_end", menu_call_end, NULL, NULL},
	{ "4", "tapi_call_send_dtmf", menu_call_send_dtmf, NULL, NULL},
	{ "5", "tapi_call_hold", menu_call_hold, NULL, NULL},
	{ "6", "tapi_call_active", menu_call_active, NULL, NULL},
	{ "7", "tapi_call_swap", menu_call_swap, NULL, NULL},
	{ "8", "tapi_call_join", menu_call_join, NULL, NULL},
	{ "9", "tapi_call_split", menu_call_split, NULL, NULL},
	{ "10", "tapi_call_transfer", menu_call_transfer, NULL, NULL},
	{ "11", "tapi_call_deflect", menu_call_deflect, NULL, NULL},
	{ "12s", "tapi_call_set_active_line", menu_call_set_active_line, NULL, NULL},
	{ "12g", "tapi_call_get_active_line", menu_call_get_active_line, NULL, NULL},
	{ "13", "tapi_call_get_status", menu_call_get_status, NULL, NULL},
	{ "14", "tapi_call_get_status_all", menu_call_get_status_all, NULL, NULL},
	{ "15s", "tapi_call_set_volume_info", menu_call_set_volume_info, NULL, NULL},
	{ "15g", "tapi_call_get_volume_info", menu_call_get_volume_info, NULL, NULL},
	{ "16", "tapi_call_set_sound_path", menu_call_set_sound_path, NULL, NULL},
	{ "17s", "tapi_call_set_mute", menu_call_set_mute, NULL, NULL},
	{ "17g", "tapi_call_get_mute_status", menu_call_get_mute_status, NULL, NULL},
	{ "18", "tapi_call_set_sound_recording", menu_call_set_sound_recording, NULL, NULL},
	{ "19", "tapi_call_set_sound_equalization", menu_call_set_sound_equalization, NULL, NULL},
	{ NULL, NULL, },
};

void register_call_event(TelHandle *handle)
{
	TelReturn ret;

	ret = tapi_register_event_id(handle, TEL_NOTI_VOICE_CALL_STATUS_IDLE, on_noti_call_status_idle, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VOICE_CALL_STATUS_IDLE - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VIDEO_CALL_STATUS_IDLE, on_noti_call_status_idle, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VIDEO_CALL_STATUS_IDLE - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VOICE_CALL_STATUS_ACTIVE, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VOICE_CALL_STATUS_ACTIVE - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VOICE_CALL_STATUS_HELD, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VOICE_CALL_STATUS_HELD - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VOICE_CALL_STATUS_DIALING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VOICE_CALL_STATUS_DIALING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VOICE_CALL_STATUS_ALERT, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VOICE_CALL_STATUS_ALERT - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VOICE_CALL_STATUS_INCOMING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VOICE_CALL_STATUS_INCOMING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VIDEO_CALL_STATUS_ACTIVE, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VIDEO_CALL_STATUS_ACTIVE - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VIDEO_CALL_STATUS_DIALING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VIDEO_CALL_STATUS_DIALING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VIDEO_CALL_STATUS_ALERT, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VIDEO_CALL_STATUS_ALERT - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_VIDEO_CALL_STATUS_INCOMING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_VIDEO_CALL_STATUS_INCOMING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_WAITING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_WAITING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_HELD, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_HELD - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_ACTIVE, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_ACTIVE - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_JOINED, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_JOINED - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_FORWARDED, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_FORWARDED - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_BARRED_INCOMING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_BARRED_INCOMING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_BARRED_OUTGOING, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_BARRED_OUTGOING - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_FORWARD_UNCONDITIONAL, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_FORWARD_UNCONDITIONAL - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_FORWARD_CONDITIONAL, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_FORWARD_CONDITIONAL - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MT_FORWARDED, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MT_FORWARDED - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MT_DEFLECTED, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MT_DEFLECTED - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_MO_DEFLECTED, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_MO_DEFLECTED - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_INFO_TRANSFERED, on_noti_call_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_INFO_TRANSFERED - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_SOUND_RINGBACK_TONE, on_noti_call_sound_ringback_tone, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_SOUND_RINGBACK_TONE - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_SOUND_WBAMR, on_noti_call_sound_wbamr, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_SOUND_WBAMR - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_SOUND_EQUALIZATION, on_noti_call_sound_equalization, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_SOUND_EQUALIZATION - register event failed: [%d]", ret);

	ret = tapi_register_event_id(handle, TEL_NOTI_CALL_SOUND_CLOCK_STATUS, on_noti_call_sound_clock_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_CALL_SOUND_CLOCK_STATUS - register event failed: [%d]", ret);

}
