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



#ifndef _GPRS_COMMON_H_
#define _GPRS_COMMON_H_

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
#include "ITapiPS.h"
#include <tet_api.h>
#include "tet_macro.h"
#include <gconf/gconf-client.h>
#include <TapiCommon.h>

#define APN_ADDRESS "web.vodafone.de"
#define CONTEXT_ID	1

#define APN_ADDRESS_2 "web.vodafone.de"
#define CONTEXT_ID_2	2


#define TLOG_HEADER(fmt, args...) printf("[1;7m<%s:%4d> " fmt "[0m\n", __FILE__, __LINE__, ##args);
#define TLOG_INFO(fmt, args...)   printf("<%s:%4d> " fmt "\n", __FILE__, __LINE__, ##args);

#define TIME_DELAY 5000
#define TEST_DEBUG(fmt, args...)	\
	printf("[%s %s:%d]\n" fmt "\n",__FILE__, __FUNCTION__,  __LINE__, ##args); 

#ifndef false
	#define false 0
#endif
#ifndef true
	#define true  !0
#endif
#ifndef bool    
	#define bool  char
#endif

extern void  ps_app_callback  (TelTapiEvent_t *TapiEvent);
extern TapiResult_t register_all_events();
extern TapiResult_t deregister_all();
extern void G_MAIN_LOOP_CLOSE();
extern int G_MAIN_LOOP_RUN(int);


extern TapiResult_t temp_gprs_session_deactivate();
extern TapiResult_t temp_gprs_session_activate();

#define TET_CHECK_TEMP(x1, y...) \
	do { \
		TapiResult_t err = y;\
		if (err != (x1)) \
		{ \
			tet_printf("[line %d] Expected Result: %d  Actual Result: %d",__LINE__, x1,y); \
			printf("- FAIL. [line %d] Expected Result: %d  Actual Result: %d\n",__LINE__, x1,y); \
			tet_result(TET_FAIL); \
			return;\
		} \
	} while(0)


#endif  //_GPRS_COMMON_H_
