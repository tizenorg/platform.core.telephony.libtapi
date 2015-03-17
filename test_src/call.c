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
#include <TelCall.h>
#include <ITapiCall.h>
#include <TapiUtility.h>

#include "menu.h"
#include "call.h"

static char data_call_id[MENU_DATA_SIZE + 1] = "1";
static char data_call_id2[MENU_DATA_SIZE + 1] = "2";
static char data_call_type[MENU_DATA_SIZE + 1] = "0";
static char data_dtmf_digit[MENU_DATA_SIZE + 1] = "1";
static char data_dtmf_string[MENU_DATA_SIZE + 1] = "12345";
static char data_dtmf_pulse_width[MENU_DATA_SIZE + 1] = "1";
static char data_dtmf_inter_digit_interval[MENU_DATA_SIZE + 1] = "1";
static char data_call_number[MENU_DATA_SIZE + 1] = "+821099777347";
static char data_answer_type[MENU_DATA_SIZE + 1] = "1";
static char data_end_type[MENU_DATA_SIZE + 1] = "1";
static char data_mute_path[MENU_DATA_SIZE + 1] = "0";
static char data_mute_status[MENU_DATA_SIZE + 1] = "0";
static char data_vol_device[MENU_DATA_SIZE + 1] = "0";
static char data_vol_type[MENU_DATA_SIZE + 1] = "1";
static char data_vol_level[MENU_DATA_SIZE + 1] = "1";
static char data_sound_path[MENU_DATA_SIZE + 1] = "1";
static char data_extra_volume_status[MENU_DATA_SIZE + 1] = "0";
static char data_call_privacy_mode[MENU_DATA_SIZE + 1] = "0";
static char data_preferred_voice_subscription[MENU_DATA_SIZE + 1] = "0";

static void on_noti_call_status_idle(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusIdleNoti_t *noti_data = (TelCallStatusIdleNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_VOICE_CALL_STATUS_IDLE);

	if (!noti_data)
		return;

	msg(" - id = %d", noti_data->id);
	msg(" - cause = 0x%x", noti_data->cause);
}

static void on_noti_call_status_active(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusActiveNoti_t *noti_data = (TelCallStatusActiveNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_VOICE_CALL_STATUS_ACTIVE);

	if (!noti_data)
		return;

	msg(" - id = %d", noti_data->id);
}

static void on_noti_call_status_held(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusHeldNoti_t *noti_data = (TelCallStatusHeldNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_VOICE_CALL_STATUS_HELD);

	if (!noti_data)
		return;

	msg(" - id = %d", noti_data->id);
}

static void on_noti_call_status_dialing(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusDialingNoti_t *noti_data = (TelCallStatusDialingNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_VOICE_CALL_STATUS_DIALING);

	if (!noti_data)
		return;

	msg(" - id = %d", noti_data->id);
}

static void on_noti_call_status_alert(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusAlertNoti_t *noti_data = (TelCallStatusAlertNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_VOICE_CALL_STATUS_ALERT);

	if (!noti_data)
		return;

	msg(" - id = %d", noti_data->id);
}

static void on_noti_call_status_incoming(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallStatusIncomingNoti_t *noti_data = (TelCallStatusIncomingNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_VOICE_CALL_STATUS_INCOMING);

	if (!noti_data)
		return;

	msg(" - id = %d", noti_data->id);
}

static void on_noti_call_sound_ringback_tone(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallSoundRingbackToneNoti_t *noti_data = (TelCallSoundRingbackToneNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_SOUND_RINGBACK_TONE);

	if (!noti_data)
		return;

	msg(" - Ringback tone status = 0x%x", *noti_data);
}

static void on_noti_call_sound_wbamr(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	gboolean *noti_data = (gboolean*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_SOUND_WBAMR);

	if (!noti_data)
		return;

	msg(" - status = 0x%x", *noti_data);
}

static void on_noti_call_preferred_voice_subscription(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallPreferredVoiceSubsNoti_t *noti_data = (TelCallPreferredVoiceSubsNoti_t *)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_PREFERRED_VOICE_SUBSCRIPTION);

	if (!noti_data)
		return;

	msg(" - preferred_subscription = 0x%x", noti_data->preferred_subs);
}

static void on_noti_call_sound_path(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSoundPath_t *noti_data = (TelSoundPath_t *)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_SOUND_PATH);

	if (!noti_data)
		return;

	msg(" - Sound path = 0x%x", *noti_data);
}

static void on_noti_call_on_hold(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	unsigned int *noti_data = (unsigned int*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_INFO_HELD);

	if (!noti_data)
		return;

	msg(" - ID = %d", *noti_data);
}

static void on_noti_call_active(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	unsigned int *noti_data = (unsigned int*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_INFO_ACTIVE);

	if (!noti_data)
		return;

	msg(" - ID = %d", *noti_data);
}

static void on_noti_call_joined(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	unsigned int *noti_data = (unsigned int*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_INFO_JOINED);

	if (!noti_data)
		return;

	msg(" - ID = %d", *noti_data);
}

static void on_noti_call_privacy_mode(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallVoicePrivacyNoti_t *noti_data = (TelCallVoicePrivacyNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_PRIVACY_MODE);

	if (!noti_data)
		return;

	msg(" Privacy Mode = %d", noti_data->privacy_mode);
}

static void on_noti_call_otasp_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallOtaspStatusNoti_t *noti_data = (TelCallOtaspStatusNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_OTASP_STATUS);

	if (!noti_data)
		return;

	msg(" OTASP Status = %d", noti_data->otasp_status);
}

static void on_noti_call_signal_info(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallSignalInfoNoti_t *noti_data = (TelCallSignalInfoNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_SIGNAL_INFO);

	if (!noti_data)
		return;
	if (noti_data->signal_type == TAPI_CALL_SIGNAL_TYPE_TONE) {
		msg(" Signal type = 0x%x, Pitch Type =0x%x, Signal = %d", noti_data->signal_type, noti_data->pitch_type, noti_data->signal.sig_tone_type);
	} else if (noti_data->signal_type == TAPI_CALL_SIGNAL_TYPE_ISDN_ALERTING) {
		msg(" Signal type = 0x%x, Pitch Type =0x%x, Signal = %d", noti_data->signal_type, noti_data->pitch_type, noti_data->signal.sig_isdn_alert_type);
	} else if (noti_data->signal_type == TAPI_CALL_SIGNAL_TYPE_IS54B_ALERTING) {
		msg(" Signal type = 0x%x, Pitch Type =0x%x, Signal = %d", noti_data->signal_type, noti_data->pitch_type, noti_data->signal.sig_is54b_alert_type);
	}
}

static void on_noti_call_otapa_status(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelCallOtapaStatusNoti_t *noti_data = (TelCallOtapaStatusNoti_t*)data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_CALL_OTAPA_STATUS);

	if (!noti_data)
		return;

	msg(" OTAPA Status = %d", noti_data->otapa_status);
}

static void on_dial_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_dial_call() - result : 0x%x", result);
}

static void on_answer_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallAnswerCnf_t *cnf_data = (TelCallAnswerCnf_t*)data;

	msg("on_answer_call() - cnf_data->id : %d", cnf_data->id);
}

static void on_end_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallEndCnf_t *cnf_data = (TelCallEndCnf_t*)data;

	msg("on_end_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_hold_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallHoldCnf_t *cnf_data = (TelCallHoldCnf_t*)data;

	msg("on_hold_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_active_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallActiveCnf_t *cnf_data = (TelCallActiveCnf_t*)data;

	msg("on_active_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_swap_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallSwapCnf_t *cnf_data = (TelCallSwapCnf_t*)data;

	msg("on_swap_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_join_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallJoinCnf_t *cnf_data = (TelCallJoinCnf_t*)data;

	msg("on_join_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_split_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallSplitCnf_t *cnf_data = (TelCallSplitCnf_t*)data;

	msg("on_split_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_transfer_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallTransferCnf_t *cnf_data = (TelCallTransferCnf_t*)data;

	msg("on_transfer_call() - cnf_data->id : %d result : 0x%x", cnf_data->id, result);
}

static void on_start_cont_dtmf_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_start_cont_dtmf_call() result : 0x%x", result);
}

static void on_stop_cont_dtmf_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_stop_cont_dtmf_call() result : 0x%x", result);
}

static void on_send_burst_dtmf_call(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_send_burst_dtmf_call() result : 0x%x", result);
}

static void on_call_set_mute_status(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_call_set_mute_status() - result : 0x%x", result);
}

static void on_call_get_mute_status(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallGetMuteStatusResp_t *resp = (TelCallGetMuteStatusResp_t *)data;

	msg("on_call_get_mute_status() - result : 0x%x", result);
	msg("on_call_get_mute_status() - path : 0x%x", resp->path);
	msg("on_call_get_mute_status() - status : 0x%x", resp->status);
}

static void on_call_set_volume_info(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_call_set_volume_info() - result : 0x%x", result);
}

static void on_call_get_volume_info(TapiHandle *handle, int result, void *data, void *user_data)
{
	unsigned int i = 0;
	TelCallGetVolumeInfoResp_t *resp = (TelCallGetVolumeInfoResp_t *)data;

	msg("on_call_get_volume_info() - result : 0x%x", result);
	msg("on_call_get_volume_info() - record_num : 0x%x", resp->record_num);

	for ( i=0; i<resp->record_num; i++ ) {
		msg("on_call_get_volume_info() - record[%d].device : 0x%x", (i+1), resp->record[i].device);
		msg("on_call_get_volume_info() - record[%d].type : 0x%x", (i+1), resp->record[i].device);
		msg("on_call_get_volume_info() - record[%d].volume : 0x%x", (i+1), resp->record[i].volume);

	}
}

static void on_call_set_sound_path(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_call_set_sound_path() - result : 0x%x", result);
}

static void on_call_set_preferred_voice_subscription(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_call_set_preferred_voice_subscription() - result : 0x%x", result);
}

static void on_call_get_privacy_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	TelCallGetPrivacyModeResp_t *resp_data = (TelCallGetPrivacyModeResp_t *) data;
	msg("on_call_get_privacy_mode() result : 0x%x, mode 0x%x", result, resp_data->mode);
}

static void on_call_set_privacy_mode(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("on_call_set_privacy_mode() result : 0x%x", result);
}

static int run_call_dial(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelCallDial_t data;
	int result;
	int len;

	memset(&data, 0, sizeof(TelCallDial_t));

	data.CallType = atoi(data_call_type);
	len = strlen(data_call_number);
	if (len > TAPI_CALL_DIALDIGIT_LEN_MAX) {
		msg("number too long");
		return 0;
	}

	memcpy(data.szNumber, data_call_number, len);

	msg("call tel_dail_call()");

	result = tel_dial_call(handle, &data, on_dial_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_answer(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	unsigned int call_id;
	TelCallAnswerType_t answer_type;


	msg("call tel_answer_call()");

	call_id = atoi(data_call_id);
	answer_type = atoi(data_answer_type);

	result = tel_answer_call(handle, call_id, answer_type, on_answer_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_end(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	unsigned int call_id;
	TelCallEndType_t end_type;

	msg("call tel_end_call()");

	call_id = atoi(data_call_id);
	end_type = atoi(data_end_type);

	result = tel_end_call(handle, call_id, end_type, on_end_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_hold(MManager *mm, struct menu_data *menu)
{
        TapiHandle *handle = menu_manager_ref_user_data(mm);
        int result;
        unsigned int call_id;

        msg("call tel_hold_call()");

        call_id = atoi(data_call_id);
        result = tel_hold_call(handle, call_id, on_hold_call, NULL);

        if (result != TAPI_API_SUCCESS) {
                msg("failed. (result = %d)", result);
        }

        return result;
}

static int run_call_join(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	unsigned int call_id;

	msg("call tel_join_call()");

	call_id = atoi(data_call_id);

	result = tel_join_call(handle, call_id, 0, on_join_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_active(MManager *mm, struct menu_data *menu)
{
        TapiHandle *handle = menu_manager_ref_user_data(mm);
        int result;
        unsigned int call_id;

        msg("call tel_active_call()");

        call_id = atoi(data_call_id);
        result = tel_active_call(handle, call_id, on_active_call, NULL);
        if (result != TAPI_API_SUCCESS) {
                msg("failed. (result = %d)", result);
        }

        return result;
}


static int run_call_split(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	unsigned int call_id;

	msg("call tel_split_call()");

	call_id = atoi(data_call_id);

	result = tel_split_call(handle, call_id, on_split_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_swap(MManager *mm, struct menu_data *menu)
{
        TapiHandle *handle = menu_manager_ref_user_data(mm);
        int result;
        unsigned int call_id;
        unsigned int call_id2;

        msg("call tel_swap_call()");

        call_id = atoi(data_call_id);
        call_id2 = atoi(data_call_id2);
        result = tel_swap_call(handle, call_id, call_id2, on_swap_call, NULL);

        if (result != TAPI_API_SUCCESS) {
                msg("failed. (result = %d)", result);
        }

        return result;
}


static int run_call_transfer(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	unsigned int call_id;


	msg("call tel_transfer_call()");

	call_id = atoi(data_call_id);

	result = tel_transfer_call(handle, call_id, on_transfer_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_start_cont_dtmf(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_start_call_cont_dtmf()");

	result = tel_start_call_cont_dtmf(handle, data_dtmf_digit[0], on_start_cont_dtmf_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_stop_cont_dtmf(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_stop_call_cont_dtmf()");

	result = tel_stop_call_cont_dtmf(handle, on_stop_cont_dtmf_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_send_burst_dtmf(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelCallBurstDtmf_t info;
	memset(&info, 0x0, sizeof(info));
	g_strlcpy(info.dtmf_string, data_dtmf_string, TAPI_CALL_BURST_DTMF_STRING_MAX + 1);
	info.pulse_width = data_dtmf_pulse_width[0];
	info.inter_digit_interval = data_dtmf_inter_digit_interval[0];

	msg("call tel_send_call_burst_dtmf()");

	result = tel_send_call_burst_dtmf(handle, &info,on_send_burst_dtmf_call, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}


static void call_status_callback( TelCallStatus_t *status, void *user_data )
{

	msg(" - id = %d", status->CallHandle);
	msg(" - direction = %d", status->bMoCall);
	msg(" - number = %s", status->pNumber);
	msg(" - type = %d", status->CallType);
	msg(" - state = %d", status->CallState);
	msg(" - multiparty = %d", status->bConferenceState);

}

static int run_call_get_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	unsigned int call_id = 0;
	TelCallStatus_t info;

	msg("call tel_get_call_status()");

	call_id = atoi(data_call_id);

	result = tel_get_call_status(handle, call_id, &info);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
		return result ;
	}

	msg(" - id = %d", info.CallHandle);
	msg(" - direction = %d", info.bMoCall);
	msg(" - number = %s", info.pNumber);
	msg(" - type = %d", info.CallType);
	msg(" - state = %d", info.CallState);
	msg(" - multiparty = %d", info.bConferenceState);

	return result;
}

static int run_call_get_status_all(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;

	msg("call tel_get_call_status_all()");

	result = tel_get_call_status_all(handle, call_status_callback, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_set_mute_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelSoundMutePath_t path = 0;
	TelSoundMuteStatus_t status = 0;

	msg("call tel_set_call_mute_status()");

	path = (TelSoundMutePath_t)atoi(data_mute_path);
	status = (TelSoundMuteStatus_t)atoi(data_mute_status);

	msg("path : (%d)", path);
	msg("status : (%d)", status);

	result = tel_set_call_mute_status(handle, status, path, on_call_set_mute_status, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_get_mute_status(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;

	msg("call tel_get_call_mute_status()");

	result = tel_get_call_mute_status(handle, on_call_get_mute_status, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_set_volume_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelCallVolumeInfo_t data;

	msg("call tel_set_call_volume_info()");

	data.device = (TelSoundDevice_t)atoi(data_vol_device);
	data.type = (TelSoundType_t)atoi(data_vol_type);
	data.volume = (TelSoundVolume_t)atoi(data_vol_level);

	msg("device : (%d)", data.device);
	msg("type : (%d)", data.type);
	msg("volume : (%d)", data.volume);

	result = tel_set_call_volume_info(handle, &data, on_call_set_volume_info, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_get_volume_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelSoundDevice_t device;
	TelSoundType_t type;

	msg("call tel_get_call_volume_info()");

	device = (TelSoundDevice_t)atoi(data_vol_device);
	type = (TelSoundType_t)atoi(data_vol_type);

	msg("device : (%d)", device);
	msg("type : (%d)", type);

	result = tel_get_call_volume_info(handle, device, type, on_call_get_volume_info, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_set_sound_path(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelCallSoundPathInfo_t sound_path ;

	msg("call tel_set_sound_path()");

	sound_path.path = (TelSoundPath_t)atoi(data_sound_path);
	sound_path.ex_volume = atoi(data_extra_volume_status);

	msg("sound_path.path : (0x%x)", sound_path.path);
	msg("sound_path.ex_volume : (0x%x)", sound_path.ex_volume);

	result = tel_set_call_sound_path(handle, &sound_path, on_call_set_sound_path, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_set_preferred_voice_subscription(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelCallPreferredVoiceSubs_t preferred_subscription;

	msg("call run_call_set_preferred_voice_subscription()");

	preferred_subscription = (TelCallPreferredVoiceSubs_t)atoi(data_preferred_voice_subscription);

	msg("preferred_subscription : (%d)", preferred_subscription);

	result = tel_set_call_preferred_voice_subscription(handle, preferred_subscription, on_call_set_preferred_voice_subscription, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}

	return result;
}

static int run_call_get_preferred_voice_subscription(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result = 0;
	TelCallPreferredVoiceSubs_t preferred_subscription = TAPI_CALL_PREFERRED_VOICE_SUBS_UNKNOWN;

	msg("call tel_get_call_preferred_voice_subscription()");

	result = tel_get_call_preferred_voice_subscription(handle, &preferred_subscription);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
	}
	else {
		msg("'preferred' Voice call subscription = (%d)", preferred_subscription);
	}

	return result;
}

static int run_call_get_call_privacy_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call tel_get_call_privacy_mode()");


	result = tel_get_call_privacy_mode(handle, on_call_get_privacy_mode, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
		return result ;
	}
	return result;
}

static int run_call_set_call_privacy_mode(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;
	TelCallPrivacyMode_t privacy_mode;

	msg("call tel_set_call_privacy_mode()");

	privacy_mode = atoi(data_call_privacy_mode);
	msg("privacy_mode : (0x%x)", privacy_mode);

	result = tel_set_call_privacy_mode(handle, privacy_mode, on_call_set_privacy_mode, NULL);
	if (result != TAPI_API_SUCCESS) {
		msg("failed. (result = %d)", result);
		return result ;
	}

	return result;
}


static struct menu_data menu_call_dial[] = {
	{ "1", "type (0=VOICE, 1=VIDEO, 2=Emergency)", NULL, NULL, data_call_type},
	{ "2", "number", NULL, NULL, data_call_number},
	{ "3", "run", NULL, run_call_dial, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_answer_3gpp[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "type (0=ACCEPT, 1=REJECT, 2=REPLACE, 3=HOLD&ACCEPT)", NULL, NULL, data_answer_type},
	{ "3", "run", NULL, run_call_answer, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_answer_3gpp2[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "type (0=ACCEPT, 1=REJECT, 3=HOLD&ACCEPT)", NULL, NULL, data_answer_type},
	{ "3", "run", NULL, run_call_answer, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_end_3gpp[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "type (0=END, 1=END ALL, 2=END ACTIVE ALL, 3=END HOLD ALL)", NULL, NULL, data_end_type},
	{ "3", "run", NULL, run_call_end, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_end_3gpp2[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "type (1=END ALL)", NULL, NULL, data_end_type},
	{ "3", "run", NULL, run_call_end, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_hold[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_hold, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_active[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_active, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_swap[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "id2", NULL, NULL, data_call_id2},
	{ "3", "run", NULL, run_call_swap, NULL},
};

static struct menu_data menu_call_join[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_join, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_split[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_split, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_transfer[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_transfer, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_start_cont_dtmf[] = {
	{ "1", "Dtmf Charater(0-9, A-D, ' * ' and ' # ') one character at a time ", NULL, NULL, data_dtmf_digit},
	{ "2", "run", NULL, run_call_start_cont_dtmf, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_stop_cont_dtmf[] = {
	{ "1", "run", NULL, run_call_stop_cont_dtmf, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_send_burst_dtmf[] = {
	{ "1", "Dtmf String(0-9, A-D, ' * ' and ' # ') one character or a string of characters ", NULL, NULL, data_dtmf_string},
	{ "2", "DTMF pulse width (ONLENGTH_95MS=0x0, ONLENGTH_150MS=0x1, ONLENGTH_200MS=0x2, ONLENGTH_250MS=0x3, ONLENGTH_300MS=0x4, ONLENGTH_350MS=0x5, ONLENGTH_SMS=0x6)", NULL, NULL, data_dtmf_pulse_width},
	{ "3", "DTMF inter digit interval (OFFLENGTH_60MS=0x0, OFFLENGTH_100MS=0x1, OFFLENGTH_150MS=0x2, OFFLENGTH_200MS=0x3)", NULL, NULL, data_dtmf_inter_digit_interval},
	{ "4", "run", NULL, run_call_send_burst_dtmf, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_status[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_get_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_status_all[] = {
	{ "1", "id", NULL, NULL, data_call_id},
	{ "2", "run", NULL, run_call_get_status_all, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_mute_status[] = {
	{ "1", "status", NULL, NULL, data_mute_status},
	{ "2", "path", NULL, NULL, data_mute_path},
	{ "3", "run", NULL, run_call_set_mute_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_mute_status[] = {
	{ "1", "run", NULL, run_call_get_mute_status, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_volume_info[] = {
	{ "1", "device", NULL, NULL, data_vol_device},
	{ "2", "type", NULL, NULL, data_vol_type},
	{ "3", "volume", NULL, NULL, data_vol_level},
	{ "4", "run", NULL, run_call_set_volume_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_get_volume_info[] = {
	{ "1", "device", NULL, NULL, data_vol_device},
	{ "2", "type", NULL, NULL, data_vol_type},
	{ "3", "run", NULL, run_call_get_volume_info, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_sound_path[] = {
	{ "1", "Sound path(1= HANDSET, 2= HEADSET, 3= HANDSFREE, 4=BUETOOTH, 5=STEREO_BLUETOOTH, 6=SPK_PHONE, 7=HEADSET_5_PI, 8=BT_NSCE_OFF, 9=MIC1, 10=MIC2, 11=HEADSET_HAC)", NULL, NULL, data_sound_path},
	{ "2", "Extra Volume Status( 0=OFF, 1=ON)", NULL, NULL, data_extra_volume_status},
	{ "3", "run", NULL, run_call_set_sound_path, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_preferred_voice_subscription[] = {
	{ "1", "Preferred Voice call Subcription (0=Current network, 1=ASK Always, 2=SIM 1, 3=SIM 2)", 0, 0, data_preferred_voice_subscription},
	{ "2", "run", 0, run_call_set_preferred_voice_subscription, 0},
	{ 0, 0, },
};

static struct menu_data menu_call_get_preferred_voice_subscription[] = {
	{ "1", "run", 0, run_call_get_preferred_voice_subscription, 0},
	{ 0, 0, },
};

static struct menu_data menu_call_get_call_privacy_mode[] = {
	{ "1", "run", NULL, run_call_get_call_privacy_mode, NULL},
	{ NULL, NULL, },
};

static struct menu_data menu_call_set_call_privacy_mode[] = {
	{ "1", "Call Privacy Mode(0=STANDARD, 1=ENHANCED)", NULL, NULL, data_call_privacy_mode},
	{ "2", "run", NULL, run_call_set_call_privacy_mode, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_call_3gpp[] = {
	{ "1", "tel_dial_call", menu_call_dial, NULL, NULL},
	{ "2", "tel_answer_call", menu_call_answer_3gpp, NULL, NULL},
	{ "3", "tel_end_call", menu_call_end_3gpp, NULL, NULL},
	{ "4", "tel_hold_call", menu_call_hold, NULL, NULL},
	{ "5", "tel_active_call", menu_call_active, NULL, NULL},
	{ "6", "tel_swap_call", menu_call_swap, NULL, NULL},
	{ "7", "tel_join_call", menu_call_join, NULL, NULL},
	{ "8", "tel_split_call", menu_call_split, NULL, NULL},
	{ "9", "tel_transfer_call", menu_call_transfer, NULL, NULL},
	{ "10", "tel_start_call_cont_dtmf_call", menu_call_start_cont_dtmf, NULL, NULL},
	{ "11", "tel_stop_call_cont_dtmf_call", menu_call_stop_cont_dtmf, NULL, NULL},
	{ "12", "tel_send_call_burst_dtmf", menu_call_send_burst_dtmf, NULL, NULL},
	{ "13", "tel_get_call_status", menu_call_get_status, NULL, NULL},
	{ "14", "tel_get_call_status_all", menu_call_get_status_all, NULL, NULL},
	{ "15", "tel_set_call_mute_status", menu_call_set_mute_status, NULL, NULL},
	{ "16", "tel_get_call_mute_status", menu_call_get_mute_status, NULL, NULL},
	{ "17", "tel_set_call_volume_info", menu_call_set_volume_info, NULL, NULL},
	{ "18", "tel_get_call_volume_info", menu_call_get_volume_info, NULL, NULL},
	{ "19", "tel_set_call_sound_path", menu_call_set_sound_path, NULL, NULL},
       { "20", "tel_set_call_preferred_voice_subscription", menu_call_set_preferred_voice_subscription, 0, 0},
       { "21", "tel_get_call_preferred_voice_subscription", menu_call_get_preferred_voice_subscription, 0, 0},
	{ NULL, NULL, },
};

struct menu_data menu_call_3gpp2[] = {
	{ "1", "tel_dial_call", menu_call_dial, NULL, NULL},
	{ "2", "tel_answer_call", menu_call_answer_3gpp2, NULL, NULL},
	{ "3", "tel_end_call", menu_call_end_3gpp2, NULL, NULL},
	{ "4", "tel_swap_call", menu_call_swap, NULL, NULL},
	{ "5", "tel_join_call", menu_call_join, NULL, NULL},
	{ "6", "tel_get_call_status", menu_call_get_status, NULL, NULL},
	{ "7", "tel_start_call_cont_dtmf", menu_call_start_cont_dtmf, NULL, NULL},
	{ "8", "tel_stop_call_cont_dtmf", menu_call_stop_cont_dtmf, NULL, NULL},
	{ "9", "tel_send_call_burst_dtmf", menu_call_send_burst_dtmf, NULL, NULL},
	{ "10", "tel_get_call_status_all", menu_call_get_status_all, NULL, NULL},
	{ "11", "tel_get_call_privacy_mode", menu_call_get_call_privacy_mode, NULL, NULL},
	{ "12", "tel_set_call_privacy_mode", menu_call_set_call_privacy_mode, NULL, NULL},
	{ "13", "tel_set_call_mute_status", menu_call_set_mute_status, NULL, NULL},
	{ "14", "tel_get_call_mute_status", menu_call_get_mute_status, NULL, NULL},
	{ "15", "tel_set_call_volume_info", menu_call_set_volume_info, NULL, NULL},
	{ "16", "tel_get_call_volume_info", menu_call_get_volume_info, NULL, NULL},
	{ "17", "tel_set_call_sound_path", menu_call_set_sound_path, NULL, NULL},
	{ NULL, NULL, },
};

struct menu_data menu_call[] = {
	{ "1", "3GPP(GSM/WCDMA/LTE)", menu_call_3gpp, NULL, NULL},
	{ "2", "3GPP2(CDMA)", menu_call_3gpp2, NULL, NULL},
	{ NULL, NULL, },
};

void register_call_event(TapiHandle *handle)
{
	int ret;

	ret = tel_register_noti_event(handle, TAPI_NOTI_VOICE_CALL_STATUS_IDLE, on_noti_call_status_idle, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_VOICE_CALL_STATUS_ACTIVE, on_noti_call_status_active, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_VOICE_CALL_STATUS_HELD, on_noti_call_status_held, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_VOICE_CALL_STATUS_DIALING, on_noti_call_status_dialing, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_VOICE_CALL_STATUS_ALERT, on_noti_call_status_alert, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_VOICE_CALL_STATUS_INCOMING, on_noti_call_status_incoming, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_INFO_HELD, on_noti_call_on_hold, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_INFO_ACTIVE, on_noti_call_active, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_INFO_JOINED, on_noti_call_joined, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_PRIVACY_MODE, on_noti_call_privacy_mode, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_OTASP_STATUS, on_noti_call_otasp_status, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_OTAPA_STATUS, on_noti_call_otapa_status, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_SIGNAL_INFO, on_noti_call_signal_info, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_SOUND_RINGBACK_TONE, on_noti_call_sound_ringback_tone, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_SOUND_WBAMR, on_noti_call_sound_wbamr, 0);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_SOUND_PATH, on_noti_call_sound_path, NULL);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}

	ret = tel_register_noti_event(handle, TAPI_NOTI_CALL_PREFERRED_VOICE_SUBSCRIPTION, on_noti_call_preferred_voice_subscription, 0);
	if (ret != TAPI_API_SUCCESS) {
		msg("event register failed(%d)", ret);
	}
}

