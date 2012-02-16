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
* @addtogroup	SS_TAPI		SS(Supplementary services)
* @{
*
* @file ITapiSs.h

     @brief This file serves as a "C" header file defines functions for Tapi Supplementary services\n
      It contains a sample set of function prototypes that would be required by applications.
 */

#ifndef _ITAPI_SS_H_
#define _ITAPI_SS_H_

 /*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include <TelSs.h>
#include <TelUtility.h>
#include <TelDefines.h>

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
                                     FUNCTION PROTOTYPES
==================================================================================================*/

 /**
 * @brief  This function sends a request to activate/deactivate call barring.
 *
 *  This service makes it possible for a mobile subscriber
 *  to activate barring of certain categories of outgoing or incoming calls according to a barring program which is selected from a set
 *  of one or more barring programs chosen at provision time and is valid for all outgoing or incoming calls, or just those associated
 *   with a specific basic service group. Also this API allows deactivating call barring based on categories set.
 * If the user call this function with wrong password 3 times subsequently, the barring service can be blocked.
 * If this function is called normally, the asynchronous response comes in a few second from network. The response indicates
 * the service request has been granted or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
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
 * @param [in] pBarringInfo
 *   - Contains the information about call barring
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  An asynchronous notification event is returned as a confirmation
 *  The event associated is TAPI_EVENT_SS_BARRING_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data is #TelSsBarringStatusInfo_t. \n
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * \see tel_get_ss_barring_status, tel_change_ss_barring_password
 *
 * @code
 * #include<ITapiSS.h>
 *
 * int ret_status = -1;
 * TelSsCallBarringInfo_t  pBarringInfo;// = {0,};
 * int pRequestId=0;
 * char bar_pwd[5]="0000";
 *
 * pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
 * pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
 * pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
 * strcpy(pBarringInfo.szPassword,bar_pwd);
 * ret_status = tel_set_ss_barring (&pBarringInfo,&pRequestId);
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
 /*================================================================================================*/
int   tel_set_ss_barring(const  TelSsCallBarringInfo_t *pBarringInfo,int * req_id);

/**
 *
 * @brief  This function gets call barring status.
 *
 * The mobile subscriber may determine by subscription of a set of one or more unique barring
 * programs what kind of outgoing or incoming calls should be barred.
 * If this function is called normally, the asynchronous response comes in a few second from network.
 * The response indicates the service status.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
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
 * @param [in] BarType
 *   - Specifies barring type i.e. Outgoing international, outgoing all, Incoming all etc.
 *
 * @param [in] CallType
 *   - Specifies the type of call i.e. Voice call, Video Call etc.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data is #TelSsBarringStatusInfo_t.
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
 * @return Return Type (#TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * \see tel_set_ss_barring, tel_change_ss_barring_password
 *
 * @code
 * #include<ITapiSS.h>
 *
 * int ret_status;
 * TelSsCallBarType_t BarType =TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL;
 * TelSsCallType_t CallType = TAPI_CALL_TYPE_VOICE_EV;
 * int pRequestId=0;
 *
 * ret_status = tel_get_ss_barring_status(BarType,CallType,&pRequestId); // get barring status
 * printf ("The return value is %d\n", ret_status);
 *
 * @endcode
 *
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_get_ss_barring_status(TelSsCallBarType_t  BarType, TelSsCallType_t  CallType,int * req_id);

 /**
 *
 * @brief  This function allows changing of the barring password in Network.
 *
 * User can request network to change his own barring password. If the old password is not same with
 * what network knows, it will be rejected by network. Also new password should be same with another new password
 * which is to confirm user input. Otherwise the request must be rejected by Telephony Sever.
 * If the user call this function with wrong old password 3 times subsequently, the barring service can be blocked.
 * If this function is called normally, the asynchronous response comes in a few second from network.
 * The response indicates the service request has been granted or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - Based on 3GPP TS 22.030 section 6.5.4 Registration of New Password. Only one Change Barring Password request is allowed
 *   at a time and TS might return TAPI_API_REQUEST_MAX_IN_PROGRESS if another request is received.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pOldPassword
 *   - Old password set for Barring in Network. This is Null terminated string
 *
 * @param [in] pNewPassword
 *   - New password for Barring to be set in Network. This is Null terminated string.
 *
 * @param [in] pNewPasswordAgain
 *   - New password again. This is Null terminated string.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  There is no event data with this event.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * @see
 * - None
 *
 * @code
 * #include<ITapiSS.h>
 *
 * int ret_status;
 * int pRequestId=0;
 *
 * ret_status = tel_change_ss_barring_password ("0000","0000","0000",&pRequestId); // change barring password
 *
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_change_ss_barring_password(const char*  pOldPassword, const char*  pNewPassword, const char * pNewPasswordAgain,int * req_id);

 /**
 *
 * @brief  This function allows to set (register/erase/activate/deactivate) call forwarding option at the Network.
 *
 * This enables to forward the calls to another party. This service permits a called mobile subscriber
 * to have the network send all incoming calls, or just those associated with a specific Basic service group,
 * addressed to the called mobile subscriber's directory number and which meet
 * various call FORWARD condition to another directory number.
 * The ability of the served mobile subscriber to originate calls is unaffected.
 * If the service is activated, a call is forwarded only if under this call forward condition.
 * Application has to call this API in order to explicitly register/erase/activate/deactivate
 * the call forwarding. Request will be passed irrespective of whether already
 * another SS call forward is ongoing.
 *
 * If this function is called normally, the asynchronous response comes in a few second from network. The response indicates
 * the service request has been granted or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
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
 * @param [in] pForwardInfo
 *   - Contains the Call forward information like forward mode, forward type, etc.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_FORWARD_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data will be #TelSsForwardingStatusInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 *
 * \image html CallForward.jpg
 * \image latex CallForward.jpg
 *
 * \see tel_get_ss_forward_status
 *
 * @code
 * #include<ITapiSS.h>
 *
 * int ret_status =0;
 * int pRequestId=0;
 * TelSsForwardInfo_t pForwardInfo;
 * pForwardInfo.Mode =TAPI_CALL_FORWARD_MODE_ENABLE_EV;
 * pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
 * pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
 * pForwardInfo.NoReplyConditionTimer = 10;
 * int len;
 * char cf_number[20]= {"9986529874"};//number to which calles need to be forwarded.
 *
 * len = strlen(cf_number);
 * printf("Length of CF number :%d \n",len);
 * memcpy(&(pForwardInfo.szPhoneNumber),cf_number,len);
 *
 * ret_status = tel_set_ss_forward (&pForwardInfo,&pRequestId);
 *
 * @endcode
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_set_ss_forward( const  TelSsForwardInfo_t *pForwardInfo,int * req_id);

 /**
 *
 * @brief  This function provides option to get the call forwarding status of different calls from Network.
 *
 * The data request procedure enables the mobile subscriber to obtain information about the data stored in the PLMN.
 * After having requested this procedure the network shall return the following information:
 * -	in response to a general data request the served mobile subscriber should be given a list of all Basic service groups
 *	to which call forwarding unconditional is registered, including information whether or not it is active and operative,
 *	and the associated forwarded-to numbers
 * -	in response to a specific request concerning one particular Basic service group,
 *	the served mobile subscriber should be informed whether or not call forwarding unconditional is registered,
 *	including information whether or not it is active and operative for that Basic service group.
 *	If CFU is registered, the associated forwarded-to number shall be given.
 *
 * If this function is called normally, the asynchronous response comes in a few second from network.
 * The response indicates the service status.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
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
 * @param [in] Type
 *   - Specifying the Forward call type i.e. voice call, Video Call, Data Calls etc.
 *
 * @param [in] Condition
 *   - Used to specify the forward condition i.e. unconditional, When no reply, when busy, when not reachable etc.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF  and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data is #TelSsForwardingStatusInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * \see tel_set_ss_forward
 *
 * @code
 * #include<ITapiSS.h>
 * int  ret_status = 0;
 * TelSsForwardType_t Type =TAPI_CS_FORWARD_TYPE_VOICE_EV;
 * TelSsForwardWhen_t  Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
 * int pRequestId=0;
 *
 * ret_status = tel_get_ss_forward_status(Type,Condition,&pRequestId); // get forward status
 *
 * @endcode
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_get_ss_forward_status(TelSsForwardType_t Type, TelSsForwardWhen_t  Condition,int * req_id);

 /**
 *
 * @brief  This function activates/deactivates the call waiting service.
 * The call waiting service permits a mobile to be notified of an incoming
 * call (as per basic call procedures) while the traffic channel (BM or LM) is not available for the incoming call and the mobile subscriber
 * is engaged in an active or held call. Subsequently, the subscriber can accept, reject, or ignore the incoming call.
 * If this function is called normally, the asynchronous response comes in a few second from network. The response indicates
 * the service request has been granted or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
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
 * @param [in] waitInfo
 *   - Specifies the status of call-waiting service i.e. On (TRUE) or off (FALSE)
 *
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_WAITING_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data is #TelSsWaitingStatusInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * \see tel_get_ss_waiting_status
 *
 * @code
 * #include<ITapiSS.h>
 *
 * int   ret_status;
 * TelSsWaitingInfo_t waitInfo;
 * waitInfo.Mode = TAPI_SS_CW_ACTIVATE;
 * waitInfo.CallType =TAPI_CALL_TYPE_VOICE_EV;
 * int RequestID;
 *
 * ret_status = tel_set_ss_waiting (&waitInfo,&RequestID); // set call waiting
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_set_ss_waiting( const TelSsWaitingInfo_t *waitInfo, int * req_id);

 /**
 *
 * @brief  This function queries the status of call waiting service.
 *
 * @par Notes:
 * The call waiting service permits a mobile to be notified of an incoming call
 * (as per basic call procedures) while the traffic channel (BM or LM) is not available for the incoming call and the mobile subscriber
 * is engaged in an active or held call. Subsequently, the subscriber can accept, reject, or ignore the incoming call.
 * If this function is called normally, the asynchronous response comes in a few second from network.
 * The response indicates the service status.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
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
 * @param [in] CallType
 *   - TelSsCallType
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data is #TelSsWaitingInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 * \see tel_set_ss_waiting
 * @code
 * #include<ITapiSS.h>
 *
 * int   ret_status;
 * TelSsCallType_t CallType = TAPI_CALL_TYPE_VOICE_EV;
 * int RequestID;
 *
 * ret_status = tel_get_ss_waiting_status ( CallType,&RequestID); // get call waiting info
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_get_ss_waiting_status( const TelSsCallType_t CallType,int * req_id);

 /**
 *
 * @brief  This function queries the status of calling line identity service.
 *
 * @par Notes:
 * The group of Line Identification Supplementary Services is divided into the following six Supplementary Services:
 * CLIP - Calling line identification presentation
 *        This Supplementary Service provides for the ability to indicate the line identity of the calling party to the called party.
 *        The network shall deliver the calling line identity to the called party at call set-up time,
 *        regardless of the terminal capability to handle the information.
 * CLIR - Calling line identification restriction
 *	  the CLIR Supplementary Service is a Supplementary Service offered to the calling party
 *	  to prevent presentation of the calling party's line identity, to the called party.
 * COLP - Connected line identification presentation
 *        This Supplementary Service is not a dialing check but an indication to the calling subscriber
 *        of the connected line identity in a full ISDN/PLMN environment,
 *        the connected line identity shall include all the information necessary to unambiguously identify the connected party.
 *        The network shall deliver the connected line identity to the calling party
 *        regardless of the terminal capability to handle the information.
 * COLR - Connected line identification restriction
 *        The COLR Supplementary Service is a Supplementary Service offered
 *        to the connected party to prevent presentation of the connected line identity, to the calling party.
 *        In the case where the calling party has an override category.
 * CDIP - Called line identification presentation
 * CNAP - Calling name presentation
 * This function queries the service status with #TelSsCliType_t
 *
 * If this function is called normally, the asynchronous response comes in a few second from network.
 * The response indicates the service status.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 *
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
 * @param [in] CliType
 *   - TelSsCliType_t
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF and asynchronous return status is indicated by #TelSsCause_t.
 *  The event data is #TelCliStatusInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include<ITapiSS.h>
 *
 * int  ret_status = 0;
 * TelSsCliType_t CliType =TAPI_SS_CLI_CDIP;
 * int pRequestId;
 *
 * ret_status = tel_get_ss_cli_status(CliType,&pRequestId); // get cli info
 * @endcode
 *
 * @see
 * - None
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int   tel_get_ss_cli_status(TelSsCliType_t CliType,int * req_id);

 /**
 *
 * @brief  This function is used to send a USSD string or User response to the Network.
 *
 * The unstructured supplementary service data (USSD) mechanism allows the Mobile Station (MS) user and
 * a PLMN operator defined application to communicate in a way which is transparent
 * to the MS and to intermediate network entities.
 * The mechanism allows development of PLMN specific supplementary services.
 * USSD string is like *109, *109*72348937857623#.
 * There are two modes of USSD: MMI-mode and application mode.
 * MMI-mode USSD is for the transparent transport of MMI strings entered by the user to the network using this function
 * and for the transparent transport of text strings from the network that are displayed
 * by the mobile for user information.
 * Application mode USSD is for the transparent transport of data between the network and the UE.
 * Application mode USSD is intended to be used by applications in the network and their peer applications in the UE.
 * The peer application in the UE send response with #tel_send_ss_ussd_response
 *
 * If this function is called normally, the asynchronous response comes in a few second from the telephony server.
 * The response indicates whether the sending request is successfully delivered or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is delivered with the async response as below.
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] szUssdString
 *   - Data coding scheme used (GSM 7bit or 8bit default alphabet etc, defined	as 3GPP TS 23.038)
 *   - USSD String data length
 *   - ANSI string passed by the client
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_USSD_CNF and asynchronous return status is indicated by #TelSsCause_t. The event data
 *  is #TelSsUssdMsgInfo_t.
 *
 * @pre
 *  - Initialize Dbus connection with #tel_init
 *  - Register caller's application name with #tel_register_app_name
 *  - Register telephony events to listen
 *  - A event loop is running to listen events
 *  - No SS and USSD Transaction should be ongoing. If there an ongoing transaction, new USSD request will be returned to the Application
 *   with an error (USSD_BUSY)
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
 * \see #tel_send_ss_ussd_response, #tel_send_ss_ussd_cancel
 *
 *\image html ussd.jpg
 *\image latex ussd.jpg
 *
 * @code
 * #include<ITapiSS.h>
 *
 * char ussdString[TAPI_SS_USSD_DATA_SIZE_MAX];
 * int  ret;
 *
 * memset(ussdString, 0, TAPI_SS_USSD_DATA_SIZE_MAX);
 * strcpy(ussdString , "*124#");
 *
 * ret = tel_send_ss_ussd_request(ussdString, &request_id); // request ussd
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int tel_send_ss_ussd_request(const TelSsUssdMsgInfo_t *ussd_info, int *req_id);

 /**
 *
 * @brief  This function is used to send a USSD User response to the Network.
 *
 * @par Notes:
 * Refer #tel_send_ss_ussd_request. In the application mode, the network initiates USSD string
 * to the peer application at UE. The UE might reply a USSD string with this function.
 *
 * If this function is called normally, the asynchronous response comes in a few second from Telephony Server.
 * The response indicates whether the string is delivered successfully to network or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered asynchronously.
 * @par Sync (or) Async:
 *
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
 * @param [in] ussdRspRecord
 *   - Data coding scheme used (GSM 7bit or 8bit default alphabet etc, defined	as 3GPP TS 23.038)
 *   - USSD String data length
 *   - ANSI string passed by the client
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_USSD_CNF and asynchronous return status is indicated by #TelSsCause_t. The event data
 *  is #TelSsUssdMsgInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * \see #tel_send_ss_ussd_request, #tel_send_ss_ussd_cancel
 *
 *\image html ussd.jpg
 *\image latex ussd.jpg
 *
 * @code
 * #include<ITapiSS.h>
 *
 * char ussd_str[25] = {"*123*0#"};
 * unsigned char len =0;
 * int ret = 0;
 * len = strlen(ussd_str);
 *
 * TelSsUssdMsgRspInfo_t * ussdRspRecord;
 * ussdRspRecord = (TelSsUssdMsgRspInfo_t*)malloc(sizeof(TelSsUssdMsgRspInfo_t));
 * ussdRspRecord->SsRespType=TAPI_USSD_RES_OK;
 * ussdRspRecord->IndType=TAPI_USSD_IND_NOTIFY;
 * ussdRspRecord->UssdInfo.Dcs=0x0f;
 * ussdRspRecord->UssdInfo.RequestedBySAT=0;
 * strcpy(ussdRspRecord->UssdInfo.szUssdString,ussd_str);
 * ussdRspRecord->UssdInfo.szUssdString[strlen(ussdRspRecord->UssdInfo.szUssdString)]='\0';
 * ussdRspRecord->UssdInfo.UssdStringLength=strlen(ussd_str);
 * int pRequestId=0;
 *
 * ret = tel_send_ss_ussd_response( ussdRspRecord,&pRequestId); // send ussd response
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int  tel_send_ss_ussd_response(const TelSsUssdMsgInfo_t *ussd_info, int *req_id);


 /**
 *
 * @brief  This function is used to send a USSD cancel to release ussd session
 *
 * If this function is called normally, the asynchronous response comes in a few second from Telephony Server.
 * The response indicates whether the string is delivered successfully to network or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered asynchronously.
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
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_USSD_CNF and asynchronous return status is indicated by #TelSsCause_t. The event data
 *  is #TelSsUssdMsgInfo_t.
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
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #TapiResult_t for failure and error code
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * \see #tel_send_ss_ussd_request, #tel_send_ss_ussd_response.
 *
 *\image html ussd.jpg
 *\image latex ussd.jpg
 * @code
 * #include<ITapiSS.h>
 *
 * char ussdString[TAPI_SS_USSD_DATA_SIZE_MAX];
 * int  ret;
 * int request_id;
 *
 * memset(ussdString, 0, TAPI_SS_USiSD_DATA_SIZE_MAX);
 * strcpy(ussdString , "*124#");
 *
 * ret= tel_send_ss_ussd_request(ussdString,&request_id);
 *
 * ret = tel_send_ss_ussd_cancel(&equestId); // ussd cancel
 * @endcode
 *
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int tel_send_ss_ussd_cancel(int *req_id);

 /**
 *
 * @brief  This function is used to Read AOC parameter values on the SIM.
 *
 * @par Notes
 * This supplementary service provides the MS with the information to produce an estimate
 * of the cost of the service used. Charges are indicated for the call(s) in progress
 * when mobile originated or for the roaming leg only when mobile terminated.
 * Any charges for non-call related transactions, and for certain supplementary services,
 * such as Call Forwarding are not indicated.
 * The MS will receive at the beginning of each call (and as necessary during the call)
 * a message, the Charge Advice Information. This message contains the elements which together define
 * the rate at which the call is to be charged, time dependence,
 * data dependence and for unit increments - see TS 22.024 [3].
 * The MS shall still indicate appropriate charges even when roaming, based on Home PLMN units.
 * Where applicable, the volume charge for Packet data service,
 * in addition to the normal time dependent and incremental charges, shall be indicated.
 * To indicate the charge per call the Mobile station shall display the units consumed
 * so far during the present call(s) and maintain this value until the MS is switched off or a new call set-up is attempted.
 * Where required to indicate the total accumulated charge, the MS shall be able to display,
 * and the SIM/USIM shall store in the ACM, the running cumulative unit charge.
 * This value must be stored securely, and all reasonable steps shall be taken to ensure that
 * the written value cannot be interrupted, reset or corrupted (except resetting under control of the unblocking key).
 * This function get the AOC configuration information from PLMN
 *
 * If this function is called normally, the asynchronous response comes in a few second from Telephony Server.
 * The response indicates whether the query is delivered successfully to network or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered asynchronously.
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
 * @param [in] AocType
 *   - Specifies which type of Advice of charge information to be read namely Maximum ACM,
 *     Price per unit currency, Current call meter and Accumulated call meter values.
 *
 * @param [out] req_id
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_AOC_RSP and asynchronous return status is indicated by #TelSsCause_t. The event data
 *  is #TelCallAocInfo_t.
 *
 * @pre
 *  - Advice of charge depends on the SIM for setting or reading AOC parameters.
 *  - Setting of AOC needs PIN2 Verification
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
 * \see tel_set_ss_aoc_info
 *
 *\image html ussd.jpg
 *\image latex ussd.jpg
 * @code
 * #include<ITapiSS.h>
 *
 * TelSsAocType_t AocType = TAPI_SS_AOC_TYPE_ACM;
 * ret_status = tel_get_ss_aoc_info (AocType,&pRequestId); // get AOC info
 * @endcode
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int  tel_get_ss_aoc_info( TelSsAocType_t AocType,int * req_id);

 /**
 *
 * @brief  This function is used to Read AOC parameter values on the SIM.
 *
 * @par Notes:
 * Refer #tel_get_ss_aoc_info
 *
 * If this function is called normally, the asynchronous response comes in a few second from Telephony Server.
 * The response indicates whether the query is delivered successfully to network or not.
 *
 * This API makes Dbus method call to Telephony Sever and gets immediate feedback.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered asynchronously.
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
 * @param [in] AocSetInfo
 *   - Specifies which type of Advice of charge information to be set namely Maximum ACM,
 *     Price per unit currency, Current call meter and Accumulated call meter values.
 *
 * @param [out] RequestId
 *   - Unique identifier for a particular request.
 *   - RequestId value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.  *
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_SS_SET_AOC_CNF and asynchronous return status is indicated by #TelSsCause_t. The event data
 *  is #TelSsErrorInfo_t.
 *
 * @pre
 *  - Advice of charge depends on the SIM for setting or reading AOC parameters.
 *  - Setting of AOC needs PIN2 Verification
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
 * \see tel_get_ss_aoc_info
 *
 *\image html ussd.jpg
 *\image latex ussd.jpg
 * @code
 * #include<ITapiSS.h>
 *
 * TelCallAocInfo_t  *pAocSetInfo;
 * int request_id=0;
 * pAocSetInfo = (TelCallAocInfo_t *)calloc(1,sizeof(TelCallAocInfo_t));
 * pAocSetInfo->AocType = TAPI_SS_AOC_TYPE_RESET;
 * int ret_status =0;
 *
 * ret = tel_set_ss_aoc_info (pAocSetInfo,&request_id); // set aoc info
 * @endcode
 *
 *
 * @remarks
 * - None
 *
 *
 */
/*================================================================================================*/
int  tel_set_ss_aoc_info( TelCallAocInfo_t * AocSetInfo, int * RequestId);

#ifdef __cplusplus
}
#endif

#endif /* _ITAPI_SS_H_ */

/**
* @}
*/

