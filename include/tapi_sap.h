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

#ifndef __TAPI_SAP_H__
#define __TAPI_SAP_H__

/**
*  @addtogroup TAPI_SAP
*  @{
*
*  @file tapi_sap.h
*  @brief TAPI SAP Interface
*/

#include "tapi.h"
#include <tel_sap.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @brief This API is used to request to connect sap.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] max_msg_size Maximum size of a message.
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_UNABLE_TO_ESTABLISH</b>: Unable to Establish Connection\n
 *  <b>TEL_SAP_RESULT_NOT_SUPPORT_MAX_SIZE</b>: Server Does Not Support Message Length That Client Want to Send\n
 *  <b>TEL_SAP_RESULT_TOO_SMALL_MAX_SIZE</b>: Client wants to Connect with very Small Message Length which is Not Supported by Server\n
 *  <b>TEL_SAP_RESULT_ONGOING_CALL</b>: Server cannot Reset the SIM due to an Ongoing Call \n
 *  \em data unsigned char max_msg_size : Valid only when TelSapResult is TEL_SAP_RESULT_SUCCESS
 */
TelReturn tapi_sap_req_connect(TelHandle *handle, unsigned int max_msg_size,
					TapiResponseCb callback, void *user_data);

/**
 *  @brief This API is used to request to disconnect sap.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_FAILURE_NO_REASON</b>: No Reason\n
 *  \em data NULL
 */
TelReturn tapi_sap_req_disconnect(TelHandle *handle, TapiResponseCb callback,
					void *user_data);

/**
 *  @brief This API is used to request the ATR.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_FAILURE_NO_REASON</b>: No Reason\n
 *  <b>TEL_SAP_RESULT_CARD_ALREADY_POWERED_OFF</b>: Card Already Powered Off\n
 *  <b>TEL_SAP_RESULT_CARD_REMOVED</b>: Card Removed\n
 *  <b>TEL_SAP_RESULT_DATA_NOT_AVAILABLE</b>: Data Not Available\n
 *  \em data #TelSapAtr : Valid only when TelSapResult is TEL_SAP_RESULT_SUCCESS
 */
TelReturn tapi_sap_get_atr(TelHandle *handle, TapiResponseCb callback,
					void *user_data);

/**
 *  @brief This API is used to exchange an APDU command.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] apdu_data #TelSapApdu APDU command to forward to the SIM card
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_FAILURE_NO_REASON</b>: No Reason\n
 *  <b>TEL_SAP_RESULT_CARD_NOT_ACCESSIBLE</b>: Not Accessible\n
 *  <b>TEL_SAP_RESULT_CARD_ALREADY_POWERED_OFF</b>: Card Already Powered Off\n
 *  <b>TEL_SAP_RESULT_CARD_REMOVED</b>: Card Removed\n
 *  \em data #TelSapApduResp : Valid only when TelSapResult is TEL_SAP_RESULT_SUCCESS
 */
TelReturn tapi_sap_req_transfer_apdu(TelHandle *handle, TelSapApdu *apdu_data,
					TapiResponseCb callback, void *user_data);

/**
 *  @brief This API is used to set the transport protocol.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] protocol #TelSimSapProtocol Identifier of the protocol to be used
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_NOT_SUPPORTED</b>: Not Supported\n
 *  \em data NULL
 */
TelReturn tapi_sap_req_transport_protocol(TelHandle *handle, TelSimSapProtocol protocol,
						TapiResponseCb callback, void *user_data);

/**
 *  @brief This API is used to power ON/OFF or reset the SIM card.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] power_mode #TelSapPowerMode Indicates the type of requested power mode
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_FAILURE_NO_REASON</b>: No Reason\n
 *  <b>TEL_SAP_RESULT_CARD_NOT_ACCESSIBLE</b>: Not Accessible\n
 *  <b>TEL_SAP_RESULT_CARD_ALREADY_POWERED_OFF</b>: Card Already Powered Off\n
 *  <b>TEL_SAP_RESULT_CARD_REMOVED</b>: Card Removed\n
 *  <b>TEL_SAP_RESULT_CARD_ALREADY_POWERED_ON</b>: Card Already Powered On\n
 *  \em data NULL
 */
TelReturn tapi_sap_req_power_operation(TelHandle *handle, TelSapPowerMode power_mode,
						TapiResponseCb callback, void *user_data);

/**
 *  @brief This API is used to get the card reader status.
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Request accepted successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] callback #TapiResponseCb Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelSapResult values:\n
 *  <b>TEL_SAP_RESULT_SUCCESS</b>: Operation successfully completed\n
 *  <b>TEL_SAP_RESULT_FAILURE_NO_REASON</b>: No Reason\n
 *  <b>TEL_SAP_RESULT_DATA_NOT_AVAILABLE</b>: Data Not Available\n
 *  \em data #TelSapCardStatus : Valid only when TelSapResult is TEL_SAP_RESULT_SUCCESS
 */
TelReturn tapi_sap_get_cardreader_status(TelHandle *handle, TapiResponseCb callback,
						void *user_data);

#ifdef __cplusplus
}
#endif

/**
* @}
*/
#endif	/* __TAPI_SAP_H__ */
