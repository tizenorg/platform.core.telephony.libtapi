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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tapi_common.h"
#include "TapiUtility.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiOem.h"
#include "TelOem.h"
#include "tapi_private.h"

#define OEM_DATA_LEN_MAX	(10 * 1024)
#define OEM_DEFAULT_TIMEOUT	(30 * 1000)

struct oem_resp_data {
	struct tapi_handle *handle;
	void* user_data;
	tapi_response_cb cb_fn;
	int oem_id;
};

#define MAKE_OEM_RESP_CB_DATA(data, handle, cb, user_data, oem_id)  do { \
	if (!handle) \
		return TAPI_API_INVALID_INPUT; \
	\
	data = g_try_malloc0(sizeof(struct oem_resp_data)); \
	if (data == NULL) \
		return TAPI_API_SYSTEM_OUT_OF_MEM; \
	data->handle = handle; \
	data->cb_fn = cb; \
	data->user_data = user_data; \
	data->oem_id = oem_id; \
} while (0)

/* LCOV_EXCL_START */
static void on_response_oem_data_async(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	struct oem_resp_data *evt_cb_data = user_data;
	int result = -1;
	GVariant *dbus_result;
	TelOemData_t oem_data = {0};
	gchar *data = NULL;

	dbus_result = g_dbus_connection_call_finish(G_DBUS_CONNECTION(source_object), res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	g_variant_get(dbus_result, "(is)", &oem_data.oem_id, &data);

	oem_data.data = g_base64_decode((const gchar *)data, (gsize *)&oem_data.data_len);
	if (oem_data.data) {
		result = TAPI_API_SUCCESS;

		dbg("id:[0x%d] len:[%d]", oem_data.oem_id, oem_data.data_len);

		TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &oem_data);

		g_free(oem_data.data);
	} else {
		TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, NULL);
	}

	g_free(data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

EXPORT_API int tel_send_oem_data(TapiHandle *handle, int oem_id,
	void *data, unsigned int data_len)
{
	GError *error = NULL;
	GVariant *rst = NULL;
	GVariant *inparam = NULL;
	gint result;
	gchar *encoded_data = NULL;

	dbg("oem_id(0x%x) data(%p) data len(%d)", oem_id, data, data_len);

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(handle->dbus_connection, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(data, TAPI_API_INVALID_PTR);

	if (data_len > OEM_DATA_LEN_MAX)
		return TAPI_API_INVALID_INPUT;

	encoded_data = g_base64_encode((const guchar *)data, data_len);
	inparam = g_variant_new("(is)", oem_id, encoded_data);

	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_OEM_INTERFACE,
		"SendOemData", inparam, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca, &error);
	g_free(encoded_data);
	if (!rst) {
		dbg("error to data(%s)", error->message);
		g_error_free(error);
		return TAPI_API_OPERATION_FAILED;
	}

	g_variant_get(rst, "(i)", &result);
	if (result) {
		result = TAPI_API_SUCCESS;
	} else {
		dbg("error (%d)", result);
		result = TAPI_API_OPERATION_FAILED;
	}
	g_variant_unref(rst);
	return result;
}

EXPORT_API int tel_send_oem_data_sync(TapiHandle *handle, int oem_id,
	void *data, unsigned int data_len, TelOemData_t *outparam)
{
	GError *error = NULL;
	GVariant *inparam = NULL;
	gchar *encoded_data = NULL;
	GVariant *rst = NULL;
	gchar *pdata = NULL;

	dbg("oem_id(0x%x) data(%p) data len(%d)", oem_id, data, data_len);

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(handle->dbus_connection, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(data, TAPI_API_INVALID_PTR);

	if (data_len > OEM_DATA_LEN_MAX)
		return TAPI_API_INVALID_INPUT;

	encoded_data = g_base64_encode((const guchar *)data, data_len);
	inparam = g_variant_new("(is)", oem_id, encoded_data);

	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_OEM_INTERFACE,
		"SendOemDataWithResponse", inparam, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca, &error);
	g_free(encoded_data);

	if (!rst) {
		dbg("error to data(%s)", error->message);
		g_error_free(error);
		return TAPI_API_OPERATION_FAILED;
	}

	g_variant_get(rst, "(is)", &outparam->oem_id, &pdata);

	outparam->data = g_base64_decode((const gchar *)pdata, (gsize *)&outparam->data_len);
	if (outparam->data_len)
		dbg("id:[0x%x] len:[%d]", outparam->oem_id, outparam->data_len);

	g_free(pdata);
	g_variant_unref(rst);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_send_oem_data_async(TapiHandle *handle, int oem_id,
	void *data, unsigned int data_len, tapi_response_cb callback, void *user_data)
{
	GVariant *inparam = NULL;
	gchar *encoded_data = NULL;
	struct oem_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_NOT_SUPPORTED(TELEPHONY_FEATURE);
	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(handle->dbus_connection, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(data, TAPI_API_INVALID_PTR);

	if (data_len > OEM_DATA_LEN_MAX)
		return TAPI_API_INVALID_INPUT;

	dbg("oem_id(0x%x) data(%p) data len(%d)", oem_id, data, data_len);

	MAKE_OEM_RESP_CB_DATA(evt_cb_data, handle, callback, user_data, oem_id);

	encoded_data = g_base64_encode((const guchar *)data, data_len);
	inparam = g_variant_new("(is)", oem_id, encoded_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_OEM_INTERFACE,
		"SendOemDataWithResponse", inparam, NULL, G_DBUS_CALL_FLAGS_NONE,
		OEM_DEFAULT_TIMEOUT, handle->ca, on_response_oem_data_async, evt_cb_data);

	g_free(encoded_data);

	return TAPI_API_SUCCESS;
}
/* LCOV_EXCL_STOP */
