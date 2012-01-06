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
#include <glib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <glib.h>

#include "TelUtility.h"
#include <TapiCommon.h>
#include <TelNetText.h>
#include <ITapiNetText.h>

#include "tapi_test_utility.h"
#include "tapi_sms_test.h"

/******* prototype ******* */
#include "tapi_sms_util.h"

#define	TAPI_NETTEXT_MSG_SIZE_MAX				918
#define	TAPI_NETTEXT_CB_SIZE_MAX				93
#define	TAPI_NETTEXT_ADDRESS_LEN_MAX			20
#define	TAPI_NETTEXT_SCADDRESS_LEN_MAX			18

#define TEST_DEBUG(frmt, args...)	\
{do { fprintf(stderr, "[SMS Test][%s:%04d] "frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}

// Global Variable
static unsigned int *	SMS_subscription_id;
int					dcs_pos =0;
unsigned int			SmsMsgId = 0;

// same as tapievent
typedef struct
{
	int group;
	int type;
	int requestid;
	int status;
	void *dst;
}data_t;

//extern int EvtDeliveryHandle ;
//int error_code;

TS_UINT8 tp_mr = 0x00;


BOOL EncodeCdmaSmsParamTeleservice(int teleservice_id ,ParamTeleserv_t *ParamTeleserv)
{
	BOOL ret = FALSE;

	if(ParamTeleserv == NULL){
		printf("[EncodeCdmaSmsParamTeleservice] NULL pointer  \n");
		return ret;
	}

	switch(teleservice_id){
		case TELESERVICEID_CPT95:
		case TELESERVICEID_CMT95:
		case TELESERVICEID_VMN95:
		case TELESERVICEID_WAP:
			ret = TRUE;
			break;
		default:
			ret = FALSE;
	}

	if(ret == TRUE){
		ParamTeleserv->param_id = PARAMID_TELESERVICE_ID;
		ParamTeleserv->param_len = 2;
		ParamTeleserv->teleserv_id = teleservice_id;
	}

	return ret;
}

BOOL EncodeCdmaSmsParamDestAddress(int NumberPlan, char* diallingNum ,ParamAddress_t *ParamAddr)
{
	BOOL ret = FALSE;
	int length = 0;

	if(ParamAddr == NULL || diallingNum == NULL){
		printf("[EncodeCdmaSmsParamAddress] NULL pointer  \n");
		return ret;
	}

	length = strlen(diallingNum); //remove CR

	ParamAddr->param_id = PARAMID_ADDRESS;
	ParamAddr->len = length;
	ParamAddr->digit = DIGITMODE_4BITDTMF;
	ParamAddr->num = NUMMODE_NONE_DATANETWORK;
	ParamAddr->numtype = NUMBER_TYPE_UNKNOWN;
	ParamAddr->numplan = NumberPlan;

	strncpy((char *)ParamAddr->CHARi, diallingNum, length);

	length +=5;
	ParamAddr->param_len = length;

	ret = TRUE;

	return ret;
}

BOOL EncodeCdmaSmsParamCallbackAddress(int NumberPlan, char* diallingNum ,ParamCallback_t *ParamCallback)
{
	BOOL ret = FALSE;
	int length = 0;

	if(ParamCallback == NULL || diallingNum == NULL){
		printf("[EncodeCdmaSmsParamAddress] NULL pointer  \n");
		return ret;
	}

	length = strlen(diallingNum); //remove CR


	ParamCallback->param_id = PARAMID_CALLBACK;
	ParamCallback->digit = DIGITMODE_4BITDTMF;
	ParamCallback->numtype = NUMBER_TYPE_UNKNOWN;
	ParamCallback->numplan = NumberPlan;
	ParamCallback->len = length;
	strncpy((char *)ParamCallback->CHARi, diallingNum, length);

	length +=4;
	ParamCallback->param_len = length;

	ret = TRUE;

	return ret;
}

BOOL EncodeCdmaSmsParamMsgId(int type, ParamMsgId_t *ParamMsg)
{
	BOOL ret = FALSE;

	if(ParamMsg == NULL){
		printf("[EncodeCdmaSmsParamMsgId] NULL pointer  \n");
		return ret;
	}

	ParamMsg->param_id = PARAMID_MESSAGE_ID;
	ParamMsg->param_len =3;
	ParamMsg->type = type;
	ParamMsg->msg_id = SmsMsgId++;


	ret = TRUE;

	return ret;
}

BOOL EncodeCdmaSmsParamUserData(char* SendTxt, ParamUserData_t *ParamUser)
{
	BOOL ret = FALSE;
	int length = 0;

	if(ParamUser == NULL || SendTxt == NULL){
		printf("[EncodeCdmaSmsParamUserData] NULL pointer  \n");
		return ret;
	}

	length = strlen(SendTxt);

	ParamUser->param_id = PARAMID_USER_DATA;
	ParamUser->encoding = MSGENCODING_7BIT_ASCII;
	strncpy((char *)ParamUser->CHARi, SendTxt, length);

	ParamUser->len =  length;
	length += 2;;
	ParamUser->param_len = length;

	ret = TRUE;

	return ret;
}

BOOL EncodeCdmaSmsSubmitTpdu(char* diallingNum, int dialNumLen, char* msgTxt, int msg_len)
{
	ParamTeleserv_t ParamTeleserv = {0, };
	ParamAddress_t ParamAddr = {0, };
	//ParamCallback_t Paramcallback = {0, };
	ParamMsgId_t ParamMsg = {0, };
	ParamUserData_t ParamUser = {0, };

	int i;
	int requestId = 0;
	//char CallbackNum[] = "1117908499";
	TapiResult_t returnStatus;

	TEST_DEBUG("EncodeCdmaSmsSubmitTpdu Func Enter");

	TelSmsMsgInfo_t *pDataPackage = malloc(sizeof(TelSmsMsgInfo_t));
	memset(pDataPackage, 0, sizeof(TelSmsMsgInfo_t));

	pDataPackage->MsgType = TAPI_NETTEXT_MESSAGETYPE_SUBMIT;

	//1 TeleService Identifier
	EncodeCdmaSmsParamTeleservice(TELESERVICEID_CMT95, &ParamTeleserv);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_TELESERVICE_MASK;
	memcpy(&pDataPackage->MsgData.OutSubmit.TeleService, &ParamTeleserv.teleserv_id, sizeof(unsigned short));
	TEST_DEBUG("teleservice at TEST APP: 0x%x", pDataPackage->MsgData.OutSubmit.TeleService);

	//1 Destination Address
	diallingNum[dialNumLen] = '\0';
	EncodeCdmaSmsParamDestAddress(NUMBER_PLAN_UNKNOWN, diallingNum, &ParamAddr);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_ADDRESS_MASK;
	pDataPackage->MsgData.OutSubmit.DstAddr.Digit = ParamAddr.digit;
	pDataPackage->MsgData.OutSubmit.DstAddr.NumberMode = ParamAddr.num;
	pDataPackage->MsgData.OutSubmit.DstAddr.NumberType = ParamAddr.numtype;
	pDataPackage->MsgData.OutSubmit.DstAddr.NumberPlan = ParamAddr.numplan;
	pDataPackage->MsgData.OutSubmit.DstAddr.szAddrLength = ParamAddr.len;
	memcpy(pDataPackage->MsgData.OutSubmit.DstAddr.szAddress, ParamAddr.CHARi, ParamAddr.len);

	//1  Message Identifier
	EncodeCdmaSmsParamMsgId(MESSAGETYPE_SUBMIT, &ParamMsg);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_MESSAGE_ID_MASK;
	pDataPackage->MsgData.OutSubmit.MsgId = ParamMsg.type;
	pDataPackage->MsgType = ParamMsg.type;

	//1  bearer
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_BEARER_REPLY_MASK;
	pDataPackage->MsgData.OutSubmit.ReplySeqNumber = 1;

#if 0
	//1 Callback Address
	EncodeCdmaSmsParamCallbackAddress(NUMBER_PLAN_UNKNOWN, CallbackNum, &Paramcallback);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_CALLBACK_MASK;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.Digit = Paramcallback.digit;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.NumberType = Paramcallback.numtype;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.NumberPlan = Paramcallback.numplan;
	pDataPackage->MsgData.OutSubmit.CallBackNumber.szAddrLength = Paramcallback.len;
	memcpy(pDataPackage->MsgData.OutSubmit.CallBackNumber.szAddress, Paramcallback.CHARi, Paramcallback.len);
#endif
	//1 User Date
	msgTxt[msg_len - 1] = '\0';
	EncodeCdmaSmsParamUserData(msgTxt, &ParamUser);
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_USER_DATA_MASK;
	pDataPackage->MsgData.OutSubmit.MsgEncoding = ParamUser.encoding;
	pDataPackage->MsgData.OutSubmit.MsgLength = ParamUser.len;
	memcpy(pDataPackage->MsgData.OutSubmit.szData, ParamUser.CHARi, ParamUser.len);

	TEST_DEBUG("");
	TEST_DEBUG("=============== Test App Message =================");
	TEST_DEBUG(" - MsgType: %d", pDataPackage->MsgType);
	TEST_DEBUG(" - ParamMask: 0x%08x", pDataPackage->ParamMask);
	printf("0000: ");
	for (i = 0; i < sizeof(TelSmsMsgInfo_t); i++) {
		printf("%02x ", *(((unsigned char *) pDataPackage) + i));
		if ((i + 1) % 8 == 0)
			printf("  ");

		if ((i + 1) % 16 == 0)
			printf("\n%04x: ", i+1);
	}
	printf("\n");
	fflush(stdout);

	TEST_DEBUG("==============================================");

	returnStatus = tel_send_sms_msg(pDataPackage, 0x26, &requestId); //more to send is false since it is not concatenated.for concatenated send 1

	TEST_DEBUG("tel_send_sms_msg() returnStatus: %d, requestId: %d", returnStatus, requestId);

	free(pDataPackage);

	return TRUE;
}


BOOL DecodeCellBroadcastMsg(int length,char * pPDU)
{
	int			msgLength;
	int			sn = 0; //serial number
	int			msgID; //message identifier
	tapi_nettext_coding_scheme  dcs;

	TS_UINT8	page[2];
	int			contents_length;
	char	 contents[TAPI_NETTEXT_CB_SIZE_MAX];

	TS_UINT8	gs;
	TS_UINT8	msgCode;
	TS_UINT8	updateNumber;

	if(pPDU == NULL){
		TEST_DEBUG("pData is Null")
		return FALSE;
	}

	TEST_DEBUG("cb msg type is %x",pPDU[0]);
	TEST_DEBUG("cb msg len is %x",pPDU[1]);


	TEST_DEBUG("$$$$$$$$$$$$$$$length of cb msg is %d",length);
	sn = (pPDU[2] <<8 ) |pPDU[1] ;

	gs = (pPDU[2] & 0xC0) >> 6;  //bit 7, bit 6

	msgCode = (( (pPDU[2] & 0x3F) <<4 ) | pPDU[3] ) & 0xF0 ;

	updateNumber = pPDU[3] & 0x0F;  //bit3, bit2, bit1, bit0

	msgID = pPDU[4] << 4 |pPDU[3];

	SmsUtilDecodeDCS( &dcs, pPDU[6] );

	page[0] = pPDU[7] & 0x0F; //total page
	page[1] = (pPDU[7] & 0xF0)>>4 ; //current page


	msgLength = length -2;

	contents_length = msgLength -6;

	if (dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		TS_BYTE	inData[TAPI_NETTEXT_CB_SIZE_MAX+1];

		memset( inData, 0x00, TAPI_NETTEXT_CB_SIZE_MAX + 1 );

		memcpy( (void*) inData, (void*) &pPDU[8], contents_length);
		SmsUtilUnpackGSMCode(contents,  &pPDU[8], msgLength );
	}
	else
	{
		memcpy( contents, &pPDU[8], msgLength);
		contents[msgLength] = 0;
	}

	printf("*************************************\n");
	printf("serical number : 0x%04x \n" , sn);
	printf("Geographical Scope: 0x%x\n" , gs);
	printf("message code : 0x%x\n", msgCode);
	printf("update Number : 0x%x\n",updateNumber);
	printf("message identifier :  0x%x\n" , msgID);
	printf("DCS-Commpressed?: %d (1: true)\n" , dcs.bCompressed);
	printf("DCS-coding_group_type:  %x\n" , dcs.coding_group_type);
	printf("contents bytes size : %d\n", contents_length);
	printf("page : (%d /%d) (page /total ) \n", page[1], page[0] );
	printf("contents: %s\n", contents);
	printf("*************************************\n");
	/*
	   if(dcs.class_type = TAPI_NETTEXT_CLASS_2)
	   {
	   TelSmsDatapackageInfo_t * pDataPackage;

	   pDataPackage = malloc(sizeof(TelSmsDatapackageInfo_t));

	   pDataPackage->MsgLength =length ;
	   memcpy(pDataPackage->szData, pPDU, pDataPackage->MsgLength);

	   returnStatus = TelTapiSmsWrite(0x01, pDataPackage,&requestId);
	   TEST_DEBUG("Write : returnstatus %d requestId :%d",returnStatus,requestId);
	   }
	   */
	return TRUE;
}

BOOL EncodeSmsSubmitTpdu(char* diallingNum, int dialNumLen, char* msg, int msg_len)
{

	TPDU_SMS_SUBMIT tpdu_submit;

	TS_BYTE  packet[TAPI_NETTEXT_MSG_SIZE_MAX];

	TS_BYTE  SCA[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //send SMS �Ҷ� ����ϴ� ���� SCA��
	int ret = 0;

	//	TS_UINT8 tp_mr = 0x03;
	tp_mr++;
	TS_UINT8 tp_pid = 0; //reaplace type (65)
	TS_UINT8 tp_dcs; //=17; //class_1(17),class_2(18)
	TS_UINT8 tp_vp;


	int index = 0, ScIndex = 0;
	int i;
	int requestId=0;
	unsigned char             sca_len =0;
	TapiResult_t returnStatus ;

	TelSmsDatapackageInfo_t *pDataPackage= malloc(sizeof(TelSmsDatapackageInfo_t));

	memset(pDataPackage, 0, sizeof(TelSmsDatapackageInfo_t));

	memset(packet, 0, TAPI_NETTEXT_MSG_SIZE_MAX);

	/************SCA ADDRESS GET START**************/
	/*
	   returnStatus = TelTapiSmsGetSCA(&sc_addr,0);
	   printf("Service center addr returned from TAPI SCA address %s returnStatus %d\n",sc_addr.szDiallingNum,returnStatus);
	   SmsUtilDecodeAddrField(decodeScaAddr, &sc_addr.szDiallingNum[0],&sc_addr.Ton, &sc_addr.Npi);
	   sca_len = strlen( decodeScaAddr );

	   memcpy( SCA, decodeScaAddr, sca_len);
	   SCA[sca_len] = '\0';
	   */
	TEST_DEBUG("Enter SCA: ");
	memset(SCA,0,sizeof(SCA));

	ret = read(0, SCA, sizeof(SCA));

	if (ret <= 0) {
		TEST_DEBUG(" NULL msg can NOT be sent ");
		return -1;
	}

	sca_len=strlen((char *)SCA)-1;

	TEST_DEBUG("Decode SCA Address =========");
	TEST_DEBUG("SCA Number :%s, SCA Length :%d ",SCA , sca_len);
	TEST_DEBUG("========================");

	//SCA ADDRESS GET END

	//SCA

	//TODO : SCA

	//	index +=SmsUtilEncodeAddrField(packet,SCA, strlen(SCA)  ,0x01,0x01);
	ScIndex  =SmsUtilEncodeAddrField((unsigned char*)pDataPackage->Sca ,(char *)SCA, sca_len  ,0x01,0x01);


	TEST_DEBUG("ScIndex is %d", ScIndex);

	// TP-MTI, TP-RD, TP-VPF, TP-SRR, TP_UDHI, TP-RP
	tpdu_submit.msg_type = SMS_TPDU_SUBMIT;
	tpdu_submit.rd   =FALSE; //false :accept duplicate msg , true: reject duplicate msg
	tpdu_submit.vpf = TAPI_NETTEXT_VP_RELATIVE;
	tpdu_submit.srr  = FALSE;   //false :A status report is not requested , true: A status report is requested
	tpdu_submit.udhi = FALSE ; //false: contains only the short message , true :a Header in addition to the short message
	tpdu_submit.rp = FALSE  ; //false :TP Reply Path parameter is not set in this SMS SUBMIT, true : TP Reply Path parameter is set in this SMS SUBMIT

	packet[index] =  tpdu_submit.msg_type;							// SUBMIT: bits 0, 1
	packet[index] |= tpdu_submit.rd? 0x04 : 0;						// bit 2
	packet[index] |= tpdu_submit.vpf  << 3;	// bits 3, 4
	packet[index] |= tpdu_submit.srr ? 0x20 : 0;						// bit 5
	packet[index] |= tpdu_submit.udhi? 0x40 : 0;						// bit 6
	packet[index] |= tpdu_submit.rp ? 0x80 : 0;						// bit 7

	index++;
	TEST_DEBUG("index of message ref is %d, msgref is %d",index,tp_mr);
	/* TP-MR */
	packet[index++] = tp_mr;

	// TP-DA
	index +=  SmsUtilEncodeAddrField( packet+index, diallingNum, dialNumLen, 0x02, 0x01);

	//TEST_DEBUG("DESTINATTION  ADDR Encoded =========");

	//TP-PID
	packet[index++] = tp_pid;

	// TP_DCS (Data Coding Scheme)

	tpdu_submit.dcs.bCompressed = FALSE;
	tpdu_submit.dcs.bmsg_class_set = TRUE;
	tpdu_submit.dcs.coding_group_type =TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;
	tpdu_submit.dcs.alphabet_type = TAPI_NETTEXT_ALPHABET_DEFAULT;
	tpdu_submit.dcs.class_type = TAPI_NETTEXT_CLASS_2;

	SmsUtilEncodeDCS(&tp_dcs, &tpdu_submit.dcs);

	packet[index++] = tp_dcs;

	//TEST_DEBUG("DCS Encoded:tp_dcs =%d =========",tp_dcs);

	//TP-VP

	tpdu_submit.vp.vp_rel_type = TAPI_NETTEXT_VP_REL_6H ;
	tpdu_submit.vp.vp_type = TAPI_NETTEXT_VP_RELATIVE;
	tpdu_submit.vp.vpValue =0xff ;

	SmsUtilEncodeValidity(&tp_vp,&tpdu_submit.vp);

	//tp_vp =65;
	packet[index++] = tp_vp;
	//TEST_DEBUG("Validity Encoded :tp_vp =%d =========",tp_vp);

	// UDL
	packet[index++] = msg_len;

	// TP-UD

	if (tpdu_submit.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		int pos;
		pos =SmsUtilPackGSMCode( packet + index, msg, msg_len);
		index += pos;
	}

	TEST_DEBUG("Send packet to phonseSever[%d] -----", index);

	for(i=0; i< index ; i++ )
	{
		TEST_DEBUG("[%02x]", packet[i]);
		if( i%10 == 9)
			TEST_DEBUG("\n");
	}
	TEST_DEBUG("\n");

	memcpy( pDataPackage->szData, packet, index);
	pDataPackage->MsgLength = index;

	TEST_DEBUG("***Sending the Message (Api:SendMsg)***");

	TEST_DEBUG("Length : %d",pDataPackage->MsgLength);

	returnStatus = tel_send_sms( pDataPackage, 0 ,&requestId); //more to send is false since it is not concatenated.for concatenated send 1

	TEST_DEBUG("SendMsg : returnStatus=%d requestId=%d", returnStatus, requestId);

	free(pDataPackage);

	return TRUE;

}
TS_BOOL DecodeSmsSubmitTpdu(int pdu_len, char * pPDU)
{
	TPDU_SMS_SUBMIT		tpdu_submit;
	TS_BYTE              scaAddr_len =0;
	int			    destAddr_len = 0;
	char		diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	char     scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	char	    destAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //destination number
	int	    sca_ton, sca_npi, dest_ton,dest_npi;
	int	    position;

	position=0;

	/* SCA_ADDR */
	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pPDU[position],&sca_ton, &sca_npi);

	position+=2;  //include Address-Length, Type of Address

	scaAddr_len = strlen( (char *)diallingNum );

	if ( scaAddr_len % 2 )
		position += scaAddr_len / 2 + 1;
	else
		position += scaAddr_len / 2;


	if ( sca_ton == SIM_TON_INTERNATIONAL )
	{
		scaAddr[0] = '+';
		memcpy( &scaAddr[1], diallingNum, scaAddr_len );
		scaAddr[scaAddr_len+1] = '\0';
	}
	else
	{
		memcpy( scaAddr, diallingNum, scaAddr_len );
		scaAddr[scaAddr_len] = '\0';
	}

	//TEST_DEBUG(" sca length: %d , sca # : %s", scaAddr_len, scaAddr);

	/* TP-MTI, TP-RD, TP-VPF,  TP-RP, TP_UDHI, TP-SRR */

	tpdu_submit.msg_type = SMS_TPDU_SUBMIT; // bit 0,1(Message Type Indicator )
	tpdu_submit.rd = ( pPDU[position] & 0x04 ) ? TRUE : FALSE;// bit 2 (Message Type Indicator)
	tpdu_submit.vpf =( pPDU[position] & 0x18) >> 3; //  bits 3, 4 ( validity period format)
	tpdu_submit.srr=( pPDU[position] & 0x20 ) ? TRUE : FALSE;	// bit 5 (Status Report Request)
	tpdu_submit.udhi=( pPDU[position] & 0x40 ) ? TRUE : FALSE;	// bit 6 (User Data Header Indicator)
	tpdu_submit.rp = ( pPDU[position] & 0x80 ) ? TRUE : FALSE;	// bit 7 (Reply Path)

	position++ ;

	/* TP-MR */
	tpdu_submit.mr = pPDU[position];

	position++ ;

	/* TP-DA */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pPDU[position],&dest_ton, &dest_npi);

	position+=2;

	destAddr_len = strlen( (char*) diallingNum );
	if ( destAddr_len % 2 )
		position += destAddr_len / 2 + 1;
	else
		position += destAddr_len / 2;


	if ( dest_ton == SIM_TON_INTERNATIONAL )
	{
		destAddr[0] = '+';
		memcpy( &destAddr[1], diallingNum, destAddr_len );
		destAddr[destAddr_len+1] = '\0';
	}
	else
	{
		memcpy( destAddr, diallingNum, destAddr_len );
		destAddr[destAddr_len] = '\0';
	}
	//TEST_DEBUG(" dest length: %d , dest # : %s", destAddr_len, destAddr);

	tpdu_submit.desAddr.DialNumLen= destAddr_len;
	memcpy(tpdu_submit.desAddr.szDiallingNum, destAddr , destAddr_len);
	tpdu_submit.desAddr.Npi = dest_npi;
	tpdu_submit.desAddr.Ton = dest_ton;

	/* TP-PID */

	tpdu_submit.pId = pPDU[position];
	position++ ;

	/* TP DCS */
	SmsUtilDecodeDCS( &tpdu_submit.dcs, pPDU[position] );
	position ++;

	/* TP VP */
	tpdu_submit.vp.vp_type = TAPI_NETTEXT_VP_RELATIVE;  //temp

	if(tpdu_submit.vp.vp_type ==  TAPI_NETTEXT_VP_RELATIVE)
	{
		tpdu_submit.vp.vpValue = pPDU[position];
		tpdu_submit.vp.vp_rel_type = TAPI_NETTEXT_VP_REL_1D;
		position++;
	}
	else if (tpdu_submit.vp.vp_type ==  TAPI_NETTEXT_VP_ABSOLUTE || tpdu_submit.vp.vp_type ==  TAPI_NETTEXT_VP_ENHANCED)
	{
		position += 7;
	}

	/* TP UDL */
	tpdu_submit.udl = pPDU[position] ;

	if ( tpdu_submit.udl> TAPI_NETTEXT_SMDATA_SIZE_MAX )
		tpdu_submit.udl = TAPI_NETTEXT_SMDATA_SIZE_MAX;

	position++;

	/* TP UD */
	tpdu_submit.userData = malloc( sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

	memset(tpdu_submit.userData, 0, sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

	if (tpdu_submit.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		char	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

		memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

		memcpy( (void*) inData, (void*) &pPDU[position], tpdu_submit.udl );

		SmsUtilUnpackGSMCode((char *)tpdu_submit.userData, inData, (TS_UINT8) tpdu_submit.udl);
	}

	TEST_DEBUG("*************************************");
	TEST_DEBUG("Msg Type: TPDU_SUBMIT");

	if(tpdu_submit.rd)
	{
		TEST_DEBUG("RD: Reject Duplicate")
	}

	else if(!tpdu_submit.rd)
	{
		TEST_DEBUG("RD: Accept Duplicate")
	}

	if(tpdu_submit.srr)
	{
		TEST_DEBUG("SRR: A status report is requested")
	}

	else if(!tpdu_submit.srr)
	{
		TEST_DEBUG("SRR: A status report is not  requested")
	}

	if(tpdu_submit.udhi)
	{
		TEST_DEBUG("UDHI: Contains a Header in addition to the short message.")
	}
	else if(!tpdu_submit.udhi)
	{
		TEST_DEBUG("UDHI: Only Short Message")
	}

	if(tpdu_submit.rp)
	{
		TEST_DEBUG("RP: Set Reply Path")
	}

	else if(!tpdu_submit.rp)
	{
		TEST_DEBUG("RP: Not set Reply Path")
	}

	TEST_DEBUG("SCA Addr : %s",scaAddr);
	TEST_DEBUG("To : %s" ,destAddr);
	TEST_DEBUG("Message : %s " , tpdu_submit.userData);
	TEST_DEBUG("*************************************");

	return TRUE;
}

int   DecodeSmsDeliverTpdu(int tpdu_len , char* pTPDU)
{
	TPDU_SMS_DELIVER   tpdu_deliver;
	TmDateTime		   SCTimeStamp;;
	int		    orgAddr_len = 0;
	char	    diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	char		    orgAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //originating number
	int		    org_ton,org_npi;
	int		    position=0;

	/* TP-MTI, TP-MMS, TP-SRI, TP_UDHI, TP-RP */
	tpdu_deliver.msg_type = SMS_TPDU_DELIVER;
	tpdu_deliver.mms = ( pTPDU[position] & 0x04 ) ? TRUE : FALSE;// bit 2 (Message Type Indicator)
	tpdu_deliver.sri =( pTPDU[position] & 0x20 ) ? TRUE : FALSE;	// bit 5 (Status Report Indication)
	tpdu_deliver.udhi=( pTPDU[position] & 0x40 ) ? TRUE : FALSE;	// bit 6 (User Data Header Indicator)
	tpdu_deliver.rp = ( pTPDU[position] & 0x80 ) ? TRUE : FALSE;	// bit 7 (Reply Path)

	position++ ;

	/* TP-OA */

	memset(diallingNum, 0, sizeof(diallingNum));

	orgAddr_len = SmsUtilDecodeAddrField(diallingNum, &pTPDU[position],&org_ton, &org_npi);

	position+=2;

	if ( orgAddr_len % 2 )
		position += orgAddr_len / 2 + 1;
	else
		position += orgAddr_len / 2;

	TEST_DEBUG("pos = %d, length = %d",position, orgAddr_len);

	if ( org_ton == SIM_TON_INTERNATIONAL )
	{
		orgAddr[0] = '+';
		memcpy( &orgAddr[1], diallingNum, orgAddr_len );
		orgAddr[orgAddr_len+1] = '\0';
	}
	else
	{
		memcpy( orgAddr, diallingNum, orgAddr_len );
		orgAddr[orgAddr_len] = '\0';
	}

	/* TP-PID */
	tpdu_deliver.pId= pTPDU[position];

	position ++;

	/* TP-DCS */
	SmsUtilDecodeDCS( &tpdu_deliver.dcs, pTPDU[position] );
	dcs_pos = position;
	position++;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp( &pTPDU[position], & SCTimeStamp );

	position+=7;

	/* TP-UDL */
	tpdu_deliver.udl = pTPDU[position];

	if ( tpdu_deliver.udl> TAPI_NETTEXT_SMDATA_SIZE_MAX )
		tpdu_deliver.udl = TAPI_NETTEXT_SMDATA_SIZE_MAX;

	position++;

	/* TP-UD */
	tpdu_deliver.userData = malloc( sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

	memset(tpdu_deliver.userData, 0, sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

	if (tpdu_deliver.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
	{
		char	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

		memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

		memcpy( (void*) inData, (void*) &pTPDU[position], tpdu_deliver.udl );

		SmsUtilUnpackGSMCode(tpdu_deliver.userData, inData, (TS_UINT8) tpdu_deliver.udl);
	}

	TEST_DEBUG("*************************************");
	TEST_DEBUG("Msg Type: TPDU_DELIVER");

	if(tpdu_deliver.rp )
	{TEST_DEBUG("RP: Set Reply Path")}
	else if(!tpdu_deliver.rp )
	{TEST_DEBUG("RP: Not set Reply Path")}

	if(tpdu_deliver.sri)
	{TEST_DEBUG("SRI: A status report shall be returned to the SME")}
	else if(!tpdu_deliver.sri)
	{TEST_DEBUG("SRI: A status report shall be not returned to the SME")}

	if(tpdu_deliver.mms)
	{TEST_DEBUG("MMS: No more messages are waiting for the MS in this SC")}
	else if(!tpdu_deliver.mms)
	{TEST_DEBUG("MMS: More messages are waiting for the MS in this SC")}

	if(tpdu_deliver.udhi)
	{TEST_DEBUG("UDHI: Contains a Header in addition to the short message.")}
	else	if(!tpdu_deliver.udhi)
	{TEST_DEBUG("UDHI: Only short message.")}

									TEST_DEBUG("DcsClass : %x (4 means Class None) ",tpdu_deliver.dcs.class_type);


	TEST_DEBUG("From : %10s" ,orgAddr );
	TEST_DEBUG("Time : %d-%d-%d  , %d:%d:%d",
			SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute ,SCTimeStamp.second);
	TEST_DEBUG("Message : %s " , tpdu_deliver.userData);
	TEST_DEBUG("*************************************");


	return dcs_pos;
}
BOOL EncodeSmsDeliverReportTpdu()
{
	TPDU_SMS_DELIVER_REPORT deliver_report;
	unsigned char packet[500];
	TelSmsDatapackageInfo_t * pDataPackage;
	TS_BYTE tp_dcs;
	TS_BYTE mask=0x69;
	int index=0;
	int i ;

	TelSmsDatapackageInfo_t  del_report;


	memset(&(del_report), 0 , sizeof(TelSmsDatapackageInfo_t));

	pDataPackage = malloc(sizeof(TelSmsDatapackageInfo_t));
	memset(pDataPackage, 0, sizeof(TelSmsDatapackageInfo_t));

	memset(packet, 0, 500);


	/* TP-MTI , TP-UDHI */
	deliver_report.msg_type = SMS_TPDU_DELIVER_REPORT;
	deliver_report.udhi = FALSE;

	packet[index] = deliver_report.msg_type;						//DELIVER REPORT
	packet[index] |= deliver_report.udhi? 0x40 : 0;	//bit 6

	index ++;

	/* TP-FCS */
	//if(result==FAIL)
	//{


	//}


	/* TP -PI */
	deliver_report.paraInd = mask;
	packet[index] = deliver_report.paraInd ;

	if ( ( mask != 0xFF ) && ( mask != 0 ) )
	{
		index++;
		/* TP-PID */
		if ( mask & 0x01)
		{
			deliver_report.pId = 0x65 ;
			packet[index++] = deliver_report.pId;
		}

		/* TP-DCS */
		if ( mask & 0x02)
		{
			deliver_report.dcs.bCompressed = FALSE;
			deliver_report.dcs.bmsg_class_set = TRUE;
			deliver_report.dcs.coding_group_type =TAPI_NETTEXT_CODGRP_SM_GENERAL_DCS;
			deliver_report.dcs.alphabet_type = TAPI_NETTEXT_ALPHABET_DEFAULT;
			deliver_report.dcs.class_type = TAPI_NETTEXT_CLASS_2;

			SmsUtilEncodeDCS(&tp_dcs, &deliver_report.dcs);
			packet[index++] = tp_dcs;
		}
		/* TP-UDL */
		if ( mask & 0x04)
		{
			deliver_report.udl =0x00;
			packet[index++] = deliver_report.udl;

			/* TP-UD */

			if (deliver_report.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
			{
				//SmsUtilPackGSMCode( packet + index, msg, msg_len);
			}
		}
	}
	TEST_DEBUG("EncodeSmsDeliverReportTpdu");
	TEST_DEBUG("Send packet to phonseSever[%d] -----", index);

	for(i=0; i< index ; i++ )
	{
		TEST_DEBUG("[%02x]", packet[i]);
		if( i%10 == 9)
			TEST_DEBUG("\n");
	}
	TEST_DEBUG("\n");
	memcpy( del_report.szData, packet, index);
	del_report.MsgLength = index;
	TEST_DEBUG("msgLength :%d",del_report.MsgLength);
	int requestId =0;
	tel_send_sms_deliver_report(&del_report,0, &requestId);//madhavi
	TEST_DEBUG("after send deliver report request id is %d",requestId);
	return TRUE;
}


BOOL DecodeSmsSubmitReportTpdu(int length , char *data)
{

	TPDU_SMS_SUBMIT_REPORT	submit_report;
	TmDateTime		   		SCTimeStamp;;
	int						position=1;
	int						i=0;
	TS_BYTE						msgRef, tpdu_len;
	int result=0;
	TS_BYTE					mask;

	/* Result */

	if(data == NULL){
		TEST_DEBUG("pData is Null")
		return FALSE;
	}

	TEST_DEBUG("length :%d",length);


	for(i=0;i<length;i++)
		TEST_DEBUG("[%02x]",data[i]);


	result = data[position];

	TEST_DEBUG("In ackrecv (return status) result is %x",result);
	position ++;

	/* Msg Reference */

	msgRef =  data[position++];
	TEST_DEBUG("In ackrecv  position :%d msgref is %x",position,msgRef);
	position ++;

	/* TPDU Length */

	tpdu_len = data[position];
	TEST_DEBUG("In ackrecv position :%d len is %d",position,tpdu_len);



	/**** SUBMIT REPORT TPDU PARSING
	 ***/

	/* TP-MTI , TP-UDHI */
	TEST_DEBUG("before TPDU position value:%d",position);

	submit_report.msg_type = data[position] & 0x03; //bit 0,1
	submit_report.udhi = data[position] & 0x40 ? TRUE : FALSE;	 //bit 6;

	position ++;

	if(result == FAIL)
	{
		/* TP-FCS */
		submit_report.fcs =  data[position] ;
		position ++;
	}

	/* TP-PI  - set to a 1 indicates that a particular optional parameter is present in the fields which follow. */
	/* Masking Bit  - TP -PID (bit 0), TP-DCS(bit 1), TP-UDL(bit 2) */

	submit_report.paraInd = data[position] ;
	mask =  submit_report.paraInd;
	position ++;

	//TEST_DEBUG("Parmeter Indication mask =%x", mask);

	/* TP-SCTS */

	SmsUtilDecodeTimeStamp( &data[position], & SCTimeStamp );

	position+=7;

	if( ( mask != 0xFF ) && ( mask != 0 ) )
	{
		/* TP-PID */
		if ( mask & 0x01)
		{
			submit_report.pId= data[position];
			position ++;
			TEST_DEBUG("TP-PID :%x", submit_report.pId);
		}

		/* TP-DCS */
		if ( mask & 0x02)
		{
			SmsUtilDecodeDCS( &submit_report.dcs, data[position] );
			position ++;
			TEST_DEBUG("TP-DCS :%x",data[position] );
		}

		if ( mask & 0x04)
		{
			/* TP-UDL */
			submit_report.udl = data[position];
			position++;

			TEST_DEBUG("TP-UDL :%x",data[position] );

			/* TP-UD */

			submit_report.userData = malloc( sizeof ( TS_BYTE ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			memset(submit_report.userData, 0, sizeof ( TS_BYTE ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			if (submit_report.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
			{
				char	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

				memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

				memcpy( (void*) inData, (void*) &data[position], submit_report.udl );

				SmsUtilUnpackGSMCode(submit_report.userData, inData, (TS_UINT8) submit_report.udl);
			}

		}
	}

	TEST_DEBUG("*************************************");

	if(result != 0x00)
		TEST_DEBUG("SMS SEND FAIL!!")
	else
		TEST_DEBUG("SMS SEND SUCCESS!!")

	if(result != 0x00)
				TEST_DEBUG("Failure Code : %x",submit_report.fcs)


					TEST_DEBUG("*************************************");

	return TRUE;
}

TS_BOOL DecodeSmsStatusReportTpdu(int tpdu_len, char* pTPDU)
{
	TPDU_SMS_STATUS_REPORT status_report;


	TmDateTime   SCTimeStamp;
	TmDateTime   DischargeTime;

	int			rcpAddr_len = 0;
	char		diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];

	char		      rcpAddr[TAPI_NETTEXT_ADDRESS_LEN_MAX]; //recipient number
	int		      rcp_ton,rcp_npi;
	int		      position=0;
	TS_UINT		      mask;

	/* TP-MTI, TP MMS, TP SRQ, TP_UDHI */

	status_report.msg_type = pTPDU[position] & 0x03; // bit 0,1
	status_report.mms = pTPDU[position] & 0x04 ? TRUE : FALSE ; // bit 2
	status_report.srq = pTPDU[position] &  0x20 ? TRUE : FALSE ; //bit 5
	status_report.udhi = pTPDU[position] & 0x40 ? TRUE : FALSE;	 //bit 6

	position ++;

	/* TP-MR */
	status_report.mr = pTPDU[position];
	position ++;

	/* TP-RA */

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, &pTPDU[position],&rcp_ton, &rcp_npi);

	position+=2;

	rcpAddr_len = strlen( (char*) diallingNum );
	if ( rcpAddr_len % 2 )
		position += rcpAddr_len / 2 + 1;
	else
		position += rcpAddr_len / 2;


	if ( rcp_ton == SIM_TON_INTERNATIONAL )
	{
		rcpAddr[0] = '+';
		memcpy( &rcpAddr[1], diallingNum, rcpAddr_len );
		rcpAddr[rcpAddr_len+1] = '\0';
	}
	else
	{
		memcpy( rcpAddr, diallingNum, rcpAddr_len );
		rcpAddr[rcpAddr_len] = '\0';
	}

	status_report.rcpAddr.DialNumLen= rcpAddr_len;
	memcpy(status_report.rcpAddr.szDiallingNum, rcpAddr , rcpAddr_len);
	status_report.rcpAddr.Npi = rcp_npi;
	status_report.rcpAddr.Ton = rcp_ton;

	/* TP-SCTS */
	SmsUtilDecodeTimeStamp( &pTPDU[position], & SCTimeStamp );
	position+=7;

	/* TP-DT */
	SmsUtilDecodeTimeStamp( &pTPDU[position], & DischargeTime );
	position+=7;

	/* TP-ST */
	status_report.status = pTPDU[position];
	position ++;

	/* TP-PI */
	status_report.paraInd = pTPDU[position];

	mask = status_report.paraInd  ;

	if ( ( mask != 0xFF ) && ( mask != 0 ) )
	{
		/* Increment only if mask is valid */
		position ++;

		/* TP-PID */
		if ( mask & 0x01)
		{
			status_report.pId= pTPDU[position];
			position ++;
		}
		/* TP-DCS */
		if ( mask & 0x02)
		{
			SmsUtilDecodeDCS( &status_report.dcs, pTPDU[position] );
			position ++;
		}
		if ( mask & 0x04)
		{
			/* TP-UDL */
			status_report.udl = pTPDU[position];
			position++;

			/* TP-UD */

			status_report.userData = malloc( sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			memset(status_report.userData, 0, sizeof ( unsigned char ) * ( TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 ) );

			if (status_report.dcs.alphabet_type == TAPI_NETTEXT_ALPHABET_DEFAULT )
			{
				char	inData[TAPI_NETTEXT_SMDATA_SIZE_MAX+1];

				memset( inData, 0x00, TAPI_NETTEXT_SMDATA_SIZE_MAX + 1 );

				memcpy( (void*) inData, (void*) &pTPDU[position], status_report.udl );

				SmsUtilUnpackGSMCode(status_report.userData, inData, (TS_UINT8) status_report.udl);
			}

		}
	}

	TEST_DEBUG("*************************************");
	TEST_DEBUG("Msg Type: STATUS_REPORT");
	if(status_report.mms)
	{TEST_DEBUG("MMS: No more messages are waiting for the MS in this SC")}
	else if(!status_report.mms)
	{TEST_DEBUG("MMS: More messages are waiting for the MS in this SC")}

	if(status_report.srq)
	{TEST_DEBUG("SRQ: the result of an SMS COMMAND ")}
	else if(!status_report.srq)
	{TEST_DEBUG("SRQ: the result of a SMS SUBMIT.")}

	if(status_report.udhi)
	{TEST_DEBUG("UDHI: Contains a Header in addition to the short message.")}
	else if(!status_report.udhi)
	{TEST_DEBUG("UDHI: Only Short Message")}

	TEST_DEBUG("STATUS:%x",status_report.status);
	if(status_report.status==0x00)
		{TEST_DEBUG("STATUS : Short message received by the SME")}
	else if(status_report.status==0x01)
	{TEST_DEBUG("STATUS : Short message forwarded by the SC to the SMEbut the SC is unable to confirm delivery")}
	else if(status_report.status==0x02)
	{TEST_DEBUG("STATUS : Short message replaced by the SC")}

	TEST_DEBUG("Recipient Number : %s" ,rcpAddr );
	TEST_DEBUG("SC Time Stamp : %d-%d-%d  , %d:%d:%d",
			SCTimeStamp.year, SCTimeStamp.month, SCTimeStamp.day, SCTimeStamp.hour, SCTimeStamp.minute ,SCTimeStamp.second);
	TEST_DEBUG("Discharge Time : %d-%d-%d  , %d:%d:%d",
			DischargeTime.year, DischargeTime.month, DischargeTime.day, DischargeTime.hour, DischargeTime.minute ,DischargeTime.second);

	if(mask & 0x04)
		TEST_DEBUG("Message : %s " , status_report.userData);

	TEST_DEBUG("*************************************");

	return TRUE;
}

void MsgCountNotification(int length, char *pData)
{

	static TelSmsStoredMsgCountInfo_t	*countInfo;
	int loop_counter = 0;
	TapiResult_t returnStatus ;
	int requestId = 0;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}

	countInfo = (TelSmsStoredMsgCountInfo_t*) pData;

	if( countInfo->UsedCount != 0x00 )	//if used count is not zero
	{
		TEST_DEBUG("Index LIST..........");
		for(loop_counter  = 0; loop_counter < countInfo->UsedCount ; loop_counter++)
		{
			TEST_DEBUG("[%02x]", countInfo->IndexList[loop_counter]);
			returnStatus = tel_read_sms_in_sim(countInfo->IndexList[loop_counter],&requestId);
			TEST_DEBUG("After read msg: returnstatus %d request id is %d",returnStatus,requestId);
		}
		TEST_DEBUG("In MsgCountNotification total cnt is %d, usedcnt is %dapi_err %d",countInfo->TotalCount, countInfo->UsedCount,returnStatus);
	}

	TEST_DEBUG("In MsgCountNotification total cnt is %d, usedcnt is %d",countInfo->TotalCount, countInfo->UsedCount);
}//Madhavi


void ReadMsgNotification(int length, char *pData)
{
	TelSmsData_t  * sim_data;
	int                scaAddr_len =0;
	char		    * pTPDU;
	int		    tpdu_len =0;
	char		    diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	char             scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	int		    sca_ton, sca_npi;
	int		    position;

	if(pData == NULL){
		TEST_DEBUG("pData is Null");
		return;
	}

	scaAddr_len = pData[7];

	int i = 0;
	for( i=0;i<length ; i++)
		TEST_DEBUG("[%02x]", pData[i]);

	sim_data = (TelSmsData_t *)pData;

	if(sim_data->MsgStatus== TAPI_NETTEXT_STATUS_UNREAD)
		TEST_DEBUG("Msg Staus : received unread msg")
	else if(sim_data->MsgStatus==TAPI_NETTEXT_STATUS_READ)
		TEST_DEBUG("Msg Staus : received read msg")
	else if(sim_data->MsgStatus==TAPI_NETTEXT_STATUS_UNSENT)
		TEST_DEBUG("Msg Staus : unsent msg")
	else
		TEST_DEBUG("Msg Staus : [%d]", sim_data->MsgStatus)

	TEST_DEBUG("First Data [%x]", sim_data->SmsData.szData[0]);
	TEST_DEBUG("Second Data [%x]", sim_data->SmsData.szData[1]);
	TEST_DEBUG("Third Data [%x]", sim_data->SmsData.szData[2]);
	TEST_DEBUG("Fourth Data [%x]", sim_data->SmsData.szData[3]);

	//if(sim_data->MsgStatus ==0x01 || sim_data->MsgStatus==0x02)
	{
		position=0;
		// SCA_ADDR
		memset(diallingNum, 0, sizeof(diallingNum));

		SmsUtilDecodeAddrField(diallingNum, (char *)sim_data->SmsData.Sca, &sca_ton, &sca_npi);

		position+=2;  //include Address-Length, Type of Address

		scaAddr_len = strlen( (char *)diallingNum );

		if ( scaAddr_len % 2 )
			position += scaAddr_len / 2 + 1;
		else
			position += scaAddr_len / 2;


		if ( sca_ton == SIM_TON_INTERNATIONAL )
		{
			scaAddr[0] = '+';
			memcpy( &scaAddr[1], diallingNum, scaAddr_len );
			scaAddr[scaAddr_len+1] = '\0';
		}
		else
		{
			memcpy( scaAddr, diallingNum, scaAddr_len );
			scaAddr[scaAddr_len] = '\0';
		}

		memcpy(&sim_data->SmsData.MsgLength, &pData[4+7 + position], sizeof(char) );	//position is 7(sca)

		tpdu_len = sim_data->SmsData.MsgLength ;

		memcpy(&sim_data->SmsData.szData, &pData[4+position+4 + 7], tpdu_len );

		TEST_DEBUG("SCA Number : %s tpdu_len is %d",scaAddr,tpdu_len);

		pTPDU = malloc(sizeof(TS_BYTE) * tpdu_len);
		TEST_DEBUG("bfor memcopy position is %d",position);
		memcpy(pTPDU, &(sim_data->SmsData.szData[0]), tpdu_len);
		TEST_DEBUG("after memcpy");

		DecodeSmsDeliverTpdu(tpdu_len, pTPDU);

		free(pTPDU);
	}

}//Madhavi

void MemoryFullNotification(int length, char *pData)
{
	int * memory_ind;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}
	memory_ind = (int *)pData;
	TEST_DEBUG("In memory full notification status %d",*memory_ind);

}//Madhavi


void DeleteMsgNotification(int length, char *pData)
{
	int *index;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}
	index = (int *) pData;
	TEST_DEBUG("In deletenotifcation the result and index are [%d]", *index);


}//madhavi

void SaveMsgNotification(int length, char *pData)
{
	int *index;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}
	index = (int *)pData;
	TEST_DEBUG("In savestatus noti, index [%d]", *index);

}

void GetCBNotification(int length, char *pData)
{
	TelSmsCbConfig_t * CBConfig;
	int i =0;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}

	CBConfig =(TelSmsCbConfig_t *) pData;


	TEST_DEBUG("=========CB Configuration=========");

	/*** CB Enable/Diable ***/
	if(CBConfig->bCBEnabled==TRUE)
		TEST_DEBUG("Cell Broadcast Msg Enabled...")
	else
		TEST_DEBUG("Cell Broadcast Msg Disabled...")


			/*** Selected ID ***/
			if(CBConfig->SelectedId == 0x01)
				TEST_DEBUG("All CBMI is selected... ")
			else if(CBConfig->SelectedId == 0x02)
				TEST_DEBUG("There are some selected CBMI...");

	/*** CBMI  Count ***/
	TEST_DEBUG("CBMI Count: %d \n",CBConfig->MsgIdCount);

	/*** CBMI  List ***/

	if(CBConfig->MsgIdCount !=0)
	{

		TEST_DEBUG("----- CBMI List -----");

		for(i =0 ; i < CBConfig->MsgIdCount; i++)
		{

			TEST_DEBUG("No.%d - [0x%04x]", i,CBConfig->MsgIDs[i]);

		}

	}
	TEST_DEBUG("==================================");

}

void param_get_noti(int length, char *pData)
{
	TelSmsParams_t *smsp_param;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}
	smsp_param = (TelSmsParams_t  *)pData;

	TEST_DEBUG("record index is %x",smsp_param->RecordIndex);
	TEST_DEBUG("record len  is %x",smsp_param->RecordLen);
	TEST_DEBUG("alpha_id len  is %lu ",smsp_param->AlphaIdLen);
	TEST_DEBUG("alpha_id is %s ",smsp_param->szAlphaId);
	TEST_DEBUG("param indicator is  %xn",smsp_param->ParamIndicator);
	TEST_DEBUG("dest dialling num is %s",smsp_param->TpDestAddr.szDiallingNum);
	TEST_DEBUG("svcaddr dialling num is %s",smsp_param->TpSvcCntrAddr.szDiallingNum);
	TEST_DEBUG("pid %x",smsp_param->TpProtocolId);
	TEST_DEBUG("dcs is %x",smsp_param->TpDataCodingScheme);
	TEST_DEBUG("validity is %x",smsp_param->TpValidityPeriod);
}


void Device_ReadyNoti(int length, char *pData)
{
	int *device_status;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}

	device_status = (int *)pData;

	if(*device_status == TRUE){
		TEST_DEBUG("Device Ready");
	}
	else if(*device_status  == FALSE){
		TEST_DEBUG("Device Not Ready");
	}
	else{
		TEST_DEBUG("In device ready noti the result is %d",*device_status );
	}
	return;
}

void Deliver_ReportNoti(int status)
{
	if(status == TAPI_NETTEXT_SENDSMS_SUCCESS){
		TEST_DEBUG("DELIVERY REPORT SUCCESS");
	}
	else{
		TEST_DEBUG("DELIVERY REPORT Fail : Reason[0x%0x]", status);
	}

	return;
}


void get_preferred_bearer_noti(int length,char *pData)

{
	TEST_DEBUG("get preferred bearer");
	int svc=0;
	TelSmsBearerType_t *bearer;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}

	bearer=(TelSmsBearerType_t *)pData;
	TEST_DEBUG("In GetPreferredBearer noti bearer:%d",*bearer);
	svc=*bearer;
	TEST_DEBUG("svc:%d",svc);

	if(svc== TAPI_NETTEXT_BEARER_PS_ONLY)
		TEST_DEBUG("Packet Domain")
	else if(svc== TAPI_NETTEXT_BEARER_CS_ONLY)
		TEST_DEBUG("Circuit Switched")
	else if(svc== TAPI_NETTEXT_BEARER_PS_PREFERRED)
		TEST_DEBUG("Packet Domain preferred ")
	else if(svc== TAPI_NETTEXT_BEARER_CS_PREFERRED)
		TEST_DEBUG("Circuit Switched preferred")

}

void set_request_confirm_noti(int length, char *pData)
{
	TEST_DEBUG("***Set Request  confirm Notification*******");

	TelSmsSetResponse *pSetReqConfType;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}
	pSetReqConfType = (TelSmsSetResponse *)pData;

	switch(*pSetReqConfType)
	{
		case TAPI_NETTEXT_CBSETCONFIG_RSP:
			TEST_DEBUG("TAPI_NETTEXT_CBSETCONFIG_RSP");
			break;
#if 0
		case TAPI_NETTEXT_SETPREFERREDBEARER_RSP:
			TEST_DEBUG("TAPI_NETTEXT_SETPREFERREDBEARER_RSP");
			break;
#endif
		case TAPI_NETTEXT_SETPARAMETERS_RSP:
			TEST_DEBUG("TAPI_NETTEXT_SETPARAMETERS_RSP");
			break;

		case TAPI_NETTEXT_SETMEMORYSTATUS_RSP:
			TEST_DEBUG("TAPI_NETTEXT_SETMEMORYSTATUS_RSP");
			break;

		case TAPI_NETTEXT_SETMESSAGESTATUS_RSP:
			TEST_DEBUG("TAPI_NETTEXT_SETMESSAGESTATUS_RSP");
			break;

		case TAPI_NETTEXT_SETDEVICESTATUS_RSP:
			TEST_DEBUG("TAPI_NETTEXT_SETDEVICESTATUS_RSP");
			break;

		case TAPI_NETTEXT_SETSCADDR_RSP:
			TEST_DEBUG("TAPI_NETTEXT_SETSCADDR_RSP");
			break;

		default:
			TEST_DEBUG("Unknown set request confirm noti");
			break;

	}

	TEST_DEBUG("Notification Received Successfully");
}

void param_count_noti(int length, char *pData)
{
	int  * RecordCount;

	if(pData == NULL){
		TEST_DEBUG("pData is Null")
		return;
	}

	RecordCount = (int *)pData;

	TEST_DEBUG("ParamCnt Length[%d]", length);
	TEST_DEBUG("In param_count_noti the record count is %d", *RecordCount);
}

int  SendMessage()
{
	int	ret;
	int	msg_len=0;
	char buf[512];
	char message[512];
	//char bearerType[2];
	char diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	int	diallingNum_len=0;

	memset(buf,0, sizeof(buf));
	memset(diallingNum,0,sizeof(diallingNum));
	diallingNum_len=0;

	TEST_DEBUG("Enter destination Number: ");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	diallingNum_len=strlen(diallingNum);
	memcpy(&diallingNum[diallingNum_len], buf,sizeof(buf));

	diallingNum_len = strlen(diallingNum); //recalculate
	TEST_DEBUG("dialling num %s and dialling num len is %d",diallingNum,diallingNum_len);
	diallingNum[diallingNum_len]=0;
	diallingNum_len =diallingNum_len-1;

	TEST_DEBUG("Enter Message: ");
	memset(message,0,sizeof(message));

	ret = read(0, message, sizeof(message));

	if (ret <= 0) {
		TEST_DEBUG(" NULL msg can NOT be sent ");
		return -1;
	}

	msg_len=strlen(message);

	TEST_DEBUG("==========================");
	TEST_DEBUG("To :%s", diallingNum);
	TEST_DEBUG("Message: %sMsg Length:%d",message, msg_len);
	TEST_DEBUG("Dialling number Length : %d" ,diallingNum_len);
	TEST_DEBUG("==========================\n");

	EncodeSmsSubmitTpdu(diallingNum, diallingNum_len,message, msg_len) ;
	return 1;

}

int SendMessageCDMA()
{
	int ret;
	int msg_len = 0;
	char buf[512] = {0,};
	char message[512] = {0,};
	char diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX] = {0,};
	int diallingNum_len = 0;

	printf("\n");
	printf("Enter destination Number:\n>> ");
	fflush(stdout);

	ret = read(0, buf, sizeof(buf));
	if (ret < 0) {
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	diallingNum_len = strlen(diallingNum);
	memcpy(&diallingNum[diallingNum_len], buf, sizeof(buf));

	diallingNum_len = strlen(diallingNum); //recalculate
	diallingNum[diallingNum_len] = 0;
	diallingNum_len = diallingNum_len - 1;
	printf("dialling num [%s], dialling num len [%d]\n", diallingNum, diallingNum_len);

	printf("Enter Message\n>> ");
	fflush(stdout);

	ret = read(0, message, sizeof(message));
	if (ret <= 0) {
		printf(" NULL msg can NOT be sent \n");
		return -1;
	}

	msg_len = strlen(message);

	printf("===========================\n");
	printf("To: [%s] (len: %d)\n", diallingNum, diallingNum_len);
	printf("Message: [%s]\nMsg Length: [%d]\n", message, msg_len);
	printf("===========================\n\n");

	EncodeCdmaSmsSubmitTpdu(diallingNum, diallingNum_len, message, msg_len);
	//EncodeSmsSubmitTpdu(diallingNum, diallingNum_len,message, msg_len) ;

	return 1;
}




int ReadMessage()
{
	int		menu;
	int		ret;
	char	buf[100];

	TapiResult_t returnStatus;
	int requestId = 0;
	memset(buf,0, sizeof(buf));

	TEST_DEBUG("*************************************");
	TEST_DEBUG("1. SIM Message");

	TEST_DEBUG("Select Number:");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	menu = atoi(buf);
	TEST_DEBUG("menu in read message(value is 1) is %d ",menu);
	switch(menu)
	{
		case 1:

			TEST_DEBUG("***Reading the message(Api:GetMsgCount)***");
			returnStatus = tel_get_sms_count(&requestId);
			TEST_DEBUG("status for read message is %d request Id %d",returnStatus,requestId);

	}
	return 1;
}

int DeleteMessage(
		)
{
	int	ret;
	int	index;
	char buf[100];

	TapiResult_t returnStatus;

	int requestId = 0;

	memset(buf, 0, sizeof(buf));

	TEST_DEBUG("Enter delete index >>");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	index=atoi(buf);
	TEST_DEBUG("index value :%d",index);


	TEST_DEBUG("***Deleting the message(Api:DeleteMsg,GetMsgCount)***");

	returnStatus = tel_delete_sms_in_sim(index,&requestId);
	TEST_DEBUG("returnstatus  delete message  is for delete%d request id %d",returnStatus,requestId);
	returnStatus = tel_get_sms_count(&requestId);
	TEST_DEBUG("returnStatus for  delete msg msgcnt  %d requestid %d",returnStatus,requestId);


	return 1;

}

int ReceiveMessage(int pdu_len, char * pPDU)
{

	int                scaAddr_len =0;
	char		    * pTPDU;
	int		    tpdu_len =0;
	char		    diallingNum[TAPI_NETTEXT_ADDRESS_LEN_MAX];
	char             scaAddr[TAPI_NETTEXT_SCADDRESS_LEN_MAX]; //service center address
	int		    sca_ton, sca_npi;
	int		    position;

	TS_BYTE MTI = 0;
	TS_BYTE	MMS = 0;
	TS_BYTE SRI = 0;
	TS_BYTE UDHI = 0;
	TS_BYTE RP = 0;

	tapi_nettext_coding_scheme dcs;
	TelSmsDatapackageInfo_t * datapackage;
	TapiResult_t returnStatus;
	position=0;
	int requestId = 0;

	if(pPDU == NULL){
		TEST_DEBUG("pData is Null")
		return FALSE;
	}
	/*	for debug
		for( i=0;i<pdu_len ; i++)
		TEST_DEBUG("[%02x]", pPDU[i]);
		*/

	datapackage = (TelSmsDatapackageInfo_t *)pPDU;

	memset(diallingNum, 0, sizeof(diallingNum));

	SmsUtilDecodeAddrField(diallingNum, (char *)datapackage->Sca, &sca_ton, &sca_npi);

	scaAddr_len = strlen( diallingNum );

	if ( sca_ton == SIM_TON_INTERNATIONAL )
	{
		scaAddr[0] = '+';
		memcpy( &scaAddr[1], diallingNum, scaAddr_len );
		scaAddr[scaAddr_len+1] = '\0';
	}
	else
	{
		memcpy( scaAddr, diallingNum, scaAddr_len );
		scaAddr[scaAddr_len] = '\0';
	}

	TEST_DEBUG("Sc address in test app is %s",scaAddr);

	RP = datapackage->szData[position] & 0x80;
	UDHI = datapackage->szData[position] & 0x40;
	SRI = datapackage->szData[position] & 0x20;
	MMS = datapackage->szData[position] & 0x04;
	MTI = datapackage->szData[position] & 0x03;

	TEST_DEBUG("RP [%x]", RP);
	TEST_DEBUG("UDHI [%x]", UDHI);
	TEST_DEBUG("SRI [%x]", SRI);
	TEST_DEBUG("MMS [%x]", MMS);
	TEST_DEBUG("MTI [%02x]", MTI);

	tpdu_len = datapackage->MsgLength;

	pTPDU = malloc(sizeof(TS_BYTE) * tpdu_len);

	memcpy(pTPDU, &datapackage->szData[position], tpdu_len);

	if(MTI ==SMS_TPDU_DELIVER)
	{
		DecodeSmsDeliverTpdu(tpdu_len, pTPDU);

		TEST_DEBUG("dcs_pos : %d", dcs_pos);

		SmsUtilDecodeDCS(&dcs, datapackage->szData[position+dcs_pos]);

		//if(dcs.class_type == TAPI_NETTEXT_CLASS_2)
		{
			TEST_DEBUG("dcs type is of class2");
			TelSmsData_t WriteData;

			memset(&WriteData, 0, sizeof(TelSmsData_t));

			memcpy(WriteData.SmsData.Sca , datapackage->Sca, TAPI_SIM_SMSP_ADDRESS_LEN);

			WriteData.SmsData.MsgLength = datapackage->MsgLength;

			memcpy(WriteData.SmsData.szData, datapackage->szData, datapackage->MsgLength);

			WriteData.MsgStatus = TAPI_NETTEXT_STATUS_READ;

			returnStatus = tel_write_sms_in_sim(&WriteData, &requestId);

			TEST_DEBUG("returnStatus for write  %d requestId :%d",returnStatus,requestId);
		}


	}
	else if(MTI==SMS_TPDU_STATUS_REPORT)
	{
		DecodeSmsStatusReportTpdu(tpdu_len, pTPDU);
	}

	TelSmsDatapackageInfo_t * del_report= NULL;

	del_report= malloc(sizeof(TelSmsDatapackageInfo_t));

	memset(del_report, 0, sizeof(TelSmsDatapackageInfo_t));

	memcpy(del_report->Sca, datapackage->Sca, TAPI_SIM_SMSP_ADDRESS_LEN);

	del_report->szData[0] = SMS_TPDU_DELIVER_REPORT;	//TP-UDHI[bit6] : 0
	del_report->szData[1] = 0;	// TP-PI = 0; bit2: TP-UDL bit1: TP-DCS bit0: TP-PID (No option field)
	del_report->MsgLength = 2;

	TEST_DEBUG("***receive message (Api:SendDeliverreport)****");


	returnStatus =tel_send_sms_deliver_report(del_report,TAPI_NETTEXT_SENDSMS_SUCCESS, &requestId);

	TEST_DEBUG("AFter send deliver report request id[%d] & result [0x%x]",requestId,returnStatus);

	free(del_report);

	free(pTPDU);

	return 1;
}

int GetInformation()
{
	int		menu;
	int		ret;
	char	buf[100];

	TapiResult_t returnStatus;

	/* Get CB Setting Information */
	//TS_UINT16 CBChannel[MAX_GSM_SMS_CBMI_LIST_SIZE];
	int requestId=0;

	memset(buf,0, sizeof(buf));

	TEST_DEBUG("*************************************");
	TEST_DEBUG("1.Get Service Center Number");
	TEST_DEBUG("2.Get CB Setting Information");
	TEST_DEBUG("3.Get SMS Parameters");
	TEST_DEBUG("4.Get SMS PreferredBearer Information (Not supported)");
	TEST_DEBUG("5.Get SMS Parameter Count");
	TEST_DEBUG("6.Get Message Count");
	TEST_DEBUG("*************************************");
	TEST_DEBUG("Select Number:");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	menu = atoi(buf);

	switch(menu)
	{
		case 1:
			{
				TEST_DEBUG("***Getting the SCA(Api: GetSCA)****");
				returnStatus = tel_get_sms_sca(0, &requestId);
				TEST_DEBUG("returnstatus for scaddr get %d",returnStatus);
#if 0
				SmsUtilDecodeAddrField(decodeScaAddr, (char*)sc_addr.szDiallingNum, (int *)&sc_addr.Ton, (int *)&sc_addr.Npi);

				sca_len = strlen( decodeScaAddr );

				if ( sc_addr.Ton== SIM_TON_INTERNATIONAL )
				{
					realScaAddr[0] = '+';
					memcpy( &realScaAddr[1], decodeScaAddr, sca_len );
					realScaAddr[sca_len+1] = '\0';
				}
				else
				{
					memcpy( realScaAddr, decodeScaAddr, sca_len );
					realScaAddr[sca_len] = '\0';
				}

				TEST_DEBUG("===== Decode SCA Address =====");
				TEST_DEBUG("SCA Number :%s ",realScaAddr);
				TEST_DEBUG("==============================");
#endif
				break;
			}


		case 2:
			{

				TEST_DEBUG("***Getting the CB Configuration(Api: GetCBConfig)***");
				returnStatus =  tel_get_sms_cb_config(&requestId);
				TEST_DEBUG("ReturnStatus[%d] Requestid0x%x]",returnStatus,requestId);
				break;
			}

		case 3:
			{

				TEST_DEBUG("****Getting the parameter(Api:GetParameter)****");
				returnStatus = tel_get_sms_parameters(0,&requestId);
				TEST_DEBUG("ReturnStatus[%d] Requestid0x%x]",returnStatus,requestId);
				break;
			}
		case 4:
			{

				//TEST_DEBUG("***Getting preferrredBearer(Api:GetPreferredBearer)***");
				//returnStatus=tel_get_sms_preferred_bearer(&requestId);
				//TEST_DEBUG("ReturnStatus[%d] Requestid0x%x]",returnStatus,requestId);
				break;
			}

		case 5:
			{
				returnStatus = tel_get_sms_parameter_count(&requestId);
				TEST_DEBUG("ReturnStatus[%d] Requestid0x%x]",returnStatus,requestId);
				break;
			}
		case 6:
			{
				returnStatus = tel_get_sms_count(&requestId);
				TEST_DEBUG("ReturnStatus[%d] Requestid0x%x]",returnStatus,requestId);
				break;
			}
		default:
			return -1;

	}

	return 1;
}

int Setting()
{
	int				settingMenu;
	int				ret;
	char			buf[512];

	char		MemoryStatus[2]; //2006/8/8
	char			bearerType[2];  //2006/8/8
	char		cbEnable[2];//2006/8/8

	TelSmsCbConfig_t   *pCBConfig;

	int				memType;
	int				index;
	int				msgStauts;

	TelSmsParams_t  smsParameters = {0,};
	TapiResult_t returnStatus;
	int requestId=0;

	memset(buf,0, sizeof(buf));

	TEST_DEBUG("*************************************");
	TEST_DEBUG("1.Set Service Center Number");
	TEST_DEBUG("2.Set PreferredBearer Type");
	TEST_DEBUG("3.Set Deliver Report");
	TEST_DEBUG("4.Set CB Enable/Disable");
	TEST_DEBUG("5.Set SMS Parameters");
	TEST_DEBUG("6.Set Memory Status");
	TEST_DEBUG("7.Set Stored MsgStaus");
	TEST_DEBUG("8.Set Device Ready");
	TEST_DEBUG("9.Check Device Status");
	TEST_DEBUG("*************************************");
	TEST_DEBUG("Select Number:");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	settingMenu = atoi(buf);


	switch(settingMenu)
	{
		case 1:  //Set Service Center Number
			{
				TEST_DEBUG("***Setting the SCA(Api:SetSCA)****");
				TelSmsAddressInfo_t sca;

				memset(&sca, 0, sizeof(TelSmsAddressInfo_t));

				sca.DialNumLen = 0x5;
				sca.Npi = TAPI_SIM_NPI_ISDN_TEL;
				sca.Ton = TAPI_SIM_TON_INTERNATIONAL;
				sca.szDiallingNum[0] = 0x28;
				sca.szDiallingNum[1] = 0x01;
				sca.szDiallingNum[2] = 0x19;
				sca.szDiallingNum[3] = 0x11;
				sca.szDiallingNum[4] = 0x11;

				returnStatus =  tel_set_sms_sca(&sca,0,&requestId);

				TEST_DEBUG("returnstatus after scaddr get is is %d requestId:%d ",returnStatus,requestId);

				break;
			}

		case 2:
			{
				TEST_DEBUG("****Setting the BearerType(Api:SetPreferredBearer)****");

				TEST_DEBUG("Enter Bearer Type(1:PD, 2:CS, 3:PDP 4:CSP):");

				memset(bearerType,0 ,sizeof(bearerType));

				ret = read(0, bearerType, sizeof(bearerType));
				if (ret <= 0) {
					TEST_DEBUG(" NULL msg can NOT be sent ");
					return -1;
				}
				TEST_DEBUG("Set Preferred Bearer : bearerType =%d ", atoi(bearerType));

				returnStatus = tel_set_sms_preferred_bearer(atoi(bearerType),&requestId);
				TEST_DEBUG("returnStatus  after prefbearer set  is %d, reqid=[%d]",returnStatus,requestId);

				break;
			}

		case 3: //Set Deliver Report
			{
				TEST_DEBUG("****Setting  Deliver Report(Api:SetDeliverReport)****");
				//EncodeSmsDeliverReportTpdu();
				break;
			}
		case 4: //Set CB Enable/Disable
			{
				TEST_DEBUG("****Setting the CB configuration(Api:SetCbConfig)****");

				TEST_DEBUG("Enter CB ON/OFF (1: Enable, 2:Disable):");

				memset(cbEnable,0 ,sizeof(cbEnable));
				pCBConfig = malloc(sizeof(TelSmsCbConfig_t));

				ret = read(0, cbEnable, sizeof(cbEnable));

				if (ret <= 0) {
					TEST_DEBUG(" NULL msg can NOT be sent ");
					return -1;
				}

				pCBConfig->bCBEnabled = atoi(cbEnable);
				pCBConfig->SelectedId = 0x01;
				pCBConfig->MsgIdCount = 0x02;

				pCBConfig->MsgIDs[0] = 0x0c;
				pCBConfig->MsgIDs[1] = 0x0c;
				pCBConfig->MsgIDs[2] = 0xdf;
				pCBConfig->MsgIDs[3] = 0x00;

				TEST_DEBUG("from sms test setting the cb configuration:CBEnabled:%d,selectedId:%d,msgCount:%d,msgId: %d\t %d\t %d\t %d",pCBConfig->bCBEnabled,pCBConfig->SelectedId,pCBConfig->MsgIdCount,pCBConfig->MsgIDs[0],pCBConfig->MsgIDs[1],pCBConfig->MsgIDs[2],pCBConfig->MsgIDs[3]);

				returnStatus = tel_set_sms_cb_config(pCBConfig,&requestId);
				TEST_DEBUG("returnStatus after cbconfig set is is [%d], reqid [%d]",returnStatus,requestId);

				free(pCBConfig);
				break;
			}

		case 5:
			{
				char* name = "Lpg";
				char* num = "0103001408";
				char* sca = "8210911111";


				TEST_DEBUG("****Setting Parameters(Api:SetParameters)****");

				smsParameters.RecordIndex = 0x00;
				smsParameters.RecordLen = 31;
				smsParameters.AlphaIdLen = 3;
				memcpy(&smsParameters.szAlphaId,name,strlen(name));


				smsParameters.ParamIndicator = 0xe0;
				smsParameters.TpDestAddr.DialNumLen = 10;
				smsParameters.TpDestAddr.Ton = 1;
				smsParameters.TpDestAddr.Npi = 1;

				smsParameters.TpSvcCntrAddr.DialNumLen = 10;
				smsParameters.TpSvcCntrAddr.Ton = 1;
				smsParameters.TpSvcCntrAddr.Npi = 1;

				memcpy(smsParameters.TpDestAddr.szDiallingNum , num, strlen(num));
				memcpy(smsParameters.TpSvcCntrAddr.szDiallingNum, sca, strlen(sca));
				smsParameters.TpProtocolId = 3;
				smsParameters.TpDataCodingScheme = 2;
				smsParameters.TpValidityPeriod = 1;

				returnStatus = tel_set_sms_parameters(&smsParameters,&requestId);
				TEST_DEBUG("returnstatus after sparam set  is %d reqid %d",returnStatus,requestId);
				break;
			}
		case 6: //Set Memory Full Notification
			{
				TEST_DEBUG("Enter Memory Status to be set(1:Memory Available, 2:Memory Full)");
				memset(MemoryStatus,0 ,sizeof(MemoryStatus));
				ret = read(0, MemoryStatus, sizeof(MemoryStatus));
				if (ret <= 0) {
					TEST_DEBUG(" NULL msg can NOT be sent ");
					return -1;
				}
				TEST_DEBUG("Memory Status type is %d ", atoi(MemoryStatus));
				returnStatus =tel_set_sms_memory_status(atoi(MemoryStatus), &requestId);//Set to full 0x02-Full, 0x01-available
				TEST_DEBUG("api err after memstatus set is is %d",returnStatus);
				break;
			}
		case 7: //Set Stored MsgStaus
			{
				memType=2;
				index=2;
				msgStauts =2;

				//TapiReadNetText(index, &CurMsgStauts, &DataPackage);
				returnStatus = tel_read_sms_in_sim(index,&requestId);//To avoid  mismatch of prototypes
				TEST_DEBUG("returnStatus after read is is %d",returnStatus);

				TEST_DEBUG("Before set status : index = %d, CurMsgStauts=%d",index,msgStauts);

				returnStatus = tel_set_sms_message_status(index,  msgStauts,&requestId);
				TEST_DEBUG("returnStatus after msgstatus set is %d",returnStatus);

				//tapi_nettext_read(index, &CurMsgStauts, &DataPackage);
				returnStatus = tel_read_sms_in_sim(index,&requestId);//To avoid  mismatch of prototypes
				TEST_DEBUG("returnStatus after read is %d",returnStatus);

				break;
			}
		case 8:
			{
				returnStatus =tel_set_sms_device_status();
				TEST_DEBUG("returnStatus after read is %d",returnStatus);
				break;
			}
		case 9:
			{
				int status = 0;
				returnStatus =tel_check_sms_device_status(&status);
				if (status == 1)
				{
					TEST_DEBUG("Device Ready");
				}
				else
				{
					TEST_DEBUG("Device Not Ready");
				}
				TEST_DEBUG("returnStatus after read is %d",returnStatus);
				break;
			}
		default:
			return -1;
	}
	return 1;
}


int sms_read_key_input_cdma(void)
{

	int	ret;
	char	buf[512];

	//initialize value
	memset(buf, 0, sizeof(buf));

	printf("\n");
	printf("*************************************\n");
	printf("1.Send New Message\n");
	printf("2.SMS Exit\n");
	printf("*************************************\n");
	printf("Select Number >> ");
	fflush(stdout);

	ret = read(0, buf, sizeof(buf));
	if (ret < 0) {
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	if (memcmp(buf, "1", sizeof("1") - 1) == 0) {
		SendMessageCDMA();
	}
	if (memcmp(buf, "2", sizeof("2") - 1) == 0) {
		return -1;
	}

	return 0;
}

int sms_read_key_input(void)
{

	int	ret;
	char	buf[512];
	int returnStatus=0;

	//initialize value
	memset(buf,0, sizeof(buf));

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	TEST_DEBUG("*************************************");
	TEST_DEBUG("1.Send New Message");
	TEST_DEBUG("2.Read Message");
	TEST_DEBUG("3.Delete Message");
	TEST_DEBUG("4.Delete All Message");
	TEST_DEBUG("5.Setting");
	TEST_DEBUG("6.Getting SMS Information");
	TEST_DEBUG("7.SMS Exit");
	TEST_DEBUG("*************************************");
	TEST_DEBUG("Select Number : %s", buf);

	int requestId = 0;

	if( memcmp(buf,"1",sizeof("1")-1) == 0 ){ SendMessage();}
	if( memcmp(buf,"2",sizeof("2")-1) == 0 ){ ReadMessage();}
	if( memcmp(buf,"3",sizeof("3")-1) == 0 ){ DeleteMessage();}
	if( memcmp(buf,"4",sizeof("4")-1) == 0 ){ returnStatus=tel_delete_sms_in_sim(-1,&requestId);TEST_DEBUG("returnStatus:%d,requestId:%d ",returnStatus,requestId);}
	if( memcmp(buf,"5",sizeof("5")-1) == 0 ){ Setting();}
	if( memcmp(buf,"6",sizeof("6")-1) == 0 ){ GetInformation();}
	if( memcmp(buf,"7",sizeof("7")-1) == 0 ){ return -1; }

	return 0;
}

void SMS_async_event_callback(TelTapiEvent_t* event)
{
	//char event_string[IF_NAME_MAX];
	//char event_string[100];
	int iSize = 0;

	TEST_DEBUG("Func Entrance");
	TEST_DEBUG("Request ID : [0x%x]\n", event->RequestId);

//	TAPI_GET_EVENT_NAME(  event->EventType, event_string);
//	TEST_DEBUG("Event Type [%s]\n", event_string);

	TEST_DEBUG("TAPI Event Status = [0x%0x]", event->Status);
	if(event->pData==NULL)
	{
		TEST_DEBUG("No Event Data!!");
	}
	else{
		iSize = sizeof(event->pData);
		TEST_DEBUG("Data length:%d", iSize);
	}


	switch(event->EventType)
	{

		case TAPI_EVENT_NETTEXT_READ_SMS_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$Read Msg Notification $$$");
			ReadMsgNotification(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;
		case TAPI_EVENT_NETTEXT_GET_COUNT_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$GetMsgCountNotification$$$$");
			MsgCountNotification(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_SENTSTATUS_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ RECEIVE  ACK NOTIFICATION $$$$$$ ");
			DecodeSmsSubmitReportTpdu(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_GET_PARAM_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$ GET PARAM NOTI$$$");
			param_get_noti(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$Delete Notification$$$");
			DeleteMsgNotification(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$Save Status Notification$$");
			SaveMsgNotification(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$ SET REQUESTCONFIRM NOTI$$$");
			set_request_confirm_noti(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$GET CB Config$$$");
			GetCBNotification(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF:
			TEST_DEBUG("****************************************");
			TEST_DEBUG("$$$ GET PREFERRED BEARER$$$");
			get_preferred_bearer_noti(iSize, event->pData);
			TEST_DEBUG("****************************************");
			break;

		case TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ DELIVER REPORT NOTI $$$$$$");
			Deliver_ReportNoti(event->Status);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_INCOM_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ INCOMING MESSAGE NOTIFICATION $$$$$$ ");
			ReceiveMessage(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_CB_INCOM_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ INCOMING CB NOTIFICATION $$$$$$ ");
			DecodeCellBroadcastMsg(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ MEMORYF STATUS NOTIFICATION $$$$$$ ");
			MemoryFullNotification(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_DEVICE_READY_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$ DEVICE READY NOTI$$$");
			Device_ReadyNoti(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_PARAM_COUNT_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$ PARAM COUNT NOTI$$$");
			param_count_noti(iSize, event->pData);
			TEST_DEBUG("***************************************");
			break;
		case TAPI_EVENT_NETTEXT_SENTSTATUS_EX_CNF :
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ SENT STATUS EX NOTIFICATION $$$$$$");
			TEST_DEBUG("Test appl. is Not implemented yet");
			TEST_DEBUG("***************************************");
			break;
		case TAPI_EVENT_NETTEXT_INCOM_EX_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ INCOM EX NOTIFICATION $$$$$$");
			TEST_DEBUG("Test appl. is Not implemented yet");
			TEST_DEBUG("***************************************");
			break;
		case TAPI_EVENT_NETTEXT_CB_INCOM_EX_IND:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ CB INCOM EX NOTIFICATION $$$$$$");
			TEST_DEBUG("Test appl. is Not implemented yet");
			TEST_DEBUG("***************************************");
			break;

		case TAPI_EVENT_NETTEXT_GET_SCA_CNF:
			TEST_DEBUG("***************************************");
			TEST_DEBUG("$$$$$$ GET SCA NOTIFICATION $$$$$$");
			TEST_DEBUG("Test appl. is Not implemented yet");
			TEST_DEBUG("***************************************");
			break;

		default:
			TEST_DEBUG("Event Type: unknown event:[%d]\n", event->EventType);
			break;
	}
}


int sms_subscribe_tapi_events()
{
	int i = 0;
	int ret_val = TRUE;
	int iNumOfSMSEvt = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;


	int SMSEvtList[] =
	{
		TAPI_EVENT_NETTEXT_READ_SMS_CNF,
		TAPI_EVENT_NETTEXT_GET_COUNT_CNF,
		TAPI_EVENT_NETTEXT_SENTSTATUS_CNF,
		TAPI_EVENT_NETTEXT_GET_PARAM_CNF,
		TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF,
		TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF,
		TAPI_EVENT_NETTEXT_SET_REQUEST_CNF,
		TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF,
		TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF,
		TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF,
		TAPI_EVENT_NETTEXT_INCOM_IND,
		TAPI_EVENT_NETTEXT_CB_INCOM_IND,
		TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND,
		TAPI_EVENT_NETTEXT_DEVICE_READY_IND,
		TAPI_EVENT_NETTEXT_PARAM_COUNT_IND,
		TAPI_EVENT_NETTEXT_SENTSTATUS_EX_CNF,
		TAPI_EVENT_NETTEXT_INCOM_EX_IND,
		TAPI_EVENT_NETTEXT_CB_INCOM_EX_IND,
		TAPI_EVENT_NETTEXT_GET_SCA_CNF,
	};

	iNumOfSMSEvt = sizeof(SMSEvtList)/sizeof(int);
	//TAPI_CALLOC(SMS_subscription_id, iNumOfSMSEvt, unsigned int)
	//#define TAPI_CALLOC(ptr, no_elements, type)	s
	SMS_subscription_id = (unsigned int *) calloc (iNumOfSMSEvt , sizeof (unsigned int));
	if(SMS_subscription_id == NULL)
	{
		TEST_DEBUG("calloc error -NULL, errno is [%d]", errno);
	}


	if(tel_init() == TAPI_API_SUCCESS)
	{
		for( i=0 ; i< iNumOfSMSEvt; i++ )
		{
			api_err = tel_register_event(SMSEvtList[i], &SMS_subscription_id[i],(TelAppCallback)&SMS_async_event_callback,NULL);

			if(api_err != TAPI_API_SUCCESS)
			{
				TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", SMSEvtList[i],api_err);
				ret_val = FALSE;
				break;
			}
		}
	}
	else
	{
		TEST_DEBUG("TelTapiInit() failed");
	}
	return ret_val;
}

void sms_select_loop_cdma(void)
{
	int ret;
	fd_set readfds;
	printf("Select Loop!\nPress enter key\n");
	fflush(stdout);

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret)
		{
			if (FD_ISSET(0, &readfds))
			{
				if( sms_read_key_input_cdma() < 0)
				{
					return;
				}
			}
		}
	}
}

void sms_select_loop_wcdma(void)
{
	int ret;
	fd_set readfds;
	printf("Select Loop!\n");

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret)
		{
			if (FD_ISSET(0, &readfds))
			{
				if( sms_read_key_input() <0 )
				{
					return;
				}
			}
		}
	}
}



void sms_select_loop()
{
	int iNum;
	char buf[255];

	while (1) {
		printf("*************************************\n");
		printf("1. WCDMA\n");
		printf("2. CDMA\n");
		printf("0. Exit\n");
		printf("*************************************\n");
		printf("Select Number by Network Type\n>> ");

		fflush(stdout);
		fflush(stdin);

		memset(buf, 0, 255);
		fgets(buf, 255, stdin);
		iNum = atoi(buf);

		if (iNum == 1) {
			sms_select_loop_wcdma();
		}
		else if (iNum == 2) {
			sms_select_loop_cdma();
		}
		else if (iNum == 0) {
			printf("SMS Program is terminated.\n");
			return;
		}
		else {
			printf("Input wrong number\n");
		}
	}

	return;
}

void tapi_sms_thread()
{
	TEST_DEBUG("start tapi_nettext_thread");
	//select_loop();
	TEST_DEBUG ("thread exited");
	pthread_exit(NULL);
}

void sms_test_main(int argc, char *argv[])
{

	int ret = 0, subscription_id = 0;
	pthread_t nettext_thread_id;
	TapiResult_t api_err = TAPI_API_SUCCESS;

	TEST_DEBUG("\n ===== NETTEXT  ver RAMU =====");

	ret = pthread_create(&nettext_thread_id, NULL, (void *)&tapi_sms_thread, NULL);

	if(ret != 0)
	{
		TEST_DEBUG("Thread for TAPI Events Failed");
	}

	TEST_DEBUG("Before pthread join");

	TEST_DEBUG("after TelTapiRegisterClass sub id is %d api_err is %d",subscription_id,api_err);

	pthread_join(nettext_thread_id, NULL);

	return;
}
//eof

