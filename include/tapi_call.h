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

#ifndef __TAPI_CALL_H__
#define __TAPI_CALL_H__

/**
*  @addtogroup TAPI_CALL
*  @{
*
*  @file tapi_call.h
*  @brief TAPI Call Interface
*/

#include "tapi.h"
#include <tel_call.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief Send a request to make outgoing calls
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] call_dial_info #TelCallDial Dialing call info
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_FDN_RESTRICTED</b>: FDN calls only\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_dial(TelHandle *handle, const TelCallDial *call_dial_info,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to answer incoming call
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] ans_type Call #TelCallAnswerType answering type. Example: accept,reject
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  \em data NULL
 */
TelReturn tapi_call_answer(TelHandle *handle, TelCallAnswerType ans_type,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to end a particular active call
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] call_end_info #TelCallEnd Info to end a call
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  \em data NULL
 */
TelReturn tapi_call_end(TelHandle *handle, const TelCallEnd *call_end_info,
			TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to send DTMF string
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] dtmf_str DTMF string to send
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  \em data NULL
 */
TelReturn tapi_call_send_dtmf(TelHandle *handle, const char *dtmf_str,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to hold a call
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_hold(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to make the hold call active
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_active(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to swap calls
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_swap(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief send a request to join calls
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_join(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to split calls
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] call_id Call Id of the call to split
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_split(TelHandle *handle, unsigned int call_id,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to transfer a call
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_transfer(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to delflect a call
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] deflect_to Deflecting Number
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_deflect(TelHandle *handle, const char *deflect_to,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to set active line
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] active_line #TelCallActiveLine Active line info
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_set_active_line(TelHandle *handle, TelCallActiveLine active_line,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to get active line status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data #TelCallActiveLine if result is TEL_CALL_RESULT_SUCCESS
 */
TelReturn tapi_call_get_active_line(TelHandle *handle, TapiResponseCb callback,
					void *user_data);

/**
 *  @brief Sends a request to get the call status of a particular call
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] call_id Id of the call to know the status of that call
 *  @param[out] call_status #TelCallStatus Status of the call
 */
TelReturn tapi_call_get_status(TelHandle *handle, unsigned int call_id,
					TelCallStatus *call_status );

/**
 *  @brief Sends a request to get the call status of all ongoing calls
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] call_status_list #TelCallStatusList List of status of all calls
 */
TelReturn tapi_call_get_status_all(TelHandle *handle, TelCallStatusList *call_status_list);

/**
 *  @brief Sends a request to set volume level
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] volume_info #TelCallVolumeInfo Info to set Call Volume
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_set_volume_info(TelHandle *handle, const TelCallVolumeInfo *volume_info,
				TapiResponseCb callback, void *user_data );

/**
 *  @brief Sends a request to get volume level
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] sound_device #TelCallSoundDevice Device info to get volume info
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data (unsigned int volume_info) if result is TEL_CALL_RESULT_SUCCESS
 */
TelReturn tapi_call_get_volume_info(TelHandle *handle, TelCallSoundDevice sound_device,
				TapiResponseCb callback, void *user_data );

/**
 *  @brief Sends a request to set call sound path
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] sound_path_info #TelCallSoundPathInfo Info to set sound path
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_set_sound_path(TelHandle *handle, const TelCallSoundPathInfo *sound_path_info,
				TapiResponseCb callback, void *user_data );

/**
 *  @brief Sends a request to set a call to mute
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] mute <b>TRUE</b>: Mute is ON\n <b>FALSE</b>: Mute is OFF
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_set_mute(TelHandle *handle, gboolean mute,
				TapiResponseCb callback, void *user_data );

/**
 *  @brief Sends a request to get the status of call mute
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data (gboolean enable) is valid if result is TEL_CALL_RESULT_SUCCESS <b>TRUE</b>: Mute is ON\n <b>FALSE</b>: Mute is OFF
 */
TelReturn tapi_call_get_mute_status(TelHandle *handle,
				TapiResponseCb callback, void *user_data );

/**
 *  @brief Sends a request to set call sound recording
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] sound_rec #TelCallSoundRecording Sound Recording start/stop info
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_set_sound_recording(TelHandle *handle, TelCallSoundRecording sound_rec,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sends a request to set sound equailization
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] sound_eq #TelCallSoundEqualization Info to set sound equalization
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelCallResult values:\n
 *  <b>TEL_CALL_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_CALL_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_CALL_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_CALL_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_CALL_RESULT_NETWORK_BUSY</b>: Network is busy\n
 *  <b>TEL_CALL_RESULT_SERVICE_NOT_ALLOWED</b>: Service is not allowed\n
 *  \em data NULL
 */
TelReturn tapi_call_set_sound_equalization(TelHandle *handle,
				const TelCallSoundEqualization *sound_eq,
				TapiResponseCb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* __TAPI_CALL_H__ */

 /**
* @}
*/
