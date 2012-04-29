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
* @open
* @ingroup		TelephonyAPI
* @addtogroup	SIM_TAPI	SIM
* @{
*
* @file ITapiSim.h

     @brief This file serves as a "C" header file defines functions for Tapi SIM Services.\n
      It contains a sample set of function prototypes that would be required by applications.

*/

#ifndef _ITAPI_SIM_H_
#define _ITAPI_SIM_H_

#include <TelSim.h>
#include <TelUtility.h>
#include <TelDefines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
* @brief  This API is used to get SIM Card Initialization status and SIM Card Identification.
* Most of modules which require SIM data should check SIM status before using it definitely.
 *
* @par Sync (or) Async:
* This is a Synchronous API.
*
* @par Important Notes:
* - Card Identification value is available in TAPI_SIM_STATUS_SIM_INIT_COMPLETED case only.
*
* @warning
* - None.
*
* @param [out] sim_status
* - This parameter returns SIM initialization status from Telephony server boot up time.
* First, You can find sim existance if returned value are not in TAPI_SIM_STATUS_CARD_NOT_PRESENT and TAPI_SIM_STATUS_CARD_REMOVED.
*
* @param [out] card_changed
* - This parameter returns sim card identification value when sim_status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED.
* - card_changed value just present whether current inserted SIM card differs with previous SIM or not.
*
* @par Async Response Message:
* - None
*
* @pre
*  - None.
*
* @post
*  - None.
*
* @return Return Type (int)
* - Integer '0' ( same with TAPI_API_SUCCESS ) - indicating that the operation has completed successfully. \n
* - Negative integer : it provides an error code (Refer #TapiResult_t)
*
* @par Prospective Clients:
* External Apps.
*
* @see None
*
* @code
* #include <ITapiSim.h>
*
* int err_code = TAPI_API_SUCCESS;
* TelSimCardStatus_t status = 0x00;
* int b_card_changed = 0;
*
* // GET SIM INIT INFORMATION
* err_code = tel_get_sim_init_info(&status, &b_card_changed);
*
* @endcode
*
* @remarks None
*
*/
/*================================================================================================*/
int tel_get_sim_init_info(TelSimCardStatus_t *sim_status, int *card_changed);


/**
 * @brief  This function performs PIN1/PIN2/SIM LOCK verification. This function performs PIN verification based on the pin type passed along with pin data.
 * PIN1 code : The SIM card has its own password is to check access permissions
 * SIM Lock code : Between Device and the SIM card using a SIM password is only available on certain handsets to be used to authorize.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - Based on Pin status (PIN/PUK/SIM LOCK) is blocked, an unsolicited event TAPI_EVENT_SIM_STATUS_IND is published to Applications on Device boot time.
 * - if pin status is PUK, you should use #tel_verify_sim_puks instead of this API.
 *
 * @warning
 * - None.
 *
 * @param[in] pin_data
 * -PIN code, entered by the user. you should make all parameters.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_VERIFY_SEC_CNF  and the event data is #TelSimSecResult_t.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 * If returned status is TAPI_SIM_PIN_OPERATION_SUCCESS, Verification successes.
 * If returned status is TAPI_SIM_PIN_INCORRECT_PASSWORD, Verification fails and you can check retry count by using event data  #TelSimSecResult_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 * - This API is used when #TelSimCardStatus_t is TAPI_SIM_STATUS_SIM_PIN_REQUIRED or TAPI_SIM_STATUS_SIM_LOCK_REQUIRED on boot time.
 * - Currently there`s no proper scenario for PIN2 verification by using this API.
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
 *
 * int request_id = 0;
 * err_code = TAPI_API_SUCCESS; // 0
 * int length = TAPI_SIM_PIN_LENGTH+1;
 * char init_pin_val[length] ;
 *
 * strcpy(init_pin_val, PIN1_NORMAL);
 *
 * TelSimSecPw_t pin_data = {0,};
 *
 * pin_data.type = TAPI_SIM_PTYPE_PIN1;
 * pin_data.pw_len = strlen(init_pin_val);
 *
 * printf("pw_len[%d]", pin_data.pw_len);
 *
 * pin_data.pw = (char*)malloc(length);
 * memcpy(pin_data.pw,init_pin_val,length);
 *
 * err_code = tel_verifiy_sim_pins(&pin_data, &request_id);
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_verifiy_sim_pins(const TelSimSecPw_t *pin_data, int *req_id);


/**
 * @brief  This function performs unblock PIN1/PIN2 operation based on PUK information passed along with
 * unblock information entered by the user.
 * If you get sim card status(TAPI_SIM_STATUS_SIM_PUK_REQUIRED) by using #tel_get_sim_init_info on boot time
 * or (TAPI_SIM_PIN_STATUS_BLOCKED) by using #tel_get_sim_security_status after normal initialization for specific operation,
 * you may unblock PIN1/PIN2.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - Based on Pin status (PIN1/PIN2) is blocked, an unsolicited event TAPI_EVENT_SIM_STATUS_IND
 * is published to Applications on boot time also.
 *
 * @warning
 * - If you fail to unblock PIN til given retry count, Your SIM card be permanent block status.
 *
 * @param[in] puk_data
 * -Specifies unblocking PIN password #TelSimSecPw_t
 *
 * @param[in] new_pin_data
 * -Specifies PIN password which you want to use after unblocking operation #TelSimSecPw_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_VERIFY_PUK_CNF  and the event data is #TelSimSecResult_t.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 * If returned status is TAPI_SIM_PIN_OPERATION_SUCCESS, Verification successes.
 * If returned status is TAPI_SIM_PUK_INCORRECT_PASSWORD, Verification fails and you can check retry count by using event data  #TelSimSecResult_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - PIN1 or PIN2 status should be TAPI_SIM_PIN_STATUS_BLOCKED and Card status should be TAPI_SIM_STATUS_SIM_PUK_REQUIRED on boot time.
 *
 * @post
 *  - After Success on unblocking for PIN1 or PIN2, each pin check facility is enabled even if previous facility is disabled.
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
 *
 * int request_id = 0;
 * err_code = TAPI_API_SUCCESS; // 0
 * int length = TAPI_SIM_PIN_LENGTH+1;
 *
 * char init_pin_val[length];
 * char init_puk_val[length];
 *
 * strcpy(init_pin_val, PIN1_NORMAL);
 * strcpy(init_puk_val, PUK1_NORMAL);
 *
 * TelSimSecPw_t puk_data = {0,};
 * TelSimSecPw_t new_pin_data = {0,};
 *
 * puk_data.type = TAPI_SIM_PTYPE_PUK1;   // 0x00
 * puk_data.pw_len = length;
 * puk_data.pw_len = strlen(init_puk_val);
 * printf("pw_len[%d]", puk_data.pw_len);
 * memcpy(puk_data.pw,init_pin_val,length);
 *
 * new_pin_data.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
 * new_pin_data.pw_len = strlen(init_pin_val);
 * printf("pw_len[%d]", new_pin_data.pw_len);
 * new_pin_data.pw = (char*)malloc(length);
 * memcpy(new_pin_data.pw,init_pin_val,length);
 *
 * err_code = tel_verify_sim_puks(&puk_data, &new_pin_data, &request_id);
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_verify_sim_puks(const TelSimSecPw_t *puk_data, const TelSimSecPw_t *new_pin_data, int *req_id);


/**
 * @brief  This function changes PIN1/PIN2 code based on the pin type passed along with old pin data and new pin data.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - User should fill up pin type equally both old_pin and new_pin.
 *
 * @warning
 * - None.
 *
 * @param[in] old_pin
 * -Old pin code, entered by the user. #TelSimSecPw_t
 *
 * @param[in] new_pin
 * -New pin code, entered by the user. #TelSimSecPw_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_CHANGE_PINS_CNF  and the event data is #TelSimSecResult_t.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 * If returned status is TAPI_SIM_PIN_OPERATION_SUCCESS, Change operation finish successfully.
 * If returned status is TAPI_SIM_PIN_INCORRECT_PASSWORD, Change operation fails and you can check retry count by using event data  #TelSimSecResult_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - PIN which user want to change check facility should be enabled.
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
 *
 * int request_id = 0;
 * err_code = TAPI_API_SUCCESS; // 0
 * int length = TAPI_SIM_PIN_LENGTH+1;
 * char init_old_pin_val[length] ;
 * char init_new_pin_val[length];
 *
 * memset(&init_old_pin_val, 0 , length);
 * memset(&init_new_pin_val, 0 , length);
 *
 * TelSimSecPw_t old_pin = {0,};
 * TelSimSecPw_t new_pin = {0,};
 *
 * strcpy(init_old_pin_val, PIN1_ORG);
 *
 * old_pin.type = TAPI_SIM_PTYPE_PIN1;
 * old_pin.pw_len = strlen(init_old_pin_val);
 * printf("pw_len[%d]", old_pin.pw_len);
 * old_pin.pw = (char*)malloc(length);
 * memcpy(old_pin.pw,init_old_pin_val,length);
 *
 * strcpy(init_new_pin_val, PIN1_NEW);
 *
 * new_pin.type = TAPI_SIM_PTYPE_PIN1;
 * new_pin.pw_len = strlen(init_new_pin_val);
 * printf("pw_len[%d]", new_pin.pw_len);
 * new_pin.pw = (char*)malloc(length);
 * memcpy(new_pin.pw,init_new_pin_val,length);
 *
 * err_code = tel_change_sim_pins(&old_pin, &new_pin, &request_id);
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_change_sim_pins(const TelSimSecPw_t *old_pin , const TelSimSecPw_t *new_pin, int *req_id);


/**
 * @brief  This function gets the current PIN1/PIN2/SIM LOCK status.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param[in] type
 * - Security type user wants to get status #TelSimPinType_t
 *
 * @param [out] status
 * This parameter returns the current status about user wants. #TelSimPinStatus_t
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimPinType_t type = TAPI_SIM_PTYPE_PIN1; //pin type
 * TelSimPinStatus_t status = -1;
 * err_code = tel_get_sim_security_status(type, &status);
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_security_status(TelSimPinType_t type, TelSimPinStatus_t *status);


/**
 * @brief  This function disables the PIN1/SIM Lock check facility based on the Pin type and pin data passed from Application.
 * PIN1 code : The SIM card has its own password is to check access permissions
 * SIM Lock code : Between Device and the SIM card using a SIM password is only available on certain handsets to be used to authorize.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -For PIN2, disabling applies only for 3G USIMs and when PIN2 in NOT mapped to CHV2 of GSM application.
 * -PIN2 disabling operation is generally not used.
 *
 * @warning
 * - None.
 *
 * @param[in] sec_data
 * -User entered PIN disable data and pin type. #TelSimSecPw_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_DISABLE_SEC_CNF  and the event data is #TelSimSecResult_t.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 * If returned status is TAPI_SIM_PIN_OPERATION_SUCCESS, disabling operation finish successfully.
 * If returned status is TAPI_SIM_PIN_INCORRECT_PASSWORD, disabling operation fails and you can check retry count by using event data  #TelSimSecResult_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - Checking Facility which user wants to disable should be enabled status.
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
 * TelSimSecPw_t sec_data = {0,};
 * sec_data.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
 * sec_data.pw_len = 4; // current pw is 4 digit
 * sec_data.pw = (char*)malloc(length);
 * memcpy(sec_data.pw,"1111",length);
 * err_code = tel_disable_sim_security(&sec_data, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_disable_sim_security(TelSimSecPw_t *sec_data, int *req_id);


/**
 * @brief  This function enables the PIN1/SIM Lock check facility based on the Pin type and pin data passed from Application.
 * PIN1 code : The SIM card has its own password is to check access permissions
 * SIM Lock code : Between Device and the SIM card using a SIM password is only available on certain handsets to be used to authorize.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in] sec_data
 * -User entered PIN enable data and pin type. #TelSimSecPw_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_ENABLE_SEC_CNF  and the event data is #TelSimSecResult_t.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 * If returned status is TAPI_SIM_PIN_OPERATION_SUCCESS, enabling operation finish successfully.
 * If returned status is TAPI_SIM_PIN_INCORRECT_PASSWORD, enabling operation fails and you can check retry count by using event data  #TelSimSecResult_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - Checking Facility which user wants to enable should be disabled status.
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
 * TelSimSecPw_t sec_data = {0,};
 * sec_data.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
 * sec_data.pw_len = 4; // current pw is 4 digit
 * sec_data.pw = (char*)malloc(length);
 * memcpy(sec_data.pw,"1111",length);
 * err_code = tel_enable_sim_security(&sec_data, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_enable_sim_security(TelSimSecPw_t *sec_data, int *req_id);


/**
 *
 * @brief  This API is used to get lock personalisation information like current network lock status or network subset lock and so on.
 * Refer '3GPP TS 22.022 - Personalisation of Mobile Equipment (ME)' for more detail.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - This functionality should be used only Administrator coverage.
 *
 * @warning
 * - None.
 *
 * @param[in] type
 * -personalisation type
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated is TAPI_EVENT_SIM_PERS_STATUS_CNF  and the event data is #TelSimPersStatus_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
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
 * TelSimPersType_t type = 0;
 * type = TAPI_SIM_PERS_NET; //network lock
 * err_code = tel_get_sim_personalization_status(type, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_personalization_status(TelSimPersType_t type, int *req_id);


/**
 * @brief  This API is used to disable personalisation of network, network subset, corporate, service provider personalisation.
 * Refer '3GPP TS 22.022 - Personalisation of Mobile Equipment (ME)' for more detail.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - This functionality should be used only Administrator coverage.
 *
 * @warning
 * - None.
 *
 * @param[in] pers_data
 * -Personalisation information passed. #TelSimPersPw_t
 *
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated is TAPI_EVENT_SIM_DISABLE_PERS_CNF.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - Checking Facility which user wants to disable should be enabled status.
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
 * TelSimPersPw_t lock_pers = {0,};
 * lock_pers.type = TAPI_SIM_PERS_NET; //network lock case
 * lock_pers.pw_len =  8; //control key
 * lock_pers.pw = (char*)malloc(8);
 * memcpy(lock_pers.pw,"11111111",length);    //temp value
 * err_code = tel_disable_sim_personalization(&lock_pers, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_disable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id);


/**
 * @brief  This API is used to enable personalisation of network, network subset, corporate, service provider personalisation.
 * Refer '3GPP TS 22.022 - Personalisation of Mobile Equipment (ME)' for more detail.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - This functionality should be used only Administrator coverage.
 *
 * @warning
 * - None.
 *
 * @param[in] pers_data
 * -Personalisation information passed. #TelSimPersPw_t
 *
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * - The event associated is TAPI_EVENT_SIM_ENABLE_PERS_CNF.
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - Checking Facility which user wants to enable should be disabled status.
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
 * TelSimPersPw_t lock_pers = {0,};
 * lock_pers.type = TAPI_SIM_PERS_NET; //network lock case
 * lock_pers.pw_len =  15; //control key
 * lock_pers.pw = (char*)malloc(15);
 * memcpy(lock_pers.pw,"111111111111111",length);    //temp value
 * err_code = tel_enable_sim_personalization(&lock_pers, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_enable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id);


/**
 * @brief  This function is used to get current SIM Phonebook mode whether FDN or not(ADN).
 * SIM phonebook ADN(or 3G Phonebook) enabled by default and  the FDN can be used if the user selects FDN activation.
 * ADN and FDN also couldn`t activate the same time.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [out] fdn_status
 * This parameter returns the current FDN status whether enabled or disabled.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int fdn_status = 0 ;
 * err_code = tel_get_sim_fdn_status(&fdn_status); // you can find in fdn_status variable
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_fdn_status(int *fdn_status);


/**
 * @brief  This function disables FDN phonebook mode.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *
 * @warning
 * - None.
 *
 * @param[in] pin2
 * -User entered pin2 data.
 *
 * @param[in] pin2_len
 * -User entered a length of pin2 data.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF  and event data associated with this event.#TelSimSecResult_t
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - FDN mode sould be enabled.
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
 * TelSimSecPw_t sec_data = {0,};
 * sec_data.type = TAPI_SIM_PTYPE_PIN2;	// pin2
 * sec_data.pw_len = 4;
 * sec_data.pw = (char*)malloc(4);
 * memcpy(sec_data.pw,"2222",length); //temp value
 * err_code = tel_disable_sim_fdn(sec_data.pw, &sec_data.pw_len, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_disable_sim_fdn(const unsigned char *pin2, int *pin2_len, int *req_id);


/**
 * @brief  This function enables FDN phonebook mode.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 *
 * @warning
 * - None.
 *
 * @param[in] pin2
 * -User entered pin2 data.
 *
 * @param[in] pin2_len
 * -User entered a length of pin2 data.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF  and event data associated with this event.#TelSimSecResult_t
 * Asynchronous return status is indicated by #TelSimPinOperationResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - FDN mode sould be disabled.
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
 * TelSimSecPw_t sec_data = {0,};
 * sec_data.type = TAPI_SIM_PTYPE_PIN2;	// pin2
 * sec_data.pw_len = 4;
 * sec_data.pw = (char*)malloc(4);
 * memcpy(sec_data.pw,"2222",length); //temp value
 * err_code = tel_enable_sim_fdn(sec_data.pw, &sec_data.pw_len, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_enable_sim_fdn(const unsigned char *pin2, int *pin2_len, int *req_id);


/**
 * @brief  This function provides common interface for accessing SIM data based on USIM generic command in ETSI TS 102 221
 * and 3GPP TS 31.102 Characteristics of the Universal Subscriber Identity Module (USIM) application.
 * Required to operate the terminal by default, the application (call, sms, setting, network and so on) are using the SIM data
 * stored in the Telephony server to the user data, and easy to use their API to provide a separate,
 * but SIM and the user's data is enormous, with some data, the usage of any use to manage all the telephony server is difficult.
 * Therefore, handling the SIM data can provide a common one API.
 * Only this API be specified in the ETSI TS 102 221 of the generic command 'SELECT', 'READ BINARY', 'READ RECORD',
 * 'UPDATE BINARY', 'UPDATE RECORD' command support.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - If you want more generic access, please use #tel_req_sim_apdu
 *
 * @warning
 * - None.
 *
 * @param[in] rsim_data
 * -Different parameters should be filled up according to command type,
 * If user wants to run 'SELECT' command, Only file_id needed. And you can find file type with response data by asynchronous return.
 * If user wants to run 'READ BINARY' command, should fill up parameter like this(p1:offset, p2:offset low, p3:bytes to be read)
 * If user wants to run 'READ RECORD' command, should fill up parameter like this(p1:record num, p2:mode, p3:bytes to be read)
 * If user wants to run 'UPDATE BINARY' command, should fill up parameter like this(p1:offset, p2:offset low, p3:bytes to be updated, data:address of data which will be updated)
 * If user wants to run 'UPDATE RECORD' command, should fill up parameter like this(p1:record num, p2:mode, p3:bytes to be updated, data:address of data which will be updated)
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_RSIM_ACCESS_CNF  and the event data is #TelSimReadFileRaw_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int err_code =0;
 * int request_id = 0;
 * TelSimRSimReq_t rsim = {0,};
 * rsim.file_id = 0x6F60;
 * rsim.rsim_cmd = TAPI_SIM_GET_RESPONSE;
 *
 * // REQUEST SIM ACCESS
 * err_code = tel_req_sim_access((const TelSimRSimReq_t*)&rsim, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sim_access(const TelSimRSimReq_t *rsim_data, int *req_id);


/**
 * @brief  This function provides common interface for accessing SIM data based on USIM generic command in ETSI TS 102 221
 * and 3GPP TS 31.102 Characteristics of the Universal Subscriber Identity Module (USIM) application.
 * This API provides only interface to send APDU. User should know well APDU usage.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
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
 * @param[in] rsim_data
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_APDU_CNF  and the event data is #TelSimApduResp_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int err_code =0;
 * int request_id = 0;
 * TelSimApdu_t apdu = {0,};
 * int tmp_apdu[5] = {0xa,5,6,7,8};
 * apdu.apdu_len = 5; //user should check
 * apdu.apdu = apdu;
 *
 * // REQUEST SIM ACCESS
 * err_code = tel_req_sim_apdu((const TelSimRSimReq_t*)&rsim, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sim_apdu(TelSimApdu_t* apdu_data, int *req_id);


/**
 * @brief  This function provides common interface to get SIM ATR(Answer To Reset) value.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
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
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_ATR_CNF  and the event data is #TelSimAtrResp_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int err_code =0;
 * int request_id = 0;
 *
 * // REQUEST SIM ATR
 * err_code = tel_req_sim_atr(&request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sim_atr(int *req_id);


/**
 * @brief  This function is a synchronous API used to get ECC(SIM) or UECC(USIM) data.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [out] ecc_data
 * This parameter returns the SIM emergency call code information like ecc length, service type etc.
 *
 * @param [out] ecc_count
 * Specifies number of ECC records.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int ecc_rec_count = 0; // count in sim card
 * int err_code = 0;
 * TelSimEccData_t ecc_info = {{{0,}}};
 * err_code = tel_get_sim_ecc(&ecc_info, &ecc_rec_count);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_ecc(TelSimEccData_t *ecc_data, int *ecc_count);


/**
 * @brief  This API is used to get the language preference(indication) information.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [out] sim_language
 * This parameter returns the language preference information.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimLanguageInfo_t li_info = {0,};
 * err_code = tel_get_sim_language(&li_info); // you can find language info with li_info variable
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_language(TelSimLanguageInfo_t *sim_language);


/**
 * @brief  This API is used to update the language preference information to SIM card.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous function.
 *
 * @par Important Notes:
 * - This function update SIM Language information directly and can effect device language setting. Use carefully.
 *
 * @warning
 * - None.
 *
 * @param[in] language
 * This parameter returns the language preference information.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_SIM_SET_LANGUAGE_CNF.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int request_id = 0;
 * TelSimLanguagePreferenceCode_t language = 0x00; //init
 * language = TAPI_SIM_LP_ENGLISH; //language value
 * err_code = tel_set_sim_language(language, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_set_sim_language(TelSimLanguagePreferenceCode_t language, int *req_id);


/**
 * @brief  This function is a synchronous API which gets the Card Type whether Card is a SIM or a USIM.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [out] card_type
 * This parameter returns the SIM card type information like whether it is GSM SIM card or USIM or it is of unknown type.
 *
 *  - TAPI_SIM_CARD_TYPE_UNKNOWN
 *  - TAPI_SIM_CARD_TYPE_GSM
 *  - TAPI_SIM_CARD_TYPE_USIM
 *
 * @par Async Response Message:
 * - None
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
 * TelSimCardType_t cardInfo; //unkwon, sim(normally gsm), usim(normally umts)
 * err_code = tel_get_sim_type(&cardInfo);
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_type(TelSimCardType_t *card_type);


/**
 * @brief  This function is a synchronous API gets the IMSI information.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - IMSI value is the value that security needs.
 *
 * @warning
 * - None.
 *
 * @param [out] imsi
 * This parameter returns the IMSI information. Refer #TelSimImsiInfo_t
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimImsiInfo_t sim_imsi_info; // mcc, mnc, msisdn
 * err_code = tel_get_sim_imsi(&sim_imsi_info);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_imsi(TelSimImsiInfo_t* imsi);


/**
 * @brief  This function is a synchronous API gets a Unique identification number of the (U)ICC.
 * ICC means Integrated Circuit Card.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - IC Card number value is the value that security needs.
 *
 * @warning
 * - None.
 *
 * @param [out] iccid
 * This parameter returns the IC Card number. Refer #TelSimIccIdInfo_t
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimIccIdInfo_t icc_data; // this struct consists of number length and number.
 * err_code = tel_get_sim_iccid(&icc_data);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_iccid(TelSimIccIdInfo_t* iccid);


/**
 * @brief  This API is used to get the SIM mailbox related data(EF-MBDN, MBDI and CPHS case).
 * this data is required for Mailbox related functionality in handset.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param [out] mbox
 *   - mailbox number information in current inserted SIM card. Refer #TelSimMailboxNumbers_s
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimMailboxNumbers_s mbox;
 * TapiResult_t err_code = 0;
 * err_code = tel_get_sim_mailbox_info(&mbox);
 * if(err_code == TAPI_API_SUCCESS) {
 *   if(mbox.b_cphs) { //b_cphs means current SIM is for CPHS or not.
 *     printf("Current SIM is CPHS");
 *   } else {
 *     printf("Current SIM is not CPHS but 3GPP");
 *   }
 * } else {
 *   printf("TAPI API FAIL: Error Code [0x%x]",err_code);
 * }
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_mailbox_info(TelSimMailboxNumbers_s *mbox);

/**
 * @brief  This API is used to get the SIM Call forwarding indication related data(EF-CFIS and CPHS case).
 * this data is required for Call forwarding related functionality in handset.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None
 *
 * @param [out] cf
 *  - Callforwarding number information in current inserted SIM card. Refer #TelSimCallForwarding_s
 *  - User should check 'b_cphs' value first before refer union data struct.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - None
 *
 * @post
 *  - None
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps
 *
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * TelSimCallForwarding_s cf_data;
 * TapiResult_t err_code = 0;
 * err_code = tel_get_sim_callforwarding_info(&cf_data);
 * if(err_code == TAPI_API_SUCCESS) {
 *   if(cf_data.b_cphs) { //b_cphs means current SIM is for CPHS or not.
 *     printf("Current SIM is CPHS");
 *   } else {
 *     printf("Current SIM is not CPHS but 3GPP");
 *   }
 * } else {
 *   printf("TAPI API FAIL: Error Code [0x%x]",err_code);
 * }
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_callforwarding_info(TelSimCallForwarding_s *cf);

/**
 * @brief  This API is used to get the SIM message waiting indication related data(EF-MWIS and CPHS case).
 * this data is required for Message waiting related functionality in handset.
 *
 * @par Sync (or) Async:
 * This is a synchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None
 *
 * @param [out] mw
 *  - Message waiting count and type information in current inserted SIM card. Refer #TelSimMessageWaiting_s
 *  - User should check 'b_cphs' value first before refer union data struct.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *  - None
 *
 * @post
 *  - None
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps
 *
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * TelSimMessageWaiting_s mw_data;
 * TapiResult_t err_code = 0;
 * err_code = tel_get_sim_messagewaiting_info(&mw_data);
 * if(err_code == TAPI_API_SUCCESS) {
 *   if(mw_data.b_cphs) { //b_cphs means current SIM is for CPHS or not.
 *     printf("Current SIM is CPHS");
 *   } else {
 *     printf("Current SIM is not CPHS but 3GPP");
 *   }
 * } else {
 *   printf("TAPI API FAIL: Error Code [0x%x]",err_code);
 * }
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_messagewaiting_info(TelSimMessageWaiting_s *mw);

/**
 * @brief  This API is used to get the SIM CPHS specific data.
 * this data is required for CPHS related functionality in handset.
 *
 * @par Sync (or) Async:
 * This is an Synchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None
 *
 * @param [out] cphs
 *   - CPHS specific data(CPHS info table, service number, operator name, etc). Refer #TelSimCphsLocalInfo_t
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *
 * @post
 *  - None
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps
 *
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * TelSimCphsLocalInfo_t cphs_data;
 * TapiResult_t err_code = 0;
 * err_code = tel_get_sim_cphs_info(&cphs_data);
 * if(err_code == TAPI_API_SUCCESS) {
 *   if(cphs_data.b_used) { //b_used means current SIM is for CPHS or not.
 *     printf("Current SIM is CPHS");
 *   } else {
 *     printf("Current SIM is not CPHS but 3GPP");
 *   }
 * } else {
 *   printf("TAPI API FAIL: Error Code [0x%x]",err_code);
 * }
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_cphs_info(TelSimCphsLocalInfo_t *cphs);


/**
 * @brief  This API is used to get the SIM MSISDN data. Regarding 3GPP specification, Current value is optional.
 *
 * @par Sync (or) Async:
 * This is an Synchronous API.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None
 *
 * @param [out] subscriber
 *   - subscriber information. if current SIM card does not have, null string will be returned. Refer #TelSimSubscriberInfo_t
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
 *
 * @post
 *  - None
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps
 *
 * @see None
 *
 * @code
 * #include <ITapiSim.h>
 * TelSimSubscriberInfo_t msisdn;
 * TapiResult_t err_code = 0;
 * err_code = tel_get_sim_msisdn(&msisdn);
 * if(err_code == TAPI_API_SUCCESS)
 * {
 *	 printf(" ============================================");
 *	 printf("name[%s]",msisdn.name);
 *	 printf("num[%s]",msisdn.num);
 *	 if(msisdn.name == '\0';) //If current SIM does not have, Null string will be returned.
 *		 printf("name is null string");
 *
 *	 if(msisdn.num == '\0';) //If current SIM does not have, Null string will be returned.
 *		 printf("number is null string");
 * }
 * else
 * {
 *	 printf("TAPI API FAIL: Error Code [0x%x]",err_code);
 * }
 *
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_msisdn(TelSimSubscriberInfo_t *subscriber);

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
 * @param [out] init_completed
 *   - phonebook init status. '0' is not init, '1' is init complete to use.
 *
 * @param [out] pb_list
 *   - available SIM phonebook list. This value is valid in phonebook init complete case.
 *
 * @param [out] first_index
 *   - first valid index to use.  only ADN, 3G phonebook and FDN case. This value is valid in phonebook init complete case.
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
int tel_get_sim_pb_init_info(int *init_completed, TelSimPbList_t *pb_list, int *first_index);


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
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbFileType_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
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
 * TelSimPbFileType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_get_sim_pb_count(storage_type, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_pb_count(TelSimPbFileType_t pb_type, int* req_id);


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
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbFileType_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
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
 * TelSimPbFileType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_get_sim_pb_meta_info(storage_type, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_get_sim_pb_meta_info(TelSimPbFileType_t pb_type, int* req_id);


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
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
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
int tel_get_sim_pb_3g_meta_info(int *req_id);


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
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. ##TelSimPbFileType_t
 *
 * @param[in] index
 * -Index for accessing the SIM data.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
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
 * TelSimPbFileType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_read_sim_pb_record(storage_type, index, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_read_sim_pb_record(TelSimPbFileType_t pb_type, unsigned short index, int *req_id );


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
 * @param[in] req_data
 * -phonebook data which will be updated or added. #TelSimPbRecordData_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
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
int tel_update_sim_pb_record(const TelSimPbRecordData_t *req_data, int *req_id );


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
 * @param[in] pb_type
 * -Different storage types to be selected in SIM. #TelSimPbFileType_t
 *
 * @param[in] index
 * -Index of the record to be deleted.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
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
 * TelSimPbFileType_t storage_type = 0x00;
 * storage_type = TAPI_SIM_PB_3GSIM; // usim phonebook
 * err_code = tel_delete_sim_pb_record(storage_type, index, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_delete_sim_pb_record(TelSimPbFileType_t pb_type, unsigned short index, int *req_id );


/**
 * @brief  This function is used to make a connection request to SIM
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param[in] req_data
 * - This input has the SAP message request to connect. #TelSimSapConnect_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_CONNECT_CNF  and the event data is #TelSimSapConnect_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimSapConnect_t con_req = {0,};
 * con_req.MsgId = TAPI_SIM_SAP_CONNECT_REQ;	// connection req id
 * con_req.MaxMsgSize = 10; //temp for testing only - not clear
 * err_code = tel_req_sap_connection(&con_req, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_connection( TelSimSapConnect_t *req_data, int *req_id);


/**
 * @brief  This function is used to get connection status information
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_CONNECT_STATUS_CNF  and the event data is #TelSimSapConnectionStatus_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * err_code = tel_req_sap_connection_status(&request_id);	//you can find status value by receiving async event value
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_connection_status( int *req_id);


/**
 * @brief  This function is used to get ATR request from SIM
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_TRANSFER_ATR_CNF  and the event data is #TelSimSapAtrInfo_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * err_code = tel_req_sap_transfer_atr(&request_id);	//you can find transfer atr request resut with async event
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_transfer_atr( int *req_id);


/**
 * @brief  This function is used to transfer APDU to SIM
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param[in] apdu_data
 * - This input has the APDU data to be passed to SIM card. #TelSimSapApduData_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_TRANSFER_APDU_CNF  and the event data is #TelSimSapApduData_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * int file_id = TAPI_SIM_EFILE_ICCID;
 * TelSimSapApduData_t apdu_data;	// for selecting EF ICCID
 * apdu_data.ApduLength = 7;
 * apdu_data.Apdu[0] = 0xA0; // class
 * apdu_data.Apdu[1] = 0xA4; // instruction
 * apdu_data.Apdu[2] = 0; // p1 = parameter 1
 * apdu_data.Apdu[3] = 0; // p2 = parameter 2
 * apdu_data.Apdu[4] = 2; // p3 - parameter 3
 * memcpy(&apdu_data.Apdu[5], &file_id, sizeof(unsigned short));
 * err_code = tel_req_sap_transfer_apdu(&apdu_data, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_transfer_apdu(TelSimSapApduData_t *apdu_data, int *req_id);


/**
 * @brief	This function is used to select either of the protocols.
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param[in] protocol
 * - This input is the protocol to set in SIM for SAP. #TelSimSapProtocol_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_SET_PROTOCOL_CNF  and the event data is #TelSimSapResultCode_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimSapProtocol_t protocol = TAPI_SIM_SAP_PROTOCOL_T0; // protocol set
 * err_code = tel_req_sap_transport_protocol(protocol, &request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_transport_protocol(TelSimSapProtocol_t protocol, int *req_id);


/**
 * @brief  This function is used to set the power ON/OFF/RESET the SIM card
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param[in] req_data
 * - This input is to power on or off the SIM card.#TelSimSapMsgId_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_SET_SIM_POWER_CNF  and the event data is #TelSimSapPower_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * TelSimSapMsgId_t	msdId = 0;
 * msdId = TAPI_SIM_SAP_POWER_SIM_ON_REQ; //power on operation
 * err_code = tel_req_sap_power_operation(msdId, &request_id);// only for testing
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_power_operation(TelSimSapMsgId_t req_data, int *req_id );


/**
 * @brief  This function is used to transfer card reader status
 *	Access to this API is limited to in-house application and we recommend you use bluetooth-engine APIs.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - We recommend user does not use SAP related APIs directly. Our SAP APIs are provided as interface between BT-engine and Modem Plugin.
 * So without BT-engine, you may not get proper operation with these SAP APIs.
 *
 * @warning
 * - None.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_SAP_CARD_READER_STATUS_CNF  and the event data is #TelSimCardReaderStatus_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *  - User can get valid return value or make operations after SIM init complete(card status is TAPI_SIM_STATUS_SIM_INIT_COMPLETED).
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
 * err_code = tel_req_sap_cardreader_status(&request_id);	//you can find card reader status with async event
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sap_cardreader_status(int *req_id);


/**
 * @brief  This API is used to execute the authentication procedure by using SIM.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 * This function supports IMS, 3G and GSM authentication.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param[in] authentication_data
 * - This input has the authentication code to be validated by ISIM,3G and GSM application in the SIM card. #TelSimAuthenticationData_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_SIM_EVENT_AUTHENTICATION_CNF  and the event data is #TelSimAuthenticationResponse_t.
 * Asynchronous return status is indicated by #TelSimAccessResult_t.
 *
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
 * TelSimAuthenticationData_t pAuthenticationData;
 * strcpy((char*)pAuthenticationData.rand_data, "username@operator.com"); //access data
 * pAuthenticationData.rand_length= strlen("username@operator.com");
 * strcpy((char*)pAuthenticationData.autn_data,  "+1-212-555-12345"); // auth data
 * pAuthenticationData.autn_length = strlen("+1-212-555-12345");
 * err_code = tel_req_sim_authentication(&pAuthenticationData,&request_id);
 * @endcode
 *
 * @remarks None
 *
 */
/*================================================================================================*/
int tel_req_sim_authentication( TelSimAuthenticationData_t *authentication_data, int *req_id);


#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_SIM_H_ */

/**
* @}
*/
