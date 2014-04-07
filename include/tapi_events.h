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

#ifndef __TAPI_EVENTS_H__
#define __TAPI_EVENTS_H__

#include <tapi.h>

/**
*  @addtogroup TAPI_EVENT
*  @{
*
*  @file tapi_events.h
*  @brief Telephony Event IDs
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * TAPI Event Callback Type
 * @param handle #TelHandle obtained from tapi_init()
 * @param evt_id Event ID. Refer \ref TAPI_EVENT
 * @param data Notification Data
 * @param user_data User Data
 */
typedef void (*TapiEvtCb)(TelHandle *handle, const char *evt_id, void *data, void *user_data);

/**
 *  @brief Register to a Telephony Event
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *
 *  @param[in] evt_id Event ID to register. Refer \ref TAPI_EVENT
 *
 *  @param[in] callback Callback to register for specified Event ID.
 *
 *  @param[in] user_data Callback user data
 *
 *  @return #TelReturn
 *  @par Usage
 *  @code
 *    #include <tapi_events.h> //include header
 *    TelReturn rv = tapi_register_event_id(handle, evt_id, callback, user_data); //Register Event
 *    if (TEL_RETURN_SUCCESS != rv) //Check if registration success
 *      DBG("Event Registration failed %d", rv);
 *  @endcode
 */
TelReturn tapi_register_event_id(TelHandle *handle, const char *evt_id, TapiEvtCb callback, void *user_data);

/**
 *  @brief De-register from a Telephony Event
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *
 *  @param[in] evt_id Event ID to deregister. Refer \ref TAPI_EVENT
 *
 *  @return #TelReturn
 */
TelReturn tapi_deregister_event_id(TelHandle *handle, const char *evt_id);

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* __TAPI_EVENTS_H__ */
