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
#include <unistd.h>

#include <assert.h>
#include <glib.h>

#include "ITapiCfg.h"
#include "TapiCommon.h"
#include "TelErr.h"
#include "TelUtility.h"
#include "vconf.h"
#include "TapiEvent.h"

//#include <ipcparam.h>

#define TEST_MAX_CFG_AKEY_LEN 26

typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}edata_t;

#define TEST_DEBUG(frmt, args...) \
	{do { fprintf(stderr, "\n[CFG Test][%s:%04d] "frmt "\n", __func__,__LINE__, ##args); } while (FALSE); }


int error_code;
extern int EvtDeliveryHandle ;
static unsigned int *	Cfg_subscription_id;

static int _PrintSIOMode(TelCfgSIOMode_t mode)
{
	switch(mode)
	{
		case TAPI_CFG_SIO_U1_DIAG_USB2_GPS:
			TEST_DEBUG("SIO mode: TAPI_CFG_SIO_U1_DIAG_USB2_GPS ");
			break;
		case TAPI_CFG_SIO_U1_HFK_USB2_GPS:
			TEST_DEBUG("SIO mode: TAPI_CFG_SIO_U1_HFK_USB2_GPS ");
			break;
		case TAPI_CFG_SIO_U1_HFK_USB2_DIAG:
			TEST_DEBUG("SIO mode: TAPI_CFG_SIO_U1_HFK_USB2_DIAG ");
			break;
		default:
			TEST_DEBUG("SIO mode: None ");
			break;
	}

	return 0;
}

static int  tapi_cfg_callback  (TelTapiEvent_t	*event)
{
	TEST_DEBUG("EVENT STATUS:[0x%x]", event->Status);

	TEST_DEBUG("Status = [0x%x]", event->Status);

	switch(event->EventClass)
	{
		case TAPI_EVENT_CLASS_CFG:
		{
			switch(event->EventType)
			{
				case TAPI_EVENT_CFG_GET_A_KEY_CNF:
				{
					TelCfgAKeyInfo_t Akey_info;
					memset(&Akey_info, 0, sizeof(Akey_info));

					TEST_DEBUG("TAPI_EVENT_CFG_GET_A_KEY_CNF");

					memcpy(&Akey_info, event->pData, sizeof(TelCfgAKeyInfo_t));

					TEST_DEBUG("Akey:[%s]",Akey_info.A_key);
				}
					break;

				case TAPI_EVENT_CFG_VERIFY_A_KEY_CNF:
				{
					TelCfgAKeyVerifyResult_t result;
					TEST_DEBUG("TAPI_EVENT_CFG_VERIFY_A_KEY_CNF");
					memcpy(&result, event->pData, sizeof(TelCfgAKeyVerifyResult_t));
					switch(result)
					{
						case TAPI_CFG_AKEY_VERIFY_FAIL:
						{
							TEST_DEBUG("A-Key verify fail");
						}
							break;

						case TAPI_CFG_AKEY_VERIFY_OK:
						{
							TEST_DEBUG("A-Key verify success");
						}
							break;
						default :
							TEST_DEBUG(" Unknown value : [%d] ", result);
							break;
					}

				}
					break;

				case TAPI_EVENT_CFG_GET_MSL_INFO_CNF:
				{
					TelCfgMSLInfo_t msl_info;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_MSL_INFO_CNF");
					memcpy(&msl_info, event->pData, sizeof(TelCfgMSLInfo_t));
					TEST_DEBUG("msl code:[%s]", msl_info.MslCode);
					TEST_DEBUG("Otksl code:[%s]", msl_info.OtkslCode);
					TEST_DEBUG("Otksl flag:[%d]", msl_info.bOtksl_flag);

				}
					break;

				case TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF:
				{
					TelCfgVocOption_t voc_option;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF");
					memcpy(&voc_option, event->pData, sizeof(TelCfgVocOption_t));
					TEST_DEBUG("vocoder option:[%d]", voc_option);

				}
					break;

				case TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF");
				}
					break;

				case TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF:
				{
					TelCfgHiddenMenuAccessType_t type;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF");
					memcpy(&type, event->pData, sizeof(TelCfgHiddenMenuAccessType_t));
					TEST_DEBUG("access type:[%d]", type);
				}
					break;

				case TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF");
				}
					break;

				case TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF:
				{
					TelCfgCurrentCarrier_t carrier;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF");
					memcpy(&carrier, event->pData, sizeof(TelCfgCurrentCarrier_t));
					TEST_DEBUG("carrier:[%d]", carrier);
				}
					break;

				case TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF");
				}
					break;

				case TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF:
				{
					TelCfgActivationDateInfo_t date_info;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF");
					memcpy(&date_info, event->pData, sizeof(TelCfgActivationDateInfo_t));
					TEST_DEBUG("year:[%d]", date_info.year);
					TEST_DEBUG("month:[%d]", date_info.month);
					TEST_DEBUG("day:[%d]", date_info.day);
					TEST_DEBUG("hour:[%d]", date_info.hour);
					TEST_DEBUG("minute:[%d]", date_info.minute);
					TEST_DEBUG("second:[%d]", date_info.second);
				}
					break;

				case TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF:
				{
					TelCfgReconditionedInfo_t date_info;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF");
					memcpy(&date_info, event->pData, sizeof(TelCfgReconditionedInfo_t));
					TEST_DEBUG("status(0:NO, 1:YES):[%d]", date_info.status);
					TEST_DEBUG("year:[%d]", date_info.year);
					TEST_DEBUG("month:[%d]", date_info.month);
					TEST_DEBUG("day:[%d]", date_info.day);
					TEST_DEBUG("hour:[%d]", date_info.hour);
					TEST_DEBUG("minute:[%d]", date_info.minute);
					TEST_DEBUG("second:[%d]", date_info.second);
				}
					break;

				case TAPI_EVENT_CFG_GET_SIO_MODE_CNF:
				{
					TelCfgSIOMode_t mode;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_SIO_MODE_CNF");
					memcpy(&mode, event->pData, sizeof(TelCfgSIOMode_t));
					_PrintSIOMode(mode);
				}
					break;

				case TAPI_EVENT_CFG_SET_SIO_MODE_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_SIO_MODE_CNF");
				}
					break;
				case TAPI_CFG_EVENT_DEFAULTCFG_IND:
				{
					TEST_DEBUG("TAPI_CFG_EVENT_DEFAULTCFG_IND");
				}
					break;
				default :
					TEST_DEBUG(" Unknown Event : [%d] ", event->EventType);
					break;
			}
			break;
		}

		default:
			break;

	}

	TEST_DEBUG("###############    event processing is done....###############");
	return 0;
}


int cfg_read_key_input(void)
{
	int ret;
	char buf[512] = {0, };
	char msg[512];
	char  diallingNum[256];
	int     diallingNum_len=0;
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	memset(buf,0, sizeof(buf));
	memset(diallingNum,0,sizeof(diallingNum));
	memset(msg,0,sizeof(msg));
	diallingNum_len=0;


	TEST_DEBUG("*************** CFG TEST MENU     ***************");
	TEST_DEBUG("1g.TelTapiCfgGetAKey");
	TEST_DEBUG("1s.TelTapiCfgVerifyAKey");
	TEST_DEBUG("2g.TelTapiCfgGetMSLInfo");
	TEST_DEBUG("3g.TelTapiCfgGetVocoderOption");
	TEST_DEBUG("3s.TelTapiCfgSetVocoderOption		<(default/evrc/13k/8k)>");
	TEST_DEBUG("4g.TelTapiCfgGetHiddenMenuAccess");
	TEST_DEBUG("4s.TelTapiCfgSetHiddenMenuAccess	<(disable/enable)>");
	TEST_DEBUG("5g.TelTapiCfgGetCurrentCarrier");
	TEST_DEBUG("5s.TelTapiCfgSetCurrentCarrier	<(test/skt/ktf/lgt/verizon/sprint)>");
	TEST_DEBUG("6g.TelTapiCfgGetActivationDate");
	TEST_DEBUG("7g.TelTapiCfgGetReconditionedDate");
	TEST_DEBUG("8g.TelTapiCfgGetSIOMode");
	TEST_DEBUG("8s.TelTapiCfgSetSIOMode	<mode(0/1/2)>");
	TEST_DEBUG("9s.tapi_cfg_defaultconfiguration_set	FULL[0]FACTORY[1]SERVICE[2]CUSTOM[3]");
	TEST_DEBUG("0.exit");
	TEST_DEBUG("*****************************************************\n");


	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	if( memcmp(buf,"0",sizeof("0")-1) == 0 )
	{
		TEST_DEBUG("Finished Testing this module, Going back to Main Menu");
		return -1;
	}
	else if( memcmp(buf,"1g",sizeof("1g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_akey....");
		ret_val = tel_get_cfg_akey();
		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"1s",sizeof("1s")-1) == 0 )
	{
		char	tempAkey[TAPI_CFG_A_KEY_DIGITS_MAX+1];
		int		length = 0;
		TelCfgAKeyInfo_t Akey_info;

		strncpy(tempAkey, &buf[3], TAPI_CFG_A_KEY_DIGITS_MAX);

		length = strlen(tempAkey);

		memset(&Akey_info, 0, sizeof(Akey_info));

		strncpy(Akey_info.A_key,tempAkey, length-1);  // removing '/n' form the end of the string

		TEST_DEBUG("tel_verify_cfg_akey with AKey [%s]",Akey_info.A_key);

		ret_val = tel_verify_cfg_akey(&Akey_info);

		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"2g",sizeof("2g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_msl_info....");
		ret_val = tel_get_cfg_msl_info();
		TEST_DEBUG(" Tapi return value: [0x%x] ", ret_val);
	}
	else if( memcmp(buf,"3g",sizeof("3g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_vocoder_option....");
		ret_val = tel_get_cfg_vocoder_option();
		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"3s",sizeof("3s")-1) == 0 )
	{
		TelCfgVocOption_t voc_opt = 0;
		if(memcmp(buf+3,"default",sizeof("default")-1) == 0 )
			voc_opt =TAPI_CFG_VOC_OPTION_DEFAULT;
		else if(memcmp(buf+3,"evrc",sizeof("evrc")-1) == 0 )
			voc_opt =TAPI_CFG_VOC_OPTION_EVRC;
		else if(memcmp(buf+3,"13k",sizeof("13k")-1) == 0 )
			voc_opt =TAPI_CFG_VOC_OPTION_13K;
		else if(memcmp(buf+3,"8k",sizeof("8k")-1) == 0 )
			voc_opt =TAPI_CFG_VOC_OPTION_8K;
		else{
			TEST_DEBUG("Unknown vocoder option");
			return 0;
		}

		TEST_DEBUG("tel_set_cfg_vocoder_option....voc_opt:[%d]",voc_opt);
		ret_val = tel_set_cfg_vocoder_option(voc_opt);
		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"4g",sizeof("4g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_hidden_menu_access....");
		ret_val = tel_get_cfg_hidden_menu_access();
		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"4s",sizeof("4s")-1) == 0 )
	{
		TelCfgHiddenMenuAccessType_t type = 0;
		if(memcmp(buf+3,"disable",sizeof("disable")-1) == 0 )
			type =TAPI_CFG_HIDDEN_MENU_DISABLE;
		else if(memcmp(buf+3,"enable",sizeof("enable")-1) == 0 )
			type =TAPI_CFG_HIDDEN_MENU_ENABLE;
		else{
			TEST_DEBUG("Unknown type");
			return 0;
		}

		TEST_DEBUG("tel_set_cfg_hidden_menu_access....type:[%d]",type);
		ret_val = tel_set_cfg_hidden_menu_access(type);
		TEST_DEBUG(" Tapi return value: [0x%x] ", ret_val);
	}
	else if( memcmp(buf,"5g",sizeof("5g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_serving_carrier....");
		ret_val = tel_get_cfg_serving_carrier();
		TEST_DEBUG(" Tapi return value: [0x%x] ", ret_val);
	}
	else if( memcmp(buf,"5s",sizeof("5s")-1) == 0 )
	{
		TelCfgCurrentCarrier_t Carrier = 0;
		TEST_DEBUG("buf :[%s]....",buf+3);
		if(memcmp(buf+3,"test",sizeof("test")-1) == 0 )
			Carrier =TAPI_CFG_CARRIER_TEST;
		else if(memcmp(buf+3,"skt",sizeof("skt")-1) == 0 )
			Carrier =TAPI_CFG_CARRIER_SKT;
		else if(memcmp(buf+3,"ktf",sizeof("ktf")-1) == 0 )
			Carrier =TAPI_CFG_CARRIER_KTF;
		else if(memcmp(buf+3,"lgt",sizeof("lgt")-1) == 0 )
			Carrier =TAPI_CFG_CARRIER_LGT;
		else if(memcmp(buf+3,"verizon",sizeof("verizon")-1) == 0 )
			Carrier =TAPI_CFG_CARRIER_VERIZON;
		else if(memcmp(buf+3,"sprint",sizeof("sprint")-1) == 0 )
			Carrier =TAPI_CFG_CARRIER_SPRINT;
		else{
			TEST_DEBUG("Unknown Carrier....");
			return 0;
		}

		TEST_DEBUG("tel_set_cfg_serving_carrier....Carrier:[%d]",Carrier);
		ret_val = tel_set_cfg_serving_carrier(Carrier);
		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"6g",sizeof("6g")-1) == 0 )
	{
		TEST_DEBUG("TelTapiCfgGetActivationDate....");
		ret_val = tel_get_cfg_activated_date_of_phone();
		TEST_DEBUG(" Tapi return value: [0x%x]  ", ret_val);
	}
	else if( memcmp(buf,"7g",sizeof("7g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_reconditioned_data_of_phone....");
		ret_val = tel_get_cfg_reconditioned_data_of_phone();
		TEST_DEBUG(" Tapi return value: [0x%x]   ", ret_val);
	}
	else if( memcmp(buf,"8g",sizeof("8g")-1) == 0 )
	{
		TEST_DEBUG("tel_get_cfg_sio_mode....");
		ret_val = tel_get_cfg_sio_mode();
		TEST_DEBUG(" Tapi return value: [0x%x] ", ret_val);
	}
	else if( memcmp(buf,"8s",sizeof("8s")-1) == 0 )
	{
		TelCfgSIOMode_t mode = 0;
		if(memcmp(buf+3,"0",sizeof("0")-1) == 0 )
			mode =TAPI_CFG_SIO_U1_DIAG_USB2_GPS;
		else if(memcmp(buf+3,"1",sizeof("1")-1) == 0 )
			mode =TAPI_CFG_SIO_U1_HFK_USB2_GPS;
		else if(memcmp(buf+3,"2",sizeof("2")-1) == 0 )
			mode =TAPI_CFG_SIO_U1_HFK_USB2_DIAG;
		else{
			TEST_DEBUG("Unknown mode");
			return 0;
		}
		TEST_DEBUG("tel_set_cfg_sio_mode....mode:[%d]",mode);
		ret_val = tel_set_cfg_sio_mode(mode);
		TEST_DEBUG(" Tapi return value: [0x%x] ", ret_val);
	}

	else if( memcmp(buf,"9s",sizeof("9s")-1) == 0 )
	{
		TelDefaultCfg_t bType =0;

		if(memcmp(buf+3,"0",sizeof("0")-1) == 0 )
			bType = TAPI_CFG_DEFAULT_FULL;
		else if(memcmp(buf+3,"1",sizeof("1")-1) == 0 )
			bType = TAPI_CFG_DEFAULT_FACTORY;
		else if(memcmp(buf+3,"2",sizeof("2")-1) == 0 )
			bType = TAPI_CFG_DEFAULT_SERVICE;
		else if(memcmp(buf+3,"3",sizeof("3")-1) == 0 )
			bType = TAPI_CFG_DEFAULT_CUSTOM;

		tel_set_cfg_default_configuration(bType);
	}

	return 0;
}


void cfg_select_loop(void)
{
	int ret;
	fd_set readfds;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret)
		{
			if (FD_ISSET(0, &readfds))
			{
				if(cfg_read_key_input()<0)
					break;
			}
		}
	}
}

#if 0
static int  tapi_cfg_callback  (TelTapiEventClass_t	eventClass,int	eventType, int  param1,void * param2)
{
	TEST_DEBUG("EVENT STATUS:[%d]", param1);

	switch(eventClass)
	{
		case TAPI_EVENT_CLASS_CFG:
		{
			switch(eventType)
			{
				case TAPI_EVENT_CFG_GET_A_KEY_CNF:
				{
					TelCfgAKeyInfo_t Akey_info;
					memset(&Akey_info, 0, sizeof(Akey_info));

					TEST_DEBUG("TAPI_EVENT_CFG_GET_A_KEY_CNF");

					memcpy(&Akey_info, param2, sizeof(TelCfgAKeyInfo_t));

					TEST_DEBUG("Akey:[%s]",Akey_info.A_key);
				}
					break;

				case TAPI_EVENT_CFG_VERIFY_A_KEY_CNF:
				{
					TelCfgAKeyVerifyResult_t result;
					TEST_DEBUG("TAPI_EVENT_CFG_VERIFY_A_KEY_CNF");
					memcpy(&result, param2, sizeof(TelCfgAKeyVerifyResult_t));
					switch(result)
					{
						case TAPI_CFG_AKEY_VERIFY_FAIL:
						{
							TEST_DEBUG("A-Key verify fail");
						}
							break;

						case TAPI_CFG_AKEY_VERIFY_OK:
						{
							TEST_DEBUG("A-Key verify success");
						}
							break;
						default :
							TEST_DEBUG(" Unknown value : [%d] ", result);
							break;
					}

				}
					break;

				case TAPI_EVENT_CFG_GET_MSL_INFO_CNF:
				{
					TelCfgMSLInfo_t msl_info;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_MSL_INFO_CNF");
					memcpy(&msl_info, param2, sizeof(TelCfgMSLInfo_t));
					TEST_DEBUG("msl code:[%s]", msl_info.MslCode);
					TEST_DEBUG("Otksl code:[%s]", msl_info.OtkslCode);
					TEST_DEBUG("Otksl flag:[%d]", msl_info.Otksl_flag);

				}
					break;

				case TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF:
				{
					TelCfgVocOption_t voc_option;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF");
					memcpy(&voc_option, param2, sizeof(TelCfgVocOption_t));
					TEST_DEBUG("vocoder option:[%d]", voc_option);

				}
					break;

				case TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF");
				}
					break;

				case TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF:
				{
					TelCfgHiddenMenuAccessType_t type;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF");
					memcpy(&type, param2, sizeof(TelCfgHiddenMenuAccessType_t));
					TEST_DEBUG("access type:[%d]", type);
				}
					break;

				case TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF");
				}
					break;

				case TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF:
				{
					TelCfgCurrentCarrier_t carrier;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF");
					memcpy(&carrier, param2, sizeof(TelCfgCurrentCarrier_t));
					TEST_DEBUG("carrier:[%d]", carrier);
				}
					break;

				case TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF");
				}
					break;

				case TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF:
				{
					TelCfgActivationDateInfo_t date_info;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF");
					memcpy(&date_info, param2, sizeof(TelCfgActivationDateInfo_t));
					TEST_DEBUG("year:[%d]", date_info.year);
					TEST_DEBUG("month:[%d]", date_info.month);
					TEST_DEBUG("day:[%d]", date_info.day);
					TEST_DEBUG("hour:[%d]", date_info.hour);
					TEST_DEBUG("minute:[%d]", date_info.minute);
					TEST_DEBUG("second:[%d]", date_info.second);
				}
					break;

				case TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF:
				{
					TelCfgReconditionedInfo_t date_info;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF");
					memcpy(&date_info, param2, sizeof(TelCfgReconditionedInfo_t));
					TEST_DEBUG("status(0:NO, 1:YES):[%d]", date_info.status);
					TEST_DEBUG("year:[%d]", date_info.year);
					TEST_DEBUG("month:[%d]", date_info.month);
					TEST_DEBUG("day:[%d]", date_info.day);
					TEST_DEBUG("hour:[%d]", date_info.hour);
					TEST_DEBUG("minute:[%d]", date_info.minute);
					TEST_DEBUG("second:[%d]", date_info.second);
				}
					break;

				case TAPI_EVENT_CFG_GET_SIO_MODE_CNF:
				{
					TelCfgSIOMode_t mode;
					TEST_DEBUG("TAPI_EVENT_CFG_GET_SIO_MODE_CNF");
					memcpy(&mode, param2, sizeof(TelCfgSIOMode_t));
					_PrintSIOMode(mode);
				}
					break;

				case TAPI_EVENT_CFG_SET_SIO_MODE_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CFG_SET_SIO_MODE_CNF");
				}
					break;

				default :
					TEST_DEBUG(" Unknown Event : [%d] ", eventType);
					break;
			}
			break;
		}

		default:
			break;

	}

	TEST_DEBUG("###############    event processing is done....###############");
	return 0;
}

bool on_cfg_event_received (EvtSysEventSourceId_t src_id, const char* noti_id, GArray* parameters, void* local_data)
{
	EvtSysEventProperty_t* e;
	int len;
	int group;
	int type;
	int reqid;
	int status;

	assert(parameters->len == 5);

	/*	1 param is	tapi event group   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 0);
	assert(e->type ==  DBUS_TYPE_INT32);
	group = e->value.i;
	//TEST_DEBUG("received group is :%d ", group);

	/*	2 param is	tapi event type   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 1);
	assert(e->type ==  DBUS_TYPE_INT32);
	type = e->value.i;
	//TEST_DEBUG("received event type is :%d ", type);

	/*	3 param is	Response ID   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 2);
	assert(e->type ==  DBUS_TYPE_INT32);
	reqid = e->value.i;
	//TEST_DEBUG("received Request ID is :%d ", reqid);

	/*	4 param is	Status   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 3);
	assert(e->type ==  DBUS_TYPE_INT32);
	status = e->value.i;
	//TEST_DEBUG("status is :%d ", status);


	if (status != 0x00)
	{
		TEST_DEBUG(" ##### Received Error## status:[%d] for event type[0x%x]", status, type);
		return true;
	}

	/*	over allocate this after receiving the len  */
	char *data;
	guchar *result;
	size_t  out_len;

	/*	5 param is	data   */
	e = &g_array_index (parameters, EvtSysEventProperty_t, 4);
	assert(e->type == DBUS_TYPE_STRING);

	data = (char *)e->value.i;

	len = strlen(data);

	TAPI_BASE64_DECODE(data, result, &out_len);

	tapi_cfg_callback(group, type, status, result);

	/* We've done with this notificaiton */
	return true;

}
#endif

//int misc_test_subscribe_tapi_events(void)
int cfg_test_subscribe_tapi_events(void)
{
	int i = 0;
	int ret_val = TRUE;
	int iNumOfCfgEvt = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;


	int cfgEvents[] =
	{
		TAPI_CFG_EVENT_DEFAULTCFG_IND,
		TAPI_EVENT_CFG_GET_A_KEY_CNF,					     /**<     */
		TAPI_EVENT_CFG_VERIFY_A_KEY_CNF,				     /**<     */
		TAPI_EVENT_CFG_GET_MSL_INFO_CNF,				     /**<     */
		TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF,			      /**<     */
		TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF,			      /**<     */
		TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF,	      /**<     */
		TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF,	      /**<     */
		TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF,			      /**<     */
		TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF,			      /**<     */
		TAPI_EVENT_CFG_GET_SIO_MODE_CNF,			      /**<     */
		TAPI_EVENT_CFG_SET_SIO_MODE_CNF,			      /**<     */
		TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF,			      /**<     */
		TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF,			      /**<     */
		TAPI_EVENT_CFG_SET_TTY_MODE_CNF,			      /**<     */
		TAPI_EVENT_CFG_GET_TTY_MODE_CNF,			      /**<     */
	};

	iNumOfCfgEvt = sizeof(cfgEvents)/sizeof(int);
//	TAPI_CALLOC(Cfg_subscription_id, iNumOfCfgEvt, unsigned int)
//#define TAPI_CALLOC(ptr, no_elements, type)
	Cfg_subscription_id = (unsigned int *) calloc (iNumOfCfgEvt , sizeof (unsigned int));
	if(Cfg_subscription_id == NULL)
	{
		TEST_DEBUG("calloc error -NULL, errno is [%d]", errno);
	}


	if(tel_init() == TAPI_API_SUCCESS)
	{
		for( i=0 ; i< iNumOfCfgEvt; i++ )
		{
			api_err = tel_register_event(cfgEvents[i], &Cfg_subscription_id[i],(TelAppCallback)&tapi_cfg_callback,NULL);
			if(api_err != TAPI_API_SUCCESS)
			{
				TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", cfgEvents[i],api_err);
				ret_val = FALSE;
				break;
			}
		}
	}
	else
	{
		TEST_DEBUG("tel_init() failed");
	}
	return ret_val;

#if 0
	//GMainLoop *mainloop;
	edata_t sdata;
	noti_subscription_id_t subscription_id;
	int count;

	char *cfgEvents[] = {
									"Telephony.User.Cfg.BtWifiTestInd",
									"Telephony.User.Cfg.PdaCalibration",
									"Telephony.Cfg.GetAKeyCnf",
									"Telephony.Cfg.VerifyAKeyCnf",
									"Telephony.Cfg.GetMslInfoCnf",
									"Telephony.Cfg.GetVocoderOptionCnf",
									"Telephony.Cfg.SetVocoderOptionCnf",
									"Telephony.Cfg.GetHiddenMenuAccessCnf",
									"Telephony.Cfg.SetHiddenMenuAccessCnf",
									"Telephony.Cfg.GetCurrentCarrierCnf",
									"Telephony.Cfg.SetCurrentCarrierCnf",
									"Telephony.Cfg.GetSioModeCnf",
									"Telephony.Cfg.SetSioModeCnf",
									"Telephony.Cfg.GetUsbPathCnf",
									"Telephony.Cfg.SetUsbPathCnf",
									"Telephony.Cfg.GetActivationDateCnf",
									"Telephony.Cfg.GetReconditionedDateCnf",
								};

	for(count =0;count  <  (sizeof(cfgEvents)/sizeof(*cfgEvents)) ; count++)
	{
		error_code = EvtSysEventSubscribe (
											EvtDeliveryHandle,
											cfgEvents[count],				/* notification ID */
											NULL,						/*	filter expression	*/
											(EvtSysEventCb)on_cfg_event_received,		/* notification handler */
											(void *)&sdata,				/* local data */
											&subscription_id			/* subscription ID */
										);
		if (error_code )
		{
			TEST_DEBUG( "[NET TEST] EvtSysEventSubscribe(): error [%#X] for event [%s]", error_code , cfgEvents[count]);
		}
	}

	return 0;
#endif
}

#if 0
void tapi_cfg_thread()
{
	TEST_DEBUG("start tapi_cfg_thread");
	cfg_select_loop();
	fprintf (stderr, " cfg exited");
	pthread_exit(NULL);
}


int cfg_main(int argc, char *argv[])
{
	int ret=0;
	pthread_t cfg_thread_id;

	TEST_DEBUG("\n\n\n ===== config module test  ver 1.01 =====");

	ret = pthread_create(&cfg_thread_id, NULL, &tapi_cfg_thread, NULL);

	if(ret != 0)
	{
		TEST_DEBUG( "Thread for TAPI Events Failed");
	}

	pthread_join(cfg_thread_id, NULL);
}
#endif
