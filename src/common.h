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

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

__BEGIN_DECLS

#define MAKE_RESP_CB_DATA(data,handle,cb,user_data)  \
	if (!handle) { return TAPI_API_INVALID_INPUT; } \
	data = g_new0(struct tapi_resp_data, 1); \
	data->handle = handle; \
	data->cb_fn = cb; \
	data->user_data = user_data

struct tapi_handle {
	gpointer dbus_connection;
	char *path;
	char *cp_name;
	GHashTable *evt_list;
	char cookie[20];
};

struct tapi_resp_data {
	tapi_response_cb cb_fn;
	void* user_data;
	struct tapi_handle *handle;
};

struct tapi_evt_cb {
	int evt_id;
	tapi_notification_cb cb_fn;
	void* user_data;
	struct tapi_handle *handle;
};

__END_DECLS

#endif /* _TAPI_COMMON_H_ */

/**
 *  @}
 */
