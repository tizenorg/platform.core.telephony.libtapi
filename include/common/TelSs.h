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
 * @addtogroup  SS_TAPI	SS(Supplementary services)
 * @{
 *
 * @file TelSs.h

 @brief This file serves as a "C" header file and defines structures for Tapi Supplementary Services\n
 It contains a sample set of constants, enums, structs that would be required by applications.
 */

#ifndef _TEL_SS_H_
#define _TEL_SS_H_

#include <TelDefines.h>
#include <TelErr.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** maximum length of barring password */
#define TAPI_SS_GSM_BARR_PW_LEN_MAX				4 /**<Maximum length of barring password */

/** maximum length of dial digit */
#define TAPI_CALL_DIALDIGIT_LEN_MAX				82 /**<maximum length of dial digit *///( To Accommodate Unpacked string length, which will be greater than 182, 182* 8 /7 = 208, Packed len = 182* 7/8 = 160)
#define TAPI_SS_USSD_DATA_SIZE_MAX				208	 /**<User-to-user data maximum size */
#define TAPI_SS_RECORD_NUM_MAX					5 /**< SS record maximum size */
#define TAPI_SS_AOC_CURRENCY_LEN_MAX			3 /**< maximum length of AOC currency */

/* Supplementary Svc */
#define TAPI_SS_ADDRESS_LEN_MAX					40
#define TAPI_SS_CCBS_SIZE_MAX					5
#define TAPI_SS_TELECOMM_SERVCE_SIZE_MAX		13
#define TAPI_SS_PHONE_NUM_LEN_MAX				33
#define TAPI_SS_MMISTRING_LEN_MAX				33
#define TAPI_SS_PWD_LEN_MAX						5
#define TAPI_MAX_RELEASE_COMPLETE_DATA_LEN		260
#define TAPI_MAX_ULONG							0xffffffff

/**
 *  This enumeration defines the call forwarding flavor.
 */
typedef enum {
	TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV, /**< Unconditional */
	TAPI_SS_FORWARD_WHEN_BUSY_EV, /**< When Busy */
	TAPI_SS_FORWARD_WHEN_NO_ANSWER_EV, /**< When No Reply */
	TAPI_SS_FORWARD_WHEN_NOT_REACHABLE_EV, /**< When Not Reachable */
	TAPI_SS_FORWARD_WHEN_ALL_FORWARDING_EV, /**< Forward All Calls */
	TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV, /**< Conditional */
} TelSsForwardWhen_t;

/**
 * The various types of call barring. Note, only one type of incoming barring and only one type of outgoing barring may be active at any time.
 */
typedef enum {
	TAPI_CALL_BARRING_ALL, /**< Barring all outgoing and incoming calls */
	TAPI_CALL_BARRING_OUTGOING, /**< Barring outgoing call */
	TAPI_CALL_BARRING_ALL_OUTGOING_CALLS, /**< Barring all outgoing calls */
	TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL, /**< Barring all outgoing international calls */
	TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL_EXCEPT, /**< Barring all international outgoing */
	TAPI_CALL_BARRING_INCOMING, /**< Barring the incoming call */
	TAPI_CALL_BARRING_ALL_INCOMING_CALLS, /**< Barring all incoming calls */
	TAPI_CALL_BARRING_ALL_INCOMING_CALLS_ROAMING, /**< Barring all incoming calls while roaming */
	TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM, /**< Barring Incoming Calls from numbers which is not stored in the SIM memory */
} TelSsCallBarType_t;

/*
 * The types of telecommunication services are used for setting the statuses of supplementary services.
 * They are typically not supported in IS-2000 or IS-136. See GSM 02.30 Annex C and GSM 22.030 Annex C.
 */
typedef enum {
	TAPI_SS_TS_NONE = 0x00, /**< Teleservice None*/

	TAPI_SS_TS_ALL_TELESERVICES, /**< All Teleservices */
	TAPI_SS_TS_ALL_SPEECH, /**< All Speech Services */
	TAPI_SS_TS_ALL_DATA_TELESERVICES, /**< All Data Teleservices */
	TAPI_SS_TS_FACS, /**< All Fax Service */
	TAPI_SS_TS_SMS, /**< SMS service	 */

	TAPI_SS_TS_ALL_TELESERVICES_EXCEPT_SMS, /**< All teleservice except SMS */
	TAPI_SS_TS_ALL_PLMN_SPEC_TELE, /**< PLMN specific teleservices*/
	TAPI_SS_TS_PLMN_SPEC_TELE_1, /**< PLMN specific teleservice 1*/
	TAPI_SS_TS_PLMN_SPEC_TELE_2, /**< PLMN specific teleservice 2*/
	TAPI_SS_TS_PLMN_SPEC_TELE_3, /**< PLMN specific teleservice 3*/

	TAPI_SS_TS_PLMN_SPEC_TELE_4, /**< PLMN specific teleservice 4*/
	TAPI_SS_TS_PLMN_SPEC_TELE_5, /**< PLMN specific teleservice 5*/
	TAPI_SS_TS_PLMN_SPEC_TELE_6, /**< PLMN specific teleservice 6*/
	TAPI_SS_TS_PLMN_SPEC_TELE_7, /**< PLMN specific teleservice 7*/
	TAPI_SS_TS_PLMN_SPEC_TELE_8, /**< PLMN specific teleservice 8*/

	TAPI_SS_TS_PLMN_SPEC_TELE_9, /**< PLMN specific teleservice 9*/
	TAPI_SS_TS_PLMN_SPEC_TELE_A, /**< PLMN specific teleservice 10*/
	TAPI_SS_TS_PLMN_SPEC_TELE_B, /**< PLMN specific teleservice 11*/
	TAPI_SS_TS_PLMN_SPEC_TELE_C, /**< PLMN specific teleservice 12*/
	TAPI_SS_TS_PLMN_SPEC_TELE_D, /**< PLMN specific teleservice 13*/

	TAPI_SS_TS_PLMN_SPEC_TELE_E, /**< PLMN specific teleservice 14*/
	TAPI_SS_TS_PLMN_SPEC_TELE_F, /**< PLMN specific teleservice 15*/
	TAPI_SS_TS_PLMN_SPEC_TELE_, /**< PLMN specific teleservice*/
	TAPI_SS_BS_ALL_BEARER_SERVICES, /**< All PLMN specific bearer services*/
	TAPI_SS_BS_ALL_DATA_CDA, /**< All Data CDA*/

	TAPI_SS_BS_ALL_DATA_CDS, /**< All Data CDS*/
	TAPI_SS_BS_ALL_ASYNCHRONOUS, /**< All  Asynchronous */
	TAPI_SS_BS_ALL_SYNCHRONOUS, /**< All  Synchronous */
	TAPI_SS_BS_DATA_CIRCUIT_SYNC, /**< All Data Circuit Synchronous */
	TAPI_SS_BS_DATA_CIRCUIT_ASYNC, /**< All Data Circuit Asynchronous */

	TAPI_SS_BS_DATA_ALL_PDS, /**< All Data PDS */
	TAPI_SS_BS_ALL_DATA_PADCA, /**< All Data PADCA */
	TAPI_SS_BS_ALL_PLMN_SPEC_BS, /**< All PLMN SPEC BS */
	TAPI_SS_BS_PLMN_SPEC_TELE_1, /**< PLMN specific bearer service 1*/
	TAPI_SS_BS_PLMN_SPEC_TELE_2, /**< PLMN specific bearer service 2*/

	TAPI_SS_BS_PLMN_SPEC_TELE_3, /**< PLMN specific bearer service 3*/
	TAPI_SS_BS_PLMN_SPEC_TELE_4, /**< PLMN specific bearer service 4*/
	TAPI_SS_BS_PLMN_SPEC_TELE_5, /**< PLMN specific bearer service 5*/
	TAPI_SS_BS_PLMN_SPEC_TELE_6, /**< PLMN specific bearer service 6*/
	TAPI_SS_BS_PLMN_SPEC_TELE_7, /**< PLMN specific bearer service 7*/

	TAPI_SS_BS_PLMN_SPEC_TELE_8, /**< PLMN specific bearer service 8*/
	TAPI_SS_BS_PLMN_SPEC_TELE_9, /**< PLMN specific bearer service 9*/
	TAPI_SS_BS_PLMN_SPEC_TELE_A, /**< PLMN specific bearer service 10*/
	TAPI_SS_BS_PLMN_SPEC_TELE_B, /**< PLMN specific bearer service 11 */
	TAPI_SS_BS_PLMN_SPEC_TELE_C, /**< PLMN specific bearer service 12 */

	TAPI_SS_BS_PLMN_SPEC_TELE_D, /**< PLMN specific bearer service 13 */
	TAPI_SS_BS_PLMN_SPEC_TELE_E, /**< PLMN specific bearer service 14 */
	TAPI_SS_BS_PLMN_SPEC_TELE_F, /**< PLMN specific bearer service 15*/
	TAPI_SS_TS_ALL_DATA, /**< All Data*/
	TAPI_SS_TS_DATA_CIRCUIT_SYNC, /**< Data Circuit Sync*/

	TAPI_SS_TS_MULTIPLE, /**< multiple*/
	TAPI_SS_TS_TELEPHONY, /**< Telephony */
	TAPI_SS_TS_DATA_CIRCUIT_ASYNC, /**< Data Circuit Async */
	TAPI_SS_TS_DATA_PACKET, /**< Data packet */

	/* CPHS - AUXILIARY SERVICE */
	TAPI_SS_TS_AUX_VOICE, /**< All Auxiliary Voice ( Auxiliary telephony ) */

	TAPI_SS_ALL_TELE_BEARER, /**< all tele and bearer services */
	TAPI_SS_TS_ALL_TELE_AND_BEARER_SERVICES, /**< Teleservices & Bearer Services */
} TelSsTeleService_t;

/**
 * This enumeration defines the ussd response type
 */
typedef enum {
	TAPI_USSD_RES_UNKNOWN_ALPHABET, /**< USSD data is unknown */
	TAPI_USSD_RES_DISPL_BUSY, /**< USSD Data response is display busy  */
	TAPI_USSD_RES_OK, /**< USSD Data is encoded and properly propagated */
	TAPI_USSD_RES_NONE /**< NO USSD Response from client */
} TelSsUssdRespType_t;

/**
 * This enumeration defines the ussd Indication Type
 */
typedef enum {
	TAPI_USSD_IND_NOTIFY = 0x00, /**< notify : to display USSD data to user */
	TAPI_USSD_IND_NO_ACTION_REQUIRE = 0x01, /**< No further user action required */
	TAPI_USSD_IND_ACTION_REQUIRE = 0x02, /**< Further user action required*/
	TAPI_USSD_IND_TERMINATED_BY_NET = 0x03, /**< 0x03 : USSD terminated by network */
	TAPI_USSD_IND_OTHER_CLIENT = 0x04, /**< 0x04 : other local client has responded */
	TAPI_USSD_IND_NOT_SUPPORT = 0x05, /**< 0x05 : operation not supported */
	TAPI_USSD_IND_TIME_OUT = 0x06 /**< 0x06 : Time out when there is no response from network */
} TelSsUssdIndType_t;

/**
 *
 * This enum defines the values for USSD type.
 */
typedef enum {
	TAPI_USSD_USER_INIT = 0x01, /**< USSD request type User Initiated. */
	TAPI_USSD_USER_RSP, /**< USSD request type User Response. */
	TAPI_USSD_USER_REL /**< USSD request type User Release. */
} TelSsUssdType_t;

/**
 *  This enumeration defines the call barring operation mode .
 */
typedef enum {
	TAPI_SS_CALL_BAR_ACTIVATE, /**< Activate call barring  */
	TAPI_SS_CALL_BAR_DEACTIVATE, /**< De Activate call barring */
} TelSsCallBarOperationMode_t;

/**
 *  This enumeration defines the call barring operation mode .
 */
typedef enum {
	TAPI_SS_CW_ACTIVATE, /**< Activate call barring  */
	TAPI_SS_CW_DEACTIVATE, /**< De Activate call barring */
} TelSsCallWaitOperationMode_t;

/**
 *  This enumeration defines the call type .
 */
typedef enum {
	TAPI_CALL_TYPE_VOICE_EV, /**< Voice Call */
	TAPI_CALL_TYPE_DATA_EV, /**< Data Call */
	TAPI_CALL_TYPE_FAX_EV, /**< FAX   Call */
	TAPI_CALL_TYPE_SHORT_MESS_EV, /**< Short Message */
	TAPI_CALL_TYPE_ALL_ASYNC_EV, /**< All Async services */
	TAPI_CALL_TYPE_ALL_SYNC_EV, /**< All sync services */
	TAPI_CALL_TYPE_DATA_CIRCUIT_SYNC_EV, /**< Data Circuit Sync */
	TAPI_CALL_TYPE_ALL_TELE_BEARER, /**< all tele and bearer services */
	TAPI_CALL_TYPE_ALL_TELE, /**< All tele services */
} TelSsCallType_t;

/**
 *  This enumeration defines the forward mode .
 */
typedef enum {
	TAPI_CALL_FORWARD_MODE_DISABLE_EV, /**< Deactivate call forwarding  */
	TAPI_CALL_FORWARD_MODE_ENABLE_EV, /**< Activate call forwarding */
	TAPI_CALL_FORWARD_MODE_REGISTRATION_EV, /**< Register Call forwarding  */
	TAPI_CALL_FORWARD_MODE_ERASURE_EV, /**< De-register call forwarding */
} TelSsForwardMode_t;

/**
 *  This enumeration defines the forward type .
 */
typedef enum {
	TAPI_CS_FORWARD_TYPE_VOICE_EV = 0, /**< Voice call forward */
	TAPI_CS_FORWARD_TYPE_ALL_ASYNC_EV, /**< All Async services */
	TAPI_CS_FORWARD_TYPE_ALL_SYNC_EV, /**< All sync services */
	TAPI_CS_FORWARD_TYPE_DATA_EV, /**< Data call forward */
	TAPI_CS_FORWARD_TYPE_FAX_EV, /**< Fax call forward */
	TAPI_CS_FORWARD_TYPE_ALL_TELE_BEARER, /**< All tele and bearer services */
	TAPI_CS_FORWARD_TYPE_ALL_TELE, /**< All tele services */
	TAPI_CS_FORWARD_TYPE_AUX_VOICE, /**< AUX Voice */
} TelSsForwardType_t;

typedef enum {
	TAPI_SS_CF_NO_REPLY_TIME_5_SECS = 5, /**< Timer value set to 5secs*/
	TAPI_SS_CF_NO_REPLY_TIME_10_SECS = 10, /**< Timer value set to 10secs*/
	TAPI_SS_CF_NO_REPLY_TIME_15_SECS = 15, /**< Timer value set to 15secs*/
	TAPI_SS_CF_NO_REPLY_TIME_20_SECS = 20, /**< Timer value set to 20secs*/
	TAPI_SS_CF_NO_REPLY_TIME_25_SECS = 25, /**< Timer value set to 25secs*/
	TAPI_SS_CF_NO_REPLY_TIME_30_SECS = 30, /**< Timer value set to 30secs*/
} TelSsNoReplyTime_t;

/**
 *  This enumeration type defines the Supplementary service types
 */
typedef enum {
	TAPI_SS_TYPE_BARRING, /**< Barring SS */
	TAPI_SS_TYPE_FORWARDING, /**<  Forward SS  */
	TAPI_SS_TYPE_WAITING, /**<  Waiting SS */
	TAPI_SS_TYPE_IDENTIFICATION, /**< Identification SS  */
	TAPI_SS_TYPE_CNAP, /**< CNAP SS */
	TAPI_SS_TYPE_USSD, /**<  Unstructured supplementary data SS  */
	TAPI_SS_TYPE_AOC, /**< AOC SS*/
} TelSsType_t;

/**
 *  Describes whether a supplementary service request was a success or a failure; and if it failed, why.
 */
typedef enum {
	TAPI_SS_SUCCESS, /**<  SS operation was successful */
	TAPI_SS_TIMEREXPIRE, /**< SS operation timer expired on network. */
	TAPI_SS_UNKNOWNSUBSCRIBER, /**< SS error indicating unknown/illegal subscriber.  */
	TAPI_SS_BEARERSERVICENOTPROVISIONED, /**<The network returns this error when it is requested to  @n
	 perform an operation on a supplementary service  */
	TAPI_SS_TELESERVICENOTPROVISIONED, /**<The network returns this error when it is requested to perform  @n
	 an operation on a supplementary service  */
	TAPI_SS_ILLEGALSSOPERATION, /**<This error is returned by the network when it is requested to perform an illegal operation @n
	 which is defined as not applicable for the relevant supplementary service */
	TAPI_SS_ERRORSTATUS, /**<This error is returned by the network when it is requested to perform an operation @n
	 which is not compatible with the current status of the relevant supplementary service. */
	TAPI_SS_NOTAVAILABLE, /**< SS not available in network */
	TAPI_SS_SUBSCRIPTIONVIOLATION, /**< SS service subscription violation. */
	TAPI_SS_INCOMPATIBILITY, /**< This error is returned by the network when it is requested for a supplementary service operation incompatible with the @n
	 status of another supplementary service or with the teleservice or bearer service for which the operation is requested */
	TAPI_SS_SYSTEMFAILURE, /**< This error is returned by the network, when it cannot perform an operation because of a failure in the network */
	TAPI_SS_DATAMISSING, /**< This error is returned by the network when an optional parameter is missing in an invoke component @n
	 or an inner data structure, while it is required by the context of the request. */
	TAPI_SS_UNEXPECTEDDATAVALUE, /**< SS error indicating unexpected data value on network side *//**< SS operation barred.  */
	TAPI_SS_PWREGISTRATIONFAILURE, /**< SS error indicating change password failure. */
	TAPI_SS_NEGATIVEPWCHECK, /**< SS error indicating negative password check.  */
	TAPI_SS_FACILITYNOTSUPPORTED, /**< SS service facility not supported  */
	TAPI_SS_RESOURCESNOTAVAILABLE, /**< SS error indicating resources not available in network.  */
	TAPI_SS_MAXNOMPTYEXCEEDED, /**< SS error indicating Maximum MPTY is reached.  */
	TAPI_SS_CALLBARRED, /**< This error is returned by the network to the MS when call independent subscriber control procedures are barred by the operator */
	TAPI_SS_NUMBEROFPWATTEMPTSVIOLATION, /**< SS error indicating barring password attempts violated.  */
	TAPI_SS_ABSENTSUBSCRIBER, /**< This error is returned when the subscriber has activated the detach service or the system detects the absence condition */
	TAPI_SS_ILLEGALSUBSCRIBER, /**<This error is returned when illegality of the access has been @n
	 established by use of authentication procedure. */
	TAPI_SS_ILLEGALEQUIPMENT, /**<This error is returned when the IMEI check procedure has shown that  @n
	 the IMEI is blacklisted or not whitelisted  */
	TAPI_SS_USSDBUSY, /**< SS error indicating USSD Busy(Already SS / USSD is ongoing).  */
	TAPI_SS_UNKNOWNALPHABET, /**< SS error indicating unknown SS data coding of alphabet */
	TAPI_SS_INVALIDDEFLECTEDTONUMBER, /**< SS error indicating the invalid deflected to number.  */
	TAPI_SS_DEFLECTIONTOSERVEDSUBSCRIBER, /**< This error is returned if a diversion to the served  @n
	 subscriber's number was requested.  */
	TAPI_SS_SPECIALSERVICECODE, /**< This error is returned if diversion to a special service code was requested.  */
	TAPI_SS_REJECTEDBYUSER, /**< SS operation rejected by user.  */
	TAPI_SS_REJECTEDBYNETWORK, /**< SS operation rejected by network.  */
	TAPI_SS_NET_NOT_ALLOWED_EMERGENCY_CALLS_ONLY, /**< SS operation is not allowed by network.  */
	TAPI_SS_UNKNOWNERROR, /**< SS error indicating unknown error  */
	TAPI_SS_OEM_NOT_SUPPORTED /**< If oem do not support any of SS requests, then this error will be returned back */
} TelSsCause_t;

/**
 *   The status of a supplementary service feature (e.g. Call Forwarding or Call Barring). @n
 *   These enumerated values should be used as masks
 */
typedef enum {
	TAPI_SS_STATUS_ACTIVE, /**< Provisioned & registered & active */
	TAPI_SS_STATUS_REGISTERED, /**< Provisioned & registered (but not active/active-quiescent */
	TAPI_SS_STATUS_PROVISIONED, /**<Provisioned but not registered (or active/active-quiescent) */
	TAPI_SS_STATUS_QUIESCENT, /**< Provisioned & registered & active but quiescent */
	TAPI_SS_STATUS_NOTHING /**< Not provisioned */
} TelSsStatus_t;

/**
 * These are the four types of identity presentation / restriction services.
 */
typedef enum {
	TAPI_CALLING_LINE_IDENTITY_PRESENTATION, /**< identify the party calling this phone */
	TAPI_CALLING_LINE_IDENTITY_RESTRICTION, /**< hide the identity of this phone when calling others */
	TAPI_CONNECTED_LINE_IDENTITY_PRESENTATION, /**< identify the party to whom the calling party (this phone) is connected */
	TAPI_CONNECTED_LINE_IDENTITY_RESTRICTION, /**< restrict yourself from being identified by incoming calls, such as forwarded calls */
	TAPI_CALLED_LINE_IDENTITY_PRESENTATION, /**< Called line identity presentation  */
	TAPI_CALLING_NAME_PRESENTATION /**< Calling Name Presentation */
} TelSsLineIdentificationType_t;

/**
 *  This enumeration defines the cli service status .
 */
typedef enum {
	TAPI_CLI_STATUS_NOT_PROVISONED = 0x01, /**<Service not provided by the service provider */
	TAPI_CLI_STATUS_PROVISIONED, /**<Service is provided by the service provider */
	TAPI_CLI_STATUS_ACTIVATED, /**<Service is activated at the network */
	TAPI_CLI_STATUS_UNKOWN, /**<Service status is unknown*/
	TAPI_CLI_STATUS_TEMP_RESTRICTED, /**<Service is temporarily restricted */
	TAPI_CLI_STATUS_TEMP_ALLOWED /**<Service is temporarily allowed */
} TelSsCliStatus_t;

/**
 *  This enumeration defines the values for Cli service type .
 */
typedef enum {
	TAPI_SS_CLI_CLIP = 0x01, /**< Calling Line Identification Presentation. */
	TAPI_SS_CLI_CLIR = 0x02, /**<  Calling Line Identification Restriction. */
	TAPI_SS_CLI_COLP = 0x03, /**<  Connected Line Identification Presentation. */
	TAPI_SS_CLI_COLR = 0x04, /**<  Connected Line Identification Restriction. */
	TAPI_SS_CLI_CDIP = 0x05, /**<  Called Line Identification Presentation. */
	TAPI_SS_CLI_CNAP = 0x06, /**<  Calling Name Presentation.*/
} TelSsCliType_t;

/**
 * This enum defines the values for AOC type.
 */
typedef enum {
	TAPI_SS_AOC_TYPE_RESET = 0x00, /**< Specifies the Reset MAXACM Value. */
	TAPI_SS_AOC_TYPE_ACM = 0x01, /**< Specifies the Accumulated call meter. */
	TAPI_SS_AOC_TYPE_CCM = 0x02, /**< Specifies the Current call meter. */
	TAPI_SS_AOC_TYPE_MAXACM = 0x04, /**< Specifies the Max accumulated call meter. */
	TAPI_SS_AOC_TYPE_PUC = 0x08 /**< Specifies the Price per unit and currency.*/
} TelSsAocType_t;

/**
 *  This structure  defines the different parameters related to forward info.
 */
typedef struct {
	TelSsForwardMode_t Mode; /**< Forward Mode */
	TelSsForwardType_t Type; /**< Forward Type */
	TelSsForwardWhen_t Condition; /**< Forward Condition */
	char szPhoneNumber[TAPI_CALL_DIALDIGIT_LEN_MAX]; /**< Phone Number*/
	TelSsNoReplyTime_t NoReplyConditionTimer; /**< No reply wait time 5-30 secs in intervals of 5. */
} TelSsForwardInfo_t;

/**
 *  This structure defines the values for USSD request type.
 */
typedef struct {
	int UssdStringLength; /**< USSD  String Length */
	char szUssdString[TAPI_SS_USSD_DATA_SIZE_MAX]; /**< USSD  String */
} TelSsUssdMsgInfo_t;

/**
 *This structure defines the UssdMessageResponse Information
 */
typedef struct {
	TelSsUssdRespType_t SsRespType; /**< USSD Response Type*/
	TelSsUssdIndType_t IndType; /**< USSD Indication Type*/
	TelSsUssdMsgInfo_t UssdInfo; /**< USSD Data*/
} TelSsUssdMsgRspInfo_t;

typedef struct {
	TelSsUssdIndType_t IndType; /**< USSD Indication Type*/
	TelSsUssdMsgInfo_t UssdInfo; /**< USSD Data */
} TelSsUssdMsgIndInfo_t;

/**
 *  This structure  defines the different parameters related to call barring.
 */
typedef struct {
	TelSsCallBarOperationMode_t Mode; /**< Barring mode  */
	TelSsCallBarType_t Type; /**< Barring type */
	TelSsCallType_t CallType; /**< Call type */
	char szPassword[TAPI_SS_GSM_BARR_PW_LEN_MAX]; /**< password */
} TelSsCallBarringInfo_t;

/**
 *  This structure  defines the different parameters related to call waiting.
 */
typedef struct {
	TelSsCallWaitOperationMode_t Mode; /**< Waiting mode  */
	TelSsCallType_t CallType; /**< Call type */
} TelSsWaitingInfo_t;

/**
 * This structure defines the values for Calling line identity service
 */
typedef struct {
	TelSsLineIdentificationType_t IdentificationType; /**< Various line identification types */
	TelSsCliStatus_t CliStatus; /**< Line identification status from network */
} TelSsCliRecord_t;

/**
 * This structure defines the release complete message notification type.
 */
typedef struct {
	unsigned char RelCompMsgLen; /**< Specifies the Release complete msg length.*/
	unsigned char szRelCompMsg[TAPI_MAX_RELEASE_COMPLETE_DATA_LEN]; /**<  Specifies the Release complete msg. */
} TelSsRelCompMsgInfo_t;

/**
 * This structure defines the values for AOC request type.
 */

typedef struct {
	float PPM;
	unsigned char CharTypeOfCurrency;
	unsigned char szCurrency[TAPI_SS_AOC_CURRENCY_LEN_MAX];
} TelAocPucInfo_t;

typedef struct {
	TelSsAocType_t AocType; /**< Specifies the AOC type. */
	unsigned int ACM; /**< Specifies the accumulated call meter value. */
	unsigned int MaxAcm; /**< Specifies the maximum value of ACM . */
	float CCM; /**< Specifies the Current call meter value. */
	float PPM; /**< Specifies the Price per unit value. */
	unsigned char CharTypeOfCurrency; /**< Specifies the Char type of currency. */
	unsigned char szCurrency[TAPI_SS_AOC_CURRENCY_LEN_MAX]; /**< Specifies the Currency characters. */
} TelCallAocInfo_t;

/**
 * This structure defines the values for ss call barring record
 */
typedef struct {
	struct {
		TelSsStatus_t Status; /**< SS status  */
		TelSsCallBarType_t Flavour; /**< Call barring types providing various barring conditions on that basis call be barred */
		TelSsTeleService_t TeleCommService; /**<  Teleservice */
	} rec_class[TAPI_SS_RECORD_NUM_MAX]; /**< Specifies the  Maximum of TAPI_SS_RECORD_NUM_MAX records. */
} TelSsCallBarringRecord_t;

/**
 * This structure defines the values for ss call forwarding record
 */
typedef struct {
	struct {
		TelSsStatus_t Status; /**< Call forwarding SS status */
		TelSsForwardWhen_t ForwardCondition; /**< Call forward types providing various conditions when call can be forwarded */
		TelSsTeleService_t TeleCommService; /**< Teleservice type */
		int bCallForwardingNumberPresent; /**< Call forwarding number present or not */
		unsigned char NoReplyWaitTime;
		unsigned char szCallForwardingNumber[TAPI_CALL_DIALDIGIT_LEN_MAX]; /**< forwarded number.[Null Terminated string]*/
	} rec_class[TAPI_SS_RECORD_NUM_MAX]; /**< Specifies the  Maximum of TAPI_SS_RECORD_NUM_MAX records. */

} TelSsForwardRecord_t;

/**
 * This structure defines the values for ss call waiting record
 */
typedef struct {
	struct {
		TelSsStatus_t Status; /**< SS status */
		TelSsTeleService_t TeleCommService; /**< Teleservice */
	} rec_class[TAPI_SS_RECORD_NUM_MAX]; /**< Specifies the  Maximum of TAPI_SS_RECORD_NUM_MAX records. */
} TelSsCallWaitingRecord_t;

/**
 *This structure defines the parameters for SS record type
 */
typedef union {
	TelSsCallWaitingRecord_t WaitingRecord; /**< Call waiting information */
	TelSsCallBarringRecord_t BarringRecord; /**< Call barring information*/
	TelSsCliRecord_t CliRecord; /**< Line identification information */
	TelSsForwardRecord_t ForwardingRecord; /**< Call forward record information */
} TelSsRecord_t;

/**
 * related all kind of SS information
 */
typedef struct {
	TelSsType_t SsType; /**< Supplementary service type */
	TelSsStatus_t SsStatus; /**<SS status information */
	unsigned int NumberOfRecords; /**< Number of records present for Call Barring / Call forwarding / Call waiting. In case of any error, this parameter value will be set to zero.*/
	TelSsRecord_t SsRecord; /**< SS record (Union of Call Barring / Call Forwarding/Call waiting / line identification information). This structure can be Null when number_of_records parameter is zero.*/
	TelSsTeleService_t SsTeleserviceType; /**< Teleservice type */
} TelSsInfo_t;

typedef struct {
	float PPM; /**< Specifies the Price per unit value. */
	unsigned char CharTypeOfCurrency; /**< Specifies the Char type of currency. */
	unsigned char szCurrency[TAPI_SS_AOC_CURRENCY_LEN_MAX]; /**< Specifies the Currency characters. */
} TelSsAoc_ppm_info_t;

#ifdef __cplusplus
}
#endif

#endif // _TEL_SS_H_
/**
 * @}
 */
