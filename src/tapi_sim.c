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
#include "tapi_log.h"

#include "tapi_sim.h"

#define CHECK_IF_SIM_PRESENT(status) \
do { \
	switch (status) { \
	case TEL_SIM_STATUS_UNKNOWN: \
	case TEL_SIM_STATUS_CARD_ERROR: \
	case TEL_SIM_STATUS_CARD_NOT_PRESENT: \
	case TEL_SIM_STATUS_CARD_REMOVED: \
		err("SIM NOT present"); \
		return TEL_RETURN_FAILURE; \
	default: \
		break; \
	} \
} while (0)

#define CHECK_IF_SIM_INITIALIZED(status) \
do { \
	if (status != TEL_SIM_STATUS_SIM_INIT_COMPLETED) { \
		err("SIM init NOT completed!!!"); \
		return TEL_RETURN_FAILURE; \
	} \
} while (0)

#define CHECK_LANGUAGE_VALID(language) \
do { \
	switch (language) { \
		case TEL_SIM_LP_GERMAN: \
		case TEL_SIM_LP_ENGLISH: \
		case TEL_SIM_LP_ITALIAN: \
		case TEL_SIM_LP_FRENCH: \
		case TEL_SIM_LP_SPANISH: \
		case TEL_SIM_LP_DUTCH: \
		case TEL_SIM_LP_SWEDISH: \
		case TEL_SIM_LP_DANISH: \
		case TEL_SIM_LP_PORTUGUESE: \
		case TEL_SIM_LP_FINNISH: \
		case TEL_SIM_LP_NORWEGIAN: \
		case TEL_SIM_LP_GREEK: \
		case TEL_SIM_LP_TURKISH: \
		case TEL_SIM_LP_HUNGARIAN: \
		case TEL_SIM_LP_POLISH: \
		case TEL_SIM_LP_KOREAN: \
		case TEL_SIM_LP_CHINESE: \
		case TEL_SIM_LP_RUSSIAN: \
		case TEL_SIM_LP_JAPANESE: \
			break; \
		default: \
			return TEL_RETURN_INVALID_PARAMETER; \
	} \
} while (0)

static void __get_sim_status_info(TelephonySim *sim_proxy,
	TelSimCardStatusInfo *status_info)
{
	GVariant *var = telephony_sim_get_card_status(sim_proxy);
	if (var) {
		GVariantIter *iter = NULL;
		GVariant *key_value;
		const gchar *key;

		g_variant_get(var, "a{sv}", &iter);
		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "status") == 0) {
				status_info->status = g_variant_get_int32(key_value);
			}
			else if (g_strcmp0(key, "change_status") == 0) {
				status_info->change_status = g_variant_get_int32(key_value);
			}
		}
		g_variant_iter_free(iter);
	}
	else {
		err("var is NULL");
		status_info->status = TEL_SIM_STATUS_UNKNOWN;
		status_info->change_status = TEL_SIM_CHANGE_STATUS_UNKNOWN;
	}
}

void on_sim_property_change_handler(TelephonySim *sim,
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
					TELEPHONY_SIM_INTERFACE,
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

		if (g_strcmp0("card_status", key) == 0) {
			TelSimCardStatusInfo status_info = {0, };

			__get_sim_status_info(sim, &status_info);
			dbg("SIM status changed!! SIM status: [%d], Card changed: [%d]",
				status_info.status, status_info.change_status);

			EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &status_info);
		}

		g_free(evt_id);
	}
	g_variant_iter_free(iter);
}

EXPORT_API TelReturn tapi_sim_get_init_info(TelHandle *handle, TelSimCardStatusInfo *status_info)
{
	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && status_info, TEL_RETURN_INVALID_PARAMETER);

	/* Fetch SIM Init status and Card changed information from DBUS property */
	__get_sim_status_info(handle->sim_proxy, status_info);

	dbg("SIM - Status: [%d] Change status: [%d]",
		status_info->status, status_info->change_status);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_sim_get_type(TelHandle *handle, TelSimCardType *card_type)
{
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && card_type, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	/* Fetch Card type from DBUS property */
	*card_type = telephony_sim_get_sim_type(handle->sim_proxy);
	dbg("SIM Card type: [%s]",
		(*card_type == TEL_SIM_CARD_TYPE_GSM ? "GSM"
		: (*card_type == TEL_SIM_CARD_TYPE_USIM ? "USIM"
		: (*card_type == TEL_SIM_CARD_TYPE_RUIM ? "RUSM"
		: (*card_type == TEL_SIM_CARD_TYPE_ISIM ? "ISIM"
		: "UNKNOWN")))));


	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_sim_get_imsi(TelHandle *handle, TelSimImsiInfo *imsi)
{
	const gchar *mcc = NULL, *mnc = NULL, *msin = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;
	GError *error = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && imsi, TEL_RETURN_INVALID_PARAMETER);

	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	/* Fetch MCC, MNC and MSIN from DBUS property */
	mcc = telephony_sim_get_mcc(handle->sim_proxy);
	if (mcc && strlen(mcc)) {
		mnc = telephony_sim_get_mnc(handle->sim_proxy);
		if (mnc && strlen(mnc)) {
			msin = telephony_sim_get_msin(handle->sim_proxy);
			if (msin && strlen(msin)) {
				goto imsi_fetched;
			}
		}
	}

	telephony_sim_call_get_imsi_sync(handle->sim_proxy,
		&sim_result, (gchar **)&mcc, (gchar **)&mnc, (gchar **)&msin,
		NULL, &error);
	if (error) {
		err("DBUS Error - Code: [%d] Message: [%s], ", error->code, error->message);
		g_error_free(error);
		return TEL_SIM_RESULT_FAILURE;
	}
	else if (sim_result != TEL_SIM_RESULT_SUCCESS) {
		err("Failed to get IMSI - sim_result: [%d]", sim_result);
		return TEL_SIM_RESULT_FAILURE;
	}

imsi_fetched:
	/* Update MCC, MNC and MSIN */
	TEL_RETURN_IF_CHECK_FAIL((strlen(mcc) < TEL_SIM_MCC_MNC_LEN_MAX)
		/* MNC can be 2 or 3 digits, considering the lower limit */
		|| (strlen(mnc) < (TEL_SIM_MCC_MNC_LEN_MAX - 1))
		|| (strlen(msin) <= TEL_SIM_MSIN_LEN_MAX), TEL_SIM_RESULT_FAILURE);

	memcpy(imsi->mcc, mcc, strlen(mcc)+1);
	memcpy(imsi->mnc, mnc, strlen(mnc)+1);
	memcpy(imsi->msin, msin, strlen(msin)+1);

	dbg("MCC: [%s] MNC: [%s] MSIN: [%s]", imsi->mcc, imsi->mnc, imsi->msin);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelReturn tapi_sim_get_ecc(TelHandle *handle, TelSimEccList *ecc)
{
	GVariant *ecc_list = NULL;
	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && ecc, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	telephony_sim_call_get_ecc_sync(handle->sim_proxy,
					&sim_result, &ecc->count, &ecc_list,
					NULL, &error);
	if (error) {
		err("DBUS Error - Code: [%d] Message: [%s], ", error->code, error->message);

		/* Free 'error' */
		g_error_free(error);

		return TEL_RETURN_FAILURE;
	}

	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get ECC list - sim_result: [%d]", sim_result);

		/* Free resources */
		g_variant_unref(ecc_list);

		return TEL_RETURN_FAILURE;
	}

	if ((ecc->count != 0)
			&& (ecc->count <= TEL_SIM_ECC_LIST_MAX)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("ECC list count: [%d]", ecc->count);

		g_variant_get(ecc_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "number") == 0) {
					g_strlcpy(ecc->list[i].number,
						g_variant_get_string(key_value, NULL),
						TEL_SIM_ECC_NUMBER_LEN_MAX + 1);
				}

				if (g_strcmp0(key, "name") == 0) {
					g_strlcpy(ecc->list[i].name,
						g_variant_get_string(key_value, NULL),
						TEL_SIM_ECC_TEXT_MAX + 1);
				}

				if (g_strcmp0(key, "category") == 0) {
					ecc->list[i].category = g_variant_get_int32(key_value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		warn("ECC list count is NOT valid - Count: [%d]", ecc->count);
		ecc->count = 0;
	}

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_iccid(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	gchar *iccid = NULL;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	dbg("Entry");

	/* Finish and extract the response data */
	telephony_sim_call_get_iccid_finish(handle->sim_proxy,
					&sim_result, &iccid, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM ICCID - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	/* Sanity check for ICCID */
	if (strlen(iccid) > TEL_SIM_ICCID_LEN_MAX) {
		err("Received invalid ICCID: [%s] Length: [%d]", iccid, strlen(iccid));
		sim_result = TEL_SIM_RESULT_FAILURE;
		g_free(iccid);			/* Send NULL iccid to in failure case */
		iccid = NULL;
	}
	else {
		dbg("Received ICCID: [%s] Length: [%d]", iccid, strlen(iccid));
	}

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, iccid);

	/* Free resources */
	g_free(iccid);
}

EXPORT_API TelReturn tapi_sim_get_iccid(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);

	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (ICCID) */
	telephony_sim_call_get_iccid(handle->sim_proxy,
		NULL, on_response_sim_get_iccid, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_language(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	gint language;	/* aligned to TelSimLanguagePreferenceCode */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	/* Finish and extract the response data */
	telephony_sim_call_get_language_finish(handle->sim_proxy,
					&sim_result, &language, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM Language - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	dbg("Language: [0x%02x]", language);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &language);
}

EXPORT_API TelReturn tapi_sim_get_language(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);

	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (Language) */
	telephony_sim_call_get_language(handle->sim_proxy,
			NULL, on_response_sim_get_language, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_set_language(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	/* Finish and extract the response data */
	telephony_sim_call_set_language_finish(handle->sim_proxy,
					&sim_result, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);

	if (TEL_SIM_RESULT_SUCCESS != sim_result)
		err("Failed to set SIM Language - sim_result: [%d]", sim_result);
	else
		dbg("SIM Language set");

	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
}

EXPORT_API TelReturn tapi_sim_set_language(TelHandle *handle,
	TelSimLanguagePreferenceCode language, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);

	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	CHECK_LANGUAGE_VALID(language);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - SET (Language) */
	telephony_sim_call_set_language(handle->sim_proxy,
			language, NULL, on_response_sim_set_language, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_callforwarding_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *cf_list = NULL;
	TelSimCfisList cf_info;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&cf_info, 0x0, sizeof(TelSimCfisList));

	/* Finish and extract the response data */
	telephony_sim_call_get_call_forwarding_info_finish(handle->sim_proxy,
		&sim_result, &cf_info.profile_count, &cf_list, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM Call forwarding info - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_variant_unref(cf_list);

		return;
	}

	/* Validate Response data */
	/**
	 * The Profile Identity shall be between 1 and 4 as defined
	 * in TS 23.097 for MSP
	 */
	if ((cf_info.profile_count != 0)
			&& (cf_info.profile_count <= TEL_SIM_MSP_CNT_MAX)
			&& (g_variant_n_children(cf_list) == cf_info.profile_count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Call forwarding list count: [%d]", cf_info.profile_count);

		g_variant_get(cf_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "profile_id") == 0) {
					cf_info.cf[i].profile_id = g_variant_get_byte(key_value);
				}

				if (g_strcmp0(key, "indication") == 0) {
					memcpy(cf_info.cf[i].indication,
						g_variant_get_data(key_value),
						TEL_SIM_CALL_FORWARDING_TYPE_MAX);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		err("Invalid Profile count: [%d]", cf_info.profile_count);

		cf_info.profile_count = 0;
		sim_result = TEL_SIM_RESULT_FAILURE;
	}

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &cf_info);
}

EXPORT_API TelReturn tapi_sim_get_callforwarding_info(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);

	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (Call Forwarding info) */
	telephony_sim_call_get_call_forwarding_info(handle->sim_proxy,
		NULL, on_response_sim_get_callforwarding_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_messagewaiting_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *mw_list = NULL;
	TelSimMwisList mw_info;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&mw_info, 0x0, sizeof(TelSimMwisList));

	/* Finish and extract the response data */
	telephony_sim_call_get_message_waiting_info_finish(handle->sim_proxy,
		&sim_result, &mw_info.profile_count, &mw_list, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM Message Waiting info - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_variant_unref(mw_list);

		return;
	}

	/* Validate Response data */
	/**
	 * The Profile Identity shall be between 1 and 4 as defined
	 * in TS 23.097 for MSP
	 */
	if ((mw_info.profile_count != 0)
			&& (mw_info.profile_count <= TEL_SIM_MSP_CNT_MAX)
			&& (g_variant_n_children(mw_list) == mw_info.profile_count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Message Waiting list count: [%d]", mw_info.profile_count);

		g_variant_get(mw_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "profile_id") == 0) {
					mw_info.mw[i].profile_id = g_variant_get_byte(key_value);
				}
				else if (g_strcmp0(key, "count_valid") == 0) {
					mw_info.mw[i].count_valid = g_variant_get_boolean(key_value);
				}
				else if (g_strcmp0(key, "msg_waiting") == 0) {
					GVariantIter *iter2 = NULL, *iter_row2 = NULL;
					GVariant *key_value2;
					const gchar *key2;
					guint j = 0;

					g_variant_get(key_value, "aa{sv}", &iter2);
					while (g_variant_iter_next(iter2, "a{sv}", &iter_row2)) {
						while (g_variant_iter_loop(iter_row2, "{sv}", &key2, &key_value2)) {
							if (g_strcmp0(key2, "indication") == 0) {
								mw_info.mw[i].msg_waiting[j].indication =
									g_variant_get_boolean(key_value2);
							}
							else if (g_strcmp0(key2, "count") == 0) {
								mw_info.mw[i].msg_waiting[j].count =
									g_variant_get_byte(key_value2);
							}
						}
						j++;
						g_variant_iter_free(iter_row2);
					}
					g_variant_iter_free(iter2);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		err("Invalid Profile count: [%d]", mw_info.profile_count);
		mw_info.profile_count = 0;
		sim_result = TEL_SIM_RESULT_FAILURE;
	}

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &mw_info);
}

EXPORT_API TelReturn tapi_sim_get_messagewaiting_info(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);

	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (Message Waiting info) */
	telephony_sim_call_get_message_waiting_info(handle->sim_proxy,
		NULL, on_response_sim_get_messagewaiting_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_set_messagewaiting_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	/* Finish and extract the response data */
	telephony_sim_call_set_message_waiting_info_finish(handle->sim_proxy,
					&sim_result, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);

	dbg("Set Message waiting info: [%s]",
			(sim_result == TEL_SIM_RESULT_SUCCESS ? "SUCCESS" : "FAIL"));

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
}

EXPORT_API TelReturn tapi_sim_set_messagewaiting_info(TelHandle *handle,
	TelSimMwis *req_mw, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	GVariant *var_mw;
	GVariantBuilder builder;
	unsigned int i;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && req_mw && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	dbg("Profile ID: [%d] ", req_mw->profile_id);

	g_variant_builder_init(&builder, G_VARIANT_TYPE("a(by)"));
	for (i = 0; i < TEL_SIM_MAILBOX_TYPE_MAX; i++) {
		g_variant_builder_add(&builder, "(by)",
				req_mw->msg_waiting[i].indication,
				req_mw->msg_waiting[i].count);
	}

	var_mw = g_variant_builder_end(&builder);

	/* DBUS Request - SET (Message Waiting info) */
	telephony_sim_call_set_message_waiting_info(handle->sim_proxy,
			req_mw->profile_id, req_mw->count_valid, var_mw, NULL,
			on_response_sim_set_messagewaiting_info, rsp_cb_data);

	g_variant_unref(var_mw);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_mailbox_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *mb_list = NULL;
	TelSimMailboxList mb_info;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&mb_info, 0x0, sizeof(TelSimMailboxList));

	/* Finish and extract the response data */
	telephony_sim_call_get_mailbox_info_finish(handle->sim_proxy,
		&sim_result, &mb_info.alpha_id_max_len, &mb_info.count, &mb_list,
		res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM Mailbox info - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	/* Validate Response data */
	/**
	 * The Profile Identity shall be between 1 and 4 as defined
	 * in TS 23.097 for MSP
	 */
	if ((mb_info.count != 0)
			&& (mb_info.count == g_variant_n_children(mb_list))) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("Mailbox list count: [%d]", mb_info.count);

		g_variant_get(mb_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "profile_id") == 0) {
					mb_info.list[i].profile_id = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "mb_type") == 0) {
					mb_info.list[i].mb_type = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "alpha_id_len") == 0) {
					mb_info.list[i].alpha_id_len = g_variant_get_int32(key_value);
				}

				if (g_strcmp0(key, "alpha_id") == 0) {
					memcpy(mb_info.list[i].alpha_id,
						g_variant_get_data(key_value),
						TEL_SIM_ALPHA_ID_LEN_MAX);
				}

				if (g_strcmp0(key, "number") == 0) {
					memcpy(mb_info.list[i].number,
						g_variant_get_data(key_value),
						TEL_SIM_MBDN_NUM_LEN_MAX);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		err("Invalid Mailbox list count: [%d]", mb_info.count);

		sim_result = TEL_SIM_RESULT_FAILURE;
		mb_info.count = 0;
	}

	/* Free resources */
	g_variant_unref(mb_list);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &mb_info);
}

EXPORT_API TelReturn tapi_sim_get_mailbox_info(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (Mailbox info) */
	telephony_sim_call_get_mailbox_info(handle->sim_proxy,
			NULL, on_response_sim_get_mailbox_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_set_mailbox_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	/* Finish and extract the response data */
	telephony_sim_call_set_mailbox_info_finish(handle->sim_proxy, &sim_result, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);

	dbg("Set Mailbox info: [%s]",
			(sim_result == TEL_SIM_RESULT_SUCCESS ? "SUCCESS" : "FAIL"));

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
}

EXPORT_API TelReturn tapi_sim_set_mailbox_info(TelHandle *handle,
	TelSimMailBoxNumber *req_mb,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && req_mb && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	dbg("Profile ID: [%d] mailbox type: [%d]  Alpha ID Length:[%d] "
		"Alpha ID: [%s] Number: [%s]", req_mb->profile_id,
		req_mb->mb_type, req_mb->alpha_id_len, req_mb->alpha_id,
		req_mb->number);

	/* DBUS Request - SET (Mailbox info) */
	telephony_sim_call_set_mailbox_info(handle->sim_proxy,
		req_mb->profile_id, req_mb->mb_type, req_mb->alpha_id_len,
		req_mb->alpha_id, req_mb->number, NULL,
		on_response_sim_set_mailbox_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_msisdn(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *msisdn_list = NULL;
	TelSimMsisdnList msisdn_info;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;
	guint i = 0;

	memset(&msisdn_info, 0x0, sizeof(TelSimMsisdnList));

	/* Finish and extract the response data */
	telephony_sim_call_get_msisdn_finish(handle->sim_proxy,
		&sim_result, &msisdn_info.count, &msisdn_list, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM MSISDN - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	if ((msisdn_info.count != 0)
			&& (msisdn_info.count == g_variant_n_children(msisdn_list))) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("MSISDN list count: [%d]", msisdn_info.count);
		msisdn_info.list = g_malloc0(msisdn_info.count * sizeof(TelSimMsisdnList));

		g_variant_get(msisdn_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "alpha_id") == 0) {
					msisdn_info.list[i].alpha_id =
						g_strdup(g_variant_get_string(key_value, NULL));
				}

				if (g_strcmp0(key, "num") == 0) {
					msisdn_info.list[i].num =
						g_strdup(g_variant_get_string(key_value, NULL));
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		err("Invalid MSISDN list count: [%d]", msisdn_info.count);

		sim_result = TEL_SIM_RESULT_FAILURE;
		msisdn_info.count = 0;
	}

	/* Free resources */
	g_variant_unref(msisdn_list);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &msisdn_info);

	/* Free resources */
	i = 0;
	while (msisdn_info.count) {
		g_free(msisdn_info.list[i].alpha_id);
		g_free(msisdn_info.list[i].num);

		msisdn_info.count--;
		i++;
	}
	g_free(msisdn_info.list);
}

EXPORT_API TelReturn tapi_sim_get_msisdn(TelHandle *handle, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (MSIDN) */
	telephony_sim_call_get_msisdn(handle->sim_proxy,
		NULL, on_response_sim_get_msisdn, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_spn(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimSpn spn_info;
	gint display_condition;	/* Aligned to TelSimSpnDispCondition */
	gchar *spn;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&spn_info, 0x0, sizeof(TelSimSpn));

	/* Finish and extract the response data */
	telephony_sim_call_get_spn_finish(handle->sim_proxy,
					&sim_result, &display_condition, &spn, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM SPN - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	/* Validate Response data */
	if (strlen(spn) > TEL_SIM_SPN_LEN_MAX) {
		err("Invalid SPN length: [%d] SPN: [%s]", strlen(spn), spn);
		sim_result = TEL_SIM_RESULT_FAILURE;

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(spn);

		return;
	}

	memcpy(spn_info.spn, spn, strlen(spn));
	spn_info.spn[strlen(spn)] = '\0';
	spn_info.display_condition = display_condition;

	/* Free resources */
	g_free(spn);

	dbg("SPN: [%s] Display condition: [%d]", spn_info.spn, spn_info.display_condition);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &spn_info);
}

EXPORT_API TelReturn tapi_sim_get_spn(TelHandle *handle, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (SPN) */
	telephony_sim_call_get_spn(handle->sim_proxy,
					NULL, on_response_sim_get_spn, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_cphs_netname(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimCphsNetName cphs_info;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&cphs_info, 0x0, sizeof(TelSimCphsNetName));

	/* Finish and extract the response data */
	telephony_sim_call_get_cphs_net_name_finish(handle->sim_proxy,
			&sim_result, &cphs_info.full_name, &cphs_info.short_name, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get CPHS Network name - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	dbg("CPHS - Full name: [%s] Short name: [%s]", cphs_info.full_name, cphs_info.short_name);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &cphs_info);

	/* Free resources */
	g_free(cphs_info.full_name);
	g_free(cphs_info.short_name);
}

EXPORT_API TelReturn tapi_sim_get_cphs_netname(TelHandle *handle, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (CPHS Network name) */
	telephony_sim_call_get_cphs_net_name(handle->sim_proxy,
					NULL, on_response_sim_get_cphs_netname, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_sp_display_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	GVariant *sp_list;
	TelSimSpPlmnList sp_info;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;
	guint i = 0;

	memset(&sp_info, 0, sizeof(TelSimSpPlmnList));

	/* Finish and extract the response data */
	telephony_sim_call_get_sp_display_info_finish(handle->sim_proxy,
		&sim_result, &sp_info.count, &sp_list, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM SP Display info - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	/* Validate Response data */
	if ((sp_info.count != 0)
			&& (g_variant_n_children(sp_list) == sp_info.count)) {
		GVariantIter *iter = NULL, *iter_row = NULL;
		GVariant *key_value;
		const gchar *key;
		guint i = 0;

		dbg("SPN list count: [%d]", sp_info.count);
		sp_info.list = g_malloc0(sp_info.count * sizeof(TelSimSpPlmnList));

		g_variant_get(sp_list, "aa{sv}", &iter);
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &key_value)) {
				if (g_strcmp0(key, "index") == 0) {
					sp_info.list[i].index =
						g_variant_get_uint32(key_value);
				}

				if (g_strcmp0(key, "plmn") == 0) {
					sp_info.list[i].plmn =
						g_strdup(g_variant_get_string(key_value, NULL));
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}
	else {
		err("Invalid SP info count: [%d] SPN: [%s]", sp_info.count);

		sim_result = TEL_SIM_RESULT_FAILURE;
		sp_info.count = 0;
	}

	/* Free  resources */
	g_variant_unref(sp_list);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &sp_info);

	/* Free  resources */
	i = 0;
	while (sp_info.count) {
		g_free(sp_info.list[i].plmn);

		sp_info.count--;
		i++;
	}
	g_free(sp_info.list);
}

EXPORT_API TelReturn tapi_sim_get_sp_display_info(TelHandle *handle,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (SP Display info) */
	telephony_sim_call_get_sp_display_info(handle->sim_proxy,
		NULL, on_response_sim_get_sp_display_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_req_authentication(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimAuthenticationResponse auth_resp;
	gint auth_type;	/* Aligned to TelSimAuthenticationType */
	gchar *resp_data, *authentication_key, *cipher_data, *integrity_data;
	guchar *decoded_data;
	gsize decoded_data_len = 0;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&auth_resp, 0x0, sizeof(TelSimAuthenticationResponse));

	/* Finish and extract the response data */
	telephony_sim_call_req_authentication_finish(handle->sim_proxy,
			&sim_result, &auth_type, &resp_data, &authentication_key,
			&cipher_data, &integrity_data, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to get SIM SP Display info - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	auth_resp.auth_type = auth_type;

	/* Decode Base64 - Response data */
	decoded_data = g_base64_decode((const gchar *)resp_data, &decoded_data_len);
	if (decoded_data_len > TEL_SIM_AUTH_MAX_RESP_DATA_LEN) {
		err("Invalid Response Data length: [%d]", decoded_data_len);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(decoded_data);
		g_free(resp_data);

		return;
	}

	if (decoded_data_len)
		memcpy(auth_resp.resp_data, decoded_data, decoded_data_len);
	auth_resp.resp_length = decoded_data_len;

	/* Free resources */
	g_free(decoded_data);
	decoded_data = NULL;
	g_free(resp_data);
	decoded_data_len = 0;

	/* Decode Base64 - Authentication Key */
	decoded_data = g_base64_decode((const gchar *)authentication_key, &decoded_data_len);
	if (decoded_data_len > TEL_SIM_AUTH_MAX_RESP_DATA_LEN) {
		err("Invalid Authentication Key length: [%d]", decoded_data_len);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(decoded_data);
		g_free(authentication_key);

		return;
	}

	if (decoded_data_len)
		memcpy(auth_resp.authentication_key, decoded_data, decoded_data_len);
	auth_resp.authentication_key_length = decoded_data_len;

	/* Free resources */
	g_free(decoded_data);
	decoded_data = NULL;
	g_free(authentication_key);
	decoded_data_len = 0;

	/* Decode Base64 - Cipher data */
	decoded_data = g_base64_decode((const gchar *)cipher_data, &decoded_data_len);
	if (decoded_data_len > TEL_SIM_AUTH_MAX_RESP_DATA_LEN) {
		err("Invalid Cipher Data length: [%d]", decoded_data_len);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(decoded_data);
		g_free(cipher_data);

		return;
	}

	if (decoded_data_len)
		memcpy(auth_resp.cipher_data, decoded_data, decoded_data_len);
	auth_resp.cipher_length = decoded_data_len;

	/* Free resources */
	g_free(decoded_data);
	decoded_data = NULL;
	g_free(cipher_data);
	decoded_data_len = 0;

	/* Decode Base64 - Integrity data */
	decoded_data = g_base64_decode((const gchar *)integrity_data, &decoded_data_len);
	if (decoded_data_len > TEL_SIM_AUTH_MAX_RESP_DATA_LEN) {
		err("Invalid Integrity Data length: [%d]", decoded_data_len);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(decoded_data);
		g_free(integrity_data);

		return;
	}

	if (decoded_data_len)
		memcpy(auth_resp.integrity_data, decoded_data, decoded_data_len);
	auth_resp.integrity_length = decoded_data_len;

	/* Free resources */
	g_free(decoded_data);
	decoded_data = NULL;
	g_free(integrity_data);
	decoded_data_len = 0;

	dbg("Authentication type: [%d] Authentication Response Length: [%d] "\
		"Authentication Key Length: [%d] Cipher Length: [%d] Integrity Length: [%d]",
		auth_resp.auth_type, auth_resp.resp_length, auth_resp.authentication_key_length,
		auth_resp.cipher_length, auth_resp.integrity_length);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &auth_resp);
}

EXPORT_API TelReturn tapi_sim_req_authentication(TelHandle *handle,
		TelSimAuthenticationData *authentication_data, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	gchar *encoded_rand, *encoded_autn;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && authentication_data && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	if (authentication_data->auth_type > TEL_SIM_AUTH_IMS_AKA) {
		err("Invalid Authentication type: [%d]", authentication_data->auth_type);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if (authentication_data->rand_length == 0) {
		err("Invalid RAND length: [%d]", authentication_data->auth_type);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if ((authentication_data->auth_type != TEL_SIM_AUTH_GSM)
			&& (authentication_data->autn_length == 0)) {
		err("Invalid inputs - Auth type: [%d] AUTN length: [%d]",
				authentication_data->auth_type, authentication_data->autn_length);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* Encode Base64 - Rand data */
	encoded_rand = g_base64_encode((const guchar *)authentication_data->rand_data,
					authentication_data->rand_length);

	/* Encode Base64 - Authentication data */
	encoded_autn = g_base64_encode((const guchar *)authentication_data->autn_data,
					authentication_data->autn_length);

	/* DBUS Request - REQ (Authentication) */
	telephony_sim_call_req_authentication(handle->sim_proxy,
			authentication_data->auth_type, encoded_rand, encoded_autn, NULL,
			on_response_sim_req_authentication, rsp_cb_data);

	/* Free resources */
	g_free(encoded_rand);
	g_free(encoded_autn);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_verify_pins(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimSecPinResult verify_pin_resp;
	gint pin_type;	/* Aligned to TelSimPinType */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&verify_pin_resp, 0, sizeof(TelSimSecPinResult));

	/* Finish and extract the response data */
	telephony_sim_call_verify_pins_finish(handle->sim_proxy,
			&sim_result, &pin_type, &verify_pin_resp.retry_count, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Verify SIM PIN - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	verify_pin_resp.pin_type = pin_type;
	dbg("Verify PIN - Type: [%d] Re-try count: [%d]", verify_pin_resp.pin_type, verify_pin_resp.retry_count);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &verify_pin_resp);
}

EXPORT_API TelReturn tapi_sim_verify_pins(TelHandle *handle,
			TelSimSecPinPw  *pin_data, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && pin_data && pin_data->pw && callback,
				TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	if ((strlen(pin_data->pw) < 4) || (strlen(pin_data->pw) > 8)) {
		err("Invalid PIN length: [%d", strlen(pin_data->pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - VERIFY (PIN) */
	telephony_sim_call_verify_pins(handle->sim_proxy,
					pin_data->pin_type, pin_data->pw, NULL,
					on_response_sim_verify_pins, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_verify_puks(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimSecPukResult verify_puk_resp;
	gint puk_type;	/* Aligned to TelSimPukType */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&verify_puk_resp, 0x0, sizeof(TelSimSecPukResult));

	/* Finish and extract the response data */
	telephony_sim_call_verify_puks_finish(handle->sim_proxy,
			&sim_result, &puk_type, &verify_puk_resp.retry_count, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Verify SIM PUK - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	verify_puk_resp.puk_type = puk_type;
	dbg("Verify PUK - Type: [%d] Re-try count: [%d]", verify_puk_resp.puk_type, verify_puk_resp.retry_count);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &verify_puk_resp);
}

EXPORT_API TelReturn tapi_sim_verify_puks(TelHandle *handle,
		TelSimSecPukPw *puk_data, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && puk_data && puk_data->puk_pw &&
				puk_data->new_pin_pw && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	if ((strlen(puk_data->puk_pw) < 4) || (strlen(puk_data->puk_pw) > 8)
			|| (strlen(puk_data->new_pin_pw) < 4) || (strlen(puk_data->new_pin_pw) > 8)) {
		err("Invalid lengths - PUK: [%d] New PIN: [%d]",
				strlen(puk_data->puk_pw), strlen(puk_data->new_pin_pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - VERIFY (PUKS) */
	telephony_sim_call_verify_puks(handle->sim_proxy,
			puk_data->puk_type, puk_data->puk_pw, puk_data->new_pin_pw,
			NULL, on_response_sim_verify_puks, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_change_pins(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;
	TelSimSecPinResult change_pin_resp;
	gint pin_type;

	memset(&change_pin_resp, 0, sizeof(TelSimSecPinResult));

	/* Finish and extract the response data */
	telephony_sim_call_change_pins_finish(handle->sim_proxy,
			&sim_result, &pin_type, &change_pin_resp.retry_count, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Change SIM PIN - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	change_pin_resp.pin_type = pin_type;
	dbg("Change PIN - Type: [%d] Re-try count: [%d]",
			change_pin_resp.pin_type, change_pin_resp.retry_count);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &change_pin_resp);
}

EXPORT_API TelReturn tapi_sim_change_pins(TelHandle *handle,
		TelSimSecChangePinPw *pin_data, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && pin_data && pin_data->old_pw &&
				pin_data->new_pw && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	if ((strlen(pin_data->old_pw) < 4) || (strlen(pin_data->old_pw) > 8)
			|| (strlen(pin_data->new_pw) < 4) || (strlen(pin_data->new_pw) > 8)) {
		err("Invalid lengths - Old PIN: [%d] New PIN: [%d]",
				strlen(pin_data->old_pw), strlen(pin_data->new_pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - CHANGE (PINS) */
	telephony_sim_call_change_pins(handle->sim_proxy,
			pin_data->pin_type, pin_data->old_pw, pin_data->new_pw,
			NULL, on_response_sim_change_pins, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_disable_facility(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimFacilityResult disable_facility_resp;
	gint facility_type;	/* Aligned to TelSimLockType */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&disable_facility_resp, 0, sizeof(TelSimFacilityResult));

	/* Finish and extract the response data */
	telephony_sim_call_disable_facility_finish(handle->sim_proxy,
			&sim_result, &facility_type, &disable_facility_resp.retry_count, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Disable SIM facility - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	disable_facility_resp.type = facility_type;
	dbg("Disable Facility - Type: [%d] Re-try count: [%d]",
			disable_facility_resp.type, disable_facility_resp.retry_count);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &disable_facility_resp);
}

EXPORT_API TelReturn tapi_sim_disable_facility(TelHandle *handle,
		TelSimFacilityPw *pw, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && pw && pw->pw && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	if (pw->lock_type >TEL_SIM_LOCK_PC) {
		err("Invalid input - Lock type: [%d]", pw->lock_type);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if ((pw->lock_type < TEL_SIM_LOCK_PN)
			&& ((strlen(pw->pw) < 4) || (strlen(pw->pw) > 8))) {
		err("Invalid input - Lock type: [%d] PIN length: [%d]",
				pw->lock_type, strlen(pw->pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if ((pw->lock_type > TEL_SIM_LOCK_FD) && (strlen(pw->pw) < 6)) {
		err("Invalid input - Lock type: [%d] PIN length: [%d]",
				pw->lock_type, strlen(pw->pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - DISABLE (Facility) */
	telephony_sim_call_disable_facility(handle->sim_proxy,
			pw->lock_type, pw->pw, NULL, on_response_sim_disable_facility, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_enable_facility(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimFacilityResult enable_facility_resp;
	gint facility_type;	/* Aligned to TelSimLockType */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&enable_facility_resp, 0x0, sizeof(TelSimFacilityResult));

	/* Finish and extract the response data */
	telephony_sim_call_enable_facility_finish(handle->sim_proxy,
			&sim_result, &facility_type, &enable_facility_resp.retry_count, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Enable SIM facility - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	enable_facility_resp.type = facility_type;
	dbg("Enable Facility - Type: [%d] Re-try count: [%d]",
			enable_facility_resp.type, enable_facility_resp.retry_count);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &enable_facility_resp);
}

EXPORT_API TelReturn tapi_sim_enable_facility(TelHandle *handle,
		TelSimFacilityPw *pw, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback && pw && pw->pw,
				TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	if (pw->lock_type >TEL_SIM_LOCK_PC) {
		err("Invalid input - Lock type: [%d]", pw->lock_type);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if ((pw->lock_type < TEL_SIM_LOCK_PN)
			&& ((strlen(pw->pw) < 4) || (strlen(pw->pw) > 8))) {
		err("Invalid input - Lock type: [%d] PIN length: [%d]",
				pw->lock_type, strlen(pw->pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if ((pw->lock_type > TEL_SIM_LOCK_FD) && (strlen(pw->pw) < 6)) {
		err("Invalid input - Lock type: [%d] PIN length: [%d]",
				pw->lock_type, strlen(pw->pw));
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - ENABLE (Facility) */
	telephony_sim_call_enable_facility(handle->sim_proxy,
			pw->lock_type, pw->pw, NULL, on_response_sim_enable_facility, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_facility(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimFacilityInfo facility_info;
	gint facility_type;	/* Aligned to TelSimLockType */
	gint facility_status;	/* Aligned to TelSimFacilityStatus */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&facility_info, 0, sizeof(TelSimFacilityInfo));

	/* Finish and extract the response data */
	telephony_sim_call_get_facility_finish(handle->sim_proxy,
			&sim_result, &facility_type, &facility_status, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Get SIM facility - sim_result: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	facility_info.type = facility_type;
	facility_info.f_status = facility_status;
	dbg("Get Facility - Type: [%d] Status: [%d]",
			facility_info.type, facility_info.f_status);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &facility_info);
}

EXPORT_API TelReturn tapi_sim_get_facility(TelHandle *handle,
		TelSimLockType type, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	if (type >TEL_SIM_LOCK_PC) {
		err("Invalid input - Facility type: [%d]", type);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (Facility) */
	telephony_sim_call_get_facility(handle->sim_proxy,
			type, NULL, on_response_sim_get_facility, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_get_lock_info(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimLockInfo lock_info;
	gint lock_type;	/* Aligned to TelSimLockType */
	gint lock_status;	/* Aligned to TelSimLockStatus */

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&lock_info, 0x0, sizeof(TelSimLockInfo));

	/* Finish and extract the response data */
	telephony_sim_call_get_lock_info_finish(handle->sim_proxy, &sim_result, &lock_type,
											&lock_status, &lock_info.retry_count,
											res, &error);
	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Get SIM Lock info: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}
	lock_info.lock_type = lock_type;
	lock_info.lock_status = lock_status;
	dbg("SIM lock - Type: [%d] Status: [%d] Re-try count: [%d]",
			lock_info.lock_type, lock_info.lock_status, lock_info.retry_count);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &lock_info);
}

EXPORT_API TelReturn tapi_sim_get_lock_info(TelHandle *handle,
		TelSimLockType type, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_PRESENT(status_info.status);

	if (type >TEL_SIM_LOCK_PC) {
		err("Invalid input - Lock type: [%d]", type);
		return TEL_RETURN_INVALID_PARAMETER;
	}

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - GET (Lock info) */
	telephony_sim_call_get_lock_info(handle->sim_proxy,
			type, NULL, on_response_sim_get_lock_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_req_apdu(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimApduResp apdu_resp;
	guchar *decode_apdu_resp;
	gsize decode_apdu_resp_len;
	gchar *apdu;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&apdu_resp, 0x0, sizeof(TelSimApduResp));

	/* Finish and extract the response data */
	telephony_sim_call_req_apdu_finish(handle->sim_proxy, &sim_result, &apdu, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to Request SIM apdu: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	/* Decode Base64 - APDU Response */
	decode_apdu_resp = g_base64_decode((const gchar *)apdu, &decode_apdu_resp_len);
	if (decode_apdu_resp_len > TEL_SIM_APDU_RESP_LEN_MAX) {
		err("Invalid APDU response length: [%d]", decode_apdu_resp_len);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(decode_apdu_resp);
		g_free(apdu);

		return;
	}

	if (decode_apdu_resp_len)
		memcpy(apdu_resp.apdu_resp, decode_apdu_resp, decode_apdu_resp_len);
	apdu_resp.apdu_resp_len = decode_apdu_resp_len;

	dbg("APDU response - Length: [%d] Data: [%s]",
			apdu_resp.apdu_resp_len, decode_apdu_resp);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &apdu_resp);

	/* Free resources */
	g_free(decode_apdu_resp);
	g_free(apdu);
}

EXPORT_API TelReturn tapi_sim_req_apdu(TelHandle *handle, TelSimApdu* apdu_data,
									  TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	gchar *encoded_req_apdu;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && apdu_data
			&& apdu_data->apdu_len && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* Encode Base64 */
	encoded_req_apdu = g_base64_encode((const guchar *)apdu_data->apdu,
							apdu_data->apdu_len);

	/* DBUS Request - REQ (APDU) */
	telephony_sim_call_req_apdu(handle->sim_proxy,
			encoded_req_apdu, NULL, on_response_sim_req_apdu, rsp_cb_data);

	/* Free resources */
	g_free(encoded_req_apdu);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sim_req_atr(GObject *source_object,
						GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelSimAtr atr_resp;
	guchar *decode_atr_resp;
	gsize decode_atr_resp_len;
	gchar *atr;

	GError *error = NULL;
	gint sim_result = TEL_SIM_RESULT_FAILURE;

	memset(&atr_resp, 0, sizeof(TelSimAtr));

	/* Finish and extract the response data */
	telephony_sim_call_req_atr_finish(handle->sim_proxy, &sim_result, &atr, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, sim_result);
	if (TEL_SIM_RESULT_SUCCESS != sim_result) {
		err("Failed to request SIM atr: [%d]", sim_result);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);
		return;
	}

	/* Decode Base64 - ATR Response */
	decode_atr_resp = g_base64_decode((const gchar *)atr, &decode_atr_resp_len);
	if (decode_atr_resp_len > TEL_SIM_ATR_LEN_MAX) {
		err("Invalid ATR response length: [%d]", decode_atr_resp_len);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, sim_result, NULL);

		/* Free resources */
		g_free(decode_atr_resp);
		g_free(atr);

		return;
	}

	if (decode_atr_resp_len)
		memcpy(atr_resp.atr, decode_atr_resp, decode_atr_resp_len);
	atr_resp.atr_len = decode_atr_resp_len;

	dbg("ATR response - Length: [%d] Data: [%s]", atr_resp.atr_len, decode_atr_resp);

	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, sim_result, &atr_resp);

	/* Free resources */
	g_free(decode_atr_resp);
	g_free(atr);
}

EXPORT_API TelReturn tapi_sim_req_atr(TelHandle *handle, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = NULL;
	TelSimCardStatusInfo status_info = {0,};

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);
	__get_sim_status_info(handle->sim_proxy, &status_info);
	CHECK_IF_SIM_INITIALIZED(status_info.status);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	/* DBUS Request - REQ (ATR) */
	telephony_sim_call_req_atr(handle->sim_proxy,
			NULL, on_response_sim_req_atr, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}
