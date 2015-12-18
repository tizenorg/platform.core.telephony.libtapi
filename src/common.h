/*
 * libslp-tapi
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
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
#include <gio/gio.h>

__BEGIN_DECLS

#define TAPI_SHORT_TIMEOUT    (5 * 1000) /* Unlimit: G_MAXINT */
#define TAPI_DEFAULT_TIMEOUT    (60 * 1000) /* Unlimit: G_MAXINT */
#define TAPI_UNRESTRICTED_TIMEOUT    (180 * 1000)

#define TAPI_MAKE_RESP_CB_DATA(data,handle,cb,user_data)  \
	if (!handle) { return TAPI_API_INVALID_INPUT; } \
	data = g_new0(struct tapi_resp_data, 1); \
	data->handle = handle; \
	data->cb_fn = cb; \
	data->user_data = user_data

#define TAPI_RESP_CHECK_ERROR(error, evt_cb_data) do { \
	if (error && (evt_cb_data)) { \
		warn("dbus error: (%d) (%s)", error->code, error->message); \
		if (error->code == G_IO_ERROR_CANCELLED \
				&& error->domain == G_IO_ERROR) { \
			err("Operation CANCELLED"); \
			/* Do not invoke callback in case of deinit TapiHandle */ \
		} else if (strstr(error->message, "AccessDenied")) { \
			err("(%s) Access denied!", (evt_cb_data)->handle->cp_name); \
			if ((evt_cb_data)->cb_fn) \
				(evt_cb_data)->cb_fn((evt_cb_data)->handle, TAPI_ERROR_ACCESS_DENIED, NULL, (evt_cb_data)->user_data); \
		} else if (strstr(error->message, "Operation not supported")) { \
			err("(%s) Operation NOT supported!", evt_cb_data->handle->cp_name); \
			if ((evt_cb_data)->cb_fn) \
				(evt_cb_data)->cb_fn((evt_cb_data)->handle, TAPI_ERROR_OPERATION_NOT_SUPPORTED, NULL, (evt_cb_data)->user_data); \
		} else { \
			err("(%s) Operation Failed!", (evt_cb_data)->handle->cp_name); \
			if ((evt_cb_data)->cb_fn) \
				(evt_cb_data)->cb_fn((evt_cb_data)->handle, TAPI_ERROR_OPERATION_FAILED, NULL, (evt_cb_data)->user_data); \
		} \
		g_error_free(error); \
		g_free(evt_cb_data); \
		return; \
	} else { \
		if (evt_cb_data && (evt_cb_data)->handle) \
			dbg("Func Enterance. cp_name[%s]", (evt_cb_data)->handle->cp_name); \
	} \
} while (0)

#define TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, data) do { \
	if (evt_cb_data && (evt_cb_data)->cb_fn) \
		(evt_cb_data)->cb_fn((evt_cb_data)->handle, result, data, (evt_cb_data)->user_data); \
} while (0)

#define TAPI_INVOKE_NOTI_CALLBACK(data) do { \
	if (evt_cb_data->cb_fn) \
		evt_cb_data->cb_fn (handle, noti_id, (data), evt_cb_data->user_data); \
} while (0)

struct tapi_handle {
	gpointer dbus_connection;
	char *path;
	char *cp_name;
	GHashTable *evt_list;
	GCancellable *ca;

	GHashTable *cache_property;
	guint prop_callback_evt_id;
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

struct signal_map {
	const char *signal_name;
	void (*callback)(TapiHandle *handle, GVariant *param, char *noti_id,
			struct tapi_evt_cb *evt_cb_data);
};

void _process_network_event(const gchar *sig, GVariant *param,
	TapiHandle *handle, char *noti_id, struct tapi_evt_cb *evt_cb_data);

__END_DECLS

#endif /* _TAPI_COMMON_H_ */

/**
 *  @}
 */
