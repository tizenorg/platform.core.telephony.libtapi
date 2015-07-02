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
 * @file ITapiCall.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_CALL
 * @{
 */

#ifndef _ITAPI_CALL_H_
#define _ITAPI_CALL_H_

#include <tapi_common.h>
#include <TelCall.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Originates MO call setup.
 *
 * @details This is an asynchronous function. The function completes immediately and call setup proceeds in the background.\n
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks MO call setup continues in the background. There are multiple states involved in the call setup procedure.\n
 *          Depending on whether request completion is successful or not, various event notifications are sent to the client along with data\n
 *          associated with the event(Successful data or error information) based on the indication or response received at the Telephony Server.\n
 *          Setting up an emergency call when #TelCallType_t is a voice_call, voice call is setup unless the @a dial_number parameter is\n
 *          an emergency number known to telephony FW(like 112 for all 2G and above, other numbers may be operator specific or device configuration specific,\n
 *          so Telephony FW will read relevant SIM files and refer to internal configuration records).\n
 *          If an emergency number is passed and #TelCallType_t is a voice_call, Telephony FW shall setup an emergency phone call.\n
 *          When #TelCallType_t is an emergency call, emergency call is always set up.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pParams #TelCallDial_t contains the CallType(whether it is a voice call or data call) and number
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,\n
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t).
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.
 *
 * @see #tel_answer_call #tel_end_call
 */
int tel_dial_call(TapiHandle *handle, const TelCallDial_t *pParams, tapi_response_cb callback, void *user_data);

/**
 * @brief Supports answering the incoming call by accepting or rejecting the call.
 *
 * @details This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle The unique handle for referring the call \n
 *                       This call handle is available to the application through an incoming call
 *                       (TAPI_NOTI_VOICE_CALL_STATUS_INCOMING) event.
 *
 * @param[in] AnsType The answer type - accept / reject / replace / hold and accept is allowed
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      Call associated with the call handle should be in the #TAPI_CALL_STATE_INCOM state otherwise the API fails and there can be a
 *      a maximum of 1 existing call.
 *
 * @see tel_dial_call()
 * @see tel_end_call()
 */
int tel_answer_call(TapiHandle *handle, unsigned int CallHandle, TelCallAnswerType_t AnsType, tapi_response_cb callback, void *user_data);

/**
 * @brief Releases the call identified by the call handle irrespective of whether the call is in the hold or active state.
 *
 * @details It is used when releasing a specific active call from a multiparty call.
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks In case of the MPTY Call handle, the call end event is returned for each call in MPTY.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle A unique handle that refers to the call
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] EndType The end call type (end specific call/end all calls/end all held calls/end all active calls)
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      The call handle should be valid and there should be an existing call in the active/hold state.
 *
 * @see tel_dial_call()
 * @see tel_answer_call()
 */
int tel_end_call(TapiHandle *handle, unsigned int CallHandle, TelCallEndType_t EndType, tapi_response_cb callback, void *user_data);

/**
 * @brief Puts the given call on hold.
 *
 * @details The call identified by the call handle should be in the active state.
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle A unique handle for referring the call
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      The call identified by the call handle should be in the active state.
 *
 * @see tel_retrieve_call()
 */
int tel_hold_call(TapiHandle *handle, unsigned int CallHandle, tapi_response_cb callback, void *user_data);

/**
 * @brief Retrieves the call being held.
 *
 * @details The call identified by the call handle must be in the held state.
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks The call duration for the call will be calculated from the moment the call has been connected until the call is released.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle A unique handle for referring the call
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      Call should be in the held state in order to retrieve it into the active state unless no active call is present.
 */
int tel_active_call(TapiHandle *handle, unsigned int CallHandle, tapi_response_cb callback, void *user_data);

/**
 * @brief Swaps calls. This is only for calls dialed or answered with Telephony.
 *
 * @details Swap is only available for voice calls.
 *          This API makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Notes: </b>
 * During a call, a user can place all active calls and accept other held calls with this function.
 * If the terminal is not within a call, it will fail.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle1 This is an active call
 *
 * @param[in] CallHandle2 This is a held call
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation has completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.
 */
int tel_swap_call(TapiHandle *handle, unsigned int CallHandle1, unsigned int CallHandle2, tapi_response_cb callback, void *user_data);

/**
 * @brief Starts continuous DTMF by sending a single digit during the call.
 *
 * @details DTMF is an abbreviation for Dual-tone-multi-frequency. It is used for telecommunication signaling\n
 *          over telephone lines in the voice-frequency band between UE and other communication devices.\n
 *          For example UE sends a DTMF tone to the server to choose from options which the server provides.\n
 *          If the UE is not within a call, this function will fail with an error code.
 *
 *          This function makes a Dbus method call to the Telephony Sever and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks There will be a single asynchronous notification for all the DTMF digits sent.
 *          If the users of this API need an asynchronous\n
 *          response for each DTMF digit then the user has to call this API multiple times passing each single DTMF digit in @a pDtmfString.
 *
 * @remarks To be invoked in the following cases:
 *
 *          i. Key Press during On-going call
 *          ii. DTMF digits passed with PAUSE (,) or WAIT (;)
 *
 *          In either of the above cases, the application can ONLY send a single DTMF Digit to Telephony.\n
 *          In case of PAUSE and WAIT, the application needs to invoke tel_stop_call_cont_dtmf() sequentially (for every digit) without waiting for response from tel_start_call_cont_dtmf().
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] dtmf_digit The DTMF digit to be sent from MS
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (TapiHandle *handle, int) \n
 *         @c 0 indicates that the operation has completed successfully, \n
 *         else it will return failure and an error code (TapiHandle *handle, Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      An active call should be present.
 */
int tel_start_call_cont_dtmf(TapiHandle *handle, unsigned char dtmf_digit, tapi_response_cb callback, void *user_data);

/**
 * @brief Stops continuous DTMF during the call.
 *
 * @details DTMF is an abbreviation for Dual-tone-multi-frequency. It is used for telecommunication signaling\n
 *          over telephone lines in the voice-frequency band between UE and other communication devices.\n
 *          For example UE sends a DTMF tone to the server to choose from options which the server provides.\n
 *          If the UE is not within a call, this function will fail with an error code.
 *
 *          This function makes a Dbus method call to the Telephony Sever and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks There will be a single asynchronous notification for all the DTMF digits sent.
 *          If the users of this API need an asynchronous
 *          response for each DTMF digit then the user has to call this API multiple times passing each single DTMF digit in @a pDtmfString.
 *
 * @remarks To be invoked in the following cases:
 *
 *          i. Key Release (post key press) during On-going call
 *          ii. DTMF digits passed with PAUSE (,) or WAIT (;)
 *
 *          Every tel_start_call_cont_dtmf() call needs to be followed by tel_stop_call_cont_dtmf() sequentially.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         @c 0 indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      An active call should be present.\n
 *      Start a continuous DTMF request should be sent already.
 */
int tel_stop_call_cont_dtmf(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sends one or more DTMF digits during the call. (3GPP2 specific)
 *
 * @details DTMF is an abbreviation for Dual-tone-multi-frequency. It is used for telecommunication signaling\n
 *          over telephone lines in the voice-frequency band between UE and other communication devices.\n
 *          For example UE sends a DTMF tone to the server to choose from options which the server provides.\n
 *          If the UE is not within a call, this function will fail with an error code.
 *
 *          This function makes a Dbus method call to the Telephony Sever and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks There will be a single asynchronous notification for all the DTMF digits sent.
 *          If the users of this API need an asynchronous\n
 *          response for each DTMF digit then the user has to call this API multiple times passing each single DTMF digit in @a pDtmfString.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] info A Burst DTMF info structure containing DTMF string, pulse width, and inter digit interval MS
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         @c 0 indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      An active call should be present.
 */
int tel_send_call_burst_dtmf(TapiHandle *handle, const TelCallBurstDtmf_t *info, tapi_response_cb callback, void *user_data);

/**
 * @brief Joins the given two calls (one call in the active conversation state and the other call in the held state) into conference.
 *
 * @details This function makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks The call handle of an active call or held call will be made as MPTY-id, and the other call handle will join in a conference.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle1 A unique handle which is either an active call or a held call
 *
 * @param[in] CallHandle2 A unique call handle
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      For a Multiparty call or for joining two calls into conference, there should be one call in the active state and another call
 *      in the held state.
 *
 * @see tel_split_call()
 */
int tel_join_call(TapiHandle *handle, unsigned int CallHandle1, unsigned int CallHandle2, tapi_response_cb callback, void *user_data);

/**
 * @brief Triggers splitting a private call from a multiparty call.
 *
 * @details This API allows creating a private communication with one of the remote parties in a
 *          multiparty session. TAPI client application has to pass the call handle which needs
 *          to be split from the multiparty call.
 *
 *          This function makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle The handle of the call to be made private \n
 *                       The call handle referring to the call that is to be split from the conference
 *                       (call to be made private)
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      The call should be in a multiparty conference call.
 *
 * @post The split call will be the active call and the conference call will be the held call.
 *
 * @see tel_join_call()
 */
int tel_split_call(TapiHandle *handle, unsigned int CallHandle, tapi_response_cb callback, void *user_data);

/**
 * @brief Triggers making an explicit call transfer by connecting the two parties where one party is being
 *        active (active state) and another party is being held (held state).
 *
 * @details This function makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Notes: </b>
 * The ECT supplementary service enables the served mobile subscriber (subscriber A) who has two calls,
 * each of which can be an incoming or outgoing call, to connect the other parties in the two calls
 * and release the served mobile subscriber's own connection.
 * Prior to transfer, the connection shall have been established on the call
 * between subscriber A and subscriber B. On the call between subscriber A and subscriber C,
 * either the connection shall have been established prior to transfer, or, as a network option,
 * or the transfer can occur while subscriber C is being informed of the call
 * (i.e. the connection has not yet been established.).
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle The call handle of an active call
 *
 * @param[in] callback To register the callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listend with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.\n
 *      In order to call transfer, served mobile subscriber should have 2 calls, one in the active state and another one
 *      in the held state.
 *
 * @post When the request has been completed successfully, a call end indication will be sent to both the calls (active and held).
 */
int tel_transfer_call(TapiHandle *handle, unsigned int CallHandle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the status of the current call identified by the call handle whenever the application wants the call status, call handle must be valid.
 *
 * @details This function makes a Dbus method call to the Telephony Server and gets an immediate feedback.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is a Synchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] call_id A unique handle for referring the call
 *
 * @param[out] out The call status information like destination number, call direction (MO or MT), call type (voice or data), whether
 *                 the call is in the conference state or not \n
 *                 The present call state is returned through this parameter
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_get_call_status(TapiHandle *handle, int call_id, TelCallStatus_t *out );

/**
 * @brief Gets the status all of the current call
 *
 * <b> Sync (or) Async: </b> This is a Synchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @see tel_get_call_status()
 */
int tel_get_call_status_all(TapiHandle *handle, TelCallStatusCallback cb, void *user_data);

/**
 * @brief Redirects the incoming call to another subscriber.
 *
 * @details If informed about an incoming call this call may be redirected to another destination by
 *          entering the destination number. The destination number to which the current
 *          incoming call needs to be redirected is specified via the info argument.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] CallHandle An incoming call handle
 *
 * @param[out] deflect_info The destination number
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.
 */
int tel_deflect_call(TapiHandle *handle, unsigned int CallHandle, const TelCallDeflectDstInfo_t *deflect_info, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the call volume.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is a Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] device The sound device
 *
 * @param[in] type The sound type
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 */
int tel_get_call_volume_info(TapiHandle *handle, TelSoundDevice_t device, TelSoundType_t type, tapi_response_cb callback, void *user_data );

/**
 * @brief Sets the call volume.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is a Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] info The call volume information
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation has completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 */
int tel_set_call_volume_info(TapiHandle *handle, TelCallVolumeInfo_t *info, tapi_response_cb callback, void *user_data );

/**
 * @brief Sets the call sound path.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is a Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] path The call sound path information
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 */
int tel_set_call_sound_path(TapiHandle *handle, TelCallSoundPathInfo_t *path, tapi_response_cb callback, void *user_data );

/**
 * @brief Sets the call mute state.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is a Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] mute The sound mute status
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicating that the operation has completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 */
int tel_set_call_mute_status(TapiHandle *handle, TelSoundMuteStatus_t mute, TelSoundMutePath_t path, tapi_response_cb callback, void *user_data );

/**
 * @brief Gets the call mute state.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is a Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         @c 0 indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 */
int tel_get_call_mute_status(TapiHandle *handle, tapi_response_cb callback, void *user_data );

/**
 * @brief Gets the voice privacy option mode in the phone. (3GPP2 specific)
 *
 * @details This function makes a Dbus method call to the Telephony Sever and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         @c 0 indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.
 */
int tel_get_call_privacy_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets the voice privacy option mode in the phone. It is available only where a call exists. (3GPP2 specific)
 *
 * @details This function makes a Dbus method call to the Telephony Sever and gets an immediate feedback.\n
 *          However it just means that the API request has been transfered to the CP successfully.\n
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> Embedded call application.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] PrivacyMode The voice privacy option mode(TapiHandle *handle, ENHANCED or STANDARD)
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         @c 0 indicates that the operation has completed successfully,\n
 *         else it will return failure and an error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.\n
 *      Register the telephony event to be listened with #tel_register_noti_event.\n
 *      An event loop runs to listen to events.
 */
int tel_set_call_privacy_mode(TapiHandle *handle, TelCallPrivacyMode_t PrivacyMode, tapi_response_cb callback, void *user_data);

/**
 * @brief This function is called to set 'preferred' Voice Subscription.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] preferred_subscription
 * - 'preferred' Voice Subscription.
 * - Refer #TelCallPreferredVoiceSubs_t for Subscription details
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 */
int tel_set_call_preferred_voice_subscription(TapiHandle *handle, TelCallPreferredVoiceSubs_t preferred_subscription,
	tapi_response_cb callback, void *user_data);

/**
 * @brief This function is called to get the 'preferred' Voice Subscription.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [out] preferred_subscription
 * - 'preferred' Voice Subscription.
 * - Refer #TelCallPreferredVoiceSubs_t for Subscription details
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 */
int tel_get_call_preferred_voice_subscription(TapiHandle *handle, TelCallPreferredVoiceSubs_t *preferred_subscription);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_CALL_H_ */

/**
 * @}
 */
