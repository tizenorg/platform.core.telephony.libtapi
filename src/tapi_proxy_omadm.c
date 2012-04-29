/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
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

#include "tel_cs_conn.h"
#include "TapiUtility.h"
#include "ITapiOmadm.h"
#include "TelUtility.h"
#include "tapi_proxy_internal.h"

EXPORT_API int tel_get_omadm_prl_size(int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_OMADM, TAPI_CS_OMADM_PRL_SIZE_GET,
					   in_param1, in_param2, in_param3, in_param4,
					   &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		ret_val = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%d], Reuest ID [%d]",ret_val, *pRequestID);

	return ret_val;
}

EXPORT_API int tel_get_omadm_model_name(int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_OMADM, TAPI_CS_OMADM_MODEL_NAME_GET,
					   in_param1, in_param2, in_param3, in_param4,
					   &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		ret_val = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%d], Reuest ID [%d]",ret_val, *pRequestID);

	return ret_val;
}

EXPORT_API int tel_get_omadm_oem_name(int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_OMADM, TAPI_CS_OMADM_OEM_NAME_GET,
					   in_param1, in_param2, in_param3, in_param4,
					   &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		ret_val = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%d], Reuest ID [%d]",ret_val, *pRequestID);

	return ret_val;
}

EXPORT_API int tel_get_omadm_sw_ver(int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_OMADM, TAPI_CS_OMADM_SW_VER_GET,
					   in_param1, in_param2, in_param3, in_param4,
					   &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		ret_val = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%d], Reuest ID [%d]",ret_val, *pRequestID);

	return ret_val;
}

EXPORT_API int tel_read_omadm_ril(int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_OMADM, TAPI_CS_OMADM_PRL_READ_GET,
					   in_param1, in_param2, in_param3, in_param4,
				   &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		ret_val = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%d], Reuest ID [%d]",ret_val, *pRequestID);

	return ret_val;
}

EXPORT_API int  tel_write_omadm_prl(TelOMADMPRLData_t *pPrlData, int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pPrlData, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pPrlData, sizeof(TelOMADMPRLData_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_OMADM, TAPI_CS_OMADM_PRL_WRITE_SET,
					   in_param1, in_param2, in_param3, in_param4,
					   &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		/*	Get the API error value as out param 1, from the server.	*/
		ret_val = g_array_index(out_param1, int, 0);

		/*	Get the Request ID as out param 2, from the server.    */
		*pRequestID = g_array_index(out_param2, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"RPC link is down");
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%d], Reuest ID [%d]",ret_val, *pRequestID);

	return ret_val;
}
