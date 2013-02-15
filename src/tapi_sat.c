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
#include "TelSat.h"

#include "common.h"
#include "tapi_log.h"
#include "ITapiSat.h"

extern TelSatEventDownloadType_t g_event_list[TAPI_SAT_EVENT_LIST_MAX_COUNT];

static GVariant* _tapi_sat_gen_app_exec_result_data(TelSatAppsRetInfo_t *result_info)
{
	GVariant *result = NULL;

	switch(result_info->commandType)
	{
		case TAPI_SAT_CMD_TYPE_SETUP_MENU:{
			gint exec_result;
			dbg("setup menu : result format (i)");

			exec_result = result_info->appsRet.setupMenu.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_REFRESH:{
			gint app_type, exec_result;
			dbg("refresh : result format (ii)");

			app_type = result_info->appsRet.refresh.appType;
			exec_result = result_info->appsRet.refresh.resp;
			result = g_variant_new("(ii)", app_type, exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_SETUP_CALL:{
			gint exec_result, me_problem, call_ctrl_problem, call_cause;
			dbg("setup call : result format (iiii)");

			exec_result = result_info->appsRet.setupCall.resp;
			me_problem = result_info->appsRet.setupCall.meProblem;
			call_ctrl_problem = result_info->appsRet.setupCall.permanentCallCtrlProblem;
			call_cause = result_info->appsRet.setupCall.tapiCause;
			result = g_variant_new("(iiii)", exec_result, me_problem, call_ctrl_problem, call_cause);
		} break;
		case TAPI_SAT_CMD_TYPE_SEND_SS:{
			gint exec_result, me_problem, ss_cause, call_ctrl_problem, ss_str_len;
			gchar *ss_string = NULL;
			dbg("send ss : result format (iiisii)");

			exec_result = result_info->appsRet.sendSs.resp;
			me_problem = result_info->appsRet.sendSs.meProblem;
			ss_cause = result_info->appsRet.sendSs.ssCause;
			call_ctrl_problem = result_info->appsRet.sendSs.additionalCallCtrlProblemInfo;
			ss_str_len = result_info->appsRet.sendSs.ssString.stringLen;
			ss_string = g_strdup((gchar*)result_info->appsRet.sendSs.ssString.string);

			result = g_variant_new("(iiisii)", exec_result, me_problem, ss_cause, ss_string, ss_str_len, call_ctrl_problem);
		} break;
		case TAPI_SAT_CMD_TYPE_SEND_USSD:{
			int index;
			gint exec_result, me_problem, ss_cause, ussd_str_len;
			GVariantBuilder *builder = NULL;
			GVariant *ussd_str = NULL;

			dbg("send ussd : result format (iiivi)");

			exec_result = result_info->appsRet.sendUssd.resp;
			me_problem = result_info->appsRet.sendUssd.meProblem;
			ss_cause = result_info->appsRet.sendUssd.ssCause;
			ussd_str_len = result_info->appsRet.sendUssd.ussdString.stringLen;

			builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
			for (index = 0; index < ussd_str_len; index++) {
				dbg("index(%d) data(%d)", index, result_info->appsRet.sendUssd.ussdString.string[index]);
				g_variant_builder_add(builder, "y", result_info->appsRet.sendUssd.ussdString.string[index]);
			}
			ussd_str = g_variant_builder_end(builder);

			result = g_variant_new("(iiivi)", exec_result, me_problem, ss_cause, ussd_str, ussd_str_len);
		} break;
		case TAPI_SAT_CMD_TYPE_SEND_SMS:{
			gint exec_result;
			dbg("send sms: result format (i)");

			exec_result = result_info->appsRet.sendSms.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_SEND_DTMF:{
			gint exec_result;
			dbg("send DTMF: result format (i)");

			exec_result = result_info->appsRet.sendDtmf.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_LAUNCH_BROWSER:{
			gint exec_result, browser_problem;
			dbg("launch browser: result format (ii)");

			exec_result = result_info->appsRet.launchBrowser.resp;
			browser_problem = result_info->appsRet.launchBrowser.browserProblem;
			result = g_variant_new("(ii)", exec_result, browser_problem);
		} break;
		case TAPI_SAT_CMD_TYPE_SETUP_IDLE_MODE_TEXT:{
			gint exec_result;
			dbg("setup idle mode text: result format (i)");

			exec_result = result_info->appsRet.setupIdleModeText.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_LANGUAGE_NOTIFICATION:{
			gint exec_result;
			dbg("language notification: result format (i)");

			exec_result = result_info->appsRet.languageNoti.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_PROVIDE_LOCAL_INFO:{
			gint exec_result;
			dbg("provide local info: result format (i)");

			exec_result = result_info->appsRet.provideLocalInfo.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_PLAY_TONE:{
			gint exec_result;
			dbg("play tone: result format (i)");

			exec_result = result_info->appsRet.playTone.resp;
			result = g_variant_new("(i)", exec_result);
		} break;
		case TAPI_SAT_CMD_TYPE_DISPLAY_TEXT:{
			gint exec_result, me_problem;
			dbg("display text: result format (ii)");

			exec_result = result_info->appsRet.displayText.resp;
			me_problem = result_info->appsRet.displayText.meProblem;
			result = g_variant_new("(ii)", exec_result, me_problem);
		} break;
		default:
			dbg("unhandled command type(0x%x", result_info->commandType);
			break;
	}

	return result;
}

static GVariant* _tapi_sat_gen_event_download_data(const TelSatEventDownloadReqInfo_t *pEventData)
{
	GVariant *result = NULL;

	switch(pEventData->eventDownloadType)
	{
		case TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE:{
			gboolean idle_screen;
			dbg("idle screen available (%d)", pEventData->u.bIdleScreenAvailable);

			idle_screen = pEventData->u.bIdleScreenAvailable;
			result = g_variant_new("(b)", idle_screen);
		} break;
		case TAPI_EVENT_SAT_DW_TYPE_LANGUAGE_SELECTION:{
			gint selected_language;
			dbg("selected language (%d)", pEventData->u.languageSelectionEventReqInfo.language);

			selected_language = pEventData->u.languageSelectionEventReqInfo.language;
			result = g_variant_new("(i)", selected_language);
		} break;
		case TAPI_EVENT_SAT_DW_TYPE_BROWSER_TERMINATION:{
			gint browser_termination_cause;
			dbg("Cause of browser termination (%d)", pEventData->u.browserTerminationEventReqInfo.browserTerminationCause);

			browser_termination_cause = pEventData->u.browserTerminationEventReqInfo.browserTerminationCause;
			result = g_variant_new("(i)", browser_termination_cause);
		} break;
		case TAPI_EVENT_SAT_DW_TYPE_DATA_AVAILABLE:
		case TAPI_EVENT_SAT_DW_TYPE_CHANNEL_STATUS:
		default :
			dbg("not support download event (%d)", pEventData->eventDownloadType);
			break;
	}

	return result;
}

static void on_response_menu_selection_envelop(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;

	GVariant *rst;
	struct tapi_resp_data *evt_cb_data = user_data;

	int result = -1;
	int envelop_rsp = 0;

	conn = G_DBUS_CONNECTION (source_object);
	rst = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if(!rst){
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &envelop_rsp, evt_cb_data->user_data);

		g_error_free (error);
		g_free(evt_cb_data);
		return;
	}

	g_variant_get (rst, "(ii)", &result, &envelop_rsp);
	dbg("menu selection envelop result(%d) envelop response(%d)", result, envelop_rsp);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &envelop_rsp, evt_cb_data->user_data);
	}

	g_variant_unref(rst);
	g_free(evt_cb_data);
}

static void on_response_download_event_envelop(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
	GError *error = NULL;
	GDBusConnection *conn = NULL;

	GVariant *rst;
	struct tapi_resp_data *evt_cb_data = user_data;

	int result = -1;
	int envelop_rsp = 0;

	conn = G_DBUS_CONNECTION (source_object);
	rst = g_dbus_connection_call_finish(conn, res, &error);
	CHECK_DEINIT(error);

	if(!rst){
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &envelop_rsp, evt_cb_data->user_data);

		g_error_free (error);
		g_free(evt_cb_data);
		return;
	}

	g_variant_get (rst, "(ii)", &result, &envelop_rsp);
	dbg("download event envelop result(%d) envelop response(%d)", result, envelop_rsp);

	if (evt_cb_data->cb_fn) {
		evt_cb_data->cb_fn(evt_cb_data->handle, result, &envelop_rsp, evt_cb_data->user_data);
	}

	g_variant_unref(rst);
	g_free(evt_cb_data);
}

/**
 *
 * To send envelope command (MENU SELECTION) to USIM.
 *
 * @return		values in enum TapiResult_t.
 * @param[in]		TelSatMenuSelectionReqInfo_t	menu selection evelope data.
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSatMenuSelectionReqInfo_t
 */

EXPORT_API int tel_select_sat_menu(TapiHandle *handle, const TelSatMenuSelectionReqInfo_t *pMenuSelect, tapi_response_cb callback, void *user_data)
{
	gint item_id;
	gboolean help_req;
	GVariant *inparam = NULL;
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection || !pMenuSelect){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	item_id = pMenuSelect->itemIdentifier;
	help_req = ( (pMenuSelect->bIsHelpRequested != 0) ? TRUE : FALSE);
	dbg("item id(%d) help request(%d)", item_id, help_req);
	inparam = g_variant_new("(ib)", item_id, help_req);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAT_INTERFACE, "SelectMenu", inparam, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			handle->ca, on_response_menu_selection_envelop, evt_cb_data);

	g_variant_unref(inparam);

	return TAPI_API_SUCCESS;
}

/**
 *
 * API to send event download to SIM.
 *
 * @return		values in enum TapiResult_t.
 * @param[in]		TelSatEventDownloadReqInfo_t	event download data.
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSatEventDownloadReqInfo_t
 */
EXPORT_API int tel_download_sat_event(TapiHandle *handle, const TelSatEventDownloadReqInfo_t *pEventData, tapi_response_cb callback, void *user_data)
{
	gboolean evt_check = FALSE;
	int g_index = 0;
	gint event_download_type, src_dev, dest_dev;
	GVariant *download_data = NULL;

	GVariant *inparam = NULL;
	struct tapi_resp_data *evt_cb_data = NULL;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection || !pEventData){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	for(g_index = 0; g_event_list[g_index] > 0; g_index++){
		if(g_event_list[g_index] == pEventData->eventDownloadType ){
			dbg("event (%d) shoud be passed to sim");
			evt_check = TRUE;
		}
	}

	if(!evt_check){
		dbg("sim does not request event(%d)", pEventData->eventDownloadType);
		return TAPI_API_SAT_EVENT_NOT_REQUIRED_BY_USIM;
	}

	MAKE_RESP_CB_DATA(evt_cb_data, handle, callback, user_data);

	event_download_type = pEventData->eventDownloadType;
	src_dev = TAPI_SAT_DEVICE_ID_ME;
	dest_dev = TAPI_SAT_DEVICE_ID_SIM;
	download_data = _tapi_sat_gen_event_download_data(pEventData);
	dbg("event type(%d)", event_download_type);
	inparam = g_variant_new("(iiiv)", event_download_type, src_dev, dest_dev, download_data);

	g_dbus_connection_call(handle->dbus_connection, DBUS_TELEPHONY_SERVICE, handle->path,
			DBUS_TELEPHONY_SAT_INTERFACE, "DownloadEvent", inparam, NULL, G_DBUS_CALL_FLAGS_NONE, -1,
			handle->ca, on_response_download_event_envelop, evt_cb_data);

	g_variant_unref(inparam);

	return TAPI_API_SUCCESS;
}

/**
 *
 * API to get main menu information.
 *
 * @return		values in enum TapiResult_t.
 * @param[out]		TelSatSetupMenuInfo_t	main menu data.
 * @Interface		Synchronous.
 * @remark
 * @Refer		TelSatSetupMenuInfo_t
 */
EXPORT_API int tel_get_sat_main_menu_info(TapiHandle *handle, TelSatSetupMenuInfo_t *pMainMenu)
{
	GError *error;
	GVariant *rst = NULL;

	gchar *title;
	gint result, command_id, item_cnt;
	gboolean b_present, b_helpinfo, b_updated;
	GVariant *items = NULL;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection || !pMainMenu){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	memset(pMainMenu, 0, sizeof(TelSatSetupMenuInfo_t));

	error = NULL;
	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE , handle->path,
			DBUS_TELEPHONY_SAT_INTERFACE, "GetMainMenuInfo", NULL, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

	if(!rst){
		dbg( "error to get main menu(%s)", error->message);
		g_error_free (error);
		return TAPI_API_OPERATION_FAILED;
	}

	dbg("menu_info type_format(%s)", g_variant_get_type_string(rst));

	g_variant_get(rst, "(iibs@vibb)", &result, &command_id, &b_present, &title, &items, &item_cnt,
			&b_helpinfo, &b_updated);

	pMainMenu->commandId = command_id;
	pMainMenu->bIsMainMenuPresent = (b_present ? 1 : 0);
	memcpy(pMainMenu->satMainTitle, title, TAPI_SAT_DEF_TITLE_LEN_MAX+1);
	pMainMenu->satMainMenuNum = item_cnt;
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
			pMainMenu->satMainMenuItem[index].itemId = item_id;
			memcpy(pMainMenu->satMainMenuItem[index].itemString, item_str, TAPI_SAT_DEF_ITEM_STR_LEN_MAX + 6);
			dbg("item index(%d) id(%d) str(%s)",index, pMainMenu->satMainMenuItem[index].itemId, pMainMenu->satMainMenuItem[index].itemString);
			//dbg("item index(%d) id(%d) str(%s)",index, item_id, item_str);
			index++;
		}
		g_variant_iter_free(iter);
	}
	pMainMenu->bIsSatMainMenuHelpInfo = (b_helpinfo ? 1 : 0);
	pMainMenu->bIsUpdatedSatMainMenu = (b_updated ? 1 : 0);

	dbg("result (%d)", result);
	dbg("command id (%d)", pMainMenu->commandId);
	dbg("menu present (%d)", pMainMenu->bIsMainMenuPresent);
	dbg("menu title (%s)", pMainMenu->satMainTitle);
	dbg("item cnt (%d)", pMainMenu->satMainMenuNum);
	dbg("menu help info (%d)", pMainMenu->bIsSatMainMenuHelpInfo);
	dbg("menu updated (%d)", pMainMenu->bIsUpdatedSatMainMenu);

	return TAPI_API_SUCCESS;
}

/**
 *
 * API to send UI display status.
 *
 * @return		values in enum TapiResult_t.
 * @param[in]		TelSatUiDisplayStatusType_t	display status data.
 command_id	command identifier
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSatUiDisplayStatusType_t
 */
EXPORT_API int tel_send_sat_ui_display_status(TapiHandle *handle, int commandId, TelSatUiDisplayStatusType_t status)
{
	GError *error;
	GVariant *rst = NULL;

	gint result, command_id;
	gboolean display_status;
	GVariant *inparam = NULL;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	command_id = commandId;
	display_status = ( (status == TAPI_SAT_DISPLAY_SUCCESS) ? TRUE : FALSE);
	dbg("command id(%d) display status(%d)", command_id, display_status);
	inparam = g_variant_new("(ib)", command_id, display_status);

	error = NULL;
	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE , handle->path,
			DBUS_TELEPHONY_SAT_INTERFACE, "SendUiDisplayStatus", inparam, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

	if(!rst){
		dbg( "error to send ui display status(%s)", error->message);
		g_error_free (error);
		return TAPI_API_OPERATION_FAILED;
	}

	dbg("send ui display status format(%s)", g_variant_get_type_string(rst));

	g_variant_get(rst, "(i)", &result);
	if(result){
		result = TAPI_API_SUCCESS;
	}
	else{
		result = TAPI_API_OPERATION_FAILED;
	}
	dbg("result (%d)", result);

	return result;
}

/**
 *
 * API to send UI user confirmation data.
 *
 * @return		values in enum TapiResult_t.
 * @param[in]		command_id	command identifier
 command_type		command type
 key_type			user confirmation value
 addtional_data		addtional data
 data_len			data length
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_send_sat_ui_user_confirm(TapiHandle *handle, TelSatUiUserConfirmInfo_t *pUserConfirmData)
{
	GError *error;
	GVariant *rst = NULL;
	GVariantBuilder *builder = NULL;
	GVariant *inner_gv = NULL;

	int index = 0;
	gchar *data;
	gint result, command_id, command_type, key_type, data_len;
	GVariant *inparam = NULL;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection || !pUserConfirmData){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	command_id = pUserConfirmData->commandId;
	command_type = pUserConfirmData->commandType;
	key_type = pUserConfirmData->keyType;
	data = (gchar *)pUserConfirmData->pAdditionalData;
	data_len = pUserConfirmData->dataLen;

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
	for (index = 0; index < data_len; index++) {
		dbg("index(%d) data(%d)", index, data[index]);
		g_variant_builder_add(builder, "y", data[index]);
	}
	inner_gv = g_variant_builder_end(builder);

	dbg("command id(%d) command type(%d) key type(%d) data len(%d)",
			command_id, command_type, key_type, data_len);
	inparam = g_variant_new("(iiiv)", command_id, command_type, key_type, inner_gv);

	error = NULL;
	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE , handle->path,
			DBUS_TELEPHONY_SAT_INTERFACE, "SendUserConfirm", inparam, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

	if(!rst){
		dbg( "error to send ui user confirm(%s)", error->message);
		g_error_free (error);
		return TAPI_API_OPERATION_FAILED;
	}

	dbg("send ui user confirm format(%s)", g_variant_get_type_string(rst));

	g_variant_get(rst, "(i)", &result);
	if(result){
		result = TAPI_API_SUCCESS;
	}
	else{
		result = TAPI_API_OPERATION_FAILED;
	}
	dbg("result (%d)", result);

	return result;
}

/**
 *
 * API to send application excution result.
 *
 * @return		values in enum TapiResult_t.
 * @param[in]		TelSatAppsRetInfo_t	application result information data.
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSatAppsRetInfo_t
 */
EXPORT_API int tel_send_sat_app_exec_result(TapiHandle *handle, TelSatAppsRetInfo_t *pAppRetInfo)
{
	GError *error;
	GVariant *rst = NULL;

	gint result, command_id, command_type;
	GVariant *app_exec_result = NULL;
	GVariant *inparam = NULL;

	dbg("Func Entrance ");

	if (!handle || !handle->dbus_connection || !pAppRetInfo){
		dbg("invalid parameter");
		return TAPI_API_INVALID_INPUT;
	}

	command_id = pAppRetInfo->commandId;
	command_type = pAppRetInfo->commandType;
	app_exec_result = _tapi_sat_gen_app_exec_result_data(pAppRetInfo);

	dbg("command id(%d) command type(%d) exec_result(%s)", command_id, command_type, g_variant_get_type_string(app_exec_result));
	inparam = g_variant_new("(iiv)", command_id, command_type, app_exec_result);

	error = NULL;
	dbg("test (%s)", g_variant_get_type_string(inparam));
	rst = g_dbus_connection_call_sync(handle->dbus_connection, DBUS_TELEPHONY_SERVICE , handle->path,
			DBUS_TELEPHONY_SAT_INTERFACE, "SendAppExecResult", inparam, NULL, G_DBUS_CALL_FLAGS_NONE, -1, NULL, &error);

	if(!rst){
		dbg( "error to send app execution result(%s)", error->message);
		g_error_free (error);
		return TAPI_API_OPERATION_FAILED;
	}

	dbg("send app execution result format(%s)", g_variant_get_type_string(rst));

	g_variant_get(rst, "(i)", &result);
	if(result){
		result = TAPI_API_SUCCESS;
	}
	else{
		result = TAPI_API_OPERATION_FAILED;
	}
	dbg("result (%d)", result);

	return result;
}
