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
#include <stdlib.h>

#include <dbus/dbus-glib.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus.h>

#include <vconf.h>

#include "TelDefines.h"
#include "tel_cs_conn.h"
#include "TapiUtility.h"
#include "TelUtility.h"
#include "TapiCommon.h"
#include "tapi_proxy.h"

#include "tel_marshaler.h"

enum {
	TAPI_SIGNAL0,
	TAPI_SIGNAL1,
	TAPI_SIGNAL2,
	TAPI_SIGNAL3,
	TAPI_SIGNAL_MAX
};

static char *_evt_sig_members[] = {
	TAPI_SIGNAL_MEMBER_ARG0,
	TAPI_SIGNAL_MEMBER_ARG2,
	TAPI_SIGNAL_MEMBER_ARG4,
	TAPI_SIGNAL_MEMBER_ARG5,
};

extern char *g_cookie_name;
extern int g_cookie_size;

tapi_dbus_connection_name conn_name;

typedef enum {
	TAPI_EVTMODE_NONE,
	TAPI_EVTMODE_GLIB,
	TAPI_EVTMODE_ECORE,
} TelTapiEvtMode_t;

typedef struct GDBusEvtItem_t {
	unsigned int id;
	DBusGProxy *robj;
	GCallback cb[4];
	void *data;
} GDBusEvtItem_t;

typedef struct {
	void *data;
	TelAppCallback cb;
} UserData_t;

typedef GCallback (*GDBusConnSig_t)(DBusGProxy *, const char *, void *);

static GCallback gdbus_conn_signal0(DBusGProxy *robj, const char *member, void *data);
static GCallback gdbus_conn_signal2(DBusGProxy *robj, const char *member, void *data);
static GCallback gdbus_conn_signal4(DBusGProxy *robj, const char *member, void *data);
static GCallback gdbus_conn_signal5(DBusGProxy *robj, const char *member, void *data);

static GDBusConnSig_t GDBusConnSig[TAPI_SIGNAL_MAX] = { gdbus_conn_signal0, gdbus_conn_signal2, gdbus_conn_signal4,
		gdbus_conn_signal5 };

static DBusGConnection *GDBusHandle = NULL;

static int _tapi_init_ref_count = 0;

static GSList *EvtList = NULL;
static long int EvtId = 0;

static TelTapiEvtMode_t EvtMode = TAPI_EVTMODE_NONE;

#define CHK_EVENT_MODE(MODE) \
	if (EvtMode != MODE) { \
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Event MODE check failed (current: %d)", EvtMode); \
		return TAPI_API_OPERATION_FAILED; \
	}

#define CHK_EVENT_INIT_MODE(MODE) \
	if (EvtMode != MODE && EvtMode != TAPI_EVTMODE_NONE) { \
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"Event MODE check failed (current: %d)", EvtMode); \
		return TAPI_API_OPERATION_FAILED; \
	}

#define EVENT_DUMP(Event) \
	{ \
		TAPI_LIB_DEBUG(LEVEL_DEBUG, " Received group is :0x%x", Event.EventClass); \
		TAPI_LIB_DEBUG(LEVEL_DEBUG, " received event type is :0x%x", Event.EventType); \
		TAPI_LIB_DEBUG(LEVEL_DEBUG, " received requestid is :0x%x", Event.RequestId); \
		TAPI_LIB_DEBUG(LEVEL_DEBUG, " received status is :%d", Event.Status); \
	} \

static void on_gdbus_event_receive0(DBusGProxy *proxy, void *local_data)
{
	TelTapiEvent_t Event;

	Event.EventClass = -1;
	Event.EventType = 0;
	Event.RequestId = -1;
	Event.Status = 0;
	Event.pData = NULL;
	Event.pDataLen = 0;

	EVENT_DUMP(Event);

	/* Contents of parameters is defined by every notification. */
	UserData_t *user_data;
	user_data = (UserData_t *) local_data;
	user_data->cb(&Event, user_data->data);

	TAPI_LIB_DEBUG(LEVEL_DEBUG," Application Callback Invoked successfully ");
}

static void on_gdbus_event_receive2(DBusGProxy *proxy, int type, const char *data, void *local_data)
{
	TelTapiEvent_t Event;
	guchar *result = NULL;
	size_t out_len;

	Event.EventClass = -1;
	Event.EventType = type;
	Event.RequestId = -1;
	Event.Status = 0;
	Event.pData = NULL;

	EVENT_DUMP(Event);

	TAPI_BASE64_DECODE(data, result, &out_len);
	if (out_len > 0)
		Event.pData = result;
	Event.pDataLen = out_len;

	/* Contents of parameters is defined by every notification. */
	UserData_t *user_data;
	user_data = (UserData_t *) local_data;
	user_data->cb(&Event, user_data->data);

	TAPI_LIB_DEBUG(LEVEL_DEBUG," Application Callback Invoked successfully ");

	if (result)
		g_free(result);
}

static void on_gdbus_event_receive4(
		DBusGProxy *proxy,
		int grp,
		int type,
		int status,
		const char *data,
		void *local_data)
{
	TelTapiEvent_t Event;
	guchar *result = NULL;
	size_t out_len;

	Event.EventClass = grp;
	Event.EventType = type;
	Event.RequestId = -1;
	Event.Status = status;
	Event.pData = NULL;

	EVENT_DUMP(Event);

	TAPI_BASE64_DECODE(data, result, &out_len);
	if (out_len > 0)
		Event.pData = result;
	Event.pDataLen = out_len;

	/* Contents of parameters is defined by every notification. */
	UserData_t *user_data;
	user_data = (UserData_t *) local_data;
	user_data->cb(&Event, user_data->data);

	TAPI_LIB_DEBUG(LEVEL_DEBUG," Application Callback Invoked successfully ");
	if (result)
		g_free(result);
}

static void on_gdbus_event_receive5(
		DBusGProxy *proxy,
		int grp,
		int type,
		int req_id,
		int status,
		const char *data,
		void *local_data)
{
	TelTapiEvent_t Event;
	guchar *result = NULL;
	size_t out_len = 0;

	Event.EventClass = grp;
	Event.EventType = type;
	Event.RequestId = req_id;
	Event.Status = status;
	Event.pData = NULL;

	EVENT_DUMP(Event);


	TAPI_BASE64_DECODE(data, result, &out_len);
	Event.pData = result;
	if (result == NULL){
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "result =0x%p", result);
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "out_len =%d", out_len);
		Event.pDataLen = 0;
	}
	else{
		Event.pDataLen = out_len;
	}

	/* Contents of parameters is defined by every notification. */
	UserData_t *user_data;
	user_data = (UserData_t *) local_data;

	user_data->cb(&Event, user_data->data);

	TAPI_LIB_DEBUG(LEVEL_DEBUG," Application Callback Invoked successfully ");
	if (result)
		g_free(result);
}

static GCallback gdbus_conn_signal0(DBusGProxy *robj, const char *member, void *data)
{
	dbus_g_proxy_add_signal(robj, member, G_TYPE_INVALID);
	dbus_g_proxy_connect_signal(robj, member, G_CALLBACK(on_gdbus_event_receive0), data, NULL);
	return G_CALLBACK(on_gdbus_event_receive0);
}

static GCallback gdbus_conn_signal2(DBusGProxy *robj, const char *member, void *data)
{
	dbus_g_proxy_add_signal(robj, member, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INVALID);
	dbus_g_proxy_connect_signal(robj, member, G_CALLBACK(on_gdbus_event_receive2), data, NULL);
	return G_CALLBACK(on_gdbus_event_receive2);
}

static GCallback gdbus_conn_signal4(DBusGProxy *robj, const char *member, void *data)
{
	dbus_g_proxy_add_signal(robj, member, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INVALID);
	dbus_g_proxy_connect_signal(robj, member, G_CALLBACK(on_gdbus_event_receive4), data, NULL);
	return G_CALLBACK(on_gdbus_event_receive4);
}

static GCallback gdbus_conn_signal5(DBusGProxy *robj, const char *member, void *data)
{
	dbus_g_proxy_add_signal(robj, member, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INVALID);
	dbus_g_proxy_connect_signal(robj, member, G_CALLBACK(on_gdbus_event_receive5), data, NULL);
	return G_CALLBACK(on_gdbus_event_receive5);
}

static void gdbus_del_signal(GDBusEvtItem_t *it)
{
	int i;
	for (i = 0; i < TAPI_SIGNAL_MAX; i++) {
		dbus_g_proxy_disconnect_signal(it->robj, _evt_sig_members[i], G_CALLBACK(it->cb[i]), it->data);
	}
	if (it->data) {
		free(it->data);
	}
	g_free(it);
}

static GDBusEvtItem_t *gdbus_add_signal(const char *event_string, void *data)
{
	GDBusEvtItem_t *it = NULL;
	DBusGProxy *robj;
	int i;

	robj = dbus_g_proxy_new_for_name(GDBusHandle, TAPI_SIGNAL_SENDER, TAPI_SIGNAL_OBJPATH, event_string);
	if (!robj) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"failed to get proxy for iface: %s", event_string);
		return it;
	}

	it = (GDBusEvtItem_t *) g_malloc0(sizeof(GDBusEvtItem_t));
	if (!it) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG,"failed to allocate item memory");
		return it;
	}

	it->id = EvtId;
	it->robj = robj;
	it->data = data;

	if ((EvtList = g_slist_append(EvtList, it)) == NULL) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "failed to event info append");
		g_free(it);
		return NULL;
	}

	for (i = 0; i < TAPI_SIGNAL_MAX; i++)
		it->cb[i] = GDBusConnSig[i](robj, _evt_sig_members[i], data);

	return it;
}

static void _cookie_deinit(void)
{
	if (g_cookie_name)
		free(g_cookie_name);

	g_cookie_name = NULL;
	g_cookie_size = 0;
}

EXPORT_API int tel_init(void)
{
	DBusGConnection *conn;
	GError *err = NULL;

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "TelTapiInit");

	if (++_tapi_init_ref_count > 1) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "Already initialized. (ref_count=%d)", _tapi_init_ref_count);
		return TAPI_API_SUCCESS;
	}

	CHK_EVENT_INIT_MODE(TAPI_EVTMODE_GLIB);
	EvtMode = TAPI_EVTMODE_GLIB;

	if (GDBusHandle)
		return TAPI_API_SUCCESS;

	g_type_init();

	conn = dbus_g_bus_get(DBUS_BUS_SYSTEM, &err);
	if (conn == NULL) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "dbus_bus_get failed: %s", (char *)err->message);
		EvtMode = TAPI_EVTMODE_NONE;
		return TAPI_API_OPERATION_FAILED;
	}
	GDBusHandle = conn;

	EvtId = 0;

	dbus_g_object_register_marshaller(g_cclosure_user_marshal_VOID__INT_INT_INT_INT_STRING, G_TYPE_NONE, G_TYPE_INT,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INVALID);

	dbus_g_object_register_marshaller(g_cclosure_user_marshal_VOID__INT_INT_INT_STRING, G_TYPE_NONE, G_TYPE_INT,
			G_TYPE_INT, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INVALID);

	dbus_g_object_register_marshaller(g_cclosure_user_marshal_VOID__INT_STRING, G_TYPE_NONE, G_TYPE_INT, G_TYPE_STRING,
			G_TYPE_INVALID);

	/* Check TAPI READY vconf */
	int ts_status = 0;
	int ret_val;

	ret_val = vconf_get_int(VCONFKEY_TELEPHONY_TAPI_STATE, &ts_status);
	if (ret_val < 0) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "[%s] vconf key default set (value=%d)",
				VCONFKEY_TELEPHONY_TAPI_STATE,
				VCONFKEY_TELEPHONY_TAPI_STATE_NONE);
		vconf_set_int(VCONFKEY_TELEPHONY_TAPI_STATE, VCONFKEY_TELEPHONY_TAPI_STATE_NONE);
	}

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_register_event(int EventType, unsigned int *SubscriptionId, TelAppCallback AppCallBack, void *data)
{
	char event_string[IF_NAME_MAX];
	GDBusEvtItem_t *it = NULL;

	CHK_EVENT_MODE(TAPI_EVTMODE_GLIB);

	/*	CHECK THE INPUT	*/
	TAPI_RET_ERR_NUM_IF_FAIL(AppCallBack, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(SubscriptionId, TAPI_API_INVALID_PTR);
	/*
	 if ((EventType < TAPI_EVENT_CALL_SETUP_CNF) || (EventType > TAPI_EVENT_TYPE_MAX)) {
	 TAPI_LIB_DEBUG(LEVEL_DEBUG,"Unknown EventClass");
	 return TAPI_EVENT_CLASS_UNKNOWN;
	 }
	 */
	UserData_t *user_data;
	user_data = (UserData_t *) malloc(sizeof(UserData_t));
	user_data->data = data;
	user_data->cb = AppCallBack;

	TAPI_GET_EVENT_NAME(EventType, event_string);
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"PROXY COMMON EVENT STRING=%s", event_string);
	it = gdbus_add_signal(event_string, (void *) user_data);
	if (!it) {
		return TAPI_API_OPERATION_FAILED;
	}

	*SubscriptionId = EvtId++;
	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Noti Subscribe Successful with id %d", *SubscriptionId);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_deregister_event(unsigned int SubscriptionId)
{
	GSList *iter;
	GDBusEvtItem_t *it;
	CHK_EVENT_MODE(TAPI_EVTMODE_GLIB);
	for (iter = EvtList; iter != NULL; iter = iter->next) {
		it = (GDBusEvtItem_t *) iter->data;
		if (it->id == SubscriptionId) {
			gdbus_del_signal(it);
			EvtList = g_slist_delete_link(EvtList, iter);
			return TAPI_API_SUCCESS;
		}
	}

	return TAPI_API_INVALID_INPUT;
}

static void _tel_remove_signal(gpointer data, gpointer user_data)
{
	GDBusEvtItem_t *it;
	it = (GDBusEvtItem_t *) data;
	if (it) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "TEL_REGI deletenode it=0x%x id=%d",it, it->id);
		gdbus_del_signal(it);
	}
}

EXPORT_API int tel_deinit(void)
{
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "TelTapiDeinit");

	if (--_tapi_init_ref_count > 0) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "no work. (ref_count=%d)", _tapi_init_ref_count);
		return TAPI_API_SUCCESS;
	}

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "remove all signal");

	g_slist_foreach(EvtList, _tel_remove_signal, NULL);
	EvtList = NULL;
	EvtId = 0;
	EvtMode = TAPI_EVTMODE_NONE;
	g_slist_free(EvtList);

	_cookie_deinit();

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_register_app_name(char *name)
{
	if (name == NULL) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "The 1st parameter, the connection name, is NULL\n");
		return TAPI_API_INVALID_PTR;
	}

	DBusConnection *dbus_connection = NULL;
	DBusError err;
	dbus_error_init(&err);

	if (GDBusHandle) {
		dbus_connection = dbus_g_connection_get_connection(GDBusHandle);
		if (dbus_bus_request_name(dbus_connection, name, DBUS_NAME_FLAG_DO_NOT_QUEUE, &err) < 0) {
			TAPI_LIB_DEBUG(LEVEL_ERR, "Fail to request dbus connection name\n");
			return TAPI_API_INVALID_OPERATION;
		}
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No valid dbus connection\n");
	}

	TAPI_LIB_DEBUG(LEVEL_DEBUG,"Request connection name = %s", name );
	snprintf(conn_name.name, 255, "%s", name);
	conn_name.length_of_name = strlen(conn_name.name);

	return TAPI_API_SUCCESS;
}

EXPORT_API int tel_get_app_name(tapi_dbus_connection_name *app_name)
{
	if (app_name == NULL) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "The 1st parameter, the application name, is NULL\n");
		return TAPI_API_INVALID_PTR;
	}

	snprintf(app_name->name, TAPI_DBUS_CONNECTION_NAME_LEN_MAX, "%s", conn_name.name);
	app_name->length_of_name = strlen(app_name->name);

	return TAPI_API_SUCCESS;
}

/*
 This Api interacts with Telephony servers STATUS library to get the ready status
 According to Lee this new code will be opened only in rel 1.0. Ramu is the author.
 */
EXPORT_API int tel_check_service_ready(int *bStatus)
{
	int ts_status = 0;
	int ret_val = 0;
	int api_err = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(bStatus, TAPI_API_INVALID_PTR);

	ret_val = vconf_get_int(VCONFKEY_TELEPHONY_TAPI_STATE, (int*) &ts_status);
	if (ret_val == 0) {
		ret_val = 1;
	}
	else {
		TAPI_LIB_DEBUG(LEVEL_ERR, "[FAIL] GET VCONFKEY_TELEPHONY_TAPI_STATE");
		ret_val = -1;
	}

	/*	for testing	*/
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "NEW: Status Check value is %d", ts_status);

	if (ret_val != 1) {
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "GETTING STATUS_TAPI_STATE Failed............\n");
		return TAPI_API_SERVER_LAYER_FAILURE;
	}
	else {
		*bStatus = ts_status;
		return TAPI_API_SUCCESS;
	}
	return api_err;
}

EXPORT_API int tel_request_tunnel(const TelTunnelInfo_t *pTunnelInfo, int *pRequestId)
{
	int ret = FALSE;
	int returnStatus = TAPI_API_SUCCESS;

	TAPI_RET_ERR_NUM_IF_FAIL(pRequestId, TAPI_API_INVALID_PTR);
	TAPI_RET_ERR_NUM_IF_FAIL(pTunnelInfo, TAPI_API_INVALID_PTR);

	if (conn_name.length_of_name == 0) {
		TAPI_LIB_DEBUG(LEVEL_ERR, "No dbus connection name");
		return TAPI_API_OPERATION_FAILED;
	}

	/*	 check for the RPC link....	*/
	TAPI_RET_ERR_NUM_IF_FAIL(tapi_check_dbus_status(), TAPI_API_SYSTEM_RPC_LINK_DOWN);
	TAPI_GLIB_INIT_PARAMS();

	TAPI_GLIB_ALLOC_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	g_array_append_vals(in_param1, pTunnelInfo, sizeof(TelTunnelInfo_t));
	g_array_append_vals(in_param2, (unsigned char *) (pTunnelInfo->pTunnelData), pTunnelInfo->TunnelDataLen);
	g_array_append_vals(in_param4, &conn_name, sizeof(tapi_dbus_connection_name));

	ret = tapi_send_request(TAPI_EVENT_CLASS_UTIL, TAPI_CS_UTIL_TUNNEL_REQ, in_param1, in_param2, in_param3, in_param4,
			&out_param1, &out_param2, &out_param3, &out_param4);

	if (TRUE == ret) {
		returnStatus = g_array_index(out_param1, int , 0);
		*pRequestId = g_array_index(out_param2, int , 0);
	}
	else {
		*pRequestId = -1;
		returnStatus = TAPI_API_SYSTEM_RPC_LINK_DOWN;
	}

	TAPI_LIB_DEBUG(LEVEL_DEBUG, "Tunnel Request: Return Status : %d", returnStatus);
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "RequestId: %d", *pRequestId);

	TAPI_GLIB_FREE_PARAMS(in_param1,in_param2,in_param3,in_param4,out_param1,out_param2,out_param3,out_param4);

	return returnStatus;
}

/*	EOF	*/

