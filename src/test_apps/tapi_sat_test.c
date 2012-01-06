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
#include <pthread.h>     /* pthread functions and data structures */
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <glib.h>

#include "TelDefines.h"
#include "ITapiCall.h"
#include "ITapiSs.h"
#include "TapiCommon.h"
#include "TelSim.h"
#include "TelErr.h"
#include "TelSatEnvelope.h"
#include "TelSatObj.h"
#include "TelSatProactvCmd.h"
#include "ITapiSat.h"
#include "TelUtility_ext.h"
#include "TelUtility.h"


TS_UINT g_hCall;
TelTapiCallInfo_t g_info;

extern int EvtDeliveryHandle ;


/***************************/
int tapi_thread_id;

int error_code;
unsigned int curr_cmd = 0;

tapi_simatk_select_item_ind_data_t sel_item_data;
tapi_simatk_resp_data_t tr_data;

typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}data_t;

unsigned char *ptrEvnts[] = {
"Telephony.Sat.UI.DisplayText",
"Telephony.Sat.UI.GetInKey",
"Telephony.Sat.UI.GetInputKey",
"Telephony.Sat.UI.PlayTone",
"Telephony.Sat.UI.Refresh",
"Telephony.Sat.UI.SetUpMenu",
"Telephony.Sat.UI.SelectItem",
"Telephony.Sat.UI.SendSms",
"Telephony.Sat.UI.SendSs",
"Telephony.Sat.UI.SendUssd",
"Telephony.Sat.UI.SetupCall",
"Telephony.Sat.ProvideLocalInfo",
"Telephony.Sat.SetupEventList",
"Telephony.Sat.SetupIdelMode",
"Telephony.Sat.UI.SendDtmf",
"Telephony.Sat.Idle.LanguageNotification",
"Telephony.Sat.UI.LaunchBrowser",
"Telephony.Sat.UI.EndProacativeSession",
"Telephony.Sat.Call.CallControlCnf",
"Telephony.Sat.SMS.MoSmsControlCnf",
"Telephony.Sat.SMS.Refresh",
"Telephony.Sat.Phonebook.Refresh",
"Telephony.Sat.Idle.Refresh",
"Telephony.Sat.SMS.SendSms",
"Telephony.Sat.Call.SetupCall",
"Telephony.Sat.SS.SendSs",
"Telephony.Sat.USSD.SendUssd",
"Telephony.Sat.Idle.SetupText",
"Telephony.Sat.DTMF.SendDtmf",
"User.WapBrowser.ESimAtkOpenURL",
"Telephony.Sat.UI.CmdProcEnd",
"Telephony.Sat.MenuSelectionCnf",
"Telephony.Sat.EventDownloadCnf"
};



int
sat_util_pack_7bit( TS_BYTE* pOut, const char* szData, int in_len )
{
	int i;
	int pos;
	int shift = 0;

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



void
test_send_tr(unsigned char gen_resp)
{
//Send TR
	int ret;
	unsigned char input;

	tr_data.tr_data.display_txt.result.general_result = gen_resp;
	tr_data.tr_data.display_txt.result.me_problem = 0x00;

	switch(gen_resp)
		{
			case 0x20:
			case 0x21:
			case 0x26:
			case 0x34:
			case 0x35:
			case 0x37:
			case 0x38:
			case 0x39:
			case 0x3a:
			case 0x3c:
			case 0x3d:
			{
				char scabuf[5] = {0};
				printf("additional response 0x00 ~ 0x0a:  0x\n");
				ret = read(0, scabuf, sizeof(scabuf));
				if (ret < 0)
				{
					if (errno == EINTR)
						perror("read(1)");
					return -1;
				}
				else if (ret == 0)
					return ret;

				input = (scabuf[0] -'0' )<< 4;
				input |= (scabuf[1]-'0')&0x0F;
				if(input >= 0x00 && input <= 0x3F)
				{
					printf("additional response gen_resp 0x%x - add_resp 0x%x\n", gen_resp, input);
					if(gen_resp == 0x39)
					{
						if(curr_cmd == TAPI_EVENT_SAT_UI_SEND_SS_IND)
							tr_data.tr_data.send_ss.additional_cc_problem_info = input;
						else if(curr_cmd == TAPI_EVENT_SAT_UI_SEND_USSD_IND)
							tr_data.tr_data.send_ussd.additional_cc_problem_info = input;
						else
							tr_data.tr_data.display_txt.result.me_problem = input;
					}
					else if(gen_resp == 0x34)
						tr_data.tr_data.send_ss.additional_ss_problem_info = input;
					else if(gen_resp == 0x37)
						tr_data.tr_data.send_ussd.additional_ussd_problem_info = input;
					else
						tr_data.tr_data.display_txt.result.me_problem = input;
				}

			}

			default:
				break;
		}

	printf(" [SAT] : test_send_tr ::: send TR. command_number=%d, type=0x%x\n",
								tr_data.command_number,	tr_data.command_type);
	TelTapiSatSendUiUserConfirm(tr_data.command_number ,tr_data.command_type,SAT_ENGINE_USER_CONFIRM_YES,NULL,0);
}


/*
void
test_call_control(void)
{
	tapi_call_setup_info_t call;
	unsigned short error;
	tapi_call_handle  call_handle;
	unsigned char num_dial[] = "+01234567890123456789";

	call.call_type = TAPI_CALL_TYPE_VOICE;
	memcpy(call.number,num_dial, sizeof(num_dial));
	call.identity_mode = 0x01;
	tapi_call_setup(&call, call_handle, &error);
}
*/

void
test_simatk_proactive_send_dtmf(tapi_simatk_send_dtmf_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : alpha_id present?= %d,  alphaID: %s\n",tst_data->alpha_id.is_present, tst_data->alpha_id.string);

	printf(" [SAT] : DTMF STRING: len = %d, USSD string = %s\n", tst_data->dtmf_string.string_length, tst_data->dtmf_string.dtmf_tone);

	if(tst_data->icon_id.is_present)
		printf(" \n[SAT] : icon identifier: icon_id=%d, icon_qulaifier = 0x%x\n", tst_data->icon_id.icon_identifier, tst_data->icon_id.icon_qualifier);
	else
		printf(" \n[SAT] : icon NOT present\n");

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
}

void
test_simatk_proactive_play_tone(tapi_simatk_play_tone_ind_data_t* tst_data)
{
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);
	printf(" [SAT] : command qulaifier: Vibrate_alert 0=optional, 1=required, %d\n", tst_data->command_detail.u.cmd_quali_play_tone.vibrate_alert);
	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);
	printf(" [SAT] : alpha identifier: is_alpha_present=%d, rawdcs=0x%x, str_len=%d, string=%s\n", tst_data->alpha_id.is_present,
				tst_data->alpha_id.dcs.raw_dcs, tst_data->alpha_id.string_len, tst_data->alpha_id.string);

	printf(" [SAT] : TONE: requested tone - 0x%02x\n", tst_data->tone.type);
	printf(" [SAT] : DURATION: interval=0x%x, time unit=0x%x\n",tst_data->duration.time_interval,tst_data->duration.time_unit);
	printf(" [SAT] : icon: is_icon_present=%d, icon id: %d\n",tst_data->icon_id.is_present, tst_data->icon_id.icon_identifier);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
}

void
test_simatk_proactive_get_input(tapi_simatk_get_input_ind_data_t* tst_data)
{
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : command qualifier: is_using_alphabet_set (1=ys, 0=using digits) [%d]\n",tst_data->command_detail.u.cmd_quali_get_input.is_using_alphabet_set);

	printf(" [SAT] : command qualifier: alpha_type = 0x%x, help info available? = %d\n",tst_data->command_detail.u.cmd_quali_get_input.alphabet_type, tst_data->command_detail.u.cmd_quali_get_input.is_help_info_available);

	printf(" [SAT] : command qualifier: is_me_echo_user_input? [%d], unpacked data? [%d]\n",tst_data->command_detail.u.cmd_quali_get_input.is_me_echo_user_input, tst_data->command_detail.u.cmd_quali_get_input.is_user_input_unpacked_format);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : text-dcs: compressed?=%d, alpha_format=0x%x\n",tst_data->text.dcs.is_compressed_format, tst_data->text.dcs.alphabet_format);

	printf(" [SAT] : text: string_length=%d, string=%s\n",tst_data->text.string_length, tst_data->text.string);

	printf(" [SAT] : text: response length: min =%d, max=%d\n",tst_data->response_length.min, tst_data->response_length.max);

	printf(" [SAT] : icon: is_icon_present=%d, icon id: %d\n",tst_data->icon_id.is_present, tst_data->icon_id.icon_identifier);

	printf(" [SAT] : Default Text: string length=%d\n",tst_data->default_text.string_length);

	printf(" [SAT] : Default Text: dcs=0x%02x, default string: %s\n",tst_data->default_text.dcs.alphabet_format, tst_data->default_text.string);

	if(!tst_data->command_detail.u.cmd_quali_get_input.is_user_input_unpacked_format)
		tr_data.tr_data.get_input.text.dcs.alphabet_format = TAPI_SAT_ALPHABET_FROMAT_SMS_DEFAULT;
	else
		tr_data.tr_data.get_input.text.dcs.alphabet_format = TAPI_SIMATK_ALPHABET_FROMAT_8BIT_DATA;
	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;

		if(tst_data->icon_id.is_present)
		tr_data.tr_data.get_input.result.general_result = TAPI_SAT_R_SUCCESS_BUT_REQUESTED_ICON_NOT_DISPLAYED;
	else
		tr_data.tr_data.get_input.result.general_result = 0x00;

}

void
test_simatk_proactive_get_inkey(tapi_simatk_get_inkey_ind_data_t* tst_data)
{
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : command qualifier: inkey_type 0== using character set, 1== yes/no type [%d]\n",tst_data->command_detail.u.cmd_quali_get_inkey.inkey_type);

	printf(" [SAT] : command qualifier: use_alpha_set? [%d], is_help info available? [%d]\n",tst_data->command_detail.u.cmd_quali_get_inkey.is_using_alphabet_set, tst_data->command_detail.u.cmd_quali_get_inkey.is_help_info_available);

	printf(" [SAT] : command qualifier: immideate response required? [%d], alphabet_type [%d]\n",tst_data->command_detail.u.cmd_quali_get_inkey.is_immediate_response_required, tst_data->command_detail.u.cmd_quali_get_inkey.alphabet_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : text-dcs: compressed?=%d, alpha_format=0x%x, msg_class=0x%x, raw_dcs=%d\n",tst_data->text.dcs.is_compressed_format, tst_data->text.dcs.alphabet_format,tst_data->text.dcs.msg_class, tst_data->text.dcs.raw_dcs);

	printf(" [SAT] : text: string_length=%d, string=%s\n",tst_data->text.string_length, tst_data->text.string);

	printf(" [SAT] : icon: is_icon_present=%d\n",tst_data->icon_id.is_present);

	printf(" [SAT] : DURATION: interval=0x%x, time unit=0x%x\n",tst_data->duration.time_interval,tst_data->duration.time_unit);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;

	if(tst_data->icon_id.is_present)
		tr_data.tr_data.get_inkey.result.general_result = TAPI_SAT_R_SUCCESS_BUT_REQUESTED_ICON_NOT_DISPLAYED;
	else
		tr_data.tr_data.get_inkey.result.general_result = 0x00;
}



void
test_simatk_event_download(tapi_simatk_event_download_type_t event)
{
	TelSatkEventDownloadData_t evt_dl;
	int requestId = -1;

	memset(&evt_dl, 0x00, sizeof(TelSatkEventDownloadData_t));

	printf(" [SAT] : EVENT DOWNLOAD\n");

	switch(event)
	{
		case TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE:
			evt_dl.event_download_type = TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE;
			break;

		case TAPI_EVENT_SAT_DW_TYPE_USER_ACTIVITY:
			evt_dl.event_download_type = TAPI_EVENT_SAT_DW_TYPE_USER_ACTIVITY;
			break;

		case TAPI_EVENT_SAT_DW_TYPE_LANGUAGE_SELECTION:
			{
				char scabuf[5] = {0};
				int ret = 0;

				printf(" en: english\n");
				printf(" de: german\n");
				ret = read(0, scabuf, sizeof(scabuf));
				if (ret < 0)
				{
					if (errno == EINTR)
						perror("read(1)");
					return -1;
				}
				else if (ret == 0)
					return ret;

				if(strncmp(scabuf, "en", 2) == 0)
					evt_dl.u.language_selection_event_req_Data.language = TAPI_SAT_LP_ENGLISH;
				else
					evt_dl.u.language_selection_event_req_Data.language = TAPI_SAT_LP_GERMAN;

			evt_dl.event_download_type = TAPI_EVENT_SAT_DW_TYPE_LANGUAGE_SELECTION;
			}
			break;

		case TAPI_EVENT_SAT_DW_TYPE_BROWSER_TERMINATION:
			evt_dl.event_download_type = TAPI_EVENT_SAT_DW_TYPE_BROWSER_TERMINATION;
			evt_dl.u.browser_termination_event_req_Data.browser_termination_cause = TAPI_SAT_BROWSER_TERMINATED_BY_USER;
			break;

		default:
			break;
	}
	TelTapiSatEventDownloadEnvelope(&evt_dl, &requestId);
}

void
test_simatk_proactive_lang_noti(tapi_simatk_language_notification_ind_data_t* tst_data)
{
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : command qualifier: language specified? = %d\n",tst_data->command_detail.u.cmd_quali_language_notification.bSpecificLanguageNotification);

	if(tst_data->command_detail.u.cmd_quali_language_notification.bSpecificLanguageNotification)
		printf(" [SAT] : command qualifier: language  = %d\n",tst_data->language.language);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;

}

void
test_simatk_proactive_provide_local_info(tapi_simatk_provide_local_info_ind_data_t* tst_data)
{

	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : command qualifier: requested info = 0x%02x\n",tst_data->command_detail.u.cmd_quali_provide_local_info);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	//tr_data.tr_data.provide_loc_info.other_info = TRUE;
	tr_data.tr_data.provide_loc_info.info_type = tst_data->command_detail.u.cmd_quali_provide_local_info;

	if(tst_data->command_detail.u.cmd_quali_provide_local_info == TAPI_SIMATK_CMD_QUALI_PROVIDE_LANGUAGE_SETTING)
	{
		tr_data.tr_data.provide_loc_info.u.language_info.language = TAPI_SIMATK_LP_ENGLISH;
	}
	else if(tst_data->command_detail.u.cmd_quali_provide_local_info == TAPI_SIMATK_CMD_QUALI_PROVIDE_DATE_TIME_AND_TIME_ZONE)
	{
		tr_data.tr_data.provide_loc_info.u.time_zone_info.year = 0x80;
		tr_data.tr_data.provide_loc_info.u.time_zone_info.month = 0x50;
		tr_data.tr_data.provide_loc_info.u.time_zone_info.day = 0x70;
		tr_data.tr_data.provide_loc_info.u.time_zone_info.hour = 0x41;
		tr_data.tr_data.provide_loc_info.u.time_zone_info.minute = 0x80;
		tr_data.tr_data.provide_loc_info.u.time_zone_info.second = 0x71;
		tr_data.tr_data.provide_loc_info.u.time_zone_info.time_zone = 0xFF;
	}
}

void
test_simatk_proactive_setup_idle_mode_text(tapi_simatk_setup_idle_mode_text_ind_data_t* tst_data)
{

	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : text-dcs: compressed?=%d, alpha_format=0x%x, msg_class=0x%x, raw_dcs=%d\n",tst_data->text.dcs.is_compressed_format, tst_data->text.dcs.alphabet_format,tst_data->text.dcs.msg_class, tst_data->text.dcs.raw_dcs);

	printf(" [SAT] : text: string_length=%d, string=%s\n",tst_data->text.string_length, tst_data->text.string);

	printf(" [SAT] : icon: is_icon_present=%d\n",tst_data->icon_id.is_present);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
}

void
test_simatk_proactive_display_text(tapi_simatk_display_text_ind_data_t* tst_data)
{

	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : command qualifier: clear=%d, priority=%d\n",tst_data->command_detail.u.cmd_quali_display_text.msg_clear,tst_data->command_detail.u.cmd_quali_display_text.msg_priority);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : text-dcs: compressed?=%d, alpha_format=0x%x, msg_class=0x%x, raw_dcs=%d\n",tst_data->text.dcs.is_compressed_format, tst_data->text.dcs.alphabet_format,tst_data->text.dcs.msg_class, tst_data->text.dcs.raw_dcs);

	printf(" [SAT] : text: string_length=%d, string=%s\n",tst_data->text.string_length, tst_data->text.string);

	printf(" [SAT] : icon: is_icon_present=%d\n",tst_data->icon_id.is_present);

	printf(" [SAT] : DURATION: interval=0x%x, time unit=0x%x\n",tst_data->duration.time_interval,tst_data->duration.time_unit);


	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
/*	tr_data.tr_data.display_txt.result.general_result = TAPI_SIMATK_R_SUCCESS;
	tr_data.tr_data.display_txt.result.me_problem = 0x00;
	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.display_txt.result.general_result);
	tapi_simatk_send_terminal_response(&tr_data);*/

}

void
test_simatk_send_menu_select_env(TS_BOOL help)
{
	TelSatMenuSelectionReqInfo_t env_data;
	char str[5] = {0,0};
	int item;
	char scabuf[5] = {0};
	int ret;
	int requestId = -1;

	ret = read(0, scabuf, sizeof(scabuf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	memcpy(str, scabuf, (strlen(scabuf)-1));

		item = atoi(str);

	env_data.item_identifier = item;
	env_data.is_help_requested = help;
	printf(" [SAT] : test_simatk_send_menu_select_env ::: MENU SELECT item_id= 0x%x, HELP?=%s\n",
								env_data.item_identifier,(env_data.is_help_requested)?"Reqd":"NOT Reqd");
	TelTapiSatMenuSelectionEnvelope(&env_data, &requestId);
}

void
test_simatk_proactive_refresh(tapi_simatk_refresh_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : command qualifier: REFRESH MODE=%d\n",tst_data->command_detail.u.cmd_quali_refresh);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : File Count=%d\n", tst_data->file_list.file_count);

	printf(" [SAT] : file ids:");
	for(i=0; i<tst_data->file_list.file_count;i++)
		printf("\tfile_#[%d] = 0x%04x",i,tst_data->file_list.file_id[i]);

	printf(" \n");


	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	/*tr_data.tr_data.refresh.result.general_result = TAPI_SIMATK_R_SUCCESS;
	tr_data.tr_data.refresh.result.me_problem = 0x00;
	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.refresh.result.general_result);
	tapi_simatk_send_terminal_response(&tr_data);*/
}


test_simatk_proactive_setup_menu(tapi_simatk_setup_menu_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : command qualifier: selection_pref=%d, help_info=%d\n",tst_data->command_detail.u.cmd_quali_setup_menu.selection_preference,tst_data->command_detail.u.cmd_quali_setup_menu.is_help_info_available);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : alpha identifier: is_alpha_present=%d, rawdcs=0x%x, str_len=%d, string=%s\n", tst_data->alpha_id.is_present,
				tst_data->alpha_id.dcs.raw_dcs, tst_data->alpha_id.string_len, tst_data->alpha_id.string);

	printf(" [SAT] : items NAI: list count=%d \n [SAT] : items NAI:",tst_data->item_next_action_ind_list.list_count);
	for(i=0; i<tst_data->item_next_action_ind_list.list_count;i++)
		printf("\tnai_ind[%d] = 0x%02x",i, tst_data->item_next_action_ind_list.list[i]);

	printf(" \n[SAT] : icon identifier: is icon present = %d\n",tst_data->icon_id.is_present);

	printf(" [SAT] : setup menu: menu_count = %d\n",tst_data->menu_item_count);
	for(i=0;i<tst_data->menu_item_count;i++)
		printf("\tmenu_item[%d]= %s, [menu_item_id = %d]\n",i, tst_data->menu_item[i].text, tst_data->menu_item[i].item_id);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	/*tr_data.tr_data.setup_menu.result.general_result = TAPI_SIMATK_R_SUCCESS;
	tr_data.tr_data.setup_menu.result.me_problem = 0x00;
	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.setup_menu.result.general_result);
	tapi_simatk_send_terminal_response(&tr_data);*/
}

void
test_simatk_proactive_send_sms(tapi_simatk_send_sms_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : send sms :is_packing_by ME rquired%d\n",tst_data->command_detail.u.cmd_quali_send_sms.is_packing_by_me_required);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : send sms: alpha_id present?= %d,  icon present? = %d, icon_id=%d\n",tst_data->alpha_id.is_present, tst_data->icon_id.is_present, tst_data->icon_id.icon_identifier);

	printf(" [SAT] : send sms: ALPHA Len = %d, Alpha ID: %s\n",tst_data->alpha_id.string_len, tst_data->alpha_id.string);
	printf(" [SAT] : send sms ADDRESS: ton=0x%x, npi=0x%x\n",tst_data->address.ton,tst_data->address.npi);

	printf(" [SAT] : send sms ADDRESS: len = %d, dialling Number = %s\n", tst_data->address.dialling_number_length, tst_data->address.dialling_number);

	printf(" [SAT] : send sms TPDU: len = %d, tpdu_data:\n",tst_data->sms_tpdu.data_length);
	for(i=0;i<tst_data->sms_tpdu.data_length;i++)
		printf("\t0x%02x",tst_data->sms_tpdu.data[i]);
	printf("\n");


	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	/*tr_data.tr_data.setup_event_list.result.general_result = TAPI_SIMATK_R_SUCCESS;
	tr_data.tr_data.setup_event_list.result.me_problem = 0x00;
	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.setup_event_list.result.general_result);
	tapi_simatk_send_terminal_response(&tr_data);*/
}

void
test_simatk_proactive_setup_evt_list(tapi_simatk_setup_event_list_ind_data_t* tst_data)
{

	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : setup menu: setup event count = %d\n",tst_data->event_list.event_list_count);
	for(i=0;i<tst_data->event_list.event_list_count;i++)
		printf("\t#[%d], [event = 0x%02x]\n",i+1, tst_data->event_list.list[i]);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	/*tr_data.tr_data.setup_event_list.result.general_result = TAPI_SIMATK_R_SUCCESS;
	tr_data.tr_data.setup_event_list.result.me_problem = 0x00;
	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.setup_event_list.result.general_result);
	tapi_simatk_send_terminal_response(&tr_data);*/
}

void
test_simatk_proactive_send_ussd(tapi_simatk_send_ussd_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : alpha_id present?= %d,  alphaID: %s\n",tst_data->alpha_id.is_present, tst_data->alpha_id.string);

//	printf(" [SAT] : USSD STRING - DCS: alpha format=0x%x, compressed format?=0x%x\n",tst_data->ussd_string.dcs.alphabet_format,tst_data->ussd_string.dcs.is_compressed_format);

	printf(" [SAT] : USSD STRING - DCS: Raw DCS =0x%x\n",tst_data->ussd_string.raw_dcs);

	printf(" [SAT] : USSD STRING: len = %d, USSD string = \n", tst_data->ussd_string.ussd_string_length);

	for(i=0;i<tst_data->ussd_string.ussd_string_length;i++)
		printf(" 0x%02x\t",tst_data->ussd_string.ussd_string[i]);

	printf(" \n[SAT] : icon identifier: is icon present = %d, icon_id=%d\n",tst_data->icon_id.is_present, tst_data->icon_id.icon_identifier);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
}

void
test_simatk_proactive_send_ss(tapi_simatk_send_ss_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : alpha_id present?= %d,  alphaID: %s\n",tst_data->alpha_id.is_present, tst_data->alpha_id.string);

	printf(" [SAT] : SS STRING: ton=0x%x, npi=0x%x\n",tst_data->ss_string.ton,tst_data->ss_string.npi);

	printf(" [SAT] : SS STRING: len = %d, ss string = %s\n", tst_data->ss_string.string_length, tst_data->ss_string.string);

	printf(" \n[SAT] : icon identifier: is icon present = %d, icon_id=%d\n",tst_data->icon_id.is_present, tst_data->icon_id.icon_identifier);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
}

void
test_simatk_proactive_setup_call(tapi_simatk_setup_call_ind_data_t* tst_data)
{
	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : setup call :Qualifier = %d\n",tst_data->command_detail.u.cmd_quali_setup_call);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : user confirmation phase: alpha_id present?= %d,  alphaID: %s\n",tst_data->user_confirm_phase_alpha_id.is_present, tst_data->user_confirm_phase_alpha_id.string);

	printf(" [SAT] : ADDRESS: ton=0x%x, npi=0x%x\n",tst_data->address.ton,tst_data->address.npi);

	printf(" [SAT] : ADDRESS: len = %d, dialling Number = %s\n", tst_data->address.dialling_number_length, tst_data->address.dialling_number);

	printf(" [SAT] : SUB ADDRESS: len = %d, data:\n",tst_data->sub_address.sub_address_len);
	for(i=0;i<tst_data->sub_address.sub_address_len;i++)
		printf("\t0x%02x",tst_data->sub_address.sub_address[i]);
	printf("\n");

	printf(" [SAT] : CCP: len = %d, data:\n",tst_data->ccp.data_length);
	for(i=0;i<tst_data->ccp.data_length;i++)
		printf("\t0x%02x",tst_data->ccp.data[i]);
	printf("\n");

	printf(" [SAT] : DURATION: interval=0x%x, time unit=0x%x\n",tst_data->duration.time_interval,tst_data->duration.time_unit);

	printf(" [SAT] : call setup phase: alpha_id present?= %d,  alphaID: %s\n",tst_data->call_setup_phase_alpha_id.is_present, tst_data->call_setup_phase_alpha_id.string);

	printf(" \n[SAT] : user confirmation phase - icon identifier: is icon present = %d, icon_id=%d\n",tst_data->user_confirm_phase_icon_id.is_present, tst_data->user_confirm_phase_icon_id.icon_identifier);

	printf(" \n[SAT] : call setup phase - icon identifier: is icon present = %d, icon_id=%d\n",tst_data->call_setup_phase_icon_id.is_present, tst_data->call_setup_phase_icon_id.icon_identifier);

	//Send TR
	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	/*tr_data.tr_data.setup_event_list.result.general_result = TAPI_SIMATK_R_SUCCESS;
	tr_data.tr_data.setup_event_list.result.me_problem = 0x00;
	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.setup_event_list.result.general_result);
	//tapi_simatk_send_terminal_response(&tr_data);*/
}

void
test_simatk_proactive_select_item_list(tapi_simatk_select_item_ind_data_t* tst_data)
{

	int i;
	printf(" [SAT] : command detail: command_num=%d, command_type=0x%x\n",tst_data->command_detail.command_number,tst_data->command_detail.command_type);

	printf(" [SAT] : command qualifier: prez_type = %d, selection_pref=%d, help_info=%d\n",tst_data->command_detail.u.cmd_quali_select_item.presentation_type,
		tst_data->command_detail.u.cmd_quali_select_item.selection_preference,tst_data->command_detail.u.cmd_quali_select_item.is_help_info_available);

	printf(" [SAT] : device identity: src=0x%x, dest=0x%x\n",tst_data->device_identities.source,tst_data->device_identities.destination);

	printf(" [SAT] : alpha identifier: is_alpha_present=%d, rawdcs=0x%x, str_len=%d, string=%s\n", tst_data->alpha_id.is_present,
				tst_data->alpha_id.dcs.raw_dcs, tst_data->alpha_id.string_len, tst_data->alpha_id.string);

	printf(" [SAT] : items NAI: list count=%d \n [SAT] : items NAI:",tst_data->item_next_action_ind_list.list_count);
	for(i=0; i<tst_data->item_next_action_ind_list.list_count;i++)
		printf("\tnai_ind[%d] = 0x%02x",i, tst_data->item_next_action_ind_list.list[i]);

	printf(" \n[SAT] : DEFAULT ITEM = %d\n",tst_data->default_item_identifier);
	printf(" \n[SAT] : icon identifier: is icon present = %d, icon_id=%d\n",tst_data->icon_id.is_present, tst_data->icon_id.icon_identifier);

	printf(" [SAT] : setup menu: item_count = %d\n",tst_data->menu_item_count);
	for(i=0;i<tst_data->menu_item_count;i++)
		printf("\tITEM#[%d]= %s, [item_id = %d]\n",i, tst_data->menu_item[i].text, tst_data->menu_item[i].item_id);

	tr_data.command_number = tst_data->command_detail.command_number;
	tr_data.command_type = tst_data->command_detail.command_type;
	if(tst_data->command_detail.u.cmd_quali_select_item.is_help_info_available == TRUE)
		tr_data.tr_data.select_item.item_identifier = 0x01;
}

void
test_select_item_send_tr(void)
{
	tapi_simatk_resp_data_t tr_data;
	char str[5] = {0,0};
	int item;
	int ret;
	//Send TR
	//if(item_id > '0' && item_id <= '9')
	{
	char scabuf[5] = {0};
		ret = read(0, scabuf, sizeof(scabuf));
		if (ret < 0)
		{
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		}
		else if (ret == 0)
			return ret;
		memcpy(str, scabuf, (strlen(scabuf)));
		item = atoi(str);

		if(item == 0)
		{
			tr_data.tr_data.select_item.result.general_result = TAPI_SIMATK_R_PROACTIVE_SESSION_TERMINATED_BY_USER;
			tr_data.tr_data.select_item.other_info = FALSE;
		}
	/*	else if(sel_item_data.menu_item_count <  item )
		{
			printf(" [SAT] : ***input should be between 1 & %d\n",sel_item_data.menu_item_count);
			tr_data.tr_data.select_item.result.general_result = TAPI_SIMATK_R_BACKWARD_MOVE_BY_USER;
			tr_data.tr_data.select_item.other_info = FALSE;

		}*/
		else
		{
			tr_data.tr_data.select_item.result.general_result = TAPI_SIMATK_R_SUCCESS;
			tr_data.tr_data.select_item.other_info = TRUE;
			tr_data.tr_data.select_item.item_identifier = (unsigned char)item;
		}
	}
	//else
	//{
	//	printf(" [SAT] : ***INCORRECT INPUT\n");
	//		tr_data.tr_data.select_item.result.general_result = TAPI_SIMATK_R_PROACTIVE_SESSION_TERMINATED_BY_USER;
	//		tr_data.tr_data.select_item.other_info = FALSE;
	//}


	tr_data.command_number = sel_item_data.command_detail.command_number;
	tr_data.command_type = sel_item_data.command_detail.command_type;

	tr_data.tr_data.select_item.result.me_problem = 0x00;

	printf(" [SAT] : send TR. command_number=%d, type=0x%x, gen_result=0x%x, item_id=%d\n",
								tr_data.command_number,	tr_data.command_type,
								tr_data.tr_data.select_item.result.general_result, item);
	TelTapiSatSendUiUserConfirm(tr_data.command_number,tr_data.command_type,SAT_ENGINE_USER_CONFIRM_YES,&tr_data.tr_data.select_item.item_identifier,sizeof(TS_UINT8));
}

int isServerDied = 0;
/********************************************************************************/


int sat_read_key_input(void)
{
	char ch;
	int ret;
	char callId=0;
	unsigned short len;
	char buf[256];
	char dtmfStr[16] = "1000103001";
	char normal[16] = "0103001";





    ret = read(0, buf, sizeof(buf));
    if (ret < 0) {
      if (errno == EINTR)
      perror("read(1)");
      return -1;
    } else if (ret == 0) return ret;


	printf("\n q : quit \n");
	if(curr_cmd == TAPI_EVENT_SAT_SELECT_ITEM_IND)
		printf(" si : select item terminal response \n");
	if(curr_cmd == TAPI_EVENT_SAT_GET_INKEY_IND || curr_cmd == TAPI_EVENT_SAT_GET_INPUT_IND)
	{
		printf(" \"inkey\" or \"input\" : get inkey/input response \n");
		printf(" \"tr\" : to send response other than success \n");
	}
	else if(curr_cmd != 0x00)
		printf(" tr :Send TR for the current command = 0x%x \n",curr_cmd);
	printf(" smhelp :  envelope cmd help \n");
	printf(" sm :  envelope cmd select menu \n");
	printf(" ed: event download\n");
	printf(" cc: MO call control\n");

	printf("--SAT_key_in : %s\n", buf);

	if(strncmp(buf, "q", 1) == 0){
		printf("### test program is terminated.\n");

            return -1;
      }

	if(strncmp(buf, "tr", 2) == 0)
	{
		char scabuf[5] = {0};
		unsigned char input=0x00;

		printf("general response 0x00 ~ 0x3d:  0x\n");

		ret = read(0, scabuf, sizeof(scabuf));
		if (ret < 0)
		{
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		}
		else if (ret == 0)
			return ret;

		input = (scabuf[0] -'0' )<< 4;
		input |= (scabuf[1]-'0')&0x0F;
		if(input >= 0x00 && input <= 0x3d)
			test_send_tr(input);
		else
			test_send_tr(0x00);
	}


	else if(strncmp(buf, "smhelp", 6) == 0)
	{
		printf("\n select menu for help: \n");
		test_simatk_send_menu_select_env(TRUE);
	}
	else if(strncmp(buf, "sm", 2) == 0)
	{
		printf("\n select menu: \n");
		test_simatk_send_menu_select_env(FALSE);
	}

	else if(strncmp(buf, "si", 2) == 0)
	{
		printf("\n select item: \n");
		test_select_item_send_tr();
	}
	else if(strncmp(buf, "ed", 2) == 0)
	{
		char scabuf[5] = {0};
		printf(" id : idle screen \n");
		printf(" ua :  user activity \n");
		printf(" ls: language selection\n");
		printf(" bt: browser termination\n");
		ret = read(0, scabuf, sizeof(scabuf));
		if (ret < 0)
		{
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		}
		else if (ret == 0)
			return ret;

		if(strncmp(scabuf, "id", 2) == 0)
			test_simatk_event_download(TAPI_SIMATK_EVENT_DW_TYPE_IDLE_SCREEN_AVAILABLE);
		else if(strncmp(scabuf, "ua", 2) == 0)
			test_simatk_event_download(TAPI_SIMATK_EVENT_DW_TYPE_USER_ACTIVITY);
		else if(strncmp(scabuf, "ls", 2) == 0)
			test_simatk_event_download(TAPI_SIMATK_EVENT_DW_TYPE_LANGUAGE_SELECTION);
		else if(strncmp(scabuf, "bt", 2) == 0)
			test_simatk_event_download(TAPI_SIMATK_EVENT_DW_TYPE_BROWSER_TERMINATION);
		else
			printf("no such event\n");

	}
	else if((strncmp(buf, "inkey", 5) == 0) ||(strncmp(buf, "input", 5) == 0))
	{
		char scabuf[255] = {0};
		printf(" ONly one character for INKEY, between min and max for INPUT: \n");
		printf(" plz read the command qualifier and provide the input accordingly... \n");
		printf(" bk : backward move \n");
		printf(" end :  end proactive session \n");
		printf(" INKEY: y OR n: for yes or no input\n");
		printf(" input char/string:\n");

		ret = read(0, scabuf, sizeof(scabuf));
		if (ret < 0)
		{
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		}
		else if (ret == 0)
			return ret;

		if((strncmp(scabuf, "y", 1) == 0) || (strncmp(scabuf, "n", 1) == 0))
		{
			//tr_data.tr_data.get_inkey.result.general_result = 0x00;
			tr_data.tr_data.get_inkey.text.dcs.msg_class = TAPI_SIMATK_MSG_CLASS_RESERVED;
			tr_data.tr_data.get_inkey.text.dcs.alphabet_format = TAPI_SIMATK_ALPHABET_FROMAT_8BIT_DATA;
			tr_data.tr_data.get_inkey.text.string_length = 1;
			if(strncmp(scabuf, "y", 1) == 0)
				tr_data.tr_data.get_inkey.text.string[0] = 0x01; //buf[0];
			else
				tr_data.tr_data.get_inkey.text.string[0] = 0x00;
			tr_data.tr_data.get_inkey.text.string[1] = '\0';
			//test_send_tr(tr_data.tr_data.get_inkey.result.general_result);
		}
		else if(strncmp(scabuf, "bk", 2) == 0)
		{
			tr_data.tr_data.get_inkey.result.general_result = 0x11;
		}
		else if(strncmp(scabuf, "end", 3) == 0)
		{
			tr_data.tr_data.get_inkey.result.general_result = 0x10;
		}
		else
		{
			if(strncmp(buf, "inkey", 5) == 0)
			{
				//tr_data.tr_data.get_inkey.result.general_result = 0x00;
				tr_data.tr_data.get_inkey.text.dcs.msg_class = TAPI_SIMATK_MSG_CLASS_RESERVED;
				tr_data.tr_data.get_inkey.text.dcs.alphabet_format = TAPI_SIMATK_ALPHABET_FROMAT_8BIT_DATA;
			tr_data.tr_data.get_inkey.text.string_length = 1;
				tr_data.tr_data.get_inkey.text.string[0] = scabuf[0]; //buf[0];
			tr_data.tr_data.get_inkey.text.string[1] = '\0';
		}
			else
			{
				int i=0;

				//tr_data.tr_data.get_input.result.general_result = 0x00;
				tr_data.tr_data.get_input.text.dcs.msg_class = TAPI_SIMATK_MSG_CLASS_RESERVED;
				printf("inkey: input: %s\n",scabuf);
				while(scabuf[i] != 0xA)
				{
					//printf("%c\n",scabuf[i]);
					i++;
				}
				if(tr_data.tr_data.get_input.text.dcs.alphabet_format == TAPI_SIMATK_ALPHABET_FROMAT_SMS_DEFAULT)
				{
					tr_data.tr_data.get_input.text.string_length = (TS_UINT8)sat_util_pack_7bit(tr_data.tr_data.get_input.text.string, &scabuf[0], i );
				}
				else
				{
					tr_data.tr_data.get_input.text.string_length = i;
					memcpy(tr_data.tr_data.get_input.text.string, &scabuf[0], tr_data.tr_data.get_input.text.string_length);
				}
			}
		}

		test_send_tr(tr_data.tr_data.get_inkey.result.general_result);
	}
	else if(strncmp(buf, "cc", 2) == 0)
	{
		char scabuf[25] = {0};
		TelCallSetupParams_t si;

		memset(&si, 0, sizeof(si));
		si.CallType=TAPI_CALL_TYPE_VOICE;

		printf("number to be dialled:\n");

		ret = read(0, scabuf, sizeof(scabuf));
		if (ret < 0)
		{
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		}
		else if (ret == 0)
			return ret;

		memcpy(si.szNumber, &scabuf, strlen(scabuf));

		si.IdentityMode = TAPI_CALL_IDENTITY_SHOW;		//default

		printf("send the call make message to server....%s", si.szNumber);

		//ret = tapi_call_setup(&si, &g_hCall);

		printf(" after calling tapi_call_setup : g_hCall - %d", g_hCall);

		printf(" Tapi return value is %d", ret);

		if(ret)
		{
			memcpy(&g_info.callNumber, si.szNumber, sizeof( si.szNumber));
		}
	}

	/*
	#if 0
	if(strncmp(buf, "prof", 4) == 0){
		test_simatk_profile_download();
      } else if(strncmp(buf, "env", 3) == 0){
		test_simatk_envelope_send();
      } else if(strncmp(buf, "ed", 2) == 0){
		test_simatk_event_send();
      } else if(strncmp(buf, "sm", 2) == 0) {
		scanf("%s", buf);

		sendEnvelopeCmd_SetupMenu(FALSE, atoi(buf));
	} else if(strncmp(buf, "si", 2) == 0) {
		scanf("%s", buf);

		sendTerminalResponse_SelectItem(0x00, TRUE, atoi(buf), 0, NULL);
	}

	#endif
	*/

	if(strncmp(buf, "menu", 4) == 0)
        {
		printf("Finished Testing this module, Going back to Main Menu\n");
		//call_test_exit();
		return -1;
		//exit(0);
        }

	memset(buf, '\0', 256);

    return 0;
}


/********************************************************************************/
static int  app_callback(TelTapiEventClass_t eventClass, TelTapiEventType_t	eventType, /*length*/int  param1,/*pDate*/void * param2)
{
	int i;
	//unsigned char *buf= (unsigned char*) param2;

	//unsigned char *count = buf;
	int            loop_count;

printf("\n [SAT] app_callback( eventType=0x%02x) \n", eventType);
#if 0
printf("   eventClass: %d, eventType: %d[%02x], paramLength = %d\n", eventClass, eventType, eventType, param1);

	for( i=0; i<param1; i++ ) printf("[%02x] ", buf[i]);
	printf("\n --------------- ------ \n");

	#endif

	switch(eventClass)
	{
		case TAPI_EVENT_CLASS_SAT:
			switch(eventType)
			{
				case TAPI_EVENT_SAT_END_PROACTIVE_SESSION_IND:
					printf("[SAT]  TAPI_EVENT_SAT_END_PROACTIVE_SESSION_IND.\n");
					break;

				case TAPI_EVENT_SAT_DISPLAY_TEXT_IND:
				{
					tapi_simatk_display_text_ind_data_t  tst_data;
					tapi_simatk_resp_data_t tr_data;
					curr_cmd = eventType;

					printf(" [SAT] TAPI_EVENT_SAT_DISPLAY_TEXT_IND: \n");
					//test_display_text_data.data_len = param1;
					memcpy(&tst_data, param2, sizeof(tapi_simatk_display_text_ind_data_t));

					test_simatk_proactive_display_text(&tst_data);

				}
					break;

				case TAPI_EVENT_SAT_UI_SETUP_MENU_IND:
				{
					tapi_simatk_setup_menu_ind_data_t  tst_data;
					curr_cmd = eventType;

					printf(" [SAT] TAPI_EVENT_SAT_UI_SETUP_MENU_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_setup_menu_ind_data_t));

					test_simatk_proactive_setup_menu(&tst_data);
				}
					break;

				case TAPI_SIMATK_EVENT_SETUP_EVENT_LIST_IND:
				{
					tapi_simatk_setup_event_list_ind_data_t tst_data;

					curr_cmd = eventType;
					printf(" [SAT] TAPI_SIMATK_EVENT_SETUP_EVENT_LIST_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_setup_event_list_ind_data_t));

					test_simatk_proactive_setup_evt_list(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_UI_REFRESH_IND:
				{
					tapi_simatk_refresh_ind_data_t tst_data;

					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_UI_REFRESH_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_refresh_ind_data_t));

					test_simatk_proactive_refresh(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_SELECT_ITEM_IND:
				{
					printf(" [SAT] TAPI_EVENT_SAT_SELECT_ITEM_IND: \n");
					memcpy(&sel_item_data, param2, sizeof(tapi_simatk_select_item_ind_data_t));
					curr_cmd = eventType;
					test_simatk_proactive_select_item_list(&sel_item_data);
				}
					break;

				case TAPI_EVENT_SAT_UI_SEND_SMS_IND:
				{
					tapi_simatk_send_sms_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_UI_SEND_SMS_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_send_sms_ind_data_t));

					test_simatk_proactive_send_sms(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_UI_SETUP_CALL_IND:
				{
					tapi_simatk_setup_call_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_UI_SETUP_CALL_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_setup_call_ind_data_t));
					test_simatk_proactive_setup_call(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_UI_SEND_SS_IND:
				{
					tapi_simatk_send_ss_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_UI_SEND_SS_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_send_ss_ind_data_t));
					test_simatk_proactive_send_ss(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_UI_SEND_USSD_IND:
				{
					tapi_simatk_send_ussd_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_UI_SEND_USSD_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_send_ussd_ind_data_t));
					test_simatk_proactive_send_ussd(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_GET_INKEY_IND:
					{
					tapi_simatk_get_inkey_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_GET_INKEY_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_get_inkey_ind_data_t));
					test_simatk_proactive_get_inkey(&tst_data);
					}
					break;

				case TAPI_EVENT_SAT_GET_INPUT_IND:
					{
					tapi_simatk_get_input_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_GET_INPUT_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_get_inkey_ind_data_t));
					test_simatk_proactive_get_input(&tst_data);
					}
					break;

				case TAPI_EVENT_SAT_PLAY_TONE_IND :
					{
					tapi_simatk_play_tone_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_PLAY_TONE_IND : \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_play_tone_ind_data_t));
					test_simatk_proactive_play_tone(&tst_data);
					}
					break;

				case TAPI_EVENT_SAT_UI_SEND_DTMF_IND:
					{
					tapi_simatk_send_dtmf_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_UI_SEND_DTMF_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_send_dtmf_ind_data_t));
					test_simatk_proactive_send_dtmf(&tst_data);
					}
					break;

				case TAPI_EVENT_SAT_SETUP_IDLE_MODE_TEXT_IND:
				{
					tapi_simatk_setup_idle_mode_text_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_SETUP_IDLE_MODE_TEXT_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_setup_idle_mode_text_ind_data_t));
					test_simatk_proactive_setup_idle_mode_text(&tst_data);
				}
					break;

				case TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND:
				{
					tapi_simatk_provide_local_info_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_provide_local_info_ind_data_t));
					test_simatk_proactive_provide_local_info(&tst_data);
				}

					break;

				case TAPI_EVENT_SAT_LANGUAGE_NOTIFICATION_IND:
				{
					tapi_simatk_language_notification_ind_data_t tst_data;
					curr_cmd = eventType;
					printf(" [SAT] TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND: \n");
					memcpy(&tst_data, param2, sizeof(tapi_simatk_language_notification_ind_data_t));
					test_simatk_proactive_lang_noti(&tst_data);
				}

					break;

				case TAPI_EVENT_SAT_CALL_CONTROL_IND:
				{
					tapi_simatk_call_control_cnf_data_t cc_result;
					int i;
					memcpy(&cc_result, param2, sizeof(tapi_simatk_call_control_cnf_data_t));
					printf(" [SAT] TAPI_EVENT_SAT_CALL_CONTROL_IND: \n");
					curr_cmd = eventType;

					printf(" [SAT] :  call type=0x%x, call control result: 0x%x\n",cc_result.call_type, cc_result.call_control_result);

					printf(" [SAT] : alpha_present=%d, alpha_len=%d, alpha_text=%s\n",cc_result.alpha_identifier.is_present, cc_result.alpha_identifier.string_len, cc_result.alpha_identifier.string);

					if(cc_result.call_control_result == SIMATK_CALL_CONTROL_R_ALLOWED_WITH_MOD)
					{
						if(cc_result.call_type == TAPI_SIMATK_CALL_TYPE_MO_VOICE)
						{
							if(cc_result.u.voice_call_data.address.dialling_number_length > 0)
							{
								printf(" [SAT] : TON = 0x%x, NPI = 0x%x\n",cc_result.u.voice_call_data.address.ton, cc_result.u.voice_call_data.address.npi);
								printf(" [SAT] : ADDRESS: len=%d, value =", cc_result.u.voice_call_data.address.dialling_number_length);
								for(i=0; i<cc_result.u.voice_call_data.address.dialling_number_length;i++)
									printf("\t0x%02x",cc_result.u.voice_call_data.address.dialling_number[i]);
							}

							if(cc_result.u.voice_call_data.sub_address.sub_address_len > 0)
							{
								printf(" [SAT] : SUBADDRESS: len=%d, value =", cc_result.u.voice_call_data.sub_address.sub_address_len);
								for(i=0; i<cc_result.u.voice_call_data.sub_address.sub_address_len;i++)
									printf("\t0x%02x",cc_result.u.voice_call_data.sub_address.sub_address[i]);
							}
							else
								printf(" [SAT] : SUBADDRESS NOT PRESENT ");

							printf(" [SAT] : BC REPEAT IND: %d", cc_result.u.voice_call_data.bc_repeat_indicator);

							if(cc_result.u.voice_call_data.ccp1.data_length > 0)
							{
								printf(" [SAT] : CCP1: len=%d, value =", cc_result.u.voice_call_data.ccp1.data_length);
								for(i=0; i<cc_result.u.voice_call_data.ccp1.data_length;i++)
									printf("\t0x%02x",cc_result.u.voice_call_data.ccp1.data[i]);
							}
							else
								printf(" [SAT] : CCP1 NOT PRESENT ");

							if(cc_result.u.voice_call_data.ccp2.data_length > 0)
							{
								printf(" [SAT] : CCP2: len=%d, value =", cc_result.u.voice_call_data.ccp2.data_length);
								for(i=0; i<cc_result.u.voice_call_data.ccp2.data_length;i++)
									printf("\t0x%02x",cc_result.u.voice_call_data.ccp2.data[i]);
							}
							else
								printf(" [SAT] : CCP2 NOT PRESENT ");
						}
						else
						{
							if(cc_result.u.ss_data.ss_string.string_length > 0)
							{
								printf(" [SAT] : TON = 0x%x, NPI = 0x%x\n",cc_result.u.ss_data.ss_string.ton, cc_result.u.ss_data.ss_string.npi);
								printf(" [SAT] : ADDRESS: len=%d, value =", cc_result.u.ss_data.ss_string.string_length);
								for(i=0; i<cc_result.u.ss_data.ss_string.string_length;i++)
									printf("\t0x%02x",cc_result.u.ss_data.ss_string.string[i]);
							}

							if(cc_result.u.ss_data.sub_address.sub_address_len > 0)
							{
								printf(" [SAT] : SUBADDRESS: len=%d, value =", cc_result.u.ss_data.sub_address.sub_address_len);
								for(i=0; i<cc_result.u.ss_data.sub_address.sub_address_len;i++)
									printf("\t0x%02x",cc_result.u.ss_data.sub_address.sub_address[i]);
							}
							else
								printf(" [SAT] : SUBADDRESS NOT PRESENT ");

							printf(" [SAT] : BC REPEAT IND: %d", cc_result.u.ss_data.bc_repeat_indicator);

							if(cc_result.u.ss_data.ccp1.data_length > 0)
							{
								printf(" [SAT] : CCP1: len=%d, value =", cc_result.u.ss_data.ccp1.data_length);
								for(i=0; i<cc_result.u.ss_data.ccp1.data_length;i++)
									printf("\t0x%02x",cc_result.u.ss_data.ccp1.data[i]);
							}
							else
								printf(" [SAT] : CCP1 NOT PRESENT ");

							if(cc_result.u.ss_data.ccp2.data_length > 0)
							{
								printf(" [SAT] : CCP2: len=%d, value =", cc_result.u.ss_data.ccp2.data_length);
								for(i=0; i<cc_result.u.ss_data.ccp2.data_length;i++)
									printf("\t0x%02x",cc_result.u.ss_data.ccp2.data[i]);
							}
							else
								printf(" [SAT] : CCP2 NOT PRESENT ");
						}
					}
				}
					break;
				case TAPI_EVENT_SAT_MO_SMS_CONTROL_IND:
				{
					tapi_simatk_call_control_cnf_data_t sm_result;
					int i;

					memcpy(&sm_result, param2, sizeof(tapi_simatk_call_control_cnf_data_t));
					printf(" [SAT] TAPI_EVENT_SAT_MO_SMS_CONTROL_IND: \n");
					curr_cmd = eventType;

					printf(" [SAT] :  call type=0x%x, MO SM control result: 0x%x\n",sm_result.call_type, sm_result.call_control_result);

					printf(" [SAT] : alpha_present=%d, alpha_len=%d, alpha_text=%s\n",sm_result.alpha_identifier.is_present, sm_result.alpha_identifier.string_len, sm_result.alpha_identifier.string);

					if(sm_result.u.sms_data.rp_dest_address.dialling_number_length > 0)
					{
						printf(" [SAT] : TON = 0x%x, NPI = 0x%x\n",sm_result.u.sms_data.rp_dest_address.ton, sm_result.u.sms_data.rp_dest_address.npi);

						printf(" [SAT] :RPD ADDRESS: len=%d, value =", sm_result.u.sms_data.rp_dest_address.dialling_number_length);
						for(i=0; i<sm_result.u.sms_data.rp_dest_address.dialling_number_length;i++)
							printf("\t0x%02x",sm_result.u.sms_data.rp_dest_address.dialling_number[i]);
					}
					else
						printf("RP Dest Address Not Present");
					printf(" \n");

					if(sm_result.u.sms_data.tp_dest_address.dialling_number_length > 0)
					{
						printf(" [SAT] : TON = 0x%x, NPI = 0x%x\n",sm_result.u.sms_data.tp_dest_address.ton, sm_result.u.sms_data.tp_dest_address.npi);

						printf(" [SAT] :TPD ADDRESS: len=%d, value =", sm_result.u.sms_data.tp_dest_address.dialling_number_length);
						for(i=0; i<sm_result.u.sms_data.tp_dest_address.dialling_number_length;i++)
							printf("\t0x%02x",sm_result.u.sms_data.tp_dest_address.dialling_number[i]);
					}
					else
						printf("TP Dest Address Not Present");
					printf(" \n");
				}
					break;

				default:
					break;
				}


			break;
		default:
			printf(" NOT a simatk event\n");
			break;

	}
printf("\n [SAT] end of app_callback(sat_test/main.c) \n");

	return TRUE;

}




/*****************************************************************/

void sat_select_loop(void)
{
    int ret;
    fd_set readfds;
//	fprintf(stderr,"sat Select Loop!\n");

    while (1)
	{
	        FD_ZERO(&readfds);
	        FD_SET(0, &readfds);

		 ret = select(0 + 1, &readfds, NULL, NULL, NULL);
	        if (ret)
		 {
		     if (FD_ISSET(0, &readfds))
			 {
			      if(sat_read_key_input()<0)
					break;
		        }
	        }
	 }
}

void tapi_sat_thread()
{
    fprintf(stderr,"start tapi_sat_thread\n");
	sat_select_loop();
	fprintf (stderr, " sat exited\n");
	pthread_exit(NULL);
}

bool on_simatk_event_received (EvtSysEventSourceId_t src_id, const char* noti_id, GArray* parameters, void* local_data)
{
	EvtSysEventProperty_t* e;
	int len;
	int reqid;
	int status;
	int group, type;

	/*	Number of params must be 5	*/
	assert(parameters->len == 5);

	/*	First param is	tapi event group   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 0);
	assert(e->type ==  DBUS_TYPE_INT32);
	group = e->value.i;
	printf("received group is :%d\n", group);

	/*	First param is	tapi event type   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 1);
	assert(e->type ==  DBUS_TYPE_INT32);
	type = e->value.i;
	printf("received event type is :%d\n", type);

	e = &g_array_index (parameters, EvtSysEventProperty_t, 2);
	assert(e->type ==  DBUS_TYPE_INT32);
	reqid = e->value.i;
	printf("received Request ID is :%d\n", reqid);

	e = &g_array_index (parameters, EvtSysEventProperty_t, 3);
	assert(e->type ==  DBUS_TYPE_INT32);
	status = e->value.i;
	printf("received STATUS is :%d\n", status);

	/*	over allocate this after receiving the len  */
	unsigned char *data;
	unsigned char *result;
	size_t  out_len;

	/*	Get the tapi event data  here   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 4);
	assert(e->type == DBUS_TYPE_STRING);

	data = e->value.i;

	len = strlen(data);

	printf("Decoding..\n");

	result = TAPI_BASE64_DECODE(data, len, &out_len);

	app_callback(group, type, len, result);
	/*	clean the buffer	*/
//	memset(result2, NULL,  (sdata->len*2));

	/* We've done with this notificaiton */
	return true;

}


int sat_test_subscribe_tapi_events(void)
{

	//GMainLoop *mainloop;
	data_t sdata;
	noti_subscription_id_t subscription_id;
	int count = 0;

	/*
	fprintf(stderr, " GMAIN LOOP\n");

	if (! noti_init(&error_code))
	{
		fprintf(stderr, "Error x_noti_init():%#X \n", error_code);
		return -1;
	}
	printf("receive_tapi_event: noti init success\n");

	*/
	for(count =0;count<sizeof(ptrEvnts)/sizeof(*ptrEvnts);count++)
	{
		error_code = EvtSysEventSubscribe (
											EvtDeliveryHandle,
											ptrEvnts[count],				/* notification ID */
											NULL,						/*	filter expression	*/
											on_simatk_event_received,		/* notification handler */
											(void *)&sdata,				/* local data */
											&subscription_id			/* subscription ID */
										);
		if (error_code )
		{
			fprintf(stderr, "Error x_noti_set_cb():%#X \n", error_code);
		}
		else
			fprintf(stderr, "SIMATK-not subscribe ok for [%d]\n", count);

#if 0

		if (! noti_subscribe (
				ptrEvnts[count],				/* notification ID */
				NULL,						/*	filter expression	*/
				on_simatk_event_received,		/* notification handler */
				(void *)&sdata,				/* local data */
				&subscription_id,				/* subscription ID */
				&error_code))
		{
			fprintf(stderr, "Error x_noti_set_cb():%#X \n", error_code);
		}
		else
			fprintf(stderr, "SIMATK-not subscribe ok for [%d]\n", count);

#endif
	}

/*
	printf("receive_tapi_event: noti subscribe is success\n");

	mainloop = g_main_loop_new (NULL, FALSE);

	g_main_loop_run (mainloop);

	printf("receive_tapi_event: finish\n");
*/
	return 0;


}


int sat_main(int argc, char *argv[])
{
	int ret=0;
	pthread_t sat_thread_id;

	printf("\n\n\n ===== SAT TEST main* =====\n");

	//ret=get_handle();
	//fprintf(stderr,"out of get handle\n");

	ret = pthread_create(&sat_thread_id, NULL, &tapi_sat_thread, NULL);

	if(ret != 0)
	{
		fprintf(stderr, "Thread for TAPI Events Failed\n");
	}

	sat_test_subscribe_tapi_events();

	pthread_join(sat_thread_id, NULL);

}

