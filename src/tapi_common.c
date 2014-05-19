/*
 * libtapi
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd. All rights reserved.
 * Copyright (c) 2013 Intel Corporation. All rights reserved.
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
#include <stdlib.h>
#include <string.h>

#include "tapi_private.h"

#include <tapi_events.h>
#include <tel_modem.h>
#include <tel_sim.h>
#include <tel_network.h>
#include <tel_call.h>
#include <tel_ss.h>
#include <tel_sms.h>
#include <tel_phonebook.h>
#include <tel_sat.h>
#include <tel_sap.h>
#include <tel_gps.h>

/* Signal Callbacks */
extern void on_network_signal_emit_handler(TelephonyNetwork *network,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_phonebook_signal_emit_handler(TelephonyPhonebook *phonebook,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_call_signal_emit_handler(TelephonyCall *call,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_ss_signal_emit_handler(TelephonySs *ss,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_sms_signal_emit_handler(TelephonySms *sms,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_sat_signal_emit_handler(TelephonySat *sat,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_sap_signal_emit_handler(TelephonySap *sap,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);
extern void on_gps_signal_emit_handler(TelephonyGps *gps,
	gchar *sender_name, gchar *signal_name,
	GVariant *parameters, gpointer user_data);

/* Property change Callbacks */
extern void on_modem_property_change_handler(TelephonyModem *modem,
	GVariant *changed_properties, const gchar *invalidated_properties, gpointer user_data);
extern void on_sim_property_change_handler(TelephonySim *sim,
	GVariant *changed_properties, const gchar *invalidated_properties, gpointer user_data);
extern void on_sms_property_change_handler(TelephonySms *sms,
	GVariant *changed_properties, const gchar *invalidated_properties, gpointer user_data);

EXPORT_API TelReturn tapi_get_cp_name_list(char ***cp_list)
{
	GDBusObjectManager *obj_manager = NULL;
	GList *objects = NULL;
	GList *iter = NULL;
	GError *error = NULL;
	unsigned int cp_count = 0;
	unsigned int index = 0;

	*cp_list = NULL;

	obj_manager = telephony_object_manager_client_new_for_bus_sync(
					G_BUS_TYPE_SYSTEM,
					G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
					TELEPHONY_SERVICE,
					TELEPHONY_OBJECT_PATH,
					NULL, &error);
	if (error) {
		err("Error getting object manager client: %s", error->message);
		g_error_free (error);
		return TEL_RETURN_FAILURE;
	}

	objects = g_dbus_object_manager_get_objects(obj_manager);

	cp_count = g_list_length(objects);
	if (cp_count == 0)
		goto out;

	/* This will be freed by caller. +1 for NULL 'str' at the end */
	*cp_list = calloc((cp_count + 1), sizeof(char*));

	for (iter = objects, index = 0; iter != NULL; iter = iter->next, index++) {
		TelephonyObject *object = TELEPHONY_OBJECT(iter->data);
		const gchar *object_path = g_dbus_object_get_object_path(G_DBUS_OBJECT(object));
		if (object_path) {
			(*cp_list)[index] = strdup(object_path); /* Will be freed up caller */
			if ((*cp_list)[index] == NULL)
				g_error ("%s: strdup failed", G_STRLOC);
		}
	}

out:
	if (objects != NULL)
		g_list_free_full(objects, g_object_unref);

	if (obj_manager != NULL)
		g_object_unref (obj_manager);

	return TEL_RETURN_SUCCESS;
}

EXPORT_API TelHandle *tapi_init(const char *cp_name)
{
#define HANDLE_GERROR(error, log_msg) \
	if (error) { \
		err(log_msg": %s", error->message); \
		g_error_free(error); \
		goto fail; \
	}

	TelHandle *handle = NULL;
	GDBusObjectManager *obj_manager = NULL;
	GList *objects = NULL;
	TelephonyObject *tel_object;
	GList *interfaces = NULL;
	GList *iter = NULL;
	GError *error = NULL;
	const gchar *cp_found = NULL;

#if !GLIB_CHECK_VERSION (2, 35, 3)
	g_type_init();
#endif

	/* Find CP objects */
	obj_manager = telephony_object_manager_client_new_for_bus_sync(
					G_BUS_TYPE_SYSTEM,
					G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
					TELEPHONY_SERVICE,
					TELEPHONY_OBJECT_PATH,
					NULL, &error);
	if (error) {
		err("Error getting object manager client:%s", error->message);
		g_error_free(error);
		return NULL;
	}

	objects = g_dbus_object_manager_get_objects(obj_manager);
	for (iter = objects; iter != NULL; iter = iter->next) {
		tel_object = TELEPHONY_OBJECT(iter->data);
		const gchar *object_path = g_dbus_object_get_object_path(G_DBUS_OBJECT(tel_object));

		/* NULL: First CP in list, else matching */
		if ((cp_name == NULL) || (g_strcmp0(cp_name, object_path) == 0)) {
			cp_found = object_path;
			break;
		}
	}

	if (cp_found == NULL)
		goto out;

	dbg("cp_found: [%s]", cp_found);

	/* Allocate new Handle */
	handle = g_new0(struct tapi_handle, 1);

	/* Find interfaces available for the given CP */
	interfaces = g_dbus_object_get_interfaces(G_DBUS_OBJECT (tel_object));
	for (iter = interfaces; iter != NULL; iter = iter->next) {
		GDBusInterface *interface = G_DBUS_INTERFACE(iter->data);
		const gchar *interface_name = g_dbus_interface_get_info(interface)->name;

		/* Create proxies and register for Signals and Property change events */
		if ((handle->modem_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_MODEM_INTERFACE) == 0)) {
			handle->modem_proxy = telephony_modem_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting modem proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->modem_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);


			handle->modem_property_handler = g_signal_connect(
						handle->modem_proxy,
						"g-properties-changed",
						G_CALLBACK(on_modem_property_change_handler),
						handle);
		} else if ((handle->sim_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_SIM_INTERFACE) == 0)) {
			handle->sim_proxy = telephony_sim_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting sim proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->sim_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->sim_property_handler = g_signal_connect(
						handle->sim_proxy,
						"g-properties-changed",
						G_CALLBACK(on_sim_property_change_handler),
						handle);
		} else if ((handle->network_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_NETWORK_INTERFACE) == 0)) {
			handle->network_proxy = telephony_network_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting network proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->network_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->network_signal_handler = g_signal_connect(
						handle->network_proxy,
						"g-signal",
						G_CALLBACK(on_network_signal_emit_handler),
						handle);
		} else if ((handle->call_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_CALL_INTERFACE) == 0)) {
			handle->call_proxy = telephony_call_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting call proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->call_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->call_signal_handler = g_signal_connect(
						handle->call_proxy,
						"g-signal",
						G_CALLBACK(on_call_signal_emit_handler),
						handle);
		} else if ((handle->ss_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_SS_INTERFACE) == 0)) {
			handle->ss_proxy = telephony_ss_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting ss proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->ss_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->ss_signal_handler = g_signal_connect(
						handle->ss_proxy,
						"g-signal",
						G_CALLBACK(on_ss_signal_emit_handler),
						handle);
		} else if ((handle->sms_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_SMS_INTERFACE) == 0)) {
			handle->sms_proxy = telephony_sms_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting sms proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->sms_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->sms_signal_handler = g_signal_connect(
						handle->sms_proxy,
						"g-signal",
						G_CALLBACK(on_sms_signal_emit_handler),
						handle);

			handle->sms_property_handler = g_signal_connect(
						handle->sms_proxy,
						"g-properties-changed",
						G_CALLBACK(on_sms_property_change_handler),
						handle);
		} else if ((handle->phonebook_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_PB_INTERFACE) == 0)) {
			handle->phonebook_proxy = telephony_phonebook_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting phonebook proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->phonebook_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->phonebook_signal_handler = g_signal_connect(
						handle->phonebook_proxy,
						"g-signal",
						G_CALLBACK(on_phonebook_signal_emit_handler),
						handle);
		} else if ((handle->sat_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_SAT_INTERFACE) == 0)) {
			handle->sat_proxy = telephony_sat_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting sat proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->sat_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->sat_signal_handler = g_signal_connect(
						handle->sat_proxy,
						"g-signal",
						G_CALLBACK(on_sat_signal_emit_handler),
						handle);
		} else if ((handle->sap_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_SAP_INTERFACE) == 0)) {
			handle->sap_proxy = telephony_sap_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting sap proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->sap_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->sap_signal_handler = g_signal_connect(
						handle->sap_proxy,
						"g-signal",
						G_CALLBACK(on_sap_signal_emit_handler),
						handle);
		} else if ((handle->gps_proxy == NULL)
			&& (g_strcmp0(interface_name, TELEPHONY_GPS_INTERFACE) == 0)) {
			handle->gps_proxy = telephony_gps_proxy_new_for_bus_sync(
						G_BUS_TYPE_SYSTEM,
						G_DBUS_PROXY_FLAGS_NONE,
						TELEPHONY_SERVICE,
						cp_found, NULL, &error);
			HANDLE_GERROR(error, "Error getting gps proxy");

			/* Setting Proxy default timeout as DEFAULT value (60 secs) */
			g_dbus_proxy_set_default_timeout((GDBusProxy *)handle->gps_proxy,
				TAPI_DBUS_TIMEOUT_DEFAULT);

			handle->gps_signal_handler = g_signal_connect(
						handle->gps_proxy,
						"g-signal",
						G_CALLBACK(on_gps_signal_emit_handler),
						handle);
		}
	}

	handle->cp_name = g_strdup(cp_found);
	handle->evt_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

	goto out;

fail:
	(void) tapi_deinit(handle);

out:
	if (interfaces != NULL)
		g_list_free_full(interfaces, g_object_unref);

	if (objects != NULL)
		g_list_free_full(objects, g_object_unref);

	if (obj_manager != NULL)
		g_object_unref (obj_manager);

	return handle;
}

EXPORT_API TelReturn tapi_deinit(TelHandle *handle)
{
	if (handle == NULL) {
		err("TAPI handle is NULL");
		return TEL_RETURN_INVALID_PARAMETER;
	}

	if (handle->modem_proxy) {
		if (handle->modem_property_handler > 0)
			g_signal_handler_disconnect(handle->modem_proxy,
				handle->modem_property_handler);

		g_object_unref(handle->modem_proxy);
	}

	if (handle->sim_proxy) {
		if (handle->sim_property_handler > 0)
			g_signal_handler_disconnect(handle->sim_proxy,
				handle->sim_property_handler);

		g_object_unref(handle->sim_proxy);
	}

	if (handle->network_proxy) {
		if (handle->network_signal_handler > 0)
			g_signal_handler_disconnect(handle->network_proxy,
				handle->network_signal_handler);

		if (handle->network_property_handler > 0)
			g_signal_handler_disconnect(handle->network_proxy,
				handle->network_property_handler);

		g_object_unref(handle->network_proxy);
	}


	if (handle->call_proxy) {
		if (handle->call_signal_handler > 0)
			g_signal_handler_disconnect(handle->call_proxy,
				handle->call_signal_handler);

		g_object_unref(handle->call_proxy);
	}

	if (handle->ss_proxy) {
		if (handle->ss_signal_handler > 0)
			g_signal_handler_disconnect(handle->ss_proxy,
				handle->ss_signal_handler);

		g_object_unref(handle->ss_proxy);
	}

	if (handle->sms_proxy) {
		if (handle->sms_signal_handler > 0)
			g_signal_handler_disconnect(handle->sms_proxy,
				handle->sms_signal_handler);

		if (handle->sms_property_handler > 0)
			g_signal_handler_disconnect(handle->sms_proxy,
				handle->sms_property_handler);

		g_object_unref(handle->sms_proxy);
	}

	if (handle->phonebook_proxy) {
		if (handle->phonebook_signal_handler > 0)
			g_signal_handler_disconnect(handle->phonebook_proxy,
				handle->phonebook_signal_handler);

		g_object_unref(handle->phonebook_proxy);
	}

	if (handle->sat_proxy) {
		if (handle->sat_signal_handler > 0)
			g_signal_handler_disconnect(handle->sat_proxy,
				handle->sat_signal_handler);

		g_object_unref(handle->sat_proxy);
	}

	if (handle->sap_proxy) {
		if (handle->sap_signal_handler > 0)
			g_signal_handler_disconnect(handle->sap_proxy,
				handle->sap_signal_handler);

		g_object_unref(handle->sap_proxy);
	}

	if (handle->gps_proxy) {
		if (handle->gps_signal_handler > 0)
			g_signal_handler_disconnect(handle->gps_proxy,
				handle->gps_signal_handler);

		g_object_unref(handle->gps_proxy);
	}

	if (handle->evt_table)
		g_hash_table_destroy(handle->evt_table);

	g_free(handle->cp_name);
	g_free(handle);

	return TEL_RETURN_SUCCESS;
}
