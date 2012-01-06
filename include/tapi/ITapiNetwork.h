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
 * @open
 * @ingroup		TelephonyAPI
 * @addtogroup	Network_TAPI	NETWORK
 * @{
 *
 * @file ITapiNetwork.h

 @brief This file serves as a "C" header file defines functions for Tapi Network\n
 It contains a sample set of function prototypes that would be required by applications.
 */

#ifndef _ITAPI_NETWORK_H_
#define _ITAPI_NETWORK_H_
/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/
#include <TelErr.h>
#include <TelDefines.h>
#include <TelNetwork.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C"
{
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

/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

/**
 * @brief  This function requests the lower layers to select the network automatically
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Important Notes:
 * - This function passes the
 *
 * @warning
 * - None.
 *
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SELECT_CNF and there is no data associated with this event. Asynchronous return status
 * is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestID;
 *
 * // AUTOMATIC MODE
 * ret_status = tel_select_network_automatic(&RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_select_network_automatic(int *pRequestId);

/**
 * @brief  This function requests the lower layers to select the network (PLMN) which has been selected by the user from the Network List
 *           displayed to the User.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Important Notes:
 * - This function passes the .
 *
 * @warning
 * - None.
 *
 *
 * @param[in] Plmn Identification
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SELECT_CNF and there is no data associated with this event. Asynchronous return status
 * is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestID;
 * unsigned int plmn = 0;
 *
 * // MANUAL MODE
 * plmn = 45454;
 * ret_status = tel_select_network_manual(Plmn, &RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_select_network_manual(unsigned int Plmn, int *pRequestId);

/**
 * @brief  This function sends a request to do manual network selection to search for the available networks and provide the
 *           Network List to the User/Application.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SEARCH_CNF and the event data is #TelNetworkPlmnList_t.
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - In the available network list, user can select one of the networks successfully.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestID;
 *
 * // NETWORK SEARCH
 * ret_status = tel_search_network(&RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_search_network(int *pRequestId);

/**
 * @brief  This function requests for the present network selection mode i.e. automatic or manual.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF and the event data is #TelNetworkSelectionMode_t.
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * // GET THE SELECTION MODE
 * ret_status = tel_get_network_selection_mode (&RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_selection_mode(int *pRequestId);

/**
 * @brief  This function is called when User/application wants to configure the service domain to only CS or Only PS or Both.
 * This API triggers the underlying protocol stack to do register with Network for only CS services or only PS services
 * or both based on the option set using this API.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[in] ServiceDomain
 *    -  Specifies the type of Service domain (Packet switch, circuit switch,etc)
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SET_SVC_DOMAIN_CNF and there is no event data associated with this event
 * and asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * // SET NETWORK SERVICE DOMAIN
 * TelNetworkServiceDomain_t ServiceDomain = TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC;
 *
 * ret_status = tel_set_network_service_domain (ServiceDomain, &RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_set_network_service_domain(TelNetworkServiceDomain_t ServiceDomain,
		int *pRequestId);

/**
 * @brief  This function requests for the present network service domain i.e. CS or PS or both or automatic.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GET_SVC_DOMAIN_CNF and the event data is #TelNetworkServiceDomain_t.
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * // GET NETWORK SERVICE DOMAIN
 * ret_status = tel_get_network_service_domain (&RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_service_domain(int *pRequestId);

/**
 * @brief  This function allows to configure the Network Mode (Automatic, GSM, WCDMA) at OEM provider based on which lower
 * layers can scan only those radio access technologies for registering with network.
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
 *
 * @param[in] NwMode
 *    - Specifies the network mode i.e. Automatic, WCDMA, GSM, CDMA and WLAN.
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SETNWMODE_CNF and there is no event data associated with this event and asynchronous
 * return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 * TelNetworkMode_t NwMode = TAPI_NETWORK_MODE_AUTOMATIC;
 *
 * // SET NETWORK MODE
 * ret_status = tel_set_network_mode(NwMode, &RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_set_network_mode(TelNetworkMode_t NwMode, int *pRequestId);

/**
 * @brief  This function requests for the present network mode.
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
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GETNWMODE_CNF and the event data is #TelNetworkMode_t.
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * ret_status = tel_get_network_mode(&RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_mode(int *pRequestId);

/**
 * @brief  This function is invoked to set the network band and allows the underlying OEM provider to scan the set band.
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
 *
 * @param[in]  BandMode
 *    -  Band preference indicates the band provide to be preferred or select the band .
 * @param[in]  Band
 *    -  This enumeration defines different network Bands that user can choose.
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SETNWBAND_CNF and there is no event data associated with this event and asynchronous
 * return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * TelNetworkBandPreferred_t BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
 * TelNetworkBand_t Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
 *
 * // SET NETWORK BAND
 * ret_status = tel_set_network_band (BandMode, Band, &RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_set_network_band(TelNetworkBandPreferred_t BandMode,
		TelNetworkBand_t Band, int *pRequestId);

/**
 * @brief  This function requests for the present network band.
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
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GETNWBAND_CNF and the event data is #TelNetworkBand_t.
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * // GET NETWORK BAND
 * ret_status = tel_get_network_band(&RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_band(int *pRequestId);

/**
 * @brief  This function is invoked to set the network preferred plmn
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
 *
 * @param[in]  Operation
 *    -  Operation indicates the operation to be done on preferred plmn .
 * @param[in]  pPreffPlmnInfo
 *    -  This gives the plmn Info.
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SETPREFFPLMN_CNF and there is no event data associated with this event and asynchronous
 * return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * TelNetworkPrefferedPlmnOp_t Operation = TAPI_NETWORK_PREF_PLMN_ADD;
 * TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
 *
 * memset(&PreffPlmnInfo, 0, sizeof(TelNetworkPrefferedPlmnInfo_t));
 * PreffPlmnInfo.Index = 0;
 * PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
 * unsigned char *plmn = "45454";
 * memcpy(PreffPlmnInfo.Plmn, plmn, strlen(plmn));
 *
 * // SET PREFERRED PLMN
 * ret_status = tel_set_network_preferred_plmn(Operation, PreffPlmnInfo, &RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_set_network_preferred_plmn(TelNetworkPrefferedPlmnOp_t Operation,
		TelNetworkPrefferedPlmnInfo_t *pPreffPlmnInfo, int *pRequestId);

/**
 * @brief  This function is invoked to get the preferred plmn list
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
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GETPREFFPLMN_CNF and event data associated with this event  is #TelNetworkPrefferedPlmnList_t
 * and asynchronous  return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int RequestId;
 *
 * // GET NETWORK PREFERRED PLMN
 * ret_status = tel_get_network_preferred_plmn(&RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_preferred_plmn(int *pRequestId);

/**
 * @brief  This function is invoked to set the CDMA Roaming Mode
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
 *
 * @param[in]  RoamingMode
 *  -  CDMA Roaming Mode.
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF
 * and asynchronous  return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * TelNetworkRoamingMode_t roamingMode;
 * int ret_status;
 * int requestId;
 *
 * roamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_HOME_ONLY;
 * roamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_ON;
 *
 * // SET NETWORK ROAMING
 * ret_status = tel_set_network_roaming(&roamingMode, &requestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_set_network_roaming(TelNetworkRoamingMode_t *RoamingMode,
		int *pRequestId);

/**
 * @brief  This function is invoked to get the CDMA Roaming Mode
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
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF and event data associated with this event  is #TelNetworkRoamingMode_t
 * and asynchronous  return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int requestId;
 *
 * // GET NETWORK ROAMING MODE
 * ret_status = tel_get_network_roaming(&requestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_roaming(int *pRequestId);

/**
 * @brief  This function is invoked to set the CDMA Hybrid Mode
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
 *
 * @param[in]  CDMAHybridMode
 *  -  CDMA Hybrid Mode.
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF
 * and asynchronous  return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * TelNetworkCDMAHybridMode_t cdmaHybridMode;
 * int ret_status;
 * int requestId;
 *
 * cdmaHybridMode = TAPI_NETWORK_CDMA_HYBRID;
 *
 * // GET NETWORK HYBRID IN CDMA
 * ret_status = tel_set_network_hybrid_in_cdma(cdmaHybridMode, &requestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_set_network_hybrid_in_cdma(TelNetworkCDMAHybridMode_t CDMAHybridMode,
		int *pRequestId);

/**
 * @brief  This function is invoked to get the CDMA Hybrid Mode
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
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF and event data associated with this event  is #TelNetworkCDMAHybridMode_t
 * and asynchronous  return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init or #tel_init_at_ecore_loop
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event or #tel_register_event_at_ecore_loop
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetwork.h>
 *
 * int ret_status;
 * int requestId;
 *
 * // GET NETWORK HYBRID IN CDMA
 * ret_status = tel_get_network_hybrid_in_cdma(&requestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 *
 */
/*================================================================================================*/
int tel_get_network_hybrid_in_cdma(int *pRequestId);

/**
 *
 * @brief  This API allows lower layers to scan based on preferred network order based options passed.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - If network order list array holds the value "TAPI_NETWORK_MODE_AUTOMATIC" then other following values in the list shall be ignored.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pPrefNwOrder
 *    -  This parameter is an array of size TAPI_NW_ORDER_MAX(which is defined as 5) of #TelNetworkMode_t type specifies the preferred
 *        network order for camping.
 * @param [out]  pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SETNWORDER_CNF and there is no event data associated with this event and asynchronous
 * return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_network_acquisition_order(TelNetworkMode_t *pPrefNwOrder,
		int *pRequestId);

/**
 *
 * @brief  This function requests for the present network selection order.
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
 *
 * @param pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GETNWORDER_CNF and the event data is array of size
 * TAPI_NW_ORDER_MAX(which is defined as 5) of #TelNetworkMode_t.
 * Asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_network_acquisition_order(int *pRequestId);

/**
 *
 * @brief  This function is invoked to set the power on attach.
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
 *
 * @param  [in]  PowerOnAttach
 *    -  Power on attach preference to set .
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_SETPOWERONATTACH_CNF and there is no event data associated with this event and asynchronous
 * return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_network_power_on_attach(TelNetworkPowerOnAttach_t PowerOnAttach,
		int *pRequestId);

/**
 *
 * @brief  This function is invoked to get the network power on attach status.
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
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_GETPOWERONATTACH_CNF and event data associated with this event is #TelNetworkPowerOnAttach_t
 * asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_network_power_on_attach(int *pRequestId);

/**
 * @brief  This function is called to cancel the triggered manual network search.
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
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_CANCELMANUALSEARCH_CNF and there is no event data associated with this event
 * and asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - Manual network search is already triggered.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * External Apps.
 *
 */
int tel_cancel_network_manual_search(int *pRequestId);

/**
 *
 * @brief  This function is called to cancel manual network selection where user has already selected a network from the network list.
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
 *
 * @param[out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  Request Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETWORK_CANCELMANUALSELECTION_CNF and there is no event data associated with this event
 * and asynchronous return status is indicated by #TelNetworkOperationCause_t.
 *
 * @pre
 *  - User should have selected a network from available network list.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_cancel_network_manual_selection(int *pRequestId);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_NETWORK_H_ */

/**
 * @}
 */
