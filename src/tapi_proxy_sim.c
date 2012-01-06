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

#include "tel_cs_conn.h"
#include "tapi_proxy.h"
#include "TelDefines.h"
#include "TelUtility.h"
#include "TelSim.h"
#include "TapiUtility.h"

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

EXPORT_API int tel_get_sim_init_info(TelSimCardStatus_t *sim_status, int *card_changed)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;
	int vconf_ret = 0;
	TelSimCardStatus_t CardStatus = 0;

	TAPI_RETURN_VAL_IF_FAIL(sim_status, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	/* for reducing dbus connection request on boot time - start */
	vconf_ret = vconf_get_int(VCONFKEY_TELEPHONY_SIM_CHV, (int*) &CardStatus);

	if (vconf_ret != 0) {
		TAPI_LIB_DEBUG(LEVEL_ALERT,"FAIL TO GET VCONFKEY_TELEPHONY_SIM_CHV ");
		return TAPI_API_SERVICE_NOT_READY;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT,"VCONFKEY_TELEPHONY_SIM_CHV api ret[%d], return value[%d] ",vconf_ret,CardStatus);

	if (CardStatus != TAPI_SIM_STATUS_SIM_INIT_COMPLETED) {
		*sim_status = CardStatus;

		if (card_changed != NULL)
			*card_changed = 0;

		return api_err;
	}
	/* for reducing dbus connection request on boot time - end */
	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETSIMINITINFO, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*sim_status = g_array_index(out_param2, TelSimCardStatus_t ,0);

		if (card_changed != NULL)
			*card_changed = g_array_index(out_param3, int ,0);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_verifiy_sim_pins(const TelSimSecPw_t *pin_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pin_data , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pin_data->pw , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Type [%d]", pin_data->type);

	if (pin_data->type != TAPI_SIM_PTYPE_PIN1 && pin_data->type != TAPI_SIM_PTYPE_PIN2 && pin_data->type
			!= TAPI_SIM_PTYPE_SIM) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Code [%s]", pin_data->pw);
	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Code length[%d]", pin_data->pw_len);

	if (((pin_data->type == TAPI_SIM_PTYPE_PIN1) || (pin_data->type == TAPI_SIM_PTYPE_PIN2) || (pin_data->type
			== TAPI_SIM_PTYPE_SIM)) && ((pin_data->pw_len < 4) || (pin_data->pw_len > 8))) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pin_data, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param2, pin_data->pw, pin_data->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_VERIFYSEC, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_verify_sim_puks(const TelSimSecPw_t *puk_data, const TelSimSecPw_t *new_pin_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL((puk_data != NULL && new_pin_data != NULL), TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL((puk_data->pw != NULL && new_pin_data->pw != NULL), TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Unblock PIN Type [%d]", puk_data->type);
	TAPI_LIB_DEBUG(LEVEL_ALERT, "New PIN Type [%d]", new_pin_data->type);

	if ((puk_data->type != TAPI_SIM_PTYPE_PUK1 && puk_data->type != TAPI_SIM_PTYPE_PUK2) || (new_pin_data->type
			!= TAPI_SIM_PTYPE_PIN1 && new_pin_data->type != TAPI_SIM_PTYPE_PIN2)) {
		return TAPI_API_INVALID_INPUT;
	}

	if ((puk_data->type != TAPI_SIM_PTYPE_PUK1 || new_pin_data->type != TAPI_SIM_PTYPE_PIN1) && (puk_data->type
			!= TAPI_SIM_PTYPE_PUK2 || new_pin_data->type != TAPI_SIM_PTYPE_PIN2)) {
		return TAPI_API_INVALID_INPUT;
	}

	//pin and puk code length validation check
	if ((puk_data->pw_len < 4) || (puk_data->pw_len > 8) || (new_pin_data->pw_len < 4) || (new_pin_data->pw_len > 8)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Unblock PIN Code [%s]", puk_data->pw);
	TAPI_LIB_DEBUG(LEVEL_ALERT, "New PIN Code [%s]", new_pin_data->pw);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, puk_data, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param1, new_pin_data, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param2, puk_data->pw, puk_data->pw_len);
	g_array_append_vals(in_param3, new_pin_data->pw, new_pin_data->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_VERIFYPUK, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_change_sim_pins(const TelSimSecPw_t *old_pin, const TelSimSecPw_t *new_pin, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL((old_pin != NULL && new_pin != NULL) , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL((old_pin->pw != NULL && new_pin->pw != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "old_pin->type[%d], new_pin->type[%d]",old_pin->type, new_pin->type);

	if ((old_pin->type != TAPI_SIM_PTYPE_PIN1) && (old_pin->type != TAPI_SIM_PTYPE_PIN2) && (new_pin->type
			!= TAPI_SIM_PTYPE_PIN1) && (new_pin->type != TAPI_SIM_PTYPE_PIN2)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (old_pin->type != new_pin->type) {
		return TAPI_API_INVALID_INPUT;
	}

	//Old Pin code length check
	if ((old_pin->pw_len < 4) || (old_pin->pw_len > 8)) {
		return TAPI_API_INVALID_INPUT;
	}

	//New Pin code length check
	if ((new_pin->pw_len < 4) || (new_pin->pw_len > 8)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Old PIN Code [%s]", old_pin->pw);
	TAPI_LIB_DEBUG(LEVEL_ALERT, "New PIN Code [%s]", new_pin->pw);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, old_pin, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param1, new_pin, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param2, old_pin->pw, sizeof(unsigned char) * (old_pin->pw_len));
	g_array_append_vals(in_param3, new_pin->pw, sizeof(unsigned char) * new_pin->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_CHANGEPIN, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_security_status(TelSimPinType_t type, TelSimPinStatus_t *status)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(status, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	if ((type != TAPI_SIM_PTYPE_PIN1) && (type != TAPI_SIM_PTYPE_PIN2) && (type != TAPI_SIM_PTYPE_SIM)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &type, sizeof(TelSimPinType_t));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETSECSTATUS, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*status = g_array_index(out_param2,TelSimPinStatus_t ,0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "api_err[%d],PIN type[%d], Status[%d]",api_err, type,*status);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_disable_sim_security(TelSimSecPw_t *sec_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((sec_data != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(sec_data->pw, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Type [%d]", sec_data->type);

	if ((sec_data->type != TAPI_SIM_PTYPE_PIN1) && (sec_data->type != TAPI_SIM_PTYPE_SIM)) {
		return TAPI_API_INVALID_INPUT;
	}

	if ((sec_data->pw_len < 4) || (sec_data->pw_len > 8)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Code [%s]", sec_data->pw);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, sec_data, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param2, sec_data->pw, sec_data->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_DISABLESEC, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_enable_sim_security(TelSimSecPw_t *sec_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((sec_data != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(sec_data->pw , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Type [%d]", sec_data->type);

	if ((sec_data->type != TAPI_SIM_PTYPE_PIN1) && (sec_data->type != TAPI_SIM_PTYPE_SIM)) {
		return TAPI_API_INVALID_INPUT;
	}

	if ((sec_data->pw_len < 4) || (sec_data->pw_len > 8)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Code [%s]", sec_data->pw);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, sec_data, sizeof(TelSimSecPw_t));
	g_array_append_vals(in_param2, sec_data->pw, sec_data->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_ENABLESEC, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_personalization_status(TelSimPersType_t type, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	if ((type != TAPI_SIM_PERS_NET) && (type != TAPI_SIM_PERS_NS) && (type != TAPI_SIM_PERS_SP) && (type
			!= TAPI_SIM_PERS_CP)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &type, sizeof(TelSimPersType_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETPERSSTATUS, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_disable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(pers_data, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pers_data->pw, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "PIN Type [%d]", pers_data->type);

	if ((pers_data->type != TAPI_SIM_PERS_NET) && (pers_data->type != TAPI_SIM_PERS_NS) && (pers_data->type
			!= TAPI_SIM_PERS_SP) && (pers_data->type != TAPI_SIM_PERS_CP)) {
		return TAPI_API_INVALID_INPUT;
	}

	//personalization code length check - need to fix
	if ((pers_data->pw_len < 6)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Personalization Password[%s]", pers_data->pw);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pers_data, sizeof(TelSimPersPw_t));
	g_array_append_vals(in_param2, pers_data->pw, pers_data->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_DISABLEPERS, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_enable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(pers_data, TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(pers_data->pw , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "personalization type[%d]", pers_data->type);

	if ((pers_data->type != TAPI_SIM_PERS_NET) && (pers_data->type != TAPI_SIM_PERS_NS) && (pers_data->type
			!= TAPI_SIM_PERS_SP) && (pers_data->type != TAPI_SIM_PERS_CP)) {
		return TAPI_API_INVALID_INPUT;
	}

	//personalization password length check - need to fix
	if ((pers_data->pw_len < 6)) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_ALERT, "personalization password[%s]", pers_data->pw);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pers_data, sizeof(TelSimPersPw_t));
	g_array_append_vals(in_param2, pers_data->pw, pers_data->pw_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_ENABLEPERS, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_fdn_status(int *fdn_status)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(fdn_status , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETFDNSTATUS, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*fdn_status = g_array_index(out_param2,TS_BOOL ,0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "FDN Status [%d]", *fdn_status);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);
	return api_err;
}

EXPORT_API int tel_disable_sim_fdn(const unsigned char *pin2, int *pin2_len, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((pin2 != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (*pin2_len < 4 || *pin2_len > 8) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "password [%s]", pin2);

	ret = tapi_check_dbus_status();

	if (FALSE == ret) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pin2, *pin2_len);
	g_array_append_vals(in_param2, pin2_len, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_DISABLEFDN, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_enable_sim_fdn(const unsigned char *pin2, int *pin2_len, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((pin2 != NULL && pin2_len != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (*pin2_len < 4 || *pin2_len > 8) {
		return TAPI_API_INVALID_INPUT;
	}

	TAPI_LIB_DEBUG(LEVEL_INFO, "password [%s]", pin2);

	ret = tapi_check_dbus_status();

	if (FALSE == ret) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pin2, *pin2_len);
	g_array_append_vals(in_param2, pin2_len, sizeof(int));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_ENABLEFDN, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sim_access(const TelSimRSimReq_t *rsim_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(rsim_data , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	input param validation check	*/
	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, rsim_data, sizeof(TelSimRSimReq_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	if ((rsim_data->rsim_cmd == TAPI_SIM_UPDATE_BINARY) || (rsim_data->rsim_cmd == TAPI_SIM_UPDATE_RECORD)) {
		g_array_append_vals(in_param2, rsim_data->data, sizeof(char*) * rsim_data->p3);
	}

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_RSIMACCESS, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sim_apdu(TelSimApdu_t* apdu_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(apdu_data , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	input param validation check	*/
	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, apdu_data, sizeof(TelSimApdu_t));
	g_array_append_vals(in_param2, apdu_data->apdu, apdu_data->apdu_len);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_APDU, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sim_atr(int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	input param validation check	*/
	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_ATR, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}


EXPORT_API int tel_get_sim_ecc(TelSimEccData_t *ecc_data, int *ecc_count)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((ecc_data != NULL && ecc_count != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETECCINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*ecc_data = g_array_index(out_param2, TelSimEccData_t ,0);
		*ecc_count = g_array_index(out_param3, int ,0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "ECC count [%d]", *ecc_count);
		TAPI_LIB_DEBUG(LEVEL_ALERT, "1st ECC data is  [%s]", ecc_data->EccInfo.szEcc1);
		TAPI_LIB_DEBUG(LEVEL_ALERT, "1st UECC data is  [%s]", ecc_data->UeccInfo[0].szEcc);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);
	return api_err;
}

EXPORT_API int tel_get_sim_language(TelSimLanguageInfo_t *sim_language)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(sim_language, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETLANGUAGEINFO, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*sim_language = g_array_index(out_param2, TelSimLanguageInfo_t ,0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "Language count [%lu]", sim_language->LpCount);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);
	return api_err;
}

EXPORT_API int tel_set_sim_language(TelSimLanguagePreferenceCode_t language, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	if ( language > TAPI_SIM_LP_JAPANESE )
		return TAPI_API_INVALID_INPUT;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &language, sizeof(TelSimLanguagePreferenceCode_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SETLANGUAGE, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_type(TelSimCardType_t *card_type)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(card_type, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETCARDTYPE, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*card_type = g_array_index(out_param2,int ,0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "Card type [%d]", *card_type);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_imsi(TelSimImsiInfo_t *imsi)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(imsi , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETIMSIINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*imsi = g_array_index(out_param2, TelSimImsiInfo_t ,0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "IMSI valid [%d]", imsi->bValid);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_iccid(TelSimIccIdInfo_t *iccid)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(iccid , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETICCIDINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);

		if(api_err == TAPI_API_SUCCESS)
		{
			*iccid = g_array_index(out_param2, TelSimIccIdInfo_t ,0);
			TAPI_LIB_DEBUG(LEVEL_ALERT, "ICCID length[%d]", iccid->icc_length);
		}
		else
		{
			TAPI_LIB_DEBUG(LEVEL_ALERT, "api_err[%d]",api_err);
		}
	}
	else
	{
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_mailbox_info(TelSimMailboxNumbers_s *mbox)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(mbox , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETMBINFO, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*mbox = g_array_index(out_param2, TelSimMailboxNumbers_s ,0);
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "chps?[%d]",mbox->b_cphs);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_callforwarding_info(TelSimCallForwarding_s *cf)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(cf , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETCFINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*cf = g_array_index(out_param2, TelSimCallForwarding_s ,0);
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "chps?[%d]",cf->b_cphs);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_messagewaiting_info(TelSimMessageWaiting_s *mw)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(mw , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETMWINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*mw = g_array_index(out_param2, TelSimMessageWaiting_s ,0);
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "chps?[%d]",mw->b_cphs);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_cphs_info(TelSimCphsLocalInfo_t *cphs)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(cphs , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETCPHSINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*cphs = g_array_index(out_param2, TelSimCphsLocalInfo_t ,0);
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "chps?[%d]",cphs->b_used);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}


EXPORT_API int tel_get_sim_msisdn(TelSimSubscriberInfo_t *subscriber)
{
	char *t_name, *t_num;
	int name_len, num_len;
	TAPI_RETURN_VAL_IF_FAIL(subscriber, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	t_name = vconf_get_str(VCONFKEY_TELEPHONY_SUBSCRIBER_NAME);
	t_num = vconf_get_str(VCONFKEY_TELEPHONY_SUBSCRIBER_NUMBER);

	if (t_name == NULL || t_num == NULL) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "FAIL TO GET VCONFKEY");
		return TAPI_API_OPERATION_FAILED;
	}

	name_len = strlen(t_name);
	num_len = strlen(t_num);

	if (name_len > TAPI_SIM_XDN_ALPHA_ID_MAX_LEN) {
		strncpy(subscriber->name, t_name, TAPI_SIM_XDN_ALPHA_ID_MAX_LEN);
		subscriber->name[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN] = '\0';
	} else {
		strncpy(subscriber->name, t_name, name_len);
		subscriber->name[name_len] = '\0';
	}

	if (num_len > TAPI_SIM_XDN_DIALING_NUMBER_LEN) {
		strncpy(subscriber->num, t_num, TAPI_SIM_XDN_DIALING_NUMBER_LEN);
		subscriber->num[TAPI_SIM_XDN_DIALING_NUMBER_LEN] = '\0';
	} else {
		strncpy(subscriber->num, t_num, num_len);
		subscriber->num[num_len] = '\0';
	}
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "MSISDN name[%s]num[%s]",subscriber->name,subscriber->num);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_sim_pb_init_info(int *init_completed, TelSimPbList_t *pb_list, int *first_index)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;
	int vconf_ret = 0;
	int vconf_val = 0;

	TAPI_RETURN_VAL_IF_FAIL((init_completed != NULL && pb_list != NULL && first_index != NULL), TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	vconf_ret = vconf_get_int(VCONFKEY_TELEPHONY_SIM_PB_INIT, &vconf_val);

	TAPI_LIB_DEBUG(LEVEL_ALERT,"VCONFKEY_TELEPHONY_SIM_PB_INIT api ret[%d], return value[%d] ",vconf_ret,vconf_val);

	if (vconf_val != 1) {
		*init_completed = 0;
		memset(pb_list, 0x00, sizeof(TelSimPbList_t));
		*first_index = 0x00;
		return api_err;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETPBINITINFO, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*init_completed = g_array_index(out_param2, int, 0);
		*pb_list = g_array_index(out_param3, TelSimPbList_t, 0);
		*first_index = g_array_index(out_param4, int, 0);

		TAPI_LIB_DEBUG(LEVEL_ALERT, "PB INIT Completed[%d]", *init_completed);
		TAPI_LIB_DEBUG(LEVEL_ALERT, "First Valid Index [%d]", *first_index);
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);
	return api_err;

}

EXPORT_API int tel_get_sim_pb_count(TelSimPbFileType_t pb_type, int* req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Storage type [%d]", pb_type);

	if ((pb_type != TAPI_SIM_PB_EN) && (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_LDN) && (pb_type
			!= TAPI_SIM_PB_MSISDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN) && (pb_type
			!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &pb_type, sizeof(TelSimPbFileType_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_PB_GETCOUNT, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_pb_meta_info(TelSimPbFileType_t pb_type, int* req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Storage type [%d]", pb_type);

	if ((pb_type != TAPI_SIM_PB_EN) && (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_LDN) && (pb_type
			!= TAPI_SIM_PB_MSISDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN) && (pb_type
			!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &pb_type, sizeof(TelSimPbFileType_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_PB_GETMETAINFO, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_get_sim_pb_3g_meta_info(int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_GETPBCAPABILITYINFO, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_read_sim_pb_record(TelSimPbFileType_t pb_type, unsigned short index, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_ALERT, "Storage type [%d]", pb_type);
	TAPI_LIB_DEBUG(LEVEL_ALERT, "Record Index [%d]", index);

	if (index == 0) {
		return TAPI_API_INVALID_INPUT;
	}

	if ((pb_type != TAPI_SIM_PB_EN) && (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_LDN) && (pb_type
			!= TAPI_SIM_PB_MSISDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN) && (pb_type
			!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS)) {

		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &pb_type, sizeof(TelSimPbFileType_t));
	g_array_append_vals(in_param2, &index, sizeof(unsigned short));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_PB_READRECORD, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);
	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_update_sim_pb_record(const TelSimPbRecordData_t *req_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((req_data != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, "Storage type [%d]", req_data->StorageFileType);
	TAPI_LIB_DEBUG(LEVEL_INFO, "Record Index [%d]", req_data->Index);

	if (req_data->Index == 0) {
		return TAPI_API_INVALID_INPUT;
	}

	if ((req_data->StorageFileType != TAPI_SIM_PB_EN) && (req_data->StorageFileType != TAPI_SIM_PB_FDN)
			&& (req_data->StorageFileType != TAPI_SIM_PB_LDN) && (req_data->StorageFileType != TAPI_SIM_PB_MSISDN)
			&& (req_data->StorageFileType != TAPI_SIM_PB_ADN) && (req_data->StorageFileType != TAPI_SIM_PB_SDN)
			&& (req_data->StorageFileType != TAPI_SIM_PB_3GSIM) && (req_data->StorageFileType != TAPI_SIM_PB_AAS)
			&& (req_data->StorageFileType != TAPI_SIM_PB_GAS)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, req_data, sizeof(TelSimPbRecordData_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_PB_UPDATERECORD, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_delete_sim_pb_record(TelSimPbFileType_t pb_type, unsigned short index, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	*req_id = INVALID_REQUEST_ID;
	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, "pb_type [%d]", pb_type);
	TAPI_LIB_DEBUG(LEVEL_INFO, "Record Index [%d]", index);

	if ((pb_type != TAPI_SIM_PB_EN) && (pb_type != TAPI_SIM_PB_FDN) && (pb_type != TAPI_SIM_PB_LDN) && (pb_type
			!= TAPI_SIM_PB_MSISDN) && (pb_type != TAPI_SIM_PB_ADN) && (pb_type != TAPI_SIM_PB_SDN) && (pb_type
			!= TAPI_SIM_PB_3GSIM) && (pb_type != TAPI_SIM_PB_AAS) && (pb_type != TAPI_SIM_PB_GAS)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (index == 0) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &pb_type, sizeof(TelSimPbFileType_t));
	g_array_append_vals(in_param2, &index, sizeof(unsigned short));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_PB_DELETERECORD, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_connection(TelSimSapConnect_t *req_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((req_data != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, "Msg Id [%d]", req_data->MsgId);
	TAPI_LIB_DEBUG(LEVEL_INFO, "Msg Max Size [%d]", req_data->MaxMsgSize);
	TAPI_LIB_DEBUG(LEVEL_INFO, "Msg Connection status [%d]", req_data->ConnectionStatus);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, req_data, sizeof(TelSimSapConnect_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPCONNECTREQUEST, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_connection_status(int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPCONNECTSTATUS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_transfer_atr(int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPATRREQUEST, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_transfer_apdu(TelSimSapApduData_t *apdu_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL((apdu_data != NULL && req_id != NULL) , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, "APDU length [%d]", apdu_data->ApduLength);
	TAPI_LIB_DEBUG(LEVEL_INFO, "APDU data [%s]", apdu_data->Apdu);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, apdu_data, sizeof(TelSimSapApduData_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPTRANSFERAPDU, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_transport_protocol(TelSimSapProtocol_t protocol, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, "SAP Protocol [%d]", protocol);

	if ((TAPI_SIM_SAP_PROTOCOL_T0 != protocol) && (TAPI_SIM_SAP_PROTOCOL_T1 != protocol)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &protocol, sizeof(TelSimSapProtocol_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPSETPROTOCOL, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_power_operation(TelSimSapMsgId_t req_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id, TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, "SAP Power Msg Id [%d]", req_data);

	if ((req_data < TAPI_SIM_SAP_CONNECT_REQ) || (req_data > TAPI_SIM_SAP_SET_TRANSPORT_PROTOCOL_RESP)) {
		return TAPI_API_INVALID_INPUT;
	}

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, &req_data, sizeof(TelSimSapMsgId_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPSETSIMPOWER, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sap_cardreader_status(int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_SAPCARDREADERSTATUS, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}

EXPORT_API int tel_req_sim_isim_authentication(TelSimIsimAuthenticationData_t *authentication_data, int *req_id)
{
	TS_BOOL ret = FALSE;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RETURN_VAL_IF_FAIL(req_id , TAPI_API_INVALID_PTR);
	TAPI_RETURN_VAL_IF_FAIL(authentication_data , TAPI_API_INVALID_PTR);

	if(_tel_check_tapi_state() != 0 )
		return TAPI_API_SERVICE_NOT_READY;

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	TAPI_GLIB_INIT_PARAMS();

	TAPI_LIB_DEBUG(LEVEL_INFO, " AuthDataLength [%d]", authentication_data->AuthDataLength);
	TAPI_LIB_DEBUG(LEVEL_INFO, " AuthData [%s]", authentication_data->AuthData);
	TAPI_LIB_DEBUG(LEVEL_INFO, " RandomAccessLength [%d]", authentication_data->RandomAccessLength);
	TAPI_LIB_DEBUG(LEVEL_INFO, " RandomAccessData [%s]", authentication_data->RandomAccessData);

	if (FALSE == tapi_check_dbus_status()) {
		return TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, authentication_data, sizeof(TelSimIsimAuthenticationData_t));
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_CS_SERVICE_SIM, TAPI_CS_SIM_ISIMAUTHENTICATION, in_param1, in_param2, in_param3,
			in_param4, &out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		api_err = g_array_index(out_param1, int ,0);
		*req_id = g_array_index(out_param2, int ,0);

		if (api_err != TAPI_API_SUCCESS) {
			*req_id = INVALID_REQUEST_ID;
		}
	}
	else {
		api_err = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return api_err;
}
