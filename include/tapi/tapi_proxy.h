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

#ifndef _TAPI_PROXY_H_
#define _TAPI_PROXY_H_

/*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include <glib-2.0/glib.h>
#include <dbus/dbus-protocol.h>
#include <dbus/dbus-glib.h>

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
/*
 * This function is used to check the connection status with telephony server through DBUS.
 *
 * @return		True / False
 * @param[in]		NONE
 * @param[out]	None
 * @remark
 * @Refer
 */
int tapi_check_dbus_status(void);

 /*
 *	This function is invoked to start as a thread and this thread will be in gmain loop to serve the
 *	the requests from clients (Tapi API's).
 *
 * @param	None
 * @return	None
 * @remark	If this functions exits, then it is an error or crash
 *
 */
int tapi_send_request( int tapi_service, int tapi_service_function,
			GArray *in_param1, GArray *in_param2, GArray *in_param3, GArray *in_param4,
			GArray **out_param1, GArray **out_param2, GArray **out_param3,GArray **out_param4);

#ifdef __cplusplus
}
#endif

#endif /*_TAPI_PROXY_H_ */

