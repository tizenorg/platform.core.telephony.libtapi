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

#ifndef __TAPI_SS_H__
#define __TAPI_SS_H__

/**
*  @addtogroup TAPI_SS
*  @{
*
*  @file tapi_ss.h
*  @brief TAPI SS Interface
*/

#include "tapi.h"
#include <tel_ss.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief Sends a request to activate/deactivate call barring
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] barring_info #TelSsBarringInfo Barring Info to set
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  <b>TEL_SS_RESULT_INVALID_PASSWORD</b>: Invalid Password\n
 *  <b>TEL_SS_RESULT_PASSWORD_BLOCKED</b>: Password Blocked\n
 *  \em data NULL
 */
 TelReturn tapi_ss_set_barring(TelHandle *handle,
			const TelSsBarringInfo *barring_info,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Sends a request to get call barring status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] get_barring_info #TelSsBarringGetInfo Info to get Barring Status
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data #TelSsBarringResp if result is TEL_SS_RESULT_SUCCESS
 */
TelReturn tapi_ss_get_barring_status(TelHandle *handle,
			const TelSsBarringGetInfo *get_barring_info,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Sends a request to change call barring password
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] barring_pwd_info #TelSsBarringPwdInfo Barring Info to change password
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  <b>TEL_SS_RESULT_INVALID_PASSWORD</b>: Invalid Password\n
 *  <b>TEL_SS_RESULT_PASSWORD_BLOCKED</b>: Password Blocked\n
 *  \em data NULL
 */
TelReturn tapi_ss_change_barring_password(TelHandle *handle,
				const TelSsBarringPwdInfo *barring_pwd_info,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to activate/deactivate call forwarding
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] forwarding_info #TelSsForwardInfo forwarding Info to set
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data NULL
 */
TelReturn tapi_ss_set_forwarding(TelHandle *handle,
				const TelSsForwardInfo *forwarding_info,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to get call forwarding status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] get_forwarding_info #TelSsForwardGetInfo Info to get Forwarding Status
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data #TelSsForwardingResp if result is TEL_SS_RESULT_SUCCESS
 */
TelReturn tapi_ss_get_forwarding_status(TelHandle *handle,
				const TelSsForwardGetInfo *get_forwarding_info,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to activate/deactivate call waiting
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] waiting_info #TelSsWaitingInfo Waiting Info to set
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data NULL
 */
TelReturn tapi_ss_set_waiting(TelHandle *handle,
				const TelSsWaitingInfo *waiting_info,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to get call waiting status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] class #TelSsClass Class Info to get Forwarding Status
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data #TelSsWaitingResp if result is TEL_SS_RESULT_SUCCESS
 */
TelReturn tapi_ss_get_waiting_status(TelHandle *handle, TelSsClass class,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to set CLI
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] cli_info #TelSsCliInfo CLI Info to set
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data NULL
 */
TelReturn tapi_ss_set_cli(TelHandle *handle,
				const TelSsCliInfo *cli_info,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to get CLI status
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] type #TelSsCliType Type Info to get CLI Status
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data #TelSsCliResp if result is TEL_SS_RESULT_SUCCESS
 */
TelReturn tapi_ss_get_cli_status(TelHandle *handle, TelSsCliType type,
				TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Sends a request to send USSD
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request Successfully registered to Telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] ussd_request #TelSsUssdInfo USSD Info to send request
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSsResult values:\n
 *  <b>TEL_SS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SS_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SS_RESULT_MEMORY_FAILURE</b>: Device could not allocate Memory to formulate a response\n
 *  <b>TEL_SS_RESULT_SERVICE_NOT_AVAILABLE</b>: Service Unavailable\n
 *  <b>TEL_SS_RESULT_OPERATION_NOT_ALLOWED</b>: Operation not allowed\n
 *  <b>TEL_SS_RESULT_INCORRECT_OPERATION</b>: Incorrect Operation\n
 *  \em data #TelSsUssdResp if result is TEL_SS_RESULT_SUCCESS
 */
TelReturn tapi_ss_send_ussd_request(TelHandle *handle,
					const TelSsUssdInfo *ussd_request,
					TapiResponseCb callback,
					void *user_data);

#ifdef __cplusplus
}
#endif

/**
* @}
*/

#endif /* __TAPI_SS_H__ */
