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
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <stdlib.h>
#include <security-server.h>

#include <glib-2.0/glib.h>

#include "TelDefines.h"
#include "tapi-lib-proxy.h"
#include "tel_cs_conn.h"
#include "TapiUtility.h"
#include "tapi_proxy.h"

/* This global stores the proxy connection to dbus */
DBusGProxy* tapi_cs_proxy = NULL;

extern tapi_dbus_connection_name conn_name;

char *g_cookie_name;
int g_cookie_size;

/* This global stores error if any during establishing an dbus connection */

static void _dump_cookie(char *data, int size)
{
	char buf[255] = { 0, };
	char tmp[3] = { 0, };
	int i = 0;

	for (i = 0; i < size; i++) {
		snprintf(tmp, 3, "%02X", data[i]);
		strncat(buf, tmp, 3);
		if ((i + 1) % 8 == 0)
			strncat(buf, "  ", 2);
	}

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "cookie=[%s]", buf);
}

static int _cookie_init(void)
{
	int ret;

	ret = security_server_get_cookie_size();
	if (ret < 0) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "get_cookie_size() failed. ret=%d", ret);
		return FALSE;
	}
	g_cookie_size = ret;

	g_cookie_name = (char *) calloc(1, g_cookie_size + 1);
	if (!g_cookie_name) {
		g_cookie_size = 0;
		TAPI_LIB_DEBUG(LEVEL_ERR, "cookie alloc failed");
		return FALSE;
	}
	else {
		ret = security_server_request_cookie(g_cookie_name, g_cookie_size);
		if (ret < 0) {
			free(g_cookie_name);
			g_cookie_name = NULL;
			g_cookie_size = 0;
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "request_cookie() failed. ret=%d", ret);
			return FALSE;
		}

		_dump_cookie(g_cookie_name, g_cookie_size);
	}

	return TRUE;
}

/**
 *
 * This function is used to initialise a connnection with telephony server through DBUS.
 *
 * @return			True / False
 * @param[in]		NONE
 * @param[out]		None
 * @remark
 * @Refer
 */
static int tapi_init_cs()
{
	DBusGConnection* tapi_cs_connection = NULL;
	GError* err = NULL;

	g_type_init();

	tapi_cs_connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &err);
	TAPI_LIB_DEBUG(LEVEL_ERR, "tapi_cs_connection = %x");

	/*	No need to assert but, need to return FALSE	*/
	if (tapi_cs_connection == NULL) {
		if (err != NULL) {
			fprintf(stderr, "[FAILED]connection to dbus failed: %s\n", err->message);
			TAPI_LIB_DEBUG(LEVEL_ERR, "failed to get CS connection with error cause");
			g_error_free(err);
		}
		else {
			TAPI_LIB_DEBUG(LEVEL_ERR, "failed to get CS connection with NULL error cause");
		}
		return FALSE;
	}

	tapi_cs_proxy = dbus_g_proxy_new_for_name(tapi_cs_connection, "org.projectx.telephony", "/org/projectx/app",
			"org.projectx.telephony");

	/*	No need to assert but, need to return FALSE	*/
	TAPI_RETURN_VAL_IF_FAIL(tapi_cs_proxy, FALSE);

	return TRUE;
}

/**
 *
 * This function is used to check the connnection status with telephony server through DBUS.
 *
 * @return			True / False
 * @param[in]		NONE
 * @param[out]		None
 * @remark
 * @Refer
 */
int tapi_check_dbus_status(void)
{
	TS_BOOL result = TRUE;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "DBUS-CS tapi_check_dbus_status");

	if (tapi_cs_proxy != NULL && dbus_g_proxy_get_interface(tapi_cs_proxy)) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "DBUS-CS Ready to use");
		return TRUE;
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "DBUS-CS Initializing...");
		if (tapi_init_cs()) {
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "DBUS-CS INIT completed");
			result = TRUE;
		}
		else {
			TAPI_LIB_DEBUG(LEVEL_ERR, "FAIL TO DBUS-CS INIT");
			result = FALSE;
		}
	}

	return result;
}

/**
 *
 * This function is used to send the service request to telephony server through DBUS.
 *
 * @return			True / False
 * @param[in]		tapi_service,tapi_service_function,in_param1,in_param2,in_param3,in_param4
 * @param[out]		out_param1,out_param2,out_param3,out_param4
 * @remark
 * @Refer
 */
int tapi_send_request(int tapi_service, int tapi_service_function, GArray* in_param1, GArray* in_param2,
		GArray* in_param3, GArray* in_param4, GArray** out_param1, GArray** out_param2, GArray** out_param3,
		GArray** out_param4)
{
	int error_occurred;
	TS_BOOL ret = FALSE;
	TS_BOOL api_ret = TRUE;
	GError* error = NULL;
	GArray* in_param5 = ((void *) 0);

	/**
	 * Add d-bus connection name to ALL REQUEST
	 *  - If already appended conn_name to in_param4[0] before tapi_send_request(),
	 *    then append conn_name to in_param4[1] and ignore by telephony-server.
	 */
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));
	if (conn_name.length_of_name > 0) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "conn_name=[%s]", conn_name.name);
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "no conn_name. pid=%d", getpid());
	}

	/**
	 * Add security cookie to ALL REQUEST
	 */
	if (!g_cookie_name) {
		if (_cookie_init() != TRUE) {
			TAPI_LIB_DEBUG(LEVEL_DEBUG, "no cookie name. pid=%d", getpid());
			return FALSE;
		}
	}

	in_param5 = g_array_new(FALSE, FALSE, sizeof(gchar));
	g_array_append_vals(in_param5, g_cookie_name, g_cookie_size + 1);

	/**
	 * D-Bus send
	 */
	api_ret = org_projectx_telephony_service_request(tapi_cs_proxy, tapi_service, tapi_service_function, in_param1,
			in_param2, in_param3, in_param4, in_param5, out_param1, out_param2, out_param3, out_param4, &error_occurred,
			&error);

	if (api_ret != TRUE) {
		// dBUS-RPC is failed
		ret = FALSE;

		// We can not say only TS is killed. This is dBUS-RPC failure including TS crash case
		TAPI_LIB_DEBUG(LEVEL_ALERT, "dBUS-RPC is failed %d", api_ret);

		if (error != NULL) {
			// dBUS gives error cause
			TAPI_LIB_DEBUG(LEVEL_ALERT,
					"D-Bus API failure: errCode[0x%x], message: [%s], service type=%d, service function=0x%x",
					error_occurred, error->message, tapi_service, tapi_service_function);

			g_error_free(error);
		}
		else {
			// dBUS does not give error cause even though dBUS-RPC is failed
			TAPI_LIB_DEBUG(LEVEL_ALERT, "dBUS RPC is failed but error returned was NULL");
		}
	}
	else {
		// dBUS-RPC is successfully done
		ret = TRUE;

		TAPI_LIB_DEBUG(LEVEL_INFO, "dBUS RPC is successfully done. service type=%d, service function=0x%x",
				tapi_service, tapi_service_function);
	}

	g_array_free(in_param5, TRUE);

	return ret;
}
