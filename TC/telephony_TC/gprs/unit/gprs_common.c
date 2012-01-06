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

#include "gprs_common.h"

GMainLoop *nw_context_loop = NULL;

static unsigned int subscription_id[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int expected_event;
bool async_response = false;

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

//static int  ps_app_callback (int group, int action, void* data, int length)
void ps_app_callback(TelTapiEvent_t *TapiEvent)
{
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

	printf("- expected event=0x%x received event=0x%x\n", expected_event, eventType);
	if (eventType == expected_event) {
		async_response = true;
		g_main_loop_quit(nw_context_loop);
	}

	return;

}

void print_all_subsciption_ids()
{
	int i = 0;
	for (i = 0; i < 16; i++) {
		printf("subscription_id[%02d] has value %d \n", i, subscription_id[i]);
	}
}

TapiResult_t register_all_events()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	api_err = tel_register_event(TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI, &subscription_id[0], (TelAppCallback) &ps_app_callback, NULL);
	api_err = tel_register_event(TAPI_EVENT_PS_CALL_STATISTICS, &subscription_id[1], (TelAppCallback) &ps_app_callback, NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_CALL_STATUS_RSP, &subscription_id[2], (TelAppCallback)&ps_app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_PS_ERROR_IND, &subscription_id[4], (TelAppCallback) &ps_app_callback, NULL);
	api_err = tel_register_event(TAPI_EVENT_PS_HSDPA_STATUS_NOTI, &subscription_id[5], (TelAppCallback) &ps_app_callback, NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_NETWORK_STATUS_RSP, &subscription_id[6], (TelAppCallback)&ps_app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO, &subscription_id[7], (TelAppCallback) &ps_app_callback, NULL);
	api_err = tel_register_event(TAPI_EVENT_PS_PDP_ACT_RSP, &subscription_id[8], (TelAppCallback) &ps_app_callback, NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_PDP_ADDRESS_RSP, &subscription_id[9], (TelAppCallback)&ps_app_callback,NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_PDP_CONTEXTINFO_RSP, &subscription_id[10], (TelAppCallback)&ps_app_callback,NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_PDP_DEACT_IND, &subscription_id[11], (TelAppCallback)&ps_app_callback,NULL);
	api_err = tel_register_event(TAPI_EVENT_PS_PDP_DEACT_RSP, &subscription_id[12], (TelAppCallback) &ps_app_callback, NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_QOS_3G_PROFILE_RSP, &subscription_id[13], (TelAppCallback)&ps_app_callback,NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_QOS_PROFILE_RSP, &subscription_id[14], (TelAppCallback)&ps_app_callback,NULL);
	//api_err = tel_register_event(TAPI_EVENT_PS_STATUS, &subscription_id[15], (TelAppCallback)&ps_app_callback,NULL);

	print_all_subsciption_ids();
	return api_err;
}

TapiResult_t deregister_all()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	api_err = tel_deregister_event(subscription_id[0]);
	api_err = tel_deregister_event(subscription_id[1]);
	//api_err = tel_deregister_event(subscription_id[2]);
	api_err = tel_deregister_event(subscription_id[4]);
	api_err = tel_deregister_event(subscription_id[5]);
	//api_err = tel_deregister_event(subscription_id[6]);
	api_err = tel_deregister_event(subscription_id[7]);
	api_err = tel_deregister_event(subscription_id[8]);
	//api_err = tel_deregister_event(subscription_id[9]);
	//api_err = tel_deregister_event(subscription_id[10]);
	//api_err = tel_deregister_event(subscription_id[11]);
	api_err = tel_deregister_event(subscription_id[12]);
	//api_err = tel_deregister_event(subscription_id[13]);
	//api_err = tel_deregister_event(subscription_id[14]);
	//api_err = tel_deregister_event(subscription_id[15]);

	return api_err;

}

TapiResult_t temp_gprs_session_activate()
{
	TapiResult_t ret_status;
	tapi_ps_net_start_req_t start_req;
	int request_id = 0;

	TEST_DEBUG("- Temp gprs activation starts\n");

	memset(&start_req, 0, sizeof(tapi_ps_net_start_req_t));

	start_req.cont_id = CONTEXT_ID;
	strncpy(start_req.pdp_info.apn, APN_ADDRESS, TAPI_PDP_APN_LEN_MAX);

	ret_status = tel_activate_gprs_pdp(&start_req, &request_id);

	printf("- return value = %d\n", ret_status);

	if (TAPI_API_SUCCESS == ret_status) {
		printf("- temp gprs mode Started successfully \n");
	}
	else {
		printf("- temp gprs mode not started \n");
	}

	expected_event = TAPI_EVENT_PS_PDP_ACT_RSP;

	G_MAIN_LOOP_RUN(10);

	TEST_DEBUG("- Temp gprs activation ends\n");

	return ret_status;
}

TapiResult_t temp_gprs_session_deactivate(int contId)
{
	TapiResult_t ret_status;
	tapi_ps_net_stop_req_t end_req;
	int request_id = 0;

	TEST_DEBUG("- Temp gprs deactivation starts\n");

	memset(&end_req, 0, sizeof(tapi_ps_net_stop_req_t));

	end_req.cont_id = contId;

	ret_status = tel_deactivate_gprs_pdp(&end_req, &request_id);

	printf("- return value = %d\n", ret_status);

	if (TAPI_API_SUCCESS == ret_status) {
		printf("- temp gprs mode ended successfully\n");
	}
	else {
		printf("- temp gprs mode not ended \n");
	}

	expected_event = TAPI_EVENT_PS_PDP_DEACT_RSP;

	G_MAIN_LOOP_RUN(10);

	TEST_DEBUG("- Temp gprs deactivation ends\n");

	return ret_status;
}

