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
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>

#include <arpa/inet.h>
#include <tapi_common.h>
#include <TelSms.h>
#include <TapiUtility.h>
#include <ITapiNetText.h>

#include "menu.h"
#include "sms.h"
#include "sms_util.h"
#include "sms_util_cdma.h"

/* Global Variables */
int dcs_pos = 0;
unsigned int SmsMsgId = 0;
unsigned char tp_mr = 0x00;

#define SMS_PTP_MSG 0x0

#define DIGIT_MODE_BIT 0x80
#define NUMBER_MODE_BIT 0x40
#define DIGIT_MODE_OFFSET 7
#define NUMBER_MODE_OFFSET 6
#define NIBBLE_BIT_LENGTH 4
#define SUBPARAM_LEN_VAL 0x03

#define u16 uint16_t
#define u8 uint8_t

const char *cmdString[] = {
	"TAPI_SERVICE_SMS_SEND_UMTS_MSG",
	"TAPI_SERVICE_SMS_READ_MSG",
	"TAPI_SERVICE_SMS_SAVE_MSG",
	"TAPI_SERVICE_SMS_DELETE_MSG",
	"TAPI_SERVICE_SMS_GET_COUNT",
	"TAPI_SERVICE_SMS_GET_SCA",
	"TAPI_SERVICE_SMS_SET_SCA",
	"TAPI_SERVICE_SMS_GET_CB_CONFIG",
	"TAPI_SERVICE_SMS_SET_CB_CONFIG",
	"TAPI_SERVICE_SMS_SET_MEM_STATUS",
	"TAPI_SERVICE_SMS_GET_PREF_BEARER",
	"TAPI_SERVICE_SMS_SET_PREF_BEARER",
	"TAPI_SERVICE_SMS_SET_DELIVERY_REPORT",
	"TAPI_SERVICE_SMS_SET_MSG_STATUS",
	"TAPI_SERVICE_SMS_GET_PARAMS",
	"TAPI_SERVICE_SMS_SET_PARAMS",
	"TAPI_SERVICE_SMS_GET_PARAMCNT",
	"TAPI_SERVICE_SMS_SEND_CDMA_MSG"
};

static void on_resp_send_msg(TapiHandle *handle, int result, void *data,
		void *user_data);
static void on_resp_send_cdma_msg(TapiHandle *handle, int result, void *data,
		void *user_data);
static void on_resp_read_msg(TapiHandle *handle, int result, void *data,
		void *user_data);
static void on_resp_save_msg(TapiHandle *handle, int result, void *data,
		void *user_data);
static void on_resp_DeliverReport_msg(TapiHandle *handle, int result,
		void *data, void *user_data);
int  _util_hexstring_to_asciistring(const unsigned char *hex_string, unsigned char *ascii_string, int hex_len);

unsigned int SmsUtilEncodeSca(TelSmsAddressInfo_t *sca, unsigned char *sca_num);

static int CdmaCbSetting(MManager *mm, struct menu_data *menu);

int  _util_hexstring_to_asciistring(const unsigned char *hex_string, unsigned char *ascii_string, int hex_len)
{
	int i, ascii_len;

	if (hex_string[hex_len - 1]>>4 == 0x0f)
		ascii_len = (hex_len * 2) - 1;
	else
		ascii_len = (hex_len * 2);

	for (i = 0; i < ascii_len; i++) {
		if (i % 2 == 0)
			ascii_string[i] = (hex_string[i/2] & 0x0f) + '0';
		else
			ascii_string[i] = ((hex_string[i/2]   >> 4)) + '0';
	}
	ascii_string[ascii_len] = '\0';
	return ascii_len;
}

unsigned int SmsUtilEncodeSca(TelSmsAddressInfo_t *sca, unsigned char *sca_num)
{
	unsigned int sca_length = 0;
	unsigned int additional_len = 0;
	unsigned local_index, j;

	sca->Npi = TAPI_SIM_NPI_ISDN_TEL;
	sca->Ton = TAPI_SIM_TON_UNKNOWN;
	if (sca_num[0] == '+') {
		sca->Ton = TAPI_SIM_TON_INTERNATIONAL;
		additional_len = 1;
	}
	sca_length = strlen((char *)sca_num) - additional_len;
	if (sca_length % 2 == 0) {
		for (local_index = additional_len, j = 0; local_index < sca_length; local_index += 2, j++)
			sca->szDiallingNum[j] = ((sca_num[local_index+1] & 0X0F) << 4) | (sca_num[local_index] & 0x0F);
	} else {
		for (local_index = additional_len, j = 0; local_index < sca_length; local_index += 2, j++) {
			if (local_index == (sca_length - 1))
				sca->szDiallingNum[j] = ((0xF0) | (sca_num[local_index] & 0x0F));
			else
				sca->szDiallingNum[j] = ((sca_num[local_index+1] & 0X0F) << 4) | (sca_num[local_index] & 0x0F);
		}
	}
	sca->szDiallingNum[(sca_length+1)/2] = '\0';
	sca->DialNumLen = (sca_length+1)/2;

	return sca->DialNumLen;
}


static BOOL EncodeCdmaSmsParamTeleservice(int teleservice_id , ParamTeleserv_t *ParamTeleserv)
{
	BOOL ret = FALSE;

	if (ParamTeleserv == NULL) {
		printf("[EncodeCdmaSmsParamTeleservice] NULL pointer  \n");
		return ret;
	}

	switch (teleservice_id) {
	case SMS_3GPP2_TELESERVICE_ID_CPT:
	case SMS_3GPP2_TELESERVICE_ID_CCMT:
	case SMS_3GPP2_TELESERVICE_ID_VMN:
	case SMS_3GPP2_TELESERVICE_ID_WAP:
		ret = TRUE;
	break;

	default:
		ret = FALSE;
	}

	if (ret == TRUE) {
		ParamTeleserv->param_id = SMS_3GPP2_PARAM_ID_TELESERVICE_ID;
		ParamTeleserv->param_len = 2;
		ParamTeleserv->teleserv_id = teleservice_id;
		printf("ParamTeleserv->teleserv_id[%d]\n", ParamTeleserv->teleserv_id);
	}

	return ret;
}
static BOOL EncodeCdmaSmsParamMsgId(int type, ParamMsgId_t *ParamMsg)
{
	BOOL ret = FALSE;

	if (ParamMsg == NULL) {
		printf("[EncodeCdmaSmsParamMsgId] NULL pointer  \n");
		return ret;
	}

	ParamMsg->param_id = SMS_3GPP2_BSUB_ID_MESSAGE_IDENTIFIER;
	ParamMsg->param_len = 3;
	ParamMsg->type = type;
	ParamMsg->msg_id = SmsMsgId++;

	ret = TRUE;

	return ret;
}

static BOOL EncodeCdmaSmsParamUserData(char *SendTxt, ParamUserData_t *ParamUser)
{
	BOOL ret = FALSE;
	int length = 0;

	if (ParamUser == NULL || SendTxt == NULL) {
		printf("[EncodeCdmaSmsParamUserData] NULL pointer  \n");
		return ret;
	}

	length = strlen(SendTxt);

	ParamUser->param_id = SMS_3GPP2_BSUB_ID_USER_DATA;
	ParamUser->encoding = SMS_3GPP2_BSUB_UD_ASCII7BIT_ENCODING;
	strncpy((char *)ParamUser->CHARi, SendTxt, length);

	ParamUser->len = length;
	length += 2;;
	ParamUser->param_len = length;

	ret = TRUE;

	return ret;
}

static BOOL EncodeCdmaSmsSubmitTpdu(MManager *mm,
	char *diallingNum, int dialNumLen, char *msgTxt, int msg_len)
{
	int ret, index = 0;
	unsigned char *encoded_msg, digit_mode_val = 0, number_mode_val = 0;
	unsigned char hex_string[SMS_ADDRESS_LEN_MAX + 1] = { 0, };
	TapiHandle *handle;
	TelSmsDatapackageInfo_t data_package;
	u16 data;

	ParamTeleserv_t param_teleservice = {0, };
	ParamMsgId_t param_msg = {0, };
	ParamUserData_t param_user = {0, };

	msg("EncodeCdmaSmsSubmitTpdu:Enter");

	handle = menu_manager_ref_user_data(mm);
	encoded_msg = g_try_malloc0(sizeof(TelSmsDatapackageInfo_t));
	if (NULL == encoded_msg) {
		msg("Memory Allocation Failed for CDMA Data Package");
		return FALSE;
	}

	/* Converting Destination number from ASCII to hex*/
	if (__sms_asciistring_to_hex(diallingNum, hex_string, dialNumLen) == FALSE) {
		msg("__sms_asciistring_to_hex() failed.");
		return FALSE;
	}

	msg("Dump of hex string");
	__util_hex_dump("    ", dialNumLen/2, hex_string);

	/* First byte specifies Messaging Type*/
	encoded_msg[index++] = SMS_PTP_MSG;

	/* 3GPP2 C.S0015-A section:3.4.3.1 Teleservice Identifier*/
	/* 1 TeleService Identifier */
	{
		EncodeCdmaSmsParamTeleservice(SMS_3GPP2_TELESERVICE_ID_CCMT, &param_teleservice);
		encoded_msg[index++] = param_teleservice.param_id;  /*TeleService Identifier Tag e.g SMS_PARAMID_TELESERVICE_ID*/
		encoded_msg[index++] = param_teleservice.param_len;  /*TeleService Identifier Length*/
		data =  htons(param_teleservice.teleserv_id);  /*Conversion of Little Endian*/
		memcpy(encoded_msg + index, &(data), sizeof(unsigned short));
		index += sizeof(unsigned short);
	}

	/* 3GPP2 C.S0015-A section:3.4.3.3 Address Parameters*/
	/* 1 Destination Address */
	{
		int dest_addr_offset = 2, previous_addr_index;
		unsigned char num_fields[SMS_ADDRESS_LEN_MAX + 1] = { 0, };

		encoded_msg[index++] = SMS_3GPP2_PARAM_ID_DESTINATION_ADDRESS;
		/* Fill the length value at the end */
		unsigned char *addr_len = &encoded_msg[index++]; /* param length */
		previous_addr_index = index;

		encoded_msg[index]  = ((digit_mode_val << (DIGIT_MODE_OFFSET)) & DIGIT_MODE_BIT);
		encoded_msg[index]  = ((number_mode_val << (NUMBER_MODE_OFFSET)) & NUMBER_MODE_BIT);

		bitarray_copy(num_fields, 0, dialNumLen * CHAR_BIT, &encoded_msg[index], dest_addr_offset);
		index++;

		bitarray_copy(hex_string, 0, dialNumLen * NIBBLE_BIT_LENGTH, &encoded_msg[index], dest_addr_offset);

		index += (dest_addr_offset + dialNumLen * NIBBLE_BIT_LENGTH) / 8 +
			((dest_addr_offset + dialNumLen * NIBBLE_BIT_LENGTH) % 8 ? 1 : 0);

		/* Updating the Address Length */
		*addr_len = index - previous_addr_index;

		msg("Dest addr final Index[%d] ", index);
		__util_hex_dump("    ", index, encoded_msg);
	}

	/* Bearer Data Options: */
	int bearer_data_previous_index;
	encoded_msg[index++] = SMS_3GPP2_PARAM_ID_BEARER_DATA;
	unsigned char *bearer_data_length = &encoded_msg[index++];
	bearer_data_previous_index = index;
	{
		/* 1  Message Identifier */
		{
			u8 offset = 4;
			EncodeCdmaSmsParamMsgId(SMS_3GPP2_BSUB_TELSERVICE_MSG_TYPE_SUBMIT, &param_msg);
			encoded_msg[index++] = param_msg.param_id;
			encoded_msg[index++] = SUBPARAM_LEN_VAL;


			/*Copying the Message ID*/
			bitarray_copy((&param_msg.type), offset, NIBBLE_BIT_LENGTH, &encoded_msg[index], 0);
			param_msg.msg_id = htons(param_msg.msg_id);

			bitarray_copy((unsigned char *)(&param_msg.msg_id), 0, sizeof(param_msg.msg_id) * CHAR_BIT, &encoded_msg[index], offset);
			param_msg.msg_id = ntohs(param_msg.msg_id);

			index += (offset + sizeof(param_msg.msg_id)*CHAR_BIT)/8 +
				(((offset + sizeof(param_msg.msg_id)*CHAR_BIT)%8) ? 1 : 0);

			msg("Message Identifier final Index[%d] ", index);
			__util_hex_dump("    ", index, encoded_msg);
		}

		/*3GPP2 C.S0015-A section 4.5.2 User Data*/
		/* 1 User Data */
		{
			int message_type = 0, offset = 5, prev_userdata_index;
			int current_index, i, initial_offset, index_data;
			EncodeCdmaSmsParamUserData(msgTxt, &param_user);
			encoded_msg[index++] = param_user.param_id;

			unsigned char *user_data_len = &encoded_msg[index++];

			prev_userdata_index = index;

			if (param_user.encoding == 0x01) {
				encoded_msg[index] = param_user.encoding << 3;
				encoded_msg[index] = (encoded_msg[index]  & (0xF8)) | (message_type >> 5);
				index++;
				encoded_msg[index] = message_type << 3;
			} else {
				encoded_msg[index] = param_user.encoding << 3;
			}

			encoded_msg[index] = (encoded_msg[index] & 0xF8) | (param_user.len >> 5);
			index++;
			encoded_msg[index] = (param_user.len << 3);


			current_index = index;
			initial_offset = offset;
			index_data = 0;
			for (i = 1; i <= param_user.len; i++) {
				bitarray_copy((const unsigned char *)&param_user.CHARi[i-1], 1, 7, (unsigned char *)&encoded_msg[current_index+index_data], offset);
				offset = (offset + 7) % 8;
				index_data = (initial_offset + i * 7) / 8;
			}
			index = index_data + current_index;
			index++;

			msg("User Data Index[%d ]:", index);
			__util_hex_dump("    ", index, encoded_msg);

			*user_data_len = index - prev_userdata_index;
		}
		*bearer_data_length = index - bearer_data_previous_index;
		__util_hex_dump("    ", index, encoded_msg);
	}

	memset(&data_package, 0x0, sizeof(TelSmsDatapackageInfo_t));

	data_package.format = TAPI_NETTEXT_NETTYPE_3GPP2;
	data_package.MsgLength = index;

	data_package.szData[data_package.MsgLength + 1] = '\0';

	memcpy(&(data_package.szData), encoded_msg, data_package.MsgLength);

	msg("Message Length[%d] ", data_package.MsgLength);

	__util_hex_dump("    ", data_package.MsgLength, data_package.szData);

	ret = tel_send_sms(handle, &data_package, 0x0, on_resp_send_cdma_msg, NULL);
	msg("Return Status of CDMA tel_send_sms[%d]", ret);

	free(encoded_msg);
	return TRUE;
}

static BOOL DecodeCellBroadcastMsg(int length, char *pPDU)
{
	int msgLength;
	int sn = 0; /* serial number */
	int msgID; /* message identifier */
	Sms_coding_scheme dcs;

	unsigned char page[2];
	int contents_length;
	char contents[SMS_CB_SIZE_MAX];

	unsigned char gs;
	unsigned char msgCode;
	unsigned char updateNumber;

	if (pPDU == NULL) {
		msg("pData is Null");
		return FALSE;
	}

	msg("cb msg type is %x", pPDU[0]);
	msg("cb msg len is %x", pPDU[1]);

	msg("$$$$$$$$$$$$$$$length of cb msg is %d", length);
	sn = (pPDU[2] << 8) | pPDU[1];

	gs = (pPDU[2] & 0xC0) >> 6;  /* bit 7, bit 6 */

	msgCode = (((pPDU[2] & 0x3F) << 4) | pPDU[3]) & 0xF0;

	updateNumber = pPDU[3] & 0x0F;  /* bit3, bit2, bit1, bit0 */

	msgID = pPDU[4] << 4 | pPDU[3];

	SmsUtilDecodeDCS(&dcs, pPDU[6]);

	page[0] = pPDU[7] & 0x0F; /* total page */
	page[1] = (pPDU[7] & 0xF0) >> 4; /* current page */

	msgLength = length - 2;

	contents_length = msgLength - 6;

	if (dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
		unsigned char inData[SMS_CB_SIZE_MAX + 1];

		memset(inData, 0x00, SMS_CB_SIZE_MAX + 1);

		memcpy((void *)inData, (void *)&pPDU[8], contents_length);
		SmsUtilUnpackGSMCode(contents, &pPDU[8], msgLength);
	} else {
		memcpy(contents, &pPDU[8], msgLength);
		contents[msgLength] = 0;
	}

	printf("*************************************\n");
	printf("serical number : 0x%04x \n", sn);
	printf("Geographical Scope: 0x%x\n", gs);
	printf("message code : 0x%x\n", msgCode);
	printf("update Number : 0x%x\n", updateNumber);
	printf("message identifier :  0x%x\n", msgID);
	printf("DCS-Commpressed?: %d (1: true)\n", dcs.bCompressed);
	printf("DCS-coding_group_type:  %x\n", dcs.coding_group_type);
	printf("contents bytes size : %d\n", contents_length);
	printf("page : (%d /%d) (page /total) \n", page[1], page[0]);
	printf("contents: %s\n", contents);
	printf("*************************************\n");

	return TRUE;
}

static BOOL EncodeSmsSubmitTpdu(MManager *mm, char *diallingNum,
		int dialNumLen, char *msg, int msg_len)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);

	TPDU_SMS_SUBMIT tpdu_submit;

	unsigned char packet[TAPI_NETTEXT_MSG_SIZE_MAX];

	unsigned char SCA[TAPI_NETTEXT_ADDRESS_LEN_MAX + 1];
	int ret = 0;

	tp_mr++;
	unsigned char tp_pid = 0; /* replace type (65) */
	unsigned char tp_dcs; /* class_1(17), class_2(18) */
	unsigned char tp_vp;

	int local_index = 0, ScIndex = 0;
	int i;
	int requestId = 0;
	unsigned char sca_len = 0;
	TapiResult_t returnStatus;

	TelSmsDatapackageInfo_t *pDataPackage = NULL;

	memset(packet, 0, TAPI_NETTEXT_MSG_SIZE_MAX);

	/************SCA ADDRESS GET START**************/
	/*
	 returnStatus = TelTapiSmsGetSCA(&sc_addr, 0);
	 printf("Service center addr returned from TAPI SCA address %s returnStatus %d\n", sc_addr.szDiallingNum, returnStatus);
	 SmsUtilDecodeAddrField(decodeScaAddr, &sc_addr.szDiallingNum[0], &sc_addr.Ton, &sc_addr.Npi);
	 sca_len = strlen(decodeScaAddr);

	 memcpy(SCA, decodeScaAddr, sca_len);
	 SCA[sca_len] = '\0';
	 */
	msg("Enter a Valid Service Centre Number  or Just Enter:");
	memset(SCA, 0, sizeof(SCA));

	ret = read(0, SCA, sizeof(SCA) - 1);
	if (ret <= 0) {
		msg(" NULL msg can NOT be sent ");
		return -1;
	}
	SCA[ret] = '\0';

	pDataPackage = malloc(sizeof(TelSmsDatapackageInfo_t));
	if (!pDataPackage)
		return -1;

	memset(pDataPackage, 0, sizeof(TelSmsDatapackageInfo_t));

	sca_len = strlen((char *)SCA) - 1;

	msg("Decode SCA Address =========");
	msg("SCA Number :%s, SCA Length :%d ", SCA, sca_len);
	msg("========================");

	/* SCA ADDRESS GET END */

	ScIndex = SmsUtilEncodeAddrField((unsigned char *)pDataPackage->Sca,
			(char *)SCA, sca_len, 0x00, 0x01);

	msg("ScIndex is %d", ScIndex);

	/* TP-MTI, TP-RD, TP-VPF, TP-SRR, TP_UDHI, TP-RP */
	tpdu_submit.msg_type = SMS_TPDU_SUBMIT;
	tpdu_submit.rd = FALSE; /* false :accept duplicate msg , true: reject duplicate msg */
	tpdu_submit.vpf = SMS_VP_RELATIVE;
	tpdu_submit.srr = FALSE; /* false :A status report is not requested , true: A status report is requested */
	tpdu_submit.udhi = FALSE; /* false: contains only the short message , true :a Header in addition to the short message */
	tpdu_submit.rp = FALSE; /* false :TP Reply Path parameter is not set in this SMS SUBMIT, true : TP Reply Path parameter is set in this SMS SUBMIT */

	packet[local_index] = tpdu_submit.msg_type; /* SUBMIT: bits 0, 1 */
	packet[local_index] |= tpdu_submit.rd ? 0x04 : 0; /* bit 2 */
	packet[local_index] |= tpdu_submit.vpf << 3; /* bits 3, 4 */
	packet[local_index] |= tpdu_submit.srr ? 0x20 : 0; /* bit 5 */
	packet[local_index] |= tpdu_submit.udhi ? 0x40 : 0; /* bit 6 */
	packet[local_index] |= tpdu_submit.rp ? 0x80 : 0; /* bit 7 */

	local_index++;
	msg("local_index of message ref is %d, msgref is %d", local_index, tp_mr);
	/* TP-MR */
	packet[local_index++] = tp_mr;

	/* TP-DA */
	local_index += SmsUtilEncodeAddrField(packet + local_index, diallingNum, dialNumLen, 0x01, 0x01);

	/* TP-PID */
	packet[local_index++] = tp_pid;

	/* TP_DCS (Data Coding Scheme) */
	tpdu_submit.dcs.bCompressed = FALSE;
	tpdu_submit.dcs.bmsg_class_set = TRUE;
	tpdu_submit.dcs.coding_group_type = SMS_CODGRP_SM_GENERAL_DCS;
	tpdu_submit.dcs.alphabet_type = SMS_ALPHABET_DEFAULT;
	tpdu_submit.dcs.class_type = SMS_CLASS_1;

	SmsUtilEncodeDCS(&tp_dcs, &tpdu_submit.dcs);

	packet[local_index++] = tp_dcs;

	/* TP-VP */
	tpdu_submit.vp.vp_rel_type = SMS_VP_REL_6H;
	tpdu_submit.vp.vp_type = SMS_VP_RELATIVE;
	tpdu_submit.vp.vpValue = 0xff;

	SmsUtilEncodeValidity(&tp_vp, &tpdu_submit.vp);

	packet[local_index++] = tp_vp;

	/* UDL */
	packet[local_index++] = msg_len;

	/* TP-UD */
	if (tpdu_submit.dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
		int pos;
		pos = SmsUtilPackGSMCode(packet + local_index, msg, msg_len);
		local_index += pos;
	}

	msg("Send packet to phonseSever[%d] -----", local_index);

	for (i = 0; i < local_index; i++) {
		msg("[%02x]", packet[i]);
		if (i % 10 == 9)
			msg("\n");
	}
	msg("\n");

	memcpy(pDataPackage->szData, packet, local_index);
	pDataPackage->MsgLength = local_index;

	msg("***Sending the Message (Api:SendMsg)***");

	msg("Length : %d", pDataPackage->MsgLength);

	pDataPackage->format = TAPI_NETTEXT_NETTYPE_3GPP;
	msg("Format : %d", pDataPackage->format);

	returnStatus = tel_send_sms(handle, pDataPackage, 0, on_resp_send_msg, NULL);

	msg("SendMsg : returnStatus=%d requestId=%d", returnStatus, requestId);

	free(pDataPackage);

	return TRUE;

}


static int DecodeSmsDeliverTpdu(int tpdu_len, char *pTPDU)
{
	TPDU_SMS_DELIVER tpdu_deliver;
	TmDateTime SCTimeStamp;
	int orgAddr_len = 0;
	char diallingNum[SMS_ADDRESS_LEN_MAX + 1];
	char orgAddr[SMS_ADDRESS_LEN_MAX + 1]; /* originating number */
	int org_ton, org_npi;
	int position = 0;

	/* TP-MTI, TP-MMS, TP-SRI, TP_UDHI, TP-RP */
	tpdu_deliver.msg_type = SMS_TPDU_DELIVER;
	tpdu_deliver.mms = (pTPDU[position] & 0x04) ? TRUE : FALSE; /* bit 2 (Message Type Indicator) */
	tpdu_deliver.sri = (pTPDU[position] & 0x20) ? TRUE : FALSE; /* bit 5 (Status Report Indication) */
	tpdu_deliver.udhi = (pTPDU[position] & 0x40) ? TRUE : FALSE; /* bit 6 (User Data Header Indicator) */
	tpdu_deliver.rp = (pTPDU[position] & 0x80) ? TRUE : FALSE; /* bit 7 (Reply Path) */

	position++;

	/* TP-OA */
	memset(diallingNum, 0, sizeof(diallingNum));

	orgAddr_len = SmsUtilDecodeAddrField(diallingNum, &pTPDU[position],
			&org_ton, &org_npi);
	if (orgAddr_len > SMS_ADDRESS_LEN_MAX - 1)
		orgAddr_len = SMS_ADDRESS_LEN_MAX - 1;

	position += 2;

	if (orgAddr_len % 2)
		position += orgAddr_len / 2 + 1;
	else
		position += orgAddr_len / 2;

	msg("pos = %d, length = %d", position, orgAddr_len);

	if (org_ton == SMS_TON_INTERNATIONAL) {
		orgAddr[0] = '+';
		memcpy(&orgAddr[1], diallingNum, orgAddr_len - 1);
		orgAddr[orgAddr_len + 1] = '\0';
	} else {
		memcpy(orgAddr, diallingNum, orgAddr_len);
		orgAddr[orgAddr_len] = '\0';
	}

	/* TP-PID */
	tpdu_deliver.pId = pTPDU[position];

	position++;

	/* TP-DCS */
	SmsUtilDecodeDCS(&tpdu_deliver.dcs, pTPDU[position]);
	dcs_pos = position;
	position++;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp(&pTPDU[position], &SCTimeStamp);

	position += 7;

	/* TP-UDL */
	tpdu_deliver.udl = pTPDU[position];

	if (tpdu_deliver.udl > SMS_SMDATA_SIZE_MAX)
		tpdu_deliver.udl = SMS_SMDATA_SIZE_MAX;

	position++;

	/* TP-UD */
	tpdu_deliver.userData = malloc(sizeof(unsigned char) * (SMS_SMDATA_SIZE_MAX + 1));
	if (!tpdu_deliver.userData)
		return 0;

	memset(tpdu_deliver.userData, 0,
		sizeof(unsigned char) * (SMS_SMDATA_SIZE_MAX + 1));

	if (tpdu_deliver.dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
		char inData[SMS_SMDATA_SIZE_MAX + 1];

		memset(inData, 0x00, SMS_SMDATA_SIZE_MAX + 1);

		memcpy((void *)inData, (void *)&pTPDU[position], tpdu_deliver.udl);

		SmsUtilUnpackGSMCode(tpdu_deliver.userData, inData,
				(unsigned char) tpdu_deliver.udl);
	}

	msg("*************************************");
	msg("Msg Type: TPDU_DELIVER");

	if (tpdu_deliver.rp)
		msg("RP: Set Reply Path");
	else if (!tpdu_deliver.rp)
		msg("RP: Not set Reply Path");

	if (tpdu_deliver.sri)
		msg("SRI: A status report shall be returned to the SME");
	else if (!tpdu_deliver.sri)
		msg("SRI: A status report shall be not returned to the SME");

	if (tpdu_deliver.mms)
		msg("MMS: No more messages are waiting for the MS in this SC");
	else if (!tpdu_deliver.mms)
		msg("MMS: More messages are waiting for the MS in this SC");

	if (tpdu_deliver.udhi)
		msg("UDHI: Contains a Header in addition to the short message.");
	else if (!tpdu_deliver.udhi)
		msg("UDHI: Only short message.");

	msg("DcsClass : %x (4 means Class None) ", tpdu_deliver.dcs.class_type);

	msg("From : %10s", orgAddr);
	msg("Time : %d-%d-%d  , %d:%d:%d",
		SCTimeStamp.year, SCTimeStamp.month,
		SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute, SCTimeStamp.second);
	msg("Message : %s ", tpdu_deliver.userData);
	msg("*************************************");

	if (tpdu_deliver.userData)
		free(tpdu_deliver.userData);

	return dcs_pos;
}

static int DecodeSmsStatusReportTpdu(int tpdu_len, char *pTPDU)
{
	TPDU_SMS_STATUS_REPORT status_report;

	TmDateTime SCTimeStamp;
	TmDateTime DischargeTime;

	int rcpAddr_len = 0;
	char diallingNum[SMS_ADDRESS_LEN_MAX];

	char rcpAddr[SMS_ADDRESS_LEN_MAX]; /* recipient number */
	int rcp_ton, rcp_npi;
	int position = 0;
	unsigned int mask;

	/* TP-MTI, TP MMS, TP SRQ, TP_UDHI */

	status_report.msg_type = pTPDU[position] & 0x03; /* bit 0, 1 */
	status_report.mms = pTPDU[position] & 0x04 ? TRUE : FALSE; /* bit 2 */
	status_report.srq = pTPDU[position] & 0x20 ? TRUE : FALSE; /* bit 5 */
	status_report.udhi = pTPDU[position] & 0x40 ? TRUE : FALSE; /* bit 6 */

	position++;

	/* TP-MR */
	status_report.mr = pTPDU[position];
	position++;

	/* TP-RA */
	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pTPDU[position], &rcp_ton, &rcp_npi);

	position += 2;

	rcpAddr_len = strlen((char *)diallingNum);
	if (rcpAddr_len % 2)
		position += rcpAddr_len / 2 + 1;
	else
		position += rcpAddr_len / 2;

	if (rcp_ton == SMS_TON_INTERNATIONAL) {
		rcpAddr[0] = '+';
		memcpy(&rcpAddr[1], diallingNum, rcpAddr_len);
		rcpAddr[rcpAddr_len + 1] = '\0';
	} else {
		memcpy(rcpAddr, diallingNum, rcpAddr_len);
		rcpAddr[rcpAddr_len] = '\0';
	}

	status_report.rcpAddr.DialNumLen = rcpAddr_len;
	memcpy(status_report.rcpAddr.szDiallingNum, rcpAddr, rcpAddr_len);
	status_report.rcpAddr.Npi = rcp_npi;
	status_report.rcpAddr.Ton = rcp_ton;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp(&pTPDU[position], &SCTimeStamp);
	position += 7;

	/* TP-DT */
	SmsUtilDecodeTimeStamp(&pTPDU[position], &DischargeTime);
	position += 7;

	/* TP-ST */
	status_report.status = pTPDU[position];
	position++;

	/* TP-PI */
	status_report.paraInd = pTPDU[position];

	mask = status_report.paraInd;

	if ((mask != 0xFF) && (mask != 0)) {
		/* Increment only if mask is valid */
		position++;

		/* TP-PID */
		if (mask & 0x01) {
			status_report.pId = pTPDU[position];
			position++;
		}

		/* TP-DCS */
		if (mask & 0x02) {
			SmsUtilDecodeDCS(&status_report.dcs, pTPDU[position]);
			position++;
		}

		if (mask & 0x04) {
			/* TP-UDL */
			status_report.udl = pTPDU[position];
			position++;

			/* TP-UD */

			status_report.userData = malloc(sizeof(unsigned char) * (SMS_SMDATA_SIZE_MAX + 1));
			if (!status_report.userData)
				return -1;

			memset(status_report.userData, 0,
				sizeof(unsigned char) * (SMS_SMDATA_SIZE_MAX + 1));

			if (status_report.dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
				char inData[SMS_SMDATA_SIZE_MAX + 1];

				memset(inData, 0x00, SMS_SMDATA_SIZE_MAX + 1);

				memcpy((void *)inData, (void *)&pTPDU[position],
						status_report.udl);

				SmsUtilUnpackGSMCode(status_report.userData, inData,
						(unsigned char)status_report.udl);
			}

		}
	}

	msg("*************************************");
	msg("Msg Type: STATUS_REPORT");
	if (status_report.mms)
		msg("MMS: No more messages are waiting for the MS in this SC");
	else if (!status_report.mms)
		msg("MMS: More messages are waiting for the MS in this SC");

	if (status_report.srq)
		msg("SRQ: the result of an SMS COMMAND ");
	else if (!status_report.srq)
		msg("SRQ: the result of a SMS SUBMIT.");

	if (status_report.udhi)
		msg("UDHI: Contains a Header in addition to the short message.");
	else if (!status_report.udhi)
		msg("UDHI: Only Short Message");

	msg("STATUS:%x", status_report.status);
	if (status_report.status == 0x00)
		msg("STATUS : Short message received by the SME");
	else if (status_report.status == 0x01)
		msg("STATUS : Short message forwarded by the SC to the SMEbut the SC is unable to confirm delivery");
	else if (status_report.status == 0x02)
		msg("STATUS : Short message replaced by the SC");

	msg("Recipient Number : %s", rcpAddr);
	msg("SC Time Stamp : %d-%d-%d  , %d:%d:%d",
		SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day,
		SCTimeStamp.hour, SCTimeStamp.minute, SCTimeStamp.second);
	msg("Discharge Time : %d-%d-%d  , %d:%d:%d",
		DischargeTime.year, DischargeTime.month, DischargeTime.day,
		DischargeTime.hour, DischargeTime.minute, DischargeTime.second);

	if (mask & 0x04)
		msg("Message : %s ", status_report.userData);

	msg("*************************************");

	if (status_report.userData)
		free(status_report.userData);

	return 1;
}

static void __on_noti_3gpp_sms_incoming_msg(TapiHandle *handle, TelSmsDatapackageInfo_t *datapackage,
	void *user_data)
{
	int scaAddr_len = 0;
	char *pTPDU;
	int tpdu_len = 0;
	unsigned char diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX + 1] = { 0, };
	unsigned char scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX + 2] = { 0, }; /* Service Center Address */
	int sca_ton, sca_npi;
	int position;
	int offset = 0;

	unsigned char MTI = 0;
	unsigned char MMS = 0;
	unsigned char SRI = 0;
	unsigned char UDHI = 0;
	unsigned char RP = 0;

	Sms_coding_scheme dcs;
	TapiResult_t returnStatus;
	int sca_length;
	position = 0;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SMS_INCOM_MSG);

	memset(diallingNum, 0, sizeof(diallingNum));

	sca_length = datapackage->Sca[0];

	sca_ton = (datapackage->Sca[offset + 1] & 0x70) >> 4;
	sca_npi = datapackage->Sca[offset + 1] & 0x0F;

	msg("TON %d", sca_ton);
	msg("NPI %d", sca_npi);

	/* Modem Send the hex encoded SCA Service center digits*/
	scaAddr_len = _util_hexstring_to_asciistring(&datapackage->Sca[2], (unsigned char *)diallingNum, sca_length-1);

	msg("SCA ascii length%d", scaAddr_len);
	msg("SCA Number:[%s]", diallingNum);

	if (scaAddr_len > TAPI_NETTEXT_SCADDRESS_LEN_MAX)
		scaAddr_len = TAPI_NETTEXT_SCADDRESS_LEN_MAX;

	if (sca_ton == SMS_TON_INTERNATIONAL) {
		scaAddr[0] = '+';
		memcpy(&scaAddr[1], diallingNum, scaAddr_len);
	} else {
		memcpy(scaAddr, diallingNum, scaAddr_len);
	}

	msg("Sc address in test app is %s", scaAddr);

	RP = datapackage->szData[position] & 0x80;
	UDHI = datapackage->szData[position] & 0x40;
	SRI = datapackage->szData[position] & 0x20;
	MMS = datapackage->szData[position] & 0x04;
	MTI = datapackage->szData[position] & 0x03;

	msg("RP [%x]", RP);
	msg("UDHI [%x]", UDHI);
	msg("SRI [%x]", SRI);
	msg("MMS [%x]", MMS);
	msg("MTI [%02x]", MTI);

	tpdu_len = datapackage->MsgLength;

	pTPDU = malloc(sizeof(unsigned char) * tpdu_len);
	if (pTPDU == NULL)
		return;

	memcpy(pTPDU, &datapackage->szData[position], tpdu_len);

	if (MTI == SMS_TPDU_DELIVER) {
		DecodeSmsDeliverTpdu(tpdu_len, pTPDU);

		msg("dcs_pos : %d", dcs_pos);

		SmsUtilDecodeDCS(&dcs, datapackage->szData[position + dcs_pos]);

		/* Simulating the behaviour of MSG_SERVICE:
		  * Storing only the Class two sms
		  *	@todo:refactoring
		  */
		if (dcs.class_type == SMS_CLASS_2) {
			TelSmsData_t WriteData;

			msg("dcs type is of class2");

			memset(&WriteData, 0, sizeof(TelSmsData_t));

			memcpy(WriteData.SmsData.Sca, datapackage->Sca, sca_length+1);

			WriteData.SmsData.MsgLength = datapackage->MsgLength;

			memcpy(WriteData.SmsData.szData, datapackage->szData, datapackage->MsgLength);

			WriteData.MsgStatus = TAPI_NETTEXT_STATUS_READ;

			returnStatus = tel_write_sms_in_sim(handle, &WriteData, on_resp_save_msg, NULL);

			msg("returnStatus for tel_write_sms_in_sim(): 0x%x", returnStatus);
		}

	} else if (MTI == SMS_TPDU_STATUS_REPORT) {
		DecodeSmsStatusReportTpdu(tpdu_len, pTPDU);
	}

	TelSmsDatapackageInfo_t *del_report = NULL;

	del_report = malloc(sizeof(TelSmsDatapackageInfo_t));
	if (del_report == NULL) {
		free(pTPDU);
		return;
	}
	memset(del_report, 0, sizeof(TelSmsDatapackageInfo_t));

	memcpy(del_report->Sca, datapackage->Sca, TAPI_SIM_SMSP_ADDRESS_LEN);

	del_report->szData[0] = SMS_TPDU_DELIVER_REPORT; /* TP-UDHI[bit6] : 0 */
	del_report->szData[1] = 0; /* TP-PI = 0; bit2: TP-UDL bit1: TP-DCS bit0: TP-PID (No option field) */
	del_report->MsgLength = 2;
	del_report->format = TAPI_NETTEXT_NETTYPE_3GPP;

	msg("***receive message (Api:SendDeliverreport)****");

	returnStatus = tel_send_sms_deliver_report(handle, del_report,
			TAPI_NETTEXT_SENDSMS_SUCCESS, on_resp_DeliverReport_msg, NULL);

	msg("returnStatus for tel_send_sms_deliver_report(): 0x%x", returnStatus);

	free(del_report);

	free(pTPDU);
}

static void __on_noti_3gpp2_sms_incoming_msg(TapiHandle *handle,
	TelSmsDatapackageInfo_t *datapackage, void *user_data)
{
	TelSmsDatapackageInfo_t del_report;
	TapiResult_t returnStatus;

	sms_util_decode_3gpp2(datapackage->MsgLength, datapackage->szData);

	memset(&del_report, 0x00, sizeof(del_report));
	del_report.format = TAPI_NETTEXT_NETTYPE_3GPP2;
	returnStatus = tel_send_sms_deliver_report(handle, &del_report,
			TAPI_NETTEXT_SENDSMS_SUCCESS, on_resp_DeliverReport_msg, NULL);

	msg("returnStatus for tel_send_sms_deliver_report(): 0x%x", returnStatus);
}

static void on_noti_sms_incom_msg(TapiHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	TelSmsDatapackageInfo_t *datapackage = data;
	if (datapackage == NULL)
		return;

	if (datapackage->format == TAPI_NETTEXT_NETTYPE_3GPP)
		__on_noti_3gpp_sms_incoming_msg(handle, datapackage, user_data);
	else if (datapackage->format == TAPI_NETTEXT_NETTYPE_3GPP2)
		__on_noti_3gpp2_sms_incoming_msg(handle, datapackage, user_data);
	else
		msg("Invalid msg format: %d", datapackage->format);
}

static void on_noti_sms_cb_incom_msg(TapiHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	TelSmsCbMsg_t *cbMsg = data;
	int length = 0;
	char pPDU[SMS_CB_SIZE_MAX + 3] = { 0, };

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SMS_CB_INCOM_MSG);

	length = cbMsg->Length;
	pPDU[0] = cbMsg->CbMsgType;
	pPDU[1] = length;
	memcpy(&(pPDU[2]), cbMsg->szMsgData, SMS_CB_SIZE_MAX + 1);

	DecodeCellBroadcastMsg(length, pPDU);
}

static void on_noti_sms_etws_incom_msg(TapiHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	TelSmsEtwsMsg_t *etwsMsg = data;
	int length = 0;
	char pPDU[TAPI_NETTEXT_ETWS_SIZE_MAX + 3] = { 0, };

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SMS_ETWS_INCOM_MSG);

	length = etwsMsg->Length;
	pPDU[0] = etwsMsg->EtwsMsgType;
	pPDU[1] = length;
	memcpy(&(pPDU[2]), etwsMsg->szMsgData, TAPI_NETTEXT_ETWS_SIZE_MAX + 1);
}

static void on_noti_sms_memory_status(TapiHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	int *memory_status = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SMS_MEMORY_STATUS);
	msg(" - memory_status = 0x%x", *memory_status);
}

static void on_noti_sms_ready_status(TapiHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	int *device_ready = data;

	msg("");
	msgb("event(%s) receive !!", TAPI_NOTI_SMS_DEVICE_READY);
	msg(" - device_ready = %d", *device_ready);

	if (TAPI_NETTEXT_READY_STATUS_3GPP == *device_ready)
		msg("TAPI_NETTEXT_READY_STATUS_3GPP");
	else if (TAPI_NETTEXT_READY_STATUS_3GPP2 == *device_ready)
		msg("TAPI_NETTEXT_READY_STATUS_3GPP");
	else if (TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2 == *device_ready)
		msg("TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2");
	else
		msg("TAPI_NETTEXT_READY_STATUS_NONE");
}

static void on_resp_send_msg(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	msg("");
	msgb("tel_send_sms() response receive");
	msg(" - result = 0x%x", result);
}

static void on_resp_send_cdma_msg(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	msg("");
	msgb("CDMA tel_send_sms() response received");
	msg(" - result = 0x%x", result);
}

static void on_resp_read_msg(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	TelSmsData_t *sim_data;
	int scaAddr_len = 0;
	char *pTPDU;
	int tpdu_len = 0;
	char diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX + 1] = { 0, };
	char scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX + 2] = { 0, }; /* Service Center Address */
	int sca_ton, sca_npi;
	int position;

	if (data == NULL) {
		msg("data is Null");
		return;
	}

	sim_data = (TelSmsData_t *)data;

	if (sim_data->MsgStatus == TAPI_NETTEXT_STATUS_UNREAD)
		msg("Msg Staus : received unread msg");
	else if (sim_data->MsgStatus == TAPI_NETTEXT_STATUS_READ)
		msg("Msg Staus : received read msg");
	else if (sim_data->MsgStatus == TAPI_NETTEXT_STATUS_UNSENT)
		msg("Msg Staus : unsent msg");
	else
		msg("Msg Staus : [%d]", sim_data->MsgStatus);

	msg("First Data [%x]", sim_data->SmsData.szData[0]);
	msg("Second Data [%x]", sim_data->SmsData.szData[1]);
	msg("Third Data [%x]", sim_data->SmsData.szData[2]);
	msg("Fourth Data [%x]", sim_data->SmsData.szData[3]);

	position = 0;
	/* SCA_ADDR */
	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, (char *)sim_data->SmsData.Sca,
			&sca_ton, &sca_npi);

	position += 2;  /* include Address-Length, Type of Address */

	scaAddr_len = strlen((char *)diallingNum);
	if (scaAddr_len > TAPI_NETTEXT_SCADDRESS_LEN_MAX - 1)
		scaAddr_len = TAPI_NETTEXT_SCADDRESS_LEN_MAX - 1;

	if (scaAddr_len % 2)
		position += scaAddr_len / 2 + 1;
	else
		position += scaAddr_len / 2;

	if (sca_ton == SMS_TON_INTERNATIONAL) {
		scaAddr[0] = '+';
		memcpy(&scaAddr[1], diallingNum, scaAddr_len);
	} else {
		memcpy(scaAddr, diallingNum, scaAddr_len);
	}

	tpdu_len = sim_data->SmsData.MsgLength;

	msg("SCA Number : %s tpdu_len is %d", scaAddr, tpdu_len);

	pTPDU = malloc(sizeof(unsigned char) * tpdu_len);
	if (!pTPDU)
		return;

	msg("bfor memcopy position is %d", position);
	memcpy(pTPDU, & (sim_data->SmsData.szData[0]), tpdu_len);
	msg("after memcpy");

	DecodeSmsDeliverTpdu(tpdu_len, pTPDU);

	free(pTPDU);
}

static void on_resp_save_msg(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	int *local_index = data;

	msg("");
	msgb("tel_write_sms_in_sim() response receive");
	msg(" - result = 0x%x", result);
	msg(" - local_index = %d", *local_index);
}

static void on_resp_delete_msg(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	int *local_index = data;

	msg("");
	msgb("tel_delete_sms_in_sim() response receive");
	msg(" - result = 0x%x", result);
	msg(" - local_index = %d", *local_index);
}

static void on_resp_DeliverReport_msg(TapiHandle *handle, int result,
		void *data, void *user_data)
{
	int *local_index = data;

	msg("");
	msgb("tel_send_sms_deliver_report() response receive");
	msg(" - result = 0x%x", result);
	msg(" - local_index = %d", *local_index);
}

static void on_resp_set_sms_sca(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	msg("");
	msgb("tel_set_sms_sca() response receive");
	msg(" - result = 0x%x", result);
}

static void on_resp_set_sms_cb_config(TapiHandle *handle, int result,
		void *data, void *user_data)
{
	msg("");
	msgb("tel_set_sms_cb_config() response receive");
	msg(" - result = 0x%x", result);
}

static void on_resp_set_sms_params(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	msg("");
	msgb("tel_set_sms_params() response receive");
	msg(" - result = 0x%x", result);
}

static void on_resp_set_mem_status(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	msg("");
	msgb("tel_set_sms_mem_status() response receive");
	msg(" - result = 0x%x", result);
}

static void on_resp_get_sms_sca(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	TelSmsAddressInfo_t *scaInfo = data;
	unsigned int i = 0;

	msg("");
	msgb("tel_get_sms_sca() response receive");
	msg(" - result = 0x%x", result);
	msg(" - TON = %d", scaInfo->Ton);
	msg(" - NPI = %d", scaInfo->Npi);
	msg(" - DialNumLen = %d", scaInfo->DialNumLen);
	msg(" - SCA Num");

	for (i = 0; i < scaInfo->DialNumLen; i++) {
		msg("[%02x]", scaInfo->szDiallingNum[i]);
		if (i % 10 == 9)
			msg("\n");
	}
}

static void on_resp_get_cb_config(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	TelSmsCbConfig_t *CBConfig;
	int i = 0;

	if (data == NULL) {
		msg("pData is Null");
		return;
	}

	CBConfig = (TelSmsCbConfig_t *)data;

	msg("");
	msgb("tel_get_sms_cb_config() response receive");
	msg(" - result = 0x%x", result);

	msg("=========CB Configuration=========");

	/*** CB Enable/Diable ***/
	if (CBConfig->CBEnabled == TRUE)
		msg("Cell Broadcast Msg Enabled...");
	else
		msg("Cell Broadcast Msg Disabled...");

	/*** CBMI  Count ***/
	msg("CBMI Range Count: %d", CBConfig->MsgIdRangeCount);

	/*** CBMI  List ***/
	if (CBConfig->Net3gppType == 0x01) {
		msg("Network type is 3gpp\n");
		if (CBConfig->MsgIdRangeCount != 0) {
			msg("----- CBMI List -----");
			for (i = 0; i < CBConfig->MsgIdRangeCount; i++) {
				msg("From No.%d - [0x%04x]", i,
					CBConfig->MsgIDs[i].Net3gpp.FromMsgId);
				msg("To No.%d - [0x%04x]", i, CBConfig->MsgIDs[i].Net3gpp.ToMsgId);
			}
		}
	} else if (CBConfig->Net3gppType == 0x02) {
		msg("Network type is CDMA\n");
		if (CBConfig->MsgIdRangeCount != 0) {
			msg("----- CBMI List -----");
			for (i = 0; i < CBConfig->MsgIdRangeCount; i++) {
				msg("No.%d Service Category - [0x%04x]", i, CBConfig->MsgIDs[i].Net3gpp2.CBCategory);
				msg("No.%d Language - [0x%02x]", i, CBConfig->MsgIDs[i].Net3gpp2.CBLanguage);
				msg("No.%d Selected - [0x%02x]", i, CBConfig->MsgIDs[i].Net3gpp2.Selected);
			}
		}
	}

	msg("==================================");
}

static void on_resp_get_sms_parameters(TapiHandle *handle, int result,
		void *data, void *user_data)
{
	TelSmsParams_t *smsp_param;
	int i = 0;

	if (data == NULL) {
		msg("data is Null");
		return;
	}
	smsp_param = (TelSmsParams_t *)data;

	msg("");
	msgb("tel_get_sms_parameters() response receive");
	msg(" - result = 0x%x", result);

	msg("record index is 0x%x", smsp_param->RecordIndex);
	msg("record len  is 0x%x", smsp_param->RecordLen);
	msg("alpha_id len  is 0x%x ", (int)smsp_param->AlphaIdLen);
	msg("alpha_id is %s ", smsp_param->szAlphaId);
	msg("param indicator is  0x%x", smsp_param->ParamIndicator);

	for (i = 0; i < (int) smsp_param->TpDestAddr.DialNumLen; i++)
		msg("DestAddr = %d [%02x]", i, smsp_param->TpDestAddr.szDiallingNum[i]);

	for (i = 0; i < (int) smsp_param->TpSvcCntrAddr.DialNumLen; i++)
		msg("SCAddr = %d [%02x]", i, smsp_param->TpSvcCntrAddr.szDiallingNum[i]);

	msg("pid 0x%x", smsp_param->TpProtocolId);
	msg("dcs is 0x%x", smsp_param->TpDataCodingScheme);
	msg("validity is 0x%x", smsp_param->TpValidityPeriod);
}

static void on_resp_get_paramcnt(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	int *RecordCount;

	if (data == NULL) {
		msg("data is Null");
		return;
	}

	RecordCount = (int *)data;

	msg("");
	msgb("tel_get_sms_parameter_count() response receive");
	msg(" - result = 0x%x", result);
	msg(" - In param_count_noti the record count is %d", *RecordCount);
}

static void on_resp_get_sms_count(TapiHandle *handle, int result, void *data,
		void *user_data)
{
	static TelSmsStoredMsgCountInfo_t *countInfo;
	int loop_counter = 0;
	TapiResult_t returnStatus = TAPI_API_SUCCESS;

	if (data == NULL) {
		msg("data is Null");
		return;
	}

	countInfo = (TelSmsStoredMsgCountInfo_t *)data;

	msg("");
	msgb("tel_get_sms_count() response receive");
	msg(" - result = 0x%x", result);

	if (countInfo->UsedCount != 0x00) {	/* if used count is not zero */
		msg("Index LIST..........");
		for (loop_counter = 0; loop_counter < countInfo->UsedCount;
				loop_counter++) {
			msg("[%02x]", countInfo->IndexList[loop_counter]);
			returnStatus = tel_read_sms_in_sim(handle,
					countInfo->IndexList[loop_counter], on_resp_read_msg,
					NULL);
			msg("After read msg: returnstatus %d", returnStatus);
		}
		msg("In MsgCountNotification total cnt is %d, usedcnt is %dapi_err %d",
				countInfo->TotalCount, countInfo->UsedCount, returnStatus);
	}

	msg("In MsgCountNotification total cnt is %d, usedcnt is %d",
			countInfo->TotalCount, countInfo->UsedCount);

}

static int SendMessage(MManager *mm, struct menu_data *menu)
{
	int ret;
	int msg_len = 0;
	char buf[SMS_ADDRESS_LEN_MAX];
	char message[512];
	char diallingNum[SMS_ADDRESS_LEN_MAX];
	int diallingNum_len = 0;

	memset(buf, 0, sizeof(buf));
	memset(diallingNum, 0, sizeof(diallingNum));
	diallingNum_len = 0;

	msg("Enter destination Number: ");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0) {
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	} else if (ret == 0)
		return ret;

	buf[SMS_ADDRESS_LEN_MAX - 1] = '\0';
	diallingNum_len = strlen(diallingNum);
	memcpy(&diallingNum[diallingNum_len], buf, strlen(buf));

	diallingNum_len = strlen(diallingNum); /* recalculate */
	msg("dialling num %s and dialling num len is %d", diallingNum,
			diallingNum_len);
	diallingNum[diallingNum_len] = 0;
	diallingNum_len = diallingNum_len - 1;

	msg("Enter Message: ");
	memset(message, 0, sizeof(message));

	ret = read(0, message, sizeof(message) - 1);

	if (ret <= 0) {
		msg(" NULL msg can NOT be sent ");
		return -1;
	}
	message[sizeof(message) - 1] = '\0';

	msg_len = strlen(message);
	message[--msg_len] = 0;

	msg("==========================");
	msg("To :%s", diallingNum);
	msg("Message: %sMsg Length:%d", message, msg_len);
	msg("Dialling number Length : %d", diallingNum_len);
	msg("==========================\n");

	EncodeSmsSubmitTpdu(mm, diallingNum, diallingNum_len, message, msg_len);
	return 1;

}

static int SendMessageCDMA(MManager *mm, struct menu_data *menu)
{
	int ret;
	int msg_len = 0;
	char buf[512] ;
	char message[512];
	char diallingNum[SMS_ADDRESS_LEN_MAX + 1] = { 0, };
	int diallingNum_len = 0;

	printf("\n");
	printf("Enter destination Number:\n>> ");
	fflush(stdout);

	memset(buf, 0x0, sizeof(buf));
	ret = read(0, buf, sizeof(buf));
	if (ret < 0) {
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	} else if (ret == 0)
		return ret;


	buf[strlen(buf)] = '\0';
	memcpy(diallingNum, buf, strlen(buf)+1);
	diallingNum_len = strlen(diallingNum);

	printf("dialling num [%s], dialling num len [%d]\n", diallingNum, diallingNum_len);
	printf("Enter Message\n>> ");
	fflush(stdout);

	memset(message, 0x0, sizeof(message));
	ret = read(0, message, sizeof(message) - 1);
	if (ret <= 0) {
		printf(" NULL msg can NOT be sent \n");
		return -1;
	}
	message[strlen(message) - 1] = '\0';

	msg_len = strlen(message);

	printf("===========================\n");
	printf("To: [%s] (len: %d)\n", diallingNum, diallingNum_len);
	printf("Message: [%s]\nMsg Length: [%d]\n", message, msg_len);
	printf("===========================\n\n");

	if (EncodeCdmaSmsSubmitTpdu(mm, diallingNum, diallingNum_len, message, msg_len) == FALSE)
		msg("EncodeCdmaSmsSubmitTpdu() failed.");

	return 1;
}

static int ReadMessage(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int selectedNo;
	int ret;
	char buf[100];
	int returnStatus = 0;

	memset(buf, 0, sizeof(buf));

	msg("*************************************");
	msg("1. SIM Message/RUIM Message");

	msg("Select Number:");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0) {
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	} else if (ret == 0)
		return ret;

	selectedNo = atoi(buf);
	msg("Selected Num in read message(value is 1) is %d ", selectedNo);
	switch (selectedNo) {
	case 1:
		msg("***Reading the message in SIM***");
		returnStatus = tel_get_sms_count(handle, on_resp_get_sms_count, NULL);
		msg("tel_get_sms_count():  0x%x", returnStatus);
	break;

	default:
		msg("Not supported Menu(%d) !!!", selectedNo);
	break;
	}

	return 1;
}

static int DeleteMessage(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);

	int ret, selected_num, index;
	char buf[100];

	TapiResult_t returnStatus;

	memset(buf, 0, sizeof(buf));

	msg("***Deleting the message(Api:DeleteMsg, GetMsgCount)***");

	msg("Select Number:");
	msg("1. Delete message at a particular index");
	msg("2. Delete all messages");

	msg("Select Number:");

	ret = read(0, buf, sizeof(buf));

	selected_num = atoi(buf);
	switch (selected_num) {
	case 1: {
		msg("Enter delete index >>");
		ret = read(0, buf, sizeof(buf));
		if (ret < 0) {
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		} else if (ret == 0)
			return ret;
		index = atoi(buf);
	}
	break;

	case 2: {
		index = -1;
	}
	break;

	default:
		msg("Invalid Number Selected");
		return -1;
	}
	msg("index value :%d", index);

	returnStatus = tel_delete_sms_in_sim(handle, index, on_resp_delete_msg, NULL);
	msg("returnstatus  tel_delete_sms_in_sim()  is for delete%d ", returnStatus);

	msg("Getting SMS Count after Deleting SMS");
	returnStatus = tel_get_sms_count(handle, on_resp_get_sms_count, NULL);
	msg("returnStatus for tel_get_sms_count()  %d", returnStatus);

	return 1;

}

static int CdmaGetting(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int selectedNo;
	int returnStatus = 0;
	int ready_status = 0;

	selectedNo = atoi(menu->key);
	switch (selectedNo) {
	case 1:
		msg("***Getting the CB Configuration(Api: GetCBConfig)***");
		returnStatus = tel_get_sms_cb_config(handle, on_resp_get_cb_config, NULL);
		msg("ReturnStatus[%d] ", returnStatus);
	break;

	case 3:
		msg("***Getting the SmsCount(Api:GetSmsCount)***");
		returnStatus = tel_get_sms_count(handle, on_resp_get_sms_count, NULL);
		msg("ReturnStatus[%d]", returnStatus);
	break;

	case 4:
		msg("***Getting the Sms Ready status(Api:Check_sms_device_status)***");
		returnStatus = tel_check_sms_device_status(handle, &ready_status);
		msg("API ReturnStatus[%d]", returnStatus);
		msg("Ready status = %d", ready_status);

		if (TAPI_NETTEXT_READY_STATUS_3GPP == ready_status)
			msg("TAPI_NETTEXT_READY_STATUS_3GPP");
		else if (TAPI_NETTEXT_READY_STATUS_3GPP2 == ready_status)
			msg("TAPI_NETTEXT_READY_STATUS_3GPP");
		else if (TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2 == ready_status)
			msg("TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2");
		else
			msg("TAPI_NETTEXT_READY_STATUS_NONE");
	break;

	default:
		return -1;
	}

	return 1;
}

static int Getting(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	int selectedNo;
	int returnStatus = 0;
	int ready_status = 0;

	selectedNo = atoi(menu->key);

	switch (selectedNo) {
	case 1:
		msg("***Getting the SCA(Api: GetSCA)****");
		returnStatus = tel_get_sms_sca(handle, 0, on_resp_get_sms_sca, NULL);
		msg("returnstatus for tel_get_sms_sca(): 0x %x", returnStatus);
	break;

	case 2:
		msg("***Getting the CB Configuration(Api: GetCBConfig)***");
		returnStatus = tel_get_sms_cb_config(handle, on_resp_get_cb_config, NULL);
		msg("ReturnStatus[%d] ", returnStatus);
	break;

	case 3:
		msg("****Getting the parameter(Api:GetParameter)****");
		returnStatus = tel_get_sms_parameters(handle, 0, on_resp_get_sms_parameters, NULL);
		msg("ReturnStatus[%d]]", returnStatus);
	break;

	case 5:
		msg("***Getting the parameterCount(Api:GetParameterCount)***");
		returnStatus = tel_get_sms_parameter_count(handle, on_resp_get_paramcnt, NULL);
		msg("ReturnStatus[%d]", returnStatus);
	break;

	case 6:
		msg("***Getting the SmsCount(Api:GetSmsCount)***");
		returnStatus = tel_get_sms_count(handle, on_resp_get_sms_count, NULL);
		msg("ReturnStatus[%d]", returnStatus);
	break;

	case 7:
		msg("***Getting the Sms Ready status(Api:Check_sms_device_status)***");
		returnStatus = tel_check_sms_device_status(handle, &ready_status);
		msg("ReturnStatus[%d]", returnStatus);
		msg("Ready status = %d", ready_status);

		if (TAPI_NETTEXT_READY_STATUS_3GPP == ready_status)
			msg("TAPI_NETTEXT_READY_STATUS_3GPP");
		else if (TAPI_NETTEXT_READY_STATUS_3GPP2 == ready_status)
			msg("TAPI_NETTEXT_READY_STATUS_3GPP");
		else if (TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2 == ready_status)
			msg("TAPI_NETTEXT_READY_STATUS_3GPP_AND_3GPP2");
		else
			msg("TAPI_NETTEXT_READY_STATUS_NONE");
	break;

	default:
		return -1;

	}

	return 1;
}

static int _get_int()
{
	char buf[255];
	int ret;

	memset(buf, 0, 255);
	ret = read(0, buf, 254);
	if (ret <= 0)
		return 0;

	return atoi(buf);
}

static int CdmaSetting(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);

	int settingMenu;
	int ret;

	char MemoryStatus[255] = {0, };

	TapiResult_t returnStatus;

	settingMenu = atoi(menu->key);

	switch (settingMenu) {
	case 2: { /* Set CB Enable/Disable */
		msg("****Setting the CB configuration(Api:SetCbConfig)****");
		CdmaCbSetting(mm, menu);
	}
	break;

	case 3: { /* Set Memory Full Notification */
		msg("Enter Memory Status to be set(1:Memory Available, 2:Memory Full)");
		memset(MemoryStatus, 0, sizeof(MemoryStatus));
		ret = read(0, MemoryStatus, sizeof(MemoryStatus));
		if (ret <= 0) {
			msg(" NULL msg can NOT be sent ");
			return -1;
		}

		msg("Memory Status type is %d ", atoi(MemoryStatus));
		returnStatus = tel_set_sms_memory_status(handle,
				atoi(MemoryStatus), on_resp_set_mem_status, NULL); /* Set to full 0x02-Full, 0x01-available */
		msg("api err after memstatus set is is %d", returnStatus);
	}
	break;

	case 4: { /* Set Stored MsgStaus */
		msg("Not suppored in this Test App !!!");
	}
	break;

	default:
		return -1;
	}

	return 1;
}

static int Setting(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);

	int settingMenu;
	int ret;
	int i;

	char MemoryStatus[255] = {0, };

	TelSmsCbConfig_t *pCBConfig;

	TelSmsParams_t smsParameters = { 0, };
	TapiResult_t returnStatus;

	settingMenu = atoi(menu->key);

	switch (settingMenu) {
	case 1: { /* Set Service Center Number */
		TelSmsAddressInfo_t sca = {0, };
		unsigned char sca_num[TAPI_SIM_SMSP_ADDRESS_LEN + 1];
		unsigned int sca_length = 0;
		unsigned int additional_len = 0;
		unsigned local_index, j;

		msg("*** Setting SCA (API: tel_set_sms_sca()) ****");

RETRY:
		msg("Enter the SCA NUMBER:");
		ret = scanf("%s", sca_num);
		if (ret > TAPI_SIM_SMSP_ADDRESS_LEN) {
			msg("Entered SCA is INVALID - SCA length cannot be greater than %d", TAPI_SIM_SMSP_ADDRESS_LEN);
			goto RETRY;
		}

		sca.Npi = TAPI_SIM_NPI_ISDN_TEL;
		sca.Ton = TAPI_SIM_TON_UNKNOWN;
		if (sca_num[0] == '+') {
			sca.Ton = TAPI_SIM_TON_INTERNATIONAL;
			additional_len = 1;
		}

		sca_length = strlen((char *)sca_num) - additional_len;
		msg("Sca Length:[%d]", sca_length);

		if (sca_length % 2 == 0) {
			for (local_index = additional_len, j = 0; local_index < sca_length; local_index += 2, j++)
				sca.szDiallingNum[j] = ((sca_num[local_index+1] & 0X0F) << 4) | (sca_num[local_index] & 0x0F);
			sca.szDiallingNum[(sca_length/2)] = '\0';
			sca.DialNumLen = (sca_length/2);
		} else {
			for (local_index = additional_len, j = 0; local_index < sca_length+1; local_index += 2, j++) {
				if (local_index == (sca_length - 1))
					sca.szDiallingNum[j] = ((0xF0) | (sca_num[local_index] & 0x0F));
				else
					sca.szDiallingNum[j] = ((sca_num[local_index+1] & 0X0F) << 4) | (sca_num[local_index] & 0x0F);
			}
			sca.szDiallingNum[(sca_length/2)] = sca.szDiallingNum[(sca_length/2)] | 0xF0;
			sca.DialNumLen = (sca_length/2)+1;
		}
		msg("%d", sca.DialNumLen);
		returnStatus = tel_set_sms_sca(handle, &sca, 0, on_resp_set_sms_sca, NULL);
		msg("Return status: [%d]", returnStatus);
	}
	break;

	case 3: { /* Set Deliver Report */
		msg("****Setting  Deliver Report(Api:SetDeliverReport)****");
	}
	break;

	case 4: { /* Set CB Enable/Disable */
		msg("****Setting the CB configuration(Api:SetCbConfig)****");

		pCBConfig = calloc(1, sizeof(TelSmsCbConfig_t));
		if (!pCBConfig)
			return -1;

		msg("Enter CB ON/OFF (1: Enable, 0:Disable): ");
		pCBConfig->CBEnabled = _get_int();

		msg("Enter Max ID Count: ");
		pCBConfig->MsgIdMaxCount = _get_int();

		msg("Enter MsgIdRangeCount (< 10): ");
		pCBConfig->MsgIdRangeCount = _get_int();

		if (pCBConfig->MsgIdRangeCount <= 0
				|| pCBConfig->MsgIdRangeCount >= TAPI_NETTEXT_SMS_CBMI_LIST_SIZE_MAX) {
			msg("Bad Range value");
			free(pCBConfig);
			return -1;
		}

		for (i = 0; i < pCBConfig->MsgIdRangeCount; i++) {
			msg("Enter %d FromMsgId : ", i + 1);
			pCBConfig->MsgIDs[i].Net3gpp.FromMsgId = _get_int();

			msg("Enter %d ToMsgId : ", i + 1);
			pCBConfig->MsgIDs[i].Net3gpp.ToMsgId = _get_int();

			msg("Enter %d Selected : ", i + 1);
			pCBConfig->MsgIDs[i].Net3gpp.Selected = _get_int();
		}

		pCBConfig->Net3gppType = 0x01;

		msg("from sms test setting the cb configuration:CBEnabled:%d, Net3gppType:%d, msgIdRangeCount:%d, From: %d\t To: %d\t Selected: %d",
			pCBConfig->CBEnabled, pCBConfig->Net3gppType,
			pCBConfig->MsgIdRangeCount,
			pCBConfig->MsgIDs[0].Net3gpp.FromMsgId,
			pCBConfig->MsgIDs[0].Net3gpp.ToMsgId,
			pCBConfig->MsgIDs[0].Net3gpp.Selected);

		returnStatus = tel_set_sms_cb_config(handle, pCBConfig,
				on_resp_set_sms_cb_config, NULL);
		msg("returnStatus after cbconfig set is is [%d]", returnStatus);

		free(pCBConfig);
	}
	break;

	case 5: {
		unsigned int sca_length;
		unsigned char sca_num[TAPI_SIM_SMSP_ADDRESS_LEN + 1];
		const char *name = "AlphaID";

		smsParameters.RecordIndex = 0x00;

		/* Alpha Id */
		smsParameters.AlphaIdLen = strlen(name);
		memcpy(&smsParameters.szAlphaId, name, strlen(name));

		/* Param Indicator*/
		smsParameters.ParamIndicator = 0xe1;

		/* Destination Number */
		memset(&smsParameters.TpDestAddr, 0x0, sizeof(TelSmsAddressInfo_t));

		smsParameters.TpProtocolId = 3;/* PID */
		smsParameters.TpDataCodingScheme = 2;/* DCS */
		smsParameters.TpValidityPeriod = 1;/* VP */

		do {
			msg("Enter the SCA NUMBER:");
			ret = scanf("%s", sca_num);
			if (ret > TAPI_SIM_SMSP_ADDRESS_LEN)
				msg("Entered SCA is INVALID - SCA length cannot be greater than %d", TAPI_SIM_SMSP_ADDRESS_LEN);
		} while (ret > TAPI_SIM_SMSP_ADDRESS_LEN);

		sca_length = SmsUtilEncodeSca(&(smsParameters.TpSvcCntrAddr), sca_num);
		msg("Encoded SCA Address Length[%d]", sca_length);

		returnStatus = tel_set_sms_parameters(handle, &smsParameters, on_resp_set_sms_params, NULL);
		msg("returnstatus after sparam set  is %d", returnStatus);
	}
	break;

	case 6: { /* Set Memory Full Notification */
		msg("Enter Memory Status to be set(1:Memory Available, 2:Memory Full)");
		memset(MemoryStatus, 0, sizeof(MemoryStatus));
		ret = read(0, MemoryStatus, sizeof(MemoryStatus));
		if (ret <= 0) {
			msg(" NULL msg can NOT be sent ");
			return -1;
		}
		msg("Memory Status type is %d ", atoi(MemoryStatus));
		returnStatus = tel_set_sms_memory_status(handle,
				atoi(MemoryStatus), on_resp_set_mem_status, NULL); /* Set to full 0x02-Full, 0x01-available */
		msg("api err after memstatus set is is %d", returnStatus);
	}
	break;

	case 7: { /* Set Stored MsgStaus */
		msg("Not suppored in this Test App !!!");
	}
	break;

	case 8: {
		msg("Not suppored in this Test App !!!");
	}
	break;

	case 9: {
		msg("Not suppored in this Test App !!!");
	}
	break;

	default:
		return -1;
	}

	return 1;
}

static int CdmaCbSetting(MManager *mm, struct menu_data *menu)
{
	TapiHandle *handle = menu_manager_ref_user_data(mm);
	TelSmsCbConfig_t *cb_config;
	int i;
	int return_status = 0;

	msg("****Setting the CB configuration(Api:SetCbConfig)****");

	cb_config = calloc(1, sizeof(TelSmsCbConfig_t));
	if (!cb_config)
		return -1;

	msg("Enter CB ON/OFF (1: Enable, 0:Disable): ");
	cb_config->CBEnabled = _get_int();

	msg("Enter Max ID Count: ");
	cb_config->MsgIdMaxCount = _get_int();

	msg("Enter MsgIdRangeCount (< 10): ");
	cb_config->MsgIdRangeCount = _get_int();

	if (cb_config->MsgIdRangeCount <= 0 || cb_config->MsgIdRangeCount >= TAPI_NETTEXT_SMS_CBMI_LIST_SIZE_MAX) {
		msg("Bad Range value");
		free(cb_config);
		return -1;
	}

	for (i = 0; i < cb_config->MsgIdRangeCount; i++) {
		msg("Enter %d Service Category : ", i + 1);
		cb_config->MsgIDs[i].Net3gpp2.CBCategory = _get_int();/*3GPP2 C.R1001-D v1.0 section 9.3 Service Category Assignments*/

		msg("Enter %d Language : ", i + 1);
		cb_config->MsgIDs[i].Net3gpp2.CBLanguage = _get_int();

		msg("Enter %d Selected : ", i + 1);
		cb_config->MsgIDs[i].Net3gpp2.Selected = _get_int();
	}

	cb_config->Net3gppType = 0x02; /* CDMA */

	msg("CBEnabled:%d, Net3gppType:%d, msgIdRangeCount:%d",
			cb_config->CBEnabled, cb_config->Net3gppType, cb_config->MsgIdRangeCount);

	return_status = tel_set_sms_cb_config(handle, cb_config, on_resp_set_sms_cb_config, NULL);
	msg("returnStatus after cbconfig set is [%d]", return_status);

	free(cb_config);
	return 1;
}

static struct menu_data menu_sms_getting[] = {
		{ "1", "Get Service Center Number", NULL, Getting, NULL },
		{ "2", "Get CB Setting Information", NULL, Getting, NULL },
		{ "3", "Get SMS Parameters", NULL, Getting, NULL },
		{ "4", "Get SMS PreferredBearer Information", NULL, Getting, NULL },
		{ "5", "Get SMS Parameter Count", NULL, Getting, NULL },
		{ "6", "Get Message Count", NULL, Getting, NULL },
		{ "7", "Get SMS ready status", NULL, Getting, NULL }, { NULL, NULL , }, };

static struct menu_data menu_cdma_sms_getting[] = {
		{ "1", "Get CB Setting Information", NULL, CdmaGetting, NULL },
		{ "2", "Get SMS PreferredBearer Information", NULL, CdmaGetting, NULL },
		{ "3", "Get Message Count in RUIM", NULL, CdmaGetting, NULL },
		{ "4", "Get SMS ready status", NULL, CdmaGetting, NULL }, { NULL, NULL , }, };

static struct menu_data menu_sms_setting[] = {
		{ "1", "Set Service Center Number", NULL, Setting, NULL},
		{ "2", "Set Preferred Bearer Type", NULL, Setting, NULL},
		{ "3", "Set CB Enable/Disable", NULL, Setting, NULL},
		{ "4", "Set SMS Parameters", NULL, Setting, NULL},
		{ "5", "Set Memory Status", NULL, Setting, NULL},
		{ "6", "Set Message Status", NULL, Setting, NULL}, { NULL, NULL , }, };

static struct menu_data menu_cdma_sms_setting[] = {
		{ "1", "Set PreferredBearer Type", NULL, CdmaSetting, NULL },
		{ "2", "Set CB Enable/Disable", NULL, CdmaSetting, NULL },
		{ "3", "Set Memory Status", NULL, CdmaSetting, NULL },
		{ "4", "Set Stored MsgStaus", NULL, CdmaSetting, NULL },
		{ NULL, NULL , }, };

static struct menu_data menu_sms_wcdma[] = {
		{ "1", "Send Message", NULL, SendMessage, NULL},
		{ "2", "Read Message", NULL, ReadMessage, NULL},
		{ "3", "Delete Message", NULL, DeleteMessage, NULL},
		{ "4", "Setting", menu_sms_setting, NULL, NULL },
		{ "5", "Getting SMS Information", menu_sms_getting, NULL, NULL },
		{ NULL, NULL , }, };

static struct menu_data menu_sms_cdma[] = {
		{ "1", "Send CDMA Message", NULL, SendMessageCDMA, NULL },
		{ "2", "Read CDMA Message in RUIM", NULL, ReadMessage, NULL },
		{ "3", "Delete CDMA Message in RUIM", NULL, DeleteMessage, NULL },
		{ "4", "Getting CDMA SMS Information", menu_cdma_sms_getting, NULL, NULL },
		{ "5", "Setting CDMA SMS Information", menu_cdma_sms_setting, NULL, NULL },
		{ NULL, NULL , }, };

struct menu_data menu_sms[] = { { "1", "3GPP[GSM/WCDMA]", menu_sms_wcdma, NULL, NULL }, {
		"2", "3GPP2[CDMA/1xEV-DO]", menu_sms_cdma, NULL, NULL }, { NULL, NULL , }, };

void register_sms_event(TapiHandle *handle)
{
	int ret;

	/* SMS */
	ret = tel_register_noti_event(handle, TAPI_NOTI_SMS_INCOM_MSG,
			on_noti_sms_incom_msg, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SMS_CB_INCOM_MSG,
			on_noti_sms_cb_incom_msg, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SMS_ETWS_INCOM_MSG,
			on_noti_sms_etws_incom_msg, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SMS_MEMORY_STATUS,
			on_noti_sms_memory_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);

	ret = tel_register_noti_event(handle, TAPI_NOTI_SMS_DEVICE_READY,
			on_noti_sms_ready_status, NULL);
	if (ret != TAPI_API_SUCCESS)
		msg("event register failed(%d)", ret);
}
