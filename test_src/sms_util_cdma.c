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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "menu.h"
#include "sms_util_cdma.h"

struct sms_3gpp2_parameter {
	unsigned char id;	/* Parameter ID */
	unsigned char len; /* Parameter Length */
	unsigned char val[0]; /* Parameter Value */
} __attribute__((packed));

/* Read len bits from start of bytes */
static unsigned char __read_bits(const unsigned char *bytes, unsigned short start, unsigned short len)
{
	unsigned char first, second;

	/* Read only from the first byte */
	if (start + len <= 8)
		return (bytes[0] >> (8 - start - len)) & ((1 << len) - 1);

	/* Read (8 - start) from the first byte, (len - (8 - start)) from the second */
	first = 8 - start;
	second = len - first;
	return (__read_bits(&bytes[0], start, first) << second) | (__read_bits(&bytes[1], 0, second));
}

static void __log_teleservice_id(const struct sms_3gpp2_parameter *param)
{
	unsigned short id;
	if (param->len != 2)
		return;

	memcpy(&id, &param->val[0], 2);
	id = GUINT16_FROM_BE(id);

	switch (id) {
	case SMS_3GPP2_TELESERVICE_ID_EPES:
		msg("--Teleservice ID: AMPS Extended Protocol Enhanced Services");
		break;
	case SMS_3GPP2_TELESERVICE_ID_CPT:
		msg("--Teleservice ID: CDMA Cellular Paging Teleservice");
		break;
	case SMS_3GPP2_TELESERVICE_ID_CCMT:
		msg("--Teleservice ID: CDMA Cellular Messaging Teleservice");
		break;
	case SMS_3GPP2_TELESERVICE_ID_VMN:
		msg("--Teleservice ID: CDMA Voice Mail Notification");
		break;
	case SMS_3GPP2_TELESERVICE_ID_TCMT:
		msg("--Teleservice ID: TDMA Cellular Messaging Teleservice");
		break;
	default:
		msg("--Teleservice ID: Unknown (%u)", id);
		break;
	} /* End Switch */
}

static void __log_service_category(const struct sms_3gpp2_parameter *param)
{
	unsigned short cat;

	if (param->len != 2)
		return;

	memcpy(&cat, &param->val[0], 2);
	cat = GUINT16_FROM_BE(cat);

	switch (cat) {
	case SMS_3GPP2_SERVICE_CAT_UNKNOWN:
		msg("--Service Category: Unknown or unspecified");
		break;
	case SMS_3GPP2_SERVICE_CAT_EMERGENCY_BROADCAST:
		msg("--Service Category: Emergency Broadcasts");
		break;
	case SMS_3GPP2_SERVICE_CAT_ADMINISTRATIVE:
		msg("--Service Category: Administrative");
		break;
	case SMS_3GPP2_SERVICE_CAT_MAINTENANCE:
		msg("--Service Category: Maintenance");
		break;
	case SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_LOCAL:
		msg("--Service Category: General News - Local");
		break;
	case SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_REGIONAL:
		msg("--Service Category: General News - Regional");
		break;
	case SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_NATIONAL:
		msg("--Service Category: General News - National");
		break;
	case SMS_3GPP2_SERVICE_CAT_GENERAL_NEWS_INTERNATIONAL:
		msg("--Service Category: General News - International");
		break;
	case SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_LOCAL:
		msg("--Service Category: Business/Financial News - Local");
		break;
	case SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_REGIONAL:
		msg("--Service Category: Business/Financial News - Regional");
		break;
	case SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_NATIONAL:
		msg("--Service Category: Business/Financial News - National");
		break;
	case SMS_3GPP2_SERVICE_CAT_BUSINESS_NEWS_INTERNATIONAL:
		msg("--Service Category: Business/Financial News - International");
		break;
	case SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_LOCAL:
		msg("--Service Category: Sports News - Local");
		break;
	case SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_REGIONAL:
		msg("--Service Category: Sports News - Regional");
		break;
	case SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_NATIONAL:
		msg("--Service Category: Sports News - National");
		break;
	case SMS_3GPP2_SERVICE_CAT_SPORTS_NEWS_INTERNATIONAL:
		msg("--Service Category: Sports News - International");
		break;
	case SMS_3GPP2_SERVICE_CAT_ENT_NEWS_LOCAL:
		msg("--Service Category: Entertainment News - Local");
		break;
	case SMS_3GPP2_SERVICE_CAT_ENT_NEWS_REGIONAL:
		msg("--Service Category: Entertainment News - Regional");
		break;
	case SMS_3GPP2_SERVICE_CAT_ENT_NEWS_NATIONAL:
		msg("--Service Category: Entertainment News - National");
		break;
	case SMS_3GPP2_SERVICE_CAT_ENT_NEWS_INTERNATIONAL:
		msg("--Service Category: Entertainment News - International");
		break;
	case SMS_3GPP2_SERVICE_CAT_LOCAL_WEATHER:
		msg("--Service Category: Local Weather");
		break;
	case SMS_3GPP2_SERVICE_CAT_AREA_TRAFFIC_REPORTS:
		msg("--Service Category: Area Traffic Reports");
		break;
	case SMS_3GPP2_SERVICE_CAT_FLIGHT_SCHEDULES:
		msg("--Service Category: Local Airport Flight Schedules");
		break;
	case SMS_3GPP2_SERVICE_CAT_RESTAURANTS:
		msg("--Service Category: Restaurants");
		break;
	case SMS_3GPP2_SERVICE_CAT_LODGINGS:
		msg("--Service Category: Lodgings");
		break;
	case SMS_3GPP2_SERVICE_CAT_RETAIL_DIRECTORY:
		msg("--Service Category: Retail Directory");
		break;
	case SMS_3GPP2_SERVICE_CAT_ADVERTISEMENTS:
		msg("--Service Category: Advertisements");
		break;
	case SMS_3GPP2_SERVICE_CAT_STOCK_QUOTES:
		msg("--Service Category: Stock Quotes");
		break;
	case SMS_3GPP2_SERVICE_CAT_EMP_OPPORTUNITIES:
		msg("--Service Category: Employment Opportunities");
		break;
	case SMS_3GPP2_SERVICE_CAT_HEALTH:
		msg("--Service Category: Medical/Health/Hospitals");
		break;
	case SMS_3GPP2_SERVICE_CAT_TECH_NEWS:
		msg("--Service Category: Technology News");
		break;
	case SMS_3GPP2_SERVICE_CAT_MULTI_CATEGORY:
		msg("--Service Category: Multi-category");
		break;
	case SMS_3GPP2_SERVICE_CAT_CATPT:
		msg("--Service Category: Card Application Toolkit Protocol Teleservice (CATPT)");
		break;
	default:
		msg("--Service Category: Unknown (%u)", cat);
		break;
	} /* End Switch */
}

static void __log_bearer_reply_option(const struct sms_3gpp2_parameter *param)
{
	unsigned char rep_seq;

	if (param->len != 1)
		return;

	/*
	 * 6 bits: Value identifying the SMS message for which acknowledgment is requested
	 * 2 bits: Always set to 00
	*/
	rep_seq = __read_bits(&param->val[0], 0, 6);
	msg("--Bearer Reply Option: (%u)", rep_seq);
}

static void __log_cause_codes(const struct sms_3gpp2_parameter *param)
{
	unsigned char rep_seq;
	unsigned char err_class;

	if (param->len != 1 && param->len != 2)
		return;

	/*
	 * 6 bits: Value identifying the SMS message for which acknowledgment was requested
	 * 2 bits: No Error / Temporary Error / Permanent Error; 01 is reserved
	 * 8 bits: If No Error, field is absent
	*/

	rep_seq = __read_bits(&param->val[0], 0, 6);
	msg("--Reply Sequence: (%u)", rep_seq);

	err_class = __read_bits(&param->val[0], 6, 2);
	msg("--Error Class: (%u)", err_class);

	if (err_class != SMS_3GPP2_ERR_CLASS_NO_ERR) {
		unsigned char cause_code;

		if (param->len != 2)
			return;

		cause_code = param->val[1];
		msg("--Cause Code: (%u)", cause_code);
	}
}

static void __log_address(const struct sms_3gpp2_parameter *param)
{
	unsigned char digit_mode;
	unsigned char number_mode;
	unsigned char num_fields;
	unsigned char number_type;

	unsigned int byte_offset = 0;
	unsigned int bit_offset = 0;

	char *number = NULL;
	unsigned int iter;

#define UPDATE_OFFSETS(num_bits) do { \
	bit_offset += num_bits; \
	if (bit_offset >= 8) { \
		bit_offset -= 8; \
		byte_offset += 1; \
	} \
} while (0)

#define SIZE_CHECK(req_size) do { \
	if (param->len < req_size) { \
		msg("Unable to read Address"); \
		return; \
	} \
} while (0)

	/*
	 * 1 bit: Digit Mode
	 * 1 bits: Number Mode
	 * 3 bits: Number Type - If Digit Mode is DTMF, field is omitted
	 * 4 bits: Number Plan - If Digit Mode is DTMF OR Number Mode is DNA, field is omitted
	 * 8 bits: Num Fields
	 */

	SIZE_CHECK(1);

	digit_mode = __read_bits(&param->val[byte_offset], bit_offset, 1);
	UPDATE_OFFSETS(1);
	switch (digit_mode) {
	case SMS_3GPP2_DIGIT_MODE_DTMF:
		msg("--Digit Mode: 4-bit DTMF code");
	break;

	case SMS_3GPP2_DIGIT_MODE_ASCII:
		msg("--Digit Mode: 8-bit DTMF code");
	break;

	default:
		msg("--Digit Mode: Unknown (%u)", digit_mode);
	return;
	}

	number_mode = __read_bits(&param->val[byte_offset], bit_offset, 1);
	UPDATE_OFFSETS(1);
	switch (number_mode) {
	case SMS_3GPP2_NUMBER_MODE_ANSI:
		msg("--Number Mode: ANSI T1.607-1990");
	break;

	case SMS_3GPP2_NUMBER_MODE_DNA:
		msg("--Number Mode: Data Network Address");
	break;

	default:
		msg("--Number Mode: Unknown (%u)", number_mode);
	return;
	}

	if (digit_mode == SMS_3GPP2_DIGIT_MODE_ASCII) {
		number_type = __read_bits(&param->val[byte_offset], bit_offset, 3);
		UPDATE_OFFSETS(3);
		msg("--Number Type: (%u)", number_type); /* TODO: print string */
	}

	if (digit_mode == SMS_3GPP2_DIGIT_MODE_ASCII
		&& number_mode == SMS_3GPP2_NUMBER_MODE_ANSI) {
		unsigned char number_plan;
		SIZE_CHECK(byte_offset + 1 + ((bit_offset + 4) / 8));

		number_plan = __read_bits(&param->val[byte_offset], bit_offset, 4);
		UPDATE_OFFSETS(4);
		msg("--Number Plan: (%u)", number_plan); /* TODO: print string */
	}

	SIZE_CHECK(byte_offset + 2);
	num_fields = __read_bits(&param->val[byte_offset], bit_offset, 8);
	UPDATE_OFFSETS(8);
	msg("--Num Fields: (%u)", num_fields);

	if (digit_mode == SMS_3GPP2_DIGIT_MODE_DTMF) {
		/*
		 * TODO:
		 * each occurrence of this field shall be set to the code value
		 * shown in Table 2.7.1.3.2.4-4 of [10] corresponding to the address digit.
		*/
	} else if (number_mode == SMS_3GPP2_NUMBER_MODE_ANSI) {
		/*
		 * Each occurrence of this field shall be set to the ASCII representation
		 * corresponding to the address digit or character, as specified in [7],
		 * with the most significant bit set to '0'.
		*/
		SIZE_CHECK(byte_offset + 1 + ((bit_offset + (num_fields * 8)) / 8));
		number = malloc(num_fields + 1);
		if (number == NULL)
			return;

		for (iter = 0; iter < num_fields; iter++) {
			number[iter] = __read_bits(&param->val[byte_offset], bit_offset, 8);
			UPDATE_OFFSETS(8);
		}
		number[iter] = '\0';

		msg("--Number: (%s)", number);
	} else if (number_type == 0x02) {
		/*
		 * TODO:
		 * each occurrence of this field shall be set to an 8-bit ASCII character
		 * of the Internet e-mail address. The CHARi fields may include more than
		 * one e-mail address, with a comma separating each address.
		*/
	} else if (number_type == 0x01) {
		/*
		 * TODO:
		 * each occurrence of this field shall be set to the binary value of an octet
		 * of the data network address, with the most significant octet first.
		*/
	}

	if (number != NULL) free(number);

#undef UPDATE_OFFSETS
#undef SIZE_CHECK
}

static void __log_bearer_data_message_identifier
(const struct sms_3gpp2_parameter *subparam)
{
	unsigned char msg_type;
	unsigned short msg_id;
	unsigned char header_id;

	if (subparam->len != 3)
		return;

	msg_type = __read_bits(&subparam->val[0], 0, 4);
	switch (msg_type) {
	case SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_DELIVER:
		msg("      Deliver (mobile-terminated only)");
	break;

	case SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_SUBMIT:
		msg("      Submit (mobile-originated only)");
	break;

	case SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_CANCEL:
		msg("      Cancellation (mobile-originated only)");
	break;

	case SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_DACK:
		msg("      Delivery Acknowledgment (mobile-terminated only)");
	break;

	case SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_UACK:
		msg("      User Acknowledgment (either direction)");
	break;

	case SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_RACK:
		msg("      Read Acknowledgment (either direction)");
	break;

	default:
		msg("      Unknown msg_type (%u)", msg_type);
	break;
	} /* End Switch */

	msg_id = ((__read_bits(&subparam->val[0], 4, 8) << 8) | (__read_bits(&subparam->val[1], 4, 8)));
	msg_id = GUINT16_FROM_BE(msg_id);
	msg("      message ID (%u)", msg_id);

	header_id = __read_bits(&subparam->val[2], 4, 1);
	msg("      Header ID (%u)", header_id);
}

static void __log_bearer_data_user_data
(const struct sms_3gpp2_parameter *subparam)
{
	unsigned char msg_encoding;
	unsigned char num_fields;

	unsigned int byte_offset = 0;
	unsigned int bit_offset = 0;

#define UPDATE_OFFSETS(num_bits) do { \
	bit_offset += num_bits; \
	if (bit_offset >= 8) { \
		bit_offset -= 8; \
		byte_offset += 1; \
	} \
} while (0)

#define SIZE_CHECK(req_size) do { \
	if (subparam->len < req_size) { \
		msg("Unable to read Address"); \
		return; \
	} \
} while (0)

	SIZE_CHECK(1);
	msg_encoding = __read_bits(&subparam->val[byte_offset], bit_offset, 5);
	UPDATE_OFFSETS(5);
	switch (msg_encoding) {
	case SMS_3GPP2_BSUB_UD_OCTET_ENCODING:
		msg("      Encoding: Octet, unspecified");
	break;

	case SMS_3GPP2_BSUB_UD_EPM_ENCODING:
		msg("      Encoding: Extended Protocol Message");
	break;

	case SMS_3GPP2_BSUB_UD_ASCII7BIT_ENCODING:
		msg("      Encoding: 7-bit ASCII");
	break;

	case SMS_3GPP2_BSUB_UD_IA5_ENCODING:
		msg("      Encoding: IA5");
	break;

	case SMS_3GPP2_BSUB_UD_UNICODE_ENCODING:
		msg("      Encoding: UNICODE");
	break;

	case SMS_3GPP2_BSUB_UD_SHIFTJIS_ENCODING:
		msg("      Encoding: Shift-JIS");
	break;

	case SMS_3GPP2_BSUB_UD_KOREAN_ENCODING:
		msg("      Encoding: Korean");
	break;

	case SMS_3GPP2_BSUB_UD_HEBREW_ENCODING:
		msg("      Encoding: Latin/Hebrew");
	break;

	case SMS_3GPP2_BSUB_UD_LATIN_ENCODING:
		msg("      Encoding: Latin");
	break;

	case SMS_3GPP2_BSUB_UD_GSM7BIT_ENCODING:
		msg("      Encoding: GSM 7-bit default alphabet");
	break;

	case SMS_3GPP2_BSUB_UD_GSMDCS_ENCODING:
		msg("      Encoding: GSM Data-Coding-Scheme");
	break;

	default:
		msg("      Unknown msg_encoding (%u)", msg_encoding);
	break;
	} /* End Switch */

	if (msg_encoding == SMS_3GPP2_BSUB_UD_EPM_ENCODING) {
		unsigned char msg_type;
		SIZE_CHECK(2);
		msg_type = __read_bits(&subparam->val[byte_offset], bit_offset, 8);
		UPDATE_OFFSETS(8);
		msg("      Message Type (%u)", msg_type);
	}

	SIZE_CHECK(byte_offset + 1 + ((bit_offset + 8) / 8));
	num_fields = __read_bits(&subparam->val[byte_offset], bit_offset, 8);
	UPDATE_OFFSETS(8);
	msg("      Num Fields (%u)", num_fields);

	if (msg_encoding == SMS_3GPP2_BSUB_UD_OCTET_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_EPM_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_ASCII7BIT_ENCODING) {
		char *message;
		unsigned int iter;

		SIZE_CHECK(byte_offset + 1 + ((bit_offset + (num_fields * 7)) / 8));

		message = malloc(num_fields + 1);
		if (message != NULL) {
			for (iter = 0; iter < num_fields; iter++) {
				message[iter] = __read_bits(&subparam->val[byte_offset], bit_offset, 7);
				UPDATE_OFFSETS(7);
			}
			message[iter] = '\0';
			msg("      Message (%s)", message);
			free(message);
		}
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_IA5_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_UNICODE_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_SHIFTJIS_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_KOREAN_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_HEBREW_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_LATIN_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_GSM7BIT_ENCODING) {
		/* TODO */
	} else if (msg_encoding == SMS_3GPP2_BSUB_UD_GSMDCS_ENCODING) {
		/* TODO */
	}

#undef UPDATE_OFFSETS
#undef SIZE_CHECK
}

static void __log_bearer_data(const struct sms_3gpp2_parameter *param)
{
	unsigned int offset = 0;

#define SIZE_CHECK(req_size) do { \
	if (param->len < req_size) { \
		msg("Unable to read Address"); \
		return; \
	} \
} while (0)

	while (offset < param->len) {
		const struct sms_3gpp2_parameter *subparam;

		SIZE_CHECK(offset + 2);
		subparam = (const struct sms_3gpp2_parameter *)(&param->val[offset]);
		offset += 2;

		SIZE_CHECK(offset + subparam->len);
		offset += subparam->len;

		switch (subparam->id) {
		case SMS_3GPP2_BSUB_ID_MESSAGE_IDENTIFIER:
			msg("--Bearer Data: Message Identifier");
			__log_bearer_data_message_identifier(subparam);
		break;

		case SMS_3GPP2_BSUB_ID_USER_DATA:
			msg("--Bearer Data: User Data");
			__log_bearer_data_user_data(subparam);
		break;

		case SMS_3GPP2_BSUB_ID_USER_RESPONSE_CODE:
			msg("--(TODO)Bearer Data: User Response Code");
		break;

		case SMS_3GPP2_BSUB_ID_MESSAGE_CENTER_TIMESTAMP:
			msg("--(TODO)Bearer Data: Message Center Time Stamp");
		break;

		case SMS_3GPP2_BSUB_ID_VP_ABSOLUTE:
			msg("--(TODO)Bearer Data: Validity Period - Absolute");
		break;

		case SMS_3GPP2_BSUB_ID_VP_RELATIVE:
			msg("--(TODO)Bearer Data: Validity Period - Relative");
		break;

		case SMS_3GPP2_BSUB_ID_DDT_ABSOLUTE:
			msg("--(TODO)Bearer Data: Deferred Delivery Time - Absolute");
		break;

		case SMS_3GPP2_BSUB_ID_DDT_RELATIVE:
			msg("--(TODO)Bearer Data: Deferred Delivery Time - Relative");
		break;

		case SMS_3GPP2_BSUB_ID_PRIORITY_INDICATOR:
			msg("--(TODO)Bearer Data: Priority Indicator");
		break;

		case SMS_3GPP2_BSUB_ID_PRIVACY_INDICATOR:
			msg("--(TODO)Bearer Data: Privacy Indicator");
		break;

		case SMS_3GPP2_BSUB_ID_REPLY_OPTION:
			msg("--(TODO)Bearer Data: Reply Option");
		break;

		case SMS_3GPP2_BSUB_ID_MESSAGES_NUMBER:
			msg("--(TODO)Bearer Data: Number of Messages");
		break;

		case SMS_3GPP2_BSUB_ID_MESSAGE_DELIVERY_ALERT:
			msg("--(TODO)Bearer Data: Alert on Message Delivery");
		break;

		case SMS_3GPP2_BSUB_ID_LANGUAGE_INDICATOR:
			msg("--(TODO)Bearer Data: Language Indicator");
		break;

		case SMS_3GPP2_BSUB_ID_CALLBACK_NUMBER:
			msg("--(TODO)Bearer Data: Call-Back Number");
		break;

		case SMS_3GPP2_BSUB_ID_MESSAGE_DISPLAY_MODE:
			msg("--(TODO)Bearer Data: Message Display Mode");
		break;

		case SMS_3GPP2_BSUB_ID_MULTI_ENCODING_UD:
			msg("--(TODO)Bearer Data: Multiple Encoding User Data");
		break;

		case SMS_3GPP2_BSUB_ID_DEPOSIT_INDEX:
			msg("--(TODO)Bearer Data: Message Deposit Index");
		break;

		case SMS_3GPP2_BSUB_ID_SERVICE_PROGRAM_DATA:
			msg("--(TODO)Bearer Data: Service Category Program Data");
		break;

		case SMS_3GPP2_BSUB_ID_SERVICE_PROGRAM_RESULTS:
			msg("--(TODO)Bearer Data: Service Category Program Results");
		break;

		case SMS_3GPP2_BSUB_ID_MESSAGE_STATUS:
			msg("--(TODO)Bearer Data: Message Status");
		break;

		default:
			msg("--Bearer Data: Unknown (%u)", subparam->id);
		break;
		} /* End Switch */
	}

#undef SIZE_CHECK
}

void sms_util_decode_3gpp2(unsigned int pdu_len, unsigned char *pdu)
{
	unsigned int offset = 0;
	unsigned int msg_type;

#define SIZE_CHECK(req_size, err_log) do { \
	if (pdu_len < req_size) { \
		msg("%s", #err_log); \
		return; \
	} \
} while (0)

	/* Message Type */
	SIZE_CHECK(offset + 1, "Unbale to read Message Type");
	msg_type = pdu[offset++];
	switch (msg_type) {
	case SMS_3GPP2_POINT_TO_POINT:
		msg("--new 3GPP2 Point to Point message");
	break;

	case SMS_3GPP2_ACKNOWLEDGE:
		msg("--new 3GPP2 ACK message");
	break;

	case SMS_3GPP2_BROADCAST:
		msg("--new 3GPP2 Broadcast message");
	break;

	default:
		msg("Unknown Message Type");
	return;
	}

	/* Rest all are sms_3gpp2_parameter */
	while (offset < pdu_len) {
		const struct sms_3gpp2_parameter *param;

		SIZE_CHECK(offset + 2, "Unable to read Parameter ID/LEN");
		param = (const struct sms_3gpp2_parameter *)(&pdu[offset]);
		offset += 2;

		SIZE_CHECK(offset + param->len, "Unable to read Parameter Value");
		offset += param->len;

		switch (param->id) {
		case SMS_3GPP2_PARAM_ID_TELESERVICE_ID:
			__log_teleservice_id(param);
		break;

		case SMS_3GPP2_PARAM_ID_SERVICE_CATEGORY:
			__log_service_category(param);
		break;

		case SMS_3GPP2_PARAM_ID_ORIGINATING_ADDRESS:
			__log_address(param);
		break;

		case SMS_3GPP2_PARAM_ID_ORIGINATING_SUBADDRESS:
			msg("TODO: Originating Sub-Address");
		break;

		case SMS_3GPP2_PARAM_ID_DESTINATION_ADDRESS:
			__log_address(param);
		break;

		case SMS_3GPP2_PARAM_ID_DESTINATION_SUBADDRESS:
			msg("TODO: Originating Sub-Address");
		break;

		case SMS_3GPP2_PARAM_ID_BEARER_REPLY_OPTION:
			__log_bearer_reply_option(param);
		break;

		case SMS_3GPP2_PARAM_ID_CAUSE_CODES:
			__log_cause_codes(param);
		break;

		case SMS_3GPP2_PARAM_ID_BEARER_DATA:
			__log_bearer_data(param);
		break;

		default:
			msg("Unhandled Parameter ID 0x%x", param->id);
		break;
		} /* End Switch */
	}

#undef SIZE_CHECK
}
