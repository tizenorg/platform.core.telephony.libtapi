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
 * @file ITapiNetwork.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_NETWORK
 * @{
 */

#ifndef _ITAPI_NETWORK_H_
#define _ITAPI_NETWORK_H_

#include <tapi_common.h>
#include <TelNetwork.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Requests the lower layers to select the network automatically.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.\n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_select_network_automatic(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests the lower layers to select the network (PLMN) which has been selected by the user from the Network List displayed to the user.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.\n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] plmn The user selected PLMN
 *
 * @param[in] act The user selected Access Technology
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_select_network_manual(TapiHandle *handle, const char *plmn, int act, tapi_response_cb callback, void *user_data);

/**
 * @brief Sends a request to do manual search for the available networks and provides the Network List to the User/Application.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.\n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 *
 * @post In the available network list, the user can select one of the networks successfully.
 */
int tel_search_network(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the present network selection mode i.e. automatic or manual.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.\n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for the result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_get_network_selection_mode(struct tapi_handle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets the network preferred PLMN.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] operation The operation to be done on the preferred PLMN
 *
 * @param[in] info The PLMN Info
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_set_network_preferred_plmn(TapiHandle *handle, TelNetworkPreferredPlmnOp_t operation,
		TelNetworkPreferredPlmnInfo_t *info, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the preferred PLMN list.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
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
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @pre Initialize the Dbus connection with #tel_init.
 */
int tel_get_network_preferred_plmn(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Cancels the triggered manual network search.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_cancel_network_manual_search(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets network serving information.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
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
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_get_network_serving(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets the network mode.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] mode The network mode (GSM only or WCDMA only or Automatic)
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS - indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_set_network_mode(TapiHandle *handle, int mode, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the network mode.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
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
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_get_network_mode(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets neighboring cell info.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
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
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_get_network_neighboring_cell_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Enters or exits the emergency callback mode.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] mode The emergency callback mode (Enter or Exit)
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_set_network_emergency_callback_mode(TapiHandle *handle, TelNetworkEmergencyCallbackMode_t mode,
		tapi_response_cb callback, void *user_data);

/**
 * @brief Sets the network roaming preference.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param [in] roam_pref The network roaming preference (AUTOMATIC, HOME_ONLY, ROAM_ONLY)
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_set_network_roaming_preference(TapiHandle *handle, TelNetworkPrefNetType_t roam_pref, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the network roaming preference.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
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
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 */
int tel_get_network_roaming_preference(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets the Subscription (related to #TapiHandle) as 'default' Data Subscription.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @post None.
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation has completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @par Prospective Clients: External Apps.
 */
int tel_set_network_default_data_subscription(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the Subscription which is configured as 'default' Data Subscription.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] The default subscription
 *             - 'default' Data Subscription.
 *             - Refer #TelNetworkDefaultDataSubs_t for Subscription details.
 *
 * @post None.
 *
 * @return The return type (int)\n
 *         #TAPI_API_SUCCESS indicates that the operation has completed successfully\n
 *         Refer #TapiResult_t for failure and an error code.
 *
 * @par Prospective Clients: External Apps.
 */
int tel_get_network_default_data_subscription(TapiHandle *handle, TelNetworkDefaultDataSubs_t *default_subscription);

/**
 * @brief This function is called to set the Subscription (related to #TapiHandle) as 'default' Subscription for CS (Voice).
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param [in] handle
 * - handle from tel_init().
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
int tel_set_network_default_subscription(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief This function is called to get the Subscription which is configured as 'default' Subscription for CS (Voice).
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [out] default_subscription
 * - 'default' Subscription for CS (Voice).
 * - Refer #TelNetworkDefaultSubs_t for Subscription details
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
int tel_get_network_default_subscription(TapiHandle *handle, TelNetworkDefaultSubs_t *default_subscription);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_NETWORK_H_ */

/**
 * @}
 */
