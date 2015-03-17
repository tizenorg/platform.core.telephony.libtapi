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
 * @file ITapiSap.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SAP
 * @{
 */

#ifndef _ITAPI_SAP_H_
#define _ITAPI_SAP_H_

#include <tapi_common.h>
#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Requests to connect SAP.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] max_msg_size The maximum message size
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_connect(TapiHandle *handle, int max_msg_size, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests to disconnect SAP.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_disconnect(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests for the SAP connection status.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n 
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n 
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_connection_status(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests to get atr.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n 
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n 
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_transfer_atr(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests to transfer APDU.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] apdu_data The APDU data
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n 
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n 
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_transfer_apdu(TapiHandle *handle, TelSapApduData_t *apdu_data, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests to transport protocol.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] protocol The SAP transport protocol type
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n 
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully\n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_transport_protocol(TapiHandle *handle, TelSimSapProtocol_t protocol, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests to control power mode.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] power_mode The data to control the power mode of client
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n 
 *         Integer '0' ( same with TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n 
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_power_operation(TapiHandle *handle, TelSimSapPowerMode_t power_mode, tapi_response_cb callback, void *user_data);

/**
 * @brief Requests to get the card reader status.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To register a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int) \n 
 *         Integer '0' ( same with #TAPI_API_SUCCESS ): indicates that the operation has completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 */
int tel_req_sap_cardreader_status(TapiHandle *handle, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_SAP_H_ */

/**
 * @}
 */
