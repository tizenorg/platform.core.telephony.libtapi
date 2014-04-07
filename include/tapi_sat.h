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

/**
*  @addtogroup TAPI_SAT
*  @{
*
*  @file tapi_sat.h
*  @brief TAPI SAT Interface
*/

#ifndef __TAPI_SAT_H__
#define __TAPI_SAT_H__

#include <tapi.h>
#include <tel_sat.h>

#ifdef __cplusplus
extern "C"
{
#endif

 /**
 * @brief Sends the user choice of the main menu options to the USIM.
 *
 * @par Notes:
 * A set of possible menu options is supplied by the USIM
 * using the proactive command SET UP MENU. Telephony server receives the command
 * and publishes this information.SAT UI application should list the menu when it initially launched.
 * If the user subsequently chooses an option, then SAT UI application replies
 * the command with user's choice using this API.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
 * @par Warning:
 * Do not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @param [in] handle - #TelHandle obtained from tapi_init()
 * @param [in] menu_select - #TelSatMenuSelectionReqInfo contains information like which SAT menu item has been selected or whether Help is required.
 * @param [in] callback - Response callback
 * @param [in] user_data - user_data for user specification.
 *
 * @par Async Response Message:
 * - The event associated is TEL_EVENT_SAT_MENU_SELECTION_CNF and the Asynchronous return status is indicated by #TelSatEnvelopeResp.
 *
 * @pre
 *  - This function supposed to be called after getting TEL_EVENT_SAT_SETUP_MENU_IND event from telephony server\n
 *
 * @post
 *  - None
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see tapi_sat_get_main_menu_info
 *
 * @code
 * #include <tapi_sat.h>
 * TelHandle *handle;  //obtained from tapi_init()
 * TapiResponseCb callback; //Response callback
 * void *user_data; // user_data for user specification
 * TelReturn ret_status = TEL_RETURN_SUCCESS;
 * TelSatMenuSelectionReqInfo selected_menu;
 * selected_menu.item_identifier = '1'; //selected menu num
 * selected_menu.b_is_help_requested = 0;
 * ret_status = tapi_sat_select_menu(handle, &selected_menu, callback, user_data);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
TelReturn tapi_sat_select_menu(TelHandle *handle, const TelSatMenuSelectionReqInfo* menu_select, TapiResponseCb callback, void *user_data);

 /**
 * @brief  Download SAT events to USIM
 *
 * @par Notes:
 * A set of events for the terminal to monitor can be supplied by the USIM using the proactive command SET UP EVENT
 * LIST. If the USIM has sent this command, and an event which is part of the list subsequently occurs, the terminal
 * informs the USIM using this function, relevant for that event.
 * If USIM commands to monitor a browser termination event, the SAT-UI application has to call this function.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
 * @par Warning:
 * Do not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @param [in] handle - #TelHandle obtained from tapi_init()
 * @param [in] event_data - #TelSatEventDownloadReqInfo contains the necessary parameters like event type and information associated with it.
 * @param [in] callback - Response callback
 * @param [in] user_data - user_data for user specification.
 *
 * @par Async Response Message:
 * - The event associated is TEL_EVENT_SAT_EVENT_DOWNLOAD_CNF and the Asynchronous return status is indicated by #TelSatEnvelopeResp.
 *
 * @pre
 *  - A SET UP EVENT LIST proactive command supplies a set of event to monitor.
 *
 * @post
 *  - None.
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see None
 *
 * @code
 * #include <tapi_sat.h>
 * TelHandle *handle;  //obtained from tapi_init()
 * TapiResponseCb callback; //Response callback
 * void *user_data; // user_data for user specification
 * TelReturn ret_status = TEL_RETURN_SUCCESS;
 * TelSatEventDownloadReqInfo event_data;
 * event_data.event_download_type = TEL_SAT_EVENT_IDLE_SCREEN_AVAILABLE;
 * event_data.u.b_idle_screen_available = 1; //event occur or not
 * ret_status = tapi_sat_download_event(handle, &event_data, callback, user_data);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
TelReturn tapi_sat_download_event(TelHandle *handle, const TelSatEventDownloadReqInfo*  event_data, TapiResponseCb callback, void *user_data);

/**
 * @brief  Send the UI display status of the alpha identifier of a specific proactive command to Telephony Server.
 *
 * When SAT-UI receives a proactive command, SAT-UI should draw a UI for relevant command.
 * As it completes , SAT-UI inform USIM with this function. Afterwards, USIM is getting ready to send another commands.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 *
 * @par Warning:
 * Do not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @param [in] handle - #TelHandle obtained from tapi_init()
 * @param [in] command_id - Specific proactive command id from the Application
 * @param [in] status - #TelSatUiDisplayStatusType contain display status(SUCCESS/FAIL).
 *
 * @par Async Response Message:
 * - None.
 *
 * @pre
 *  - Display request for the alpha identifier of a Proactive Command should be sent by Telephony Server.
 *
 * @post
 *  - If the display status is SUCCESS Telephony Server sends a request to application for Proactive Command Execution.
 *  - If the display status is FAIL Telephony Server sends Terminal Response for the Proactive Command.
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see None
 *
 * @code
 * #include <tapi_sat.h>
 * TelHandle *handle;  //obtained from tapi_init()
 * int command_id; //this value should be server given value
 * TelReturn ret_status = TEL_RETURN_SUCCESS;
 * ret_status = tapi_sat_send_ui_display_status(handle, command_id, TEL_SAT_DISPLAY_SUCCESS);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
TelReturn tapi_sat_send_ui_display_status(TelHandle *handle, int command_id, TelSatUiDisplayStatusType status);

 /**
 * @brief  This function sends the UI User confirmation data for a specific Proactive Command to the Telephony Server.
 *
 * In case that the proactive commands need user response, SAT-UI can send it using this function.
 * The response can be 'OK', 'Cancel', 'Move Back' and 'End Session'. Upon this response, USIM can send
 * a proactive command subsequently to indicate next UI action.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 *
 * @par Warning:
 * Do not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @param [in] handle - #TelHandle obtained from tapi_init()
 * @param [in] pUserConfirmData -#TelSatUiUserConfirmInfo contains Specific user confirmation data.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User Confirmation request for a specific Proactive Command should be sent to application by Telephony Server.
 *
 * @post
 *  - If the User Confirmation is positive Telephony Server sends a request to application for Proactive Command Execution.
 *  - If the User Confirmation is negative Telephony Server sends Terminal Response for the Proactive Command.
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see None
 *
 * @code
 * #include <tapi_sat.h>
 * TelHandle *handle;  //obtained from tapi_init()
 * TelReturn ret_status = TEL_RETURN_SUCCESS;
 * TelSatUiUserConfirmInfo cfm_data;
 * cfm_data.command_id = '1'; //this value should be server given value
 * cfm_data.command_type = TEL_SAT_CMD_TYPE_SETUP_CALL;
 * cfm_data.key_type = TEL_SAT_USER_CONFIRM_YES;
 * ret_status = tapi_sat_send_ui_user_confirm(handle, &cfm_data);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
TelReturn tapi_sat_send_ui_user_confirm(TelHandle *handle, TelSatUiUserConfirmInfo *user_confirm_data);

 /**
 * @brief  This function provides SAT(Sim Application toolkit) Main Menu information for SAT-UI.
 *
 * Once the USIM supplies the SET UP MENU proactivae command, telephony server not only publish
 * TAPI_EVENT_SAT_SETUP_MENU_IND event but also caches the menu information.
 * The SAT-UI applicatoin can get the menu list using this function.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 *
 * @par Warning:
 * Do not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @param [in] handle - #TelHandle obtained from tapi_init()
 * @param [out] main_menu - #TelSatSetupMenuInfo contain all menu related information which are required like menu title, icon, item count, etc.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - When SAT SIM is inserted. we can get meaningful data. without SAT SIM, Null is returned
 *
 * @post
 *  - None.
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see tapi_sat_select_menu
 *
 * @code
 * #include <tapi_sat.h>
 * TelHandle *handle;  //obtained from tapi_init()
 * TelReturn ret_status = TEL_RETURN_SUCCESS;
 * TelSatSetupMenuInfo menu; //this struct will be pull up with SIM menu info
 * ret_status = tapi_sat_get_main_menu_info(handle, &menu);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
TelReturn tapi_sat_get_main_menu_info(TelHandle *handle, TelSatSetupMenuInfo *main_menu);

 /**
 * @brief  This API provides the Operation result(s) for the Proactive Command execution by the Application(s) to the Telephony Server.
 *
 * The USIM commands the terminal to do some predefined action, such as sending short message,
 * making a voice call, launching an Internet browser and so on. Those actions are defined by 3GPP TS31.111.
 * Once a application executes the requested action by USIM, it reports the operation result to USIM using this function.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 *
 * @par Warning:
 * Do not use this function. This function is dedicated to the SAT UI embedded application only.
 **
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @param [in] handle - #TelHandle obtained from tapi_init()
 * @param [out] app_ret_info - #TelSatAppsRetInfo contains execution result of a specific proactive command by application.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - Proactive Command execution request should be sent by Telephony Server to SAT related applications.
 *
 * @post
 *  - None.
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *
 * @par Prospective Clients:
 * Embeded applications which are predefined by 3GPP TS31.111
 *
 * @see None
 *
 * @code
 * #include <tapi_sat.h>
 * TelHandle *handle;  //obtained from tapi_init()
 * TelReturn ret_status = TEL_RETURN_SUCCESS;
 * TelSatAppsRetInfo app_ret;
 * app_ret.command_type = TEL_SAT_CMD_TYPE_SETUP_CALL;
 * app_ret.command_id = 1; //this value should be server given value
 * app_ret.apps_ret.setup_call.resp = TEL_SAT_R_SUCCESS;
 * ret_status = tapi_sat_send_app_exec_result(handle, &app_ret);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
TelReturn tapi_sat_send_app_exec_result(TelHandle *handle, TelSatAppsRetInfo *app_ret_info);

#ifdef __cplusplus
}
#endif

#endif	/* __TAPI_SAT_H__ */

/**
 * @}
 */
