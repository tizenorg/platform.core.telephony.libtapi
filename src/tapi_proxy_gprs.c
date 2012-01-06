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

#include "tel_cs_conn.h"
#include "ITapiPS.h"
#include "TapiUtility.h"
#include "TelUtility.h"
#include "tapi_proxy.h"

extern tapi_dbus_connection_name conn_name;

/**
 * Sends a request to Telephony Server to set pdp context define info, pdp activation
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]		tapi_ps_net_start_req_t
 * @param[out]	None
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 */
EXPORT_API int tel_activate_gprs_pdp(const tapi_ps_net_start_req_t *net_start_req_info, int *pRequestID)
{
	TS_BOOL ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_RETURN_VAL_IF_FAIL((pRequestID&&net_start_req_info) , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/* cont_id: 1,2,..MAX_PDP_CONTEXTS_ALLOWED */
	if (net_start_req_info->cont_id <= 0 || net_start_req_info->cont_id > MAX_PDP_CONTEXTS_ALLOWED) {
		ret_val = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_ERR, " Invalid GPRS context id");
		return ret_val;
	}

	if (net_start_req_info->pdp_info.auth_type >= TAPI_PDP_AUTH_NONE
			&& net_start_req_info->pdp_info.auth_type < TAPI_PDP_AUTH_MAX) {
		TAPI_GLIB_INIT_PARAMS();

		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1, out_param2,out_param3,out_param4);
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Connection Name=%s",conn_name.name);

		g_array_append_vals(in_param1, net_start_req_info, sizeof(tapi_ps_net_start_req_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		ret = tapi_send_request(TAPI_CS_SERVICE_GPRS, TAPI_CS_GPRS_STARTNETWORK, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			ret_val = g_array_index(out_param1, int, 0);
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_DEBUG,"proxy error = %d ",ret_val);
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		ret_val = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid PTR  =%d ",ret_val);
	}

	return ret_val;
}

/**
 * Sends  to Telephony Server to deactivate the pdp service
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]	tapi_ps_net_stop_req_t
 * @param[out]	None
 * @Interface	Asynchronous.
 * @remark
 * @Refer		tel_activate_gprs_pdp
 *
 */
EXPORT_API int tel_deactivate_gprs_pdp(const tapi_ps_net_stop_req_t *net_stop_req_info, int *pRequestID)
{
	TS_BOOL ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_RETURN_VAL_IF_FAIL((pRequestID&&net_stop_req_info) , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if (net_stop_req_info->pdp_info.auth_type >= TAPI_PDP_AUTH_NONE
			&& net_stop_req_info->pdp_info.auth_type < TAPI_PDP_AUTH_MAX) {
		TAPI_GLIB_INIT_PARAMS();

		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, net_stop_req_info, sizeof(tapi_ps_net_stop_req_t));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		ret = tapi_send_request(TAPI_CS_SERVICE_GPRS, TAPI_CS_GPRS_PDP_DEACTIVATION_SET, in_param1, in_param2,
				in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			ret_val = g_array_index(out_param1, int, 0);
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_DEBUG,"Proxy error =%d ",ret_val);
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		ret_val = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Invalid PTR  =%d ",ret_val);
	}

	return ret_val;
}

/**
 * Sends  to Telephony Server to know whether Signal line is on or net
 *
 * @return		TAPI_API_SUCCESS on success and one of the enum types of  TapiResult_t on failure
 * @param[in]	tapi_ps_btdun_pincontrol
 * @param[out]	None
 * @Interface	Asynchronous.
 * @remark		This means that the MS is ready to start Data Service using DPRAM.
 * @Refer
 *
 */
EXPORT_API int tel_control_gprs_btdun_pin(tapi_ps_btdun_pincontrol pincontrol, int *pRequestID)
{
	TS_BOOL ret;
	int ret_val = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_RETURN_VAL_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	switch (pincontrol.signal) {
		case GPRS_SIGNAL_DCD:
		case GPRS_DATA_SIGNAL_DTR:
		case GPRS_DATA_SIGNAL_DSR:
		case GPRS_DATA_SIGNAL_RTS:
		case GPRS_DATA_SIGNAL_CTS:
		case GPRS_DATA_SIGNAL_RI:
			break;

		default:
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "invalid signal(%d:0x%x)", pincontrol.signal, pincontrol.signal);
			ret_val = TAPI_API_INVALID_INPUT;
	}

	switch (pincontrol.status) {
		case GPRS_SIGNAL_STATUS_OFF:
		case GPRS_SIGNAL_STATUS_ON:
			break;

		default:
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "invalid status(%d:0x%x)", pincontrol.status, pincontrol.status);
			ret_val = TAPI_API_INVALID_INPUT;
	}

	if (ret_val != TAPI_API_SUCCESS) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "failed. ret_val = %d", ret_val);
		return ret_val;
	}

	if ((pincontrol.signal >= GPRS_SIGNAL_DCD && pincontrol.signal <= GPRS_DATA_SIGNAL_RI)
			&& (pincontrol.status >= GPRS_SIGNAL_STATUS_OFF && pincontrol.status <= GPRS_SIGNAL_STATUS_ON)) {
		TAPI_GLIB_INIT_PARAMS();

		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &pincontrol, sizeof(tapi_ps_btdun_pincontrol));
		g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

		ret = tapi_send_request(TAPI_CS_SERVICE_GPRS, TAPI_CS_GPRS_BTDUN_PINCTRL_GET, in_param1, in_param2, in_param3,
				in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			ret_val = g_array_index(out_param1, int, 0);
			*pRequestID = g_array_index(out_param2, int, 0);
		}
		else {
			ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
			TAPI_LIB_DEBUG(LEVEL_DEBUG,"Proxy error =%d ",ret_val);
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}

	return ret_val;
}
