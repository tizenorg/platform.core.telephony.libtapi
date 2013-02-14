/*
 * libslp-tapi
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd. All rights reserved.
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

__BEGIN_DECLS

#define DBUS_TELEPHONY_SERVICE      "org.tizen.telephony"
#define DBUS_TELEPHONY_DEFAULT_PATH "/org/tizen/telephony"

typedef struct tapi_handle TapiHandle;

typedef void (*tapi_notification_cb)(TapiHandle *handle, const char *noti_id, void *data, void *user_data);
typedef void (*tapi_response_cb)(TapiHandle *handle, int result, void *data, void *user_data);

__END_DECLS

#endif /* __TAPI_TYPE_H__ */
