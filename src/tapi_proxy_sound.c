/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
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


#include "tel_cs_conn.h"
#include "tapi_proxy.h"
#include "ITapiSound.h"
#include "TapiUtility.h"
#include "TelErr.h"
#include "TelUtility.h"

extern tapi_dbus_connection_name conn_name;

/**
 *
 * send to Telephony Server to get the volume status of the phone
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]		tapi_sound_volume_type_t
 * @param[out]	None
 * @Interface		Asynchronous.
 * @remark
 * @Refer		tapi_sound_volume_control_set
 *
 */
EXPORT_API int tel_get_sound_volume_info(tapi_sound_volume_type_t vol_control_type, int *pRequestId)
{
	TS_BOOL ret;
	int api_err;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((vol_control_type != TAPI_SOUND_VOL_VOICE)
			&& (vol_control_type != TAPI_SOUND_VOL_KEYTONE)
			&& (vol_control_type != TAPI_SOUND_VOL_BELL)
			&& (vol_control_type != TAPI_SOUND_VOL_MESSAGE)
			&& (vol_control_type != TAPI_SOUND_VOL_ALARM)
			&& (vol_control_type != TAPI_SOUND_VOL_SPK_PHONE)
			&& (vol_control_type != TAPI_SOUND_VOL_HFK_VOICE)
			&& (vol_control_type != TAPI_SOUND_VOL_HFK_KEYTONE)
			&& (vol_control_type != TAPI_SOUND_VOL_HFK_BELL)
			&& (vol_control_type != TAPI_SOUND_VOL_HFK_MESSAGE)
			&& (vol_control_type != TAPI_SOUND_VOL_HFK_ALARM)
			&& (vol_control_type != TAPI_SOUND_VOL_HFK_PDA_MISC)
			&& (vol_control_type != TAPI_SOUND_VOL_HEADSET_VOICE)
			&& (vol_control_type != TAPI_SOUND_VOL_BT_VOICE)
			&& (vol_control_type != TAPI_SOUND_VOL_TYPE_ALL)) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid Input for vol_control_type = %x", vol_control_type);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
			TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &vol_control_type, sizeof(tapi_sound_volume_type_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_VOLUME_GET, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ",api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * send to Telephony Server to control the phone voulume
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]		tapi_sound_volume_control_t
 * @param[out]	None
 * @Interface		Asynchronous.
 * @remark
 * @Refer		tapi_sound_volume_control_get
 *
 */
EXPORT_API int tel_set_sound_volume_info(tapi_sound_volume_control_t vol_control, int *pRequestId)
{
	TS_BOOL ret;
	int api_err;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if ((vol_control.volume > TAPI_SOUND_VOLUME_LEVEL_9) || (vol_control.volume < TAPI_SOUND_MUTE)) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid Input for vol_control.volume = %x", vol_control.volume);
		return TAPI_API_INVALID_INPUT;
	}

	if ((vol_control.vol_type != TAPI_SOUND_VOL_VOICE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_KEYTONE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_BELL)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_MESSAGE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_ALARM)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_SPK_PHONE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HFK_VOICE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HFK_KEYTONE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HFK_KEYTONE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HFK_MESSAGE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HFK_ALARM)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HFK_PDA_MISC)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_HEADSET_VOICE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_BT_VOICE)
			&& (vol_control.vol_type != TAPI_SOUND_VOL_TYPE_ALL)) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid Input for vol_control.vol_type = %x", vol_control.vol_type);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
			TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &vol_control, sizeof(tapi_sound_volume_control_t));

	ret = tapi_send_request(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_VOLUME_SET, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ",api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * Send to Telephony Server to set voice path
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]		tapi_sound_audio_path_t
 * @param[out]	None
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 *
 **/
EXPORT_API int tel_set_sound_path(tapi_sound_audio_path_t vc_path, int *pRequestId)
{
	TS_BOOL ret;
	int api_err;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if ((vc_path > TAPI_SOUND_HEADSET_HAC) || (vc_path < TAPI_SOUND_HANDSET)) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid Input for vc_path = %x", vc_path);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &vc_path, sizeof(tapi_sound_audio_path_t));

	ret = tapi_send_request(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_AUDIO_PATH_CTRL, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ",api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * Send to Telehpny Server to control  the microphone as mute during the call.
 *
 * @param	tapi_sound_mic_mute_t
 * @param[out]	None
 * @Interface		Asynchronous.
 * @return	enum type TapiResult_t
 * @remark
 *
 * @Refer		tapi_sound_micmute_control_get
 */
EXPORT_API int tel_set_sound_mute_status(tapi_sound_mic_mute_t micmute_set, int *pRequestId)
{
	TS_BOOL ret;
	int api_err;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance... mic mute [%s] ", micmute_set? "MUTE" : "UNMUTE");
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if ((micmute_set > TAPI_SOUND_MIC_MUTE) || (micmute_set < TAPI_SOUND_MIC_UNMUTE)) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid Input for micmute_get = %x", micmute_set);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
			TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &micmute_set, sizeof(tapi_sound_mic_mute_t));

	ret = tapi_send_request(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_MIC_MUTE_SET, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ", api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * send to Telephony Server to get the status whether mic mute or not
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]		None
 * @param[out]	None
 * @Interface		Asynchronous.
 * @remark
 * @Refer		tapi_sound_micgain_control_set
 *
 */
EXPORT_API int tel_get_sound_mute_status(int *pRequestId)
{
	TS_BOOL ret;
	int api_err;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_MIC_MUTE_GET, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ",api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}


/**
 * Process the starting Audio Loopback test.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_sound_loopback(TelAudioLoopbackMode_t *Mode, int *pRequestId)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if (Mode->path > TAPI_SOUND_HEADSET_3_5PI || Mode->path < TAPI_SOUND_HANDSET) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Invalid Input for path = %x", Mode->path);
		return TAPI_API_INVALID_INPUT;
	}

	if (Mode->status > TAPI_SOUND_AUDIO_LOOPBACK_PACKET_ON || Mode->status < TAPI_SOUND_AUDIO_LOOPBACK_OFF) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Invalid Input for status = %x", Mode->status);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, Mode, sizeof(TelAudioLoopbackMode_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_AUDIOLOOPBACK_SET,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ",api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/**
 * Process the control of audio clock
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_set_sound_clock(int bEnable, int *pRequestId)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if (bEnable > TRUE || bEnable < FALSE) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Invalid Input for bEnable = %x", bEnable);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &bEnable, sizeof(int));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_AUDIO_CLOCK_CTRL,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ",api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/**
 *
 * send to Telephony Server to control the voice call recording
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]		None
 * @param[out]	None
 * @Interface		Asynchronous.
 * @remark
 * @Refer		tapi_sound_micgain_control_set
 *
 */
EXPORT_API int tel_control_sound_voice_recording(tapi_sound_voice_recording_status_t voice_record, int *pRequestId)
{
	TS_BOOL ret;
	int api_err;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance... voice_record [%d] ", voice_record);

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	if (voice_record < TAPI_SOUND_VOICE_RECORDING_OFF || voice_record > TAPI_SOUND_VOICE_RECORDING_ON) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Invalid Input for voice recoring = %x", voice_record);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &voice_record, sizeof(tapi_sound_voice_recording_status_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SOUND, TAPI_CS_SOUND_VOICE_RECORDING_SET,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %x ", api_err);
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

// eof
