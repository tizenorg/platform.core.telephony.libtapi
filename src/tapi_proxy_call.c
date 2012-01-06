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

#include "TelDefines.h"
#include "TelSs.h"
#include "TelCall.h"
#include "tel_cs_conn.h"
#include "tapi_proxy.h"
#include "TapiUtility.h"
#include "TelUtility.h"

extern tapi_dbus_connection_name conn_name;

/******************************************************************************
 *******************************************************************************
 *
 *							G L O B A L   V A R I A B L E S
 *
 ******************************************************************************
 ******************************************************************************/
//	None.


/******************************************************************************
 *******************************************************************************
 *
 *						P R O X Y   C A L  L   A P I's
 *
 *
 ******************************************************************************
 ******************************************************************************/

/**
 *
 * Initiate a new call. Call could be either voice or video.
 *
 * The client specifies the telephone number and the desired call parameters via the
 *
 * tapi_call_setup_info_t argument. After successful completion of the dial request a call identifier is
 *
 * returned back to the client via the ptr_call_handle argument. The call identifier is needed to hold,
 *
 * resume, swap, and terminate the call. It is also needed to monitor the status of the call.
 *
 * This is an asynchronous API. After successful completion of the request in Phone Alert Event is
 *
 * published. otherwise Error event is published.
 *
 * @param[in]		* TelCallSetupParams_t		telephone number and desired call parameters.
 * @param[out]		* pCallHandle ,	*pRequestID.   Handle of the call. specifies the request identifier.
 * @Interface		Synchronous.
 * @return			int				API Result Code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			ptr_call_setup_info, and ptr_call_handle can not be NULL.
 * @see				See below API's also.
 *					- tel_answer_call(),
 *					- tel_release_call(),
 *					- tel_hold_call(),
 *					- tel_retrieve_call(),
 *					.
 */
EXPORT_API int tel_exe_call_mo(const TelCallSetupParams_t *pParams, unsigned int *pCallHandle, int *pRequestID)
{
	int num_len = 0;
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/* check for invalid ptr etc etc if applicable.... */
	TAPI_RET_ERR_NUM_IF_FAIL((pParams && pCallHandle && pRequestID ),
			TAPI_API_INVALID_PTR);

	num_len = strlen(pParams->szNumber);

	/* check for invalid ptr etc etc if applicable.... */
	if ((TAPI_CALL_DIALDIGIT_LEN_MAX < num_len) || (0 >= num_len)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_ : Invalid telephone no length [%+d]", num_len);
		return TAPI_API_INVALID_INPUT;
	}
	else {
		// Do Nothing........
	}

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/* check for the RPC link.... */
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	/* Check input range for possible call types */
	if ((TAPI_CALL_TYPE_VOICE == pParams->CallType) || (TAPI_CALL_TYPE_DATA == pParams->CallType)
			|| (TAPI_CALL_TYPE_E911 == pParams->CallType)) {
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param1);

		g_array_append_vals(in_param1, pParams, sizeof(TelCallSetupParams_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Setup Call: Call Type [%d], Number [%s]",
				pParams->CallType, pParams->szNumber);

		TAPI_PRINT_TIME("tel_exe_call_mo() is called..");

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_SETUP, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/* API return value */
			api_err = g_array_index(out_param1, int, 0);

			/* Request ID */
			*pRequestID = g_array_index(out_param2, int, 0);

			/* Call handle */
			*pCallHandle = g_array_index(out_param3, unsigned int, 0);
		}
		else {
			/* RPC API failed, return FALSE to APP */
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		/* Invalid input range given by the APP. */
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid Input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d] RequestId [%+d] Hanlde [%+d]", api_err, *pRequestID, *pCallHandle);

	return api_err;
}

/**
 * Accept or Reject an incoming new call.
 *
 * Answering an incoming call is possible only when the call status is incoming or waiting.
 *
 * Client can choose to answer an incoming call, even when an active call exists, by specifying argument
 *
 * TelCallAnswerType_t.  The TelCallAnswerType_t can be accept a single call, or hold a current call and
 *
 * accept the incoming call, or replace the current active call with waiting incoming call, or reject the call.
 *
 * @param[in]		CallHandle, a call identifier
 * @param[in]		TelCallAnswerType_t, accept / reject /hold and accept etc.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API Result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should refer a call state which is in incoming/waiting state.
 * @see				See below API's also.
 *					- tel_exe_call_mo(),
 *					- tel_release_call(),
 *					- tel_hold_call(),
 *					- tel_retrieve_call(),
 *					.
 */
EXPORT_API int tel_answer_call(unsigned int CallHandle, TelCallAnswerType_t AnsType, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	/* check for invalid ptr etc etc if applicable.... */
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/* Check the call handle value... */
	if (CallHandle <= 0) {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_ : Invalid Call Handle.");
		return api_err;
	}

	/* Check input range for tapi_answer_type_t */
	if ((TAPI_CALL_ANSWER_ACCEPT <= AnsType) && (TAPI_CALL_ANSWER_HOLD_AND_ACCEPT >= AnsType)) {
		/* check for the RPC link.... */
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param2, &AnsType, sizeof(TelCallAnswerType_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, " Answer Call: Answer Type [%d], Call Handle [%d]",
				AnsType, CallHandle);

		TAPI_PRINT_TIME("tel_answer_call() is called..");

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_ANSWER, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/* Get the API error value as out param 1, from the server. */
			api_err = g_array_index(out_param1, int, 0);

			/* Get the Request ID as out param 2, from the server. */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/* RPC API failed, return FALSE to APP */
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

	}
	else {
		/* Invalid Input Range */
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid Answer Type");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 * Hangup calls. This is only for calls you dialed or answered with Telephony.
 *
 * The client specifies the call handle via the call_handle argument.
 *
 * Upon successful completion of Hangup, the information related to this call handle is destroyed.
 *
 * client can specify to Hangup a single call, all active calls, or all calls via argument tapi_release_type_t.
 *
 * @param[in]		CallHandle		Handle of the call which needs to be Hungup.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int	 API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should contain a call handle which is in connected or held state.
 * @see				See below API's also.
 *					- tel_exe_call_mo(),
 *					- tel_answer_call(),
 *					- tel_hold_call(),
 *					- tel_retrieve_call(),
 *					.
 */

EXPORT_API int tel_release_call(unsigned int CallHandle, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/* check for invalid ptr etc etc if applicable.... */
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if (0 < CallHandle) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Release Call: Call Handle [%d]", CallHandle);

		TAPI_PRINT_TIME("tel_release_call() is called..");

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_RELEASE, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/* Get the API error value as out param 1, from the server. */
			api_err = g_array_index(out_param1, int, 0);

			/* Get the Request ID as out param 2, from the server. */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/* RPC API failed, return FALSE to APP */
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

	}
	else {
		/* Invalid Input Range.. */
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_:: Invalid Call Handle.");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 * Hangup calls. This is only for calls you dialed or answered with Telephony.
 *
 * The client specifies the call handle via the call_handle argument.
 *
 * Upon successful completion of Hangup, the information related to this call handle is destroyed.
 *
 * client can specify to Hangup a single call, all active calls, or all calls via argument tapi_release_type_t.
 *
 * @param[in]		NONE.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int	API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should contain a call handle which is in connected or held state.
 * @see				See below API's also.
 *					- tel_exe_call_mo(),
 *					- tel_answer_call(),
 *					- tel_hold_call(),
 *					- tel_retrieve_call(),
 *					.
 */

EXPORT_API int tel_release_call_all(int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/* check for invalid ptr etc etc if applicable.... */
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	/* check for the RPC link.... */
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
			TAPI_API_SYSTEM_RPC_LINK_DOWN);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Release all calls:");

	TAPI_PRINT_TIME("tel_release_call_all() is called..");

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_RELEASEALL, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/* Get the API error value as out param 1, from the server. */
		api_err = g_array_index(out_param1, int, 0);

		/* Get the Request ID as out param 2, from the server. */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		/* RPC API failed, return FALSE to APP */
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 * Hangup calls. This is only for calls you dialed or answered with Telephony.
 *
 * Upon successful completion of Hangup, the information related to this call handle is destroyed.
 *
 *
 * @param[in]		NONE.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should contain a call handle which is in connected or held state.
 * @see				See below API's also.
 *					- tel_exe_call_mo(),
 *					- tel_answer_call(),
 *					- tel_hold_call(),
 *					- tel_retrieve_call(),
 *					.
 */
EXPORT_API int tel_release_call_all_active(int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
			TAPI_API_SYSTEM_RPC_LINK_DOWN);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Release all active calls:");

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_RELEASEALL_ACTIVE, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err = g_array_index(out_param1, int, 0);

		/* Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		/*	RPC API failed, return FALSE to APP	*/
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;

}

/**
 * Hangup all held calls. This is only for calls you dialed or answered with Telephony.
 *
 *
 * Upon successful completion of Hangup, the information related to this call handle is destroyed.
 *
 *
 * @param[in]		NONE.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int			API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should contain a call handle which is in connected or held state.
 * @see				See below API's also.
 *					- tel_exe_call_mo(),
 *					- tel_answer_call(),
 *					- tel_hold_call(),
 *					- tel_retrieve_call(),
 *					.
 */
EXPORT_API int tel_release_call_all_held(int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
			TAPI_API_SYSTEM_RPC_LINK_DOWN);
	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Release all held calls:");
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_RELEASEALL_HELD, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err = g_array_index(out_param1, int, 0);

		/* Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		/*	RPC API failed, return FALSE to APP	*/
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;

}

/**
 *
 * Places a call on hold. This is only for calls you dialed or answered with Telephony.
 *
 * The client specifies the call handle via the call_handle argument.
 *
 * @param[in]		CallHandle,	Hndle of active call.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @return			int		API result code.
 * @Interface		Asynchronous.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should refer a call which is in ACTIVE state
 * @see				See below API's also.
 *					- tel_retrieve_call().
 *					.
 */
EXPORT_API int tel_hold_call(unsigned int CallHandle, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the call handle  value...	*/
	if (0 < CallHandle) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
				TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Hold Call Handle [%d]", CallHandle);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_HOLD, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}

	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Retrieve held calls. This is only for calls you dialed or answered with Telephony.
 *
 * The client specifies the call handle via the call_handle argument.
 *
 * @param[in]		CallHandle,	Handle of the held call
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_handle should refer a call which is in HELD state..
 * @see				None.
 */
EXPORT_API int tel_retrieve_call(unsigned int CallHandle, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the call handle  value...	*/
	if (0 < CallHandle) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Activate Call Handle [%d]", CallHandle);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_ACTIVATE, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Swap calls. This is only for calls you dialed or answered with Telephony.
 *
 * The client specifies the call handles via the active_call and held_call arguments.
 *
 * Swap is only available for the voice calls.
 *
 * @param[in]		CallHandle1	Handle of the active call.
 * @param[in]		CallHandle2	Handle of the held call.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			NONE.
 * @see				NONE.
 */
EXPORT_API int tel_swap_call(unsigned int CallHandle1, unsigned int CallHandle2, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if ((0 < CallHandle1) && (0 < CallHandle2)) {
		if (CallHandle1 == CallHandle2) {
			return TAPI_API_INVALID_CALL_HANDLE;
		}

		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle1, sizeof(unsigned int));
		g_array_append_vals(in_param2, &CallHandle2, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Swap calls with handles [%d], and [%d]",
				CallHandle1, CallHandle2);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_SWAP, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Setup a conference call.
 *
 * The client specifies the call handles via the active_call and held_call arguments.
 *
 * Setup a conference call is only available for the voice calls.
 *
 * @param[in]		CallHandle1	Handle of the active call.
 * @param[in]		CallHandle2	Handle of the held call.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			NONE.
 * @see				NONE.
 */
EXPORT_API int tel_join_call(unsigned int CallHandle1, unsigned int CallHandle2, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((0 < CallHandle1) && (0 < CallHandle2)) {
		if (CallHandle1 == CallHandle2) {
			return TAPI_API_INVALID_CALL_HANDLE;
		}

		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle1, sizeof(unsigned int));
		g_array_append_vals(in_param2, &CallHandle2, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Join Calls with Handles [%d], [%d]",
				CallHandle1, CallHandle2);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_SETUPCONFCALL, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Split a call from conference call.
 *
 * The client specifies the call handle via the call_handle argument.
 *
 * Split a conference call is only available for the voice calls.
 *
 * @param[in]		CallHandle	Handle of the call which needs to be seperated.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			NONE.
 * @see				NONE.
 */
EXPORT_API int tel_split_call(unsigned int CallHandle, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((0 < CallHandle)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(),
				TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Split Call Handle [%d]", CallHandle);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_SPLITCONFCALL, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Transfers a call to another call.
 *
 * client specifies the handle  of the calls via active_call, held_call arguments.
 *
 * Transfer call is only available for the voice calls.
 *
 * @param[in]		CallHandle,	Handle of the active call.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			NONE.
 * @see				NONE.
 */
EXPORT_API int tel_exe_call_explicit_transfer(unsigned int CallHandle, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if (0 < CallHandle) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "ECT Call Handle [%d]", CallHandle);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_TRANSFERCALL, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Activate Call Completion to a Busy Subscriber.
 *
 * client specifies the handle  of the call via call_handle argument.
 *
 * Activate CCBS is only available for the voice calls.
 *
 * @param[in]		CallHandle,	Handle of the call.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			NONE.
 * @see				NONE.
 */
EXPORT_API int tel_activate_call_ccbs(unsigned int CallHandle, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	Check the call handle  value...	*/
	if (0 < CallHandle) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "CCBS Call Handle [%d]", CallHandle);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_ACTIVATECCBS, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *  Send DTMF digits to the network.
 *
 * Client specifies the Handle of the active call via call_handle argument.
 *
 * The handle of the call is voice call only.
 *
 * @param[in]		tapi_call_dtmf_t	DTMF digits.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			tapi_call_dtmf_t can not be NULL.
 * @see				NONE.
 */
EXPORT_API int tel_send_call_dtmf(const char *pDtmfString, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int length = 0;
	int app_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL( (pRequestID && pDtmfString), TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	length = strlen(pDtmfString);

	/*	Check the input value...	*/
	if (length != 0) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		TAPI_LIB_DEBUG(LEVEL_INFO, "Send DTMF digits [%s] and len [%d]",
				pDtmfString, length);

		/*	to allocate for null termiated string	*/
		g_array_append_vals(in_param1, pDtmfString, (length + 1));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_DTMF, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			app_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			app_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		app_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]", app_err, *pRequestID);

	return app_err;
}

/**
 *
 * Call Deflection.
 *
 * If informed about an incoming call this call may be redirected to an another destination by
 *
 * entering the destination Number. The cleint spcifies the dstiantion number to which the current
 *
 * incoming call needs to be redirected is specifed via info argument.
 *
 * @param[in]		call_handle,			Incoming call handle.
 * @param[in]		TelCallDeflectDstInfo_t	Destination Number.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			info can not be NULL.
 * @see				None.
 */
EXPORT_API int tel_deflect_call(unsigned int CallHandle, const TelCallDeflectDstInfo_t *pInfo, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	 check for the input pointer .. */
	TAPI_RET_ERR_NUM_IF_FAIL( (pRequestID && pInfo), TAPI_API_INVALID_PTR);

	/*	Check the input value...	*/
	if (0 < CallHandle) {
		/*	 check for the RPC link.... */
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));
		g_array_append_vals(in_param2, pInfo, sizeof(TelCallDeflectDstInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Deflect Call Handle [%d] to [%s]",
				CallHandle, pInfo->number);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_DEFLECT, in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);

		}
		else {
			/*	RPC API failed, return FALSE to APP */
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid CALL HANDLE");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Get the current calling line identification number.
 *
 * The line id is returned via the argument ptr_active_line.
 *
 * @param[in]		NONE
 * @param[out]		TelCallActiveLine_t 	current active line id, *pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			TelCallActiveLine_t can not be NULL.
 * @see				None.
 */
EXPORT_API int tel_get_call_act_line(int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL( pRequestID, TAPI_API_INVALID_PTR);

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Get Active Line");

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_GETACTIVELINE, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);

	}
	else {
		/*	RPC API failed, return FALSE to APP	*/
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * Set the current calling line identification number.
 *
 * Client specifies the desired line id via the argument active_line.
 *
 * @param[in]		TelCallActiveLine_t	calling line identification.
 * @param[out]		pRequestID.   specifies the request identifier.
 * @Interface		Asynchronous.
 * @return			int		API result code.
 * @exception		In case of exceptions return value contains appropriate error code.
 * @remarks			NONE.
 * @see				NONE.
 */
EXPORT_API int tel_set_call_act_line(TelCallActiveLine_t active_line, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((TAPI_CALL_ACTIVE_LINE1 == active_line) || (TAPI_CALL_ACTIVE_LINE2 == active_line)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &active_line, sizeof(TelCallActiveLine_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Set Active Line ID [%d]", active_line);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_SETACTIVELINE, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1,out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 * @brief This function gets status for the current call identified by Call Handle whenever
 *	application wants the call status. Call handle must be valid.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[in] CallHandle
 *  - Unique handle for referring the call.
 *
 * @param [out] pCallStatus
 *   - Call status information like destination number, call direction (MO or MT), call type (voice or data etc), whether
 *      the call is in conference state or not, present call state etc are returned through this parameter.
 *
 * @par Async Response Message:
 *   - None.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 */
EXPORT_API int tel_get_call_status(unsigned int CallHandle, TelCallStatus_t *pCallStatus)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "CallHandle:[%d]\n",CallHandle);

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pCallStatus, TAPI_API_INVALID_PTR);

	if (CallHandle > 0) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));

		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Call Service Request: .(Get Call Status)");

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_GETSTATUS, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the call status as out param 3, from the server.	*/
			*pCallStatus = g_array_index(out_param3, TelCallStatus_t , 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			pCallStatus = NULL;
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		pCallStatus = NULL;
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "#####Invalid Handle");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d]", api_err);

	return api_err;
}

/*================================================================================================*/

/**
 * \addtogroup  tel_get_call_duration
 * @{
 */

/**
 * @brief This function gets duration of the given call. This is a synchronous function. Duration is accounted from the moment
 *          the connection is established, i.e. call goes into active state for first time.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[in] CallHandle
 *  - Unique handle for referring the call.
 *
 * @param [out] pDurationInSecs
 *   - The total call duration in seconds from the call connecting time to the present time is returned through this parameter.
 *
 * @par Async Response Message:
 *  - None.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 *
 */
EXPORT_API int tel_get_call_duration(unsigned int CallHandle, unsigned int *pDurationInSecs)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance \n");
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"CallHandle:[%d]\n",CallHandle);

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pDurationInSecs, TAPI_API_INVALID_PTR);

	if (CallHandle > 0) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));

		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Call Service Request: .(Get Call Duration)");

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_GETDURATION, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the call durations as out param 3, from the server.	*/
			*pDurationInSecs = g_array_index(out_param3, unsigned int , 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			*pDurationInSecs = 0;
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);
	}
	else {
		*pDurationInSecs = 0;
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "#####Invalid Handle.");
	}

	return api_err;
}

/**
 * @brief This is a synchronous function returns all call handles within the given conference call.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *  - Call should be a Multi-party conference call in order to return actual no of calls and call handles in the conference calls.
 *
 * @warning
 * - None.
 *
 *
 * @param[in] CallHandle
 *  - Handle of call which is associated with the conference.
 *
 * @param [out] pCallList
 *   - list of call joined in the conference call. In case there is no active conference. The list will be zero and
 *      number of calls parameter value will also be zero. Maximum number of calls in a conference can be upto 5
 *      (Based on 3GPP TS 22.084). Memory allocation for calllist is integer array of size 5.
 *
 * @param [out] pNoOfCalls
 *   - Number of the calls present in conference.
 *
 * @par Async Response Message:
 *  -None.
 *
 * @pre
 *  -None.
 *
 * @post
 *  -None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 */
EXPORT_API int tel_get_call_conf_list(unsigned int CallHandle, unsigned int *pCallList, int *pNoOfCalls)
{
	TS_BOOL ret = FALSE;
	unsigned int * tempCallList;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"<PROXY> Func Entrance \n");
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"CallHandle:[%d]\n",CallHandle);

	/*	 check for the input pointer .. */
	TAPI_RET_ERR_NUM_IF_FAIL( (pCallList && pNoOfCalls), TAPI_API_INVALID_PTR);

	*pNoOfCalls = 0;
	*pCallList = 0;

	if (0 < CallHandle) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &CallHandle, sizeof(unsigned int));

		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Call Service Request: .(Get Conference List)");

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_GETCONFERENCELIST, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the No. of Calls as out param 4, from the server.    */
			*pNoOfCalls = g_array_index(out_param4, int , 0);

			if (*pNoOfCalls > 0) {
				/*	Get the Call List as out param 3, from the server.    */
				tempCallList = &g_array_index(out_param3, unsigned int , 0);
			}
			else {
				tempCallList = NULL;
			}

			if (tempCallList != NULL)
				memcpy(pCallList, tempCallList, sizeof(unsigned int) * (*pNoOfCalls));

		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", api_err);
	}
	else {
		api_err = TAPI_API_INVALID_CALL_HANDLE;
		TAPI_LIB_DEBUG(LEVEL_ERR, "#####Invalid Handle.");
	}

	return api_err;
}

/**
 *
 * @brief This function gets voice privacy option mode in phone(CDMA only).
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[in]
 * - None.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t. #TelCallPrivacyMode_t is included in this event.
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 *
 */
EXPORT_API int tel_get_call_privacy_mode(TelCallPrivacyType_t PrivacyType, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((TAPI_CALL_PRIVACY_TYPE_MS <= PrivacyType) && (PrivacyType <= TAPI_CALL_PRIVACY_TYPE_CURRENT)) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &PrivacyType, sizeof(TelCallPrivacyType_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Get Request PrivacyType:[%d]", PrivacyType);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_GETPRIVACYMODE, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]",api_err, *pRequestID);

	return api_err;
}

/**
 *
 * @brief This function sets voice privacy option mode in phone. It is available only where call exists(CDMA only).
 *
 * Access to this API is limited, we recommand you use Voice Call engine API.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[in] PrivacyMode
 * - voice privacy option mode(ENHANCED or STANDARD)
 *
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t.
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 *
 */
EXPORT_API int tel_set_call_privacy_mode(TelCallVoicePrivacyInfo_t PrivacyInfo, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (((TAPI_CALL_PRIVACY_TYPE_MS <= PrivacyInfo.PrivacyType) && (TAPI_CALL_PRIVACY_TYPE_CURRENT
			>= PrivacyInfo.PrivacyType)) && ((TAPI_CALL_PRIVACY_MODE_STANDARD == PrivacyInfo.PrivacyMode)
			|| (TAPI_CALL_PRIVACY_MODE_ENHANCED == PrivacyInfo.PrivacyMode))) {
		/*	 check for the RPC link....	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);

		g_array_append_vals(in_param1, &PrivacyInfo, sizeof(TelCallVoicePrivacyInfo_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		TAPI_LIB_DEBUG(LEVEL_INFO, "Set PrivacyType:[%d], PrivacyMode:[%d]", PrivacyInfo.PrivacyType, PrivacyInfo.PrivacyMode);

		ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_SETPRIVACYMODE, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (TRUE == ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			api_err = g_array_index(out_param1, int, 0);

			/*	Get the Request ID as out param 2, from the server.    */
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			/*	RPC API failed, return FALSE to APP	*/
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
		}

		TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				out_param1, out_param2, out_param3, out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, "_CALL_ERR_: Invalid input");
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 *
 * @brief This function requests to send a Flash with Information Message(CDMA only).
 *
 * Access to this API is limited, we recommand you use Voice Call engine API.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[in] pDialNumber
 * - this is the calling number for 3 way call. But in the call waiting, this param should be NULL.
 *
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_FLASHINFO_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t.
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 *
 *
 */
EXPORT_API int tel_exe_call_flash_info(const char *pDialNumber, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int length = 0;
	int app_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL( pRequestID, TAPI_API_INVALID_PTR);

	if (pDialNumber) {
		length = strlen(pDialNumber);
		TAPI_LIB_DEBUG(LEVEL_INFO, "DialNumber: [%s] and len:[%d]", pDialNumber, length);
	}

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	/*	to allocate for null termiated string	*/

	if (length > 0)
		g_array_append_vals(in_param1, pDialNumber, (length + 1));

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_FLASHINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		app_err = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		/*	RPC API failed, return FALSE to APP	*/
		app_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d], Reuest ID [%+d]",app_err, *pRequestID);

	return app_err;
}

/**
 * tel_exit_call_emergency_mode
 *
 *
 */
EXPORT_API int tel_exit_call_emergency_mode(int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL( pRequestID, TAPI_API_INVALID_PTR);

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_EXITEMERGENCYMODE, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);

	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/**
 * tel_get_call_time
 *
 *
 */
EXPORT_API int tel_get_call_time(unsigned short req_mask, int *pRequestID)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");

	/*	 check for invalid ptr etc etc if applicable....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &req_mask, sizeof(unsigned short));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	TAPI_LIB_DEBUG(LEVEL_INFO, "Get Call time req type:[%x]", req_mask);

	ret = tapi_send_request(TAPI_CS_SERVICE_CALL, TAPI_CS_CALL_GETCALLTIME, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		api_err = g_array_index(out_param1, TapiResult_t, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		/*	RPC API failed, return FALSE to APP	*/
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_ALERT, "_CALL_ERR_ : RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
			out_param1, out_param2, out_param3, out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, " Returning TapiResult_t [%+d], Reuest ID [%+d]", api_err, *pRequestID);

	return api_err;
}

/*	EOF  */

