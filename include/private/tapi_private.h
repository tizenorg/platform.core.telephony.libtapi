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

#ifndef __TAPI_PRIVATE_H__
#define __TAPI_PRIVATE_H__

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "tapi_log.h"
#include "tapi_events.h"

#include "generated-code.h"

#define TAPI_DBUS_TIMEOUT_DEFAULT		(60000)
#define TAPI_DBUS_TIMEOUT_MAX			(180000)

#define TEL_RETURN_IF_CHECK_FAIL(expr, ret) \
	do { \
		if (!(expr)) \
			return ret; \
	} while(0)

#define MAKE_RESP_CB_DATA(data, handle, cb, user_data)  \
	data = g_new0(TapiRespCbData, 1); \
	data->handle = handle; \
	data->resp_cb = cb; \
	data->user_data = user_data

#define MAKE_EVT_CB_DATA(data, cb, user_data)  \
	data = g_new0(TapiEvtCbData, 1); \
	data->evt_cb = cb; \
	data->user_data = user_data

#define RESP_CALLBACK_CALL(resp_cb_data, tel_result, result_data) \
	if (resp_cb_data && resp_cb_data->resp_cb) \
		resp_cb_data->resp_cb(resp_cb_data->handle, tel_result, \
					result_data, resp_cb_data->user_data); \
	g_free(resp_cb_data)

#define CHECK_DEINIT(error, resp_cb_data, tel_result) \
	if (error) { \
		dbg("dbus error = %d (%s)", error->code, error->message); \
		 if (error->code == G_IO_ERROR_CANCELLED \
				&& error->domain == G_IO_ERROR) { \
			g_error_free(error); \
			g_free(resp_cb_data); \
			return; \
		} \
		RESP_CALLBACK_CALL(resp_cb_data, tel_result, NULL); \
		g_error_free(error); \
		return; \
	}

#define EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, data) \
	if (evt_cb_data->evt_cb) \
		evt_cb_data->evt_cb(handle, evt_id, data, evt_cb_data->user_data)

#define GET_TAPI_HANDLE(data) data->handle

/**
 *  @private
 */
struct tapi_handle {
	TelephonyModem *modem_proxy;	/* Modem proxy created with GDBus helper */
	TelephonyCall *call_proxy;
	TelephonySms *sms_proxy;
	TelephonySim *sim_proxy;
	TelephonyNetwork *network_proxy;
	TelephonySap *sap_proxy;
	TelephonyGps *gps_proxy;
	TelephonyPhonebook *phonebook_proxy;
	TelephonySs *ss_proxy;
	TelephonySat *sat_proxy;

	int modem_property_handler;
	int network_property_handler;
	int sim_property_handler;
	int sms_property_handler;

	int network_signal_handler;
	int phonebook_signal_handler;
	int call_signal_handler;
	int ss_signal_handler;
	int sms_signal_handler;
	int sat_signal_handler;
	int sap_signal_handler;
	int gps_signal_handler;

	char *cp_name;
	GHashTable *evt_table;
};

/**
 * This structure defines the tapi response callback data
 */
typedef struct {
	struct tapi_handle *handle; /**< Handle */
	TapiResponseCb resp_cb; /**< Response Callback Type*/
	void* user_data; /**< User Data */
} TapiRespCbData;

/**
 * This structure defines the tapi event callback data
 */
typedef struct {
	TapiEvtCb evt_cb; /**< Event Callback Type */
	void *user_data; /**< User Data */
} TapiEvtCbData;

#endif /* __TAPI_PRIVATE_H__ */
