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

 /**
* @open
* @ingroup		TelephonyAPI
* @addtogroup	TAPI_SOUND	SOUND
* @{
*
* @file ITapiSound.h

     @brief This file serves as a "C" header file defines functions for Tapi Sound Services.\n
      It contains a sample set of function prototypes that would be required by applications.

	  Note: TAPI sound service API's are used by applications for configuration and control of MIC,SPEAKER and audio path\n

*/

#ifndef	_ITAPI_SOUND_H_
#define _ITAPI_SOUND_H_

#include <TelDefines.h>
#include <TelErr.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define TAPI_SOUND_VOLUME_TYPE		20	/**<Sound volume type	length is 20*/

/**
* @enum tapi_sound_type_t
*Sound type
*/
typedef enum {
	TAPI_SND_VOLUMN_CTRL			= 0x1,	/**< Sound Volume Control Message */
	TAPI_SND_MIC_MUTE_CTRL,			/**< Sound MIC Mute Control Message */
	TAPI_SND_AUDIO_PATH_CTRL,			/**< Audio Path Control Message */
	TAPI_SND_VOICE_RECORDING_CTRL    = 0x06,
	TAPI_SND_AUDIO_LOOPBACK_CTRL	= 0x10,	/**< Audio Loobpack Message */
	TAPI_SND_MASTER_CLOCK_CTRL,					/**< Audio Master Clock Message */
	TAPI_SND_MAX
} tapi_sound_type_t;

/**
* @enum tapi_sound_volume_t
*Sound volume
*/
typedef enum {
	TAPI_SOUND_MUTE		     =0x00,		/**<Sound is mute*/
	TAPI_SOUND_VOLUME_LEVEL_1=0x01,		/**<Volume level is 1*/
	TAPI_SOUND_VOLUME_LEVEL_2=0x02,		/**<Volume level is 2*/
	TAPI_SOUND_VOLUME_LEVEL_3=0x03,		/**<Volume level is 3*/
	TAPI_SOUND_VOLUME_LEVEL_4=0x04,		/**<Volume level is 4*/
	TAPI_SOUND_VOLUME_LEVEL_5=0x05,		/**<Volume level is 5*/
	TAPI_SOUND_VOLUME_LEVEL_6=0x06,		/**<Volume level is 6*/
	TAPI_SOUND_VOLUME_LEVEL_7=0x07,		/**<Volume level is 7*/
	TAPI_SOUND_VOLUME_LEVEL_8=0x08,		/**<Volume level is 8*/
	TAPI_SOUND_VOLUME_LEVEL_9=0x09		/**<Volume level is 9*/
} tapi_sound_volume_t;

/**
* @enum tapi_sound_volume_type_t
* Sound volume Type
*/
typedef enum {
      TAPI_SOUND_VOL_VOICE			=0x01,	/**< Voice volume*/
      TAPI_SOUND_VOL_KEYTONE	    =0x02,	/**< keytone volume*/
      TAPI_SOUND_VOL_BELL		=0x03,	/**< bell volume		*/
      TAPI_SOUND_VOL_MESSAGE	    =0x04,	/**< message tone volume*/
      TAPI_SOUND_VOL_ALARM		=0x05,	/**< alarm volume*/
      TAPI_SOUND_VOL_SPK_PHONE	    =0x11,	/**< Voice volume*/
      TAPI_SOUND_VOL_HFK_VOICE	    =0x21,	/**< HFK Voice volume*/
      TAPI_SOUND_VOL_HFK_KEYTONE    =0x22,	/**< HFK keytone volume*/
      TAPI_SOUND_VOL_HFK_BELL	=0x23,	/**< HFK bell volume*/
      TAPI_SOUND_VOL_HFK_MESSAGE	=0x24,	/**< HFK message volume*/
      TAPI_SOUND_VOL_HFK_ALARM	=0x25,	/**< HFK alarm volume*/
      TAPI_SOUND_VOL_HFK_PDA_MISC   =0x26,	/**< HFK PDA misc volume*/
      TAPI_SOUND_VOL_HEADSET_VOICE  =0x31,	/**< HFK Voice volume*/
      TAPI_SOUND_VOL_BT_VOICE	=0x41,	/**< HFK Voice volume*/
      TAPI_SOUND_VOL_TYPE_ALL	=0xFF,	/**< All volume types*/
} tapi_sound_volume_type_t;

typedef enum {
      TAPI_SOUND_MIC_UNMUTE	    =0x00,
      TAPI_SOUND_MIC_MUTE	        =0x01,
} tapi_sound_mic_mute_t;

/**
* @enum tapi_sound_audio_path_type
* Sound audio path type
*/
typedef enum {
      TAPI_SOUND_HANDSET			=0x01,		/**<Audio path is handset*/
      TAPI_SOUND_HEADSET	        =0x02,		/**<Audio path is handset*/
      TAPI_SOUND_HANDSFREE	        =0x03,		/**<Audio path is Handsfree*/
      TAPI_SOUND_BLUETOOTH	        =0x04,	/**<Audio path is bluetooth*/
      TAPI_SOUND_STEREO_BLUETOOTH   =0x05,	/**<Audio path is stereo bluetooth*/
      TAPI_SOUND_SPK_PHONE	        =0x06,	/**<Audio path is speaker phone*/
      TAPI_SOUND_HEADSET_3_5PI	    =0x07,	/**<Audio path is headset_3_5PI*/
      TAPI_SOUND_BT_NSEC_OFF	    =0x08,
      TAPI_SOUND_MIC1		    =0x09,
      TAPI_SOUND_MIC2		    =0x0A,
      TAPI_SOUND_HEADSET_HAC	    =0x0B,
} tapi_sound_audio_path_t;

typedef enum {
      TAPI_SOUND_SOURCE_PHONE	=0x01,	//Audio source is phone
      TAPI_SOUND_SOURCE_PDA	    =0x02,  //Audio source is pda
} tapi_sound_voice_source_t;

typedef enum {
	TAPI_SOUND_AUDIO_SOURCE_ON	= 0x01,
	TAPI_SOUND_AUDIO_SOURCE_OFF
} tapi_sound_audio_source_type_t;

typedef enum {
	TAPI_SOUND_VOICE_RECORDING_OFF	= 0x00,
	TAPI_SOUND_VOICE_RECORDING_ON   = 0x01,
} tapi_sound_voice_recording_status_t;

typedef enum {
	TAPI_SOUND_AUDIO_LOOPBACK_OFF,
	TAPI_SOUND_AUDIO_LOOPBACK_PCM_ON,
	TAPI_SOUND_AUDIO_LOOPBACK_PACKET_ON,
	TAPI_SOUND_AUDIO_LOOPBACK_MAX,
} TelAudioLoopbackStatus_t;

typedef struct {
	tapi_sound_audio_path_t path;
	TelAudioLoopbackStatus_t status;
} TelAudioLoopbackMode_t;

/**
* This structure defines sound volume info result control type.
*/
typedef struct {
	tapi_sound_volume_type_t	type;				/**<volume type*/
	tapi_sound_volume_t		level;				/**<volume level*/
} tapi_sound_volumn_info_res;

/**
* This structure defines sound volume control result type.
*/
typedef struct {
	char						num_record;					/**<Number of record*/
	tapi_sound_volumn_info_res	pinfo[TAPI_SOUND_VOLUME_TYPE];	/**<Volume info result*/
} tapi_sound_volumn_ctrl_res;

/**
* This structure defines sound volume control type.
*/
typedef struct {
	tapi_sound_volume_t		volume;			/**<volume*/
	tapi_sound_volume_type_t	vol_type;		/**<volume type*/
} tapi_sound_volume_control_t;

typedef struct {
	tapi_sound_type_t	type;
	tapi_phone_err_t	cause;
} tapi_sound_error_info_t;

/**
 *
 * @brief  This function is used to send to Telephony Server to get the volume status of the phone.
 * Application can make use this API.
 * This is permitted to the Samsung inhouse application only.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 *
 *
 * @param[in]  vol_control_type
 * -#tapi_sound_volume_type_t is the type of sound volume
 *
 * @param[out] pRequestId
 *  - Unique identifier for a particular request.
 *  (pRequestId value can be any value from 0 to 255. Invalid request_id has value -1)
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SOUND_VOLUMECTRL_RSP and the event data is #tapi_sound_volumn_ctrl_res.  Asynchronous return status
 *  is always success as TAPI_API_SUCCESS.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embeded call application
 *
 * @code
 * #include <ITapiSound.h>
 *
 * int ret_val = 0;
 * int requestId = -1;
 * tapi_sound_volume_type_t vol_type = TAPI_SOUND_VOL_VOICE;
 *
 * // GET VOLUME INFO
 * ret_val= tel_get_sound_volume_info (vol_type, &requestId);
 *
 * @endcode
 *
 * @see
 * - None
 *
 * @remarks
 * - None
 *
 *
 */
int	tel_get_sound_volume_info(tapi_sound_volume_type_t vol_control_type, int *pRequestId);

/**
 *
 * @brief  This function is used to send to Telephony Server to control the phone volume .
 * Application can make use this API.
 * This is permitted to the Samsung inhouse application only.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 *
 *
 * @param[in]  vol_control
 * -#tapi_sound_volume_control_t  is the sound volume control type
 *
 * @param [out] pRequestId
 *  - Unique identifier for a particular request.
 *  (pRequestId value can be any value from 0 to 255. Invalid request_id has value -1)
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embeded call application
 *
 * @code
 * #include <ITapiSound.h>
 *
 * int ret_val=0;
 * int requestId=-1;
 * tapi_sound_volume_control_t vol_control;
 * vol_control.volume=TAPI_SOUND_VOLUME_LEVEL_3;
 * vol_control.vol_type=TAPI_SOUND_VOL_VOICE;
 *
 * ret_val= tel_set_sound_volume_info(vol_control,&requestId); // volume level set
 *
 * @endcode
 *
 * @see
 * - None
 *
 * @remarks
 * - None
 *
 *
 */
int tel_set_sound_volume_info(tapi_sound_volume_control_t vol_control, int *pRequestId);

/**
 * @brief  This function is used to Send to Telephony Server to set voice path .
 * Application can make use this API.
 * This is permitted to the Samsung inhouse application only.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 *
 *
 * @param[in]   vc_path
 * -#tapi_sound_audio_path_t is the sound audio path
 *
 * @param [out] pRequestId
 *  - Unique identifier for a particular request.
 *  (pRequestId value can be any value from 0 to 255. Invalid request_id has value -1)
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embeded call application
 *
 *
 * @code
 * #include <ITapiSound.h>
 * tapi_sound_audio_path_t  vc_path;
 *
 * //VOICE PATH
 * vc_path=TAPI_SOUND_HANDSET;
 * ret_val= tel_set_sound_path (vc_path,&requestId); // sound path
 *
 * @endcode
 *
 * @see
 * - None
 *
 * @remarks
 * - None
 *
 *
 */
int tel_set_sound_path(tapi_sound_audio_path_t  vc_path, int *pRequestId);

/**
 * @brief  This function is used to Send to Telephony Server to control  the microphone as mute during the call.
 * Application can make use this API.
 * This is permitted to the Samsung inhouse application only.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 *
 *
 * @param[in]   micmute_set
 * -#tapi_sound_mic_mute_t is the status of mic mute
 *
 * @param [out] pRequestId
 *  - Unique identifier for a particular request.
 *  (pRequestId value can be any value from 0 to 255. Invalid request_id has value -1)

 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embeded call application
 *
 * @code
 * #include <ITapiSound.h>
 *
 * tapi_sound_mic_mute_t  set_micmute;
 * set_micmute=TAPI_SOUND_MIC_MUTE;
 * printf("tel_set_sound_mute_status \n");
 * ret_val= tel_set_sound_mute_status (set_micmute,&requestId); // set mute status
 *
 * @endcode
 *
 * @see
 * - None
 *
 * @remarks
 * - None
 *
 *
 */
int	tel_set_sound_mute_status(tapi_sound_mic_mute_t micmute_set, int * pRequestId);

/**
 * @brief  This function is used to Send to Telephony Server to get whether the microphone is muted or not .
 * Application can make use this API.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 *
 * @param [out] pRequestId
 *  - Unique identifier for a particular request.
 *  (pRequestId value can be any value from 0 to 255. Invalid request_id has value -1)
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SOUND_MICMUTECTRL_RSP and the event data is #tapi_sound_mic_mute_t.  Asynchronous return status
 *  is always success as TAPI_API_SUCCESS.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embeded call application
 *
 * @code
 * #include <ITapiSound.h>
 *
 * // GET MUTE STATUS
 * ret_val = tel_get_sound_mute_status(&requestId);
 *
 * @endcode
 *
 * @see
 * - None
 *
 * @remarks
 * - None
 *
 *
 */
int tel_get_sound_mute_status(int *pRequestId);


/**
 *
 * @brief  This function is invoked to test audio loopback
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [in]  Mode
 *    -  Audioloopback parameter
 *
 * @param  [in]  pRequestId
 *    -  Request ID
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_sound_loopback(TelAudioLoopbackMode_t *Mode, int *pRequestId);

/**
 *
 * @brief  This function is invoked to control audio clock
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [in]  bEnable
 *    -  Enable/Disable
 *
 * @param  [in]  pRequestId
 *    -  Request ID
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_sound_clock(int bEnable, int *pRequestId);

/**
 * @brief  This function is used to send to Telephony Server to control whether Voice recording is started or not.
 * Application can make use this API.
 * This is permitted to the Samsung inhouse application only.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 *
 * @param[in] voice_record
 *    -  tapi_sound_voice_recording_status_t
 *
 * @param [out] pRequestId
 *  - Unique identifier for a particular request.
 *  (pRequestId value can be any value from 0 to 255. Invalid request_id has value -1)
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 *
 */
int tel_control_sound_voice_recording(tapi_sound_voice_recording_status_t voice_record, int *pRequestId);

#ifdef __cplusplus
}
#endif

#endif //_ITAPI_SOUND_H_

/**
* @}
*/
