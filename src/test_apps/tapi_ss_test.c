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
#include "assert.h"
#include "glib.h"

#include "TelErr.h"
#include "TelUtility_ext.h"
#include "ITapiMisc.h"
#include "ITapiSs.h"
#include "TapiCommon.h"
#include "ITapiSim.h"


#define TEST_DEBUG(frmt, args...)	\
	{do { fprintf(stderr, "[SS Test][%s:%04d] " frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}


#ifndef bool
	#define bool  char
#endif

// same as tapievent
typedef struct
{
	int group;
	int type;
	int ID;
	int status;
	void *dst;
}data_t;

static unsigned int *	SS_subscription_id;

extern int EvtDeliveryHandle ;


int ss_strlen2(register const char * s ) {
	int count = 0 ;
	while( *s++ ) count ++ ;
	return count ;
} // end strlen()


int ss_fgets(register char * digit, int n)
{
	if( ! fgets( digit , n , stdin ) )  return -1 ;

	if( *(digit + (n = ss_strlen2(digit)-1)) ^ '\n' )  {
		n = 0 ;
		while( getchar() ^ '\n' ) n -- ;
		if( n ) return 1 ;

		return 0;
	}
	else
		*( digit + n ) = 0 ;
	return 0 ;
}

static int _PrintTeleCommService(TelSsTeleService_t telservice)
{
	switch(telservice)
	{
		case TAPI_SS_TS_ALL_TELESERVICES:
			TEST_DEBUG("TeleService: TAPI_SS_TS_ALL_TELESERVICES ");
			break;
		case TAPI_SS_TS_ALL_SPEECH:
			TEST_DEBUG("TeleService: TAPI_SS_TS_ALL_SPEECH ");
			break;
		case TAPI_SS_TS_ALL_DATA_TELESERVICES:
			TEST_DEBUG("TeleService: TAPI_SS_TS_ALL_DATA_TELESERVICES ");
			break;
		case TAPI_SS_TS_FACS:
			TEST_DEBUG("TeleService: TAPI_SS_TS_FACS ");
			break;
		case TAPI_SS_TS_SMS:
			TEST_DEBUG("TeleService: TAPI_SS_TS_SMS ");
			break;
		case TAPI_SS_TS_ALL_TELESERVICES_EXCEPT_SMS:
			TEST_DEBUG("TeleService: TAPI_SS_TS_ALL_TELESERVICES_EXCEPT_SMS ");
			break;
		case TAPI_SS_TS_ALL_PLMN_SPEC_TELE:
			TEST_DEBUG("TeleService: TAPI_SS_TS_ALL_PLMN_SPEC_TELE ");
			break;
		case TAPI_SS_BS_ALL_BEARER_SERVICES:
			TEST_DEBUG("TeleService: TAPI_SS_BS_ALL_BEARER_SERVICES ");
			break;
		case TAPI_SS_BS_ALL_ASYNCHRONOUS:
			TEST_DEBUG("TeleService: TAPI_SS_BS_ALL_ASYNCHRONOUS ");
			break;
		case TAPI_SS_BS_ALL_SYNCHRONOUS:
			TEST_DEBUG("TeleService: TAPI_SS_BS_ALL_SYNCHRONOUS ");
			break;
		case TAPI_SS_BS_DATA_CIRCUIT_SYNC:
			TEST_DEBUG("TeleService: TAPI_SS_BS_DATA_CIRCUIT_SYNC ");
			break;
		case TAPI_SS_TS_ALL_DATA:
			TEST_DEBUG("TeleService: TAPI_SS_TS_ALL_DATA ");
			break;
		case TAPI_SS_ALL_TELE_BEARER:
			TEST_DEBUG("TeleService: TAPI_SS_ALL_TELE_BEARER ");
			break;
		default:
			TEST_DEBUG("TeleService: %d ", telservice);
			break;
	}

	return 0;
}
static int _PrintBarringType(TelSsCallBarType_t BarType)
{
	switch(BarType)
	{
		case TAPI_CALL_BARRING_ALL:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL ");
			break;
		case TAPI_CALL_BARRING_OUTGOING:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_OUTGOING ");
			break;
		case TAPI_CALL_BARRING_ALL_OUTGOING_CALLS:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL_OUTGOING_CALLS ");
			break;
		case TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL ");
			break;
		case TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL_EXCEPT:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL_EXCEPT ");
			break;
		case TAPI_CALL_BARRING_INCOMING:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_INCOMING ");
			break;
		case TAPI_CALL_BARRING_ALL_INCOMING_CALLS:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL_INCOMING_CALLS ");
			break;
		case TAPI_CALL_BARRING_ALL_INCOMING_CALLS_ROAMING:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL_INCOMING_CALLS_ROAMING ");
			break;
		case TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM:
			TEST_DEBUG("BarringType: TAPI_CALL_BARRING_ALL_INCOMING_CALLS_INSIM ");
			break;
		default:
			TEST_DEBUG("BarringType: None [%d] ", BarType);
			break;
	}

	return 0;
}

static int _PrintSsStatus(TelSsStatus_t Status)
{
	switch(Status)
	{
		case TAPI_SS_STATUS_ACTIVE:
			TEST_DEBUG("SS Status: TAPI_SS_STATUS_ACTIVE ");
			break;
		case TAPI_SS_STATUS_REGISTERED:
			TEST_DEBUG("SS Status: TAPI_SS_STATUS_REGISTERED (for CF) ");
			break;
		default:
			TEST_DEBUG("SS Status: Not Active ");
			break;
	}

	return 0;
}

static int _PrintSsCliType(TelSsLineIdentificationType_t CliType)
{
	switch(CliType)
	{
		case TAPI_CALLING_LINE_IDENTITY_PRESENTATION:
			TEST_DEBUG("Type: TAPI_CALLING_LINE_IDENTITY_PRESENTATION ");
			break;
		case TAPI_CALLING_LINE_IDENTITY_RESTRICTION:
			TEST_DEBUG("Type: TAPI_CALLING_LINE_IDENTITY_RESTRICTION ");
			break;
		case TAPI_CONNECTED_LINE_IDENTITY_PRESENTATION:
			TEST_DEBUG("Type: TAPI_CONNECTED_LINE_IDENTITY_PRESENTATION ");
			break;
		case TAPI_CONNECTED_LINE_IDENTITY_RESTRICTION:
			TEST_DEBUG("Type: TAPI_CONNECTED_LINE_IDENTITY_RESTRICTION ");
			break;
		case TAPI_CALLED_LINE_IDENTITY_PRESENTATION:
			TEST_DEBUG("Type: TAPI_CALLED_LINE_IDENTITY_PRESENTATION ");
			break;
		case TAPI_CALLING_NAME_PRESENTATION:
			TEST_DEBUG("Type: TAPI_CALLING_NAME_PRESENTATION ");
			break;
		default:
			TEST_DEBUG("Type: None [%d] ", CliType);
			break;
	}

	return 0;
}

static int _PrintSsCliStatus(TelSsCliStatus_t CliStatus)
{
	switch(CliStatus)
	{
		case TAPI_CLI_STATUS_NOT_PROVISONED:
			TEST_DEBUG("CliStatus: TAPI_CLI_STATUS_NOT_PROVISONED ");
			break;
		case TAPI_CLI_STATUS_PROVISIONED:
			TEST_DEBUG("CliStatus: TAPI_CLI_STATUS_PROVISIONED ");
			break;
		case TAPI_CLI_STATUS_ACTIVATED:
			TEST_DEBUG("CliStatus: TAPI_CLI_STATUS_ACTIVATED ");
			break;
		case TAPI_CLI_STATUS_UNKOWN:
			TEST_DEBUG("CliStatus: TAPI_CLI_STATUS_UNKOWN ");
			break;
		case TAPI_CLI_STATUS_TEMP_RESTRICTED:
			TEST_DEBUG("CliStatus: TAPI_CLI_STATUS_TEMP_RESTRICTED ");
			break;
		case TAPI_CLI_STATUS_TEMP_ALLOWED:
			TEST_DEBUG("CliStatus: TAPI_CLI_STATUS_TEMP_ALLOWED ");
			break;
		default:
			TEST_DEBUG("CliStatus: None [%d] ", CliStatus);
			break;
	}

	return 0;
}

static int _PrintSsUssdRecord(TelSsUssdMsgIndInfo_t ussd_record)
{
	switch(ussd_record.IndType)
	{
		case TAPI_USSD_IND_NOTIFY:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_NOTIFY ");
			break;
		case TAPI_USSD_IND_NO_ACTION_REQUIRE:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_NO_ACTION_REQUIRE ");
			break;
		case TAPI_USSD_IND_ACTION_REQUIRE:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_ACTION_REQUIRE ");
			break;
		case TAPI_USSD_IND_TERMINATED_BY_NET:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_TERMINATED_BY_NET ");
			break;
		case TAPI_USSD_IND_OTHER_CLIENT:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_OTHER_CLIENT ");
			break;
		case TAPI_USSD_IND_NOT_SUPPORT:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_NOT_SUPPORT ");
			break;
		case TAPI_USSD_IND_TIME_OUT:
			TEST_DEBUG("UssdIndType: TAPI_USSD_IND_TIME_OUT ");
			break;
		default:
			TEST_DEBUG("UssdIndType: None [%d] ", ussd_record.IndType);
			break;
	}

	TEST_DEBUG("RequestedBySAT:[%d]",ussd_record.UssdInfo.RequestedBySAT);
	TEST_DEBUG("DCS		: [0x%x]",ussd_record.UssdInfo.Dcs);
	TEST_DEBUG("ussd length : [%d] ", ussd_record.UssdInfo.UssdStringLength);
	TEST_DEBUG("ussd string : [\n%s\n] ", ussd_record.UssdInfo.szUssdString);

	return 0;
}

static int _PrintCFNoReplyWaitTime(TelSsNoReplyTime_t time)
{
	switch(time)
	{
		case TAPI_SS_CF_NO_REPLY_TIME_5_SECS:
			TEST_DEBUG("CF reply time : TAPI_SS_CF_NO_REPLY_TIME_5_SECS ");
			break;
		case TAPI_SS_CF_NO_REPLY_TIME_10_SECS:
			TEST_DEBUG("CF reply time : TAPI_SS_CF_NO_REPLY_TIME_10_SECS ");
			break;
		case TAPI_SS_CF_NO_REPLY_TIME_15_SECS:
			TEST_DEBUG("CF reply time : TAPI_SS_CF_NO_REPLY_TIME_15_SECS ");
			break;
		case TAPI_SS_CF_NO_REPLY_TIME_20_SECS:
			TEST_DEBUG("CF reply time : TAPI_SS_CF_NO_REPLY_TIME_20_SECS ");
			break;
		case TAPI_SS_CF_NO_REPLY_TIME_25_SECS:
			TEST_DEBUG("CF reply time : TAPI_SS_CF_NO_REPLY_TIME_25_SECS ");
			break;
		case TAPI_SS_CF_NO_REPLY_TIME_30_SECS:
			TEST_DEBUG("CF reply time : TAPI_SS_CF_NO_REPLY_TIME_30_SECS ");
			break;
		default:
			TEST_DEBUG("CF reply time: None [%d] ", time);
			break;
	}
	return 0;
}

static int _PrintCFCondition(TelSsForwardWhen_t cond)
{
	switch(cond)
	{
		case TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV:
			TEST_DEBUG("CF condition : TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV ");
			break;
		case TAPI_SS_FORWARD_WHEN_BUSY_EV:
			TEST_DEBUG("CF condition : TAPI_SS_FORWARD_WHEN_BUSY_EV ");
			break;
		case TAPI_SS_FORWARD_WHEN_NO_ANSWER_EV:
			TEST_DEBUG("CF condition : TAPI_SS_FORWARD_WHEN_NO_ANSWER_EV ");
			break;
		case TAPI_SS_FORWARD_WHEN_NOT_REACHABLE_EV:
			TEST_DEBUG("CF condition : TAPI_SS_FORWARD_WHEN_NOT_REACHABLE_EV ");
			break;
		case TAPI_SS_FORWARD_WHEN_ALL_FORWARDING_EV:
			TEST_DEBUG("CF condition : TAPI_SS_FORWARD_WHEN_ALL_FORWARDING_EV ");
			break;
		case TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV:
			TEST_DEBUG("CF condition : TAPI_SS_FORWARD_WHEN_ALL_CONDITIONAL_EV ");
			break;
		default:
			TEST_DEBUG("CF condition : None [%d] ", cond);
			break;
	}
	return 0;
}

//static int  ss_async_event_callback(TelTapiEventClass_t eventClass, int eventType, int  param1,void * param2)
static int  ss_async_event_callback(TelTapiEvent_t	*event)
{
	TEST_DEBUG("EVENT TYPE:[0x%x], STATUS:[0x%x]", event->EventType, event->Status);

	int i = 0;

	switch(event->EventClass)
	{
		case TAPI_EVENT_CLASS_SS:
		{
			switch(event->EventType)
			{
				case TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY:
				{
					TelSsRelCompMsgInfo_t      rel_comp_msg;
					memcpy(&rel_comp_msg, (TelSsRelCompMsgInfo_t *)event->pData, sizeof(TelSsRelCompMsgInfo_t));
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY  ##### ");
				}
					break;

				case TAPI_EVENT_SS_WAITING_CNF:
				{
					TelSsInfo_t SsInfo = {0,};
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_WAITING_CNF  ##### ");

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));

						TEST_DEBUG("Number of records %d ",SsInfo.NumberOfRecords);
						for(i=0;i<SsInfo.NumberOfRecords;i++)
						{
							TEST_DEBUG("Index:[%d]",i);
							_PrintSsStatus(SsInfo.SsRecord.WaitingRecord.rec_class[i].Status);
							_PrintTeleCommService(SsInfo.SsRecord.WaitingRecord.rec_class[i].TeleCommService);
						}
					}
				}

					break;

				case TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF:
				{
					TelSsInfo_t SsInfo = {0,};
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF  ##### ");

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));

						TEST_DEBUG("Number of records %d ",SsInfo.NumberOfRecords);
						for(i=0;i<SsInfo.NumberOfRecords;i++)
						{
							TEST_DEBUG("Index:[%d]",i);
							_PrintSsStatus(SsInfo.SsRecord.WaitingRecord.rec_class[i].Status);
							_PrintTeleCommService(SsInfo.SsRecord.WaitingRecord.rec_class[i].TeleCommService);
						}
					}
				}
					break;
				case TAPI_EVENT_SS_BARRING_CNF:
				{
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_BARRING_CNF  ##### ");
					TelSsInfo_t SsInfo = {0,};

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));

						TEST_DEBUG("Number of records %d ",SsInfo.NumberOfRecords);
						for(i=0;i<SsInfo.NumberOfRecords;i++)
						{
							TEST_DEBUG("index:[%d] ",i);
							_PrintSsStatus(SsInfo.SsRecord.BarringRecord.rec_class[i].Status);
							_PrintTeleCommService(SsInfo.SsRecord.BarringRecord.rec_class[i].TeleCommService);
							_PrintBarringType(SsInfo.SsRecord.BarringRecord.rec_class[i].Flavour);

						}
					}
				}
					break;

				case TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF:
				{
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF  ##### ");
					TelSsInfo_t SsInfo = {0,};

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));

						TEST_DEBUG("Number of records %d ",SsInfo.NumberOfRecords);
						for(i=0;i<SsInfo.NumberOfRecords;i++)
						{
							TEST_DEBUG("index:[%d] ",i);
							_PrintSsStatus(SsInfo.SsRecord.BarringRecord.rec_class[i].Status);
							_PrintTeleCommService(SsInfo.SsRecord.BarringRecord.rec_class[i].TeleCommService);
							_PrintBarringType(SsInfo.SsRecord.BarringRecord.rec_class[i].Flavour);
						}

					}

				}
					break;

				case TAPI_EVENT_SS_FORWARD_CNF:
				{
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_FORWARD_CNF  ##### ");
					TelSsInfo_t SsInfo = {0,};

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));

						TEST_DEBUG("Number of records %d ",SsInfo.NumberOfRecords);
						for(i=0;i<SsInfo.NumberOfRecords;i++)
						{
							TEST_DEBUG("index:[%d] ",i);
							_PrintSsStatus(SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status);
							_PrintTeleCommService(SsInfo.SsRecord.ForwardingRecord.rec_class[i].TeleCommService);
							_PrintCFNoReplyWaitTime(SsInfo.SsRecord.ForwardingRecord.rec_class[i].NoReplyWaitTime);
							_PrintCFCondition(SsInfo.SsRecord.ForwardingRecord.rec_class[i].ForwardCondition);
							TEST_DEBUG("bCFNumPresent:[%d] ",SsInfo.SsRecord.ForwardingRecord.rec_class[i].bCallForwardingNumberPresent);
							TEST_DEBUG("bCFNum:[%s] ",SsInfo.SsRecord.ForwardingRecord.rec_class[i].szCallForwardingNumber);
						}
					}
				}
					break;

				case TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF:
				{
					TEST_DEBUG("####  Received Event :TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF  ##### ");
					TelSsInfo_t SsInfo = {0,};

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));

						TEST_DEBUG("Number of records %d ",SsInfo.NumberOfRecords);

						for(i=0;i<SsInfo.NumberOfRecords;i++)
						{
							TEST_DEBUG("index:[%d] ",i);
							_PrintSsStatus(SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status);
							_PrintTeleCommService(SsInfo.SsRecord.ForwardingRecord.rec_class[i].TeleCommService);
							_PrintCFNoReplyWaitTime(SsInfo.SsRecord.ForwardingRecord.rec_class[i].NoReplyWaitTime);
							_PrintCFCondition(SsInfo.SsRecord.ForwardingRecord.rec_class[i].ForwardCondition);
							TEST_DEBUG("bCFNumPresent:[%d] ",SsInfo.SsRecord.ForwardingRecord.rec_class[i].bCallForwardingNumberPresent);
							TEST_DEBUG("bCFNum:[%s] ",SsInfo.SsRecord.ForwardingRecord.rec_class[i].szCallForwardingNumber);
						}
					}
				}
					break;

				case TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF:
				{
					TEST_DEBUG("####  Received Event : TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF  ##### ");
				}
					break;

				case TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF:
				{
					TelSsInfo_t SsInfo = {0,};

					TEST_DEBUG("####  Received Event : TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF #### ");

					if (event->pData!=NULL)
					{
						memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));
						 _PrintSsCliType(SsInfo.SsRecord.CliRecord.IdentificationType);
						_PrintSsCliStatus(SsInfo.SsRecord.CliRecord.CliStatus);
					}

				}
					break;

				case TAPI_EVENT_SS_USSD_CNF:
				{
					TelSsUssdMsgIndInfo_t      ussd_record;
					TEST_DEBUG("####  Received Event : TAPI_EVENT_SS_USSD_CNF #### ");

					if (event->pData != NULL)
					{
						memcpy(&ussd_record, (TelSsUssdMsgIndInfo_t *)event->pData, sizeof(TelSsUssdMsgInfo_t));
						_PrintSsUssdRecord(ussd_record);
					}
					else
					{
						TEST_DEBUG("NULL param");
					}

				}
					break;
				//Indication with ACTION_REQUIRE
				case TAPI_EVENT_SS_USSD_IND:
				{
					TelSsUssdMsgIndInfo_t      ussd_record;
					TEST_DEBUG("####  Received Event : TAPI_EVENT_SS_USSD_IND  ##### ");

					if (event->pData != NULL)
					{
						memcpy(&ussd_record, (TelSsUssdMsgInfo_t *)event->pData, sizeof(TelSsUssdMsgInfo_t));
						_PrintSsUssdRecord(ussd_record);
					}
					else
					{
						TEST_DEBUG("NULL param");
					}

				}
				break;


				case TAPI_EVENT_SS_AOC_RSP:
				{
					TelCallAocInfo_t aoc_info;
					TEST_DEBUG("####  Received Event : SS -- TAPI_EVENT_SS_AOC_RSP---#### ");
					if(event->Status != 0)
					{
						memcpy(&aoc_info, (TelCallAocInfo_t*)event->pData, sizeof(TelCallAocInfo_t));

						TEST_DEBUG("############# AOC TYPE :%d",aoc_info.AocType);
						TEST_DEBUG("Printing ### ACM : [%ld]  MAX_ACM :[%ld] CCM:[%0.4f] Char type:[%d] ",aoc_info.ACM,aoc_info.MaxAcm,aoc_info.CCM,aoc_info.CharTypeOfCurrency);
						TEST_DEBUG("Currency : %c %c %c ",aoc_info.szCurrency[0], aoc_info.szCurrency[1], aoc_info.szCurrency[2]);
					}

				}
					break;
				case TAPI_EVENT_SS_SET_AOC_CNF:
					TEST_DEBUG("####  Received Event : SS -- TAPI_EVENT_SS_SET_AOC_CNF---#### ");
					break;
				case TAPI_EVENT_SS_BARRING_TYPE_IND:
				{
					TelSsCallBarType_t BarringType;
					TEST_DEBUG("####  Received Event : SS -- TAPI_EVENT_SS_BARRING_TYPE_IND---#### ");

					memcpy(&BarringType, (TelSsCallBarType_t*)event->pData, sizeof(TelSsCallBarType_t));
					TEST_DEBUG("########### BarringType received is [%x]",BarringType);
				}
					break;

				default:
					break;
			}
		}
			break;

		default:
			break;
	}
	TEST_DEBUG("###################################################################");

	return 1;

}



int ss_read_key_input(void)
{
	int ret2;
	char buf[256];
	int request_id =0;
	TapiResult_t ret = TAPI_API_SUCCESS;

	ret2 = read(0, buf, sizeof(buf));
	 if (ret2 < 0) {
	  if (errno == EINTR)
	  perror("read(1)");
	  return -1;
	 } else if (ret2 == 0) return ret2;


	printf("\n########## SS TEST Application #########\n");
	printf("########## Supplementary Services  - Options ############\n");

	printf("Call Waiting [1] Call Barring  [2] Call Forwarding [3] \n");
	printf("Query Status [a]    Activate [b]   Deactivate [c]  Registration [d]  Deregistration [e]\n");
	printf("Change Call barring Password  Enter - [cbp]\n");
	printf("Interrogate  the Status of Line Identity Services like CLIP, CLIR etc - Enter [cli]\n");
	printf("Send USSD String  - Enter [ussd]\n");
	printf("Test AOC  - First - SimOpen and Enter PIN2 ---[simopen] [pin2]\n");
	printf("Read AOC Information - Enter [raoc] \n");
	printf("Set Maximum Cost limit- [5e], Set Price/Unit Currency-[5f], Reset Cost Limit-[5g]\n");
	printf("quit - [quit]\n");

	printf("##########################################\n");
	printf("SS Enter key   : %s", buf);

	/* Call waiting */
	if(strncmp(buf, "1a", 2) == 0 )
	{
		char cwType[1];
		TelSsWaitingInfo_t cwInfo = {0,};
		TEST_DEBUG("###	Interrogate the status of Call Waiting	#### ");

		TEST_DEBUG("Select the Call Waiting Type ");
		TEST_DEBUG(" voice-[0], data circuit sync-[6], all tele bearer-[7]");
		ss_fgets(cwType,2);

		cwInfo.CallType = atoi(cwType);

		TEST_DEBUG("SS CallType:[%d]",cwInfo.CallType);

		ret = tel_get_ss_waiting_status(cwInfo.CallType,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	}

	if(strncmp(buf, "1b", 2) == 0 )
	{
		char cwType[1];
		TEST_DEBUG("###	Activate Call Waiting  #### ");
		TelSsWaitingInfo_t cwInfo = {0,};

		TEST_DEBUG("Select the Call Waiting Type ");
		TEST_DEBUG(" voice-[0], data circuit sync-[6], all tele bearer-[7]");
		ss_fgets(cwType,2);

		cwInfo.CallType = atoi(cwType);
		cwInfo.Mode = TAPI_SS_CW_ACTIVATE;

		TEST_DEBUG("SS CallType:[%d]",cwInfo.CallType);
		ret = tel_set_ss_waiting(&cwInfo,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	}

	if(strncmp(buf, "1c", 2) == 0 )
	{
		char cwType[1];
		TEST_DEBUG("###	DeActivate Call Waiting  #### ");
		TelSsWaitingInfo_t cwInfo = {0,};

		TEST_DEBUG("Select the Call Waiting Type ");
		TEST_DEBUG(" voice-[0], data circuit sync-[6], all tele bearer-[7]");
		ss_fgets(cwType,2);

		cwInfo.CallType = atoi(cwType);
		cwInfo.Mode = TAPI_SS_CW_DEACTIVATE;

		TEST_DEBUG("SS CallType:[%d]",cwInfo.CallType);
		ret = tel_set_ss_waiting(&cwInfo,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);

	}

	/* Call barring */
	if(strncmp(buf, "2a", 2) == 0)
	{
		TelSsCallBarringInfo_t  cb_req = {0,};
		char cbType[1];
		char cba_fla[10];
		TEST_DEBUG("####	Interrogate Status of Call barring for Voice Calls 		####");

		TEST_DEBUG("Enter Call Barring Flavor type(BAOC -2, BAIC-6) ");
		TEST_DEBUG(" All Barring -[0] Out Going calls - [1] All outgoing calls [2] ");
		TEST_DEBUG(" Outgoing Intl calls -[3] All ougoing intl calls except home[4] - Barring incoming [5]  All Incoming calls [6]");
		TEST_DEBUG(" All incoming calls in roaming -[7] All incoming calls in Sim [2] ");
		ss_fgets(cba_fla,5);
		cb_req.Type = atoi(cba_fla);

		TEST_DEBUG("Select the SS Call Type ");
		TEST_DEBUG(" voice-[0], data circuit sync-[6], all tele bearer-[7]");
		ss_fgets(cbType,2);
		cb_req.CallType = atoi(cbType);

		TEST_DEBUG("CB type:[%d], SS CallType:[%d]", cb_req.Type, cb_req.CallType);

		ret = tel_get_ss_barring_status(cb_req.Type, cb_req.CallType,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	  }

	if(strncmp(buf, "2b",2) == 0)
	{
		TelSsCallBarringInfo_t  cb_req = {0,};
		char cbType[1];
		char cba_fla[10];
		char new_pwd[10];

		TEST_DEBUG("####	Activate the Call Barring for Voice calls.	####");

		TEST_DEBUG("Enter Call Barring Flavor type(BAOC -2, BAIC-6) ");
		TEST_DEBUG(" All Barring -[0] Out Going calls - [1] All outgoing calls [2] ");
		TEST_DEBUG(" Outgoing Intl calls -[3] All ougoing intl calls except home[4] - Barring incoming [5]  All Incoming calls [6]");
		TEST_DEBUG(" All incoming calls in roaming -[7] All incoming calls in Sim [8] ");

		ss_fgets(cba_fla,5);
		cb_req.Type = atoi(cba_fla);
		TEST_DEBUG(" Enter Call Barring  Password - size minimum 4 bytes");
		ss_fgets(new_pwd,5);
		memcpy(&cb_req.szPassword,&new_pwd,4);
		cb_req.Mode =  TAPI_SS_CALL_BAR_ACTIVATE;

		TEST_DEBUG("Select the SS Call Type ");
		TEST_DEBUG(" voice-[0], data circuit sync-[6], all tele bearer-[7]");
		ss_fgets(cbType,2);
		cb_req.CallType = atoi(cbType);

		TEST_DEBUG("CB type:[%d], SS CallType:[%d]", cb_req.Type, cb_req.CallType);

		ret = tel_set_ss_barring(&cb_req,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	}

	if(strncmp(buf, "2c", 2) == 0)
	{
		TelSsCallBarringInfo_t  cb_req = {0,};
		char cbType[1];

		char cba_fla[10];
		char new_pwd[10];
		TEST_DEBUG("Enter Call Barring Flavor type(BAOC -2, BAIC-6) ");
		TEST_DEBUG(" All Barring -[0] Out Going calls - [1] All outgoing calls [2] ");
		TEST_DEBUG(" Outgoing Intl calls -[3] All ougoing intl calls except home[4] - Barring incoming [5]  All Incoming calls [6]");
		TEST_DEBUG(" All incoming calls in roaming -[7] All incoming calls in Sim [8] ");

		ss_fgets(cba_fla,5);
		cb_req.Type = atoi(cba_fla);

		TEST_DEBUG(" Enter Call Barring  Password - size minimum 4 bytes");
		ss_fgets(new_pwd,5);
		memcpy(&cb_req.szPassword,&new_pwd,4);

		TEST_DEBUG("Select the SS Call Type ");
		TEST_DEBUG(" voice-[0], data circuit sync-[6], all tele bearer-[7]");
		ss_fgets(cbType,2);
		cb_req.CallType = atoi(cbType);

		cb_req.Mode =  TAPI_SS_CALL_BAR_DEACTIVATE;

		TEST_DEBUG("CB type:[%d], SS CallType:[%d]", cb_req.Type, cb_req.CallType);

		ret = tel_set_ss_barring(&cb_req,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret, request_id);
	}


	if(strncmp(buf, "3a", 2) == 0)
	{
		char flavor[1];
		char cfType[1];

		TelSsForwardInfo_t  cfReq ={0,};


		TEST_DEBUG("###	Interrogate the Status of Call forwarding .");
		TEST_DEBUG("Select the Call Forwarding Flavor ");
		TEST_DEBUG(" CF-Unconditional -[0]  CF-Busy [1]  CF-No Reply [2]  CF-Not reachable [3] CF -All forwarding[4] All Cond CF [5]  ");
		ss_fgets(flavor,2);
		cfReq.Condition = atoi(flavor);

		TEST_DEBUG("Select the Call Forwarding Type ");
		TEST_DEBUG("voice -[0] data -[3] fax-[4] all_tele and bearer -[5] ");
		ss_fgets(cfType,2);
		cfReq.Type = atoi(cfType);

		TEST_DEBUG("CF Flavor:[%d], SS CF CallType:[%d]", cfReq.Condition, cfReq.Type);

		ret = tel_get_ss_forward_status(cfReq.Type,  cfReq.Condition,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);

	}
	if(strncmp(buf, "3b", 2) == 0)
	{

		char cf_number[20];
		unsigned char len;
		char flavor[1];
		char cfType[1];

		TelSsForwardInfo_t  cfActReq ={0,};

		TEST_DEBUG("#### Call Forwarding Activation #### ");
		TEST_DEBUG("Enter the Call forwarding number ");
		ss_fgets(cf_number,15);
		len = strlen(cf_number);
		TEST_DEBUG("Length of CF number :%d ",len);
		memcpy(&(cfActReq.szPhoneNumber),cf_number,len);

		TEST_DEBUG("Select the Call Forwarding Flavor ");
		TEST_DEBUG(" CF-Unconditional -[0]  CF-Busy [1]  CF-No Reply [2]  CF-Not reachable [3] CF -All forwarding[4] All Cond CF [5] ");
		ss_fgets(flavor,2);
		cfActReq.Condition = atoi(flavor);
		TEST_DEBUG("********** flavor=%x ",cfActReq.Condition);

		TEST_DEBUG("Select the Call Forwarding Type ");
		TEST_DEBUG("voice -[0] data -[3] fax-[4] all_tele and bearer -[5] ");
		ss_fgets(cfType,2);
		cfActReq.Type = atoi(cfType);

		cfActReq.NoReplyConditionTimer = TAPI_SS_CF_NO_REPLY_TIME_10_SECS;
		cfActReq.Mode=  TAPI_CALL_FORWARD_MODE_ENABLE_EV;

		TEST_DEBUG("CF Flavor:[%d], SS CF CallType:[%d]", cfActReq.Condition, cfActReq.Type);

		ret = tel_set_ss_forward(&cfActReq,&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);

	}
	if(strncmp(buf, "3c", 2) == 0)
	{
		char cf_number[20];
		unsigned char len;
		char flavor[1];
		char cfType[1];
		TelSsForwardInfo_t  cfActReq ={0,};

		TEST_DEBUG("#### Call Forwarding DEActivation #### ");
		TEST_DEBUG("Enter the Call forwarding number ");
		ss_fgets(cf_number,15);
		len = strlen(cf_number);
		TEST_DEBUG("Length of CF number :%d ",len);
		memcpy(&(cfActReq.szPhoneNumber),cf_number,len);

		TEST_DEBUG("Select the Call Forwarding Flavor ");
		TEST_DEBUG(" CF-Unconditional -[0]  CF-Busy [1]  CF-No Reply [2]  CF-Not reachable [3] CF -All forwarding[4] All Cond CF [5] ");

		ss_fgets(flavor,2);
		cfActReq.Condition = atoi(flavor);
		TEST_DEBUG("********** flavor=%x ",cfActReq.Condition);

		TEST_DEBUG("Select the Call Forwarding Type ");
		TEST_DEBUG("voice -[0] data -[3] fax-[4] all_tele and bearer -[5] ");
		ss_fgets(cfType,2);
		cfActReq.Type = atoi(cfType);

		cfActReq .NoReplyConditionTimer = TAPI_SS_CF_NO_REPLY_TIME_10_SECS;
		cfActReq.Mode=  TAPI_CALL_FORWARD_MODE_DISABLE_EV;

		TEST_DEBUG("CF Flavor:[%d], SS CF CallType:[%d]", cfActReq.Condition, cfActReq.Type);

		ret = tel_set_ss_forward(&cfActReq,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);

	}
	if(strncmp(buf, "3d", 2) == 0)
	{

		char cf_number[20];
		unsigned char len;
		char flavor[1];
		char cfType[1];

		TelSsForwardInfo_t  cfActReq ={0,};

		TEST_DEBUG("#### Call Forwarding Registration #### ");
		TEST_DEBUG("Enter the Call forwarding number ");
		ss_fgets(cf_number,15);
		len = strlen(cf_number);
		TEST_DEBUG("Length of CF number :%d ",len);
		memcpy(&(cfActReq.szPhoneNumber),cf_number,len);

		TEST_DEBUG("Select the Call Forwarding Flavor ");
		TEST_DEBUG(" CF-Unconditional -[0]  CF-Busy [1]  CF-No Reply [2]  CF-Not reachable [3] CF -All forwarding[4] All Cond CF [5] ");
		ss_fgets(flavor,2);

		cfActReq.Condition = atoi(flavor);
		TEST_DEBUG("********** flavor=%x ",cfActReq.Condition);


		TEST_DEBUG("Select the Call Forwarding Type ");
		TEST_DEBUG("voice -[0] data -[3] fax-[4] all_tele and bearer -[5] ");
		ss_fgets(cfType,2);

		cfActReq.Type = atoi(cfType);

		cfActReq.NoReplyConditionTimer = TAPI_SS_CF_NO_REPLY_TIME_10_SECS;
		cfActReq.Mode=  TAPI_CALL_FORWARD_MODE_REGISTRATION_EV;

		TEST_DEBUG("CF Flavor:[%d], SS CF CallType:[%d]", cfActReq.Condition, cfActReq.Type);

		ret = tel_set_ss_forward(&cfActReq,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);


	}
	if(strncmp(buf, "3e", 2) == 0)
	{
		char cf_number[20];
		unsigned char len;
		char flavor[1];
		char cfType[1];
		TelSsForwardInfo_t  cfActReq ={0,};

		TEST_DEBUG("#### Call Forwarding Deregistration #### ");
		TEST_DEBUG("Enter the Call forwarding number ");
		ss_fgets(cf_number,15);
		len = strlen(cf_number);
		TEST_DEBUG("Length of CF number :%d ",len);
		memcpy(&(cfActReq.szPhoneNumber),cf_number,len);

		TEST_DEBUG("Select the Call Forwarding Flavor ");
		TEST_DEBUG(" CF-Unconditional -[0]  CF-Busy [1]  CF-No Reply [2]  CF-Not reachable [3] CF -All CF [4] All Cond CF [5]  ");
		ss_fgets(flavor,2);
		cfActReq.Condition = atoi(flavor);
		TEST_DEBUG("********** flavor=%x ",cfActReq.Condition);

		TEST_DEBUG("Select the Call Forwarding Type ");
		TEST_DEBUG("voice -[0] data -[3] fax-[4] all_tele and bearer -[5] ");
		ss_fgets(cfType,2);

		cfActReq.Type = atoi(cfType);

		cfActReq.NoReplyConditionTimer = TAPI_SS_CF_NO_REPLY_TIME_10_SECS;
		cfActReq.Mode=  TAPI_CALL_FORWARD_MODE_ERASURE_EV;

		TEST_DEBUG("CF Flavor:[%d], SS CF CallType:[%d]", cfActReq.Condition, cfActReq.Type);

		ret = tel_set_ss_forward(&cfActReq,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	}


	/* Change Call barring password */
	if(strncmp(buf, "cbp",3) == 0)
	{
		char old_pwd[10];
		char new_pwd[10];
		char new_pwd_again[10];

		memset(old_pwd, 0, 10);
		memset(new_pwd, 0, 10);
		memset(new_pwd_again, 0, 10);

		TEST_DEBUG("####	Change the Call Barring Password 	####");
		TEST_DEBUG(" Enter Call Barring Old Password - size minimum 4 bytes");
		ss_fgets(old_pwd,5);

		TEST_DEBUG(" Enter Call Barring New Password - size minimum 4 bytes");
		ss_fgets(new_pwd,5);

		TEST_DEBUG(" Enter Call Barring New Password Again - size minimum 4 bytes");
		ss_fgets(new_pwd_again,5);

		ret = tel_change_ss_barring_password(old_pwd, new_pwd, new_pwd_again,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	}

	/* CLI */
	if(strncmp(buf, "cli", 3) == 0)
	{
		char cli_type[1];
		TEST_DEBUG("####	Interrogate the Status of CLI Services (except CDIP) ####");
		TEST_DEBUG(" CLIP -[1] CLIR -[2] COLP -[3] COLR-[4] CDIP-[5] CNAP -[6] ");
		ss_fgets(cli_type,2);

		TEST_DEBUG("CLI type:[%d]", atoi(cli_type));

		ret = tel_get_ss_cli_status(atoi(cli_type),&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret,request_id);
	}
	/* USSD */
	if(strncmp(buf, "ussd", 4) == 0)
	{

		char ussdString[TAPI_SS_USSD_DATA_SIZE_MAX];
		int  ret;

		memset(ussdString, 0, TAPI_SS_USSD_DATA_SIZE_MAX);
		strcpy(ussdString , "*124#");

		TEST_DEBUG("ussd string:[%s],ussd len:[%d]", ussdString, strlen(ussdString));
		ret= tel_send_ss_ussd_request(ussdString,&request_id);
		TEST_DEBUG(" API RETURN VAL:[%d]",ret);

	}

    /* Read AOC */
    if(strncmp(buf, "raoc", 4) == 0)
	{
		char aoc_type[1];
		TEST_DEBUG("Read AOC Information - Select AOC type");
		TEST_DEBUG("ACM Accumulated Call meter -[1]  CCM -[2]  Cost Limit MAX ACM -[4]  Price per unit currency PUC [8]");

		ss_fgets(aoc_type,2);
		TEST_DEBUG("Aoc Type :%d",atoi(aoc_type));

		ret = tel_get_ss_aoc_info(atoi(aoc_type),&request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret);
	}

	/* SIM OPEN */
	if(strncmp(buf, "simopen", 7) == 0)
	{
		int i = 0;
		TelSimStatusData_t SimOpenData = {0,};

		TEST_DEBUG("********************************************");

		TEST_DEBUG("open sim");
	}

	/* Enter PIN2 */
	if(strncmp(buf, "pin2", 4) == 0)
	{
		/* nothing */
	}

	if(strncmp(buf, "5e", 2) == 0)
	{
		TEST_DEBUG("########### Setting Maximum Cost Limit -  MAX_ACM Value..############# ");
		TelCallAocInfo_t pAocSetInfo = {0,};
		char maxacm_val[10];
		TEST_DEBUG("Enter the MAX ACM value ");
		ss_fgets(maxacm_val,5);

		pAocSetInfo.AocType = TAPI_SS_AOC_TYPE_MAXACM;

		pAocSetInfo.MaxAcm =atoi(maxacm_val);

		ret = tel_set_ss_aoc_info(&pAocSetInfo, &request_id);

		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret, request_id);
	}
	if(strncmp(buf, "5f", 2) == 0)
	{
		TEST_DEBUG("########### Setting Price/Unit Currency - PPUC Value.############# ");
		TelCallAocInfo_t *pAocSetInfo;
		char ppu_curr[10];

		TEST_DEBUG("Enter price per unit currency Characters ");
		ss_fgets(ppu_curr,3);
		pAocSetInfo = calloc(1,sizeof(TelCallAocInfo_t));

		pAocSetInfo->AocType = TAPI_SS_AOC_TYPE_PUC;


		pAocSetInfo->PPM = 0x02;

		pAocSetInfo->CharTypeOfCurrency = 0x02;
		pAocSetInfo->szCurrency[0] = ppu_curr[0];
		pAocSetInfo->szCurrency[1] = ppu_curr[1];
		pAocSetInfo->szCurrency[2] = ppu_curr[2];

		TEST_DEBUG("Set  AOC Request");

		ret = tel_set_ss_aoc_info(pAocSetInfo,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret, request_id);

		free(pAocSetInfo);

	}
	if(strncmp(buf, "5g", 2) == 0)
	{
		TEST_DEBUG("################## Resetting Total Cost   MAX ACM..################ ");;
		TelCallAocInfo_t  *pAocSetInfo;
		pAocSetInfo = calloc(1,sizeof(TelCallAocInfo_t));

		pAocSetInfo->AocType = TAPI_SS_AOC_TYPE_RESET;

		ret = tel_set_ss_aoc_info(pAocSetInfo,&request_id);
		TEST_DEBUG("Tapi return api_err value is [0x%x]  RequestID:[%d]", ret, request_id);

		free(pAocSetInfo);
	}

	if(strncmp(buf, "quit", 4) == 0)
	{
		TEST_DEBUG("Finished Testing this module, Going back to Main Menu");
		return -1;
	}

	memset(buf, '\0', 256);
	return 0;
}


/*****************************************************************/

void ss_select_loop(void)
{
	int ret;
	fd_set readfds;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);


	 ret = select(0 + 1, &readfds, NULL, NULL, NULL);
        if (ret) {

	    if (FD_ISSET(0, &readfds)) {
		  if(ss_read_key_input()<0)
			break;
            }
        }
    }
}

int ss_test_subscribe_tapi_events(void)
{
	int i = 0;
	int ret_val = TRUE;
	int iNumOfSSEvt = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;


	int SSEvtList[] =
	{
		TAPI_EVENT_SS_BARRING_CNF,   /**<0x0400,	This confirmation event is sent when Barring activation/deactivation is requested*/
		TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF,   /**<	This event is returned as Barring password change confirmation*/
		TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF,   /**<	This event is returned  as Barring status query confirmation*/
		TAPI_EVENT_SS_FORWARD_CNF,   /**<	This event is returned as confirmation for Forward Activation/Deactivation/Registration/Erasure */
		TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF,   /**<	This event is returned as confirmation for Forward Status query confirmation*/
		TAPI_EVENT_SS_WAITING_CNF,   /**<	This event is returned as confirmation for Call waiting activation/deactivation*/
		TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF,   /**<	This event is returned as Call waiting status query confirmation*/
		TAPI_EVENT_SS_USSD_CNF,   /**<	This event is returned as USSD send confirmation*/
		TAPI_EVENT_SS_USSD_CANCEL_CNF,	/**<	This event is returned as USSD cancel confirmation	>**/
		TAPI_EVENT_SS_USSD_IND,   /**<	This is an unsolicited USSD indication*/
		TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF,   /**<	This event is indicated to the Application for CLI service interrogation*/
		TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY, /**<	This event is indicated to the Application when tapi receives Release complete message notification for any SS transactions*/
		TAPI_EVENT_SS_AOC_RSP,				/**<This event is used to send the aoc information */
		TAPI_EVENT_SS_SET_AOC_CNF,			/**<This event is used to send the confirmation for setting aoc values */
		TAPI_EVENT_SS_BARRING_TYPE_IND,		/**<This event is used to send the barring type while making an MO call */
	};

	iNumOfSSEvt = sizeof(SSEvtList)/sizeof(int);
	TAPI_CALLOC(SS_subscription_id, iNumOfSSEvt, unsigned int)


	for( i=0 ; i< iNumOfSSEvt; i++ )
	{
		api_err = tel_register_event(SSEvtList[i], &SS_subscription_id[i],(TelAppCallback)&ss_async_event_callback,NULL);
		if(api_err != TAPI_API_SUCCESS)
		{
			TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", SSEvtList[i],api_err);
			ret_val = FALSE;
			break;
		}
	}

	return ret_val;

}
