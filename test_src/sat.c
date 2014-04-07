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

#include <tapi.h>
#include <tapi_sat.h>
#include <tapi_events.h>

#include "menu.h"
#include "sat.h"

TelSatSetupMenuInfo main_menu;

/* Event Callback */
static void on_noti_sat_setup_menu(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int index;
	TelSatSetupMenuInfo *setup_menu = NULL;

	msg("noti id (%s)", noti_id);

	if(!data){
		msg("noti data is null");
		return;
	}

	setup_menu = (TelSatSetupMenuInfo *)data;
	msg("command id (%d)", setup_menu->command_id);
	msg("menu present (%d)", setup_menu->is_main_menu_present);
	msg("menu title (%s)", setup_menu->sat_main_title);
	msg("item cnt (%d)", setup_menu->sat_main_menu_item_count);
	for(index=0;index < setup_menu->sat_main_menu_item_count; index++){
		msg("item id(%d) (%s)", setup_menu->sat_main_menu_item[index].item_id, setup_menu->sat_main_menu_item[index].item_string);
	}
	msg("menu help info (%d)", setup_menu->is_sat_main_menu_help_info);
	msg("menu updated (%d)", setup_menu->is_updated_sat_main_menu);
}

static void on_noti_sat_display_text(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSatDisplayTextInd *display_text = NULL;

	msg("noti id (%s)", noti_id);

	if(!data){
		msg("noti data is null");
		return;
	}

	display_text = (TelSatDisplayTextInd *)data;

	msg("command id (%d)", display_text->command_id);
	msg("display text (%s)", display_text->text.string);
	msg("string len(%d)", display_text->text.string_len);
	msg("duration (%d)", display_text->duration);
	msg("high priority (%d)", display_text->is_priority_high);
	msg("user response required(%d)", display_text->is_user_resp_required);
}

static void on_noti_sat_select_item(TelHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int index;
	TelSatSelectItemInd *select_item = NULL;

	msg("noti id (%s)", noti_id);

	if(!data){
		msg("noti data is null");
		return;
	}

	select_item = (TelSatSelectItemInd *)data;

	msg("command id (%d)", select_item->command_id);
	msg("help info(%d)", select_item->is_help_info_available);
	msg("selected item string(%s)", select_item->text.string);
	msg("string len(%d)", select_item->text.string_len);
	msg("default item index(%d)", select_item->default_item_index);
	msg("item count(%d)", select_item->menu_item_count);
	for(index=0;index < select_item->menu_item_count; index++){
		msg("item index(%d) id(%d) len(%d) str(%s)", index,
			select_item->menu_item[index].item_id, select_item->menu_item[index].text_len, select_item->menu_item[index].text);
	}
}

/* Requests and Responses */
static void on_sat_select_menu(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msg("select menu item result(%d)", result);
}

static int run_sat_get_main_menu_info(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn result;

	msg("call tapi_sat_get_main_menu_info()");

	result = tapi_sat_get_main_menu_info(handle, &main_menu);
	CHECK_RT(result);

	msg("success to get main menu");
	return 0;
}

static int run_sat_select_menu(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelSatMenuSelectionReqInfo selected_menu;
	int index=0, item_id;
	TelReturn result;

	msg("call tapi_sat_select_menu()");

	for(index=0;index < main_menu.sat_main_menu_item_count; index++){
		msg("item id(%d) (%s)", main_menu.sat_main_menu_item[index].item_id, main_menu.sat_main_menu_item[index].item_string);
	}
	msg("select item >>> ");
	if (scanf("%d", &item_id) < 0) {
		msg("scanf() failed.");
		return 0;
	}

	selected_menu.is_help_requested = 0;
	selected_menu.item_identifier = item_id;

	msg("selected item id (%d)", selected_menu.item_identifier);
	result = tapi_sat_select_menu(handle, &selected_menu, on_sat_select_menu, NULL);
	CHECK_RT(result);

	return 0;
}

/* SAT Menu */
struct menu_data menu_sat[] = {
	{ "1", "GET Main Menu Info", NULL, run_sat_get_main_menu_info, NULL},
	{ "2", "SELECT Main Menu", NULL, run_sat_select_menu, NULL},
	{ NULL, NULL, },
};

/* Register Events */
void register_sat_event(TelHandle *handle)
{
	TelReturn ret;

	ret = tapi_register_event_id(handle, TEL_NOTI_SAT_SETUP_MENU, on_noti_sat_setup_menu, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TAPI_NOTI_SAT_SETUP_MENU - event register failed [%d]", ret);
	ret = tapi_register_event_id(handle, TEL_NOTI_SAT_DISPLAY_TEXT, on_noti_sat_display_text, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TAPI_NOTI_SAT_DISPLAY_TEXT - event register failed [%d]", ret);
	ret = tapi_register_event_id(handle, TEL_NOTI_SAT_SELECT_ITEM, on_noti_sat_select_item, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TAPI_NOTI_SAT_SELECT_ITEM - event register failed [%d]", ret);
}
