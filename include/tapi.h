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

#ifndef __TAPI_H__
#define __TAPI_H__

/**
*  @addtogroup TAPI
*  @{
*
*  @file tapi.h
*  @brief TAPI Interface
*/

#include <tel_return.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * TAPI Handle
 */
typedef struct tapi_handle TelHandle;

/**
 * TAPI Response Callback Type
 * @param handle #TelHandle obtained from tapi_init()
 * @param result Result of operation of type TelResult<Module>
 * @param data Response Data from operation
 * @param user_data User Data
 */
typedef void (*TapiResponseCb)(TelHandle *handle, int result, void *data, void *user_data);

/**
 *  @brief Fetches the list of available CPs
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @param[out] cp_list List of available CP names
 *
 *  @return #TelReturn
 */
TelReturn tapi_get_cp_name_list(char ***cp_list);

/**
 *  @brief Acquires a TAPI Handle for specified CP name
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @param[in] cp_name CP Name against which TAPI handle is required \n
 *  NULL CP Name will return TelHandle bound to the first CP in the list of available CPs
 *
 *  @return #TelHandle
 */
TelHandle *tapi_init(const char *cp_name);

/**
 *  @brief De-initializes the TAPI Handle
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *
 *  @return #TelReturn
 */
TelReturn tapi_deinit(TelHandle *handle);

#ifdef __cplusplus
}
#endif

/** @}*/

#endif /* __TAPI_H__ */
