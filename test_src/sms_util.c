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
#include <stdio.h>
#include <string.h>
#include "sms_util.h"

/* Utilites */

static char *__AcItoa(int n, char* str, int b)
{
	int i = 0;

	do
		str[i++] = "0123456789ABCDEF"[n%b];

	while ((n /= b) > 0);
	str[i] = '\0';
	g_strreverse(str);

	return str;
}

void sms_util_decode_dcs(SmsCodingScheme *coding_scheme, unsigned char dcs)
{
	memset(coding_scheme, 0, sizeof(SmsCodingScheme));

	/* bits 7..4 = 00xx : general data coding indication */
	if ( dcs < 0x40 ) {
		coding_scheme->coding_group_type = SMS_CODGRP_SM_GENERAL_DCS;

		/* bit 5 = 1 : indicates the text is compressed */
		if ( dcs & 0x20 )
			coding_scheme->compressed = TRUE;

		/* bit 4 = 1 : indicates that bits  1 to 0 have a message class meaning */
		if ( dcs & 0x10 ) {
			coding_scheme->bmsg_class_set = TRUE;

			switch ( dcs & 0x03 )  { /* bits 1 to 0 : message class */
			case 0x00:
				coding_scheme->class_type = SMS_CLASS_0;
				break;
			case 0x01:
				coding_scheme->class_type = SMS_CLASS_1;
				break;
			case 0x02:
				coding_scheme->class_type = SMS_CLASS_2;
				break;
			case 0x03:
				coding_scheme->class_type = SMS_CLASS_3;
			}
		}
		else /* bit 4 = 0 : indicates that bits 1 to 0 are reserved and have no message class meaning */
			coding_scheme->class_type = SMS_CLASS_NONE;

		switch ( dcs & 0x0C ) { /* bits 4 to 3 : character set */
		case 0x00:
			coding_scheme->alphabet_type = SMS_ALPHABET_DEFAULT;
			break;
		case 0x04:
			coding_scheme->alphabet_type = SMS_ALPHABET_8BIT;
			break;
		case 0x08:
			coding_scheme->alphabet_type = SMS_ALPHABET_UCS2;
			break;
		case 0x0C:
			coding_scheme->alphabet_type = SMS_ALPHABET_MAX;
		}
	} else if ( dcs >= 0x40 && dcs < 0x80 ) {
		/* bits 7..4 = 01xx : message marked for automatic deletion group. bits 5..0
		are coded exactly the same as group 00xx */

		coding_scheme->coding_group_type = SMS_CODGRP_SM_AUTO_DELETION;

		/* bit 5 = 1 : indicates the text is compressed */
		if ( dcs & 0x20 )
			coding_scheme->compressed = TRUE;

		/* bit 4 = 1 : indicates that bits  1 to 0 have a message class meaning */
		if ( dcs & 0x10 ) {
			coding_scheme->bmsg_class_set = TRUE;

			switch ( dcs & 0x03 ) { /* bits 1 to 0 : message class */
			case 0x00:
				coding_scheme->class_type = SMS_CLASS_0;
				break;
			case 0x01:
				coding_scheme->class_type = SMS_CLASS_1;
				break;
			case 0x02:
				coding_scheme->class_type = SMS_CLASS_2;
				break;
			case 0x03:
				coding_scheme->class_type = SMS_CLASS_3;
			}
		}
		else /* bit 4 = 0 : indicates that bits 1 to 0 are reserved and have no message class meaning */
			coding_scheme->class_type = SMS_CLASS_NONE;

		switch ( dcs & 0x0C ) { /* bits 4 to 3 : character set */
		case 0x00:
			coding_scheme->alphabet_type = SMS_ALPHABET_DEFAULT;
			break;
		case 0x04:
			coding_scheme->alphabet_type = SMS_ALPHABET_8BIT;
			break;
		case 0x08:
			coding_scheme->alphabet_type = SMS_ALPHABET_UCS2;
			break;
		case 0x0C:
			coding_scheme->alphabet_type = SMS_ALPHABET_MAX;
		}
	}
	/* bits 7..4 = 1000 ~ 1011 : reserved */
	/* bits 7..4 = 1100 : message waiting indication group, discard message */
	else if ( dcs == 0xC0 ) {
		coding_scheme->coding_group_type = SMS_CODGRP_SM_WAITING_DISCARD;
	}
	else if ( dcs < 0xE0 ) {
		coding_scheme->coding_group_type = SMS_CODGRP_SM_WAITING_STORE;

		if ( dcs & 0x08 )
			coding_scheme->bmsg_ind_active = TRUE;

		switch ( dcs & 0x03 ) {
		case 0x00:
			coding_scheme->waiting_type = SMS_WAITING_VOICE_MSG;
			break;
		case 0x01:
			coding_scheme->waiting_type = SMS_WAITING_FAX_MSG;
			break;
		case 0x02:
			coding_scheme->waiting_type = SMS_WAITING_EMAIL_MSG;
			break;
		case 0x03:
			coding_scheme->waiting_type = SMS_WAITING_OTHER_MSG;
		}
	}
	else if ( dcs < 0xF0 ) {
		coding_scheme->coding_group_type = SMS_CODGRP_SM_WAITING_STORE_UCS2;

		if ( dcs & 0x08 )
			coding_scheme->bmsg_ind_active = TRUE;

		switch ( dcs & 0x03 ) {
		case 0x00:
			coding_scheme->waiting_type = SMS_WAITING_VOICE_MSG;
			break;
		case 0x01:
			coding_scheme->waiting_type = SMS_WAITING_FAX_MSG;
			break;
		case 0x02:
			coding_scheme->waiting_type = SMS_WAITING_EMAIL_MSG;
			break;
		case 0x03:
			coding_scheme->waiting_type = SMS_WAITING_OTHER_MSG;
		}
	}
	else {
		coding_scheme->coding_group_type = SMS_CODGRP_SM_CLASS_CODING;

		if ( dcs & 0x04 )
			coding_scheme->alphabet_type = SMS_ALPHABET_8BIT;

		switch ( dcs & 0x03 ) {
			case 0x00:
				coding_scheme->class_type = SMS_CLASS_0;
				break;
			case 0x01:
				coding_scheme->class_type = SMS_CLASS_1;
				break;
			case 0x02:
				coding_scheme->class_type = SMS_CLASS_2;
				break;
			case 0x03:
				coding_scheme->class_type = SMS_CLASS_3;
		}
	}
}

void sms_util_convert_bcd_2_digit( char* digits, char* bcd, int digit_len )
{
	int i, bcd_len;
	char c[2];
	unsigned char higher, lower;

	if ( bcd == NULL || digits == NULL ) {
		msg( " sms_util_convert_bcd_2_digit: bcd == NULL || digits == NULL. return.\n " );
		return;
	}

	if ( digit_len == 0 ) {
		//printf("__SmsConvertBCD2Digit: digit_len == 0. return.\n" );
		digits[0] = 0x00;
		return;
	}

	bcd_len = (digit_len + 1) / 2;

	memset( digits, 0, bcd_len * 2 );

	for ( i = 0; i < bcd_len; i++ ) {
		lower = bcd[i] & 0x0F; // get low nibble

		if ( lower == 0x0A )
			lower = '*';
		else if ( lower == 0x0B )
			lower = '#';
		else if ( lower == 0x0C )
			lower = 'p'; //DTMF Control digits seperator
		else if ( lower == 0x0F )
			lower = 0;
		else {
			__AcItoa( lower, c, 16 );
			lower = g_ascii_toupper(c[0]);
		}

		higher = ( bcd[i] >> 4 ) & 0x0F; // get high nibble

		if ( higher == 0x0A )
			higher = '*'; // =0x2A
		else if ( higher == 0x0B )
			higher = '#'; // =0x23
		else if ( higher == 0x0C )
			higher = 'p'; // =0x70, DTMF Control digits seperator
		else if ( higher == 0x0F ) { // if higher semi-octet is 0x0F, filled bits.
			//higher = 0;
			sprintf(digits + strlen(digits), "%c", lower);
			digits[/*digit_len-1*/bcd_len*2-1] = '\0';

			//printf("__SmsConvertBCD2Digit: digits [%s].\n", digits	);

			return;
		}
		else {
			__AcItoa(higher, c, 16);
			higher = g_ascii_toupper(c[0]);
		}

		//sprintf(digits, "%s%c%c", digits, lower, higher);
		sprintf(digits + strlen(digits), "%c%c", lower, higher);
	}

	digits[digit_len] = '\0';

	//printf("__SmsConvertBCD2Digit: digits [%s].\n", digits	);
}

char *sms_util_unpack_gsm_code(char *sz_data, const char *in, int in_len )
{
	int i;
	int pos = 0;
	int shift = 0;

	for ( i = 0; i < in_len; i++, pos++ ) {
		sz_data[i] = ( in[pos] << shift ) & 0x7F;

		if ( pos != 0 ) {
			/* except the first byte, a character contains some bits
			** from the previous byte.
			*/
			sz_data[i] |= in[pos-1] >> (8-shift);
		}
		shift ++;

		if ( shift == 7 ) {
			shift = 0;

			/* a possible extra complete character is available */
			i++;
			sz_data[i] = in[pos] >> 1;

			if( sz_data[i] == 0 ) {
				/* this is the end of the input, quit */
				break;
			}
		}
	}
	return sz_data;
}

int sms_util_decode_addr_field(char *dialling_num, char* addr_field, int *result_ton, int *result_npi)
{
	int index = 0;
	int ton, npi;
	int dial_num_len = 0;

	ton = ( addr_field[index+1] & 0x70 ) >> 4;
	npi = addr_field[index+1] & 0x0F;

	if ( ton != SMS_TON_ALPHA_NUMERIC ) {
		dial_num_len = addr_field[index++];
	} else {
		dial_num_len = ( ( ( addr_field[index++] + 1 ) / 2 ) * 8 ) / 7;
	}

	if ( dial_num_len > SMS_ADDRESS_LEN_MAX ) {
		dial_num_len = SMS_ADDRESS_LEN_MAX;
	}

	msg(" dial_num_len = %d\n", dial_num_len );

	index++; /* ignore Type of Address field */

	if (ton != SMS_TON_ALPHA_NUMERIC ) {
		sms_util_convert_bcd_2_digit( dialling_num, (char*) &addr_field[index], dial_num_len );
	} else {
		sms_util_unpack_gsm_code( dialling_num, &addr_field[index], dial_num_len );
	}

	msg(  "sms_util_decode_addr_field: dialling_num [%s].\n", (char*) dialling_num  );

	*result_ton=ton;
	*result_npi=npi;

	msg("ton %d npi %d\n", ton, npi);

	return dial_num_len;
}

TmDateTime *sms_util_decode_time_stamp(char *time_stamp, TmDateTime *tm_date_time )
{
	char buf[3];

	if ( time_stamp == NULL )
		return NULL;

	sms_util_convert_bcd_2_digit( buf, (char*) &time_stamp[0], 2 );
	tm_date_time->year = atoi( buf ) + 2000;
	if ( ( tm_date_time->year >= 1900 + MAX_YEAR )/* && ( tm_date_time->year < 2000 + BASE_YEAR )*/ )
		tm_date_time->year -= 100;
	sms_util_convert_bcd_2_digit( buf, (char*) &time_stamp[1], 2 );
	tm_date_time->month = atoi( buf );
	sms_util_convert_bcd_2_digit( buf, (char*) &time_stamp[2], 2 );
	tm_date_time->day = atoi( buf );
	sms_util_convert_bcd_2_digit( buf, (char*) &time_stamp[3], 2 );
	tm_date_time->hour = atoi( buf );
	sms_util_convert_bcd_2_digit( buf, (char*) &time_stamp[4], 2 );
	tm_date_time->minute = atoi( buf );
	sms_util_convert_bcd_2_digit( buf, (char*) &time_stamp[5], 2 );
	tm_date_time->second = atoi( buf );

	if ( ( tm_date_time->year < 1900 + BASE_YEAR ) || ( tm_date_time->year > 1900 + MAX_YEAR ) )
		tm_date_time->year = 1900 + BASE_YEAR;

	return tm_date_time;
}

void sms_util_convert_digit_2_bcd( char *bcd, char *digits, int digit_len )
{
	int i, j, digit;
	unsigned char higher, lower;

	if ( bcd == NULL || digits == NULL )
		return;

	// 0123456789 -> 1032547698
	for ( i = 0, j = 0; i < digit_len; i = i + 2, j++ )
	{
		if ( digits[i] == '*' )
			digit = 0x0A;
		else if ( digits[i] == '#' )
			digit = 0x0B;
		else if (g_ascii_toupper(digits[i]) == 'P' )
			digit = 0x0C;
		else
			digit = (int) ( digits[i] - '0' );

		lower = digit & 0x0F;

		if ( digit_len != i + 1 ) {
			if ( digits[i+1] == '*' )
				digit = 0x0A;
			else if ( digits[i+1] == '#' )
				digit = 0x0B;
			else if (g_ascii_toupper(digits[i+1]) == 'P' )
				digit = 0x0C;
			else
				digit = (int) ( digits[i+1] - '0' );

			higher = digit & 0x0F;
		} else {
			higher = 0xFF;
		}

		bcd[j] = ( higher << 4 ) | lower;
	}
}

int sms_util_pack_gsm_code( unsigned char *out, const char *data, int in_len )
{
	int i;
	int pos;
	int shift = 0;

	for( pos = 0, i = 0; i < in_len; pos++, i++ ) {
		/* pack the low bits */
		out[pos] = data[i] >> shift;

		if ( i + 1 < in_len ) {
			/* pack the high bits using the low bits of the next character */
			out[pos] |= data[i+1] << ( 7 - shift );
			shift++;
			if( shift == 7 ) {
				shift = 0;
				i++;
			}
		}
	}

	/* done */
	return pos;
}

int sms_util_encode_addr_field(unsigned char *addr_field, char* dialling_num,
	int dial_num_len, int ton, int npi)
{
	int index = 0;

	if ( dialling_num == NULL || addr_field == NULL )
		return -1;

	if ( dialling_num[0] == '+' ) {
		dialling_num++;
		dial_num_len--;
		ton = SMS_TON_INTERNATIONAL;
	}

	if ( ton != SMS_TON_ALPHA_NUMERIC ) {
		addr_field[index++] = (unsigned char)dial_num_len;
	} else {
		addr_field[index] = (unsigned char) ( ( ( dial_num_len * 7 + 7 ) / 8 ) * 2 );
		if ( ( ( dial_num_len * 7 ) % 8 ) <= 4 )
			addr_field[index]--;

		msg(" addr len packet: %d out of SMS_TON_ALPAHA\n", addr_field[index]);
		index++;
	}

	SET_TON_NPI(addr_field[index], ton, npi);

	index++;

	if ( ton != SMS_TON_ALPHA_NUMERIC ) {
		sms_util_convert_digit_2_bcd( (char*) &addr_field[index], (char*) dialling_num, dial_num_len );
		index += (dial_num_len + 1) / 2;
	} else {
		index += sms_util_pack_gsm_code( &addr_field[index], dialling_num, (int) dial_num_len );
	}

	return index;
}

void sms_util_encode_dcs( unsigned char *result_dcs, SmsCodingScheme *coding_scheme )
{
	unsigned char dcs = 0x00;

	if( coding_scheme->coding_group_type == SMS_CODGRP_SM_GENERAL_DCS ) { // bit 7..4 is 00xx
		if ( coding_scheme->compressed )
			dcs |= 0x20; // bit 5 is 1
		if ( coding_scheme->bmsg_class_set ) {
			dcs |= 0x10; // bit 4 is 1
			if( coding_scheme->class_type== SMS_CLASS_0 )
				dcs |= 0x00;
			else if ( coding_scheme->class_type == SMS_CLASS_1 )
				dcs |= 0x01;
			else if ( coding_scheme->class_type == SMS_CLASS_2 )
				dcs |= 0x02;
			else if ( coding_scheme->class_type == SMS_CLASS_3 )
				dcs |= 0x03;
		}

		switch ( coding_scheme->alphabet_type ) {
		case SMS_ALPHABET_DEFAULT: // bit 3..2 is 00
			dcs |= 0x00;
			break;
		case SMS_ALPHABET_8BIT: // bit 3..2 is 01
			dcs |= 0x04;
			break;
		case SMS_ALPHABET_UCS2: // bit 3..2 is 10
			dcs |= 0x08;
			break;
		default: // bit 3..2 is 11
			dcs |= 0x0C;
		}
	} else if ( coding_scheme->coding_group_type == SMS_CODGRP_SM_WAITING_DISCARD ) { // bit 7..4 is 1100
		dcs |= 0xC0;
	} else if ( coding_scheme->coding_group_type == SMS_CODGRP_SM_WAITING_STORE ) { // bit 7..4 is 1101
		dcs |= 0xD0;
		if ( coding_scheme->bmsg_ind_active ) // bit 3..2 is 10
			dcs |= 0x08;
		else if( coding_scheme->waiting_type == SMS_WAITING_VOICE_MSG)
			dcs |= 0x00;
		else if( coding_scheme->waiting_type == SMS_WAITING_FAX_MSG)
			dcs |= 0x01;
		else if( coding_scheme->waiting_type == SMS_WAITING_EMAIL_MSG) // bit 1..0 is 10
			dcs |= 0x02;
		else if( coding_scheme->waiting_type == SMS_WAITING_OTHER_MSG) // bit 1..0 is 11
			dcs |= 0x03;
	} else if ( coding_scheme->coding_group_type == SMS_CODGRP_SM_WAITING_STORE_UCS2 ) { // bit 7..4 is 1110
		dcs |= 0xE0;
		if ( coding_scheme->bmsg_ind_active ) // bit 3..2 is 10
			dcs |= 0x08;
		if( coding_scheme->waiting_type == SMS_WAITING_VOICE_MSG ) // bit 1..0 is 00
			dcs |= 0x00;
		else if( coding_scheme->waiting_type == SMS_WAITING_FAX_MSG )
			dcs |= 0x01;
		else if( coding_scheme->waiting_type == SMS_WAITING_EMAIL_MSG )
			dcs |= 0x02;
		else if( coding_scheme->waiting_type == SMS_WAITING_OTHER_MSG )
			dcs |= 0x03;
	} else if ( coding_scheme->coding_group_type == SMS_CODGRP_SM_CLASS_CODING ) { // bit 7..4 is 1111
		dcs |= 0xF0;
		if( coding_scheme->alphabet_type == SMS_ALPHABET_DEFAULT ) // bit 2 is 0
			dcs |= 0x00;
		else if( coding_scheme->alphabet_type == SMS_ALPHABET_8BIT ) // bit 2 is 1
			dcs |= 0x04;
		if( coding_scheme->class_type == SMS_CLASS_0) // bit 1..0 is 00
			dcs |= 0x00;
		else if( coding_scheme->class_type == SMS_CLASS_1) // bit 1..0 is 01
			dcs |= 0x01;
		else if( coding_scheme->class_type == SMS_CLASS_2) // bit 1..0 is 10
			dcs |= 0x02;
		else if( coding_scheme->class_type == SMS_CLASS_3) // bit 1..0 is 11
			dcs |= 0x03;
	}
	memcpy( result_dcs, &dcs, sizeof ( unsigned char ) );
}

unsigned char sms_util_encode_validity( unsigned char *validity, SmsVp *vp )
{
	unsigned char pos = 0;

	switch( vp->vp_type ) {
	case SMS_VP_NOT_USED:
	case SMS_VP_ENHANCED:
		break;
	case SMS_VP_RELATIVE:
		validity[pos] = (unsigned char) vp->vp_value;
		pos ++;
		break;
	case SMS_VP_ABSOLUTE:
		//TO DO
		//SmsUtilEncodeTimeStamp( validity, vp->vpValue );
		pos += 7;
	}
	return pos;
}

