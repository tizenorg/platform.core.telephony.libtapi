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
#include "tapi_sat.h"

#include <string.h>

#define TEL_SAT_EVENT_LIST_MAX_COUNT	17 /**< max count of sat event list */

TelSatEventListType g_event_list[TEL_SAT_EVENT_LIST_MAX_COUNT];

void on_sat_signal_emit_handler(TelephonySat *sat, gchar *sender_name,
		gchar *signal_name, GVariant *parameters, gpointer user_data)
{
	TapiEvtCbData *evt_cb_data;
	TelHandle *handle = user_data;
	char *evt_id;

	if (handle == NULL)
	return;

	evt_id = g_strdup_printf("%s:%s", TELEPHONY_SAT_INTERFACE,
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

	if (!g_strcmp0(signal_name, "SetupMenu")) {
		TelSatSetupMenuInfo setup_menu;
		gint item_cnt;
		GVariant *items = NULL;

		memset(&setup_menu, 0, sizeof(TelSatSetupMenuInfo));

		g_variant_get(parameters, "(ibs@vibb)", &setup_menu.command_id, &setup_menu.is_main_menu_present, &setup_menu.sat_main_title, &items, &item_cnt,
					&setup_menu.is_sat_main_menu_help_info, &setup_menu.is_updated_sat_main_menu);

		setup_menu.sat_main_menu_item_count = item_cnt;
		if (items && item_cnt > 0) {
			int index = 0;
			GVariant *unbox;
			GVariantIter *iter;

			gchar *item_str;
			gint item_id;
			unbox = g_variant_get_variant(items);
			dbg("items(%p) items type_format(%s)", items, g_variant_get_type_string(unbox));

			g_variant_get(unbox, "a(si)", &iter);
			while (g_variant_iter_loop(iter,"(si)",&item_str, &item_id)) {
				setup_menu.sat_main_menu_item[index].item_id = item_id;
				memcpy(setup_menu.sat_main_menu_item[index].item_string, item_str, TEL_SAT_DEF_ITEM_STR_LEN_MAX + 6);
				dbg("item index(%d) id(%d) str(%s)",index, setup_menu.sat_main_menu_item[index].item_id, setup_menu.sat_main_menu_item[index].item_string);
				index++;
			}
			g_variant_iter_free(iter);
		}

		dbg("command id (%d)", setup_menu.command_id);
		dbg("menu present (%d)", setup_menu.is_main_menu_present);
		dbg("menu title (%s)", setup_menu.sat_main_title);
		dbg("item cnt (%d)", setup_menu.sat_main_menu_item_count);
		dbg("menu help info (%d)", setup_menu.is_sat_main_menu_help_info);
		dbg("menu updated (%d)", setup_menu.is_updated_sat_main_menu);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &setup_menu);
	}
	else if (!g_strcmp0(signal_name, "DisplayText")) {
		TelSatDisplayTextInd display_text;

		memset(&display_text, 0, sizeof(TelSatDisplayTextInd));

		g_variant_get(parameters, "(isiibbb)", &display_text.command_id, &display_text.text.string, &display_text.text.string_len, &display_text.duration,
			&display_text.is_priority_high, &display_text.is_user_resp_required, &display_text.immediately_resp);

		dbg("command id (%d)", display_text.command_id);
		dbg("display text (%s)", display_text.text.string);
		dbg("string len(%d)", display_text.text.string_len);
		dbg("duration (%d)", display_text.duration);
		dbg("high priority (%d)", display_text.is_priority_high);
		dbg("user response required(%d)", display_text.is_user_resp_required);
		dbg("immediately response (%d)", display_text.immediately_resp);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &display_text);
	}
	else if (!g_strcmp0(signal_name, "SelectItem")) {
		TelSatSelectItemInd select_item;
		gint menu_cnt;
		GVariant *menu_items;

		memset(&select_item, 0, sizeof(TelSatSelectItemInd));

		g_variant_get(parameters, "(ibsiii@v)", &select_item.command_id, &select_item.is_help_info_available, &select_item.text.string,
			&select_item.text.string_len, &select_item.default_item_index, &menu_cnt, &menu_items);

		select_item.menu_item_count = menu_cnt;
		if (menu_items && menu_cnt > 0) {
			int index = 0;
			GVariant *unbox;
			GVariantIter *iter;

			gchar *item_str;
			gint item_id, item_len;
			unbox = g_variant_get_variant(menu_items);
			dbg("items(%p) items type_format(%s)", menu_items, g_variant_get_type_string(unbox));

			g_variant_get(unbox, "a(iis)", &iter);
			while (g_variant_iter_loop(iter,"(iis)",&item_id, &item_len, &item_str)) {
				select_item.menu_item[index].item_id = item_id;
				select_item.menu_item[index].text_len = item_len;
				memcpy(select_item.menu_item[index].text, item_str, TEL_SAT_ITEM_TEXT_LEN_MAX + 1);
				dbg("item index(%d) id(%d) len(%d) str(%s)", index,
						select_item.menu_item[index].item_id, select_item.menu_item[index].text_len, select_item.menu_item[index].text);
				index++;
			}
			g_variant_iter_free(iter);
		}

		dbg("command id (%d)", select_item.command_id);
		dbg("help info(%d)", select_item.is_help_info_available);
		dbg("selected item string(%s)", select_item.text.string);
		dbg("string len(%d)", select_item.text.string_len);
		dbg("default item index(%d)", select_item.default_item_index);
		dbg("item count(%d)", select_item.menu_item_count);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &select_item);
	}
	else if (!g_strcmp0(signal_name, "GetInkey")) {
		TelSatGetInkeyInd get_inkey;

		memset(&get_inkey, 0, sizeof(TelSatGetInkeyInd));

		g_variant_get(parameters, "(iiibbsii)", &get_inkey.command_id, &get_inkey.key_type, &get_inkey.input_char_mode,
			&get_inkey.is_numeric, &get_inkey.is_help_info_available, &get_inkey.text.string, &get_inkey.text.string_len, &get_inkey.duration);

		dbg("command id(%d)", get_inkey.command_id);
		dbg("key type(%d)", get_inkey.key_type);
		dbg("input character mode(%d)", get_inkey.input_char_mode);
		dbg("numeric(%d)", get_inkey.is_numeric);
		dbg("help info available(%d)", get_inkey.is_help_info_available);
		dbg("text (%s)", get_inkey.text.string);
		dbg("text length", get_inkey.text.string_len);
		dbg("duration", get_inkey.duration);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &get_inkey);
	}
	else if (!g_strcmp0(signal_name, "GetInput")) {
		TelSatGetInputInd get_input;

		memset(&get_input, 0, sizeof(TelSatGetInputInd));

		g_variant_get(parameters, "(iibbbsiiisi)", &get_input.command_id, &get_input.input_char_mode, &get_input.is_numeric, &get_input.is_help_info_available, &get_input.is_echo_input,
			&get_input.text.string, &get_input.text.string_len, &get_input.resp_len.max, &get_input.resp_len.min, &get_input.default_text.string, &get_input.default_text.string_len);

		dbg("command id(%d)", get_input.command_id);
		dbg("input character mode(%d)", get_input.input_char_mode);
		dbg("numeric(%d)", get_input.is_numeric);
		dbg("help info avaiable(%d)", get_input.is_help_info_available);
		dbg("echo input(%d)", get_input.is_echo_input);
		dbg("text(%s)", get_input.text.string);
		dbg("text length(%d)", get_input.text.string_len);
		dbg("response length max(%d)", get_input.resp_len.max);
		dbg("response length min(%d)", get_input.resp_len.min);
		dbg("default text(%s)", get_input.default_text.string);
		dbg("default text length(%d)", get_input.default_text.string_len);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &get_input);
	}
	else if (!g_strcmp0(signal_name, "SendSMS")) {
		TelSatSendSmsIndSmsData send_sms;
		GVariant *tpdu_data;
		gchar* text;
		gint text_len;

		memset(&send_sms, 0, sizeof(TelSatSendSmsIndSmsData));

		g_variant_get(parameters, "(isibiisii@vi)", &send_sms.command_id, &text, &text_len, &send_sms.is_packing_required, &send_sms.address.ton, &send_sms.address.npi,
					&send_sms.address.dialing_number, &send_sms.address.dialing_number_len, &send_sms.sms_tpdu.tpdu_type, &tpdu_data, &send_sms.sms_tpdu.data_len);

		if (tpdu_data) {
			int index = 0;
			guchar data;
			GVariantIter *iter = NULL;
			GVariant *inner_gv = NULL;

			inner_gv = g_variant_get_variant(tpdu_data);
			dbg("tpdu data exist type_format(%s)", g_variant_get_type_string(inner_gv));

			g_variant_get(inner_gv, "ay", &iter);
			while ( g_variant_iter_loop (iter, "y", &data)) {
				dbg("index(%d) data(%c)", index, data);
				send_sms.sms_tpdu.data[index] = data;
				index++;
			}
			g_variant_iter_free(iter);
			g_variant_unref(inner_gv);
		}

		dbg("command id(%d)", send_sms.command_id);
		dbg("packing required(%d)", send_sms.is_packing_required);
		dbg("address ton(%d)", send_sms.address.ton);
		dbg("address npi(%d)", send_sms.address.npi);
		dbg("address dialing number (%s)", send_sms.address.dialing_number);
		dbg("address number length (%d)", send_sms.address.dialing_number_len);
		dbg("tpdu type (%d)", send_sms.sms_tpdu.tpdu_type);
		dbg("tpdu length (%d)", send_sms.sms_tpdu.data_len);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &send_sms);
	}
	else if (!g_strcmp0(signal_name, "SetupEventList")) {
		int g_index = 0;
		gint event_cnt;
		GVariant *evt_table;
		TelSatEventListData event_list;

		memset(&event_list, 0, sizeof(TelSatEventListData));

		g_variant_get(parameters, "(i@v)", &event_cnt, &evt_table);

		if (evt_table) {
			guchar data;
			GVariantIter *iter = NULL;
			GVariant *inner_gv = NULL;

			inner_gv = g_variant_get_variant(evt_table);
			dbg("event list exist type_format(%s)", g_variant_get_type_string(inner_gv));

			g_variant_get(inner_gv, "ai", &iter);
			while ( g_variant_iter_loop (iter, "i", &data)) {
				dbg("g_index(%d) event(%d)", g_index, data);
				g_event_list[g_index] = data;

				if (data == TEL_SAT_EVENT_IDLE_SCREEN_AVAILABLE) {
					event_list.is_idle_screen_available = 1;
				}
				else if (data == TEL_SAT_EVENT_LANGUAGE_SELECTION) {
					event_list.is_language_selection = 1;
				}
				else if (data == TEL_SAT_EVENT_BROWSER_TERMINATION) {
					event_list.is_browser_termination = 1;
				}
				else if (data == TEL_SAT_EVENT_DATA_AVAILABLE) {
					event_list.is_data_available = 1;
				}
				else if (data == TEL_SAT_EVENT_CHANNEL_STATUS) {
					event_list.is_channel_status = 1;
				}
				g_index++;
			}
			//while end
			g_variant_iter_free(iter);
			g_variant_unref(inner_gv);
		}

		dbg("event list cnt(%d)", event_cnt);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &event_list);
	}
	else if (!g_strcmp0(signal_name, "Refresh")) {
		TelSatRefreshInd refresh_info;
		gint file_cnt = 0;
		GVariant *file_list = NULL;

		memset(&refresh_info, 0, sizeof(TelSatRefreshInd));

		g_variant_get(parameters, "(ii@v)", &refresh_info.command_id, &refresh_info.refresh_mode, &file_list);

		if (file_list) {
			int g_index = 0;
			guchar data;
			GVariantIter *iter = NULL;
			GVariant *inner_gv = NULL;

			inner_gv = g_variant_get_variant(file_list);
			dbg("file list exist type_format(%s)", g_variant_get_type_string(inner_gv));

			g_variant_get(inner_gv, "ai", &iter);
			while ( g_variant_iter_loop (iter, "i", &data)) {
				dbg("g_index(%d) file id(%d)", g_index, data);
				/* TODO: Use TelSimFileID from tel_sim.h */
				//refresh_info.file_id[g_index] = data;
				g_index++;
			}
			file_cnt = g_index;

			//while end
			g_variant_iter_free(iter);
			g_variant_unref(inner_gv);
		}
		refresh_info.file_count = file_cnt;

		dbg("refresh event/file cnt(%d)", refresh_info.file_count);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &refresh_info);
	}
	else if (!g_strcmp0(signal_name, "SendDtmf")) {
		TelSatSendDtmfIndDtmfData send_dtmf;

		gint command_id = 0;
		gint text_len = 0;
		gchar *text = NULL;

		memset(&send_dtmf, 0, sizeof(TelSatSendDtmfIndDtmfData));

		g_variant_get(parameters, "(isisi)", &send_dtmf.command_id, &text, &text_len, &send_dtmf.dtmf_string.string, &send_dtmf.dtmf_string.string_len);

		send_dtmf.command_id = command_id;
		if (text_len <= 0 || g_strcmp0(text,"") == 0 ) {
			send_dtmf.is_hidden_mode = 1;
		}

		dbg("dtmf event command id(%d)", send_dtmf.command_id);
		dbg("dtmf event dtmf(%s)", send_dtmf.dtmf_string.string);

		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &send_dtmf);
	}
	else if (!g_strcmp0(signal_name, "EndProactiveSession")) {
		int command_type = 0;

		g_variant_get(parameters, "(i)", &command_type);
		dbg("end session evt : command type(%d)", command_type);
		EVT_CALLBACK_CALL(handle, evt_cb_data, evt_id, &command_type);
	}
	else {
		dbg("not handled Sat noti[%s]", signal_name);
	}
}

static GVariant* generate_event_download_data(const TelSatEventDownloadReqInfo *event_data)
{
	GVariant *result = NULL;

	switch(event_data->event_download_type)
	{
		case TEL_SAT_EVENT_IDLE_SCREEN_AVAILABLE:{
			gboolean idle_screen;
			dbg("idle screen available (%d)", event_data->event_download_data.is_idle_screen_available);

			idle_screen = event_data->event_download_data.is_idle_screen_available;
			result = g_variant_new("(b)", idle_screen);
		} break;
		case TEL_SAT_EVENT_LANGUAGE_SELECTION:{
			gint selected_language;
			dbg("selected language (%d)", event_data->event_download_data.language_selection_event_req_info);

			selected_language = event_data->event_download_data.language_selection_event_req_info;
			result = g_variant_new("(i)", selected_language);
		} break;
		case TEL_SAT_EVENT_BROWSER_TERMINATION:{
			gint browser_termination_cause;
			dbg("Cause of browser termination (%d)", event_data->event_download_data.browser_termination_event_req_info);

			browser_termination_cause = event_data->event_download_data.browser_termination_event_req_info;
			result = g_variant_new("(i)", browser_termination_cause);
		} break;
		case TEL_SAT_EVENT_DATA_AVAILABLE:
		case TEL_SAT_EVENT_CHANNEL_STATUS:
		default :
			dbg("not support download event (%d)", event_data->event_download_type);
			result = g_variant_new("()");
			break;
	}

	return result;
}

static GVariant* generate_app_exec_result_data(TelSatAppsRetInfo *result_info)
{
	GVariant *result = NULL;

	switch(result_info->command_type)
	{
		case TEL_SAT_PROATV_CMD_SETUP_MENU:{
			gint exec_result;
			dbg("setup menu : result format (i)");

			exec_result = result_info->apps_ret.setup_menu_resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_REFRESH:{
			gint app_type, exec_result;
			dbg("refresh : result format (ii)");

			app_type = result_info->apps_ret.refresh.app_type;
			exec_result = result_info->apps_ret.refresh.resp;
			result = g_variant_new("(ii)", app_type, exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_SETUP_CALL:{
			gint exec_result, me_problem, call_ctrl_problem, call_cause;
			dbg("setup call : result format (iiii)");

			exec_result = result_info->apps_ret.setup_call.resp;
			me_problem = result_info->apps_ret.setup_call.me_problem;
			call_ctrl_problem = result_info->apps_ret.setup_call.permanent_call_ctrl_problem;
			/* TODO: need to confirm call_cause */
			//call_cause = result_info->apps_ret.setup_call.call_cause;
			call_cause = 0;
			result = g_variant_new("(iiii)", exec_result, me_problem, call_ctrl_problem, call_cause);
		} break;
		case TEL_SAT_PROATV_CMD_SEND_SS:{
			gint exec_result, me_problem, ss_cause, call_ctrl_problem, ss_str_len;
			gchar *ss_string = NULL;
			dbg("send ss : result format (iiisii)");

			exec_result = result_info->apps_ret.send_ss.resp;
			me_problem = result_info->apps_ret.send_ss.me_problem;
			/* TODO: need to confirm ss_cause */
			//ss_cause = result_info->apps_ret.send_ss.ss_cause;
			ss_cause = 0;
			call_ctrl_problem = result_info->apps_ret.send_ss.additional_call_ctrl_problem_info;
			ss_str_len = result_info->apps_ret.send_ss.ss_string.string_len;
			ss_string = g_strdup((gchar*)result_info->apps_ret.send_ss.ss_string.string);

			result = g_variant_new("(iiisii)", exec_result, me_problem, ss_cause, ss_string, ss_str_len, call_ctrl_problem);
		} break;
		case TEL_SAT_PROATV_CMD_SEND_USSD:{
			int index;
			gint exec_result, me_problem, ss_cause, ussd_str_len;
			GVariantBuilder *builder = NULL;
			GVariant *ussd_str = NULL;

			dbg("send ussd : result format (iiivi)");

			exec_result = result_info->apps_ret.send_ussd.resp;
			me_problem = result_info->apps_ret.send_ussd.me_problem;
			/* TODO: need to confirm ss_cause */
			//ss_cause = result_info->apps_ret.send_ussd.ss_cause;
			ss_cause = 0;
			ussd_str_len = result_info->apps_ret.send_ussd.ussd_string.string_len;

			builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
			for (index = 0; index < ussd_str_len; index++) {
				dbg("index(%d) data(%d)", index, result_info->apps_ret.send_ussd.ussd_string.string[index]);
				g_variant_builder_add(builder, "y", result_info->apps_ret.send_ussd.ussd_string.string[index]);
			}
			ussd_str = g_variant_builder_end(builder);

			result = g_variant_new("(iiivi)", exec_result, me_problem, ss_cause, ussd_str, ussd_str_len);
		} break;
		case TEL_SAT_PROATV_CMD_SEND_SMS:{
			gint exec_result;
			dbg("send sms: result format (i)");

			exec_result = result_info->apps_ret.send_sms_resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_SEND_DTMF:{
			gint exec_result;
			dbg("send DTMF: result format (i)");

			exec_result = result_info->apps_ret.send_dtmf_resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_LAUNCH_BROWSER:{
			gint exec_result, browser_problem;
			dbg("launch browser: result format (ii)");

			exec_result = result_info->apps_ret.launch_browser.resp;
			browser_problem = result_info->apps_ret.launch_browser.browser_problem;
			result = g_variant_new("(ii)", exec_result, browser_problem);
		} break;
		case TEL_SAT_PROATV_CMD_SETUP_IDLE_MODE_TEXT:{
			gint exec_result;
			dbg("setup idle mode text: result format (i)");

			exec_result = result_info->apps_ret.setup_idle_mode_text_resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_LANGUAGE_NOTIFICATION:{
			gint exec_result;
			dbg("language notification: result format (i)");

			exec_result = result_info->apps_ret.language_noti_resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_PROVIDE_LOCAL_INFO:{
			gint exec_result;
			dbg("provide local info: result format (i)");

			exec_result = result_info->apps_ret.provide_local_info.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_PLAY_TONE:{
			gint exec_result;
			dbg("play tone: result format (i)");

			exec_result = result_info->apps_ret.play_tone_resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TEL_SAT_PROATV_CMD_DISPLAY_TEXT:{
			gint exec_result, me_problem;
			dbg("display text: result format (ii)");

			exec_result = result_info->apps_ret.display_text.resp;
			me_problem = result_info->apps_ret.display_text.me_problem;
			result = g_variant_new("(ii)", exec_result, me_problem);
		} break;
		default:
			dbg("unhandled command type(0x%x", result_info->command_type);
			break;
	}

	return result;
}

static void on_response_sat_select_menu(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSatResult result = TEL_SAT_RESULT_FAILURE;
	GError *error = NULL;
	gint envelop_rsp;

	/* Finish and extract the response data */
	telephony_sat_call_select_menu_finish(handle->sat_proxy,
						(gint *)&result, &envelop_rsp, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SAT_RESULT_SUCCESS) {
		err("Failed to select menu item - result: [%d] envelop response: [%d]", result, envelop_rsp);

		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	dbg("menu selection: [SUCCESS]");
	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, result, &envelop_rsp);
}

/**
 *
 * To send envelope command (MENU SELECTION) to USIM.
 *
 * @return		values in enum TelReturn.
 * @param[in]		TelSatMenuSelectionReqInfo menu selection evelope data.
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSatMenuSelectionReqInfo
 */
EXPORT_API TelReturn tapi_sat_select_menu(TelHandle *handle, const TelSatMenuSelectionReqInfo *menu_select, TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && callback, TEL_RETURN_INVALID_PARAMETER);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	dbg("item_identifier: [%d] is_help_requested: [%s]",
		menu_select->item_identifier, menu_select->is_help_requested ? "HELP" : "NO HELP");

	telephony_sat_call_select_menu(handle->sat_proxy, menu_select->item_identifier,
					menu_select->is_help_requested,
					NULL,
					on_response_sat_select_menu,
					rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

static void on_response_sat_download_event(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	TapiRespCbData *rsp_cb_data = user_data;
	TelHandle *handle = GET_TAPI_HANDLE(rsp_cb_data);
	TelSatResult result = TEL_SAT_RESULT_FAILURE;
	GError *error = NULL;
	gint envelop_rsp;

	/* Finish and extract the response data */
	telephony_sat_call_download_event_finish(handle->sat_proxy,
						(gint *)&result, &envelop_rsp, res, &error);

	/* Process Response data */
	CHECK_DEINIT(error, rsp_cb_data, result);

	if (result != TEL_SAT_RESULT_SUCCESS) {
		err("Failed to download event - result: [%d] envelop response: [%d]", result, envelop_rsp);
		/* Invoke response callback */
		RESP_CALLBACK_CALL(rsp_cb_data, result, NULL);
		return;
	}

	dbg("download event: [SUCCESS]");
	/* Invoke response callback */
	RESP_CALLBACK_CALL(rsp_cb_data, result, &envelop_rsp);
}

/**
 *
 * API to send event download to SIM.
 *
 * @return		values in enum TelReturn.
 * @param[in]		TelSatEventDownloadReqInfo	event download data.
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSatEventDownloadReqInfo
 */
EXPORT_API TelReturn tapi_sat_download_event(TelHandle *handle,
	const TelSatEventDownloadReqInfo *event_data,
	TapiResponseCb callback, void *user_data)
{
	TapiRespCbData *rsp_cb_data;
	GVariant *download_data = NULL;
	gboolean evt_check = FALSE;
	int g_index = 0;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && event_data && callback, TEL_RETURN_INVALID_PARAMETER);

	if (event_data->event_download_type != TEL_SAT_EVENT_BROWSER_TERMINATION) {
		for (g_index = 0; g_event_list[g_index] > 0; g_index++) {
			if (g_event_list[g_index] == event_data->event_download_type) {
				dbg("event (%d) shoud be passed to sim", event_data->event_download_type);
				evt_check = TRUE;
				break;
			}
		}
	}
	else
		evt_check = TRUE;

	if (!evt_check) {
		dbg("Request (%d) not initiated from SIM", event_data->event_download_type);
		return TEL_RETURN_OPERATION_NOT_SUPPORTED;
	}

	download_data = generate_event_download_data(event_data);

	MAKE_RESP_CB_DATA(rsp_cb_data, handle, callback, user_data);

	telephony_sat_call_download_event(handle->sat_proxy,
		event_data->event_download_type, TEL_SAT_DEVICE_ID_ME,
		TEL_SAT_DEVICE_ID_SIM, download_data, NULL,
		on_response_sat_download_event, rsp_cb_data);

	return TEL_RETURN_SUCCESS;
}

/**
 *
 * API to get main menu information.
 *
 * @return		values in enum TelReturn.
 * @param[out]	TelSatSetupMenuInfo	main menu data.
 * @Interface		Synchronous.
 * @remark
 * @Refer		TelSatSetupMenuInfo
 */
EXPORT_API TelReturn tapi_sat_get_main_menu_info(TelHandle *handle, TelSatSetupMenuInfo *main_menu)
{
	GError *error = NULL;
	TelReturn result = TEL_RETURN_SUCCESS;
	gint sat_res;
	gchar *title;
	GVariant *items = NULL;
	GVariant *unbox;
	GVariantIter *iter;
	gchar *item_str;
	gint item_id;
	int index = 0;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle, TEL_RETURN_INVALID_PARAMETER);

	telephony_sat_call_get_main_menu_info_sync(handle->sat_proxy,
		&sat_res, &main_menu->command_id, &main_menu->is_main_menu_present,
		(gchar **)&title, &items, (gint *)&main_menu->sat_main_menu_item_count,
		&main_menu->is_sat_main_menu_help_info, &main_menu->is_updated_sat_main_menu,
		NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		result = TEL_RETURN_FAILURE;
		goto out;
	}
	memcpy(main_menu->sat_main_title, title, TEL_SAT_DEF_TITLE_LEN_MAX);
	dbg("Command ID: [%d] Menu: [%s] Title: [%s] count: [%d]",
		main_menu->command_id, main_menu->is_main_menu_present ? "PRESENT" : "NOT PRESENT",
		main_menu->sat_main_title, main_menu->sat_main_menu_item_count);
	if (items && main_menu->sat_main_menu_item_count > 0) {
		unbox = g_variant_get_variant(items);
		dbg("items(%p) items type_format(%s)", items, g_variant_get_type_string(unbox));

		g_variant_get(unbox, "a(si)", &iter);
		while (g_variant_iter_loop(iter,"(si)",&item_str, &item_id)) {
			main_menu->sat_main_menu_item[index].item_id = item_id;

			if (strlen(item_str) > TEL_SAT_DEF_ITEM_STR_LEN_MAX + 6) {
				memcpy(main_menu->sat_main_menu_item[index].item_string, item_str, TEL_SAT_DEF_ITEM_STR_LEN_MAX + 6);
			}
			else {
				memcpy(main_menu->sat_main_menu_item[index].item_string, item_str, strlen(item_str));
			}

			dbg("item index(%d) id(%d) str(%s)",index, main_menu->sat_main_menu_item[index].item_id, main_menu->sat_main_menu_item[index].item_string);
			index++;
		}
		g_variant_iter_free(iter);
	}

	if (sat_res != TEL_SAT_RESULT_SUCCESS)
		result = TEL_RETURN_FAILURE;

out:
	return result;
}

/**
 *
 * API to send UI display status.
 *
 * @return		values in enum TelReturn.
 * @param[in]		TelSatUiDisplayStatusType 	display status data.
 *  command_id	command identifier
 * @Interface		Synchronous.
 * @remark
 * @Refer		TelSatUiDisplayStatusType
 */
EXPORT_API TelReturn tapi_sat_send_ui_display_status(TelHandle *handle, int command_id, TelSatUiDisplayStatusType status)
{
	GError *error = NULL;
	TelReturn result = TEL_RETURN_SUCCESS;
	gint sat_res;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && command_id && status , TEL_RETURN_INVALID_PARAMETER);

	telephony_sat_call_send_ui_display_status_sync(handle->sat_proxy,
							command_id, status, &sat_res,
							NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		result = TEL_RETURN_FAILURE;
		goto out;
	}

	if (sat_res != TEL_SAT_RESULT_SUCCESS)
		result = TEL_RETURN_FAILURE;

out:
	return result;
}

/**
 *
 * API to send UI user confirmation data.
 *
 * @return		values in enum TelReturn.
 * @param[in]		command_id	command identifier
 * command_type	command type
 * key_type		user confirmation value
 * addtional_data	addtional data
 * data_len		data length
 * @Interface		Synchronous.
 * @remark
 * @Refer
 */
EXPORT_API TelReturn tapi_sat_send_ui_user_confirm(TelHandle *handle, TelSatUiUserConfirmInfo *user_confirm_data)
{
	GError *error = NULL;
	TelReturn result = TEL_RETURN_SUCCESS;
	gint sat_res, data_len;
	GVariantBuilder *builder = NULL;
	GVariant *add_data_var =  NULL;
	int index = 0;
	gchar *data;

	dbg("Entry");

	TEL_RETURN_IF_CHECK_FAIL(handle && user_confirm_data->command_id && user_confirm_data->command_type
								   && user_confirm_data->key_type && user_confirm_data->additional_data, TEL_RETURN_INVALID_PARAMETER);

	data = (gchar *)user_confirm_data->additional_data;
	data_len = user_confirm_data->data_len;

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
	for (index = 0; index < data_len; index++) {
		dbg("index(%d) data(%d)", index, data[index]);
		g_variant_builder_add(builder, "y", data[index]);
	}
	add_data_var = g_variant_builder_end(builder);

	telephony_sat_call_send_user_confirm_sync(handle->sat_proxy,
							user_confirm_data->command_id, user_confirm_data->command_type,
							user_confirm_data->key_type, add_data_var, &sat_res,
							NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		result = TEL_RETURN_FAILURE;
		goto out;
	}

	if (sat_res != TEL_SAT_RESULT_SUCCESS)
		result = TEL_RETURN_FAILURE;

out:
	return result;
}

/**
 *
 * API to send application excution result.
 *
 * @return		values in enum TelReturn.
 * @param[in]		TelSatAppsRetInfo	application result information data.
 * @Interface		Synchronous.
 * @remark
 * @Refer		TelSatAppsRetInfo
 */
EXPORT_API TelReturn tapi_sat_send_app_exec_result(TelHandle *handle, TelSatAppsRetInfo *app_ret_info)
{
	GError *error = NULL;
	TelReturn result = TEL_RETURN_SUCCESS;
	gint sat_res;
	GVariant *app_exec_result = NULL;

	dbg("Entry");

	app_exec_result = generate_app_exec_result_data(app_ret_info);

	TEL_RETURN_IF_CHECK_FAIL(handle && app_ret_info->command_id && app_ret_info->command_type && app_exec_result, TEL_RETURN_INVALID_PARAMETER);

	telephony_sat_call_send_app_exec_result_sync(handle->sat_proxy,
							app_ret_info->command_id, app_ret_info->command_type, app_exec_result, &sat_res,
							NULL, &error);
	if (error) {
		dbg("dbus error = %d (%s)", error->code, error->message);
		g_error_free(error);
		result = TEL_RETURN_FAILURE;
		goto out;
	}

	if (sat_res != TEL_SAT_RESULT_SUCCESS)
		result = TEL_RETURN_FAILURE;

out:
	return result;
}
