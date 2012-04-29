/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
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
* @open
* @ingroup		TelephonyAPI
* @addtogroup	SAT_TAPI	SAT
* @{
*
* @file ITapiSat.h

     @brief This file serves as a "C" header file defines functions for Tapi Sat Services.\n
      It contains a sample set of function prototypes that would be required by applications.

      Note: Telephony SAT functionality is message relaying from USIM application to SAT related applications.
 */

#ifndef _ITAPI_SAT_H_
#define _ITAPI_SAT_H_

#include <TelSat.h>
#include <TelDefines.h>
#include <TapiCommon.h>

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
 * @param [in] pMenuSelect
 * - #TelSatMenuSelectionReqInfo_t contains information like which SAT menu item has been selected or whether Help is required.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated is TAPI_EVENT_SAT_MENU_SELECTION_CNF and the Asynchronous return status is indicated by #TelSatEnvelopeResp_t.
 *
 * @pre
 *  - This function supposed to be called after getting TAPI_EVENT_SAT_SETUP_MENU_IND event from telephony server\n
 *
 * @post
 *  - None
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see tel_get_sat_main_menu_info
 *
 * @code
 * #include <ITapiSat.h>
 * int ret_status =0;
 * int pRequestID=0;
 * TelSatMenuSelectionReqInfo_t selected_menu;
 * selected_menu.itemIdentifier = '1'; //selected menu num
 * selected_menu.bIsHelpRequested = 0;
 * ret_status = tel_select_sat_menu(&selected_menu, &pRequestId);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
int tel_select_sat_menu(const TelSatMenuSelectionReqInfo_t* pMenuSelect, int* pRequestId);

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
 * @param [in] pEventData
 * - #TelSatEventDownloadReqInfo_t contains the necessary parameters like event type and information associated with it.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated is TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF and the Asynchronous return status is indicated by #TelSatEnvelopeResp_t.
 *
 * @pre
 *  - A SET UP EVENT LIST proactive command supplies a set of event to monitor.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see None
 *
 * @code
 * #include <ITapiSat.h>
 * int ret_status =0;
 * int pRequestID=0;
 * TelSatEventDownloadReqInfo_t pEventData;
 * pEventData.eventDownloadType = TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE;
 * pEventData.u.bIdleScreenAvailable = 1; //event occur or not
 * ret_status = tel_download_sat_event(&pEventData, &pRequestId);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
int tel_download_sat_event(const TelSatEventDownloadReqInfo_t*  pEventData, int* pRequestId);

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
 * @param [in] commandId
 * - Specific proactive command id from the Application
 *
 * @param [in] status
 * - #TelSatUiDisplayStatusType_t contain display status(SUCCESS/FAIL).
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
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see None
 *
 * @code
 * #include <ITapiSat.h>
 * int commandId = 1; //this value should be server given value
 * ret_status = 0;
 * ret_status = tel_send_ui_display_status(1, TAPI_SAT_DISPLAY_SUCCESS);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
int tel_send_ui_display_status(int commandId, TelSatUiDisplayStatusType_t status);

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
 *@param [in] pUserConfirmData
 * -#TelSatUiUserConfirmInfo_t contains Specific user confirmation data.
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
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see None
 *
 * @code
 * #include <ITapiSat.h>
 * int ret_status =0;
 * TelSatUiUserConfirmInfo_t cfm_data;
 * cfm_data.commandId = '1'; //this value should be server given value
 * cfm_data.commandType = TAPI_SAT_CMD_TYPE_SETUP_CALL;
 * cfm_data.keyType = TAPI_SAT_USER_CONFIRM_YES;
 * ret_status = tel_send_sat_ui_user_confirm(&cfm_data);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
int tel_send_sat_ui_user_confirm(TelSatUiUserConfirmInfo_t * pUserConfirmData);

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
 * @param [out] pMainMenu
 * - #TelSatSetupMenuInfo_t contain all menu related information which are required like menu title, icon, item count, etc.
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
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * SAT-UI
 *
 * @see tel_select_sat_menu
 *
 * @code
 * #include <ITapiSat.h>
 * int ret_status =0;
 * TelSatSetupMenuInfo_t menu; //this struct will be pull up with SIM menu info
 * ret_status = tel_get_sat_main_menu_info(&menu);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
int tel_get_sat_main_menu_info(TelSatSetupMenuInfo_t* pMainMenu);

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
 * @param [out] pAppRetInfo
 * - #TelSatAppsRetInfo_t contains execution result of a specific proactive command by application.
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
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embeded applications which are predefined by 3GPP TS31.111
 *
 * @see None
 *
 * @code
 * #include <ITapiSat.h>
 * int ret_status =0;
 * TelSatAppsRetInfo_t app_ret;
 * app_ret.commandType = TAPI_SAT_CMD_TYPE_SETUP_CALL;
 * app_ret.commandId = 1; //this value should be server given value
 * app_ret.appsRet.setupCall.resp = TAPI_SAT_R_SUCCESS;
 * ret_status = tel_send_sat_app_exec_result(&app_ret);
 * @endcode
 *
 * @remarks None
 *
 *
 */
/*================================================================================================*/
int tel_send_sat_app_exec_result(TelSatAppsRetInfo_t* pAppRetInfo);


#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_SAT_H_ */

/**
 * @}
 */
