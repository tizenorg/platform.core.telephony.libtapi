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

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>     /* pthread functions and data structures */
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include "assert.h"
#include "glib.h"

#define MAX_UDH_LEN			137
#define UDH_MAX					7

#define SMS_SMSP_ALPHA_ID_LEN_MAX			128		/* EF-SMSP alpha id length */

#define SMS_MSG_SIZE_MAX				918
#define SMS_CB_SIZE_MAX				93
#define SMS_ADDRESS_LEN_MAX			20
#define SMS_SCADDRESS_LEN_MAX		18

#define SMS_CB_PAGE_SIZE_MAX				9		/**< CB maximum page size*/
#define SMS_GSM_SMS_MSG_NUM_MAX			255		/**< Maximum GSM SMS message number*/
#define SMS_GSM_SMS_CBMI_LIST_SIZE_MAX		50		/**< Maximum GSM SMS CBMI list size*/
#define SMS_SMDATA_SIZE_MAX					255		/**< Maximum SMS data size that can be stored*/
#define SMS_MAX_SMS_SERVICE_CENTER_ADDR		12		/**<Maximum SMS service center address*/
#define SMS_MAX_INDEX						25		/**< Maximum index value for SMS */

#define FAIL			0x00
#define SUCCESS		0x01

typedef enum
{
	SMS_STATUS_READ,						/**< MT message, stored and read */
	SMS_STATUS_UNREAD,					/**< MT message, stored and unread */
	SMS_STATUS_SENT,						/**< MO message, stored and  sent */
	SMS_STATUS_UNSENT,					/**< MO message, stored but not sent */
	SMS_STATUS_DELIVERED,					/**< delivered destination */
	SMS_STATUS_DELIVERY_UNCONFIRMED,	/**< Service centre forwarded message but is unable to confirm delivery*/
	SMS_STATUS_MESSAGE_REPLACED,		/**< Message has been replaced*/
	SMS_STATUS_RESERVED					/**< Reserved for future use*/
} SMS_MsgStatus;

typedef enum
{
	SMS_SENDSMS_SUCCESS = 0x00,					/**<Message send success*/
	SMS_ROUTING_NOT_AVAILABLE,					/**< Message routing not available*/
	SMS_INVALID_PARAMETER,						/**< Invalid parameter present in TPDU*/
	SMS_DEVICE_FAILURE,							/**< Device failure*/
	SMS_SERVICE_RESERVED,							/**< Reserved Service*/
	SMS_INVALID_LOCATION,							/**< Invalid location*/
	SMS_NO_SIM,                           					/**< No SIM error*/
	SMS_SIM_NOT_READY,							/**< SIM not ready error*/
	SMS_NO_NETWORK_RESP,							/**< No response from network*/
	SMS_DEST_ADDRESS_FDN_RESTRICTED,			/**< Destination address restricted*/
	SMS_SCADDRESS_FDN_RESTRICTED,				/**< Service center address restricted*/
	SMS_RESEND_ALREADY_DONE,					/**< Resend a operation already done*/
	SMS_SCADDRESS_NOT_AVAILABLE,				/**< SCA address not available*/
	SMS_UNASSIGNED_NUMBER = 0x8001,				/**< Unassigned number*/
	SMS_OPERATOR_DETERMINED_BARRING = 0x8008,	/**< Operator determined barring*/
	SMS_CALL_BARRED = 0x800A,						/**< Call barred*/
	SMS_MESSAGE_TRANSFER_REJECTED = 0x8015,		/**< Message transfer rejected*/
	SMS_MEMORY_CAPACITY_EXCEEDED = 0x8016,		/**< Memory capacity exceeded/memory full*/
	SMS_DESTINAITION_OUTOFSERVICE = 0x801B,		/**< Destination number/address out of service*/
	SMS_UNSPECIFIED_SUBSCRIBER = 0x801C,			/**< Unspecified subscriber*/
	SMS_FACILITY_REJECTED = 0x801D,				/**< Facility rejected*/
	SMS_UNKNOWN_SUBSCRIBER = 0x801E,				/**< Unknown subscriber*/
	SMS_NETWORK_OUTOFORDER = 0x8026,			/**< Network out of order*/
	SMS_TEMPORARY_FAILURE = 0x8029,				/**< Temporary failure*/
	SMS_CONGESTION = 0x802A,						/**< Congestion happened*/
	SMS_RESOURCES_UNAVAILABLE = 0x802F,			/**< Resource unavailable*/
	SMS_FACILITY_NOT_SUBSCRIBED = 0x8032,			/**< Facility not subscribed by the user*/
	SMS_FACILITY_NOT_IMPLEMENTED = 0x8045,		/**< Facility not implemented*/
	SMS_INVALID_REF_VALUE = 0x8051,				/**< Invalid reference value*/
	SMS_INVALID_MSG = 0x805F,						/**< Invalid message*/
	SMS_INVALID_MANDATORY_INFO = 0x8060,			/**< Invalid Mandatory information*/
	SMS_MESSAGE_TYPE_NOT_IMPLEMENTED = 0x8061,	/**< Message type not implemented*/
	SMS_MESSAGE_NOT_COMPAT_PROTOCOL = 0x8062,	/**< Message not compact protocol*/
	SMS_IE_NOT_IMPLEMENTED = 0x8063,         		 /**< Information element not implemented*/
	SMS_PROTOCOL_ERROR = 0x806F,					/**< Protocol error*/
	SMS_INTERWORKING = 0x807F,					/**< Networking error*/
	SMS_ME_FULL = 0x8080,							/**< SMS ME FULL */
	SMS_SIM_FULL = 0x8081,							/**< SMS SIM FULL */
	SMS_TIMEOUT									/**< Timeout error */
} Sms_Response;

//////////////////////////////////TPDU type////////////////////////////////
typedef enum
{
	SMS_TPDU_DELIVER				= 0x00, // sc -> ms
	SMS_TPDU_DELIVER_REPORT		= 0x00, // ms -> sc
	SMS_TPDU_SUBMIT				= 0x01, // ms -> sc
	SMS_TPDU_SUBMIT_REPORT		= 0x01, // sc -> ms
	SMS_TPDU_STATUS_REPORT		= 0x02, // sc -> ms
	SMS_TPDU_COMMAND				= 0x02, // ms -> sc
	SMS_PDU_INVALID_TYPE

} SmsTPDUType;

typedef enum
{
	SMS_DAEMON_UNKNOWN_MESSAGE,
	SMS_DAEMON_RECEIVED_UNREAD_MESSAGE,
	SMS_DAEMON_RECEIVED_READ_MESSAGE,
	SMS_DAEMON_UNSENT_MESSAGE,
	SMS_DAEMON_STORED_SENT_MESSAGE,

} sd_message_status;

typedef enum
{
	SMS_TON_UNKNOWN = 0, /**< unknown */
	SMS_TON_INTERNATIONAL=1, /**< international number */
	SMS_TON_NATIONAL = 2, /**< national number */
	SMS_TON_NETWORK_SPECIFIC = 3, /**< network specific number */
	SMS_TON_DEDICATED_ACCESS = 4, /**< subscriber number */
	SMS_TON_ALPHA_NUMERIC = 5, /**< alphanumeric, GSM 7-bit default alphabet) */
	SMS_TON_ABBREVIATED_NUMBER = 6, /**< abbreviated number */
	SMS_TON_RESERVED_FOR_EXT = 7 /**< reserved for extension */
} SimTypeOfNum_t;

typedef enum
{
	SMS_NPI_UNKNOWN = 0, /**< Unknown */
	SMS_NPI_ISDN_TEL = 1, /**< ISDN/Telephone numbering plan */
	SMS_NPI_DATA_NUMBERING_PLAN = 3, /**< Data numbering plan */
	SMS_NPI_TELEX = 4, /**< Telex numbering plan */
	SMS_NPI_SVC_CNTR_SPECIFIC_PLAN = 5, /**< Service Center Specific plan */
	SMS_NPI_SVC_CNTR_SPECIFIC_PLAN2 = 6, /**< Service Center Specific plan */
	SMS_NPI_NATIONAL = 8, /**< National numbering plan */
	SMS_NPI_PRIVATE = 9, /**< Private numbering plan */
	SMS_NPI_ERMES_NUMBERING_PLAN = 10, /**< ERMES numbering plan */
	SMS_NPI_RESERVED_FOR_EXT = 0xF /**< Reserved for extension */
} SimNumberingPlanIdentity_t;

typedef struct {
	unsigned int DialNumLen; /**< length of address. If Service center address is not set, then this will be zero */
	SimTypeOfNum_t Ton; /**< Type of number*/
	SimNumberingPlanIdentity_t Npi; /**<Numbering plan identification*/
	unsigned char szDiallingNum[SMS_ADDRESS_LEN_MAX + 1]; /**< destination address. If Address not set, then set to 0xFF */
} SmsAddressInfo_t;

/*
 * Sms_vp_type
 * This enumeration defines the validity period type.
 */
typedef enum
{
	SMS_VP_NOT_USED = 0,		/**< validity period not used */
	SMS_VP_RELATIVE = 2,		/**< after a predetermined time period after the service center has received the message */
	SMS_VP_ABSOLUTE = 3,
	SMS_VP_ENHANCED = 1,

	SMS_VP_MAX
} Sms_vp_type;
/*
 * Sms_coding_group_type
 This enum defined the different sms coding group types
 */
typedef enum
{
	SMS_CODGRP_SM_GENERAL_DCS,			/**< Bits 7..4 00xx */
	SMS_CODGRP_CB_GENERAL_LANG,		/**< 0000 */
	SMS_CODGRP_CB_ALPHA,				/**< 0001 */
	SMS_CODGRP_CB_RES1,				/**< 0010 */
	SMS_CODGRP_CB_RES2,				/**< 0011 */
	SMS_CODGRP_SM_AUTO_DELETION,		/**< 01xx Bits 5..0 are coded the same as SM_GENERAL_DCS */
	SMS_CODGRP_CB_GENERAL_DCS,			/**< 01xx behavior is as SM_GENERAL_DCS */
	SMS_CODGRP_SM_WAITING_DISCARD,		/**< 1100 */
	SMS_CODGRP_SM_WAITING_STORE,		/**< 1101 */
	SMS_CODGRP_SM_WAITING_STORE_UCS2,	/**< 1110 */
	SMS_CODGRP_CB_WAP,					/**< 1110 Cell Broadcast */
	SMS_CODGRP_SM_CLASS_CODING,		/**< 1111 */
	SMS_CODGRP_CB_CLASS_CODING,		/**< 1111 Cell Broadcast */

	SMS_CODGRP_MAX
} Sms_coding_group_type;
/*
 *  Sms_waiting_type
 Defines different types of waiting indicators
 */
typedef enum
{
	SMS_WAITING_VOICE_MSG,		/**< voice message waiting */
	SMS_WAITING_FAX_MSG,		/**< FAX message waiting */
	SMS_WAITING_EMAIL_MSG,		/**< E-mail waiting */
	SMS_WAITING_OTHER_MSG,		/**< Other message waiting */

	SMS_WAITING_MAX
} Sms_waiting_type;
/*
 * Sms_alphabet_type
 Defines different alphabet types of an sms
 */
typedef enum
{
	SMS_ALPHABET_DEFAULT,	/**< GSM alphabet default 7bit */
	SMS_ALPHABET_8BIT,		/**< 8bit */
	SMS_ALPHABET_UCS2,		/**< 16bit ucs2 */

	SMS_ALPHABET_KSC5601,	/**< for Korean */

	SMS_ALPHABET_MAX
} Sms_alphabet_type;
/*
 * Sms_class_type
 This enum defines the different classes of SMS which indicates the
 type of storage of the sms.
 */
typedef enum
{
	SMS_CLASS_0,	/**< display immediately */
	SMS_CLASS_1,	/**< may be stored */
	SMS_CLASS_2,	/**< store in SIM */
	SMS_CLASS_3,	/**< transfer to TE */
	SMS_CLASS_NONE,

	SMS_CLASS_MAX
} Sms_class_type;

/*
This structure contains the information related to sms coding schemes

*/
typedef struct
{
	int							bCompressed;		/**< if text is compressed this is TRUE */
	int							bmsg_class_set;		/**< message class indication. if TRUE = bits 1..0, if FALSE = reserved */
	int							bmsg_ind_active;		/**< message indication group. if TRUE = on, FALSE = off */
	Sms_coding_group_type		coding_group_type;	/**< Coding group, GSM 03.38 */
	Sms_waiting_type			waiting_type;		/**< Type of message waiting */
	Sms_alphabet_type			alphabet_type;		/**< How to encode a message. */
	Sms_class_type			class_type;			/**< The message class */
} Sms_coding_scheme;

/*
 *  Sms_vp_rel_Value
 * This structure defines the validity period types.
 */
typedef enum
{
	SMS_VP_REL_1H,		/**< One hour */
	SMS_VP_REL_6H,		/**< Six hours */
	SMS_VP_REL_12H,	/**< Twelve hours */
	SMS_VP_REL_1D,		/**< One day */
	SMS_VP_REL_3D,		/**< Three days */
	SMS_VP_REL_1W,		/**< One week */
	SMS_VP_REL_1M,		/**< One Month */
	SMS_VP_REL_MAX		/**< Max = 63 weeks */
} Sms_vp_rel_Value;
/*
This structure contains all the information
related to validity period of a message.

*/
typedef struct
{
	Sms_vp_type vp_type;			  /**< Validity period format */
	Sms_vp_rel_Value vp_rel_type; /**< Validity period value duration the message must retain in the service center while application attempts delivery */
	unsigned long vpValue;						  /** Validity period value */
} Sms_vp;


/* This structure contains the details of delete status notification, which comes after calling
TelTapiSmsDeleteMsg*/
typedef struct
{
	unsigned short result;
	unsigned char index;
}TelTapiSmsDeleteMsg_status_ind;


/* This structure contains the details of save status notification, which comes after calling
TelTapiSmsWriteMsg*/

typedef struct
{
	unsigned short result;
	unsigned char index;
}Sms_save_status_ind;


typedef struct
{
	int		year;
	int		month;
	int		day;
	int		hour;
	int		minute;
	int		second;

} TmDateTime;


#define BASE_YEAR		70L					/* 1970 is the base year */
#define MAX_YEAR			130L					/* 2030 is the max year */


#define	SIM_SMSP_ADDRESS_LEN					20	// digit length

#define SET_TON_NPI(dest, ton, npi) {	\
	dest = 0x80;						\
	dest |= (ton & 0x07) << 4;			\
	dest |= npi & 0x0F;					\
}




typedef struct
{
	/************HEADER************/

	// message type [M]
	SmsTPDUType  msg_type ;   //bit 0 ,1

	// reject duplicates [M]
	int rd ;  // bit 2

	// validity period format [M]
	Sms_vp_type vpf ; //  bits 3, 4

	// reply path [M]
	int rp ; //bit 7

	// user data header indication
	int udhi ;   //bit 6

	// status report request
	int  srr ;  // bit 5

	/************BODY************/
	//message reference [M]
	unsigned int mr;

	// destination address [M]
	SmsAddressInfo_t desAddr;

	// protocol identifier [M]
	unsigned char pId;

	// data coding scheme [M]
	Sms_coding_scheme dcs;

	// validity period
	Sms_vp vp;

	// user data length [M]
	unsigned int udl;

	// user data
	char *userData;


}TPDU_SMS_SUBMIT;

typedef struct
{
	/************HEADER************/

	// message type [M]
	SmsTPDUType  msg_type;  //bit 0 ,1

	// user data header indication
	int udhi ;   //bit 6

	/************BODY************/

	// failure Cause [M]
        unsigned int	fcs;

	// parameter indicator [M]
	unsigned char paraInd;

	// service centre time stamp [M]
	TmDateTime scts;

	// protocol identifier [M]
	unsigned char pId;

	// data coding scheme
	Sms_coding_scheme dcs;

	// user data length [M]
	unsigned int udl;

	// user data
	char*  userData;

}TPDU_SMS_SUBMIT_REPORT;


typedef struct
{
	/************HEADER************/
	SmsTPDUType msg_type;   //bit 0 ,1

	int mms; // bit 2

	int rp; // bit 7

	int udhi;  //bit 6

	int sri; // bit , 5status_report_indication

	/************BODY************/

	SmsAddressInfo_t orgAddr;

	unsigned char  pId;

	TmDateTime scts;

	Sms_coding_scheme dcs;

	unsigned int udl;

	char  * userData;

}TPDU_SMS_DELIVER;

typedef struct
{
	/************HEADER************/
	SmsTPDUType msg_type ;   //bit 0 ,1

	int udhi ;  //bit 6

	/************BODY************/

	unsigned int fcs;

	unsigned char paraInd;

	unsigned char pId;

	Sms_coding_scheme dcs;

	unsigned int udl;

	unsigned char * userData;

}TPDU_SMS_DELIVER_REPORT;


typedef struct
{
	/************HEADER************/
	SmsTPDUType msg_type ;   //bit 0 ,1

	int udhi ; //bit 6

	int srr; //bit 5, status_report_request

	/************BODY************/

	unsigned int  mr; //message_ref

	unsigned char pId;

	unsigned char cmdType;

	unsigned char msgNum;

	SmsAddressInfo_t destAddr;

	unsigned int udl;

	unsigned char * userData;

}TPDU_SMS_COMMAND;


typedef struct
{
	/************HEADER************/

	SmsTPDUType msg_type ;   //bit 0 ,1

	int  udhi ; //bit 6

	int mms ; //bit 2

	int srq; //bit 5, status_report_qualifier

	/************BODY************/

	unsigned int  mr;

	SmsAddressInfo_t rcpAddr; //recipient_addr

	TmDateTime scts;

	TmDateTime dt; //discharge_time

        unsigned char  status;

        unsigned char paraInd;

	unsigned char pId;

	Sms_coding_scheme dcs;

	unsigned int udl;

	char * userData;

}TPDU_SMS_STATUS_REPORT;

/***************************************************************************************/
void reverse(char* x, int len);
char* AcItoa(int n, char* str, int b) ;
int AcToupper(int ch);

char* SmsUtilUnpackGSMCode(char* szData, const char* pIn, int in_len );
int SmsUtilPackGSMCode( unsigned char* pOut, const char* szData, int in_len );

void SmsUtilConvertBCD2Digit( char* pDigits, char* pBCD, int digitLen );
void SmsUtilConvertDigit2BCD( char* pBCD, char* pDigits, int digitLen );

TmDateTime* SmsUtilDecodeTimeStamp(char* pTimeStamp, TmDateTime *tmDateTime );
unsigned char* SmsUtilEncodeTimeStamp( TmDateTime* tmDateTime, unsigned char* pTimeStamp );


int  SmsUtilDecodeAddrField(char *diallingNum, char * pAddrField, int *result_ton, int *result_npi );
int SmsUtilEncodeAddrField( unsigned char* pAddrField, char* diallingNum, int dialnumLen, int ton, int npi );

int SmsUtilDecodeScAddrField( SmsAddressInfo_t* pSmsAddrField, unsigned char* pAddrField );
 int  SmsUtilEncodeScAddrField( unsigned char* pAddrField, SmsAddressInfo_t * pSmsAddrField );

void SmsUtilDecodeDCS( Sms_coding_scheme* pCodingScheme,   unsigned char dcs );
void SmsUtilEncodeDCS( unsigned char* pDCS, Sms_coding_scheme* pCodingScheme);

 unsigned char SmsUtilEncodeValidity( unsigned char* pValidity, Sms_vp* pVP );

 void __util_hex_dump(const char *pad, int size, const void *data);
 unsigned char __sms_hexchar_to_int(char ch);
 gboolean __sms_asciistring_to_hex(const char *ascii_string,
					unsigned char *hex_string,
					int ascii_len);
 void bitarray_copy(const unsigned char *src_org, int src_offset,
 				int src_len,  unsigned char *dst_org, unsigned int dst_offset);

