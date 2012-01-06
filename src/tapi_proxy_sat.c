/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
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
#include <string.h>

#include <vconf.h>

#include "TelDefines.h"
#include "tel_cs_conn.h"
#include "tapi_proxy.h"
#include "TelUtility.h"
#include "TapiUtility.h"
#include "TelSat.h"

extern tapi_dbus_connection_name conn_name;

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
EXPORT_API int tel_select_sat_menu(TelSatMenuSelectionReqInfo_t *pMenuSelect, int *pRequestId)
{
	int ret_val;
	TS_BOOL ret = FALSE;
	TAPI_GLIB_INIT_PARAMS();

	if (pMenuSelect == NULL)
		return TAPI_API_INVALID_PTR;

	if (pRequestId == NULL)
		return TAPI_API_INVALID_PTR;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pMenuSelect, sizeof(TelSatMenuSelectionReqInfo_t));

	TAPI_LIB_DEBUG(LEVEL_DEBUG, " Send env cmd... menu selection");

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIMATK, TAPI_CS_SIMATK_SEND_MENU_SELECTION_ENVELOPE, in_param1, in_param2,
			in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		ret_val = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		*pRequestId = INVALID_REQUEST_ID;
	}

	TAPI_LIB_DEBUG(LEVEL_DEBUG, " ret: %d \n", ret);

	TAPI_LIB_DEBUG(LEVEL_DEBUG, " ret_val: %d \n", ret_val);

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return ret_val;
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
EXPORT_API int tel_download_sat_event(TelSatEventDownloadReqInfo_t *pEventData, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Enter");

	int ret_val;
	TS_BOOL ret = FALSE;

	TAPI_GLIB_INIT_PARAMS();

	if (pEventData == NULL){
		TAPI_LIB_DEBUG(LEVEL_ERR, "Error: 1st param is NULL");
		return TAPI_API_INVALID_PTR;
	}

	if (pRequestId == NULL){
		TAPI_LIB_DEBUG(LEVEL_ERR, "Error: 2nd param is NULL");
		return TAPI_API_INVALID_PTR;
	}

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Error: No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pEventData, sizeof(TelSatEventDownloadReqInfo_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	TAPI_LIB_DEBUG(LEVEL_DEBUG, " event download: event=0x%x", pEventData->eventDownloadType);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIMATK, TAPI_CS_SIMATK_SEND_EVENT_DOWNLOAD, in_param1, in_param2,
			in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);
	//prevent 2223
	if (TRUE == ret) {
		ret_val = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		*pRequestId = INVALID_REQUEST_ID;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return ret_val;
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
EXPORT_API int tel_get_sat_main_menu_info(TelSatSetupMenuInfo_t *pMainMenu)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, " Get Main Menu Information");
	int ret_val;
	TS_BOOL ret = FALSE;
	TAPI_GLIB_INIT_PARAMS();

	if (pMainMenu == NULL)
		return TAPI_API_INVALID_PTR;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pMainMenu, sizeof(TelSatSetupMenuInfo_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIMATK, TAPI_CS_SIMATK_GET_MAIN_MENU_INFO, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		ret_val = g_array_index(out_param1, int, 0);
		if (ret_val == TAPI_API_SUCCESS)
			*pMainMenu = g_array_index(out_param2,TelSatSetupMenuInfo_t,0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return ret_val;
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
EXPORT_API int tel_send_sat_app_exec_result(TelSatAppsRetInfo_t *pAppRetInfo)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, " tel_send_sat_app_exec_result");
	int ret_val;
	TS_BOOL ret = FALSE;
	TAPI_GLIB_INIT_PARAMS();

	if (pAppRetInfo == NULL)
		return TAPI_API_INVALID_PTR;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pAppRetInfo, sizeof(TelSatAppsRetInfo_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIMATK, TAPI_CS_SIMATK_SEND_APP_EXEC_RESULT, in_param1, in_param2,
			in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		ret_val = g_array_index(out_param1, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return ret_val;
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
EXPORT_API int tel_send_ui_display_status(int commandId, TelSatUiDisplayStatusType_t status)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, " tel_send_ui_display_status");
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"status [%d]\n", status);
	int ret_val;
	TS_BOOL ret = FALSE;
	TAPI_GLIB_INIT_PARAMS();

	if (commandId < 0) {
		TAPI_LIB_DEBUG(LOG_DEBUG,"command id should be positive");
		return TAPI_API_SAT_INVALID_COMMAND_ID;
	}

	if ((status != TAPI_SAT_DISPLAY_SUCCESS) && (status != TAPI_SAT_DISPLAY_FAIL)) {
		TAPI_LIB_DEBUG(LOG_DEBUG,"invalid status sent");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &commandId, sizeof(int));
	g_array_append_vals(in_param1, &status, sizeof(TelSatUiDisplayStatusType_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIMATK, TAPI_CS_UI_DISPLAY_STATUS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);
	if (TRUE == ret) {
		ret_val = g_array_index(out_param1, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return ret_val;
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
EXPORT_API int tel_send_sat_ui_user_confirm(TelSatUiUserConfirmInfo_t *pUserConfirmData)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, " tel_send_sat_ui_user_confirm");
	int ret_val;
	TS_BOOL ret = FALSE;
	TelSatTextInfo_t add_data;
	TAPI_GLIB_INIT_PARAMS();

	if (pUserConfirmData == NULL) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, " pUserConfirmData is NULL");
		return TAPI_API_INVALID_PTR;
	}

	if (pUserConfirmData->commandId < 0) {
		TAPI_LIB_DEBUG(LOG_DEBUG,"command id should be positive");
		return TAPI_API_SAT_INVALID_COMMAND_ID;
	}

	if (pUserConfirmData->dataLen < 0) {
		TAPI_LIB_DEBUG(LOG_DEBUG,"data length should not be negative [%d]", pUserConfirmData->dataLen);
		return TAPI_API_INVALID_INPUT;
	}

	if ((pUserConfirmData->keyType < TAPI_SAT_USER_CONFIRM_YES) || (pUserConfirmData->keyType
			> TAPI_SAT_USER_CONFIRM_TIMEOUT)) {
		TAPI_LIB_DEBUG(LOG_DEBUG,"invalid key type");
		return TAPI_API_INVALID_INPUT;
	}

	if ((pUserConfirmData->pAdditionalData == NULL && pUserConfirmData->dataLen != 0)) {
		TAPI_LIB_DEBUG(LOG_DEBUG,"essential data(pAdditionalData or dataLen) is absent");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &pUserConfirmData->commandId, sizeof(int));
	g_array_append_vals(in_param1, &pUserConfirmData->commandType, sizeof(int));
	g_array_append_vals(in_param1, &pUserConfirmData->keyType, sizeof(int));
	g_array_append_vals(in_param1, &pUserConfirmData->dataLen, sizeof(int));

	if (pUserConfirmData->pAdditionalData != NULL && pUserConfirmData->dataLen != 0) {
		memset(&add_data, 0x00, sizeof(TelSatTextInfo_t));
		memcpy(add_data.string, pUserConfirmData->pAdditionalData, pUserConfirmData->dataLen);
		add_data.stringLen = pUserConfirmData->dataLen;
		g_array_append_vals(in_param2, &add_data, sizeof(TelSatTextInfo_t));
	}

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIMATK, TAPI_CS_UI_USER_CONFIRM, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);
	if (TRUE == ret) {
		ret_val = g_array_index(out_param1, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	if (ret_val == TAPI_API_SUCCESS) {
		return TRUE;
	}

	return FALSE;
}
