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
* @addtogroup	NetText_TAPI    NetText(SMS)
* @{
*
* @file TelNetText.h

     @brief This file serves as a "C" header file defines structures for Tapi Network Text Services. \n
      It contains a sample set of constants, enums, structs that would be required by applications.

 */
#ifndef _TEL_NETTEXT_H_
#define _TEL_NETTEXT_H_

#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* NetText */
/** EF-SMSP digit length */
#define TAPI_SIM_SMSP_ADDRESS_LEN		20
/** EF-SMSP alpha id length */
#define TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX		128

#define TAPI_NETTEXT_MSG_SIZE_MAX		918 /**< Maximum Message Size */
#define TAPI_NETTEXT_CB_SIZE_MAX			93 /** Maximum CB Message Size */
#define TAPI_NETTEXT_ETWS_SIZE_MAX			56 /** Maximum ETWS Message Size */
#define TAPI_NETTEXT_ADDRESS_LEN_MAX	20 /* Nettext Address Length */
#define TAPI_NETTEXT_SCADDRESS_LEN_MAX	18 /* SC Address Length */

#define TAPI_NETTEXT_CB_PAGE_SIZE_MAX				 9    /**< CB maximum page size*/
#define TAPI_NETTEXT_GSM_SMS_MSG_NUM_MAX             255    /**< Maximum GSM SMS message number*/
#define TAPI_NETTEXT_GSM_SMS_CBMI_LIST_SIZE_MAX      50    /**< Maximum GSM SMS CBMI list size*/
#define TAPI_NETTEXT_SMDATA_SIZE_MAX                           165  /**< Maximum SMS data size that can be stored*/
#define TAPI_NETTEXT_MAX_SMS_SERVICE_CENTER_ADDR   12    /**<Maximum SMS service center address*/
#define TAPI_NETTEXT_MAX_INDEX                                        255 /**< Maximum index value for SMS */

#define TAPI_NETTEXT_SMSP_PARAMS_MAX_LEN		28

// ************************  CDMA Features  **************************//
#define TAPI_NETTEXT_PARAM_TELESERVICE_MASK			1 << 0				/**< Teleservice parameter bit position */
#define TAPI_NETTEXT_PARAM_SERVICE_CATEGORY_MASK		1 << 1				/**< Service Category parameter bit position */
#define TAPI_NETTEXT_PARAM_ADDRESS_MASK				1 << 2				/**< Address parameter bit position */
#define TAPI_NETTEXT_PARAM_SUBADDRESS_MASK			1 << 3				/**< Subaddress parameter bit position */
#define TAPI_NETTEXT_PARAM_BEARER_REPLY_MASK			1 << 4				/**< Bearer reply parameter bit position */
#define TAPI_NETTEXT_PARAM_CAUSE_CODES_MASK			1 << 5				/**< Cause Code parameter bit position */
#define TAPI_NETTEXT_PARAM_MESSAGE_ID_MASK			1 << 6				/**< Message ID parameter bit position */
#define TAPI_NETTEXT_PARAM_USER_DATA_MASK			1 << 7				/**< User Data parameter bit position */
#define TAPI_NETTEXT_PARAM_USER_RESPONSE_CODE_MASK		1 << 8				/**< User Response Code parameter bit position */
#define TAPI_NETTEXT_PARAM_MC_TIME_STAMP_MASK			1 << 9				/**< Message Center Timestamp parameter bit position */
#define TAPI_NETTEXT_PARAM_VALIDITY_PERIOD_ABS_MASK		1 << 10				/**< Valid period Absolute parameter bit position */
#define TAPI_NETTEXT_PARAM_VALIDITY_PERIOD_REL_MASK			1 << 11				/**< Valid period Relative parameter bit position */
#define TAPI_NETTEXT_PARAM_DEFERRED_DELIVERY_ABS_MASK		1 << 12				/**< Deferred delivery absolute parameter bit position */
#define TAPI_NETTEXT_PARAM_DEFERRED_DELIVERY_REL_MASK		1 << 13				/**< Deferred delivery relative parameter bit position */
#define TAPI_NETTEXT_PARAM_PRIORITY_MASK				1 << 14				/**< Priority parameter bit position */
#define TAPI_NETTEXT_PARAM_PRIVACY_MASK				1 << 15				/**< Privacy parameter bit position */
#define TAPI_NETTEXT_PARAM_REPLY_OPTION_MASK			1 << 16				/**< Reply Option parameter bit position */
#define TAPI_NETTEXT_PARAM_NUMBER_OF_MESSAGE_MASK		1 << 17				/**< Number of message parameter bit position */
#define TAPI_NETTEXT_PARAM_ALERT_ON_DELIVERY_MASK			1 << 18				/**< Alert on delivery parameter bit position */
#define TAPI_NETTEXT_PARAM_LANGUAGE_MASK			1 << 19				/**< Language parameter bit position */
#define TAPI_NETTEXT_PARAM_CALLBACK_MASK				1 << 20				/**< Callback Number parameter bit position */
#define TAPI_NETTEXT_PARAM_DISPLAY_MODE_MASK			1 << 21				/**< Display mode parameter bit position */
#define TAPI_NETTEXT_PARAM_MULTI_ENCODING_USER_DATA_MASK	1 << 22				/**< Multi Encoding user data parameter bit position */

#define TAPI_NETTEXT_MAXLENGTH_SMS_MT_USER_DATA				160					/**< Maximum MT user data  size*/
#define TAPI_NETTEXT_MAXLENGTH_SMS_MO_USER_DATA				160					/**< Maximum MO user data  size*/

#define TAPI_NETTEXT_MAXLENGTH_SMS_ADDRESS						32					/**< MAX sms destination(or origination ) address /call back number */

#define TAPI_NETTEXT_ERR_CLASS0_STATUS_SEND_OK								0	/**< send success */
#define TAPI_NETTEXT_ERR_CLASS23_ADDRESS_VACANT								0	/**< address vacant */
#define TAPI_NETTEXT_ERR_CLASS23_ADDRESS_TRANSLATION_FAILURE					1	/**< address translation failure */
#define TAPI_NETTEXT_ERR_CLASS23_NETWORK_RESOURCE_SHORTAGE					2	/**< network resource shortage */
#define TAPI_NETTEXT_ERR_CLASS23_NETWORK_FAILURE								3	/**< network failure */
#define TAPI_NETTEXT_ERR_CLASS23_INVALID_TELESERVICE_ID							4	/**< invalid teleservice id */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_NETWORK_PROBLEM						5	/**< other network problem */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_NETWORK_PROBLEM_MORE_FIRST			6	/**< other network problem more first */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_NETWORK_PROBLEM_MORE_LAST			31	/**< other network problem more last */
#define TAPI_NETTEXT_ERR_CLASS23_NO_PAGE_RESPONSE								32	/**< no page response */
#define TAPI_NETTEXT_ERR_CLASS23_DESTINATION_BUSY								33	/**< destination busy */
#define TAPI_NETTEXT_ERR_CLASS23_NO_ACK											34	/**< no ack */
#define TAPI_NETTEXT_ERR_CLASS23_DESTINATION_RESOURCE_SHORTAGE				35	/**< destination resource shortage */
#define TAPI_NETTEXT_ERR_CLASS23_SMS_DELIVERY_POSTPONED						36	/**< sms delivery postponed */
#define TAPI_NETTEXT_ERR_CLASS23_DESTINATION_OUT_OF_SERVICE					37	/**< destination out of service */
#define TAPI_NETTEXT_ERR_CLASS23_DESTINATION_NO_LONGER_AT_THIS_ADDRESS		38	/**< destination no longer at this address */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_TERMINAL_PROBLEM						39	/**< other terminal problem */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_TERMINAL_PROBLEM_MORE_FIRST			40	/**< other terminal problem more first */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_TERMINAL_PROBLEM_MORE_LAST			47	/**< other terminal problem more last */
#define TAPI_NETTEXT_ERR_CLASS23_SMS_DELIVERY_POSTPONED_MORE_FIRST			48	/**< sms delivery postpone more first */
#define TAPI_NETTEXT_ERR_CLASS23_SMS_DELIVERY_POSTPONED_MORE_LAST			63	/**< sms delivery postpone more last */
#define TAPI_NETTEXT_ERR_CLASS23_RADIO_IF_RESOURCE_SHORTAGE					64	/**< radio interface resource shortage */
#define TAPI_NETTEXT_ERR_CLASS23_RADIO_IF_INCOMPATIBLE							65	/**< radio interface incompatible */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_RADIO_IF_PROBLEM						66	/**< other radio interface problem */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_RADIO_IF_PROBLEM_MORE_FIRST			67	/**< other radio interface problem more first */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_RADIO_IF_PROBLEM_MORE_LAST			95	/**< other radio interface problem more last */
#define TAPI_NETTEXT_ERR_CLASS23_UNEXPECTED_PARM_SIZE							96	/**< unexpected parameter size */
#define TAPI_NETTEXT_ERR_CLASS23_SMS_ORIGINATION_DENIED						97	/**< sms origination denied */
#define TAPI_NETTEXT_ERR_CLASS23_SMS_TERMINATION_DENIED						98	/**< sms termination denied */
#define TAPI_NETTEXT_ERR_CLASS23_SUPPL_SERVICE_NOT_SUPPORTED					99	/**< supplementary service not supported */
#define TAPI_NETTEXT_ERR_CLASS23_SMS_NOT_SUPPORTED								100	/**< sms not supported */
#define TAPI_NETTEXT_ERR_CLASS23_RESERVED_101									101	/**< reserved 101 */
#define TAPI_NETTEXT_ERR_CLASS23_MISSING_EXPECTED_PARM						102	/**< missing expected parameter */
#define TAPI_NETTEXT_ERR_CLASS23_MISSING_MANDATORY_PARM						103	/**< missing mandatory parameter value */
#define TAPI_NETTEXT_ERR_CLASS23_UNRECOGNIZED_PARM_VALUE						104	/**< unrecognized parameter value */
#define TAPI_NETTEXT_ERR_CLASS23_UNEXPECTED_PARM_VALUE						105	/**< unexpected parameter value */
#define TAPI_NETTEXT_ERR_CLASS23_USER_DATA_SIZE_ERROR							106	/**< user data size error */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_GENERAL_PROBLEMS						107	/**< other general problem */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_GENERAL_PROBLEMS_MORE_FIRST			108	/**< other general problem first */
#define TAPI_NETTEXT_ERR_CLASS23_OTHER_GENERAL_PROBLEMS_MORE_LAST			255	/**< other general problem last */

#define TAPI_NETTEXT_ERR_CLASS4_WAITING_FOR_TL_ACK								1	/**< waiting for transport ack */
#define TAPI_NETTEXT_ERR_CLASS4_OUT_OF_RESOURCES								2	/**< out of resource */
#define TAPI_NETTEXT_ERR_CLASS4_ACCESS_TOO_LARGE								3	/**< access to large */
#define TAPI_NETTEXT_ERR_CLASS4_DTC_TOO_LARGE								4	/**< DTC too large */
#define TAPI_NETTEXT_ERR_CLASS4_DTC_CONNECTED									5	/**< DTC connected */
#define TAPI_NETTEXT_ERR_CLASS4_NETWORK_NOT_READY								6	/**< network not ready */
#define TAPI_NETTEXT_ERR_CLASS4_NO_SVC											7	/**< no service */
#define TAPI_NETTEXT_ERR_CLASS4_PHONE_NOT_READY								8	/**< phone not ready */
#define TAPI_NETTEXT_ERR_CLASS4_NOT_ALLOWED_IN_AMPS							9	/**< fail to allowed in amps */
#define TAPI_NETTEXT_ERR_CLASS4_CANNOT_SEND_BROADCAST							10	/**< fail to send broadcast */
#define TAPI_NETTEXT_ERR_CLASS4_INVALID_TRANSACTION_ID
// ********************************************************************//

/**
 * @enum TelSmsMsgStatus_t
 * This enumeration defines the network text status type.
 */
typedef enum
{
	TAPI_NETTEXT_STATUS_READ,					/**< MT message, stored and read */
	TAPI_NETTEXT_STATUS_UNREAD,					/**< MT message, stored and unread */
	TAPI_NETTEXT_STATUS_SENT,					/**< MO message, stored and  sent */
	TAPI_NETTEXT_STATUS_UNSENT,					/**< MO message, stored but not sent */
	TAPI_NETTEXT_STATUS_DELIVERED,                         /**< delivered destination */
	TAPI_NETTEXT_STATUS_DELIVERY_UNCONFIRMED,  /**< Service centre forwarded message but is unable to confirm delivery*/
	TAPI_NETTEXT_STATUS_MESSAGE_REPLACED,          /**< Message has been replaced*/
	TAPI_NETTEXT_STATUS_RESERVED                            /**< Reserved for future use*/
} TelSmsMsgStatus_t;

/**
 * @enum TelSmsMemStatusType
 * This enumeration defines the memory status type.
 */
typedef enum
{
	TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE		= 0x01, /**< PDA Memory Available */
	TAPI_NETTEXT_PDA_MEMORY_STATUS_FULL			= 0x02,  /**< PDAMemory is Full */
	TAPI_NETTEXT_PHONE_MEMORY_STATUS_AVAILABLE	= 0x03, /**< Phone memory Available */
	TAPI_NETTEXT_PHONE_MEMORY_STATUS_FULL			= 0x04, /**< phone memory is full */
}TelSmsMemStatusType;

/**
 * @enum TelSmsBearerType_t
 * This enum defines the different bearer types
 */
typedef enum
{
	TAPI_NETTEXT_BEARER_PS_ONLY = 0x01,       /**< Send SMS only on PS Bearer */
	TAPI_NETTEXT_BEARER_CS_ONLY,                  /**< Send SMS only on CS Bearer */
	TAPI_NETTEXT_BEARER_PS_PREFERRED,        /**<Send SMS preferably on PS Bearer*/
	TAPI_NETTEXT_BEARER_CS_PREFERRED ,       /**<Send SMS preferably on CS Bearer*/
	TAPI_NETTEXT_NO_PREFERRED_BEARER         /**<SMS is sent based on default preferred bearer set at OEM based on vendor/operator preference*/
} TelSmsBearerType_t;


/**
* @enum TelSmsCbMsgType_t
* This enumeration defines the different CB message types.
*/
typedef enum
{
		TAPI_NETTEXT_CB_MSG_GSM = 1,		/**< GSM Cell broadcast message */
		TAPI_NETTEXT_CB_MSG_UMTS			/**< UMTSCell broadcast message */
} TelSmsCbMsgType_t;

/**
* @enum TelSmsEtwsMsgType_t
* This enumeration defines the different ETWS message types.
*/
typedef enum
{
	   TAPI_NETTEXT_ETWS_PRIMARY = 0,             /**< Primary ETWS message */
	   TAPI_NETTEXT_ETWS_SECONDARY_GSM,            /**< GSM Secondary ETWS message  */
	   TAPI_NETTEXT_ETWS_SECONDARY_UMTS,               /**< UMTS Secondary ETWS message  */
} TelSmsEtwsMsgType_t;

/**
 * @enum TelSmsResponse_t
 * This enum defines the different response types that come in the
 * sent status acknowledgement/notification after sending a message to the network
 */
typedef enum
{
	TAPI_NETTEXT_SENDSMS_SUCCESS = 0x00,     /**<Message send success*/
	TAPI_NETTEXT_ROUTING_NOT_AVAILABLE,     /**< Message routing not available*/
	TAPI_NETTEXT_INVALID_PARAMETER,             /**< Invalid parameter present in TPDU*/
	TAPI_NETTEXT_DEVICE_FAILURE,                    /**< Device failure*/
	TAPI_NETTEXT_SERVICE_RESERVED,                /**< Reserved Service*/
	TAPI_NETTEXT_INVALID_LOCATION,                /**< Invalid location*/
	TAPI_NETTEXT_NO_SIM,                                    /**< No SIM error*/
	TAPI_NETTEXT_SIM_NOT_READY,            /**< SIM not ready error*/
	TAPI_NETTEXT_NO_NETWORK_RESP,          /**< No response from network*/
	TAPI_NETTEXT_DEST_ADDRESS_FDN_RESTRICTED,/**< Destination address restricted*/
	TAPI_NETTEXT_SCADDRESS_FDN_RESTRICTED,   /**< Service center address restricted*/
	TAPI_NETTEXT_RESEND_ALREADY_DONE,        /**< Resend a operation already done*/
	TAPI_NETTEXT_SCADDRESS_NOT_AVAILABLE,    /**< SCA address not available*/
	TAPI_NETTEXT_UNASSIGNED_NUMBER = 0x8001,          /**< Unassigned number*/
	TAPI_NETTEXT_OPERATOR_DETERMINED_BARRING = 0x8008,/**< Operator determined barring*/
	TAPI_NETTEXT_CALL_BARRED = 0x800A,                /**< Call barred*/
	TAPI_NETTEXT_MESSAGE_TRANSFER_REJECTED = 0x8015,  /**< Message transfer rejected*/
	TAPI_NETTEXT_MEMORY_CAPACITY_EXCEEDED = 0x8016,   /**< Memory capacity exceeded/memory full*/
	TAPI_NETTEXT_DESTINAITION_OUTOFSERVICE = 0x801B,  /**< Destination number/address out of service*/
	TAPI_NETTEXT_UNSPECIFIED_SUBSCRIBER = 0x801C,     /**< Unspecified subscriber*/
	TAPI_NETTEXT_FACILITY_REJECTED = 0x801D,          /**< Facility rejected*/
	TAPI_NETTEXT_UNKNOWN_SUBSCRIBER = 0x801E,         /**< Unknown subscriber*/
	TAPI_NETTEXT_NETWORK_OUTOFORDER = 0x8026,         /**< Network out of order*/
	TAPI_NETTEXT_TEMPORARY_FAILURE = 0x8029,          /**< Temporary failure*/
	TAPI_NETTEXT_CONGESTION = 0x802A,                 /**< Congestion happened*/
	TAPI_NETTEXT_RESOURCES_UNAVAILABLE = 0x802F,      /**< Resource unavailable*/
	TAPI_NETTEXT_FACILITY_NOT_SUBSCRIBED = 0x8032,    /**< Facility not subscribed by the user*/
	TAPI_NETTEXT_FACILITY_NOT_IMPLEMENTED = 0x8045,   /**< Facility not implemented*/
	TAPI_NETTEXT_INVALID_REF_VALUE = 0x8051,          /**< Invalid reference value*/
	TAPI_NETTEXT_INVALID_MSG = 0x805F,                /**< Invalid message*/
	TAPI_NETTEXT_INVALID_MANDATORY_INFO = 0x8060,     /**< Invalid Mandatory information*/
	TAPI_NETTEXT_MESSAGE_TYPE_NOT_IMPLEMENTED = 0x8061,/**< Message type not implemented*/
	TAPI_NETTEXT_MESSAGE_NOT_COMPAT_PROTOCOL = 0x8062, /**< Message not compact protocol*/
	TAPI_NETTEXT_IE_NOT_IMPLEMENTED = 0x8063,          /**< Information element not implemented*/
	TAPI_NETTEXT_PROTOCOL_ERROR = 0x806F,              /**< Protocol error*/
	TAPI_NETTEXT_INTERWORKING = 0x807F,              /**< Networking error*/
	TAPI_NETTEXT_ME_FULL = 0x8080,                     /**< SMS ME FULL */
	TAPI_NETTEXT_SIM_FULL = 0x8081,                     /**< SMS SIM FULL */
	TAPI_NETTEXT_TIMEOUT						/**< Timeout error */
}TelSmsResponse_t;

 /** @enum  TelSmsCause_t
 * This enum defines the different cause types that come in the
 * sent status acknowledgement/notification after sending a message to the network
 */

typedef enum
{

  TAPI_NETTEXT_SUCCESS,                     /**< SMS Operation successful*/
  TAPI_NETTEXT_INVALID_PARAMETER_FORMAT,    /**< Invalid format for some parameters passed in Data package information(TPDU)*/
  TAPI_NETTEXT_PHONE_FAILURE,               /**<SMS operation failed due to Modem failure*/
  TAPI_NETTEXT_SIM_BUSY,                    /**< SMS SIM operation cannot be performed as SIM is busy with some other operation*/
  TAPI_NETTEXT_SIM_FAILURE,                 /**< SMS SIM operation cannot be performed due to SIM failure */
  TAPI_NETTEXT_UNKNOWN,                     /**< unknown error*/
  TAPI_NETTEXT_MEMORY_FAILURE,              /**< Error while accessing memory or allocation of memory for SMS operation.*/
  TAPI_NETTEXT_OPERATION_NOT_SUPPORTED      /**< operation not allowed/supported*/
}  TelSmsCause_t;

/**
 * @enum TelSmsSetResponse
 * This defines the response values
 */
typedef enum
{
	TAPI_NETTEXT_CBSETCONFIG_RSP,     /**<  cellbroadcast config response */
	TAPI_NETTEXT_SETPREFERREDBEARER_RSP,     /**<  set preferred bearer response */
	TAPI_NETTEXT_SETPARAMETERS_RSP,     /**<  set parameter response */
	TAPI_NETTEXT_SETMEMORYSTATUS_RSP,     /**<   set memory status response*/
	TAPI_NETTEXT_SETMESSAGESTATUS_RSP,     /**<   set message status response*/
	TAPI_NETTEXT_SETDEVICESTATUS_RSP,    /**<   set device status response*/
	TAPI_NETTEXT_SETSCADDR_RSP,     /**<   set SCA address response */
	TAPI_NETTEXT_SET_RSP_MAX     /**<   maximum limit*/
}TelSmsSetResponse;

// ************************  CDMA Features  **************************//
 /** @enum  TelSmsIs637AlertPriority_t
 * This enum defines the type of alerts used to distinguish different priorities
 * of the message
 */
typedef enum{
  TAPI_NETTEXT_ALERT_PRIORITY_DEFAULT = 0,	/**< Mobile default alert */
  TAPI_NETTEXT_ALERT_PRIORITY_LOW,			/**< Low priority alert */
  TAPI_NETTEXT_ALERT_PRIORITY_MEDIUM,		/**< Medium priority alert */
  TAPI_NETTEXT_ALERT_PRIORITY_HIGH,			/**< High priority alert */
} TelSmsIs637AlertPriority_t;

 /** @enum  TelSmsIs637Privacy_t
 * This enum defines the desired privacy level of the message
 */
typedef enum{
  TAPI_NETTEXT_PRIVACY_NOT_RESTRICTED = 0,	/**< Not restricted */
  TAPI_NETTEXT_PRIVACY_RESTRICTED,			/**< Restricted */
  TAPI_NETTEXT_PRIVACY_CONFIDENTIAL,			/**< Confidential */
  TAPI_NETTEXT_PRIVACY_SECRET,				/**< Secret */
} TelSmsIs637Privacy_t;

 /** @enum  TelSmsIs637Priority_t
 * This enum defines the priority level of the message
 */
typedef enum{
  TAPI_NETTEXT_PRIORITY_NORMAL = 0,			/**< Normal */
  TAPI_NETTEXT_PRIORITY_INTERACTIVE,			/**< Interactive */
  TAPI_NETTEXT_PRIORITY_URGENT,				/**< Urgent */
  TAPI_NETTEXT_PRIORITY_EMERGENCY,			/**< Emergency */
} TelSmsIs637Priority_t;

 /** @enum  TelSmsIs637LangIndicator_t
 * This enum defines the language of the message so that the receiving mobile station can
 * discard those messages that are not in the user's preferred language
 */
typedef enum{
  TAPI_NETTEXT_LANG_UNKNOWN = 0x0,			/**< Unknown or unspecified */
  TAPI_NETTEXT_LANG_ENGLISH,					/**< English */
  TAPI_NETTEXT_LANG_FRENCH,					/**< French */
  TAPI_NETTEXT_LANG_SPANISH,					/**< Spanish */
  TAPI_NETTEXT_LANG_JAPANESE,					/**< Japanese */
  TAPI_NETTEXT_LANG_KOREAN,					/**< Korean */
  TAPI_NETTEXT_LANG_CHINESE,					/**< Chinese */
  TAPI_NETTEXT_LANG_HEBREW,					/**< Hebrew */
} TelSmsIs637LangIndicator_t;

 /** @enum  TelSmsIs637MsgDisplayMode_t
 * This enum defines the display mode to the mobile station when to display the received message
 */
typedef enum{
  TAPI_NETTEXT_MSG_DISPLAY_IMMEDIATE = 0x0,	/**< The mobile station is to display the received message as soon as possible */
  TAPI_NETTEXT_MSG_DISPLAY_DEFAULT,			/**< he mobile station is to display the received message based on a pre-defined mode in the mobile station. */
  TAPI_NETTEXT_MSG_DISPLAY_USER_INVOKE = 0x3, /**< The mobile station is to display the received message based on the mode selected by the user. */
  TAPI_NETTEXT_MSG_DISPLAY_RESERVED,			/**< Reserved */
} TelSmsIs637MsgDisplayMode_t;

 /** @enum  TelSmsIs637ErrorClass_t
 * This enum defines the error report class
 */
typedef enum{
  TAPI_NETTEXT_MSG_ERROR_CLASS_NONE = 0x0,					/**< None error(for SMS ack) */
  TAPI_NETTEXT_MSG_ERROR_CLASS_TEMPORARY_ERROR = 0x2,		/**< Temporary error(for SMS ack) */
  TAPI_NETTEXT_MSG_ERROR_CLASS_PERMANENT_ERROR = 0x3,		/**< Permanent error(for SMS ack) */
  TAPI_NETTEXT_MSG_ERROR_CLASS_PHONE_INTERNAL_ERROR = 0x4,	/**< Phone Internal Status (for Send status) */
} TelSmsIs637ErrorClass_t;

 /** @enum  TelSmsBroadCastCategory_t
 * This enum defines the Broadcast Service Category
 */
typedef enum{
  TAPI_NETTEXT_CATEGORY_UNKNOWN			= 0x00,			/**< Unknown category */
  TAPI_NETTEXT_CATEGORY_EMERGENCY			= 0x01,			/**< Emergency category */
  TAPI_NETTEXT_CATEGORY_ADMIN				= 0x02,			/**< Admin category */
  TAPI_NETTEXT_CATEGORY_MAINTENANCE		= 0x03,			/**< Maintenance category */
  TAPI_NETTEXT_CATEGORY_GEN_NEWS_LOC		= 0x04,			/**< General News(Local) category */
  TAPI_NETTEXT_CATEGORY_GEN_NEWS_REG		= 0x05,			/**< General News(Regional) category */
  TAPI_NETTEXT_CATEGORY_GEN_NEWS_NAT		= 0x06,			/**< General News(National) category */
  TAPI_NETTEXT_CATEGORY_GEN_NEWS_INT		= 0x07,			/**< General News(International) category */
  TAPI_NETTEXT_CATEGORY_FIN_NEWS_LOC		= 0x08,			/**< Business/Financial News(Local) category */
  TAPI_NETTEXT_CATEGORY_FIN_NEWS_REG		= 0x09,			/**< Business/Financial News(Regional) category */
  TAPI_NETTEXT_CATEGORY_FIN_NEWS_NAT		= 0x0A,			/**< Business/Financial News(National) category */
  TAPI_NETTEXT_CATEGORY_FIN_NEWS_INT		= 0x0B,			/**< Business/Financial News(International) category */
  TAPI_NETTEXT_CATEGORY_SPT_NEWS_LOC		= 0x0C,			/**< Sports News(Local) category */
  TAPI_NETTEXT_CATEGORY_SPT_NEWS_REG		= 0x0D,			/**< Sports News(Regional) category */
  TAPI_NETTEXT_CATEGORY_SPT_NEWS_NAT		= 0x0E,			/**< Sports News(National) category */
  TAPI_NETTEXT_CATEGORY_SPT_NEWS_INT		= 0x0F,			/**< Sports News(International) category */
  TAPI_NETTEXT_CATEGORY_ENT_NEWS_LOC		= 0x10,			/**< Entertainment News(Local) category */
  TAPI_NETTEXT_CATEGORY_ENT_NEWS_REG		= 0x11,			/**< Entertainment News(Regional) category */
  TAPI_NETTEXT_CATEGORY_ENT_NEWS_NAT		= 0x12,			/**< Entertainment News(National) category */
  TAPI_NETTEXT_CATEGORY_ENT_NEWS_INT		= 0x13,			/**< Entertainment News(International) category */
  TAPI_NETTEXT_CATEGORY_LOC_WEATHER		= 0x14,			/**< Local Weather category */
  TAPI_NETTEXT_CATEGORY_AREA_TRAFFIC		= 0x15,			/**< Area Traffic Reports category */
  TAPI_NETTEXT_CATEGORY_AIRPORT_SCHED		= 0x16,			/**< Local Airport Flight Schedules category */
  TAPI_NETTEXT_CATEGORY_RESTAURANTS		= 0x17,			/**< Restaurants category */
  TAPI_NETTEXT_CATEGORY_LODGINGS			= 0x18,			/**< Lodgings category */
  TAPI_NETTEXT_CATEGORY_RETAILS			= 0x19,			/**< Retail Directory category */
  TAPI_NETTEXT_CATEGORY_ADS				= 0x1A,			/**< Advertisements category */
  TAPI_NETTEXT_CATEGORY_STOCK_QUOTES		= 0x1B,			/**< Stock Quotes category */
  TAPI_NETTEXT_CATEGORY_JOBS				= 0x1C,			/**< Employment Opportunities category */
  TAPI_NETTEXT_CATEGORY_MEDICAL			= 0x1D,			/**< Medical/Health/Hospitals category */
  TAPI_NETTEXT_CATEGORY_TECH_NEWS			= 0x1E,			/**< Technology News category */
  TAPI_NETTEXT_CATEGORY_MULTI				=  0x1F,			/**< Multi-category */
} TelSmsBroadCastCategory_t;

/** @enum  TelSmsMsgType_t
 * This enum defines the type of IS637 message
 */
typedef enum{
  TAPI_NETTEXT_MESSAGETYPE_DELIVER = 0x01,					/**< sms deliver message  */
  TAPI_NETTEXT_MESSAGETYPE_SUBMIT = 0x02,					/**< sms submit message  */
  TAPI_NETTEXT_MESSAGETYPE_CANCEL = 0x03,					/**< sms cancellation message  */
  TAPI_NETTEXT_MESSAGETYPE_DELIVERY_ACK = 0x04,				/**< sms delivery acknowledgment message  */
  TAPI_NETTEXT_MESSAGETYPE_USER_ACK = 0x05,					/**< sms user acknowledgment message  */
} TelSmsMsgType_t;

/** @enum  TelSmsIs637DigitMode_t
 * This enum defines the type of address whether it is 4-bit mode or 8-bit mode
 */
typedef enum{
  TAPI_NETTEXT_DIGITMODE_4BITDTMF	= 0x00,					/**< 4-bit mode  */
  TAPI_NETTEXT_DIGITMODE_8BITCODE	= 0x01,					/**< 8-bit mode  */
} TelSmsIs637DigitMode_t;

/** @enum  TelSmsIs637NumberMode_t
 * This enum defines the mode of address number to indicate whether the address type is as defined in ANSI TI.607 or is a data network address
 */
typedef enum{
  TAPI_NETTEXT_NUMMODE_NONE_DATANETWORK =	0x00,		/**< in ANSI TI.607 */
  TAPI_NETTEXT_NUMMODE_DATANETWORK = 0x01,					/**< in Data Network */
} TelSmsIs637NumberMode_t;

/** @enum  TelSmsIs637NumberType_t
 * This enum defines the type of address
 */
typedef enum{
  TAPI_NETTEXT_NUMBER_TYPE_UNKNOWN          = 0x00,				/**<  Unknown */
  TAPI_NETTEXT_NUMBER_TYPE_INTERNATIONAL    = 0x01,			/**<  International number*/
  TAPI_NETTEXT_NUMBER_TYPE_NATIONAL         = 0x02,				/**<  National number */
  TAPI_NETTEXT_NUMBER_TYPE_NETWORK          = 0x03,				/**<  Abbreviated number */
  TAPI_NETTEXT_NUMBER_TYPE_SUBSCRIBER       = 0x04,			/**< Abbreviated number */
  TAPI_NETTEXT_NUMBER_TYPE_RESERVED_5       = 0x05,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_TYPE_ABBREVIATED       = 0x06,			/**<  Abbreviated number */
  TAPI_NETTEXT_NUMBER_TYPE_RESERVED_7       = 0x07,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_TYPE_IP			    = 0x11,			/**<  Internet Protocol(RFC 791) */
  TAPI_NETTEXT_NUMBER_TYPE_EMAILADDR	    = 0x12,			/**<  Internet Email Address(RFC 822) */
} TelSmsIs637NumberType_t;

/** @enum  TelSmsIs637NumberPlan_t
 * This enum defines the plan of address
 */
typedef enum{
  TAPI_NETTEXT_NUMBER_PLAN_UNKNOWN			    = 0x00,		/**<  Unknown */
  TAPI_NETTEXT_NUMBER_PLAN_TELEPHONY		    = 0x01,		/**< ISDN/Telephony numbering plan */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_2		    = 0x02,		/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_DATA			        = 0x03,			/**< Data numbering plan */
  TAPI_NETTEXT_NUMBER_PLAN_TELEX			      = 0x04,		/**< CTelex numbering plan */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_5		    = 0x05,		/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_6		    = 0x06,		/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_7		    = 0x07,		/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_8		    = 0x08,		/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_PRIVATE			    = 0x09,		/**<  Private numbering plan */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_10		  = 0x0A,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_11		  = 0x0B,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_12		  = 0x0C,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_13		  = 0x0D,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_14		  = 0x0E,			/**<  Reserved */
  TAPI_NETTEXT_NUMBER_PLAN_RESERVED_15		  = 0x0F,			/**<  Reserved */
} TelSmsIs637NumberPlan_t;

/** @enum  TelSmsIs637SubAddressType_t
 * This enum defines the plan of address
 */
typedef enum{
  TAPI_NETTEXT_SUBADDR_NSAP				          = 0x00,	/**< NSAP ( CCITT Recommendation X.213 or ISO 8348 AD2 ) */
  TAPI_NETTEXT_SUBADDR_USER_SPECIFIED		    = 0x01,	/**<  User-specified */
} TelSmsIs637SubAddressType_t;

/**
 * @enum telephony_sms_3gpp_type
 * This defines the type of 3gpp
 */
typedef enum  {
	TAPI_NETTEXT_NETTYPE_3GPP = 0x01,						/**< 3gpp type */
	TAPI_NETTEXT_NETTYPE_3GPP2 = 0x02,    					/**< 3gpp2 type (CDMA) */
} TelSms3gppType_t;

// ********************************************************************//

/**
 * This structure defines the different parameters of  CB configuration
 */
typedef struct {
 unsigned short FromMsgId; /**< Starting point of the range of CBS message ID */
 unsigned short ToMsgId; /**< Ending point of the range of CBS message ID */
 unsigned char Selected; /**< 0x00 . Not selected. 0x01 . Selected */
} TelSmsCbMsgInfo3gpp_t;

typedef struct {
 unsigned short CBCategory; /**< CB Service category */
 unsigned short CBLanguage; /**< Language indicator value
 								. 0x00 . LANGUAGE_UNKNOWN .
									Unknown or Unspecified
								. 0x01 . LANGUAGE_ENGLISH . English
								. 0x02 . LANGUAGE_FRENCH . French
								. 0x03 . LANGUAGE_SPANISH . Spanish
								. 0x04 . LANGUAGE_JAPANESE . Japanese
								. 0x05 . LANGUAGE_KOREAN . Korean
								. 0x06 . LANGUAGE_CHINESE . Chinese
								. 0x07 . LANGUAGE_HEBREW . Hebrew*/
 unsigned char Selected; /**< 0x00 . Not selected. 0x01 . Selected */
} TelSmsCbMsgInfo3gpp2_t;

typedef union {
	TelSmsCbMsgInfo3gpp_t Net3gpp; /**< 3GPP Broadcast Configuration Information */
	TelSmsCbMsgInfo3gpp2_t Net3gpp2; /**< 3GPP2 Broadcast Configuration Information, CDMA*/
} TelSmsCbMsgInfo_t;

typedef struct {
	int Net3gppType;  /**< Type of 3gpp, 0x01 . 3gpp. 0x02 . 3gpp2(CDMA) */
	int CBEnabled; /**< CB service state. If cb_enabled is true then cell broadcast service will be enabled and underlying modem will enable CB Channel to receiving CB messages. Otherwise CB service will be disabled, underlying modem will deactivate the CB channel. (enabled/disabled) */
	unsigned char MsgIdMaxCount; /**< CB Channel List Max Count For Response */
	int MsgIdRangeCount; /**< Range of CB message ID count */
	TelSmsCbMsgInfo_t MsgIDs[TAPI_NETTEXT_GSM_SMS_CBMI_LIST_SIZE_MAX]; /**< Range of CB message ID information */
} TelSmsCbConfig_t;

/**
 * This structure defines the properties of a dialing number
 * Type of Number, Numbering Plan Indicator, length and the actual number.
 */
typedef struct {
	unsigned int DialNumLen; /**< length of address. If Service center address is not set, then this will be zero */
	TelSimTypeOfNum_t Ton; /**< Type of number*/
	TelSimNumberingPlanIdentity_t Npi; /**<Numbering plan identification*/
	unsigned char szDiallingNum[TAPI_SIM_SMSP_ADDRESS_LEN + 1]; /**< destination address. If Address not set, then set to 0xFF */
} TelSmsAddressInfo_t;

/**
 * This structure defines different fields involved in setting the parameters of
 * a particular sms in EFsmsp.
 */
typedef struct {

	unsigned char RecordIndex; /**< Index*/
	unsigned char RecordLen; /**< SMS Parameter Record length*/
	unsigned long AlphaIdLen; /**< Alpha id length */
	char szAlphaId[TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1]; /**< Alpha id .It is a unique identifier for each row/record in EFsmsp */
	unsigned char ParamIndicator; /**< SMS parameter indicator is a 1 byte value. Each bit in this value indicates the presence/absence of the sms header parameters.If the parameter is present the corresponding bit value is set to 0.If the parameter is absent then it is set as 1.Refer 3GPP TS 31.02 :4.4.23 */
	TelSmsAddressInfo_t TpDestAddr; /**< TP-destination address (TP-DA) */
	TelSmsAddressInfo_t TpSvcCntrAddr; /**< TP-service center address */
	unsigned short TpProtocolId; /**< TP-protocol Id */
	unsigned short TpDataCodingScheme; /**< TP-data coding scheme */
	unsigned short TpValidityPeriod; /**< TP-validity period */
} TelSmsParams_t;

/**
 * This structure defines the fields related to an Sms like SIM index, TPDU  and length
 */
typedef struct {
	unsigned char Sca[TAPI_SIM_SMSP_ADDRESS_LEN]; /**< Service Centre address - an optional parameter. If this parameter is not present, then this field will be Null.If present, the valid service centre address information is filled as per 3GPP TS23.040 9.1.2.5 Address fields */
	int MsgLength; /**< Size of array szData (which is actual TPDU message) */
	unsigned char szData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1]; /**<SMS TPDU message */
} TelSmsDatapackageInfo_t;

/**
 *This structure defines the data Related to SimIndex,MessageStatus and SMS Data Stored.
 */
typedef struct {
	int SimIndex; /**< Index where SMS is stored. */
	TelSmsMsgStatus_t MsgStatus; /**< Message status.*/
	TelSmsDatapackageInfo_t SmsData; /**<SMS message */
} TelSmsData_t;

/**
 * This structure defines the different parameters that are related to the message count
 *in a particular memory(Phone/SIM)
 */
typedef struct {
	unsigned int TotalCount; /**< Total count of messages stored in SIM*/
	int UsedCount; /**< Stored message count in SIM in SIM*/
	int IndexList[TAPI_NETTEXT_GSM_SMS_MSG_NUM_MAX]; /**< Message index list stored in SIM. And the maximum size of this array can be of totalCount.This array contains the list of index values in which the messages are stored.i.e. index_list[totalcount] = [2,3] denotes that indexs 2 and 3 are stored(valid) and others not(empty).*/
} TelSmsStoredMsgCountInfo_t;

/**
 * This structure defines a cell broadcast message.
 */
typedef struct {

	TelSmsCbMsgType_t CbMsgType; /**< Cell Broadcast  message type */
	unsigned short Length; /**<Size of array szMsgData (which is actual TPDU message) */
	char szMsgData[TAPI_NETTEXT_CB_SIZE_MAX + 1]; /**<Cell broadcast message data[Refer 3GPP TS 23.041 9.4.1]*/
} TelSmsCbMsg_t;

/**
 * This structure defines a ETWS message.
 */
typedef struct {
	TelSmsEtwsMsgType_t EtwsMsgType; /**< Cell Broadcast  message type */
	unsigned short Length; /**<Size of array szMsgData (which is actual TPDU message) */
	char szMsgData[TAPI_NETTEXT_ETWS_SIZE_MAX + 1]; /**<Cell broadcast message data[Refer 3GPP TS 23.041 9.4.1]*/
} TelSmsEtwsMsg_t;

// ************************  CDMA Features  **************************//
/**
 *  This structure defines cause code as an indication whether an SMS error has occurred and
 *  if so, whether the condition is considered temporary or permanent
 **/
typedef struct {
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	TelSmsIs637ErrorClass_t ErrClass; /**< Error report class */
	unsigned char Cause; /**< Error cause identifier */
} TelSmsIs637CauseCode_t;

/**
 * This structure defines the parameters of address and its type
 */
typedef struct {
	TelSmsIs637DigitMode_t Digit; /**< Digit mode indicator (0x00:4bit_dtmf, 0x01:8bit_code) */
	TelSmsIs637NumberMode_t NumberMode; /**< Number mode indicator (0x00:ANSI TI607, 0x01:Data network) */
	TelSmsIs637NumberType_t NumberType; /**< Type of number */
	TelSmsIs637NumberPlan_t NumberPlan; /**< Numbering plan */
	unsigned char szAddrLength; /**< The number of CHARi */
	unsigned char szAddress[TAPI_NETTEXT_MAXLENGTH_SMS_ADDRESS]; /**< The address digit or character */
} TelSmsIs637Address_t;

/**
 * This structure defines the parameters of subaddress of originator and destination
 */
typedef struct {
	TelSmsIs637SubAddressType_t SubType; /**< Subaddress type */
	unsigned char Odd; /**< Odd/even indicator */
	unsigned char szAddrLength; /**< The number of CHARi */
	unsigned char szAddress[TAPI_NETTEXT_MAXLENGTH_SMS_ADDRESS]; /**< A subaddress octet */
} TelSmsIs637SubAddress_t;

/**
 * This structure defines the message center time stamp may be include
 * with SMS message sent from a message center
 */
typedef struct {
	unsigned int year; /**< Year - if the year is 2002, the year field contains 0x02 */
	unsigned int month; /**< Month (1 ~ 12) */
	unsigned int day; /**< Day (1 ~ 31) */
	unsigned int hours; /**< Hours (0 ~ 23) */
	unsigned int minutes; /**< Minute (0 ~ 59) */
	unsigned int seconds; /**< Seconds (0 ~ 59) */
} TelSmsTimeStamp_t;

/**
 * The structure defines the parameter of SMS submit message
 */
typedef struct {
	TelSmsIs637Address_t DstAddr; /**< Destination address */
	TelSmsIs637SubAddress_t DstSubAddr; /**< Destination subaddress */
	unsigned short TeleService; /**< Teleservice Identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgId; /**< Message identifier */
	unsigned short MsgEncoding; /**< Message encoding (defined in TSB-58A) */
	unsigned short MsgLength; /**< Message length */
	unsigned char szData[TAPI_NETTEXT_MAXLENGTH_SMS_MO_USER_DATA]; /**< SMS user data */
	TelSmsTimeStamp_t ValidityPeriodAbs; /**< Validity period - Absolute (not supported) */
	unsigned char ValidityPeriodRel; /**< Validity period - Relative (not supported) */
	TelSmsTimeStamp_t DeferredDelTimeAbs; /**< Deferred delivery time - Absolute */
	unsigned char DeferredDelTimeRel; /**< Deferred delivery time - Relative */
	TelSmsIs637Priority_t Priority; /**< Priority indicator */
	TelSmsIs637Privacy_t Privacy; /**< Privacy indicator */
	int bUserAckRequest; /**< User acknowledge request */
	int bDeliveryAckRequest; /**< Delivery acknowledge request */
	TelSmsIs637AlertPriority_t AlertPriority; /**< Alert priority of message */
	TelSmsIs637LangIndicator_t MsgLang; /**< Language indicator */
	TelSmsIs637Address_t CallBackNumber; /**< Callback number address */
} TelSmsIs637OutSubmit_t;

/**
 * The structure defines the parameter of SMS acknowledgement message for submit
 */
typedef struct {
	TelSmsIs637Address_t DstAddr; /**< Destination address */
	TelSmsIs637SubAddress_t DstSubAddr; /**< Destination subaddress */
	unsigned short TeleService; /**< Teleservice Identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgId; /**< Message identifier */
	unsigned short MsgEncoding; /**< Message encoding (defined in TSB-58A) */
	unsigned short MsgLength; /**< Message length */
	unsigned char szData[TAPI_NETTEXT_MAXLENGTH_SMS_MO_USER_DATA]; /**< SMS user data */
	unsigned char UserResponseCode; /**< User response code */
} TelSmsIs637OutAck_t;

/**
 * The structure defines the parameter of SMS cancellation message
 */
typedef struct {
	TelSmsIs637Address_t DstAddr; /**< Destination address */
	TelSmsIs637SubAddress_t DstSubAddr; /**< Destination subaddress */
	unsigned short TeleService; /**< Teleservice Identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgId; /**< Message identifier */
} TelSmsIs637OutCancel_t;

/**
 * The structure defines the parameter of SMS devivery message
 */
typedef struct {
	TelSmsIs637Address_t OrigAddr; /**< Origination address */
	TelSmsIs637SubAddress_t OrigSubAddr; /**< Origination subaddress */
	unsigned short TeleService; /**< Teleservice Identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgId; /**< Message identifier */
	unsigned short MsgEncoding; /**< Message encoding (defined in TSB-58A) */
	unsigned short MsgLength; /**< Message length */
	unsigned char szData[TAPI_NETTEXT_MAXLENGTH_SMS_MT_USER_DATA]; /**< SMS user data */
	TelSmsTimeStamp_t MessageCenterTimeStamp; /**< Message center time stamp */
	TelSmsTimeStamp_t ValidityPeriodAbs; /**< Validity period - Absolute */
	unsigned char ValidityPeriodRel; /**< Validity period - Relative */
	TelSmsTimeStamp_t DeferredDelTimeAbs; /**< Deferred delivery time - Absolute (not supported) */
	unsigned char DeferredDelTimeRel; /**< Deferred delivery time - Relative (not supported) */
	TelSmsIs637Priority_t Priority; /**< Priority indicator */
	TelSmsIs637Privacy_t Privacy; /**< Privacy indicator */
	unsigned char NumMsg; /**< Number of voice mail (0-99) */
	int bUserAckRequest; /**< User acknowledge request */
	int bDeliveryAckRequest; /**< Delivery acknowledge request */
	TelSmsIs637AlertPriority_t AlertPriorty; /**< Alert priority of message */
	TelSmsIs637LangIndicator_t MsgLang; /**< Language indicator */
	TelSmsIs637Address_t CallBackNumer; /**< Callback number address */
	TelSmsIs637MsgDisplayMode_t Display; /**< Message display mode */
} TelSmsIs637InDeliver_t;

/**
 * The structure defines the parameter of SMS acknowledge message for deliver
 */
typedef struct {
	TelSmsIs637Address_t OrigAddr; /**< Origination address */
	TelSmsIs637SubAddress_t OrigSubAddr; /**< Origination subaddress */
	unsigned short TeleService; /**< Teleservice Identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgId; /**< Message identifier */
	unsigned short MsgEncoding; /**< Message encoding (defined in TSB-58A) */
	unsigned short MsgLength; /**< Message length */
	unsigned char szData[TAPI_NETTEXT_MAXLENGTH_SMS_MT_USER_DATA]; /**< SMS user data */
	unsigned char UserResponseCode; /**< User response code */
	TelSmsTimeStamp_t MessageCenterTimeStamp; /**< Message center time stamp */
} TelSmsIs637InAck_t;

/**
 * The structure defines the parameter of SMS acknowledge message from message center
 */
typedef struct {
	TelSmsIs637Address_t OrigAddr; /**< Origination address */
	TelSmsIs637SubAddress_t OrigSubAddr; /**< Origination subaddress */
	unsigned short TeleService; /**< Teleservice Identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgId; /**< Message identifier */
	unsigned short MsgEncoding; /**< Message encoding (defined in TSB-58A) */
	unsigned short MsgLength; /**< Message length */
	unsigned char szData[TAPI_NETTEXT_MAXLENGTH_SMS_MT_USER_DATA]; /**< SMS user data */
	TelSmsTimeStamp_t MessageCenterTimeStamp; /**< Message center time stamp */
} TelSmsIs637InDeliverAck_t;

/**
 * The structure defines the parameter of SMS broadcast message
 */
typedef struct {
	TelSmsBroadCastCategory_t ServiceCategory; /**< Broadcast service category */
	unsigned short MsgId; /**< Message identifier */
	int bBearerReplySeqRequest; /**< Bearer reply sequence request */
	unsigned char ReplySeqNumber; /**< Reply sequence number */
	unsigned short MsgEncoding; /**< Message encoding (defined in TSB-58A) */
	unsigned short MsgLength; /**< Message length */
	unsigned char szData[TAPI_NETTEXT_MAXLENGTH_SMS_MT_USER_DATA]; /**< SMS user data */
	TelSmsTimeStamp_t MessageCenterTimeStamp; /**< Message center time stamp */
	TelSmsTimeStamp_t ValidityPeriodAbs; /**< Validity period - Absolute */
	unsigned char ValidityPeriodRel; /**< Validity period - Relative */
	TelSmsIs637Priority_t Priority; /**< Priority indicator */
	TelSmsIs637AlertPriority_t AlertPriorty; /**< Alert priority of message */
	TelSmsIs637LangIndicator_t MsgLang; /**< Language indicator */
	TelSmsIs637MsgDisplayMode_t Display; /**< Message display mode */
} TelSmsIs637InBroadCast_t;

/**
 * The structure defines the type of SMS message as union
 */
typedef union {
	TelSmsIs637OutSubmit_t OutSubmit; /**< Outgoing Submit message */
	TelSmsIs637OutAck_t OutAck; /**< Outgoing Acknowledge message */
	TelSmsIs637OutCancel_t OutCancel; /**< Outgoing Cancel message */
	TelSmsIs637InDeliver_t InDeliver; /**< Incoming Delivery message */
	TelSmsIs637InAck_t InAck; /**< Incoming Acknowledge message */
	TelSmsIs637InDeliverAck_t InDeliverAck; /**< Incoming Delivery Acknowledge message */
	TelSmsIs637InBroadCast_t InBc; /**< Incoming Broadcast message */
} TelSmsMessage_t;

/**
 * The structure defines the parameter of entire SMS message of each type
 */
typedef struct {
	int ParamMask; /**< Parameter ID mask */
	TelSmsMsgType_t MsgType; /**< Message type */
	TelSmsMessage_t MsgData; /**< Message data */
} TelSmsMsgInfo_t;
// ********************************************************************//

struct tel_noti_sms_incomming_msg {
        char Sca[TAPI_SIM_SMSP_ADDRESS_LEN];
        int MsgLength;
        char szData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1];
};

struct tel_noti_sms_incomming_cb_msg {
	TelSmsCbMsgType_t CbMsgType; /**< Cell Broadcast  message type */
        short Length; /**<Size of array szMsgData (which is actual TPDU message) */
        char szMsgData[TAPI_NETTEXT_CB_SIZE_MAX + 1]; /**<Cell broadcast message data[Refer 3GPP TS 23.041 9.4.1]*/
};

struct tel_noti_sms_incomming_etws_msg {
	TelSmsEtwsMsgType_t EtwsMsgType; /**< ETWS  message type */
        short Length; /**<Size of array szMsgData (which is actual TPDU message) */
        char szMsgData[TAPI_NETTEXT_ETWS_SIZE_MAX + 1]; /**<ETWS message data[Refer 3GPP TS 23.041 9.4.1]*/
};


#ifdef __cplusplus
}
#endif


#endif  // _TEL_NETTEXT_H_

/**
 * @}
*/
