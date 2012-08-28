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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tapi_common.h"
#include "TapiUtility.h"
#include "TelSim.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiSim.h"

static int _tel_check_tapi_state()
{
	return 0;
}

static int _tel_check_sim_state(TapiHandle *handle)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int api_err = TAPI_API_SUCCESS;
	TelSimCardStatus_t init_status = 0;
	int changed = FALSE;

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetInitStatus", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(ib)", &init_status, &changed);
		dbg("init_status[%d]",init_status);

		switch(init_status){
			case TAPI_SIM_STATUS_CARD_NOT_PRESENT :
			case TAPI_SIM_STATUS_CARD_REMOVED :
				api_err = TAPI_API_SIM_NOT_FOUND;
				break;
			case TAPI_SIM_STATUS_CARD_BLOCKED :
			case TAPI_SIM_STATUS_CARD_ERROR :
				api_err = TAPI_API_SIM_CARD_ERROR;
				break;
			case TAPI_SIM_STATUS_SIM_INITIALIZING :
				api_err = TAPI_API_SIM_NOT_INITIALIZED;
				break;
			case TAPI_SIM_STATUS_SIM_INIT_COMPLETED :
				api_err = TAPI_API_SUCCESS;
				break;
			case TAPI_SIM_STATUS_SIM_PIN_REQUIRED :
			case TAPI_SIM_STATUS_SIM_PUK_REQUIRED :
			case TAPI_SIM_STATUS_SIM_NCK_REQUIRED :
			case TAPI_SIM_STATUS_SIM_NSCK_REQUIRED :
			case TAPI_SIM_STATUS_SIM_SPCK_REQUIRED :
			case TAPI_SIM_STATUS_SIM_CCK_REQUIRED :
			case TAPI_SIM_STATUS_SIM_LOCK_REQUIRED :
				api_err = TAPI_API_SIM_LOCKED;
				break;
			case TAPI_SIM_STATUS_UNKNOWN:
				api_err = TAPI_API_SERVICE_NOT_READY;
				break;
			default:
				dbg("not handled status[%d] in here",init_status);
				break;
		}
	} else {
		dbg( "g_dbus_conn failed in _tel_check_sim_state. error (%s)", gerr->message);
		g_error_free(gerr);
		return TAPI_API_OPERATION_FAILED;
	}
	dbg("api_err[%d]", api_err);
	return api_err;
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

	dbg("Func Entrance");
	memset(&iccid_info, 0, sizeof(TelSimIccIdInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(is)", &result, &iccid);
	iccid_info.icc_length = strlen(iccid);
	if(iccid_info.icc_length > TAPI_SIM_ICCID_LEN_MAX){
		dbg("current tapi support 20 byte but received length[%d] so changed");
		iccid_info.icc_length = TAPI_SIM_ICCID_LEN_MAX;
	}
	memcpy(iccid_info.icc_num, iccid, iccid_info.icc_length);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &iccid_info, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(ii)", &result, &lang);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &lang, evt_cb_data->user_data);
	}
}

static void on_response_set_sim_language(GObject *source_object, GAsyncResult *res,
		gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;

	dbg("Func Entrance");
	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}
}

static void on_response_get_sim_callforwarding_info(GObject *source_object, GAsyncResult *res,
		gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimCallForwardingInfo_t cf;

	memset(&cf, 0, sizeof(TelSimCallForwardingInfo_t));

	dbg("Func Entrance");
	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(ibb)", &result, &cf.line1, &cf.line2);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &cf, evt_cb_data->user_data);
	}
}

static void on_response_get_sim_messagewaiting_info(GObject *source_object, GAsyncResult *res,
		gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimMessageWaitingInfo_t msg;

	dbg("Func Entrance");
	memset(&msg, 0, sizeof(TelSimMessageWaitingInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iiiii)", &result, &msg.line1, &msg.line2, &msg.fax, &msg.video);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &msg, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&list, 0, sizeof(TelSimMailboxList_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);
	list.count = g_variant_iter_n_children(iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "type")) {
				list.list[i].type = g_variant_get_int32(value);
			}
			if (!g_strcmp0(key, "name")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].name, strlen(str_value) + 1, "%s", str_value);
			}
			if (!g_strcmp0(key, "number")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].number, strlen(str_value) + 1, "%s", str_value);
			}
			if (!g_strcmp0(key, "ton")) {
				list.list[i].ton = g_variant_get_int32(value);
			}
		}
		i++;
		g_variant_iter_free0(iter_row);
	}
	g_variant_iter_free0(iter);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&cphs, 0, sizeof(TelSimCphsInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iibbbbb)", &result, &cphs.CphsPhase,
			&cphs.CphsServiceTable.bOperatorNameShortForm, &cphs.CphsServiceTable.bMailBoxNumbers,
			&cphs.CphsServiceTable.bServiceStringTable,
			&cphs.CphsServiceTable.bCustomerServiceProfile,
			&cphs.CphsServiceTable.bInformationNumbers);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &cphs, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&list, 0, sizeof(TelSimMsisdnList_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);
	list.count = g_variant_iter_n_children(iter);

	/*this can be changed regarding concept*/
	if(list.count > 3){
		dbg("current list.count[%d] but we fixed maximum with 3", list.count);
		list.count = 3;
	}
	/*this can be changed regarding concept*/

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "name")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].name, strlen(str_value) + 1, "%s", str_value);
			}
			if (!g_strcmp0(key, "number")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].num, strlen(str_value) + 1, "%s", str_value);
			}
		}
		i++;
		g_variant_iter_free0(iter_row);
		/*this can be changed regarding concept*/
		if (i == 3)
			break;
		/*this can be changed regarding concept*/
	}
	g_variant_iter_free0(iter);

	dbg("msisdn count[%d]", list.count);
	for(i =0; i < list.count; i++){
		dbg("msisdn[%d]-name[%s]number[%s]",i,list.list[i].name, list.list[i].num);
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&list, 0, sizeof(TelSimOplmnwactList_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);
	list.count = g_variant_iter_n_children(iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "plmn")) {
				str_value = g_variant_get_string(value, NULL);
				snprintf(list.list[i].plmn, strlen(str_value) + 1, "%s", str_value);
			}
			if (!g_strcmp0(key, "b_umst")) {
				list.list[i].b_umts = g_variant_get_boolean(value);
			}
			if (!g_strcmp0(key, "b_gsm")) {
				list.list[i].b_gsm = g_variant_get_boolean(value);
			}
		}
		i++;
		g_variant_iter_free0(iter_row);
	}
	g_variant_iter_free0(iter);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iys)", &result, &dc, &spn);

	dbg("result[%d]", result);
	if ( result == TAPI_SIM_ACCESS_SUCCESS) {
		dbg("spn[%s], display condition[0x%x]", spn, dc);
		spn_info.display_condition = dc;
		snprintf((char *)spn_info.spn, strlen(spn)+1, "%s", spn);
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &spn_info, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iss)", &result, &full_name, &short_name);

	dbg("result[%d]", result);
	if ( result == TAPI_SIM_ACCESS_SUCCESS) {
		dbg("full name[%s], short name[%s]", full_name, short_name);
		snprintf((char *)cphs_net.full_name, strlen(full_name)+1, "%s", full_name);
		snprintf((char *)cphs_net.short_name, strlen(short_name)+1, "%s", short_name);
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &cphs_net, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&auth_resp, 0, sizeof(TelSimAuthenticationResponse_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	if (dbus_result) {
		dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));
		g_variant_get(dbus_result, "(iii@v@v@v@v)",
				&result, &auth_resp.auth_type, &auth_resp.auth_result,
				&ak_gv, &cp_gv, &it_gv, &resp_gv);

		ak = g_variant_get_variant(ak_gv);
		g_variant_get(ak, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			auth_resp.authentication_key[i] = rt_i;
			dbg("auth_resp.authentication_key[%d]=[0x%02x]", i, auth_resp.authentication_key[i]);
			i++;
		}
		auth_resp.authentication_key_length = i;
		g_variant_iter_free0(iter);
		i = 0;

		cp = g_variant_get_variant(cp_gv);
		g_variant_get(cp, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			auth_resp.cipher_data[i] = rt_i;
			dbg("auth_resp.cipher_data[%d]=[0x%02x]", i, auth_resp.cipher_data[i]);
			i++;
		}
		auth_resp.cipher_length = i;
		g_variant_iter_free0(iter);
		i = 0;

		it = g_variant_get_variant(it_gv);
		g_variant_get(it, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			auth_resp.integrity_data[i] = rt_i;
			dbg("auth_resp.integrity_data[%d]=[0x%02x]", i, auth_resp.integrity_data[i]);
			i++;
		}
		auth_resp.integrity_length = i;
		g_variant_iter_free0(iter);
		i = 0;

		resp = g_variant_get_variant(resp_gv);
		g_variant_get(resp, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			auth_resp.resp_data[i] = rt_i;
			dbg("auth_resp.resp_data[%d]=[0x%02x]", i, auth_resp.resp_data[i]);
			i++;
		}
		auth_resp.resp_length = i;
		g_variant_iter_free0(iter);
		i = 0;
	}
	else {
		result = TAPI_SIM_ACCESS_FAILED;
		dbg( "g_dbus_conn failed. error (%s)", error->message);
		g_error_free(error);
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &auth_resp, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&sec_rt, 0, sizeof(TelSimSecResult_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iii)", &result, &sec_rt.type, &sec_rt.retry_count);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &sec_rt, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&sec_rt, 0, sizeof(TelSimSecResult_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iii)", &result, &sec_rt.type, &sec_rt.retry_count);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &sec_rt, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&sec_rt, 0, sizeof(TelSimSecResult_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iii)", &result, &sec_rt.type, &sec_rt.retry_count);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &sec_rt, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&f_rt, 0, sizeof(TelSimFacilityResult_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iii)", &result, &f_rt.type, &f_rt.retry_count);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &f_rt, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&f_rt, 0, sizeof(TelSimFacilityResult_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iii)", &result, &f_rt.type, &f_rt.retry_count);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &f_rt, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&fi, 0, sizeof(TelSimFacilityInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iii)", &result, &fi.type, &fi.f_status);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &fi, evt_cb_data->user_data);
	}
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

	dbg("Func Entrance");
	memset(&lock, 0, sizeof(TelSimLockInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iiii)", &result, &lock.lock_type, &lock.lock_status,
			&lock.retry_count);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &lock, evt_cb_data->user_data);
	}
}

static void on_response_req_sim_apdu(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar rt_i;
	int i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimApduResp_t r_apdu;

	dbg("Func Entrance");
	memset(&r_apdu, 0, sizeof(TelSimApduResp_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	if (dbus_result) {
		/*	dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));*/
		g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
		inner_gv = g_variant_get_variant(param_gv);

		g_variant_get(inner_gv, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			r_apdu.apdu_resp[i] = rt_i;
			i++;
		}
		r_apdu.apdu_resp_len = (unsigned char) i;
		g_variant_iter_free0(iter);
		g_variant_unref(inner_gv);
		g_variant_unref(param_gv);
		/*		for(i=0; i < (int)r_apdu.apdu_resp_len; i++)
		 dbg("apdu[%d][0x%02x]",i, r_apdu.apdu_resp[i]);*/
	}
	else {
		result = TAPI_SIM_ACCESS_FAILED;
		dbg( "g_dbus_conn failed. error (%s)", error->message);
		g_error_free(error);
	}
	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &r_apdu, evt_cb_data->user_data);
	}
}

static void on_response_req_sim_atr(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	GVariantIter *iter = NULL;
	GVariant *param_gv = NULL;
	GVariant *inner_gv = NULL;
	guchar rt_i;
	int i = 0;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimAccessResult_t result = TAPI_SIM_ACCESS_SUCCESS;
	TelSimAtrResp_t r_atr;

	dbg("Func Entrance");
	memset(&r_atr, 0, sizeof(TelSimAtrResp_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	if (dbus_result) {
		dbg("dbus_result type_format(%s)", g_variant_get_type_string(dbus_result));
		g_variant_get(dbus_result, "(i@v)", &result, &param_gv);
		inner_gv = g_variant_get_variant(param_gv);

		g_variant_get(inner_gv, "ay", &iter);
		while (g_variant_iter_loop(iter, "y", &rt_i)) {
			r_atr.atr_resp[i] = rt_i;
			i++;
		}
		r_atr.atr_resp_len = (unsigned char) i;
		g_variant_iter_free0(iter);
		g_variant_unref(inner_gv);
		g_variant_unref(param_gv);
		/*		for(i=0; i < (int)r_atr.atr_resp_len; i++)
		 dbg("r_atr[%d][0x%02x]",i, r_atr.atr_resp[i]);*/
	}
	else {
		result = TAPI_SIM_ACCESS_FAILED;
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &r_atr, evt_cb_data->user_data);
	}
}

EXPORT_API int tel_get_sim_init_info(TapiHandle *handle, TelSimCardStatus_t *sim_status,
		int *card_changed)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int api_err = TAPI_API_SUCCESS;
	TelSimCardStatus_t init_status = 0;
	int changed = FALSE;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(sim_status, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(card_changed, TAPI_API_INVALID_PTR);

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetInitStatus", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(ib)", &init_status, &changed);
		*sim_status = init_status;
		*card_changed = changed;
		dbg("init_status[%d]",init_status); dbg("changed[%d]",changed);
	} else {
		dbg( "g_dbus_conn failed. error (%s)", gerr->message);
		g_error_free(gerr);
		return TAPI_API_OPERATION_FAILED;
	}
	return api_err;
}

EXPORT_API int tel_get_sim_type(TapiHandle *handle, TelSimCardType_t *card_type)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int sim_ret =0;

	dbg("Func Entrance");

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(card_type, TAPI_API_INVALID_PTR);

	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	sim_ret = _tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetCardType", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(i)", card_type);
	} else {
		dbg( "g_dbus_conn failed. error (%s)", gerr->message);
		g_error_free(gerr);
		return TAPI_API_OPERATION_FAILED;
	}
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_imsi(TapiHandle *handle, TelSimImsiInfo_t *imsi)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	gchar *gplmn = NULL;
	gchar *gmsin = NULL;
	int sim_ret = 0;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL(imsi, TAPI_API_INVALID_PTR);

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetIMSI", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
		dbg("imsi type_format(%s)", g_variant_get_type_string(sync_gv));
		g_variant_get(sync_gv, "(ss)", &gplmn, &gmsin);

		dbg("gplmn[%s],gmsin[%s]",gplmn,gmsin);
		snprintf(imsi->szMcc, 3 + 1, "%s", gplmn);
		snprintf(imsi->szMnc, strlen(gplmn) - 3 + 1, "%s", &gplmn[3]);
		snprintf(imsi->szMsin, strlen(gmsin) + 1, "%s", gmsin);

		dbg("imsi->szMcc[%s]", imsi->szMnc);
		dbg("imsi->szMnc[%s]", imsi->szMnc);
		dbg("imsi->szMcc[%s]", imsi->szMcc);
	} else {
		dbg( "g_dbus_conn failed. error (%s)", gerr->message);
		g_error_free(gerr);
		return TAPI_API_OPERATION_FAILED;
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
	int sim_ret = 0;

	dbg("Func Entrance");
	memset(ecc, 0, sizeof(TelSimEccList_t));
	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(ecc, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_SIM_INTERFACE, "GetECC", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
/*		dbg("ecc type_format(%s)", g_variant_get_type_string(sync_gv));*/
		g_variant_get(sync_gv, "(aa{sv})", &iter);
		ecc->ecc_count = g_variant_iter_n_children(iter);
		dbg("ecc->ecc_count[%d]",ecc->ecc_count);
		i = 0;
		while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
			while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
				if (!g_strcmp0(key, "name")) {
					str_value = g_variant_get_string(value, NULL);
					snprintf(ecc->list[i].name, strlen(str_value) + 1, "%s", str_value);
				}
				if (!g_strcmp0(key, "number")) {
					str_value = g_variant_get_string(value, NULL);
					snprintf(ecc->list[i].number, strlen(str_value) + 1, "%s", str_value);
				}
				if (!g_strcmp0(key, "category")) {
					ecc->list[i].category = g_variant_get_int32(value);
				}
			}
			i++;
			g_variant_iter_free0(iter_row);
		}
		g_variant_iter_free0(iter);
	} else {
		dbg( "g_dbus_conn failed. error (%s)", gerr->message);
		g_error_free(gerr);
		return TAPI_API_OPERATION_FAILED;
	}
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_iccid(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetICCID", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_sim_iccid, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_language(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetLanguage", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_language, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_set_sim_language(TapiHandle *handle, TelSimLanguagePreferenceCode_t language,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int sim_ret = 0;

	dbg("Func Entrance w/ lang[%d]",language);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", language);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "SetLanguage", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_set_sim_language, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_callforwarding_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetCallforwarding", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
			-1, NULL, on_response_get_sim_callforwarding_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_messagewaiting_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetMessageWaiting", NULL, NULL, G_DBUS_CALL_FLAGS_NONE,
			-1, NULL, on_response_get_sim_messagewaiting_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_mailbox_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetMailbox", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_mailbox_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_cphs_info(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetCPHSInfo", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_sim_cphs_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_msisdn(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetMSISDN", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_sim_msisdn, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_oplmnwact(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetOplmnwact", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_oplmnwact, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_spn(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetSpn", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_spn, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_cphs_netname(TapiHandle *handle, tapi_response_cb callback,
		void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetCphsNetName", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_cphs_netname, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_req_sim_authentication(TapiHandle *handle,
		TelSimAuthenticationData_t *authentication_data, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariantBuilder *builder = NULL;
	GVariant *param = NULL;
	GVariant *rand_gv = NULL;
	GVariant *autn_gv = NULL;
	int i =0;
	int sim_ret = 0;

	dbg("Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL(authentication_data, TAPI_API_INVALID_PTR);

	if (authentication_data->auth_type > TAPI_SIM_AUTH_TYPE_3G)
		return TAPI_API_INVALID_INPUT;

	if (authentication_data->rand_length == 0)
		return TAPI_API_INVALID_INPUT;

	if (authentication_data->auth_type != TAPI_SIM_AUTH_TYPE_GSM
			&& authentication_data->autn_length == 0)
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
	for (i = 0; i < authentication_data->rand_length; i++) {
		dbg("authentication_data->rand_data[%d][0x%02x]", i,authentication_data->rand_data[i]);
		g_variant_builder_add(builder, "y", authentication_data->rand_data[i]);
	}
	rand_gv = g_variant_builder_end(builder);

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
		for (i = 0; i < authentication_data->autn_length; i++) {
			dbg("authentication_data->autn_data[%d][0x%02x]", i,authentication_data->autn_data[i]);
			g_variant_builder_add(builder, "y", authentication_data->autn_data[i]);
		}
	autn_gv = g_variant_builder_end(builder);

	param = g_variant_new("(ivv)", authentication_data->auth_type, rand_gv, autn_gv);
	/*g_variant_builder_unref (builder);*/

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "Authentication", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_req_sim_authentication, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_verifiy_sim_pins(TapiHandle *handle, const TelSimSecPw_t *pin_data,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpw = NULL;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL(pin_data, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pin_data->pw, TAPI_API_INVALID_PTR);

	dbg("pin type[%d]", pin_data->type);
	if (pin_data->type != TAPI_SIM_PTYPE_PIN1 && pin_data->type != TAPI_SIM_PTYPE_PIN2
			&& pin_data->type != TAPI_SIM_PTYPE_SIM)
		return TAPI_API_INVALID_INPUT;

	if ((pin_data->pw_len < 4) || (pin_data->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpw = calloc(pin_data->pw_len+1, 1);
	memcpy((void*)gpw, (const void*)pin_data->pw, pin_data->pw_len);

	param = g_variant_new("(is)", pin_data->type, gpw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "VerifySec", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_verify_sim_pins, evt_cb_data);

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
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL((puk_data != NULL && new_pin_data != NULL), TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL((puk_data->pw != NULL && new_pin_data->pw != NULL),	TAPI_API_INVALID_PTR);

	dbg("puk type[%d] pin type[%d]", puk_data->type, new_pin_data->type);
	if ((puk_data->type != TAPI_SIM_PTYPE_PUK1 && puk_data->type != TAPI_SIM_PTYPE_PUK2)
			|| (new_pin_data->type != TAPI_SIM_PTYPE_PIN1 && new_pin_data->type != TAPI_SIM_PTYPE_PIN2))
		return TAPI_API_INVALID_INPUT;

	if ((puk_data->type != TAPI_SIM_PTYPE_PUK1 || new_pin_data->type != TAPI_SIM_PTYPE_PIN1)
			&& (puk_data->type != TAPI_SIM_PTYPE_PUK2 || new_pin_data->type != TAPI_SIM_PTYPE_PIN2))
		return TAPI_API_INVALID_INPUT;

	if ((puk_data->pw_len < 4) || (puk_data->pw_len > 8) || (new_pin_data->pw_len < 4)
			|| (new_pin_data->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpin = calloc(new_pin_data->pw_len+1, 1);
	gpuk = calloc(puk_data->pw_len+1, 1);
	memcpy((void*)gpin, (const void*)new_pin_data->pw, new_pin_data->pw_len);
	memcpy((void*)gpuk, (const void*)puk_data->pw, puk_data->pw_len);

	param = g_variant_new("(iss)", puk_data->type, gpuk, gpin);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "VerifyPUK", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_verify_sim_puks, evt_cb_data);

	free(gpin);
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
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;


	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);


	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL((old_pin != NULL && new_pin != NULL), TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL((old_pin->pw != NULL && new_pin->pw != NULL), TAPI_API_INVALID_PTR);

	dbg("old_pin type[%d],new_pin type[%d]", old_pin->type, new_pin->type);
	if ((old_pin->type != TAPI_SIM_PTYPE_PIN1) && (old_pin->type != TAPI_SIM_PTYPE_PIN2)
			&& (new_pin->type != TAPI_SIM_PTYPE_PIN1) && (new_pin->type != TAPI_SIM_PTYPE_PIN2))
		return TAPI_API_INVALID_INPUT;

	if (old_pin->type != new_pin->type)
		return TAPI_API_INVALID_INPUT;

	if ((old_pin->pw_len < 4) || (old_pin->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	if ((new_pin->pw_len < 4) || (new_pin->pw_len > 8))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpin_o = calloc(old_pin->pw_len+1, 1);
	gpin_n = calloc(new_pin->pw_len+1, 1);
	memcpy((void*)gpin_o, (const void*)old_pin->pw, old_pin->pw_len);
	memcpy((void*)gpin_n, (const void*)new_pin->pw, new_pin->pw_len);

	param = g_variant_new("(iss)", old_pin->type, old_pin->pw, new_pin->pw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "ChangePIN", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_change_sim_pins, evt_cb_data);

	free(gpin_o);
	free(gpin_n);
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_disable_sim_facility(TapiHandle *handle, TelSimFacilityPw_t *pw,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpw = NULL;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);


	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL(pw, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pw->pw, TAPI_API_INVALID_PTR);

	dbg("facility type[%d]", pw->lock_type);
	if(pw->lock_type < TAPI_SIM_LOCK_PS || pw->lock_type >TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type < TAPI_SIM_LOCK_PN) && ((pw->pw_len < 4) || (pw->pw_len > 8)) )
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type > TAPI_SIM_LOCK_FD) && (pw->pw_len < 6) )
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpw = calloc(pw->pw_len+1, 1);
	memcpy((void*)gpw, (const void*)pw->pw, pw->pw_len);

	param = g_variant_new("(is)", pw->lock_type, gpw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "DisableFacility", param, NULL, G_DBUS_CALL_FLAGS_NONE,
			-1, NULL, on_response_disable_sim_facility, evt_cb_data);

	free(gpw);
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_enable_sim_facility(TapiHandle *handle, TelSimFacilityPw_t *pw,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	gchar *gpw = NULL;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);


	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL(pw, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pw->pw, TAPI_API_INVALID_PTR);

	dbg("facility type[%d]", pw->lock_type);
	if(pw->lock_type < TAPI_SIM_LOCK_PS || pw->lock_type >TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type < TAPI_SIM_LOCK_PN) && ((pw->pw_len < 4) || (pw->pw_len > 8)) )
		return TAPI_API_INVALID_INPUT;
	if ((pw->lock_type > TAPI_SIM_LOCK_FD) && (pw->pw_len < 6) )
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	gpw = calloc(pw->pw_len+1, 1);
	memcpy((void*)gpw, (const void*)pw->pw, pw->pw_len);

	param = g_variant_new("(is)", pw->lock_type, gpw);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "EnableFacility", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_enable_sim_facility, evt_cb_data);

	free(gpw);
	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_facility(TapiHandle *handle, TelSimLockType_t type,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;


	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	dbg("facility type[%d]", type);
	if(type < TAPI_SIM_LOCK_PS || type >TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", type);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetFacility", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_facility, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_lock_info(TapiHandle *handle, TelSimLockType_t type,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;


	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret != TAPI_API_SUCCESS)
		return sim_ret;

	dbg("lock type[%d]", type);
	if(type < TAPI_SIM_LOCK_PS || type >TAPI_SIM_LOCK_PC)
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", type);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetLockInfo", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_get_sim_lock_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_req_sim_apdu(TapiHandle *handle, TelSimApdu_t* apdu_data,
		tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariantBuilder *builder = NULL;
	GVariant *param = NULL;
	GVariant *inner_gv = NULL;
	int i = 0;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);


	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	TAPI_RETURN_VAL_IF_FAIL(apdu_data, TAPI_API_INVALID_PTR);
	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
	for (i = 0; i < apdu_data->apdu_len; i++) {
		dbg("apdu_data->apdu[%d][0x%02x]", i,apdu_data->apdu[i]);
		g_variant_builder_add(builder, "y", apdu_data->apdu[i]);
	}
	inner_gv = g_variant_builder_end(builder);
	param = g_variant_new("(v)", inner_gv);
	/*g_variant_builder_unref (builder);*/

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "TransferAPDU", param, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			NULL, on_response_req_sim_apdu, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_req_sim_atr(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	int sim_ret = 0;

	dbg("Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(handle, TAPI_API_INVALID_PTR);

	sim_ret =_tel_check_sim_state(handle);
	if (sim_ret == TAPI_API_SIM_CARD_ERROR || sim_ret == TAPI_API_SIM_NOT_FOUND)
		return sim_ret;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SIM_INTERFACE, "GetATR", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_req_sim_atr, evt_cb_data);

	return TAPI_API_SUCCESS;
}
