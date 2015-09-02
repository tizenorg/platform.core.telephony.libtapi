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
 * @brief Definition for the Call interface
 */
#define DBUS_TELEPHONY_CALL_INTERFACE	DBUS_TELEPHONY_SERVICE".Call"

/**
 * @brief Definition for voice call idle status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusIdleNoti_t' will be stored
 */
#define TAPI_NOTI_VOICE_CALL_STATUS_IDLE	DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusIdle"

/**
 * @brief Definition for voice call active status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusActiveNoti_t' will be stored
 */
#define TAPI_NOTI_VOICE_CALL_STATUS_ACTIVE	DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusActive"

/**
 * @brief Definition for voice call held status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusHeldNoti_t' will be stored
 */
#define TAPI_NOTI_VOICE_CALL_STATUS_HELD	DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusHeld"

/**
 * @brief Definition for voice call dialing status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusDialingNoti_t' will be stored
 */
#define TAPI_NOTI_VOICE_CALL_STATUS_DIALING	DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusDialing"

/**
 * @brief Definition for voice call alerting status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusAlertNoti_t' will be stored
 */
#define TAPI_NOTI_VOICE_CALL_STATUS_ALERT	DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusAlert"

/**
 * @brief Definition for voice call incoming status notification
 * @since_tizen 2.3
 * @remarks '#TelCallIncomingCallInfo_t' will be stored
 */
#define TAPI_NOTI_VOICE_CALL_STATUS_INCOMING	DBUS_TELEPHONY_CALL_INTERFACE":VoiceCallStatusIncoming"

/**
 * @brief Definition for video call idle status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusIdleNoti_t' will be stored
 */
#define TAPI_NOTI_VIDEO_CALL_STATUS_IDLE	DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusIdle"

/**
 * @brief Definition for video call active status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusActiveNoti_t' will be stored
 */
#define TAPI_NOTI_VIDEO_CALL_STATUS_ACTIVE	DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusActive"

/**
 * @brief Definition for video call dialing status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusDialingNoti_t' will be stored
 */
#define TAPI_NOTI_VIDEO_CALL_STATUS_DIALING	DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusDialing"

/**
 * @brief Definition for video call alerting status notification
 * @since_tizen 2.3
 * @remarks '#TelCallStatusAlertNoti_t' will be stored
 */
#define TAPI_NOTI_VIDEO_CALL_STATUS_ALERT	DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusAlert"

/**
 * @brief Definition for video call incoming status notification
 * @since_tizen 2.3
 * @remarks '#TelCallIncomingCallInfo_t' will be stored
 */
#define TAPI_NOTI_VIDEO_CALL_STATUS_INCOMING	DBUS_TELEPHONY_CALL_INTERFACE":VideoCallStatusIncoming"

/**
 * @brief Definition for outgoing call waiting notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoWaitingNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_WAITING	DBUS_TELEPHONY_CALL_INTERFACE":Waiting"

/**
 * @brief Definition for outgoing call forwarded notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoForwardedNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_FORWARDED	DBUS_TELEPHONY_CALL_INTERFACE":Forwarded"

/**
 * @brief Definition for incoming call barred notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoBarredIncomingNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_BARRED_INCOMING	DBUS_TELEPHONY_CALL_INTERFACE":BarredIncoming"

/**
 * @brief Definition for outgoing call barred notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoBarredOutgoingNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_BARRED_OUTGOING	DBUS_TELEPHONY_CALL_INTERFACE":BarredOutgoing"

/**
 * @brief Definition for MO call deflected notification
 * @since_tizen 2.3
 * @remarks No data stored
 */
#define TAPI_NOTI_CALL_INFO_DEFLECTED	DBUS_TELEPHONY_CALL_INTERFACE":Deflected"

/**
 * @brief Definition for CLIR suppression reject notification
 * @since_tizen 2.3
 * @remarks No data stored
 */
#define TAPI_NOTI_CALL_INFO_CLIR_SUPPRESSION_REJECT	DBUS_TELEPHONY_CALL_INTERFACE":ClirSuppressionReject"

/**
 * @brief Definition for unconditional call forward active notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoForwardUnconditionalNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_FORWARD_UNCONDITIONAL	DBUS_TELEPHONY_CALL_INTERFACE":ForwardUnconditional"

/**
 * @brief Definition for conditional call forward active notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoForwardConditionalNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_FORWARD_CONDITIONAL		DBUS_TELEPHONY_CALL_INTERFACE":ForwardConditional"

/**
 * @brief Definition for incoming call forwarded notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoForwardedCallNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_FORWARDED_CALL	DBUS_TELEPHONY_CALL_INTERFACE":ForwardedCall"

/**
 * @brief Definition for MT deflected call notification
 * @since_tizen 2.3
 * @remarks No data stored
 */
#define TAPI_NOTI_CALL_INFO_DEFLECTED_CALL	DBUS_TELEPHONY_CALL_INTERFACE":DeflectedCall"

/**
 * @brief Definition for MT transfered call notification
 * @since_tizen 2.3
 * @remarks No data stored
 */
#define TAPI_NOTI_CALL_INFO_TRANSFERED_CALL	DBUS_TELEPHONY_CALL_INTERFACE":TransferedCall"

/**
 * @brief Definition for call is in held notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoHeldNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_HELD	DBUS_TELEPHONY_CALL_INTERFACE":CallHeld"

/**
 * @brief Definition for call is in retrieved notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoActiveNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_ACTIVE	DBUS_TELEPHONY_CALL_INTERFACE":CallActive"

/**
 * @brief Definition for call is in multiparty notification
 * @since_tizen 2.3
 * @remarks '#TelCallInfoJoinedNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_JOINED	DBUS_TELEPHONY_CALL_INTERFACE":CallJoined"

/**
 * @brief Definition for call transfer alerting notification
 * @since_tizen 2.3
 * @remarks No data stored
 */
#define TAPI_NOTI_CALL_INFO_TRANSFER_ALERT	DBUS_TELEPHONY_CALL_INTERFACE":TransferAlert"

/**
 * @brief Definition for call forward check message notification
 * @since_tizen 2.3
 * @remarks No data stored
 */
#define TAPI_NOTI_CALL_INFO_CF_CHECK_MESSAGE	DBUS_TELEPHONY_CALL_INTERFACE":CfCheckMessage"

/**
 * @brief Definition for new call information notification (CDMA only)
 * @since_tizen 2.3
 * @remarks '#TelCallRecordInfoNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_INFO_REC	DBUS_TELEPHONY_CALL_INTERFACE":CallInfoRec"

/**
 * @brief Definition for call info fallback notification
 * @since_tizen TBD
 * @remarks TBD
 */
#define TAPI_NOTI_CALL_INFO_FALLBACK	DBUS_TELEPHONY_CALL_INTERFACE:":CallFallback"

/**
 * @brief Definition for voice privacy mode change notification (CDMA only)
 * @since_tizen 2.3
 * @remarks '#TelCallVoicePrivacyNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_PRIVACY_MODE	DBUS_TELEPHONY_CALL_INTERFACE":CallPrivacyMode"

/**
 * @brief Definition for OTASP(Over The Air Service Provisioning) status notification (CDMA only)
 * @since_tizen 2.3
 * @remarks '#TelCallOtaspStatusNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_OTASP_STATUS	DBUS_TELEPHONY_CALL_INTERFACE":CallOtaspStatus"

/**
 * @brief Definition for OTAPA(Over The Air Parameter Administration) status notification (CDMA only)
 * @since_tizen 2.3
 * @remarks '#TelCallOtapaStatusNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_OTAPA_STATUS	DBUS_TELEPHONY_CALL_INTERFACE":CallOtapaStatus"

/**
 * @brief Definition for call signal information notification (CDMA only)
 * @since_tizen 2.3
 * @remarks '#TelCallSignalInfoNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_SIGNAL_INFO	DBUS_TELEPHONY_CALL_INTERFACE":CallSignalInfo"

/**
 * @brief Definition for call sound patch change notification
 * @since_tizen 2.3
 * @remarks '#TelCallSoundPathNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_SOUND_PATH	DBUS_TELEPHONY_CALL_INTERFACE":CallSoundPath"

/**
 * @brief Definition for call ringback tone sound patch change notification
 * @since_tizen 2.3
 * @remarks '#TelCallSoundRingbackToneNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_SOUND_RINGBACK_TONE	DBUS_TELEPHONY_CALL_INTERFACE":CallSoundRingbackTone"

/**
 * @brief Definition for call sound WB-AMR(Wide Band Adaptive Multi-Rate) status notification
 * @since_tizen 2.3
 * @remarks '#TelCallSoundWbamrNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_SOUND_WBAMR	DBUS_TELEPHONY_CALL_INTERFACE":CallSoundWbamr"

/**
 * @brief Definition for call sound noise redunction notification
 * @since_tizen 2.3
 * @remarks '#TelCallSoundNoiseReductionNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_SOUND_NOISE_REDUCTION	DBUS_TELEPHONY_CALL_INTERFACE":CallSoundNoiseReduction"

/**
 * @brief Definition for call sound clock status notification
 * @since_tizen 2.3
 * @remarks 'gboolean' will be stored
 */
#define TAPI_NOTI_CALL_SOUND_CLOCK_STATUS	DBUS_TELEPHONY_CALL_INTERFACE":CallSoundClockStatus"

/**
 * @brief Definition for preferred voice subscription notification
 * @since_tizen 2.3
 * @remarks '#TelCallPreferredVoiceSubsNoti_t' will be stored
 */
#define TAPI_NOTI_CALL_PREFERRED_VOICE_SUBSCRIPTION	DBUS_TELEPHONY_CALL_INTERFACE":CallPreferredVoiceSubscription"

/**
 * @brief Definition for the Modem interface
 */
#define DBUS_TELEPHONY_MODEM_INTERFACE	DBUS_TELEPHONY_SERVICE".Modem"

/**
 * @brief Definition for modem power notification
 * @since_tizen 2.3
 * @remarks '#tapi_power_phone_power_status_t' will be stored
 */
#define TAPI_NOTI_MODEM_POWER	DBUS_TELEPHONY_MODEM_INTERFACE":Power"

/**
 * @brief Definition for modem power property
 * @since_tizen 2.3
 * @remarks '#tapi_power_phone_power_status_t' will be stored
 */
#define TAPI_PROP_MODEM_POWER	DBUS_TELEPHONY_MODEM_INTERFACE":power"

/**
 * @brief Definition for dongle status property
 * @since_tizen 2.4
 * @remarks 'gboolean' will be stored (Inserted or not)
 */
#define TAPI_PROP_MODEM_DONGLE_STATUS	DBUS_TELEPHONY_MODEM_INTERFACE":dongle_status"

/**
 * @brief Definition for dongle login property
 * @since_tizen 2.4
 * @remarks 'gboolean' will be stored (Login success or not)
 */
#define TAPI_PROP_MODEM_DONGLE_LOGIN	DBUS_TELEPHONY_MODEM_INTERFACE":dongle_login"

/**
 * @brief Definition for the Sim interface
 */
#define DBUS_TELEPHONY_SIM_INTERFACE	DBUS_TELEPHONY_SERVICE".Sim"

/**
 * @brief Definition for SIM status notification
 * @since_tizen 2.3
 * @remarks '#TelSimCardStatus_t' will be stored
 */
#define TAPI_NOTI_SIM_STATUS	DBUS_TELEPHONY_SIM_INTERFACE":Status"

/**
 * TODO
 */
#define TAPI_NOTI_SIM_REFRESHED	DBUS_TELEPHONY_SIM_INTERFACE":Refreshed"

/**
 * @brief Definition for SIM call forward state property
 * @since_tizen 2.3
 * @remarks 'gboolean' will be stored
 */
#define TAPI_PROP_SIM_CALL_FORWARD_STATE	DBUS_TELEPHONY_SIM_INTERFACE":cf_state"

/**
 * @brief Definition for the Sap interface
 */
#define DBUS_TELEPHONY_SAP_INTERFACE	DBUS_TELEPHONY_SERVICE".Sap"

/**
 * @brief Definition for Sap card status notification
 * @since_tizen 2.3
 * @remarks '#TelSimSapCardStatus_t' will be stored
 */
#define TAPI_NOTI_SAP_STATUS	DBUS_TELEPHONY_SAP_INTERFACE":Status"

/**
 * @brief Definition for Sap disconnect type notification
 * @since_tizen 2.3
 * @remarks '#TelSimSapDissconnectType_t' will be stored
 */
#define TAPI_NOTI_SAP_DISCONNECT	DBUS_TELEPHONY_SAP_INTERFACE":Disconnect"

/**
 * @brief Definition for the SAT interface
 */
#define DBUS_TELEPHONY_SAT_INTERFACE	DBUS_TELEPHONY_SERVICE".SAT"

/**
 * @brief Definition for sat setup menu notification
 * @since_tizen 2.3
 * @remarks '#TelSatSetupMenuInfo_t' will be stored
 */
#define TAPI_NOTI_SAT_SETUP_MENU	DBUS_TELEPHONY_SAT_INTERFACE":SetupMenu"

/**
 * @brief Definition for sat display text notification
 * @since_tizen 2.3
 * @remarks '#TelSatDisplayTextInd_t' will be stored
 */
#define TAPI_NOTI_SAT_DISPLAY_TEXT	DBUS_TELEPHONY_SAT_INTERFACE":DisplayText"

/**
 * @brief Definition for sat select item notification
 * @since_tizen 2.3
 * @remarks '#TelSatSelectItemInd_t' will be stored
 */
#define TAPI_NOTI_SAT_SELECT_ITEM	DBUS_TELEPHONY_SAT_INTERFACE":SelectItem"

/**
 * @brief Definition for sat get inkey notification
 * @since_tizen 2.3
 * @remarks '#TelSatGetInkeyInd_t' will be stored
 */
#define TAPI_NOTI_SAT_GET_INKEY	DBUS_TELEPHONY_SAT_INTERFACE":GetInkey"

/**
 * @brief Definition for sat get input notification
 * @since_tizen 2.3
 * @remarks '#TelSatGetInputInd_t' will be stored
 */
#define TAPI_NOTI_SAT_GET_INPUT	DBUS_TELEPHONY_SAT_INTERFACE":GetInput"

/**
 * @brief Definition for sat refresh notification
 * @since_tizen 2.3
 * @remarks '#TelSatRefreshInd_t' will be stored
 */
#define TAPI_NOTI_SAT_REFRESH	DBUS_TELEPHONY_SAT_INTERFACE":Refresh"

/**
 * @brief Definition for sat send sms notification
 * @since_tizen 2.3
 * @remarks '#TelSatSendSmsIndSmsData_t' will be stored
 */
#define TAPI_NOTI_SAT_SEND_SMS	DBUS_TELEPHONY_SAT_INTERFACE":SendSMS"

/**
 * @brief Definition for sat setup event list notification
 * @since_tizen 2.3
 * @remarks '#TelSatEventListData_t' will be stored
 */
#define TAPI_NOTI_SAT_SETUP_EVENT_LIST	DBUS_TELEPHONY_SAT_INTERFACE":SetupEventList"

/**
 * @brief Definition for sat send dtmf notification
 * @since_tizen 2.3
 * @remarks '#TelSatSendDtmfIndDtmfData_t' will be stored
 */
#define TAPI_NOTI_SAT_SEND_DTMF	DBUS_TELEPHONY_SAT_INTERFACE":SendDtmf"

/**
 * @brief Definition for sat end proactive session notification
 * @since_tizen 2.3
 * @remarks '#TelSatCommandType_t' will be stored
 */
#define TAPI_NOTI_SAT_SESSION_END_EVENT	DBUS_TELEPHONY_SAT_INTERFACE":EndProactiveSession"

/**
 * @brief Definition for sat call control result notification
 * @since_tizen 2.3
 * @remarks '#TelSatCallCtrlIndData_t' will be stored
 */
#define TAPI_NOTI_SAT_CALL_CONTROL_RESULT	DBUS_TELEPHONY_SAT_INTERFACE":CallControlResult"

/**
 * @brief Definition for sat mo sms control result notification
 * @since_tizen 2.3
 * @remarks '#TelSatMoSmCtrlIndData_t' will be stored
 */
#define TAPI_NOTI_SAT_MO_SM_CONTROL_RESULT	DBUS_TELEPHONY_SAT_INTERFACE":MoSmControlResult"

/**
 * @brief Definition for sat setup call notification
 * @since_tizen 2.4
 * @remarks '#TelSatSetupCallIndCallData_t' will be stored
 */
#define TAPI_NOTI_SAT_SETUP_CALL	DBUS_TELEPHONY_SAT_INTERFACE":SetupCall"

/**
 * @brief Definition for the Phonebook interface
 */
#define DBUS_TELEPHONY_PB_INTERFACE	DBUS_TELEPHONY_SERVICE".Phonebook"

/**
 * @brief Definition for phonebook status notification
 * @since_tizen 2.3
 * @remarks '#TelSimPbStatus_t' will be stored
 */
#define TAPI_NOTI_PB_STATUS	DBUS_TELEPHONY_PB_INTERFACE":Status"

/**
 * @brief Definition for phonebook change notification
 * @since_tizen 2.3
 * @remarks '#TelSimPbContactChangeInfo_t' will be stored
 */
#define TAPI_NOTI_PB_CONTACT_CHANGE	DBUS_TELEPHONY_PB_INTERFACE":ContactChange"

/**
 * @brief Definition for the Network interface
 */
#define DBUS_TELEPHONY_NETWORK_INTERFACE	DBUS_TELEPHONY_SERVICE".Network"

/**
 * @brief Definition for network registration status notification
 * @since_tizen 2.3
 * @remarks '#TelNetworkRegistrationStatus_t' will be stored
 */
#define TAPI_NOTI_NETWORK_REGISTRATION_STATUS	DBUS_TELEPHONY_NETWORK_INTERFACE":RegistrationStatus"

/**
 * @brief Definition for network cell information notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_cell_info' will be stored
 */
#define TAPI_NOTI_NETWORK_CELLINFO	DBUS_TELEPHONY_NETWORK_INTERFACE":CellInfo"

/**
 * @brief Definition for network change notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_change' will be stored
 */
#define TAPI_NOTI_NETWORK_CHANGE	DBUS_TELEPHONY_NETWORK_INTERFACE":Change"

/**
 * @brief Definition for network time information notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_time_info' will be stored
 */
#define TAPI_NOTI_NETWORK_TIMEINFO	DBUS_TELEPHONY_NETWORK_INTERFACE":TimeInfo"

/**
 * @brief Definition for network identity notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_identity' will be stored
 */
#define TAPI_NOTI_NETWORK_IDENTITY	DBUS_TELEPHONY_NETWORK_INTERFACE":Identity"

/**
 * @brief Definition for network signal strength notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_signal_strength' will be stored
 */
#define TAPI_NOTI_NETWORK_SIGNALSTRENGTH	DBUS_TELEPHONY_NETWORK_INTERFACE":SignalStrength"

/**
 * @brief Definition for network emergency callback mode notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_emergency_callback_mode' will be stored
 */
#define TAPI_NOTI_NETWORK_EMERGENCY_CALLBACK_MODE	DBUS_TELEPHONY_NETWORK_INTERFACE":EmergencyCallbackMode"

/**
 * @brief Definition for network default data subscription notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_default_data_subs' will be stored
 */
#define TAPI_NOTI_NETWORK_DEFAULT_DATA_SUBSCRIPTION	DBUS_TELEPHONY_NETWORK_INTERFACE":DefaultDataSubscription"

/**
 * @brief Definition for network default subscription notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_network_default_subs' will be stored
 */
#define TAPI_NOTI_NETWORK_DEFAULT_SUBSCRIPTION	DBUS_TELEPHONY_NETWORK_INTERFACE":DefaultSubscription"

/**
 * @brief Definition for network LAC property (In case of LTE network, it represents Tracking Area Code)
 * @since_tizen 2.3
 * @remarks 'unsigned int' will be stored
 */
#define TAPI_PROP_NETWORK_LAC	DBUS_TELEPHONY_NETWORK_INTERFACE":lac"

/**
 * @brief Definition for network PLMN property
 * @since_tizen 2.3
 * @remarks 'char *' will be stored
 */
#define TAPI_PROP_NETWORK_PLMN	DBUS_TELEPHONY_NETWORK_INTERFACE":plmn"

/**
 * @brief Definition for network cell-id property
 * @since_tizen 2.3
 * @remarks 'unsigned int' will be stored
 */
#define TAPI_PROP_NETWORK_CELLID	DBUS_TELEPHONY_NETWORK_INTERFACE":cell_id"

/**
 * @brief Definition for network service type property
 * @since_tizen 2.3
 * @remarks '#TelNetworkServiceType_t' will be stored
 */
#define TAPI_PROP_NETWORK_SERVICE_TYPE	DBUS_TELEPHONY_NETWORK_INTERFACE":service_type"

/**
 * @brief Definition for network access technology property
 * @since_tizen 2.3
 * @remarks '#TelNetworkSystemType_t' will be stored
 */
#define TAPI_PROP_NETWORK_ACT	DBUS_TELEPHONY_NETWORK_INTERFACE":access_technology"

/**
 * @brief Definition for network ps type property
 * @since_tizen 2.3
 * @remarks '#TelNetworkPsType_t' will be stored
 */
#define TAPI_PROP_NETWORK_PS_TYPE	DBUS_TELEPHONY_NETWORK_INTERFACE":ps_type"

/**
 * @brief Definition for network circuit status property
 * @since_tizen 2.3
 * @remarks '#TelNetworkServiceLevel_t' will be stored
 */
#define TAPI_PROP_NETWORK_CIRCUIT_STATUS	DBUS_TELEPHONY_NETWORK_INTERFACE":circuit_status"

/**
 * @brief Definition for network packet status property
 * @since_tizen 2.3
 * @remarks '#TelNetworkServiceLevel_t' will be stored
 */
#define TAPI_PROP_NETWORK_PACKET_STATUS	DBUS_TELEPHONY_NETWORK_INTERFACE":packet_status"

/**
 * @brief Definition for network roaming status property
 * @since_tizen 2.3
 * @remarks 'gboolean' will be stored
 */
#define TAPI_PROP_NETWORK_ROAMING_STATUS	DBUS_TELEPHONY_NETWORK_INTERFACE":roaming_status"

/**
 * @brief Definition for network name option property
 * @since_tizen 2.3
 * @remarks '#TelNetworkNameDispCondition_t' will be stored
 */
#define TAPI_PROP_NETWORK_NAME_OPTION	DBUS_TELEPHONY_NETWORK_INTERFACE":name_option"

/**
 * @brief Definition for network name property
 * @since_tizen 2.3
 * @remarks 'char *' will be stored
 */
#define TAPI_PROP_NETWORK_NETWORK_NAME	DBUS_TELEPHONY_NETWORK_INTERFACE":network_name"

/**
 * @brief Definition for SPN name property stored in SIM card
 * @since_tizen 2.3
 * @remarks 'char *' will be stored
 */
#define TAPI_PROP_NETWORK_SPN_NAME	DBUS_TELEPHONY_NETWORK_INTERFACE":spn_name"

/**
 * @brief Definition for network signal dbm property
 * @since_tizen 2.3
 * @remarks 'int' will be stored
 */
#define TAPI_PROP_NETWORK_SIGNALSTRENGTH_DBM	DBUS_TELEPHONY_NETWORK_INTERFACE":sig_dbm"

/**
 * @brief Definition for network signal level property
 * @since_tizen 2.3
 * @remarks 'int' will be stored
 */
#define TAPI_PROP_NETWORK_SIGNALSTRENGTH_LEVEL	DBUS_TELEPHONY_NETWORK_INTERFACE":sig_level"

/**
 * @brief Definition for network ims voice support status notification
 * @since_tizen TBD
 * @remarks TBD
 */
#define TAPI_PROP_NETWORK_IMS_VOICE_SUPPORT_STATUS	DBUS_TELEPHONY_NETWORK_INTERFACE":ims_voice_status"

/**
 * @brief Definition for the Ss interface
 */
#define DBUS_TELEPHONY_SS_INTERFACE	DBUS_TELEPHONY_SERVICE".Ss"

/**
 * @brief Definition for ss ussd receive notification
 * @since_tizen 2.3
 * @remarks '#TelSsUssdMsgInfo_t' will be stored
 */
#define TAPI_NOTI_SS_USSD	DBUS_TELEPHONY_SS_INTERFACE":NotifyUSSD"

/**
 * @brief Definition for ss release complete notification
 * @since_tizen 2.3
 * @remarks '#TelSsRelCompMsgInfo_t' will be stored
 */
#define TAPI_NOTI_SS_RELEASE_COMPLETE	DBUS_TELEPHONY_SS_INTERFACE":ReleaseComplete"

/**
 * @brief Definition for ss call forwarding status notification
 * @since_tizen 2.3
 * @remarks '#TelSsForwardNoti_t' will be stored
 */
#define TAPI_NOTI_SS_FORWARD_STATUS	DBUS_TELEPHONY_SS_INTERFACE":NotifyForwarding"

/**
 * @brief Definition for ss call barring status notification
 * @since_tizen 2.3
 * @remarks '#TelSsBarringNoti_t' will be stored
 */
#define TAPI_NOTI_SS_BARRING_STATUS	DBUS_TELEPHONY_SS_INTERFACE":NotifyBarring"

/**
 * @brief Definition for ss call waiting status notification
 * @since_tizen 2.3
 * @remarks '#TelSsWaitingNoti_t' will be stored
 */
#define TAPI_NOTI_SS_WAITING_STATUS	DBUS_TELEPHONY_SS_INTERFACE":NotifyWaiting"

/**
 * @brief Definition for ss information notification
 * @since_tizen 2.3
 * @remarks '#TelSsInfo_t' will be stored
 */
#define TAPI_NOTI_SS_INFO	DBUS_TELEPHONY_SS_INTERFACE":NotifySsInfo"

/**
 * @brief Definition for the sms interface
 */
#define DBUS_TELEPHONY_SMS_INTERFACE	DBUS_TELEPHONY_SERVICE".sms"

/**
 * @brief Definition for sms incoming message notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_sms_incomming_msg' will be stored
 */
#define TAPI_NOTI_SMS_INCOM_MSG	DBUS_TELEPHONY_SMS_INTERFACE":IncommingMsg"

/**
 * @brief Definition for sms cell broadcast message incoming notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_sms_incomming_cb_msg' will be stored
 */
#define TAPI_NOTI_SMS_CB_INCOM_MSG	DBUS_TELEPHONY_SMS_INTERFACE":IncommingCbMsg"

/**
 * @brief Definition for sms ETWS(Earthquake and Tsunami Warning System) message incoming notification
 * @since_tizen 2.3
 * @remarks '#tel_noti_sms_incomming_cb_msg' will be stored
 */
#define TAPI_NOTI_SMS_ETWS_INCOM_MSG	DBUS_TELEPHONY_SMS_INTERFACE":IncommingEtwsMsg"

/**
 * @brief Definition for sms device memory status notification
 * @since_tizen 2.3
 * @remarks '#TelSmsMemStatusType' will be stored
 */
#define TAPI_NOTI_SMS_MEMORY_STATUS	DBUS_TELEPHONY_SMS_INTERFACE":MemoryStatus"

/**
 * @brief Definition for sms ready notification
 * @since_tizen 2.3
 * @remarks '#TelSmsReadyStatus_t' will be stored
 */
#define TAPI_NOTI_SMS_DEVICE_READY	DBUS_TELEPHONY_SMS_INTERFACE":SmsReady"

/**
 * @brief Definition for the OEM interface
 */
#define DBUS_TELEPHONY_OEM_INTERFACE DBUS_TELEPHONY_SERVICE".OEM"

/**
 * @brief Definition for oem data notification
 * @since_tizen 2.4
 * @remarks '#TelNotiOemData_t' will be stored
 */
#define TAPI_NOTI_OEM_DATA DBUS_TELEPHONY_OEM_INTERFACE":OemData"

/**
 * @}
 */

__END_DECLS

#endif /* __TAPI_EVENT_H__ */
