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
#include "ITapiGps.h"
#include "TelDefines.h"
#include "tapi_proxy_internal.h"

EXPORT_API int tel_init_gps(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_INIT,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_deinit_gps(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_DEINIT,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_set_gps_prefix_info(tapi_gps_fix_request_msg_t *type)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (!type)
	{
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter gps_session_params as in_param1*/
	g_array_append_vals(in_param1, type, sizeof(tapi_gps_fix_request_msg_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_FIX_REQUEST,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

    return api_err;
}

EXPORT_API int tel_stop_gps_session(void)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_STOP_SESSION,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_gps_parameters(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_GET_PARAMETERS,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_set_gps_parameters(tapi_gps_param_t *gps_params)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (!gps_params) {
		return TAPI_API_INVALID_PTR;
	}

	/*
	if(!tapi_gps_check_param(gps_params))
		return TAPI_API_INVALID_INPUT;
	*/

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter gps_session_params as in_param1*/
	g_array_append_vals(in_param1, gps_params, sizeof(tapi_gps_param_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_SET_PARAMETERS,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_confirm_gps_data_connection(tapi_gps_data_conn_cnf_t *pdp_cnf)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (!pdp_cnf)
	{
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter time_info as in_param1*/
	g_array_append_vals(in_param1, pdp_cnf, sizeof(tapi_gps_data_conn_cnf_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_AGPS_PDP_CONN_CNF,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_confirm_gps_dns_lookup(tapi_gps_dns_lookup_cnf_t *dns_cof)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");
	if (!dns_cof) {
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter time_info as in_param1*/
	g_array_append_vals(in_param1, dns_cof, sizeof(tapi_gps_dns_lookup_cnf_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_AGPS_DNS_LOOKUP_CNF,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);
	return api_err;
}

EXPORT_API int tel_confirm_gps_verification(tapi_gps_verification_cnf_t result)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (result == TAPI_GPS_VER_CNF_NO||result == TAPI_GPS_VER_CNF_YES||result == TAPI_GPS_VER_CNF_NORESPONSE) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
							   out_param1,out_param2,out_param3,out_param4);

		/* Passing parameter gps_session_params as in_param1*/
		g_array_append_vals(in_param1, &result, sizeof(tapi_gps_verification_cnf_t));

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_SEND_VER_CNF,
							 in_param1,in_param2,in_param3,in_param4,
							 &out_param1,&out_param2,&out_param3,&out_param4);

		if (ret) {
			api_err = g_array_index(out_param1, int, 0);
		}
		else {
			api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						      out_param1,out_param2,out_param3,out_param4);
	}
	else {
		api_err = TAPI_API_INVALID_INPUT;
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"##### Received Invalid Input ####");
	}

	return api_err;
}

EXPORT_API int tel_init_gps_xtra(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_XTRA_INIT,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_deinit_gps_xtra(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_XTRA_DEINIT,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_enable_gps_xtra(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_XTRA_ENABLE,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_disable_gps_xtra(void)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_XTRA_DISABLE,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_set_gps_xtra_time(tapi_gps_xtra_time_info_t *time_info)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (!time_info)	{
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter time_info as in_param1*/
	g_array_append_vals(in_param1, time_info, sizeof(tapi_gps_xtra_time_info_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_XTRA_SET_TIMEINFO,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_set_gps_xtra_data(tapi_gps_xtra_data_info_t *data_info)
{
	TS_BOOL	ret = FALSE;
	int		api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (!data_info)
	{
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter time_info as in_param1*/
	g_array_append_vals(in_param1, data_info, sizeof(tapi_gps_xtra_data_info_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_XTRA_SET_DATA,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_confirm_gps_measure_pos(tapi_gps_measure_position_confirm_t *data)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (!data) {
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter time_info as in_param1*/
	g_array_append_vals(in_param1, data, sizeof(tapi_gps_measure_position_confirm_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_MEASURE_POS_CNF,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_set_gps_frequency_aiding(unsigned char state)
{
	TS_BOOL	ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	if (state != 1 && state != 0) {
		return TAPI_API_INVALID_PTR;
	}

	/*	Check if D-Bus service is available. If Failed then return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	/* Passing parameter time_info as in_param1*/
	g_array_append_vals(in_param1, &state, sizeof(unsigned char));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_GPS,TAPI_CS_GPS_SET_FREQ_AIDING,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);
	if (ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}
