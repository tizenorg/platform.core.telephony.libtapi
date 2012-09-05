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
#include "ITapiPhonebook.h"

static int _tel_check_pb_state(TapiHandle *handle)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int api_err = TAPI_API_SUCCESS;
	TelSimPbList_t pb_list;
	int init_status = 0;

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_PB_INTERFACE, "GetInitStatus", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(ibbbbbb)",
				&init_status,
				&pb_list.b_fdn,
				&pb_list.b_adn,
				&pb_list.b_sdn,
				&pb_list.b_3g,
				&pb_list.b_aas,
				&pb_list.b_gas);

		if(init_status == 1)
			api_err = TAPI_API_SUCCESS;
		else
			api_err = TAPI_API_INVALID_OPERATION;

	} else {
		dbg( "g_dbus_conn failed in _tel_check_pb_state. error (%s)", gerr->message);
		g_error_free(gerr);
		return TAPI_API_OPERATION_FAILED;
	}

	return api_err;
}

static void on_response_get_sim_pb_count(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPbAccessResult_t result = TAPI_SIM_PB_SUCCESS;
	TelSimPbStorageInfo_t pb_cnt ;
	gint used = 0, total = 0;

	memset(&pb_cnt, 0, sizeof(TelSimPbStorageInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(iiii)",
			&result,
			&pb_cnt.StorageFileType,
			&used,
			&total);

	pb_cnt.UsedRecordCount = used;
	pb_cnt.TotalRecordCount = total;

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &pb_cnt, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_sim_pb_meta_info(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPbAccessResult_t result = TAPI_SIM_PB_SUCCESS;
	TelSimPbEntryInfo_t pb_entry ;

	gint imin = 0, imax = 0, nmax = 0, tmax = 0;

	memset(&pb_entry, 0, sizeof(TelSimPbEntryInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(iiiiii)",
			&result,
			&pb_entry.StorageFileType,
			&imin,
			&imax,
			&nmax,
			&tmax);

	pb_entry.PbIndexMin = imin;
	pb_entry.PbIndexMax = imax;
	pb_entry.PbNumLenMax =nmax;
	pb_entry.PbTextLenMax = tmax;

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &pb_entry, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_get_sim_pb_usim_meta_info(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;
	GVariant *value = NULL;
	GVariantIter *iter = NULL;
	GVariantIter *iter_row = NULL;
	const gchar *key = NULL;
	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPbAccessResult_t result = TAPI_SIM_PB_SUCCESS;
	TelSimPbCapabilityInfo_t list ;
	int i = 0;

	dbg("Func Entrance");
	memset(&list, 0, sizeof(TelSimPbCapabilityInfo_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get(dbus_result, "(iaa{sv})", &result, &iter);
	list.FileTypeCount = g_variant_iter_n_children(iter);

	i = 0;
	while (g_variant_iter_next(iter, "a{sv}", &iter_row)) {
		while (g_variant_iter_loop(iter_row, "{sv}", &key, &value)) {
			if (!g_strcmp0(key, "filed_type")) {
				list.FileTypeInfo[i].field_type = g_variant_get_int32(value);
			}
			if (!g_strcmp0(key, "index_max")) {
				list.FileTypeInfo[i].index_max = g_variant_get_int32(value);
			}
			if (!g_strcmp0(key, "text_max")) {
				list.FileTypeInfo[i].text_max = g_variant_get_int32(value);
			}
			if (!g_strcmp0(key, "used_count")) {
				list.FileTypeInfo[i].used_count = g_variant_get_int32(value);
			}
		}
		i++;
		g_variant_iter_free(iter_row);
	}
	g_variant_iter_free(iter);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &list, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_read_sim_pb_record(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPbAccessResult_t result = TAPI_SIM_PB_SUCCESS;
	TelSimPbRecord_t pb_rec ;

	gint i = 0, ni = 0;
	gchar *name = NULL;
	gchar *number = NULL;
	gchar *anr1 = NULL;
	gchar *anr2 = NULL;
	gchar *anr3 = NULL;
	gchar *email1 = NULL;
	gchar *email2 = NULL;
	gchar *email3 = NULL;
	gchar *email4 = NULL;

	memset(&pb_rec, 0, sizeof(TelSimPbRecord_t));

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(iiiisisisisisissssi)",
			&result,
			&pb_rec.phonebook_type,
			&i,
			&ni,
			&name,
			&pb_rec.dcs,
			&number,
			&pb_rec.ton,
			&anr1,
			&pb_rec.anr1_ton,
			&anr2,
			&pb_rec.anr2_ton,
			&anr3,
			&pb_rec.anr3_ton,
			&email1,
			&email2,
			&email3,
			&email4,
			&pb_rec.group_index);

	pb_rec.index = i;
	pb_rec.next_index = ni;

	if(strlen(name))
		snprintf((char*)pb_rec.name, strlen(name) + 1, "%s", name);

	if(strlen(number))
		snprintf((char*)pb_rec.number, strlen(number) + 1, "%s", number);

	if(strlen(anr1))
		snprintf((char*)pb_rec.anr1, strlen(anr1) + 1, "%s", anr1);

	if(strlen(anr2))
		snprintf((char*)pb_rec.anr2, strlen(anr2) + 1, "%s", anr2);

	if(strlen(anr3))
		snprintf((char*)pb_rec.anr3, strlen(anr3) + 1, "%s", anr3);

	if(strlen(email1))
		snprintf((char*)pb_rec.email1, strlen(email1) + 1, "%s", email1);

	if(strlen(email2))
		snprintf((char*)pb_rec.email2, strlen(email2) + 1, "%s", email2);

	if(strlen(email3))
		snprintf((char*)pb_rec.email3, strlen(email3) + 1, "%s", email3);

/*	dbg("pb_rec.phonebook_type[%d]",pb_rec.phonebook_type );
	dbg("pb_rec.index[%d]",pb_rec.index );
	dbg("pb_rec.next_index[%d]",pb_rec.next_index );
	dbg("pb_rec.name[%s]",pb_rec.name );
	dbg("pb_rec.dcs[%d]",pb_rec.dcs );
	dbg("pb_rec.number[%s]",pb_rec.number );
	dbg("pb_rec.ton[%d]",pb_rec.ton );*/

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &pb_rec, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_update_sim_pb_record(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPbAccessResult_t result = TAPI_SIM_PB_SUCCESS;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

static void on_response_delete_sim_pb_record(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;
	GVariant *dbus_result;

	struct tapi_resp_data *evt_cb_data = user_data;
	TelSimPbAccessResult_t result = TAPI_SIM_PB_SUCCESS;

	conn = G_DBUS_CONNECTION (source_object);
	dbus_result = g_dbus_connection_call_finish(conn, res, &error);

	g_variant_get (dbus_result, "(i)", &result);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, NULL, evt_cb_data->user_data);
	}

	free(evt_cb_data);
}

EXPORT_API int tel_get_sim_pb_init_info(TapiHandle *handle, int *init_completed, TelSimPbList_t *pb_list)
{
	GError *gerr = NULL;
	GVariant *sync_gv = NULL;
	int api_err = TAPI_API_SUCCESS;
	int init_status = 0;

	dbg("Func Entrance");

	TAPI_RETURN_VAL_IF_FAIL(init_completed, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pb_list, TAPI_API_INVALID_PTR);

	sync_gv = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE,
			handle->path, DBUS_TELEPHONY_PB_INTERFACE, "GetInitStatus", NULL, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL, &gerr);

	if (sync_gv) {
		g_variant_get(sync_gv, "(ibbbbbb)",
				&init_status,
				&pb_list->b_fdn,
				&pb_list->b_adn,
				&pb_list->b_sdn,
				&pb_list->b_3g,
				&pb_list->b_aas,
				&pb_list->b_gas);
		*init_completed = init_status;
	}

	return api_err;
}

EXPORT_API int tel_get_sim_pb_count(TapiHandle *handle, TelSimPbType_t pb_type, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int pb_ready = 0;

	dbg("Func Entrance");
	pb_ready = _tel_check_pb_state(handle);
	if(pb_ready != 0)
		return pb_ready;

	if ( (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN)
			&& (pb_type	!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", pb_type);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_PB_INTERFACE,
			"GetCount", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_sim_pb_count, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_pb_meta_info(TapiHandle *handle, TelSimPbType_t pb_type, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int pb_ready = 0;

	dbg("Func Entrance");
	pb_ready = _tel_check_pb_state(handle);
	if(pb_ready != 0)
		return pb_ready;

	if ( (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN)
			&& (pb_type	!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(i)", pb_type);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_PB_INTERFACE,
			"GetInfo", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_sim_pb_meta_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_pb_usim_meta_info(TapiHandle *handle, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int pb_ready = 0;

	dbg("Func Entrance");
	pb_ready = _tel_check_pb_state(handle);
	if(pb_ready != 0)
		return pb_ready;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_PB_INTERFACE,
			"GetUsimMetaInfo", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_get_sim_pb_usim_meta_info, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_read_sim_pb_record(TapiHandle *handle, TelSimPbType_t pb_type, unsigned short index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int pb_ready = 0;

	dbg("Func Entrance");
	pb_ready = _tel_check_pb_state(handle);
	if(pb_ready != 0)
		return pb_ready;

	if (index == 0)
		return TAPI_API_INVALID_INPUT;

	if ( (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN)
			&& (pb_type	!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", pb_type, index);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_PB_INTERFACE,
			"ReadRecord", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_read_sim_pb_record, evt_cb_data);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_update_sim_pb_record(TapiHandle *handle, const TelSimPbRecord_t *req_data, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int pb_ready = 0;

	dbg("Func Entrance");
	pb_ready = _tel_check_pb_state(handle);
	if(pb_ready != 0)
		return pb_ready;

	TAPI_RETURN_VAL_IF_FAIL((req_data != NULL) , TAPI_API_INVALID_PTR);

	if (req_data->index == 0) {
		return TAPI_API_INVALID_INPUT;
	}

	if ((req_data->phonebook_type != TAPI_SIM_PB_FDN)
			&& (req_data->phonebook_type != TAPI_SIM_PB_ADN) && (req_data->phonebook_type != TAPI_SIM_PB_SDN)
			&& (req_data->phonebook_type != TAPI_SIM_PB_3GSIM) && (req_data->phonebook_type != TAPI_SIM_PB_AAS)
			&& (req_data->phonebook_type != TAPI_SIM_PB_GAS)) {
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	dbg("req_data->phonebook_type[%d]",req_data->phonebook_type );
	dbg("req_data->index[%d]",req_data->index );
	dbg("req_data->name[%s]",req_data->name );
	dbg("req_data->dcs[%d]",req_data->dcs );
	dbg("req_data->number[%s]",req_data->number );
	dbg("req_data->ton[%d]",req_data->ton );

	param = g_variant_new("(iisisisisisissssi)",
			req_data->phonebook_type,
			req_data->index,
			req_data->name,
			req_data->dcs,
			req_data->number,
			req_data->ton,
			req_data->anr1,
			req_data->anr1_ton,
			req_data->anr2,
			req_data->anr2_ton,
			req_data->anr3,
			req_data->anr3_ton,
			req_data->email1,
			req_data->email2,
			req_data->email3,
			req_data->email4,
			req_data->group_index);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_PB_INTERFACE,
			"UpdateRecord", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_update_sim_pb_record, evt_cb_data);

	return TAPI_API_SUCCESS;

}

EXPORT_API int tel_delete_sim_pb_record(TapiHandle *handle, TelSimPbType_t pb_type, unsigned short index, tapi_response_cb callback, void *user_data)
{
	struct tapi_resp_data *evt_cb_data = NULL;
	GVariant *param = NULL;
	int pb_ready = 0;

	dbg("Func Entrance");
	pb_ready = _tel_check_pb_state(handle);
	if(pb_ready != 0)
		return pb_ready;

	if (index == 0)
		return TAPI_API_INVALID_INPUT;

	if ( (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN)
			&& (pb_type	!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS))
		return TAPI_API_INVALID_INPUT;

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	param = g_variant_new("(ii)", pb_type, index);

	g_dbus_connection_call(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE , handle->path, DBUS_TELEPHONY_PB_INTERFACE,
			"DeleteRecord", param, NULL,
			G_DBUS_CALL_FLAGS_NONE, -1, NULL,
			on_response_delete_sim_pb_record, evt_cb_data);

	return TAPI_API_SUCCESS;
}
