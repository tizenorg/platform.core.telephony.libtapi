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
#include "TelSim.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiSim.h"

#define DBUS_SIM_STATUS_ERROR "SIM STATUS ERROR"
#define DBUS_SIM_NOT_FOUND "SIM NOT FOUND"
#define DBUS_SIM_PERM_BLOCKED "SIM PERM BLOCKED"
#define DBUS_SIM_CARD_ERROR "SIM CARD ERROR"
#define DBUS_SIM_NOT_INITIALIZED "SIM NOT INITIALIZED"
#define DBUS_SIM_INIT_COMPLETED "SIM INIT COMPLETED"
#define DBUS_SIM_LOCKED "SIM LOCKED"
#define DBUS_SIM_NOT_READY "SIM NOT READY"
#define DBUS_SIM_RESPONSE_DATA_ERROR "SIM RESPONSE DATA ERROR"
#define DBUS_SIM_SERVICE_IS_DISABLED "SIM SERVICE IS DISABLED"
#define DBUS_SIM_SERVICE_NOT_SUPPORTED_FOR_NVSIM "SERVICE NOT SUPPORTED FOR NVSIM"
#define DBUS_SIM_ACCESS_DENIED "No access rights"

#define TAPI_PIN_TIMEOUT    (350 * 1000) /* Unlimit: G_MAXINT */

#define TAPI_SIM_FUNCTION_ENTER(handle) { \
	dbg("Func Enterance. cp_name[%s]", handle->cp_name); \
}

#define TAPI_SIM_CHECK_TAPI_STATE() do { \
	TAPI_SIM_FUNCTION_ENTER(handle); \
	if (_tel_check_tapi_state() != 0) \
		return TAPI_API_SERVICE_NOT_READY; \
} while (0)

#define TAPI_SIM_CHECK_ERR_MSG(error) do { \
		TapiResult_t ret = TAPI_API_OPERATION_FAILED; \
		if (error) { \
			ret = __check_err_msg(error->message); \
			g_error_free(error); \
		} \
		return ret; \
} while (0)

#define TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evtcbdata) do { \
	TapiResult_t ret = TAPI_API_OPERATION_FAILED; \
	if (error) { \
		ret = __check_err_msg(error->message); \
		g_error_free(error); \
	} \
	TAPI_INVOKE_RESP_CALLBACK(evtcbdata, ret, NULL); \
	g_free(evtcbdata); \
	return; \
} while (0)

static TapiResult_t __check_err_msg(gchar *err_msg)
{
	TapiResult_t ret = TAPI_API_OPERATION_FAILED;
	if (err_msg == NULL)
		return ret;

	err("error from dbus layer. (%s)", err_msg);

	if (strstr(err_msg, DBUS_SIM_NOT_FOUND))
		ret = TAPI_API_SIM_NOT_FOUND;
	else if (strstr(err_msg, DBUS_SIM_PERM_BLOCKED))
		ret = TAPI_API_SIM_PERM_BLOCKED;
	else if (strstr(err_msg, DBUS_SIM_CARD_ERROR))
		ret = TAPI_API_SIM_CARD_ERROR;
	else if (strstr(err_msg, DBUS_SIM_NOT_INITIALIZED))
		ret = TAPI_API_SIM_NOT_INITIALIZED;
	else if (strstr(err_msg, DBUS_SIM_INIT_COMPLETED))
		ret = TAPI_API_SUCCESS;
	else if (strstr(err_msg, DBUS_SIM_LOCKED))
		ret = TAPI_API_SIM_LOCKED;
	else if (strstr(err_msg, DBUS_SIM_NOT_READY))
		ret = TAPI_API_SERVICE_NOT_READY;
	else if (strstr(err_msg, DBUS_SIM_RESPONSE_DATA_ERROR))
		ret = TAPI_API_OPERATION_FAILED;
	else if (strstr(err_msg, DBUS_SIM_SERVICE_IS_DISABLED))
		ret = TAPI_API_SIM_SERVICE_IS_DISABLED;
	else if (strstr(err_msg, DBUS_SIM_ACCESS_DENIED))
		ret = TAPI_API_ACCESS_DENIED;
	else if (strstr(err_msg, DBUS_SIM_SERVICE_NOT_SUPPORTED_FOR_NVSIM))
		ret = TAPI_API_NOT_SUPPORTED;
	else
		ret = TAPI_API_OPERATION_FAILED;

	return ret;
}

static int _tel_check_tapi_state()
{
	return 0;
}

static void on_response_get_sim_iccid(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimIccIdInfo_t iccid_info;
	gchar *iccid = NULL;

	memset(&iccid_info, 0, sizeof(TelSimIccIdInfo_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(is)", &result, &iccid);
	iccid_info.icc_length = strlen((const char *)iccid);
	if (iccid_info.icc_length > TAPI_SIM_ICCID_LEN_MAX) {
		dbg("current tapi support 20 byte but received length[%d] so changed", iccid_info.icc_length);
		iccid_info.icc_length = TAPI_SIM_ICCID_LEN_MAX;
	}
	memcpy(iccid_info.icc_num, iccid, iccid_info.icc_length);
	g_free(iccid);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &iccid_info);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_impi(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result;
	TelSimImpi_t impi_info = {0,};

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (dbus_result == NULL)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(is)", &result, &impi_info.impi);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &impi_info);
	g_free(impi_info.impi);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_impu(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	struct tapi_resp_data *evt_cb_data = user_data;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	GVariant *value = NULL;
	TelSimAccessResult_t result;
	TelSimImpuList_t impu_list = {0,};
	unsigned int i = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (dbus_result == NULL)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);

	impu_list.count = g_variant_iter_n_children(iter);
	impu_list.list = g_malloc0(sizeof(TelSimImpu_t) * impu_list.count);

	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "impu"))
				impu_list.list[i].impu = g_strdup(g_variant_get_string(value, NULL));
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &impu_list);
	for (i = 0; i < impu_list.count; i++)
		g_free(impu_list.list[i].impu);

	g_free(impu_list.list);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_domain(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result;
	TelSimDomain_t domain_info = {0,};

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (dbus_result == NULL)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(is)", &result, &domain_info.domain);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &domain_info);
	g_free(domain_info.domain);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_pcscf(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	struct tapi_resp_data *evt_cb_data = user_data;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	GVariant *value = NULL;
	TelSimAccessResult_t result;
	TelSimPcscfList_t pcscf_list = {0,};
	unsigned int i = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (dbus_result == NULL)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);

	pcscf_list.count = g_variant_iter_n_children(iter);
	pcscf_list.list = g_malloc0(sizeof(TelSimPcscf_t) * pcscf_list.count);

	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "type"))
				pcscf_list.list[i].type = g_variant_get_int32(value);
			else if (!g_strcmp0(key, "pcscf"))
				pcscf_list.list[i].pcscf = g_strdup(g_variant_get_string(value, NULL));
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &pcscf_list);
	for (i = 0; i < pcscf_list.count; i++)
		g_free(pcscf_list.list[i].pcscf);

	g_free(pcscf_list.list);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_language(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimLanguagePreferenceCode_t lang = TAPI_SIM_LP_LANG_UNSPECIFIED;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(ii)", &result, &lang);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &lang, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_set_sim_language(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_callforwarding_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	GVariant *value = NULL;
	GVariantIter *cphs_iter = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	const gchar *str_value = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimCallForwardingResp_t cf;
	int i = 0;

	memset(&cf, 0, sizeof(TelSimCallForwardingResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	dbg("dbus_result format(%s)", g_variant_get_type_string(dbus_result));
	g_variant_get(dbus_result, "(ibaa{sv}a{sv})", &result, &cf.b_cphs, &iter, &cphs_iter);

	if (cf.b_cphs) {
		while (g_variant_iter_loop(cphs_iter, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "b_line1"))
				cf.cphs_cf.b_line1 = g_variant_get_boolean(value);
			else if (!g_strcmp0(key, "b_line2"))
				cf.cphs_cf.b_line2 = g_variant_get_boolean(value);
			else if (!g_strcmp0(key, "b_fax"))
				cf.cphs_cf.b_fax = g_variant_get_boolean(value);
			else if (!g_strcmp0(key, "b_data"))
				cf.cphs_cf.b_data = g_variant_get_boolean(value);
		}
	} else {
		cf.cf_list.profile_count = g_variant_iter_n_children(iter);

		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
				if (!g_strcmp0(key, "rec_index")) {
					cf.cf_list.cf[i].rec_index = g_variant_get_int32(value);
				} else if (!g_strcmp0(key, "msp_num")) {
					cf.cf_list.cf[i].msp_num = g_variant_get_byte(value);
				} else if (!g_strcmp0(key, "cfu_status")) {
					cf.cf_list.cf[i].cfu_status = g_variant_get_byte(value);
				} else if (!g_strcmp0(key, "cfu_num")) {
					str_value = g_variant_get_string(value, NULL);
					snprintf(cf.cf_list.cf[i].cfu_num, strlen((const char *) str_value) + 1, "%s", str_value);
				} else if (!g_strcmp0(key, "ton")) {
					cf.cf_list.cf[i].ton = g_variant_get_int32(value);
				} else if (!g_strcmp0(key, "npi")) {
					cf.cf_list.cf[i].npi = g_variant_get_int32(value);
				} else if (!g_strcmp0(key, "cc2_id")) {
					cf.cf_list.cf[i].cc2_id = g_variant_get_byte(value);
				} else if (!g_strcmp0(key, "ext7_id")) {
					cf.cf_list.cf[i].ext7_id = g_variant_get_byte(value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);
	}

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &cf, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_set_sim_callforwarding_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}


static void on_response_get_sim_messagewaiting_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	GVariant *value = NULL;
	GVariantIter *cphs_iter = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimMessageWaitingResp_t mw;
	int i = 0;

	memset(&mw, 0, sizeof(TelSimMessageWaitingResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	dbg("dbus_result format(%s)", g_variant_get_type_string(dbus_result));
	g_variant_get(dbus_result, "(ibaa{sv}a{sv})", &result, &mw.b_cphs, &iter, &cphs_iter);

	if (mw.b_cphs) {
		while (g_variant_iter_loop(cphs_iter, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "b_voice1"))
				mw.cphs_mw.b_voice1 = g_variant_get_boolean(value);
			else if (!g_strcmp0(key, "b_voice2"))
				mw.cphs_mw.b_voice2 = g_variant_get_boolean(value);
			else if (!g_strcmp0(key, "b_fax"))
				mw.cphs_mw.b_fax = g_variant_get_boolean(value);
			else if (!g_strcmp0(key, "b_data"))
				mw.cphs_mw.b_data = g_variant_get_boolean(value);
		}
		g_variant_iter_free(cphs_iter);
	} else {
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
				if (!g_strcmp0(key, "rec_index"))
					mw.mw_list.mw[i].rec_index = g_variant_get_int32(value);
				else if (!g_strcmp0(key, "indicator_status"))
					mw.mw_list.mw[i].indicator_status = g_variant_get_byte(value);
				else if (!g_strcmp0(key, "voice_count"))
					mw.mw_list.mw[i].voice_count = g_variant_get_int32(value);
				else if (!g_strcmp0(key, "fax_count"))
					mw.mw_list.mw[i].fax_count = g_variant_get_int32(value);
				else if (!g_strcmp0(key, "email_count"))
					mw.mw_list.mw[i].email_count = g_variant_get_int32(value);
				else if (!g_strcmp0(key, "other_count"))
					mw.mw_list.mw[i].other_count = g_variant_get_int32(value);
				else if (!g_strcmp0(key, "video_count"))
					mw.mw_list.mw[i].video_count = g_variant_get_int32(value);
			}
			i++;
			g_variant_iter_free(iter_row);
			mw.mw_list.profile_count++;

			/* Max MSP record supported check */
			if (i == TAPI_SIM_MSP_CNT_MAX)
				break;
		}
		g_variant_iter_free(iter);
	}

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &mw, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_set_sim_messagewaiting_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_mailbox_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	const gchar *str_value = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimMailboxList_t list;
	int i = 0;
	gboolean b_cphs = 0;

	memset(&list, 0, sizeof(TelSimMailboxList_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(ibaa{sv})", &result, &b_cphs, &iter);
	list.count = g_variant_iter_n_children(iter);
	msg("list.count=[%d]", list.count);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			list.list[i].b_cphs = b_cphs;

			if (!g_strcmp0(key, "rec_index")) {
				list.list[i].rec_index = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "profile_num")) {
				list.list[i].profile_num = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "mb_type")) {
				list.list[i].mb_type = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "alpha_id_max_len")) {
				list.list[i].alpha_id_max_len = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "alpha_id")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].alpha_id, strlen((const char *)str_value) + 1, "%s", str_value);
			} else if (!g_strcmp0(key, "ton")) {
				list.list[i].ton = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "npi")) {
				list.list[i].npi = g_variant_get_int32(value);
			} else if (!g_strcmp0(key, "num")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].num, strlen((const char *)str_value) + 1, "%s", str_value);
			} else if (!g_strcmp0(key, "cc_id")) {
				list.list[i].cc_id = g_variant_get_byte(value);
			} else if (!g_strcmp0(key, "ext1_id")) {
				list.list[i].ext1_id = g_variant_get_byte(value);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_set_sim_mailbox_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_cphs_info(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimCphsInfo_t cphs;

	memset(&cphs, 0, sizeof(TelSimCphsInfo_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iibbbbb)", &result, &cphs.CphsPhase,
		&cphs.CphsServiceTable.bOperatorNameShortForm,
		&cphs.CphsServiceTable.bMailBoxNumbers,
		&cphs.CphsServiceTable.bServiceStringTable,
		&cphs.CphsServiceTable.bCustomerServiceProfile,
		&cphs.CphsServiceTable.bInformationNumbers);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &cphs, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_service_table(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result = NULL;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar value = 0;
	int i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimServiceTable_t svct;

	memset(&svct, 0, sizeof(TelSimServiceTable_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(ii@v)", &result, &svct.sim_type, &param_gv);
	inner_gv = g_variant_get_variant(param_gv);

	g_variant_get(inner_gv, "ay", &iter);

	if (TAPI_SIM_CARD_TYPE_GSM == svct.sim_type) {
		while (g_variant_iter_loop(iter, "y", &value)) {
			svct.table.sst.service[i] = value;
			i++;
		}
	} else if (TAPI_SIM_CARD_TYPE_USIM == svct.sim_type) {
		while (g_variant_iter_loop(iter, "y", &value)) {
			svct.table.ust.service[i] = value;
			i++;
		}
	} else if (TAPI_SIM_CARD_TYPE_RUIM == svct.sim_type) {
		g_variant_get(dbus_result, "(ii@v)", &result, &svct.table.cst.cdma_svc_table, &param_gv);
		inner_gv = g_variant_get_variant(param_gv);

		g_variant_get(inner_gv, "ay", &iter);

		if (TAPI_SIM_CDMA_SVC_TABLE == svct.table.cst.cdma_svc_table) {
			while (g_variant_iter_loop(iter, "y", &value)) {
				svct.table.cst.service.cdma_service[i] = value;
				i++;
			}
		} else if (TAPI_SIM_CSIM_SVC_TABLE == svct.table.cst.cdma_svc_table) {
			while (g_variant_iter_loop(iter, "y", &value)) {
				svct.table.cst.service.csim_service[i] = value;
				i++;
			}
		} else {
			err("Invalid cdma_svc_table:[%d]", svct.table.cst.cdma_svc_table);
		}
	} else {
		err("Invalid sim_type:[%d]", svct.sim_type);
	}

	g_variant_iter_free(iter);
	g_variant_unref(inner_gv);
	g_variant_unref(param_gv);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &svct, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_msisdn(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	const gchar *str_value = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimMsisdnList_t list;
	int i = 0;

	memset(&list, 0, sizeof(TelSimMsisdnList_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);
	list.count = g_variant_iter_n_children(iter);

	/*this can be changed regarding concept*/
	if (list.count > 3) {
		dbg("current list.count[%d] but we fixed maximum with 3", list.count);
		list.count = 3;
	}
	/*this can be changed regarding concept*/

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "name")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].name, strlen((const char *)str_value) + 1, "%s", str_value);
			} else if (!g_strcmp0(key, "number")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].num, strlen((const char *)str_value) + 1, "%s", str_value);
			}
		}
		i++;
		g_variant_iter_free(iter_row);

		/*this can be changed regarding concept*/
		if (i == 3)
			break;

		/*this can be changed regarding concept*/
	}
	g_variant_iter_free(iter);

	msg("msisdn count[%d]", list.count);
	for (i = 0; i < list.count; i++)
		dbg("msisdn[%d]-name[%s]number[%s]", i, list.list[i].name, list.list[i].num);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &list);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_oplmnwact(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	const gchar *str_value = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimOplmnwactList_t list;
	int i = 0;

	memset(&list, 0, sizeof(TelSimOplmnwactList_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);
	list.count = g_variant_iter_n_children(iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "plmn")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].plmn, strlen((const char *)str_value) + 1, "%s", str_value);
			} else if (!g_strcmp0(key, "b_umst")) {
				list.list[i].b_umts = g_variant_get_boolean(value);
			} else if (!g_strcmp0(key, "b_gsm")) {
				list.list[i].b_gsm = g_variant_get_boolean(value);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_spn(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimSpn_t spn_info;
	gchar *spn = NULL;
	guchar dc = 0;

	memset(&spn_info, 0, sizeof(TelSimSpn_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iys)", &result, &dc, &spn);

	dbg("result[%d]", result);
	if (result == TAPI_SIM_ACCESS_SUCCESS) {
		msg("spn[%s], display condition[0x%x]", spn, dc);
		spn_info.display_condition = dc;
		snprintf((char *)spn_info.spn, strlen((const char *)spn) + 1, "%s", spn);
	}
	g_free(spn);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &spn_info);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_cphs_netname(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimCphsNetName_t cphs_net;
	gchar *full_name = NULL;
	gchar *short_name = NULL;

	memset(&cphs_net, 0, sizeof(TelSimCphsNetName_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iss)", &result, &full_name, &short_name);

	dbg("result[%d]", result);
	if (result == TAPI_SIM_ACCESS_SUCCESS) {
		msg("full name[%s], short name[%s]", full_name, short_name);
		snprintf((char *)cphs_net.full_name, strlen((const char *)full_name) + 1, "%s", full_name);
		snprintf((char *)cphs_net.short_name, strlen((const char *)short_name) + 1, "%s", short_name);
	}
	g_free(full_name);
	g_free(short_name);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &cphs_net, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_req_sim_authentication(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *ak_gv = NULL;
	GVariant *cp_gv = NULL;
	GVariant *it_gv = NULL;
	GVariant *resp_gv = NULL;
	GVariant *ak = NULL;
	GVariant *cp = NULL;
	GVariant *it = NULL;
	GVariant *resp = NULL;
	guchar rt_i;
	int i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimAuthenticationResponse_t auth_resp;

	memset(&auth_resp, 0, sizeof(TelSimAuthenticationResponse_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));
	g_variant_get(dbus_result, "(iii@v@v@v@v)", &result, &auth_resp.auth_type,
		&auth_resp.auth_result, &ak_gv, &cp_gv, &it_gv, &resp_gv);

	ak = g_variant_get_variant(ak_gv);
	g_variant_get(ak, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &rt_i)) {
		auth_resp.authentication_key[i] = rt_i;
		i++;
	}
	auth_resp.authentication_key_length = i;
	g_variant_iter_free(iter);
	i = 0;

	cp = g_variant_get_variant(cp_gv);
	g_variant_get(cp, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &rt_i)) {
		auth_resp.cipher_data[i] = rt_i;
		i++;
	}
	auth_resp.cipher_length = i;
	g_variant_iter_free(iter);
	i = 0;

	it = g_variant_get_variant(it_gv);
	g_variant_get(it, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &rt_i)) {
		auth_resp.integrity_data[i] = rt_i;
		i++;
	}
	auth_resp.integrity_length = i;
	g_variant_iter_free(iter);
	i = 0;

	resp = g_variant_get_variant(resp_gv);
	g_variant_get(resp, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &rt_i)) {
		auth_resp.resp_data[i] = rt_i;
		i++;
	}
	auth_resp.resp_length = i;
	g_variant_iter_free(iter);
	i = 0;

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &auth_resp, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_verify_sim_pins(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimSecResult_t sec_rt;

	memset(&sec_rt, 0, sizeof(TelSimSecResult_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)", &result, &sec_rt.type, &sec_rt.retry_count);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &sec_rt, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_verify_sim_puks(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimSecResult_t sec_rt;

	memset(&sec_rt, 0, sizeof(TelSimSecResult_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)", &result, &sec_rt.type, &sec_rt.retry_count);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &sec_rt, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_change_sim_pins(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimSecResult_t sec_rt;

	memset(&sec_rt, 0, sizeof(TelSimSecResult_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)", &result, &sec_rt.type, &sec_rt.retry_count);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &sec_rt, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_disable_sim_facility(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimFacilityResult_t f_rt;

	memset(&f_rt, 0, sizeof(TelSimFacilityResult_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)", &result, &f_rt.type, &f_rt.retry_count);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &f_rt);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_enable_sim_facility(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimFacilityResult_t f_rt;

	memset(&f_rt, 0, sizeof(TelSimFacilityResult_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)", &result, &f_rt.type, &f_rt.retry_count);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &f_rt);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_facility(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimFacilityInfo_t fi;

	memset(&fi, 0, sizeof(TelSimFacilityInfo_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iii)", &result, &fi.type, &fi.f_status);

	TAPI_INVOKE_RESP_CALLBACK(evt_cb_data, result, &fi);
	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_lock_info(GObject *source_object, GAsyncResult *res,
		gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPinOperationResult_t result = TAPI_SIM_PIN_OPERATION_SUCCESS;
	TelSimLockInfo_t lock;

	memset(&lock, 0, sizeof(TelSimLockInfo_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(iiii)", &result, &lock.lock_type, &lock.lock_status,
			&lock.retry_count);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &lock, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_req_sim_apdu(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar rt_i;
	unsigned short i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimApduResp_t r_apdu;

	memset(&r_apdu, 0, sizeof(TelSimApduResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	/*	dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));*/
	g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
	inner_gv = g_variant_get_variant(param_gv);

	g_variant_get(inner_gv, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &rt_i)) {
		r_apdu.apdu_resp[i] = rt_i;
		i++;
		if (i >= TAPI_SIM_APDU_MAX_LEN)
			break;
	}
	r_apdu.apdu_resp_len = i;
	dbg("r_apdu.apdu_resp_len=[%d]", r_apdu.apdu_resp_len);
	g_variant_iter_free(iter);
	g_variant_unref(inner_gv);
	g_variant_unref(param_gv);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &r_apdu, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_req_sim_atr(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar rt_i;
	unsigned short i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimAtrResp_t r_atr;

	memset(&r_atr, 0, sizeof(TelSimAtrResp_t));

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));
	g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
	inner_gv = g_variant_get_variant(param_gv);

	g_variant_get(inner_gv, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &rt_i)) {
		r_atr.atr_resp[i] = rt_i;
		i++;
	}
	r_atr.atr_resp_len = i;
	dbg("r_atr.atr_resp_len=[%d]", r_atr.atr_resp_len);
	g_variant_iter_free(iter);
	g_variant_unref(inner_gv);
	g_variant_unref(param_gv);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &r_atr, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_set_sim_power_state(GObject *source_object, GAsyncResult *res,
	gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPowerSetResult_t result = TAPI_SIM_POWER_SET_SUCCESS;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

static void on_response_get_sim_isim_service_table(GObject *source_object,
		GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	struct tapi_resp_data *evt_cb_data = user_data;
	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimIST_t ist;
	guchar value;
	int i = 0;

	conn = G_DBUS_CONNECTION(source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);
	TAPI_RESP_CHECK_ERROR(error, evt_cb_data);

	if (!dbus_result)
		TAPI_SIM_CHECK_ERR_MSG_AND_CALL_NULL_CBFUNC(error, evt_cb_data);

	g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
	inner_gv = g_variant_get_variant(param_gv);

	g_variant_get(inner_gv, "ay", &iter);
	while (g_variant_iter_loop(iter, "y", &value)) {
		ist.service[i] = value;
		i++;
	}

	if (evt_cb_data->cb_fn)
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &ist, evt_cb_data->user_data);

	g_free(evt_cb_data);
	g_variant_unref(dbus_result);
}

EXPORT_API int tel_get_sim_init_info(TapiHandle *handle,
	TelSimCardStatus_t *sim_status, int *card_changed)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	TelSimCardStatus_t init_status = 0;
	int changed = FALSE;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(sim_status, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(card_changed, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetInitStatus", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(ib)", &init_status, &changed);
		*sim_status = init_status;
		*card_changed = changed;
		msg("[%s] sim_init_status[%d] changed[%d]", handle->cp_name, init_status, changed);

		g_variant_unref(sync_gv);
	} else {
		TAPI_SIM_CHECK_ERR_MSG(gerr);
	}

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_type(TapiHandle *handle, TelSimCardType_t *card_type)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(card_type, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetCardType", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca, &gerr);
	if (sync_gv) {
		g_variant_get(sync_gv, "(i)", card_type);

		g_variant_unref(sync_gv);
	} else {
		TAPI_SIM_CHECK_ERR_MSG(gerr);
	}
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_imsi(TapiHandle *handle, TelSimImsiInfo_t *imsi)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(imsi, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetIMSI", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca, &gerr);
	if (sync_gv) {
		gchar *gplmn = NULL;
		gchar *gmsin = NULL;
		int gplmn_len = 0;
		int gmsin_len = 0;

		g_variant_get(sync_gv, "(ss)", &gplmn, &gmsin);

		gplmn_len = strlen((const char *)gplmn);
		gmsin_len = strlen((const char *)gmsin);

		if (gplmn_len >= 5 && gmsin_len >= 9) {
			snprintf(imsi->szMcc, 3 + 1, "%s", gplmn);
			snprintf(imsi->szMnc, gplmn_len - 3 + 1, "%s", &gplmn[3]);
			snprintf(imsi->szMsin, gmsin_len + 1, "%s", gmsin);
		} else {
			msg("invalid length.(plmn_len=%d, msin_len=%d)", gplmn_len, gmsin_len);
			memset(imsi, 0, sizeof(TelSimImsiInfo_t));
		}
		g_free(gplmn);
		g_free(gmsin);

		g_variant_unref(sync_gv);
	} else {
		TAPI_SIM_CHECK_ERR_MSG(gerr);
	}

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_ecc(TapiHandle *handle, TelSimEccList_t *ecc)
{
	GError *gerr = NULL;
	GVariant *sync_gv;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	const gchar *str_value = NULL;
	int i = 0;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(ecc, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	memset(ecc, 0, sizeof(TelSimEccList_t));

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetECC", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca, &gerr);

	if (sync_gv) {
/*		dbg("ecc type_format(%s)", g_variant_get_type_string(sync_gv));*/
		g_variant_get(sync_gv, "(aa{sv})", &iter);
		ecc->ecc_count = g_variant_iter_n_children(iter);
		msg("ecc->ecc_count[%d]", ecc->ecc_count);
		i = 0;
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
				if (!g_strcmp0(key, "name")) {
					str_value = g_variant_get_string(value, NULL);
					snprintf(ecc->list[i].name, strlen((const char *)str_value) + 1, "%s", str_value);
				} else if (!g_strcmp0(key, "number")) {
					str_value = g_variant_get_string(value, NULL);
					snprintf(ecc->list[i].number, strlen((const char *)str_value) + 1, "%s", str_value);
				} else if (!g_strcmp0(key, "category")) {
					ecc->list[i].category = g_variant_get_int32(value);
				}
			}
			i++;
			g_variant_iter_free(iter_row);
		}
		g_variant_iter_free(iter);

		g_variant_unref(sync_gv);
	} else {
		TAPI_SIM_CHECK_ERR_MSG(gerr);
	}

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_iccid(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetICCID", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_sim_iccid, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_language(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetLanguage", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_language, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sim_language(TapiHandle *handle, TelSimLanguagePreferenceCode_t language,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", language);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "SetLanguage", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_set_sim_language, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_callforwarding_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetCallForwarding", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
			TAPI_DEFAULT_TIMEOUT, handle->ca, on_response_get_sim_callforwarding_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sim_callforwarding_info(TapiHandle *handle, TelSimCallForwardingReq_t *req_cf,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *g_number = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(req_cf, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_number = calloc(1, strlen((const char *)&req_cf->cf_data_u.cf.cfu_num)+1);
	if (!g_number) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)g_number, (const void *)&req_cf->cf_data_u.cf.cfu_num, strlen((const char *)&req_cf->cf_data_u.cf.cfu_num));

	param = g_variant_new("(biiyiisiibbbb)",
			req_cf->b_cphs,
			req_cf->cf_data_u.cf.rec_index,
			req_cf->cf_data_u.cf.msp_num,
			req_cf->cf_data_u.cf.cfu_status,
			req_cf->cf_data_u.cf.ton,
			req_cf->cf_data_u.cf.npi,
			g_number,
			req_cf->cf_data_u.cf.cc2_id,
			req_cf->cf_data_u.cf.ext7_id,
			req_cf->cf_data_u.cphs_cf.b_line1,
			req_cf->cf_data_u.cphs_cf.b_line2,
			req_cf->cf_data_u.cphs_cf.b_fax,
			req_cf->cf_data_u.cphs_cf.b_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "SetCallForwarding", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_set_sim_callforwarding_info, evt_cb_data);

	if (g_number)
		free(g_number);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_messagewaiting_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetMessageWaiting", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
			TAPI_DEFAULT_TIMEOUT, handle->ca, on_response_get_sim_messagewaiting_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sim_messagewaiting_info(TapiHandle *handle, TelSimMessageWaitingReq_t *req_mw,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(req_mw, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("b_cphs[%d], rec_index[%d], indicator_status[0x%x], voice_count[%d], fax_count[%d], " \
		"email_count[%d], other_count[%d], video_count[%d], b_voice1[%d], b_voice2[%d], b_fax[%d], b_data[%d]",
		req_mw->b_cphs,
		req_mw->mw_data_u.mw.rec_index,
		req_mw->mw_data_u.mw.indicator_status,
		req_mw->mw_data_u.mw.voice_count,
		req_mw->mw_data_u.mw.fax_count,
		req_mw->mw_data_u.mw.email_count,
		req_mw->mw_data_u.mw.other_count,
		req_mw->mw_data_u.mw.video_count,
		req_mw->mw_data_u.cphs_mw.b_voice1,
		req_mw->mw_data_u.cphs_mw.b_voice2,
		req_mw->mw_data_u.cphs_mw.b_fax,
		req_mw->mw_data_u.cphs_mw.b_data);

	param = g_variant_new("(biyiiiiibbbb)",
		req_mw->b_cphs,
		req_mw->mw_data_u.mw.rec_index,
		req_mw->mw_data_u.mw.indicator_status,
		req_mw->mw_data_u.mw.voice_count,
		req_mw->mw_data_u.mw.fax_count,
		req_mw->mw_data_u.mw.email_count,
		req_mw->mw_data_u.mw.other_count,
		req_mw->mw_data_u.mw.video_count,
		req_mw->mw_data_u.cphs_mw.b_voice1,
		req_mw->mw_data_u.cphs_mw.b_voice2,
		req_mw->mw_data_u.cphs_mw.b_fax,
		req_mw->mw_data_u.cphs_mw.b_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "SetMessageWaiting", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_set_sim_messagewaiting_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_mailbox_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetMailbox", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_mailbox_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sim_mailbox_info(TapiHandle *handle, TelSimMailBoxNumber_t *req_mb,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *g_alpha = NULL;
	gchar *g_number = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(req_mb, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_alpha = calloc(1, strlen((const char *)&req_mb->alpha_id)+1);
	if (!g_alpha) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)g_alpha, (const void *)&req_mb->alpha_id, strlen((const char *)&req_mb->alpha_id));

	g_number = calloc(1, strlen((const char *)&req_mb->num)+1);
	if (!g_number) {
		free(g_alpha);
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)g_number, (const void *)&req_mb->num, strlen((const char *)&req_mb->num));

	dbg("req_mb->b_cphs[%d], req_mb->rec_index[%d], req_mb->profile_num[%d], req_mb->mb_type[%d], req_mb->alpha_id_max_len[%d], req_mb->alpha_id[%s]",
			req_mb->b_cphs,
			req_mb->rec_index,
			req_mb->profile_num,
			req_mb->mb_type,
			req_mb->alpha_id_max_len,
			g_alpha);

	dbg("req_mb->ton[%d], req_mb->npi[%d], g_number[%s], req_mb->cc_id[%d], req_mb->ext1_id[%d]",
			req_mb->ton,
			req_mb->npi,
			g_number,
			req_mb->cc_id,
			req_mb->ext1_id);

	param = g_variant_new("(biiiisiisii)",
			req_mb->b_cphs,
			req_mb->mb_type,
			req_mb->rec_index,
			req_mb->profile_num,
			req_mb->alpha_id_max_len,
			g_alpha,
			req_mb->ton,
			req_mb->npi,
			g_number,
			req_mb->cc_id,
			req_mb->ext1_id);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "SetMailbox", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_set_sim_mailbox_info, evt_cb_data);

	if (g_alpha)
		free(g_alpha);

	if (g_number)
		free(g_number);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_cphs_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetCPHSInfo", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_sim_cphs_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_service_table(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetServiceTable", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_sim_service_table, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_msisdn(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetMSISDN", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_get_sim_msisdn, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_oplmnwact(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetOplmnwact", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_oplmnwact, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_spn(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetSpn", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_spn, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_cphs_netname(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetCphsNetName", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_cphs_netname, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_req_sim_authentication(TapiHandle *handle,
		TelSimAuthenticationData_t *authentication_data, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariantBuilder builder;
	GVariant *param = NULL;
	GVariant *rand_gv = NULL;
	GVariant *autn_gv = NULL;
	int i = 0;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(authentication_data, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	if (authentication_data->auth_type > TAPI_SIM_AUTH_TYPE_3G)
		return TAPI_API_INVALID_INPUT;

	if (authentication_data->rand_length == 0)
		return TAPI_API_INVALID_INPUT;

	if (authentication_data->auth_type != TAPI_SIM_AUTH_TYPE_GSM
			&& authentication_data->autn_length == 0)
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_variant_builder_init(&builder, G_VARIANT_TYPE("ay"));
	for (i = 0; i < authentication_data->rand_length; i++) {
		dbg("authentication_data->rand_data[%d][0x%02x]", i, authentication_data->rand_data[i]);
		g_variant_builder_add(&builder, "y", authentication_data->rand_data[i]);
	}
	rand_gv = g_variant_builder_end(&builder);

	g_variant_builder_init(&builder, G_VARIANT_TYPE("ay"));
		for (i = 0; i < authentication_data->autn_length; i++) {
			dbg("authentication_data->autn_data[%d][0x%02x]", i, authentication_data->autn_data[i]);
			g_variant_builder_add(&builder, "y", authentication_data->autn_data[i]);
		}
	autn_gv = g_variant_builder_end(&builder);

	param = g_variant_new("(ivv)", authentication_data->auth_type, rand_gv, autn_gv);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "Authentication", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_req_sim_authentication, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_verifiy_sim_pins(TapiHandle *handle, const TelSimSecPw_t *pin_data,
		tapi_response_cb callback, void *user_data)
{
	return tel_verify_sim_pins(handle, pin_data, callback, user_data);
}

EXPORT_API int tel_verify_sim_pins(TapiHandle *handle, const TelSimSecPw_t *pin_data,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpw = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pin_data, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pin_data->pw, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	msg("pin type[%d]", pin_data->type);
	if (pin_data->type != TAPI_SIM_PTYPE_PIN1 && pin_data->type != TAPI_SIM_PTYPE_PIN2
			&& pin_data->type != TAPI_SIM_PTYPE_SIM)
		return TAPI_API_INVALID_INPUT;

	if ((pin_data->pw_len < 4) || (pin_data->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpw = calloc(1, pin_data->pw_len+1);
	if (!gpw) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)gpw, (const void *)pin_data->pw, pin_data->pw_len);

	param = g_variant_new("(is)", pin_data->type, gpw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "VerifySec", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_PIN_TIMEOUT,
			handle->ca, on_response_verify_sim_pins, evt_cb_data);

	if (gpw)
		free(gpw);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_verify_sim_puks(TapiHandle *handle, const TelSimSecPw_t *puk_data,
		const TelSimSecPw_t *new_pin_data, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpin = NULL;
	gchar *gpuk = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL((puk_data != NULL && new_pin_data != NULL), TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL((puk_data->pw != NULL && new_pin_data->pw != NULL), TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	msg("puk type[%d] pin type[%d]", puk_data->type, new_pin_data->type);
	if ((puk_data->type != TAPI_SIM_PTYPE_PUK1 && puk_data->type != TAPI_SIM_PTYPE_PUK2)
			|| (new_pin_data->type != TAPI_SIM_PTYPE_PIN1 && new_pin_data->type != TAPI_SIM_PTYPE_PIN2))
		return TAPI_API_INVALID_INPUT;

	if ((puk_data->type != TAPI_SIM_PTYPE_PUK1 || new_pin_data->type != TAPI_SIM_PTYPE_PIN1)
			&& (puk_data->type != TAPI_SIM_PTYPE_PUK2 || new_pin_data->type != TAPI_SIM_PTYPE_PIN2))
		return TAPI_API_INVALID_INPUT;

	if ((puk_data->pw_len < 4) || (puk_data->pw_len > 8) || (new_pin_data->pw_len < 4)
			|| (new_pin_data->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpin = calloc(1, new_pin_data->pw_len+1);
	if (!gpin) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	gpuk = calloc(1, puk_data->pw_len+1);
	if (!gpuk) {
		g_free(evt_cb_data);
		free(gpin);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)gpin, (const void *)new_pin_data->pw, new_pin_data->pw_len);
	memcpy((void *)gpuk, (const void *)puk_data->pw, puk_data->pw_len);

	param = g_variant_new("(iss)", puk_data->type, gpuk, gpin);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "VerifyPUK", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_PIN_TIMEOUT,
			handle->ca, on_response_verify_sim_puks, evt_cb_data);

	if (gpin)
		free(gpin);

	if (gpuk)
		free(gpuk);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_change_sim_pins(TapiHandle *handle, const TelSimSecPw_t *old_pin,
		const TelSimSecPw_t *new_pin, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpin_o = NULL;
	gchar *gpin_n = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL((old_pin != NULL && new_pin != NULL), TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL((old_pin->pw != NULL && new_pin->pw != NULL), TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	msg("old_pin type[%d], new_pin type[%d]", old_pin->type, new_pin->type);
	if ((old_pin->type != TAPI_SIM_PTYPE_PIN1) && (old_pin->type != TAPI_SIM_PTYPE_PIN2)
			&& (new_pin->type != TAPI_SIM_PTYPE_PIN1) && (new_pin->type != TAPI_SIM_PTYPE_PIN2))
		return TAPI_API_INVALID_INPUT;

	if (old_pin->type != new_pin->type)
		return TAPI_API_INVALID_INPUT;

	if ((old_pin->pw_len < 4) || (old_pin->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	if ((new_pin->pw_len < 4) || (new_pin->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpin_o = calloc(1, old_pin->pw_len+1);
	if (!gpin_o) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	gpin_n = calloc(1, new_pin->pw_len+1);
	if (!gpin_n) {
		free(gpin_o);
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)gpin_o, (const void *)old_pin->pw, old_pin->pw_len);
	memcpy((void *)gpin_n, (const void *)new_pin->pw, new_pin->pw_len);

	param = g_variant_new("(iss)", old_pin->type, gpin_o, gpin_n);
	msg("old_pin len[%d], new_pin len[%d]", old_pin->pw_len, new_pin->pw_len);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "ChangePIN", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_change_sim_pins, evt_cb_data);

	if (gpin_o)
		free(gpin_o);

	if (gpin_n)
		free(gpin_n);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_disable_sim_facility(TapiHandle *handle, TelSimFacilityPw_t *pw,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpw = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pw, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pw->pw, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	dbg("facility type[%d]", pw->lock_type);
	if (pw->lock_type < TAPI_SIM_LOCK_PS || pw->lock_type > TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type < TAPI_SIM_LOCK_PN) && ((pw->pw_len < 4) || (pw->pw_len > 8)))
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type > TAPI_SIM_LOCK_FD) && (pw->pw_len < 6))
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpw = calloc(1, pw->pw_len+1);
	if (!gpw) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)gpw, (const void *)pw->pw, pw->pw_len);

	param = g_variant_new("(is)", pw->lock_type, gpw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "DisableFacility", param, NULL, G_DBUS_CALL_FLAGS_NONE,
			TAPI_DEFAULT_TIMEOUT, handle->ca, on_response_disable_sim_facility, evt_cb_data);

	if (gpw)
		free(gpw);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_enable_sim_facility(TapiHandle *handle, TelSimFacilityPw_t *pw,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpw = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pw, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pw->pw, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	msg("facility type[%d]", pw->lock_type);
	if (pw->lock_type < TAPI_SIM_LOCK_PS || pw->lock_type > TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type < TAPI_SIM_LOCK_PN) && ((pw->pw_len < 4) || (pw->pw_len > 8)))
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type > TAPI_SIM_LOCK_FD) && (pw->pw_len < 6))
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpw = calloc(1, pw->pw_len+1);
	if (!gpw) {
		g_free(evt_cb_data);
		return TAPI_API_SYSTEM_OUT_OF_MEM;
	}

	memcpy((void *)gpw, (const void *)pw->pw, pw->pw_len);

	param = g_variant_new("(is)", pw->lock_type, gpw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "EnableFacility", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_enable_sim_facility, evt_cb_data);

	if (gpw)
		free(gpw);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_facility(TapiHandle *handle, TelSimLockType_t type,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	msg("facility type[%d]", type);
	if (type < TAPI_SIM_LOCK_PS || type > TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", type);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetFacility", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_facility, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_lock_info(TapiHandle *handle, TelSimLockType_t type,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	msg("lock type[%d]", type);
	if (type < TAPI_SIM_LOCK_PS || type > TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", type);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetLockInfo", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_get_sim_lock_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sim_power_state(TapiHandle *handle, TelSimPowerState_t state,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	dbg("Func Enterance. cp_name[%s], state[%d]", handle->cp_name, state);

	if (state >= TAPI_SIM_POWER_UNSPECIFIED)
		return TAPI_API_INVALID_INPUT;

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", state);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "SetPowerstate", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_set_sim_power_state, evt_cb_data);

	return TAPI_API_SUCCESS;
}


EXPORT_API int tel_req_sim_apdu(TapiHandle *handle, TelSimApdu_t* apdu_data,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariantBuilder builder;
	GVariant *param = NULL;
	GVariant *inner_gv = NULL;
	int i = 0;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(apdu_data, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_variant_builder_init(&builder, G_VARIANT_TYPE("ay"));
	for (i = 0; i < apdu_data->apdu_len; i++)
		g_variant_builder_add(&builder, "y", apdu_data->apdu[i]);
	inner_gv = g_variant_builder_end(&builder);
	param = g_variant_new("(v)", inner_gv);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "TransferAPDU", param, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT,
			handle->ca, on_response_req_sim_apdu, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_req_sim_atr(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_SIM_CHECK_TAPI_STATE();

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetATR", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca,
			on_response_req_sim_atr, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_impi(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_SIM_INTERFACE,
		"GetImpi", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sim_impi, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_impu(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_SIM_INTERFACE,
		"GetImpu", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sim_impu, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_domain(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_SIM_INTERFACE,
		"GetDomain", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sim_domain, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_pcscf(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_SIM_INTERFACE,
		"GetPcscf", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sim_pcscf, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_application_list(TapiHandle *handle, unsigned char *app_list)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(app_list, TAPI_API_INVALID_PTR);

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetAppList", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, TAPI_DEFAULT_TIMEOUT, handle->ca, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(y)", app_list);
		g_variant_unref(sync_gv);
	} else {
		TAPI_SIM_CHECK_ERR_MSG(gerr);
	}

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_isim_service_table(TapiHandle *handle,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data;

	dbg("Func Entrance");

	TAPI_RET_ERR_NUM_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	TAPI_MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
		handle->path, DBUS_TELEPHONY_SIM_INTERFACE,
		"GetIsimServiceTable", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
		TAPI_DEFAULT_TIMEOUT, handle->ca,
		on_response_get_sim_isim_service_table, evt_cb_data);

	return TAPI_API_SUCCESS;
}
