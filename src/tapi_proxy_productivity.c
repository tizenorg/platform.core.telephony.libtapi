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
#include "ITapiProductivity.h"
#include "TapiUtility.h"
#include "TelErr.h"
#include "tapi_proxy_internal.h"

/**********************************************************************************************
 *
 *                      NEW API
 *
***********************************************************************************************/


/**********************************************************************************************
 *
 * This function is used to start the service mode.
 *
 * @param	svc_mode    tapi_service_mode_t , test_sub     tapi_test_mode_sub_t
 * @return	enum type TapiResult_t
 * @remark	NONE
 *
***********************************************************************************************/
EXPORT_API int tel_start_svcmode(tapi_service_mode_t svc_mode, tapi_test_mode_sub_t test_sub)
{
	TS_BOOL	ret;
	int	ret_val = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &svc_mode, sizeof(tapi_service_mode_t));
	g_array_append_vals(in_param2, &test_sub, sizeof(tapi_test_mode_sub_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SVCMODE,TAPI_CS_SVCMODE_ENTERSVCMODE,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
			ret_val = g_array_index(out_param1, int, 0);
	}
	else {
			ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return ret_val;
}

/**********************************************************************************************
 *
 * This function is used to end the service mode.
 *
 * @param	svc_mode    tapi_service_mode_t
 * @return	enum type TapiResult_t
 * @remark	NONE
 *
***********************************************************************************************/
EXPORT_API int tel_end_svcmode(tapi_service_mode_t  svc_mode)
{
	TS_BOOL ret;
	int	ret_val = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &svc_mode, sizeof(tapi_service_mode_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SVCMODE,TAPI_CS_SVCMODE_ENDSVCMODE,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
			ret_val = g_array_index(out_param1, int, 0);
	}
	else {
			ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return ret_val;
}

/**********************************************************************************************
 *
 * This function is used to process the code enterred when service mode is running.
 *
 * @param	key_code  char type
 * @return	enum type TapiResult_t
 * @remark	NONE
 *
***********************************************************************************************/
EXPORT_API int tel_send_svcmode_keycode(char key_code)
{
	TS_BOOL	ret;
	int	ret_val = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &key_code, sizeof(char));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SVCMODE,TAPI_CS_SVCMODE_PROCESS_KEYCODE,
						in_param1,in_param2,in_param3,in_param4,
						&out_param1,&out_param2,&out_param3,&out_param4);

	if (ret) {
			ret_val = g_array_index(out_param1, int, 0);
	}
	else {
			ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return ret_val;
}

/**********************************************************************************************
 *
 * This function is invoked to dump cp debug.
 *
 * @param	tapi_service_mode_debug_cp_dump_t [in] type  - cp dump type. refer #tapi_service_mode_debug_cp_dump_t
 * @return	Return Type (int) \n
 *		- Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 *		- Negative integer : it provides an error code (Refer #TapiResult_t)
 * @remark	NONE
 *
***********************************************************************************************/
EXPORT_API int tel_request_svcmode_dump(tapi_service_mode_debug_cp_dump_t type)
{
	TS_BOOL ret = FALSE;
	int ret_val = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1, in_param2, in_param3, in_param4,
				   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &type, sizeof(tapi_service_mode_debug_cp_dump_t));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_SVCMODE, TAPI_CS_SVCMODE_DEBUG_DUMP,
					in_param1, in_param2, in_param3, in_param4,
					&out_param1, &out_param2, &out_param3, &out_param4);

	if (ret) {
		ret_val = g_array_index(out_param1, int, 0);
	}
	else {
		ret_val = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1, in_param2, in_param3, in_param4,
				  out_param1, out_param2, out_param3, out_param4);

	return ret_val;
}

/**
 * Process the starting Factory Process.
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_start_imei_process(void)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_IMEI,TAPI_CS_IMEI_STARTFACTORYPROCESS,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

/**
 * Process the tapi_imei_CompareItemIND
 *
 * @return		TapiResult_t		API result code. Either Success or some Error Code.
 * @param[in]
 * @param[out]
 * @Changes
 */
EXPORT_API int tel_compare_imei_item(unsigned char nItemID, unsigned char bResult)
{
	TS_BOOL	ret = FALSE;
	int	api_err = TAPI_API_SUCCESS;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance ");

	TAPI_GLIB_INIT_PARAMS();

	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_NOT_EST);

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
						   out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &nItemID, sizeof(unsigned char));
	g_array_append_vals(in_param2, &bResult, sizeof(unsigned char));

	ret = tapi_send_request_internal(TAPI_CS_SERVICE_IMEI,TAPI_CS_IMEI_COMPARE_ITEM_IND,
						 in_param1,in_param2,in_param3,in_param4,
						 &out_param1,&out_param2,&out_param3,&out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int, 0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
						  out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_factory_omission_avoidance(tapi_factory_omission_avoidance_read_cmd_t cmd)
{
	int ret = 0;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	/*	Check input range for tele service type and call barring flavour	*/
	if ((cmd >= TAPI_FACTORY_OMISSION_AVOIDANCE_ITEM_READ)
			&& (cmd <= TAPI_FACTORY_OMISSION_AVOIDANCE_FAILDATA_READ_FULL)) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, &cmd, sizeof(tapi_factory_omission_avoidance_read_cmd_t));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func");

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_FACTORY, TAPI_CS_FACTORY_OMISSION_AVOIDANCE_GET,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			ret = g_array_index(out_param1, int, 0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			ret = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		/*	Invalid input range given by the APP.	*/
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		ret = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", ret);

	return ret;
}

EXPORT_API int tel_set_factory_omission_avoidance(tapi_factory_omission_avoidance_info_t *info)
{
	int ret = 0;

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Func Entrance");

	/*	Check input range for tele service type and call barring flavour	*/
	if ((info->cmd.write_cmd >= TAPI_FACTORY_OMISSION_AVOIDANCE_ITEM_WRITE)
			&& (info->cmd.write_cmd <= TAPI_FACTORY_OMISSION_AVOIDANCE_FAILDATA_WRITE_FULL)) {
		/*	Check if D-Bus service is available. If Failed then return FALSE	*/
		TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status_internal(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
		TAPI_GLIB_INIT_PARAMS();

		TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);

		g_array_append_vals(in_param1, info, sizeof(tapi_factory_omission_avoidance_info_t));

		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Calling Call Back func");

		ret = tapi_send_request_internal(TAPI_CS_SERVICE_FACTORY, TAPI_CS_FACTORY_OMISSION_AVOIDANCE_SET,
				in_param1, in_param2, in_param3, in_param4,
				&out_param1, &out_param2, &out_param3, &out_param4);

		if (ret) {
			/*	Get the API error value as out param 1, from the server.	*/
			ret = g_array_index(out_param1, int, 0);
		}
		else {
			/*	RPC API failed, return err to APP	*/
			TAPI_LIB_DEBUG(LEVEL_ALERT, "RPC Link is down");
			ret = TAPI_API_SYSTEM_RPC_LINK_DOWN;
		}

		TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,
				out_param1,out_param2,out_param3,out_param4);
	}
	else {
		/*	Invalid input range given by the APP.	*/
		TAPI_LIB_DEBUG(LEVEL_ERR,"Received Invalid Input ");
		ret = TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "Returning TapiResult_t [%+d]", ret);

	return ret;
}

//eof
