/*
 * libtapi
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd. All rights reserved.
 * Copyright (c) 2013 Intel Corporation. All rights reserved.
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

#ifndef TAPI_SMS_CDMA_H
#define TAPI_SMS_CDMA_H

/*
   typedef unsigned char	INT8;
   typedef unsigned short	INT16;
   typedef unsigned int	INT;
   typedef unsigned long	INT32;

   typedef unsigned char	BYTE;
   */

typedef int BOOL;

#ifndef packed__
#define packed__ __attribute__ ((packed))
#endif

//=============================================================
//
//		PARAMETER_ID
//
//=============================================================
#define PARAMID_TELESERVICE_ID				0x01	/* Teleservice Identifier */
#define PARAMID_SERVICE_CATEGORY			0x02	/* Broadcast Service Category */
#define PARAMID_ADDRESS					0x03	/* Address */
#define PARAMID_SUBADDRESS				0x04	/* Subaddress */
#define PARAMID_BEARER_REPLY				0x05	/* Bearer Reply Option */
#define PARAMID_CAUSE_CODES				0x06	/* Cause Codes */
#define PARAMID_MESSAGE_ID				0x07	/* Message Identifier */
#define PARAMID_USER_DATA					0x08	/* User Data */
#define PARAMID_USER_RESPONSE_CODE		0x09	/* User Response Code */
#define PARAMID_MC_TIME_STAMP			0x0A	/* Message Center Time Stamp */
#define PARAMID_VALIDITY_PERIOD_ABS		0x0B	/* Validity Period - Absolute */
#define PARAMID_VALIDITY_PERIOD_REL		0x0C	/* Validiry Period - Relative */
#define PARAMID_DEFERRED_DELIVERY_ABS	0x0D	/* Deferred Delivery Time - Absolute */
#define PARAMID_DEFERRED_DELIVERY_REL	0x0E	/* Deferred Delivery Time - Relative */
#define PARAMID_PRIORITY					0x0F	/* Priority Indicator */
#define PARAMID_PRIVACY					0x10	/* Privacy Indicator */
#define PARAMID_REPLY_OPTION				0x11	/* Reply Option */
#define PARAMID_NUMBER_OF_MESSAGE		0x12	/* Number of Messages : Voice Mail Count */
#define PARAMID_ALERT_ON_DELIVERY			0x13	/* Alert on Message Delivery */
#define PARAMID_LANGUAGE					0x14	/* Langauge Indicator */
#define PARAMID_CALLBACK					0x15	/* Call Back Number */
#define PARAMID_DISPLAY_MODE				0x16	/* Display Mode */
#define PARAMID_MULTI_ENCODING_USER_DATA	0x17	/* Multiply Encoding User Data */
#define PARAMID_MEMORY_INDEX				0x18	/* Memory address stored in Phone Memory */
#define PARAMID_BEARER_DATA				0x19	/* Bearer data - raw data  */

//=============================================================
//
//		PARAMETER_DATA
//
//=============================================================

//------------------------------------------------------------------------------
// PARAMID_TELESERVICE_ID			0x01	/* Teleservice Identifier */
//------------------------------------------------------------------------------
/* Common TELESERVICE ID's */
#define TELESERVICEID_CPT95				0x1001	/* 4097 : Paging */
#define TELESERVICEID_CMT95				0x1002	/* 4098 : Text Message */
#define TELESERVICEID_VMN95				0x1003	/* 4099 : Voice Mail Notification */
#define TELESERVICEID_WAP					0x1004	/* WAP Message */

//------------------------------------------------------------------------------
// PARAMID_BCSERVICECATEGORY		0x02	/* Broadcast Service Category */
//------------------------------------------------------------------------------
// The Korean Wireless Service Carrier's do not use below Service Category's
// Below Service Category's depend on the Carriers.
#define CATEGORY_UNKNOWN				0x00
#define CATEGORY_EMERGENCY				0x01
#define CATEGORY_ADMIN					0x02
#define CATEGORY_MAINTENANCE			0x03
#define CATEGORY_GEN_NEWS_LOC			0x04
#define CATEGORY_GEN_NEWS_REG			0x05
#define CATEGORY_GEN_NEWS_NAT			0x06
#define CATEGORY_GEN_NEWS_INT			0x07
#define CATEGORY_FIN_NEWS_LOC			0x08
#define CATEGORY_FIN_NEWS_REG			0x09
#define CATEGORY_FIN_NEWS_NAT			0x0A
#define CATEGORY_FIN_NEWS_INT			0x0B
#define CATEGORY_SPT_NEWS_LOC			0x0C
#define CATEGORY_SPT_NEWS_REG			0x0D
#define CATEGORY_SPT_NEWS_NAT			0x0E
#define CATEGORY_SPT_NEWS_INT			0x0F
#define CATEGORY_ENT_NEWS_LOC			0x10
#define CATEGORY_ENT_NEWS_REG			0x11
#define CATEGORY_ENT_NEWS_NAT			0x12
#define CATEGORY_ENT_NEWS_INT			0x13
#define CATEGORY_LOC_WEATHER			0x14
#define CATEGORY_AREA_TRAFFIC			0x15
#define CATEGORY_AIRPORT_SCHED			0x16
#define CATEGORY_RESTAURANTS			0x17
#define CATEGORY_LODGINGS				0x18
#define CATEGORY_RETAILS				0x19
#define CATEGORY_ADS					0x1A
#define CATEGORY_STOCK_QUOTES			0x1B
#define CATEGORY_JOBS					0x1C
#define CATEGORY_MEDICAL				0x1D
#define CATEGORY_TECH_NEWS				0x1E
#define CATEGORY_MULTI					0x1F

//------------------------------------------------------------------------------
// PARAMID_ADDRESS					0x03	/* Address */
//------------------------------------------------------------------------------

/* DIGIT MODE : CHARi[] filed�� 4bit dtmf code���� 8bit code������ �ǹ��Ѵ�.
*/
#define DIGITMODE_4BITDTMF			0x00
#define DIGITMODE_8BITCODE			0x01

/* NUMBER MODE
*/
#define NUMMODE_NONE_DATANETWORK	0x00	/* in ANSI TI.607 */
#define NUMMODE_DATANETWORK			0x01

/* NUMBER TYPE
*/
/* The following are used when number mode is not data network address.
*/
#define NUMBER_TYPE_UNKNOWN			0x00
#define NUMBER_TYPE_INTERNATIONAL	0x01
#define NUMBER_TYPE_NATIONAL		0x02
#define NUMBER_TYPE_NETWORK			0x03
#define NUMBER_TYPE_SUBSCRIBER		0x04
#define NUMBER_TYPE_RESERVED_5		0x05
#define NUMBER_TYPE_ABREVIATED		0x06
#define NUMBER_TYPE_RESERVED_7		0x07
/* The following are used only when number mode is data network address
   mode.
   */
#define NUMBER_TYPE_IP				0x01
#define NUMBER_TYPE_EMAILADDR		0x02

/* NUMBER PLAN
*/
#define NUMBER_PLAN_UNKNOWN			0x00
#define NUMBER_PLAN_TELEPHONY		0x01	/* CCITT E.164 and E.163,  including ISDN plan */
#define NUMBER_PLAN_RESERVED_2		0x02
#define NUMBER_PLAN_DATA			0x03	/* CCITT X.121 */
#define NUMBER_PLAN_TELEX			0x04	/* CCITT F.69 */
#define NUMBER_PLAN_RESERVED_5		0x05
#define NUMBER_PLAN_RESERVED_6		0x06
#define NUMBER_PLAN_RESERVED_7		0x07
#define NUMBER_PLAN_RESERVED_8		0x08
#define NUMBER_PLAN_PRIVATE			0x09
#define NUMBER_PLAN_RESERVED_10		0x0A
#define NUMBER_PLAN_RESERVED_11		0x0B
#define NUMBER_PLAN_RESERVED_12		0x0C
#define NUMBER_PLAN_RESERVED_13		0x0D
#define NUMBER_PLAN_RESERVED_14		0x0E
#define NUMBER_PLAN_RESERVED_15		0x0F

/* NUM_FIELDS
*/
/*		MAXLENGTH_SMS_ADDRESS			Maximum sms destination(or origination ) address /call back number */
#define	MAXLENGTH_SMS_ADDRESS		20
/* CHARi
*/

//------------------------------------------------------------------------------
// PARAMID_SUBADDRESS				0x04	/* Subaddress */
//------------------------------------------------------------------------------
/* SUB ADDRESS TYPE
*/
#define SUBADDR_NSAP				0x00	/* CCITT X.213 or ISO 8348 AD2 */
#define SUBADDR_USER_SPECIFIED		0x01	/* e.g. X.25 */

/* ODD  : �̰��� �� �ʿ��Ѱ� ? phone���� ������ 4bit�� �����ϸ� �Ǵ°� �ƴѰ� ?
*/
/* If the last CHARi field contains information only in the 4 most significant bits,
   the ODD field shall be set to '1'.  Otherwise, the ODD field shall be set to '0'.
   */
#define SUBADDR_EVEN				0x00
#define SUBADDR_ODD					0x01

/* NUM_FIELDS
*/

/* CHARi
*/

//------------------------------------------------------------------------------
// PARAMID_BEARER_REPLY				0x05	/* Bearer Reply Option */
//------------------------------------------------------------------------------
/* REPLY_SEQ
*/
// Range : 0 - 63
//	 refer to MAXLENGTH_SMS_REPLY_SEQUENCE_NUMBER

//------------------------------------------------------------------------------
// PARAMID_CAUSE_CODES				0x06	/* Cause Codes */
//------------------------------------------------------------------------------
/* REPLY_SEQ
*/
// Range : 0 - 63
// Refer to MAXLENGTH_SMS_REPLY_SEQUENCE_NUMBER

/* ERROR_CLASS
*/
#define ERRORCLASS_NONE					0x00	/* No Error */
#define ERRORCLASS_TEMP					0x02	/* Tempory Error */
#define ERRORCLASS_PERMANENT			0x03	/* Permanent Error */
#define ERRORCLASS_INTERNAL				0x04	/* Phone Internal Error */

/* CAUSE_CODE : defined in IS-41D
*/

/* A. Network Problems:
*/
#define CAUSECODE_ADDRESS_VACANT							0
#define CAUSECODE_ADDRESS_TRANSLATION_FAILURE				1
#define CAUSECODE_NETWORK_RESOURCE_SHORTAGE					2
#define CAUSECODE_NETWORK_FAILURE							3
#define CAUSECODE_INVALID_TELESERVICE_ID					4
#define CAUSECODE_OTHER_NETWORK_PROBLEM						5
#define CAUSECODE_OTHER_NETWORK_PROBLEM_MORE_FIRST			6
/* all values within this range are treated as IPC_CAUSECODE_OTHER_NETWORK_PROBLEM_S	*/
#define IPC_CAUSECODE_OTHER_NETWORK_PROBLEM_MORE_LAST			31

/* B. Terminal Problems:
*/
#define CAUSECODE_NO_PAGE_RESPONSE							32
#define CAUSECODE_DESTINATION_BUSY							33
#define CAUSECODE_NO_ACK									34
#define CAUSECODE_DESTINATION_RESOURCE_SHORTAGE				35
#define CAUSECODE_SMS_DELIVERY_POSTPONED					36
#define CAUSECODE_DESTINATION_OUT_OF_SERVICE				37
#define CAUSECODE_DESTINATION_NO_LONGER_AT_THIS_ADDRESS		38
#define CAUSECODE_OTHER_TERMINAL_PROBLEM					39
#define CAUSECODE_OTHER_TERMINAL_PROBLEM_MORE_FIRST			40
/* all values within this range are treated as IPC_CAUSECODE_OTHER_TERMINAL_PROBLEM_S */
#define CAUSECODE_OTHER_TERMINAL_PROBLEM_MORE_LAST			47
#define CAUSECODE_SMS_DELIVERY_POSTPONED_MORE_FIRST			48
#define CAUSECODE_SMS_DELIVERY_POSTPONED_MORE_LAST			63

/* C. Radio Interface Problems:
*/
#define CAUSECODE_RADIO_IF_RESOURCE_SHORTAGE				64
#define CAUSECODE_RADIO_IF_INCOMPATIBLE						65
#define CAUSECODE_OTHER_RADIO_IF_PROBLEM					66
#define CAUSECODE_OTHER_RADIO_IF_PROBLEM_MORE_FIRST			67
/* all values within this range are treated as IPC_CAUSECODE_OTHER_RADIO_IF_PROBLEM */
#define CAUSECODE_OTHER_RADIO_IF_PROBLEM_MORE_LAST			95

/* D. General Problems:
*/
#define CAUSECODE_UNEXPECTED_PARM_SIZE						96
#define CAUSECODE_SMS_ORIGINATION_DENIED					97
#define CAUSECODE_SMS_TERMINATION_DENIED					98
#define CAUSECODE_SUPPL_SERVICE_NOT_SUPPORTED				99
#define CAUSECODE_SMS_NOT_SUPPORTED							100
#define CAUSECODE_RESERVED_101								101
#define CAUSECODE_MISSING_EXPECTED_PARM						102
#define CAUSECODE_MISSING_MANDATORY_PARM					103
#define CAUSECODE_UNRECOGNIZED_PARM_VALUE					104
#define CAUSECODE_UNEXPECTED_PARM_VALUE						105
#define CAUSECODE_USER_DATA_SIZE_ERROR						106
#define CAUSECODE_OTHER_GENERAL_PROBLEMS					107
#define CAUSECODE_OTHER_GENERAL_PROBLEMS_MORE_FIRST			108
/* all values within this range are treated as IPC_CAUSECODE_OTHER_GENERAL_PROBLEMS */
#define CAUSECODE_OTHER_GENERAL_PROBLEMS_MORE_LAST			255

//------------------------------------------------------------------------------
// PARAMID_MESSAGE_ID				0x07	/* Message Identifier */
//------------------------------------------------------------------------------
/* MESSAGE_TYPE
*/
#define MESSAGETYPE_DELIVER				0x01
#define MESSAGETYPE_SUBMIT				0x02
#define MESSAGETYPE_CANCEL				0x03
#define MESSAGETYPE_DELIVERY_ACK		0x04
#define MESSAGETYPE_USER_ACK			0x05

/* MESSAGE_ID
*/
// 2 bytes :    Range : 0 - 65535

//------------------------------------------------------------------------------
// PARAMID_USER_DATA					0x08	/* User Data */
//------------------------------------------------------------------------------
/* MESSAGE_ENCODING
*/
#define MSGENCODING_UNSPECIFIED						0x00	/* OCTET */
#define MSGENCODING_IS91_EXTENDED_PROTOCOL			0x01	/* */
#define MSGENCODING_7BIT_ASCII						0x02	/* */
#define MSGENCODING_IA5								0x03	/* */
#define MSGENCODING_UNICODE							0x04	/* */
#define MSGENCODING_SHIFT_JIS						0x05	/* */
#define MSGENCODING_KSC5601							0x06	/* Korean */
#define MSGENCODING_HEBREW							0x07	/* ISO_8859_8 */
#define MSGENCODING_LATIN							0x08	/* ISO_8859_1 */
#define MSGENCODING_KSC5601_3PCS						0x10	/* Korean Standard */

#define	MAXLENGTH_SMS_USER_DATA						160
//------------------------------------------------------------------------------
// PARAMID_USER_RESPONSE_CODE		0x09	/* User Response Code */
//------------------------------------------------------------------------------
/* USER_RESPONSE_CODE
*/
// This field value depends on the Teleservices or the carrier
// This field had used in the Interactive Teleservices in Korea
// currently the wireless service carriers adopt the browser( WAP,etc )
// instead of SMS protocol in order to serve the interactive teleservice.

//------------------------------------------------------------------------------
// PARAMID_MC_TIME_STAMP				0x0A	/* Message Center Time Stamp */
//------------------------------------------------------------------------------

/* YEAR
*/
// 4bit BCD Code,  example : if the year is 2002, the YEAR field contains 0x02.

/* MONTH
*/
// 1 ~ 12 ( in decimal )

/* DAY
*/
// 1 ~ 31( in decimal )

/* HOUR
*/
// 0 ~ 23 ( in decimal )

/* MINUTE
*/
// 0 ~ 59 ( in decimal )

/* SECOND
*/
// 0 ~ 59 ( in decimal )

//------------------------------------------------------------------------------
// PARAMID_VALIDITY_PERIOD_ABS		0x0B	/* Validity Period - Absolute */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// PARAMID_VALIDITY_PERIOD_REL		0x0C	/* Validiry Period - Relative */
//------------------------------------------------------------------------------
/* VALIDITY
*/

//------------------------------------------------------------------------------
// PARAMID_DEFERRED_DELIVERY_ABS		0x0D	/* Deferred Delivery Time - Absolute */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// PARAMID_DEFERRED_DELIVERY_REL		0x0E	/* Deferred Delivery Time - Relative */
//------------------------------------------------------------------------------
/* DELIVERY_TIME
*/

//------------------------------------------------------------------------------
// PARAMID_PRIORITY					0x0F	/* Priority Indicator */
//------------------------------------------------------------------------------
#define PRIORITY_NORMAL					0x00
#define PRIORITY_INTERACTIVE			0x01
#define PRIORITY_URGENT					0x02
#define PRIORITY_EMERGENCY				0x03

//------------------------------------------------------------------------------
// PARAMID_PRIVACY					0x10	/* Privacy Indicator */
//------------------------------------------------------------------------------
#define PRIVACY_NOT_RESTRICTED			0x00
#define PRIVACY_RESTRICTED				0x01
#define PRIVACY_CONFIDENTIAL			0x02
#define PRIVACY_SECRET					0x03

//------------------------------------------------------------------------------
// PARAMID_REPLY_OPTION				0x11	/* Reply Option */
//------------------------------------------------------------------------------

// USER_ACK_REQ : user ack requested
#define SMS_USER_ACK_NOT_REQUESTED		0x00
#define SMS_USER_ACK_REQUESTED			0x01

// DAK_REQ : delivery ack requested
#define SMS_DELIVERY_ACK_NOT_REQUESTED	0x00
#define SMS_DELIVERY_ACK_REQUESTED		0x01

//------------------------------------------------------------------------------
// PARAMID_NUMBER_OF_MESSAGE			0x12	/* Number of Messages : Voice Mail Count */
//------------------------------------------------------------------------------
/* MESSAGE_COUNT : voice mail count ( 0 - 99 )
*/
// refer to		MAXLENGTH_SMS_VOICE_MAIL_COUNT		99

//------------------------------------------------------------------------------
// PARAMID_ALERT_ON_DELIVERY			0x13	/* Alert on Message Delivery */
//------------------------------------------------------------------------------
// IS-637B Feature : not yet adopted
//		IPC_ALERT_PRIORITY_DEFAULT			0x00	/* mobile default */
//		IPC_ALERT_PRIORITY_LOW				0x01	/* low priority alert: This alert is defined by the mobile station */
//		IPC_ALERT_PRIORITY_MED				0x02	/* medium priority alert : This alert is defined by the mobile station */
//		IPC_ALERT_PRIORITY_HIGH				0x03	/* high priority alert : This alert is defined by the mobile station */

//------------------------------------------------------------------------------
// PARAMID_LANGUAGE					0x14	/* Langauge Indicator */
//------------------------------------------------------------------------------
#define IPC_LANGUAGE_UNKNOWN				0x00
#define IPC_LANGUAGE_ENGLISH				0x01
#define IPC_LANGUAGE_FRENCH					0x02
#define IPC_LANGUAGE_SPANISH				0x03
#define IPC_LANGUAGE_JAPANESE				0x04
#define IPC_LANGUAGE_KOREAN					0x05
#define IPC_LANGUAGE_CHINESE				0x06
#define IPC_LANGUAGE_HEBREW					0x07
#define IPC_LANGUAGE_KOREAN1				0x40	/* Used in korean 3 PCS's and STI */
#define IPC_LANGUAGE_KOREAN_SKT				0xFE	/* Used in only SKT  */

//------------------------------------------------------------------------------
// PARAMID_CALLBACK					0x15	/* Call Back Number */
//------------------------------------------------------------------------------
/* DIGIT_MODE : CHARi[] filed�� 4bit dtmf code���� 8bit code������ �ǹ��Ѵ�.
*/
// refer to :
//		DIGITMODE_4BITDTMF			0x00
//		DIGITMODE_8BITCODE			0x01

/* NUMBER_TYPE
*/
// refer to :
//		NUMBER_TYPE_UNKNOWN			0x00
//		NUMBER_TYPE_INTERNATIONAL		0x01
//		NUMBER_TYPE_NATIONAL			0x02
//		NUMBER_TYPE_NETWORK			0x03
//		NUMBER_TYPE_SUBSCRIBER		0x04
//		NUMBER_TYPE_RESERVED_5		0x05
//		NUMBER_TYPE_ABREVIATED		0x06
//		NUMBER_TYPE_RESERVED_7		0x07

/* NUMBER_PLAN
*/
// refer to :
//		NUMBER_PLAN_UNKNOWN			0x00
//		NUMBER_PLAN_TELEPHONY			0x01	/* CCITT E.164 and E.163,  including ISDN plan */
//		NUMBER_PLAN_RESERVED_2		0x02
//		NUMBER_PLAN_DATA				0x03	/* CCITT X.121 */
//		NUMBER_PLAN_TELEX				0x04	/* CCITT F.69 */
//		NUMBER_PLAN_RESERVED_5		0x05
//		NUMBER_PLAN_RESERVED_6		0x06
//		NUMBER_PLAN_RESERVED_7		0x07
//		NUMBER_PLAN_RESERVED_8		0x08
//		NUMBER_PLAN_PRIVATE			0x09
//		NUMBER_PLAN_RESERVED_10		0x0A
//		NUMBER_PLAN_RESERVED_11		0x0B
//		NUMBER_PLAN_RESERVED_12		0x0C
//		NUMBER_PLAN_RESERVED_13		0x0D
//		NUMBER_PLAN_RESERVED_14		0x0E
//		NUMBER_PLAN_RESERVED_15		0x0F

/* NUM_FIELDS
*/
// refer to :
//		MAXLENGTH_SMS_ADDRESS			32

/* CHARi
*/

//------------------------------------------------------------------------------
// IPC_SMSPARAMID_DISPLAY_MODE				0x16	/* Display Mode */
//------------------------------------------------------------------------------
// IS-637B Feature : not yet adopted


//------------------------------------------------------------------------------
// IPC_SMSPARAMID_MULTI_ENCODING_USER_DATA	0x17	/* Multiply Encoding User Data */
//------------------------------------------------------------------------------
// refer to :

// Teleservice Identifier
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned short			teleserv_id;
} packed__ ParamTeleserv_t;

// Broadcast Service Category
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned short			category;
} packed__ ParamBcServCat_t;

// Address
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			digit;
	unsigned char			num;
	unsigned char			numtype;
	unsigned char			numplan;
	unsigned char			len;
	unsigned char			CHARi[MAXLENGTH_SMS_ADDRESS];
} packed__ ParamAddress_t;

// Sub-Address
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			type;
	unsigned char			odd;
	unsigned char			len;
	unsigned char			CHARi[MAXLENGTH_SMS_ADDRESS];
} packed__ ParamSubAddress_t;

// Bearer Reply Option
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned short			repyseq;
} packed__ ParamBearerReplyOption_t;

// Cause Codes
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			repyseq;
	unsigned char			class;
	unsigned char			code;
} packed__ ParamErrorCause_t;

// Message Identifier
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			type;
	unsigned short			msg_id;
} packed__ ParamMsgId_t;

// User Data
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			encoding;
	unsigned char			len;
	unsigned char			CHARi[MAXLENGTH_SMS_USER_DATA];
} packed__ ParamUserData_t;

// User Response code
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			code;
} packed__ ParamUserResp_t;

// Message Center Time Stamp, Validity Period, Deferred Delivery Time
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			year;
	unsigned char			month;
	unsigned char			day;
	unsigned char			hours;
	unsigned char			minutes;
	unsigned char			seconds;
} packed__ ParamValidPeriodA_t, ParamDefDeliverTimeA_t;

// Validity Period - Relative
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			valid;
} packed__ ParamValidPeriodR_t, ParamDefDeliverTimeR_t;

// Priority Indicator, Alert on Message Delivery
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			priority;
} packed__ ParamPriority_t, ParamAlertPriority_t;

// Privacy Indicator
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			privacy;
} packed__ ParamPrivacy_t;

// Reply Option
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			user_ack;
	unsigned char			delivery_ack;
} packed__ ParamReplyOption_t;

// Number of Messages
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			count;
} packed__ ParamNumOfMsg_t;

// Language indicator
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			language;
} packed__ ParamLangInd_t;

// Message display Mode
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			disply_mode;
} packed__ ParamMsgDisplayMode_t;

// Address
typedef struct{
	unsigned char			param_id;
	unsigned char			param_len;
	unsigned char			digit;
	unsigned char			numtype;
	unsigned char			numplan;
	unsigned char			len;
	unsigned char			CHARi[MAXLENGTH_SMS_ADDRESS];
} packed__ ParamCallback_t;

typedef struct{
	unsigned long			index;
	unsigned char			data[255];
} packed__ Message_t;

void register_sms_event(TelHandle*handle);

#endif //TAPI_SMS_CDMA_H
// EOF

