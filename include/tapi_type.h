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

#ifndef __TAPI_TYPE_H__
#define __TAPI_TYPE_H__

/**
 * @file tapi_type.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Definition for the telephony service name.
 */
#define DBUS_TELEPHONY_SERVICE      "org.tizen.telephony"

/**
 * @brief Definition for the telephony object path.
 */
#define DBUS_TELEPHONY_DEFAULT_PATH "/org/tizen/telephony"

/**
 * @brief The structure type containing the TAPI handle.
 */
typedef struct tapi_handle TapiHandle;

/**
 * @brief Called for the TAPI notification.
 * @param[in] handle The #TapiHandle obtained from tel_init()
 * @param[in] noti_id The notification ID for a callback has to be invoked
 * @param[in] data The notification data
 * @param[in] user_data The user data
 */
typedef void (*tapi_notification_cb)(TapiHandle *handle, const char *noti_id, void *data, void *user_data);

/**
 * @brief Called for the TAPI response.
 * @param[in] handle  The #TapiHandle obtained from tel_init()
 * @param[in] result  The result of the operation
 * @param[in] data The response data from the operation
 * @param[in] user_data The user data
 */
typedef void (*tapi_response_cb)(TapiHandle *handle, int result, void *data, void *user_data);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* __TAPI_TYPE_H__ */
