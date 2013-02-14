/*
 * libslp-tapi
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Ja-young Gu <jygu@samsung.com>
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

#ifndef _TAPI_COMMON_H_
#define _TAPI_COMMON_H_

#include <stdio.h>
#include <glib.h>

#include <tapi_type.h>
#include <tapi_event.h>

__BEGIN_DECLS


char**       tel_get_cp_name_list(void);

TapiHandle*  tel_init(const char *cp_name);
int          tel_deinit(TapiHandle *handle);

int          tel_register_noti_event(TapiHandle *handle, const char *noti_id,
                 tapi_notification_cb callback, void *user_data);
int          tel_deregister_noti_event(TapiHandle *handle, const char *noti_id);

int          tel_get_property_int(TapiHandle *h, const char *property, int *result);

__END_DECLS

#endif /* _TAPI_COMMON_H_ */

/**
 *  @}
 */
