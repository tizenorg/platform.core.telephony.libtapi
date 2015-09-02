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
 * @file ITapiPhonebook.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_PHONEBOOK
 * @{
 */

#ifndef _ITAPI_PHONEBOOK_H_
#define _ITAPI_PHONEBOOK_H_

#include <tapi_common.h>
#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Gets the current inserted SIM phonebook init status, available phonebook list, and first valid index in case of FDN, ADN, and 3G phonebook.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *
 *          <b> Sync (or) Async: </b> This is a synchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks You must use phonebook-engine APIs to handle a phonebook (including SIM phonebook).
 *          If a user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *          The @a pb_list and @a first_index value are available when the @a init_completed status is '1' only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] init_completed The phonebook init status to use - '0' is not init, '1' is init complete
 *
 * @param[out] pb_list The available SIM phonebook list; this value is valid in the phonebook init complete case
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully\n
 *         Negative integer - It provides an error code (Refer #TapiResult_t).
 */
int tel_get_sim_pb_init_info(TapiHandle *handle, int *init_completed, TelSimPbList_t *pb_list);

/**
 * @brief Gets the number of used records and total records of a specific SIM phonebook type.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 *          <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks You must use phonebook-engine APIs to handle a phonebook(including SIM phonebook).
 *          If the user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pb_type The different storage types to be selected in the SIM; #TelSimPbType_t
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully\n
 *         Negative integer - It provides an error code (Refer #TapiResult_t).
 *
 * @pre Initialize the Dbus connection with #tel_init.
 * @post callback will be invoked and #TelSimPbStorageInfo_t will be stored in data on success case.
 */
int tel_get_sim_pb_count(TapiHandle *handle, TelSimPbType_t pb_type, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the max text length and max number length supported by the SIM phone book elementary file.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *         <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *         <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks The max number length includes the storage space provided by the corresponding EXT file for a given Dialling Number file.\n
 *          You must use phonebook-engine APIs to handle a phonebook(including SIM phonebook).
 *          If a user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pb_type The different storage types to be selected in the SIM; #TelSimPbType_t
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully \n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 *
 * @pre Initialize the Dbus connection with #tel_init.
 * @post callback will be invoked and #TelSimPbEntryInfo_t will be stored in data on success case.
 */
int tel_get_sim_pb_meta_info(TapiHandle *handle, TelSimPbType_t pb_type, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM 3G phonebook supported EFs like ANR, SNE, GRP, EMAIL and the corresponding EFs max text length, number length, and size.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However, it just means that the API request has been transferred to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 *          <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks You must use phonebook-engine APIs to handle a phonebook (including SIM phonebook).
 *          If a user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully\n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 *
 * @pre Initialize Dbus connection with #tel_init\n
 * @post callback will be invoked and #TelSimPbCapabilityInfo_t will be stored in data on success case.
 */
int tel_get_sim_pb_usim_meta_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Reads SIM phone book entry information from the given storage type and index.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 *          <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks The index ranges from 1 to a maximum of 254 for a Linear fixed file and 255 for a cyclic file.\n
 *          You must use phonebook-engine APIs to handle a phonebook (including SIM phonebook).
 *          If a user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pb_type The different storage types to be selected in the SIM; #TelSimPbType_t
 *
 * @param[in] pb_index The index for accessing the SIM data
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully\n
 *         Negative integer : It provides an error code (Refer #TapiResult_t)
 *
 * @pre Initialize the Dbus connection with #tel_init.
 * @post callback will be invoked and #TelSimPbRecord_t will be stored in data on success case.
 */
int tel_read_sim_pb_record(TapiHandle *handle, TelSimPbType_t pb_type, unsigned short pb_index, tapi_response_cb callback, void *user_data);

/**
 * @brief Adds or edits SIM phone book record entry information.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously. \n
 *
 *         <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *         <b> Prospective Clients: </b> External Apps. \n
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks You must use phonebook-engine APIs to handle a phonebook (including SIM phonebook).
 *          If a user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] req_data The phonebook data to be updated or added; #TelSimPbRecord_t
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully\n
 *         Negative integer : It provides an error code (Refer #TapiResult_t).
 *
 * @pre Initialize the Dbus connection with #tel_init.
 * @post callback will be invoked and data is NULL.
 */
int tel_update_sim_pb_record(TapiHandle *handle, const TelSimPbRecord_t *req_data, tapi_response_cb callback, void *user_data);

/**
 * @brief Deletes a SIM phonebook record.
 *
 * @details Access to this API is limited to in-house applications and phonebook-engine APIs are recommended instead. \n
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 *          <b> Sync (or) Async: </b> This is an Asynchronous API. \n
 *
 *          <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks The index ranges from 1 to a maximum of 254 for a Linear fixed file and 255 for a cyclic file.\n
 *          You must use phonebook-engine APIs to handle a phonebook(including SIM phonebook).
 *          If the user uses SIM phonebook related APIs directly, it can break device phonebook consistency because of all the phonebook information managed in the phonebook-engine.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pb_type The different storage types to be selected in the SIM; #TelSimPbType_t
 *
 * @param[in] pb_index The index of the record to be deleted
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)\n
 *         Integer '0' ( same with #TAPI_API_SUCCESS ) - indicates that the operation is completed successfully\n
 *         Negative integer : (It provides an error code (Refer #TapiResult_t).
 *
 * @pre Initialize the Dbus connection with #tel_init.
 * @post callback will be invoked and data is NULL.
 */
int tel_delete_sim_pb_record(TapiHandle *handle, TelSimPbType_t pb_type, unsigned short pb_index, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_PHONEBOOK_H_ */

/**
 * @}
 */
