/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
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
 * @addtogroup	COMMON_TAPI	EVENT
 * @{
 *
 * @file TapiEvent.h
 * @brief  This file serves as a "C" header file defines type of events. \n
 */

#ifndef _TAPI_EVENT_H_
#define _TAPI_EVENT_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 CONSTANTS
 ==================================================================================================*/
/**
 *   This value defines the Telephony Event Class. Class is nothing but a group of events
 *   related to a particular subsystem. Example of one Event Class is TAPI_EVENT_CLASS_CALL_VOICE.
 *   This becomes the first parameter of telephony notifications.
 */

#define TAPI_EVENT_NAME_MAX_LEN			128	/**< Maximum length of theevent name */
#define TAPI_REQUEST_NOTI_ID			0xff	/**< Request id for notification message */
#define TAPI_REQUEST_INVALID_ID			-1		/**< Invalid request id */

#define TAPI_EVENT_TYPE_CONFIRMATION	(0 << 16)
#define TAPI_EVENT_TYPE_NOTIFICATION	(1 << 16)
#define TAPI_EVENT_TYPE_RESERVED		(2 << 16)
#define TAPI_EVENT_TYPE_MAX				(3 << 16)

#define TAPI_EVENT_NONE					(-1)							/**< Event None */
#define TAPI_EVENT_CLASS_BASE			(0)								/**< Event None */
#define TAPI_EVENT_CLASS_CALL_VOICE		(TAPI_EVENT_CLASS_BASE + 0)		/**<	Voice Call Event Class	*/
#define TAPI_EVENT_CLASS_CALL_DATA		(TAPI_EVENT_CLASS_BASE + 1)		/**<	Data Call Event Class	*/
#define TAPI_EVENT_CLASS_SIM			(TAPI_EVENT_CLASS_BASE + 2)		/**<	SIM Event  Class	*/
#define TAPI_EVENT_CLASS_NETTEXT		(TAPI_EVENT_CLASS_BASE + 3)		/**<	Nettext (SMS/CB) Event Class	*/
#define TAPI_EVENT_CLASS_NETWORK		(TAPI_EVENT_CLASS_BASE + 4)		/**<	Network Event  Class	*/
#define TAPI_EVENT_CLASS_SS				(TAPI_EVENT_CLASS_BASE + 5)		/**<	SS Event  Class	*/
#define TAPI_EVENT_CLASS_SAT			(TAPI_EVENT_CLASS_BASE + 6)		/**<	SAT Events  Class	*/
#define TAPI_EVENT_CLASS_DATA			(TAPI_EVENT_CLASS_BASE + 7)		/**<	GPRS DATA Events  Class	*/
#define TAPI_EVENT_CLASS_SOUND			(TAPI_EVENT_CLASS_BASE + 8)		/**<	SOUND Events  Class	*/
#define TAPI_EVENT_CLASS_UTIL			(TAPI_EVENT_CLASS_BASE + 9)		/**<	COMMON Events  Class	*/
#define TAPI_EVENT_CLASS_EXTENSION		(TAPI_EVENT_CLASS_BASE + 0x1F)	/**<	Extension Event */
#define TAPI_EVENT_CLASS_MAX			(TAPI_EVENT_CLASS_BASE + 0x3F)	/**<	MAX event Class	*/

#define	TAPI_EVENT_CLASS_ERR			(TAPI_EVENT_CLASS_EXTENSION)		/**<	Fatal error Event  Class	*/
#define	TAPI_EVENT_CLASS_POWER			(TAPI_EVENT_CLASS_EXTENSION + 1)	/**<	POWEREvents  Class	*/
#define	TAPI_EVENT_CLASS_DISPLAY		(TAPI_EVENT_CLASS_EXTENSION + 2)	/**<	DISPLAY Events  Class	*/
#define	TAPI_EVENT_CLASS_MISC			(TAPI_EVENT_CLASS_EXTENSION + 3)	/**<	MISC Events  Class	*/
#define	TAPI_EVENT_CLASS_CFG			(TAPI_EVENT_CLASS_EXTENSION + 4)	/**<	CFG Events  Class	*/
#define	TAPI_EVENT_CLASS_IMEI			(TAPI_EVENT_CLASS_EXTENSION + 5)	/**<	MISC Events  Class	*/
#define	TAPI_EVENT_CLASS_GPS			(TAPI_EVENT_CLASS_EXTENSION + 6)	/**<GPS Events  Class	*/
#define	TAPI_EVENT_CLASS_OMADM			(TAPI_EVENT_CLASS_EXTENSION + 7)	/**<    OMADM Events  Class */
#define TAPI_EVENT_CLASS_SVCMODE		(TAPI_EVENT_CLASS_EXTENSION + 8)
#define TAPI_EVENT_CLASS_FACTORY		(TAPI_EVENT_CLASS_EXTENSION + 9)
#define TAPI_EVENT_CLASS_CALL_E911		(TAPI_EVENT_CLASS_EXTENSION + 10)	// check why this is needed.
#define	TAPI_EVENT_CLASS_CDMA_DATA		(TAPI_EVENT_CLASS_EXTENSION + 11)	/**<    CDMA Data Events  Class */

/*	Events releated to Voice Call */
#define TAPI_EVENT_CALL_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_CALL_VOICE << 8))
#define TAPI_EVENT_CALL_SETUP_CNF				(TAPI_EVENT_CALL_CONFIRMATION + 0)		/**<	This event indicates the Call Setup request triggered */
#define TAPI_EVENT_CALL_ANSWER_CNF				(TAPI_EVENT_CALL_CONFIRMATION + 1)		/**<	Incoming call was answered*/
#define TAPI_EVENT_CALL_RETRIEVE_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 2)		/**<	This event indicates Call is retrieved from held state*/
#define TAPI_EVENT_CALL_HOLD_CNF				(TAPI_EVENT_CALL_CONFIRMATION + 3)		/**<	This event indicates is call is put on hold*/
#define TAPI_EVENT_CALL_TRANSFER_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 4)		/**<	This event indicates is call is explicitly transferred*/
#define TAPI_EVENT_CALL_SETUPCONFERENCE_CNF		(TAPI_EVENT_CALL_CONFIRMATION + 5)		/**<	This event indicates that Call is joined in a conference*/
#define TAPI_EVENT_CALL_SPLITCONFERENCE_CNF		(TAPI_EVENT_CALL_CONFIRMATION + 6)		/**<	This event indicates Call is split from conference*/
#define TAPI_EVENT_CALL_SEND_DTMF_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 7)		/**<	This event indicates the confirmation for sending DTMF Digits to the network*/
#define TAPI_EVENT_CALL_RELEASE_CNF				(TAPI_EVENT_CALL_CONFIRMATION + 8)		/**<	This event is indicated when a specific call is released requested by the application*/
#define TAPI_EVENT_CALL_RELEASE_ALL_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 9)		/**<	This event is indicated when all the calls released requested by the application*/
#define TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF	(TAPI_EVENT_CALL_CONFIRMATION + 10)		/**<	This event is indicated when all the active calls released requested by the application*/
#define TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF	(TAPI_EVENT_CALL_CONFIRMATION + 11)		/**<	This event is indicated when all the held calls released requested by the application*/
#define TAPI_EVENT_CALL_GET_LINEID_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 12)		/**<	Error in Get Line ID	*/
#define TAPI_EVENT_CALL_SET_LINEID_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 13)		/**<	Error in Set Line Id		*/
#define TAPI_EVENT_CALL_SWAP_CNF				(TAPI_EVENT_CALL_CONFIRMATION + 14)		/**<	Call Swap Confirmation */
#define TAPI_EVENT_CALL_TIME_CNF				(TAPI_EVENT_CALL_CONFIRMATION + 15)		/**<	call time cnf */
#define TAPI_EVENT_CALL_FLASHINFO_CNF			(TAPI_EVENT_CALL_CONFIRMATION + 16)		/**<	Flash info	*/
#define TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF		(TAPI_EVENT_CALL_CONFIRMATION + 17)		/**<	Get Privacy mode	*/
#define TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF		(TAPI_EVENT_CALL_CONFIRMATION + 18)		/**<	Set Privacy mode	*/
#define TAPI_EVENT_CALL_EXIT_EMERGENCYMODE_CNF	(TAPI_EVENT_CALL_CONFIRMATION + 19)		/**<	Exit Emergency mode	cnf */
#define TAPI_EVENT_CALL_CNF_MAX					(TAPI_EVENT_CALL_CONFIRMATION + 20)

#define TAPI_EVENT_CALL_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_CALL_VOICE << 8))
#define TAPI_EVENT_CALL_ALERT_IND				(TAPI_EVENT_CALL_NOTIFICATION + 0)		/**<	This event indicates the Call is in Alerting stage*/
#define TAPI_EVENT_CALL_CONNECTING_IND			(TAPI_EVENT_CALL_NOTIFICATION + 1)		/**<	This event indicates the Call is in Connecting stage*/
#define TAPI_EVENT_CALL_CONNECTED_IND			(TAPI_EVENT_CALL_NOTIFICATION + 2)		/**<	This event indicates the Call is Connected */
#define TAPI_EVENT_CALL_END_IND					(TAPI_EVENT_CALL_NOTIFICATION + 3)		/**<	This event indicates the Call is Ended. This event can be solicited/unsolicited.*/
#define TAPI_EVENT_CALL_INCOM_IND				(TAPI_EVENT_CALL_NOTIFICATION + 4)		/**<	This event indicates Incoming Call indication to Application*/
#define TAPI_EVENT_CALL_HOLD_IND				(TAPI_EVENT_CALL_NOTIFICATION + 5)		/**<	This event indicates that other party is has put the call on hold*/
#define TAPI_EVENT_CALL_RETRIEVE_IND			(TAPI_EVENT_CALL_NOTIFICATION + 6)		/**<	This event indicates that other party has retrieved the call from hold*/
#define TAPI_EVENT_CALL_SETUPCONFERENCE_IND		(TAPI_EVENT_CALL_NOTIFICATION + 7)		/**<	Conference setup indication*/
#define TAPI_EVENT_CALL_WAITING_IND				(TAPI_EVENT_CALL_NOTIFICATION + 8)		/**<	Call waiting indication*/
#define TAPI_EVENT_CALL_FORWARD_IND				(TAPI_EVENT_CALL_NOTIFICATION + 9)		/**<	This event indication for Call is forwarded */
#define TAPI_EVENT_CALL_TRANSFER_IND			(TAPI_EVENT_CALL_NOTIFICATION + 10)		/**<	Call Transfer Indication*/
#define TAPI_EVENT_CALL_TIME_IND				(TAPI_EVENT_CALL_NOTIFICATION + 11)		/**<	call time ind */
#define TAPI_EVENT_CALL_BARRING_IND				(TAPI_EVENT_CALL_NOTIFICATION + 12)		/**<	This event indicates the Call is Barred*/
#define TAPI_EVENT_CALL_FORWARDED_INFO_IND		(TAPI_EVENT_CALL_NOTIFICATION + 13)		/**<	This event indicates the MT call is Forwarded*/
#define TAPI_EVENT_CALL_CUGINFO_IND				(TAPI_EVENT_CALL_NOTIFICATION + 14)		/**<	This event indicates CUG information */
#define TAPI_EVENT_CALL_AOCINFO_IND				(TAPI_EVENT_CALL_NOTIFICATION + 15)		/**<	This event indicates the AOC Information*/
#define TAPI_EVENT_CALL_CALLINGNAMEINFO_IND		(TAPI_EVENT_CALL_NOTIFICATION + 16)		/**<	This event indicates the Calling Name Information*/
#define TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND	(TAPI_EVENT_CALL_NOTIFICATION + 17)		/**<	This event indicates the Connected Number Information*/
#define TAPI_EVENT_CALL_CLISUPRESSIONREJ_IND	(TAPI_EVENT_CALL_NOTIFICATION + 18)		/**<	This event indicates the CLI Suppression Indication*/
#define TAPI_EVENT_CALL_DEFLECTED_IND			(TAPI_EVENT_CALL_NOTIFICATION + 19)		/**<	This event indicates the Call is Deflected*/
#define TAPI_EVENT_CALL_UNCOND_FORWARDING_IND	(TAPI_EVENT_CALL_NOTIFICATION + 20)		/**<	This event indicates the Call is unconditionally forwarded*/
#define TAPI_EVENT_CALL_COND_FORWARDING_IND		(TAPI_EVENT_CALL_NOTIFICATION + 21)		/**<	This event indicates the Call is conditionally forwarded*/
#define TAPI_EVENT_CALL_CONNECTED_ECT_REMOTE_IND	(TAPI_EVENT_CALL_NOTIFICATION + 22)		/**<	This event indicates the ECT remote connection  */
#define TAPI_EVENT_CALL_ON_HOLD_RELEASED_IND	(TAPI_EVENT_CALL_NOTIFICATION + 23)		/**<	This event indicates the Call on hold is released*/
#define TAPI_EVENT_CALL_ALERT_ECT_REMOTE_IND	(TAPI_EVENT_CALL_NOTIFICATION + 24)		/**<	This event indicates the ECT remote alert*/
#define TAPI_EVENT_CALL_SS_FORWARDED_MSG_RXED_IND	(TAPI_EVENT_CALL_NOTIFICATION + 25)		/**<	This event indicates that the forwarded msg has been received*/
#define TAPI_EVENT_CALL_OTA_PROGRESS_IND		(TAPI_EVENT_CALL_NOTIFICATION + 26)		/**<       ota progress ind */
#define TAPI_EVENT_CALL_DISPLAY_INFO_IND		(TAPI_EVENT_CALL_NOTIFICATION + 27)		/**<       display info ind */
#define TAPI_EVENT_CALL_EMERGENCYMODE_IND		(TAPI_EVENT_CALL_NOTIFICATION + 28)		/**<	Emergency mode	ind */
#define TAPI_EVENT_CALL_PRIVACYMODE_IND			(TAPI_EVENT_CALL_NOTIFICATION + 29)		/**<	Privacy mode Indication	*/
#define TAPI_EVENT_CALL_IND_MAX					(TAPI_EVENT_CALL_NOTIFICATION + 30)

/*	Events releated to Data Call */
#define TAPI_EVENT_DATA_CALL_CONFIRMATION		(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_CALL_DATA << 8))
#define TAPI_EVENT_DATA_SETUP_CNF				(TAPI_EVENT_DATA_CALL_CONFIRMATION + 0)		/**<	This event indicates the Call Setup request triggered */
#define TAPI_EVENT_DATA_CNF_MAX					(TAPI_EVENT_DATA_CALL_CONFIRMATION + 1)

#define TAPI_EVENT_DATA_CALL_NOTIFICATION		(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_CALL_DATA << 8))
#define TAPI_EVENT_DATA_ALERT_IND				(TAPI_EVENT_DATA_CALL_NOTIFICATION + 0)		/**<	This event indicates the Call is in Alerting stage*/
#define TAPI_EVENT_DATA_CONNECTED_IND			(TAPI_EVENT_DATA_CALL_NOTIFICATION + 2)		/**<	This event indicates the Call is Connected */
#define TAPI_EVENT_DATA_END_IND				(TAPI_EVENT_DATA_CALL_NOTIFICATION + 3)		/**<	This event indicates the Call is Ended. This event can be solicited/unsolicited.*/
#define TAPI_EVENT_DATA_INCOM_IND			(TAPI_EVENT_DATA_CALL_NOTIFICATION + 4)		/**<	This event indicates Incoming Call indication to Application*/
#define TAPI_EVENT_DATA_AOCINFO_IND			(TAPI_EVENT_DATA_CALL_NOTIFICATION + 15)	/**<	This event indicates the Call is in Alerting stage*/
#define TAPI_EVENT_DATA_IND_MAX					(TAPI_EVENT_DATA_CALL_NOTIFICATION + 16)

/*	Events releated to SIM */
#define TAPI_EVENT_SIM_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_SIM << 8))
#define TAPI_EVENT_SIM_VERIFY_SEC_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 0)		/**<	verify pin,sim*/
#define TAPI_EVENT_SIM_VERIFY_PUK_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 1)		/**<	verify puk1, puk2*/
#define TAPI_EVENT_SIM_CHANGE_PINS_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 2)		/**<	change pin1,2*/
#define TAPI_EVENT_SIM_DISABLE_SEC_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 3)		/**<	disable security facility*/
#define TAPI_EVENT_SIM_ENABLE_SEC_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 4)		/**<	enable security facility*/
#define TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 5)		/**<	disable fdn mode*/
#define TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 6)		/**<	enable fdn mode*/
#define TAPI_EVENT_SIM_PERS_STATUS_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 7)		/**<	status of personalization*/
#define TAPI_EVENT_SIM_DISABLE_PERS_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 8)		/**<	disable personalization*/
#define TAPI_EVENT_SIM_ENABLE_PERS_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 9)		/**<	enable personalization*/
#define TAPI_EVENT_SIM_GET_INDICATION_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 10)		/**<	read sim indication information(mwis, cfis)*/
#define TAPI_EVENT_SIM_SET_INDICATION_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 11)		/**<	update sim indication information(mwis, cfis)*/
#define TAPI_EVENT_SIM_GET_MAILBOX_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 12)		/**<	read sim mailbox information*/
#define TAPI_EVENT_SIM_SET_MAILBOX_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 13)		/**<	update sim mailbox information*/
#define TAPI_EVENT_SIM_SET_LANGUAGE_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 14)		/**<	update sim language information*/
#define TAPI_EVENT_SIM_RSIM_ACCESS_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 15)		/**<	rsim operation*/
#define TAPI_EVENT_SIM_AUTHENTICATION_CNF	(TAPI_EVENT_SIM_CONFIRMATION + 16)		/**<	ISIM authentication confirmation*/
#define TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 17)		/**<	Phonebook storage count*/
#define TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 18)		/**<	Phonebook entry information*/
#define TAPI_EVENT_SIM_PB_ACCESS_READ_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 19)		/**<	Phonebook access read*/
#define TAPI_EVENT_SIM_PB_UPDATE_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 20)		/**<	Phonebook update result*/
#define TAPI_EVENT_SIM_PB_DELETE_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 21)		/**<	Phonebook delete result*/
#define TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF	(TAPI_EVENT_SIM_CONFIRMATION + 22)		/**<	3G SIM PB capability Info */
#define TAPI_EVENT_SIM_SAP_CONNECT_CNF			(TAPI_EVENT_SIM_CONFIRMATION + 23)		/**<	SAP connect confirmation */
#define TAPI_EVENT_SIM_SAP_CONNECT_STATUS_CNF	(TAPI_EVENT_SIM_CONFIRMATION + 24)		/**<	SAP connection status confirmation*/
#define TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 25)		/**<	SAP transfer ATR confirmation */
#define TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF	(TAPI_EVENT_SIM_CONFIRMATION + 26)		/**<	SAP transfer APDU confirmation */
#define TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF		(TAPI_EVENT_SIM_CONFIRMATION + 27)		/**<	SAP set protocol confirmation */
#define TAPI_EVENT_SIM_SAP_SET_SIM_POWER_CNF	(TAPI_EVENT_SIM_CONFIRMATION + 28)		/**<	SAP set sim power confirmation */
#define TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF	(TAPI_EVENT_SIM_CONFIRMATION + 29)	/**<	SAP card reader status confirmation*/
#define TAPI_EVENT_SIM_APDU_CNF					(TAPI_EVENT_SIM_CONFIRMATION + 30)	/**<	apdu confirmation*/
#define TAPI_EVENT_SIM_ATR_CNF					(TAPI_EVENT_SIM_CONFIRMATION + 31)	/**<	atr confirmation*/
#define TAPI_EVENT_SIM_CNF_MAX					(TAPI_EVENT_SIM_CONFIRMATION + 32)

#define TAPI_EVENT_SIM_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_SIM << 8))
#define TAPI_EVENT_SIM_STATUS_IND				(TAPI_EVENT_SIM_NOTIFICATION + 0)		/**<	Unsolicited event --- whenever there is a changes in SIM status*/
#define TAPI_EVENT_SIM_FDN_STATUS_IND			(TAPI_EVENT_SIM_NOTIFICATION + 1)		/**<	Unsolicited event --- whenever there is a changes in FDN status and phonebook init status*/
#define TAPI_EVENT_SIM_SAP_CONNECT_NOTI			(TAPI_EVENT_SIM_NOTIFICATION + 2)		/**<	SAP connect notification */
#define TAPI_EVENT_SIM_IND_MAX					(TAPI_EVENT_SIM_NOTIFICATION + 3)		/**<	SAP connect notification */

/* Events related on Nettext Messages  */
#define TAPI_EVENT_NETTEXT_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_NETTEXT << 8))
#define TAPI_EVENT_NETTEXT_SENTSTATUS_CNF		(TAPI_EVENT_NETTEXT_CONFIRMATION + 0)	/**<	This event is returned in response to SMS  message sent*/
#define TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF	(TAPI_EVENT_NETTEXT_CONFIRMATION + 1)	/**<	This event is returned in response to send delivery report*/
#define TAPI_EVENT_NETTEXT_READ_SMS_CNF		(TAPI_EVENT_NETTEXT_CONFIRMATION + 2)	/**<	This event is returned in response to SMS Read message*/
#define TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF		(TAPI_EVENT_NETTEXT_CONFIRMATION + 3)	/**<	This confirmation event is a returned indicating the Save SMS into SIM status*/
#define TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF	(TAPI_EVENT_NETTEXT_CONFIRMATION + 4)	/**<	This confirmation event is a returned indicating the delete message status*/
#define TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF	(TAPI_EVENT_NETTEXT_CONFIRMATION + 5)	/**<	This event is returned with Get CB configuration details*/
#define TAPI_EVENT_NETTEXT_GET_COUNT_CNF		(TAPI_EVENT_NETTEXT_CONFIRMATION + 6)	/**<	This event is returned in response to Get Number of messages stored in SIM*/
#define TAPI_EVENT_NETTEXT_GET_SCA_CNF			(TAPI_EVENT_NETTEXT_CONFIRMATION + 7)	/**<	This event is returned with Get SCA Cnf*/
#define TAPI_EVENT_NETTEXT_PARAM_COUNT_IND		(TAPI_EVENT_NETTEXT_CONFIRMATION + 8)	/**<	Param count response */
#define TAPI_EVENT_NETTEXT_GET_PARAM_CNF		(TAPI_EVENT_NETTEXT_CONFIRMATION + 9)	/**<	This event is returned in response to NetText Get parameters*/
#define TAPI_EVENT_NETTEXT_SET_REQUEST_CNF		(TAPI_EVENT_NETTEXT_CONFIRMATION + 10)	/**<	This event is returned as a confirmation for SET nettext APIs*/
#define TAPI_EVENT_NETTEXT_SENTSTATUS_EX_CNF	(TAPI_EVENT_NETTEXT_CONFIRMATION + 20)	/**<	This event is returned in response to SMS  message extended sent*/
#define TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF	(TAPI_EVENT_NETTEXT_CONFIRMATION + 21)	/**<	This event is returned as a response to get preferred bearer used for SMS Transport*/
#define TAPI_EVENT_NETTEXT_CNF_MAX				(TAPI_EVENT_NETTEXT_CONFIRMATION + 22)

#define TAPI_EVENT_NETTEXT_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_NETTEXT << 8))
#define TAPI_EVENT_NETTEXT_INCOM_IND			(TAPI_EVENT_NETTEXT_NOTIFICATION + 0)	/**<	This is an unsolicited Incoming SMS Event*/
#define TAPI_EVENT_NETTEXT_CB_INCOM_IND			(TAPI_EVENT_NETTEXT_NOTIFICATION + 1)	/**<	This is an unsolicited Incoming cell broadcast message*/
#define TAPI_EVENT_NETTEXT_INCOM_EX_IND			(TAPI_EVENT_NETTEXT_NOTIFICATION + 2)	/**<	This is an extended unsolicited Incoming SMS Event*/
#define TAPI_EVENT_NETTEXT_CB_INCOM_EX_IND		(TAPI_EVENT_NETTEXT_NOTIFICATION + 3)	/**<	This is an extended unsolicited Incoming cell broadcast message*/
#define TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND	(TAPI_EVENT_NETTEXT_NOTIFICATION + 4)	/**<	This event is an unsolicited event indicating SIM memory status */
#define TAPI_EVENT_NETTEXT_DEVICE_READY_IND		(TAPI_EVENT_NETTEXT_NOTIFICATION + 5)	/**<	Device Ready indication */
#define TAPI_EVENT_NETTEXT_IND_MAX				(TAPI_EVENT_NETTEXT_NOTIFICATION + 6)

/* Events related on Network Messages  */
#define TAPI_EVENT_NETWORK_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_NETWORK << 8))
#define TAPI_EVENT_NETWORK_SELECT_CNF			(TAPI_EVENT_NETWORK_CONFIRMATION + 0)	/**<	This confirmation event is returned when user manually selected a network from list for Network selection */
#define TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 1)	/**<	This event is returned on getting network selection mode*/
#define TAPI_EVENT_NETWORK_SEARCH_CNF			(TAPI_EVENT_NETWORK_CONFIRMATION + 2)	/**<	This event is returned as confirmation for manual Network search with the available network list*/
#define TAPI_EVENT_NETWORK_SETNWBAND_CNF		(TAPI_EVENT_NETWORK_CONFIRMATION + 3)	/**<	This event indicates the Network Band has been set*/
#define TAPI_EVENT_NETWORK_GETNWBAND_CNF		(TAPI_EVENT_NETWORK_CONFIRMATION + 4)	/**<	This event indicates the Network Band has been identified and returned*/
#define TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 5)	/**<	This event confirms the request to set the service domain */
#define TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 6)	/**<	This event confirms the request to get the service domain */
#define TAPI_EVENT_NETWORK_SETNWMODE_CNF		(TAPI_EVENT_NETWORK_CONFIRMATION + 7)	/**<	This event is returned on setting network mode*/
#define TAPI_EVENT_NETWORK_GETNWMODE_CNF		(TAPI_EVENT_NETWORK_CONFIRMATION + 8)	/**<	This event is returned on getting network mode*/
#define TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 9)	/**<	This event indicates the preferred PLMN has been set*/
#define TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 10)	/**<	This event indicates that the PLMN used has been identified and returned */
#define TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 11)	/**<	 This event is returned on setting roaming mode */
#define TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 12)	/**<	This event is returned on getting roaming mode */
#define TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 13)	/**<	This event is returned on setting cdma hybrid mode */
#define TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF	(TAPI_EVENT_NETWORK_CONFIRMATION + 14)	/**<	This event is returned on getting cdma hybrid mode */
#define TAPI_EVENT_NETWORK_CNF_MAX				(TAPI_EVENT_NETWORK_CONFIRMATION + 15)	/**<	This event is returned on getting cdma hybrid mode */

#define TAPI_EVENT_NETWORK_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_NETWORK << 8))
#define TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND	(TAPI_EVENT_NETWORK_NOTIFICATION + 0)	/**<	This event indicates the LAC and CELLID has been obtained*/
#define TAPI_EVENT_NETWORK_RADIO_INFO_IND		(TAPI_EVENT_NETWORK_NOTIFICATION + 1)	/**<	This event indicates the Radio Information is got*/
#define TAPI_EVENT_NETWORK_SPN_INFO				(TAPI_EVENT_NETWORK_NOTIFICATION + 2)	/**<	This event indicates the SPN info is got*/
#define TAPI_EVENT_NETWORK_DISPLAYINFO_IND		(TAPI_EVENT_NETWORK_NOTIFICATION + 3)	/**<	This event indicates the Display Information is obtained*/
#define TAPI_EVENT_NETWORK_CHANGE_IND			(TAPI_EVENT_NETWORK_NOTIFICATION + 4)	/**<	This event indicates there is a change in Network*/
#define TAPI_EVENT_NETWORK_IND_MAX				(TAPI_EVENT_NETWORK_NOTIFICATION + 5)

/* Events related on SS Messages  */
#define TAPI_EVENT_SS_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_SS << 8))
#define TAPI_EVENT_SS_BARRING_CNF				(TAPI_EVENT_SS_CONFIRMATION + 0)		/**<	This confirmation event is sent when Barring activation/deactivation is requested*/
#define TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF	(TAPI_EVENT_SS_CONFIRMATION + 1)		/**<	This event is returned as Barring password change confirmation*/
#define TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF	(TAPI_EVENT_SS_CONFIRMATION + 2)		/**<	This event is returned  as Barring status query confirmation*/
#define TAPI_EVENT_SS_FORWARD_CNF				(TAPI_EVENT_SS_CONFIRMATION + 3)		/**<	This event is returned as confirmation for Forward Activation/Deactivation/Registration/Erasure */
#define TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF	(TAPI_EVENT_SS_CONFIRMATION + 4)		/**<	This event is returned as confirmation for Forward Status query confirmation*/
#define TAPI_EVENT_SS_WAITING_CNF				(TAPI_EVENT_SS_CONFIRMATION + 5)		/**<	This event is returned as confirmation for Call waiting activation/deactivation*/
#define TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF	(TAPI_EVENT_SS_CONFIRMATION + 6)		/**<	This event is returned as Call waiting status query confirmation*/
#define TAPI_EVENT_SS_USSD_CNF					(TAPI_EVENT_SS_CONFIRMATION + 7)		/**<	This event is returned as USSD send confirmation*/
#define TAPI_EVENT_SS_USSD_CANCEL_CNF			(TAPI_EVENT_SS_CONFIRMATION + 8)		/**<	This event is returned as USSD cancel confirmation	>**/
#define TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF		(TAPI_EVENT_SS_CONFIRMATION + 9)		/**<	This event is indicated to the Application for CLI service interrogation*/
#define TAPI_EVENT_SS_AOC_RSP					(TAPI_EVENT_SS_CONFIRMATION + 10)		/**<	This event is used to send the aoc information */
#define TAPI_EVENT_SS_SET_AOC_CNF				(TAPI_EVENT_SS_CONFIRMATION + 11)		/**<	This event is used to send the confirmation for setting aoc values */
#define TAPI_EVENT_SS_USSD_RSP_CNF			(TAPI_EVENT_SS_CONFIRMATION + 12)	/**<	This event is returned as USSD response confirmation	>**/
#define TAPI_EVENT_SS_CNF_MAX					(TAPI_EVENT_SS_CONFIRMATION + 13)

#define TAPI_EVENT_SS_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_SS << 8))
#define TAPI_EVENT_SS_USSD_IND					(TAPI_EVENT_SS_NOTIFICATION + 0)		/**<	This is an unsolicited USSD indication*/
#define TAPI_EVENT_SS_BARRING_TYPE_IND			(TAPI_EVENT_SS_NOTIFICATION + 1)		/**<	This event is used to send the barring type while making an MO call */
#define TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY		(TAPI_EVENT_SS_NOTIFICATION + 2)		/**<	This event is indicated to the Application when tapi receives Release complete message notification for any SS transactions*/
#define TAPI_EVENT_SS_REL_CO					(TAPI_EVENT_SS_NOTIFICATION + 3)		/**<	This event is indicated to the Application when tapi receives Release complete message notification for any SS transactions*/
#define TAPI_EVENT_SS_IND_MAX					(TAPI_EVENT_SS_NOTIFICATION + 4)

/*SAT events*/
#define TAPI_EVENT_SAT_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_SAT << 8))
#define TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF		(TAPI_EVENT_SAT_CONFIRMATION + 0)		/**< response from sim for event download request */
#define TAPI_EVENT_SAT_MENU_SELECTION_CNF		(TAPI_EVENT_SAT_CONFIRMATION + 1)		/**< response from sim for menu selection request */
#define TAPI_EVENT_SAT_CNF_MAX					(TAPI_EVENT_SAT_CONFIRMATION + 2)

#define TAPI_EVENT_SAT_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_SAT << 8))
#define TAPI_EVENT_SAT_DISPLAY_TEXT_IND			(TAPI_EVENT_SAT_NOTIFICATION + 0)		/**< 0x0D00, SAT Display Text Event*/
#define TAPI_EVENT_SAT_GET_INKEY_IND			(TAPI_EVENT_SAT_NOTIFICATION + 1)		/**< SAT Get Inkey Event */
#define TAPI_EVENT_SAT_GET_INPUT_IND			(TAPI_EVENT_SAT_NOTIFICATION + 2)		/**< SAT Get Input Event */
#define TAPI_EVENT_SAT_UI_PLAY_TONE_IND			(TAPI_EVENT_SAT_NOTIFICATION + 3)		/**< SAT Play Tone Event for UI client*/
#define TAPI_EVENT_SAT_PLAY_TONE_IND			(TAPI_EVENT_SAT_NOTIFICATION + 4)		/**< SAT Play Tone Event for MM client */
#define TAPI_EVENT_SAT_UI_REFRESH_IND			(TAPI_EVENT_SAT_NOTIFICATION + 5)		/**< SAT Refresh Event for UI client*/
#define TAPI_EVENT_SAT_SETUP_MENU_IND			(TAPI_EVENT_SAT_NOTIFICATION + 6)		/**< SAT Setup Menu Event for UI client */
#define TAPI_EVENT_SAT_UI_SETUP_MENU_IND		(TAPI_EVENT_SAT_NOTIFICATION + 7)		/**< SAT Setup Menu Event for UI client */
#define TAPI_EVENT_SAT_SELECT_ITEM_IND			(TAPI_EVENT_SAT_NOTIFICATION + 8)		/**< SAT Select Item Event */
#define TAPI_EVENT_SAT_UI_SEND_SMS_IND			(TAPI_EVENT_SAT_NOTIFICATION + 9)		/**< SAT Send SMS Event for UI client */
#define TAPI_EVENT_SAT_UI_SEND_SS_IND			(TAPI_EVENT_SAT_NOTIFICATION + 10)		/**< SAT Send SS Event for UI client */
#define TAPI_EVENT_SAT_UI_SEND_USSD_IND			(TAPI_EVENT_SAT_NOTIFICATION + 11)		/**< SAT Send USSD Event for UI client */
#define TAPI_EVENT_SAT_UI_SETUP_CALL_IND		(TAPI_EVENT_SAT_NOTIFICATION + 12)		/**< SAT Setup Call Event for UI client */
#define TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND	(TAPI_EVENT_SAT_NOTIFICATION + 13)		/**< SAT Provide Local Information Event*/
#define TAPI_EVENT_SAT_SETUP_IDLE_MODE_TEXT_IND	(TAPI_EVENT_SAT_NOTIFICATION + 14)		/**< SAT Setup Idle Mode Event*/
#define TAPI_EVENT_SAT_UI_SEND_DTMF_IND			(TAPI_EVENT_SAT_NOTIFICATION + 15)		/**< SAT Send DTMF Event for UI client*/
#define TAPI_EVENT_SAT_LANGUAGE_NOTIFICATION_IND	(TAPI_EVENT_SAT_NOTIFICATION + 16)	/**< SAT Language notification Event */
#define TAPI_EVENT_SAT_UI_LAUNCH_BROWSER_IND	(TAPI_EVENT_SAT_NOTIFICATION + 17)		/**< SAT Launch Browser Event for UI client*/
#define TAPI_EVENT_SAT_END_PROACTIVE_SESSION_IND	(TAPI_EVENT_SAT_NOTIFICATION + 18)	/**< SAT End Proactive Command Session Event. This is to indicate that Proactive UICC Session ended*/
#define TAPI_EVENT_SAT_CALL_CONTROL_IND			(TAPI_EVENT_SAT_NOTIFICATION + 19)		/**< response from sim for call control by USIM. This is to indicate that the voice call/ss/ussd parameters have been modified by USIM  */
#define TAPI_EVENT_SAT_MO_SMS_CONTROL_IND		(TAPI_EVENT_SAT_NOTIFICATION + 20)		/**< response from sim for Mo sms control by USIM. This is to indicate that the SMS parameters have been modified by USIM */
#define TAPI_EVENT_SAT_SMS_REFRESH_IND	(TAPI_EVENT_SAT_NOTIFICATION + 21)		/**< SAT Refresh Event for SMS client */
#define TAPI_EVENT_SAT_PB_REFRESH_IND	(TAPI_EVENT_SAT_NOTIFICATION + 22)		/**< SAT Refresh Event for Phonebook client*/
#define TAPI_EVENT_SAT_IDLE_REFRESH_IND		(TAPI_EVENT_SAT_NOTIFICATION + 23)		/**< SAT Refresh Event for Idle Mode client */
#define TAPI_EVENT_SAT_SEND_SMS_IND			(TAPI_EVENT_SAT_NOTIFICATION + 24)		/**< SAT Send SMS Event for SMS client*/
#define TAPI_EVENT_SAT_SETUP_CALL_IND	(TAPI_EVENT_SAT_NOTIFICATION + 25)		/**< SAT Setup Call Event for Call client */
#define TAPI_EVENT_SAT_SEND_SS_IND		(TAPI_EVENT_SAT_NOTIFICATION + 26)		/**< SAT Send SS Event for SS client*/
#define TAPI_EVENT_SAT_SEND_USSD_IND	(TAPI_EVENT_SAT_NOTIFICATION + 27)		/**< SAT Send SS Event for USSD client */
#define TAPI_EVENT_SAT_SEND_DTMF_IND		(TAPI_EVENT_SAT_NOTIFICATION + 28)		/**< SAT Send DTMF Event for UI client*/
#define TAPI_EVENT_SAT_LAUNCH_BROWSER_IND	(TAPI_EVENT_SAT_NOTIFICATION + 29)		/**< SAT Launch Browser Event for Browser */
#define TAPI_EVENT_SAT_END_OF_APP_EXEC_IND		(TAPI_EVENT_SAT_NOTIFICATION + 30)		/**< SAT End Proactive command Execution Event. This event will be indicated to the application to end the execution of a Proactive Command when required  */
#define TAPI_EVENT_SAT_EVENT_DOWNLOAD_IND	(TAPI_EVENT_SAT_NOTIFICATION + 31)		/**< SAT Event Download Indication for Idle, Browser, Setting applications */
#define TAPI_EVENT_SAT_IND_MAX					(TAPI_EVENT_SAT_NOTIFICATION + 32)

/* DATA Events */
#define TAPI_EVENT_PS_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_DATA << 8))
#define TAPI_EVENT_PS_PDP_ACT_RSP				(TAPI_EVENT_PS_CONFIRMATION + 0)			/**< 0x0B00,   -  PDP activation response event */
#define TAPI_EVENT_PS_PDP_DEACT_RSP				(TAPI_EVENT_PS_CONFIRMATION + 1)			/**<      PDP deactivation response event */
#define TAPI_EVENT_PS_CNF_MAX					(TAPI_EVENT_PS_CONFIRMATION + 5)

#define TAPI_EVENT_PS_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_DATA << 8))
#define TAPI_EVENT_PS_PDP_DEACT_IND				(TAPI_EVENT_PS_NOTIFICATION + 0)			/**<     PDP deactivation indication event. */
#define TAPI_EVENT_PS_STATUS					(TAPI_EVENT_PS_NOTIFICATION + 1)			/**<     PDP status event. */
#define TAPI_EVENT_PS_CALL_STATISTICS			(TAPI_EVENT_PS_NOTIFICATION + 2)			/**<     publish notification or response of the call statistics  */
#define TAPI_EVENT_PS_HSDPA_STATUS_NOTI			(TAPI_EVENT_PS_NOTIFICATION + 3)			/**<   HSDPA Status notification event  */
#define TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI		(TAPI_EVENT_PS_NOTIFICATION + 4)			/**<  BT DUN PIN notification event   */
#define TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO		(TAPI_EVENT_PS_NOTIFICATION + 5)			/**<  PDP IP information event  */
#define TAPI_EVENT_PS_EXTERNAL_CALL_IND			(TAPI_EVENT_PS_NOTIFICATION + 6)			/**<  External call indication event  */
#define TAPI_EVENT_PS_ERROR_IND					(TAPI_EVENT_PS_NOTIFICATION + 7)			/**<  PS Error event   */
#define TAPI_EVENT_PS_IND_MAX					(TAPI_EVENT_PS_NOTIFICATION + 8)

/* Sound events */
#define TAPI_EVENT_SOUND_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_SOUND << 8))
#define TAPI_EVENT_SOUND_VOLUMECTRL_RSP			(TAPI_EVENT_SOUND_CONFIRMATION + 0)	       /**<  Phone volumn response event  */
#define TAPI_EVENT_SOUND_MICMUTECTRL_RSP		(TAPI_EVENT_SOUND_CONFIRMATION + 1)		/**<   MIC mute response event */
#define TAPI_EVENT_SOUND_SUCCESS_IND			(TAPI_EVENT_SOUND_CONFIRMATION + 2)		/**<   Sound Error response event */
#define TAPI_EVENT_SOUND_ERROR_IND				(TAPI_EVENT_SOUND_CONFIRMATION + 3)		/**<   Sound Success response event */
#define TAPI_EVENT_SOUND_CNF_MAX				(TAPI_EVENT_SOUND_CONFIRMATION + 4)

#define TAPI_EVENT_SOUND_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_SOUND << 8))
#define TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI		(TAPI_EVENT_SOUND_NOTIFICATION + 0)	/**<  Audio path indication event   */
#define TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI	(TAPI_EVENT_SOUND_NOTIFICATION + 1)	/**<   Audio source indication event  */
#define TAPI_EVENT_SOUND_IND_MAX				(TAPI_EVENT_SOUND_NOTIFICATION + 2)

/* Util events */
#define TAPI_EVENT_UTIL_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_UTIL << 8))
#define TAPI_EVENT_UTIL_TUNNEL_CNF				(TAPI_EVENT_UTIL_CONFIRMATION + 0)	       /**<  tunnel response event  */

#define TAPI_EVENT_UTIL_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_UTIL << 8))
#define TAPI_EVENT_UTIL_TUNNEL_NOTI				(TAPI_EVENT_UTIL_NOTIFICATION + 0)	       /**<  tunnel notification event  */

/* Extension events */
#define	TAPI_EVENT_CLASS_ERR			(TAPI_EVENT_CLASS_EXTENSION)		/**< Fatal error Event  Class */
#define	TAPI_EVENT_CLASS_POWER			(TAPI_EVENT_CLASS_EXTENSION + 1)	/**< POWEREvents  Class */
#define	TAPI_EVENT_CLASS_DISPLAY		(TAPI_EVENT_CLASS_EXTENSION + 2)	/**< DISPLAY Events  Class */
#define	TAPI_EVENT_CLASS_MISC			(TAPI_EVENT_CLASS_EXTENSION + 3)	/**< MISC Events  Class */
#define	TAPI_EVENT_CLASS_CFG			(TAPI_EVENT_CLASS_EXTENSION + 4)	/**< CFG Events  Class */
#define	TAPI_EVENT_CLASS_IMEI			(TAPI_EVENT_CLASS_EXTENSION + 5)	/**< MISC Events  Class */
#define	TAPI_EVENT_CLASS_GPS			(TAPI_EVENT_CLASS_EXTENSION + 6)	/**< GPS Events  Class */
#define	TAPI_EVENT_CLASS_OMADM			(TAPI_EVENT_CLASS_EXTENSION + 7)	/**< OMADM Events  Class */
#define TAPI_EVENT_CLASS_SVCMODE		(TAPI_EVENT_CLASS_EXTENSION + 8)
#define TAPI_EVENT_CLASS_FACTORY		(TAPI_EVENT_CLASS_EXTENSION + 9)
#define TAPI_EVENT_CLASS_CALL_E911		(TAPI_EVENT_CLASS_EXTENSION + 10)	// check why this is needed.
#define	TAPI_EVENT_CLASS_CDMA_DATA		(TAPI_EVENT_CLASS_EXTENSION + 11)	/**< CDMA Data Events  Class */

#define	TAPI_EVENT_NETWORK_GETPOWERONATTACH_CNF			(TAPI_EVENT_NETWORK_CNF_MAX + 0)	/**< This event indicates the Power on attach options has been identified and returned */
#define	TAPI_EVENT_NETWORK_SETPOWERONATTACH_CNF			(TAPI_EVENT_NETWORK_CNF_MAX + 1)	/**< This event indicates the Power on attach has been set */
#define	TAPI_EVENT_NETWORK_GETNWORDER_CNF				(TAPI_EVENT_NETWORK_CNF_MAX + 2)	/**< This event is returned on getting network order */
#define	TAPI_EVENT_NETWORK_SETNWORDER_CNF				(TAPI_EVENT_NETWORK_CNF_MAX + 3)	/**< This event is returned on setting network order */
#define	TAPI_EVENT_NETWORK_CANCELMANUALSELECTION_CNF	(TAPI_EVENT_NETWORK_CNF_MAX + 4)	/**< This event is returned on cancellation of manual network selection from network list */
#define	TAPI_EVENT_NETWORK_CANCELMANUALSEARCH_CNF		(TAPI_EVENT_NETWORK_CNF_MAX + 5)	/**< This event is returned on cancellation of manual network search */
#define	TAPI_EVENT_NETWORK_SEARCH_POPUP_ENABLE			(TAPI_EVENT_NETWORK_CNF_MAX + 6)	/**< This event indicates a status which applications refer to show network search popup */
#define	TAPI_EVENT_NETWORK_SEARCH_POPUP_DISABLE			(TAPI_EVENT_NETWORK_CNF_MAX + 7)	/**< This event indicates a status which applications refer to show network search popup */
#define	TAPI_EVENT_NETWORK_INTERNAL_CNF_MAX				(TAPI_EVENT_NETWORK_CNF_MAX + 8)

/* Error Class Events*/
#define	TAPI_EVENT_ERR_NOTIFICATION					(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_ERR<<8))
#define	TAPI_EVENT_ERR								(TAPI_EVENT_ERR_NOTIFICATION + 0)		/**< Fatal Error Information */
#define	TAPI_EVENT_ERR_RESERVED1_IND				(TAPI_EVENT_ERR_NOTIFICATION + 1)		/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
#define	TAPI_EVENT_ERR_RESERVED2_IND				(TAPI_EVENT_ERR_NOTIFICATION + 2)		/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
#define	TAPI_EVENT_ERR_RESERVED3_IND				(TAPI_EVENT_ERR_NOTIFICATION + 3)		/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
#define	TAPI_EVENT_ERR_RESERVED4_IND				(TAPI_EVENT_ERR_NOTIFICATION + 4)		/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
#define	TAPI_EVENT_ERR_RESERVED5_IND				(TAPI_EVENT_ERR_NOTIFICATION + 5)		/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */

/*	Power Module Event Types...	*/
#define	TAPI_EVENT_POWER_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_POWER<<8))
#define	TAPI_EVENT_POWER_FLIGHT_MODE_RESP			(TAPI_EVENT_POWER_CONFIRMATION + 0)		/**< Flight Mode Response */
#define	TAPI_EVENT_POWER_CNF_MAX					(TAPI_EVENT_POWER_CONFIRMATION + 1)

#define	TAPI_EVENT_POWER_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_POWER<<8))
#define	TAPI_EVENT_POWER_PHONE_OFF					(TAPI_EVENT_POWER_NOTIFICATION + 0)		/**< Modem power Off */
#define	TAPI_EVENT_POWER_BATT_STAT_IND				(TAPI_EVENT_POWER_NOTIFICATION + 1)		/**< Battery level Indication */
#define	TAPI_EVENT_POWER_SERVICE_READY_IND			(TAPI_EVENT_POWER_NOTIFICATION + 2)		/**< telephony service ready */
#define	TAPI_EVENT_POWER_IND_MAX					(TAPI_EVENT_POWER_NOTIFICATION + 3)

/*	Display Module Event Types...	*/
#define	TAPI_EVENT_DISPLAY_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_DISPLAY<<8))
#define	TAPI_EVENT_DISPLAY_ICON_INFO_IND			(TAPI_EVENT_DISPLAY_NOTIFICATION + 0)	/**< Display Icon Info Indication */
#define	TAPI_EVENT_DISPLAY_HMZ_O2_IND				(TAPI_EVENT_DISPLAY_NOTIFICATION + 1)	/**< Home Zone Info Indication */
#define	TAPI_EVENT_DISPLAY_PHONE_FATAL_NOTI			(TAPI_EVENT_DISPLAY_NOTIFICATION + 2)	/**< Phone Fatal Message Notification */
#define	TAPI_EVENT_DISPLAY_EXT_ROAM_INFO_NOTI		(TAPI_EVENT_DISPLAY_NOTIFICATION + 3)	/**< Roam Information Notification */
#define	TAPI_EVENT_DISPLAY_EXT_USER_IND_MSG_NOTI	(TAPI_EVENT_DISPLAY_NOTIFICATION + 4)	/**< User Indication Message Notification */
#define	TAPI_EVENT_DISPLAY_IND_MAX					(TAPI_EVENT_DISPLAY_NOTIFICATION + 5)

/*	Misc Module Types...	*/
#define	TAPI_EVENT_MISC_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_MISC<<8))
#define	TAPI_EVENT_MISC_NITZ_GETCNF				(TAPI_EVENT_MISC_CONFIRMATION + 0)
#define	TAPI_EVENT_MISC_NITZ_NOTIFY			(TAPI_EVENT_MISC_CONFIRMATION + 1)
#define	TAPI_EVENT_MISC_NITZ_IND				(TAPI_EVENT_MISC_CONFIRMATION + 2)
#define	TAPI_EVENT_MISC_AT_FUS				(TAPI_EVENT_MISC_CONFIRMATION + 3)
#define	TAPI_EVENT_MISC_SERIAL_NUMBER_IND			(TAPI_EVENT_MISC_CONFIRMATION + 4)
#define	TAPI_EVENT_MISC_GET_NAM_INFO_CNF			(TAPI_EVENT_MISC_CONFIRMATION + 5)
#define	TAPI_EVENT_MISC_SET_NAM_INFO_CNF			(TAPI_EVENT_MISC_CONFIRMATION + 6)
#define	TAPI_EVENT_MISC_NAM_INFO_NOTI				(TAPI_EVENT_MISC_CONFIRMATION + 7)
#define	TAPI_EVENT_MISC_CNF_MAX						(TAPI_EVENT_MISC_CONFIRMATION + 8)

/*	Config Module Types...	*/
#define	TAPI_EVENT_CFG_CONFIRMATION					(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_CFG<<8))
#define	TAPI_CFG_EVENT_DEFAULTCFG_IND				(TAPI_EVENT_CFG_CONFIRMATION + 0)	/**< Default configuration Event Type */
#define	TAPI_EVENT_CFG_GET_A_KEY_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 1)	/**< Get A key Event Type */
#define	TAPI_EVENT_CFG_VERIFY_A_KEY_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 2)	/**< A key verification Event Type */
#define	TAPI_EVENT_CFG_GET_MSL_INFO_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 3)	/**< Get MSL information Event Type */
#define	TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF		(TAPI_EVENT_CFG_CONFIRMATION + 4)	/**< Get vocoder option Event Type */
#define	TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF		(TAPI_EVENT_CFG_CONFIRMATION + 5)	/**< Set vocoder option Event Type */
#define	TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF	(TAPI_EVENT_CFG_CONFIRMATION + 6)	/**< Get hidden menu access Event Type */
#define	TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF	(TAPI_EVENT_CFG_CONFIRMATION + 7)	/**< Set hidden menu access Event  Type */
#define	TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF		(TAPI_EVENT_CFG_CONFIRMATION + 8)	/**< Get current carrier Event */
#define	TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF		(TAPI_EVENT_CFG_CONFIRMATION + 9)	/**< Set current carrier Event */
#define	TAPI_EVENT_CFG_GET_SIO_MODE_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 10)	/**< Get Phone SIO mode Event */
#define	TAPI_EVENT_CFG_SET_SIO_MODE_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 11)	/**< Set Phone SIO mode Event */
#define	TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF		(TAPI_EVENT_CFG_CONFIRMATION + 12)	/**< Get Activated Data Event */
#define	TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF	(TAPI_EVENT_CFG_CONFIRMATION + 13)	/**< Get Reconditioned data Event */
#define	TAPI_EVENT_CFG_SET_TTY_MODE_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 14)	/**< Set TTY mode Event */
#define	TAPI_EVENT_CFG_GET_TTY_MODE_CNF				(TAPI_EVENT_CFG_CONFIRMATION + 15)	/**< Get TTY mode Event */
#define	TAPI_EVENT_CFG_CNF_MAX						(TAPI_EVENT_CFG_CONFIRMATION + 16)

/*	IMEI Module Types...	*/
#define	TAPI_EVENT_IMEI_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_IMEI<<8))
#define	TAPI_EVENT_IMEI_CMLA_IND					(TAPI_EVENT_IMEI_CONFIRMATION + 0)
#define	TAPI_EVENT_IMEI_COMPAREITEM				(TAPI_EVENT_IMEI_CONFIRMATION + 1)
#define	TAPI_EVENT_IMEI_CNF_MAX						(TAPI_EVENT_IMEI_CONFIRMATION + 2)

/*	GPS Module Types...	*/
#define	TAPI_EVENT_GPS_CONFIRMATION				(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_GPS<<8))
#define	TAPI_EVENT_GPS_INIT_RESP				(TAPI_EVENT_GPS_CONFIRMATION + 0)	/**< Start session response event type. */
#define	TAPI_EVENT_GPS_DEINIT_RESP				(TAPI_EVENT_GPS_CONFIRMATION + 1)	/**< GPS option set response  event type. */
#define	TAPI_EVENT_GPS_FIX_REQUEST_RESP	(TAPI_EVENT_GPS_CONFIRMATION + 2)
#define	TAPI_EVENT_GPS_STOP_SESSION_RESP		(TAPI_EVENT_GPS_CONFIRMATION + 3)	/**< Close session response event type */
#define	TAPI_EVENT_GPS_PARAMETERS_SET_RESP		(TAPI_EVENT_GPS_CONFIRMATION + 4)
#define	TAPI_EVENT_GPS_XTRA_INIT			(TAPI_EVENT_GPS_CONFIRMATION + 5)
#define	TAPI_EVENT_GPS_XTRA_DEINIT		(TAPI_EVENT_GPS_CONFIRMATION + 6)
#define	TAPI_EVENT_GPS_XTRA_ENABLE_SET_RESP	(TAPI_EVENT_GPS_CONFIRMATION + 7)
#define	TAPI_EVENT_GPS_XTRA_SET_TIME_RESP	(TAPI_EVENT_GPS_CONFIRMATION + 8)
#define	TAPI_EVENT_GPS_XTRA_SET_DATA_RESP	(TAPI_EVENT_GPS_CONFIRMATION + 9)
#define	TAPI_EVENT_GPS_PARAMETERS_GET_RESP	(TAPI_EVENT_GPS_CONFIRMATION + 10)
#define	TAPI_EVENT_GPS_FREQ_AIDING_SET_RESP	(TAPI_EVENT_GPS_CONFIRMATION + 11)
#define	TAPI_EVENT_GPS_CNF_MAX				(TAPI_EVENT_GPS_CONFIRMATION + 12)

#define	TAPI_EVENT_GPS_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_GPS<<8))
#define	TAPI_EVENT_GPS_POSITION_NOTI			(TAPI_EVENT_GPS_NOTIFICATION + 0)
#define	TAPI_EVENT_GPS_EXT_POSITION_NOTI		(TAPI_EVENT_GPS_NOTIFICATION + 1)
#define	TAPI_EVENT_GPS_PD_CMD_CB_NOTI			(TAPI_EVENT_GPS_NOTIFICATION + 2)
#define	TAPI_EVENT_GPS_PDP_CONNECTION_IND		(TAPI_EVENT_GPS_NOTIFICATION + 3)
#define	TAPI_EVENT_GPS_DNS_LOOKUP_IND			(TAPI_EVENT_GPS_NOTIFICATION + 4)
#define	TAPI_EVENT_GPS_VERIFICATION_IND			(TAPI_EVENT_GPS_NOTIFICATION + 5)	/**< Verification indication event type. */
#define	TAPI_EVENT_GPS_SUPL_NOTI				(TAPI_EVENT_GPS_NOTIFICATION + 6)	/**< SUPL Notification event type. */
#define	TAPI_EVENT_GPS_XTRA_TIME_NOTI			(TAPI_EVENT_GPS_NOTIFICATION + 7)	/**< XTRA Time info notification event type. */
#define	TAPI_EVENT_GPS_XTRA_DATA_NOTI			(TAPI_EVENT_GPS_NOTIFICATION + 8)	/**< XTRA DATA Status injection notification. */
#define	TAPI_EVENT_GPS_ASSIST_DATA_NOTI			(TAPI_EVENT_GPS_NOTIFICATION + 9)	/**< XTRA DATA Status injection notification. */
#define	TAPI_EVENT_GPS_MEASURE_POSITION_IND		(TAPI_EVENT_GPS_NOTIFICATION + 10)	/**< XTRA DATA Status injection notification. */
#define	TAPI_EVENT_GPS_MEASURE_POSITION_CNF		(TAPI_EVENT_GPS_NOTIFICATION + 11)	/**< XTRA DATA Status injection notification. */
#define	TAPI_EVENT_GPS_RESET_ASSIST_DATA_NOTI	(TAPI_EVENT_GPS_NOTIFICATION + 12)	/**< XTRA DATA Status injection notification. */
#define	TAPI_EVENT_GPS_IND_MAX					(TAPI_EVENT_GPS_NOTIFICATION + 13)

/* OMA-DM events */
#define	TAPI_EVENT_OMADM_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_OMADM<<8))
#define	TAPI_EVENT_OMADM_PRL_SIZE_GET_CNF		(TAPI_EVENT_OMADM_CONFIRMATION + 0)	/**< OMA-DM PRL size get confirm event type. */
#define	TAPI_EVENT_OMADM_MODEL_NAME_GET_CNF		(TAPI_EVENT_OMADM_CONFIRMATION + 1)	/**< OMA-DM Model name get confirm event type. */
#define	TAPI_EVENT_OMADM_OEM_NAME_GET_CNF		(TAPI_EVENT_OMADM_CONFIRMATION + 2)	/**< OMA-DM OEM name get confirm event type. */
#define	TAPI_EVENT_OMADM_SW_VERSION_GET_CNF		(TAPI_EVENT_OMADM_CONFIRMATION + 3)	/**< OMA-DM SW version get confirm event type. */
#define	TAPI_EVENT_OMADM_PRL_READ_GET_CNF		(TAPI_EVENT_OMADM_CONFIRMATION + 4)	/**< OMA-DM PRL Data read confirm event type. */
#define	TAPI_EVENT_OMADM_PRL_WRITE_SET_CNF		(TAPI_EVENT_OMADM_CONFIRMATION + 5)	/**< OMA-DM PRL Data write confirm event type. */
#define	TAPI_EVENT_OMADM_CNF_MAX				(TAPI_EVENT_OMADM_CONFIRMATION + 6)

/* SVC event */
#define	TAPI_EVENT_SVCMODE_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_SVCMODE<<8))
#define	TAPI_EVENT_SVCMODE_START_NOTI			(TAPI_EVENT_SVCMODE_CONFIRMATION + 0)	// should be checked
#define	TAPI_EVENT_SVCMODE_END_NOTI				(TAPI_EVENT_SVCMODE_CONFIRMATION + 1)
#define	TAPI_EVENT_SVCMODE_DEBUG_DUMP_CNF		(TAPI_EVENT_SVCMODE_CONFIRMATION + 2)
#define	TAPI_EVENT_SVCMODE_CNF_MAX				(TAPI_EVENT_SVCMODE_CONFIRMATION + 3)

#define	TAPI_EVENT_SVCMODE_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_SVCMODE<<8))
#define	TAPI_EVENT_SVCMODE_CHANGE_NOTI			(TAPI_EVENT_SVCMODE_NOTIFICATION + 0)
#define	TAPI_EVENT_SVCMODE_SCREEN_CFG_NOTI		(TAPI_EVENT_SVCMODE_NOTIFICATION + 1)
#define	TAPI_EVENT_SVCMODE_DISP_SCREEN_NOTI		(TAPI_EVENT_SVCMODE_NOTIFICATION + 2)
#define	TAPI_EVENT_SVCMODE_DEVICE_TEST_NOTI		(TAPI_EVENT_SVCMODE_NOTIFICATION + 3)
#define	TAPI_EVENT_SVCMODE_IND_MAX				(TAPI_EVENT_SVCMODE_NOTIFICATION + 4)

#define TAPI_EVENT_FACTORY_CONFIRMATION					(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_FACTORY<<8))
#define TAPI_EVENT_FACTORY_OMISSION_AVOIDANCE_SET_CNF	(TAPI_EVENT_FACTORY_CONFIRMATION + 0)
#define TAPI_EVENT_FACTORY_OMISSION_AVOIDANCE_GET_CNF	(TAPI_EVENT_FACTORY_CONFIRMATION + 1)

#define	TAPI_EVENT_FACTORY_NOTIFICATION				(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_FACTORY<<8))
#define	TAPI_EVENT_FACTORY_DFT_SMS					(TAPI_EVENT_FACTORY_NOTIFICATION + 0)
#define	TAPI_EVENT_FACTORY_IND_MAX					(TAPI_EVENT_FACTORY_NOTIFICATION + 1)

/* CDMA DATA events */
#define	TAPI_EVENT_CDMA_DATA_CONFIRMATION			(TAPI_EVENT_TYPE_CONFIRMATION + (TAPI_EVENT_CLASS_CDMA_DATA<<8))
#define	TAPI_EVENT_DATA_MIP_NAI_GET_CNF				(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 0)		/**< CDMA Data Service PIN Control confirm event type. */
#define	TAPI_EVENT_DATA_MIP_NAI_SET_CNF				(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 1)		/**< CDMA Data MIP NAI set confirm event type. */
#define	TAPI_EVENT_DATA_CURRENT_NAI_INDEX_GET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 2)		/**< CDMA Data Current NAI Index get confirm event type. */
#define	TAPI_EVENT_DATA_TE2_STATUS_GET_CNF			(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 3)		/**< CDMA Data TE2 Status get confirm event type. */
#define	TAPI_EVENT_DATA_BYTES_COUNTER_RESET_CNF		(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 4)		/**< CDMA Data Bytes Counter Reset confirm event type. */
#define	TAPI_EVENT_DATA_BYTES_COUNTER_GET_CNF		(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 5)		/**< CDMA Data Bytes Counter get confirm event type. */
#define	TAPI_EVENT_DATA_PACKET_DATA_CALL_CONFIGURATION_GET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 6)	/**<  CDMA Data Packet data call configuration get confirm event type. */
#define	TAPI_EVENT_DATA_HDR_SESSION_CLEAR_SET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 7)		/**< CDMA Data HDR session clear set confirm event type. */
#define	TAPI_EVENT_DATA_ROAM_GUARD_GET_CNF			(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 8)		/**< CDMA Roam guard get confirm event type. */
#define	TAPI_EVENT_DATA_ROAM_GUARD_SET_CNF			(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 9)		/**< CDMA Roam guard set confirm event type. */
#define	TAPI_EVENT_DATA_MODEM_NAI_GET_CNF			(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 10)	/**< CDMA Modem NAI get confirm event type. */
#define	TAPI_EVENT_DATA_EVDO_REV_CONFIG_GET_CNF		(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 11)	/**< CDMA Force RevA mode get confirm event type. */
#define	TAPI_EVENT_DATA_EVDO_REV_CONFIG_SET_CNF		(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 12)	/**< CDMA Force RevA mode set confirm event type. */
#define	TAPI_EVENT_DATA_NAI_SETTING_MODE_GET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 13)	/**< CDMA NAI setting mode get confirm event type. */
#define	TAPI_EVENT_DATA_NAI_SETTING_MODE_SET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 14)	/**< CDMA NAI setting mode set confirm event type. */
#define	TAPI_EVENT_DATA_RESTORE_NAI_SET_CNF			(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 15)	/**< CDMA Data Restore NAI set confirm event type. */
#define	TAPI_EVENT_DATA_DDTM_CONFIGURATION_GET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 16)	/**< CDMA Data DDTM Configuration get confirm event type. */
#define	TAPI_EVENT_DATA_DDTM_CONFIGURATION_SET_CNF	(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 17)	/**< CDMA Data DDTM Configuration set confirm event type. */
#define	TAPI_EVENT_DATA_INTERNAL_CNF_MAX			(TAPI_EVENT_CDMA_DATA_CONFIRMATION + 18)

#define	TAPI_EVENT_CDMA_DATA_NOTIFICATION			(TAPI_EVENT_TYPE_NOTIFICATION + (TAPI_EVENT_CLASS_CDMA_DATA<<8))
#define	TAPI_EVENT_DATA_TE2_STATUS_NOTI				(TAPI_EVENT_CDMA_DATA_NOTIFICATION + 0)		/**< CDMA Data TE2 Status changed indication event type. */
#define	TAPI_EVENT_DATA_DS_TE2_DATA_RATE_INFO_NOTI	(TAPI_EVENT_CDMA_DATA_NOTIFICATION + 1)		/**< CDMA Data DS TE2 Data Rate changed indication event type. */
#define	TAPI_EVENT_DATA_INTERNAL_IND_MAX			(TAPI_EVENT_CDMA_DATA_NOTIFICATION + 2)

/*==================================================================================================
 MACROS
 ==================================================================================================*/

/*==================================================================================================
 ENUMS
 ==================================================================================================*/

/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/**
 *This structure defines the Telephony Event.
 */
typedef struct {
	int EventClass; /**< Event Class */
	int EventType;  /**< Event type */
	int RequestId;  /**< This parameter is applicable only for Asynchronous solicited responses. The request id for a service request is sent back in the asynchronous response. For unsolicited indications, request id will be -1 */
	int Status;     /**< This parameter is applicable only for Asynchronous solicited responses. This status parameter informs the actual result of Asynchronous response. For unsolicited indications, status will be -1 */
	void *pData;    /**< Event Data - Based on Event type response/indications. Event data information is captured in each API Section */
	int pDataLen;   /**< Event Data size */
} TelTapiEvent_t;

typedef void (*TapiCallback)(int eventClass, int eventType, int param1, char *param2);

/**
 * This structure defines the Telephony Event .
 */
typedef struct {
	int eventClass; /**< Event class */
	int eventType;  /**< Event type */
	int param1;     /**< Event Data Length */
	void *param2;   /**< Event Data */
} TapiEvent;

/*	New Telephony Fw Event structure to be used with ts_delivery_power_display_event()	*/
typedef struct {
	int group;   // Event Class
	int type;    // Event Group
	char name[TAPI_EVENT_NAME_MAX_LEN]; // Event Name
	int len;     // Data Length
	void *data;  // Data
} tapi_event_type_t;

/**
 * prototype for TAPI Callback function.
 */
typedef int (*TelAppCallback)(const TelTapiEvent_t *pTapiEvent, void *data);

/**
 * this structure contains the necessary information on subscription list.
 */
typedef struct TapiSubscription {
	int SubscriptId; /**< Subscription ID */
	int EventClass; /**< Event Class */
	TelAppCallback AppCallback; /**< Callback function pointer */
	struct TapiSubscription * next; /**< Link to the next subscription */
} TapiSubscriptionList;

/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* _TAPI_EVENT_H_ */

/**
 *  @}
 */

