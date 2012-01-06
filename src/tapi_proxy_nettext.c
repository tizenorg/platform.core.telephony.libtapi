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


/**
 * @ingroup		TAPI
 * @defgroup	NetText
 * @{
 *
 * NetText APIs allow a client to accomplish the following features: @n
 * - Send,Recieve ,Save,Delete,Read short messages  @n
 * - Set and Get information like Service Center Address, Cell Broadcast configuration,Preferred Bearer, SMS parameters @n
 * - Retrieve information like Current Memory selected, NetTextCount related to the messages @n
 * - Set delivery report @n
 */

#include "tel_cs_conn.h"
#include "tapi_proxy.h"
#include "TelNetText.h"
#include "TapiUtility.h"

#include <vconf.h>

extern tapi_dbus_connection_name conn_name;

/**
 *
 * This function enables the applications to send nettext to the network.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]	       pDataPackage - SMS-SUBMIT TPDU or SMS-COMMAND, and its length have to be passed in this structure.
 *				tapi_sms_more_to_send_t parameter denotes whether the sms is short or concatenated.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer		tapi_nettext_datapackage, tapi_sms_more_to_send_t.
 */
EXPORT_API int tel_send_sms(const TelSmsDatapackageInfo_t *pDataPackage, int bMoreMsgToSend, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;
	int emergency_mode = 0;

	if (vconf_get_int("db/telephony/emergency", &emergency_mode) != 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL]GET db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
	if (emergency_mode) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "emergency mode on");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(pDataPackage ,TAPI_API_INVALID_PTR);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pDataPackage, sizeof(TelSmsDatapackageInfo_t));
	g_array_append_vals(in_param2, &bMoreMsgToSend, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SEND, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to set the deliver report for an incoming message(MT).
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]	       pDataPackage - SMS-DELIVER-REPORT, and its length have to be passed in this structure.
 * @param[in]		RPCause - the result cause
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Synchronous.
 * @remark
 * @Refer	       tapi_nettext_deliver_report
 */
EXPORT_API int tel_send_sms_deliver_report(
		const TelSmsDatapackageInfo_t *pDataPackage,
		TelSmsResponse_t RPCause,
		int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RET_ERR_NUM_IF_FAIL(pDataPackage , TAPI_API_INVALID_PTR);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pDataPackage, sizeof(TelSmsDatapackageInfo_t));
	g_array_append_vals(in_param2, &RPCause, sizeof(TelSmsResponse_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETDELIVERREPORT, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);

	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to read nettext from the preferred storage.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be read
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark		tapi_nettext_selmem_set is to be called always to select the
 *				preferred memory before calling this api.
 * @Refer
 */
EXPORT_API int tel_read_sms_in_sim(int Index, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if (Index < 0) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "Invalid Input -Read Nettext %d",Index);

		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &Index, sizeof(unsigned int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_READ, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to write/save nettext to the preferred storage.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be saved, msg_status will denote the status
 *				of the message whether Sent, Unsent, Read, Unread, Unknown.
 *				pDataPackage- the SMS-SUBMIT PDU or SMS-DELIVER,SMS-STATUSREPORT
 *				being passed to be saved in memory.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @Refer		tapi_nettext_datapackage, tapi_nettext_status_type.
 */
EXPORT_API int tel_write_sms_in_sim(const TelSmsData_t *pWriteData, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RET_ERR_NUM_IF_FAIL(pWriteData ,TAPI_API_INVALID_PTR);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((pWriteData->MsgStatus < TAPI_NETTEXT_STATUS_READ) || (pWriteData->MsgStatus > TAPI_NETTEXT_STATUS_RESERVED)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "Invalid Input -Write Nettext %d",pWriteData->MsgStatus);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pWriteData, sizeof(TelSmsData_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_WRITE, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);

	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to delete nettext to the preferred storage.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be deleted. if index is -1, all sms in the sim are deleted.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark		tapi_nettext_selmem_set has to be called always before calling this API
 * @Refer
 */
EXPORT_API int tel_delete_sms_in_sim(int Index, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if ((Index < 0) || (Index > TAPI_NETTEXT_MAX_INDEX)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &Index, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_DELETE, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to get the count of the messages stored in the memory
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]	None
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *              Its value can be any value from 0 to 255 if the API is returned successfully
 *	            -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface	Asynchronous
 * @remark		The requested memory details come in response API
 * @Refer	    None
 */
EXPORT_API int tel_get_sms_count(int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_GETCOUNT, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;

}

/**
 *
 * This function enables the application to set the status of a message
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		index - the message to be changed
 *				msgStatus -SMS message status
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_nettext_memory_type,tapi_nettext_status_type
 */
EXPORT_API int tel_set_sms_message_status(int Index, TelSmsMsgStatus_t msgStatus, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((Index < 0) || (Index > TAPI_NETTEXT_MAX_INDEX) || (msgStatus < TAPI_NETTEXT_STATUS_READ) || (msgStatus
			> TAPI_NETTEXT_STATUS_RESERVED)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -MsgStatus Set Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &Index, sizeof(int));
	g_array_append_vals(in_param2, &msgStatus, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETMSGSTATUS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to set the phone memory status whether full or available
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		memory_status whether full or available
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_nettext_memstatus_type
 */
EXPORT_API int tel_set_sms_memory_status(int memoryStatus, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((memoryStatus < TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE) || (memoryStatus
			> TAPI_NETTEXT_PDA_MEMORY_STATUS_FULL)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -MemoryStatus Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &memoryStatus, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETMEMSTATUS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);

	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to set the configuration for cell broadcast messages
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		tapi_nettext_cb_config the configuration details to be set
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_nettext_cb_config
 */
EXPORT_API int tel_set_sms_cb_config(const TelSmsCbConfig_t *pCBConfig, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	TAPI_RET_ERR_NUM_IF_FAIL(pCBConfig , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, " CBEnabled(%d)",pCBConfig->bCBEnabled);

	if ((pCBConfig->SelectedId >= 3) || (pCBConfig->MsgIdCount < 0)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -SelectedId(%d)",pCBConfig->SelectedId);
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -MsgIdCount(%d)",pCBConfig->MsgIdCount);

		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pCBConfig, sizeof(TelSmsCbConfig_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETCBCONFIG, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to set the configuration for cell broadcast messages
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       None
 */
EXPORT_API int tel_get_sms_cb_config(int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_GETCBCONFIG, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to set the service center address in the SIM storage
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		pSCA - the service center address that is to be set in the SIM storage
 *					    refer to 3GPP TS 23.040:9.1.2.5 Address fields
 *				index - the index that is to be set in the SIM storage
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark
 * @Refer	       tapi_nettext_sms_addr_info
 */
EXPORT_API int tel_set_sms_sca(const TelSmsAddressInfo_t *pSCA, int Index, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RET_ERR_NUM_IF_FAIL(pSCA , TAPI_API_INVALID_PTR);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((Index < 0) || (Index > TAPI_NETTEXT_MAX_INDEX)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Index Input");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pSCA, sizeof(TelSmsAddressInfo_t));
	g_array_append_vals(in_param2, &Index, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETSCA, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to get the service center address from the SIM storage
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		pSCA - the service center address that is to be set in the SIM storage
 *					    refer to 3GPP TS 23.040:9.1.2.5 Address fields
 *				index - the index that is to be set in the SIM storage
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_nettext_sms_addr_info
 */
EXPORT_API int tel_get_sms_sca(int Index, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((Index < 0) || (Index > TAPI_NETTEXT_MAX_INDEX)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -SMS Param Get Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &Index, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_GETSCA, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to set the preferred bearer
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		service option ie the bearer type to be set
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer	       tapi_nettext_bearer_type
 */
EXPORT_API int tel_set_sms_preferred_bearer(TelSmsBearerType_t BearerType, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((TAPI_NETTEXT_BEARER_PS_ONLY > BearerType) || (TAPI_NETTEXT_NO_PREFERRED_BEARER < BearerType)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "Invalid Input -PrefBearer Set Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &BearerType, sizeof(TelSmsBearerType_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETPREFBEARER, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function is used to get SMS preferred bearer on which SMS has to be transmitted. This is a synchronous function
 *
 * @return		TRUE in case of success and FALSE in case of error .
 * @param[in]        None
 *
 * @param[out]	requestId-Unique identifier for a particular request
 *                          Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_get_sms_preferred_bearer(int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_GETPREFBEARER, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;

}

/**
 *
 * This function enables to application to get the sms parameter count from the EFsmsp file.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark		Requested details come in response API
 * @Refer
 */
EXPORT_API int tel_get_sms_parameter_count(int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);
	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_GETPARAMCNT, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables to application to get the sms parameters
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		record index of the record in which the sms parameters are stored in the EFsmsp file
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark		Requested details come in response API
 * @Refer
 */
EXPORT_API int tel_get_sms_parameters(int index, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;

	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);
	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((index < 0) || (index > TAPI_NETTEXT_MAX_INDEX)) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid Input -SMS Param Get Nettext");
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &index, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_GETPARAMS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables to application to get the sms parameters
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		tapi_smsp_set_param_t sms parameters to be set in the EFsmsp file
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 * @Interface		ASynchronous.
 * @remark
 * @Refer	       tapi_smsp_set_param_t
 */
EXPORT_API int tel_set_sms_parameters(const TelSmsParams_t *pSmsSetParameters, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_RETURN_VAL_IF_FAIL(pSmsSetParameters, TAPI_API_INVALID_PTR);

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pSmsSetParameters, sizeof(TelSmsParams_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SETPARAMS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 *  This API is used to send an SMS message to the network. This API allows keeping
 *  the dedicated link at lower layers by passing information as more messages to send in parameters.
 * This will enable not to release if a dedicated connection is used for transmission.
 *
 * @return		TRUE in case of success and FALSE in case of error .
 * @param[in]		TelSmsMsgInfo_t - SMS_SUBMIT/ACK/CANCEL and its length have to be passed in this structure.
 *
 *				unsigned int MoreMsgToSend will be set to TRUE when there are more than one message to be
 *                         send or Multiple segmented concatenated message to be send, otherwise FALSE. This flag
 *                         indicates OEM that more messages to send.
 *
 * @param[out]	RequestId-Unique identifier for a particular request
 *                         Its value can be any value from 0 to 255 if the API is returned successfully
 *	                     -1 (INVALID_REQUEST_ID) will be sent in case of failure
 *
 * @Interface		Asynchronous.
 * @remark
 * @Refer		TelSmsMsgInfo_t.
 */
EXPORT_API int tel_send_sms_msg(const TelSmsMsgInfo_t *pSmsMsgInfo, unsigned int MoreMsgToSend, int *pRequestId)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;
	int emergency_mode = 0;

	if (vconf_get_int("db/telephony/emergency", &emergency_mode) != 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL]GET db/telephony/emergency");
		return TAPI_API_OPERATION_FAILED;
	}
	if (emergency_mode) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "emergency mode on");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(pSmsMsgInfo ,TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pSmsMsgInfo, sizeof(TelSmsMsgInfo_t));
	g_array_append_vals(in_param2, &MoreMsgToSend, sizeof(unsigned int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_SEND_EX, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 *
 * This function enables the applications to check the device status
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 *
 * @param[out]	Device status whether Ready or Not Ready
 * @Interface		Synchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_check_sms_device_status(int *pDeviceStatus)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(pDeviceStatus, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_DEVICESTATUS,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pDeviceStatus = g_array_index(out_param3, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

/**
 *
 * This function enables to application to set that the device is ready to recieve messages from network.
 *
 * @return		Returns appropriate error code. Refer TapiResult_t .
 * @param[in]		None
 *
 * @param[out]	None
 * @Interface		Synchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_set_sms_device_status(void)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_INIT_PARAMS();
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETTEXT, TAPI_CS_NETTEXT_DEVICEREADY,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

//eof
