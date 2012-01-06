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

#ifndef	__UTS_TAPI_COMMON_H_
#define   __UTS_TAPI_COMMON_H_

/*  Includes */

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <glib-2.0/glib.h>
#include <pthread.h>
#include <stdio.h>

#include <ITapiCall.h>
#include <TapiCommon.h>



#define CALL_TEST_DEBUG(fmt, args...)	\
	printf("[%s %s:%d]\n" fmt "\n",__FILE__, __FUNCTION__,  __LINE__, ##args); 

extern int gmain_loop_for_tapi_events(int delay);

extern void  app_callback  (TelTapiEvent_t * TapiEvent);
extern gboolean TimeOutFun (void *data);


extern TapiResult_t register_all_events();

extern TapiResult_t deregister_all();

void print_all_subsciption_ids();


#endif
