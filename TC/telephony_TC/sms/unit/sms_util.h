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


#ifndef __TET_SMS_UTIL_H_
#define __TET_SMS_UTIL_H_

#include "ITapiNetText.h"

#define SIM_TON_UNKNOWN					0 //unknown
#define	SIM_TON_INTERNATIONAL			1 //international number
#define	SIM_TON_NATIONAL				2 //national number
#define SIM_TON_NETWORK_SPECIFIC		3 //natwork specific number
#define	SIM_TON_DEDICATED_ACCESS		4 //subscriber number
#define	SIM_TON_ALPHA_NUMERIC			5//alphanumeric, GSM 7-bit default alphabet)
#define	SIM_TON_ABBREVIATED_NUMBER		6 //abbreviated number
#define	SIM_TON_RESERVED_FOR_EXT		7 //reserved for extension


#define MAX_UDH_LEN		137
#define UDH_MAX			7
#define SMDATA_SIZE_MAX 160

#define FAIL			0x00
#define SUCCESS			0x01

//////////////////////////////////TPDU type////////////////////////////////
typedef enum {
	SMS_TPDU_DELIVER			= 0x00, // sc -> ms
	SMS_TPDU_DELIVER_REPORT		= 0x00, // ms -> sc
	SMS_TPDU_SUBMIT				= 0x01, // ms -> sc
	SMS_TPDU_SUBMIT_REPORT		= 0x01, // sc -> ms
	SMS_TPDU_STATUS_REPORT		= 0x02, // sc -> ms
	SMS_TPDU_COMMAND			= 0x02, // ms -> sc
	SMS_PDU_INVALID_TYPE
} SmsTPDUType;

typedef enum {
	SMS_DAEMON_UNKNOWN_MESSAGE,
	SMS_DAEMON_RECEIVED_UNREAD_MESSAGE,
	SMS_DAEMON_RECEIVED_READ_MESSAGE,
	SMS_DAEMON_UNSENT_MESSAGE,
	SMS_DAEMON_STORED_SENT_MESSAGE,
} sd_message_status;


/*
 * tapi_nettext_vp_type
 * This enumeration defines the validity period type.
 */
typedef enum {
	TAPI_NETTEXT_VP_NOT_USED = 0,		/**< validity period not used */
	TAPI_NETTEXT_VP_RELATIVE = 2,		/**< after a predetermined time period after the service center has received the message */
	TAPI_NETTEXT_VP_ABSOLUTE = 3,
	TAPI_NETTEXT_VP_ENHANCED = 1,

	TAPI_NETTEXT_VP_MAX
} tapi_nettext_vp_type;

/*
 * tapi_nettext_coding_group_type
 This enum defined the different sms coding group types
 */
typedef enum {
	TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS,			/**< Bits 7..4 00xx */
	TAPI_NETTEXT_CODGRP_CB_GENERAL_LANG,		/**< 0000 */
	TAPI_NETTEXT_CODGRP_CB_ALPHA,				/**< 0001 */
	TAPI_NETTEXT_CODGRP_CB_RES1,				/**< 0010 */
	TAPI_NETTEXT_CODGRP_CB_RES2,				/**< 0011 */
	TAPI_NETTEXT_CODGRP_SM_AUTO_DELETION,		/**< 01xx Bits 5..0 are coded the same as SM_GENERAL_DCS */
	TAPI_NETTEXT_CODGRP_CB_GENERAL_DCS,			/**< 01xx behavior is as SM_GENERAL_DCS */
	TAPI_NETTEXT_CODGRP_SM_WAITING_DISCARD,		/**< 1100 */
	TAPI_NETTEXT_CODGRP_SM_WAITING_STORE,		/**< 1101 */
	TAPI_NETTEXT_CODGRP_SM_WAITING_STORE_UCS2,	/**< 1110 */
	TAPI_NETTEXT_CODGRP_CB_WAP,					/**< 1110 Cell Broadcast */
	TAPI_NETTEXT_CODGRP_SM_CLASS_CODING,		/**< 1111 */
	TAPI_NETTEXT_CODGRP_CB_CLASS_CODING,		/**< 1111 Cell Broadcast */

	TAPI_NETTEXT_CODGRP_MAX
} tapi_nettext_coding_group_type;

/*
 *  tapi_nettext_waiting_type
 Defines different types of waiting indicators
 */
typedef enum {
	TAPI_NETTEXT_WAITING_VOICE_MSG,		/**< voice message waiting */
	TAPI_NETTEXT_WAITING_FAX_MSG,		/**< FAX message waiting */
	TAPI_NETTEXT_WAITING_EMAIL_MSG,		/**< E-mail waiting */
	TAPI_NETTEXT_WAITING_OTHER_MSG,		/**< Other message waiting */

	TAPI_NETTEXT_WAITING_MAX
} tapi_nettext_waiting_type;

/*
 * tapi_nettext_alphabet_type
 Defines different alphabet types of an sms
 */
typedef enum {
	TAPI_NETTEXT_ALPHABET_DEFAULT,	/**< GSM alphabet default 7bit */
	TAPI_NETTEXT_ALPHABET_8BIT,		/**< 8bit */
	TAPI_NETTEXT_ALPHABET_UCS2,		/**< 16bit ucs2 */

	TAPI_NETTEXT_ALPHABET_KSC5601,	/**< for Korean */

	TAPI_NETTEXT_ALPHABET_MAX
} tapi_nettext_alphabet_type;

/*
 * tapi_nettext_class_type
 This enum defines the different classes of SMS which indicates the
 type of storage of the sms.
 */
typedef enum {
	TAPI_NETTEXT_CLASS_0,	/**< display immediately */
	TAPI_NETTEXT_CLASS_1,	/**< may be stored */
	TAPI_NETTEXT_CLASS_2,	/**< store in SIM */
	TAPI_NETTEXT_CLASS_3,	/**< transfer to TE */
	TAPI_NETTEXT_CLASS_NONE,

	TAPI_NETTEXT_CLASS_MAX
} tapi_nettext_class_type;

/*
This structure contains the information related to sms coding schemes

*/
typedef struct {
	TS_BOOL							bCompressed;		/**< if text is compressed this is TRUE */
	TS_BOOL							bmsg_class_set;		/**< message class indication. if TRUE = bits 1..0, if FALSE = reserved */
	TS_BOOL							bmsg_ind_active;		/**< message indication group. if TRUE = on, FALSE = off */
	tapi_nettext_coding_group_type	coding_group_type;	/**< Coding group, GSM 03.38 */
	tapi_nettext_waiting_type		waiting_type;		/**< Type of message waiting */
	tapi_nettext_alphabet_type		alphabet_type;		/**< How to encode a message. */
	tapi_nettext_class_type			class_type;			/**< The message class */
} tapi_nettext_coding_scheme;

/*
 *  tapi_nettext_vp_rel_Value
 * This structure defines the validity period types.
 */
typedef enum {
	TAPI_NETTEXT_VP_REL_1H,		/**< One hour */
	TAPI_NETTEXT_VP_REL_6H,		/**< Six hours */
	TAPI_NETTEXT_VP_REL_12H,	/**< Twelve hours */
	TAPI_NETTEXT_VP_REL_1D,		/**< One day */
	TAPI_NETTEXT_VP_REL_3D,		/**< Three days */
	TAPI_NETTEXT_VP_REL_1W,		/**< One week */
	TAPI_NETTEXT_VP_REL_1M,		/**< One Month */
	TAPI_NETTEXT_VP_REL_MAX		/**< Max = 63 weeks */
} tapi_nettext_vp_rel_Value;

/*
This structure contains all the information
related to validity period of a message.
*/
typedef struct {
	tapi_nettext_vp_type vp_type;			/**< Validity period format */
	tapi_nettext_vp_rel_Value vp_rel_type;	/**< Validity period value duration the message must retain in the service center while application attempts delivery */
	TS_UINT32 vpValue;						/** Validity period value */
} tapi_nettext_vp;


/* This structure contains the details of delete status notification, which comes after calling
 tel_delete_sms_in_simMsg*/
typedef struct {
	TS_UINT16 result;
	TS_UINT8 index;
} tel_delete_sms_in_simMsg_status_ind;


/* This structure contains the details of save status notification, which comes after calling
 tel_write_sms_in_simMsg*/

typedef struct {
	TS_UINT16 result;
	TS_UINT8 index;
} tapi_nettext_save_status_ind;

typedef struct {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} TmDateTime;


#define BASE_YEAR				70L		/* 1970 is the base year */
#define MAX_YEAR				130L	/* 2030 is the max year */
#define	SIM_SMSP_ADDRESS_LEN	20		/* digit length */

#define SET_TON_NPI(dest, ton, npi) {	\
	dest = 0x80;						\
	dest |= (ton & 0x07) << 4;			\
	dest |= npi & 0x0F;					\
}

typedef struct {
	/************HEADER************/

	// message type [M]
	SmsTPDUType msg_type; //bit 0 ,1

	// reject duplicates [M]
	TS_BOOL rd; // bit 2

	// validity period format [M]
	tapi_nettext_vp_type vpf; //  bits 3, 4

	// reply path [M]
	TS_BOOL rp; //bit 7

	// user data header indication
	TS_BOOL udhi; //bit 6

	// status report request
	TS_BOOL srr; // bit 5

	/************BODY************/

	//message reference [M]
	TS_UINT mr;

	// destination address [M]
	TelSmsAddressInfo_t desAddr;

	// protocol identifier [M]
	TS_BYTE pId;

	// data coding scheme [M]
	tapi_nettext_coding_scheme dcs;

	// validity period
	tapi_nettext_vp vp;

	// user data length [M]
	TS_UINT udl;

	// user data
	TS_BYTE * userData;

} TPDU_SMS_SUBMIT;

typedef struct {
	/************HEADER************/

	// message type [M]
	SmsTPDUType msg_type; //bit 0 ,1

	// user data header indication
	TS_BOOL udhi; //bit 6

	/************BODY************/

	// failure Cause [M]
	TS_UINT fcs;

	// parameter indicator [M]
	TS_BYTE paraInd;

	// service centre time stamp [M]
	TmDateTime scts;

	// protocol identifier [M]
	TS_BYTE pId;

	// data coding scheme
	tapi_nettext_coding_scheme dcs;

	// user data length [M]
	TS_UINT udl;

	// user data
	TS_BYTE * userData;

} TPDU_SMS_SUBMIT_REPORT;

typedef struct {
	/************HEADER************/
	SmsTPDUType msg_type; //bit 0 ,1

	TS_BOOL mms; // bit 2

	TS_BOOL rp; // bit 7

	TS_BOOL udhi; //bit 6

	TS_BOOL sri; // bit , 5status_report_indication

	/************BODY************/

	TelSmsAddressInfo_t orgAddr;

	TS_BYTE pId;

	TmDateTime scts;

	tapi_nettext_coding_scheme dcs;

	TS_UINT udl;

	TS_BYTE * userData;

} TPDU_SMS_DELIVER;

typedef struct {
	/************HEADER************/
	SmsTPDUType msg_type; //bit 0 ,1

	TS_BOOL udhi; //bit 6

	/************BODY************/

	TS_UINT fcs;

	TS_BYTE paraInd;

	TS_BYTE pId;

	tapi_nettext_coding_scheme dcs;

	TS_UINT udl;

	TS_BYTE * userData;

} TPDU_SMS_DELIVER_REPORT;

typedef struct {
	/************HEADER************/
	SmsTPDUType msg_type; //bit 0 ,1

	TS_BOOL udhi; //bit 6

	TS_BOOL srr; //bit 5, status_report_request

	/************BODY************/

	TS_UINT mr; //message_ref

	TS_BYTE pId;

	TS_BYTE cmdType;

	TS_BYTE msgNum;

	TelSmsAddressInfo_t destAddr;

	TS_UINT udl;

	TS_BYTE * userData;

} TPDU_SMS_COMMAND;

typedef struct {
	/************HEADER************/

	SmsTPDUType msg_type; //bit 0 ,1

	TS_BOOL udhi; //bit 6

	TS_BOOL mms; //bit 2

	TS_BOOL srq; //bit 5, status_report_qualifier

	/************BODY************/

	TS_UINT mr;

	TelSmsAddressInfo_t rcpAddr; //recipient_addr

	TmDateTime scts;

	TmDateTime dt; //discharge_time

	TS_BYTE status;

	TS_BYTE paraInd;

	TS_BYTE pId;

	tapi_nettext_coding_scheme dcs;

	TS_UINT udl;

	TS_BYTE * userData;

} TPDU_SMS_STATUS_REPORT;

/**************************************************************************************
*** (main.c) function header****************************************************************
***************************************************************************************
***************************************************************************************/
int SendMessage();
int ReadMessage();
int DeleteMessage();
int ReceiveMessage(int pdu_len, char * pPDU);
int GetInformation();
int Setting();

TS_BOOL DecodeCellBroadcastMsg(int length, char * pPDU);
TS_BOOL EncodeSmsSubmitTpdu(char* diallingNum, int dialNumLen, char* msg, int msg_len);
TS_BOOL DecodeSmsSubmitTpdu(int pdu_len, char * pPDU);
int DecodeSmsDeliverTpdu(int pdu_len, char * pPDU);
TS_BOOL EncodeSmsDeliverReportTpdu();
TS_BOOL DecodeSmsSubmitReportTpdu(int length, char *data);
TS_BOOL DecodeSmsStatusReportTpdu(int pdu_len, char * pPDU);

/***************************************************************************************/
void reverse(char* x, int len);
char* AcItoa(int n, char* str, int b);
int AcToupper(int ch);

char* SmsUtilUnpackGSMCode(char* szData, const TS_BYTE* pIn, int in_len);
int SmsUtilPackGSMCode(TS_BYTE* pOut, const char* szData, int in_len);

void SmsUtilConvertBCD2Digit(char* pDigits, char* pBCD, int digitLen);
void SmsUtilConvertDigit2BCD(char* pBCD, char* pDigits, int digitLen);

TmDateTime* SmsUtilDecodeTimeStamp(unsigned char* pTimeStamp, TmDateTime *tmDateTime);
unsigned char* SmsUtilEncodeTimeStamp(TmDateTime* tmDateTime, unsigned char* pTimeStamp);

void SmsUtilDecodeAddrField(char *diallingNum, unsigned char * pAddrField, int *result_ton, int *result_npi);
int SmsUtilEncodeAddrField(unsigned char* pAddrField, char* diallingNum, int dialnumLen, int ton, int npi);

int SmsUtilDecodeScAddrField(TelSmsAddressInfo_t* pSmsAddrField, TS_BYTE* pAddrField);
int SmsUtilEncodeScAddrField(TS_BYTE* pAddrField, TelSmsAddressInfo_t * pSmsAddrField);

void SmsUtilDecodeDCS(tapi_nettext_coding_scheme* pCodingScheme, unsigned char dcs);
void SmsUtilEncodeDCS(TS_BYTE* pDCS, tapi_nettext_coding_scheme* pCodingScheme);
TS_UINT8 SmsUtilEncodeValidity(TS_BYTE* pValidity, tapi_nettext_vp* pVP);

void PrepareMessage(TelSmsDatapackageInfo_t *pDataPackage);

#endif


