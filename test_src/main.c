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
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <tapi.h>

#include "menu.h"
#include "network.h"
#include "sat.h"
#include "sms.h"
#include "sim.h"
#include "phonebook.h"
#include "modem.h"
#include "call.h"
#include "ss.h"
#include "gps.h"

#define SIM_SEL_MENU_KEY_COUNT 2

extern struct menu_data menu_modem[];
extern struct menu_data menu_sat[];
extern struct menu_data menu_net[];
extern struct menu_data menu_sms[];
extern struct menu_data menu_sim[];
extern struct menu_data menu_phonebook[];
extern struct menu_data menu_ss[];
extern struct menu_data menu_call[];
extern struct menu_data menu_gps[];

TelHandle *handle = NULL;
char **cp_list = NULL;
int cp_count = 0;


static char data_subscription_type[MENU_DATA_SIZE + 1] = "";
/* TODO
static char data_property_int[MENU_DATA_SIZE + 1] = "org.tizen.telephony.Modem:power";
static char data_property_string[MENU_DATA_SIZE + 1] = "org.tizen.telephony.Network:network_name";
*/

static int get_modem_info(MManager *mm, struct menu_data *menu)
{
	int i = 0;

	while (cp_list[i]) {
		msg("[%02d] strlen(%d), '%s'", i, strlen(cp_list[i]), cp_list[i]);
		i++;
	}

	return 0;
}

static int init(MManager *mm, struct menu_data *menu)
{
	int subs_type = atoi(data_subscription_type);

	if (handle) {
		msg("handle already exists");
		return 0;
	}

	if (cp_list[subs_type-1]) {
		msg("call tapi_init(%s)", cp_list[subs_type-1]);
		handle = tapi_init(cp_list[subs_type-1]);
	}

	if (!handle)
		msg("handle is null");

	menu_manager_set_user_data(mm, handle);

	return 0;
}

static int deinit(MManager *mm, struct menu_data *menu)
{
	TelReturn ret;
	if (!handle) {
		msg("handle is null");
		return 0;
	}

	msg("call tapi_deinit()");
	ret = tapi_deinit(handle);
	if (ret != TEL_RETURN_SUCCESS)
	handle = NULL;

	menu_manager_set_user_data(mm, handle);

	return 0;
}

#if 0
static int get_property_int(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	int result_int = -1;
	int ret;

	msg("call tapi_get_property_int()");

	ret = tapi_get_property_int(handle, data_property_int, &result_int);
	if (ret != TAPI_API_SUCCESS) {
		msg("failed. (return = %d)", ret);
	}

	msg(" - result = %d", result_int);

	return 0;
}

static int get_property_string(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	char *result_str = NULL;
	int ret;

	msg("call tapi_get_property_string()");

	ret = tapi_get_property_string (handle, data_property_string, &result_str);
	if (ret != TAPI_API_SUCCESS) {
		msg("failed. (return = %d)", ret);
	}

	msg(" - result = %s", result_str);

	if (result_str)
		free (result_str);

	return 0;
}
#endif

static struct menu_data menu_common[] = {
	{ "1", "get modem info", NULL, get_modem_info, NULL},
	{ "2", "tapi_init", NULL, init, NULL},
	{ "3", "tapi_deinit", NULL, deinit, NULL},


/*Todo */
#if 0
	{ "4", "tapi_get_property_int",		NULL,	get_property_int,	NULL},
	{ "4s", " - property",	NULL,	NULL,	data_property_int},
	{ "5", "tapi_get_property_string",		NULL,	get_property_string,	NULL},
	{ "5s", " - property",	NULL,	NULL,	data_property_string},
#endif
	{ NULL, NULL, },
};

static struct menu_data menu_main[] = {
	{ "1", "Common", menu_common, NULL, NULL},
	{ "2", "Modem", menu_modem, NULL, NULL},
	{ "3", "Network", menu_net, NULL, NULL},
	{ "4", "SIM", menu_sim, NULL, NULL},
	{ "5", "CALL", menu_call, NULL, NULL},
	{ "6", "SMS", menu_sms, NULL, NULL},
	{ "7", "SS", menu_ss, NULL, NULL},
	{ "8", "Phonebook", menu_phonebook, NULL, NULL},
	{ "9", "SAT", menu_sat, NULL, NULL},
	{ "a", "GPS", menu_gps, NULL, NULL},
	{ NULL, NULL, },
};

static int __select_handle_register_event (MManager *mm, struct menu_data *menu)
{
	int subs_type = atoi(data_subscription_type);

	if (subs_type < 1 || subs_type > cp_count) {
		msg("Please select an existing CP Name");
		return RET_FAILURE;
	}

	if (handle) {
		tapi_deinit(handle);
		handle = NULL;
	}

	handle = tapi_init(cp_list[subs_type-1]);

	if (!handle) {
		msg("[SIM%d] tapi_init(%s) - FAILED!!!.", subs_type, cp_list[subs_type-1]);
		return RET_FAILURE;
	} else {
		msg("[SIM%d] tapi_init(%s) - SUCCESS!!!.", subs_type, cp_list[subs_type-1]);
	}

	menu_manager_set_user_data(mm, handle);

	register_network_event(handle);
	register_modem_event(handle);
	register_sim_event(handle);
	register_sat_event(handle);
	register_sms_event(handle);
	register_phonebook_event(handle);
	register_call_event(handle);
	register_ss_event(handle);
	register_gps_event(handle);

	return RET_SUCCESS;
}


static gboolean __create_sim_selection_menu(struct menu_data sim_selection_menu[SIM_SEL_MENU_KEY_COUNT+1])
{
	unsigned int total_cp_len = 0;
	unsigned int title_len = 0;
	unsigned int index_count = 0;
	unsigned int i = 0;

	if( tapi_get_cp_name_list(&cp_list) != TEL_RETURN_SUCCESS) {
		msg("tapi_get_cp_name_list() failed. Exiting!");
		return FALSE;
	}

	if (!cp_list) {
		msg("cp_list is null");
		return FALSE;
	}

	while (cp_list[i]) {
		total_cp_len += strlen(cp_list[i]);
		cp_count++;
		i++;
	}

	/**
	 * Available CPs are displayed as below (in UI) -
	 *	   Modems(1=Modem0,2=Modem1,3=Modem2)
	 *
	 * Title length is calculated as,
	 *	   title_len = total_cp_len + (2*cp_count) + (cp_count-1) + 8 + 1;
	 * where,
	 *	   total_cp_len = strlen(Modem0)+strlen(Modem1)+ strlen(Modem2)
	 *	   (2*cp_count) = To print "1="  "2="  "3="
	 *	   cp_count-1	= Number of commas
	 *	   8 = Modems()
	 *	   1 = NULL termination
	 */
	title_len = total_cp_len + (2*cp_count) + (cp_count-1) + 8 + 1;
	sim_selection_menu[0].title = (char *)g_try_malloc0(title_len);
	index_count += sprintf(&sim_selection_menu[0].title[index_count], "Modems(");

	i=0;
	while (cp_list[i]) {
		if (i != 0)
			sim_selection_menu[0].title[index_count++] = ',';

		index_count += sprintf(&sim_selection_menu[0].title[index_count], "%d=", (i+1));
		index_count += sprintf(&sim_selection_menu[0].title[index_count], "%s", cp_list[i]);
		i++;
	}

	if (index_count <= title_len - 2) {
		sim_selection_menu[0].title[index_count++] = ')';
		sim_selection_menu[0].title[index_count] = '\0';
	} else {
		msg("Invalid count");
		return FALSE;
	}

	sim_selection_menu[0].key = "1";
	sim_selection_menu[0].sub_menu = NULL;
	sim_selection_menu[0].callback = NULL;
	sim_selection_menu[0].data = data_subscription_type;

	sim_selection_menu[1].key = "2";
	sim_selection_menu[1].title = "run";
	sim_selection_menu[1].sub_menu = menu_main;
	sim_selection_menu[1].callback = __select_handle_register_event;
	sim_selection_menu[1].data = NULL;

	return TRUE;
}


int main(int arg, char **argv)
{
	GMainLoop *mainloop;
	GIOChannel *channel = g_io_channel_unix_new(STDIN_FILENO);
	MManager *manager;
	struct menu_data sim_selection_menu[SIM_SEL_MENU_KEY_COUNT+1] = { {NULL, NULL, }, };

	g_type_init();
	mainloop = g_main_loop_new(NULL, FALSE);

	msg("");
	msg("* Telephony Test application ");
	msg("* Build On: %s  %s", __DATE__, __TIME__);

	if (__create_sim_selection_menu(sim_selection_menu) == FALSE)
		goto OUT;

	manager = menu_manager_new(sim_selection_menu, mainloop);
	menu_manager_run(manager);

	g_io_add_watch(channel, (G_IO_IN | G_IO_ERR | G_IO_HUP | G_IO_NVAL), on_menu_manager_keyboard, manager);

	g_main_loop_run(mainloop);

OUT:
	g_strfreev(cp_list);
	g_free(sim_selection_menu[0].title);
	g_main_loop_unref(mainloop);
	msg("******* Bye bye *******");

	return 0;
}


