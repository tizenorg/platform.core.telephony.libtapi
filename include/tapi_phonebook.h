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

#ifndef __TAPI_PHONEBOOK_H__
#define __TAPI_PHONEBOOK_H__

/**
*  @addtogroup TAPI_PHONEBOOK
*  @{
*
*  @file tapi_phonebook.h
*  @brief TAPI Phonebook Interface
*/

#include <tapi.h>
#include <tel_phonebook.h>
#include <tel_return.h>

#ifdef __cplusplus
extern "C"
{
#endif

 /**
 *  @brief Get SIM Phone book init status
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
 *  @param[out] init_completed Phonebook init status. '0' is not init, '1' is init complete to use
 *  @param[out] pb_list Available SIM phonebook list. This value is valid in phonebook init complete case (#TelPbList)
 *  Out params are valid only when TelReturn is TEL_RETURN_SUCCESS.
 */
TelReturn tapi_pb_get_sim_pb_init_info(TelHandle *handle,
				gboolean *init_completed, TelPbList *pb_list);


/**
 *  @brief Get current inserted SIM phonebook informations
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] pb_type Different storage types to be selected in SIM (#TelPbType)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_PB_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_PB_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_PB_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_PB_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_PB_RESULT_UNKNOWN_FAILURE</b>: Unknown failure\n
 *  <b>TEL_PB_RESULT_NOT_INITIALIZED</b>: Phonebook not initialized\n
 *  <b>TEL_PB_RESULT_ACCESS_RESTRICTED</b>: Access restricted\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelPbInfo if result is TEL_PB_RESULT_SUCCESS
 */
TelReturn tapi_pb_get_sim_pb_info(TelHandle *handle, TelPbType pb_type,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Read SIM phone book entry information from given storage type
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] record Index for accessing the SIM data \n Different storage types to be selected in SIM (#TelPbRecordInfo)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_PB_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_PB_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_PB_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_PB_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_PB_RESULT_UNKNOWN_FAILURE</b>: Unknown failure\n
 *  <b>TEL_PB_RESULT_INVALID_INDEX</b>: The index passed was not a valid index\n
 *  <b>TEL_PB_RESULT_NOT_INITIALIZED</b>: Phonebook not initialized\n
 *  <b>TEL_PB_RESULT_ACCESS_RESTRICTED</b>: Access restricted\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelPbReadRecord if result is TEL_PB_RESULT_SUCCESS
 */
TelReturn tapi_pb_read_sim_pb_record(TelHandle *handle, const TelPbRecordInfo *record,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Add or edit SIM phone book record entry information
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] req_data Phonebook data which will be updated or added (#TelPbUpdateRecord)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_PB_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_PB_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_PB_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_PB_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_PB_RESULT_UNKNOWN_FAILURE</b>: Unknown failure\n
 *  <b>TEL_PB_RESULT_INVALID_INDEX</b>: The index passed was not a valid index\n
 *  <b>TEL_PB_RESULT_NOT_INITIALIZED</b>: Phonebook not initialized\n
 *  <b>TEL_PB_RESULT_ACCESS_RESTRICTED</b>: Access restricted\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data NULL
 */
TelReturn tapi_pb_update_sim_pb_record(TelHandle *handle, const TelPbUpdateRecord *req_data,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Delete a SIM phonebook record
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request is successfully posted to telephony\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] record Index for accessing the SIM data \n Different storage types to be selected in SIM (#TelPbRecordInfo)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelNetworkResult values:\n
 *  <b>TEL_PB_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_PB_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_PB_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_PB_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_PB_RESULT_UNKNOWN_FAILURE</b>: Unknown failure\n
 *  <b>TEL_PB_RESULT_INVALID_INDEX</b>: The index passed was not a valid index\n
 *  <b>TEL_PB_RESULT_NOT_INITIALIZED</b>: Phonebook not initialized\n
 *  <b>TEL_PB_RESULT_ACCESS_RESTRICTED</b>: Access restricted\n
 *  <b>TEL_PB_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data NULL
 */
TelReturn tapi_pb_delete_sim_pb_record(TelHandle *handle, const TelPbRecordInfo *record,
				TapiResponseCb callback, void *user_data);

#ifdef __cplusplus
}
#endif

/** @}*/

#endif	/* __TAPI_PHONEBOOK_H__ */
