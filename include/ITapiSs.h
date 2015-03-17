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
 * @file ITapiSs.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SS
 * @{
 */

#ifndef _ITAPI_SS_H_
#define _ITAPI_SS_H_

#include <tapi_common.h>
#include <TelSs.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Sends a request to activate/deactivate call barring.
 *
 * @details This service allows a mobile subscriber to activate barring of certain categories
 *          of outgoing or incoming calls according to a barring program which is selected from a set
 *          of one or more barring programs chosen at provision time and is valid for all outgoing or incoming calls, or just those associated
 *          with a specific basic service group. Also this API allows deactivating call barring based on categories set.
 *          If the user calls this function with a wrong password 3 times subsequently, the barring service can be blocked.
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network. The response indicates
 *          whether the service request has been granted or not.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] info The information about call barring
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @see tel_get_ss_barring_status()
 * @see tel_change_ss_barring_password()
 */
int tel_set_ss_barring(TapiHandle *handle, TelSsBarringInfo_t *info, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets a call barring status.
 *
 * @details The mobile subscriber may determine which outgoing or incoming call should be barred
 *          by subscribing a set of one or more unique barring programs.
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network.
 *          The response indicates the service status.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] ss_class The type of call i.e. Voice call, Video Call, and so on
 *
 * @param[in] type The barring type i.e. Outgoing international, outgoing all, Incoming all, and so on
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (#TapiResult_t) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @see tel_set_ss_barring()
 * @asee tel_change_ss_barring_password()
 */
int tel_get_ss_barring_status(TapiHandle *handle, TelSsClass_t ss_class, TelSsBarringType_t type, tapi_response_cb callback, void *user_data);

/**
 * @brief Allows changing of the barring password in the network.
 *
 * @details The user can request the network to change his own barring password. If the old password is not same as
 *          what the network knows, the request will be rejected by the network. New password should be entered separately twice
 *          to confirm the correctness of the user input. Otherwise, the request must be rejected by the Telephony Server.
 *          If the user calls this function with a wrong old password 3 times subsequently, the barring service can be blocked.
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network.
 *          The response indicates whether the service request has been granted or not.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below.
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Based on 3GPP TS 22.030 section 6.5.4 Registration of New Password. Only one Change Barring Password request is allowed
 *          at a time and TS might return #TAPI_API_REQUEST_MAX_IN_PROGRESS if another request is received.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] old_password The old password set for Barring in the Network \n
 *                         This is a Null terminated string.
 *
 * @param[in] new_password The new password set for Barring in the Network \n
 *                         This is a Null terminated string
 *
 * @param[in] new_password_again A new password again \n
 *                               This is a Null terminated string
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *      An event loop runs to listen to events.
 */
int tel_change_ss_barring_password(TapiHandle *handle,
		const char *old_password,
		const char *new_password,
		const char *new_password_again,
		tapi_response_cb callback, void *user_data);

/**
 *
 * @brief Allows to set the (register/erase/activate/deactivate) call forwarding option at the network.
 *
 * @details This enables to forward the calls to another party. This service permits a called mobile subscriber
 *          to have the network send all incoming calls, or just those associated with a specific Basic service group,
 *          addressed to the called mobile subscriber's directory number and which meets
 *          various call FORWARD conditions, to another directory number.
 *          The ability of the served mobile subscriber to originate calls is unaffected.
 *          If the service is activated, a call is forwarded only if this call forward condition occurs.
 *          An application has to call this API in order to explicitly register/erase/activate/deactivate
 *          call forwarding. Request will be passed irrespective of whether 
 *          another SS call forward is already ongoing or not.
 *
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network. The response indicates
 *          whether the service request has been granted or not.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] info The Call forward information such as a forward mode, a forward type, and so on
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @see tel_get_ss_forward_status()
 */
int tel_set_ss_forward(TapiHandle *handle, const TelSsForwardInfo_t *info, tapi_response_cb callback, void *user_data);

/**
 * @brief Provides an option to get the call forwarding status of different calls from the Network.
 *
 * @details The data request procedure enables the mobile subscriber to obtain information about the data stored in the PLMN.
 *          After having requested this procedure the network shall return the following information:\n
 *          - In response to a general data request the served mobile subscriber should be given a list of all Basic service groups
 *	          to which call forwarding unconditional is registered, including information on whether or not it is active and operative,
 *	          and the associated forwarded-to numbers\n
 *          - In response to a specific request concerning one particular Basic service group,
 *	          the served mobile subscriber should be informed whether or not call forwarding unconditional is registered,
 *	          including information on whether or not it is active and operative for that Basic service group.\n
 *	          If CFU is registered, the associated forwarded-to number shall be given.
 *
 *            If this function is called normally, the asynchronous response comes in a few seconds from the network.
 *            The response indicates the service status.
 *
 *            This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *            However it just means that the API request has been transfered to the CP successfully.
 *            The actual operation result is delivered with the async response as below.
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] ss_class The Forward call type i.e. voice call, Video Call, Data Calls, and so on
 *
 * @param[in] condition The forward condition i.e. unconditional, when no reply, when busy, when not reachable, and so on
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @see tel_set_ss_forward()
 */
int tel_get_ss_forward_status(TapiHandle *handle, TelSsClass_t ss_class, TelSsForwardWhen_t condition, tapi_response_cb callback, void *user_data);

/**
 *
 * @brief Activates/deactivates the call waiting service.
 *
 * @details The call waiting service permits a mobile to be notified of an incoming
 *          call (as per basic call procedures) while the traffic channel (BM or LM) is not available for the incoming call and the mobile subscriber
 *          is engaged in an active or held call. Subsequently, the subscriber can accept, reject, or ignore the incoming call.
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network. The response indicates
 *          whether the service request has been granted or not.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] info The status of call-waiting service i.e. On (TRUE) or off (FALSE)
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @see tel_get_ss_waiting_status()
 */
int tel_set_ss_waiting(TapiHandle *handle, const TelSsWaitingInfo_t *info, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the status of the call waiting service.
 *
 * @details The call waiting service permits a mobile to be notified of an incoming call
 *          (as per basic call procedures) while the traffic channel (BM or LM) is not available for the incoming call and the mobile subscriber
 *          is engaged in an active or held call. Subsequently, the subscriber can accept, reject, or ignore the incoming call.
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network.
 *          The response indicates the service status.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below.
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] ss_class The call types
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @see tel_set_ss_waiting()
 */
int tel_get_ss_waiting_status(TapiHandle *handle, const TelSsClass_t ss_class, tapi_response_cb callback, void *user_data);

/**
 * @brief Activates/deactivates the status of the calling line identity service.
 *
 * @details The group of Line Identification Supplementary Services is divided into the following six Supplementary Services:\n
 *          CLIP - Calling line identification presentation\n
 *                 This Supplementary Service provides the ability to indicate the line identity of the calling party to the called party.
 *                 The network shall deliver the calling line identity to the called party at call set-up time,
 *                 regardless of the terminal capability to handle the information.\n
 *          CLIR - Calling line identification restriction\n
 *	               The CLIR Supplementary Service is a Supplementary Service offered to the calling party
 *	               to prevent presentation of the calling party's line identity, to the called party.\n
 *          COLP - Connected line identification presentation\n
 *                 This Supplementary Service is not a dialing check but an indication to the calling subscriber
 *                 of the connected line identity in a full ISDN/PLMN environment,
 *                 the connected line identity shall include all the information necessary to unambiguously identify the connected party.
 *                 The network shall deliver the connected line identity to the calling party
 *                 regardless of the terminal capability to handle the information.\n
 *          COLR - Connected line identification restriction\n
 *                 The COLR Supplementary Service is a Supplementary Service offered
 *                 to the connected party to prevent presentation of the connected line identity, to the calling party.
 *                 In the case where the calling party has an override category.\n
 *          CDIP - Called line identification presentation\n
 *          CNAP - Calling name presentation\n
 *                This function queries the service status with #TelSsCliType_t.
 *
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network.
 *          The response indicates the service status.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below:
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] type The Cli service type
 *
 * @param[in] status The Cli Status
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_set_ss_cli_status(TapiHandle *handle, TelSsCliType_t type, TelSsCliStatus_t status, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the status of the calling line identity service.
 *
 * @details The group of Line Identification Supplementary Services is divided into the following six Supplementary Services:\n
 *          CLIP - Calling line identification presentation\n
 *                 This Supplementary Service provides the ability to indicate the line identity of the calling party to the called party.
 *                 The network shall deliver the calling line identity to the called party at call set-up time,
 *                 regardless of the terminal capability to handle the information.\n
 *          CLIR - Calling line identification restriction\n
 *	               The CLIR Supplementary Service is a Supplementary Service offered to the calling party
 *	               to prevent presentation of the calling party's line identity, to the called party.\n
 *          COLP - Connected line identification presentation\n
 *                 This Supplementary Service is not a dialing check but an indication to the calling subscriber
 *                 of the connected line identity in a full ISDN/PLMN environment,
 *                 the connected line identity shall include all the information necessary to unambiguously identify the connected party.
 *                 The network shall deliver the connected line identity to the calling party
 *                 regardless of the terminal capability to handle the information.\n
 *          COLR - Connected line identification restriction\n
 *                 The COLR Supplementary Service is a Supplementary Service offered
 *                 to the connected party to prevent presentation of the connected line identity, to the calling party.
 *                 In the case where the calling party has an override category.\n
 *          CDIP - Called line identification presentation\n
 *          CNAP - Calling name presentation\n
 *                 This function queries the service status with #TelSsCliType_t.
 *
 *          If this function is called normally, the asynchronous response comes in a few seconds from the network.
 *          The response indicates the service status.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below:
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] type The Cli service type
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_get_ss_cli_status(TapiHandle *handle, TelSsCliType_t type, tapi_response_cb callback, void *user_data);

/**
 * @brief Sends a USSD string or User response to the Network.
 *
 * @details The unstructured supplementary service data (USSD) mechanism allows the Mobile Station (MS) user and
 *          a PLMN operator defined application to communicate in a way which is transparent
 *          to the MS and the intermediate network entities.
 *          The mechanism allows development of PLMN specific supplementary services.
 *          A USSD string is like *109, *109*72348937857623#.
 *          There are two modes of USSD: The MMI-mode and the application mode.
 *          The MMI-mode USSD is for the transparent transport of MMI strings entered by the user to the network using this function
 *          and for the transparent transport of text strings from the network that are displayed
 *          by the mobile for user information.
 *          The application mode USSD is for the transparent transport of data between the network and the UE.
 *          The application mode USSD is intended to be used by applications in the network and their peer applications in the UE.
 *
 *          If this function is called normally, the asynchronous response comes in a few seconds from the telephony server.
 *          The response indicates whether the sending request is successfully delivered or not.
 *
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered with the async response as below:
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] info The data coding scheme used (GSM 7bit or 8bit default alphabet etc, defined	as 3GPP TS 23.038), USSD String data length, ANSI string passed by the client
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully. \n
 *         Refer #TapiResult_t for failure and error code
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listend with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.
 *      No SS and USSD transaction should be ongoing. If there is an ongoing transaction, a new USSD request will be returned to the application with an error (USSD_BUSY).
 *
 */
int tel_send_ss_ussd_request(TapiHandle *handle, const TelSsUssdMsgInfo_t *info, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* _ITAPI_SS_H_ */

/**
 * @}
 */
