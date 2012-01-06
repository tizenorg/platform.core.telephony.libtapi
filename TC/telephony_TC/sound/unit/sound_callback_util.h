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

#ifndef _SOUND_CALLBACK_UTIL_H_
#define _SOUND_CALLBACK_UTIL_H_


#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include "string.h"
#include "assert.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <glib.h>
#include <pthread.h>
#include <stdio.h>

#include <ITapiSound.h>
#include <tet_api.h>
#include "tet_macro.h"
#include <gconf/gconf-client.h>
#include <ITapiCall.h>
#include <TapiCommon.h>
#include <TelErr.h>


#define TIME_DELAY 5000

extern int app_sound_callback (const TelTapiEvent_t * TapiEvent);

extern TapiResult_t register_all_events();
extern  TapiResult_t deregister_all();
void print_all_subsciption_ids();
extern int gmain_loop_for_tapi_events(int delay);
extern gboolean TimeOutFun (void *data);

#endif //_SOUND_CALLBACK_UTIL_H_
