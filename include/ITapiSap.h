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

/**
* @open
* @ingroup		TelephonyAPI
* @addtogroup	SAP_TAPI	SAP
* @{
*
* @file ITapiSap.h

     @brief This file serves as a "C" header file defines functions for Tapi SAP Services.\n
      It contains a sample set of function prototypes that would be required by applications.

*/

#ifndef _ITAPI_SAP_H_
#define _ITAPI_SAP_H_

#include <tapi_common.h>
#include <TelErr.h>
#include <TelSim.h>
#include <TelDefines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  This API is used to request to connect sap.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param[in] max_msg_size
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_connect(TapiHandle *handle, int max_msg_size, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request sap to disconnect sap.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_disconnect(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request sap connection status.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_connection_status(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request to get atr.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_transfer_atr(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request to transfer apdu.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] apdu_data
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_transfer_apdu(TapiHandle *handle, TelSapApduData_t *apdu_data, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request to transport protocol.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] protocol
 * - SAP transport protocol type
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_transport_protocol(TapiHandle *handle, TelSimSapProtocol_t protocol, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request to control power_mode.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] power_mode
 * - enum data for controlling power mode of client
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_power_operation(TapiHandle *handle, TelSimSapPowerMode_t power_mode, tapi_response_cb callback, void *user_data);

/**
 * @brief  This API is used to request to get card_reader_status.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par
 *
 * @pre
 *
 * @post
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @see None
 *
 * @code
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_cardreader_status(TapiHandle *handle, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_SAP_H_ */

/**
* @}
*/
