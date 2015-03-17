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
 * @file ITapiNetText.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SMS
 * @{
 */

#ifndef _ITAPI_NETTEXT_H_
#define _ITAPI_NETTEXT_H_

#include <tapi_common.h>
#include <TelSms.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Sends an SMS message to the network.
 *
 * @details Allows transmitting SMS PDU as defined by 3GPP TS 23.040 (SMS-SUBMIT, SMS-COMMAND) to lower layers.\n
 * It allows keeping the dedicated link at lower layers by passing information as messages to send through parameters.\n
 * This will enable to avoid releasing a dedicated connection used for transmission.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] pDataPackage The SMS-SUBMIT TPDU
 * @param[in] bMoreMsgToSend If @c true there is more than one message to be sent or a multiple segmented concatenated
 *                           message has to be sent, otherwise @c false \n
 *                           This flag informs OEM that more messages have to be sent
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_send_sms(TapiHandle *handle, const TelSmsDatapackageInfo_t *pDataPackage, int bMoreMsgToSend, tapi_response_cb callback, void* user_data);

/**
 * @brief Reads a message from the SIM card storage.
 *
 * @details The SIM index (index used when storing the SMS) is the number to be passed in order
 *          to read a particular SMS message. Total number of messages can be retrieved by tel_get_sms_count function.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] read_index  The index number of the message to be read
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_read_sms_in_sim(TapiHandle *handle, int read_index, tapi_response_cb callback, void* user_data);

/**
 * @brief Writes a message to the SIM storage area.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] pWriteData The SMS message structure
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_write_sms_in_sim(TapiHandle *handle, const TelSmsData_t *pWriteData, tapi_response_cb callback, void* user_data);

/**
 * @brief Deletes the message specified by an index (SIM index used when storing the SMS)
 *        number from the SIM storage area.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] del_index The message index to delete
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_delete_sms_in_sim(TapiHandle *handle, int del_index, tapi_response_cb callback, void* user_data);

/**
 * @brief Retrieves message count information like total record count,
 *        used record count, and indices list for SIM storage.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b>  External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks This is an asynchronous function and the response will be sent back in the callback function.
 *          Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_get_sms_count(TapiHandle *handle, tapi_response_cb callback, void* user_data);

/**
 * @brief Sets SMS CB Message Identifiers in the appropriate EF-CBMI/EF-CBMIR file in the (U)SIM.
 *
 * @details This function also indicates an underlying provider to enable or disable the CB Channel on which CB messages are received.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *          @a pCBConfig contains parameters to set the SMS CB configuration.
 *          (i)  Cell broadcast service state. (Enabled/disabled.)
 *          (ii) Cell broadcast message identifier, which denotes whether all cell broadcast message identifiers are selected or only some of them.
 *          (iii) The list count of message identifiers to be set.
 *          (iv) The actual list of message identifiers to be set. Each identifier occupies 2 bytes. And the list ranges from 0x00 to 0xff.
 *          These message identifiers denote the source and type of the CB message. Based on the message identifiers set in the SIM,
 *          the ME receives CB messages with selected message identifiers.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] pCBConfig The parameters of the SMS CB configuration
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_set_sms_cb_config(TapiHandle *handle, const TelSmsCbConfig_t *pCBConfig, tapi_response_cb callback, void* user_data);

/**
 * @brief Retrieves the SMS CB configuration parameters from the EFcbmi file in the SIM.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b>  External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_get_sms_cb_config(TapiHandle *handle, tapi_response_cb callback, void* user_data);

/**
 * @brief Gets the SMS parameters for a particular SMS (sent/received)
 * based on the SIM index where it is stored.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] get_index The SIM index where the SMS is stored
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_get_sms_parameters(TapiHandle *handle, int get_index, tapi_response_cb callback, void* user_data);

/**
 * @brief Sets the short message service header parameters which are used in the origination of MO messages.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Important Notes: </b> This API updates/sets SMS Parameters in the EFsmsp file in the SIM storage.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *          @a pSmsSetParameters contains SMS header parameters information like: service center address,
 *          destination address, validity period, protocol identifier, and data coding scheme.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pSmsSetParameters The SMS header parameters information
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_set_sms_parameters(TapiHandle *handle, const TelSmsParams_t *pSmsSetParameters, tapi_response_cb callback, void* user_data);

/**
 * @brief Sends a delivery report for the received incoming SMS to the network.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] pDataPackage The SMS Data package
 * @param[in] RPCause The reason for SMS-DELIVER failure
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation has completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 *
 * @pre The SMS-DELIVER-REPORT message structure should be in the TPDU format as specified by 3GPP TS 23.040 SMS TPDU.
 */
int tel_send_sms_deliver_report(TapiHandle *handle, const TelSmsDatapackageInfo_t *pDataPackage, TelSmsResponse_t RPCause, tapi_response_cb callback, void* user_data);

/**
 * @brief Sets SMS Service Centre Address information in order to send the SMS.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Important Notes: </b> This API updates/sets the service center address in the EFsmsp file in the SIM storage.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] pSCA The service centre address information
 * @param[in] sca_index The record index of the Service center address information in the EF
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_set_sms_sca(TapiHandle *handle, const TelSmsAddressInfo_t *pSCA, int sca_index, tapi_response_cb callback, void* user_data);

/**
 * @brief Gets current SMS Service Centre Address information.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] sca_index The record index of the Service center address information in the SIM EF
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_get_sms_sca(TapiHandle *handle, int sca_index, tapi_response_cb callback, void* user_data);

/**
 * @brief Informs a modem about the memory status of the PDA (whether FULL or AVAILABLE) so that the modem can inform the network.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] memoryStatus The Memory Status
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_set_sms_memory_status(TapiHandle *handle, int memoryStatus, tapi_response_cb callback, void* user_data);

/**
 * @brief Sets the message status in the SIM EF.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks Access to this API is limited to in-house applications and Message Framework API(MAPI) is recommended instead.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] set_index The index of the message whose status has to be set
 * @param[in] msgStatus The status of the message to be set
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_set_sms_message_status(TapiHandle *handle, int set_index, TelSmsMsgStatus_t msgStatus, tapi_response_cb callback, void* user_data);

/**
 * @brief Gets the count of the SMS parameter records stored in the SIM EF.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks Access to this API is limited to in-house applications.
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_get_sms_parameter_count(TapiHandle *handle, tapi_response_cb callback, void* user_data);

/**
 * @brief Checks the SMS ready status (whether ready or not).
 *
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[out] pReadyStatus The Ready Status
 *
 * @return The return type (int)
 *         #TAPI_API_SUCCESS indicates that the operation is completed successfully,
 *         else it will return failure and an error code (Refer #TapiResult_t)
 */
int tel_check_sms_device_status(TapiHandle *handle, int *pReadyStatus);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_NETTEXT_H_ */

/**
 * @}
 */
