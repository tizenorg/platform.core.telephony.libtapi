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
#include "cfg_util.h"

static unsigned int subscription_id[7];
GMainLoop *main_loop = NULL;

void  app_cfg_callback (TelTapiEvent_t * TapiEvent)
{

	TelTapiEventClass_t	eventClass;
	TelTapiEventType_t	eventType;
	int				requestId;
	int				status;
	void 			*EventData = NULL;

	eventClass = TapiEvent->EventClass;
	eventType = TapiEvent->EventType;
	requestId = TapiEvent->RequestId;
	status = TapiEvent->Status;
	EventData = TapiEvent->pData;

	printf("\nClass:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if( EventData==NULL)
	{
		printf("\n******  EventData is NULL.........   ******\n");
	}

	switch(eventClass)
	{
			case TAPI_EVENT_CLASS_CFG:
				switch(eventType)
				{
					case TAPI_EVENT_CFG_GET_A_KEY_CNF :
						printf("***  TAPI_EVENT_CFG_GET_A_KEY_CNF received : %x ****\n", eventType);
						break;

					case TAPI_EVENT_CFG_VERIFY_A_KEY_CNF:
						printf("*** TAPI_EVENT_CFG_VERIFY_A_KEY_CNF received  : %x ****\n", eventType);
						break;

					case TAPI_EVENT_CFG_GET_MSL_INFO_CNF:
						printf("***  TAPI_EVENT_CFG_GET_MSL_INFO_CNF received : %x ****\n", eventType);
						break;

					case TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF:
						printf("***  TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF received : %x ****\n", eventType);
						break;

					case TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF:
						printf("***  TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF received : %x ****\n", eventType);
						break;

					case TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF:
						printf("***  TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF received : %x ****\n", eventType);
						break;
					case TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF:
						printf("***  TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF received : %x ****\n", eventType);
						break;
                                        case TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF:
						printf("***  TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF received : %x ****\n", eventType);
						break;
                                         case TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF:
						printf("***  TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF received : %x ****\n", eventType);
						break;
                                         case TAPI_EVENT_CFG_GET_SIO_MODE_CNF:
						printf("***  TAPI_EVENT_CFG_GET_SIO_MODE_CNF received : %x ****\n", eventType);
						break;
                                          case 	TAPI_EVENT_CFG_SET_SIO_MODE_CNF:
						printf("***  TAPI_EVENT_CFG_SET_SIO_MODE_CNF received : %x ****\n", eventType);
						break;
                                         case 	TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF:
						printf("***  TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF received : %x ****\n", eventType);
						break;
                                         case 	TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF:
						printf("*** TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF received : %x ****\n", eventType);
						break;
                                         case 	TAPI_EVENT_CFG_SET_TTY_MODE_CNF:
						printf("*** 	TAPI_EVENT_CFG_SET_TTY_MODE_CNF received : %x ****\n", eventType);
						break;
                                        
                                        case 	TAPI_EVENT_CFG_GET_TTY_MODE_CNF:
						printf("*** 	TAPI_EVENT_CFG_GET_TTY_MODE_CNF received : %x ****\n", eventType);
						break;
					default:
						printf("***  Default Event  %x ****\n", eventType);
						break;

				}

				break;

			default:
				printf("*** Other TAPI EVENT received : class : %x ****\n", eventClass);
				break;
	}

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
	for(i=0;i<7;i++)
	{
		printf("\n subscription_id[%d] has value %d \n",i,subscription_id[i]);
	}
}


TapiResult_t register_all_events()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	api_err = tel_register_event(TAPI_EVENT_CFG_GET_A_KEY_CNF, &subscription_id[0], app_cfg_callback);

	api_err = tel_register_event(TAPI_EVENT_CFG_VERIFY_A_KEY_CNF, &subscription_id[1], app_cfg_callback);
	api_err = tel_register_event(TAPI_EVENT_CFG_GET_MSL_INFO_CNF, &subscription_id[2], app_cfg_callback);
	api_err = tel_register_event(TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF, &subscription_id[3],app_cfg_callback);
	api_err = tel_register_event(TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF, &subscription_id[4], app_cfg_callback);
	api_err = tel_register_event(TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF, &subscription_id[5], app_cfg_callback);
	api_err = tel_register_event(TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF, &subscription_id[6], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF, &subscription_id[7], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF, &subscription_id[8], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_GET_SIO_MODE_CNF, &subscription_id[9], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_SET_SIO_MODE_CNF, &subscription_id[10], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF, &subscription_id[11], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF, &subscription_id[12], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_SET_TTY_MODE_CNF, &subscription_id[13], app_cfg_callback);
        api_err = tel_register_event(TAPI_EVENT_CFG_GET_TTY_MODE_CNF, &subscription_id[14], app_cfg_callback);

	print_all_subsciption_ids();
	return 0;
}


 TapiResult_t deregister_all()
{
  int i ;
        for(i=0 ;i<=14; i++)
        {
	tel_deregister_event(subscription_id[i]);
        }
	
	return 0;

}


