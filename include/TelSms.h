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
 * @file TelSms.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SMS
 * @{
 */

#ifndef _TEL_NETTEXT_H_
#define _TEL_NETTEXT_H_

#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* NetText */

/**
 * @brief Definition for the EF-SMSP digit length.
 * @since_tizen 2.3
 */
#define TAPI_SIM_SMSP_ADDRESS_LEN		20

/**
 * @brief Definition for the EF-SMSP alpha ID length.
 * @since_tizen 2.3
 */
#define TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX		128

/**
 * @brief Definition for the maximum message size.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_MSG_SIZE_MAX		918

/**
 * @brief Definition for the maximum CB message size.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_CB_SIZE_MAX			1252

/**
 * @brief Definition for the maximum ETWS message size.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_ETWS_SIZE_MAX			1252

/**
 * @brief Definition for the nettext address length.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_ADDRESS_LEN_MAX	20

/**
 * @brief Definition for the SC address length.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_SCADDRESS_LEN_MAX	18

/**
 * @brief Definition for the CB maximum page size.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_CB_PAGE_SIZE_MAX				 9

/**
 * @brief Definition for the maximum GSM SMS message number.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_SMS_MSG_NUM_MAX             255    /**< Maximum SMS message number*/

/**
 * @brief Definition for the maximum GSM SMS CBMI list size.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_SMS_CBMI_LIST_SIZE_MAX      50    /**< Maximum SMS CBMI list size*/

/**
 * @brief Definition for the maximum SMS data size that can be stored.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_SMDATA_SIZE_MAX                           255  /**< Maximum SMS data size that can be stored*/

/**
 * @brief Definition for the maximum SMS service center address.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_MAX_SMS_SERVICE_CENTER_ADDR   12

/**
 * @brief Definition for the maximum index value of the SMS.
 * @since_tizen 2.3
 */
#define TAPI_NETTEXT_MAX_INDEX                                        255

#define TAPI_NETTEXT_SMSP_PARAMS_MAX_LEN		28

/**
 * @brief Enumeration for network text status type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETTEXT_STATUS_READ,					/**< MT message, stored and read */
	TAPI_NETTEXT_STATUS_UNREAD,					/**< MT message, stored and unread */
	TAPI_NETTEXT_STATUS_SENT,					/**< MO message, stored and sent */
	TAPI_NETTEXT_STATUS_UNSENT,					/**< MO message, stored but not sent */
	TAPI_NETTEXT_STATUS_DELIVERED,                         /**< Delivered destination */
	TAPI_NETTEXT_STATUS_DELIVERY_UNCONFIRMED,  /**< Service centre forwarded message but is unable to confirm delivery */
	TAPI_NETTEXT_STATUS_MESSAGE_REPLACED,          /**< Message has been replaced */
	TAPI_NETTEXT_STATUS_RESERVED                            /**< Reserved for future use */
} TelSmsMsgStatus_t;

/**
 * @brief Enumeration for memory status type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE		= 0x01, /**< PDA memory is available */
	TAPI_NETTEXT_PDA_MEMORY_STATUS_FULL			= 0x02,  /**< PDA memory is full */
	TAPI_NETTEXT_PHONE_MEMORY_STATUS_AVAILABLE	= 0x03, /**< Phone memory is available */
	TAPI_NETTEXT_PHONE_MEMORY_STATUS_FULL			= 0x04, /**< Phone memory is full */
}TelSmsMemStatusType;

/**
 * @brief Enumeration for different CB message types.
 * @since_tizen 2.3
 */
typedef enum
{
		TAPI_NETTEXT_CB_MSG_GSM = 1,		/**< GSM Cell broadcast message */
		TAPI_NETTEXT_CB_MSG_UMTS,			/**< UMTSCell broadcast message */
		TAPI_NETTEXT_CB_MSG_CDMA			/**< CDMA broadcast message */
} TelSmsCbMsgType_t;

/**
 * @brief Enumeration for different ETWS message types.
 * @since_tizen 2.3
 */
typedef enum
{
	   TAPI_NETTEXT_ETWS_PRIMARY = 0,             /**< Primary ETWS message */
	   TAPI_NETTEXT_ETWS_SECONDARY_GSM,            /**< GSM Secondary ETWS message  */
	   TAPI_NETTEXT_ETWS_SECONDARY_UMTS,               /**< UMTS Secondary ETWS message  */
	   TAPI_NETTEXT_ETWS_SECONDARY_CDMA /*TODO:Will be supported*/
} TelSmsEtwsMsgType_t;

/**
 * @brief Enumeration for different response types that come in the
 * sent status acknowledgement/notification after sending a message to the network.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETTEXT_SENDSMS_SUCCESS = 0x00,     /**< Message sent successfully */
	TAPI_NETTEXT_ROUTING_NOT_AVAILABLE,     /**< Message routing not available */
	TAPI_NETTEXT_INVALID_PARAMETER,             /**< Invalid parameter present in TPDU */
	TAPI_NETTEXT_DEVICE_FAILURE,                    /**< Device failure */
	TAPI_NETTEXT_SERVICE_RESERVED,                /**< Reserved Service */
	TAPI_NETTEXT_INVALID_LOCATION,                /**< Invalid location */
	TAPI_NETTEXT_NO_SIM,                                    /**< No SIM error */
	TAPI_NETTEXT_SIM_NOT_READY,            /**< SIM not ready error */
	TAPI_NETTEXT_NO_NETWORK_RESP,          /**< No response from network */
	TAPI_NETTEXT_DEST_ADDRESS_FDN_RESTRICTED,/**< Destination address restricted */
	TAPI_NETTEXT_SCADDRESS_FDN_RESTRICTED,   /**< Service center address restricted */
	TAPI_NETTEXT_RESEND_ALREADY_DONE,        /**< Resend an already done operation */
	TAPI_NETTEXT_SCADDRESS_NOT_AVAILABLE,    /**< SCA address not available */
	TAPI_NETTEXT_UNASSIGNED_NUMBER = 0x8001,          /**< Unassigned number */
	TAPI_NETTEXT_OPERATOR_DETERMINED_BARRING = 0x8008,/**< Operator determined barring */
	TAPI_NETTEXT_CALL_BARRED = 0x800A,                /**< Call barred */
	TAPI_NETTEXT_MESSAGE_TRANSFER_REJECTED = 0x8015,  /**< Message transfer rejected */
	TAPI_NETTEXT_MEMORY_CAPACITY_EXCEEDED = 0x8016,   /**< Memory capacity exceeded/memory full */
	TAPI_NETTEXT_DESTINAITION_OUTOFSERVICE = 0x801B,  /**< Destination number/address out of service */
	TAPI_NETTEXT_UNSPECIFIED_SUBSCRIBER = 0x801C,     /**< Unspecified subscriber */
	TAPI_NETTEXT_FACILITY_REJECTED = 0x801D,          /**< Facility rejected */
	TAPI_NETTEXT_UNKNOWN_SUBSCRIBER = 0x801E,         /**< Unknown subscriber */
	TAPI_NETTEXT_NETWORK_OUTOFORDER = 0x8026,         /**< Network out of order */
	TAPI_NETTEXT_TEMPORARY_FAILURE = 0x8029,          /**< Temporary failure */
	TAPI_NETTEXT_CONGESTION = 0x802A,                 /**< Congestion occurred */
	TAPI_NETTEXT_RESOURCES_UNAVAILABLE = 0x802F,      /**< Resource unavailable */
	TAPI_NETTEXT_FACILITY_NOT_SUBSCRIBED = 0x8032,    /**< Facility not subscribed by the user */
	TAPI_NETTEXT_FACILITY_NOT_IMPLEMENTED = 0x8045,   /**< Facility not implemented */
	TAPI_NETTEXT_INVALID_REF_VALUE = 0x8051,          /**< Invalid reference value */
	TAPI_NETTEXT_INVALID_MSG = 0x805F,                /**< Invalid message */
	TAPI_NETTEXT_INVALID_MANDATORY_INFO = 0x8060,     /**< Invalid Mandatory information */
	TAPI_NETTEXT_MESSAGE_TYPE_NOT_IMPLEMENTED = 0x8061,/**< Message type not implemented */
	TAPI_NETTEXT_MESSAGE_NOT_COMPAT_PROTOCOL = 0x8062, /**< Message not compact protocol */
	TAPI_NETTEXT_IE_NOT_IMPLEMENTED = 0x8063,          /**< Information element not implemented */
	TAPI_NETTEXT_PROTOCOL_ERROR = 0x806F,              /**< Protocol error */
	TAPI_NETTEXT_INTERWORKING = 0x807F,              /**< Networking error */
	TAPI_NETTEXT_ME_FULL = 0x8080,                     /**< SMS ME FULL */
	TAPI_NETTEXT_SIM_FULL = 0x8081,                     /**< SMS SIM FULL */
	TAPI_NETTEXT_TIMEOUT						/**< Timeout error */
}TelSmsResponse_t;

/**
 * @brief Enumeration for different cause types that come in the
 * sent status acknowledgement/notification after sending a message to the network.
 * @since_tizen 2.3
 */
typedef enum
{

  TAPI_NETTEXT_SUCCESS,                     /**< SMS operation successful */
  TAPI_NETTEXT_INVALID_PARAMETER_FORMAT,    /**< Invalid format for some parameters passed in Data package information(TPDU) */
  TAPI_NETTEXT_PHONE_FAILURE,               /**< SMS operation failed due to Modem failure */
  TAPI_NETTEXT_SIM_BUSY,                    /**< SMS SIM operation cannot be performed as the SIM is busy with some other operation */
  TAPI_NETTEXT_SIM_FAILURE,                 /**< SMS SIM operation cannot be performed due to SIM failure */
  TAPI_NETTEXT_UNKNOWN,                     /**< Unknown error */
  TAPI_NETTEXT_MEMORY_FAILURE,              /**< Error while accessing memory or allocating memory for SMS operation */
  TAPI_NETTEXT_OPERATION_NOT_SUPPORTED      /**< Operation not allowed/supported */
}  TelSmsCause_t;

/**
 * @brief Enumeration for response values.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETTEXT_CBSETCONFIG_RSP,     /**< Cell broadcast config response */
	TAPI_NETTEXT_SETPREFERREDBEARER_RSP,     /**< Set preferred bearer response */
	TAPI_NETTEXT_SETPARAMETERS_RSP,     /**< Set parameter response */
	TAPI_NETTEXT_SETMEMORYSTATUS_RSP,     /**< Set memory status response */
	TAPI_NETTEXT_SETMESSAGESTATUS_RSP,     /**< Set message status response */
	TAPI_NETTEXT_SETDEVICESTATUS_RSP,    /**< Set device status response */
	TAPI_NETTEXT_SETSCADDR_RSP,     /**< Set SCA address response */
	TAPI_NETTEXT_SET_RSP_MAX     /**< Maximum limit */
}TelSmsSetResponse;

/**
 * @brief Enumeration for the type of Network
 * @since_tizen 2.3
 */
typedef enum  {
	TAPI_NETTEXT_NETTYPE_3GPP = 0x01,						/**< 3gpp type */
	TAPI_NETTEXT_NETTYPE_3GPP2 = 0x02,    					/**< 3gpp2 type (CDMA) */
} TelSmsNetType_t;

/**
 * @brief Enumeration for the sms ready status type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETTEXT_READY_STATUS_NONE = 0x00,
	TAPI_NETTEXT_READY_STATUS_3GPP = 0x01,
	TAPI_NETTEXT_READY_STATUS_3GPP2 = 0x02,
	TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2 = 0x03,
}TelSmsReadyStatus_t;

/**
 * @brief The structure type for different parameters of the CB configuration.
 * @since_tizen 2.3
 */
typedef struct {
 unsigned short FromMsgId; /**< Starting point of the range of the CBS message ID */
 unsigned short ToMsgId; /**< Ending point of the range of the CBS message ID */
 unsigned char Selected; /**< 0x00 . Not selected. 0x01 . Selected */
} TelSmsCbMsgInfo3gpp_t;

typedef struct {
 unsigned short CBCategory; /**< CB Service category */
 unsigned short CBLanguage; /**< Language indicator value
 								. 0x00 . LANGUAGE_UNKNOWN . Unknown or Unspecified
								. 0x01 . LANGUAGE_ENGLISH . English
								. 0x02 . LANGUAGE_FRENCH . French
								. 0x03 . LANGUAGE_SPANISH . Spanish
								. 0x04 . LANGUAGE_JAPANESE . Japanese
								. 0x05 . LANGUAGE_KOREAN . Korean
								. 0x06 . LANGUAGE_CHINESE . Chinese
								. 0x07 . LANGUAGE_HEBREW . Hebrew */
 unsigned char Selected; /**< 0x00 . Not selected. 0x01 . Selected */
} TelSmsCbMsgInfo3gpp2_t;

typedef union {
	TelSmsCbMsgInfo3gpp_t Net3gpp; /**< 3GPP Broadcast Configuration Information */
	TelSmsCbMsgInfo3gpp2_t Net3gpp2; /**< 3GPP2 Broadcast Configuration Information, CDMA */
} TelSmsCbMsgInfo_t;

typedef struct {
	int Net3gppType;  /**< Type of 3GPP, 0x01 . 3gpp. 0x02 . 3gpp2(CDMA) */
	int CBEnabled; /**< CB service state. If @a CBEnabled is @c true then the cell broadcast service will be enabled and the underlying modem will enable the CB Channel to receive CB messages. Otherwise the CB service will be disabled and the underlying modem will deactivate the CB channel. (enabled/disabled) */
	unsigned char MsgIdMaxCount; /**< CB Channel List Max Count For Response */
	int MsgIdRangeCount; /**< Range of CB message ID count */
	TelSmsCbMsgInfo_t MsgIDs[TAPI_NETTEXT_SMS_CBMI_LIST_SIZE_MAX]; /**< Range of CB message ID information */
} TelSmsCbConfig_t;

/**
 * @brief The structure type for properties of a dialing number.
 * @details Those properties are: type of the number, numbering plan indicator, length, and the actual number.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int DialNumLen; /**< Length of the address. If the Service center address is not set, then this will be zero */
	TelSimTypeOfNum_t Ton; /**< Type of number */
	TelSimNumberingPlanIdentity_t Npi; /**< Numbering plan identification */
	unsigned char szDiallingNum[TAPI_SIM_SMSP_ADDRESS_LEN + 1]; /**< Destination address. If the address is not set, then it is set to 0xFF */
} TelSmsAddressInfo_t;

/**
 * @brief The structure type for different fields involved in setting the parameters of a particular SMS in EFsmsp.
 * @since_tizen 2.3
 */
typedef struct {

	unsigned char RecordIndex; /**< Index */
	unsigned char RecordLen; /**< SMS Parameter Record length */
	unsigned long AlphaIdLen; /**< Alpha ID length */
	char szAlphaId[TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX + 1]; /**< Alpha ID. It is a unique identifier for each row/record in EFsmsp */
	unsigned char ParamIndicator; /**< SMS parameter indicator is a 1 byte value. Each bit in this value indicates the presence/absence of the SMS header parameters. If the parameter is present the corresponding bit value is set to @c 0. If the parameter is absent then it is set as @c 1. Refer 3GPP TS 31.02 :4.4.23 */
	TelSmsAddressInfo_t TpDestAddr; /**< TP-destination address (TP-DA) */
	TelSmsAddressInfo_t TpSvcCntrAddr; /**< TP-service center address */
	unsigned short TpProtocolId; /**< TP-protocol ID */
	unsigned short TpDataCodingScheme; /**< TP-data coding scheme */
	unsigned short TpValidityPeriod; /**< TP-validity period */
} TelSmsParams_t;

/**
 * @brief The structure type for the fields related to an SMS like format(3GPP/3GPP2), TPDU and length
 * @details
 * SCA will be filled by MSG_SERVICE for GSM/UMTS technology only.\n
 * For CDMA SCA need not be filled by MSG_SERVICE.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char Sca[TAPI_SIM_SMSP_ADDRESS_LEN]; /**< Service Centre address - an optional parameter. If this parameter is not present, then this field will be Null.If present, the valid service centre address information is filled as per 3GPP TS23.040 9.1.2.5 Address fields */
	int MsgLength; /**< Size of array szData (which is actual TPDU message) */
	unsigned char szData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1]; /**<SMS TPDU message */
	TelSmsNetType_t format;
} TelSmsDatapackageInfo_t;

/**
 * @brief The structure type for data related to SimIndex, MessageStatus, and SMS Data Stored.
 * @since_tizen 2.3
 */
typedef struct {
	int SimIndex; /**< Index where SMS is stored */
	TelSmsMsgStatus_t MsgStatus; /**< Message status */
	TelSmsDatapackageInfo_t SmsData; /**< SMS message */
} TelSmsData_t;

/**
 * @brief The structure type for different parameters that are related to the message count in a particular memory (Phone/SIM).
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int TotalCount; /**< Total count of messages stored in the SIM */
	int UsedCount; /**< Stored message count in the SIM */
	int IndexList[TAPI_NETTEXT_SMS_MSG_NUM_MAX]; /**< Message index list stored in the SIM. And the maximum size of this array can be of @a TotalCount. This array contains the list of index values in which the messages are stored, i.e. index_list[totalcount] = [2,3] denotes that indexes 2 and 3 are stored(valid) and others are not(empty) */
} TelSmsStoredMsgCountInfo_t;

/**
 * @brief The structure type for a cell broadcast message.
 * @since_tizen 2.3
 */
typedef struct {

	TelSmsCbMsgType_t CbMsgType; /**< Cell Broadcast message type */
	unsigned short Length; /**< Size of array @a szMsgData (which is the actual TPDU message) */
	char szMsgData[TAPI_NETTEXT_CB_SIZE_MAX + 1]; /**< Cell broadcast message data[Refer 3GPP TS 23.041 9.4.1] */
} TelSmsCbMsg_t;

/**
 * @brief The structure type for a ETWS message.
 * @since_tizen 2.3
 */
typedef struct {
	TelSmsEtwsMsgType_t EtwsMsgType; /**< Cell Broadcast message type */
	unsigned short Length; /**< Size of array @a szMsgData (which is the actual TPDU message) */
	char szMsgData[TAPI_NETTEXT_ETWS_SIZE_MAX + 1]; /**< Cell broadcast message data[Refer 3GPP TS 23.041 9.4.1] */
} TelSmsEtwsMsg_t;

struct tel_noti_sms_incomming_msg {
        char Sca[TAPI_SIM_SMSP_ADDRESS_LEN];	/**< TBD */
        int MsgLength;	/**< TBD */
        char szData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1];	/**< TBD */
	TelSmsNetType_t format;
};

struct tel_noti_sms_incomming_cb_msg {
	TelSmsCbMsgType_t CbMsgType; /**< Cell Broadcast message type */
        short Length; /**< Size of array @a szMsgData (which is the actual TPDU message) */
        char szMsgData[TAPI_NETTEXT_CB_SIZE_MAX + 1]; /**< Cell broadcast message data[Refer 3GPP TS 23.041 9.4.1] */
};

struct tel_noti_sms_incomming_etws_msg {
	TelSmsEtwsMsgType_t EtwsMsgType; /**< ETWS message type */
        short Length; /**< Size of array @a szMsgData (which is the actual TPDU message) */
        char szMsgData[TAPI_NETTEXT_ETWS_SIZE_MAX + 1]; /**< ETWS message data[Refer 3GPP TS 23.041 9.4.1] */
};


#ifdef __cplusplus
}
#endif

#endif  // _TEL_NETTEXT_H_

/**
 * @}
*/
