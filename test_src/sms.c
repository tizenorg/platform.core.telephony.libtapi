
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <glib.h>
#include <glib-object.h>

#include <tapi.h>
#include <tapi_sms.h>
#include <tapi_events.h>

#include "menu.h"
#include "sms.h"
#include "sms_util.h"

static int dcs_pos = 0;

#define NETTEXT_MSG_SIZE_MAX 918

#define CHECK_SMS_RESULT(x) \
{ \
	if (x >= TEL_SMS_RESULT_SUCCESS && x <= TEL_SMS_RESULT_OPERATION_NOT_PERMITTED) { \
		msg("result[%d][%s]", x, dbg_sms_result[x]); \
	} else { \
		msg("API failed with result[%d]", x); \
	} \
	if (x != TEL_SMS_RESULT_SUCCESS) \
		return ; \
}

char *dbg_sms_result[] = {
	"TEL_SMS_RESULT_SUCCESS",
	"TEL_SMS_RESULT_FAILURE",
	"TEL_SMS_RESULT_INVALID_PARAMETER",
	"TEL_SMS_RESULT_MEMORY_FAILURE",
	"TEL_SMS_RESULT_OPERATION_NOT_SUPPORTED",
	"TEL_SMS_RESULT_UNKNOWN_FAILURE",
	"TEL_SMS_RESULT_INVALID_MANDATORY_INFO",
	"TEL_SMS_RESULT_NETWORK_CONGESTION",
	"TEL_SMS_RESULT_SIM_FAILURE",
	"TEL_SMS_RESULT_NETWORK_FAILURE",
	"TEL_SMS_RESULT_OPERATION_NOT_PERMITTED",
};

static char data_sms_index[MENU_DATA_SIZE + 1] = {};
static char data_sms_msg_status[MENU_DATA_SIZE + 1] = {};
static char data_sms_memory_status[MENU_DATA_SIZE + 1] = {};
static char data_sms_sca_ton[MENU_DATA_SIZE + 1] = {};
static char data_sms_sca_npi[MENU_DATA_SIZE + 1] = {1, };
static char data_sms_sca_number[MENU_DATA_SIZE + 1] = {};

static char data_sms_set_params_index[MENU_DATA_SIZE + 1] = {};
static char data_sms_set_params_vp[MENU_DATA_SIZE + 1] = {};

static char data_sms_set_cb_status[MENU_DATA_SIZE + 1] = {};
static char data_sms_set_cb_msg_id_rng_cnt[MENU_DATA_SIZE + 1] = {0, };
static char data_sms_set_cb_from_msg_id[MENU_DATA_SIZE + 1] = {};
static char data_sms_set_cb_to_msg_id[MENU_DATA_SIZE + 1] = {};
static char data_sms_set_cb_msg_id_selected[MENU_DATA_SIZE + 1] = {};

static char data_sms_send_more_msgs[MENU_DATA_SIZE + 1] = {};
static char data_sms_send_msg[MENU_DATA_SIZE + 1] = {};
static char data_sms_send_msg_dialling_num[MENU_DATA_SIZE + 1] = {};

static void on_resp_write_sms_in_sim(TelHandle *handle, int result, void *data, void *user_data);
static void on_resp_sms_delivery_report(TelHandle *handle, int result, void *data, void *user_data);
static void on_send_sms(TelHandle *handle, int result, void *data, void *user_data);

/* Utilities */
static int __encode_and_send_sms_submit_tpdu (MManager *mm, char *dialling_num, unsigned int dial_num_len,
		char *msg, unsigned int msg_len, TelSmsSendInfo *send_sms_info)
{
	TelHandle *handle = menu_manager_ref_user_data (mm);
	TpduSmsSubmit tpdu_submit;
	unsigned char packet[NETTEXT_MSG_SIZE_MAX] = {0, };
	unsigned char tp_mr = 0;
	unsigned char tp_pid = 0; //reaplace type (65)
	unsigned char tp_dcs; //=17; //class_1(17),class_2(18)
	unsigned char tp_vp;
	unsigned int index = 0;
	unsigned int i;
	TelReturn rt = 0;

	tp_mr++; /* msg_ref */

	// TP-MTI, TP-RD, TP-VPF, TP-SRR, TP_UDHI, TP-RP
	tpdu_submit.msg_type = SMS_TPDU_SUBMIT;
	tpdu_submit.rd = FALSE; //false :accept duplicate msg , true: reject duplicate msg
	tpdu_submit.vpf = SMS_VP_RELATIVE;
	tpdu_submit.srr = FALSE; //false :A status report is not requested , true: A status report is requested
	tpdu_submit.udhi = FALSE; //false: contains only the short message , true :a Header in addition to the short message
	tpdu_submit.rp = FALSE; //false :TP Reply Path parameter is not set in this SMS SUBMIT, true : TP Reply Path parameter is set in this SMS SUBMIT

	packet[index] = tpdu_submit.msg_type; // SUBMIT: bits 0, 1
	packet[index] |= tpdu_submit.rd ? 0x04 : 0; // bit 2
	packet[index] |= tpdu_submit.vpf << 3; // bits 3, 4
	packet[index] |= tpdu_submit.srr ? 0x20 : 0; // bit 5
	packet[index] |= tpdu_submit.udhi ? 0x40 : 0; // bit 6
	packet[index] |= tpdu_submit.rp ? 0x80 : 0; // bit 7

	index++;
	msg("index of message ref is %d, msgref is %d", index, tp_mr);
	/* TP-MR */
	packet[index++] = tp_mr;

	// TP-DA
#if 1
	index += sms_util_encode_addr_field (packet + index, dialling_num, dial_num_len,
			0x02, 0x01);
#else
	memcpy(&(packet[index]), dialling_num, dial_num_len);
    index = index + dial_num_len;
#endif

	//msg("DESTINATTION  ADDR Encoded =========");

	//TP-PID
	packet[index++] = tp_pid;

	// TP_DCS (Data Coding Scheme)

	tpdu_submit.dcs.compressed = FALSE;
	tpdu_submit.dcs.bmsg_class_set = FALSE;
	tpdu_submit.dcs.coding_group_type = SMS_CODGRP_SM_GENERAL_DCS;
	tpdu_submit.dcs.alphabet_type = SMS_ALPHABET_DEFAULT;
	tpdu_submit.dcs.class_type = SMS_CLASS_2;

	sms_util_encode_dcs (&tp_dcs, &tpdu_submit.dcs);

	packet[index++] = tp_dcs;

	//msg("DCS Encoded:tp_dcs =%d =========",tp_dcs);

	//TP-VP
	tpdu_submit.vp.vp_rel_type = SMS_VP_REL_6H;
	tpdu_submit.vp.vp_type = SMS_VP_RELATIVE;
	tpdu_submit.vp.vp_value = 0x01;

	sms_util_encode_validity (&tp_vp, &tpdu_submit.vp);

	//tp_vp =65;
	packet[index++] = tp_vp;
	//msg("Validity Encoded :tp_vp =%d =========",tp_vp);

	// UDL
	packet[index++] = msg_len;

	// TP-UD

	if (tpdu_submit.dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
		int pos;
		pos = sms_util_pack_gsm_code (packet + index, msg, msg_len);
		index += pos;
	}

	for (i = 0; i < index; i++) {
		msg("[%02x]", packet[i]);
		if (i % 10 == 9)
			msg("\n");
	}
	msg("\n");

	memcpy (send_sms_info->send_data.tpdu, packet, index);
	send_sms_info->send_data.tpdu_length = index;

	msg("call tapi_sms_send()");

	msg("Length : %d", send_sms_info->send_data.tpdu_length);

	rt = tapi_sms_send (handle, send_sms_info, on_send_sms, NULL );
	CHECK_RT(rt);
	return 0;
}

static gboolean __decode_cb_msg (unsigned int length, char *pdu)
{
	unsigned int msg_length;
	int msg_id;
	int serial_number = 0;
	SmsCodingScheme dcs;
	unsigned char page[2];
	unsigned int contents_length;
	char contents[TEL_SMS_CB_DATA_SIZE_MAX];
	unsigned char gs;
	unsigned char msg_code;
	unsigned char update_number;

	serial_number = (pdu[2] << 8) | pdu[1];

	gs = (pdu[2] & 0xC0) >> 6;  //bit 7, bit 6

	msg_code = ( ( (pdu[2] & 0x3F) << 4) | pdu[3]) & 0xF0;

	update_number = pdu[3] & 0x0F; //bit3, bit2, bit1, bit0

	msg_id = pdu[4] << 4 | pdu[3];

	sms_util_decode_dcs(&dcs, pdu[6]);

	page[0] = pdu[7] & 0x0F; //total page
	page[1] = (pdu[7] & 0xF0) >> 4; //current page

	msg_length = length - 2;
	contents_length = msg_length - 6;

	if (dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
		sms_util_unpack_gsm_code (contents, &pdu[8], msg_length);
	} else {
		memcpy (contents, &pdu[8], msg_length);
		contents[msg_length] = 0;
	}

	msg("*************************************\n");
	msg("serial number : 0x%04x \n" , serial_number);
	msg("Geographical Scope: 0x%x\n" , gs);
	msg("message code : 0x%x\n", msg_code);
	msg("update Number : 0x%x\n",update_number);
	msg("message identifier :  0x%x\n" , msg_id);
	msg("DCS-Commpressed?: %d (1: true)\n" , dcs.compressed);
	msg("DCS-coding_group_type:	%x\n" , dcs.coding_group_type);
	msg("contents bytes size : %d\n", contents_length);
	msg("page : (%d /%d) (page /total ) \n", page[1], page[0]);
	msg("contents: %s\n", contents);
	msg("*************************************\n");

	return TRUE;
}

static int __decode_sms_deliver_tpdu (unsigned int tpdu_len, char *tpdu)
{
	TpduSmsDeliver tpdu_deliver;
	TmDateTime time_stamp;
	unsigned int org_addr_len = 0;
	char dialling_num[SMS_ADDRESS_LEN_MAX + 1];
	char org_addr[SMS_ADDRESS_LEN_MAX + 1]; //originating number
	int org_ton, org_npi;
	int position = 0;

	/* TP-MTI, TP-MMS, TP-SRI, TP_UDHI, TP-RP */
	tpdu_deliver.msg_type = SMS_TPDU_DELIVER;
	tpdu_deliver.mms = (tpdu[position] & 0x04) ? TRUE : FALSE; // bit 2 (Message Type Indicator)
	tpdu_deliver.sri = (tpdu[position] & 0x20) ? TRUE : FALSE; // bit 5 (Status Report Indication)
	tpdu_deliver.udhi = (tpdu[position] & 0x40) ? TRUE : FALSE;// bit 6 (User Data Header Indicator)
	tpdu_deliver.rp = (tpdu[position] & 0x80) ? TRUE : FALSE;// bit 7 (Reply Path)

	position++;

	/* TP-OA */

	memset (dialling_num, 0, sizeof (dialling_num));

	org_addr_len = sms_util_decode_addr_field (dialling_num, &tpdu[position],
			&org_ton, &org_npi);
	if (org_addr_len > SMS_ADDRESS_LEN_MAX - 1)
		org_addr_len = SMS_ADDRESS_LEN_MAX - 1;

	position += 2;

	position += (org_addr_len + 1) / 2;
	msg("pos = %d, length = %d", position, org_addr_len);

	if (org_ton == SMS_TON_INTERNATIONAL) {
		org_addr[0] = '+';
		memcpy (&org_addr[1], dialling_num, org_addr_len - 1);
		org_addr[org_addr_len + 1] = '\0';
	} else {
		memcpy (org_addr, dialling_num, org_addr_len);
		org_addr[org_addr_len] = '\0';
	}

	/* TP-PID */
	tpdu_deliver.pid= tpdu[position];

	position++;

	/* TP-DCS */
	sms_util_decode_dcs(&tpdu_deliver.dcs, tpdu[position]);
	dcs_pos = position;
	position++;

	/* TP-SCTS */
	sms_util_decode_time_stamp (&tpdu[position], &time_stamp);

	position += 7;

	/* TP-UDL */
	tpdu_deliver.udl = tpdu[position];

	if (tpdu_deliver.udl > TEL_SMS_SMDATA_SIZE_MAX)
		tpdu_deliver.udl = TEL_SMS_SMDATA_SIZE_MAX;

	position++;

	/* TP-UD */
	tpdu_deliver.user_data = g_try_malloc0(TEL_SMS_SMDATA_SIZE_MAX + 1);

	if (tpdu_deliver.dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
		char in_data[TEL_SMS_SMDATA_SIZE_MAX + 1];

		memset (in_data, 0x00, TEL_SMS_SMDATA_SIZE_MAX + 1);

		memcpy ((void*) in_data, (void*) &tpdu[position], tpdu_deliver.udl);

		sms_util_unpack_gsm_code (tpdu_deliver.user_data, in_data,
				(unsigned char) tpdu_deliver.udl);
	}

	msg("*************************************");
	msg("Msg Type: TPDU_DELIVER");

	if (tpdu_deliver.rp) {
		msg("RP: Set Reply Path");
	} else {
		msg("RP: Not set Reply Path");
	}

	if (tpdu_deliver.sri) {
		msg("SRI: A status report shall be returned to the SME");
	} else {
		msg("SRI: A status report shall be not returned to the SME");
	}

	if (tpdu_deliver.mms) {
		msg("MMS: No more messages are waiting for the MS in this SC");
	} else {
		msg("MMS: More messages are waiting for the MS in this SC");
	}

	if (tpdu_deliver.udhi) {
		msg("UDHI: Contains a Header in addition to the short message.");
	} else {
		msg("UDHI: Only short message.");
	}

	msg("DcsClass : %x (4 means Class None) ", tpdu_deliver.dcs.class_type);

	msg("From : %10s", org_addr);
	msg("Time : %d-%d-%d  , %d:%d:%d", time_stamp.year, time_stamp.month,
			time_stamp.day, time_stamp.hour, time_stamp.minute,
			time_stamp.second);
	msg("Message : %s ", tpdu_deliver.user_data);
	msg("*************************************");

	g_free(tpdu_deliver.user_data);

	return dcs_pos;
}

static void __decode_sms_status_report_tpdu(unsigned int tpdu_len, char *tpdu)
{
	TpduSmsStatusReport status_report;
	TmDateTime time_stamp;
	TmDateTime discharge_time;
	unsigned int rcp_addr_len = 0;
	char dialling_num[SMS_ADDRESS_LEN_MAX];
	char rcp_addr[SMS_ADDRESS_LEN_MAX]; //recipient number
	int rcp_ton, rcp_npi;
	int position = 0;
	unsigned int mask;

	/* TP-MTI, TP MMS, TP SRQ, TP_UDHI */
	status_report.msg_type = tpdu[position] & 0x03; // bit 0,1
	status_report.mms = tpdu[position] & 0x04 ? TRUE : FALSE; // bit 2
	status_report.srq = tpdu[position] & 0x20 ? TRUE : FALSE; //bit 5
	status_report.udhi = tpdu[position] & 0x40 ? TRUE : FALSE;	 //bit 6

	position++;

	/* TP-MR */
	status_report.mr = tpdu[position];
	position++;

	/* TP-RA */
	memset (dialling_num, 0, sizeof (dialling_num));
	sms_util_decode_addr_field(dialling_num, &tpdu[position], &rcp_ton, &rcp_npi);
	position += 2;

	rcp_addr_len = strlen ((char*) dialling_num);
	position += (rcp_addr_len + 1) / 2;

	if (rcp_ton == SMS_TON_INTERNATIONAL) {
		rcp_addr[0] = '+';
		memcpy (&rcp_addr[1], dialling_num, rcp_addr_len);
		rcp_addr[rcp_addr_len + 1] = '\0';
	} else {
		memcpy (rcp_addr, dialling_num, rcp_addr_len);
		rcp_addr[rcp_addr_len] = '\0';
	}

	status_report.rcp_addr.dial_num_len = rcp_addr_len + 1;
	memcpy (status_report.rcp_addr.dialling_num, rcp_addr, rcp_addr_len + 1);
	status_report.rcp_addr.npi = rcp_npi;
	status_report.rcp_addr.ton = rcp_ton;

	/* TP-SCTS */
	sms_util_decode_time_stamp(&tpdu[position], &time_stamp);
	position += 7;

	/* TP-DT */
	sms_util_decode_time_stamp (&tpdu[position], &discharge_time);
	position += 7;

	/* TP-ST */
	status_report.status = tpdu[position];
	position++;

	/* TP-PI */
	status_report.para_ind = tpdu[position];

	mask = status_report.para_ind;

	if ( (mask != 0xFF) && (mask != 0)) {
		/* Increment only if mask is valid */
		position++;

		/* TP-PID */
		if (mask & 0x01) {
			status_report.pid = tpdu[position];
			position++;
		}
		/* TP-DCS */
		if (mask & 0x02) {
			sms_util_decode_dcs(&status_report.dcs, tpdu[position]);
			position++;
		}
		if (mask & 0x04) {
			/* TP-UDL */
			status_report.udl = tpdu[position];
			position++;

			/* TP-UD */
			status_report.user_data = g_try_malloc0(TEL_SMS_SMDATA_SIZE_MAX + 1);

			if (status_report.dcs.alphabet_type == SMS_ALPHABET_DEFAULT) {
				char in_data[TEL_SMS_SMDATA_SIZE_MAX + 1];

				memset (in_data, 0x00, TEL_SMS_SMDATA_SIZE_MAX + 1);

				memcpy ((void*) in_data, (void*) &tpdu[position],
						status_report.udl);

				sms_util_unpack_gsm_code (status_report.user_data, in_data,
						(unsigned char) status_report.udl);
			}

		}
	}

	msg("*************************************");
	msg("Msg Type: STATUS_REPORT");
	if (status_report.mms) {
		msg("MMS: No more messages are waiting for the MS in this SC");
	} else {
		msg("MMS: More messages are waiting for the MS in this SC");
	}

	if (status_report.srq) {
		msg("SRQ: the result of an SMS COMMAND ");
	} else {
		msg("SRQ: the result of a SMS SUBMIT.");
	}

	if (status_report.udhi) {
		msg("UDHI: Contains a Header in addition to the short message.");
	} else {
		msg("UDHI: Only Short Message");
	}

	msg("STATUS:%x", status_report.status);
	if (status_report.status == 0x00) {
		msg("STATUS : Short message received by the SME");
	} else if (status_report.status == 0x01) {
		msg("STATUS : Short message forwarded by the SC to the SMEbut the SC is unable to confirm delivery");
	} else {
		msg("STATUS : Short message replaced by the SC");
	}

	msg("Recipient Number : %s", rcp_addr);
	msg("SC Time Stamp : %d-%d-%d  , %d:%d:%d", time_stamp.year,
			time_stamp.month, time_stamp.day, time_stamp.hour,
			time_stamp.minute, time_stamp.second);
	msg("Discharge Time : %d-%d-%d  , %d:%d:%d", discharge_time.year,
			discharge_time.month, discharge_time.day, discharge_time.hour,
			discharge_time.minute, discharge_time.second);

	if (mask & 0x04)
		msg("Message : %s ", status_report.user_data);

	msg("*************************************");
	g_free (status_report.user_data);
}

/* Event Callbacks */
static void on_noti_sms_incom_msg (TelHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	TelSmsDatapackageInfo *datapackage = NULL;
	char *temp_tpdu;
	unsigned int tpdu_len = 0;
	unsigned char MTI = 0;
	unsigned char MMS = 0;
	unsigned char SRI = 0;
	unsigned char UDHI = 0;
	unsigned char RP = 0;
	int position = 0;
	TelReturn return_status;
	SmsCodingScheme dcs;

	msgb("event(%s) received !!", TEL_NOTI_SMS_INCOM_MSG);

	if (data == NULL ) {
		msg("TEL_NOTI_SMS_INCOM_MSG data is Null");
		return;
	}

	datapackage = (TelSmsDatapackageInfo *)data;

	RP = datapackage->tpdu[position] & 0x80;
	UDHI = datapackage->tpdu[position] & 0x40;
	SRI = datapackage->tpdu[position] & 0x20;
	MMS = datapackage->tpdu[position] & 0x04;
	MTI = datapackage->tpdu[position] & 0x03;

	msg("RP [%x]", RP);
	msg("UDHI [%x]", UDHI);
	msg("SRI [%x]", SRI);
	msg("MMS [%x]", MMS);
	msg("MTI [%02x]", MTI);

	tpdu_len = datapackage->tpdu_length;
	temp_tpdu = malloc (sizeof(unsigned char) * tpdu_len);
	memcpy (temp_tpdu, &datapackage->tpdu[position], tpdu_len);

	if (MTI == SMS_TPDU_DELIVER) {
		__decode_sms_deliver_tpdu (tpdu_len, (char *)temp_tpdu);

		msg("dcs_pos : %d", dcs_pos);

		sms_util_decode_dcs(&dcs, datapackage->tpdu[position + dcs_pos]);

		{
			TelSmsSimDataInfo wdata;

			memset (&wdata, 0, sizeof(TelSmsSimDataInfo));
			memcpy (&(wdata.data.sca), &(datapackage->sca),
					sizeof(TelSmsSca));

			wdata.data.tpdu_length = datapackage->tpdu_length;
			memcpy (wdata.data.tpdu, datapackage->tpdu, datapackage->tpdu_length);

			wdata.status= TEL_SMS_STATUS_MT_READ;

			return_status = tapi_sms_write_sms_in_sim (handle, &wdata,
					on_resp_write_sms_in_sim, NULL );
			msg("return_status for tapi_sms_write_sms_in_sim(): [%d]", return_status);
		}
	} else if (MTI == SMS_TPDU_STATUS_REPORT) {
		__decode_sms_status_report_tpdu(tpdu_len, temp_tpdu);
	}

	TelSmsDeliverReportInfo *del_report = NULL;

	del_report = malloc (sizeof(TelSmsDeliverReportInfo));

	memset (del_report, 0, sizeof(TelSmsDeliverReportInfo));

	memcpy(&(del_report->data.sca), &(datapackage->sca), sizeof(TelSmsSca));

	del_report->data.tpdu[0] = SMS_TPDU_DELIVER_REPORT;	//TP-UDHI[bit6] : 0
	del_report->data.tpdu[1] = 0;// TP-PI = 0; bit2: TP-UDL bit1: TP-DCS bit0: TP-PID (No option field)
	del_report->data.tpdu_length = 2;
	del_report->report = TEL_SMS_DELIVERY_REPORT_SUCCESS;

	msg("***receive message (Api:SendDeliverreport)****");

	return_status = tapi_sms_send_deliver_report (handle, del_report, on_resp_sms_delivery_report, NULL );
	msg("return_status for tapi_sms_send_deliver_report(): [%d]", return_status);

	free (del_report);
	free (temp_tpdu);
}

static void on_noti_sms_cb_incom_msg (TelHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	TelSmsCbMsgInfo *cb_msg;
	char pdu[TEL_SMS_CB_DATA_SIZE_MAX + 3] = {0, };

	msg("");
	msgb("event(%s) received !!", TEL_NOTI_SMS_CB_INCOM_MSG);

	if (!data) {
		msg("TEL_NOTI_SMS_CB_INCOM_MSG data is null");
		return;
	}
	cb_msg = (TelSmsCbMsgInfo *)data;

	msg("cb_type[%d]", cb_msg->cb_type);
	msg("cb_msg_length[%d]", cb_msg->length);

	pdu[0] = cb_msg->cb_type;
	pdu[1] = cb_msg->length;
	memcpy(&(pdu[2]), cb_msg->cb_data, TEL_SMS_CB_DATA_SIZE_MAX + 1);

	__decode_cb_msg(cb_msg->length, pdu);
}

static void on_noti_sms_etws_incom_msg (TelHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	TelSmsEtwsMsgInfo *etws_msg;

	msg("event [%s] received !!!", TEL_NOTI_SMS_ETWS_INCOM_MSG);

	if (!data) {
		msg("TEL_NOTI_SMS_ETWS_INCOM_MSG data is null");
		return;
	}

	etws_msg = (TelSmsEtwsMsgInfo *)data;

	msg("etws_type[%d]", etws_msg->etws_type);
	msg("etws_length[%d]", etws_msg->length);
	/* TODO */
}

static void on_noti_sms_memory_status (TelHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	gboolean *mem_status;
	msg("event [%s] received !!!", TEL_NOTI_SMS_SIM_MEMORY_STATUS);

	if (!data) {
		msg("TEL_NOTI_SMS_SIM_MEMORY_STATUS data is null");
		return;
	}

	mem_status = (gboolean *)data;
	msg("sms memory status[%s]", (*mem_status == TRUE) ? "Available" : "Not Available");
}

static void on_noti_sms_init_status (TelHandle *handle, const char *noti_id,
		void *data, void *user_data)
{
	gboolean *ready_status;

	msg("event [%s] received !!!", TEL_NOTI_SMS_INIT_STATUS);

	if (!data) {
		msg("TEL_NOTI_SMS_INIT_STATUS data is null");
		return;
	}

	ready_status = (gboolean *)data;
	msg("[%s]", (*ready_status == TRUE) ? "Device Ready" : "Device Not Ready");
}

/* Requests and Responses */
static void on_resp_write_sms_in_sim(TelHandle *handle, int result, void *data, void *user_data)
{
	unsigned int *index;
	msg("tapi_sms_write_sms_in_sim() response received");
	CHECK_SMS_RESULT(result);

	if (!data) {
		msg("data is null");
		return;
	}

	index = (unsigned int *)data;
	msg("index[%d]", *index);
}

static void on_resp_sms_delivery_report(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_send_deliver_report() response received");
	CHECK_SMS_RESULT(result);
}

static int run_get_msg_sim_memory_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	gboolean memory_full;
	TelReturn rt = 0;

	msg("call tapi_sms_get_sim_memory_status()");

	rt = tapi_sms_get_sim_memory_status(handle, &memory_full);
	CHECK_RT(rt);

	msg("SMS SIM memory status: [%s]", ((memory_full == TRUE) ? "FULL" : "NOT FULL"));

	return 0;
}

static int run_get_init_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	gboolean init_status;
	TelReturn rt = 0;

	msg("call tapi_sms_get_init_status()");

	rt = tapi_sms_get_init_status(handle, &init_status);
	CHECK_RT(rt);

	msg("init status: [%s]", ((init_status == TRUE) ? "INITIALIZED" : "NOT INITIALIZED"));

	return 0;
}

static void on_set_msg_status(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_set_message_status() response received");
	CHECK_SMS_RESULT(result);
}

static int run_set_msg_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelSmsStatusInfo sms_status_info;
	TelReturn rt = 0;

	msg("call tapi_sms_set_message_status()");

	memset(&sms_status_info, 0, sizeof(TelSmsStatusInfo));

	sms_status_info.index = atoi(data_sms_index);
	sms_status_info.status = atoi(data_sms_msg_status);

	msg("index[%d]", sms_status_info.index);
	msg("msg status[%d]", sms_status_info.status);

	rt = tapi_sms_set_message_status(handle, &sms_status_info, on_set_msg_status, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_set_memory_status(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_set_memory_status() response received");
	CHECK_SMS_RESULT(result);
}

static int run_set_memory_status(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	gboolean mem_status;
	TelReturn rt = 0;

	msg("call tapi_sms_set_memory_status()");

	mem_status = atoi(data_sms_memory_status);
	msg("mem_status: [%s]", (mem_status == TRUE) ? "AVAILABLE" : "NOT AVAILABLE");

	rt = tapi_sms_set_memory_status(handle, mem_status, on_set_memory_status, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_get_sca(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSmsSca *sca;

	msg("tapi_sms_get_sca() response received");
	CHECK_SMS_RESULT(result);

	if(!data) {
		msg("get sca data is null");
		return;
	}

	sca = (TelSmsSca *)data;

	msg("sca_ton[%d]", sca->ton);
	msg("sca_npi[%d]", sca->npi);
	msg("sca_number[%s]", sca->number);
}

static int run_get_sca(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sms_get_sca()");

	rt = tapi_sms_get_sca(handle, on_get_sca, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_set_sca(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_set_sca() response received");
	CHECK_SMS_RESULT(result);
}

static int run_set_sca(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelSmsSca sca;
	TelReturn rt = 0;

	msg("call tapi_sms_set_sca()");

	memset(&sca, 0, sizeof(TelSmsSca));

	sca.ton = atoi(data_sms_sca_ton);
	sca.npi = atoi(data_sms_sca_npi);
	g_strlcpy(sca.number, data_sms_sca_number, TEL_SMS_SCA_LEN_MAX + 1);

	msg("sca_ton[%d]", sca.ton);
	msg("sca_npi[%d]", sca.npi);
	msg("sca_number[%s]", sca.number);

	rt = tapi_sms_set_sca(handle, &sca, on_set_sca, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_get_parameters(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSmsParamsInfoList *param_list;
	unsigned int i = 0;

	msg("tapi_sms_get_parameters() response received");
	CHECK_SMS_RESULT(result);

	if (!data) {
		msg("get parameters data is null");
		return;
	}

	param_list = (TelSmsParamsInfoList *)data;

	msg("count[%d]", param_list->count);

	for(i = 0; i < param_list->count; i++) {
		msg("index[%d]", param_list->params[i].index);
		msg("sca_ton[%d]", param_list->params[i].sca.ton);
		msg("sca_npi[%d]", param_list->params[i].sca.npi);
		msg("sca_number[%s]", param_list->params[i].sca.number);
		msg("validity period[%d]", param_list->params[i].vp);
	}
}

static int run_get_parameters(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sms_get_parameters()");

	rt = tapi_sms_get_parameters(handle, on_get_parameters, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_set_parameters(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_set_parameters() response received");
	CHECK_SMS_RESULT(result);
}

static int run_set_parameters(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelSmsParamsInfo params;
	TelReturn rt = 0;

	msg("call tapi_sms_set_parameters()");

	memset(&params, 0, sizeof(TelSmsParamsInfo));

	params.index = atoi(data_sms_set_params_index);
	params.sca.ton = atoi(data_sms_sca_ton);
	params.sca.npi = atoi(data_sms_sca_npi);
	g_strlcpy(params.sca.number, data_sms_sca_number, TEL_SMS_SCA_LEN_MAX+1);
	params.vp = atoi(data_sms_set_params_vp);

	msg("index[%d]", params.index);
	msg("sca_ton[%d]", params.sca.ton);
	msg("sca_npi[%d]", params.sca.npi);
	msg("sca_num[%s]", params.sca.number);
	msg("validity period[%d]", params.vp);

	rt = tapi_sms_set_parameters(handle, &params, on_set_parameters, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_get_cb_config(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSmsCbConfigInfo *config;
	unsigned int i = 0;

	msg("tapi_sms_get_cb_config() response received");
	CHECK_SMS_RESULT(result);

	if (!data) {
		msg("get cb config data is null");
		return;
	}

	config = (TelSmsCbConfigInfo *)data;

	msg("ebabled[%s]", (config->cb_enabled) ? "cb enabled" : "cb disabled");
	msg("msg_id range count[%d]", config->msg_id_range_cnt);

	for(i = 0; i < config->msg_id_range_cnt; i++)
		msg("[%d].from msg_id[%d] to msg_id[%d] selected[%s]", i+1,
		config->msg_ids[i].from_msg_id, config->msg_ids[i].to_msg_id,
		(config->msg_ids[i].selected) ? "YES" : "NO");
}

static int run_get_cb_config(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sms_get_cb_config()");

	rt = tapi_sms_get_cb_config(handle, on_get_cb_config, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_set_cb_config(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_set_cb_config() response received");
	CHECK_SMS_RESULT(result);
}

static int run_set_cb_config(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelSmsCbConfigInfo cb_config;
	TelReturn rt = 0;
	int ret = 0;
	unsigned int i = 0;

	msg("call tapi_sms_set_cb_config()");

	memset(&cb_config, 0, sizeof(TelSmsCbConfigInfo));

	cb_config.cb_enabled = atoi(data_sms_set_cb_status);
	cb_config.msg_id_range_cnt = atoi(data_sms_set_cb_msg_id_rng_cnt);

	for(i = 0; i < cb_config.msg_id_range_cnt; i++) {
		msg("[%d].Enter from_msg_id", i+1);
		ret = read(0, data_sms_set_cb_from_msg_id, sizeof(data_sms_set_cb_from_msg_id));
		if (ret <= 0) {
			msg("read 'from_msg_id' returned error: [%d]", ret);
			return -1;
		}
		cb_config.msg_ids[i].from_msg_id = atoi(data_sms_set_cb_from_msg_id);

		msg("[%d].Enter to_msg_id", i+1);
		ret = read(0, data_sms_set_cb_to_msg_id, sizeof(data_sms_set_cb_to_msg_id));
		if (ret <= 0) {
			msg("read 'to_msg_id' returned error: [%d]", ret);
			return -1;
		}
		cb_config.msg_ids[i].to_msg_id = atoi(data_sms_set_cb_to_msg_id);

		msg("[%d].Enter msg_id selected", i+1);
		ret = read(0, data_sms_set_cb_msg_id_selected, sizeof(data_sms_set_cb_msg_id_selected));
		if (ret <= 0) {
			msg("read 'selected' returned error: [%d]", ret);
			return -1;
		}
		cb_config.msg_ids[i].selected = atoi(data_sms_set_cb_msg_id_selected);
	}

	msg("cb_status[%s]", (cb_config.cb_enabled) ? "cb enabled" : "cb disabled");
	msg("msg_id range count[%d]", cb_config.msg_id_range_cnt);

	for(i = 0; i < cb_config.msg_id_range_cnt; i++)
		msg("[%d].from msg_id[%d] to msg_id[%d] selected[%s]", i+1,
		cb_config.msg_ids[i].from_msg_id, cb_config.msg_ids[i].to_msg_id,
		(cb_config.msg_ids[i].selected) ? "YES" : "NO");

	rt = tapi_sms_set_cb_config(handle, &cb_config, on_set_cb_config, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_get_msg_count(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSmsStoredMsgCountInfo *sms_count_info;
	unsigned int i = 0;

	msg("tapi_sms_get_count() response received");
	CHECK_SMS_RESULT(result);

	if (!data) {
		msg("get msg count data is null");
		return;
	}

	sms_count_info = (TelSmsStoredMsgCountInfo *)data;

	msg("total count[%d]", sms_count_info->total_count);
	msg("used count [%d]", sms_count_info->used_count);

	msg("Index List :");
	for(i = 0; i < sms_count_info->used_count; i++)
		msg("[%d]", sms_count_info->index_list[i]);

}

static int run_get_msg_count(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	TelReturn rt = 0;

	msg("call tapi_sms_get_count()");

	rt = tapi_sms_get_count(handle, on_get_msg_count, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_delete_sms(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_delete_in_sim() response received");
	CHECK_SMS_RESULT(result);
}

static int run_delete_sms(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	unsigned int index;
	TelReturn rt = 0;

	msg("call tapi_sms_delete_sms_in_sim()");

	index = atoi(data_sms_index);
	msg("index[%d]", index);

	rt = tapi_sms_delete_sms_in_sim(handle, index, on_delete_sms, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_read_msg_in_sim(TelHandle *handle, int result, void *data, void *user_data)
{
	TelSmsSimDataInfo *read_sms_data;

	msg("tapi_sms_read_sms_in_sim() response received");
	CHECK_SMS_RESULT(result);

	if (!data) {
		msg("read sms in sim data is null");
		return;
	}

	read_sms_data = (TelSmsSimDataInfo *)data;

	msg("status[%d]", read_sms_data->status);
	msg("sca_ton[%d]", read_sms_data->data.sca.ton);
	msg("sca_npi[%d]", read_sms_data->data.sca.npi);
	msg("sca_number[%s]", read_sms_data->data.sca.number);

	__decode_sms_deliver_tpdu(read_sms_data->data.tpdu_length, (char *)read_sms_data->data.tpdu);
}

static int run_read_msg_in_sim(MManager *mm, struct menu_data *menu)
{
	TelHandle *handle = menu_manager_ref_user_data(mm);
	unsigned int index;
	TelReturn rt = 0;

	msg("call tapi_sms_read_sms_in_sim()");

	index = atoi(data_sms_index);
	msg("index[%d]", index);

	rt = tapi_sms_read_sms_in_sim(handle, index, on_read_msg_in_sim, NULL);
	CHECK_RT(rt);

	return 0;
}

static void on_send_sms(TelHandle *handle, int result, void *data, void *user_data)
{
	msg("tapi_sms_send() response received");
	CHECK_SMS_RESULT(result);
}

static int run_send_sms(MManager *mm, struct menu_data *menu)
{
	TelSmsSendInfo send_sms_info;
	char dialling_num[SMS_ADDRESS_LEN_MAX + 1];
	char message[512];
	unsigned int dial_num_len = 0, msg_len = 0;

	msg("call tapi_sms_send()");

	memset(&send_sms_info, 0, sizeof(TelSmsSendInfo));

	send_sms_info.more_msgs = atoi(data_sms_send_more_msgs);
	msg("more msgs: [%s]", (send_sms_info.more_msgs == TRUE)? "Yes" : "No");

	/* sca */
	send_sms_info.send_data.sca.ton = atoi(data_sms_sca_ton);
	send_sms_info.send_data.sca.npi = atoi(data_sms_sca_npi);
	if (g_strlcpy(send_sms_info.send_data.sca.number, data_sms_sca_number,
			TEL_SMS_SCA_LEN_MAX + 1) > TEL_SMS_SCA_LEN_MAX) {
		msg("Invalid sca number length entered");
		return 0;
	}
	msg("sca_ton[%d] sca_npi[%d] sca_num[%s]", send_sms_info.send_data.sca.ton,
		send_sms_info.send_data.sca.npi, send_sms_info.send_data.sca.number);

	/* dialling number */
	if (g_strlcpy(dialling_num, data_sms_send_msg_dialling_num,
			SMS_ADDRESS_LEN_MAX + 1) > SMS_ADDRESS_LEN_MAX) {
		msg("Invalid dialling num length entered");
		return 0;
	}
	dial_num_len = strlen(dialling_num);

	/* message */
	g_strlcpy(message, data_sms_send_msg, strlen(data_sms_send_msg)+1);
	msg_len = strlen(message);

	msg("To: [%s]", dialling_num);
	msg("Message: [%s]", message);
	msg("dial num length[%d] msg_len[%d]", dial_num_len, msg_len);

	__encode_and_send_sms_submit_tpdu (mm, dialling_num, dial_num_len, message, msg_len, &send_sms_info);

	return 0;
}

/* SubMenu */
static struct menu_data menu_sms_send_sms[] = {
	{ "1", "more messages (0=No 1=Yes)", NULL, NULL, data_sms_send_more_msgs },
	{ "2", "sca ton (0=Unknown 1=International 2=National)", NULL, NULL, data_sms_sca_ton },
	{ "3", "sca npi (0=Unknown 1=Telephony)", NULL, NULL, data_sms_sca_npi },
	{ "4", "sca number", NULL, NULL, data_sms_sca_number },
	{ "5", "message", NULL, NULL, data_sms_send_msg},
	{ "6", "dialling number (<= 20)", NULL, NULL, data_sms_send_msg_dialling_num},
	{ "7", "run", NULL, run_send_sms, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_read_msg_in_sim[] = {
	{ "1", "index", NULL, NULL, data_sms_index },
	{ "2", "run", NULL, run_read_msg_in_sim, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_delete_msg_in_sim[] = {
	{ "1", "index", NULL, NULL, data_sms_index },
	{ "2", "run", NULL, run_delete_sms, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_get_msg_count[] = {
	{ "1", "run", NULL, run_get_msg_count, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_set_cb_config[] = {
	{ "1", "cb status (0=disabled 1=enabled)", NULL, NULL, data_sms_set_cb_status },
	{ "2", "msg id range count ( < 50)", NULL, NULL, data_sms_set_cb_msg_id_rng_cnt },
	{ "3", "run", NULL, run_set_cb_config, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_get_cb_config[] = {
	{ "1", "run", NULL, run_get_cb_config, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_set_parameters[] = {
	{ "1", "index", NULL, NULL, data_sms_set_params_index },
	{ "2", "sca ton(0=Unknown 1=International 2=National)", NULL, NULL, data_sms_sca_ton },
	{ "3", "sca npi (0=Unknown 1=Telephony)", NULL, NULL, data_sms_sca_npi },
	{ "4", "sca number", NULL, NULL, data_sms_sca_number },
	{ "5", "validity period", NULL, NULL, data_sms_set_params_vp },
	{ "6", "run", NULL, run_set_parameters, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_get_parameters[] = {
	{ "1", "run", NULL, run_get_parameters, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_set_sca[] = {
	{ "1", "sca ton(0=Unknown 1=International 2=National)", NULL, NULL, data_sms_sca_ton },
	{ "2", "sca npi (0=Unknown 1=Telephony)", NULL, NULL, data_sms_sca_npi },
	{ "3", "sca number", NULL, NULL, data_sms_sca_number },
	{ "4", "run", NULL, run_set_sca, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_get_sca[] = {
	{ "1", "run", NULL, run_get_sca, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_set_memory_status[] = {
	{ "1", "status (0=not available, 1=available)", NULL, NULL, data_sms_memory_status },
	{ "2", "run", NULL, run_set_memory_status, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_set_msg_status[] = {
	{ "1", "index", NULL, NULL, data_sms_index },
	{ "2", "status (0=MT_READ 1=MT_UNREAD 2=MO_SENT 3=MO_NOT_SENT 4=MO_DELIVERED" \
		"5=MO_DELIVERY_NOT_CONFIRMED 6=REPLACED )", NULL, NULL, data_sms_msg_status },
	{ "3", "run", NULL, run_set_msg_status, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_get_init_status[] = {
	{ "1", "run", NULL, run_get_init_status, NULL },
	{ NULL, NULL, },
};

static struct menu_data menu_sms_get_msg_sim_memory_status[] = {
	{ "1", "run", NULL, run_get_msg_sim_memory_status, NULL },
	{ NULL, NULL, },
};

/* Menu */
struct menu_data menu_sms[] = {
	{ "1", "Send SMS", menu_sms_send_sms, NULL, NULL},
	{ "2", "Read SMS in SIM", menu_sms_read_msg_in_sim, NULL, NULL},
	{ "3", "Delete SMS in SIM", menu_sms_delete_msg_in_sim, NULL, NULL},
	{ "4", "Get SMS count", menu_sms_get_msg_count, NULL, NULL},
	{ "5", "Set SMS cell broadcast configuration", menu_sms_set_cb_config, NULL, NULL},
	{ "6", "Get SMS cell broadcast configuration", menu_sms_get_cb_config, NULL, NULL},
	{ "7", "Set SMS parameters", menu_sms_set_parameters, NULL, NULL},
	{ "8", "Get SMS parameters", menu_sms_get_parameters, NULL, NULL},
	{ "9", "Set SCA", menu_sms_set_sca, NULL, NULL},
	{ "10", "Get SCA", menu_sms_get_sca, NULL, NULL},
	{ "11", "Set SMS memory status", menu_sms_set_memory_status, NULL, NULL},
	{ "12", "Set SMS msg status", menu_sms_set_msg_status, NULL, NULL},
	{ "13", "Get SMS init status", menu_sms_get_init_status, NULL, NULL},
	{ "14", "Get SMS SIM memory status", menu_sms_get_msg_sim_memory_status, NULL, NULL},
	{ NULL, NULL, },
};

void register_sms_event(TelHandle *handle)
{
	TelReturn ret;

    ret = tapi_register_event_id(handle, TEL_NOTI_SMS_INCOM_MSG, on_noti_sms_incom_msg, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_SMS_INCOM_MSG - Event register failed: [%d]", ret);

    ret = tapi_register_event_id(handle, TEL_NOTI_SMS_CB_INCOM_MSG, on_noti_sms_cb_incom_msg, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_SMS_CB_INCOM_MSG - Event register failed: [%d]", ret);

    ret = tapi_register_event_id(handle, TEL_NOTI_SMS_ETWS_INCOM_MSG, on_noti_sms_etws_incom_msg, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_SMS_ETWS_INCOM_MSG - Event register failed: [%d]", ret);

    ret = tapi_register_event_id(handle, TEL_NOTI_SMS_SIM_MEMORY_STATUS, on_noti_sms_memory_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_SMS_SIM_MEMORY_STATUS - Event register failed: [%d]", ret);

    ret = tapi_register_event_id(handle, TEL_NOTI_SMS_INIT_STATUS, on_noti_sms_init_status, NULL);
	if (ret != TEL_RETURN_SUCCESS)
		msg("TEL_NOTI_SMS_INIT_STATUS - Event register failed: [%d]", ret);
}
