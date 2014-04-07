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

/**
*  @addtogroup TAPI_SIM
*  @{
*
*  @file tapi_sim.h
*  @brief TAPI SIM Interface
*/

#ifndef __TAPI_SIM_H__
#define __TAPI_SIM_H__

#include <tapi.h>
#include <tel_sim.h>
#include <tel_return.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief Get SIM Card Initialization status and SIM Card Identification
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] card_status SIM initialization status/Sim card identification value(#TelSimCardStatusInfo)
 */
TelReturn tapi_sim_get_init_info(TelHandle *handle, TelSimCardStatusInfo *card_status);

/**
 *  @brief Get the Card Type whether Card is a SIM or a USIM
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] card_type SIM card type information (#TelSimCardType)
 */
TelReturn tapi_sim_get_type(TelHandle *handle, TelSimCardType *card_type);

/**
 *  @brief Get the IMSI information
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] imsi IMSI information (#TelSimImsiInfo)
 */
TelReturn tapi_sim_get_imsi(TelHandle *handle, TelSimImsiInfo *imsi);

/**
 *  @brief Get sim ECC data
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] ecc SIM emergency call code information (#TelSimEccList)
 */
TelReturn tapi_sim_get_ecc(TelHandle *handle, TelSimEccList *ecc);

/**
 *  @brief Get a Unique identification number of the sim
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data unsigned char icc_id : Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_iccid(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the language preference(indication) information
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimLanguagePreferenceCode : Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_language(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Update the language preference information to SIM card
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] language Language preference information (#TelSimLanguagePreferenceCode)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
  *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data NULL
 */
TelReturn tapi_sim_set_language(TelHandle *handle, TelSimLanguagePreferenceCode language, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the SIM Call forwarding indication related data(EF-CFIS and CPHS case)
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimCfisList : Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_callforwarding_info(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the SIM message waiting indication related data(EF-MWIS and CPHS case)
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimMwisList: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_messagewaiting_info(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Set the SIM message waiting indication related data(EF-MWIS and CPHS case)
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] req_mw Message Waiting related informations (#TelSimMwis)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data NULL
 */
TelReturn tapi_sim_set_messagewaiting_info(TelHandle *handle, TelSimMwis *req_mw, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the SIM mailbox related data(EF-MBDN, MBDI and CPHS case)
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimMailboxList: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_mailbox_info(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Set the SIM mailbox related data(EF-MBDN, MBDI and CPHS case)
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] req_mb Mailbox related informations (#TelSimMailBoxNumber)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data NULL
 */
TelReturn tapi_sim_set_mailbox_info(TelHandle *handle, TelSimMailBoxNumber *req_mb, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the SIM MSISDN data
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimMsisdnList: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_msisdn(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the the SIM SPN data
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimSpn: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_spn(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the SIM CPHS NETNAME data
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimCphsNetName: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_cphs_netname(TelHandle *handle, TapiResponseCb callback,	void *user_data);

/**
 *  @brief Get the Service Provider Display Information
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimSpPlmnList: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_sp_display_info(TelHandle *handle, TapiResponseCb callback, void *user_data);

/**
 *  @brief Execute the authentication procedure by using SIM
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] authentication_data Authentication code to be validated by ISIM,3G and GSM application in the SIM card (#TelSimAuthenticationData)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimAuthenticationResponse: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_req_authentication(TelHandle *handle, TelSimAuthenticationData *authentication_data, TapiResponseCb callback, void *user_data);


/**
 *  @brief This function performs PIN1/PIN2/SIM LOCK verification. This function performs PIN verification based on the pin type passed along with pin data
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] pin_data PIN code, entered by the user. you should make all parameters (#TelSimSecPinPw)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimSecPinResult: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_verify_pins(TelHandle *handle, TelSimSecPinPw  *pin_data, TapiResponseCb callback, void *user_data);

/**
 *  @brief This function performs unblock PIN1/PIN2 operation based on PUK information passed along with
 *  unblock information entered by the user
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] puk_data Specifies unblocking PIN password and Specifies PIN password which you want to use after unblocking operation (#TelSimSecPukPw)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimSecPukResult: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_verify_puks(TelHandle *handle, TelSimSecPukPw  *puk_data, TapiResponseCb callback, void *user_data);

/**
 *  @brief Changes PIN1/PIN2 code based on the pin type passed along with old pin data and new pin data
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] pin_data Pin types, old pin code and new pin code (#TelSimSecChangePinPw)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimSecPinResult: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_change_pins(TelHandle *handle, TelSimSecChangePinPw *pin_data, TapiResponseCb callback, void *user_data);

/**
 *  @brief Disable the SIM facility
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] pw Structure which contains facility type and password (#TelSimFacilityPw)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimFacilityResult: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_disable_facility(TelHandle *handle, TelSimFacilityPw *pw, TapiResponseCb callback, void *user_data);

/**
 *  @brief Enable the SIM facility
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] pw Structure which contains facility type and password (#TelSimFacilityPw)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimFacilityResult: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_enable_facility(TelHandle *handle, TelSimFacilityPw *pw, TapiResponseCb callback, void *user_data);


/**
 *  @brief Get the SIM facility
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] type Type of security lock type enum values (#TelSimLockType)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimFacilityInfo: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_facility(TelHandle *handle, TelSimLockType type, TapiResponseCb callback, void *user_data);

/**
 *  @brief Get the SIM LOCK TYPE information
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] type Type of security lock type enum values (#TelSimLockType)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimLockInfo: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_get_lock_info(TelHandle *handle, TelSimLockType type, TapiResponseCb callback, void *user_data);

/**
 *  @brief  This function provides common interface for accessing SIM data based on USIM generic command in ETSI TS 102 221
 * and 3GPP TS 31.102 Characteristics of the Universal Subscriber Identity Module (USIM) application.
 * This API provides only interface to send APDU. User should know well APDU usage.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] apdu_data APDU data (#TelSimApdu)
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimApduResp: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_req_apdu(TelHandle *handle, TelSimApdu* apdu_data, TapiResponseCb callback, void *user_data);

/**
 *  @brief  This function provides common interface to get SIM ATR(Answer To Reset) value
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSimResult values:\n
 *  <b>TEL_SIM_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SIM_RESULT_FAILURE</b>: Operation failed\n
 *  <b>TEL_SIM_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SIM_RESULT_MEMORY_FAILURE</b>: Memory allocation failed\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  <b>TEL_SIM_RESULT_CARD_ERROR</b>: SIM card error - Permanently blocked and general errors\n
 *  <b>TEL_SIM_RESULT_ACCESS_CONDITION_NOT_SATISFIED</b>: SIM Incompatible pin operation that is in case
 *															when invalid SIM command is given or incorrect
 *															parameters are supplied to the SIM\n
 *  <b>TEL_SIM_RESULT_INCOMPATIBLE_PIN_OPERATION</b>: SIM PIN Incorrect password\n
 *  <b>TEL_SIM_RESULT_INCORRECT_PASSWORD</b>: SIM Incorrect password\n
 *  <b>TEL_SIM_RESULT_PIN_REQUIRED</b>: PIN Required\n
 *  <b>TEL_SIM_RESULT_PUK_REQUIRED</b>: PUK Required\n
 *  <b>TEL_SIM_RESULT_PERM_BLOCKED</b>: Permanent block SIM\n
 *  <b>TEL_SIM_RESULT_SQN_FAILURE</b>: status - SQN(SeQuenceNumber) failure\n
 *  <b>TEL_SIM_RESULT_OPERATION_NOT_PERMITTED</b>: Operation not permitted\n
 *  \em data #TelSimAtr: Valid only when TelSimResult is TEL_SIM_RESULT_SUCCESS
 */
TelReturn tapi_sim_req_atr(TelHandle *handle, TapiResponseCb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* __TAPI_SIM_H__ */

/**
* @}
*/
