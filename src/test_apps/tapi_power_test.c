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

#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <glib.h>
#include <string.h>
#include <unistd.h>

#include "TapiCommon.h"
#include "ITapiPower.h"
#include "TelDisplay.h"
#include "TapiEvent.h"

#define TEST_DEBUG(frmt, args...)	\
{do { fprintf(stderr, "[Pow Test][%s:%04d] "frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}


#ifndef false
#define false 0
#endif
#ifndef true
#define true  !0
#endif
#ifndef bool
#define bool  char
#endif

#define	NUM_OF_POWER_EVENT			4

static unsigned int power_subscription_id[NUM_OF_POWER_EVENT] = {0, };

static unsigned int  display_subscription_id = 0;

void __PrintBatteryStatus(void *pData)
{
	tapi_power_battery_status_level_t*  pBattLevel;

	pBattLevel = (tapi_power_battery_status_level_t*)pData;
	switch(*pBattLevel){
		case TAPI_POWER_BATT_STAT_POWER_OFF_LEVEL:
			TEST_DEBUG("Battery Level  [Power Off]");
			break;
		case TAPI_POWER_BATT_STAT_CRIT_LOW_LEVEL:
			TEST_DEBUG("Battery Level [Critical-Low]");
			break;
		case TAPI_POWER_BATT_STAT_LOW_LEVEL:
			TEST_DEBUG("Battery Level [Low Battery]");
			break;
		case TAPI_POWER_BATT_STAT_NORMAL_LEVEL:
			TEST_DEBUG("Battery Level [Normal Level]");
			break;
		default:
			TEST_DEBUG("Unknown Battery Level");
			break;
	}

	return;
}

void __PrintFlightModeStatus(void *pData)
{
	tapi_power_flight_mode_resp_type_t*  pFlightStatus;

	pFlightStatus = (tapi_power_flight_mode_resp_type_t *)pData;

	switch(*pFlightStatus){
		case TAPI_POWER_FLIGHT_MODE_RESP_ON:
			TEST_DEBUG("Flight Mode  [On]");
			break;
		case TAPI_POWER_FLIGHT_MODE_RESP_OFF:
			TEST_DEBUG("Flight Mode  [Off]");
			break;
		case TAPI_POWER_FLIGHT_MODE_RESP_FAIL:
			TEST_DEBUG("Flight Mode  [Mode Change Fail]");
			break;
		default:
			TEST_DEBUG("Unknown Flight Mode");
			break;
	}
	return;
}

void __PrintDisplayIconInfo(void *pData)
{
	tapi_display_icon_info_t*  pDisplayIcon = NULL;

	pDisplayIcon = (tapi_display_icon_info_t*)pData;

	switch(pDisplayIcon->rssi){
		case TAPI_DISPLAY_RSSI_0:
			TEST_DEBUG("RSSI [0]");
			break;
		case TAPI_DISPLAY_RSSI_1:
			TEST_DEBUG("RSSI [1]");
			break;
		case TAPI_DISPLAY_RSSI_2:
			TEST_DEBUG("RSSI [2]");
			break;
		case TAPI_DISPLAY_RSSI_3:
			TEST_DEBUG("RSSI [3]");
			break;
		case TAPI_DISPLAY_RSSI_4:
			TEST_DEBUG("RSSI [4]");
			break;
		case TAPI_DISPLAY_RSSI_5:
			TEST_DEBUG("RSSI [5]");
			break;
		case TAPI_DISPLAY_RSSI_6:
			TEST_DEBUG("RSSI [6]");
			break;
		default:
			TEST_DEBUG("Unknow RSSI Value");
			break;

	}

	switch(pDisplayIcon->batt){
		case TAPI_POWER_PHONE_BATT_LEVEL0:
			TEST_DEBUG("Battery Level [0]");;
			break;
		case TAPI_POWER_PHONE_BATT_LEVEL1:
			TEST_DEBUG("Battery Level [1]");
			break;
		case TAPI_POWER_PHONE_BATT_LEVEL2:
			TEST_DEBUG("Battery Level [2]");
			break;
		case TAPI_POWER_PHONE_BATT_LEVEL3:
			TEST_DEBUG("Battery Level [3]");
			break;
		case TAPI_POWER_PHONE_BATT_LEVEL4:
			TEST_DEBUG("Battery Level [4]");
			break;
		case TAPI_POWER_PHONE_BATT_LEVEL5:
			TEST_DEBUG("Battery Level [5]");
			break;
		case TAPI_POWER_PHONE_BATT_LEVEL_INIT:
			TEST_DEBUG("Battery Level [Init]");
			break;
		default:
			TEST_DEBUG("Unknow Battery Level Value");
			break;

	}

}

void  power_async_event_callback(TelTapiEvent_t* event,void *data )
{

	TEST_DEBUG("$$$$$$ POWER EVENT NOTIFICATION $$$$$$");
	TEST_DEBUG("Request ID : [0x%x]", event->RequestId);

	switch(event->EventType)
	{
		case TAPI_EVENT_POWER_PHONE_OFF:
			TEST_DEBUG("Event Type: [TAPI_EVENT_POWER_PHONE_OFF]");
			break;
		case TAPI_EVENT_POWER_BATT_STAT_IND:
			TEST_DEBUG("Event Type: [TAPI_EVENT_POWER_BATT_STAT_IND]");
			__PrintBatteryStatus(event->pData);
			break;
		case TAPI_EVENT_POWER_FLIGHT_MODE_RESP:
			TEST_DEBUG("Event Type: [TAPI_EVENT_POWER_FLIGHT_MODE_RESP]");
			__PrintFlightModeStatus(event->pData);
			break;
		case TAPI_EVENT_DISPLAY_ICON_INFO_IND:
			TEST_DEBUG("Event Type: [TAPI_EVENT_DISPLAY_ICON_INFO_IND]");
			__PrintDisplayIconInfo(event->pData);
			break;
		default:
			TEST_DEBUG("Event Type: unknown event:[%d]", event->EventType);
			break;
	}
	TEST_DEBUG("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

}


int power_read_key_input(void)
{
	int ret;
	char buf[512] = {0, };
	char msg[512];
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	memset(buf,0, sizeof(buf));
	memset(msg,0,sizeof(msg));

	printf("******* Power & Display Test MENU ******************\n");
	printf("*************** Asynchronous API *****************\n");
	printf("Power Off[PwrOff]\n");
	printf("Power On[PwrOn]\n");
	printf("Reset [Reset]\n");
	printf("Flight Mode On[FlightModeOn]  \n");
	printf("Flight Mode Off[FlightModeOff] \n");
	printf("Reboot Modem[RebootModem] \n");
	printf("Ramdump [Ramdump] \n");
	printf("**************** Synchronous API *****************\n");
	printf("Get Power status[GetPwrStatus] \n");
	printf("Quit Power Test[quit] \n");
	printf("************************************************\n");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	if(strncmp(buf, "PwrOff", sizeof("PwrOff")-1) == 0)
	{
		ret_val = tel_process_power_command(TAPI_PHONE_POWER_OFF);
		TEST_DEBUG("tel_process_power_command return value is %d \n",ret_val);
	}

	else if(strncmp(buf, "PwrOn", sizeof("PwrOn")-1) == 0)
	{
		ret_val = tel_process_power_command(TAPI_PHONE_POWER_ON);
		TEST_DEBUG("tel_process_power_command return value is %d \n",ret_val);
	}

	else if(strncmp(buf, "Reset", sizeof("Reset")-1) == 0)
	{
		ret_val = tel_process_power_command(TAPI_PHONE_POWER_RESET);
		TEST_DEBUG("tel_process_power_command return value is %d \n",ret_val);
	}

	else if(strncmp(buf, "FlightModeOn", sizeof("FlightModeOn")-1) == 0)
	{
		ret_val = tel_set_flight_mode(TAPI_POWER_FLIGHT_MODE_ENTER);
		TEST_DEBUG("tel_set_flight_mode return value is %d \n",ret_val);
	}

	else if(strncmp(buf, "FlightModeOff", sizeof("FlightModeOff")-1) == 0)
	{
		ret_val = tel_set_flight_mode(TAPI_POWER_FLIGHT_MODE_LEAVE);
		TEST_DEBUG("tel_set_flight_mode return value is %d \n",ret_val);
	}

	else if(strncmp(buf, "RebootModem", sizeof("RebootModem")-1) == 0)
	{
		ret_val = tel_reset_modem();
		TEST_DEBUG("tel_reset_modem return value is %d \n",ret_val);
	}

	else if(strncmp(buf, "Ramdump", sizeof("Ramdump")-1) == 0)
	{
		ret_val = tel_enforce_ramdump_of_modem();
		TEST_DEBUG("tel_enforce_ramdump_of_modem return value is %d",ret_val);
	}
	else if(strncmp(buf, "quit", sizeof("quit")-1) == 0)
	{
		return -1;
	}

	memset(buf, 0, 512);

	return 0;
}


void power_select_loop(void)
{
	int ret;
	fd_set readfds;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret)
		{
			if (FD_ISSET(0, &readfds))
			{
				if(power_read_key_input() < 0)
					break;
			}
		}
	}
	return;
}

int power_test_unsubscribe_tapi_event(void)
{
	int i = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;
	int ret_val = TRUE;

	for(i=0;i<NUM_OF_POWER_EVENT; i++){
		api_err = tel_deregister_event(power_subscription_id[i]);
		if(api_err != TAPI_API_SUCCESS){
			TEST_DEBUG("tel_deregister_event isn't unsubscribed. sub id is %d api_err is %d\n",power_subscription_id[i],api_err);
			ret_val = FALSE;
			break;
		}

	}

	tel_deinit();

	return ret_val;
}

int power_test_subscribe_tapi_events (void)
{
	TEST_DEBUG("1");

	int i = 0;
	int ret_val = TRUE;
	TapiResult_t	api_err = TAPI_API_SUCCESS;

	int PowerEvtList[NUM_OF_POWER_EVENT] = {
		TAPI_EVENT_POWER_PHONE_OFF,
		TAPI_EVENT_POWER_BATT_STAT_IND,
		TAPI_EVENT_POWER_FLIGHT_MODE_RESP,
		TAPI_EVENT_POWER_SERVICE_READY_IND,
	};

	//TEST_DEBUG("########### Power TEST #############");
	if(tel_init() == TAPI_API_SUCCESS)
	{
		TEST_DEBUG("2");
		for(i=0;i<NUM_OF_POWER_EVENT; i++){
			TEST_DEBUG("3");
			api_err = tel_register_event(PowerEvtList[i], &power_subscription_id[i],(TelAppCallback)&power_async_event_callback,NULL);
			TEST_DEBUG("4");

			if(api_err != TAPI_API_SUCCESS){
				TEST_DEBUG("tel_register_event isn't subscribed. sub id is %d api_err is %d\n",power_subscription_id[i],api_err);
				ret_val = FALSE;
				break;
			}
		}

		TEST_DEBUG("5");

		api_err = tel_register_event(TAPI_EVENT_DISPLAY_ICON_INFO_IND, &display_subscription_id,(TelAppCallback)&power_async_event_callback,NULL);
		TEST_DEBUG("6");
		if(api_err != TAPI_API_SUCCESS){
			TEST_DEBUG("tel_register_event isn't subscribed. sub id is %d api_err is %d\n",display_subscription_id,api_err);
			ret_val = FALSE;
		}
	}
	else {

		TEST_DEBUG("tel_init() failed");
	}
	return ret_val;
}

int power_test_main(int argc, char *argv[])
{
	return 0;
}
