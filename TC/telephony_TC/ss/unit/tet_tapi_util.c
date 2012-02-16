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

#include <tet_api.h>
#include "tet_tapi_util.h"
#include "glib.h"

GMainLoop *nw_context_loop = NULL;

void G_MAIN_LOOP_CLOSE()
{
	g_main_loop_quit(nw_context_loop);
}

gboolean g_timeout_callback(void * data)
{
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}

int G_MAIN_LOOP_RUN(int sec)
{
	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_timeout_add_seconds(sec, g_timeout_callback, NULL);
	g_main_loop_run(nw_context_loop);

	return 0;
}
#if 0
#define TAPI_EVENT_CLASS_MAX TAPI_EVENT_CLASS_SAT+2


TAPI_EventClass_String g_TAPI_event_class_string[TAPI_EVENT_CLASS_MAX] = {	/* Tapi Call Related Event Types */
	{TAPI_EVENT_NONE,"TAPI_EVENT_NONE"},						/**< Event None */
	{TAPI_EVENT_CLASS_CALL_VOICE, "TAPI_EVENT_CLASS_CALL_VOICE"},	/**< 	Voice Call Event Class	*/
	{TAPI_EVENT_CLASS_CALL_DATA,"TAPI_EVENT_CLASS_CALL_DATA"},	/**< 	Data Call Event Class	*/
	{TAPI_EVENT_CLASS_NETTEXT,"TAPI_EVENT_CLASS_NETTEXT"},	/**<	Nettext (SMS/CB) Event Class	*/
	{TAPI_EVENT_CLASS_NETWORK,"TAPI_EVENT_CLASS_NETWORK"},	/**<	Network Event  Class	*/
	{TAPI_EVENT_CLASS_SS,"TAPI_EVENT_CLASS_SS"}, 			/**<	SS Event  Class	*/
	{TAPI_EVENT_CLASS_SIM,"TAPI_EVENT_CLASS_SIM"},			/**<	SIM Event  Class	*/
//	{TAPI_EVENT_CLASS_ERR,"TAPI_EVENT_CLASS_ERR"},			/**<	Fatal error Event  Class	*/
//	{TAPI_EVENT_CLASS_UTIL,"TAPI_EVENT_CLASS_UTIL"},			/**<	Utility Events  Class	*/
//	{TAPI_EVENT_CLASS_POWER,"TAPI_EVENT_CLASS_POWER"},		/**<	POWEREvents  Class	*/
//	{TAPI_EVENT_CLASS_DISPLAY,"TAPI_EVENT_CLASS_DISPLAY"},	/**<	DISPLAY Events  Class	*/
//	{TAPI_EVENT_CLASS_MISC,"TAPI_EVENT_CLASS_MISC"},			/**<	MISC Events  Class	*/
	{TAPI_EVENT_CLASS_DATA,"TAPI_EVENT_CLASS_DATA"},			/**<	DATA Events  Class	*/
	{TAPI_EVENT_CLASS_SOUND,"TAPI_EVENT_CLASS_SOUND"},		/**<	SOUND Events  Class	*/
	{TAPI_EVENT_CLASS_SAT,"TAPI_EVENT_CLASS_SAT"},			/**<	SAT Events  Class	*/
//	{TAPI_EVENT_CLASS_GPS,"TAPI_EVENT_CLASS_GPS"},			/**<GPS Events  Class	*/
//	{TAPI_EVENT_CLASS_ALL,"TAPI_EVENT_CLASS_ALL"},
};

#define TAPI_EVENT_MAX TAPI_EVENT_TYPE_MAX+1


TAPI_Event_String g_TAPI_EventString[TAPI_EVENT_MAX] = {
	/*	Events releated to Call -voice	 */
	{TAPI_EVENT_CALL_SETUP_CNF,"TAPI_EVENT_CLASS_CALL_VOICE"},  /**<0x0000,""},	This event indicates the Call Setup request triggred */
	{TAPI_EVENT_CALL_ALERT_IND,"TAPI_EVENT_CALL_ALERT_IND"},			/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_CONNECTING_IND,"TAPI_EVENT_CALL_CONNECTING_IND"}, 	/**<  	This event indicates the Call is in Connecting stage*/
	{TAPI_EVENT_CALL_CONNECTED_IND,"TAPI_EVENT_CALL_CONNECTED_IND"},   	/**<      	This event indicates the Call is Connected */
	{TAPI_EVENT_CALL_INCOM_IND    ,"TAPI_EVENT_CALL_INCOM_IND"},		/**<    	This event indicates Incoming Call indication to Application*/
	{TAPI_EVENT_CALL_ANSWER_CNF," TAPI_EVENT_CALL_ANSWER_CNF"},       	/**<	Incoming call was answered*/
	{TAPI_EVENT_CALL_RETRIEVE_CNF,"TAPI_EVENT_CALL_RETRIEVE_CNF"},	/**<	This event indicates Call is retrieved from held state*/
	{TAPI_EVENT_CALL_RETRIEVE_IND,"TAPI_EVENT_CALL_RETRIEVE_IND"},   	/**<	This event indicates that other party has retrieved the call from hold*/
	{TAPI_EVENT_CALL_HOLD_IND,"TAPI_EVENT_CALL_HOLD_IND"},		/**<	This event indicates that other party is has put the call on hold*/
	{TAPI_EVENT_CALL_HOLD_CNF ," TAPI_EVENT_CALL_HOLD_CNF"},		/**< 	This event indicates is call is put on hold*/
	{TAPI_EVENT_CALL_TRANSFER_CNF,"TAPI_EVENT_CALL_TRANSFER_CNF"},	/**<	This event indicates is call is explicitly transferred*/
	{TAPI_EVENT_CALL_TRANSFER_IND,"TAPI_EVENT_CALL_TRANSFER_IND"},	/**<	Call Transfer Indication*/
	{TAPI_EVENT_CALL_SETUPCONFERENCE_CNF,"TAPI_EVENT_CALL_SETUPCONFERENCE_CNF"},		/**<	This event indicates that Call is joined in a conference*/
	{TAPI_EVENT_CALL_SETUPCONFERENCE_IND,"TAPI_EVENT_CALL_SETUPCONFERENCE_IND"},		/**<	Conference setup indication*/
	{TAPI_EVENT_CALL_SPLITCONFERENCE_CNF,"TAPI_EVENT_CALL_SPLITCONFERENCE_CNF"},		/**<	This event indicates Call is split from conference*/
	{TAPI_EVENT_CALL_SEND_DTMF_CNF," TAPI_EVENT_CALL_SEND_DTMF_CNF"}, 	/**<	This event indicates the confirmation for sending DTMF Digits to the network*/
	{TAPI_EVENT_CALL_WAITING_IND,"TAPI_EVENT_CALL_WAITING_IND"},			/**<	Call waiting indication*/
	{TAPI_EVENT_CALL_FORWARD_IND,"TAPI_EVENT_CALL_FORWARD_IND"},		/**<	This event indication for Call is forwarded */
//	{TAPI_EVENT_CALL_USERINFO_IND,"TAPI_EVENT_CALL_USERINFO_IND"},		/**<	This event is indicated when User Information is exchanged during the call */
	{TAPI_EVENT_CALL_RELEASE_CNF,"TAPI_EVENT_CALL_RELEASE_CNF"},		/**<	This event is indicated when a specific call is released requested by the application*/
	{TAPI_EVENT_CALL_RELEASE_ALL_CNF,"TAPI_EVENT_CALL_RELEASE_ALL_CNF"},	/**<	This event is indicated when all the calls released requested by the application*/
	{TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF,"TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF"},	/**<	This event is indicated when all the active calls released requested by the application*/
	{TAPI_EVENT_CALL_END_IND,"TAPI_EVENT_CALL_END_IND"},			 /**<	This event indicates the Call is Ended. This event can be solicited/unsolicited.*/
	{TAPI_EVENT_CALL_GET_LINEID_CNF," TAPI_EVENT_CALL_GET_LINEID_CNF"}, 	/**<	Error in Get Line ID	*/
	{TAPI_EVENT_CALL_SET_LINEID_CNF,"TAPI_EVENT_CALL_SET_LINEID_CNF"}, /**< 		Error in Set Line Id		*/
	{TAPI_EVENT_CALL_SWAP_CNF," TAPI_EVENT_CALL_SWAP_CNF"}, /**< Call Swap Confirmation */

	{TAPI_EVENT_CALL_BARRING_IND,"TAPI_EVENT_CALL_BARRING_IND"},			/**<	This event indicates the Call is Barred*/
	{TAPI_EVENT_CALL_CUGINFO_IND,"TAPI_EVENT_CALL_CUGINFO_IND"},		/**<	This event indicates CUG information */
	{TAPI_EVENT_CALL_AOCINFO_IND,"TAPI_EVENT_CALL_AOCINFO_IND"},	/**<	This event indicates the AOC Information*/
//	{TAPI_EVENT_CALL_SSNOTIFY_IND,"TAPI_EVENT_CALL_SSNOTIFY_IND"},	/**<	This event indicates SS Notification*/
	{TAPI_EVENT_CALL_CALLINGNAMEINFO_IND,"TAPI_EVENT_CALL_CALLINGNAMEINFO_IND"},	/**<	This event indicates the Calling Name Information*/
	{TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND,"TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND"},	/**<	This event indicates the Connected Number Information*/
	{TAPI_EVENT_CALL_CLISUPRESSIONREJ_IND,"TAPI_EVENT_CALL_CLISUPRESSIONREJ_IND"},	/**<	This event indicates the CLI Suppression Indication*/
	{TAPI_EVENT_CALL_UNCOND_FORWARDING_IND,"TAPI_EVENT_CALL_UNCOND_FORWARDING_IND"},	/**<	This event indicates the Call is unconditionally forwarded*/
	{TAPI_EVENT_CALL_COND_FORWARDING_IND,"TAPI_EVENT_CALL_COND_FORWARDING_IND"},	/**<	This event indicates the Call is conditionally forwarded*/
	{TAPI_EVENT_CALL_CONNECTED_ECT_REMOTE_IND," TAPI_EVENT_CALL_CONNECTED_ECT_REMOTE_IND"},	/**<	This event indicates the ECT remote connection  */
	{TAPI_EVENT_CALL_ON_HOLD_RELEASED_IND,"TAPI_EVENT_CALL_ON_HOLD_RELEASED_IND"},	/**<	This event indicates the Call on hold is released*/
	{TAPI_EVENT_CALL_ALERT_ECT_REMOTE_IND," TAPI_EVENT_CALL_ALERT_ECT_REMOTE_IND"},	/**<	This event indicates the ECT remote alert*/
	{TAPI_EVENT_CALL_SS_FORWARDED_MSG_RXED_IND,"TAPI_EVENT_CALL_SS_FORWARDED_MSG_RXED_IND"},	/**<	This event indicates that the forwarded msg has been received*/

	/*	Events releated to Call 	video */
	{TAPI_EVENT_CALL_SETUP_CNF," TAPI_EVENT_DATA_SETUP_CNF"}, 		/**<0x0100,""},	This event indicates the Call Setup request triggred */
	{TAPI_EVENT_CALL_ALERT_IND," TAPI_EVENT_DATA_ALERT_IND"},		/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_CONNECTED_IND,"TAPI_EVENT_DATA_CONNECTED_IND"},  	/**<      	This event indicates the Call is Connected */
	{TAPI_EVENT_CALL_INCOM_IND    ,"{TAPI_EVENT_DATA_INCOM_IND"},		/**<    	This event indicates Incoming Call indication to Application*/
	{TAPI_EVENT_CALL_ANSWER_CNF,"TAPI_EVENT_DATA_ANSWER_CNF"},       	/**<	Incoming call was answered*/
	{TAPI_EVENT_CALL_FORWARD_IND," TAPI_EVENT_DATA_FORWARD_IND"},		/**<	This event indication for Call is forwarded */
	{TAPI_EVENT_CALL_RELEASE_CNF,"TAPI_EVENT_DATA_RELEASE_CNF"},		/**<	This event is indicated when all the calls released requested by the application*/
	{TAPI_EVENT_CALL_END_IND," TAPI_EVENT_DATA_END_IND"},			 /**<	This event indicates the Call is Ended. This event can be solicited/unsolicited.*/

	{TAPI_EVENT_CALL_BARRING_IND,"TAPI_EVENT_DATA_BARRING_IND"},			/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_AOCINFO_IND,"TAPI_EVENT_DATA_AOCINFO_IND"},	/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND," TAPI_EVENT_DATA_CONNECTEDNUMBERINFO_IND"},	/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_UNCOND_FORWARDING_IND," TAPI_EVENT_DATA_UNCOND_FORWARDING_IND"},	/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_COND_FORWARDING_IND," TAPI_EVENT_DATA_COND_FORWARDING_IND"},	/**<	This event indicates the Call is in Alerting stage*/
	{TAPI_EVENT_CALL_CONNECTED_ECT_REMOTE_IND,"TAPI_EVENT_DATA_CONNECTED_ECT_REMOTE_IND"},	/**<	This event indicates the Call is in Alerting stage*/


	/* Events related on Nettext Messages  */
	{TAPI_EVENT_NETTEXT_READ_SMS_CNF,"TAPI_EVENT_CLASS_NETTEXT"},		/**<0x0200,""},		This event is returned in response to SMS Read message*/
	{TAPI_EVENT_NETTEXT_GET_COUNT_CNF,"TAPI_EVENT_NETTEXT_GET_COUNT_CNF"},		/**<	This event is returned in response to Get Number of messages stored in SIM*/
	{TAPI_EVENT_NETTEXT_SENTSTATUS_CNF,"TAPI_EVENT_NETTEXT_SENTSTATUS_CNF"},		/**<	This event is returned in response to SMS  message sent*/
	{TAPI_EVENT_NETTEXT_GET_PARAM_CNF,"TAPI_EVENT_NETTEXT_GET_PARAM_CNF"},		/**<	This event is returned in response to NetText Get parameters*/
	{TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF," TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF"},	/**<	This confirmation event is a returned indicating the delete message e status*/
	{TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF,"TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF"},	/**<	This confirmation event is a returned indicating the Save SMS into SIM status*/
	{TAPI_EVENT_NETTEXT_SET_REQUEST_CNF," TAPI_EVENT_NETTEXT_SET_REQUEST_CNF"},	/**<	This event is returned as a confirmation for SET nettext APIs*/
	{TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF," TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF"}, 	/**<	This event is returned with Get CB configuration details*/
	{TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF,"TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF"},	/**<	This event is returned as a response to get preferred bearer used for SMS Transport*/
	{TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF,"TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF"},/**<	This event is returned in response to send delivery report*/
	{TAPI_EVENT_NETTEXT_INCOM_IND,"TAPI_EVENT_NETTEXT_INCOM_IND"},			/**<	This is an unsolicited Incoming SMS Event*/
	{TAPI_EVENT_NETTEXT_CB_INCOM_IND,"TAPI_EVENT_NETTEXT_CB_INCOM_IND"},		/**<	This is an unsolicited Incoming cell broadcast message*/
	{TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND,"TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND"},	/**<	This event is an unsolicited event indicating SIM memory status */
	{TAPI_EVENT_NETTEXT_DEVICE_READY_IND,"TAPI_EVENT_NETTEXT_DEVICE_READY_IND"}, /**< Device Ready indication */
	{TAPI_EVENT_NETTEXT_PARAM_COUNT_IND,"TAPI_EVENT_NETTEXT_PARAM_COUNT_IND"},  /**< Param count indication */
	/* Events related on Network Messages  */
	{TAPI_EVENT_NETWORK_GETNWBAND_CNF,"TAPI_EVENT_NETWORK_GETNWBAND_CNF"},		/**<0x0300,""},	This event indicates the Network Band has been identified and returned*/
	{TAPI_EVENT_NETWORK_SETNWBAND_CNF,"TAPI_EVENT_NETWORK_SETNWBAND_CNF"},  	/**<	This event indicates the Network Band has been set*/
	{TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF," TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF"},	/**<	This event confirms the request to get the service domain */
	{TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF,"TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF"},	/**<	This event confirms the request to set the service domain */
//	{TAPI_EVENT_NETWORK_CANCELMANUALSEARCH_CNF,"TAPI_EVENT_NETWORK_CANCELMANUALSEARCH_CNF"},	/**<	This event is returned on cancellation of manual network search*/
	{TAPI_EVENT_NETWORK_GETNWMODE_CNF," TAPI_EVENT_NETWORK_GETNWMODE_CNF"},/**<	This event is returned on getting network mode*/
	{TAPI_EVENT_NETWORK_SETNWMODE_CNF,"TAPI_EVENT_NETWORK_SETNWMODE_CNF"},	/**<	This event is returned on setting network mode*/
	{TAPI_EVENT_NETWORK_SELECT_CNF," TAPI_EVENT_NETWORK_SELECT_CNF"},	/**<	This confirmation event is returned when user manually selected a network from list for Network selection */
	{TAPI_EVENT_NETWORK_SEARCH_CNF," TAPI_EVENT_NETWORK_SEARCH_CNF"},	/**<	This event is returned as confirmation for manual Network search with the available network list*/
	{TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF,"TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF"},	/**<	This event is returned on getting network selection mode*/
//	{TAPI_EVENT_NETWORK_GETNETWORKINFO_CNF,"TAPI_EVENT_NETWORK_GETNETWORKINFO_CNF"},	/**<	This event is returned when Get network information is requested*/
	{TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF," TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF"},	/**<	This event indicates that the PLMN used has been identified and returned */
	{TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF,"TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF"},	/**<	This event indicates the preferred PLMN has been set*/
	{TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND,"TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND"},	/**<	This event indicates the LAC and CELLID has been obtained*/
//	{TAPI_EVENT_NETWORK_IDENTITY_IND,"TAPI_EVENT_NETWORK_IDENTITY_IND"},	/**<	This event indicates the Network ID has been detected*/
	{TAPI_EVENT_NETWORK_CHANGE_IND," TAPI_EVENT_NETWORK_CHANGE_IND"},	/**<	This event indicates there is a change in Network*/
//	{TAPI_EVENT_NETWORK_SELECT_IND," TAPI_EVENT_NETWORK_SELECT_IND"},	/**<	This event indicates the chosen network has been selected*/
//	{TAPI_EVENT_NETWORK_SEARCH_IND," TAPI_EVENT_NETWORK_SEARCH_IND"},	/**<	This event indicates that available networks are being searched for */
//	{TAPI_EVENT_NETWORK_RADIO_INFO_IND," TAPI_EVENT_NETWORK_RADIO_INFO_IND"},	/**<	This event indicates the Radio Information is got*/
//	{TAPI_EVENT_NETWORK_PLMNLIST_INFO,"TAPI_EVENT_NETWORK_PLMNLIST_INFO"},	/**<	This event indicates the list of available PLMN's is got*/
//	{TAPI_EVENT_NETWORK_SPN_INFO,"TAPI_EVENT_NETWORK_SPN_INFO"},	/**<	This event indicates the SPN info is got*/
//	{TAPI_EVENT_NETWORK_DISPLAYINFO_IND," TAPI_EVENT_NETWORK_DISPLAYINFO_IND"},	/**<	This event indicates the Display Information is obtained*/

	/* Events Related on Supplementary services	 */
//	{TAPI_EVENT_SS_BARRING_CNF, "TAPI_EVENT_CLASS_SS"},   /**<0x0400," "},	This confirmation event is sent when Barring activation/deactivation is requested*/
	{TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF," TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF"},   /**<	This event is returned as Barring password change confirmation*/
	{TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF," TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF"},   /**<	This event is returned  as Barring status query confirmation*/
	{TAPI_EVENT_SS_FORWARD_CNF," TAPI_EVENT_SS_FORWARD_CNF"},   /**<	This event is returned as confirmation for Forward Activation/Deactivation/Registration/Erasure */
	{TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF,"TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF "},   /**<	This event is returned as confirmation for Forward Status query confirmation*/
	{TAPI_EVENT_SS_WAITING_CNF,"TAPI_EVENT_SS_WAITING_CNF "},   /**<	This event is returned as confirmation for Call waiting activation/deactivation*/
	{TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF,"TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF "},   /**<	This event is returned as Call waiting status query confirmation*/
	{TAPI_EVENT_SS_USSD_CNF,"TAPI_EVENT_SS_USSD_CNF "},   /**<	This event is returned as USSD send confirmation*/
//	{TAPI_EVENT_SS_QUERYCAPABILITY_INFO_CNF,"TAPI_EVENT_SS_QUERYCAPABILITY_INFO_CNF "},   /**<	Event associated with the interrogation of supported supplementary services at OEM.*/
	{TAPI_EVENT_SS_USSD_IND," TAPI_EVENT_SS_USSD_IND"},   /**<	This is an unsolicited USSD indication*/
//	{TAPI_EVENT_SS_GET_PASSWORD_IND," TAPI_EVENT_SS_GET_PASSWORD_IND"},   /**<	This event is indicated to the Application when network requests for Call barring Get password for barring operation*/
//	{TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF,"TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF "},   /**<	This event is indicated to the Application for CLI service interrogation*/
//	{TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY,"TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY "}, /**<	This event is indicated to the Application when tapi receives Release complete message notiification for any SS transactions*/
//	{TAPI_EVENT_SS_ERROR_IND," TAPI_EVENT_SS_ERROR_IND"},				/**<This event is used to indicate ss related errors sent by the network*/
//	{TAPI_EVENT_SS_AOC_RSP," TAPI_EVENT_SS_AOC_RSP"},				/**<This event is used to send the aoc information */
//	{TAPI_EVENT_SS_SET_AOC_CNF,"TAPI_EVENT_SS_SET_AOC_CNF "},			/**<This event is used to send the confirmation for setting aoc values */
//	{TAPI_EVENT_SS_BARRING_TYPE_IND,"TAPI_EVENT_SS_BARRING_TYPE_IND "},		/**<This event is used to send the barring type while making an MO call */

	/* Events Related on SIM services	 */
//	{TAPI_EVENT_SIM_FILE_INFO_CNF, "TAPI_EVENT_CLASS_SIM "},	/**<0x0500," "},	  File info */
//	{TAPI_EVENT_SIM_READ_FILE_CNF," TAPI_EVENT_SIM_READ_FILE_CNF"},	/**< Read file */
//	{TAPI_EVENT_SIM_UPDATE_FILE_CNF," TAPI_EVENT_SIM_UPDATE_FILE_CNF"},	/**< Updated file */
//	{TAPI_EVENT_SIM_VERIFY_PIN_CNF," TAPI_EVENT_SIM_VERIFY_PIN_CNF"},	/**< Verified Chv */
//	{TAPI_EVENT_SIM_CHANGE_PIN_CNF,"TAPI_EVENT_SIM_CHANGE_PIN_CNF "},	/**< Changed Chv */
//	{TAPI_EVENT_SIM_DISABLE_PIN_CNF," TAPI_EVENT_SIM_DISABLE_PIN_CNF"},	/**< Disabled Chv */
//	{TAPI_EVENT_SIM_ENABLE_PIN_CNF,"TAPI_EVENT_SIM_ENABLE_PIN_CNF "},	/**< Enabled Chv */
//	{TAPI_EVENT_SIM_UNBLOCK_PIN_CNF,"TAPI_EVENT_SIM_UNBLOCK_PIN_CNF "},	/**< Unblocked Chv */
//	{TAPI_EVENT_SIM_DISABLE_FDN_CNF,"TAPI_EVENT_SIM_DISABLE_FDN_CNF "},	/**< Disabled Fdn */
//	{TAPI_EVENT_SIM_ENABLE_FDN_CNF,"TAPI_EVENT_SIM_ENABLE_FDN_CNF "},	/**< Enabled Fdn */

//	{TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF," TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF"}, /**<Phonebook storage count*/
//	{TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF," TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF"},        /**<Phonebook entry information*/
//	{TAPI_EVENT_SIM_PB_ACCESS_READ_CNF," TAPI_EVENT_SIM_PB_ACCESS_READ_CNF"},     /**<Phonebook access read*/

//	{TAPI_EVENT_SIM_PB_UPDATE_CNF," TAPI_EVENT_SIM_PB_UPDATE_CNF"},       /**<Phonebook update result*/
//	{TAPI_EVENT_SIM_PB_DELETE_CNF,"TAPI_EVENT_SIM_PB_DELETE_CNF "},       /**<Phonebook delete result*/
//	{TAPI_EVENT_SIM_STATUS_IND,"TAPI_EVENT_SIM_STATUS_IND "},         /**<Unsolicited event --- whenever there is a changes in SIM status*/
//	{TAPI_EVENT_SIM_FDN_STATUS_IND,"TAPI_EVENT_SIM_FDN_STATUS_IND "},         /**<Unsolicited event --- whenever there is a changes in FDN status */

	/* Extensions */
	{TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF,"TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF "}, /** < 3G SIM PB capability Info */
	{TAPI_EVENT_SIM_AUTHENTICATION_CNF,"TAPI_EVENT_SIM_AUTHENTICATION_CNF "}, /**< ISIM authentication confirmation*/
	{TAPI_EVENT_SIM_SAP_CONNECT_NOTI,"TAPI_EVENT_SIM_SAP_CONNECT_NOTI "}, /**<SAP connect notification */
	{TAPI_EVENT_SIM_SAP_CONNECT_CNF,"TAPI_EVENT_SIM_SAP_CONNECT_CNF "}, /**< SAP connect confirmation */
	{TAPI_EVENT_SIM_SAP_CONNECT_STATUS_CNF," TAPI_EVENT_SIM_SAP_CONNECT_STATUS_CNF"}, /**< SAP connection status confirmation*/
	{TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF," TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF"}, /**< SAP transfer ATR confirmation */
	{TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF," TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF"}, /**< SAP transfer APDU confirmation */
	{TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF," TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF"}, /**< SAP set protocaol confirmation */
	{TAPI_EVENT_SIM_SAP_SET_SIM_POWER_CNF," TAPI_EVENT_SIM_SAP_SET_SIM_POWER_CNF"}, /**< SAP set sim power confirmation */
	{TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF," TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF"}, /**< SAP card reader status confirmation*/
//	{TAPI_EVENT_SIM_ENABLE_PERSONALIZATION_CNF,"TAPI_EVENT_SIM_ENABLE_PERSONALIZATION_CNF "}, /**< Enable personalization confirmation*/
//	{TAPI_EVENT_SIM_DISABLE_PERSONALIZATION_CNF," TAPI_EVENT_SIM_DISABLE_PERSONALIZATION_CNF"}, /**<Disable personalization confirmation*/

	/* Error Class Events*/
	/*	fatal Error Event*/
	//{TAPI_EVENT_ERR,"TAPI_EVENT_CLASS_ERR "},		/**0x0600," "},   <  Fatal Error Information */
//	{TAPI_EVENT_ERR_MODEM_OFF," TAPI_EVENT_ERR_MODEM_OFF"},
//	{TAPI_EVENT_ERR_MODEM_RESET," TAPI_EVENT_ERR_MODEM_RESET"},
//	{TAPI_EVENT_ERR_RESERVED1_IND ,"TAPI_EVENT_ERR_RESERVED1_IND "},	/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
//	{TAPI_EVENT_ERR_RESERVED2_IND," TAPI_EVENT_ERR_RESERVED2_IND"},	/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
//	{TAPI_EVENT_ERR_RESERVED3_IND," TAPI_EVENT_ERR_RESERVED3_IND"},	/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
//	{TAPI_EVENT_ERR_RESERVED4_IND," TAPI_EVENT_ERR_RESERVED4_IND"},	/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */
//	{TAPI_EVENT_ERR_RESERVED5_IND," TAPI_EVENT_ERR_RESERVED5_IND"},	/**< This event is notified to application about some error has occurred at OEM and can be used for debugging purpose */

    /*	Power Module Event Types...	*/
//	{TAPI_EVENT_POWER_PHONE_OFF, "TAPI_EVENT_CLASS_POWER"},			/**0x0800," "},  <  Modem powerd Off	*/
	/*	Display Module Event Types...	*/
//	{TAPI_EVENT_DISPLAY_ICON_INFO_IND, "TAPI_EVENT_CLASS_DISPLAY"},		/**0x0900," "}, <  Display Icon Info Indication */
//	{TAPI_EVENT_DISPLAY_HMZ_O2_IND," TAPI_EVENT_DISPLAY_HMZ_O2_IND"},				/**<  Home Zone Info Indication */

//	{TAPI_EVENT_MISC_NITZ_GETCNF, "TAPI_EVENT_CLASS_MISC"}, /**< 0x0A00," "}, */
//	{TAPI_EVENT_MISC_NITZ_NOTIFY,"TAPI_EVENT_MISC_NITZ_NOTIFY "},      /**<     */
//	{TAPI_EVENT_MISC_NITZ_IND," TAPI_EVENT_MISC_NITZ_IND"},      /**<     */

	/*	PACKET CALL Events 	*/
	{TAPI_EVENT_PS_PDP_ACT_RSP , "API_EVENT_CLASS_DATA"}, 	/**< 0x0B00," "},   -  PDP activation response event */
	{TAPI_EVENT_PS_PDP_DEACT_RSP,"TAPI_EVENT_PS_PDP_DEACT_RSP "},				/**<      PDP deactivation response event */
	{TAPI_EVENT_PS_PDP_DEACT_IND," TAPI_EVENT_PS_PDP_DEACT_IND"},			/**<     PDP deactivation indication event. */
	{TAPI_EVENT_PS_STATUS,"TAPI_EVENT_PS_STATUS "},			/**<     PDP status event. */
	{TAPI_EVENT_PS_CALL_STATISTICS," TAPI_EVENT_PS_CALL_STATISTICS"},		/**<     publish notification or response of the call statistics  */
//	{TAPI_EVENT_PS_PDP_CONTEXTINFO_RSP," TAPI_EVENT_PS_PDP_CONTEXTINFO_RSP"},       /**<     */
//	{TAPI_EVENT_PS_CALL_STATUS_RSP," TAPI_EVENT_PS_CALL_STATUS_RSP"},      /**<     */
//	{TAPI_EVENT_PS_PDP_ADDRESS_RSP," TAPI_EVENT_PS_PDP_ADDRESS_RSP"},      /**<     */
//	{TAPI_EVENT_PS_QOS_PROFILE_RSP,"TAPI_EVENT_PS_QOS_PROFILE_RSP "},      /**<     */
//	{TAPI_EVENT_PS_QOS_3G_PROFILE_RSP,"TAPI_EVENT_PS_QOS_3G_PROFILE_RSP "},      /**<     */
//	{TAPI_EVENT_PS_NETWORK_STATUS_RSP," TAPI_EVENT_PS_NETWORK_STATUS_RSP"},      /**<     */
//	{TAPI_EVENT_PS_HSDPA_STATUS_NOTI," TAPI_EVENT_PS_HSDPA_STATUS_NOTI"},      /**<     */
//	{TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI,"TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI "},      /**<     */
	{TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO," TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO"},      /**<     */
	{TAPI_EVENT_PS_ERROR_IND," TAPI_EVENT_PS_ERROR_IND"},      /**<     */

	{TAPI_EVENT_SOUND_VOLUMECTRL_RSP, "TAPI_EVENT_CLASS_SOUND"}, 	      /**<     */
//	{TAPI_EVENT_SOUND_USRCONFIGURATIONCTRL_RSP," TAPI_EVENT_SOUND_USRCONFIGURATIONCTRL_RSP"},      /**<     */
	{TAPI_EVENT_SOUND_ERROR_IND," TAPI_EVENT_SOUND_ERROR_IND"},      /**<     */
	{TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI," TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI"},      /**<     */
	{TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI," TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI"},      /**<     */
	{TAPI_EVENT_SOUND_MICMUTECTRL_RSP,"TAPI_EVENT_SOUND_MICMUTECTRL_RSP "},      /**<     */
//	{TAPI_EVENT_SOUND_MICGAINCTRL_RSP,"TAPI_EVENT_SOUND_MICGAINCTRL_RSP "},      /**<     */

/*SAT events*/
//	{TAPI_EVENT_SAT_DISPLAY_TEXT_IND, "TAPI_EVENT_CLASS_SAT"},	/**< 0x0D00," "}, SAT Dsiplay Text Event*/
//	{TAPI_EVENT_SAT_GET_INKEY_IND," TAPI_EVENT_SAT_GET_INKEY_IND"},                                       /**< SAT Get Inkey Event */
//	{TAPI_EVENT_SAT_GET_INPUT_IND," TAPI_EVENT_SAT_GET_INPUT_IND"},                                     /**< SAT Get Input Event */
//	{TAPI_EVENT_SAT_UI_PLAY_TONE_IND,"TAPI_EVENT_SAT_UI_PLAY_TONE_IND "},                              /**< SAT Play Tone Event for UI client*/
//	{TAPI_EVENT_SAT_PLAY_TONE_IND ," TAPI_EVENT_SAT_PLAY_TONE_IND"},                                     /**< SAT Play Tone Event for MM client */
  //  	{TAPI_EVENT_SAT_UI_REFRESH_IND," TAPI_EVENT_SAT_UI_REFRESH_IND"},                                     /**< SAT Refresh Event for UI client*/
//	{TAPI_EVENT_SAT_SETUP_MENU_IND," TAPI_EVENT_SAT_SETUP_MENU_IND"},                                 /**< SAT Setup Menu Event */
//	{TAPI_EVENT_SAT_SELECT_ITEM_IND," TAPI_EVENT_SAT_SELECT_ITEM_IND"},                                   /**< SAT Select Item Event */
//	{TAPI_EVENT_SAT_UI_SEND_SMS_IND,"TAPI_EVENT_SAT_UI_SEND_SMS_IND "},                                                /**< SAT Send SMS Event for UI client */
//	{TAPI_EVENT_SAT_UI_SEND_SS_IND," TAPI_EVENT_SAT_UI_SEND_SS_IND"},                                  /**< SAT Send SS Event for UI client */
//	{TAPI_EVENT_SAT_UI_SEND_USSD_IND,"TAPI_EVENT_SAT_UI_SEND_USSD_IND "},                              /**< SAT Send USSD Event for UI client */
//	{TAPI_EVENT_SAT_UI_SETUP_CALL_IND,"TAPI_EVENT_SAT_UI_SETUP_CALL_IND "},                             /**< SAT Setup Call Event for UI client */
//	{TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND," TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND"}, /**< SAT Provide Local Information Event*/
//	{TAPI_EVENT_SAT_SETUP_IDLE_MODE_TEXT_IND," TAPI_EVENT_SAT_SETUP_IDLE_MODE_TEXT_IND"}, /**< SAT Setup Idle Mode Event*/
//	{TAPI_EVENT_SAT_UI_SEND_DTMF_IND," TAPI_EVENT_SAT_UI_SEND_DTMF_IND"},         /**< SAT Send DTMF Event for UI client*/
//	{TAPI_EVENT_SAT_LANGUAGE_NOTIFICATION_IND,"TAPI_EVENT_SAT_LANGUAGE_NOTIFICATION_IND "},      /**< SAT Language notification Event */
//	{TAPI_EVENT_SAT_UI_LAUNCH_BROWSER_IND," TAPI_EVENT_SAT_UI_LAUNCH_BROWSER_IND"},             /**< SAT Launch Browser Event for UI client*/
//	{TAPI_EVENT_SAT_OPEN_CHANNEL_LOCAL_LINK_IND," TAPI_EVENT_SAT_OPEN_CHANNEL_LOCAL_LINK_IND"},
//	{TAPI_EVENT_SAT_OPEN_CHANNEL_CSB_IND,"TAPI_EVENT_SAT_OPEN_CHANNEL_CSB_IND "},
//	{TAPI_EVENT_SAT_OPEN_CHANNEL_GPRS_IND," TAPI_EVENT_SAT_OPEN_CHANNEL_GPRS_IND"},
//	{TAPI_EVENT_SAT_CLOSE_CHANNEL_IND," TAPI_EVENT_SAT_CLOSE_CHANNEL_IND"},
//	{TAPI_EVENT_SAT_RECEIVE_DATA_IND," TAPI_EVENT_SAT_RECEIVE_DATA_IND"},
//	{TAPI_EVENT_SAT_SEND_DATA_IND," TAPI_EVENT_SAT_SEND_DATA_IND"},
//	{TAPI_EVENT_SAT_GET_CHANNEL_STATUS_IND," TAPI_EVENT_SAT_GET_CHANNEL_STATUS_IND"},
//	{TAPI_EVENT_SAT_END_PROACTIVE_SESSION_IND," TAPI_EVENT_SAT_END_PROACTIVE_SESSION_IND"},       /**< SAT End Proactive Command Session Event. This is to indicate that Proactive UICC Session ended*/
	{TAPI_EVENT_SAT_MENU_SELECTION_CNF,"TAPI_EVENT_SAT_MENU_SELECTION_CNF "},      /**< response from sim for menu selection request */
	{TAPI_EVENT_SAT_CALL_CONTROL_IND," TAPI_EVENT_SAT_CALL_CONTROL_IND"},	            /**< response from sim for call control by USIM. This is to indicate that the voice call/ss/ussd parameters have been modified by USIM  */
	{TAPI_EVENT_SAT_MO_SMS_CONTROL_IND," TAPI_EVENT_SAT_MO_SMS_CONTROL_IND"},               /**< response from sim for Mo sms control bu USIM. This is to indicate that the SMS parameters have been modified by USIM */
	{TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF,"TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF "},               /**< response from sim for event download request */

	/**EVENTS SENT TO OTHER APPS**/
	{TAPI_EVENT_SAT_SMS_REFRESH_IND," TAPI_EVENT_SAT_SMS_REFRESH_IND"},      /**< SAT Refresh Event for SMS client */
	{TAPI_EVENT_SAT_PB_REFRESH_IND," TAPI_EVENT_SAT_PB_REFRESH_IND"},        /**< SAT Refresh Event for Phonebook client*/
	{TAPI_EVENT_SAT_IDLE_REFRESH_IND,"TAPI_EVENT_SAT_IDLE_REFRESH_IND "}, /**< SAT Refresh Event for Idle Mode client */
	{TAPI_EVENT_SAT_SEND_SMS_IND," TAPI_EVENT_SAT_SEND_SMS_IND"}, /**< SAT Send SMS Event for SMS client*/
	{TAPI_EVENT_SAT_SETUP_CALL_IND," TAPI_EVENT_SAT_SETUP_CALL_IND"},        /**< SAT Setup Call Event for Call client */
	{TAPI_EVENT_SAT_SEND_SS_IND,"TAPI_EVENT_SAT_SEND_SS_IND "},        /**< SAT Send SS Event for SS client*/
	{TAPI_EVENT_SAT_SEND_USSD_IND," TAPI_EVENT_SAT_SEND_USSD_IND"},        /**< SAT Send SS Event for USSD client */
	{TAPI_EVENT_SAT_SEND_DTMF_IND,"TAPI_EVENT_SAT_SEND_DTMF_IND "},      /**< SAT Send DTMF Event for UI client*/
	{TAPI_EVENT_SAT_LAUNCH_BROWSER_IND," TAPI_EVENT_SAT_LAUNCH_BROWSER_IND"},   /**< SAT Launch Browser Event for Browser */
	{TAPI_EVENT_SAT_END_OF_APP_EXEC_IND," TAPI_EVENT_SAT_END_OF_APP_EXEC_IND"},    /**< SAT End Proactive command Execution Event. This event will be indicated to the application to end the execution of a Proactive Command when required  */


	//{TAPI_GPS_EVENT_START_SESSION_RESP, "TAPI_EVENT_CLASS_GPS"},		/**<0x0F00," "}, 	Start session response event type. 		*/
	//{TAPI_GPS_EVENT_OPTION_SET_RESP,"TAPI_GPS_EVENT_OPTION_SET_RESP "},			/**<	GPS option set response  event type. 		*/

//	{TAPI_GPS_EVENT_CLOSE_SESSION_RESP,"TAPI_GPS_EVENT_CLOSE_SESSION_RESP "},				/**<	Close session response event type */

//	{TAPI_GPS_EVENT_AGPS_SSL_SET_RESP," TAPI_GPS_EVENT_AGPS_SSL_SET_RESP"},      /**<     */
//	{TAPI_GPS_EVENT_AGPS_BEARER_SET_RESP," TAPI_GPS_EVENT_AGPS_BEARER_SET_RESP"},      /**<     */

//	{TAPI_GPS_EVENT_AGPS_PDP_CONNECTION_IND," TAPI_GPS_EVENT_AGPS_PDP_CONNECTION_IND"},			/**<	. 		*/

//	{TAPI_GPS_EVENT_AGPS_SSL_GET_RESPONSE," TAPI_GPS_EVENT_AGPS_SSL_GET_RESPONSE"},      /**<     */

//	{TAPI_GPS_EVENT_DEVSTATE_NOTI,"TAPI_GPS_EVENT_DEVSTATE_NOTI "},						/**<	Device state notification event type . 		*/
//	{TAPI_GPS_EVENT_DEVSTATE_RESP," TAPI_GPS_EVENT_DEVSTATE_RESP"},						/**<	Device state response type. 		*/
//	{TAPI_GPS_EVENT_VERIFICATION_IND," TAPI_GPS_EVENT_VERIFICATION_IND"},					/**<	Verification indication event type. 		*/
//	{TAPI_GPS_EVENT_SUPL_NOTI,"TAPI_GPS_EVENT_SUPL_NOTI "},							/**<	SUPL Notification event type . 		*/

	/*	All please add your events if any," "}, above this one 	*/
	{TAPI_EVENT_TYPE_MAX," "},/**< Event Max */

	/* Events Related on Supplementary services	 */
	{TAPI_EVENT_SS_BARRING_CNF,"TAPI_EVENT_SS_BARRING_CNF"},
	{TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF,"TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF"},
	{TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF,"TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF"},
	{TAPI_EVENT_SS_FORWARD_CNF,"TAPI_EVENT_SS_FORWARD_CNF"},
	{TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF,"TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF"},
	{TAPI_EVENT_SS_WAITING_CNF,"TAPI_EVENT_SS_WAITING_CNF"},
	{TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF,"TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF"},
	{TAPI_EVENT_SS_USSD_CNF,"TAPI_EVENT_SS_USSD_CNF"},
};
#endif

#define TAPI_ERROR_MAX TAPI_API_SERVER_LAYER_FAILURE+1

TAPI_Error_String g_TAPI_ErrorString[200] = {

	/* #################### COMMON START ################ */
	{TAPI_API_SUCCESS, 		"TAPI_API_SUCCESS : No Error occured"},
	{TAPI_API_INVALID_INPUT, 	"TAPI_API_INVALID_INPUT : Input values are not correct in TAPI Library"},
	{TAPI_API_INVALID_PTR, 		"TAPI_API_INVALID_PTR : Invalid pointer"},
	{TAPI_API_NOT_SUPPORTED, 	"TAPI_API_NOT_SUPPORTED: The feature corresponding to requested API is not supported. This may be due to market/network/vendor reasons such as the feature is not available in the network"},
	{TAPI_API_DEPRICATED, 		"TAPI_API_DEPRICATED : This API is deprecated and will be so in future also"},
	{TAPI_API_SYSTEM_OUT_OF_MEM, 	"TAPI_API_SYSTEM_OUT_OF_MEM : Out of memory"},
	{TAPI_API_SYSTEM_RPC_LINK_DOWN, "TAPI_API_SYSTEM_RPC_LINK_DOWN : RPC link down"},
	{TAPI_API_MODEM_POWERED_OFF, 	"TAPI_API_MODEM_POWERED_OFF : The Modem is powered off"},
	{TAPI_API_SERVICE_NOT_READY, 	"TAPI_API_SERVICE_NOT_READY : Phone was powered on, but yet to receive the power up completed notification"},
	{TAPI_API_SERVER_FAILURE , 	"TAPI_API_SERVER_FAILURE : error occurred in Telephony server"},
	{TAPI_API_OEM_PLUGIN_FAILURE , 	"TAPI_API_OEM_PLUGIN_FAILURE  : Plug-in layer failure"},
	{TAPI_API_TRANSPORT_LAYER_FAILURE, "TAPI_API_TRANSPORT_LAYER_FAILURE"},

	{TAPI_API_INVALID_CALL_HANDLE, 	"TAPI_API_INVALID_CALL_HANDLE : Invalid call handle"},
	{TAPI_API_INVALID_CALL_STATE, 	"TAPI_API_INVALID_CALL_STATE : Invalid call state- Newly added. Need to raise a CR for this"},

	{TAPI_API_INVALID_DATA_LEN, 	"TAPI_API_INVALID_DATA_LEN : Invalid data length"},
	{TAPI_API_SIM_NOT_FOUND, 	"TAPI_API_SIM_NOT_FOUND : Sim is not present / removed "},
	{TAPI_API_SIM_BUSY, 		"TAPI_API_SIM_BUSY : SIM is busy"},
	{TAPI_API_SIM_INVALID_CARD_TYPE, "TAPI_API_SIM_INVALID_CARD_TYPE : Sim card is unknown "},
	{TAPI_API_SIM_CARD_PERMANENT_ERROR, "TAPI_API_SIM_CARD_PERMANENT_ERROR : SIM error/blocked state"},
	{TAPI_API_SIM_NOT_INITIALIZED, 	"TAPI_API_SIM_NOT_INITIALIZED : SIM has not initialized yet (waiting for PIN verifcation, etc)"},
	{TAPI_API_REQUEST_MAX_IN_PROGRESS, "TAPI_API_REQUEST_MAX_IN_PROGRESS : Maximum number of API Request for the same service are already in progress "},
	{TAPI_API_OFFLINE_MODE_ERROR,	"TAPI_API_OFFLINE_MODE_ERROR : OEM Telephony Provider is in Offline mode"},
	{TAPI_EVENT_CLASS_UNKNOWN, 	"TAPI_EVENT_CLASS_UNKNOWN : Event class specified is not present in Event Class list"},
	{TAPI_EVENT_UNKNOWN, 		"TAPI_EVENT_UNKNOWN : Event specified is not present in TAPI Unsolicited Event list"},
	{TAPI_REGISTRATION_OP_FAILED, 	"TAPI_REGISTRATION_OP_FAILED : Callback Registration/De-registration failed"},
	{TAPI_NETTEXT_SCADDRESS_NOT_SET, "TAPI_NETTEXT_SCADDRESS_NOT_SET"},
	{TAPI_API_OPERATION_FAILED, 	"TAPI_API_OPERATION_FAILED"},
	{TAPI_API_INVALID_OPERATION,	"TAPI_API_INVALID_OPERATION"},
};

/*static char * nondefined_event_class = "Non defined event class";
static const char* TET_EVENT_CLASS_TO_STRING(int event_class)
{
	int index;
	for( index=0; g_TAPI_event_class_string[index].event_class != TAPI_EVENT_CLASS_MAX ; index++)
	{
		if (g_TAPI_event_class_string[index].event_class == event_class || g_TAPI_event_class_string[index].event_class == TAPI_EVENT_CLASS_MAX)
			return g_TAPI_event_class_string[index].string;
	}
	return nondefined_event_class;
}

static char * nondefined_event = "Non defined event";
static const char* TET_EVENT_TO_STRING(int event_class)
{
	int index;
	for( index=0; g_TAPI_EventString[index].event != TAPI_EVENT_MAX ; index++)
	{
		if (g_TAPI_EventString[index].event == event_class || g_TAPI_EventString[index].event == TAPI_EVENT_MAX)
			return g_TAPI_EventString[index].string;
	}
	return nondefined_event;
}*/

char * nondefined_err = "Non defined error code";
const char* TET_RESULT_TO_STRING(TapiResult_t err_code)
{
	int index;
	for( index=0; g_TAPI_ErrorString[index].err != TAPI_ERROR_MAX ; index++)
	{
		if (g_TAPI_ErrorString[index].err == err_code || g_TAPI_ErrorString[index].err == TAPI_ERROR_MAX)
			return g_TAPI_ErrorString[index].string;
	}
	return nondefined_err;
}

void  ss_utc_callback(TelTapiEvent_t *Event, void *data)
{
	int eventClass;
	int eventType;

	eventClass= Event->EventClass;
	eventType= Event->EventType;

	int expected_event = (int)data;

	if(expected_event == data){
		printf("expected event=%d, received=%d\n", expected_event, eventType);
		G_MAIN_LOOP_CLOSE();
	}	
	
	return;
}
