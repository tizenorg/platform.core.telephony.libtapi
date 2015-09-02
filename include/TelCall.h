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
 * @file TelCall.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_CALL
 * @{
 *
 */

#ifndef _TEL_CALL_H_
#define _TEL_CALL_H_

#include <TelSs.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Definition for the maximum length of the string for the calling party name.
 * @since_tizen 2.3
 */
#define TAPI_CALLING_NAME_SIZE_MAX	80

/**
 * @brief Definition for the maximum length of the user to user signalling data string.
 * @since_tizen 2.3
 */
#define TAPI_CALL_UUS_DATA_LEN_MAX	131

/**
 * @brief Definition for the maximum length of the string for display record in CDMA.
 * @since_tizen 2.3
 */
#define TAPI_CALL_DISPLAY_RECORD_MAX	32

/**
 * @brief Definition for the maximum number of calls possible at a time.
 * @since_tizen 2.3
 */
#define MAX_TAPI_CALL_COUNT	6

/**
 * @brief Definition for the maximum number of calls possible in a conference call at a time.
 * @since_tizen 2.3
 */
#define MAX_TAPI_CONF_CALL_COUNT	5

/**
 * @brief Definition for the invalid call handle.
 * @details TS_UINT is used for the call handle.
 * @since_tizen 2.3
 */
#define INVALID_CALL_HANDLE	0

/**
 * @brief Definition for the maximum dialling digit length.
 * @since_tizen 2.3
 */
#define	TAPI_CALL_DIALDIGIT_LEN_MAX		82

/**
 * @brief Definition for the maximum call subaddress length.
 * @since_tizen 2.3
 */
#define	TAPI_CALL_SUBADDRESS_LEN_MAX	40

/**
 * @brief Definition for the dialling number length.
 * @since_tizen 2.3
 */
#define	TAPI_CALL_NUMBER_LEN_MAX		7

/**
 * @brief Definition for the maximum call context size.
 * @since_tizen 2.3
 */
#define	TAPI_CONTEXT_CALL_SIZE_MAX		6

/**
 * @brief Definition for the maximum dialling number length.
 * @since_tizen 2.3
 */
#define	TAPI_CALL_DIAL_NUMBER_LEN_MAX	82

/**
 * @brief Definition for the maximum Burst DTMF string length.
 * @since_tizen 2.3
 */
#define	TAPI_CALL_BURST_DTMF_STRING_MAX 32

/**
 * @brief Enumeration for the call cause values.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CAUSE_SUCCESS,			/**< Success */
	TAPI_CAUSE_ACCESSDISCARD,		/**< Access discarded */
	TAPI_CAUSE_BARR_BYOPERATOR,		/**< Barred by operator */
	TAPI_CAUSE_BEARERMODE_NOTIMPL,		/**< Bearer mode not implemented */
	TAPI_CAUSE_BEARERMODE_RESTRICTED,	/**< Bearer mode restricted */
	TAPI_CAUSE_BEARERMODE_UNAUTH,		/**< Bearer mode unauthorized */
	TAPI_CAUSE_BEARERMODE_UNAVAIL,		/**< Bearer mode not available */
	TAPI_CAUSE_BUSY,			/**< (Network/Server) busy */
	TAPI_CAUSE_CALLMETER_EXPIRED,		/**< Call meter expired */
	TAPI_CAUSE_CALLNO_ERROR,		/**< Call number error */
	TAPI_CAUSE_CKTUNAVAIL,			/**< Circuit channel unavailable */		/* 10 */
	TAPI_CAUSE_CONGESTION,			/**< Congestion occurred */
	TAPI_CAUSE_NO_CIRCUIT_AVAIL,		/**< Circuit not available */
	TAPI_CAUSE_DESTIN_INCOMPAT,		/**< Destination incompatibility */
	TAPI_CAUSE_DTMF_NOSPEECH,		/**< No speech in DTMF */
	TAPI_CAUSE_DTMF_REJECTED,		/**< DTMF rejected */
	TAPI_CAUSE_FACILITY_NOTIMPL,		/**< Facility not implemented */
	TAPI_CAUSE_FACILITY_NOTSUBSCRIBED,	/**< Facility not subscribed */
	TAPI_CAUSE_INCOMINGCUGCALLS_BARRED,	/**< Incoming CUG Calls barred */
	TAPI_CAUSE_INVALNUM,			/**< Invalid number */
	TAPI_CAUSE_MPTY_ERROR,			/**< Multiparty error */			/* 20 */
	TAPI_CAUSE_NOANSWER,			/**< No answer */
	TAPI_CAUSE_NONCUGMEMBER,		/**< Non CUG member */
	TAPI_CAUSE_NUMBERCHANGED,		/**< Number changed */
	TAPI_CAUSE_NUMBER_ERROR,		/**< Number error */
	TAPI_CAUSE_NWFAILURE,			/**< Network failure */
	TAPI_CAUSE_OPERATIONUNAVAIL,		/**< Operation not available */
	TAPI_CAUSE_OUTOFORDER,			/**< Out of order */
	TAPI_CAUSE_PROTOCOL_ERROR,		/**< Protocol error */
	TAPI_CAUSE_QOSUNAVAIL,			/**< QOS unavailable */
	TAPI_CAUSE_REJECT,			/**< Rejected */				/* 30 */
	TAPI_CAUSE_REJ_FAIL,			/**< Rejection failed */
	TAPI_CAUSE_REJ_SRVC_NOT_AVL,		/**< Rejection service not available */
	TAPI_CAUSE_REJ_SAT_CALL_CTRL,		/**< Rejection SAT Call Control */
	TAPI_CAUSE_REMOTE_CKTUNAVAIL,		/**< Remote Circuit channel unavailable */
	TAPI_CAUSE_RESOURCEUNAVAIL,		/**< Resource not available */
	TAPI_CAUSE_SERVICEID_ERROR,		/**< Service ID error */
	TAPI_CAUSE_SERVICE_NOTIMPL,		/**< Service not implemented */
	TAPI_CAUSE_SERVICE_UNAVAIL,		/**< Service not available */
	TAPI_CAUSE_MODIFY_SAME_BEARER,		/**< Modify same bearer */
	TAPI_CAUSE_MODIFY_NOT_PERMITTED,	/**< Modification not permitted */
	TAPI_CAUSE_HOLD_FAIL,			/**< Call hold failure */			/* 40 */
	TAPI_CAUSE_TEMPFAILURE,			/**< Temporary failure */
	TAPI_CAUSE_WRONGNUM,			/**< Wrong number */
	TAPI_CAUSE_NORMAL_UNSPECIFIED,		/**< Normal but unspecified */
	TAPI_CAUSE_SSERRINVALIDTIVALUE,		/**< SS invalid transaction ID value */
	TAPI_CAUSE_SSERRINVALIDTI,		/**< SS Invalid transaction ID */
	TAPI_CAUSE_SSINCOMPATIBLESTATE,		/**< SS incompatible state */
	TAPI_CAUSE_SSERRSYNTAXERROR,		/**< SS syntax error */
	TAPI_CAUSE_SSPROTOCOLERROR,		/**< Protocol error */
	TAPI_CAUSE_SSNEGATIVEPASSWORDCHECK,	/**< Negative password check */
	TAPI_CAUSE_SSSENTASPHASE1,		/**< SS sent as phase1 message */		/* 50 */
	TAPI_CAUSE_SSERROR,			/**< Supplementary service error */
	TAPI_CAUSE_SS_USSD_BUSY,		/**< Second USSD request will be rejected when a USSD transaction is already in progess */
	TAPI_CAUSE_IDLE,			/**< Idle */
	TAPI_CAUSE_NETWORK_SPECIFIC,		/**< Network specific error */
	TAPI_CAUSE_FADE,			/**< Voice call is dropped e.g. because of a loss of signal */
	TAPI_CAUSE_UNKNOWN,			/**< Unknown and no details available */
	TAPI_CAUSE_INCOM_CALL,			/**< Terminated by an incoming call */
	TAPI_CAUSE_ALERT_STOP,			/**< Terminated by an alert stop */
	TAPI_CAUSE_INTERCEPT,			/**< Voice call is dropped by interception */
	TAPI_CAUSE_REORDER,			/**< Voice call is dropped by reordering */	/* 60 */
	TAPI_CAUSE_CLIENT_END,			/**< Client ended the call */
	TAPI_CAUSE_ACCESS_CLASS_BLOCKED,	/**< Access class blocked */
	TAPI_CAUSE_MEMCAPEXCEEDED,		/**< Memory capacity exceeded */
	TAPI_CAUSE_TYPENOTSUPPORTED,		/**< Type not supported */
	TAPI_CAUSE_REPLACENOTSUPPORTED,		/**< Replace not supported */
	TAPI_CAUSE_PROTOCOLID_ERROR,		/**< Protocol ID error */
	TAPI_CAUSE_CLASSNOTSUPPORTED,		/**< Class not supported */
	TAPI_CAUSE_DATACODING_ERROR,		/**< Data coding error */
	TAPI_CAUSE_INVAL_MSG,			/**< Invalid message */
	TAPI_CAUSE_CALL_BARRED,			/**< Call barred */				/* 70 */
	TAPI_CAUSE_SIM_CALL_CONTROL_CHANGED_TO_SS,	/**< Sim call control changed to SS */
	TAPI_CAUSE_SIM_CALL_CONTROL_CHANGED_TO_CALL,	/**< SIM Call control changed to call */
	TAPI_CAUSE_SIM_ERROR,				/**< SIM error */
	TAPI_CAUSE_SIM_CALL_CONTROL_MODIFIED,		/**< SIM call control modified */
	TAPI_CAUSE_RANDOM_ACCESS_FAILURE,		/**< Random access error */
	TAPI_CAUSE_LOWER_LAYER_FAILURE,			/**< Lower layer error */
	TAPI_CAUSE_ACCESS_REJECTED,			/**< Access rejected */
	TAPI_CAUSE_RESET_NOT_ALLOWED,			/**< Reset not allowed */
	TAPI_CAUSE_EMERGENCY_CALL_NOT_ALLOWED,		/**< Emergency call not allowed */
	TAPI_CAUSE_ACCESS_NOT_ALLOWED,			/**< Access not allowed */		/* 80 */
	TAPI_CAUSE_RADIO_LINK_FAILURE,			/**< Radio link failure */
	TAPI_CAUSE_INVALID_IDENTITY,			/**< Invalid identity */
	TAPI_CAUSE_UNKNOWN_IDENTITY,			/**< Unknown identity */
	TAPI_CAUSE_UNACCEPTABLE_IDENTITY,		/**< Unacceptable identity */
	TAPI_CAUSE_NO_SERVICE_HERE,			/**< No service here */
	TAPI_CAUSE_SERVICE_NOT_ALLOWED,			/**< Service not allowed */
	TAPI_CAUSE_SERVICE_NOT_AVAIL,			/**< Service not available */
	TAPI_CAUSE_SERVICE_NOT_WORKING,			/**< Service not working */
	TAPI_CAUSE_CANNOT_IDENTIFY_CALL,		/**< Cannot identify the call */
	TAPI_CAUSE_DEACTIVATION,			/**< Deactivation */			/* 90 */
	TAPI_CAUSE_FATAL_ERROR,				/**< Fatal error */
	TAPI_CAUSE_SEND_DTMF_SUCCESS,			/**< Sending DTMF succeeded */
	TAPI_CAUSE_SEND_DTMF_FAIL,			/**< Sending DTMF failed */
	TAPI_CAUSE_FIXED_DIALING_NUMBER_ONLY,		/**< Fixed Dialing Number Only */
	TAPI_CAUSE_TIMER_EXPIRED,			/**< Call Timer Expired */
} TelCallCause_t;

/**
 * @brief Enumeration when a call is ended, this end cause is given with the CALL END notification to the application.
 * @details This end cause can be originated from different elements in the communication path or different modules in the same element.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_END_NO_CAUSE = 0x00,			/**< No Cause */

	TAPI_CC_CAUSE_UNASSIGNED_NUMBER,		/**< Unassigned Number */
	TAPI_CC_CAUSE_NO_ROUTE_TO_DEST,			/**< No Route to Destination */
	TAPI_CC_CAUSE_CHANNEL_UNACCEPTABLE,		/**< Channel Unacceptable */
	TAPI_CC_CAUSE_OPERATOR_DETERMINED_BARRING,	/**< Operator Determined Barring */
	TAPI_CC_CAUSE_NORMAL_CALL_CLEARING,		/**< Normal Call Clearing */

	TAPI_CC_CAUSE_USER_BUSY,			/**< User Busy */
	TAPI_CC_CAUSE_NO_USER_RESPONDING,		/**< No user responding */
	TAPI_CC_CAUSE_USER_ALERTING_NO_ANSWER,		/**< User Alerting no Answer */
	TAPI_CC_CAUSE_CALL_REJECTED,			/**< Call Rejected */
	TAPI_CC_CAUSE_NUMBER_CHANGED,			/**< Number Changed */

	TAPI_CC_CAUSE_NON_SELECTED_USER_CLEARING,	/**< Non Selected User Clearing */
	TAPI_CC_CAUSE_DESTINATION_OUT_OF_ORDER,		/**< Destination out of Order */
	TAPI_CC_CAUSE_INVALID_NUMBER_FORMAT,		/**< Invalid Number Format */
	TAPI_CC_CAUSE_FACILITY_REJECTED,		/**< Facility Rejected */
	TAPI_CC_CAUSE_RESPONSE_TO_STATUS_ENQUIRY,	/**< Response to Status Enquiry */

	TAPI_CC_CAUSE_NORMAL_UNSPECIFIED,		/**< Normal Unspecified */
	TAPI_CC_CAUSE_NO_CIRCUIT_CHANNEL_AVAILABLE,	/**< No Circuit Channel Available */
	TAPI_CC_CAUSE_NETWORK_OUT_OF_ORDER,		/**< Network out of Order */
	TAPI_CC_CAUSE_TEMPORARY_FAILURE,		/**< Temporary Failure */
	TAPI_CC_CAUSE_SWITCHING_EQUIPMENT_CONGESTION,	/**< Switching Equipment Congestion */

	TAPI_CC_CAUSE_ACCESS_INFORMATION_DISCARDED,		/**< Access Information Discarded */
	TAPI_CC_CAUSE_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE,	/**< Requested Circuit channel not available */
	TAPI_CC_CAUSE_RESOURCES_UNAVAILABLE_UNSPECIFIED,	/**< Resources unavailable and unspecified */
	TAPI_CC_CAUSE_QUALITY_OF_SERVICE_UNAVAILABLE,		/**< Quality of service unavailable */
	TAPI_CC_CAUSE_REQUESTED_FACILITY_NOT_SUBSCRIBED,	/**< Requested facility not subscribed */

	TAPI_CC_CAUSE_INCOMING_CALL_BARRED_WITHIN_CUG,		/**< Incoming call barred within CUG */
	TAPI_CC_CAUSE_BEARER_CAPABILITY_NOT_AUTHORISED,		/**< Bearer capability not Authorised */
	TAPI_CC_CAUSE_BEARER_CAPABILITY_NOT_PRESENTLY_AVAILABLE,/**< Bearer capability not presently Available */
	TAPI_CC_CAUSE_SERVICE_OR_OPTION_NOT_AVAILABLE,		/**< Service or Option not available */
	TAPI_CC_CAUSE_BEARER_SERVICE_NOT_IMPLEMENTED,		/**< Service not implemented */

	TAPI_CC_CAUSE_ACM_GEQ_ACMMAX,				/**< ACM GEQ ACMAX */
	TAPI_CC_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED,	/**< Requested facility not implemented */
	TAPI_CC_CAUSE_ONLY_RESTRICTED_DIGITAL_INFO_BC_AVAILABLE,/**< Restricted digital info BC not available */
	TAPI_CC_CAUSE_SERVICE_OR_OPTION_NOT_IMPLEMENTED,	/**< Service or Option not Implemented */
	TAPI_CC_CAUSE_INVALID_TRANSACTION_ID_VALUE,		/**< Transaction ID value */

	TAPI_CC_CAUSE_USER_NOT_MEMBER_OF_CUG,			/**< Not a member of CUG */
	TAPI_CC_CAUSE_INCOMPATIBLE_DESTINATION,			/**< Incompatible Destination */
	TAPI_CC_CAUSE_INVALID_TRANSIT_NETWORK_SELECTION,	/**< Transit Network selection */
	TAPI_CC_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,		/**< Semantically Incorrect message */
	TAPI_CC_CAUSE_INVALID_MANDATORY_INFORMATION,		/**< Invalid Mandatory Message */

	TAPI_CC_CAUSE_MESSAGE_TYPE_NON_EXISTENT,			/**< Message Type Non Existent */
	TAPI_CC_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STATE,	/**< Message type not compatible with Prot state */
	TAPI_CC_CAUSE_IE_NON_EXISTENT_OR_NOT_IMPLEMENTED,		/**< IE non exitent or not implemented */
	TAPI_CC_CAUSE_CONDITIONAL_IE_ERROR,				/**< Conditional IE error */
	TAPI_CC_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE,	/**< Not Compatible with protocol state */

	TAPI_CC_CAUSE_RECOVERY_ON_TIMER_EXPIRY,	/**< Recovery on timer expiry */
	TAPI_CC_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,	/**< Protocol error unspecified */
	TAPI_CC_CAUSE_INTERWORKING_UNSPECIFIED,	/**< Interworking unspecified */
	TAPI_CC_CAUSE_REORDER,	/**< Reorder */

	TAPI_CC_CAUSE_END = 128,	/**< End unspecified */

	/* Reject causes*/
	TAPI_REJECT_CAUSE_IMSI_UNKNOWN_IN_HLR,	/**< IMSI unknown in HLR */
	TAPI_REJECT_CAUSE_ILLEGAL_MS,		/**< Illegal MS */
	TAPI_REJECT_CAUSE_IMSI_UNKNOWN_IN_VLR,	/**< IMSI unknown in VLR */
	TAPI_REJECT_CAUSE_IMEI_NOT_ACCEPTED,	/**< IMEI not accepted */
	TAPI_REJECT_CAUSE_ILLEGAL_ME,		/**< Illegal ME */

	TAPI_REJECT_CAUSE_GPRS_SERVICES_NOT_ALLOWED,				/**< GPRS service not allowed */
	TAPI_REJECT_CAUSE_GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED,	/**< GPRS services and Non-GPRS services not allowed */
	TAPI_REJECT_CAUSE_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK,	/**< MS device cannot be derived by the network */
	TAPI_REJECT_CAUSE_IMPLICITLY_DETACHED,				/**< Implicitly detached */
	TAPI_REJECT_CAUSE_PLMN_NOT_ALLOWED,				/**< PLMN not allowed */

	TAPI_REJECT_CAUSE_LA_NOT_ALLOWED,				/**< LA not allowed */
	TAPI_REJECT_CAUSE_NATIONAL_ROAMING_NOT_ALLOWED,			/**< National roaming not allowed */
	TAPI_REJECT_CAUSE_GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN,	/**< GPRS services not allowed in this PLMN */
	TAPI_REJECT_CAUSE_NO_SUITABLE_CELLS_IN_LA,			/**< No suitable cells in the LA */
	TAPI_REJECT_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE,		/**< MSC temporarily not reachable */

	TAPI_REJECT_CAUSE_NETWORK_FAILURE,	/**< Network unavailable */
	TAPI_REJECT_CAUSE_MAC_FAILURE,		/**< MAC failure */
	TAPI_REJECT_CAUSE_SYNCH_FAILURE,	/**< SYNCH failure */
	TAPI_REJECT_CAUSE_CONGESTTION,		/**< Congestion */
	TAPI_REJECT_CAUSE_GSM_AUTH_UNACCEPTED,	/**< GSM Auth unaccepted */

	TAPI_REJECT_CAUSE_SERVICE_OPTION_NOT_SUPPORTED, /**< Service option not supported */
	TAPI_REJECT_CAUSE_REQ_SERV_OPT_NOT_SUBSCRIBED,	/**< REQ_SERV option not suscribed */
	TAPI_REJECT_CAUSE_SERVICE_OPT__OUT_OF_ORDER,	/**< Service OPT out of order */
	TAPI_REJECT_CAUSE_CALL_CANNOT_BE_IDENTIFIED,	/**< Call cannot be identified */
	TAPI_REJECT_CAUSE_NO_PDP_CONTEXT_ACTIVATED,	/**< No PDP context Activated */

	TAPI_REJECT_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_MIN_VALUE,	/**< Retry upon entry into a new call min value */
	TAPI_REJECT_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_MAX_VALUE,	/**< Retry upon entry into a new call max value */
	TAPI_REJECT_CAUSE_SEMANTICALLY_INCORRECT_MSG,			/**< Semantically incorret message */
	TAPI_REJECT_CAUSE_INVALID_MANDATORY_INFO,			/**< Invalid mandatory information */
	TAPI_REJECT_CAUSE_MESSAGE_TYPE_NON_EXISTANT,			/**< Message type non-existant */

	TAPI_REJECT_CAUSE_MESSAGE_TYPE_NOT_COMP_PRT_ST,		/**< Message type not COMP PRT ST */
	TAPI_REJECT_CAUSE_IE_NON_EXISTANT,			/**< IE non existent */
	TAPI_REJECT_CAUSE_MSG_NOT_COMPATIBLE_PROTOCOL_STATE,	/**< MSG not compatible protocol state */

	/* Connection Management establishment rejection cause */
	TAPI_REJECT_CAUSE_REJ_UNSPECIFIED,	/**< REJ unspecified */

	/* AS reject causes */
	TAPI_REJECT_CAUSE_AS_REJ_RR_REL_IND,			/**< RR release indication */
	TAPI_REJECT_CAUSE_AS_REJ_RR_RANDOM_ACCESS_FAILURE,	/**< Random Access Failure */
	TAPI_REJECT_CAUSE_AS_REJ_RRC_REL_IND,			/**< RRC release indication */
	TAPI_REJECT_CAUSE_AS_REJ_RRC_CLOSE_SESSION_IND,		/**< RRC close session indication */
	TAPI_REJECT_CAUSE_AS_REJ_RRC_OPEN_SESSION_FAILURE,	/**< RRC open session failure */
	TAPI_REJECT_CAUSE_AS_REJ_LOW_LEVEL_FAIL,		/**< Low level failure */
	TAPI_REJECT_CAUSE_AS_REJ_LOW_LEVEL_FAIL_REDIAL_NOT_ALLOWED, /**< Low level failure redial not alowed */
	TAPI_REJECT_CAUSE_AS_REJ_LOW_LEVEL_IMMED_RETRY,		/**< Low level immediate retry */

	/* MM reject causes */
	TAPI_REJECT_CAUSE_MM_REJ_INVALID_SIM,		/**< Invalid SIM */
	TAPI_REJECT_CAUSE_MM_REJ_NO_SERVICE,		/**< No service */
	TAPI_REJECT_CAUSE_MM_REJ_TIMER_T3230_EXP,	/**< Timer T3230 expiry */
	TAPI_REJECT_CAUSE_MM_REJ_NO_CELL_AVAILABLE,	/**< No call available */
	TAPI_REJECT_CAUSE_MM_REJ_WRONG_STATE,		/**< Wrong state */
	TAPI_REJECT_CAUSE_MM_REJ_ACCESS_CLASS_BLOCKED,	/**< Access class blocked */

	/* Definitions for release ind causes between MM and CNM */
	TAPI_REJECT_CAUSE_ABORT_MSG_RECEIVED,	/**< Abort Message received */
	TAPI_REJECT_CAUSE_OTHER_CAUSE,		/**< Other cause */

	/* CNM reject causes */
	TAPI_REJECT_CAUSE_CNM_REJ_TIMER_T303_EXP,	/**< Timer T303 expiry */
	TAPI_REJECT_CAUSE_CNM_REJ_NO_RESOURCES,		/**< Rejected due to unavailibilty of resources */
	TAPI_REJECT_CAUSE_CNM_MM_REL_PENDING,		/**< MM release pending */
	TAPI_REJECT_CAUSE_CNM_INVALID_USER_DATA,	/**< Invalid user data */
	TAPI_CALL_END_CAUSE_MAX = 255,			/**< Maximum End Cause limit for GSM/WCDMA */

	/* CDMA CALL END CAUSE */
	TAPI_CDMA_END_CAUSE_REL_BY_USER = 0x1001,	/**< Call Released by User */
	TAPI_CDMA_END_CAUSE_REL_BY_NET,			/**< Call Released by Network */
	TAPI_CDMA_END_CAUSE_REL_NET_BUSY,		/**< Call Released because the network is busy */
	TAPI_CDMA_END_CAUSE_NO_SVC,			/**< Call Released because of No Service area */
	TAPI_CDMA_END_CAUSE_FADING,			/**< Call Released because of Fading */
	TAPI_CDMA_END_CAUSE_RELEASE_BY_REORDER,		/**< Call Released because of reorder */
	TAPI_CDMA_END_CAUSE_RELEASE_BY_INTERCEPT,	/**< Call Released because of intercept */
	TAPI_CDMA_END_CAUSE_SILENT_ZONE_RETRY,		/**< Call Released because of silent zone retry */
	TAPI_CDMA_END_CAUSE_OTA_CALL_FAIL,		/**< Call Released because of OTA call failure */
	TAPI_CDMA_END_CAUSE_PHONE_OFFLINE,		/**< Call Released because phone is offline */
	TAPI_CDMA_END_CAUSE_PHONE_IS_CDMA_LOCKED,	/**< Call Released because CDMA is locked */
	TAPI_CDMA_END_CAUSE_FLASH_IS_IN_PROGRESS_ERR,	/**< Call Released because of the flash-is-in-progress error */
	TAPI_CDMA_END_CAUSE_E911_MODE_ERR,		/**< Call Released because of the e911 mode */
	TAPI_CDMA_END_CAUSE_OTHERS,			/**< Call Released by Others */
	TAPI_CDMA_END_CAUSE_MAX				/**< Maximum End Cause limit for CDMA */
} TelTapiEndCause_t;

/**
 * @brief Enumeration for application usable states. Applications can have their own states as well.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_STATE_IDLE,		/**< Call is in the idle state - i.e. no call */
	TAPI_CALL_STATE_ACTIVE,		/**< Call is in the connected and conversation state */
	TAPI_CALL_STATE_HELD,		/**< Call is in the held state */
	TAPI_CALL_STATE_DIALING,	/**< Call is in the dialing state */
	TAPI_CALL_STATE_ALERT,		/**< Call is in the alerting state */
	TAPI_CALL_STATE_INCOMING,	/**< Call is in the incoming state */
	TAPI_CALL_STATE_WAITING,	/**< Call is in the answered state, and waiting for connected indication event */
	TAPI_CALL_STATE_MAX		/**< Call state unknown */
} TelCallStates_t;

/**
 * @brief Enumeration for call active line(IN GSM ONLY: call identification number).
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ACTIVE_LINE1, /**< Line 1 */
	TAPI_CALL_ACTIVE_LINE2	/**< Line 2 */
} TelCallActiveLine_t;

/**
 * @brief Enumeration for the answer type used as a parameter in the answer call API.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ANSWER_ACCEPT = 0,		/**< Used to answer an incoming call when there are no current active calls */
	TAPI_CALL_ANSWER_REJECT,		/**< Used to reject the incoming call */
	TAPI_CALL_ANSWER_REPLACE,		/**< Release current active call and accept the waiting call */
	TAPI_CALL_ANSWER_HOLD_AND_ACCEPT,	/**< Hold the current active call, and accept the waiting call */
} TelCallAnswerType_t;

/**
 * @brief Enumeration for the end type used as a parameter in the end call API.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_END = 0,	/**< End Specific Call */
	TAPI_CALL_END_ALL,	/**< End All Calls */
	TAPI_CALL_END_ACTIVE_ALL,	/**< End All Active Calls */
	TAPI_CALL_END_HOLD_ALL,	/**< End All Held Calls */
} TelCallEndType_t;

/**
 * @brief Enumeration for call type to be used by applications while using the setup call API. Like the requested call is a voice call or video call.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_TYPE_VOICE,	/**< Voice call type */
	TAPI_CALL_TYPE_DATA,	/**< Data call type - (for modem, fax, packet, and other such calls) */
	TAPI_CALL_TYPE_E911	/**< Emergency call type */
} TelCallType_t;

/**
 * @brief Enumeration for the call name mode.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_NAME_AVAIL = 0,	/**< This identifier refers to presenting the calling party's name identity to the called party */
	TAPI_CALL_NAME_RESTRICTED = 1,	/**< This identifier refers to restricting the name identity of the calling party from being presented to the called party */
	TAPI_CALL_NAME_UNAVAIL = 2,	/**< This identifier refers to the unavailability of the calling party's name identity from being offered to the called party */
	TAPI_CALL_NAME_AVAIL_RESTRICTED = 3	/**< This identifier refers to offering the calling party's name identity to the called party with which the presentation restriction is overridden */
} TelCallNameMode_t;

/**
 * @brief Enumeration for the "No Cli cause" value.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_NO_CLI_CAUSE_NONE = -1,		/**< None */
	TAPI_CALL_NO_CLI_CAUSE_UNAVAILABLE = 0x00,	/**< Unavailable */
	TAPI_CALL_NO_CLI_CAUSE_REJECTBY_USER = 0x01,	/**< Rejected by User */
	TAPI_CALL_NO_CLI_CAUSE_INTERACTION_OTHERSERVICES = 0x02,	/**< Other services */
	TAPI_CALL_NO_CLI_CAUSE_COINLINE_PAYPHONE = 0x03,		/**< Coin line phone */
} TelCallNoCliCause_t;

/**
 * @brief Enumeration for the "Cli mode" value.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_PRES_AVAIL,		/**< Presentation Allowed */
	TAPI_CALL_PRES_RESTRICTED,	/**< Presentation Restricted */
	TAPI_CALL_NUM_UNAVAIL,		/**< Number Not Available */
	TAPI_CALL_PRES_DEFAULT,		/**< Presentation Default */
} TelCallCliMode_t;

/**
 * @brief Enumeration for the the fwded ind type used for MO and Mt from SS Noti Info.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_OUTGOING_FWDED = 0x00,	/**< Outgoing call forwarded */
	TAPI_CALL_INCOMMING_FWDED = 0x01,	/**< Incoming call forwarded */
} TelCallForwardType_t;

/**
 * @brief Enumeration for the voice privacy option mode. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_PRIVACY_MODE_STANDARD = 0x00,	/**< Standard mode */
	TAPI_CALL_PRIVACY_MODE_ENHANCED		/**< Enhanced mode */
} TelCallPrivacyMode_t;

/**
 * @brief Enumeration for the OTASP Status. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_OTASP_OK_SPL_UNLOCKED = 0x01, /**< OTASP : SPL unlocked ok */
	TAPI_CALL_OTASP_OK_AKEY_EXCESS,		/**< OTASP : A-Key excess ok */
	TAPI_CALL_OTASP_OK_SSD_UPDATE,		/**< OTASP : SSD update ok */
	TAPI_CALL_OTASP_OK_NAM_DWNLD,		/**< OTASP : NAM download ok */
	TAPI_CALL_OTASP_OK_MDN_DWNLD,		/**< OTASP : MDN download ok */
	TAPI_CALL_OTASP_OK_IMSI_DWNLD,		/**< OTASP : IMSI download ok */
	TAPI_CALL_OTASP_OK_PRL_DWNLD,		/**< OTASP : PRL download ok */
	TAPI_CALL_OTASP_OK_COMMIT,		/**< OTASP : commit ok */
	TAPI_CALL_OTASP_OK_PROGRAMMING,		/**< OTASP : programming ok */
	TAPI_CALL_OTASP_SUCCESS,		/**< OTASP : success */
	TAPI_CALL_OTASP_UNSUCCESS,		/**< OTASP : unsuccess */
	TAPI_CALL_OTASP_OK_OTAPA_VERIFY,	/**< OTASP : verify ok */
	TAPI_CALL_OTASP_PROGRESS,		/**< OTASP : progress */
	TAPI_CALL_OTASP_FAILURES_EXCESS_SPC,	/**< OTASP : SPC excess failure */
	TAPI_CALL_OTASP_LOCK_CODE_PW_SET,	/**< OTASP : lock code password set */
} TelCallOtaspStatus_t;

/**
 * @brief Enumeration for the OTAPA Status. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_OTAPA_STOP = 0x00,	/**< OTAPA : stop */
	TAPI_CALL_OTAPA_START,		/**< OTAPA : start */
} TelCallOtapaStatus_t;

/**
 * @brief Enumeration for the Alert Signal Type. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_SIGNAL_TYPE_TONE = 0x00, /**< Tone */
	TAPI_CALL_SIGNAL_TYPE_ISDN_ALERTING, /**< ISDN Alerting */
	TAPI_CALL_SIGNAL_TYPE_IS54B_ALERTING, /**< IS54B Alerting */
	TAPI_CALL_SIGNAL_TYPE_RESERVED /** Reserved */
} TelCallAlertSignalType_t;

/**
 * @brief Enumeration for the Alert Pitch Type. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ALERT_PITCH_MED = 0x00, /**< Alert Pitch Med */
	TAPI_CALL_ALERT_PITCH_HIGH, /**< Alert Pitch High */
	TAPI_CALL_ALERT_PITCH_LOW, /**< Alert Pitch Low */
	TAPI_CALL_ALERT_PITCH_RESERVED /**< Reserved */
} TelCallAlertPitchType_t;

/**
 * @brief Enumeration for the signals specific to the alert signal type TAPI_CALL_SIGNAL_TYPE_TONE. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_SIGNAL_TONE_DIAL = 0x00, /**< Dial */
	TAPI_CALL_SIGNAL_TONE_RINGBACK_TONE_ON, /**< Ringback Tone ON */
	TAPI_CALL_SIGNAL_TONE_INTERCEPT_TONE_ON, /**< Intercept Tone ON */
	TAPI_CALL_SIGNAL_TONE_ABBREV_TONE, /**< Abbreviation Tone */
	TAPI_CALL_SIGNAL_TONE_NETWORK_CONGESTION_TONE_ON, /**< Network Congestion Tone ON */
	TAPI_CALL_SIGNAL_TONE_ABBREV_NETWORK_CONGESTION, /**< Abbreviation Network Congestion */
	TAPI_CALL_SIGNAL_TONE_BUSY_TONE_ON, /**< Busy Tone ON */
	TAPI_CALL_SIGNAL_TONE_CFRM_TONE_ON, /**< CFRM Tone ON */
	TAPI_CALL_SIGNAL_TONE_ANSWER_TONE_ON, /**< Answer Tone ON */
	TAPI_CALL_SIGNAL_TONE_CALL_WAITING_TONE_ON, /**< Call Waiting Tone ON */
	TAPI_CALL_SINGNAL_TONE_PIPE_TONE_ON, /**< Pipe Tone ON */
	TAPI_CALL_SIGNAL_TONE_OFF /**< Tone OFF */
} TelCallToneSignal_t;

/**
 * @brief Enumeration for the signals specific to the alert signal type TAPI_CALL_SIGNAL_TYPE_ISDN_ALERTING. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_SIGNAL_ISDN_ALERT_NORMAL = 0x00, /**< Normal */
	TAPI_CALL_SIGNAL_ISDN_ALERT_INTER_GROUP, /**< Inter group */
	TAPI_CALL_SIGNAL_ISDN_ALERT_SPECIAL_PRIORITY, /**< Special Priority */
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED1, /**< ISDN Reserved1 */
	TAPI_CALL_SIGNAL_ISDN_ALERT_PING_RING, /**< Ping Ring */
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED2, /**< ISDN Reserved2 */
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED3, /**< ISDN Reserved3 */
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED4, /**< ISDN Reserved4 */
	TAPI_CALL_SIGNAL_ISDN_ALERT_OFF /**< Alert OFF */
} TelCallIsdnAlertSignal_t;

/**
 * @brief Enumeration for the signals specific to the alert signal type TAPI_CALL_SIGNAL_TYPE_IS54B_ALERTING. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_SIGNAL_IS54B_ALERT_NOTONE = 0x00, /**< No Tone */
	TAPI_CALL_SIGNAL_IS54B_ALERT_LONG, /**< Long */
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT, /**< Short Short */
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT_LONG, /**< Short Short Long */
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT_2, /**< Short Short 2*/
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_LONG_SHORT, /**< Short Long Short */
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT_SHORT_SHORT, /**< Short Short Short Short */
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_LONG, /**< PBX Long */
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_SHORT, /**< PBX(Private Branch Exchange) Short Short */
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_SHORT_LONG, /**< PBX Short Short Long */
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_LONG_SHORT, /**< PBX Short Long Short */
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_SHORT_SHORT_SHORT, /**< PBX Short Short Short Short */
	TAPI_CALL_SIGNAL_IS54B_ALERT_PIP_PIP_PIP_PIP /**< PIP PIP PIP PIP */
} TelCallIs54bAlertSignal_t;

/**
 * @brief Enumeration for the emergency category type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ECC_DEFAULT = 0x00,		/**< Default case */
	TAPI_CALL_ECC_POLICE = 0x01,		/**< Police emergency */
	TAPI_CALL_ECC_AMBULANCE = 0x02,		/**< Ambulance emergency */
	TAPI_CALL_ECC_FIREBRIGADE = 0x04,	/**< Firebrigade emergency */
	TAPI_CALL_ECC_MARINEGUARD = 0x08,	/**< Marineguard emergency */
	TAPI_CALL_ECC_MOUNTAINRESCUE = 0x10,	/**< Mountain rescue emergency */
	TAPI_CALL_ECC_MANUAL_ECALL = 0x20,	/**< Manual emergency call */
	TAPI_CALL_ECC_AUTO_ECALL = 0x40,	/**< Automatic emergency call */
	TAPI_CALL_ECC_NONE = 0xff,		/**< Unspecified emergency */
} TelCallEmergencyCategory_t;

/**
 * @brief Enumeration for the offlength to send DTMF.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_DTMF_OFFLENGTH_60MS, /**< 60 ms */
	TAPI_CALL_DTMF_OFFLENGTH_100MS, /**< 100 ms */
	TAPI_CALL_DTMF_OFFLENGTH_150MS, /**< 150 ms */
	TAPI_CALL_DTMF_OFFLENGTH_200MS, /**< 200 ms */
} TelCallDtmfInterDigitInterval_t;

/**
 * @brief Enumeration for the onlength to send DTMF.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_DTMF_ONLENGTH_95MS, /**< 95 ms */
	TAPI_CALL_DTMF_ONLENGTH_150MS, /**< 150 ms */
	TAPI_CALL_DTMF_ONLENGTH_200MS, /**< 200 ms */
	TAPI_CALL_DTMF_ONLENGTH_250MS, /**< 250 ms */
	TAPI_CALL_DTMF_ONLENGTH_300MS, /**< 300 ms */
	TAPI_CALL_DTMF_ONLENGTH_350MS, /**< 350 ms */
	TAPI_CALL_DTMF_ONLENGTH_SMS /**< SMS */
} TelCallDtmfPulseWidth_t;

/**
 * @brief Enumeration for the Call Record Info Type
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_REC_INFO_TYPE_NAME, /**< Name Type */
	TAPI_CALL_REC_INFO_TYPE_NUMBER, /**< Number Type */
} TelCallRecordInfoType_t;

/**
 * @brief Enumeration for call preferred voice subscription.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_PREFERRED_VOICE_SUBS_UNKNOWN = -1, /**< Unknown status **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_CURRENT_NETWORK = 0, /**< Current network **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_ASK_ALWAYS, /**< ASK Always **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_SIM1, /**< SIM 1 **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_SIM2 /**< SIM 2 **/
} TelCallPreferredVoiceSubs_t;

/**
 * @brief The structure type to redirect destination information.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char number[TAPI_CALL_DIAL_NUMBER_LEN_MAX + 1]; /**< Redirect destination number */
} TelCallDeflectDstInfo_t;

/**
 * @brief The structure type for calling name information.
 * @since_tizen 2.3
 */
typedef struct {
	TelCallNameMode_t NameMode;	/**< Display mode of @a szNameData */
	char szNameData[TAPI_CALLING_NAME_SIZE_MAX];	/**< Calling party name string */
} TelCallingNameInfo_t;

/**
 * @brief The structure type for information about an outgoing call.
 * @since_tizen 2.3
 */
typedef struct {
	TelCallType_t CallType;		/**< Sets type of call (voice, data, video, emergency) */
	TelCallEmergencyCategory_t Ecc; /**< Emergency Category */
	char szNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1]; /**< A string containing the destination phone number. This follows the dial number format */
} TelCallDial_t;

/**
 * @brief The structure type for information about call status.
 * @since_tizen 2.3
 */
typedef struct {
	int CallHandle; /**< Call Handle */
	int bMoCall;	/**< Status will be yes for MO Calls. If MT call, then FALSE */
	char pNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1];	/**< Mobile Number */
	TelCallType_t CallType;	/**< Type of call (voice, data, emergency) */
	TelCallStates_t CallState;	/**< Current Call state */
	int bConferenceState;	/**< Whether Call is in Conference */
} TelCallStatus_t;

/**
 * @brief Called to get the call status.
 * @since_tizen 2.3
 */
typedef void (*TelCallStatusCallback)(TelCallStatus_t *out, void *user_data);

/**
 * @brief The structure type for incoming call information.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int CallHandle; /**< At the Foundation API Level, call handle indicates the handle of the call for the application. At the Framework API Level, call handle indicates the call identifier used for a call by the OEM Adaptation Layer Plug-in */
	TelCallType_t CallType; /**< Call type */
	char szCallingPartyNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1]; /**< Caller number, null terminated ASCII */
	TelCallingNameInfo_t CallingNameInfo; /**< Call name info. If there is no information from the network, this information will be @c NULL */
	TelCallCliMode_t CliMode; /**< CLI mode */
	TelCallNoCliCause_t CliCause; /**< No CLI cause */
	int fwded; /**< True or false. If the incoming call is a forwarded call, then @c true else @c false */
	TelCallActiveLine_t ActiveLine; /**< Current Active Line */
} TelCallIncomingCallInfo_t;

/**
 * @brief The structure type for the connected number information.
 * @since_tizen 2.3
 */
typedef struct {
	TelCallCliMode_t CliMode;	/**< CLI mode */
	TelCallNoCliCause_t no_cli_cause;	/**< Cause when no CLI number */
	unsigned char dcs;		/**< DCS */
	unsigned char number_type;	/**< Number type */
	TelCallNameMode_t name_mode;	/**< Display mode of the name */
	char number[TAPI_CALL_DIALDIGIT_LEN_MAX + 1];		/**< Caller number, null terminated ASCII */
	char sub_address[TAPI_CALL_SUBADDRESS_LEN_MAX + 1];	/**< Called sub-address, null terminated ASCII */
} TelCallConnectedNumberInfo_t;

/**
 * @brief The structure type for information about display record in CDMA.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char Record[TAPI_CALL_DISPLAY_RECORD_MAX + 1]; /**< Display record */
} TelCallDisplayRecordInfo_t;

/**
 * @brief The structure type for information about burst DTMF in CDMA.
 * @since_tizen 2.3
 */
typedef struct {
	char dtmf_string[TAPI_CALL_BURST_DTMF_STRING_MAX + 1]; /** Burst DTMF string NULL terminated */
	TelCallDtmfPulseWidth_t pulse_width; /**< DTMF Pulse Width */
	TelCallDtmfInterDigitInterval_t inter_digit_interval; /**< DTMF Inter Digit Interval */
} TelCallBurstDtmf_t;

/**
 * @brief Structure type for the information about records in CDMA.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< Call id */
	TelCallRecordInfoType_t type;
	union {
		char name[TAPI_CALLING_NAME_SIZE_MAX + 1]; /**< name record */
		char number[TAPI_CALL_DIAL_NUMBER_LEN_MAX + 1]; /**< number record */
	} data;
} TelCallRecordInfo_t;

/**
 * @brief Enumeration for call sound volume level.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_MUTE = 0x00,		/**< Sound is mute */
	TAPI_SOUND_VOLUME_LEVEL_1 = 0x01,	/**< Volume level is 1 */
	TAPI_SOUND_VOLUME_LEVEL_2 = 0x02,	/**< Volume level is 2 */
	TAPI_SOUND_VOLUME_LEVEL_3 = 0x03,	/**< Volume level is 3 */
	TAPI_SOUND_VOLUME_LEVEL_4 = 0x04,	/**< Volume level is 4 */
	TAPI_SOUND_VOLUME_LEVEL_5 = 0x05,	/**< Volume level is 5 */
	TAPI_SOUND_VOLUME_LEVEL_6 = 0x06,	/**< Volume level is 6 */
	TAPI_SOUND_VOLUME_LEVEL_7 = 0x07,	/**< Volume level is 7 */
	TAPI_SOUND_VOLUME_LEVEL_8 = 0x08,	/**< Volume level is 8 */
	TAPI_SOUND_VOLUME_LEVEL_9 = 0x09	/**< Volume level is 9 */
} TelSoundVolume_t;

/**
 * @brief Enumeration for call sound device type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_DEVICE_RECEIVER = 0x00,	/**< Device type Receiver */
	TAPI_SOUND_DEVICE_SPEAKER_PHONE = 0x10, /**< Device type Speaker */
	TAPI_SOUND_DEVICE_HFK = 0x20,		/**< Device type Handsfree */
	TAPI_SOUND_DEVICE_HEADSET = 0x30,	/**< Device type HeadSet */
	TAPI_SOUND_DEVICE_BLUETOOTH = 0x40,	/**< Device type Bluetooth */
	TAPI_SOUND_DEVICE_EC = 0xA0,		/**< Device type external*/
} TelSoundDevice_t;

/**
 * @brief Enumeration for call sound type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_TYPE_VOICE = 0x01,	/**< Call Sound type voice */
	TAPI_SOUND_TYPE_KEYTONE = 0x02,	/**< Call Sound type keytone */
	TAPI_SOUND_TYPE_BELL = 0x03,	/**< Call Sound type bell */
	TAPI_SOUND_TYPE_MESSAGE = 0x04,	/**< Call Sound type message */
	TAPI_SOUND_TYPE_ALARM = 0x05,	/**< Call Sound type alarm */
	TAPI_SOUND_TYPE_PDA_MISC = 0x06, /**< Call Sound type PDA miscellaneous */
} TelSoundType_t;

/**
 * @brief The structure type for call volume info.
 * @since_tizen 2.3
 */
typedef struct {
	TelSoundDevice_t device;	/**< Call Sound device info structure */
	TelSoundType_t type;		/**< Call Sound type info structure */
	TelSoundVolume_t volume;	/**< Call Sound volume info structure */
} TelCallVolumeInfo_t;

/**
 * @brief Enumeration for call sound path.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_PATH_HANDSET = 0x01,		/**< Audio path is handset */
	TAPI_SOUND_PATH_HEADSET = 0x02,		/**< Audio path is handset */
	TAPI_SOUND_PATH_HANDSFREE = 0x03,	/**< Audio path is Handsfree */
	TAPI_SOUND_PATH_BLUETOOTH = 0x04,	/**< Audio path is bluetooth */
	TAPI_SOUND_PATH_STEREO_BLUETOOTH = 0x05,	/**< Audio path is stereo bluetooth */
	TAPI_SOUND_PATH_SPK_PHONE = 0x06,	/**< Audio path is speaker phone */
	TAPI_SOUND_PATH_HEADSET_3_5PI = 0x07,	/**< Audio path is headset_3_5PI */
	TAPI_SOUND_PATH_BT_NSEC_OFF = 0x08,	/**< Audio path Bluetooth NSEC is off */
	TAPI_SOUND_PATH_MIC1 = 0x09,	/**< Audio path Mic one */
	TAPI_SOUND_PATH_MIC2 = 0x0A,	/**< Audio path Bluetooth Mic two */
	TAPI_SOUND_PATH_HEADSET_HAC = 0x0B,	/**< Audio path is headset HAC */
} TelSoundPath_t;

/**
 * @brief The structure type for call sound path.
 * @since_tizen 2.3
 */
typedef struct {
	TelSoundPath_t path;	/**< Sound Path */
	enum ExtraVolumeStatus {
		TAPI_SOUND_EX_VOLUME_OFF,	/**< Extra Volume OFF */
		TAPI_SOUND_EX_VOLUME_ON,	/**< Extra volume ON */
	} ex_volume;
} TelCallSoundPathInfo_t;

/**
 * @brief Enumeration for call sound mute path.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_MUTE_PATH_TX = 0x00, /**< Sound mute path transmit */
	TAPI_SOUND_MUTE_PATH_RX = 0x02, /**< Sound mute path receiver */
	TAPI_SOUND_MUTE_PATH_ALL = 0x04, /**< Sound mute path all */
} TelSoundMutePath_t;

/**
 * @brief Enumeration for call sound mute status.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_MUTE_STATUS_OFF = 0x00, /**< Sound mute status off */
	TAPI_SOUND_MUTE_STATUS_ON = 0x01, /**< Sound mute status on */
} TelSoundMuteStatus_t;

/**
 * @brief Enumeration for call sound noise reduction.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_NOISE_REDUCTION_OFF, /**< Sound noise reduction off */
	TAPI_SOUND_NOISE_REDUCTION_ON, /**< Sound noise reduction on */
} TelSoundNoiseReduction_t;

/**
 * @brief The structure type for call answer response data.
 * @since_tizen 2.3
 * @see tel_answer_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallAnswerCnf_t;

/**
 * @brief The structure type for call end response data.
 * @since_tizen 2.3
 * @see tel_end_call()
 */
typedef struct {
	TelCallEndType_t type;	/**< End Type */
	unsigned int id;	/**< ID */
} TelCallEndCnf_t;

/**
 * @brief The structure type for call hold response data.
 * @since_tizen 2.3
 * @see tel_hold_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallHoldCnf_t;

/**
 * @brief The structure type for call active response data.
 * @since_tizen 2.3
 * @see tel_active_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallActiveCnf_t;

/**
 * @brief The structure type for call swap response data.
 * @since_tizen 2.3
 * @see tel_swap_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallSwapCnf_t;

/**
 * @brief The structure type for call join response data.
 * @since_tizen 2.3
 * @see tel_join_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallJoinCnf_t;

/**
 * @brief The structure type for call split response data.
 * @since_tizen 2.3
 * @see tel_split_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallSplitCnf_t;

/**
 * @brief The structure type for call transfer response data.
 * @since_tizen 2.3
 * @see tel_transfer_call()
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallTransferCnf_t;

/**
 * @brief The structure type for call volume info response data.
 * @since_tizen 2.3
 * @see tel_get_call_volume_info()
 */
typedef struct {
	unsigned int record_num;	/**< Record Number */
	TelCallVolumeInfo_t record[20];	/**< Volume Info Record */
} TelCallGetVolumeInfoResp_t;

/**
 * @brief The structure type for call mute status response data.
 * @since_tizen 2.3
 * @see tel_get_call_mute_status()
 */
typedef struct {
	TelSoundMutePath_t path;	/**< Mute Path */
	TelSoundMuteStatus_t status;	/**< Mute Status */
} TelCallGetMuteStatusResp_t;

/**
 * @brief The structure type for call privacy mode response data.
 * @since_tizen 2.3
 * @see tel_get_call_privacy_mode()
 */
typedef struct {
	TelCallPrivacyMode_t mode; /**< Privacy Mode */
} TelCallGetPrivacyModeResp_t;

/**
 * @brief The structure type for call idle status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
	TelTapiEndCause_t cause; /**< End cause for the call indicates whether the call is released normally or due to some other cause */
} TelCallStatusIdleNoti_t;

/**
 * @brief The structure type for call active status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallStatusActiveNoti_t;

/**
 * @brief The structure type for call held status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallStatusHeldNoti_t;

/**
 * @brief The structure type for call dialing status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallStatusDialingNoti_t;

/**
 * @brief The structure type for call alert status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallStatusAlertNoti_t;

/**
 * @brief The structure type for call incoming status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallStatusIncomingNoti_t;

/**
 * @brief The structure type for call waiting status notification data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallStatusWaitingNoti_t;

/**
 * @brief The structure type for call waiting info notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_WAITING
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoWaitingNoti_t;

/**
 * @brief The structure type for call forward info notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_FORWARDED
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoForwardedNoti_t;

/**
 * @brief The structure type for call forwarded info notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_FORWARDED_CALL
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoForwardedCallNoti_t;

/**
 * @brief The structure type for call barred incoming notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_BARRED_INCOMING
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoBarredIncomingNoti_t;

/**
 * @brief The structure type for call barred outgoing notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_BARRED_OUTGOING
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoBarredOutgoingNoti_t;

/**
 * @brief The structure type for call forward conditional notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_FORWARD_CONDITIONAL
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoForwardConditionalNoti_t;

/**
 * @brief The structure type for call forward unconditional notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_FORWARD_UNCONDITIONAL
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoForwardUnconditionalNoti_t;

/**
 * @brief The structure type for call active info notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_ACTIVE
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoActiveNoti_t;

/**
 * @brief The structure type for call held info notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_HELD
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoHeldNoti_t;

/**
 * @brief The structure type for call joined info notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_JOINED
 */
typedef struct {
	unsigned int id;	/**< ID */
} TelCallInfoJoinedNoti_t;

/**
 * @brief The structure type for OTASP status notification data. (CDMA only)
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_OTASP_STATUS
 */
typedef struct {
	TelCallOtaspStatus_t otasp_status; /**< OTASP Status */
} TelCallOtaspStatusNoti_t;

/**
 * @brief The structure type for OTAPA status notification data. (CDMA only)
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_OTAPA_STATUS
 */
typedef struct {
	TelCallOtapaStatus_t otapa_status; /**< OTAPA Status */
} TelCallOtapaStatusNoti_t;

/**
 * @brief The structure type for voice privacy notification data. (CDMA only)
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_PRIVACY_MODE
 */
typedef struct {
	TelCallPrivacyMode_t privacy_mode; /**< Voice Privacy Mode */
} TelCallVoicePrivacyNoti_t;

/**
 * @brief The structure type for call record info notification data. (CDMA only)
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_INFO_REC
 */
typedef struct {
	TelCallRecordInfo_t info; /**< Record Info */
} TelCallRecordInfoNoti_t;

/**
 * @brief The structure type for signal info notification data. (CDMA only)
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_SIGNAL_INFO
 */
typedef struct {
	TelCallAlertSignalType_t signal_type; /**< Signal Type */
	TelCallAlertPitchType_t pitch_type; /**< Pitch Type */
	union {
		TelCallToneSignal_t sig_tone_type; /**< Signal Tone Type */
		TelCallIsdnAlertSignal_t sig_isdn_alert_type; /**< Signal ISDN Alert Type */
		TelCallIs54bAlertSignal_t sig_is54b_alert_type; /**< Signal IS54B Alert Type */
	} signal;
} TelCallSignalInfoNoti_t;

/**
 * @brief The structure type for call sound path notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_SOUND_PATH
 */
typedef struct {
	TelSoundPath_t path;	/**< Sound Path */
} TelCallSoundPathNoti_t;

/**
 * @brief Enumeration for call sound ringback tone notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_SOUND_RINGBACK_TONE
 */
typedef enum {
	TAPI_CALL_SOUND_RINGBACK_TONE_END,	/**< Ringback Tone End */
	TAPI_CALL_SOUND_RINGBACK_TONE_START,	/**< Ringback Tone Start */
} TelCallSoundRingbackToneNoti_t;

/**
 * @brief Enumeration for call sound WBAMR notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_SOUND_WBAMR
 */
typedef enum {
	TAPI_CALL_SOUND_WBAMR_STATUS_OFF,	/**< Status OFF */
	TAPI_CALL_SOUND_WBAMR_STATUS_ON,	/**< Status ON */
	TAPI_CALL_SOUND_WBAMR_STATUS_OFF_16K,	/**< Status OFF 16k */
	TAPI_CALL_SOUND_WBAMR_STATUS_ON_8K,	/**< Status ON 8k */
} TelCallSoundWbamrNoti_t;

/**
 * @brief The structure type for call sound noise reduction notification data.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_SOUND_NOISE_REDUCTION
 */
typedef struct {
	TelSoundNoiseReduction_t status;	/**< Noise Reduction Status */
} TelCallSoundNoiseReductionNoti_t;

/**
 * @brief The structure type for preferred voice subscription.
 * @since_tizen 2.3
 * @see #TAPI_NOTI_CALL_PREFERRED_VOICE_SUBSCRIPTION
 */
typedef struct {
	TelCallPreferredVoiceSubs_t preferred_subs; /**< Preferred Voice Subscription */
} TelCallPreferredVoiceSubsNoti_t;

#ifdef __cplusplus
}
#endif

#endif /* _TEL_CALL_H_ */

/**
 * @}
 */
