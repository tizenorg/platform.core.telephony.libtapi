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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tapi_sms_util.h"


void  reverse(char* x, int len)
{
	int i, j = len - 1;

	for (i = 0; i < j; i++)
	{
		int t = x[i];
		x[i] = x[j];
		x[j--] = t;
	}
}

char*  AcItoa(int n, char* str, int b)
{
	int i = 0;

	do
		str[i++] = "0123456789ABCDEF"[n%b];

	while ((n /= b) > 0);
	reverse(str, i);
	str[i] = '\0';

	return str;
}

int  AcToupper(int ch)
{
	return (('a' <= (ch) && (ch) <= 'z')? ((ch) - ('a'-'A')) : (ch));
}

char* SmsUtilUnpackGSMCode(char* szData, const char* pIn, int in_len )
{
	int i;
	int pos = 0;
	int shift = 0;

	/* If the number of fill bits != 0, then it would cause an additional shift */
	/*
	if ( shift != 0 )
		pos = pos + 1;

	if ( shift ==7 )
		shift = 0;
	*/

	for ( i = 0; i < in_len; i++, pos++ )
	{
		szData[i] = ( pIn[pos] << shift ) & 0x7F;

		if ( pos != 0 )
		{
			/* except the first byte, a character contains some bits
			** from the previous byte.
			*/
			szData[i] |= pIn[pos-1] >> (8-shift);
		}

		shift ++;

		if ( shift == 7 )
		{
			shift = 0;

			/* a possible extra complete character is available */
			i++;
			szData[i] = pIn[pos] >> 1;

			if( szData[i] == 0 )
			{
				/* this is the end of the input, quit */
				break;
			}
		}
	}

	return szData;
}

int SmsUtilPackGSMCode( TS_BYTE* pOut, const char* szData, int in_len )
{
	int i;
	int pos;
	int shift = 0;
	//shift = fill_bits;

	//  memset( out, 0, out_len_max );

	/* pack the ASCII characters
	*/
	/*
	if ( shift == 7 )
		shift = 0;
	*/

	for( pos = 0, i = 0; /*pos < out_len_max &&*/ i < in_len; pos++, i++ )
	{
		/* pack the low bits */
		pOut[pos] = szData[i] >> shift;

		if ( i + 1 < in_len )
		{
			/* pack the high bits using the low bits of the next character */
			pOut[pos] |= szData[i+1] << ( 7 - shift );

			shift++;

			if( shift == 7 )
			{
				shift = 0;
				i++;
			}
		}
	}

	/* done */
	return pos;
}


void SmsUtilConvertBCD2Digit( char* pDigits, char* pBCD, int digitLen )
{
	int		i, bcdLen;
	char	c[2];
	unsigned char	higher, lower;

	if ( pBCD == NULL || pDigits == NULL )
	{
		printf("__SmsConvertBCD2Digit: pBCD == NULL || pDigits == NULL. return.\n"  );
		return;
	}

	if ( digitLen == 0 )
	{
		//printf("__SmsConvertBCD2Digit: digitLen == 0. return.\n" );

		pDigits[0] = 0x00;

		return;
	}

	if ( digitLen % 2 )
		bcdLen = digitLen / 2 + 1;
	else
		bcdLen = digitLen / 2;

	memset( pDigits, 0, bcdLen * 2 );

	for ( i = 0; i < bcdLen; i++ )
	{
		lower = pBCD[i] & 0x0F;					// get low nibble

		if ( lower == 0x0A )
			lower = '*';
		else if ( lower == 0x0B )
			lower = '#';
		else if ( lower == 0x0C )
			lower = 'p';			//DTMF Control pDigits seperator
		else if ( lower == 0x0F )
			lower = 0;
		else
		{
			AcItoa( lower, c, 16 );
			lower = (char) AcToupper(c[0]);
		}

		higher = ( pBCD[i] >> 4 ) & 0x0F;			// get high nibble

		if ( higher == 0x0A )
			higher = '*';		// =0x2A
		else if ( higher == 0x0B )
			higher = '#';		// =0x23
		else if ( higher == 0x0C )
			higher = 'p';		// =0x70, DTMF Control pDigits seperator
		else if ( higher == 0x0F ) // if higher semi-octet is 0x0F, filled bits.
		{
			//higher = 0;
			sprintf(pDigits + strlen(pDigits), "%c", lower);
			pDigits[/*digitLen-1*/bcdLen*2-1] = '\0';

			//printf("__SmsConvertBCD2Digit: pDigits [%s].\n", pDigits  );

			return;
		}
		else
		{
			AcItoa(higher, c, 16);
			higher = (char) AcToupper(c[0]);
		}

		//sprintf(pDigits, "%s%c%c", pDigits, lower, higher);
		sprintf(pDigits + strlen(pDigits), "%c%c", lower, higher);
	}

	pDigits[digitLen] = '\0';

	//printf("__SmsConvertBCD2Digit: pDigits [%s].\n", pDigits  );
}


void SmsUtilConvertDigit2BCD( char* pBCD, char* pDigits, int digitLen )
{
	int	i, j, digit;

	unsigned char	higher, lower;

	if ( pBCD == NULL || pDigits == NULL )
		return;

	// 0123456789 -> 1032547698
	for ( i = 0, j = 0; i < digitLen; i = i + 2, j++ )
	{
		if ( pDigits[i] == '*' )
			digit = 0x0A;
		else if ( pDigits[i] == '#' )
			digit = 0x0B;
		else if ( AcToupper( pDigits[i] ) == 'P' )
			digit = 0x0C;
		else
			digit = (int) ( pDigits[i] - '0' );

		lower = digit & 0x0F;

		if ( digitLen != i + 1 )
		{
			if ( pDigits[i+1] == '*' )
				digit = 0x0A;
			else if ( pDigits[i+1] == '#' )
				digit = 0x0B;
			else if ( AcToupper( pDigits[i+1] ) == 'P' )
				digit = 0x0C;
			else
				digit = (int) ( pDigits[i+1] - '0' );

			higher = digit & 0x0F;
		}
		else
		{
			higher = 0xFF;
		}

		pBCD[j] = ( higher << 4 ) | lower;
	}
}


TmDateTime* SmsUtilDecodeTimeStamp(char * pTimeStamp, TmDateTime *tmDateTime )
{
	//TmDateTime tmDateTime;
	char szBuf[3];
	//TS_UINT32	time;

	if ( pTimeStamp == NULL )
		return NULL;

	SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[0], 2 );
	tmDateTime->year = atoi( szBuf ) + 2000;
	if ( ( tmDateTime->year >= 1900 + MAX_YEAR )/* && ( tmDateTime->year < 2000 + BASE_YEAR )*/ )
		tmDateTime->year -= 100;
	SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[1], 2 );
	tmDateTime->month = atoi( szBuf );
	SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[2], 2 );
	tmDateTime->day = atoi( szBuf );
	SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[3], 2 );
	tmDateTime->hour = atoi( szBuf );
	SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[4], 2 );
	tmDateTime->minute = atoi( szBuf );
	SmsUtilConvertBCD2Digit( szBuf, (char*) &pTimeStamp[5], 2 );
	tmDateTime->second = atoi( szBuf );

	if ( ( tmDateTime->year < 1900 + BASE_YEAR ) || ( tmDateTime->year > 1900 + MAX_YEAR ) )
		tmDateTime->year = 1900 + BASE_YEAR;

	/*
	time = TmDateTimeToSeconds( &tmDateTime );
	if ( time > MAX_SECONDS )
		time = MAX_SECONDS;

	*/

	return tmDateTime;
}

unsigned char* SmsUtilEncodeTimeStamp( TmDateTime* tmDateTime, unsigned char* pTimeStamp )
{
	//TmDateTime tmDateTime;
	char szBuf[3];
	int	year;

	if ( pTimeStamp == NULL )
		return NULL;

	memset( (void*) pTimeStamp, 0x00, sizeof ( unsigned char ) * 7 );

	//TmSecondsToDateTime( timeStamp, &tmDateTime );

	year = tmDateTime->year - 2000;
	if ( year < 0 )
		year += 100;
	sprintf( szBuf, "%02d", year );
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[0], szBuf, 2 );
	sprintf( szBuf, "%02d", tmDateTime->month );
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[1], szBuf, 2 );
	sprintf( szBuf, "%02d", tmDateTime->day );
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[2], szBuf, 2 );
	sprintf( szBuf, "%02d", tmDateTime->hour );
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[3], szBuf, 2 );
	sprintf( szBuf, "%02d", tmDateTime->minute );
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[4], szBuf, 2 );
	sprintf( szBuf, "%02d", tmDateTime->second );
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[5], szBuf, 2 );


	/*	ignore Time zone (assume it is using 0x00 as default)
	timeZone = TmGetTimeZoneOffset() /15;
	if ( timeZone < 0 )
		absTimeZone = -timeZone;
	else
		absTimeZone = timeZone;
	*/

	sprintf( szBuf, "%02d", 0);
	SmsUtilConvertDigit2BCD( (char*) &pTimeStamp[6], szBuf, 2 );

	//if ( timeZone < 0 )
	//	pTimeStamp[6] |= 0x80;

	return pTimeStamp;
}



int  SmsUtilDecodeAddrField(char *diallingNum, char* pAddrField, int *result_ton, int *result_npi )
{
	int index = 0;
	int  ton,npi;
	int DialNumLen=0;

	ton = ( pAddrField[index+1] & 0x70 ) >> 4;
	npi = pAddrField[index+1] & 0x0F;

	if ( ton != SIM_TON_ALPHA_NUMERIC )
	{
		// Origination/Destination address �ʵ忡���� length�� ���� address length
		// origination/destination address �ʵ��� ��� length�� 0 �� ���� number type/plan �ʵ�� 0xFF ���� ���´�.
		DialNumLen = pAddrField[index++];
	}
	else
	{
		DialNumLen = ( ( ( pAddrField[index++] + 1 ) / 2 ) * 8 ) / 7;
	}




	// SIM_SMSP_ADDRESS_LEN ���� address length �� ũ�� SIM_SMSP_ADDRESS_LEN ��ŭ�� ��ȯ�� �Ѵ�.

	if ( DialNumLen > SIM_SMSP_ADDRESS_LEN )
	{
		DialNumLen = SIM_SMSP_ADDRESS_LEN;
	}

	printf(" DialNumLen = %d\n", DialNumLen  );

	index++; /* ignore Type of Address field */

	if (ton != SIM_TON_ALPHA_NUMERIC )
	{
		SmsUtilConvertBCD2Digit( diallingNum, (char*) &pAddrField[index],DialNumLen );
	}
	else
	{
		SmsUtilUnpackGSMCode( diallingNum, &pAddrField[index],DialNumLen );
	}

	printf(  "__SmsDecodeAddrField: diallingNum [%s].\n", (char*) diallingNum  );

	*result_ton=ton;
	*result_npi=npi;

	printf("ton %d npi %d\n",ton,npi);

	return DialNumLen;


}

int  SmsUtilEncodeAddrField(unsigned char* pAddrField, char* diallingNum, int DialNumLen, int ton, int npi )
{
	int index = 0;

	if ( diallingNum == NULL || pAddrField == NULL )
		return -1;

	if ( ton != SIM_TON_ALPHA_NUMERIC )
	{
		// Origination/Destination address �ʵ忡���� length�� ���� address length
		pAddrField[index++] = (unsigned char)DialNumLen;
		//printf(" addr len packet: %d\n", pAddrField[index]);
	}
	else
	{
		pAddrField[index] = (unsigned char) ( ( ( DialNumLen * 7 + 7 ) / 8 ) * 2 );

		// ������ ����Ʈ���� ���� 4��Ʈ�� ������ ������ length �ʵ尪�� -1�� �Ѵ�.
		if ( ( ( DialNumLen * 7 ) % 8 ) <= 4 )
			pAddrField[index]--;

	        printf(" addr len packet: %d out of SIM_TON_ALPAHA\n", pAddrField[index]);

		index++;



	}

	SET_TON_NPI( pAddrField[index], ton, npi );

	index++; // SET_TON_NPI �� MACRO �̹Ƿ� ���ο��� ������Ű�� ����

	if ( ton != SIM_TON_ALPHA_NUMERIC )
	{
		SmsUtilConvertDigit2BCD( (char*) &pAddrField[index], (char*) diallingNum, DialNumLen );

		if ( DialNumLen % 2 )
			index += DialNumLen / 2 + 1;
		else
			index += DialNumLen / 2;
	}
	else
	{
		index += SmsUtilPackGSMCode( &pAddrField[index], diallingNum, (int) DialNumLen );
	}

	return index;
}
int SmsUtilDecodeScAddrField( TelSmsAddressInfo_t* pSmsAddrField, TS_BYTE* pAddrField )
{
	printf("SmsUtilDecodeScAddrField\n");
	int index = 0;
	int length = 0;

	if ( pSmsAddrField == NULL || pAddrField == NULL )
	{
		printf( "SmsUtilDecodeScAddrField: pSimAddrField or pAddrField is NULL.\n"  );

		return 0;
	}

	// Service Center address �ʵ忡���� length�� �ڿ� ������ byte�� ��
	// -> ���� address ���̴� TON/API ����Ʈ�� �����ϰ� ������ ����Ʈ�� 2�� or 2�� - 1(���� ���̰� Ȧ���ΰ��)
	length = pAddrField[index];
	// ������ ���� �޽����� ��쿡�� service center address�� ���� ���� �ִ�.
	// �� ��쿡 length �� 0 �̸� number type, plan �� ��� ���
	// length �� 1 �̸� type, plan �� �ִ� ���
	if ( length > 1 )
	{
		pSmsAddrField->DialNumLen = ( pAddrField[index++] - 1 ) * 2; // -1�� TON/API �ʵ�

		// SIM_SMSP_ADDRESS_LEN ���� address length �� ũ�� SIM_SMSP_ADDRESS_LEN ��ŭ�� ��ȯ�� �Ѵ�.
		if ( pSmsAddrField->DialNumLen > SIM_SMSP_ADDRESS_LEN )
		{
			pSmsAddrField->DialNumLen = SIM_SMSP_ADDRESS_LEN;
		}

		pSmsAddrField->Ton = ( pAddrField[index] & 0x70 ) >> 4;
		pSmsAddrField->Npi = pAddrField[index] & 0x0F;

		index++; /* ignore Type of Address field */

		SmsUtilConvertBCD2Digit( (char*) pSmsAddrField->DialNumLen, (char*) &pAddrField[index], pSmsAddrField->DialNumLen );

		printf( "SmsUtilDecodeScAddrField: diallingNum [%s].\n", (char*) pSmsAddrField->DialNumLen  );

		printf( "length=%d , ton %d, npi =%d\n",pSmsAddrField->DialNumLen, pSmsAddrField->Ton,pSmsAddrField->Npi );
	}

	return ++length;
}

 int  SmsUtilEncodeScAddrField( TS_BYTE* pAddrField, TelSmsAddressInfo_t * pSmsAddrField )
{
	int index = 0;

	if ( pSmsAddrField == NULL || pAddrField == NULL )
		return -1;

	// Service Center address �ʵ忡���� length�� �ڿ� ������ byte�� ��
	// -> ���� address ���̴� TON/API ����Ʈ�� �����ϰ� ������ ����Ʈ�� 2�� or 2�� - 1(���� ���̰� Ȧ���ΰ��)
	if ( pSmsAddrField->DialNumLen % 2 )
	{
		pAddrField[index++] = pSmsAddrField->DialNumLen / 2 + 1 + 1; // +1 �� TON/NPI �ʵ�, Ȧ������ ���� ������ ���߱� ���� �ѹ� �� +1
	}
	else
	{
		pAddrField[index++] = pSmsAddrField->DialNumLen / 2 + 1; // +1 �� TON/NPI �ʵ�
	}

	SET_TON_NPI( pAddrField[index], pSmsAddrField->Ton, pSmsAddrField->Npi );

	index++; // SET_TON_NPI �� MACRO �̹Ƿ� ���ο��� ������Ű�� ���׹߻�

	SmsUtilConvertDigit2BCD( (char*) &pAddrField[index], (char*) pSmsAddrField->DialNumLen, pSmsAddrField->DialNumLen );

	if ( pSmsAddrField->DialNumLen % 2 )
		index += pSmsAddrField->DialNumLen / 2 + 1;
	else
		index += pSmsAddrField->DialNumLen / 2;

	return index;
}

void SmsUtilDecodeDCS( tapi_nettext_coding_scheme* pCodingScheme,   unsigned char dcs )
{
	assert( pCodingScheme != NULL );

	memset( pCodingScheme, 0, sizeof ( tapi_nettext_coding_scheme ) );

	if ( dcs < 0x40 ) // bits 7..4 = 00xx : general data coding indication
	{
		pCodingScheme->coding_group_type = TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;

		if ( dcs & 0x20 ) // bit 5 = 1 : indicates the text is compressed
			pCodingScheme->bCompressed = TRUE;

		if ( dcs & 0x10 ) // bit 4 = 1 : indicates that bits  1 to 0 have a message class meaning
		{
			pCodingScheme->bmsg_class_set = TRUE;

			switch ( dcs & 0x03 ) // bits 1 to 0 : message class
			{
				case 0x00:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_0;
					break;
				case 0x01:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_1;
					break;
				case 0x02:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_2;
					break;
				case 0x03:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_3;
					break;
			}
		}
		else // bit 4 = 0 : indicates that bits 1 to 0 are reserved and have no message class meaning
			pCodingScheme->class_type = TAPI_NETTEXT_CLASS_NONE;

		switch ( dcs & 0x0C ) // bits 4 to 3 : character set
		{
			case 0x00:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_DEFAULT;
				break;
			case 0x04:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_8BIT;
				break;
			case 0x08:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_UCS2;
				break;
			case 0x0C:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_MAX;
				break;
		}
	}
	else if ( dcs >= 0x40 && dcs < 0x80 ) // bits 7..4 = 01xx : message marked for automatic deletion group. bits 5..0 are coded exactly the same as group 00xx
	{
		pCodingScheme->coding_group_type = TAPI_NETTEXT_CODGRP_SM_AUTO_DELETION;

		if ( dcs & 0x20 ) // bit 5 = 1 : indicates the text is compressed
			pCodingScheme->bCompressed = TRUE;

		if ( dcs & 0x10 ) // bit 4 = 1 : indicates that bits  1 to 0 have a message class meaning
		{
			pCodingScheme->bmsg_class_set = TRUE;

			switch ( dcs & 0x03 ) // bits 1 to 0 : message class
			{
				case 0x00:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_0;
					break;
				case 0x01:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_1;
					break;
				case 0x02:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_2;
					break;
				case 0x03:
					pCodingScheme->class_type = TAPI_NETTEXT_CLASS_3;
					break;
			}
		}
		else // bit 4 = 0 : indicates that bits 1 to 0 are reserved and have no message class meaning
			pCodingScheme->class_type = TAPI_NETTEXT_CLASS_NONE;

		switch ( dcs & 0x0C ) // bits 4 to 3 : character set
		{
			case 0x00:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_DEFAULT;
				break;
			case 0x04:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_8BIT;
				break;
			case 0x08:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_UCS2;
				break;
			case 0x0C:
				pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_MAX;
				break;
		}
	}
	// bits 7..4 = 1000 ~ 1011 : reserved
	else if ( dcs == 0xC0 ) // bits 7..4 = 1100 : message waiting indication group, discard message
	{
		pCodingScheme->coding_group_type = TAPI_NETTEXT_CODGRP_SM_WAITING_DISCARD;
	}
	else if ( dcs < 0xE0 )
	{
		pCodingScheme->coding_group_type = TAPI_NETTEXT_CODGRP_SM_WAITING_STORE;

		if ( dcs & 0x08 )
			pCodingScheme->bmsg_ind_active = TRUE;

		switch ( dcs & 0x03 )
		{
			case 0x00:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_VOICE_MSG;
				break;
			case 0x01:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_FAX_MSG;
				break;
			case 0x02:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_EMAIL_MSG;
				break;
			case 0x03:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_OTHER_MSG;
				break;
		}
	}
	else if ( dcs < 0xF0 )
	{
		pCodingScheme->coding_group_type = TAPI_NETTEXT_CODGRP_SM_WAITING_STORE_UCS2;

		if ( dcs & 0x08 )
			pCodingScheme->bmsg_ind_active = TRUE;

		switch ( dcs & 0x03 )
		{
			case 0x00:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_VOICE_MSG;
				break;
			case 0x01:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_FAX_MSG;
				break;
			case 0x02:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_EMAIL_MSG;
				break;
			case 0x03:
				pCodingScheme->waiting_type = TAPI_NETTEXT_WAITING_OTHER_MSG;
				break;
		}
	}
	else
	{
		pCodingScheme->coding_group_type = TAPI_NETTEXT_CODGRP_SM_CLASS_CODING;

		if ( dcs & 0x04 )
			pCodingScheme->alphabet_type = TAPI_NETTEXT_ALPHABET_8BIT;

		switch ( dcs & 0x03 )
		{
			case 0x00:
				pCodingScheme->class_type = TAPI_NETTEXT_CLASS_0;
				break;
			case 0x01:
				pCodingScheme->class_type = TAPI_NETTEXT_CLASS_1;
				break;
			case 0x02:
				pCodingScheme->class_type = TAPI_NETTEXT_CLASS_2;
				break;
			case 0x03:
				pCodingScheme->class_type = TAPI_NETTEXT_CLASS_3;
				break;
		}
	}
}

void SmsUtilEncodeDCS( TS_BYTE* pDCS, tapi_nettext_coding_scheme* pCodingScheme )
{
	printf("SmsUtilEncodeDCS Start\n");
	TS_BYTE dcs = 0x00;

	assert( pCodingScheme != NULL );

	if( pCodingScheme->coding_group_type == TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS ) // bit 7..4 is 00xx
	{
		if ( pCodingScheme->bCompressed )
				dcs |= 0x20; // bit 5 is 1

			if ( pCodingScheme->bmsg_class_set )
			{
				dcs |= 0x10; // bit 4 is 1

				if( pCodingScheme->class_type== TAPI_NETTEXT_CLASS_0 )
					dcs |= 0x00;

				else if ( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_1 )
					dcs |= 0x01;

				else if ( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_2 )
					dcs |= 0x02;

				else if ( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_3 )
					dcs |= 0x03;
			}

			switch ( pCodingScheme->alphabet_type )
			{
				case TAPI_NETTEXT_ALPHABET_DEFAULT: // bit 3..2 is 00
				{
					dcs |= 0x00;
					break;
				}
				case TAPI_NETTEXT_ALPHABET_8BIT: // bit 3..2 is 01
				{
					dcs |= 0x04;
					break;
				}
				case TAPI_NETTEXT_ALPHABET_UCS2: // bit 3..2 is 10
				{
					dcs |= 0x08;
					break;
				}
				default: // bit 3..2 is 11
				{
					dcs |= 0x0C;
					break;
				}
			}
	}
	else if ( pCodingScheme->coding_group_type == TAPI_NETTEXT_CODGRP_SM_WAITING_DISCARD ) // bit 7..4 is 1100
	{
		dcs |= 0xC0;
	}
	else if ( pCodingScheme->coding_group_type == TAPI_NETTEXT_CODGRP_SM_WAITING_STORE ) // bit 7..4 is 1101
	{
		dcs |= 0xD0;

		if ( pCodingScheme->bmsg_ind_active ) // bit 3..2 is 10
			dcs |= 0x08;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_VOICE_MSG)
			dcs |= 0x00;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_FAX_MSG)
			dcs |= 0x01;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_EMAIL_MSG)	// bit 1..0 is 10
			dcs |= 0x02;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_OTHER_MSG)	// bit 1..0 is 11
			dcs |= 0x03;

	}
	else if ( pCodingScheme->coding_group_type == TAPI_NETTEXT_CODGRP_SM_WAITING_STORE_UCS2 ) // bit 7..4 is 1110
	{
		dcs |= 0xE0;

		if ( pCodingScheme->bmsg_ind_active ) // bit 3..2 is 10
			dcs |= 0x08;

		if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_VOICE_MSG ) // bit 1..0 is 00
			dcs |= 0x00;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_FAX_MSG )
			dcs |= 0x01;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_EMAIL_MSG )
			dcs |= 0x02;

		else if( pCodingScheme->waiting_type == TAPI_NETTEXT_WAITING_OTHER_MSG )
			dcs |= 0x03;
	}
	else if ( pCodingScheme->coding_group_type == TAPI_NETTEXT_CODGRP_SM_CLASS_CODING )	// bit 7..4 is 1111
	{
		dcs |= 0xF0;

		if( pCodingScheme->alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )	// bit 2 is 0
			dcs |= 0x00;
		else if( pCodingScheme->alphabet_type == TAPI_NETTEXT_ALPHABET_8BIT ) // bit 2 is 1
			dcs |= 0x04;

		if( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_0) // bit 1..0 is 00
			;

		else if( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_1) // bit 1..0 is 01
			dcs |= 0x01;

		else if( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_2) // bit 1..0 is 10
			dcs |= 0x02;

		else if( pCodingScheme->class_type == TAPI_NETTEXT_CLASS_3) // bit 1..0 is 11
			dcs |= 0x03;
	}

	memcpy( pDCS, &dcs, sizeof ( TS_BYTE ) );

	printf("SmsUtilEncodeDCS End\n");
}

 TS_UINT8 SmsUtilEncodeValidity( TS_BYTE* pValidity, tapi_nettext_vp* pVP )
{
	TS_UINT8	pos = 0;

	switch( pVP->vp_type )
	{
		case TAPI_NETTEXT_VP_NOT_USED:
			break;

		case TAPI_NETTEXT_VP_RELATIVE:
			pValidity[pos] = (TS_UINT8) pVP->vpValue;
			pos ++;
			break;

		case TAPI_NETTEXT_VP_ABSOLUTE:
			//TO DO
			//SmsUtilEncodeTimeStamp( pValidity, pVP->vpValue );
			pos += 7;
			break;

		case TAPI_NETTEXT_VP_ENHANCED:
			break;
	}

	return pos;
}

