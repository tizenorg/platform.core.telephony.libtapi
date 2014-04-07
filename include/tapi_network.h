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

#ifndef __TAPI_NETWORK_H__
#define __TAPI_NETWORK_H__

/**
*  @addtogroup TAPI_NETWORK
*  @{
*
*  @file tapi_network.h
*  @brief TAPI Network Interface
*/

#include "tapi.h"
#include <tel_network.h>
#include <tel_return.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief Get network registration status
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation failed\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] reg_info Network Registration Information (#TelNetworkRegistrationInfo).
 *  Out params are valid only when TelReturn is TEL_RETURN_SUCCESS.
 */
TelReturn tapi_network_get_registration_info(TelHandle *handle,
					TelNetworkRegistrationInfo *reg_info);

/**
 *  @brief Get network identity information
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  \em data #TelNetworkIdentityInfo (data is valid only when TelNetworkResult is TEL_NETWORK_RESULT_SUCCESS)
 */
TelReturn tapi_network_get_identity_info(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Scan networks
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data #TelNetworkPlmnList (data is valid only when TelNetworkResult is TEL_NETWORK_RESULT_SUCCESS)
 */
TelReturn tapi_network_search(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Cancel the triggered network scanning.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  \em data NULL
 */
TelReturn tapi_network_cancel_search(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Register to network automatically
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_network_select_automatic(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Register to specified network
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameter
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] select_info Manual Selection Information (#TelNetworkSelectManualInfo)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_network_select_manual(TelHandle *handle,
			const TelNetworkSelectManualInfo *select_info,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Get network selection mode i.e. automatic or manual.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation failed
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  \em data #TelNetworkSelectionMode (data is valid only when TelNetworkResult is TEL_NETWORK_RESULT_SUCCESS)
 */
TelReturn tapi_network_get_selection_mode(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Set the network preferred plmn
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameter
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] info Preferred PLMN Information (#TelNetworkPreferredPlmnInfo)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_network_set_preferred_plmn(TelHandle *handle,
			TelNetworkPreferredPlmnInfo *info,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Get preferred plmn list
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameter
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data #TelNetworkPreferredPlmnList (data is valid only when TelNetworkResult is TEL_NETWORK_RESULT_SUCCESS)
 */
TelReturn tapi_network_get_preferred_plmn(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Set network mode.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameter
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] mode Network Mode (#TelNetworkMode)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_network_set_mode(TelHandle *handle,
			TelNetworkMode mode,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Get network mode.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameter
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_NETWORK_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data #TelNetworkMode (data is valid only when TelNetworkResult is TEL_NETWORK_RESULT_SUCCESS)
 */
TelReturn tapi_network_get_mode(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Get neighboring cells info.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameter
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_NETWORK_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_NETWORK_RESULT_FAILURE</b>: Operation failed\n
 *  \em data #TelNetworkNeighbourCellInfo (data is valid only when TelNetworkResult is TEL_NETWORK_RESULT_SUCCESS)
 */
TelReturn tapi_network_get_neighboring_cell_info(TelHandle *handle,
			TapiResponseCb callback,
			void *user_data);

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* __TAPI_NETWORK_H__ */

