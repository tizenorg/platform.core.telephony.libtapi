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

#include "tel_cs_conn.h"
#include "tapi_proxy.h"
#include "TapiUtility.h"
#include "TelDefines.h"
#include "TelSs.h"

#include <vconf.h>

extern tapi_dbus_connection_name conn_name;

/**********************************************************************************************
 *
 * This function is used to Activate or Deactivate the call barring feature.
 *
 * @param	barringInfo[IN] of type TelSsCallBarringInfo_t Specifies the Teleservice and barring flavour and CB password.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark	Call barring password must be known to activate or to deactivate.
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_barring(const TelSsCallBarringInfo_t *barringInfo, int *req_id)
{

	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(barringInfo, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/*	Check input range for tele service type and call barring flavour	*/
	if (((barringInfo->CallType >= TAPI_CALL_TYPE_VOICE_EV) && (barringInfo->CallType <= TAPI_CALL_TYPE_ALL_TELE))
			&& ((barringInfo->Type >= TAPI_CALL_BARRING_ALL) && (barringInfo->Type <= TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM))
			&& ((barringInfo->Mode >= TAPI_SS_CALL_BAR_ACTIVATE) && (barringInfo->Mode <= TAPI_SS_CALL_BAR_DEACTIVATE))
			&& (strnlen(barringInfo->szPassword, 4) == TAPI_SS_GSM_BARR_PW_LEN_MAX)) {
		/*	 Check for the RPC link.	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, barringInfo, sizeof(TelSsCallBarringInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_BARRSETREQ, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);

			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;

}

/**********************************************************************************************
 *
 * This function is used to Activate or Deactivate the call barring feature.
 *
 * @param	barType[IN] of type TelSsCallBarType_t Specifies the  barring flavour.
 * @param	callType[IN] of type TelSsCallType_t Specifies the teleservice type.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_get_ss_barring_status(TelSsCallBarType_t barType, TelSsCallType_t callType, int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/*	Check input range for tele service type and call barring flavour	*/
	if (((barType >= TAPI_CALL_BARRING_ALL) && (barType <= TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM))
			&& ((callType >= TAPI_CALL_TYPE_VOICE_EV) && (callType <= TAPI_CALL_TYPE_ALL_TELE))) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &barType, sizeof(TelSsCallBarType_t));
		g_array_append_vals(in_param2, &callType, sizeof(TelSsCallType_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_BARRQUERYSTATUSREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		/*	Invalid input range given by the APP.	*/
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to change call barring password.
 *
 * @param	pOldPassword[IN] of type char* Specifies the Old Call Barring Password.
 * @param	pNewPassword[IN] of type char* Specifies the New Call Barring Password.
 * @param	pNewPasswordAgain[IN] of type char* Specifies the New Call Barring Password for Confirm.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_change_ss_barring_password(
		const char *pOldPassword,
		const char *pNewPassword,
		const char *pNewPasswordAgain,
		int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(pOldPassword, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pNewPassword, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pNewPasswordAgain, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	// check should be done at APP
	if ((strlen(pOldPassword) == TAPI_SS_GSM_BARR_PW_LEN_MAX) && (strlen(pNewPassword) == TAPI_SS_GSM_BARR_PW_LEN_MAX)) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, pOldPassword, TAPI_SS_GSM_BARR_PW_LEN_MAX);
		g_array_append_vals(in_param2, pNewPassword, TAPI_SS_GSM_BARR_PW_LEN_MAX);
		g_array_append_vals(in_param3, pNewPasswordAgain, TAPI_SS_GSM_BARR_PW_LEN_MAX);
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_BARRCHANGEPWDREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);

		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		/*	Invalid input range given by the APP.	*/
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input  - Password length error");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to Activate or Deactivate the call forwarding feature.
 *
 * @param	forwardInfo[IN] of type TelSsForwardInfo_t which specifies the Teleservice and CF flavour, Forwarded to number.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_forward(const TelSsForwardInfo_t *forwardInfo, int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(forwardInfo, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);


	TAPI_LIB_DEBUG(LEVEL_DEBUG, "szPhoneNumber is [%s]", forwardInfo->szPhoneNumber);
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "szPhoneNumber Length is [%d]", strlen(forwardInfo->szPhoneNumber));

	/*	Check input range for tele service type and call forwarding flavour	*/
	if (((forwardInfo->Condition >= TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV) && (forwardInfo->Condition <= TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV))
			&& ((forwardInfo->Mode >= TAPI_CALL_FORWARD_MODE_DISABLE_EV) && (forwardInfo->Mode <= TAPI_CALL_FORWARD_MODE_ERASURE_EV))
			&& ((forwardInfo->Type >= TAPI_CS_FORWARD_TYPE_VOICE_EV) && (forwardInfo->Type <= TAPI_CS_FORWARD_TYPE_ALL_TELE))
			&& (strnlen(forwardInfo->szPhoneNumber, (TAPI_CALL_DIALDIGIT_LEN_MAX)) < (TAPI_CALL_DIALDIGIT_LEN_MAX))) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, forwardInfo, sizeof(TelSsForwardInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_FWDSETREQ, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);
		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to interrogate the status of call forwarding feature.
 *
 * @param	forwardType[IN] of type TelSsForwardType_t which specifies the telecommunication service.
 * @param	condition[IN]	is of type TelSsForwardWhen_t specified the CF flavour
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_get_ss_forward_status(TelSsForwardType_t forwardType, TelSsForwardWhen_t condition, int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/*	Check input range for tele service type and call forwarding flavour	*/
	if (((condition >= TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV)
			&& (condition <= TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV))
			&& ((forwardType >= TAPI_CS_FORWARD_TYPE_VOICE_EV) && (forwardType <= TAPI_CS_FORWARD_TYPE_ALL_TELE))) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &forwardType, sizeof(TelSsForwardType_t));
		g_array_append_vals(in_param2, &condition, sizeof(TelSsForwardWhen_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_FWDQUERYSTATUSREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);

		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to Activate or dectivate call waiting feature.
 *
 * @param	waitInfo[IN] of type TelSsWaitingInfo_t which specifies the telecommunication service and mode.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_waiting(const TelSsWaitingInfo_t *waitInfo, int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(waitInfo, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	/*	Check input range for tele service type and call forwarding flavour	*/
	if (((waitInfo->Mode >= TAPI_SS_CW_ACTIVATE)
			&& (waitInfo->Mode <= TAPI_SS_CW_DEACTIVATE))
			&& ((waitInfo->CallType >= TAPI_CALL_TYPE_VOICE_EV) && (waitInfo->CallType <= TAPI_CALL_TYPE_ALL_TELE))) {

		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, waitInfo, sizeof(TelSsWaitingInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_WAITSETREQ, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to interrogate the status of call waiting feature.
 *
 * @param	CallType[IN] of type TelSsCallType_t which specifies the telecommunication service .
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/

EXPORT_API int tel_get_ss_waiting_status(const TelSsCallType_t CallType, int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/*	Check input range for tele service type	*/
	if ((CallType >= TAPI_CALL_TYPE_VOICE_EV) && (CallType <= TAPI_CALL_TYPE_ALL_TELE)) {

		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &CallType, sizeof(TelSsCallType_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_WAITQUERYSTATUSREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		returnStatus = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", returnStatus);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to interrogate the status of CLI services feature.
 *
 * @param	CliType[IN] of type TelSsCliType_t which specifies the CLI service .
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_get_ss_cli_status(TelSsCliType_t CliType, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/*	Check input range for CLI service type	*/
	if ((CliType >= TAPI_SS_CLI_CLIP) && (CliType <= TAPI_SS_CLI_CNAP)) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &CliType, sizeof(TelSsCliType_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_CLIQUERYSTATUSREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);

		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		api_err = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

EXPORT_API int tel_send_ss_ussd_request(const TelSsUssdMsgInfo_t *ussd_info, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;
	int emergency_mode = 0;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (vconf_get_int("db/telephony/emergency", &emergency_mode) != 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL]GET db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
	if (emergency_mode) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "emergency mode on");
		return TAPI_API_OPERATION_FAILED;
	}

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(ussd_info, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if (((ussd_info->UssdStringLength > 0) && (ussd_info->UssdStringLength < TAPI_SS_USSD_DATA_SIZE_MAX))) {

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"--- USSD STR(%s),length = %d", ussd_info->szUssdString,ussd_info->UssdStringLength);

		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, ussd_info, sizeof(TelSsUssdMsgInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_USSDSENDSTRINGREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input -USSD  length Error ");
		api_err = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**********************************************************************************************
 *
 * This function is used to send a USSD Rsp.
 *
 * @param	ussdRspRecord, req_id
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_send_ss_ussd_response(const TelSsUssdMsgInfo_t *ussd_rsp, int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(ussd_rsp, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if (((ussd_rsp->UssdStringLength > 0) && (ussd_rsp->UssdStringLength < TAPI_SS_USSD_DATA_SIZE_MAX))) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, ussd_rsp, sizeof(TelSsUssdMsgInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG, "SS Service Request:(Send USSD string req)");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_USSDRSP, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			returnStatus = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		returnStatus = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR,"INVALID INPUT TYPE  \n");
		TAPI_LIB_DEBUG(LEVEL_ERR, "##### Invalid Response Type ......");
	}

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to send a USSD cancel to release ussd session.
 *
 * @param	req_id
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_send_ss_ussd_cancel(int *req_id)
{
	TS_BOOL ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/* Check the input ptr. If NULL then return FALSE */
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/* Check if D-Bus service is available. If Failed then return FALSE */
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "SS Service Request:(Send USSD cancel)");
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_USSDCANCEL, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		returnStatus = g_array_index(out_param1, int, 0);
		*req_id = g_array_index(out_param2, int ,0);
	}
	else {
		returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return returnStatus;
}

/**********************************************************************************************
 *
 * This function is used to Sets AOC information.
 *
 * @param	AocType[IN] of type TelSsAocType_t which specifies the which AOC type to read.
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_get_ss_aoc_info(TelSsAocType_t AocType, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	/*	Check input range for Aoc type	*/
	if ((AocType == TAPI_SS_AOC_TYPE_ACM)
			|| (AocType == TAPI_SS_AOC_TYPE_PUC)
			|| (AocType == TAPI_SS_AOC_TYPE_MAXACM)
			|| (AocType == TAPI_SS_AOC_TYPE_CCM)) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &AocType, sizeof(TelSsAocType_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_GETCALLCOSTREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);
			*req_id = g_array_index(out_param2, int ,0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input - Invalid AOC type");
		api_err = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/**********************************************************************************************
 *
 * This function is used to Sets AOC information.
 *
 * @param	AocSetInfo[IN] of type TelCallAocInfo_t which specifies the AOC info parameters .
 * @return	enum type TapiResult_t Specifies the API's Return result.
 * @remark
 *
 ***********************************************************************************************/
EXPORT_API int tel_set_ss_aoc_info(TelCallAocInfo_t *AocSetInfo, int *RequestId)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the input ptr. If NULL then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(AocSetInfo, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(RequestId , TAPI_API_INVALID_PTR);

	if ((AocSetInfo->PPM) < ((float) (TAPI_MAX_ULONG / 1000))) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Valid ... Input PPM smaller than MAX");
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"INVALID.... INPUT greater than MAX");
	}

	/*	Check input range for Aoc type	*/
	if ((AocSetInfo->PPM < (float) (TAPI_MAX_ULONG / 1000))
			&& ((AocSetInfo->AocType == TAPI_SS_AOC_TYPE_RESET)
				|| (AocSetInfo->AocType == TAPI_SS_AOC_TYPE_PUC)
				|| (AocSetInfo->AocType == TAPI_SS_AOC_TYPE_MAXACM) )) {
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, AocSetInfo, sizeof(TelCallAocInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func ");

		ret = tapi_send_request(TAPI_CS_SERVICE_SS, TAPI_CS_SS_SETCALLCOSTREQ, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*RequestId = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input -Invalid AOC type ");
		api_err = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/*	EOF  */
