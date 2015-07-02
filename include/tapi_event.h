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

#ifndef __TAPI_EVENT_H__
#define __TAPI_EVENT_H__

#include <tapi_type.h>

__BEGIN_DECLS

/**
 * @file tapi_event.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE
 * @{
 */

/**
 * @brief (Needs to be updated).
 */
#define DBUS_TELEPHONY_CALL_INTERFACE				DBUS_TELEPHONY_SERVICE".Call"

#define TAPI_NOTI_VOICE_CALL_STATUS_IDLE			DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusIdle"
#define TAPI_NOTI_VOICE_CALL_STATUS_ACTIVE			DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusActive"
#define TAPI_NOTI_VOICE_CALL_STATUS_HELD			DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusHeld"
#define TAPI_NOTI_VOICE_CALL_STATUS_DIALING			DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusDialing"
#define TAPI_NOTI_VOICE_CALL_STATUS_ALERT			DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusAlert"
#define TAPI_NOTI_VOICE_CALL_STATUS_INCOMING		DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusIncoming"
#define TAPI_NOTI_VOICE_CALL_STATUS_WAITING			DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusWaiting"

#define TAPI_NOTI_VIDEO_CALL_STATUS_IDLE			DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusIdle"
#define TAPI_NOTI_VIDEO_CALL_STATUS_ACTIVE			DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusActive"
#define TAPI_NOTI_VIDEO_CALL_STATUS_DIALING			DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusDialing"
#define TAPI_NOTI_VIDEO_CALL_STATUS_ALERT			DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusAlert"
#define TAPI_NOTI_VIDEO_CALL_STATUS_INCOMING		DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusIncoming"

#define TAPI_NOTI_CALL_INFO_CALL_CONNECTED_LINE		DBUS_TELEPHONY_CALL_INTERFACE":CallConnectedLine"
#define TAPI_NOTI_CALL_INFO_WAITING					DBUS_TELEPHONY_CALL_INTERFACE":Waiting"
#define TAPI_NOTI_CALL_INFO_CUG						DBUS_TELEPHONY_CALL_INTERFACE":Cug"
#define TAPI_NOTI_CALL_INFO_FORWARDED				DBUS_TELEPHONY_CALL_INTERFACE":Forwarded"
#define TAPI_NOTI_CALL_INFO_BARRED_INCOMING			DBUS_TELEPHONY_CALL_INTERFACE":BarredIncoming"
#define TAPI_NOTI_CALL_INFO_BARRED_OUTGOING			DBUS_TELEPHONY_CALL_INTERFACE":BarredOutgoing"
#define TAPI_NOTI_CALL_INFO_DEFLECTED				DBUS_TELEPHONY_CALL_INTERFACE":Deflected"
#define TAPI_NOTI_CALL_INFO_CLIR_SUPPRESSION_REJECT	DBUS_TELEPHONY_CALL_INTERFACE":ClirSuppressionReject"
#define TAPI_NOTI_CALL_INFO_FORWARD_UNCONDITIONAL	DBUS_TELEPHONY_CALL_INTERFACE":ForwardUnconditional"
#define TAPI_NOTI_CALL_INFO_FORWARD_CONDITIONAL		DBUS_TELEPHONY_CALL_INTERFACE":ForwardConditional"
#define TAPI_NOTI_CALL_INFO_CALL_LINE_IDENTITY		DBUS_TELEPHONY_CALL_INTERFACE":CallLineIdentity"
#define TAPI_NOTI_CALL_INFO_CALL_NAME_INFORMATION	DBUS_TELEPHONY_CALL_INTERFACE":CallNameInformation"
#define TAPI_NOTI_CALL_INFO_FORWARDED_CALL			DBUS_TELEPHONY_CALL_INTERFACE":ForwardedCall"
#define TAPI_NOTI_CALL_INFO_CUG_CALL				DBUS_TELEPHONY_CALL_INTERFACE":CugCall"
#define TAPI_NOTI_CALL_INFO_DEFLECTED_CALL			DBUS_TELEPHONY_CALL_INTERFACE":DeflectedCall"
#define TAPI_NOTI_CALL_INFO_TRANSFERED_CALL			DBUS_TELEPHONY_CALL_INTERFACE":TransferedCall"
#define TAPI_NOTI_CALL_INFO_HELD					DBUS_TELEPHONY_CALL_INTERFACE":CallHeld"
#define TAPI_NOTI_CALL_INFO_ACTIVE					DBUS_TELEPHONY_CALL_INTERFACE":CallActive"
#define TAPI_NOTI_CALL_INFO_JOINED					DBUS_TELEPHONY_CALL_INTERFACE":CallJoined"
#define TAPI_NOTI_CALL_INFO_RELEASED_ON_HOLD		DBUS_TELEPHONY_CALL_INTERFACE":ReleaseOnHold"
#define TAPI_NOTI_CALL_INFO_TRANSFER_ALERT			DBUS_TELEPHONY_CALL_INTERFACE":TransferAlert"
#define TAPI_NOTI_CALL_INFO_TRANSFERED				DBUS_TELEPHONY_CALL_INTERFACE":Transfered"
#define TAPI_NOTI_CALL_INFO_CF_CHECK_MESSAGE		DBUS_TELEPHONY_CALL_INTERFACE":CfCheckMessage"
#define TAPI_NOTI_CALL_INFO_REC						DBUS_TELEPHONY_CALL_INTERFACE":CallInfoRec"
#define TAPI_NOTI_CALL_INFO_FALLBACK 					DBUS_TELEPHONY_CALL_INTERFACE:":CallFallback"
#define TAPI_NOTI_CALL_PRIVACY_MODE					DBUS_TELEPHONY_CALL_INTERFACE":CallPrivacyMode"
#define TAPI_NOTI_CALL_OTASP_STATUS					DBUS_TELEPHONY_CALL_INTERFACE":CallOtaspStatus"
#define TAPI_NOTI_CALL_OTAPA_STATUS					DBUS_TELEPHONY_CALL_INTERFACE":CallOtapaStatus"
#define TAPI_NOTI_CALL_SIGNAL_INFO				DBUS_TELEPHONY_CALL_INTERFACE":CallSignalInfo"
#define TAPI_NOTI_CALL_MODIFY_REQUEST		DBUS_TELEPHONY_CALL_INTERFACE":CallModifyRequest" /* VoLTE only */

#define TAPI_NOTI_CALL_SOUND_PATH					DBUS_TELEPHONY_CALL_INTERFACE":CallSoundPath"
#define TAPI_NOTI_CALL_SOUND_RINGBACK_TONE			DBUS_TELEPHONY_CALL_INTERFACE":CallSoundRingbackTone"
#define TAPI_NOTI_CALL_SOUND_WBAMR					DBUS_TELEPHONY_CALL_INTERFACE":CallSoundWbamr"
#define TAPI_NOTI_CALL_SOUND_EQUALIZATION			DBUS_TELEPHONY_CALL_INTERFACE":CallSoundEqualiztion"
#define TAPI_NOTI_CALL_SOUND_NOISE_REDUCTION		DBUS_TELEPHONY_CALL_INTERFACE":CallSoundNoiseReduction"
#define TAPI_NOTI_CALL_SOUND_CLOCK_STATUS			DBUS_TELEPHONY_CALL_INTERFACE":CallSoundClockStatus"
#define TAPI_NOTI_CALL_PREFERRED_VOICE_SUBSCRIPTION		DBUS_TELEPHONY_CALL_INTERFACE":CallPreferredVoiceSubscription"


#define DBUS_TELEPHONY_MODEM_INTERFACE				DBUS_TELEPHONY_SERVICE".Modem"
#define TAPI_NOTI_MODEM_POWER						DBUS_TELEPHONY_MODEM_INTERFACE":Power"
#define TAPI_PROP_MODEM_POWER						DBUS_TELEPHONY_MODEM_INTERFACE":power"
#define TAPI_PROP_MODEM_DONGLE_STATUS				DBUS_TELEPHONY_MODEM_INTERFACE":dongle_status" /* Dongle inserted or not */
#define TAPI_PROP_MODEM_DONGLE_LOGIN				DBUS_TELEPHONY_MODEM_INTERFACE":dongle_login"  /* Dongle Login success or not */

// SIM
#define DBUS_TELEPHONY_SIM_INTERFACE				DBUS_TELEPHONY_SERVICE".Sim"
#define TAPI_NOTI_SIM_STATUS						DBUS_TELEPHONY_SIM_INTERFACE":Status"
#define TAPI_NOTI_SIM_REFRESHED						DBUS_TELEPHONY_SIM_INTERFACE":Refreshed"
#define TAPI_PROP_SIM_CALL_FORWARD_STATE			DBUS_TELEPHONY_SIM_INTERFACE":cf_state"

#define DBUS_TELEPHONY_SAP_INTERFACE				DBUS_TELEPHONY_SERVICE".Sap"
#define TAPI_NOTI_SAP_STATUS						DBUS_TELEPHONY_SAP_INTERFACE":Status"
#define TAPI_NOTI_SAP_DISCONNECT					DBUS_TELEPHONY_SAP_INTERFACE":Disconnect"

#define DBUS_TELEPHONY_SAT_INTERFACE				DBUS_TELEPHONY_SERVICE".SAT"
#define TAPI_NOTI_SAT_SETUP_MENU					DBUS_TELEPHONY_SAT_INTERFACE":SetupMenu"
#define TAPI_NOTI_SAT_DISPLAY_TEXT					DBUS_TELEPHONY_SAT_INTERFACE":DisplayText"
#define TAPI_NOTI_SAT_SELECT_ITEM					DBUS_TELEPHONY_SAT_INTERFACE":SelectItem"
#define TAPI_NOTI_SAT_GET_INKEY						DBUS_TELEPHONY_SAT_INTERFACE":GetInkey"
#define TAPI_NOTI_SAT_GET_INPUT						DBUS_TELEPHONY_SAT_INTERFACE":GetInput"
#define TAPI_NOTI_SAT_REFRESH						DBUS_TELEPHONY_SAT_INTERFACE":Refresh"
#define TAPI_NOTI_SAT_SEND_SMS						DBUS_TELEPHONY_SAT_INTERFACE":SendSMS"
#define TAPI_NOTI_SAT_SETUP_EVENT_LIST				DBUS_TELEPHONY_SAT_INTERFACE":SetupEventList"
#define TAPI_NOTI_SAT_SEND_DTMF						DBUS_TELEPHONY_SAT_INTERFACE":SendDtmf"
#define TAPI_NOTI_SAT_SESSION_END_EVENT				DBUS_TELEPHONY_SAT_INTERFACE":EndProactiveSession"
#define TAPI_NOTI_SAT_CALL_CONTROL_RESULT			DBUS_TELEPHONY_SAT_INTERFACE":CallControlResult"
#define TAPI_NOTI_SAT_MO_SM_CONTROL_RESULT			DBUS_TELEPHONY_SAT_INTERFACE":MoSmControlResult"

#define DBUS_TELEPHONY_PB_INTERFACE					DBUS_TELEPHONY_SERVICE".Phonebook"
#define TAPI_NOTI_PB_STATUS							DBUS_TELEPHONY_PB_INTERFACE":Status"
#define TAPI_NOTI_PB_CONTACT_CHANGE				DBUS_TELEPHONY_PB_INTERFACE":ContactChange"

// Network
#define DBUS_TELEPHONY_NETWORK_INTERFACE			DBUS_TELEPHONY_SERVICE".Network"
#define TAPI_NOTI_NETWORK_REGISTRATION_STATUS		DBUS_TELEPHONY_NETWORK_INTERFACE":RegistrationStatus"
#define TAPI_NOTI_NETWORK_CELLINFO					DBUS_TELEPHONY_NETWORK_INTERFACE":CellInfo"
#define TAPI_NOTI_NETWORK_INFO						DBUS_TELEPHONY_NETWORK_INTERFACE":Info"
#define TAPI_NOTI_NETWORK_CHANGE					DBUS_TELEPHONY_NETWORK_INTERFACE":Change"
#define TAPI_NOTI_NETWORK_TIMEINFO					DBUS_TELEPHONY_NETWORK_INTERFACE":TimeInfo"
#define TAPI_NOTI_NETWORK_IDENTITY					DBUS_TELEPHONY_NETWORK_INTERFACE":Identity"
#define TAPI_NOTI_NETWORK_SIGNALSTRENGTH			DBUS_TELEPHONY_NETWORK_INTERFACE":SignalStrength"
#define TAPI_NOTI_NETWORK_EMERGENCY_CALLBACK_MODE	DBUS_TELEPHONY_NETWORK_INTERFACE":EmergencyCallbackMode"
#define TAPI_NOTI_NETWORK_DEFAULT_DATA_SUBSCRIPTION		DBUS_TELEPHONY_NETWORK_INTERFACE":DefaultDataSubscription"
#define TAPI_NOTI_NETWORK_DEFAULT_SUBSCRIPTION		DBUS_TELEPHONY_NETWORK_INTERFACE":DefaultSubscription"
#define TAPI_NOTI_NETWORK_PROPERTY_INFO				DBUS_TELEPHONY_NETWORK_INTERFACE":PropertyInfo"
#define TAPI_PROP_NETWORK_LAC						DBUS_TELEPHONY_NETWORK_INTERFACE":lac"
#define TAPI_PROP_NETWORK_PLMN						DBUS_TELEPHONY_NETWORK_INTERFACE":plmn"
#define TAPI_PROP_NETWORK_CELLID					DBUS_TELEPHONY_NETWORK_INTERFACE":cell_id"
#define TAPI_PROP_NETWORK_SERVICE_TYPE				DBUS_TELEPHONY_NETWORK_INTERFACE":service_type"
#define TAPI_PROP_NETWORK_ACT						DBUS_TELEPHONY_NETWORK_INTERFACE":access_technology"
#define TAPI_PROP_NETWORK_PS_TYPE					DBUS_TELEPHONY_NETWORK_INTERFACE":ps_type"
#define TAPI_PROP_NETWORK_CIRCUIT_STATUS			DBUS_TELEPHONY_NETWORK_INTERFACE":circuit_status"
#define TAPI_PROP_NETWORK_PACKET_STATUS				DBUS_TELEPHONY_NETWORK_INTERFACE":packet_status"
#define TAPI_PROP_NETWORK_ROAMING_STATUS			DBUS_TELEPHONY_NETWORK_INTERFACE":roaming_status"
#define TAPI_PROP_NETWORK_NAME_OPTION				DBUS_TELEPHONY_NETWORK_INTERFACE":name_option"
#define TAPI_PROP_NETWORK_NETWORK_NAME				DBUS_TELEPHONY_NETWORK_INTERFACE":network_name"
#define TAPI_PROP_NETWORK_SPN_NAME					DBUS_TELEPHONY_NETWORK_INTERFACE":spn_name"
#define TAPI_PROP_NETWORK_SIGNALSTRENGTH_DBM		DBUS_TELEPHONY_NETWORK_INTERFACE":sig_dbm"
#define TAPI_PROP_NETWORK_SIGNALSTRENGTH_LEVEL		DBUS_TELEPHONY_NETWORK_INTERFACE":sig_level"
#define TAPI_PROP_NETWORK_IMS_VOICE_SUPPORT_STATUS	DBUS_TELEPHONY_NETWORK_INTERFACE":ims_voice_status"

// SS
#define DBUS_TELEPHONY_SS_INTERFACE					DBUS_TELEPHONY_SERVICE".Ss"
#define TAPI_NOTI_SS_USSD							DBUS_TELEPHONY_SS_INTERFACE":NotifyUSSD"
#define TAPI_NOTI_SS_RELEASE_COMPLETE				DBUS_TELEPHONY_SS_INTERFACE":ReleaseComplete"
#define TAPI_NOTI_SS_FORWARD_STATUS					DBUS_TELEPHONY_SS_INTERFACE":NotifyForwarding"
#define TAPI_NOTI_SS_BARRING_STATUS					DBUS_TELEPHONY_SS_INTERFACE":NotifyBarring"
#define TAPI_NOTI_SS_WAITING_STATUS					DBUS_TELEPHONY_SS_INTERFACE":NotifyWaiting"
#define TAPI_NOTI_SS_INFO								DBUS_TELEPHONY_SS_INTERFACE":NotifySsInfo"

// SMS
#define DBUS_TELEPHONY_SMS_INTERFACE	DBUS_TELEPHONY_SERVICE".sms"

/**
 * @brief Definition for the unsolicited incoming SMS Event.
 */
#define TAPI_NOTI_SMS_INCOM_MSG			DBUS_TELEPHONY_SMS_INTERFACE":IncommingMsg"

/**
 * @brief Definition for the unsolicited incoming cell broadcast message.
 */
#define TAPI_NOTI_SMS_CB_INCOM_MSG		DBUS_TELEPHONY_SMS_INTERFACE":IncommingCbMsg"

/**
 * @brief Definition for the unsolicited incoming cell broadcast message.
 */
#define TAPI_NOTI_SMS_ETWS_INCOM_MSG	DBUS_TELEPHONY_SMS_INTERFACE":IncommingEtwsMsg"

/**
 * @brief Definition for the unsolicited event indicating SIM memory status.
 */
#define TAPI_NOTI_SMS_MEMORY_STATUS		DBUS_TELEPHONY_SMS_INTERFACE":MemoryStatus"

/**
 * @brief Definition for the device ready indication.
 */
#define TAPI_NOTI_SMS_DEVICE_READY		DBUS_TELEPHONY_SMS_INTERFACE":SmsReady"

// Gps
#define DBUS_TELEPHONY_GPS_INTERFACE		DBUS_TELEPHONY_SERVICE".Gps"
#define TAPI_NOTI_GPS_ASSIST_DATA			DBUS_TELEPHONY_GPS_INTERFACE":AssistData"
#define TAPI_NOTI_GPS_RESET_ASSIST_DATA		DBUS_TELEPHONY_GPS_INTERFACE":ResetAssistData"
#define TAPI_NOTI_SMART_ASSISTANT_AREA_STATUS	DBUS_TELEPHONY_GPS_INTERFACE":AreaStatus"
#define TAPI_NOTI_SMART_ASSISTANT_SYNC_STATUS	DBUS_TELEPHONY_GPS_INTERFACE":SyncStatus"

#define DBUS_TELEPHONY_OEM_INTERFACE DBUS_TELEPHONY_SERVICE".OEM"
#define TAPI_NOTI_OEM_DATA DBUS_TELEPHONY_OEM_INTERFACE":OemData"

/**
 * @}
 */

__END_DECLS

#endif /* __TAPI_EVENT_H__ */
