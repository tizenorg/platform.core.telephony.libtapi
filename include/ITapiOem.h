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
 * @file ITapiOem.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_OEM
 * @{
 */

#ifndef _ITAPI_OEM_H_
#define _ITAPI_OEM_H_

#include <tapi_common.h>
#include <TelOem.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief This function sends oem data directly and getting response as notification.
 *
 * @details Access to this API is limited to in-house applications. \n
 *
 *          <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] oem_id
 * - oem_id for user specification.
 *
 * @param [in] data
 * - data for sending.
 *
 * @param [in] data_len
 * - data length for sending.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 */
int tel_send_oem_data(TapiHandle *handle, int oem_id,
	void *data, unsigned int data_len);

/**
 * @brief This function sends oem data directly and getting response from TelOemData_t.
 *
 * @details Access to this API is limited to in-house applications. \n
 *
 *          <b> Sync (or) Async: </b> This is a Synchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] oem_id
 * - oem_id for user specification.
 *
 * @param [in] data
 * - data for sending.
 *
 * @param [in] data_len
 * - data length for sending.
 *
 * @param [in] outparam
 * - data for receiving #TelOemData_t.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 */
int tel_send_oem_data_sync(TapiHandle *handle, int oem_id,
	void *data, unsigned int data_len, TelOemData_t *outparam);

/**
 * @brief This function sends oem data and gets response from async callback.
 *
 * @details Access to this API is limited to in-house applications. \n
 *
 *          <b> Sync (or) Async: </b> This is a Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] oem_id
 * - oem_id for user specification.
 *
 * @param [in] data
 * - data for sending.
 *
 * @param [in] data_len
 * - data length for sending.
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par Async Response Message:
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 * Data associated with the response is #TelOemData_t.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @post callback will be invoked and #TelOemData_t will be stored in data on success case.
 */
int tel_send_oem_data_async(TapiHandle *handle, int oem_id,
	void *data, unsigned int data_len, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_OEM_H_ */

/**
 * @}
 */
