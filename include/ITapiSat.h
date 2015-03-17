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

/**
 * @file ITapiSat.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SAT
 * @{
 */

#ifndef _ITAPI_SAT_H_
#define _ITAPI_SAT_H_

#include <tapi_common.h>
#include <TelSat.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Sends the user choice of the main menu options to the USIM.
 *
 * @details <b> Notes: </b>
 *          A set of possible menu options are supplied by the USIM
 *          using the proactive command SET UP MENU. The telephony server receives the command
 *          and publishes this information. The SAT UI application should list the menu when it is initially launched.
 *          If the user subsequently chooses an option, then the SAT UI application replies to
 *          the command with the user's choice using this API. \n
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below. \n
 *
 *          <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> SAT-UI.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks You must not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pMenuSelect #TelSatMenuSelectionReqInfo_t contains information like which SAT menu item has been selected or whether help is required
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS - indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer #TapiResult_t).
 *
 * @pre This function is supposed to be called after getting the #TAPI_EVENT_SAT_SETUP_MENU_IND event from the telephony server.
 *
 * @see tel_get_sat_main_menu_info()
 */
int tel_select_sat_menu(TapiHandle *handle, const TelSatMenuSelectionReqInfo_t* pMenuSelect, tapi_response_cb callback, void *user_data);

/**
 * @brief Downloads SAT events to the USIM.
 *
 * @details <b> Notes: </b>
 *          A set of events for the terminal to monitor can be supplied by the USIM using the proactive command SET UP EVENT
 *          LIST. If the USIM has sent this command, and an event which is part of the list subsequently occurs, the terminal
 *          informs the USIM using this function, relevant for that event.
 *          If the USIM commands to monitor a browser termination event, the SAT-UI application has to call this function. \n
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below. \n
 *
 *          <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> SAT-UI \n
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks You must not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pEventData #TelSatEventDownloadReqInfo_t contains the necessary parameters like event types and information associated with it
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully, \n
 *         else it will return failure and an error code (Refer #TapiResult_t).
 *
 * @pre A SET UP EVENT LIST proactive command supplies a set of events to monitor.
 */
int tel_download_sat_event(TapiHandle *handle, const TelSatEventDownloadReqInfo_t*  pEventData, tapi_response_cb callback, void *user_data);

/**
 * @brief Sends the UI display status of the alpha identifier of a specific proactive command to the Telephony Server.
 *
 * @details When SAT-UI receives a proactive command, SAT-UI should draw a UI for the relevant command.
 *          As it completes, SAT-UI informs the USIM by calling this function. Afterwards, the USIM is getting ready to send another command. \n
 *
 *          This function makes a Dbus method call to the Telephony Server and gets an immediate feedback. \n
 *
 *          <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> SAT-UI \n
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks You must not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] commandId The proactive command ID from the application
 *
 * @param[in] status #TelSatUiDisplayStatusType_t contains the display status(SUCCESS/FAIL)
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS - indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer #TapiResult_t).
 *
 * @pre The display request for the alpha identifier of a Proactive Command should be sent by the Telephony Server.
 *
 * @post If the display status is SUCCESS, the Telephony Server sends a request to the application for Proactive Command Execution.
 *       If the display status is FAIL, the Telephony Server sends a Terminal Response for the Proactive Command.
 */
int tel_send_sat_ui_display_status(TapiHandle *handle, int commandId, TelSatUiDisplayStatusType_t status);

/**
 * @brief Sends UI User confirmation data for a specific Proactive Command to the Telephony Server.
 *
 * @details In case the proactive commands need user response, SAT-UI can send it using this function.
 *          The response can be 'OK', 'Cancel', 'Move Back', and 'End Session'. Upon this response, the USIM can send
 *          a proactive command subsequently to indicate the next UI action. \n
 *
 *          This function makes a Dbus method call to the Telephony Server and gets an immediate feedback. \n
 *
 *          <b> Sync (or) Async: </b> This is a synchronous API. \n
 *
 *          <b> Prospective Clients: </b> SAT-UI. \n
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks You must not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pUserConfirmData #TelSatUiUserConfirmInfo_t contains specific user confirmation data
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS - indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer #TapiResult_t).
 *
 * @pre The User Confirmation request for a specific Proactive Command should be sent to the application by the Telephony Server.
 *
 * @post If the User Confirmation is positive, the Telephony Server sends a request to the application for Proactive Command Execution.
 *       If the User Confirmation is negative, the Telephony Server sends a Terminal Response for the Proactive Command.
 */
int tel_send_sat_ui_user_confirm(TapiHandle *handle, TelSatUiUserConfirmInfo_t *pUserConfirmData);

/**
 * @brief Provides SAT(Sim Application toolkit) Main Menu information for SAT-UI.
 *
 * @details Once the USIM supplies the SET UP MENU proactive command, the telephony server not only publishes the
 *          #TAPI_EVENT_SAT_SETUP_MENU_IND event but also caches the menu information.
 *          The SAT-UI application can get the menu list using this function. \n
 *
 *          This function makes a Dbus method call to the Telephony Server and gets an immediate feedback. \n
 *
 *         <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *         <b> Prospective Clients: </b> SAT-UI.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks You must not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] pMainMenu #TelSatSetupMenuInfo_t contains all menu related information which is required such as a menu title, an icon, an item count, and so on
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS - indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer #TapiResult_t).
 *
 * @pre When SAT SIM is inserted we can get meaningful data. Without SAT SIM, null is returned.
 *
 * @see tel_select_sat_menu()
 */
int tel_get_sat_main_menu_info(TapiHandle *handle, TelSatSetupMenuInfo_t *pMainMenu);

/**
 * @brief Provides the Operation result(s) for Proactive Command execution by the Application(s) to the Telephony Server.
 *
 * @details The USIM commands the terminal to do some predefined action, such as sending a short message,
 *          making a voice call, launching an Internet browser, and so on. Those actions are defined by 3GPP TS31.111.
 *          Once an application executes the requested action by USIM, it reports the operation result to the USIM using this function. \n
 *
 *          This function makes a Dbus method call to the Telephony Server and gets an immediate feedback. \n
 *
 *          <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> Embeded applications which are predefined by 3GPP TS31.111.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks You must not use this function. This function is dedicated to the SAT UI embedded application only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] pAppRetInfo #TelSatAppsRetInfo_t contains an execution result of a specific proactive command by the application
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS - indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer #TapiResult_t).
 *
 * @pre The Proactive Command execution request should be sent by the Telephony Server to SAT related applications.
 */
int tel_send_sat_app_exec_result(TapiHandle *handle, TelSatAppsRetInfo_t *pAppRetInfo);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_SAT_H_ */

/**
 * @}
 */
