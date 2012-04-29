/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
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
 * @internal
 * @ingroup           TELEPHONY_API
 * @addtogroup	OMADM OMA-DM
 * @{
 * @file ITapiOmadm.h
 *
 * OMA-DM APIs allow an application to accomplish the following services: @n
 * - Get Model Name. @n
 */

#ifndef _ITAPI_OMADM_H_
#define _ITAPI_OMADM_H_

#include <TelDefines.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TAPI_OMADM_MAX_PRL_SIZE_LEN		17		/** Maximum length of PRL size string */
#define TAPI_OMADM_MAX_MODEL_NAME_LEN	17		/** Maximum length of Model name string */
#define TAPI_OMADM_MAX_OEM_NAME_LEN		65		/** Maximum length of OEM name string */
#define TAPI_OMADM_MAX_SW_VER_LEN		17		/** Maximum length of SW version string */
#define TAPI_OMADM_MAX_PRL_DATA_LEN		237		/** Maximum length of PRL data */

/**
 * @enum TelDataSignalType
 * Data Signal Type
 */
typedef enum {
	TAPI_OMADM_PRL_WRITE_SUCCESS = 0x00, /**< Success */
	TAPI_OMADM_PRL_WRITE_INVALID_SIZE = 0x01, /**< PRL size is invalid */
	TAPI_OMADM_PRL_WRITE_FAIL = 0x02 /**< Fail */
} TelOMADMPRLWriteStatus_t;

/**
 * This structure contains PRL size string.
 */
typedef struct {
	unsigned char prl_size[TAPI_OMADM_MAX_PRL_SIZE_LEN]; /**< PRL size string information */
} TelOMADMPRLSize_t;

/**
 * This structure contains Model name string.
 */
typedef struct {
	unsigned char model_name[TAPI_OMADM_MAX_MODEL_NAME_LEN]; /**< model name information */
} TelOMADMModelName_t;

/**
 * This structure contains OEM name string.
 */
typedef struct {
	unsigned char oem_name[TAPI_OMADM_MAX_OEM_NAME_LEN]; /**< OEM information */
} TelOMADMOEMName_t;

/**
 * This structure contains S/W version string.
 */
typedef struct {
	unsigned char sw_ver[TAPI_OMADM_MAX_SW_VER_LEN]; /**< S/W version information */
} TelOMADMSWVersion_t;

/**
 * This structure contains PRL data packet.
 */
typedef struct {
	unsigned int total_size; /**< total size */
	unsigned short current_size; /**< current size */
	unsigned char more; /**< more data or not */
	unsigned char prl_data[TAPI_OMADM_MAX_PRL_DATA_LEN]; /**< PRL data information */
} TelOMADMPRLData_t;

/**
 * @brief This function requests PRL size.
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
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
 * - The event associated with this request is TAPI_EVENT_OMADM_PRL_SIZE_GET_CNF.
 *
 * @pre
 *  - None
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_omadm_prl_size(int *pRequestID);

/**
 * @brief This function requests Model name.
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
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
 * - The event associated with this request is TAPI_EVENT_OMADM_MODEL_NAME_GET_CNF.
 *
 * @pre
 *  - None
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_omadm_model_name(int *pRequestID);

/**
 * @brief This function requests OEM name.
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
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
 * - The event associated with this request is TAPI_EVENT_OMADM_OEM_NAME_GET_CNF.
 *
 * @pre
 *  - None
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_omadm_oem_name(int *pRequestID);

/**
 * @brief This function requests SW version.
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_OMADM_SW_VERSION_GET_CNF.
 *
 * @pre
 *  - None
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_omadm_sw_ver(int *pRequestID);

/**
 * @brief This function requests read PRL data.
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
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
 * - The event associated with this request is TAPI_EVENT_OMADM_PRL_READ_GET_CNF.
 *
 * @pre
 *  - None
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_read_omadm_ril(int *pRequestID);

/**
 * @brief This function requests write PRL data.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 *   - None.
 *
 * @warning
 * - None.
 *
 * @param[in] pPrlData
 * - PRL data.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - Request Id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated with this request is TAPI_EVENT_OMADM_PRL_WRITE_SET_CNF.
 *
 * @pre
 *  - None
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_write_omadm_prl(TelOMADMPRLData_t *pPrlData, int *pRequestID);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_OMADM_H_ */

/**
 * @}
 */

