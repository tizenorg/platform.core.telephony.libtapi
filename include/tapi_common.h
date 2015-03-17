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

#ifndef _TAPI_COMMON_H_
#define _TAPI_COMMON_H_

/**
 * @file tapi_common.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE
 * @{
 */

#include <stdio.h>
#include <glib.h>

#include <tapi_type.h>
#include <tapi_event.h>
#include <TapiUtility.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fetches a list of available CPs.
 *
 * @details <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 *
 * @return The list of available CPs on SUCCESS,
 *         otherwise @c NULL on FAILURE
 */
char **tel_get_cp_name_list(void);

/**
 * @brief Acquires a TAPI Handle for the specified CP name.
 *
 * @details <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 *
 * @param[in] cp_name The CP Name against which a TAPI handle is required (A NULL CP Name will return a #TapiHandle bound to the first CP in the list of available CPs)
 *
 * @return #TapiHandle on SUCCESS,
 *         otherwise @c NULL on FAILURE
 * @see tel_deinit()
 */
TapiHandle *tel_init(const char *cp_name);

/**
 * @brief Deinitializes the TAPI Handle.
 * @details <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 *
 * @param[in] handle The #TapiHandle obtained from tel_init()
 *
 * @return #TapiResult_t
 * @see tel_init()
 */
int tel_deinit(TapiHandle *handle);

/**
 * @brief Registers a notification callback for events on the DBus interface.
 *
 * @details
 * <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The #TapiHandle obtained from tel_init()
 *
 * @param[in] noti_id The notification ID for which a callback has to be registered
 *
 * @param[in] callback #tapi_notification_cb - The notification callback that will be invoked on the event
 *
 * @param user_data The user data
 *
 * @return #TapiResult_t
 */
int tel_register_noti_event(TapiHandle *handle, const char *noti_id, tapi_notification_cb callback, void *user_data);

/**
 * @brief Deregisters a notification callback for events on the DBus interface.
 *
 * @details <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The #TapiHandle obtained from tel_init()
 *
 * @param[in] noti_id The notification ID for which a callback has to be de-registered
 *
 * @return #TapiResult_t
 */
int tel_deregister_noti_event(TapiHandle *handle, const char *noti_id);

/**
 * @brief Gets the property value in an integer format for the given property.
 *
 * @details <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The #TapiHandle obtained from tel_init()
 *
 * @param[in] property The property to be retrieved from Dbus
 *
 * @param[out] result The property value in integer format
 *
 * @return #TapiResult_t
 */
int tel_get_property_int(TapiHandle *handle, const char *property, int *result);

/**
 * @brief Gets the property value in a string format for the given property.
 *
 * @details <b> Sync (or) Async: </b> Synchronous API.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The #TapiHandle obtained from tel_init()
 *
 * @param[in] property The property to be retrieved from Dbus
 *
 * @param[out] result The property value in string format
 *
 * @return #TapiResult_t
 */
int tel_get_property_string(TapiHandle *handle, const char *property, char **result);

#ifdef __cplusplus
}
#endif

#endif /* _TAPI_COMMON_H_ */

/**
 * @}
 */
