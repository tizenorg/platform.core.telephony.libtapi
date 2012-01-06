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
#include <stdlib.h>
#include <string.h>

#include <TelNetwork.h>
#include <glib.h>

#define TET_CHECK(x1, y...) \
 { \
   TapiResult_t err = y;\
   if (err != (x1)) \
        { \
                tet_printf("%s[%d] : test fail. error code =%d ",__FILE__,__LINE__, err); \
                tet_result(TET_FAIL); \
                return;\
        } \
 }

extern void GetNetworkInfoCB(void *);
extern void SearchNetworkCB(void *);
extern void SelectNetworkCB(void *);
extern void GetSelectionModeCB(void *);
extern int gmain_loop_for_tapi_events(void);
extern int tapi_callback(const TelTapiEvent_t *tapievent, void *data);

