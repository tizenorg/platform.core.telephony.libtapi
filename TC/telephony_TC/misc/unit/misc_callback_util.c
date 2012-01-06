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

#include "misc_callback_util.h"


static unsigned int subscription_id[6]={0,0,0,0,0,0};

/**
* @file     misc_callback_util.c
* @brief     This is the implementation file for misc callback utility APIs
* @version   [09-03-04] Initial Creation V0.1
*/


GMainLoop *main_loop = NULL;

void  misc_Callback (TelTapiEvent_t * TapiEvent)
{

	int			eventClass;
	int 			eventType;
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
			case TAPI_EVENT_CLASS_MISC:
				switch(eventType)
				{
					case TAPI_EVENT_MISC_NITZ_GETCNF :
						printf("***  TAPI_EVENT_MISC_NITZ_GETCNF received : %x ****\n", eventType);
						break;

					case TAPI_EVENT_MISC_NITZ_NOTIFY :
						printf("***  TAPI_EVENT_MISC_NITZ_NOTIFY received : %x ****\n", eventType);
						break;

					case TAPI_EVENT_MISC_NITZ_IND :
						printf("***  TAPI_EVENT_MISC_NITZ_IND received : %x ****\n", eventType);
						break;

					default:
						printf("*** MISC Default Event  %x ****\n", eventType);
						break;

				}

				break;

			default:
				printf("*** Other TAPI EVENT received : class : %x ****\n", eventClass);
				break;
	}

}


gboolean
TimeOutFun (void *data)
{
	g_main_loop_quit (main_loop);
	printf ("Exit from this callback\n");
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
	for(i=0;i<3;i++)
	{
		printf("\n subscription_id[%d] has value %d \n",i,subscription_id[i]);
	}
}

TapiResult_t register_all_events()
{
	TapiResult_t api_err = TAPI_API_SUCCESS;

	api_err = tel_register_event(TAPI_EVENT_MISC_NITZ_GETCNF, &subscription_id[0], (TelAppCallback)misc_Callback, NULL);
	api_err = tel_register_event(TAPI_EVENT_MISC_NITZ_NOTIFY, &subscription_id[1], (TelAppCallback)misc_Callback, NULL);
	api_err = tel_register_event(TAPI_EVENT_MISC_NITZ_IND, &subscription_id[2], (TelAppCallback)misc_Callback, NULL);
	print_all_subsciption_ids();
	return 0;
}


 TapiResult_t deregister_all()
{
	tel_deregister_event(subscription_id[0]);
	tel_deregister_event(subscription_id[1]);
	tel_deregister_event(subscription_id[2]);
	return 0;
}







