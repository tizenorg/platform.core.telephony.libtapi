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


#ifndef UTS_TAPI_COMMON_H
#define UTS_TAPI_COMMON_H

#include <stdio.h>
#include <glib.h>

GMainLoop *main_loop = NULL;

#define TIME_DELAY 5000

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
#endif
