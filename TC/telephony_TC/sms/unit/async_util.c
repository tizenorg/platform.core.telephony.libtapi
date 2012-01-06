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


#include <stdio.h>
#include <unistd.h>

#include "async_util.h"

static void G_MAIN_TIMER_HANDLER(int sgi);

// Global variable for G_MAIN system
#if 0
static int gEventHandle=0;
static unsigned int gSubscription_id;
#endif
static GMainLoop * nw_context_loop = NULL;
static EvtAlarmCb gpAlarm_Handler = NULL;

GMainLoop *main_loop = NULL;

void G_MAIN_LOOP_CLOSE()
{
	g_main_loop_quit(nw_context_loop);
}

void G_MAIN_TIMER_HANDLER(int sgi)
{
	if (gpAlarm_Handler)
		gpAlarm_Handler(sgi);
	else {
		fprintf(stderr, "SIGALRM received. But user handler not registered. Close g_main_loop()");
		G_MAIN_LOOP_CLOSE();
	}
}

#if 0
TS_BOOL G_MAIN_INIT(EvtSysEventCb pNotiHandler, EvtAlarmCb pAlarmHandler)
{
	int open_status;
	int error_code;
	gpAlarm_Handler = pAlarmHandler;

	g_type_init();
	/*
	 open_status = EvtSysLibraryOpen(&gEventHandle);
	 if (open_status!=0)
	 {
	 printf("Unable to get connection to event delivery\n");
	 printf("the error is %d\n",open_status);
	 return FALSE;
	 }
	 printf("event handle is %d\n",gEventHandle);

	 error_code = EvtSysEventSubscribe(
	 0,
	 "Telephony.Nettext.*",          // notification ID
	 NULL,                       //  filter expression   //
	 pNotiHandler,      // notification handler
	 NULL,                       // local data //
	 &gSubscription_id            // subscription ID //
	 );
	 */
	if (error_code )
	{
		fprintf(stderr, "EvtSysEventSubscribe(): error [%#X] for event [Telephony.test.Async]\n", error_code);
		return FALSE;
	}
	return TRUE;
}

TS_BOOL G_MAIN_CLOSE()
{
	//int close_status = EvtSysLibraryClose(gEventHandle);
	if (close_status!=0)
	{
		printf("Unable to get close connection to event delivery\n");
		printf("The error is %d\n",close_status);
		return FALSE;
	}
	gEventHandle = 0;
	sleep(3);
	printf("############ Notification subscriber remove  #######\n");
	return TRUE;
}
#endif

void G_MAIN_LOOP_RUN(int sec)
{
	(void) signal(SIGALRM, G_MAIN_TIMER_HANDLER);
	alarm(sec);
	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);
	alarm(0);
}

gboolean TimeOutFun(void *data)
{
	g_main_loop_quit(main_loop);
	printf("[TimeOutFun()] : g_main_loop_quit.\n");
	return FALSE;

}

int gmain_loop_for_events(int delay)
{
	main_loop = g_main_loop_new(NULL, FALSE);
	g_timeout_add(delay, TimeOutFun, NULL);
	g_main_loop_run(main_loop);

	return 0;
}
