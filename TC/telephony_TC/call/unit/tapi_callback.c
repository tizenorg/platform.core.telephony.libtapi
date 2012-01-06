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

#include "tapi_callback.h"

GMainLoop *main_loop = NULL;
TS_UINT g_CallHandle = -1;
TS_BOOL call_status = FALSE;
/***** TESTING FOR VALID SET OF INPTS*****/

TS_UINT conf_call_handle=-1 ;
static unsigned int subscription_id[32]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int expected_event;

void  app_callback  (TelTapiEvent_t * TapiEvent)
{

	int	eventClass;
	int	eventType;
	int			requestId;
	int			status;
	void 			*EventData = NULL;

	TS_UINT callHandle;

	eventClass = TapiEvent->EventClass;
	eventType = TapiEvent->EventType;
	requestId = TapiEvent->RequestId;
	status = TapiEvent->Status;
	EventData = TapiEvent->pData;

	CALL_TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, 				status );
	if( EventData==NULL)
	{
		CALL_TEST_DEBUG("******  EventData is NULL.........   ******\n");
	}

	switch(eventClass)
	{
		case TAPI_EVENT_CLASS_CALL_VOICE:
		case TAPI_EVENT_CLASS_CALL_DATA:
		{

			switch(eventType)
			{

				case  TAPI_EVENT_CALL_SETUP_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_SETUP_CNF. callHandle:[%d]\n",callHandle);
					break;


				case TAPI_EVENT_CALL_ALERT_IND:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_ALERT_IND. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_INCOM_IND:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					g_CallHandle = callHandle;
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_INCOM_IND. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_ANSWER_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_ANSWER_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_WAITING_IND:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_WAITING_IND. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_CONNECTED_IND:

					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_CONNECTED_IND. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_CONNECTING_IND:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_CONNECTING_IND. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_RETRIEVE_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_RETRIEVE_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_HOLD_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_HOLD_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_SETUPCONFERENCE_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					conf_call_handle = callHandle;
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_SETUPCONFERENCE_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_SPLITCONFERENCE_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));

					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_SPLITCONFERENCE_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_TRANSFER_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_TRANSFER_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_SEND_DTMF_CNF: // no data.
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_SEND_DTMF_CNF. \n");
					break;

				case TAPI_EVENT_CALL_RELEASE_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_RELEASE_CNF. callHandle:[%d]\n",callHandle);
					break;

				case TAPI_EVENT_CALL_RELEASE_ALL_CNF: // no data.
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_RELEASE_ALL_CNF. \n");
					break;

				case TAPI_EVENT_CALL_END_IND:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_END_IND. callHandle:[%d]\n",callHandle);
					call_status = TRUE;
					break;

				case TAPI_EVENT_CALL_SET_LINEID_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_SET_LINEID_CNF. callHandle:[%d]\n",callHandle);
					call_status = TRUE;
					break;

				case TAPI_EVENT_CALL_GET_LINEID_CNF:
					memcpy(&callHandle, EventData, sizeof(TS_UINT));
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_GET_LINEID_CNF. callHandle:[%d]\n",callHandle);
					call_status = TRUE;
					break;

				case TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF:
					CALL_TEST_DEBUG(" === TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF\n");
					call_status = TRUE;
					break;

				default:
					CALL_TEST_DEBUG(" === VOICE EVENT CLASS. but UNKNOWN EVENT TYPE:[%d] \n", eventType);
					call_status = FALSE;
					break;

			}
                        break;
		}
		default:
			CALL_TEST_DEBUG(" UNKNOWN EVENT CLASS:[%d] \n", eventClass);
			break;

	}
	if(eventType == expected_event)
		g_main_loop_quit(main_loop);
}

gboolean
TimeOutFun (void *data)
{
	g_main_loop_quit (main_loop);
	printf ("Exit from this callback\n");
	call_status = FALSE;
	return FALSE;


}

int gmain_loop_for_tapi_events(int delay)
{

	main_loop = g_main_loop_new (NULL, FALSE);
	g_timeout_add (delay, TimeOutFun, NULL);
	g_main_loop_run (main_loop);

	return 0;


}





void print_all_subsciption_ids()
{
	int i=0;
	for(i=0;i<27;i++)
	{
		printf("\n subscription_id[%d] has value %d \n",i,subscription_id[i]);
	}
}

TapiResult_t register_all_events()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	api_err = tel_register_event(TAPI_EVENT_CALL_SETUP_CNF, &subscription_id[0], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_ALERT_IND, &subscription_id[1], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_CONNECTING_IND, &subscription_id[2], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_CONNECTED_IND, &subscription_id[3], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_INCOM_IND, &subscription_id[4], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_ANSWER_CNF, &subscription_id[5], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_RETRIEVE_CNF, &subscription_id[6], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_RETRIEVE_IND, &subscription_id[7], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_HOLD_IND, &subscription_id[8], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_HOLD_CNF, &subscription_id[9], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_TRANSFER_CNF, &subscription_id[10], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_TRANSFER_IND, &subscription_id[11], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_SETUPCONFERENCE_CNF, &subscription_id[12], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_SETUPCONFERENCE_IND, &subscription_id[13], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_SPLITCONFERENCE_CNF, &subscription_id[14], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_SEND_DTMF_CNF, &subscription_id[15], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_WAITING_IND, &subscription_id[16], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_FORWARD_IND, &subscription_id[17], (TelAppCallback)&app_callback,NULL);
	//api_err = tel_register_event(TAPI_EVENT_CALL_USERINFO_IND, &subscription_id[18], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_RELEASE_CNF, &subscription_id[19], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_RELEASE_ALL_CNF, &subscription_id[20], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF, &subscription_id[21], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_END_IND, &subscription_id[22], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_GET_LINEID_CNF, &subscription_id[23], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_SET_LINEID_CNF, &subscription_id[24], (TelAppCallback)&app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_CALL_SWAP_CNF, &subscription_id[25], (TelAppCallback)&app_callback,NULL);
	/* For multiparty call */
	api_err = tel_register_event(TAPI_EVENT_SS_WAITING_CNF, &subscription_id[26], (TelAppCallback)&app_callback,NULL);

	/* For multiparty call */
	api_err = tel_register_event(TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF, &subscription_id[27], (TelAppCallback)&app_callback,NULL);

//	api_err = tel_register_event(TAPI_EVENT_CALL_EXIT_EMERGENCYMODE_CNF, &subscription_id[28], (TelAppCallback)&app_callback,NULL);

	api_err = tel_register_event(TAPI_EVENT_CALL_FLASHINFO_CNF, &subscription_id[28], (TelAppCallback)&app_callback,NULL);

//	api_err = tel_register_event(TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF, &subscription_id[30], (TelAppCallback)&app_callback,NULL);

//	api_err = tel_register_event(TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF, &subscription_id[31], (TelAppCallback)&app_callback,NULL);


//	print_all_subsciption_ids();
	return api_err;
}


 TapiResult_t deregister_all()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	api_err = tel_deregister_event(subscription_id[0]);
	api_err = tel_deregister_event(subscription_id[1]);
	api_err = tel_deregister_event(subscription_id[2]);
	api_err = tel_deregister_event(subscription_id[3]);
	api_err = tel_deregister_event(subscription_id[4]);
	api_err = tel_deregister_event(subscription_id[5]);
	api_err = tel_deregister_event(subscription_id[6]);
	api_err = tel_deregister_event(subscription_id[7]);
	api_err = tel_deregister_event(subscription_id[8]);
	api_err = tel_deregister_event(subscription_id[9]);
	api_err = tel_deregister_event(subscription_id[10]);
	api_err = tel_deregister_event(subscription_id[11]);
	api_err = tel_deregister_event(subscription_id[12]);
	api_err = tel_deregister_event(subscription_id[13]);
	api_err = tel_deregister_event(subscription_id[14]);
	api_err = tel_deregister_event(subscription_id[15]);
	api_err = tel_deregister_event(subscription_id[16]);
	api_err = tel_deregister_event(subscription_id[17]);
	api_err = tel_deregister_event(subscription_id[18]);
	api_err = tel_deregister_event(subscription_id[19]);
	api_err = tel_deregister_event(subscription_id[20]);
	api_err = tel_deregister_event(subscription_id[21]);
	api_err = tel_deregister_event(subscription_id[22]);
	api_err = tel_deregister_event(subscription_id[23]);
	api_err = tel_deregister_event(subscription_id[24]);
	api_err = tel_deregister_event(subscription_id[25]);
	api_err = tel_deregister_event(subscription_id[26]);
	api_err = tel_deregister_event(subscription_id[27]);
//	api_err = tel_deregister_event(subscription_id[28]);
	api_err = tel_deregister_event(subscription_id[28]);
///	api_err = tel_deregister_event(subscription_id[30]);
//	api_err = tel_deregister_event(subscription_id[31]);

	return api_err;

}

