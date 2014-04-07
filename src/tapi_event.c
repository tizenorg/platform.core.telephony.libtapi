/*
 * libtapi
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd. All rights reserved.
 * Copyright (c) 2013 Intel Corporation. All rights reserved.
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

#include "tapi_private.h"
#include "tapi_events.h"

EXPORT_API TelReturn tapi_register_event_id(TelHandle *handle,
						const char *evt_id,
						TapiEvtCb callback,
						void *user_data)
{
	TapiEvtCbData *evt_cb_data;
	gchar **dbus_str;

	if (handle == NULL || callback == NULL || evt_id == NULL) {
		err("Invalid input parameters");
		return TEL_RETURN_INVALID_PARAMETER;
	}

	dbus_str = g_strsplit(evt_id, ":", 2);
	if (!dbus_str) {
		dbg("invalid evt_id");
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if (!dbus_str[0] || !dbus_str[1]) {
		g_strfreev(dbus_str);
		dbg("invalid evt_id");
		return TEL_RETURN_INVALID_PARAMETER;
	}

	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data != NULL) {
		dbg("evt_id(%s) is already registered", evt_id);
		g_strfreev(dbus_str);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_EVT_CB_DATA(evt_cb_data, callback, user_data);

	dbg("path(%s) interface (%s) signal (%s)", handle->cp_name, dbus_str[0], dbus_str[1]);

	g_hash_table_insert(handle->evt_table, g_strdup(evt_id), evt_cb_data);

	g_strfreev(dbus_str);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_deregister_event_id(TelHandle *handle, const char *evt_id)
{
	gboolean rv = FALSE;

	if (handle == NULL) {
		err("invalid parameter");
		return TEL_RETURN_INVALID_PARAMETER;
	}

	rv = g_hash_table_remove(handle->evt_table, evt_id);
	if (!rv) {
		err("fail to deregister noti event(%s)", evt_id);
		return TEL_RETURN_FAILURE;
	}

	return TEL_RETURN_SUCCESS;
}
