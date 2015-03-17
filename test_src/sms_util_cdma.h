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

#pragma once

typedef enum {
	SMS_3GPP2_POINT_TO_POINT 	= 0,
	SMS_3GPP2_BROADCAST 			= 1,
	SMS_3GPP2_ACKNOWLEDGE 		= 2
} sms_3gpp2_type;

typedef enum {
	SMS_3GPP2_PARAM_ID_TELESERVICE_ID 				= 0,
	SMS_3GPP2_PARAM_ID_SERVICE_CATEGORY 			= 1,
	SMS_3GPP2_PARAM_ID_ORIGINATING_ADDRESS 		= 2,
	SMS_3GPP2_PARAM_ID_ORIGINATING_SUBADDRESS 	= 3,
	SMS_3GPP2_PARAM_ID_DESTINATION_ADDRESS 		= 4,
	SMS_3GPP2_PARAM_ID_DESTINATION_SUBADDRESS 	= 5,
	SMS_3GPP2_PARAM_ID_BEARER_REPLY_OPTION 		= 6,
	SMS_3GPP2_PARAM_ID_CAUSE_CODES 				= 7,
	SMS_3GPP2_PARAM_ID_BEARER_DATA 				= 8
} sms_3gpp2_param_id_type;

#define SMS_3GPP2_TELESERVICE_ID_EPES 0x1000 /* 4096: AMPS Extended Protocol Enhanced Services */
#define SMS_3GPP2_TELESERVICE_ID_CPT 0x1001 /* 4097: CDMA Cellular Paging Teleservice */
#define SMS_3GPP2_TELESERVICE_ID_CCMT 0x1002 /* 4098: CDMA Cellular Messaging Teleservice */
#define SMS_3GPP2_TELESERVICE_ID_VMN 0x1003 /* 4099: CDMA Voice Mail Notification */
#define SMS_3GPP2_TELESERVICE_ID_WAP 0x1004 /* 4100: CDMA WAP Message */
#define SMS_3GPP2_TELESERVICE_ID_TCMT 0x7F01 /* 32513: TDMA Cellular Messaging Teleservice */

#define SMS_3GPP2_SERVICE_CAT_UNKNOWN 0x0000 /* Unknown or unspecified */
#define SMS_3GPP2_SERVICE_CAT_EMERGENCY_BROADCAST 0x0001 /* Emergency Broadcasts */
#define SMS_3GPP2_SERVICE_CAT_ADMINISTRATIVE 0x0002 /* Administrative */
#define SMS_3GPP2_SERVICE_CAT_MAINTENANCE 0x0003 /* Maintenance */
#define SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_LOCAL 0x0004 /* General News - Local */
#define SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_REGIONAL 0x0005 /* General News - Regional */
#define SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_NATIONAL 0x0006 /* General News - National */
#define SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_INTERNATIONAL 0x0007 /* General News - International */
#define SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_LOCAL 0x0008 /* Business/Financial News - Local */
#define SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_REGIONAL 0x0009 /* Business/Financial News - Regional */
#define SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_NATIONAL 0x000A /* Business/Financial News - National */
#define SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_INTERNATIONAL 0x000B /* Business/Financial News - International */
#define SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_LOCAL 0x000C /* Sports News - Local */
#define SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_REGIONAL 0x000D /* Sports News - Regional */
#define SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_NATIONAL 0x000E /* Sports News - National */
#define SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_INTERNATIONAL 0x000F /* Sports News - International */
#define SMS_3GPP2_SERVICE_CAT_ENT_NEWS_LOCAL 0x0010 /* Entertainment News - Local */
#define SMS_3GPP2_SERVICE_CAT_ENT_NEWS_REGIONAL 0x0011 /* Entertainment News - Regional */
#define SMS_3GPP2_SERVICE_CAT_ENT_NEWS_NATIONAL 0x0012 /* Entertainment News - National */
#define SMS_3GPP2_SERVICE_CAT_ENT_NEWS_INTERNATIONAL 0x0013 /* Entertainment News - International */
#define SMS_3GPP2_SERVICE_CAT_LOCAL_WEATHER 0x0014 /* Local Weather */
#define SMS_3GPP2_SERVICE_CAT_AREA_TRAFFIC_REPORTS 0x0015 /* Area Traffic Reports */
#define SMS_3GPP2_SERVICE_CAT_FLIGHT_SCHEDULES 0x0016 /* Local Airport Flight Schedules */
#define SMS_3GPP2_SERVICE_CAT_RESTAURANTS 0x0017 /* Restaurants */
#define SMS_3GPP2_SERVICE_CAT_LODGINGS 0x0018 /* Lodgings */
#define SMS_3GPP2_SERVICE_CAT_RETAIL_DIRECTORY 0x0019 /* Retail Directory */
#define SMS_3GPP2_SERVICE_CAT_ADVERTISEMENTS 0x001A /* Advertisements */
#define SMS_3GPP2_SERVICE_CAT_STOCK_QUOTES 0x001B /* Stock Quotes */
#define SMS_3GPP2_SERVICE_CAT_EMP_OPPORTUNITIES 0x001C /* Employment Opportunities */
#define SMS_3GPP2_SERVICE_CAT_HEALTH 0x001D /* Medical/Health/Hospitals */
#define SMS_3GPP2_SERVICE_CAT_TECH_NEWS 0x001E /* Technology News */
#define SMS_3GPP2_SERVICE_CAT_MULTI_CATEGORY 0x001F /* Multi-category */
#define SMS_3GPP2_SERVICE_CAT_CATPT 0x0020 /* Card Application Toolkit Protocol Teleservice (CATPT) */

#define SMS_3GPP2_ERR_CLASS_NO_ERR 0x00 /* No Error: 00 bits */
#define SMS_3GPP2_ERR_CLASS_TEMPORARY 0x10 /* Temporary Error Condition: 10 bits */
#define SMS_3GPP2_ERR_CLASS_PERMANENT 0x11 /* Permanent Error Condition: 11 bits */

#define SMS_3GPP2_DIGIT_MODE_DTMF 0x00 /* 4-bit DTMF code: 0 bits */
#define SMS_3GPP2_DIGIT_MODE_ASCII 0x01 /* 8-bit DTMF code: 1 bits */

#define SMS_3GPP2_NUMBER_MODE_ANSI 0x00 /* ANSI T1.607-1990: 0 bits */
#define SMS_3GPP2_NUMBER_MODE_DNA 0x01 /* Data Network Address: 1 bits */

#define SMS_3GPP2_BSUB_ID_MESSAGE_IDENTIFIER 			0x00 /* Message Identifier */
#define SMS_3GPP2_BSUB_ID_USER_DATA 						0x01 /* User Data */
#define SMS_3GPP2_BSUB_ID_USER_RESPONSE_CODE			0x02 /* User Response Code */
#define SMS_3GPP2_BSUB_ID_MESSAGE_CENTER_TIMESTAMP	0x03 /* Message Center Time Stamp */
#define SMS_3GPP2_BSUB_ID_VP_ABSOLUTE					0x04 /* Validity Period - Absolute */
#define SMS_3GPP2_BSUB_ID_VP_RELATIVE					0x05 /* Validity Period - Relative */
#define SMS_3GPP2_BSUB_ID_DDT_ABSOLUTE					0x06 /* Deferred Delivery Time - Absolute */
#define SMS_3GPP2_BSUB_ID_DDT_RELATIVE					0x07 /* Deferred Delivery Time - Relative */
#define SMS_3GPP2_BSUB_ID_PRIORITY_INDICATOR			0x08 /* Priority Indicator */
#define SMS_3GPP2_BSUB_ID_PRIVACY_INDICATOR				0x09 /* Privacy Indicator */
#define SMS_3GPP2_BSUB_ID_REPLY_OPTION					0x0A /* Reply Option */
#define SMS_3GPP2_BSUB_ID_MESSAGES_NUMBER				0x0B /* Number of Messages */
#define SMS_3GPP2_BSUB_ID_MESSAGE_DELIVERY_ALERT		0x0C /* Alert on Message Delivery */
#define SMS_3GPP2_BSUB_ID_LANGUAGE_INDICATOR			0x0D /* Language Indicator */
#define SMS_3GPP2_BSUB_ID_CALLBACK_NUMBER				0x0E /* Call-Back Number */
#define SMS_3GPP2_BSUB_ID_MESSAGE_DISPLAY_MODE			0x0F /* Message Display Mode */
#define SMS_3GPP2_BSUB_ID_MULTI_ENCODING_UD			0x10 /* Multiple Encoding User Data */
#define SMS_3GPP2_BSUB_ID_DEPOSIT_INDEX					0x11 /* Message Deposit Index */
#define SMS_3GPP2_BSUB_ID_SERVICE_PROGRAM_DATA		0x12 /* Service Category Program Data */
#define SMS_3GPP2_BSUB_ID_SERVICE_PROGRAM_RESULTS		0x13 /* Service Category Program Results */
#define SMS_3GPP2_BSUB_ID_MESSAGE_STATUS				0x14 /* Message Status */

#define SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_DELIVER		0x01 /* Deliver (MT) */
#define SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_SUBMIT		0x02 /* Submit (MO) */
#define SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_CANCEL		0x03 /* Cancellation (MO) */
#define SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_DACK		0x04 /* Delivery Acknowledgment (MT) */
#define SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_UACK		0x05 /* User Acknowledgment (MO/MT) */
#define SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_RACK		0x06 /* Read Acknowledgment (MO/MT) */

#define SMS_3GPP2_BSUB_UD_OCTET_ENCODING		0x00 /* Octet, unspecified */
#define SMS_3GPP2_BSUB_UD_EPM_ENCODING			0x01 /* Extended Protocol Message */
#define SMS_3GPP2_BSUB_UD_ASCII7BIT_ENCODING	0x02 /* 7-bit ASCII */
#define SMS_3GPP2_BSUB_UD_IA5_ENCODING			0x03 /* IA5 */
#define SMS_3GPP2_BSUB_UD_UNICODE_ENCODING		0x04 /* UNICODE */
#define SMS_3GPP2_BSUB_UD_SHIFTJIS_ENCODING		0x05 /* Shift-JIS */
#define SMS_3GPP2_BSUB_UD_KOREAN_ENCODING		0x06 /* Korean */
#define SMS_3GPP2_BSUB_UD_HEBREW_ENCODING		0x07 /* Latin/Hebrew */
#define SMS_3GPP2_BSUB_UD_LATIN_ENCODING		0x08 /* Latin */
#define SMS_3GPP2_BSUB_UD_GSM7BIT_ENCODING		0x09 /* GSM 7-bit default alphabet */
#define SMS_3GPP2_BSUB_UD_GSMDCS_ENCODING		0x0A /* GSM Data-Coding-Scheme */

void sms_util_decode_3gpp2(unsigned int pdu_len, unsigned char *pdu);
