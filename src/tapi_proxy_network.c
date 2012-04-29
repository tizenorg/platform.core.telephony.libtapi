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
#include "tapi_proxy.h"
#include "ITapiNetwork.h"
#include "TelNetwork.h"
#include "TapiUtility.h"

#include <vconf.h>

extern tapi_dbus_connection_name conn_name;

static int _tel_check_tapi_state()
{
	int vconf_ret = 0;
	int tapi_state = 0;

	vconf_ret = vconf_get_int(VCONFKEY_TELEPHONY_TAPI_STATE, (int*) &tapi_state);

	if (vconf_ret != 0 || tapi_state !=1) {
		TAPI_LIB_DEBUG(LEVEL_ALERT,"FAIL TO GET VCONFKEY_TELEPHONY_TAPI_STATE or SERVICE NOT READY");
		return TAPI_API_SERVICE_NOT_READY;
	}
	return 0;
}

EXPORT_API int tel_select_network_automatic(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SELECT_AUTOMATIC, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_select_network_manual(unsigned int Plmn, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &Plmn, sizeof(unsigned int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SELECT_MANUAL, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_search_network(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SEARCH, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_get_network_selection_mode(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETSELECTIONMODE, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_set_network_preferred_plmn(
		TelNetworkPrefferedPlmnOp_t Operation,
		TelNetworkPrefferedPlmnInfo_t *pPreffPlmnInfo,
		int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance : Operation[%d]", Operation);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	TAPI_RET_ERR_NUM_IF_FAIL(pPreffPlmnInfo ,TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((Operation > TAPI_NETWORK_PREF_PLMN_DELETE) || (Operation < TAPI_NETWORK_PREF_PLMN_ADD)
			|| (pPreffPlmnInfo->SystemType > TAPI_NETWORK_SYSTEM_HSDPA) || (pPreffPlmnInfo->SystemType
			<= TAPI_NETWORK_SYSTEM_NO_SRV)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "INVALID INPUT [%d],[%d]", Operation, pPreffPlmnInfo->SystemType);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &Operation, sizeof(TelNetworkPrefferedPlmnOp_t));
	g_array_append_vals(in_param2, pPreffPlmnInfo, sizeof(TelNetworkPrefferedPlmnInfo_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETPREFPLMN, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_get_network_preferred_plmn(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETPREFPLMN, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_get_network_band(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETNETWORKBAND, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_set_network_band(TelNetworkBandPreferred_t BandMode, TelNetworkBand_t Band, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance Band Mode[%d] Band[%d]",BandMode, Band);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((BandMode > TAPI_NETWORK_BAND_MODE_ONLY) || (BandMode < TAPI_NETWORK_BAND_MODE_PREFERRED) || (Band
			> TAPI_NETWORK_BAND_TYPE_GSM_850_1900) || (Band < TAPI_NETWORK_BAND_TYPE_ANY)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "INVALID INPUT [%d],[%d]", BandMode, Band);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &BandMode, sizeof(TelNetworkBandPreferred_t));
	g_array_append_vals(in_param2, &Band, sizeof(TelNetworkBand_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETNETWORKBAND, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_set_network_service_domain(TelNetworkServiceDomain_t ServiceDomain, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance : Service Domain[%d]", ServiceDomain);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((ServiceDomain > TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC) || (ServiceDomain
			< TAPI_NETWORK_SERVICE_DOMAIN_COMBINED)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "INVALID INPUT [%d]", ServiceDomain);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &ServiceDomain, sizeof(TelNetworkServiceDomain_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETSERVICEDOMAIN, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, TapiResult_t, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_network_service_domain(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETSERVICEDOMAIN, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_set_network_mode(TelNetworkMode_t NwMode, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance : Network Mode[%d]", NwMode);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((NwMode > TAPI_NETWORK_MODE_WLAN) || (NwMode <= TAPI_NETWORK_MODE_NONE)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "INVALID INPUT [%d]", NwMode);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter network_mode as in_param1*/
	g_array_append_vals(in_param1, &NwMode, sizeof(TelNetworkMode_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETNETWORKMODE, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_get_network_mode(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETNETWORKMODE, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_set_network_roaming(TelNetworkRoamingMode_t *RoamingMode, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(RoamingMode , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((RoamingMode->pref_net_type > TAPI_NETWORK_PREF_NET_TYPE_ROAM_ONLY) || (RoamingMode->pref_net_type
			< TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter service_domain as in_param1*/
	g_array_append_vals(in_param1, RoamingMode, sizeof(TelNetworkRoamingMode_t));

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETROAMINGMODE, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_get_network_roaming(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETROAMINGMODE, in_param1, in_param2, in_param3,
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

EXPORT_API int tel_set_network_hybrid_in_cdma(TelNetworkCDMAHybridMode_t CDMAHybridMode, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance : Hybrid Mode[%d]", CDMAHybridMode);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((CDMAHybridMode > TAPI_NETWORK_CDMA_EVDO_ONLY) || (CDMAHybridMode < TAPI_NETWORK_CDMA_HYBRID)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &CDMAHybridMode, sizeof(TelNetworkCDMAHybridMode_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETCDMAHYBRIDMODE, in_param1, in_param2,
			in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

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

EXPORT_API int tel_get_network_hybrid_in_cdma(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId , TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
			out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETCDMAHYBRIDMODE, in_param1, in_param2,
			in_param3, in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

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

EXPORT_API int tel_set_network_acquisition_order(TelNetworkMode_t *pPrefNwOrder, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;
	tapi_dbus_connection_name app_name;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pPrefNwOrder, TAPI_API_INVALID_PTR);

	tel_get_app_name(&app_name);
	if (app_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	/* Passing parameter network_order as in_param1*/
	g_array_append_vals(in_param1, pPrefNwOrder, (sizeof(TelNetworkMode_t) * TAPI_NETWORK_ORDER_MAX));
	g_array_append_vals(in_param4, &app_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETNETWORKORDER,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_get_network_acquisition_order(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;
	tapi_dbus_connection_name app_name;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	tel_get_app_name(&app_name);
	if (app_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param4, &app_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETNETWORKORDER,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_set_network_power_on_attach(TelNetworkPowerOnAttach_t PowerOnAttach, int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;
	tapi_dbus_connection_name app_name;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance Power On Attach[%d]", PowerOnAttach);
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	tel_get_app_name(&app_name);
	if (app_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ((PowerOnAttach > TAPI_NETWORK_POWERON_ATTACH_ENABLE) || (PowerOnAttach < TAPI_NETWORK_POWERON_ATTACH_DISABLE)) {
		TAPI_LIB_DEBUG(LEVEL_ALERT, "INVALID INPUT [%d]", PowerOnAttach);
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param1, &PowerOnAttach, sizeof(TelNetworkPowerOnAttach_t));
	g_array_append_vals(in_param4, &app_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_SETPOWERONATTACH,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_get_network_power_on_attach(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;
	tapi_dbus_connection_name app_name;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	tel_get_app_name(&app_name);
	if (app_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param4, &app_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_GETPOWERONATTACH,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_cancel_network_manual_search(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;
	tapi_dbus_connection_name app_name;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	tel_get_app_name(&app_name);
	if (app_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param4, &app_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_CANCELMANUALSEARCH,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}

EXPORT_API int tel_cancel_network_manual_selection(int *pRequestId)
{
	int api_err = TAPI_API_SUCCESS;
	TS_BOOL ret;
	tapi_dbus_connection_name app_name;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Func Entrance ");
	if (_tel_check_tapi_state() != 0)
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_RETURN_VAL_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);

	tel_get_app_name(&app_name);
	if (app_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	g_array_append_vals(in_param4, &app_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_NETWORK, TAPI_CS_NETWORK_CANCELMANUALSELECTION,
			in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		api_err	= g_array_index(out_param1, int, 0);
		*pRequestId = g_array_index(out_param2, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4, out_param1, out_param2, out_param3, out_param4);

	return api_err;
}
