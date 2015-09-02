/*
 * libslp-tapi
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>

#include <tapi_common.h>
#include <ITapiSat.h>
#include <TapiUtility.h>

#include "menu.h"
#include "sat.h"

TelSatSetupMenuInfo_t main_menu;

static void on_noti_setup_menu(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int local_index;
	TelSatSetupMenuInfo_t *setup_menu = NULL;

	msg("noti id (%s)", noti_id);

	if (!data) {
		msg("noti data is null");
		return;
	}

	setup_menu = (TelSatSetupMenuInfo_t *)data;
	msg("command id (%d)", setup_menu->commandId);
	msg("menu present (%d)", setup_menu->bIsMainMenuPresent);
	msg("menu title (%s)", setup_menu->satMainTitle);
	msg("item cnt (%d)", setup_menu->satMainMenuNum);
	for (local_index = 0; local_index < setup_menu->satMainMenuNum; local_index++)
		msg("item id(%d) (%s)", setup_menu->satMainMenuItem[local_index].itemId, setup_menu->satMainMenuItem[local_index].itemString);

	msg("menu help info (%d)", setup_menu->bIsSatMainMenuHelpInfo);
	msg("menu updated (%d)", setup_menu->bIsUpdatedSatMainMenu);
}

static void on_noti_display_text(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSatDisplayTextInd_t *display_text = NULL;

	msg("noti id (%s)", noti_id);

	if (!data) {
		msg("noti data is null");
		return;
	}

	display_text = (TelSatDisplayTextInd_t *)data;

	msg("command id (%d)", display_text->commandId);
	msg("display text (%s)", display_text->text.string);
	msg("string len(%d)", display_text->text.stringLen);
	msg("duration (%d)", display_text->duration);
	msg("high priority (%d)", display_text->bIsPriorityHigh);
	msg("user response required(%d)", display_text->bIsUserRespRequired);
}

static void on_noti_select_item(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	int local_index;
	TelSatSelectItemInd_t *select_item = NULL;

	msg("noti id (%s)", noti_id);

	if (!data) {
		msg("noti data is null");
		return;
	}

	select_item = (TelSatSelectItemInd_t *)data;

	msg("command id (%d)", select_item->commandId);
	msg("help info(%d)", select_item->bIsHelpInfoAvailable);
	msg("selected item string(%s)", select_item->text.string);
	msg("string len(%d)", select_item->text.stringLen);
	msg("default item local_index(%d)", select_item->defaultItemIndex);
	msg("item count(%d)", select_item->menuItemCount);
	for (local_index = 0; local_index < select_item->menuItemCount; local_index++) {
		msg("item local_index(%d) id(%d) len(%d) str(%s)", local_index,
			select_item->menuItem[local_index].itemId, select_item->menuItem[local_index].textLen, select_item->menuItem[local_index].text);
	}
}

static void on_noti_setup_call(TapiHandle *handle, const char *noti_id, void *data, void *user_data)
{
	TelSatSetupCallIndCallData_t *setup_call = NULL;

	msg("noti id (%s)", noti_id);

	if (!data) {
		msg("noti data is null");
		return;
	}

	setup_call = (TelSatSetupCallIndCallData_t *)data;

	msg("command id (%d)", setup_call->commandId);
	msg("call type(%d)", setup_call->calltype);
	msg("dispText (%s)", setup_call->dispText.string);
	msg("callNumber(%s)", setup_call->callNumber.string);
	msg("duration (%d)", setup_call->duration);
}


static void on_resp_select_menu(TapiHandle *handle, int result, void *data, void *user_data)
{
	msg("");
	msg("select menu item result(%d)", result);
}

static int run_sat_get_main_menu_info(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int result;

	msg("call get main menu info()");

	result = tel_get_sat_main_menu_info(handle, &main_menu);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	msg("success to get main menu");
	return 0;
}

static int run_sat_select_menu(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelSatMenuSelectionReqInfo_t selected_menu;
	int result, local_index = 0;
	int item_id;

	msg("call select menu()");

	for (local_index = 0; local_index < main_menu.satMainMenuNum; local_index++)
		msg("item id(%d) (%s)", main_menu.satMainMenuItem[local_index].itemId, main_menu.satMainMenuItem[local_index].itemString);

	msg("select item >>> ");
	if (scanf("%d", &item_id) < 0) {
		msg("scanf() failed.");
		return 0;
	}

	selected_menu.bIsHelpRequested = 0;
	selected_menu.itemIdentifier = item_id;

	msg("selected item id (%d)", selected_menu.itemIdentifier);
	result = tel_select_sat_menu(handle, &selected_menu, on_resp_select_menu, NULL);
	if (result != TAPI_API_SUCCESS)
		msg("failed. (result = %d)", result);

	return 0;
}

struct menu_data menu_sat_common[] = {
	{ "1", "GET Main Menu Info", NULL, run_sat_get_main_menu_info, NULL},
	{ "2", "SELECT Main Menu", NULL, run_sat_select_menu, NULL},
/*	{ "2a", "SET PLMN Selection mode (Automatic)", menu_net_set_plmn_mode_automatic, NULL, NULL},
	{ "2s", "SET PLMN Selection mode (Manual)", menu_net_set_plmn_mode_manual, NULL, NULL},
	{ "2g", "GET PLMN Selection mode", menu_net_get_plmn_mode, NULL, NULL},
	{ "3s", "SET Service Domain", menu_net_set_service_domain, NULL, NULL},
	{ "3g", "GET Service Domain", menu_net_get_service_domain, NULL, NULL},
	{ "4s", "SET Band", menu_net_set_band, NULL, NULL},
	{ "4g", "GET Band", menu_net_get_band, NULL, NULL},
	{ "5s", "SET Preferred PLMN", menu_net_set_preferred_plmn, NULL, NULL},
	{ "5g", "GET Preferred PLMN", menu_net_get_preferred_plmn, NULL, NULL},
	{ "6", "SET Cancel manual search", menu_net_set_cancel_manual_search, NULL, NULL},
	{ "7", "GET Serving network", menu_net_get_serving_network, NULL, NULL},*/
	{ NULL, NULL, },
};

struct menu_data menu_sat[] = {
		{"1", "3GPP(WCDMA/GSM/LTE)" , menu_sat_common, NULL, NULL},
		{"2", "3GPP2(CDMA)" , menu_sat_common, NULL, NULL},
		{ NULL, NULL, },
};

void register_sat_event(TapiHandle *handle)
{
	int ret;

	/* SAT */
	ret = tel_register_noti_event(handle, TAPI_NOTI_SAT_SETUP_MENU, on_noti_setup_menu, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SAT_DISPLAY_TEXT, on_noti_display_text, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SAT_SELECT_ITEM, on_noti_select_item, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SAT_SETUP_CALL, on_noti_setup_call, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	/*	ret = tel_register_noti_event(handle, TAPI_NOTI_SAT_GET_INKEY, on_noti_get_inkey, NULL);
	ret = tel_register_noti_event(handle, TAPI_NOTI_SAT_GET_INPUT, on_noti_get_input, NULL);*/
}
