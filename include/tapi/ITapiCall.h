/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
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
 * @addtogroup	TAPI_CALL	CALL
 * @{
 *
 * @file ITapiCall.h
 *
 * Call APIs allow an application to accomplish the following services: @n
 * - Make a mobile originated Emergency call, Voice call, or Video call. @n
 * - Accept or Reject incoming calls. @n
 * - Release Calls. @n
 * - Call dependent supplementary services such as call hold, retrieve, DTMF. @n
 * - Multi -party call management, setup conference call, split conference call. @n
 * - Call information such as get call information, get line identification number. @n
 * - To verify whether a call (voice / video ) is possible at any given time. @n
 */

#ifndef _ITAPI_CALL_H_
#define _ITAPI_CALL_H_

/*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include <TelCall.h>
#include <TelUtility.h>
#include <TelDefines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                            MACROS
==================================================================================================*/

/*==================================================================================================
                                             ENUMS
==================================================================================================*/


/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

 /**
 * @brief  This function originates MO call setup. This is an asynchronous function. The function completes immediately and
 * call setup proceeds in background.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -  If call to tel_exe_call_mo() is successful, call handle and request ID are assigned by Telephony Server
 *     and returned to client by given Call Handle and Request Id parameters. MO call setup continues in
 *     background. There are multiple states involved in the call setup procedure. Upon request completion
 *     successful or not, various event notifications are sent to client along with data associated with event
 *     (Successful data or error information) based on the indication or response received at the Telephony Server.
 * -  Setting up Emergency call
 *     - when #TelCallType_t  is voice_call, voice call is setup unless dial_number parameter is an
 *     emergency number known to telephony FW (like 112 for all 2G and above, other numbers may operator
 *     specific or device configuration specific, so Telephony FW will read relevant SIM files and refer to internal
 *     configuration records). If an emergency number is passed and #TelCallType_t  is voice_call, Telephony
 *     FW shall setup emergency phone call.
 *     - When #TelCallType_t  is emergency call, emergency call is always set up.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface
 *
 * @param[in] pParams
 * - #TelCallSetupParams_t contains the CallType(whether it is a voice call or data call etc) and number.
 *
 * @param[out] pCallHandle
 * - Unique handle for referring the call
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * TAPI_EVENT_CALL_SETUP_CNF is sent to the application when the lower layers get the call originate response.
 * Asynchronous return status is indicated by #TelCallCause_t and call handle is sent in the event data.
 * Various asynchronous indications are described in the unsolicited notifications section . [Refer, \ref page8]
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_exe_call_mo tel_answer_call tel_release_call
 * \image html CallSetup.jpg
 * \image latex CallSetup.jpg
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status =0;
 * unsigned int  pCallHandle;
 * int pRequestID=0;
 * TelCallSetupParams_t pParams;
 *
 * char normal[16] = "1234";//Called party number
 * memset(&pParams, 0, sizeof(TelCallSetupParams_t));
 * pParams.CallType = TAPI_CALL_TYPE_VOICE;
 * strcpy(pParams.szNumber,normal);
 *
 * ret_status = tel_exe_call_mo (&pParams,&pCallHandle,&pRequestID); // outgoing call
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
int tel_exe_call_mo(const TelCallSetupParams_t *pParams,  unsigned int  *pCallHandle,  int * pRequestId);

 /**
 * @brief This function is used to answer the incoming call by accepting or rejecting the call.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface
 *
 * @param[in] CallHandle
 * - This is the unique handle for referring the call. This call handle is available to Application through incoming call
 *   (TAPI_EVENT_CALL_INCOM_IND) event.
 *
 * @param[in] AnsType
 * - Answer type - accept / reject / replace / hold and accept. but, for CDMA, only accept is allowed.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_ANSWER_CNF. Asynchronous return status is
 *    indicated by #TelCallCause_t and call handle is sent in the event data.
 * - TAPI_EVENT_CALL_CONNECTED_IND / TAPI_EVENT_CALL_END_IND event will be notified in case of call is accepted/rejected.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - Call associated with call handle should be TAPI_CALL_STATE_INCOM state otherwise the API fails and there can be a
 *     maximum of 1 existing call.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_exe_call_mo tel_release_call
 *
 * \image html CallIncoming.jpg
 * \image latex CallIncoming.jpg
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int  ret_status ;
 * unsigned int  pCallHandle;
 * int pRequestID=0;
 *
 * ret_status = tel_answer_call (pCallHandle,&pRequestID); // call answer
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 * @remarks
 * - None
 *
 *
 */
int tel_answer_call(unsigned int CallHandle, TelCallAnswerType_t AnsType, int * pRequestId);

 /**
 *
 * @brief This function releases the call identified by Call Handle irrespective of call is hold or active state.
 * It is used when releasing specific active call in multiparty call.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - In case of MPTY Call handle, Call end event is returned for each call in MPTY.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface
 *
 * @param[in] CallHandle
 * - Unique handle for referring the call.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_RELEASE_CNF and Asynchronous return status
 *    is indicated by #TelCallCause_t and call handle for the released call is sent in the event data..
 *
 * - The event TAPI_EVENT_CALL_END_IND_IND is received as final asynchronous response and the associated data is #TelCallEndInfo_t.
 *    Asynchronous return status is indicated by #TelCallCause_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - Call handle should be valid and there should be an existing call in Active/hold state.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_exe_call_mo tel_answer_call tel_release_call_all tel_release_call_all_active
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle;
 * int pRequestID=0;
 *
 * ret_status = tel_release_call(pCallHandle,&pRequestID); // call release
 * @endcode
 *
 *
 * @remarks
 * - None
 *
 *
 */
int tel_release_call(unsigned int  CallHandle, int * pRequestId);

 /**
 *
 * @brief This function releases the all the calls irrespective of the call is hold or active state. Due to this API request,
 *        lower layers will release the call and END Indication Event will be sent to all the Applications.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - This asynchronous event is received for each call getting released.The event associated with this request is
 *    TAPI_EVENT_CALL_END_IND_IND and the associated data is #TelCallEndInfo_t. There will be no request id present associated
 *    with this TAPI_EVENT_CALL_END_IND_IND when this API is called.
 *    Once all the calls are released, Telephony server will indicate application through TAPI Library by sending
 *    TAPI_EVENT_CALL_RELEASE_ALL_CNF. Asynchronous return status is indicated by #TelCallCause_t and there is no
 *    associated event data.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status;
 * int pRequestID;
 *
 * ret_status = tel_release_call_all(&pRequestID); // release call all
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int tel_release_call_all (int * pRequestId);

 /**
 *
 * @brief This function releases the all activate call. Due to this API request,
 *          lower layers will release the call and END Indication Event will be sent to all the Applications.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *  - None.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status;
 * int pRequestID;
 *
 * ret_status = tel_release_call_all_active(&pRequestID); // release all active call
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
int tel_release_call_all_active(int *pRequestId);

 /**
 *
 * @brief This function releases the all held call. Due to this API request,
 *          lower layers will release the call and END Indication Event will be sent to all the Applications.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *  - None.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status;
 * int pRequestID;
 *
 * ret_status = tel_release_call_all_held(&pRequestID); // release all held call
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */

 int tel_release_call_all_held(int * pRequestId);

 /**
 *
 * @brief This function puts the given call on hold. The call identified by Call Handle should be in active state.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - Call duration for the call will be calculated from the time call has been connected until call is released,
 *    i.e. Time duration during the call on hold will also be counted.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle
 *  - Unique handle for referring the call.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_HOLD_CNF. Asynchronous return status is
 *    indicated by #TelCallCause_t and call handle is sent in the event data.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - The call identified by the Call Handle should be in active state.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle;//HANDLE OF AN ACTIVE CALL
 * int pRequestID=0;
 *
 * ret_status = tel_hold_call(pCallHandle,&pRequestID); // hold call
 * @endcode
 *
 *
 * @see tel_retrieve_call
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_hold_call(unsigned int CallHandle, int * pRequestId);

 /**
 *
 * @brief This function retrieves the held call. The call identified by Call Handle must be in held state.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - Call duration for the call will be calculated from the moment call has been connected until call is released.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 * - None.
 *
 *
 * @param[in] CallHandle
 *  - Unique handle for referring the call.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_RETRIEVE_CNF. Asynchronous return status is indicated
 *    by #TelCallCause_t and call handle is sent in the event data.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - Call should be in held state in order to retrieve into active state and no active call present.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_retrieve_call
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle;//held call handle
 * int pRequestID=0;
 *
 * ret_status = tel_retrieve_call(pCallHandle,&pRequestID); // retrieve call
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
int tel_retrieve_call(unsigned int CallHandle, int * pRequestId);

 /**
 *
 * @brief Swap calls. This is only for calls you dialed or answered with Telephony.
 *  Swap is only available for the voice calls.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Notes:
 * During a call, user can place all active calls and accepts the other held call with this function.
 * If the terminal is not within a call, it will fail.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 * @param[in] CallHandle1
 *  - This is active call
 *
 * @param[in] CallHandle2
 *  - This is Held call
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle1;
 * unsigned int  pCallHandle2;
 * int pRequestID=0;
 *
 * ret_status = tel_swap_call(pCallHandle1, pCallHandle2,&pRequestID); // swap call
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_swap_call(unsigned int CallHandle1, unsigned int CallHandle2, int * pRequestId);

 /**
 *
 * @brief This function sends one or more DTMF digits during the call.
 *
 * @par Notes:
 * DTMF is abbreviation of Dual-tone-multi-frequency. It is used for telecommunication signaling
 * over telephone lines in the voice-frequency band between UE and other communications devices.
 * For example UE send DTMF tone to a server to choose options which the server provides.
 * If the UE is not within a call, this function will fail with error code. *
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  -There will be a single asynchronous notification for all the DTMF digits sent. If the users of this API need an asynchronous
 *    response for each DTMF digit then User has to call this API multiple times passing each single DTMF digit in pDtmfString.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] pDtmfString
 *  - A Null terminated DTMF string to be sent from MS.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_SEND_DTMF_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t and call handle is sent in the event data.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - Active call should be present.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 *
 * \image html CallDtmf.jpg
 * \image latex CallDtmf.jpg
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle;
 * int pRequestID=0;
 * char DtmfString[10] = {0, };
 * strcpy(DtmfString, "123456789");//Sample DTMF digits that need to be sent
 * ret_status= tel_send_call_dtmf(DtmfString,&pRequestID);
 * @endcode
 *
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_send_call_dtmf(const char *pDtmfString, int * pRequestId);

 /**
 *
 * @brief This function joins the given two calls (one call in active conversation state and other call in held state) into conference.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - Call handle of an active call or held call will be made as MPTY-id, and the other call handle will join in a conference).
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle1
 *  - The unique handle can be either an active call or held call.
 *
 * @param[in] CallHandle2
 *  - Unique call handle.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_SETUPCONFERENCE_CNF. Asynchronous return
 *    status is indicated by #TelCallCause_t and call handle of the MPTY call is sent in the event data.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - For a Multiparty call or for joining two calls into conference, there should be one call in active state and another call
 *    in held state.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_split_call
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle1;
 * unsigned int  pCallHandle2;
 * int pRequestID=0;
 *
 * ret_status= tel_join_call(pCallHandle1, pCallHandle2,&pRequestID); // call join
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 *
 *
 */
 int tel_join_call(unsigned int CallHandle1, unsigned int CallHandle2, int * pRequestId);

 /**
 *
 * @brief This function triggers to splits a private call from multiparty call. This API allows creating a private communication
 *          with one of remote parties in a multiparty session. TAPI client application has to pass the call handle which needs
 *          to split from the multiparty call.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - The list of call handles present in the conference call is retrieved using #tel_get_call_conf_list API
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle
 *  - Handle of call to be made private. Call handle referring to the call that is to be split from the conference
 *     (call to be made private).
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_SPLITCONFERENCE_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t and call handle for the split call is sent in the event data.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - Call should be in multiparty conference call.
 *
 * @post
 *  - Split call will be the active call and the conference will be held call.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_join _call
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle;
 * int pRequestID=0;
 *
 * ret_status= tel_split_call(pCallHandle,&pRequestID); // call split
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 *
 *
 */
 int tel_split_call(unsigned int CallHandle, int * pRequestId);

 /**
 *
 * @brief Get the current calling line identification number.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Notes:
 * If the USIM card has multiple number, an user can choose which number he use.
 * For example, a line is for private, the other line is for business.
 * Using this function, user can get information about which line is currently active.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  NONE
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 * @param [out] ptr_active_line
 *    current active line id.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_set_call_act_line
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int  pRequestID=0;
 * ret_status= tel_get_call_act_line(&pRequestID); // get call active line
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_get_call_act_line( int * pRequestId);

 /**
 *
 * @brief Set the current calling line identification number.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  NONE
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] active_line
 *  - calling line identification
 *
 * @param[out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see tel_get_call_act_line
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * TelCallActiveLine_t g_curLine =TAPI_CALL_ACTIVE_LINE1;
 * TelCallActiveLine_t active_line ;
 * int  pRequestID=0;
 *
 * ret_status = tel_set_call_act_line (g_curLine,&pRequestID); // set call active line
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_set_call_act_line(TelCallActiveLine_t active_line, int * pRequestId);

 /**
 *
 * @brief This function triggers to do an explicit call transfer by connecting the two parties where in one party being
 *          active (active state) and another party being held (held state).
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Notes:
 * The ECT supplementary service enables the served mobile subscriber (subscriber A) who has two calls ,
 * each of which can be an incoming or outgoing call, to connect the other parties in the two calls
 * and release the served mobile subscribers own connection.
 * Prior to transfer, the connection shall have been established on the call
 * between subscriber A and subscriber B. On the call between subscriber A and subscriber C,
 * either the connection shall have been established prior to transfer, or, as a network option,
 * transfer can occur while subscriber C is being informed of the call
 * (i.e. the connection has not yet been established.)
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *  - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle
 *  - Call handle of an active call.
 *
 * @param [out] pRequestId
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_TRANSFER_CNF. Asynchronous return status is
 *    indicated by #TelCallCause_t and call handle is sent in the event data. Call handle in event data is active
 *    call handle which is used in the #tel_exe_call_explicit_transfer request.
 *
 * @pre
 *  - In order to call transfer, Served mobile subscriber should have 2 calls in one in active state and another one Call
 *    in Held state.
 *
 * @post
 *  - When the request has been completed successfully, Call end indication will be sent to both the calls (active and held).
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @see None.
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int  pCallHandle;//active call
 * int pRequestID=0;
 *
 * ret_status= tel_exe_call_explicit_transfer (pCallHandle,&pRequestID); // explicit transfer
 * @endcode
 *
 * @remarks
 * -None
 *
 *
 */
 int tel_exe_call_explicit_transfer(unsigned int CallHandle, int * pRequestId);


/**
 *
 * @brief This function gets status for the current call identified by Call Handle whenever application wants the call status. Call handle must be valid.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle
 *  - Unique handle for referring the call.
 *
 * @param [out] pCallStatus
 *   - Call status information like destination number, call direction (MO or MT), call type (voice or data etc), whether
 *      the call is in conference state or not, present call state etc are returned through this parameter.
 *
 * @par Async Response Message:
 *   - None.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 *
 *
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int  ret_status ;
 * unsigned int CallHandle;
 * TelCallStatus_t	callStatus;
 *
 * ret_status= tel_get_call_status(CallHandle,&callStatus); // get call status info
 * @endcode
 *
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */

 int tel_get_call_status(unsigned int CallHandle, TelCallStatus_t * pCallStatus) ;


 /**
 *
 * @brief This function gets duration of the given call. This is a synchronous function. Duration is accounted from the moment
 *          the connection is established, i.e. call goes into active state for first time.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle
 *  - Unique handle for referring the call.
 *
 * @param [out] pDurationInSecs
 *   - The total call duration in seconds from the call connecting time to the present time is returned through this parameter.
 *
 * @par Async Response Message:
 *  - None.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 *
 *
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int CallHandle;
 * unsigned int duration;
 *
 * ret_status= tel_get_call_duration(CallHandle,&duration); // get call duration
 * @endcode
 *
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_get_call_duration(unsigned int CallHandle, unsigned int * pDurationInSecs) ;

 /**
  *@brief Deflect the incoming call to other subscriber
  *
  * If informed about an incoming call this call may be redirected to an another destination by
  * entering the destination Number. The cleint spcifies the dstiantion number to which the current
  * incoming call needs to be redirected is specifed via info argument.
  *
  *
  * @par Sync (or) Async:
  * This is an Asynchronous API.
  *
  * @par Important Notes:
  *  - None.
  *
  * @warning
  * Do not use this function. This function is dedicated to the embedded call application only.
  * Please use its AUL interface instead of this.
  *
  * @param[in] CallHandle
  *  - This is incoming call handle
  *
  * @param[out] deflect_info
  *  - Destination Number
  *
  * @param [out] pRequestID
  *   - Unique identifier for a particular request.
  *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
  *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
  *
  * @par Async Response Message:
  * -
  *
  * @pre
  *  - Initialize Dbus connection with #tel_init
  *  - Register caller's application name with #tel_register_app_name
  *  - Register telephony events to listen
  *  - A event loop is running to listen events
  *
  * @post
  *  - None.
  *
  * @return Return Type (int) \n
  * - 0 indicating that the operation has completed successfully. \n
  * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
  *
  * @par Prospective Clients:
  * Embedded call application
  *
  * @remarks
  * - None
  *
  * @code
  * #include <ITapiCall.h>
  * #include <TelCall.h>
  *
  * unsigned int call_handle;
  * TelCallDeflectDstInfo_t deflect_info;
  * int req_id;
  *
  * tel_deflect_call( call_handel, &deflect_info, req_id); // call deflect
  *
  * @endcode
  *
  * @see None.
  *
  * @remarks
  * - None
  *
  */
 int tel_deflect_call(unsigned int CallHandle, const TelCallDeflectDstInfo_t *deflect_info, int *pRequestID);


  /**
  *
  * @brief Activate Call Completion to a Busy Subscriber.
  *
  * @par Notes:
  * When subscriber A encounters a Network Determined User Busy (NDUB) destination B,
  * subscriber A can request the CCBS supplementary service (i.e. activate a CCBS Request against destination B).
  * The network will then monitor the wanted destination B for becoming idle.
  * When the wanted destination B becomes idle, then the network will wait a short time
  * in order to allow destination B to make an outgoing call.
  * If destination B does not make any outgoing call within this time,
  * then the network shall automatically recall subscriber A.
  * When subscriber A accepts the CCBS recall, within a defined time, then the network will automatically
  * generate a CCBS call to destination B.
  *
  * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
  * However it just means that the API request has been transfered to the CP successfully.
  * The actual operation result is being delivered in the corresponding event asynchronously.
  *
  * @par Sync (or) Async:
  * This is an Asynchronous API.
  *
  * @par Important Notes:
  *   - None.
  *
  * @warning
  * Do not use this function. This function is dedicated to the embedded call application only.
  * Please use its AUL interface instead of this.
  *
  * @param [in] CallHandle
  *  - Handle of the call.
  *
  * @param [out] pRequestID
  *   - Unique identifier for a particular request.
  *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
  *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
  *
  * @par Async Response Message:
  * -
  *
  * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
  *
  * @post
  *  - None.
  *
  * @return Return Type (int) \n
  * - 0 - indicating that the operation has completed successfully. \n
  * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
  *
  * @par Prospective Clients:
  * Embedded call application
  *
  * @code
  * #include <ITapiCall.h>
  *
  * int ret;
  * unsigned int call_handle;
  * int req_id;
  *
  * ret = tel_activate_call_ccbs( call_handle, &req_id ); // ccbs activate
  * @endcode
  *
 *
 * @see None.
 *
 * @remarks
 * - None
 *
  */
 int tel_activate_call_ccbs(unsigned int CallHandle, int * pRequestID);


 /**
 * @brief This is a synchronous function returns all call handles within the given conference call.
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 *  - Call should be a Multi-party conference call in order to return actual no of calls and call handles in the conference calls.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] CallHandle
 *  - Handle of call which is associated with the conference.
 *
 * @param [out] pCallList
 *   - list of call joined in the conference call. In case there is no active conference. The list will be zero and
 *      number of calls parameter value will also be zero. Maximum number of calls in a conference can be up to 5
 *      (Based on 3GPP TS 22.084). Memory allocation for call list is integer array of size 5.
 *
 * @param [out] pNoOfCalls
 *   - Number of the calls present in conference.
 *
 * @par Async Response Message:
 *  -None.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  -None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status ;
 * unsigned int CallHandle;
 * unsigned int  callList[5];
 * int  noOfCalls;
 *
 * ret_status= tel_get_call_conf_list(CallHandle, callList, &noOfCalls); // get conference call list
 * @endcode
 *
 * @see tel_join_call
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_get_call_conf_list (unsigned int CallHandle, const unsigned int * pCallList, int * pNoOfCalls);


 /**
 *
 * @brief This function gets voice privacy option mode in phone(CDMA only).
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] PrivacyType
 * - Privacy Type.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF. Asynchronous return status
 *   is indicated by #TelCallCause_t. #TelCallPrivacyMode_t is included in this event.
 *
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 *
 *
 * @code
 * #include <ITapiCall.h>
 *
 * Int ReqId=-1;
 * int ret_status = -1;
 * int api_err;
 * TelCallPrivacyType_t privacyType;
 * privacyType=TAPI_CALL_PRIVACY_TYPE_MS;
 * ret_status =tel_get_call_privacy_mode (privacyType,&req_id); // get call privacy_mode
 * @endcode
 *
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_get_call_privacy_mode(TelCallPrivacyType_t PrivacyType, int * pRequestID);


 /**
 *
 * @brief This function sets voice privacy option mode in phone. It is available only where call exists(CDMA only).
 *
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] PrivacyInfo
 * - voice privacy option mode(ENHANCED or STANDARD)
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t.
 *
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 *
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ReqId=-1;
 * int ret_status = -1;
 * int api_err;
 * TelCallPrivacyMode_t pPrivacyMode_info={0,};
 * pPrivacyMode_info = TAPI_CALL_PRIVACY_MODE_STANDARD;
 *
 * ret_status = tel_set_call_privacy_mode (pPrivacyMode_info,&reqId); // set call privacy mode
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_set_call_privacy_mode(TelCallVoicePrivacyInfo_t PrivacyInfo, int * pRequestID);


/**
 *
 * @brief This function requests to send a Flash with Information Message(CDMA only).
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] pDialNumber
 * - this is the calling number for 3 way call. But in the call waiting, this param should be NULL.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_FLASHINFO_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t.
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status = -1;
 * int api_err;
 * char const * const pDialNumber = "9999900000";
 * ret_status = tel_exe_call_flash_info(pDialNumber,&reqId); // call flash info
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_exe_call_flash_info(const char *pDialNumber, int * pRequestID);


/**
 *
 * @brief This function requests to exit emergency call mode(CDMA only).
 *
 * This function makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 * @param[out] pRequestID
 *   - Unique identifier for a particular request.
 *   Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_EXIT_EMERGENCYMODE_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t.
 *
 * @pre
 *  - current state is emergency callback mode.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int req_id;
 * int ret;
 *
 * ret = tel_exit_call_emergency_mode ( &req_id ); // emergency call
 *
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_exit_call_emergency_mode(int * pRequestID);


/**
 *
 * @brief This function is used for querying the information about a call time and call count.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * Do not use this function. This function is dedicated to the embedded call application only.
 * Please use its AUL interface instead of this.
 *
 *
 * @param[in] req_mask
 * - call time request type mask.
 *
 * @param[out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_CALL_GET_CALL_TIME_CNF. Asynchronous return status
 *    is indicated by #TelCallCause_t.
 *
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - 0 - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer Doxygen doc or #TapiResult_t)
 *
 * @par Prospective Clients:
 * Embedded call application
 *
 * @code
 * #include <ITapiCall.h>
 *
 * int ret_status = -1;
 * int api_err;
 * TelCallTimeMaskType_t mask_byte1 = {0,};
 * TelCallTimeMaskType_t  mask_byte2 = {0,};
 * unsigned short mask_type;
 * mask_byte1 = TAPI_CALL_TIME_TYPE_TOTAL_CALL_CNT;  //masking call type
 * mask_byte2 = TAPI_CALL_TIME_TYPE_TOTAL_CALL_TIME;
 *
 * mask_type = mask_byte1;
 * mask_type << 8;
 * mask_type = mask_byte2;
 *
 * ret_status = tel_get_call_time(mask_type,&reqId);
 *
 * @endcode
 *
 * @see None.
 *
 * @remarks
 * - None
 *
 *
 */
 int tel_get_call_time(unsigned short req_mask, int * pRequestID);


#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_CALL_H_ */

 /**
* @}
*/

