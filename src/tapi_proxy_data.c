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
#include "TapiUtility.h"
#include "ITapiData.h"
#include "TelUtility.h"
#include "tapi_proxy_internal.h"

EXPORT_API int tel_get_data_mobile_ipnai (TelDataNAIIndex_t *pMipNaiIdx, unsigned short *pParamMask, int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL((pMipNaiIdx && pParamMask && pRequestID ), TAPI_API_INVALID_PTR);

	if(*pMipNaiIdx < TAPI_DATA_FACTORY_NAI || *pMipNaiIdx > TAPI_DATA_CUSTOM_NAI)
		return TAPI_API_INVALID_INPUT;

	if(*pParamMask == 0)
		return TAPI_API_INVALID_INPUT;

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
						   out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, pMipNaiIdx, sizeof(TelDataNAIIndex_t));
	g_array_append_vals(in_param2, pParamMask, sizeof(unsigned short));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_MOBILE_IP_NAI_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_set_data_mobile_ipnai(TelDataMipNaiParam_t *pMipNaiParam, int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL((pMipNaiParam && pRequestID ), TAPI_API_INVALID_PTR);

	if(pMipNaiParam->naiIndex < TAPI_DATA_FACTORY_NAI || pMipNaiParam->naiIndex  > TAPI_DATA_CUSTOM_NAI)
		return TAPI_API_INVALID_INPUT;

	if(pMipNaiParam->paramMask == 0)
		return TAPI_API_INVALID_INPUT;

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pMipNaiParam, sizeof(TelDataMipNaiParam_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_MOBILE_IP_NAI_SET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_get_data_current_nai_index (int *pRequestID)
{
	TS_BOOL ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_CURRENT_NAI_INDEX_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_get_data_te2_status (int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_TE2_STATUS_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_get_data_roam_guard (int *pRequestID)
{
	TS_BOOL ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_ROAM_GUARD_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_set_data_roam_guard (TelDataRoamGuardMode_t *pRoamGuard, int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL((pRoamGuard && pRequestID), TAPI_API_INVALID_PTR);

	if(*pRoamGuard < TAPI_DATA_ROAM_GUARD_DEFAULT || *pRoamGuard > TAPI_DATA_ROAM_ROAM_GUARD_ON)
		return TAPI_API_INVALID_INPUT;

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pRoamGuard, sizeof(TelDataRoamGuardMode_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_ROAM_GUARD_SET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_get_data_nai(int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_MODEM_NAI_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_set_data_nai(TelDataModemNAIMode_t *pModemNai, int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL((pModemNai && pRequestID), TAPI_API_INVALID_PTR);

	if(*pModemNai < TAPI_DATA_MODEM_NAI_OFF || *pModemNai > TAPI_DATA_MODEM_NAI_ON)
		return TAPI_API_INVALID_INPUT;

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pModemNai, sizeof(TelDataModemNAIMode_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_MODEM_NAI_SET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_get_data_evdo_rev_config(int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret=tapi_send_request_internal(	TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_EVDO_REV_CONFIG_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_set_data_evdo_rev_config (TelDataRevConfig_t *pRevConfig, int *pRequestID)
{
	TS_BOOL	ret;
	int ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL((pRevConfig &&pRequestID), TAPI_API_INVALID_PTR);

	if(*pRevConfig < TAPI_DATA_REVA || *pRevConfig > TAPI_DATA_REV0)
		return TAPI_API_INVALID_INPUT;

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pRevConfig, sizeof(TelDataRevConfig_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_EVDO_REV_CONFIG_SET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_restore_data_nai(int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret=tapi_send_request_internal(	TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_RESTORE_NAI,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_get_data_ddtm_config (int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL(pRequestID, TAPI_API_INVALID_PTR);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_DDTM_CONFIGURATION_GET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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

EXPORT_API int tel_set_data_ddtm_config(TelDataDDTMStatus_t *pDDTMStatus, int *pRequestID)
{
	TS_BOOL	ret;
	int	ret_val;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_RET_ERR_NUM_IF_FAIL((pDDTMStatus && pRequestID), TAPI_API_INVALID_PTR);

	if(*pDDTMStatus < TAPI_DATA_DDTM_MODE_OFF || *pDDTMStatus > TAPI_DATA_DDTM_MODE_ON)
		return TAPI_API_NOT_SUPPORTED;

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pDDTMStatus, sizeof(TelDataDDTMStatus_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_DATA,TAPI_CS_DATA_DDTM_CONFIGURATION_SET,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

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
