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
* @addtogroup	PHONEBOOK_TAPI	PHONEBOOK
* @{
*
* @file ITapiPhonebook.h

     @brief This file serves as a "C" header file defines functions for Tapi SIM phonebook Services.\n
      It contains a sample set of function prototypes that would be required by applications.

*/

#ifndef _ITAPI_PHONEBOOK_H_
#define _ITAPI_PHONEBOOK_H_

#include <tapi_common.h>
#include <TelErr.h>
#include <TelSim.h>
#include <TelDefines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief  This function is used to get current inserted SIM phonebook init status, available phonebook list and first valid index in case of FDN, ADN, 3G phonebook.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * @par Sync (or) Async:
 * This is an synchronous API.
 *
 * @par Important Notes:
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
 * pb_list and first_index value are available at init_completed status is '1' only.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param [out] init_completed
 *   - phonebook init status. '0' is not init, '1' is init complete to use.
 *
 * @param [out] pb_list
 *   - available SIM phonebook list. This value is valid in phonebook init complete case.
 *
 * @par Async Response Message:
 * - None.
 *
 * @pre
 *  - None.
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int valid_index = 0; // usim or sim adn first valid index
 * TelSimPbList_t pb_list = {0,};
 * int pPbInitCompleted = 0; // init or not
 *
 * // GET SIM PHONEBOOK INIT INFORMATION
 * err_code = tel_get_sim_pb_init_info(&pPbInitCompleted, &pb_list, &valid_index);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_pb_init_info(TapiHandle *handle, int *init_completed, TelSimPbList_t *pb_list);


/**
 * @brief  This API is used to get number of used records and total records in specific SIM phonebook type.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbType_t
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF  and the event data is #TelSimPbStorageInfo_t.
 * Asynchronous return status is indicated by #TelSimPbAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int request_id = 0;
 * TelSimPbType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_get_sim_pb_count(storage_type, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_pb_count(TapiHandle *handle, TelSimPbType_t pb_type, tapi_response_cb callback, void *user_data);


/**
 * @brief  This API is used to get max text length and max number length supported by SIM phone book elementary file.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - The max number length includes the storage space provided by the corresponding EXT file for a given Dialling Number file.
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbType_t
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF  and the event data is #TelSimPbEntryInfo_t.
 * Asynchronous return status is indicated by #TelSimPbAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int request_id = 0;
 * TelSimPbType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_get_sim_pb_meta_info(storage_type, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_pb_meta_info(TapiHandle *handle, TelSimPbType_t pb_type, tapi_response_cb callback, void *user_data);


/**
 * @brief  This API is used to get SIM 3G phonebook supported EFs like ANR, SNE, GRP, EMAIL etc and corresponding EFs max text length, number length and size.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
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
 * @par Async Response Message:
 * - The event associated is TAPI_SIM_EVENT_PB_3G_CAPABILITY_INFO_CNF  and the event data is #TelSimPbCapabilityInfo_t.
 * Asynchronous return status is indicated by #TelSimPbAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int request_id = 0;
 * err_code = tel_get_sim_pb_3g_meta_info(&request_id); // you can find result by receiving asynch event response
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_pb_usim_meta_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);


/**
 * @brief  This API is used to read SIM phone book entry information from given storage type.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - The index ranges from 1 through to a maximum of 254 for a Linear fixed file and 255 for a cyclic file.
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
 *
 * @warning
 * - None.
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbType_t
 *
 * @param[in] index
 * -Index for accessing the SIM data.
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_PB_ACCESS_READ_CNF  and the event data is #TelSimPbRecordData_t.
 * Asynchronous return status is indicated by #TelSimPbAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int request_id = 0;
 * unsigned short index = 1;
 * TelSimPbType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_read_sim_pb_record(storage_type, index, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_read_sim_pb_record(TapiHandle *handle, TelSimPbType_t pb_type, unsigned short index, tapi_response_cb callback, void *user_data);


/**
 * @brief  This API is used to add or edit SIM phone book record entry information.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param[in] req_data
 * -phonebook data which will be updated or added. #TelSimPbRecordData_t
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_PB_UPDATE_CNF  and the event data is #TelSimPbUpdateResp_t.
 * Asynchronous return status is indicated by #TelSimPbAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int request_id = 0;
 * unsigned short index = 1;
 * TelSimPbRecordData_t data;
 * data.StorageFileType  = TAPI_SIM_PB_3GSIM; // usim phonebook
 * data.Index  = 1; // index which will be updated
 * data.NextIndex  = 0;
 * //data.ContactInfo will be added
 * err_code = tel_update_sim_pb_record(&data, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_update_sim_pb_record(TapiHandle *handle, const TelSimPbRecord_t *req_data, tapi_response_cb callback, void *user_data);


/**
 * @brief  This API is used to delete a SIM phonebook record.
 *	Access to this API is limited to in-house application and we recommend you use phonebook-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - The index ranges from 1 through to a maximum of 254 for a Linear fixed file and 255 for a cyclic file.
 * - We recommend to use phonebook-engine APIs for handling phonebook(including SIM phonebook).
 * If user uses SIM phonebook related APIs directly, it can break device phonebook consistency because all phonebook information managed in phonebook-engine.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] handle
 * - handle from tel_init().
 *
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbType_t
 *
 * @param[in] index
 * -Index of the record to be deleted.
 *
 * @param [in] callback
 * - To register callback function for result.
 *
 * @param [in] user_data
 * - user_data for user specification.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_PB_DELETE_CNF  and the event data is #TelSimPbUpdateResp_t.
 * Asynchronous return status is indicated by #TelSimPbAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
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
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * int err_code = 0;
 * int request_id = 0;
 * unsigned short index = 1;
 * TelSimPbType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_delete_sim_pb_record(storage_type, index, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_delete_sim_pb_record(TapiHandle *handle, TelSimPbType_t pb_type, unsigned short index, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_PHONEBOOK_H_ */

/**
* @}
*/
