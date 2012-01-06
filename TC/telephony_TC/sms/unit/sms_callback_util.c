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
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

#include <assert.h>
#include <glib.h>
#include <TapiCommon.h>

#include "sms_callback_util.h"

int error_code;
int dcs_pos =0;

TS_UINT8 tp_mr = 0x00;


TS_BOOL DecodeCellBroadcastMsg(int length, char *pPDU)
{
	int msgLength;
	int sn = 0; //serial number
	int msgID; //message identifier
	tapi_nettext_coding_scheme dcs;

	TS_UINT8 page[2];
	int contents_length;
	TS_UINT8 contents[TAPI_NETTEXT_CB_SIZE_MAX];

	TS_UINT8 gs;
	TS_UINT8 msgCode;
	TS_UINT8 updateNumber;

	printf("cb msg type is %x\n", pPDU[0]);
	printf("cb msg len is %x\n", pPDU[1]);

	printf("$$$$$$$$$$$$$$$length of cb msg is %d\n", length);
	sn = (pPDU[2] << 8) | pPDU[1];

	gs = (pPDU[2] & 0xC0) >> 6; //bit 7, bit 6

	msgCode = (((pPDU[2] & 0x3F) << 4) | pPDU[3]) & 0xF0;

	updateNumber = pPDU[3] & 0x0F; //bit3, bit2, bit1, bit0

	msgID = (pPDU[4] << 4) | pPDU[3];

	SmsUtilDecodeDCS(&dcs, pPDU[6]);

	page[0] = pPDU[7] & 0x0F; //total page
	page[1] = (pPDU[7] & 0xF0) >> 4; //current page


	msgLength = length - 2;

	contents_length = msgLength - 6;

	if (dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT) {
		TS_BYTE inData[TAPI_NETTEXT_CB_SIZE_MAX + 1];

		memset(inData, 0x00, TAPI_NETTEXT_CB_SIZE_MAX + 1);

		memcpy((void*) inData, (void*) &pPDU[8], contents_length);
		SmsUtilUnpackGSMCode((char *) contents, (TS_BYTE *) &pPDU[8], msgLength);
	}
	else {
		memcpy(contents, &pPDU[8], msgLength);
		contents[msgLength] = 0; //NULL;
	}

	printf("*************************************\n");
	printf("serical number : 0x%04x \n", sn);
	printf("Geographical Scope: %x\n", gs);
	printf("message code : %x\n", msgCode);
	printf("update Number : %x\n", updateNumber);
	printf("message identifier :  %x\n", msgID);
	printf("data coding scheme :  %x\n", (unsigned int) &dcs);
	printf("contents bytes size : %d\n", contents_length);
	printf("page : (%d /%d) (page /total ) \n", page[1], page[0]);
	printf("contents: %s\n", contents);
	printf("*************************************\n");

	return TRUE;
}

TS_BOOL DecodeSmsSubmitTpdu(int pdu_len, char *pPDU)
{
	TPDU_SMS_SUBMIT tpdu_submit;

	TS_BYTE scaAddr_len = 0;
	int destAddr_len = 0;
	TS_BYTE diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	TS_BYTE scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	TS_BYTE destAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //destination number
	int sca_ton, sca_npi, dest_ton, dest_npi;
	int position;

	position = 0;

	/* SCA_ADDR */
	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField((char *) diallingNum, (unsigned char *) &pPDU[position], &sca_ton, &sca_npi);

	position += 2; //include Address-Length, Type of Address

	scaAddr_len = strlen((char *) diallingNum);

	if (scaAddr_len % 2)
		position += scaAddr_len / 2 + 1;
	else
		position += scaAddr_len / 2;

	if (sca_ton == SIM_TON_INTERNATIONAL) {
		scaAddr[0] = '+';
		memcpy(&scaAddr[1], diallingNum, scaAddr_len);
		scaAddr[scaAddr_len + 1] = '\0';
	}
	else {
		memcpy(scaAddr, diallingNum, scaAddr_len);
		scaAddr[scaAddr_len] = '\0';
	}

	/* TP-MTI, TP-RD, TP-VPF,  TP-RP, TP_UDHI, TP-SRR */

	tpdu_submit.msg_type = SMS_TPDU_SUBMIT; // bit 0,1(Message Type Indicator )
	tpdu_submit.rd = (pPDU[position] & 0x04) ? TRUE : FALSE;// bit 2 (Message Type Indicator)
	tpdu_submit.vpf = (pPDU[position] & 0x18) >> 3; //  bits 3, 4 ( validity period format)
	tpdu_submit.srr = (pPDU[position] & 0x20) ? TRUE : FALSE; // bit 5 (Status Report Request)
	tpdu_submit.udhi = (pPDU[position] & 0x40) ? TRUE : FALSE; // bit 6 (User Data Header Indicator)
	tpdu_submit.rp = (pPDU[position] & 0x80) ? TRUE : FALSE; // bit 7 (Reply Path)

	position++;

	/* TP-MR */
	tpdu_submit.mr = pPDU[position];

	position++;

	/* TP-DA */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField((char *) diallingNum, (unsigned char *) &pPDU[position], &dest_ton, &dest_npi);

	position += 2;

	destAddr_len = strlen((char*) diallingNum);
	if (destAddr_len % 2)
		position += destAddr_len / 2 + 1;
	else
		position += destAddr_len / 2;

	if (dest_ton == SIM_TON_INTERNATIONAL) {
		destAddr[0] = '+';
		memcpy(&destAddr[1], diallingNum, destAddr_len);
		destAddr[destAddr_len + 1] = '\0';
	}
	else {
		memcpy(destAddr, diallingNum, destAddr_len);
		destAddr[destAddr_len] = '\0';
	}

	tpdu_submit.desAddr.DialNumLen = destAddr_len;
	memcpy(tpdu_submit.desAddr.szDiallingNum, destAddr, destAddr_len);
	tpdu_submit.desAddr.Npi = dest_npi;
	tpdu_submit.desAddr.Ton = dest_ton;

	/* TP-PID */

	tpdu_submit.pId = pPDU[position];
	position++;

	/* TP DCS */
	SmsUtilDecodeDCS(&tpdu_submit.dcs, pPDU[position]);
	position++;

	/* TP VP */
	tpdu_submit.vp.vp_type = TAPI_NETTEXT_VP_RELATIVE; //temp

	switch (tpdu_submit.vp.vp_type) {
		case TAPI_NETTEXT_VP_RELATIVE:
			tpdu_submit.vp.vpValue = pPDU[position];
			tpdu_submit.vp.vp_rel_type = TAPI_NETTEXT_VP_REL_1D;
			position++;
			break;

		case TAPI_NETTEXT_VP_ABSOLUTE:
		case TAPI_NETTEXT_VP_ENHANCED:
			position += 7;
			break;

		default:
			break;
	}

	/* TP UDL */
	tpdu_submit.udl = pPDU[position];

	if (tpdu_submit.udl > TAPI_NETTEXT_SMDATA_SIZE_MAX)
		tpdu_submit.udl = TAPI_NETTEXT_SMDATA_SIZE_MAX;

	position++;

	/* TP UD */
	tpdu_submit.userData = malloc(sizeof(unsigned char) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

	memset(tpdu_submit.userData, 0, sizeof(unsigned char) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

	if (tpdu_submit.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT) {
		TS_BYTE inData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1];

		memset(inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);

		memcpy((void*) inData, (void*) &pPDU[position], tpdu_submit.udl);

		SmsUtilUnpackGSMCode((char *) tpdu_submit.userData, inData, (TS_UINT8) tpdu_submit.udl);
	}

	printf("*************************************\n");
	printf("Msg Type: TPDU_SUBMIT\n");

	if (tpdu_submit.rd)
		printf("RD: Reject Duplicate\n");
	else if (!tpdu_submit.rd)
		printf("RD: Accept Duplicate\n");

	if (tpdu_submit.srr)
		printf("SRR: A status report is requested\n");
	else if (!tpdu_submit.srr)
		printf("SRR: A status report is not  requested\n");

	if (tpdu_submit.udhi)
		printf("UDHI: Contains a Header in addition to the short message.\n");
	else if (!tpdu_submit.udhi)
		printf("UDHI: Only Short Message\n");

	if (tpdu_submit.rp)
		printf("RP: Set Reply Path\n");
	else if (!tpdu_submit.rp)
		printf("RP: Not set Reply Path\n");

	printf("SCA Addr : %s\n", scaAddr);
	printf("To : %s\n", destAddr);
	printf("Message : %s \n", tpdu_submit.userData);
	printf("*************************************\n");

	return TRUE;
}

int DecodeSmsDeliverTpdu(int tpdu_len, char *pTPDU)
{
	TPDU_SMS_DELIVER tpdu_deliver;
	TmDateTime SCTimeStamp;
	int orgAddr_len = 0;
	TS_BYTE diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	TS_BYTE orgAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //originating number
	int org_ton, org_npi;
	int position;

	position = 0;

	/* TP-MTI, TP-MMS, TP-SRI, TP_UDHI, TP-RP */
	tpdu_deliver.msg_type = SMS_TPDU_DELIVER;
	tpdu_deliver.mms = (pTPDU[position] & 0x04) ? TRUE : FALSE;// bit 2 (Message Type Indicator)
	tpdu_deliver.sri = (pTPDU[position] & 0x20) ? TRUE : FALSE; // bit 5 (Status Report Indication)
	tpdu_deliver.udhi = (pTPDU[position] & 0x40) ? TRUE : FALSE; // bit 6 (User Data Header Indicator)
	tpdu_deliver.rp = (pTPDU[position] & 0x80) ? TRUE : FALSE; // bit 7 (Reply Path)

	position++;

	/* TP-OA */
	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField((char *) diallingNum, (unsigned char *) &pTPDU[position], &org_ton, &org_npi);

	position += 2;

	orgAddr_len = strlen((char*) diallingNum);
	if (orgAddr_len % 2)
		position += orgAddr_len / 2 + 1;
	else
		position += orgAddr_len / 2;

	if (org_ton == SIM_TON_INTERNATIONAL) {
		orgAddr[0] = '+';
		memcpy(&orgAddr[1], diallingNum, orgAddr_len);
		orgAddr[orgAddr_len + 1] = '\0';
	}
	else {
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
	SmsUtilDecodeTimeStamp((unsigned char *) &pTPDU[position], &SCTimeStamp);

	position += 7;

	/* TP-UDL */
	tpdu_deliver.udl = pTPDU[position];
	if (tpdu_deliver.udl > TAPI_NETTEXT_SMDATA_SIZE_MAX)
		tpdu_deliver.udl = TAPI_NETTEXT_SMDATA_SIZE_MAX;

	position++;

	/* TP-UD */
	tpdu_deliver.userData = malloc(sizeof(unsigned char) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

	memset(tpdu_deliver.userData, 0, sizeof(unsigned char) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

	if (tpdu_deliver.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT) {
		TS_BYTE inData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1];

		memset(inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);

		memcpy((void*) inData, (void*) &pTPDU[position], tpdu_deliver.udl);

		SmsUtilUnpackGSMCode((char *) tpdu_deliver.userData, inData, (TS_UINT8) tpdu_deliver.udl);
	}

	printf("*************************************\n");
	printf("Msg Type: TPDU_DELIVER\n");

	if (tpdu_deliver.rp)
		printf("RP: Set Reply Path\n");
	else if (!tpdu_deliver.rp)
		printf("RP: Not set Reply Path\n");

	if (tpdu_deliver.sri)
		printf("SRI: A status report shall be returned to the SME\n");
	else if (!tpdu_deliver.sri)
		printf("SRI: A status report shall be not returned to the SME\n");

	if (tpdu_deliver.mms)
		printf("MMS: No more messages are waiting for the MS in this SC\n");
	else if (!tpdu_deliver.mms)
		printf("MMS: More messages are waiting for the MS in this SC\n");

	if (tpdu_deliver.udhi)
		printf("UDHI: Contains a Header in addition to the short message.\n");
	else if (!tpdu_deliver.udhi)
		printf("UDHI: Only short message.\n");

	printf("DcsClass : %x \n", tpdu_deliver.dcs.class_type);

	printf("From : %s\n", orgAddr);
	printf("Time : %d-%d-%d  , %d:%d:%d\n", SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute, SCTimeStamp.second);
	printf("Message : %s \n", tpdu_deliver.userData);
	printf("*************************************\n");

	return dcs_pos;
}

TS_BOOL DecodeSmsSubmitReportTpdu(int length, char *data)
{
	TPDU_SMS_SUBMIT_REPORT submit_report;
	TmDateTime SCTimeStamp;
	int position = 1;
	int i = 0;
	TS_BYTE msgRef, tpdu_len;
	int result = 0;
	TS_BYTE mask;

	if (data == NULL)
		return FALSE;

	/* Result */
	printf("length :%d\n", length);

	for (i = 0; i < length; i++)
		printf("[%02x]", data[i]);

	result = data[position];

	printf("\nIn ackrecv (return status) result is %x\n", result);
	position++;

	/* Msg Reference */
	msgRef = data[position++];
	printf("In ackrecv  position :%d msgref is %x\n", position, msgRef);
	position++;

	/* TPDU Length */
	tpdu_len = data[position];
	printf("In ackrecv position :%d len is %d\n", position, tpdu_len);

	/**** SUBMIT REPORT TPDU PARSING***/

	/* TP-MTI , TP-UDHI */
	printf("before TPDU position value:%d\n", position);

	submit_report.msg_type = data[position] & 0x03; //bit 0,1
	submit_report.udhi = data[position] & 0x40 ? TRUE : FALSE; //bit 6;

	position++;

	if (result == FAIL) {
		/* TP-FCS */
		submit_report.fcs = data[position];
		position++;
	}

	/* TP-PI  - set to a 1 indicates that a particular optional parameter is present in the fields which follow. */
	/* Masking Bit  - TP -PID (bit 0), TP-DCS(bit 1), TP-UDL(bit 2) */

	submit_report.paraInd = data[position];
	mask = submit_report.paraInd;
	position++;

	/* TP-SCTS */

	SmsUtilDecodeTimeStamp((unsigned char *) &data[position], &SCTimeStamp);

	position += 7;

	if ((mask != 0xFF) && (mask != 0)) {
		/* TP-PID */
		if (mask & 0x01) {
			submit_report.pId = data[position];
			position++;
			printf("TP-PID :%x\n", submit_report.pId);
		}

		/* TP-DCS */
		if (mask & 0x02) {
			SmsUtilDecodeDCS(&submit_report.dcs, data[position]);
			position++;
			printf("TP-DCS :%x\n", data[position]);
		}

		if (mask & 0x04) {
			/* TP-UDL */
			submit_report.udl = data[position];
			position++;

			printf("TP-UDL :%x\n", data[position]);

			/* TP-UD */

			submit_report.userData = malloc(sizeof(TS_BYTE) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

			memset(submit_report.userData, 0, sizeof(TS_BYTE) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

			if (submit_report.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT) {
				TS_BYTE inData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1];

				memset(inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);

				memcpy((void*) inData, (void*) &data[position], submit_report.udl);

				SmsUtilUnpackGSMCode((char *) submit_report.userData, inData, (TS_UINT8) submit_report.udl);
			}

		}
	}

	printf("*************************************\n");

	if (result != 0x00)
		printf("SMS SEND FAIL!!\n");
	else
		printf("SMS SEND SUCCESS!!\n");

	if (result != 0x00)
		printf("Failure Code : %x\n", submit_report.fcs);

	printf("*************************************\n");

	return TRUE;
}

TS_BOOL DecodeSmsStatusReportTpdu(int tpdu_len, char *pTPDU)
{
	TPDU_SMS_STATUS_REPORT status_report;

	TmDateTime SCTimeStamp;
	TmDateTime DischargeTime;

	int rcpAddr_len = 0;
	TS_BYTE diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	//	TS_BYTE              scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	TS_BYTE rcpAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //recipient number
	int rcp_ton, rcp_npi;
	int position = 0;
	TS_UINT mask;

	/* TP-MTI, TP MMS, TP SRQ, TP_UDHI */

	status_report.msg_type = pTPDU[position] & 0x03; // bit 0,1
	status_report.mms = pTPDU[position] & 0x04 ? TRUE : FALSE; // bit 2
	status_report.srq = pTPDU[position] & 0x20 ? TRUE : FALSE; //bit 5
	status_report.udhi = pTPDU[position] & 0x40 ? TRUE : FALSE; //bit 6

	position++;

	/* TP-MR */
	status_report.mr = pTPDU[position];
	position++;

	/* TP-RA */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField((char *) diallingNum, (unsigned char *) &pTPDU[position], &rcp_ton, &rcp_npi);

	position += 2;

	rcpAddr_len = strlen((char*) diallingNum);
	if (rcpAddr_len % 2)
		position += rcpAddr_len / 2 + 1;
	else
		position += rcpAddr_len / 2;

	if (rcp_ton == SIM_TON_INTERNATIONAL) {
		rcpAddr[0] = '+';
		memcpy(&rcpAddr[1], diallingNum, rcpAddr_len);
		rcpAddr[rcpAddr_len + 1] = '\0';
	}
	else {
		memcpy(rcpAddr, diallingNum, rcpAddr_len);
		rcpAddr[rcpAddr_len] = '\0';
	}

	status_report.rcpAddr.DialNumLen = rcpAddr_len;
	memcpy(status_report.rcpAddr.szDiallingNum, rcpAddr, rcpAddr_len);
	status_report.rcpAddr.Npi = rcp_npi;
	status_report.rcpAddr.Ton = rcp_ton;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp((unsigned char *) &pTPDU[position], &SCTimeStamp);
	position += 7;

	/* TP-DT */
	SmsUtilDecodeTimeStamp((unsigned char *) &pTPDU[position], &DischargeTime);
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

			status_report.userData = malloc(sizeof(unsigned char) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

			memset(status_report.userData, 0, sizeof(unsigned char) * (TAPI_NETTEXT_SMDATA_SIZE_MAX + 1));

			if (status_report.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT) {
				TS_BYTE inData[TAPI_NETTEXT_SMDATA_SIZE_MAX + 1];

				memset(inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1);

				memcpy((void*) inData, (void*) &pTPDU[position], status_report.udl);

				SmsUtilUnpackGSMCode((char *) status_report.userData, inData, (TS_UINT8) status_report.udl);
			}
		}
	}

	printf("*************************************\n");
	printf("Msg Type: STATUS_REPORT\n");

	if (status_report.mms)
		printf("MMS: No more messages are waiting for the MS in this SC\n");
	else if (!status_report.mms)
		printf("MMS: More messages are waiting for the MS in this SC\n");

	if (status_report.srq)
		printf("SRQ: the result of an SMS COMMAND \n");
	else if (!status_report.srq)
		printf("SRQ: the result of a SMS SUBMIT.\n");

	if (status_report.udhi)
		printf("UDHI: Contains a Header in addition to the short message.\n");
	else if (!status_report.udhi)
		printf("UDHI: Only Short Message\n");

	printf("STATUS:%x\n", status_report.status);

	if (status_report.status == 0x00)
		printf("STATUS : Short message received by the SME\n");
	else if (status_report.status == 0x01)
		printf("STATUS : Short message forwarded by the SC to the SME\nbut the SC is unable to confirm delivery\n");
	else if (status_report.status == 0x02)
		printf("STATUS : Short message replaced by the SC\n");

	printf("Recipient Number : %s\n", rcpAddr);
	printf("SC Time Stamp : %d-%d-%d  , %d:%d:%d\n", SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute,
			SCTimeStamp.second);
	printf("Discharge Time : %d-%d-%d  , %d:%d:%d\n", DischargeTime.year, DischargeTime.month, DischargeTime.day, DischargeTime.hour, DischargeTime.minute,
			DischargeTime.second);

	if (mask & 0x04)
		printf("Message : %s \n", status_report.userData);

	printf("*************************************\n");

	return TRUE;
}

void MsgCountNotification(int length, char *pData)
{
	static TelSmsStoredMsgCountInfo_t *countInfo;

	printf("length %d", length);
	printf("pdata %s", pData);

	countInfo = (TelSmsStoredMsgCountInfo_t*) pData;
	printf("In MsgCountNotification total cnt is %d, usedcnt is %d\n", countInfo->TotalCount, countInfo->UsedCount);
	printf("In MsgCountNotification total cnt is %d, usedcnt is %d\n", countInfo->TotalCount, countInfo->UsedCount);
}

void ReadMsgNotification(int length, char *pData)
{
	TelSmsData_t * sim_data;
	int scaAddr_len = 0;
	char * pTPDU;
	int tpdu_len = 0;
	TS_BYTE diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	TS_BYTE scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	int sca_ton, sca_npi;
	int position;

	sim_data = (TelSmsData_t *) pData;
	printf("length in readmsg notification is %d\n", length);

	if (sim_data->MsgStatus == 0x00)
		printf("Msg Staus : unknown msg\n");
	else if (sim_data->MsgStatus == 0x01)
		printf("Msg Staus : received unread msg\n");
	else if (sim_data->MsgStatus == 0x02)
		printf("Msg Staus : received read msg\n");
	else if (sim_data->MsgStatus == 0x03)
		printf("Msg Staus : unsent msg\n");
	else if (sim_data->MsgStatus == 0x04)
		printf("Msg Staus : stored sent msg\n");

	if (sim_data->MsgStatus == 0x01 || sim_data->MsgStatus == 0x02) {
		position = 0;
		// SCA_ADDR
		memset(diallingNum, 0, sizeof(diallingNum));

		SmsUtilDecodeAddrField((char *) diallingNum, &sim_data->SmsData.szData[position], &sca_ton, &sca_npi);

		position += 2; //include Address-Length, Type of Address

		scaAddr_len = strlen((char *) diallingNum);

		if (scaAddr_len % 2)
			position += scaAddr_len / 2 + 1;
		else
			position += scaAddr_len / 2;

		if (sca_ton == SIM_TON_INTERNATIONAL) {
			scaAddr[0] = '+';
			memcpy(&scaAddr[1], diallingNum, scaAddr_len);
			scaAddr[scaAddr_len + 1] = '\0';
		}
		else {
			memcpy(scaAddr, diallingNum, scaAddr_len);
			scaAddr[scaAddr_len] = '\0';
		}

		tpdu_len = sim_data->SmsData.MsgLength - position;

		printf("SCA Number : %s tpdu_len is %d\n", scaAddr, tpdu_len);

		pTPDU = malloc(sizeof(TS_BYTE) * tpdu_len);
		printf("bfor memcopy position is %d\n", position);
		memcpy(pTPDU, &(sim_data->SmsData.szData[position]), tpdu_len);
		printf("after memcpy\n");

		DecodeSmsDeliverTpdu(tpdu_len, pTPDU);
	}
}

void MemoryFullNotification(int length, char *pData)
{
	int *memory_ind;

	memory_ind = (int *) pData;
	printf("In memory full notification status %d\n", *memory_ind);
}

void DeleteMsgNotification(int length, char *pData)
{
	tel_delete_sms_in_simMsg_status_ind *delete_ind;

	delete_ind = (tel_delete_sms_in_simMsg_status_ind *) pData;
	printf("In deletenotifcation the result and index are %x %d\n", delete_ind->result, delete_ind->index);
}

void SaveMsgNotification(int length, char *pData)
{
	tapi_nettext_save_status_ind *save_ind;

	save_ind = (tapi_nettext_save_status_ind *) pData;
	printf("In savestatus noti, index %x result %x\n", save_ind->index, save_ind->result);
}

void GetCBNotification(int length, char *pData)
{
	TelSmsCbConfig_t *CBConfig;
	int i = 0;

	CBConfig = (TelSmsCbConfig_t *) pData;

	printf("=========CB Configuration=========\n");

	/*** CB Enable/Diable ***/
	if (CBConfig->bCBEnabled == 0x01)
		printf("Cell Broadcast Msg Enabled...\n");
	else if (CBConfig->bCBEnabled == 0x02)
		printf("Cell Broadcast Msg Disabled...\n");

	/*** Selected ID ***/
	if (CBConfig->SelectedId == 0x01)
		printf("All CBMI is selected... \n");
	else if (CBConfig->SelectedId == 0x02)
		printf("There are some selected CBMI...\n");

	/*** CBMI  Count ***/
	printf("CBMI Count: %d \n\n", CBConfig->MsgIdCount);

	/*** CBMI  List ***/

	if (CBConfig->MsgIdCount != 0) {
		printf("----- CBMI List -----\n");

		for (i = 0; i < CBConfig->MsgIdCount; i++) {
			printf("No.%d - [0x%04x]\n", i, CBConfig->MsgIDs[i]);
		}

	}
	printf("\n==================================\n");
}

void Deliver_ReportNoti()
{
	printf("DELIVERY REPORT SUCCESS\n");
}

void get_preferred_bearer_noti(int length, char *pData)
{
	int svc = 0;
	TelSmsBearerType_t *bearer;

	printf("get preferred bearer\n");
	bearer = (TelSmsBearerType_t *) pData;
	printf("In GetPreferredBearer noti bearer:%d\n", *bearer);
	svc = *bearer;
	printf("svc:%d\n", svc);

	if (svc == TAPI_NETTEXT_BEARER_PS_ONLY)
		printf("Packet Domain\n");
	else if (svc == TAPI_NETTEXT_BEARER_CS_ONLY)
		printf("Circuit Switched\n");
	else if (svc == TAPI_NETTEXT_BEARER_PS_PREFERRED)
		printf("Packet Domain preferred \n");
	else if (svc == TAPI_NETTEXT_BEARER_CS_PREFERRED)
		printf("Circuit Switched preferred\n");

}

void set_request_confirm_noti()
{
	printf("***Set Request  confirm Notification*******\n");
	printf("Notification Received Successfully\n");
}

void param_get_noti(int length, char *pData)
{
	TelSmsParams_t *smsp_param;

	smsp_param = (TelSmsParams_t *) pData;

	printf("record index is %x\n", smsp_param->RecordIndex);
	printf("record len  is %x\n", smsp_param->RecordLen);
	printf("alpha_id len  is %lu \n", smsp_param->AlphaIdLen);
	printf("alpha_id is %s \n", smsp_param->szAlphaId);
	printf("param indicator is  %xn", smsp_param->ParamIndicator);
	printf("dest dialling num is %s\n", smsp_param->TpDestAddr.szDiallingNum);
	printf("svcaddr dialling num is %s\n", smsp_param->TpSvcCntrAddr.szDiallingNum);
	printf("pid %x\n", smsp_param->TpProtocolId);
	printf("dcs is %x\n", smsp_param->TpDataCodingScheme);
	printf("validity is %x\n", smsp_param->TpValidityPeriod);
}

void Device_ReadyNoti(int length, char *pData)
{
	//TelSmsDeviceReadyInd *device_status;

	//device_status = (TelSmsDeviceReadyInd *)pData;

	//printf("In device ready noti the result is %d\n",device_status->Result);
}

void param_count_noti(int length, char *pData)
{
	printf("In param_count_noti the record count:");
}

int tapi_sms_callback(const TelTapiEvent_t *TapiEvent, void *data)
{
	printf("\n******  Inside SMS Callback func........   ******\n");

	int eventClass;
	int eventType;
	int requestId;
	int status;
	char *EventData = NULL;
	int param1 = 0;
	char *pData = NULL;

	int expected_event = (int) data;

	if (TapiEvent == NULL) {
		printf("Event is NULL\n");
		return 0;
	}
	
	eventClass = TapiEvent->EventClass;
	eventType = TapiEvent->EventType;
	requestId = TapiEvent->RequestId;
	status = TapiEvent->Status;
	EventData = (char *) TapiEvent->pData;

	if (EventData == NULL) {
		printf("\n******  EventData is NULL.........******\n");
	}
	else {
		printf("\nClass:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType, requestId, status);

		param1 = strlen((char *) TapiEvent->pData);
		printf("\n param1 = %d", param1);
		if (TapiEvent->pData != NULL) {
			pData = (char *) TapiEvent->pData;
			printf("pData(size):%d\n", strlen(pData));
		}
	}

	if (eventClass != TAPI_EVENT_CLASS_NETTEXT) {
		return 0;
	}

	switch (eventType) {
		case TAPI_EVENT_NETTEXT_INCOM_IND:
			printf("\n***************************************\n");
			printf("\n$$$$$$ INCOMING MESSAGE NOTIFICATION $$$$$$ \n");
			printf("param1(length):%d\n", param1);
			//	 					ReceiveMessage(param1,pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_SENTSTATUS_CNF:
			printf("\n***************************************\n");
			printf("\n$$$$$$ RECEIVE  ACK NOTIFICATION $$$$$$ \n");
			printf("param1(length):%d\n", param1);
			DecodeSmsSubmitReportTpdu(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_CB_INCOM_IND:
			printf("\n***************************************\n");
			printf("\n$$$$$$ INCOMING CB NOTIFICATION $$$$$$ \n");
			printf("length:%d\n", param1);
			DecodeCellBroadcastMsg(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND:
			printf("\n***************************************\n");
			printf("\n$$$$$$ MEMORYF  FULL NOTIFICATION $$$$$$ \n");
			printf("param1(length):%d\n", param1);
			MemoryFullNotification(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_GET_COUNT_CNF:
			printf("\n***************************************\n");
			printf("\n$$$GetMsgCountNotification$$$$\n");
			MsgCountNotification(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_READ_SMS_CNF:
			printf("\n***************************************\n");
			printf("\n$$Read Msg Notification $$$\n");
			printf("param1(length):%d\n", param1);
			//ReadMsgNotification(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF:
			printf("\n***************************************\n");
			printf("\n$$Delete Notification$$$\n");
			printf("param1(length):%d\n", param1);
			DeleteMsgNotification(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF:
			printf("\n***************************************\n");
			printf("\n$$Save Status Notification$$\n");
			printf("param1(length):%d\n", param1);
			SaveMsgNotification(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF:
			printf("\n***************************************\n");
			printf("\n$$$GET CB Config$$$\n");
			printf("param1(length):%d\n", param1);
			GetCBNotification(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ DELIVER REPORT NOTI$$$\n");
			Deliver_ReportNoti();
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_GET_PARAM_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ GET PARAM NOTI$$$\n");
			printf("param1(length):%d\n", param1);
			param_get_noti(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF:
			printf("\n****************************************\n");
			printf("\n$$$ GET PREFERRED BEARER$$$\n");
			printf("param1(length):%d\n", param1);
			get_preferred_bearer_noti(param1, pData);
			printf("\n****************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ SET REQUESTCONFIRM NOTI$$$\n");
			set_request_confirm_noti();
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_DEVICE_READY_IND:
			printf("\n***************************************\n");
			printf("\n$$$ DEVICE READY NOTI$$$\n");
			Device_ReadyNoti(param1, pData);
			printf("\n***************************************\n");
			break;

		case TAPI_EVENT_NETTEXT_PARAM_COUNT_IND:
			printf("\n***************************************\n");
			printf("\n$$$ PARAM COUNT NOTI$$$\n");
			param_count_noti(param1, pData);
			printf("\n***************************************\n");
			break;

		default:
			printf("\n****************DEFAULT***************\n");
			break;
	}

	if (eventType == expected_event)
		TimeOutFun(NULL);

	return 0;
}//end callback


