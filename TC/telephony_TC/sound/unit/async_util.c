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


#include "sound_callback_util.h"
#include "async_util.h"

static void G_MAIN_TIMER_HANDLER(int sgi);

// Global variable for G_MAIN system
static int gEventHandle=0;
static GMainLoop * context_loop = NULL;
static EvtAlarmCb gpAlarm_Handler = NULL;
static unsigned int gSubscription_id;

void G_MAIN_LOOP_CLOSE()
{
    g_main_loop_quit(context_loop);
}

void G_MAIN_TIMER_HANDLER(int sgi)
{
    if (gpAlarm_Handler)
    	gpAlarm_Handler(sgi);
    else
    {
    	fprintf(stderr, "SIGALRM received. But user handler not registered. Close g_main_loop()");
    	G_MAIN_LOOP_CLOSE();
    }
}

void G_MAIN_LOOP_RUN(int sec)
{
    (void) signal(SIGALRM, G_MAIN_TIMER_HANDLER);
    alarm(sec);
    context_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(context_loop);
    alarm(0);
}
