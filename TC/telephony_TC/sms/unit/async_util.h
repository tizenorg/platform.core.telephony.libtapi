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

#ifndef _ASYNC_UTIL_H_
#define _ASYNC_UTIL_H_

/*
 G_MAIN event system
 */
#include <sys/types.h>
#include <signal.h>
#include <glib.h>
#include <glib/gbase64.h>
#include <TelDefines.h>

typedef void (*EvtAlarmCb)(int sgi);
TS_BOOL gAsyncResult;

// Prototype for G_MAIN system
//TS_BOOL G_MAIN_INIT(EvtSysEventCb pNotiHandler, EvtAlarmCb pAlarmHandler);
//TS_BOOL G_MAIN_CLOSE();
void G_MAIN_LOOP_RUN(int sec);
void G_MAIN_LOOP_CLOSE();
int gmain_loop_for_events(int delay);
gboolean TimeOutFun(void *data);

#endif
