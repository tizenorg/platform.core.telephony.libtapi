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

#ifndef _ITAPI_SIM_H_
#define _ITAPI_SIM_H_

/**
 * @file ITapiSim.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_SIM
 * @{
 */

#include <tapi_common.h>
#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Gets SIM card initialization status and SIM card identification.
 * @details Most of the modules which require SIM data should definitely check SIM status before using it.\n
 *
 * <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks Card identification value is available in case of #TAPI_SIM_STATUS_SIM_INIT_COMPLETED only.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] sim_status The SIM initialization status from the Telephony server boot up time \n
 *                        First, you can find SIM existence if the returned value is not in #TAPI_SIM_STATUS_CARD_NOT_PRESENT and #TAPI_SIM_STATUS_CARD_REMOVED.
 *
 * @param[out] card_changed The SIM card identification value when @a sim_status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED \n
 *                          The @a card_changed value is present when the current inserted SIM card differs from the previous SIM.
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 */
int tel_get_sim_init_info(TapiHandle *handle, TelSimCardStatus_t *sim_status, int *card_changed);

/**
 * @brief Gets the card type (SIM/USIM).
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] card_type The SIM card type information such as GSM SIM card, USIM, and other unknown types
 *                       TAPI_SIM_CARD_TYPE_UNKNOWN
 *                       TAPI_SIM_CARD_TYPE_GSM
 *                       TAPI_SIM_CARD_TYPE_USIM
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation is completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code while actual operations (Refer #TelSimAccessResult_t)
 */
int tel_get_sim_type(TapiHandle *handle, TelSimCardType_t *card_type);

/**
 * @brief Gets IMSI information.
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks IMSI value is the value that security requires.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] imsi The IMSI information \n
 *                  Refer #TelSimImsiInfo_t.
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 */
int tel_get_sim_imsi(TapiHandle *handle, TelSimImsiInfo_t *imsi);

/**
 * @brief Gets ECC(SIM) or UECC(USIM) data.
 * @details <b> Sync (or) Async: </b> This is a Synchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[out] ecc The SIM emergency call code information like ECC length, service type, and the number of ECC records
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 */
int tel_get_sim_ecc(TapiHandle *handle, TelSimEccList_t *ecc);

/**
 * @brief Gets the unique identification number of the (U)ICC.
 * @details ICC means Integrated Circuit Card.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @remarks ICC number value is the value that security needs.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation is completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimIccIdInfo_t will be stored in data on success case.
 */
int tel_get_sim_iccid(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets language preference(indication) information.
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimLanguagePreferenceCode_t will be stored in data on success case.
 */
int tel_get_sim_language(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Updates language preference information to the SIM card.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks This function updates SIM Language information directly and can have an effect on the device language setting. Use carefully.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] language The language preference information
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 * @post callback will be invoked and data is NULL.
 */
int tel_set_sim_language(TapiHandle *handle, TelSimLanguagePreferenceCode_t language, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM call forwarding indication related data(EF-CFIS and CPHS case).
 * @details This data is required for Call forwarding related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimCallForwardingResp_t will be stored in data on success case.
 */
int tel_get_sim_callforwarding_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets SIM call forwarding indication related data(EF-CFIS and CPHS case).
 * @details This data is required for Call forwarding related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] req_cf The data requesting for call forwarding
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and data is NULL.
 */
int tel_set_sim_callforwarding_info(TapiHandle *handle, TelSimCallForwardingReq_t *req_cf, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM message waiting indication related data(EF-MWIS and CPHS case).
 * @details This data is required for Message waiting related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimMessageWaitingResp_t will be stored in data on success case.
 */
int tel_get_sim_messagewaiting_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets SIM message waiting indication related data(EF-MWIS and CPHS case).
 * @details This data is required for Message waiting related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] req_mw The data requesting for message waiting
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and data is NULL.
 */
int tel_set_sim_messagewaiting_info(TapiHandle *handle, TelSimMessageWaitingReq_t *req_mw, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM mailbox related data(EF-MBDN, MBDI, and CPHS case).
 * @details This data is required for Mailbox related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimMailboxList_t will be stored in data on success case.
 */
int tel_get_sim_mailbox_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Sets SIM mailbox related data(EF-MBDN, MBDI and CPHS case).
 * @details This data is required for Mailbox related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] req_mb The data requesting for mailbox info
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and data is NULL.
 */
int tel_set_sim_mailbox_info(TapiHandle *handle, TelSimMailBoxNumber_t *req_mb, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM CPHS specific data.
 * @details This data is required for CPHS related functionality in the handset.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : it provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : it provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimCphsInfo_t will be stored in data on success case.
 */
int tel_get_sim_cphs_info(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the SIM Service Table.
 * @details This data is required to know which SIM services are enabled or disabled.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimServiceTable_t will be stored in data on success case.
 */
int tel_get_sim_service_table(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM MSISDN data.
 * @details Regarding 3GPP specification, the current value is optional.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimMsisdnList_t will be stored in data on success case.
 */
int tel_get_sim_msisdn(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM OPLMNWACT(Operator controlled PLMN Selector with Access Technology) data.
 * @details Regarding 3GPP specification, the current value is optional.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code while actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimOplmnwactList_t will be stored in data on success case.
 */
int tel_get_sim_oplmnwact(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM SPN data.
 * @details Regarding 3GPP specification, the current value is optional.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return value or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimSpn_t will be stored in data on success case.
 */
int tel_get_sim_spn(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM CPHS NETNAME data.
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimCphsNetName_t will be stored in data on success case.
 */
int tel_get_sim_cphs_netname(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @brief Executes an authentication procedure by using SIM.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.\n
 *          This function supports IMS, 3G, and GSM authentication.\n
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.\n
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] authentication_data The authentication code to be validated by the ISIM, 3G, and GSM application in the SIM card \n
 *                                Refer #TelSimAuthenticationData_t.
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 * @post callback will be invoked and #TelSimAuthenticationResponse_t will be stored in data on success case.
 */
int tel_req_sim_authentication(TapiHandle *handle, TelSimAuthenticationData_t *authentication_data, tapi_response_cb callback, void *user_data);

/**
 * @deprecated Deprecated Since 2.4. Use tel_verify_sim_pins().
 * @brief Performs PIN1/PIN2/SIM LOCK verification.
 * @details This function performs PIN verification based on the PIN type passed along with @a pin_data:\n
 *          PIN1 code: The SIM card has its own password to check access permissions.\n
 *          SIM Lock code: Between Device and SIM card using a SIM password is only available on certain handsets to authorize.\n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks If the PIN status (PIN/PUK/SIM LOCK) is blocked, an unsolicited event #TAPI_EVENT_SIM_STATUS_IND will be published to Applications on Device boot time.
 *          If the PIN status is PUK, you should use #tel_verify_sim_puks instead of this API.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pin_data The PIN code, entered by the user \n
 *                     You should make all the parameters.
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation is completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre This API is used when #TelSimCardStatus_t is #TAPI_SIM_STATUS_SIM_PIN_REQUIRED or #TAPI_SIM_STATUS_SIM_LOCK_REQUIRED on boot time.
 * @post callback will be invoked and #TelSimSecResult_t will be stored in data on success case.
 */
int tel_verifiy_sim_pins(TapiHandle *handle, const TelSimSecPw_t *pin_data, tapi_response_cb callback, void *user_data);

/**
 * @brief Performs PIN1/PIN2/SIM LOCK verification.
 * @details This function performs PIN verification based on the PIN type passed along with @a pin_data:\n
 *          PIN1 code: The SIM card has its own password to check access permissions.\n
 *          SIM Lock code: Between Device and SIM card using a SIM password is only available on certain handsets to authorize.\n
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks If the PIN status (PIN/PUK/SIM LOCK) is blocked, an unsolicited event #TAPI_EVENT_SIM_STATUS_IND will be published to Applications on Device boot time.
 *          If the PIN status is PUK, you should use #tel_verify_sim_puks instead of this API.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pin_data The PIN code, entered by the user \n
 *                     You should make all the parameters.
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation is completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre This API is used when #TelSimCardStatus_t is #TAPI_SIM_STATUS_SIM_PIN_REQUIRED or #TAPI_SIM_STATUS_SIM_LOCK_REQUIRED on boot time.
 * @post callback will be invoked and #TelSimSecResult_t will be stored in data on success case.
 */
int tel_verify_sim_pins(TapiHandle *handle, const TelSimSecPw_t *pin_data, tapi_response_cb callback, void *user_data);

/**
 * @brief Performs PIN1/PIN2 unblocking operation based on PUK information passed along with
 *        unblock information entered by the user.
 *
 * @details If you get the SIM card status (#TAPI_SIM_STATUS_SIM_PUK_REQUIRED) using #tel_get_sim_init_info on boot time
 *          or (#TAPI_SIM_PIN_STATUS_BLOCKED) using #tel_get_sim_facility after normal initialization for a specific operation,
 *          you may unblock PIN1/PIN2.
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks If the PIN status (PIN1/PIN2) is blocked, an unsolicited event #TAPI_EVENT_SIM_STATUS_IND
 *          will be published to Applications on boot time.
 *          If you exceed the specified number of attempts to unblock PIN, the SIM card will be blocked permanently.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] puk_data The unblocking PIN password #TelSimSecPw_t
 *
 * @param[in] new_pin_data The PIN password to use after the unblocking operation #TelSimSecPw_t
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre PIN1 or PIN2 status should be #TAPI_SIM_PIN_STATUS_BLOCKED and Card status should be #TAPI_SIM_STATUS_SIM_PUK_REQUIRED on boot time.
 *
 * @post callback will be invoked and #TelSimSecResult_t will be stored in data on success case.
 * @post After success on unblocking for PIN1 or PIN2, each pin check facility is enabled even if the previous facility is disabled.
 */
int tel_verify_sim_puks(TapiHandle *handle, const TelSimSecPw_t *puk_data, const TelSimSecPw_t *new_pin_data, tapi_response_cb callback, void *user_data);

/**
 * @brief Changes the PIN1/PIN2 code based on the PIN type passed along with old PIN data and new PIN data.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @remarks The user should set the same PIN type for both @a old_pin and @a new_pin.
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] old_pin The old PIN code entered by the user \n
 *                    Refer #TelSimSecPw_t.
 *
 * @param[in] new_pin The new PIN code entered by the user \n 
 *                    Refer #TelSimSecPw_t.
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation is completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimSecResult_t will be stored in data on success case.
 */
int tel_change_sim_pins(TapiHandle *handle, const TelSimSecPw_t *old_pin, const TelSimSecPw_t *new_pin, tapi_response_cb callback, void *user_data);

/**
 * @brief Disables the SIM facility.
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pw A structure which contains the facility type and password
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 * @post callback will be invoked and #TelSimFacilityResult_t will be stored in data on success case.
 */
int tel_disable_sim_facility(TapiHandle *handle, TelSimFacilityPw_t *pw, tapi_response_cb callback, void *user_data);

/**
 * @brief Enables the SIM facility.
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] pw A structure which contains the facility type and password
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 * @post callback will be invoked and #TelSimFacilityResult_t will be stored in data on success case.
 */
int tel_enable_sim_facility(TapiHandle *handle, TelSimFacilityPw_t *pw, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets the SIM facility.
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] type The type of security lock
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 * @post callback will be invoked and #TelSimFacilityInfo_t will be stored in data on success case.
 */
int tel_get_sim_facility(TapiHandle *handle, TelSimLockType_t type, tapi_response_cb callback, void *user_data);

/**
 * @brief Gets SIM LOCK TYPE info.
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] type The type of security lock
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 * @post callback will be invoked and #TelSimLockInfo_t will be stored in data on success case.
 */
int tel_get_sim_lock_info(TapiHandle *handle, TelSimLockType_t type, tapi_response_cb callback, void *user_data);

/**
 * @brief  Sets the SIM power state.
 *
 * @details <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param [in] handle The handle from tel_init()
 *
 * @param[in] state The state of SIM to be set \n
 *                  It is either ON or OFF.
 *
 * @param [in] callback To register a callback function for result
 *
 * @param [in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_SIM_POWER_SET_SUCCESS ) - indicates that the operation has completed successfully
 *         Positive integer : It provides an error code while actual operations (Refer #TelSimPowerSetResult_t)
 * @post callback will be invoked and data is NULL.
 */
int tel_set_sim_power_state(TapiHandle *handle, TelSimPowerState_t state, tapi_response_cb callback, void *user_data);

/**
 * @brief Provides a common interface for accessing SIM data.
 *
 * @details This function is based on the USIM generic command in ETSI TS 102 221
 *          and 3GPP TS 31.102 Characteristics of the Universal Subscriber Identity Module (USIM) application.
 *
 *          This API provides only an interface to send APDU. The user should know well how to use APDU.
 *
 *          This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/telephony.admin
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] apdu_data The APDU data
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS )  - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimApduResp_t will be stored in data on success case.
 */
int tel_req_sim_apdu(TapiHandle *handle, TelSimApdu_t* apdu_data, tapi_response_cb callback, void *user_data);

/**
 * @brief Provides a common interface to get the SIM ATR(Answer To Reset) value.
 *
 * @details This function makes a Dbus method call to the Telephony Server and returns an immediate value.
 *          However it just means that the API request has been transfered to the CP successfully.
 *          The actual operation result is delivered in the corresponding event asynchronously.
 *
 * <b> Sync (or) Async: </b> This is an Asynchronous API.
 *
 * <b> Prospective Clients: </b> External Apps.
 *
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 *
 * @param[in] callback To register a callback function for result
 *
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimAtrResp_t will be stored in data on success case.
 */
int tel_req_sim_atr(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @breif Gets the list of application on UICC.
 * @details It is possible to have multiple applications on single UICC.
 *
 * <b> Sync (or) Async: </b> This is Synchronous API.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[out] app_list The masking value for below values
 *                      #TAPI_SIM_APP_TYPE_SIM 0x01 GSM Application
 *                      #TAPI_SIM_APP_TYPE_USIM 0x02 USIM Application
 *                      #TAPI_SIM_APP_TYPE_CSIM 0x04 CSIM Application
 *                      #TAPI_SIM_APP_TYPE_ISIM 0x08 ISIM Application
 *
 * @return The return type (int)
 *         Integer '0' (same with #TAPI_API_SUCCESS) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 */
int tel_get_sim_application_list(TapiHandle *handle, unsigned char *app_list);

/**
 * @breif Gets the IMPI(IMS private user identity). (ISIM only)
 * @details Private user identity of the user.
 *
 * <b> Sync (or) Async: </b> This is Asynchronous API.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To reigster a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimImpi_t will be stored in data on success case.
 */
int tel_get_sim_impi(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @breif Gets the IMPU(IMS public user identity). (ISIM only)
 * @details SIP URI by which other parties know the subscriber.
 *
 * <b> Sync (or) Async: </b> This is Asynchronous API.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To reigster a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimImpuList_t will be stored in data on success case.
 */
int tel_get_sim_impu(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @breif Gets the Domain(Home Network Domain Name). (ISIM only)
 * @details Home Network Domain Name.
 *
 * <b> Sync (or) Async: </b> This is Asynchronous API.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To reigster a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimDomain_t will be stored in data on success case.
 */
int tel_get_sim_domain(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @breif Gets the P-CSCF(Proxy Call Session Control Function). (ISIM only)
 * @details Address of Proxy Call Session Control Function, in the format of a FQDN, an IPv4 address, or an IPv6 address. \n
 *          A UE supporting IMS Local Breakout shall use this EF only if EFIST indicates that service n=5 is "available".
 *
 * <b> Sync (or) Async: </b> This is Asynchronous API.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To reigster a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimPcscfList_t will be stored in data on success case.
 */
int tel_get_sim_pcscf(TapiHandle *handle, tapi_response_cb callback, void *user_data);

/**
 * @breif Gets the ISIM service table. (ISIM only)
 * @details This EF indicates which optional services are available. \n
 *          If a service is not indicated as available in the ISIM, the ME shall not select this service.
 *
 * <b> Sync (or) Async: </b> This is Asynchronous API.
 *
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/telephony
 *
 * @param[in] handle The handle from tel_init()
 * @param[in] callback To reigster a callback function for result
 * @param[in] user_data The user data for user specification
 *
 * @return The return type (int)
 *         Integer '0' ( same with #TAPI_API_SUCCESS & #TAPI_SIM_ACCESS_SUCCESS ) - indicates that the operation has completed successfully
 *         Negative integer : It provides an error code before actual operations (Refer #TapiResult_t)
 *         Positive integer : It provides an error code during actual operations (Refer #TelSimAccessResult_t)
 *
 * @pre The user can get valid return values or make operations after SIM init completes(card status is #TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 * @post callback will be invoked and #TelSimIST_t will be stored in data on success case.
 */
int tel_get_sim_isim_service_table(TapiHandle *handle, tapi_response_cb callback, void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* _ITAPI_SIM_H_ */

/**
 * @}
 */
