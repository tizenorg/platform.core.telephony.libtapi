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

/**
 * @internal
 * @ingroup           TELEPHONY_API
 * @addtogroup	Data_Service Data Services
 * @{
 * @file ITapiData.h
 *
 * Data APIs allow an application to accomplish the following services: @n
 * - process data pin control. @n
 */

#ifndef _ITAPI_DATA_H_
#define _ITAPI_DATA_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/
#include <TelDefines.h>
#include <TelUtility.h>
#include <TelData.h>

#ifdef __cplusplus
extern "C" {
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

/**
 *
 * @brief  Request current MIP NAI index
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [in]  pMipNaiIdx
 * -  Mobile IP NAI Index.
 *
 * @param  [in]  pParamMask
 * -  Parameter mask.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_mobile_ipnai(TelDataNAIIndex_t *pMipNaiIdx, unsigned short *pParamMask, int *pRequestID);

/**
 *
 * @brief  Enable the NAI index by the IPC parameter and then set the detail values for NAI
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [in]  pMipNaiParam
 * -  Mobile IP NAI Parameters.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_set_data_mobile_ipnai(TelDataMipNaiParam_t *pMipNaiParam, int *pRequestID);

/**
 *
 * @brief Get current NAI index which is now enabled.
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_current_nai_index(int *pRequestID);

/**
 *
 * @brief MS must report Data Cable(UART/USB/Blue tooth) attachment status whenever the status changed. PDA should reflect this status change on LCD.
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_te2_status(int * pRequestID);

/**
 *
 * @brief This API gets the roam guard.
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_roam_guard(int *pRequestID);

/**
 *
 * @brief This API sets a roam guard.
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  pRoamGuard
 * -  Roam guard.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_set_data_roam_guard(TelDataRoamGuardMode_t *pRoamGuard, int *pRequestID);

/**
 *
 * @brief This API gets a modem NAI.
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_nai(int *pRequestID);

/**
 *
 * @brief This API sets a modem NAI.
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  pModemNai
 * -  Modem NAI.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_set_data_nai(TelDataModemNAIMode_t *pModemNai, int *pRequestID);

/**
 *
 * @brief This API sets a data service type
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  pDSType
 * -  Data service type
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_set_data_ds_type(TelDataDSType_t *pDSType, int *pRequestID);

/**
 *
 * @brief This API gets EVDO revision config
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_evdo_rev_config(int *pRequestID);

/**
 *
 * @brief This API sets EVDO revision config
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  pRevConfig
 * -  EVDO Revision config information.
 *
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_set_data_evdo_rev_config(TelDataRevConfig_t *pRevConfig, int *pRequestID);

/**
 *
 * @brief This API restores NAI
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_restore_data_nai(int *pRequestID);

/**
 *
 * @brief This API gets Dedicate Data Transmission Mode setting. When it is set, Modem block the 1X paging
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_get_data_ddtm_config(int *pRequestID);

/**
 *
 * @brief This API sets Dedicate Data Transmission Mode setting. When it is set, Modem block the 1X paging.
 *
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.

 * @param  [in]  pDDTMStatus
 * -  DDTM setting information.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - In case of failure, error code will be returned. (Refer #int).
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_set_data_ddtm_config(TelDataDDTMStatus_t *pDDTMStatus, int *pRequestID);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_DATA_H_ */

/**
 * @}
 */

