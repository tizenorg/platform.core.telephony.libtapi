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

#ifndef _UTS_TEL_SEND_SMS_MSG_H_
#define _UTS_TEL_SEND_SMS_MSG_H_

static void utc_tel_send_sms_msg_01();
static void utc_tel_send_sms_msg_02();
static void utc_tel_send_sms_msg_03();
static void utc_tel_send_sms_msg_04();

#define MAXLENGTH_SMS_ADDRESS 20
#define MAXLENGTH_SMS_USER_DATA 160
#define TELESERVICEID_CMT95 0x1002
#define NUMBER_PLAN_TELEPHONY 0x01
#define NUMBER_PLAN_UNKNOWN  0
#define MESSAGETYPE_SUBMIT 2

/* Common TELESERVICE ID's */
#define TELESERVICEID_CPT95				0x1001	/* 4097 : Paging */
#define TELESERVICEID_CMT95				0x1002	/* 4098 : Text Message */
#define TELESERVICEID_VMN95				0x1003	/* 4099 : Voice Mail Notification */
#define TELESERVICEID_WAP				0x1004	/* WAP Message */

/* DIGIT MODE : CHARi[] filed°¡ 4bit dtmf codeÀÎÁö 8bit codeÀÎÁöžŠ ÀÇ¹ÌÇÑŽÙ.
*/
#define DIGITMODE_4BITDTMF			0x00
#define DIGITMODE_8BITCODE			0x01

/* NUMBER MODE
*/
#define NUMMODE_NONE_DATANETWORK	0x00	/* in ANSI TI.607 */
#define NUMMODE_DATANETWORK			0x01

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
#define MSGENCODING_KSC5601_3PCS					0x10	/* Korean Standard */


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

//=============================================================
#define PARAMID_TELESERVICE_ID			0x01	/* Teleservice Identifier */
#define PARAMID_SERVICE_CATEGORY		0x02	/* Broadcast Service Category */
#define PARAMID_ADDRESS					0x03	/* Address */
#define PARAMID_SUBADDRESS				0x04	/* Subaddress */
#define PARAMID_BEARER_REPLY			0x05	/* Bearer Reply Option */
#define PARAMID_CAUSE_CODES				0x06	/* Cause Codes */
#define PARAMID_MESSAGE_ID				0x07	/* Message Identifier */
#define PARAMID_USER_DATA				0x08	/* User Data */
#define PARAMID_USER_RESPONSE_CODE		0x09	/* User Response Code */
#define PARAMID_MC_TIME_STAMP			0x0A	/* Message Center Time Stamp */
#define PARAMID_VALIDITY_PERIOD_ABS		0x0B	/* Validity Period - Absolute */
#define PARAMID_VALIDITY_PERIOD_REL		0x0C	/* Validiry Period - Relative */
#define PARAMID_DEFERRED_DELIVERY_ABS	0x0D	/* Deferred Delivery Time - Absolute */
#define PARAMID_DEFERRED_DELIVERY_REL	0x0E	/* Deferred Delivery Time - Relative */
#define PARAMID_PRIORITY				0x0F	/* Priority Indicator */
#define PARAMID_PRIVACY					0x10	/* Privacy Indicator */
#define PARAMID_REPLY_OPTION			0x11	/* Reply Option */
#define PARAMID_NUMBER_OF_MESSAGE		0x12	/* Number of Messages : Voice Mail Count */
#define PARAMID_ALERT_ON_DELIVERY		0x13	/* Alert on Message Delivery */
#define PARAMID_LANGUAGE				0x14	/* Langauge Indicator */
#define PARAMID_CALLBACK				0x15	/* Call Back Number */
#define PARAMID_DISPLAY_MODE			0x16	/* Display Mode */
#define PARAMID_MULTI_ENCODING_USER_DATA	0x17	/* Multiply Encoding User Data */
#define PARAMID_MEMORY_INDEX			0x18	/* Memory address stored in Phone Memory */
#define PARAMID_BEARER_DATA				0x19	/* Bearer data - raw data  */

typedef struct {
	TS_UINT8 param_id;
	TS_UINT8 param_len;
	TS_UINT16 teleserv_id;
} ParamTeleserv_t;

typedef struct {
	TS_UINT8 param_id;
	TS_UINT8 param_len;
	TS_UINT8 digit;
	TS_UINT8 num;
	TS_UINT8 numtype;
	TS_UINT8 numplan;
	TS_UINT8 len;
	TS_UINT8 CHARi[MAXLENGTH_SMS_ADDRESS];
} ParamAddress_t;

typedef struct {
	TS_UINT8 param_id;
	TS_UINT8 param_len;
	TS_UINT8 type;
	TS_UINT16 msg_id;
} ParamMsgId_t;

typedef struct {
	TS_UINT8 param_id;
	TS_UINT8 param_len;
	TS_UINT8 encoding;
	TS_UINT8 len;
	TS_UINT8 CHARi[MAXLENGTH_SMS_USER_DATA];
} ParamUserData_t;

typedef struct {
	TS_UINT8 param_id;
	TS_UINT8 param_len;
	TS_UINT8 digit;
	TS_UINT8 numtype;
	TS_UINT8 numplan;
	TS_UINT8 len;
	TS_UINT8 CHARi[MAXLENGTH_SMS_ADDRESS];
} ParamCallback_t;


#endif
