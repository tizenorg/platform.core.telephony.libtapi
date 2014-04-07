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
#include "tapi.h"
#include "tapi_modem.h"

#include <tel_modem.h>
#include <string.h>

static gboolean __tapi_check_power_status(TelModemPowerStatus status)
{
	switch (status) {
	case TEL_MODEM_POWER_OFF:
	case TEL_MODEM_POWER_ON:
	case TEL_MODEM_POWER_ERROR:
		return TRUE;
	}

	return FALSE;
}

void on_modem_property_change_handler(TelephonyModem *modem,
	GVariant *changed_properties, const gchar *invalidated_properties,
	gpointer user_data)
{
	TapiEvtCbData *evt_cb_data;
	TelHandle *handle = user_data;
	GVariantIter *iter;
	const gchar *key;
	GVariant *value;

	if (handle == NULL)
		return;

	if (g_variant_n_children(changed_properties) == 0)
		return;

	dbg("Properties Changed:");

	g_variant_get(changed_properties, "a{sv}", &iter);
	while (g_variant_iter_loop(iter, "{sv}", &key, &value)) {
		char *evt_id;

		dbg("%s", key);

		evt_id = g_strdup_printf("%s:%s",
					TELEPHONY_MODEM_INTERFACE,
					key);
		/*
		 * If an event callback is registered process
		 * g-properties-changed event
		 */
		evt_cb_data = g_hash_table_lookup(handle->evt_table,
							evt_id);
		if (!evt_cb_data) {
			dbg("Application not registered on event %s",
							evt_id);
			g_free(evt_id);
			continue;
		}

		if (g_strcmp0("status", key) == 0) {
			TelModemPowerStatus status = g_variant_get_int32(value);
			dbg("value: %d", status);
			EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id,
						&status);
		} else if (g_strcmp0("flight_mode_status", key) == 0) {
			int fm = g_variant_get_int32(value);
			gboolean enable;

			switch (fm) {
			case TEL_MODEM_FLIGHT_MODE_ON:
				enable = TRUE;
				break;
			case TEL_MODEM_FLIGHT_MODE_OFF:
				enable = FALSE;
				break;
			case TEL_MODEM_FLIGHT_MODE_UNKNOWN:
			default:
				err("flight mode status error");
				continue;
			}
			dbg("value: %d", enable);
			EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id,
						&enable);

		} else if (g_strcmp0("imei", key) == 0) {
			char *imei = (char *)g_variant_get_string(value, NULL);

			if (imei)
				if (strlen(imei) > TEL_MODEM_IMEI_LENGTH_MAX) {
					err("Invalid IMEI length");
					return;
				}

			dbg("value: %s", imei);
			EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, imei);

		} else if (g_strcmp0("version", key) == 0) {
			TelModemVersion version = {{'\0'}, {'\0'}, {'\0'}, {'\0'}};
			GVariantIter *iter = NULL;
			GVariant *key_value;
			const gchar *key;

			g_variant_get(value, "a{sv}", &iter);
			while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "software_version") == 0) {
					g_strlcpy(version.software_version,
						g_variant_get_string(key_value, NULL),
						TEL_MODEM_VERSION_LENGTH_MAX);
				}

				if (g_strcmp0(key, "hardware_version") == 0) {
					g_strlcpy(version.hardware_version,
						g_variant_get_string(key_value, NULL),
						TEL_MODEM_VERSION_LENGTH_MAX);
				}

				if (g_strcmp0(key, "calibration_date") == 0) {
					g_strlcpy(version.calibration_date,
						g_variant_get_string(key_value, NULL),
						TEL_MODEM_VERSION_LENGTH_MAX);
				}

				if (g_strcmp0(key, "product_code") == 0) {
					g_strlcpy(version.product_code,
						g_variant_get_string(key_value, NULL),
						TEL_MODEM_VERSION_LENGTH_MAX);
				}
			}
			g_variant_iter_free(iter);

			EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id,
						&version);
		}

		g_variant_iter_free(iter);
		g_free(evt_id);
	}
}


static void on_response_modem_set_power_status(GObject *source_object,
						GAsyncResult *res,
						gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	int result = TEL_MODEM_RESULT_FAILURE;
	GError *error = NULL;

	telephony_modem_call_set_power_status_finish(handle->modem_proxy,
						&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_MODEM_RESULT_SUCCESS)
		err("Failed to set modem power status - result: [%d]", result);
	else
		dbg("set modem power status: [SUCCESS]");

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

/* Set modem power status to ON OFF or RESET */
EXPORT_API TelReturn tapi_modem_set_power_status(TelHandle *handle,
						TelModemPowerStatus status,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && __tapi_check_power_status(status)
				&& callback, TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_modem_call_set_power_status(handle->modem_proxy, status,
					NULL,
					on_response_modem_set_power_status,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_modem_set_flight_mode(GObject *source_object,
						GAsyncResult *res,
						gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	int result = TEL_MODEM_RESULT_FAILURE;
	GError *error = NULL;

	telephony_modem_call_set_flight_mode_finish(handle->modem_proxy,
							&result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_MODEM_RESULT_SUCCESS)
		err("Failed to set modem flight mode - result: [%d]", result);
	else
		dbg("set modem flight mode: [SUCCESS]");

	RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
}

EXPORT_API TelReturn tapi_modem_set_flight_mode(TelHandle *handle,
						gboolean enable,
						TapiResponseCb callback,
						void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback,
					TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_modem_call_set_flight_mode(handle->modem_proxy, enable,
					NULL,
					on_response_modem_set_flight_mode,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_modem_get_flight_mode(TelHandle *handle,
						gboolean *enable)
{
	int flight_mode_status;
	GError *error = NULL;
	TelReturn result = TEL_RETURN_SUCCESS;
	int modem_res;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle, TEL_RETURN_INVALID_PARAMETER);

	flight_mode_status =
		telephony_modem_get_flight_mode_status(handle->modem_proxy);

	switch (flight_mode_status) {
	case TEL_MODEM_FLIGHT_MODE_ON:
		*enable = TRUE;
		goto out;
	case TEL_MODEM_FLIGHT_MODE_OFF:
		*enable = FALSE;
		goto out;
	case TEL_MODEM_FLIGHT_MODE_UNKNOWN:
		/*
		 * Need to read state from modem directly to update DBus cached
		 * property.
		 */
		break;
	default:
		err("flight mode status error");
		result = TEL_RETURN_FAILURE;
		goto out;
	}

	telephony_modem_call_get_flight_mode_sync(handle->modem_proxy,
							&modem_res, enable,
							NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		result = TEL_RETURN_FAILURE;
		goto out;
	}

	if (modem_res != TEL_MODEM_RESULT_SUCCESS)
		result = TEL_RETURN_FAILURE;

out:
	return result;
}

EXPORT_API TelReturn tapi_modem_get_version(TelHandle *handle,
						TelModemVersion *version)
{
	GVariant *dbus_version;
	TelReturn result = TEL_RETURN_SUCCESS;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle, TEL_RETURN_INVALID_PARAMETER);

	/* dbus_version doesn't need to be g_variant_unref() */
	dbus_version = telephony_modem_get_version(handle->modem_proxy);
	if (dbus_version != NULL) {
		GVariantIter *iter = NULL;
		GVariant *key_value;
		const gchar *key;

		g_variant_get(dbus_version, "a{sv}", &iter);
		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "software_version") == 0) {
				g_strlcpy(version->software_version,
					g_variant_get_string(key_value, NULL),
					TEL_MODEM_VERSION_LENGTH_MAX + 1);
			}
			else if (g_strcmp0(key, "hardware_version") == 0) {
				g_strlcpy(version->hardware_version,
					g_variant_get_string(key_value, NULL),
					TEL_MODEM_VERSION_LENGTH_MAX + 1);
			}
			else if (g_strcmp0(key, "calibration_date") == 0) {
				g_strlcpy(version->calibration_date,
					g_variant_get_string(key_value, NULL),
					TEL_MODEM_VERSION_LENGTH_MAX + 1);
			}
			else if (g_strcmp0(key, "product_code") == 0) {
				g_strlcpy(version->product_code,
					g_variant_get_string(key_value, NULL),
					TEL_MODEM_VERSION_LENGTH_MAX + 1);
			}
		}
		g_variant_iter_free(iter);

		if (strlen(version->software_version)
				|| strlen(version->hardware_version)
				|| strlen(version->calibration_date)
				|| strlen(version->product_code)) {
			dbg("Version information - Software: [%s] "\
				"Hardware: [%s] Calibration date: [%d] "\
				"Product code: [%s]", version->software_version,
				version->hardware_version,
				version->calibration_date,
				version->product_code);
			return result;
		}
	}

	/* Fetch version information from Modem */
	{
		gchar *soft, *hard, *cal_date, *prod_code;
		gint modem_res;
		GError *error = NULL;

		telephony_modem_call_get_version_sync(handle->modem_proxy,
			&modem_res, &soft, &hard, &cal_date, &prod_code, NULL, &error);
		if (error) {
			dbg("dbus error = %d (%s)", error->code, error->message);
			g_error_free(error);
			result = TEL_RETURN_FAILURE;
			goto out;
		}

		if (modem_res != TEL_MODEM_RESULT_SUCCESS) {
			result = TEL_RETURN_FAILURE;
			goto out;
		}

		if (soft) {
			strncpy(version->software_version, soft,
				TEL_MODEM_VERSION_LENGTH_MAX);
			version->software_version[TEL_MODEM_VERSION_LENGTH_MAX] = '\0';
		}

		if (hard) {
			strncpy(version->hardware_version, hard,
				TEL_MODEM_VERSION_LENGTH_MAX);
			version->hardware_version[TEL_MODEM_VERSION_LENGTH_MAX] = '\0';
		}

		if (cal_date) {
			strncpy(version->calibration_date, cal_date,
				TEL_MODEM_VERSION_LENGTH_MAX);
			version->calibration_date[TEL_MODEM_VERSION_LENGTH_MAX] = '\0';
		}

		if (prod_code) {
			strncpy(version->product_code, prod_code,
				TEL_MODEM_VERSION_LENGTH_MAX);
			version->product_code[TEL_MODEM_VERSION_LENGTH_MAX] = '\0';
		}

		g_free(soft);
		g_free(hard);
		g_free(cal_date);
		g_free(prod_code);
	}

out:
	return result;
}

EXPORT_API TelReturn tapi_modem_get_imei(TelHandle *handle,
				char imei[TEL_MODEM_IMEI_LENGTH_MAX + 1])
{
	GError *error = NULL;
	char *dbus_imei;
	TelReturn result = TEL_RETURN_FAILURE;
	int modem_res;
	unsigned int imei_len;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle, TEL_RETURN_INVALID_PARAMETER);

	/* dbus_imei doesn't need to be g_free() */
	dbus_imei= (char *)telephony_modem_get_imei(handle->modem_proxy);
	if (dbus_imei != NULL && strlen(dbus_imei) > 0) {
		imei_len = strlen(dbus_imei);
		if (imei_len > TEL_MODEM_IMEI_LENGTH_MAX)
			err("Invalid IMEI length %d", imei_len);
		else {
			memcpy(imei, dbus_imei, imei_len + 1);
			result = TEL_RETURN_SUCCESS;
		}

		goto out;
	}

	/*
	 * dbus_imei needs to be g_free() because this function is using
	 * g_variant_get() internally.
	 */
	telephony_modem_call_get_imei_sync(handle->modem_proxy, &modem_res,
						&dbus_imei, NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		goto out;
	}

	if (modem_res != TEL_MODEM_RESULT_SUCCESS || dbus_imei == NULL)
		goto out;

	imei_len = strlen(dbus_imei);
	if (imei_len > TEL_MODEM_IMEI_LENGTH_MAX)
		err("Invalid IMEI length %d", imei_len);
	else {
		memcpy(imei, dbus_imei, imei_len + 1);
		result = TEL_RETURN_SUCCESS;
	}

	g_free(dbus_imei);

out:
	return result;
}


EXPORT_API TelReturn tapi_modem_get_power_status(TelHandle *handle,
						TelModemPowerStatus *status)
{
	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle, TEL_RETURN_INVALID_PARAMETER);

	*status = telephony_modem_get_status(handle->modem_proxy);

	return TEL_RETURN_SUCCESS;
}
