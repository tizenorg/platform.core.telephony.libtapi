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
#define TAPI_CALLING_NAME_SIZE_MAX      80

/**
 * @brief Definition for the maximum length of the user to user signalling data string.
 * @since_tizen 2.3
 */
#define TAPI_CALL_UUS_DATA_LEN_MAX      131

/**
 * @brief Definition for the maximum length of the string for display record in CDMA.
 * @since_tizen 2.3
 */
#define TAPI_CALL_DISPLAY_RECORD_MAX    32

/**
 * @brief Definition for the maximum number of calls possible at a time.
 * @since_tizen 2.3
 */
#define MAX_TAPI_CALL_COUNT				6

/**
 * @brief Definition for the maximum number of calls possible in a conference call at a time.
 * @since_tizen 2.3
 */
#define MAX_TAPI_CONF_CALL_COUNT		5

/**
 * @brief Definition for the invalid call handle.
 * @details TS_UINT is used for the call handle.
 * @since_tizen 2.3
 */
#define INVALID_CALL_HANDLE				0

/* Call */

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
	TAPI_CAUSE_SUCCESS,				    /**< Success */
	TAPI_CAUSE_ACCESSDISCARD,			/**< Access discarded */
	TAPI_CAUSE_BARR_BYOPERATOR,		    /**< Barred by operator */
	TAPI_CAUSE_BEARERMODE_NOTIMPL,		/**< Bearer mode not implemented */
	TAPI_CAUSE_BEARERMODE_RESTRICTED,	/**< Bearer mode restricted */
	TAPI_CAUSE_BEARERMODE_UNAUTH,		/**< Bearer mode unauthorized */
	TAPI_CAUSE_BEARERMODE_UNAVAIL,		/**< Bearer mode not available */
	TAPI_CAUSE_BUSY,					/**< (Network/Server) busy */
	TAPI_CAUSE_CALLMETER_EXPIRED,		/**< Call meter expired */
	TAPI_CAUSE_CALLNO_ERROR,			/**< Call number error */
	TAPI_CAUSE_CKTUNAVAIL,				/**< Circuit channel unavailable */			//10
	TAPI_CAUSE_CONGESTION,				/**< Congestion occurred */
	TAPI_CAUSE_NO_CIRCUIT_AVAIL,		/**< Circuit not available */
	TAPI_CAUSE_DESTIN_INCOMPAT,		    /**< Destination incompatibility */
	TAPI_CAUSE_DTMF_NOSPEECH,			/**< No speech in DTMF */
	TAPI_CAUSE_DTMF_REJECTED,			/**< DTMF rejected */
	TAPI_CAUSE_FACILITY_NOTIMPL,		/**< Facility not implemented */
	TAPI_CAUSE_FACILITY_NOTSUBSCRIBED,	/**< Facility not subscribed */
	TAPI_CAUSE_INCOMINGCUGCALLS_BARRED, /**< Incoming CUG Calls barred */
	TAPI_CAUSE_INVALNUM,				/**< Invalid number */
	TAPI_CAUSE_MPTY_ERROR,				/**< Multiparty error */						//20
	TAPI_CAUSE_NOANSWER,				/**< No answer */
	TAPI_CAUSE_NONCUGMEMBER,			/**< Non CUG member */
	TAPI_CAUSE_NUMBERCHANGED,			/**< Number changed */
	TAPI_CAUSE_NUMBER_ERROR,			/**< Number error */
	TAPI_CAUSE_NWFAILURE,				/**< Network failure */
	TAPI_CAUSE_OPERATIONUNAVAIL,		/**< Operation not available */
	TAPI_CAUSE_OUTOFORDER,				/**< Out of order */
	TAPI_CAUSE_PROTOCOL_ERROR,			/**< Protocol error */
	TAPI_CAUSE_QOSUNAVAIL,				/**< QOS unavailable */
	TAPI_CAUSE_REJECT,					/**< Rejected */								//30
	TAPI_CAUSE_REJ_FAIL,				/**< Rejection failed */
	TAPI_CAUSE_REJ_SRVC_NOT_AVL,		/**< Rejection service not available */
	TAPI_CAUSE_REJ_SAT_CALL_CTRL,		/**< Rejection SAT Call Control */
	TAPI_CAUSE_REMOTE_CKTUNAVAIL,		/**< Remote Circuit channel unavailable */
	TAPI_CAUSE_RESOURCEUNAVAIL,		    /**< Resource not available */
	TAPI_CAUSE_SERVICEID_ERROR,		    /**< Service ID error */
	TAPI_CAUSE_SERVICE_NOTIMPL,		    /**< Service not implemented */
	TAPI_CAUSE_SERVICE_UNAVAIL,		    /**< Service not available */
	TAPI_CAUSE_MODIFY_SAME_BEARER,		/**< Modify same bearer */
	TAPI_CAUSE_MODIFY_NOT_PERMITTED,	/**< Modification not permitted */
	TAPI_CAUSE_HOLD_FAIL,				/**< Call hold failure */							//40
	TAPI_CAUSE_TEMPFAILURE,			    /**< Temporary failure */
	TAPI_CAUSE_WRONGNUM,				/**< Wrong number */
	TAPI_CAUSE_NORMAL_UNSPECIFIED,		/**< Normal but unspecified */
	TAPI_CAUSE_SSERRINVALIDTIVALUE,	    /**< SS invalid transaction ID value */
	TAPI_CAUSE_SSERRINVALIDTI,			/**< SS Invalid transaction ID */
	TAPI_CAUSE_SSINCOMPATIBLESTATE,	    /**< SS incompatible state */
	TAPI_CAUSE_SSERRSYNTAXERROR,		/**< SS syntax error */
	TAPI_CAUSE_SSPROTOCOLERROR,		    /**< Protocol error */
	TAPI_CAUSE_SSNEGATIVEPASSWORDCHECK, /**< Negative password check */
	TAPI_CAUSE_SSSENTASPHASE1,			/**< SS sent as phase1 message */				//50
	TAPI_CAUSE_SSERROR,				    /**< Supplementary service error */
	TAPI_CAUSE_SS_USSD_BUSY,			/**< Second USSD request will be rejected when a USSD transaction is already in progess */
	TAPI_CAUSE_IDLE,					/**< Idle */
	TAPI_CAUSE_NETWORK_SPECIFIC,		/**< Network specific error */
	TAPI_CAUSE_FADE,					/**< Voice call is dropped e.g. because of a loss of signal */
	TAPI_CAUSE_UNKNOWN,				    /**< Unknown and no details available */
	TAPI_CAUSE_INCOM_CALL,				/**< Terminated by an incoming call */
	TAPI_CAUSE_ALERT_STOP,				/**< Terminated by an alert stop */
	TAPI_CAUSE_INTERCEPT,				/**< Voice call is dropped by interception */
	TAPI_CAUSE_REORDER,				    /**< Voice call is dropped by reordering */	//60
	TAPI_CAUSE_CLIENT_END,				/**< Client ended the call */
	TAPI_CAUSE_ACCESS_CLASS_BLOCKED,	/**< Access class blocked */
	TAPI_CAUSE_MEMCAPEXCEEDED,			/**< Memory capacity exceeded */
	TAPI_CAUSE_TYPENOTSUPPORTED,		/**< Type not supported */
	TAPI_CAUSE_REPLACENOTSUPPORTED,	    /**< Replace not supported */
	TAPI_CAUSE_PROTOCOLID_ERROR,		/**< Protocol ID error */
	TAPI_CAUSE_CLASSNOTSUPPORTED,		/**< Class not supported */
	TAPI_CAUSE_DATACODING_ERROR,		/**< Data coding error */
	TAPI_CAUSE_INVAL_MSG,				/**< Invalid message */
	TAPI_CAUSE_CALL_BARRED,			    /**< Call barred */							//70
	TAPI_CAUSE_SIM_CALL_CONTROL_CHANGED_TO_SS,		/**< Sim call control changed to SS */
	TAPI_CAUSE_SIM_CALL_CONTROL_CHANGED_TO_CALL,	/**< SIM Call control changed to call */
	TAPI_CAUSE_SIM_ERROR,				            /**< SIM error */
	TAPI_CAUSE_SIM_CALL_CONTROL_MODIFIED,			/**< SIM call control modified */
	TAPI_CAUSE_RANDOM_ACCESS_FAILURE,	            /**< Random access error */
	TAPI_CAUSE_LOWER_LAYER_FAILURE,	                /**< Lower layer error */
	TAPI_CAUSE_ACCESS_REJECTED,		                /**< Access rejected */
	TAPI_CAUSE_RESET_NOT_ALLOWED,		            /**< Reset not allowed */
	TAPI_CAUSE_EMERGENCY_CALL_NOT_ALLOWED,			/**< Emergency call not allowed */
	TAPI_CAUSE_ACCESS_NOT_ALLOWED,		            /**< Access not allowed */						//80
	TAPI_CAUSE_RADIO_LINK_FAILURE,		            /**< Radio link failure */
	TAPI_CAUSE_INVALID_IDENTITY,		            /**< Invalid identity */
	TAPI_CAUSE_UNKNOWN_IDENTITY,		            /**< Unknown identity */
	TAPI_CAUSE_UNACCEPTABLE_IDENTITY,	            /**< Unacceptable identity */
	TAPI_CAUSE_NO_SERVICE_HERE,		                /**< No service here */
	TAPI_CAUSE_SERVICE_NOT_ALLOWED,	                /**< Service not allowed */
	TAPI_CAUSE_SERVICE_NOT_AVAIL,		            /**< Service not available */
	TAPI_CAUSE_SERVICE_NOT_WORKING,	                /**< Service not working */
	TAPI_CAUSE_CANNOT_IDENTIFY_CALL,	            /**< Cannot identify the call */
	TAPI_CAUSE_DEACTIVATION,			            /**< Deactivation */							//90
	TAPI_CAUSE_FATAL_ERROR,			                /**< Fatal error */
	TAPI_CAUSE_SEND_DTMF_SUCCESS,		            /**< Sending DTMF succeeded */
	TAPI_CAUSE_SEND_DTMF_FAIL,			            /**< Sending DTMF failed */
	TAPI_CAUSE_FIXED_DIALING_NUMBER_ONLY,	        /**< Fixed Dialing Number Only */
	TAPI_CAUSE_TIMER_EXPIRED,			            /**< Call Timer Expired */
} TelCallCause_t;

/**
 * @brief Enumeration when a call is ended, this end cause is given with the CALL END notification to the application.
 * @details This end cause can be originated from different elements in the communication path or different modules in the same element.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_END_NO_CAUSE = 0x00,				/**< No Cause */

	TAPI_CC_CAUSE_UNASSIGNED_NUMBER,			/**< Unassigned Number */
	TAPI_CC_CAUSE_NO_ROUTE_TO_DEST,			    /**< No Route to Destination */
	TAPI_CC_CAUSE_CHANNEL_UNACCEPTABLE,		    /**< Channel Unacceptable */
	TAPI_CC_CAUSE_OPERATOR_DETERMINED_BARRING,	/**< Operator Determined Barring */
	TAPI_CC_CAUSE_NORMAL_CALL_CLEARING,		    /**< Normal Call Clearing */

	TAPI_CC_CAUSE_USER_BUSY,					/**< User Busy */
	TAPI_CC_CAUSE_NO_USER_RESPONDING,			/**< No user responding */
	TAPI_CC_CAUSE_USER_ALERTING_NO_ANSWER,		/**< User Alerting no Answer */
	TAPI_CC_CAUSE_CALL_REJECTED,				/**< Call Rejected */
	TAPI_CC_CAUSE_NUMBER_CHANGED,				/**< Number Changed */

	TAPI_CC_CAUSE_NON_SELECTED_USER_CLEARING,	/**< Non Selected User Clearing */
	TAPI_CC_CAUSE_DESTINATION_OUT_OF_ORDER,	    /**< Destination out of Order */
	TAPI_CC_CAUSE_INVALID_NUMBER_FORMAT,		/**< Invalid Number Format */
	TAPI_CC_CAUSE_FACILITY_REJECTED,			/**< Facility Rejected */
	TAPI_CC_CAUSE_RESPONSE_TO_STATUS_ENQUIRY,	/**< Response to Status Enquiry */

	TAPI_CC_CAUSE_NORMAL_UNSPECIFIED,				/**< Normal Unspecified */
	TAPI_CC_CAUSE_NO_CIRCUIT_CHANNEL_AVAILABLE,	    /**< No Circuit Channel Available */
	TAPI_CC_CAUSE_NETWORK_OUT_OF_ORDER,			    /**< Network out of Order */
	TAPI_CC_CAUSE_TEMPORARY_FAILURE,				/**< Temporary Failure */
	TAPI_CC_CAUSE_SWITCHING_EQUIPMENT_CONGESTION,	/**< Switching Equipment Congestion */

	TAPI_CC_CAUSE_ACCESS_INFORMATION_DISCARDED,			    /**< Access Information Discarded */
	TAPI_CC_CAUSE_REQUESTED_CIRCUIT_CHANNEL_NOT_AVAILABLE,	/**< Requested Circuit channel not available */
	TAPI_CC_CAUSE_RESOURCES_UNAVAILABLE_UNSPECIFIED,		/**< Resources unavailable and unspecified */
	TAPI_CC_CAUSE_QUALITY_OF_SERVICE_UNAVAILABLE,			/**< Quality of service unavailable */
	TAPI_CC_CAUSE_REQUESTED_FACILITY_NOT_SUBSCRIBED,		/**< Requested facility not subscribed */

	TAPI_CC_CAUSE_INCOMING_CALL_BARRED_WITHIN_CUG,			/**< Incoming call barred within CUG */
	TAPI_CC_CAUSE_BEARER_CAPABILITY_NOT_AUTHORISED,		    /**< Bearer capability not Authorised */
	TAPI_CC_CAUSE_BEARER_CAPABILITY_NOT_PRESENTLY_AVAILABLE,/**< Bearer capability not presently Available */
	TAPI_CC_CAUSE_SERVICE_OR_OPTION_NOT_AVAILABLE,			/**< Service or Option not available */
	TAPI_CC_CAUSE_BEARER_SERVICE_NOT_IMPLEMENTED,			/**< Service not implemented */

	TAPI_CC_CAUSE_ACM_GEQ_ACMMAX,							/**< ACM GEQ ACMAX */
	TAPI_CC_CAUSE_REQUESTED_FACILITY_NOT_IMPLEMENTED,		/**< Requested facility not implemented */
	TAPI_CC_CAUSE_ONLY_RESTRICTED_DIGITAL_INFO_BC_AVAILABLE,/**< Restricted digital info BC not available */
	TAPI_CC_CAUSE_SERVICE_OR_OPTION_NOT_IMPLEMENTED,		/**< Service or Option not Implemented */
	TAPI_CC_CAUSE_INVALID_TRANSACTION_ID_VALUE,			    /**< Transaction ID value */

	TAPI_CC_CAUSE_USER_NOT_MEMBER_OF_CUG,				    /**< Not a member of CUG */
	TAPI_CC_CAUSE_INCOMPATIBLE_DESTINATION,			        /**< Incompatible Destination */
	TAPI_CC_CAUSE_INVALID_TRANSIT_NETWORK_SELECTION,	    /**< Transit Network selection */
	TAPI_CC_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,		    /**< Semantically Incorrect message */
	TAPI_CC_CAUSE_INVALID_MANDATORY_INFORMATION,		    /**< Invalid Mandatory Message */

	TAPI_CC_CAUSE_MESSAGE_TYPE_NON_EXISTENT,					/**< Message Type Non Existent */
	TAPI_CC_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STATE,	/**< Message type not compatible with Prot state */
	TAPI_CC_CAUSE_IE_NON_EXISTENT_OR_NOT_IMPLEMENTED,			/**< IE non exitent or not implemented */
	TAPI_CC_CAUSE_CONDITIONAL_IE_ERROR,						    /**< Conditional IE error */
	TAPI_CC_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE,	/**< Not Compatible with protocol state */

	TAPI_CC_CAUSE_RECOVERY_ON_TIMER_EXPIRY,	    /**< Recovery on timer expiry */
	TAPI_CC_CAUSE_PROTOCOL_ERROR_UNSPECIFIED,	/**< Protocol error unspecified */
	TAPI_CC_CAUSE_INTERWORKING_UNSPECIFIED,	/**< Interworking unspecified */
	TAPI_CC_CAUSE_REORDER,	/**< Reorder */

	TAPI_CC_CAUSE_END = 128,	/**< End unspecified */

	/* Reject causes*/
	TAPI_REJECT_CAUSE_IMSI_UNKNOWN_IN_HLR,	/**< IMSI unknown in HLR */
	TAPI_REJECT_CAUSE_ILLEGAL_MS,			/**< Illegal MS */
	TAPI_REJECT_CAUSE_IMSI_UNKNOWN_IN_VLR,	/**< IMSI unknown in VLR */
	TAPI_REJECT_CAUSE_IMEI_NOT_ACCEPTED,	/**< IMEI not accepted */
	TAPI_REJECT_CAUSE_ILLEGAL_ME,			/**< Illegal ME */

	TAPI_REJECT_CAUSE_GPRS_SERVICES_NOT_ALLOWED,						/**< GPRS service not allowed */
	TAPI_REJECT_CAUSE_GPRS_SERVICES_AND_NON_GPRS_SERVICES_NOT_ALLOWED,	/**< GPRS services and Non-GPRS services not allowed */
	TAPI_REJECT_CAUSE_MS_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK,	    /**< MS device cannot be derived by the network */
	TAPI_REJECT_CAUSE_IMPLICITLY_DETACHED,								/**< Implicitly detached */
	TAPI_REJECT_CAUSE_PLMN_NOT_ALLOWED,								    /**< PLMN not allowed */

	TAPI_REJECT_CAUSE_LA_NOT_ALLOWED,							/**< LA not allowed */
	TAPI_REJECT_CAUSE_NATIONAL_ROAMING_NOT_ALLOWED,			    /**< National roaming not allowed */
	TAPI_REJECT_CAUSE_GPRS_SERVICES_NOT_ALLOWED_IN_THIS_PLMN,	/**< GPRS services not allowed in this PLMN */
	TAPI_REJECT_CAUSE_NO_SUITABLE_CELLS_IN_LA,					/**< No suitable cells in the LA */
	TAPI_REJECT_CAUSE_MSC_TEMPORARILY_NOT_REACHABLE,			/**< MSC temporarily not reachable */

	TAPI_REJECT_CAUSE_NETWORK_FAILURE,		/**< Network unavailable */
	TAPI_REJECT_CAUSE_MAC_FAILURE,			/**< MAC failure */
	TAPI_REJECT_CAUSE_SYNCH_FAILURE,		/**< SYNCH failure */
	TAPI_REJECT_CAUSE_CONGESTTION,			/**< Congestion */
	TAPI_REJECT_CAUSE_GSM_AUTH_UNACCEPTED,	/**< GSM Auth unaccepted */

	TAPI_REJECT_CAUSE_SERVICE_OPTION_NOT_SUPPORTED, /**< Service option not supported */
	TAPI_REJECT_CAUSE_REQ_SERV_OPT_NOT_SUBSCRIBED,	/**< REQ_SERV option not suscribed */
	TAPI_REJECT_CAUSE_SERVICE_OPT__OUT_OF_ORDER,	/**< Service OPT out of order */
	TAPI_REJECT_CAUSE_CALL_CANNOT_BE_IDENTIFIED,	/**< Call cannot be identified */
	TAPI_REJECT_CAUSE_NO_PDP_CONTEXT_ACTIVATED,	    /**< No PDP context Activated */

	TAPI_REJECT_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_MIN_VALUE,	/**< Retry upon entry into a new call min value */
	TAPI_REJECT_CAUSE_RETRY_UPON_ENTRY_INTO_A_NEW_CELL_MAX_VALUE,	/**< Retry upon entry into a new call max value */
	TAPI_REJECT_CAUSE_SEMANTICALLY_INCORRECT_MSG,					/**< Semantically incorret message */
	TAPI_REJECT_CAUSE_INVALID_MANDATORY_INFO,						/**< Invalid mandatory information */
	TAPI_REJECT_CAUSE_MESSAGE_TYPE_NON_EXISTANT,					/**< Message type non-existant */

	TAPI_REJECT_CAUSE_MESSAGE_TYPE_NOT_COMP_PRT_ST,		    /**< Message type not COMP PRT ST */
	TAPI_REJECT_CAUSE_IE_NON_EXISTANT,						/**< IE non existent */
	TAPI_REJECT_CAUSE_MSG_NOT_COMPATIBLE_PROTOCOL_STATE,	/**< MSG not compatible protocol state */

	/* Connection Management establishment rejection cause */
	TAPI_REJECT_CAUSE_REJ_UNSPECIFIED,	/**< REJ unspecified */

	/* AS reject causes */
	TAPI_REJECT_CAUSE_AS_REJ_RR_REL_IND,						/**< RR release indication */
	TAPI_REJECT_CAUSE_AS_REJ_RR_RANDOM_ACCESS_FAILURE,			/**< Random Access Failure */
	TAPI_REJECT_CAUSE_AS_REJ_RRC_REL_IND,						/**< RRC release indication */
	TAPI_REJECT_CAUSE_AS_REJ_RRC_CLOSE_SESSION_IND,			    /**< RRC close session indication */
	TAPI_REJECT_CAUSE_AS_REJ_RRC_OPEN_SESSION_FAILURE,			/**< RRC open session failure */
	TAPI_REJECT_CAUSE_AS_REJ_LOW_LEVEL_FAIL,					/**< Low level failure */
	TAPI_REJECT_CAUSE_AS_REJ_LOW_LEVEL_FAIL_REDIAL_NOT_ALLOWED, /**< Low level failure redial not alowed */
	TAPI_REJECT_CAUSE_AS_REJ_LOW_LEVEL_IMMED_RETRY,			    /**< Low level immediate retry */

	/* MM reject causes */
	TAPI_REJECT_CAUSE_MM_REJ_INVALID_SIM,			/**< Invalid SIM */
	TAPI_REJECT_CAUSE_MM_REJ_NO_SERVICE,			/**< No service */
	TAPI_REJECT_CAUSE_MM_REJ_TIMER_T3230_EXP,		/**< Timer T3230 expiry */
	TAPI_REJECT_CAUSE_MM_REJ_NO_CELL_AVAILABLE,	    /**< No call available  */
	TAPI_REJECT_CAUSE_MM_REJ_WRONG_STATE,			/**< Wrong state */
	TAPI_REJECT_CAUSE_MM_REJ_ACCESS_CLASS_BLOCKED,	/**< Access class blocked */

	/* Definitions for release ind causes between MM and CNM */
	TAPI_REJECT_CAUSE_ABORT_MSG_RECEIVED,	/**< Abort Message received */
	TAPI_REJECT_CAUSE_OTHER_CAUSE,			/**< Other cause */

	/* CNM reject causes */
	TAPI_REJECT_CAUSE_CNM_REJ_TIMER_T303_EXP,	/**< Timer T303 expiry */
	TAPI_REJECT_CAUSE_CNM_REJ_NO_RESOURCES,	    /**< Rejected due to unavailibilty of resources */
	TAPI_REJECT_CAUSE_CNM_MM_REL_PENDING,		/**< MM release pending */
	TAPI_REJECT_CAUSE_CNM_INVALID_USER_DATA,	/**< Invalid user data */
	TAPI_CALL_END_CAUSE_MAX = 255,				/**< Maximum End Cause limit for GSM/WCDMA */

	/* CDMA CALL END CAUSE */
	TAPI_CDMA_END_CAUSE_REL_BY_USER = 0x1001,		/**< Call Released by User */
	TAPI_CDMA_END_CAUSE_REL_BY_NET,				    /**< Call Released by Network */
	TAPI_CDMA_END_CAUSE_REL_NET_BUSY,				/**< Call Released because the network is busy */
	TAPI_CDMA_END_CAUSE_NO_SVC,					    /**< Call Released because of No Service area */
	TAPI_CDMA_END_CAUSE_FADING,					    /**< Call Released because of Fading */
	TAPI_CDMA_END_CAUSE_RELEASE_BY_REORDER,		    /**< Call Released because of reorder */
	TAPI_CDMA_END_CAUSE_RELEASE_BY_INTERCEPT,		/**< Call Released because of intercept */
	TAPI_CDMA_END_CAUSE_SILENT_ZONE_RETRY,			/**< Call Released because of silent zone retry */
	TAPI_CDMA_END_CAUSE_OTA_CALL_FAIL,				/**< Call Released because of OTA call failure */
	TAPI_CDMA_END_CAUSE_PHONE_OFFLINE,				/**< Call Released because phone is offline */
	TAPI_CDMA_END_CAUSE_PHONE_IS_CDMA_LOCKED,		/**< Call Released because CDMA is locked */
	TAPI_CDMA_END_CAUSE_FLASH_IS_IN_PROGRESS_ERR,	/**< Call Released because of the flash-is-in-progress error */
	TAPI_CDMA_END_CAUSE_E911_MODE_ERR,				/**< Call Released because of the e911 mode */
	TAPI_CDMA_END_CAUSE_OTHERS,					    /**< Call Released by Others */
	TAPI_CDMA_END_CAUSE_MAX						    /**< Maximum End Cause limit for CDMA */
} TelTapiEndCause_t;

/**
 * @brief Enumeration for the call conference states.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_CONFERENCE_IDLE,	/**< Idle */
	TAPI_CALL_CONFERENCE_ACTIVE /**< Active */
} TelConferenceCallState_t;

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
	TAPI_CALL_STATE_MAX	        /**< Call state unknown */
} TelCallStates_t;

/**
 * @brief IN GSM ONLY: call identification number.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ACTIVE_LINE1, /**< Line 1 */
	TAPI_CALL_ACTIVE_LINE2	/**< Line 2 */
} TelCallActiveLine_t;

typedef enum {
	TAPI_CALL_UUS_NONE, /**< No User to User information  */
	TAPI_CALL_UUS_1,	/**< User to User information 1 */
} TelCallUusType_t;

/**
 * @brief Enumeration for SS info messages of an incoming call indication.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_MT_CLI_PRESENT = 0x31, /**< Number details are present in the "char number" of the call info struct. If the number is not present then the "no cli cause" member in  mt_ss_info is set accordingly */
	TAPI_CALL_MT_CNA_PRESENT = 0x32, /**< Calling Name Info present, #TelCallingNameInfo_t of the call info struct contains this info */
	TAPI_CALL_MT_SS_UNDEFINED = 0x00, /**< Undefined SS indication. Ignore mt_ss_info */

} TelCallMtSSInfo_t;

/**
 * @brief Enumeration for the answer type used as a parameter in the answer call API.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ANSWER_ACCEPT = 0,		/**< Used to answer an incoming call when there are no current active calls */
	TAPI_CALL_ANSWER_REJECT,			/**< Used to reject the incoming call */
	TAPI_CALL_ANSWER_REPLACE,			/**< Release current active call and accept the waiting call */
	TAPI_CALL_ANSWER_HOLD_AND_ACCEPT,	/**< Hold the current active call, and accept the waiting call */
} TelCallAnswerType_t;

/**
 * @brief Enumeration for the end type used as a parameter in the end call API.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_END = 0,        /**< End Specific Call */
	TAPI_CALL_END_ALL,        /**< End All Calls */
	TAPI_CALL_END_ACTIVE_ALL, /**< End All Active Calls */
	TAPI_CALL_END_HOLD_ALL,   /**< End All Held Calls */
} TelCallEndType_t;




/**
 * @brief Enumeration for call type to be used by applications while using the setup call API. Like the requested call is a voice call or video call.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_TYPE_VOICE,	/**< Voice call type */// in ipcv4, 0x0100
	TAPI_CALL_TYPE_DATA,	/**< Data call type - (for modem, fax, packet, and other such calls) */// in ipcv4, 0x0200
	TAPI_CALL_TYPE_E911	    /**< Emergency call type */// in ipcv4, 0x0700
} TelCallType_t;

/**
 * @brief Enumeration for the various alerting patterns available.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ALERTING_PATTERN_1 = 0x00, /**< Pattern 1 */
	TAPI_CALL_ALERTING_PATTERN_2 = 0x01, /**< Pattern 2 */
	TAPI_CALL_ALERTING_PATTERN_3 = 0x02, /**< Pattern 3 */
	TAPI_CALL_ALERTING_PATTERN_4 = 0x03, /**< Pattern 4 */
	TAPI_CALL_ALERTING_PATTERN_5 = 0x04, /**< Pattern 5 */
	TAPI_CALL_ALERTING_PATTERN_6 = 0x05, /**< Pattern 6 */
	TAPI_CALL_ALERTING_PATTERN_7 = 0x06, /**< Pattern 7 */
	TAPI_CALL_ALERTING_PATTERN_8 = 0x07, /**< Pattern 8 */
	TAPI_CALL_NO_ALERTING_PATTERN = 0x08 /**< No Alerting Pattern */
} TelCallAlertingPattern_t;

/**
 * @brief Enumeration for state of the current call.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_CONNECT_IDLE,	    /**< Call is in the Idle state */
	TAPI_CALL_CONNECT_ACTIVE,	/**< Call is in the Active state */
	TAPI_CALL_CONNECT_HELD		/**< Call is in the Held state */
} TelCallActiveState_t;

/**
 * @brief Enumeration for closed user group options.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_CUG_NO_INFO = 0x00,		/**< No information */
	TAPI_CALL_CUG_SUPRESS_OA,			/**< Suppress OA */
	TAPI_CALL_CUG_SUPRESS_PRF_CUG,		/**< Suppress preferential CUG */
	TAPI_CALL_CUG_SUPRESS_OA_AND_CUG	/**< Suppress OA and preferential CUG */
} TelCallCugOptions_t;

/**
 * @brief Enumeration for tapi call UUS protocol types.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_UUS_PROTO_SPECIFIC = 0,			/**< The user information is structured according to user needs */
	TAPI_CALL_UUS_PROTO_OSI_HIGHER_LAYER = 1,	/**< OSI high layer protocols */
	TAPI_CALL_UUS_PROTO_IA5_CHARS = 4,			/**< User information consists of IA5 characters */
	TAPI_CALL_UUS_PROTO_V120 = 7,				/**< Rate adaptation */
	TAPI_CALL_UUS_PROTO_Q931 = 8				/**< User-network call control messages */
} TelCallUusProtocolType_t;

/**
 * @brief Enumeration for the call name mode.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_NAME_AVAIL = 0,	        /**< This identifier refers to presenting the calling party's name identity to the called party */
	TAPI_CALL_NAME_RESTRICTED = 1,	    /**< This identifier refers to restricting the name identity of the calling party from being presented to the called party */
	TAPI_CALL_NAME_UNAVAIL = 2,			/**< This identifier refers to the unavailability of the calling party's name identity from being offered to the called party */
	TAPI_CALL_NAME_AVAIL_RESTRICTED = 3	/**< This identifier refers to offering the calling party's name identity to the called party with which the presentation restriction is overridden */
} TelCallNameMode_t;

/**
 * @brief Enumeration for the "No Cli cause" value.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_NO_CLI_CAUSE_NONE = -1,	                     /**< None */
	TAPI_CALL_NO_CLI_CAUSE_UNAVAILABLE = 0x00,		         /**< Unavailable */
	TAPI_CALL_NO_CLI_CAUSE_REJECTBY_USER = 0x01,		     /**< Rejected by User */
	TAPI_CALL_NO_CLI_CAUSE_INTERACTION_OTHERSERVICES = 0x02, /**< Other services */
	TAPI_CALL_NO_CLI_CAUSE_COINLINE_PAYPHONE = 0x03,	     /**< Coin line phone */
} TelCallNoCliCause_t;

/**
 * @brief Enumeration for the "Cli mode" value.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_PRES_AVAIL,		/**< Presentation Allowed */
	TAPI_CALL_PRES_RESTRICTED,	/**< Presentation Restricted */
	TAPI_CALL_NUM_UNAVAIL,		/**< Number Not Available */
}TelCallCliMode_t;

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
	TAPI_CALL_PRIVACY_MODE_STANDARD = 0x00, /**< Standard mode */
	TAPI_CALL_PRIVACY_MODE_ENHANCED		    /**< Enhanced mode */
} TelCallPrivacyMode_t;

/**
 * @brief Enumeration for the OTASP Status. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_OTASP_OK_SPL_UNLOCKED = 0x01, /**< OTASP : SPL unlocked ok */
	TAPI_CALL_OTASP_OK_AKEY_EXCESS,		    /**< OTASP : A-Key excess ok */
	TAPI_CALL_OTASP_OK_SSD_UPDATE,		    /**< OTASP : SSD update ok */
	TAPI_CALL_OTASP_OK_NAM_DWNLD,		    /**< OTASP : NAM download ok */
	TAPI_CALL_OTASP_OK_MDN_DWNLD,		    /**< OTASP : MDN download ok */
	TAPI_CALL_OTASP_OK_IMSI_DWNLD,		    /**< OTASP : IMSI download ok */
	TAPI_CALL_OTASP_OK_PRL_DWNLD,		    /**< OTASP : PRL download ok */
	TAPI_CALL_OTASP_OK_COMMIT,		        /**< OTASP : commit ok */
	TAPI_CALL_OTASP_OK_PROGRAMMING,		    /**< OTASP : programming ok */
	TAPI_CALL_OTASP_SUCCESS,		        /**< OTASP : success */
	TAPI_CALL_OTASP_UNSUCCESS,		        /**< OTASP : unsuccess */
	TAPI_CALL_OTASP_OK_OTAPA_VERIFY,	    /**< OTASP : verify ok */
	TAPI_CALL_OTASP_PROGRESS,		        /**< OTASP : progress */
	TAPI_CALL_OTASP_FAILURES_EXCESS_SPC,	/**< OTASP : SPC excess failure */
	TAPI_CALL_OTASP_LOCK_CODE_PW_SET,	    /**< OTASP : lock code password set */
} TelCallOtaspStatus_t;

/**
 * @brief Enumeration for the OTAPA Status. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_OTAPA_STOP = 0x00,	/**< OTAPA : stop */
	TAPI_CALL_OTAPA_START,		    /**< OTAPA : start */
} TelCallOtapaStatus_t;

/**
 * @brief Enumeration for the Alert Signal Type. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_SIGNAL_TYPE_TONE = 0x00,
	TAPI_CALL_SIGNAL_TYPE_ISDN_ALERTING,
	TAPI_CALL_SIGNAL_TYPE_IS54B_ALERTING,
	TAPI_CALL_SIGNAL_TYPE_RESERVED
} TelCallAlertSignalType_t;

/**
 * @brief Enumeration for the Alert Pitch Type. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ALERT_PITCH_MED = 0x00,
	TAPI_CALL_ALERT_PITCH_HIGH,
	TAPI_CALL_ALERT_PITCH_LOW,
	TAPI_CALL_ALERT_PITCH_RESERVED
} TelCallAlertPitchType_t;

/**
 * @brief Enumeration for the signals specific to the alert signal type TAPI_CALL_SIGNAL_TYPE_TONE. (CDMA only)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_SIGNAL_TONE_DIAL = 0x00,
	TAPI_CALL_SIGNAL_TONE_RINGBACK_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_INTERCEPT_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_ABBREV_TONE,
	TAPI_CALL_SIGNAL_TONE_NETWORK_CONGESTION_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_ABBREV_NETWORK_CONGESTION,
	TAPI_CALL_SIGNAL_TONE_BUSY_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_CFRM_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_ANSWER_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_CALL_WAITING_TONE_ON,
	TAPI_CALL_SINGNAL_TONE_PIPE_TONE_ON,
	TAPI_CALL_SIGNAL_TONE_OFF
} TelCallToneSignal_t;

/**
 * @brief Enumeration for the signals specific to the alert signal type TAPI_CALL_SIGNAL_TYPE_ISDN_ALERTING. (CDMA only)
 */
typedef enum {
	TAPI_CALL_SIGNAL_ISDN_ALERT_NORMAL = 0x00,
	TAPI_CALL_SIGNAL_ISDN_ALERT_INTER_GROUP,
	TAPI_CALL_SIGNAL_ISDN_ALERT_SPECIAL_PRIORITY,
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED1,
	TAPI_CALL_SIGNAL_ISDN_ALERT_PING_RING,
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED2,
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED3,
	TAPI_CALL_SIGNAL_ISDN_ALERT_ISDN_RESERVED4,
	TAPI_CALL_SIGNAL_ISDN_ALERT_OFF
} TelCallIsdnAlertSignal_t;

/**
 * @brief Enumeration for the signals specific to the alert signal type TAPI_CALL_SIGNAL_TYPE_IS54B_ALERTING. (CDMA only)
 */
typedef enum {
	TAPI_CALL_SIGNAL_IS54B_ALERT_NOTONE = 0x00,
	TAPI_CALL_SIGNAL_IS54B_ALERT_LONG,
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT,
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT_LONG,
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT_2,
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_LONG_SHORT,
	TAPI_CALL_SIGNAL_IS54B_ALERT_SHORT_SHORT_SHORT_SHORT,
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_LONG,
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_SHORT,
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_SHORT_LONG,
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_LONG_SHORT,
	TAPI_CALL_SIGNAL_IS54B_ALERT_PBX_SHORT_SHORT_SHORT_SHORT,
	TAPI_CALL_SIGNAL_IS54B_ALERT_PIP_PIP_PIP_PIP
} TelCallIs54bAlertSignal_t;

/**
 * @brief Enumeration for the call time request mask type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_TIME_TYPE_TOTAL_CALL_CNT = 0x01,	/**< Total Call Count Mask */
	TAPI_CALL_TIME_TYPE_TOTAL_CALL_TIME = 0x02, /**< Total Call Time Mask */
	TAPI_CALL_TIME_TYPE_LAST_CALL_TIME = 0x04,	/**< Last Call Time Mask */
} TelCallTimeMaskType_t;

/**
 * @brief Enumeration for the DTMF tone length.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_TONE_DURATION_SHORT = 0x01, /**< Tone Duration Short */
	TAPI_CALL_TONE_DURATION_LONG          /**< Tone Duration Long */
} TelCallToneDuration_t;

/**
 * @brief Enumeration for the emergency category type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_ECC_DEFAULT = 0x00,        /**< Default case */
	TAPI_CALL_ECC_POLICE = 0x01,         /**< Police emergency */
	TAPI_CALL_ECC_AMBULANCE = 0x02,      /**< Ambulance emergency */
	TAPI_CALL_ECC_FIREBRIGADE = 0x04,    /**< Firebrigade emergency */
	TAPI_CALL_ECC_MARINEGUARD = 0x08,    /**< Marineguard emergency */
	TAPI_CALL_ECC_MOUNTAINRESCUE = 0x10, /**< Mountain rescue emergency */
	TAPI_CALL_ECC_MANUAL_ECALL = 0x20,   /**< Manual emergency call */
	TAPI_CALL_ECC_AUTO_ECALL = 0x40,     /**< Automatic emergency call */
	TAPI_CALL_ECC_NONE = 0xff,           /**< Unspecified emergency */
} TelCallEmergencyCategory_t;

typedef enum {
	TAPI_CALL_IDENTITY_DEFAULT, /**< Calling line identity will be sent. Based on the network setting, the line identity will be presented to the called party */
	TAPI_CALL_IDENTITY_SHOW,	/**< Display the calling line identity */
	TAPI_CALL_IDENTITY_HIDE	    /**< Hide the display of the calling line identity */
} TelCallIdentityMode_t;

/**
 * @brief Enumeration for the context type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CONTEXT_TYPE_NONE = 0x00,       /**< Unspecified context */
	TAPI_CONTEXT_TYPE_VOICE_CALL = 0x01, /**< Voice call context */
	TAPI_CONTEXT_TYPE_DATA_CALL = 0x2,   /**< Data call context */
	TAPI_CONTEXT_TYPE_VIDEO_CALL = 0x3,  /**< Video call context */
	TAPI_CONTEXT_TYPE_NETTEXT,           /**< Network text context */
	TAPI_CONTEXT_TYPE_E911,              /**< Emergency context */
} TapiContextType;

/**
 * @brief Enumeration for the timer state and/or call timer ID.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_TSTATE_IDLE = 0x00,		/**< No timer running */
	TAPI_CALL_TSTATE_SETUP_PENDING,	    /**< Setup Timer */
	TAPI_CALL_TSTATE_CONNECT_PENDING,	/**< Connect Timer */
	TAPI_CALL_TSTATE_END_PENDING,		/**< Release Timer */
	TAPI_CALL_TSTATE_HOLD_PENDING,		/**< Hold Timer */
	TAPI_CALL_TSTATE_RETRIEVE_PENDING,	/**< Retrieve Timer */
	TAPI_CALL_TSTATE_JOIN_PENDING,		/**< Join Timer */
	TAPI_CALL_TSTATE_SPLIT_PENDING,	    /**< Split Timer */
	TAPI_CALL_TSTATE_ECT_PENDING,		/**< ECT Timer */
	TAPI_CALL_TSTATE_DTMF_PENDING,		/**< DTMF Timer */
} TelTapiCallTimerState;

/**
 * @brief Enumeration for the offlength to send DTMF.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_DTMF_OFFLENGTH_60MS,
	TAPI_CALL_DTMF_OFFLENGTH_100MS,
	TAPI_CALL_DTMF_OFFLENGTH_150MS,
	TAPI_CALL_DTMF_OFFLENGTH_200MS,
} TelCallDtmfInterDigitInterval_t;

/**
 * @brief Enumeration for the onlength to send DTMF.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_DTMF_ONLENGTH_95MS,
	TAPI_CALL_DTMF_ONLENGTH_150MS,
	TAPI_CALL_DTMF_ONLENGTH_200MS,
	TAPI_CALL_DTMF_ONLENGTH_250MS,
	TAPI_CALL_DTMF_ONLENGTH_300MS,
	TAPI_CALL_DTMF_ONLENGTH_350MS,
	TAPI_CALL_DTMF_ONLENGTH_SMS
}TelCallDtmfPulseWidth_t;

/**
 * @brief Enumeration for the Call Record Info Type
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_REC_INFO_TYPE_NAME,
	TAPI_CALL_REC_INFO_TYPE_NUMBER,
}TelCallRecordInfoType_t;

/**
 * @brief TelCallPreferredVoiceSubs_t
 * This enumeration defines possible 'preferred' Voice Subscriptions
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_CALL_PREFERRED_VOICE_SUBS_UNKNOWN = -1,  /**<  Unknown status **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_CURRENT_NETWORK = 0, /**< Current network **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_ASK_ALWAYS, /**< ASK Always **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_SIM1, /**< SIM 1 **/
	TAPI_CALL_PREFERRED_VOICE_SUBS_SIM2 /**<  SIM 2 **/
} TelCallPreferredVoiceSubs_t;


typedef struct {
	int bRequestedBySAT;			/**< This flag is to indicate whether the request has been initiated by SAT. Possible values are @c 0 and @c 1. This flag should be updated only when initiating a setup request */
	TelCallIdentityMode_t IdentityMode; /**< Sets the Calling Line Identity mode. If the default value #TAPI_IDENTITY_DEFAULT is set, OEM will set the default value */
	TelCallEmergencyCategory_t ecc;
} tel_set_call_satflag_identiymode;

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
	TelCallNameMode_t NameMode;					    /**< Display mode of @a szNameData */
	char szNameData[TAPI_CALLING_NAME_SIZE_MAX];	/**< Calling party name string */
} TelCallingNameInfo_t;

/**
 * @brief The structure type for information about an outgoing call.
 * @since_tizen 2.3
 */
typedef struct {
	TelCallType_t CallType;						/**< Sets type of call (voice, data, video, emergency) */
	TelCallEmergencyCategory_t Ecc;
	char szNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1]; /**< A string containing the destination phone number. This follows the dial number format */
} TelCallDial_t;

/**
 * @brief The structure type for information about call status.
 * @since_tizen 2.3
 */
typedef struct {
	int CallHandle;
	int bMoCall;									/**< Status will be yes for MO Calls. If MT call, then FALSE */
	char pNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1];	/**< Mobile Number */
	TelCallType_t CallType;						    /**< Type of call (voice, data, emergency) */
	TelCallStates_t CallState;						/**< Current Call state */
	int bConferenceState;							/**< Whether Call is in Conference */
} TelCallStatus_t;

/**
 * @brief Called to get the call status.
 * @since_tizen 2.3
 */
typedef void (*TelCallStatusCallback)( TelCallStatus_t *out, void *user_data );


/**
 * @brief The structure type for information about call redirection details.
 * @since_tizen 2.3
 */
typedef struct {
	char szRedirectedNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1];		/**< Redirected party number */
	char szRedirectSubAddress[TAPI_CALL_SUBADDRESS_LEN_MAX + 1];	/**< Redirect party sub-address */
} TelCallRedirectionInfo_t;

/**
 * @brief The structure type for incoming call information.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int CallHandle;				/**< At the Foundation API Level, call handle indicates the handle of the call for the application. At the Framework API Level, call handle indicates the call identifier used for a call by the OEM Adaptation Layer Plug-in */
	TelCallType_t CallType;				    /**< Call type */
	TelSsLineIdentificationType_t CliPresentationIndicator; /**< Calling Party presentation indicator */
	char szCallingPartyNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1]; /**< Caller number, null terminated ASCII */
	TelCallingNameInfo_t CallingNameInfo;	/**< Call name info. If there is no information from the network, this information will be @c NULL */
	TelCallRedirectionInfo_t RedirectInfo;	/**< The data for the Call Redirect information. If there is no information from the network, this information will be @c NULL */
	TelCallCliMode_t CliMode;		        /**< CLI mode */
	TelCallNoCliCause_t CliCause;		    /**< No CLI cause */
	int fwded;								/**< True or false. If the incoming call is a forwarded call, then @c true else @c false */
	TelCallActiveLine_t ActiveLine;		    /**< Current Active Line */
} TelCallIncomingCallInfo_t;

/**
 * @brief The structure type for SS Info message.
 * @since_tizen 2.3
 */
typedef struct {
	TelCallMtSSInfo_t type;             /**< Type of SS Info present */
	TelCallCliMode_t CliMode;		    /**< CLI mode */
	TelCallNoCliCause_t no_cli_cause;	/**< TBD */
} TelCallMtSsInfo_t; // not used

typedef struct {
	int isMobileOriginated;					     /**< Is this call Mobile Originated? */
	TelCallNameMode_t nameMode;				     /**< Calling Party Name availability mode */
	char callNumber[TAPI_CALL_DIALDIGIT_LEN_MAX + 1]; /**< A character array of the destination address. This follows the dial @a callNumber format */
	unsigned int startTime;					    /**< Time when a call is started */
	unsigned int endTime;						/**< Time when a call is ended */
	TelCallType_t callType;					    /**< Type of a call */
	TelCallStates_t callState;					/**< State of a call */
	TelCallActiveState_t activeState;			/**< Status of a call */
	TelConferenceCallState_t conferenceState;	/**< State of a conference call */
	TelCallingNameInfo_t callingNameInfo;		/**< Calling name information of a call */
	TelCallActiveLine_t current_line;			/**< Line information of a call */
	TelCallCause_t cause;						/**< End type of a call when a call is ended */
	TelTapiEndCause_t endCause;				    /**< End Cause */
} TelTapiCallInfo_t;

/**
 * @brief The structure type for the connected number information.
 * @since_tizen 2.3
 */
typedef struct {
	TelCallCliMode_t CliMode;		    /**< CLI mode */
	TelCallNoCliCause_t no_cli_cause;   /**< Cause when no CLI number */
	unsigned char dcs;					/**< DCS */
	unsigned char number_type;			/**< Number type */
	TelCallNameMode_t name_mode;		/**< Display mode of the name */
	char number[TAPI_CALL_DIALDIGIT_LEN_MAX + 1];		/**< Caller number, null terminated ASCII */
	char sub_address[TAPI_CALL_SUBADDRESS_LEN_MAX + 1]; /**< Called sub-address, null terminated ASCII */
} TelCallConnectedNumberInfo_t;

/**
 * @brief The structure type for information about call time and call count.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned short req_type_mask;	/**< Call time request type */
	unsigned long TotalCallCnt;	    /**< Total Call Count */
	unsigned long OutgoingCallCnt;	/**< Outgoing Call Count */
	unsigned long IncomingCallCnt;	/**< Incoming Call Count */
	unsigned long TotalCallTime;	/**< Overall Call Time(Outgoing + Incoming, in seconds) */
	unsigned long OutgoingCallTime; /**< Overall Outgoing Call Time(in seconds) */
	unsigned long IncomingCallTime; /**< Overall Incoming Call Time(in seconds) */
	unsigned long LastCallTime;	    /**< Last Call Time(in seconds) */
} TelCallTimeInfo_t;

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
	TelCallDtmfPulseWidth_t pulse_width;
	TelCallDtmfInterDigitInterval_t inter_digit_interval;
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
	TAPI_SOUND_MUTE		     =0x00,		/**< Sound is mute */
	TAPI_SOUND_VOLUME_LEVEL_1=0x01,		/**< Volume level is 1 */
	TAPI_SOUND_VOLUME_LEVEL_2=0x02,		/**< Volume level is 2 */
	TAPI_SOUND_VOLUME_LEVEL_3=0x03,		/**< Volume level is 3 */
	TAPI_SOUND_VOLUME_LEVEL_4=0x04,		/**< Volume level is 4 */
	TAPI_SOUND_VOLUME_LEVEL_5=0x05,		/**< Volume level is 5 */
	TAPI_SOUND_VOLUME_LEVEL_6=0x06,		/**< Volume level is 6 */
	TAPI_SOUND_VOLUME_LEVEL_7=0x07,		/**< Volume level is 7 */
	TAPI_SOUND_VOLUME_LEVEL_8=0x08,		/**< Volume level is 8 */
	TAPI_SOUND_VOLUME_LEVEL_9=0x09		/**< Volume level is 9 */
} TelSoundVolume_t;

/**
 * @brief Enumeration for call sound device type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_DEVICE_RECEIVER = 0x00,      /**< Device type Receiver */
	TAPI_SOUND_DEVICE_SPEAKER_PHONE = 0x10, /**< Device type Speaker */
	TAPI_SOUND_DEVICE_HFK = 0x20,           /**< Device type Handsfree */
	TAPI_SOUND_DEVICE_HEADSET = 0x30,       /**< Device type HeadSet */
	TAPI_SOUND_DEVICE_BLUETOOTH = 0x40,     /**< Device type Bluetooth */
	TAPI_SOUND_DEVICE_EC = 0xA0,            /**< Device type external*/
} TelSoundDevice_t;

/**
 * @brief Enumeration for call sound type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SOUND_TYPE_VOICE = 0x01,    /**< Call Sound type voice */
	TAPI_SOUND_TYPE_KEYTONE = 0x02,  /**< Call Sound type keytone */
	TAPI_SOUND_TYPE_BELL = 0x03,     /**< Call Sound type bell */
	TAPI_SOUND_TYPE_MESSAGE = 0x04,  /**< Call Sound type message */
	TAPI_SOUND_TYPE_ALARM = 0x05,    /**< Call Sound type alarm */
	TAPI_SOUND_TYPE_PDA_MISC = 0x06, /**< Call Sound type PDA miscellaneous */
} TelSoundType_t;


typedef struct {
	TelSoundDevice_t device; /**< Call Sound device info structure */
	TelSoundType_t type;     /**< Call Sound type info structure */
	TelSoundVolume_t volume; /**< Call Sound volume info structure */
} TelCallVolumeInfo_t;

typedef enum {
	TAPI_SOUND_PATH_HANDSET			=0x01,		/**< Audio path is handset */
	TAPI_SOUND_PATH_HEADSET	        =0x02,		/**< Audio path is handset */
	TAPI_SOUND_PATH_HANDSFREE	        =0x03,	/**< Audio path is Handsfree */
	TAPI_SOUND_PATH_BLUETOOTH	        =0x04,	/**< Audio path is bluetooth */
	TAPI_SOUND_PATH_STEREO_BLUETOOTH   =0x05,	/**< Audio path is stereo bluetooth */
	TAPI_SOUND_PATH_SPK_PHONE	        =0x06,	/**< Audio path is speaker phone */
	TAPI_SOUND_PATH_HEADSET_3_5PI	    =0x07,	/**< Audio path is headset_3_5PI */
	TAPI_SOUND_PATH_BT_NSEC_OFF	    =0x08,      /**< Audio path Bluetooth NSEC is off */
	TAPI_SOUND_PATH_MIC1		    =0x09,      /**< Audio path Mic one */
	TAPI_SOUND_PATH_MIC2		    =0x0A,      /**< Audio path Bluetooth Mic two */
	TAPI_SOUND_PATH_HEADSET_HAC	    =0x0B,      /**< Audio path is headset HAC */
} TelSoundPath_t;

typedef struct {
	TelSoundPath_t path;	/**< TBD */
	enum ExtraVolumeStatus {
		TAPI_SOUND_EX_VOLUME_OFF,	/**< TBD */
		TAPI_SOUND_EX_VOLUME_ON,	/**< TBD */
	} ex_volume;	/**< TBD */
} TelCallSoundPathInfo_t;

typedef enum {
	TAPI_SOUND_MUTE_PATH_TX = 0x00, /**< Sound mute path transmit */
	TAPI_SOUND_MUTE_PATH_RX = 0x02, /**< Sound mute path receiver */
	TAPI_SOUND_MUTE_PATH_ALL= 0x04, /**< Sound mute path all */
} TelSoundMutePath_t;

typedef enum {
	TAPI_SOUND_MUTE_STATUS_OFF = 0x00, /**< Sound mute status off */
	TAPI_SOUND_MUTE_STATUS_ON  = 0x01, /**< Sound mute status on */
} TelSoundMuteStatus_t;

typedef enum {
	TAPI_SOUND_NOISE_REDUCTION_OFF, /**< Sound noise reduction off */
	TAPI_SOUND_NOISE_REDUCTION_ON,  /**< Sound noise reduction on */
} TelSoundNoiseReduction_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallAnswerCnf_t;

typedef struct {
	TelCallEndType_t type;	/**< TBD */
	unsigned int id;	/**< TBD */
} TelCallEndCnf_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallHoldCnf_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallActiveCnf_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallSwapCnf_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallJoinCnf_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallSplitCnf_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallTransferCnf_t;

typedef struct {
	unsigned int record_num;	/**< TBD */
	TelCallVolumeInfo_t record[ 20 ];	/**< TBD */
} TelCallGetVolumeInfoResp_t;

typedef struct {
	TelSoundMutePath_t path;	/**< TBD */
	TelSoundMuteStatus_t status;	/**< TBD */
} TelCallGetMuteStatusResp_t;

typedef struct {
	TelCallPrivacyMode_t mode;
} TelCallGetPrivacyModeResp_t;

typedef struct {
	unsigned int id;	/**< TBD */
	TelTapiEndCause_t cause; /**< End cause for the call indicates whether the call is released normally or due to some other cause */
} TelCallStatusIdleNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallStatusActiveNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallStatusHeldNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallStatusDialingNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallStatusAlertNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallStatusIncomingNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallStatusWaitingNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoWaitingNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoForwardedNoti_t;

typedef struct {
	unsigned int id;
} TelCallInfoForwardedCallNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoBarredIncomingNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoBarredOutgoingNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoForwardConditionalNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoForwardUnconditionalNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoActiveNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoHeldNoti_t;

typedef struct {
	unsigned int id;	/**< TBD */
} TelCallInfoJoinedNoti_t;

typedef struct {
	TelCallOtaspStatus_t otasp_status;
} TelCallOtaspStatusNoti_t;

typedef struct {
	TelCallOtapaStatus_t otapa_status;
} TelCallOtapaStatusNoti_t;


typedef struct {
	TelCallPrivacyMode_t privacy_mode; /**< Voice Privacy Mode */
} TelCallVoicePrivacyNoti_t;


typedef struct {
	TelCallRecordInfo_t info;
} TelCallRecordInfoNoti_t;

typedef struct {
	TelCallAlertSignalType_t signal_type;
	TelCallAlertPitchType_t pitch_type;
	union {
		TelCallToneSignal_t sig_tone_type;
		TelCallIsdnAlertSignal_t sig_isdn_alert_type;
		TelCallIs54bAlertSignal_t sig_is54b_alert_type;
	} signal;
} TelCallSignalInfoNoti_t;

typedef struct {
	TelSoundPath_t path;	/**< TBD */
} TelCallSoundPathNoti_t;

typedef enum {
	TAPI_CALL_SOUND_RINGBACK_TONE_END,	/**< TBD */
	TAPI_CALL_SOUND_RINGBACK_TONE_START,	/**< TBD */
} TelCallSoundRingbackToneNoti_t;

typedef enum {
	TAPI_CALL_SOUND_WBAMR_STATUS_OFF,	/**< TBD */
	TAPI_CALL_SOUND_WBAMR_STATUS_ON,	/**< TBD */
	TAPI_CALL_SOUND_WBAMR_STATUS_OFF_16K,	/**< TBD */
	TAPI_CALL_SOUND_WBAMR_STATUS_ON_8K,	/**< TBD */
} TelCallSoundWbamrNoti_t;

typedef struct {
	TelSoundNoiseReduction_t status;	/**< TBD */
} TelCallSoundNoiseReductionNoti_t;

typedef struct {
	TelCallPreferredVoiceSubs_t preferred_subs;
} TelCallPreferredVoiceSubsNoti_t;

#ifdef __cplusplus
}
#endif

#endif  // _TEL_CALL_H_

/**
 * @}
 */
