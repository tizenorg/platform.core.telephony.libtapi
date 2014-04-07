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

#include "tapi_private.h"
#include "tapi.h"
#include "tapi_call.h"

#include <tel_call.h>
#include <string.h>

static gboolean __tapi_check_call_type(TelCallType type)
{
	switch (type) {
	case TEL_CALL_TYPE_VOICE:
	case TEL_CALL_TYPE_VIDEO:
	case TEL_CALL_TYPE_E911:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_ecc(TelCallEmergencyCategory ecc)
{
	switch (ecc) {
	case TEL_CALL_ECC_DEFAULT:
	case TEL_CALL_ECC_POLICE:
	case TEL_CALL_ECC_AMBULANCE:
	case TEL_CALL_ECC_FIREBRIGADE:
	case TEL_CALL_ECC_MARINEGUARD:
	case TEL_CALL_ECC_MOUNTAINRESCUE:
	case TEL_CALL_ECC_MANUAL_ECALL:
	case TEL_CALL_ECC_AUTO_ECALL:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_answer_type(TelCallAnswerType type)
{
	switch (type) {
	case TEL_CALL_ANSWER_ACCEPT:
	case TEL_CALL_ANSWER_REJECT:
	case TEL_CALL_ANSWER_REPLACE:
	case TEL_CALL_ANSWER_HOLD_AND_ACCEPT:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_end_type(TelCallEndType type)
{
	switch (type) {
	case TEL_CALL_END:
	case TEL_CALL_END_ALL:
	case TEL_CALL_END_ACTIVE_ALL:
	case TEL_CALL_END_HOLD_ALL:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_active_line(TelCallActiveLine active_line)
{
	switch (active_line) {
	case TEL_CALL_ACTIVE_LINE1:
	case TEL_CALL_ACTIVE_LINE2:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_sound_device(TelCallSoundDevice dev)
{
	switch (dev) {
	case TEL_SOUND_DEVICE_RECEIVER:
	case TEL_SOUND_DEVICE_SPEAKER_PHONE:
	case TEL_SOUND_DEVICE_HFK:
	case TEL_SOUND_DEVICE_HEADSET:
	case TEL_SOUND_DEVICE_BLUETOOTH:
	case TEL_SOUND_DEVICE_EC:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_sound_rec(TelCallSoundRecording sound_rec)
{
	switch (sound_rec) {
	case TEL_SOUND_RECORDING_STOP:
	case TEL_SOUND_RECORDING_START:
	case TEL_SOUND_RECORDING_PAUSE:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_sound_direction(TelCallSoundDirection dir)
{
	switch (dir) {
	case TEL_SOUND_DIRECTION_LEFT:
	case TEL_SOUND_DIRECTION_RIGHT:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_sound_mode(TelCallSoundEqualizationMode mode)
{
	switch (mode) {
	case TEL_SOUND_EQUALIZATION_MODE_OFF:
	case TEL_SOUND_EQUALIZATION_MODE_ON:
	case TEL_SOUND_EQUALIZATION_MODE_FLAG_OFF:
	case TEL_SOUND_EQUALIZATION_MODE_FLAG_ON:
	case TEL_SOUND_EQUALIZATION_MODE_SOFT1:
	case TEL_SOUND_EQUALIZATION_MODE_SOFT2:
		return TRUE;
	}

	return FALSE;
}

static gboolean __tapi_check_sound_path(TelCallSoundPath path)
{
	switch (path) {
	case TEL_SOUND_PATH_HANDSET:
	case TEL_SOUND_PATH_HEADSET:
	case TEL_SOUND_PATH_HANDSFREE:
	case TEL_SOUND_PATH_BLUETOOTH:
	case TEL_SOUND_PATH_STEREO_BLUETOOTH:
	case TEL_SOUND_PATH_SPK_PHONE:
	case TEL_SOUND_PATH_HEADSET_3_5PI:
	case TEL_SOUND_PATH_BT_NSEC_OFF:
	case TEL_SOUND_PATH_MIC2:
	case TEL_SOUND_PATH_MIC1:
	case TEL_SOUND_PATH_HEADSET_HAC:
		return TRUE;
	}

	return FALSE;
}

void on_call_signal_emit_handler(TelephonyCall *call, gchar *sender_name,
	gchar *signal_name, GVariant *parameters, gpointer user_data)
{
	TapiEvtCbData *evt_cb_data;
	TelHandle *handle = user_data;
	char *evt_id;

	if (handle == NULL)
		return;

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_CALL_INTERFACE,
					signal_name);

	/*
	 * If an event callback is registered process
	 * g-signal event
	 */
	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "VoiceCallStatusIdle")
			|| !g_strcmp0(signal_name, "VideoCallStatusIdle")) {
		TelCallStatusIdleNoti idle_noti;

		g_variant_get(parameters, "(ui)", &idle_noti.call_id,
				&idle_noti.cause);

		dbg("%s call_id(%d), end_cause(0x%x)", signal_name,
			idle_noti.call_id, idle_noti.cause);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &idle_noti);
	} else if (!g_strcmp0(signal_name, "VoiceCallStatusDialing")
			|| !g_strcmp0(signal_name, "VideoCallStatusDialing")
			|| !g_strcmp0(signal_name, "VoiceCallStatusAlert")
			|| !g_strcmp0(signal_name, "VideoCallStatusAlert")
			|| !g_strcmp0(signal_name, "VoiceCallStatusActive")
			|| !g_strcmp0(signal_name, "VideoCallStatusActive")
			|| !g_strcmp0(signal_name, "VoiceCallStatusHeld")
			|| !g_strcmp0(signal_name, "VoiceCallStatusIncoming")
			|| !g_strcmp0(signal_name, "VideoCallStatusIncoming")) {
		guint call_id;
		g_variant_get(parameters, "(u)", &call_id);

		dbg("%s call_id(%d)", signal_name, call_id);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &call_id);
	} else if (!g_strcmp0(signal_name, "MoWaiting")
			|| !g_strcmp0(signal_name, "MoForwarded")
			|| !g_strcmp0(signal_name, "MoBarredIncoming")
			|| !g_strcmp0(signal_name, "MoBarredOutgoing")
			|| !g_strcmp0(signal_name, "MoForwardConditional")
			|| !g_strcmp0(signal_name, "MoForwardUnconditional")
			|| !g_strcmp0(signal_name, "MtForwarded")
			|| !g_strcmp0(signal_name, "MoDeflected")
			|| !g_strcmp0(signal_name, "MtDeflected")
			|| !g_strcmp0(signal_name, "Transfered")
			|| !g_strcmp0(signal_name, "CallActive")
			|| !g_strcmp0(signal_name, "CallHeld")
			|| !g_strcmp0(signal_name, "CallJoined")) {
		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, NULL);
	} else if (!g_strcmp0(signal_name, "CallSoundRingbackTone")) {
		TelCallSoundRingbackToneNoti rb_tone;;

		g_variant_get(parameters, "(i)", &rb_tone);
		dbg("%s (%d)", signal_name, rb_tone);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &rb_tone);
	} else if (!g_strcmp0(signal_name, "CallSoundWbamr")) {
		TelCallSoundWbamrNoti wbamr;

		g_variant_get(parameters, "(i)", &wbamr);
		dbg("%s (%d)", signal_name, wbamr);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &wbamr);
	} else if (!g_strcmp0(signal_name, "CallSoundEqualization")) {
		TelCallSoundEqualizationNoti equ_noti;

		g_variant_get(parameters, "(ii)", &equ_noti.mode,
					&equ_noti.direction);

		dbg("%s mode(%d), direction(%d)", signal_name, equ_noti.mode,
							equ_noti.direction);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &equ_noti);
	} else if (!g_strcmp0(signal_name, "CallSoundClockStatus")) {
		gboolean clock_status;

		g_variant_get(parameters, "(b)", &clock_status);
		dbg("%s clock_status(%s)", signal_name, clock_status ? "TRUE":"FALSE");

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &clock_status);
	} else
		err("Unsupported signal %s", signal_name);

	g_free(evt_id);
}


static void on_response_call_dial(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_dial_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_dial(TelHandle *handle,
	const TelCallDial *call_dial_info,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && call_dial_info && callback
			&& __tapi_check_ecc(call_dial_info->ecc)
			&& __tapi_check_call_type(call_dial_info->call_type),
			TEL_RETURN_INVALID_PARAMETER);

	/* If call type is not Emergency number must not be NULL */
	if (call_dial_info->call_type != TEL_CALL_TYPE_E911)
		TEL_RETURN_IF_CHECK_FAIL(strlen(call_dial_info->number) != 0,
						TEL_RETURN_INVALID_PARAMETER);

	dbg("Destination Number : %s", call_dial_info->number);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_dial(handle->call_proxy, call_dial_info->call_type,
				call_dial_info->ecc,
				call_dial_info->number, NULL,
				on_response_call_dial, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_answer(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_answer_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_answer(TelHandle *handle,
					TelCallAnswerType ans_type,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
					&& __tapi_check_answer_type(ans_type),
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_answer(handle->call_proxy, ans_type, NULL,
					on_response_call_answer, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_end(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_end_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_end(TelHandle *handle,
					const TelCallEnd *call_end_info,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && call_end_info
				&& __tapi_check_end_type(call_end_info->end_type),
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_end(handle->call_proxy, call_end_info->call_id,
				call_end_info->end_type, NULL,
				on_response_call_end, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_send_dtmf(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_send_dtmf_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_send_dtmf(TelHandle *handle,
						const char *dtmf_str,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && dtmf_str,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_send_dtmf(handle->call_proxy, dtmf_str, NULL,
					on_response_call_send_dtmf, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_hold(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_hold_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_hold(TelHandle *handle, TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_hold(handle->call_proxy, NULL,
					on_response_call_hold, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}


static void on_response_call_active(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_active_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_active(TelHandle *handle,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_active(handle->call_proxy, NULL,
					on_response_call_active, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_swap(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_swap_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_swap(TelHandle *handle, TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_swap(handle->call_proxy, NULL,
					on_response_call_swap, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_join(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_join_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_join(TelHandle *handle, TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_join(handle->call_proxy, NULL,
					on_response_call_join, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_split(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_split_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_split(TelHandle *handle, unsigned int call_id,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_split(handle->call_proxy, call_id, NULL,
					on_response_call_split, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_transfer(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_transfer_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_transfer(TelHandle *handle,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_transfer(handle->call_proxy, NULL,
					on_response_call_transfer, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_deflect(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_deflect_finish(handle->call_proxy, (int *)&result, res,
						&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_deflect(TelHandle *handle,
					const char *deflect_to,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && deflect_to,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_deflect(handle->call_proxy, deflect_to, NULL,
					on_response_call_deflect, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_set_active_line(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_set_active_line_finish(handle->call_proxy, (int *)&result,
							res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_set_active_line(TelHandle *handle,
						TelCallActiveLine active_line,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
					&& __tapi_check_active_line(active_line),
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_set_active_line(handle->call_proxy, active_line,
					NULL, on_response_call_set_active_line,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_get_active_line(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallActiveLine active_line;
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_get_active_line_finish(handle->call_proxy, (int *)&result,
							(int *)&active_line, res,
							&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_CALL_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &active_line);
	}
}

EXPORT_API TelReturn tapi_call_get_active_line(TelHandle *handle,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_get_active_line(handle->call_proxy,
		NULL, on_response_call_get_active_line, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static inline void __display_call_status(TelCallStatus *call_status)
{
	dbg("Call ID		%u", call_status->call_id);
	dbg("####################");
	dbg("Call type		%d", call_status->call_type);
	dbg("Call state		%d", call_status->call_state);
	dbg("Mobile %s", call_status->mo_call ? "Originated" : "Terminated");
	dbg("Multiparty		%s", call_status->mpty ? "YES" : "NO");
	dbg("Active line		%d", call_status->active_line);
	dbg("Caller Information:");
	dbg("CLI validity	%d", call_status->cli_validity);
	dbg("Number		%s", call_status->number);
	if (call_status->mo_call)
		return;
	dbg("CNI validity	%d", call_status->cni_validity);
	dbg("Name		%s", call_status->name);
	dbg("forward		%s", call_status->forward ? "YES" : "NO");
}

static void __set_call_info(TelCallStatus *call_status,
	unsigned int call_id, TelCallType call_type, TelCallState call_state,
	gboolean mo_call, gboolean mpty, TelCallCliValidity cli_validity,
	char *number, TelCallCniValidity cni_validity, char *name,
	gboolean forward, TelCallActiveLine active_line)
{
	call_status->call_id = call_id;
	call_status->call_type = call_type;
	call_status->call_state = call_state;
	call_status->mo_call = mo_call;
	call_status->mpty = mpty;
	call_status->active_line = active_line;
	call_status->cli_validity = cli_validity;
	if (number)
		g_strlcpy(call_status->number, number, TEL_CALL_CALLING_NUMBER_LEN_MAX + 1);
	/* Mobile terminated call */
	if (mo_call == TRUE)
		return;

	call_status->cni_validity = cni_validity;

	if (name)
		g_strlcpy(call_status->name, name, TEL_CALL_CALLING_NAME_LEN_MAX + 1);

	call_status->forward = forward;
}

EXPORT_API TelReturn tapi_call_get_status(TelHandle *handle,
	unsigned int call_id, TelCallStatus *call_status)
{
	TelReturn result = TEL_RETURN_FAILURE;
	GError *error = NULL;
	TelCallResult call_res;
	unsigned int out_cid;
	TelCallType call_type;
	TelCallState call_state;
	gboolean mo_call;
	gboolean mpty;
	TelCallCliValidity cli_validity;
	char *number;
	TelCallCniValidity cni_validity;
	char *name;
	gboolean forward;
	TelCallActiveLine active_line;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && call_status,
					TEL_RETURN_INVALID_PARAMETER);

	telephony_call_call_get_status_sync(handle->call_proxy, call_id,
		(int *)&call_res, &out_cid, (int *)&call_type,
		(int *)&call_state, &mo_call, &mpty, (int *)&cli_validity,
		&number, (int *)&cni_validity, &name, &forward,
		(int *)&active_line, NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		goto exit;
	}

	if (call_res != TEL_CALL_RESULT_SUCCESS) {
		err("GetStatus failed with error %d", call_res);
		goto exit;
	}
	dbg("number[%s]", number);

	__set_call_info(call_status, out_cid, call_type, call_state, mo_call,
		mpty, cli_validity, number, cni_validity,
		name, forward, active_line);

	g_free(number);
	g_free(name);

	result = TEL_RETURN_SUCCESS;

	__display_call_status(call_status);

exit:
	return result;
}

EXPORT_API TelReturn tapi_call_get_status_all(TelHandle *handle,
	TelCallStatusList *call_status_list)
{
	TelReturn result = TEL_RETURN_FAILURE;
	GError *error = NULL;
	int call_res;
	unsigned int count;
	GVariant *call_list;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && call_status_list,
					TEL_RETURN_INVALID_PARAMETER);

	telephony_call_call_get_status_all_sync(handle->call_proxy,
		&call_res, &count, &call_list, NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		goto out_error;
	}

	if (call_res != TEL_CALL_RESULT_SUCCESS) {
		err("GetStatusAll failed with error %d", call_res);
		g_variant_unref(call_list);
		goto out_error;
	}

	if ((count != 0) && (g_variant_n_children(call_list) == count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;

		unsigned int call_id = 0;
		TelCallType call_type = -1;
		TelCallState call_state = -1;
		gboolean mo_call = FALSE;
		gboolean mpty = FALSE;
		TelCallCliValidity cli_validity = -1;
		char *number = NULL;
		TelCallCniValidity cni_validity = -1;
		char *name = NULL;
		gboolean forward = FALSE;
		TelCallActiveLine active_line = -1;

		guint i = 0;

		dbg("Call status list count: [%d]", count);

		g_variant_get(call_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "call_id") == 0) {
					call_id = g_variant_get_uint32(key_value);
				}
				else if (g_strcmp0(key, "call_type") == 0) {
					call_type = g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "call_state") == 0) {
					call_state = g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "mo_call") == 0) {
					mo_call = g_variant_get_boolean(key_value);
				}
				else if (g_strcmp0(key, "mpty") == 0) {
					mpty = g_variant_get_boolean(key_value);
				}
				else if (g_strcmp0(key, "cli_validity") == 0) {
					cli_validity = g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "num") == 0) {
					number = g_strdup(g_variant_get_string(key_value, NULL));
				}
				else if (g_strcmp0(key, "cni_validity") == 0) {
					cni_validity = g_variant_get_int32(key_value);
				}
				else if (g_strcmp0(key, "name") == 0) {
					name = g_strdup(g_variant_get_string(key_value, NULL));
				}
				else if (g_strcmp0(key, "forward") == 0) {
					forward = g_variant_get_boolean(key_value);
				}
				else if (g_strcmp0(key, "active_line") == 0) {
					active_line = g_variant_get_int32(key_value);
				}
			}
			g_variant_iter_free(iter_row);

			/* Update out parameter */
			__set_call_info(&call_status_list->status[i],
				call_id, call_type, call_state, mo_call,
				mpty, cli_validity, number, cni_validity,
				name, forward, active_line);

			/* Free and reset the variables */
			g_free(number);
			g_free(name);
			call_id = 0;
			call_type = -1;
			call_state = -1;
			mo_call = FALSE;
			mpty = FALSE;
			cli_validity = -1;
			number = NULL;
			cni_validity = -1;
			name = NULL;
			forward = FALSE;
			active_line = -1;

			/* Print data */
			__display_call_status(&call_status_list->status[i]);
			i++;
		}
		g_variant_iter_free(iter);

		call_status_list->count = count;
		result = TEL_RETURN_SUCCESS;
	}
	else {
		call_status_list->count = 0;
	}
	g_variant_unref(call_list);

	dbg("Call count: [%d]", call_status_list->count);

out_error:
	return result;
}

static void on_response_call_set_volume_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_set_volume_info_finish(handle->call_proxy, (int *)&result,
							res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_set_volume_info(TelHandle *handle,
					const TelCallVolumeInfo *volume_info,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && volume_info
				&& __tapi_check_sound_device(volume_info->device)
				&& (volume_info->volume <= 100),
				TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_set_volume_info(handle->call_proxy,
					volume_info->device,
					volume_info->volume,
					NULL, on_response_call_set_volume_info,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_get_volume_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	unsigned int volume;
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_get_volume_info_finish(handle->call_proxy, (int *)&result,
							&volume, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_CALL_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &volume);
	}
}

EXPORT_API TelReturn tapi_call_get_volume_info(TelHandle *handle,
					TelCallSoundDevice sound_device,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && __tapi_check_sound_device(sound_device),
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_get_volume_info(handle->call_proxy, sound_device,
					NULL, on_response_call_get_volume_info,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_set_sound_path(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_set_sound_path_finish(handle->call_proxy, (int *)&result,
							res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_set_sound_path(TelHandle *handle,
				const TelCallSoundPathInfo *sound_path_info,
				TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && sound_path_info
				&& __tapi_check_sound_path(sound_path_info->path),
				TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_set_sound_path(handle->call_proxy,
					sound_path_info->path,
					sound_path_info->ex_volume, NULL,
					on_response_call_set_sound_path,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_set_mute(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_set_mute_finish(handle->call_proxy, (int *)&result,
							res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_set_mute(TelHandle *handle, gboolean mute,
				TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_set_mute(handle->call_proxy, mute,
					NULL, on_response_call_set_mute,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_get_mute_status(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	gboolean mute;
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_get_mute_status_finish(handle->call_proxy, (int *)&result,
							&mute, res,
							&error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_CALL_RESULT_SUCCESS) {
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
	} else {
		RESP_CALLBACK_CALL(rsp_cb_data, result, &mute);
	}
}

EXPORT_API TelReturn tapi_call_get_mute_status(TelHandle *handle,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_get_mute_status(handle->call_proxy, NULL,
					on_response_call_get_mute_status,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_set_sound_recording(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_set_sound_recording_finish(handle->call_proxy,
							(int *)&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_set_sound_recording(TelHandle *handle,
					TelCallSoundRecording sound_rec,
					TapiResponseCb callback,
					void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback
					&& __tapi_check_sound_rec(sound_rec),
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_call_call_set_sound_recording(handle->call_proxy, sound_rec,
					NULL,
					on_response_call_set_sound_recording,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_call_set_sound_equalization(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelCallResult result = TEL_CALL_RESULT_FAILURE;
	GError *error = NULL;

	dbg("Entry");

	telephony_call_call_set_sound_equalization_finish(handle->call_proxy,
								(int *)&result,
								res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_call_set_sound_equalization(TelHandle *handle,
				const TelCallSoundEqualization *sound_eq,
				TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariantBuilder *builder;
	GVariant *parameter;
	int i;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && sound_eq
				&& __tapi_check_sound_direction(sound_eq->direction)
				&& __tapi_check_sound_mode(sound_eq->mode),
				TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	builder = g_variant_builder_new(G_VARIANT_TYPE("aq"));
	for (i = 0; i < TEL_CALL_MAX_SOUND_EQ_PARAMETER_SIZE; i++)
		g_variant_builder_add(builder, "q", sound_eq->parameter[i]);

	parameter = g_variant_new ("aq", builder);
	g_variant_builder_unref (builder);

	telephony_call_call_set_sound_equalization(handle->call_proxy,
				sound_eq->direction, sound_eq->mode, parameter,
				NULL, on_response_call_set_sound_equalization,
				rsp_cb_data);

	g_variant_unref(parameter);

	return TEL_RETURN_SUCCESS;
}
