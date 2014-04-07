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

#ifndef __TAPI_SMS_H__
#define __TAPI_SMS_H__

/**
*  @addtogroup TAPI_SMS
*  @{
*
*  @file tapi_sms.h
*  @brief TAPI SMS Interface
*/

#include "tapi.h"
#include <tel_sms.h>
#include <tel_return.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief Sends an SMS message to the Network(UMTS).
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] send_info #TelSmsSendInfo Sms Information to be sent
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_NETWORK_CONGESTION</b>: Network Congestion\n
 *  <b>TEL_SMS_RESULT_NETWORK_FAILURE</b>: Network Failure\n
 *  <b>TEL_SMS_RESULT_INVALID_MANDATORY_INFO</b>: Invalid Mandatory Info\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_send(TelHandle *handle, const TelSmsSendInfo *send_info,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Reads an SMS message from the SIM Card storage.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] index Index of the Message to be Read
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data #TelSmsSimDataInfo
 */
TelReturn tapi_sms_read_sms_in_sim(TelHandle *handle, unsigned int index,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Writes an SMS message to the SIM Card storage.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] wdata #TelSmsSimDataInfo SMS Message structure to write on the SIM
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data unsigned int index
 */
TelReturn tapi_sms_write_sms_in_sim(TelHandle *handle, const TelSmsSimDataInfo *wdata,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Deletes an SMS message from the SIM Card storage.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] index Index of the Message to be Deleted
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_delete_sms_in_sim(TelHandle *handle, unsigned int index,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Fetches SIM card's SMS capacity and state.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_UNKNOWN_FAILURE</b>: Unknown Failure
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data #TelSmsStoredMsgCountInfo
 */
TelReturn tapi_sms_get_count(TelHandle *handle, TapiResponseCb callback,
			void *user_data);

/**
 *  @brief Sets SMS CB Message Identifiers in the appropriate EF-CBMI/EF-CBMIR file in (U)SIM.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] cb_conf #TelSmsCbConfigInfo Structure containing SMS CB configuration parameters to be set in (U)SIM
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_set_cb_config(TelHandle *handle, const TelSmsCbConfigInfo *cb_conf,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Retrieves SMS CB configuration parameters from EF-CBMI file in the (U)SIM.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_UNKNOWN_FAILURE</b>: Unknown Failure
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data #TelSmsCbConfigInfo
 */
TelReturn tapi_sms_get_cb_config(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief Fetches SMS parameters for a particular SIM.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_UNKNOWN_FAILURE</b>: Unknown Failure
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data #TelSmsParamsInfoList
 */
TelReturn tapi_sms_get_parameters(TelHandle *handle, TapiResponseCb callback,
					void *user_data);

/**
 *  @brief Set SMS parameters which are used in the origination of MO messages.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] params #TelSmsParamsInfo SMS Parameter information
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_set_parameters(TelHandle *handle,
					const TelSmsParamsInfo *params,
					TapiResponseCb callback,
					void *user_data);

/**
 *  @brief Sends a deliver report (for the received incoming SMS) to the network.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] dr_info #TelSmsDeliverReportInfo SMS Data package and Report.
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_send_deliver_report(TelHandle *handle,
				const TelSmsDeliverReportInfo *dr_info,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Sets SMS Service Centre Address information (in order to send the SMS).
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] sca #TelSmsSca SCA address information
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_set_sca(TelHandle *handle, TelSmsSca *sca,
				TapiResponseCb callback, void *user_data);

/**
 *  @brief Fetches the current SMS Service Centre Address information.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_UNKNOWN_FAILURE</b>: Unknown Failure
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data #TelSmsSca
 */
TelReturn tapi_sms_get_sca(TelHandle *handle, TapiResponseCb callback,
				void *user_data);

/**
 *  @brief  Informs modem about  the memory status of PDA (whether FULL or AVAILABLE).
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] available Memory Status. <b>TRUE</b>: PDA Memory available\n <b>FALSE</b>: PDA Memory FULL\n
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_set_memory_status(TelHandle *handle, gboolean available,
					TapiResponseCb callback, void *user_data);

/**
 *  @brief  Set the message status for an SMS.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] status_info #TelSmsStatusInfo Status of the message to be set.
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSmsResult values:\n
 *  <b>TEL_SMS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_SMS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_SMS_RESULT_SIM_FAILURE</b>: SIM Failure\n
 *  <b>TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation Not Supported\n
 *  <b>TEL_SMS_RESULT_MEMORY_FAILURE</b>: Memory Failure\n
 *  <b>TEL_SMS_RESULT_UNKNOWN_FAILURE</b>: Unknown Failure\n
 *  \em data NULL
 */
TelReturn tapi_sms_set_message_status(TelHandle *handle, TelSmsStatusInfo *status_info,
					TapiResponseCb callback, void *user_data);

/**
 *  @brief Fetches the Init Status of SMS service.
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] init_status <b>TRUE</b>: SMS service is ready\n <b>FALSE</b>: SMS service is NOT ready.
 */
TelReturn tapi_sms_get_init_status(TelHandle *handle, gboolean *init_status);

/**
 *  @brief Fetches the  SIM memory status.
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[out] memory_full <b>TRUE</b>: Memory is available\n <b>FALSE</b>: Memory is NOT available.
 */
TelReturn tapi_sms_get_sim_memory_status(TelHandle *handle,
					gboolean *memory_full);

#ifdef __cplusplus
}
#endif

/**
* @}
*/

#endif	/* __TAPI_SMS_H__ */
