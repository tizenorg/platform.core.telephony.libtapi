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

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>     /* pthread functions and data structures */
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include "assert.h"
#include "glib.h"
#include "menu.h"

#define SMS_ADDRESS_LEN_MAX 20

#define BASE_YEAR 70L /* 1970 is the base year */
#define MAX_YEAR 130L /* 2030 is the max year */

#define SET_TON_NPI(dest, ton, npi) { \
	dest = 0x80; \
	dest |= (ton & 0x07) << 4; \
	dest |= npi & 0x0F; \
}

typedef enum
{
	SMS_CODGRP_SM_GENERAL_DCS,
	SMS_CODGRP_CB_GENERAL_LANG,
	SMS_CODGRP_CB_ALPHA,
	SMS_CODGRP_CB_RES1,
	SMS_CODGRP_CB_RES2,
	SMS_CODGRP_SM_AUTO_DELETION, /* 01xx Bits 5..0 are coded the same as SM_GENERAL_DCS */
	SMS_CODGRP_CB_GENERAL_DCS, /* 01xx behavior is as SM_GENERAL_DCS */
	SMS_CODGRP_SM_WAITING_DISCARD,
	SMS_CODGRP_SM_WAITING_STORE,
	SMS_CODGRP_SM_WAITING_STORE_UCS2,
	SMS_CODGRP_CB_WAP,
	SMS_CODGRP_SM_CLASS_CODING,
	SMS_CODGRP_CB_CLASS_CODING,
	SMS_CODGRP_MAX
} SmsCodingGroupType;

typedef enum
{
	 SMS_WAITING_VOICE_MSG, /* voice message waiting */
	 SMS_WAITING_FAX_MSG, /* FAX message waiting */
	 SMS_WAITING_EMAIL_MSG, /* E-mail waiting */
	 SMS_WAITING_OTHER_MSG, /* Other message waiting */
	 SMS_WAITING_MAX
} SmsWaitingType;

typedef enum
{
	SMS_ALPHABET_DEFAULT, /* GSM alphabet default 7bit */
	SMS_ALPHABET_8BIT, /* 8bit */
	SMS_ALPHABET_UCS2, /* 16bit ucs2 */
	SMS_ALPHABET_KSC5601, /* for Korean */
	SMS_ALPHABET_MAX
} SmsAlphabetType;

typedef enum
{
	SMS_CLASS_0, /* display immediately */
	SMS_CLASS_1, /* may be stored */
	SMS_CLASS_2, /* store in SIM */
	SMS_CLASS_3, /* transfer to TE */
	SMS_CLASS_NONE,
	SMS_CLASS_MAX
} SmsClassType;

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
} SimTon;

typedef enum
{
	SMS_TPDU_DELIVER = 0x00, // sc -> ms
	SMS_TPDU_DELIVER_REPORT = 0x00, // ms -> sc
	SMS_TPDU_SUBMIT = 0x01, // ms -> sc
	SMS_TPDU_SUBMIT_REPORT = 0x01, // sc -> ms
	SMS_TPDU_STATUS_REPORT = 0x02, // sc -> ms
	SMS_TPDU_COMMAND = 0x02, // ms -> sc
	SMS_PDU_INVALID_TYPE
} SmsTpduType;

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
} SimNumberingPlanIdentity;

typedef enum
{
	SMS_VP_NOT_USED = 0, /**< validity period not used */
	SMS_VP_RELATIVE = 2, /**< after a predetermined time period after the service center has received the message */
	SMS_VP_ABSOLUTE = 3,
	SMS_VP_ENHANCED = 1,
	SMS_VP_MAX
} SmsVpType;

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
} SmsVpRelValue;

typedef struct
{
	gboolean compressed; /* if text is compressed this is TRUE */
	gboolean bmsg_class_set; /* message class indication. if TRUE = bits 1..0, if FALSE = reserved */
	gboolean bmsg_ind_active; /* message indication group. if TRUE = on, FALSE = off */
	SmsCodingGroupType coding_group_type; /* Coding group, GSM 03.38 */
	SmsWaitingType waiting_type; /* Type of message waiting */
	SmsAlphabetType alphabet_type; /* How to encode a message. */
	SmsClassType class_type; /* The message class */
} SmsCodingScheme;

typedef struct {
	unsigned int dial_num_len; /**< length of address. If Service center address is not set, then this will be zero */
	SimTon ton; /**< Type of number*/
	SimNumberingPlanIdentity npi; /**<Numbering plan identification*/
	unsigned char sz_dialling_num[SMS_ADDRESS_LEN_MAX + 1]; /**< destination address. If Address not set, then set to 0xFF */
} SmsAddrInfo;

typedef struct
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} TmDateTime;

typedef struct
{
	/************HEADER************/
	SmsTpduType msg_type;   //bit 0 ,1
	int mms; // bit 2
	int rp; // bit 7
	int udhi;  //bit 6
	int sri; // bit , 5status_report_indication

	/************BODY************/
	SmsAddrInfo org_addr;
	unsigned char  pid;
	TmDateTime scts;
	SmsCodingScheme dcs;
	unsigned int udl;
	char  *user_data;
} TpduSmsDeliver;

typedef struct {
	unsigned int dial_num_len; /**< length of address. If Service center address is not set, then this will be zero */
	SimTon ton; /**< Type of number*/
	SimNumberingPlanIdentity npi; /**<Numbering plan identification*/
	unsigned char dialling_num[SMS_ADDRESS_LEN_MAX + 1]; /**< destination address. If Address not set, then set to 0xFF */
} SmsAddressInfo;

typedef struct
{
	SmsVpType vp_type; /**< Validity period format */
	SmsVpRelValue vp_rel_type; /**< Validity period value duration the message must retain in the service center while application attempts delivery */
	unsigned long vp_value; /** Validity period value */
} SmsVp;

typedef struct
{
	/************HEADER************/
	SmsTpduType  msg_type; /* bit 0 ,1 (message type) */
	int rd; /* bit 2 (reject duplicates) */
	SmsVpType vpf; /* bits 3, 4 (validity period format) */
	int rp; /* bit 7 (reply path) */
	int udhi; /* bit 6 (user data header indication) */
	int  srr; /* bit 5 (status report request) */

	/************BODY************/
	unsigned int mr; /* (message reference) */
	SmsAddressInfo dest_addr; /*(destination address) */
	unsigned char pid; /* (protocol identifier) */
	SmsCodingScheme dcs; /* (data coding scheme) */
	SmsVp vp; /* (validity period) */
	unsigned int udl; /* (user data length) */
	char *user_data; /* (user data) */
} TpduSmsSubmit;

typedef struct
{
	/************HEADER************/
	SmsTpduType msg_type; //bit 0 ,1
	int udhi; //bit 6
	int mms; //bit 2
	int srq; //bit 5, status_report_qualifier

	/************BODY************/
	unsigned int mr;
	SmsAddressInfo rcp_addr; //recipient_addr
	TmDateTime scts;
	TmDateTime dt; //discharge_time
	unsigned char status;
	unsigned char para_ind;
	unsigned char pid;
	SmsCodingScheme dcs;
	unsigned int udl;
	char * user_data;
}TpduSmsStatusReport;

void sms_util_decode_dcs(SmsCodingScheme *coding_scheme, unsigned char dcs);
void sms_util_convert_bcd_2_digit( char* digits, char* bcd, int digit_len );
char *sms_util_unpack_gsm_code(char *sz_data, const char *in, int in_len );
int sms_util_decode_addr_field(char *dialling_num, char* addr_field, int *result_ton, int *result_npi);
TmDateTime *sms_util_decode_time_stamp(char *time_stamp, TmDateTime *tm_date_time );
void sms_util_convert_digit_2_bcd( char *bcd, char *digits, int digit_len );
int sms_util_pack_gsm_code( unsigned char *out, const char *data, int in_len );
int sms_util_encode_addr_field(unsigned char *addr_field, char* dialling_num,
	int dial_num_len, int ton, int npi);
void sms_util_encode_dcs( unsigned char *result_dcs, SmsCodingScheme *coding_scheme );
unsigned char sms_util_encode_validity( unsigned char *validity, SmsVp *vp );

