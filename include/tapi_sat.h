/*
 * libslp-tapi
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd. All rights reserved.
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

#ifndef _TAPI_SAT_H_
#define _TAPI_SAT_H_

#include <tapi_type.h>

__BEGIN_DECLS

int tapi_get_sat_main_menu_info(TapiHandle *handle, TelSatSetupMenuInfo_t* pMainMenu);
int tapi_send_ui_display_status(TapiHandle *handle, int command_id, TelSatUiDisplayStatusType_t status);
int tapi_send_sat_ui_user_confirm(TapiHandle *handle, TelSatUiUserConfirmInfo_t * pUserConfirmData);
int tapi_send_sat_app_exec_result(TapiHandle handle, TelSatAppsRetInfo_t* pAppRetInfo);

int tapi_sat_req_menu_selection(TapiHandle *handle, const TelSatMenuSelectionReqInfo_t *menu_select_data, tapi_response_cb callback, void *user_data);
int tapi_download_sat_event(TapiHandle *handle, const TelSatEventDownloadReqInfo_t *event_data, tapi_response_cb callback, void *user_data);

__END_DECLS

#endif /* _TAPI_SAT_H_ */
