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
#include "tapi_phonebook.h"

#include <tel_phonebook.h>
#include <string.h>

static gboolean __tapi_check_pb_type(TelPbType pb_type)
{
	switch (pb_type) {
	case TEL_PB_FDN:
	case TEL_PB_ADN:
	case TEL_PB_SDN:
	case TEL_PB_USIM:
		return TRUE;
	}

	return FALSE;
}

void on_phonebook_signal_emit_handler(TelephonyPhonebook *phonebook,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data)
{
	TapiEvtCbData *evt_cb_data;
	TelHandle *handle = user_data;
	char *evt_id;

	if (handle == NULL || signal_name == NULL)
		return;

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_PB_INTERFACE,
					signal_name);

	/*
	 * If an event callback is registered process
	 * g-signal event
	 */
	evt_cb_data = g_hash_table_lookup(handle->evt_table, evt_id);
	if (evt_cb_data == NULL) {
		g_free(evt_id);
		return;
	}

	if (!g_strcmp0(signal_name, "Status")) {
		TelPbInitInfo init_info;

		g_variant_get(parameters, "(bbbbb)",
			&init_info.init_status,
			&init_info.pb_list.fdn, &init_info.pb_list.adn,
			&init_info.pb_list.sdn, &init_info.pb_list.usim);

		dbg("%s pb_status(%s), FDN(%d), ADN(%d), SDN(%d), USIM(%d)",
			signal_name, init_info.init_status ? "TRUE" : "FALSE",
			init_info.pb_list.fdn, init_info.pb_list.adn,
			init_info.pb_list.sdn, init_info.pb_list.usim);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &init_info);
	} else
		err("Unhandled Signal: %s", signal_name);

	g_free(evt_id);
}


EXPORT_API TelReturn tapi_pb_get_sim_pb_init_info(TelHandle *handle,
	gboolean *init_completed, TelPbList *pb_list)
{
	GError *error = NULL;
	TelPbResult pb_result = TEL_PB_RESULT_FAILURE;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle
		&& init_completed && pb_list,
		TEL_RETURN_INVALID_PARAMETER);

	telephony_phonebook_call_get_init_info_sync(handle->phonebook_proxy,
		(gint *)&pb_result, init_completed, &pb_list->fdn, &pb_list->adn,
		&pb_list->sdn, &pb_list->usim, NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		return TEL_RETURN_FAILURE;
	}

	if (pb_result != TEL_PB_RESULT_SUCCESS) {
		err("Failed to get SIM PB init info - pb_result: [%d]", pb_result);
		return TEL_RETURN_FAILURE;
	}

	dbg("init_completed: [%d] pb_list->fdn: [%d] pb_list->adn:[%d] "\
		"pb_list->sdn: [%d] pb_list->usim: [%d]", *init_completed,
		pb_list->fdn, pb_list->adn, pb_list->sdn, pb_list->usim);

	return TEL_RETURN_SUCCESS;
}

static void on_response_pb_get_sim_pb_info(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelPbResult pb_result = TEL_PB_RESULT_FAILURE;
	int pb_type;
	TelPbInfo pb_info = {0, };
	GVariant *dbus_info;
	GError *error = NULL;
	GVariantIter *iter = NULL;
	GVariant *key_value;
	const gchar *key;

	dbg("Entry");

	telephony_phonebook_call_get_info_finish(handle->phonebook_proxy,
		(gint *)&pb_result, &pb_type, &dbus_info, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, pb_result);

	if (pb_result != TEL_PB_RESULT_SUCCESS) {
		g_variant_unref(dbus_info);
		err("Failed to get SIM PB info - pb_result: [%d]", pb_result);
		RESP_CALLBACK_CALL(rsp_cb_data, pb_result, NULL);
		return;
	}

	pb_info.pb_type = pb_type;
	g_variant_get(dbus_info, "a{sv}", &iter);
	if (pb_info.pb_type == TEL_PB_USIM) {
		TelPbUsimInfo *usim = (TelPbUsimInfo *)(&pb_info.info_u.usim);

		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "max_count") == 0)
				usim->max_count = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "used_count") == 0)
				usim->used_count = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_num_len") == 0)
				usim->max_num_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_text_len") == 0)
				usim->max_text_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_anr_count") == 0)
				usim->max_anr_count = g_variant_get_byte(key_value);
			else if (g_strcmp0(key, "max_anr_len") == 0)
				usim->max_anr_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_email_count") == 0)
				usim->max_email_count = g_variant_get_byte(key_value);
			else if (g_strcmp0(key, "max_email_len") == 0)
				usim->max_email_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_sne_len") == 0)
				usim->max_sne_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_gas_count") == 0)
				usim->max_gas_count = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_gas_len") == 0)
				usim->max_gas_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_aas_count") == 0)
				usim->max_aas_count = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_aas_len") == 0)
				usim->max_aas_len = g_variant_get_uint32(key_value);
		}

		dbg("pb_type: [%d] max_count: [%d] used_count: [%d] max_num_len: [%d] " \
			"max_text_len: [%d] max_anr_count: [%d] max_anr_len: [%d] " \
			"max_email_count: [%d] max_email_len: [%d] max_sne_len: [%d] " \
			"max_gas_count: [%d] max_gas_len: [%d] "\
			"max_aas_count: [%d] max_aas_len: [%d]",
			pb_info.pb_type, usim->max_count,
			usim->used_count, usim->max_num_len,
			usim->max_text_len, usim->max_anr_count,
			usim->max_anr_len, usim->max_email_count,
			usim->max_email_len, usim->max_sne_len,
			usim->max_gas_count, usim->max_gas_len,
			usim->max_aas_count, usim->max_aas_len);
	}
	else {
		TelPbSimInfo *sim = (TelPbSimInfo *)(&pb_info.info_u.sim);

		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "max_count") == 0)
				sim->max_count = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "used_count") == 0)
				sim->used_count = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_num_len") == 0)
				sim->max_num_len = g_variant_get_uint32(key_value);
			else if (g_strcmp0(key, "max_text_len") == 0)
				sim->max_text_len = g_variant_get_uint32(key_value);
		}

		dbg("pb_type: [%d] max_count: [%d] used_count: [%d] max_num_len: [%d]" \
			"max_text_len: [%d]",
			pb_info.pb_type, sim->max_count, sim->used_count,
			sim->max_num_len, sim->max_text_len);
	}
	g_variant_iter_free(iter);
	g_variant_unref(dbus_info);

	RESP_CALLBACK_CALL(rsp_cb_data, pb_result, &pb_info);
}

EXPORT_API TelReturn tapi_pb_get_sim_pb_info(TelHandle *handle,
	TelPbType pb_type, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle
		&& callback && __tapi_check_pb_type(pb_type),
		TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_phonebook_call_get_info(handle->phonebook_proxy,
		pb_type,
		NULL,
		on_response_pb_get_sim_pb_info, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_pb_read_sim_pb_record(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);

	TelPbReadRecord pb_rec;
	int pb_type;

	GVariant *dbus_rec;
	GVariantIter *iter = NULL;

	GError *error = NULL;
	TelPbResult pb_result = TEL_PB_RESULT_FAILURE;

	memset(&pb_rec, 0, sizeof(TelPbReadRecord));

	telephony_phonebook_call_read_record_finish(handle->phonebook_proxy,
		(gint *)&pb_result, &pb_rec.index, &pb_rec.next_index, &pb_type, &dbus_rec,
		res, &error);
	CHECK_DEINIT(error, rsp_cb_data, pb_result);

	if (pb_result != TEL_PB_RESULT_SUCCESS) {
		g_variant_unref(dbus_rec);
		err("Failed to read SIM PB record - pb_result: [%d]", pb_result);
		RESP_CALLBACK_CALL(rsp_cb_data, pb_result, NULL);
		return;
	}

	pb_rec.pb_type = pb_type;
	g_variant_get(dbus_rec, "a{sv}", &iter);
	if (pb_rec.pb_type == TEL_PB_USIM) {
		GVariant *key_value;
		const gchar *key;

		TelPbUsimRecord *usim = (TelPbUsimRecord *)&(pb_rec.rec_u.usim);

		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "name") == 0) {
				g_strlcpy(usim->name,
					g_variant_get_string(key_value, NULL),
					TEL_PB_TEXT_MAX_LEN + 1);
			}
			else if (g_strcmp0(key, "number") == 0) {
				g_strlcpy(usim->number,
					g_variant_get_string(key_value, NULL),
					TEL_PB_NUMBER_MAX_LEN + 1);
			}
			else if (g_strcmp0(key, "sne") == 0) {
				g_strlcpy(usim->sne,
					g_variant_get_string(key_value, NULL),
					TEL_PB_TEXT_MAX_LEN + 1);
			}
			else if (g_strcmp0(key, "grp_name") == 0) {
				g_strlcpy(usim->grp_name,
					g_variant_get_string(key_value, NULL),
					TEL_PB_TEXT_MAX_LEN + 1);
			}
			else if (g_strcmp0(key, "anr_count") == 0) {
				usim->anr_count = g_variant_get_byte(key_value);
			}
			else if (g_strcmp0(key, "anr") == 0) {
				GVariantIter *iter2 = NULL, *iter_row2= NULL;
				GVariant *key_value2;
				const gchar *key2;
				guint count = 0;

				g_variant_get(key_value, "aa{sv}", &iter2);
				while (g_variant_iter_next(iter2, "a{sv}", &iter_row2)) {
					while (g_variant_iter_loop(iter_row2, "{sv}", &key2, &key_value2)) {
						if (g_strcmp0(key2, "number") == 0) {
							g_strlcpy(usim->anr[count].number,
								g_variant_get_string(key_value2, NULL),
								TEL_PB_NUMBER_MAX_LEN + 1);
						}
						else if (g_strcmp0(key2, "description") == 0) {
							usim->anr[count].description =
								g_variant_get_boolean(key_value2);
						}
						else if (g_strcmp0(key2, "aas") == 0) {
							g_strlcpy(usim->anr[count].aas,
								g_variant_get_string(key_value2, NULL),
								TEL_PB_TEXT_MAX_LEN + 1);
						}
					}
					count++;
					g_variant_iter_free(iter_row2);
				}
				g_variant_iter_free(iter2);
			}
			else if (g_strcmp0(key, "email_count") == 0) {
				usim->email_count = g_variant_get_byte(key_value);
			}
			else if (g_strcmp0(key, "email") == 0) {
				GVariantIter *iter2 = NULL;
				GVariant *key_value2;
				const gchar *key2;
				guint count = 0;
				char *tmp;

				g_variant_get(key_value, "a{sv}", &iter2);
				while (g_variant_iter_loop(iter2, "{sv}", &key2, &key_value2)) {
					for (count = 0; count < usim->email_count; count++) {
						tmp = g_strdup_printf("%d", count);
						if (g_strcmp0(key2, tmp) == 0) {
							gconstpointer email = g_variant_get_data(key_value2);
							g_strlcpy(usim->email[count], email, strlen(email)+1);
						}
						g_free(tmp);
					}
				}
				g_variant_iter_free(iter2);
			}
			else if (g_strcmp0(key, "hidden") == 0) {
				usim->hidden = g_variant_get_boolean(key_value);
			}
		}
	}
	else {
		GVariant *key_value;
		const gchar *key;

		TelPbSimRecord *sim = (TelPbSimRecord *)&(pb_rec.rec_u.sim);

		while (g_variant_iter_loop(iter, "{sv}", &key, &key_value)) {
			if (g_strcmp0(key, "name") == 0) {
				g_strlcpy(sim->name,
					g_variant_get_string(key_value, NULL),
					TEL_PB_TEXT_MAX_LEN + 1);
			}
			else if (g_strcmp0(key, "number") == 0) {
				g_strlcpy(sim->number,
					g_variant_get_string(key_value, NULL),
					TEL_PB_NUMBER_MAX_LEN + 1);
			}
		}
	}
	g_variant_iter_free(iter);
	g_variant_unref(dbus_rec);

	RESP_CALLBACK_CALL(rsp_cb_data, pb_result, &pb_rec);
}

EXPORT_API TelReturn tapi_pb_read_sim_pb_record(TelHandle *handle,
	const TelPbRecordInfo *record,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && record && callback
		&& __tapi_check_pb_type(record->pb_type)
		&& record->index, TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_phonebook_call_read_record(handle->phonebook_proxy,
		record->pb_type, record->index,
		NULL,
		on_response_pb_read_sim_pb_record, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_pb_update_sim_pb_record(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelPbResult pb_result = TEL_PB_RESULT_FAILURE;
	GError *error = NULL;

	telephony_phonebook_call_update_record_finish(handle->phonebook_proxy,
		(gint *)&pb_result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, pb_result);

	if (pb_result != TEL_PB_RESULT_SUCCESS)
		err("Failed to update SIM PB record - pb_result: [%d]", pb_result);
	else
		dbg("update SIM PB record: [SUCCESS]");

	RESP_CALLBACK_CALL(rsp_cb_data, pb_result, NULL);
}

EXPORT_API TelReturn tapi_pb_update_sim_pb_record(TelHandle *handle,
	const TelPbUpdateRecord *record,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	GVariantBuilder update_builder;
	GVariant *var_update = NULL;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && record && callback
		&& __tapi_check_pb_type(record->pb_type)
		&& record->index, TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	dbg("Phonebook Type: [%d] Index: [%d]", record->pb_type, record->index);

	g_variant_builder_init(&update_builder, G_VARIANT_TYPE("a{sv}"));
	if (record->pb_type == TEL_PB_USIM) {
		GVariant *var_anr = NULL, *var_email = NULL;
		GVariantBuilder anr_builder, email_builder;

		TelPbUsimRecord *usim = (TelPbUsimRecord *)&(record->rec_u.usim);
		guint count = 0;

		dbg("Name: [%s] Number: [%s] SNE: [%s] Group Name: [%s]",
			usim->name, usim->number, usim->sne, usim->grp_name);
		g_variant_builder_add(&update_builder, "{sv}",
			"name", g_variant_new_string(usim->name));
		g_variant_builder_add(&update_builder, "{sv}",
			"number", g_variant_new_string(usim->number));
		g_variant_builder_add(&update_builder, "{sv}",
			"sne", g_variant_new_string(usim->sne));
		g_variant_builder_add(&update_builder, "{sv}",
			"grp_name", g_variant_new_string(usim->grp_name));

		/* ANR */
		dbg("ANR count: [%d]", usim->anr_count);
		g_variant_builder_add(&update_builder, "{sv}",
			"anr_count", g_variant_new_byte(usim->anr_count));

		g_variant_builder_init(&anr_builder, G_VARIANT_TYPE("aa{sv}"));
		if (usim->anr_count && usim->anr_count <= TEL_PB_ANR_MAX_COUNT) {
			for (count = 0; count < usim->anr_count; count++) {
				g_variant_builder_open(&anr_builder, G_VARIANT_TYPE("a{sv}"));

				dbg("ANR[%d] - Number: [%s] Description: [%s] AAS: [%s]",
					count, usim->anr[count].number,
					(usim->anr[count].description ? "YES" : "NO"),
					usim->anr[count].aas);
				g_variant_builder_add(&anr_builder, "{sv}",
					"number", g_variant_new_string(usim->anr[count].number));
				g_variant_builder_add(&anr_builder, "{sv}",
					"description", g_variant_new_boolean(usim->anr[count].description));
				g_variant_builder_add(&anr_builder, "{sv}",
					"aas", g_variant_new_string(usim->anr[count].aas));

				g_variant_builder_close(&anr_builder);
			}
		}
		var_anr = g_variant_builder_end(&anr_builder);
		g_variant_builder_add(&update_builder, "{sv}",
			"anr", var_anr);

		/* e-mail */
		dbg("e-mail count: [%d]", usim->email_count);
		g_variant_builder_add(&update_builder, "{sv}",
			"email_count", g_variant_new_byte(usim->email_count));

		g_variant_builder_init(&email_builder, G_VARIANT_TYPE("a{sv}"));
		if (usim->email_count && usim->email_count <= TEL_PB_EMAIL_MAX_COUNT) {
			for (count = 0; count < usim->email_count; count++) {
				char *tmp = g_strdup_printf("%d", count);

				dbg("e-mail[%s] - [%s]", tmp, usim->email[count]);
				g_variant_builder_add(&email_builder, "{sv}",
					tmp,
					g_variant_new_from_data(G_VARIANT_TYPE("ay"),
						usim->email[count], strlen(usim->email[count]),
						TRUE, NULL, NULL));
				g_free(tmp);
			}
		}
		var_email = g_variant_builder_end(&email_builder);
		g_variant_builder_add(&update_builder, "{sv}",
			"email", var_email);

		dbg("Hidden: [%s]", usim->hidden ? "YES" : "NO");
		g_variant_builder_add(&update_builder, "{sv}",
			"hidden", g_variant_new_boolean(usim->hidden));
	}
	else {
		TelPbSimRecord *sim = (TelPbSimRecord *)&(record->rec_u.sim);

		dbg("Name: [%s] Number: [%s]",
			sim->name, sim->number);
		g_variant_builder_add(&update_builder, "{sv}",
			"name", g_variant_new_string(sim->name));
		g_variant_builder_add(&update_builder, "{sv}",
			"number", g_variant_new_string(sim->number));
	}
	var_update = g_variant_builder_end(&update_builder);

	telephony_phonebook_call_update_record(handle->phonebook_proxy,
		record->pb_type, record->index, var_update,
		NULL,
		on_response_pb_update_sim_pb_record, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_pb_delete_sim_pb_record(GObject *source_object,
	GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelPbResult pb_result = TEL_PB_RESULT_FAILURE;
	GError *error = NULL;

	telephony_phonebook_call_delete_record_finish(handle->phonebook_proxy,
		(gint *)&pb_result, res, &error);

	CHECK_DEINIT(error, rsp_cb_data, pb_result);

	if (pb_result != TEL_PB_RESULT_SUCCESS)
		err("Failed to delete SIM PB record - pb_result: [%d]", pb_result);
	else
		dbg("delete SIM PB record: [SUCCESS]");

	RESP_CALLBACK_CALL(rsp_cb_data, pb_result, NULL);
}

EXPORT_API TelReturn tapi_pb_delete_sim_pb_record(TelHandle *handle,
	const TelPbRecordInfo *record,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && record && callback
		&& __tapi_check_pb_type(record->pb_type)
		&& record->index, TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	dbg("Delete SIM record - Phonebook type: [%d] Index: [%d] index",
		record->pb_type, record->index);

	telephony_phonebook_call_delete_record(handle->phonebook_proxy,
		record->pb_type, record->index,
		NULL,
		on_response_pb_delete_sim_pb_record, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}
