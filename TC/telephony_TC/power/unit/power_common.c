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

#include "power_common.h"
#include <TapiCommon.h>
#define TIME_DELAY 5000
GMainLoop *g_main_loop = NULL;

static unsigned int subscription_id[5]={0,0,0,0,0};
int expected_event;
bool async_response = false;


void G_MAIN_LOOP_CLOSE()
{
	g_main_loop_quit(g_main_loop);
}

gboolean g_timeout_callback(void * data)
{
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}

int G_MAIN_LOOP_RUN(int sec)
{
	g_main_loop = g_main_loop_new(NULL, FALSE);
	g_timeout_add_seconds(sec, g_timeout_callback, NULL);
	g_main_loop_run(g_main_loop);

	return 0;
}

void print_events(int event) {

	printf("\n event in prnt_events function %d\n", event);

	switch (event) {
	case TAPI_EVENT_NETTEXT_GET_COUNT_CNF: /**<	This event is returned in response to Get Number of messages stored in SIM*/
	case TAPI_EVENT_NETTEXT_SENTSTATUS_CNF: /**<	This event is returned in response to SMS  message sent*/
	case TAPI_EVENT_NETTEXT_GET_PARAM_CNF: /**<	This event is returned in response to NetText Get parameters*/
	case TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF: /**<	This confirmation event is a returned indicating the delete message e status*/
	case TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF: /**<	This confirmation event is a returned indicating the Save SMS into SIM status*/
	case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF: /**<	This event is returned as a confirmation for SET nettext APIs*/
	case TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF: /**<	This event is returned with Get CB configuration details*/
	case TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF: /**<	This event is returned as a response to get preferred bearer used for SMS Transport*/
	case TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF:/**<	This event is returned in response to send delivery report*/
	case TAPI_EVENT_NETTEXT_INCOM_IND: /**<	This is an unsolicited Incoming SMS Event*/
	case TAPI_EVENT_NETTEXT_CB_INCOM_IND: /**<	This is an unsolicited Incoming cell broadcast message*/
	case TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND: /**<	This event is an unsolicited event indicating SIM memory status */
	case TAPI_EVENT_NETTEXT_DEVICE_READY_IND: /**< Device Ready indication */
	case TAPI_EVENT_NETTEXT_PARAM_COUNT_IND: /**< Param count indication */
		/* Events related on Network Messages  */
	case TAPI_EVENT_NETWORK_GETNWBAND_CNF: //=  TAPI_EVENT_CLASS_NETWORK<<8:		/**<0x0300:	This event indicates the Network Band has been identified and returned*/
	case TAPI_EVENT_NETWORK_SETNWBAND_CNF: /**<	This event indicates the Network Band has been set*/
	case TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF: /**<	This event confirms the request to get the service domain */
	case TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF: /**<	This event confirms the request to set the service domain */
	case TAPI_EVENT_NETWORK_CANCELMANUALSELECTION_CNF: /**<	This event is returned on cancellation of manual network selection from network list*/
	case TAPI_EVENT_NETWORK_CANCELMANUALSEARCH_CNF: /**<	This event is returned on cancellation of manual network search*/
	case TAPI_EVENT_NETWORK_GETNWORDER_CNF: /**<	This event is returned on getting network order*/
	case TAPI_EVENT_NETWORK_SETNWORDER_CNF: /**<	This event is returned on setting network order*/
	case TAPI_EVENT_NETWORK_GETNWMODE_CNF:/**<	This event is returned on getting network mode*/
	case TAPI_EVENT_NETWORK_SETNWMODE_CNF: /**<	This event is returned on setting network mode*/
	case TAPI_EVENT_NETWORK_SELECT_CNF: /**<	This confirmation event is returned when user manually selected a network from list for Network selection */
	case TAPI_EVENT_NETWORK_SEARCH_CNF: /**<	This event is returned as confirmation for manual Network search with the available network list*/
	case TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF: /**<	This event is returned on getting network selection mode*/
	case TAPI_EVENT_SS_BARRING_CNF: //=  TAPI_EVENT_CLASS_SS<<8:   /**<0x0400:	This confirmation event is sent when Barring activation/deactivation is requested*/
	case TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF: /**<	This event is returned as Barring password change confirmation*/
	case TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF: /**<	This event is returned  as Barring status query confirmation*/
	case TAPI_EVENT_SS_FORWARD_CNF: /**<	This event is returned as confirmation for Forward Activation/Deactivation/Registration/Erasure */
	case TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF: /**<	This event is returned as confirmation for Forward Status query confirmation*/
	case TAPI_EVENT_SS_WAITING_CNF: /**<	This event is returned as confirmation for Call waiting activation/deactivation*/
	case TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF: /**<	This event is returned as Call waiting status query confirmation*/
	case TAPI_EVENT_SS_USSD_CNF: /**<	This event is returned as USSD send confirmation*/
	case TAPI_EVENT_SS_USSD_IND: /**<	This is an unsolicited USSD indication*/
	case TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF: /**<	This event is indicated to the Application for CLI service interrogation*/
	case TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY: /**<	This event is indicated to the Application when tapi receives Release complete message notification for any SS transactions*/
	case TAPI_EVENT_SS_AOC_RSP: /**<This event is used to send the aoc information */
	case TAPI_EVENT_SS_SET_AOC_CNF: /**<This event is used to send the confirmation for setting aoc values */
	case TAPI_EVENT_SS_BARRING_TYPE_IND: /**<This event is used to send the barring type while making an MO call */
	{
		printf("here sound starts");
		break;
	}
#if 0
		case TAPI_EVENT_SOUND_VOLUMECTRL_RSP ://=  TAPI_EVENT_CLASS_SOUND<<8:	      /**<     */
		case TAPI_EVENT_SOUND_USRCONFIGURATIONCTRL_RSP: /**<     */
		case TAPI_EVENT_SOUND_ERROR_IND: /**<     */
		case TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI: /**<     */
		case TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI: /**<     */
		case TAPI_EVENT_SOUND_MICMUTECTRL_RSP: /**<     */
		case TAPI_EVENT_SOUND_MICGAINCTRL_RSP: /**<     */
#endif
		/*SAT events*/
#if 0
		case TAPI_IMEI_CMLAIND ://=  TAPI_EVENT_CLASS_IMEI<<8:		/**<0x0E00:*/
		{
			printf("here TAPI_IMEI_CMLAIND starts");
			break;
		}
		case TAPI_IMEI_COMPAREITEM: /**<     */
		{
			printf("here TAPI_IMEI_COMPAREITEM starts");
			break;
		}
#endif
#if 0
		case TAPI_GPS_EVENT_START_SESSION_RESP: //=  TAPI_EVENT_CLASS_GPS<<8:		/**<0x0F00:	Start session response event type.		*/
		case TAPI_GPS_EVENT_OPTION_SET_RESP: /**<	GPS option set response  event type.		*/
#if 0 //remove_telephony_dpram_read
		case TAPI_GPS_EVENT_NMEA_DATA_IND: /**<	NMEA Data indication event type.		*/
#endif
		case TAPI_GPS_EVENT_CLOSE_SESSION_RESP: /**<	Close session response event type */

		case TAPI_GPS_EVENT_XTRA_ENABLE_SET_RESP: /**<     */
		case TAPI_GPS_EVENT_XTRA_AUTODL_ENABLE_SET_RESP: /**<     */
		case TAPI_GPS_EVENT_XTRA_CLIENT_INIT_DL_RESP: /**<     */
		case TAPI_GPS_EVENT_XTRA_SET_TIME_INFO_RESP: /**<     */
		case TAPI_GPS_EVENT_XTRA_QUERY_DATA_VALIDITY_RESP: /**<     */

		case TAPI_GPS_EVENT_AGPS_SSL_SET_RESP: /**<     */
		case TAPI_GPS_EVENT_AGPS_BEARER_SET_RESP: /**<     */

		case TAPI_GPS_EVENT_XTRA_TIME_NOTI: /**<	XTRA Time info notification event type.		*/
		case TAPI_GPS_EVENT_XTRA_DOWNLOAD_NOTI: /**<	XTRA  download data notification event type.		*/
		case TAPI_GPS_EVENT_XTRA_DATA_VALIDITY_NOTI: /**<	XTRA  DATA Validity notification.		*/
		case TAPI_GPS_EVENT_XTRA_QUERY_DATA_VALIDITY_NOTI: /**<	XTRA  DATA Validity notification for the Query.		*/
		case TAPI_GPS_EVENT_XTRA_DATA_INJECTION_NOTI: /**<	XTRA  DATA Status injection notification.		*/

		case TAPI_GPS_EVENT_AGPS_PDP_CONNECTION_IND: /**<	.		*/

		case TAPI_GPS_EVENT_AGPS_SSL_GET_RESPONSE: /**<     */

		case TAPI_GPS_EVENT_DEVSTATE_NOTI: /**<	Device state notification event type .		*/
		case TAPI_GPS_EVENT_DEVSTATE_RESP: /**<	Device state response type.		*/
		case TAPI_GPS_EVENT_VERIFICATION_IND: /**<	Verification indication event type.		*/
		case TAPI_GPS_EVENT_SUPL_NOTI: /**<	SUPL Notification event type .		*/
#endif

		/*	All please add your events if any: above this one	*/
	case TAPI_EVENT_TYPE_MAX:/**< Event Max */
	{
		printf("here max starts");
		break;
	}

	default: {
		printf("\n in the default case of print_events \n");
		break;
	}
	}
}

void power_app_callback(TelTapiEvent_t *TapiEvent) {
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void *EventData = NULL;

	eventClass = TapiEvent->EventClass;
	eventType = TapiEvent->EventType;
	requestId = TapiEvent->RequestId;
	status = TapiEvent->Status;
	EventData = TapiEvent->pData;
	printf(" app _callback is called \n");

	print_events((int) eventType);
	printf("\n event came in power_callback [%d] \n", eventType);
	printf("\n expected event is  in callback [%d] \n", expected_event);

	switch (eventClass) {
	default:
		TEST_DEBUG("##Undefined Power Event received")
		;
		break;
	}

	if (eventType == expected_event) {
		g_main_loop_quit(g_main_loop);
		async_response = true;
	}
	return;
}

void print_all_subsciption_ids() {
	int i = 0;
	for (i = 0; i < 2; i++) {
		printf("\n subscription_id[%d] has value %d \n", i, subscription_id[i]);
	}
}

TapiResult_t register_all_events() {
	TapiResult_t api_err = -1;

	api_err = tel_register_event(TAPI_EVENT_POWER_FLIGHT_MODE_RESP,
			&subscription_id[0], (TelAppCallback) &power_app_callback, NULL);
	api_err = tel_register_event(TAPI_EVENT_POWER_PHONE_OFF,
			&subscription_id[1], (TelAppCallback) &power_app_callback, NULL);

	print_all_subsciption_ids();
	return api_err;
}

TapiResult_t deregister_all() {
	TapiResult_t api_err = -1;

	api_err = tel_deregister_event(subscription_id[0]);
	api_err = tel_deregister_event(subscription_id[1]);
	return api_err;

}
