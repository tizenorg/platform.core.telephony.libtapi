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

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include "tapi_common.h"
#include "TapiUtility.h"

//
#include "ITapiPS.h"
#include "TelNetwork.h"
#include "TelSms.h"
#include "TelSat.h"
#include "TelSs.h"
#include "TelCall.h"

//
#include "common.h"
#include "tapi_log.h"

extern char *g_cookie_name;
extern int g_cookie_size;

TelSatEventDownloadType_t g_event_list[TAPI_SAT_EVENT_LIST_MAX_COUNT] = {-1};

/*static void _cookie_deinit(void)
{
	if (g_cookie_name)
		free(g_cookie_name);

	g_cookie_name = NULL;
	g_cookie_size = 0;
}*/

static void on_signal_callback(GDBusConnection *conn, const gchar *name, const gchar *path, const gchar *interface,
		const gchar *sig, GVariant *param, gpointer user_data)
{
	TapiHandle *handle = user_data;
	struct tapi_evt_cb *evt_cb_data = NULL;
	char *noti_id;
	void *noti_data = NULL;

	if (!handle) {
		dbg("handle is NULL");
		return;
	}

	noti_id = g_strdup_printf("%s:%s", interface, sig);

	evt_cb_data = g_hash_table_lookup(handle->evt_list, noti_id);
	if (!evt_cb_data) {
		dbg("can't find noti_id(%s) callback info", noti_id);
		g_free(noti_id);
		return;
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_NETWORK_INTERFACE)) {
		if (!g_strcmp0(sig, "RegistrationStatus")) {
			struct tel_noti_network_registration_status noti;

			g_variant_get(param, "(iiib)", &noti.cs, &noti.ps, &noti.type, &noti.is_roaming);
			noti_data = &noti;
		}
		else if (!g_strcmp0(sig, "Info")) {
			struct tel_noti_network_info noti;

			memset(&noti, 0, sizeof(struct tel_noti_network_info));
			g_variant_get(param, "(ii)", &noti.rssi, &noti.battery);

			noti_data = &noti;
		}
		else if (!g_strcmp0(sig, "CellInfo")) {
			struct tel_noti_network_cell_info noti;

			memset(&noti, 0, sizeof(struct tel_noti_network_cell_info));
			g_variant_get(param, "(ii)", &noti.lac, &noti.cell_id);

			noti_data = &noti;
		}
		else if (!g_strcmp0(sig, "Change")) {
			struct tel_noti_network_change noti;
			char *plmn = NULL;

			memset(&noti, 0, sizeof(struct tel_noti_network_change));

			g_variant_get(param, "(isi)", &noti.act, &plmn, &noti.lac);

			if (plmn) {
				snprintf(noti.plmn, 7, "%s", plmn);
			}

			noti_data = &noti;
		}
		else if (!g_strcmp0(sig, "TimeInfo")) {
			struct tel_noti_network_time_info noti;
			char *plmn = NULL;

			memset(&noti, 0, sizeof(struct tel_noti_network_time_info));
			g_variant_get(param, "(iiiiiiiiibs)", &noti.year, &noti.month, &noti.day,
					&noti.hour, &noti.minute, &noti.second,
					&noti.wday, &noti.gmtoff, &noti.dstoff, &noti.isdst,
					&plmn);

			if (plmn) {
				snprintf(noti.plmn, 7, "%s", plmn);
			}

			noti_data = &noti;
		}
		else if (!g_strcmp0(sig, "Identity")) {
			struct tel_noti_network_identity noti;
			char *plmn = NULL, *s_name = NULL, *f_name = NULL;

			memset(&noti, 0, sizeof(struct tel_noti_network_identity));

			g_variant_get(param, "(sss)", &plmn, &s_name, &f_name);

			if (plmn) {
				snprintf(noti.plmn, 7, "%s", plmn);
			}
			if (s_name) {
				snprintf(noti.short_name, 17, "%s", s_name);
			}
			if (f_name) {
				snprintf(noti.full_name, 33, "%s", f_name);
			}

			noti_data = &noti;
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_SMS_INTERFACE)) {
		if (!g_strcmp0(sig, "IncommingMsg")) {
			struct tel_noti_sms_incomming_msg noti;

			const char *sca = NULL;
			const char *tpdu = NULL;
			gsize length;
			guchar *decoded_sca = NULL;
			guchar *decoded_tpdu = NULL;

			memset(&noti, 0, sizeof(struct tel_noti_sms_incomming_msg));

			g_variant_get(param, "(sis)", &sca, &noti.MsgLength, &tpdu);

			decoded_sca = g_base64_decode(sca, &length);
			memcpy(&(noti.Sca[0]), decoded_sca, TAPI_SIM_SMSP_ADDRESS_LEN);

			decoded_tpdu = g_base64_decode(tpdu, &length);
			memcpy(&(noti.szData[0]), decoded_tpdu, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);

			noti_data = &noti;

			if(decoded_sca)
				g_free(decoded_sca);

			if(decoded_tpdu)
				g_free(decoded_tpdu);

		}
		else if (!g_strcmp0(sig, "IncommingCbMsg")) {
			struct tel_noti_sms_incomming_cb_msg noti;

			const char *cbMsg = NULL;
			gsize length;
			guchar *decoded_cbMsg = NULL;

			memset(&noti, 0, sizeof(struct tel_noti_sms_incomming_cb_msg));
			g_variant_get(param, "(iis)", &noti.CbMsgType, &noti.Length, &cbMsg);

			decoded_cbMsg = g_base64_decode(cbMsg, &length);
			memcpy(&(noti.szMsgData[0]), decoded_cbMsg, TAPI_NETTEXT_CB_PAGE_SIZE_MAX + 1);

			noti_data = &noti;

			if(decoded_cbMsg)
				g_free(decoded_cbMsg);

		}
		else if (!g_strcmp0(sig, "MemoryStatus")) {
			int noti = 0;

			g_variant_get(param, "(i)", &noti);
			noti_data = &noti;
		}
		else if (!g_strcmp0(sig, "SmsReady")) {
			gboolean noti = 0;

			g_variant_get(param, "(b)", &noti);
			noti_data = &noti;
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_CALL_INTERFACE)) {
		if (!g_strcmp0(sig, "VoiceCallStatusIdle") || !g_strcmp0(sig, "VideoCallStatusIdle") ) {
			TelCallStatusIdleNoti_t data;
			int start_time = 0, end_time = 0;
			g_variant_get(param, "(iiii)", &data.id, &data.cause, &start_time, &end_time);
			dbg("[ check ] %s : call_id(%d), end_cause(0x%x)", "Status Idle noti", data.id, data.cause);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "VoiceCallStatusDialing") || !g_strcmp0(sig, "VideoCallStatusDialing")) {
			TelCallStatusDialingNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : call_id(%d)", "Status Dialing noti", data.id);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "VoiceCallStatusAlert") || !g_strcmp0(sig, "VideoCallStatusAlert")) {
			TelCallStatusAlertNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : call_id(%d)", "Status Alert noti", data.id);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "VoiceCallStatusActive") || !g_strcmp0(sig, "VideoCallStatusActive")) {
			TelCallStatusActiveNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : call_id(%d)", "Status Active noti", data.id);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "VoiceCallStatusHeld") ) {
			TelCallStatusHeldNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : call_id(%d)", "Status Held noti", data.id);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "VoiceCallStatusIncoming") || !g_strcmp0(sig, "VideoCallStatusIncoming")) {
			TelCallStatusIncomingNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : call_id(%d)", "Status Incoming noti", data.id);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "Waiting")) {
			TelCallInfoWaitingNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Waiting noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "Forwarded")) {
			TelCallInfoForwardedNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Forwarded noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "BarredIncoming")) {
			TelCallInfoBarredIncomingNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Barred Incoming noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "BarredOutgoing")) {
			TelCallInfoBarredOutgoingNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Barred Outgoing noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "ForwardConditional")) {
			TelCallInfoForwardConditionalNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Forward Conditional noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "ForwardUnconditional")) {
			TelCallInfoForwardUnconditionalNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Forward Unconditional noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "CallActive")) {
			TelCallInfoActiveNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Call Active noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "CallHeld")) {
			TelCallInfoHeldNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Call Held noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "CallJoined")) {
			TelCallInfoJoinedNoti_t data;
			g_variant_get(param, "(i)", &data.id);
			dbg("[ check ] %s : data.id(%d)", "Call Info Call Joined noti", data.id);
			noti_data = &data.id;
		}
		if (!g_strcmp0(sig, "CallSoundPath")) {
			TelCallSoundPathNoti_t data;
			g_variant_get(param, "(i)", &data.path);
			dbg("[ check ] %s : path(%d)", "Call Sound Path noti", data.path);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "CallSoundRingbackTone")) {
			TelCallSoundRingbackToneNoti_t status;
			g_variant_get(param, "(i)", &status);
			dbg("[ check ] %s : status(%d)", "Call Sound Ringbacktone noti", status);
			noti_data = &status;
		}
		if (!g_strcmp0(sig, "CallSoundWbamr")) {
			TelCallSoundWbamrNoti_t status;
			g_variant_get(param, "(i)", &status);
			dbg("[ check ] %s : status(%d)", "Call Sound Wbamr noti", status);
			noti_data = &status;
		}
		if (!g_strcmp0(sig, "CallSoundEqualization")) {
			TelCallSoundEqualizationNoti_t data;
			g_variant_get(param, "(ii)", &data.mode, &data.direction);
			dbg("[ check ] %s : mode(%d), direction(%d)", "Call Sound Equalization noti", data.mode, data.direction);
			noti_data = &data;
		}
		if (!g_strcmp0(sig, "CallSoundNoiseReduction")) {
			TelCallSoundNoiseReductionNoti_t data;
			g_variant_get(param, "(i)", &data.status);
			dbg("[ check ] %s : status(%d)", "Call Sound Noise Reduction noti", data.status);
			noti_data = &data;
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_SAT_INTERFACE)) {
		if (!g_strcmp0(sig, "SetupMenu")) {
			TelSatSetupMenuInfo_t setup_menu;

			gchar *title;
			gint command_id, item_cnt;
			gboolean b_present, b_helpinfo, b_updated;
			GVariant *items = NULL;

			memset(&setup_menu, 0, sizeof(TelSatSetupMenuInfo_t));

			g_variant_get(param, "(ibs@vibb)", &command_id, &b_present, &title, &items, &item_cnt,
						&b_helpinfo, &b_updated);

			setup_menu.commandId = command_id;
			setup_menu.bIsMainMenuPresent = (b_present ? 1 : 0);
			memcpy(setup_menu.satMainTitle, title, TAPI_SAT_DEF_TITLE_LEN_MAX+1);
			setup_menu.satMainMenuNum = item_cnt;
			if(items && item_cnt > 0){
				int index = 0;
				GVariant *unbox;
				GVariantIter *iter;

				gchar *item_str;
				gint item_id;
				unbox = g_variant_get_variant(items);
				dbg("items(%p) items type_format(%s)", items, g_variant_get_type_string(unbox));

				g_variant_get(unbox, "a(si)", &iter);
				while(g_variant_iter_loop(iter,"(si)",&item_str, &item_id)){
					setup_menu.satMainMenuItem[index].itemId = item_id;
					memcpy(setup_menu.satMainMenuItem[index].itemString, item_str, TAPI_SAT_DEF_ITEM_STR_LEN_MAX + 6);
					dbg("item index(%d) id(%d) str(%s)",index, setup_menu.satMainMenuItem[index].itemId, setup_menu.satMainMenuItem[index].itemString);
					//dbg("item index(%d) id(%d) str(%s)",index, item_id, item_str);
					index++;
				}
				g_variant_iter_free(iter);
			}
			setup_menu.bIsSatMainMenuHelpInfo = (b_helpinfo ? 1 : 0);
			setup_menu.bIsUpdatedSatMainMenu = (b_updated ? 1 : 0);

			dbg("command id (%d)", setup_menu.commandId);
			dbg("menu present (%d)", setup_menu.bIsMainMenuPresent);
			dbg("menu title (%s)", setup_menu.satMainTitle);
			dbg("item cnt (%d)", setup_menu.satMainMenuNum);
			dbg("menu help info (%d)", setup_menu.bIsSatMainMenuHelpInfo);
			dbg("menu updated (%d)", setup_menu.bIsUpdatedSatMainMenu);

			noti_data = &setup_menu;
		}
		else if (!g_strcmp0(sig, "DisplayText")) {
			TelSatDisplayTextInd_t display_text;

			gchar* text;
			gint command_id, text_len, duration;
			gboolean high_priority, user_rsp_required, immediately_rsp;

			memset(&display_text, 0, sizeof(TelSatDisplayTextInd_t));

			g_variant_get(param, "(isiibbb)", &command_id, &text, &text_len, &duration,
				&high_priority, &user_rsp_required, &immediately_rsp);

			display_text.commandId = command_id;
			memcpy(display_text.text.string, text, TAPI_SAT_DEF_TEXT_STRING_LEN_MAX+1);
			display_text.text.stringLen = text_len;
			display_text.duration = duration;
			display_text.bIsPriorityHigh = (high_priority ? 1 : 0);
			display_text.bIsUserRespRequired = (user_rsp_required ? 1 : 0);

			dbg("command id (%d)", display_text.commandId);
			dbg("display text (%s)", display_text.text.string);
			dbg("string len(%d)", display_text.text.stringLen);
			dbg("duration (%d)", display_text.duration);
			dbg("high priority (%d)", display_text.bIsPriorityHigh);
			dbg("user response required(%d)", display_text.bIsUserRespRequired);

			noti_data = &display_text;
		}
		else if (!g_strcmp0(sig, "SelectItem")) {
			TelSatSelectItemInd_t select_item;

			gboolean help_info ;
			gchar *selected_text;
			gint command_id, default_item_id, menu_cnt, text_len =0;
			GVariant *menu_items;

			memset(&select_item, 0, sizeof(TelSatSelectItemInd_t));

			g_variant_get(param, "(ibsiii@v)", &command_id, &help_info, &selected_text,
				&text_len, &default_item_id, &menu_cnt, &menu_items);

			select_item.commandId = command_id;
			select_item.bIsHelpInfoAvailable = (help_info ? 1 : 0);
			memcpy(select_item.text.string, selected_text, TAPI_SAT_DEF_TITLE_LEN_MAX+1);
			select_item.text.stringLen = text_len;
			select_item.defaultItemIndex = default_item_id;
			select_item.menuItemCount = menu_cnt;
			if(menu_items && menu_cnt > 0){
				int index = 0;
				GVariant *unbox;
				GVariantIter *iter;

				gchar *item_str;
				gint item_id, item_len;
				unbox = g_variant_get_variant(menu_items);
				dbg("items(%p) items type_format(%s)", menu_items, g_variant_get_type_string(unbox));

				g_variant_get(unbox, "a(iis)", &iter);
				while(g_variant_iter_loop(iter,"(iis)",&item_id, &item_len, &item_str)){
					select_item.menuItem[index].itemId = item_id;
					select_item.menuItem[index].textLen = item_len;
					memcpy(select_item.menuItem[index].text, item_str, TAPI_SAT_ITEM_TEXT_LEN_MAX + 1);
					dbg("item index(%d) id(%d) len(%d) str(%s)", index,
							select_item.menuItem[index].itemId, select_item.menuItem[index].textLen, select_item.menuItem[index].text);
					index++;
				}
				g_variant_iter_free(iter);
			}

			dbg("command id (%d)", select_item.commandId);
			dbg("help info(%d)", select_item.bIsHelpInfoAvailable);
			dbg("selected item string(%s)", select_item.text.string);
			dbg("string len(%d)", select_item.text.stringLen);
			dbg("default item index(%d)", select_item.defaultItemIndex);
			dbg("item count(%d)", select_item.menuItemCount);

			noti_data = &select_item;
		}
		else if (!g_strcmp0(sig, "GetInkey")) {
			TelSatGetInkeyInd_t get_inkey;

			gint command_id, key_type, input_character_mode;
			gint text_len, duration;
			gboolean b_numeric, b_help_info;
			gchar *text;

			memset(&get_inkey, 0, sizeof(TelSatGetInkeyInd_t));

			g_variant_get(param, "(iiibbsii)", &command_id, &key_type, &input_character_mode,
				&b_numeric,&b_help_info, &text, &text_len, &duration);

			get_inkey.commandId = command_id;
			get_inkey.keyType = key_type;
			get_inkey.inputCharMode = input_character_mode;
			get_inkey.bIsNumeric = (b_numeric ? 1 : 0);
			get_inkey.bIsHelpInfoAvailable = (b_help_info ? 1 : 0);
			memcpy(get_inkey.text.string, text, TAPI_SAT_DEF_TEXT_STRING_LEN_MAX+1);
			get_inkey.text.stringLen = text_len;
			get_inkey.duration = duration;

			dbg("command id(%d)", get_inkey.commandId);
			dbg("key type(%d)", get_inkey.keyType);
			dbg("input character mode(%d)", get_inkey.inputCharMode);
			dbg("numeric(%d)", get_inkey.bIsNumeric);
			dbg("help info available(%d)", get_inkey.bIsHelpInfoAvailable);
			dbg("text (%s)", get_inkey.text.string);
			dbg("text length", get_inkey.text.stringLen);
			dbg("duration", get_inkey.duration);

			noti_data = &get_inkey;
		}
		else if (!g_strcmp0(sig, "GetInput")) {
			TelSatGetInputInd_t get_input;

			gint command_id, input_character_mode;
			gint text_len, def_text_len, rsp_len_min, rsp_len_max;
			gboolean b_numeric, b_help_info, b_echo_input;
			gchar *text, *def_text;

			memset(&get_input, 0, sizeof(TelSatGetInputInd_t));

			g_variant_get(param, "(iibbbsiiisi)", &command_id, &input_character_mode, &b_numeric, &b_help_info, &b_echo_input,
				&text, &text_len, &rsp_len_max, &rsp_len_min, &def_text, &def_text_len);

			get_input.commandId = command_id;
			get_input.inputCharMode = input_character_mode;
			get_input.bIsNumeric = (b_numeric ? 1 : 0);
			get_input.bIsHelpInfoAvailable = (b_help_info ? 1 : 0);
			get_input.bIsEchoInput = (b_echo_input ? 1 : 0);
			memcpy(get_input.text.string, text, TAPI_SAT_DEF_TEXT_STRING_LEN_MAX+1);
			get_input.text.stringLen = text_len;
			get_input.respLen.max = rsp_len_max;
			get_input.respLen.min = rsp_len_min;
			memcpy(get_input.defaultText.string, def_text, TAPI_SAT_DEF_TEXT_STRING_LEN_MAX+1);
			get_input.defaultText.stringLen = def_text_len;

			dbg("command id(%d)", get_input.commandId);
			dbg("input character mode(%d)", get_input.inputCharMode);
			dbg("numeric(%d)", get_input.bIsNumeric);
			dbg("help info avaiable(%d)", get_input.bIsHelpInfoAvailable);
			dbg("echo input(%d)", get_input.bIsEchoInput);
			dbg("text(%s)", get_input.text.string);
			dbg("text length(%d)", get_input.text.stringLen);
			dbg("response length max(%d)", get_input.respLen.max);
			dbg("response length min(%d)", get_input.respLen.min);
			dbg("default text(%s)", get_input.defaultText.string);
			dbg("default text length(%d)", get_input.defaultText.stringLen);

			noti_data = &get_input;
		}
		else if (!g_strcmp0(sig, "SendSMS")) {
			TelSatSendSmsIndSmsData_t send_sms;

			gint command_id, ton, npi, tpdu_type;
			gboolean b_packing_required;
			gint text_len, number_len, tpdu_data_len, tmp_data_len;
			gchar* text, *dialling_number;
			GVariant *tpdu_data;

			memset(&send_sms, 0, sizeof(TelSatSendSmsIndSmsData_t));

			g_variant_get(param, "(isibiisii@vi)", &command_id, &text, &text_len, &b_packing_required, &ton, &npi,
						&dialling_number, &number_len, &tpdu_type, &tpdu_data, &tpdu_data_len);

			send_sms.commandId = command_id;
			send_sms.bIsPackingRequired = (b_packing_required ? 1 : 0);

			send_sms.address.ton = ton;
			send_sms.address.npi = npi;
			send_sms.address.diallingNumberLen = number_len;
			memcpy(send_sms.address.diallingNumber, dialling_number, TAPI_SAT_DIALLING_NUMBER_LEN_MAX);

			send_sms.smsTpdu.tpduType = tpdu_type;
			send_sms.smsTpdu.dataLen = tpdu_data_len;

			if(tpdu_data){
				int index = 0;
				guchar data;
				GVariantIter *iter = NULL;
				GVariant *inner_gv = NULL;

				inner_gv = g_variant_get_variant(tpdu_data);
				dbg("tpdu data exist type_format(%s)", g_variant_get_type_string(inner_gv));

				g_variant_get(inner_gv, "ay", &iter);
				while( g_variant_iter_loop (iter, "y", &data)){
					dbg("index(%d) data(%c)", index, data);
					send_sms.smsTpdu.data[index] = data;
					index++;
				}
				g_variant_iter_free(iter);
				g_variant_unref(inner_gv);
				tmp_data_len = index - 1;
			}

			dbg("command id(%d)", send_sms.commandId);
			dbg("packing required(%d)", send_sms.bIsPackingRequired);
			dbg("address ton(%d)", send_sms.address.ton);
			dbg("address npi(%d)", send_sms.address.npi);
			dbg("address dialing number (%s)", send_sms.address.diallingNumber);
			dbg("address number length (%d)", send_sms.address.diallingNumberLen);
			dbg("tpdu type (%d)", send_sms.smsTpdu.tpduType);
			dbg("tpdu length (%d)", send_sms.smsTpdu.dataLen);

			noti_data = &send_sms;
		}
		else if (!g_strcmp0(sig, "SetupEventList")) {
			int g_index = 0;
			gint event_cnt;
			GVariant *evt_list;
			TelSatEventListData_t event_list;

			memset(&event_list, 0, sizeof(TelSatEventListData_t));

			g_variant_get(param, "(i@v)", &event_cnt, &evt_list);

			if(evt_list){
				guchar data;
				GVariantIter *iter = NULL;
				GVariant *inner_gv = NULL;

				inner_gv = g_variant_get_variant(evt_list);
				dbg("event list exist type_format(%s)", g_variant_get_type_string(inner_gv));

				g_variant_get(inner_gv, "ai", &iter);
				while( g_variant_iter_loop (iter, "i", &data)){
					dbg("g_index(%d) event(%d)", g_index, data);
					g_event_list[g_index] = data;

					if(data == TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE){
						event_list.bIsIdleScreenAvailable = 1;
					}
					else if(data == TAPI_EVENT_SAT_DW_TYPE_LANGUAGE_SELECTION){
						event_list.bIsLanguageSelection = 1;
					}
					else if(data == TAPI_EVENT_SAT_DW_TYPE_BROWSER_TERMINATION){
						event_list.bIsBrowserTermination = 1;
					}
					else if(data == TAPI_EVENT_SAT_DW_TYPE_DATA_AVAILABLE){
						event_list.bIsDataAvailable = 1;
					}
					else if(data == TAPI_EVENT_SAT_DW_TYPE_CHANNEL_STATUS){
						event_list.bIsChannelStatus = 1;
					}

					g_index++;
				}
				//while end
				g_variant_iter_free(iter);
				g_variant_unref(inner_gv);
			}

			dbg("event list cnt(%d)", event_cnt);

			noti_data = &event_list;
		}
		else if (!g_strcmp0(sig, "EndProactiveSession")) {
			int command_type = 0;

			g_variant_get(param, "(i)", &command_type);
			dbg("end session evt : command type(%d)", command_type);
			noti_data = &command_type;
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_SIM_INTERFACE)) {
		if (!g_strcmp0(sig, "Status")) {
			int noti = 0;
			g_variant_get(param, "(i)", &noti);
			noti_data = &noti;
		} else {
			dbg("not handled SIM noti[%s]",sig );
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_PB_INTERFACE)) {
		if (!g_strcmp0(sig, "Status")) {
			int noti = 0;
			g_variant_get(param, "(i)", &noti);
			noti_data = &noti;
		} else {
			dbg("not handled Phonebook noti[%s]",sig );
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_SAP_INTERFACE)) {
		if (!g_strcmp0(sig, "Status")) {
			int noti = 0;
			g_variant_get(param, "(i)", &noti);
			noti_data = &noti;
		} else if (!g_strcmp0(sig, "Disconnect")) {
			int disconnect = 0;
			g_variant_get(param, "(i)", &disconnect);
			noti_data = &disconnect;
		} else {
			dbg("not handled SAP noti[%s]",sig );
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_MODEM_INTERFACE)) {
		if (!g_strcmp0(sig, "Power")) {
			int noti = 0;
			g_variant_get(param, "(i)", &noti);
			noti_data = &noti;
		} else if (!g_strcmp0(sig, "DunPinCtrl")) {
			tapi_ps_btdun_pincontrol pin_ctrl;
			int signal = 0, status = 0;

			memset(&pin_ctrl, 0, sizeof(tapi_ps_btdun_pincontrol));
			g_variant_get(param, "(ib)", &signal, &status);
			pin_ctrl.signal = signal;
			pin_ctrl.status = status;

			noti_data = &pin_ctrl;
		} else if (!g_strcmp0(sig, "DunExternalCall")) {
			gboolean external_call;

			g_variant_get(param, "(b)", &external_call);

			noti_data = &external_call;
		}
		else {
			dbg("not handled Modem noti[%s]", sig);
		}
	}

	if (!g_strcmp0(interface, DBUS_TELEPHONY_SS_INTERFACE)) {
		if (!g_strcmp0(sig, "NotifyUSSD")) {
			TelSsUssdMsgInfo_t noti;
			char *str = NULL;
			memset(&noti, 0, sizeof(TelSsUssdMsgInfo_t));

			g_variant_get(param, "(iis)", &noti.Type, &noti.Length, &str);

			if (str)
				g_strlcpy(noti.szString, str, TAPI_SS_USSD_DATA_SIZE_MAX);

			noti_data = &noti;
		}
		else {
			dbg("not handled SS noti[%s]", sig);
		}
	}

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(handle, noti_id, noti_data, evt_cb_data->user_data);
	}

	g_free(noti_id);
	return;
}

EXPORT_API char** tel_get_cp_name_list(void)
{
	GError *error = NULL;
	GDBusProxy *proxy = NULL;

	GVariantIter *iter = NULL;
	GVariant *rst = NULL;

	gchar *modem_path = NULL;
	GSList *list = NULL;
	GSList *l = NULL;

	int i = 0, element_cnt = 0;
	gchar **cp_list = NULL;

	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
				"org.tizen.telephony", "/org/tizen/telephony", "org.tizen.telephony.Manager",
				NULL, &error);
	if (!proxy) {
		dbg("g_dbus_proxy_new_for_bus_sycn() failed. (%s)", error->message);
		g_error_free (error);
		return NULL;
	}

	rst = g_dbus_proxy_call_sync (proxy, "GetModems", NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);
	if (!rst) {
		dbg( "GetModems() failed. (%s)", error->message);
		g_error_free (error);
		goto OUT;
	}

	g_variant_get (rst, "(as)", &iter);
	while (g_variant_iter_next(iter, "s", &modem_path)){
		list = g_slist_append(list, modem_path);
	}
	g_variant_iter_free (iter);

	if (!list) {
		dbg( "No CP name");
		goto OUT;
	}

	element_cnt = g_slist_length(list);
	cp_list = g_new0(char*, element_cnt + 1);
	if (!cp_list) {
		goto OUT;
	}

	for (l = list; l; l = l->next, i++) {
		cp_list[i] = g_strdup(l->data);
		dbg("cp name[%d] = %s", i, cp_list[i]);
	}
	cp_list[element_cnt] = NULL;

	g_slist_free(list);

OUT:
	if (proxy)
		g_object_unref(proxy);

	return cp_list;
}

EXPORT_API int tel_get_property_int(TapiHandle *handle, const char *property, int *result)
{
	char **dbus_info;
	GDBusProxy *proxy = NULL;
	GVariant *value = NULL;
	GError *error = NULL;
	int ret = TAPI_API_SUCCESS;

	if (!handle || !property)
		return TAPI_API_INVALID_INPUT;

	dbus_info = g_strsplit(property, ":", 2);
	if (!dbus_info) {
		dbg("invalid property");
		return TAPI_API_INVALID_INPUT;
	}

	if (!dbus_info[0] || !dbus_info[1]) {
		dbg("invalid property");
		ret = TAPI_API_INVALID_INPUT;
		goto OUT;
	}

	proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM, G_DBUS_PROXY_FLAGS_NONE, NULL,
				DBUS_TELEPHONY_SERVICE, handle->path, dbus_info[0],
				NULL, &error);
	if (!proxy) {
		dbg("g_dbus_proxy_new_for_bus_sycn() failed. (%s)", error->message);
		g_error_free (error);
		ret = TAPI_API_OPERATION_FAILED;
		goto OUT;
	}

	value = g_dbus_proxy_get_cached_property(proxy, dbus_info[1]);
	if (!value) {
		dbg("g_dbus_proxy_get_cached_property('power') failed.");
		ret = TAPI_API_OPERATION_FAILED;
		goto OUT;
	}

	g_variant_get(value, "i", result);
	g_variant_unref(value);

OUT:
	if (proxy)
		g_object_unref(proxy);

	g_strfreev(dbus_info);

	return ret;
}

EXPORT_API TapiHandle* tel_init(const char *cp_name)
{
	GError *error = NULL;
	struct tapi_handle *handle;

	g_type_init();

	handle = g_new0(struct tapi_handle, 1);
	if (!handle)
		return NULL;

	if (cp_name) {
		handle->cp_name = g_strdup(cp_name);
	}
	else {
		char **list = NULL;

		list = tel_get_cp_name_list();
		if (!list)
			return NULL;

		if (!list[0])
			return NULL;

		dbg("use default plugin(%s)", list[0]);
		handle->cp_name = g_strdup(list[0]);
	}

	handle->dbus_connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
	if (!handle->dbus_connection) {
		dbg("Error creating connection: %s\n", error->message);
		g_free(handle->cp_name);
		g_free(handle);
		g_error_free (error);
		return NULL;
	}

	handle->evt_list = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	handle->path = g_strdup_printf("%s/%s",
			DBUS_TELEPHONY_DEFAULT_PATH, handle->cp_name);

	return handle;
}

EXPORT_API int tel_deinit(TapiHandle* handle)
{
	if (!handle)
		return -1;

	if (handle->cp_name)
		g_free(handle->cp_name);

	if (handle->path)
		g_free(handle->path);

	dbg( "remove all signal");
	/*
	 * TODO
	 */

	memset(handle, 0, sizeof(struct tapi_handle));
	g_free(handle);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_register_noti_event(TapiHandle *handle, const char *noti_id, tapi_notification_cb callback, void *user_data)
{
	gchar **dbus_str = NULL;
	gpointer tmp = NULL;
	struct tapi_evt_cb *evt_cb_data = NULL;

	if (!handle || !handle->dbus_connection || !callback || !noti_id) {
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	tmp = g_hash_table_lookup(handle->evt_list, noti_id);
	if (tmp != NULL) {
		dbg("noti_id(%s) is already registered", noti_id);
		return TAPI_API_INVALID_INPUT;
	}

	dbus_str = g_strsplit(noti_id, ":", 2);
	if (!dbus_str) {
		dbg("invalid noti_id");
		return TAPI_API_INVALID_INPUT;
	}

	if (!dbus_str[0] || !dbus_str[1]) {
		g_strfreev(dbus_str);
		dbg("invalid noti_id");
		return TAPI_API_INVALID_INPUT;
	}

	evt_cb_data = g_new0(struct tapi_evt_cb, 1);
	evt_cb_data->cb_fn = callback;
	evt_cb_data->user_data = user_data;

	dbg("path(%s) interface (%s) signal (%s)", handle->path, dbus_str[0], dbus_str[1]);

	evt_cb_data->evt_id = g_dbus_connection_signal_subscribe(handle->dbus_connection,
			DBUS_TELEPHONY_SERVICE, /* Sender */
			dbus_str[0], /* Interface */
			dbus_str[1], /* Member */
			handle->path, /* Object path */
			NULL, /* arg0 */
			G_DBUS_SIGNAL_FLAGS_NONE,
			on_signal_callback, handle,
			NULL);

	g_hash_table_insert(handle->evt_list, g_strdup(noti_id), evt_cb_data);

	g_strfreev(dbus_str);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_deregister_noti_event(TapiHandle *handle, const char *noti_id)
{
	struct tapi_evt_cb *evt_cb_data = NULL;
	gboolean rv = FALSE;

	if (!handle || !handle->dbus_connection){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	evt_cb_data = g_hash_table_lookup(handle->evt_list, noti_id);

	if(!evt_cb_data){
		dbg("event does not registered");
		return TAPI_API_INVALID_INPUT;
	}

	g_dbus_connection_signal_unsubscribe(handle->dbus_connection, evt_cb_data->evt_id);

	rv = g_hash_table_remove(handle->evt_list, noti_id);
	if(!rv){
		dbg("fail to deregister noti event(%s)", noti_id);
		return TAPI_API_OPERATION_FAILED;
	}

	return TAPI_API_SUCCESS;
}
/*	EOF	*/
