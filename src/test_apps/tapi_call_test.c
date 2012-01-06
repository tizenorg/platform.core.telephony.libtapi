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
#include <string.h>
#include <sys/time.h>
#include <pthread.h>     /* pthread functions and data structures */
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include "assert.h"
#include "glib.h"
#include "unistd.h"

#include "TelErr.h"
#include "ITapiPower.h"
#include "ITapiCall.h"
#include "TapiCommon.h"
#include "TelCall.h"
#include "vconf.h"

#ifndef false
	#define false 0
#endif
#ifndef true
	#define true  !0
#endif
#ifndef bool
	#define bool  char
#endif

#define _CALL_TEST_KOREA_FMT_		4
#define _CALL_TEST_INDIA_FMT_		10

#define TEST_DEBUG(frmt, args...)	\
	{do { fprintf(stderr, "[Call Test][%s:%04d] " frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}

#define AUTO_LOG(frmt, args...)


#define MAX_BURST_DTMF_STRLEN		32

extern int EvtDeliveryHandle ;


typedef struct
{
	TS_UINT	handle;
	TelTapiCallInfo_t	info;
	TS_BOOL			used;
	int				index;
} call_ctxt_t;

// same as tapi event
typedef struct
{
	int group;
	int type;
	int ID;
	int status;
	void *dst;
}data_t;

int error_code;
static TelCallActiveLine_t		_glob_test_curr_line = 0x00;
//static TS_UINT		endHCall = -1;
static long	__call_test_auto_mo = 0;
static long	__call_test_auto_ans = 0;
static long	 _timer_exp_count = 0;

call_ctxt_t		ctxt[TAPI_CONTEXT_CALL_SIZE_MAX];
TelCallStatus_t	gcallStatus[TAPI_CONTEXT_CALL_SIZE_MAX];
TS_UINT	incoming_handle= -1;
TS_UINT	curr_handle= -1;
int requestID;

FILE *log_fd;

TelCallSetupParams_t _glob_setup_info;

struct sigaction sa;
struct itimerval timer;

/*	Success returns 0	*/
extern pthread_t	  __glob_tapi_keypad_thread;

//static int  _country_spc_num_ = 0;
char pre_string[16] = {0, };
int _auto_dtmf_send_count = 0;
int _auto_dtmf_cnf_count = 0;

static unsigned int *	CALL_subscription_id;

void __PrintCallType(char* str, TelCallType_t eCallType)
{
	switch(eCallType)
	{
		case TAPI_CALL_TYPE_VOICE:
			fprintf(stderr, "%s [TAPI_CALL_TYPE_VOICE]", str);
			break;
		case TAPI_CALL_TYPE_DATA:
			fprintf(stderr, "%s [TAPI_CALL_TYPE_DATA]", str);
			break;
		case TAPI_CALL_TYPE_E911:
			fprintf(stderr, "%s [TAPI_CALL_TYPE_E911]", str);
			break;
		default:
			fprintf(stderr, "%s Undefined Call Type:[0x%x]", str, eCallType);
			break;
	}
}

void __PrintActiveCallState(char* str, TelCallActiveState_t eCallActiveState)
{
	switch(eCallActiveState)
	{
		case TAPI_CALL_CONNECT_IDLE:
			fprintf(stderr, "%s [TAPI_CALL_CONNECT_IDLE]", str);
			break;
		case TAPI_CALL_CONNECT_ACTIVE:
			fprintf(stderr, "%s [TAPI_CALL_CONNECT_ACTIVE]", str);
			break;
		case TAPI_CALL_CONNECT_HELD:
			fprintf(stderr, "%s [TAPI_CALL_CONNECT_HELD]", str);
			break;
		default:
			fprintf(stderr, "%s Undefined Call Active State", str);
			break;
	}
}

void __PrintCallState(char* str, TelCallStates_t eCallState)
{
	switch(eCallState)
	{
		case TAPI_CALL_STATE_IDLE:
			fprintf(stderr, "%s [TAPI_CALL_STATE_IDLE]", str);
			break;
		case TAPI_CALL_STATE_ORIG:
			fprintf(stderr, "%s [TAPI_CALL_STATE_ORIG]", str);
			break;
		case TAPI_CALL_STATE_SETUPIND:
			fprintf(stderr, "%s [TAPI_CALL_STATE_SETUPIND]", str);
			break;
		case TAPI_CALL_STATE_CONNECTING:
			fprintf(stderr, "%s [TAPI_CALL_STATE_CONNECTING]", str);
			break;
		case TAPI_CALL_STATE_ALERT:
			fprintf(stderr, "%s [TAPI_CALL_STATE_ALERT]", str);
			break;
		case TAPI_CALL_STATE_INCOM:
			fprintf(stderr, "%s [TAPI_CALL_STATE_INCOM]", str);
			break;
		case TAPI_CALL_STATE_ANSWER:
			fprintf(stderr, "%s [TAPI_CALL_STATE_ANSWER]", str);
			break;
		case TAPI_CALL_STATE_CONV:
			fprintf(stderr, "%s [TAPI_CALL_STATE_CONV]", str);
			break;
		case TAPI_CALL_STATE_ENDING:
			fprintf(stderr, "%s [TAPI_CALL_STATE_ENDING]", str);
			break;
		case TAPI_CALL_STATE_INVALID:
			fprintf(stderr, "%s [TAPI_CALL_STATE_INVALID]", str);
			break;
		default:
			fprintf(stderr, "%s Undefined Call State", str);
			break;
	}
}

void __PrintCallFwType(char* str, TelCallForwardType_t eCallFwType)
{
	switch(eCallFwType)
	{
		case TAPI_CALL_OUTGOING_FWDED:
			fprintf(stderr, "%s [TAPI_CALL_OUTGOING_FWDED]", str);
			break;
		case TAPI_CALL_INCOMMING_FWDED:
			fprintf(stderr, "%s [TAPI_CALL_INCOMMING_FWDED]", str);
			break;
		default:
			fprintf(stderr, "%s Undefined Call ForwardType", str);
			break;
	}
}


int call_test_ret_first_free_index(void);



void timer_handler (int signum)
{
	TEST_DEBUG("Timer Expired [%ld] times", ++_timer_exp_count);

	/*	Free the current call context	*/

}


void timer_reset()
{
	 /* Configure the timer to expire after 250 msec... */
	 timer.it_value.tv_sec = 60;
	 timer.it_value.tv_usec = 0;
	 /* ... and every 250 msec after that. */
	 timer.it_interval.tv_sec = 0;
	 timer.it_interval.tv_usec = 0;
}


void timer_start()
{
	 /* Start a virtual timer. It counts down whenever this process is
	   executing. */
	 setitimer (ITIMER_REAL, &timer, NULL);
}



void call_test_auto_hold(void)
{
	TapiResult_t	api_err;

	TEST_DEBUG("Request to Auto Hold call with Handle [%d]", curr_handle);

	api_err = tel_hold_call(curr_handle, &requestID);

	TEST_DEBUG("Tapi API Err Code [%x], Auto MO Count [%ld]", api_err, __call_test_auto_mo);
	AUTO_LOG("API: tel_hold_call:-IN Handle [%d], Return [%d]", curr_handle, api_err);
  }



void call_test_auto_act(void)
{
	TapiResult_t	api_err;

	TEST_DEBUG("Request to Auto Activate Call with Handle [%d]", curr_handle);

	api_err = tel_retrieve_call(curr_handle, &requestID);

	TEST_DEBUG("Tapi API Err Code [%x], Auto Count [%ld]", api_err, __call_test_auto_ans);
	AUTO_LOG("API: tel_retrieve_call:-IN Handle [%d], Return [%d]", curr_handle, api_err);
  }


void call_test_auto_ans(void)
{
	TapiResult_t	api_err;

	TEST_DEBUG("Request to Auto Ans call with Handle [%d]", incoming_handle);

	__call_test_auto_ans++;

	api_err = tel_answer_call(incoming_handle, TAPI_CALL_ANSWER_ACCEPT, &requestID);

	TEST_DEBUG("Tapi API Err Code [%x], Auto Count [%ld]", api_err, __call_test_auto_ans);

	AUTO_LOG("API: tel_answer_call:- IN Hanlde [%d] Return [%d]", incoming_handle, api_err);
  }





void call_test_auto_mo(void)
{
	int	free_id;
	TapiResult_t	api_err;
	int RequestId=0;

	TEST_DEBUG("Request to Auto Setup call with Number ...%s", _glob_setup_info.szNumber);

	__call_test_auto_mo++;

	api_err = tel_exe_call_mo(&_glob_setup_info, &curr_handle, &RequestId);

	TEST_DEBUG("After calling tel_exe_call_mo : Handle [%d]", curr_handle);

	TEST_DEBUG("Tapi API Err Code [%x], Auto Count [%ld]", api_err, __call_test_auto_mo);

	if(api_err == TAPI_API_SUCCESS)
	{
		free_id = call_test_ret_first_free_index();

		if (free_id == -1)
		{
			TEST_DEBUG("No Free Context ...");
		}

		else
		{
			memcpy(&ctxt[free_id].info.callNumber, _glob_setup_info.szNumber, sizeof( _glob_setup_info.szNumber));
			ctxt[free_id].info.callState = TAPI_CALL_STATE_ORIG;
			ctxt[free_id].info.activeState = TAPI_CALL_CONNECT_IDLE;
			ctxt[free_id].info.isMobileOriginated = true;
			ctxt[free_id].handle = curr_handle;
		}

		AUTO_LOG("API: tel_exe_call_mo:- Return [%d], Handle [%d]", api_err, curr_handle);

	}


  }



void call_test_count_mo()
{
	if (__call_test_auto_mo)
		call_test_auto_mo();
	else
		TEST_DEBUG("Non Automated testing...");
}


void call_test_count_ans()
{
	if (__call_test_auto_ans)
		call_test_auto_ans();
	else
		TEST_DEBUG("Non Automated testing...");
}

void call_test_auto_dtmf()
{
	int api_err = 0;
	int RequestID;

	api_err = tel_send_call_dtmf("9\0", &RequestID);
    TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
    _auto_dtmf_send_count++;
	AUTO_LOG("API: tel_send_call_dtmf: Send Count [%d]", _auto_dtmf_send_count);
}


TS_UINT  call_test_ret_handle_by_active_state(void)
{
	int start = 0;
	int max = TAPI_CONTEXT_CALL_SIZE_MAX;

	for (start = 0; start < max; start++)
	{
		if (ctxt[start].info.activeState == TAPI_CALL_CONNECT_ACTIVE)
		{
			TEST_DEBUG("Current Active Call Handle is [%d]", ctxt[start].handle);
			return ctxt[start].handle;
		}
		else
		{
			//
		}
	}

	if (start == max)
	{
		TEST_DEBUG("Could not find Call with Active State")
	}

	return -1;

}




int call_test_ret_index_by_handle(TS_UINT handle)
{
	int start = 0;
	int max = TAPI_CONTEXT_CALL_SIZE_MAX;

	for (start = 0; start < max; start++)
	{
		if (ctxt[start].handle == handle)
		{
			return start;
		}
		else
		{
			//
		}
	}

	if (start == max)
	{
		TEST_DEBUG("Could not find CTXT")
	}

	return -1;

}



int call_test_ret_first_free_index(void)
{
	int start = 0;
	int max = TAPI_CONTEXT_CALL_SIZE_MAX;

	for (start = 0; start < max; start++)
	{
		/*	by default they are free	*/
		if (ctxt[start].used == false)
		{
			//TEST_DEBUG("Creating a Context with Index [%d]", start);

			ctxt[start].used = true;
			ctxt[start].index = start;

			return start;
		}
		else
		{
			//
		}
	}

	if (start == max)
	{
		TEST_DEBUG("Could not find FREE CTXT")
	}

	return -1;

}


/*	Create a context and populate the data	*/
void call_test_process_incom_ind(void *data)
{
	int free_id = -1;
	TelCallIncomingCallInfo_t	IncomingInfo = {0, };
	int len;

	free_id = call_test_ret_first_free_index();

	if (free_id == -1)
	{
		//
	}
	else
	{
		memcpy(&IncomingInfo, data, sizeof(TelCallIncomingCallInfo_t));

		ctxt[free_id].handle = IncomingInfo.CallHandle;

		len = strlen(IncomingInfo.szCallingPartyNumber);

		memcpy(&ctxt[free_id].info.callNumber, IncomingInfo.szCallingPartyNumber, len);

		ctxt[free_id].info.callState = TAPI_CALL_STATE_INCOM;
		ctxt[free_id].info.callType = IncomingInfo.CallType;
		//ctxt[free_id].info.CliCause = IncomingInfo.CliCause;

		TEST_DEBUG("Incoming Call Information: call handle:[%d], Number [%s], Type [0x%x]",
			ctxt[free_id].handle, ctxt[free_id].info.callNumber, ctxt[free_id].info.callType);

		TEST_DEBUG("No CLI Cause [0x%x]",  IncomingInfo.CliCause);

		incoming_handle = ctxt[free_id].handle;

		AUTO_LOG("EVENT: INCOMING IND:- Hanlde [%d], Number [%s]",
			incoming_handle, ctxt[free_id].info.callNumber);

		/*	if the test is automated, then call ans api	*/
		call_test_count_ans();

	}

}

//static void  call_test_process_events (TelTapiEventClass_t eventClass, int eventType, void * param2)
static void  call_async_event_callback(TelTapiEvent_t	*event, void *data)
{
	TS_UINT	temp_handle = -1;
	int	RequestId=0;

	TEST_DEBUG("EVENT TYPE:[0x%x], STATUS:[0x%x]", event->EventType, event->Status);

	int *userdata;
	userdata = (int *)data;

	switch(event->EventClass)
	{
		case TAPI_EVENT_CLASS_CALL_VOICE:
			switch(event->EventType)
			{
				case TAPI_EVENT_CALL_SWAP_CNF:
				{
						TEST_DEBUG("TAPI_EVENT_CALL_SWAP_CNF");
				}
					break;

				case TAPI_EVENT_CALL_ANSWER_CNF:
				{
						TEST_DEBUG("TAPI_EVENT_CALL_ANSWER_CNF");
				}
					break;

				case TAPI_EVENT_CALL_RELEASE_CNF:
				{
						TEST_DEBUG("TAPI_EVENT_CALL_RELEASE_CNF");
				}
					break;

				case TAPI_EVENT_CALL_RELEASE_ALL_CNF:
				{
						TEST_DEBUG("TAPI_EVENT_CALL_RELEASE_ALL_CNF");
				}
					break;

				case TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF:
				{
						TEST_DEBUG("TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF");
				}
					break;

				case TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF:
				{
						TEST_DEBUG("TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF");
				}
					break;

				case TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF");
					TelCallVoicePrivacyInfo_t vp_info={0,};
					memcpy(&vp_info, event->pData, sizeof(TelCallVoicePrivacyInfo_t));
					switch(vp_info.PrivacyType)
					{
						case TAPI_CALL_PRIVACY_TYPE_MS:
							TEST_DEBUG("TAPI_CALL_PRIVACY_TYPE_MS");
							break;
						case TAPI_CALL_PRIVACY_TYPE_BS:
							TEST_DEBUG("TAPI_CALL_PRIVACY_TYPE_BS");
							break;
						case TAPI_CALL_PRIVACY_TYPE_CURRENT:
							TEST_DEBUG("TAPI_CALL_PRIVACY_TYPE_CURRENT");
							break;
						default:
							TEST_DEBUG("NONE");
							break;
					}
					switch(vp_info.PrivacyMode)
					{
						case TAPI_CALL_PRIVACY_MODE_STANDARD:
							TEST_DEBUG("TAPI_CALL_PRIVACY_MODE_STANDARD");
							break;
						case TAPI_CALL_PRIVACY_MODE_ENHANCED:
							TEST_DEBUG("TAPI_CALL_PRIVACY_MODE_ENHANCED");
							break;
						default:
							TEST_DEBUG("NONE");
							break;
					}
				}
					break;

				case TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF");
				}
					break;

				case TAPI_EVENT_CALL_PRIVACYMODE_IND:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_PRIVACYMODE_IND");
					TelCallVoicePrivacyInfo_t vp_info={0,};
					memcpy(&vp_info, event->pData, sizeof(TelCallVoicePrivacyInfo_t));
					switch(vp_info.PrivacyType)
					{
						case TAPI_CALL_PRIVACY_TYPE_MS:
							TEST_DEBUG("TAPI_CALL_PRIVACY_TYPE_MS");
							break;
						case TAPI_CALL_PRIVACY_TYPE_BS:
							TEST_DEBUG("TAPI_CALL_PRIVACY_TYPE_BS");
							break;
						case TAPI_CALL_PRIVACY_TYPE_CURRENT:
							TEST_DEBUG("TAPI_CALL_PRIVACY_TYPE_CURRENT");
							break;
						default:
							TEST_DEBUG("NONE");
							break;
					}
					switch(vp_info.PrivacyMode)
					{
						case TAPI_CALL_PRIVACY_MODE_STANDARD:
							TEST_DEBUG("TAPI_CALL_PRIVACY_MODE_STANDARD");
							break;
						case TAPI_CALL_PRIVACY_MODE_ENHANCED:
							TEST_DEBUG("TAPI_CALL_PRIVACY_MODE_ENHANCED");
							break;
						default:
							TEST_DEBUG("NONE");
							break;
					}
				}
					break;

				case TAPI_EVENT_CALL_FLASHINFO_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_FLASHINFO_CNF");
				}
					break;

				case TAPI_EVENT_CALL_EXIT_EMERGENCYMODE_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_EXIT_EMERGENCYMODE_CNF");
				}
					break;

				case TAPI_EVENT_CALL_EMERGENCYMODE_IND:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_EMERGENCYMODE_IND");
					TelCallEmergecyMode_t mode= 0;
					memcpy(&mode, event->pData, sizeof(TelCallEmergecyMode_t));
					switch(mode)
					{
						case TAPI_CALL_EMERGENCY_MODE_EXIT:
							TEST_DEBUG("TAPI_CALL_EMERGENCY_MODE_EXIT");
							break;
						case TAPI_CALL_EMERGENCY_MODE_ENTER:
							TEST_DEBUG("TAPI_CALL_EMERGENCY_MODE_ENTER");
							break;
						default:
							TEST_DEBUG("NONE");
							break;
					}
				}
					break;

				case TAPI_EVENT_CALL_TIME_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_TIME_CNF");
					TelCallTimeInfo_t calltime_info={0,};
					memcpy(&calltime_info, event->pData, sizeof(TelCallTimeInfo_t));
					TEST_DEBUG("req_type_mask = [0x%x]",calltime_info.req_type_mask);
					TEST_DEBUG("TotalCallCnt = [%ld]",calltime_info.TotalCallCnt);
					TEST_DEBUG("OutgoingCallCnt = [%ld]",calltime_info.OutgoingCallCnt);
					TEST_DEBUG("IncomingCallCnt = [%ld]",calltime_info.IncomingCallCnt);
					TEST_DEBUG("TotalCallTime = [%ld]",calltime_info.TotalCallTime);
					TEST_DEBUG("OutgoingCallTime = [%ld]",calltime_info.OutgoingCallTime);
					TEST_DEBUG("IncomingCallTime = [%ld]",calltime_info.IncomingCallTime);
					TEST_DEBUG("LastCallTime = [%ld]",calltime_info.LastCallTime);
				}
					break;

				case TAPI_EVENT_CALL_TIME_IND:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_TIME_IND");
					TelCallTimeInfo_t calltime_info={0,};
					memcpy(&calltime_info, event->pData, sizeof(TelCallTimeInfo_t));
					TEST_DEBUG("req_type_mask = [%d]",calltime_info.req_type_mask);
					TEST_DEBUG("TotalCallCnt = [%ld]",calltime_info.TotalCallCnt);
					TEST_DEBUG("OutgoingCallCnt = [%ld]",calltime_info.OutgoingCallCnt);
					TEST_DEBUG("IncomingCallCnt = [%ld]",calltime_info.IncomingCallCnt);
					TEST_DEBUG("TotalCallTime = [%ld]",calltime_info.TotalCallTime);
					TEST_DEBUG("OutgoingCallTime = [%ld]",calltime_info.OutgoingCallTime);
					TEST_DEBUG("IncomingCallTime = [%ld]",calltime_info.IncomingCallTime);
					TEST_DEBUG("LastCallTime = [%ld]",calltime_info.LastCallTime);
				}
					break;

				case TAPI_EVENT_CALL_OTA_PROGRESS_IND:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_OTA_PROGRESS_IND");
					TelCallOtaProgressInfo_t ota_info={0,};
					memcpy(&ota_info, event->pData, sizeof(TelCallOtaProgressInfo_t));
					switch(ota_info.OtaType)
					{
						case TAPI_CALL_OTATYPE_OTASP:
						{
							TEST_DEBUG("TAPI_CALL_OTATYPE_OTASP");

							switch(ota_info.OtaspStatus)
							{
								case TAPI_CALL_OTASP_OK_SPL_UNLOCKED:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_SPL_UNLOCKED");
									break;
								case TAPI_CALL_OTASP_OK_AKEY_EXCESS:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_AKEY_EXCESS");
									break;
								case TAPI_CALL_OTASP_OK_SSD_UPDATE:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_SSD_UPDATE");
									break;
								case TAPI_CALL_OTASP_OK_NAM_DWNLD:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_NAM_DWNLD");
									break;
								case TAPI_CALL_OTASP_OK_MDN_DWNLD:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_MDN_DWNLD");
									break;
								case TAPI_CALL_OTASP_OK_IMSI_DWNLD:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_IMSI_DWNLD");
									break;
								case TAPI_CALL_OTASP_OK_PRL_DWNLD:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_PRL_DWNLD");
									break;
								case TAPI_CALL_OTASP_OK_COMMIT:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_COMMIT");
									break;
								case TAPI_CALL_OTASP_OK_PROGRAMMING:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_PROGRAMMING");
									break;
								case TAPI_CALL_OTASP_SUCCESS:
									TEST_DEBUG("TAPI_CALL_OTASP_SUCCESS");
									break;
								case TAPI_CALL_OTASP_UNSUCCESS:
									TEST_DEBUG("TAPI_CALL_OTASP_UNSUCCESS");
									break;
								case TAPI_CALL_OTASP_OK_OTAPA_VERIFY:
									TEST_DEBUG("TAPI_CALL_OTASP_OK_OTAPA_VERIFY");
									break;
								case TAPI_CALL_OTASP_PROGRESS:
									TEST_DEBUG("TAPI_CALL_OTASP_PROGRESS");
									break;
								case TAPI_CALL_OTASP_FAILURES_EXCESS_SPC:
									TEST_DEBUG("TAPI_CALL_OTASP_FAILURES_EXCESS_SPC");
									break;
								case TAPI_CALL_OTASP_LOCK_CODE_PW_SET:
									TEST_DEBUG("TAPI_CALL_OTASP_LOCK_CODE_PW_SET");
									break;
								default:
									TEST_DEBUG("NONE");
									break;
							}

						}
							break;
						case TAPI_CALL_OTATYPE_OTAPA:
						{
							TEST_DEBUG("TAPI_CALL_OTATYPE_OTAPA");

							switch(ota_info.OtapaStatus)
							{
								case TAPI_CALL_OTAPA_STOP:
									TEST_DEBUG("TAPI_CALL_OTAPA_STOP");
									break;
								case TAPI_CALL_OTAPA_START:
									TEST_DEBUG("TAPI_CALL_OTAPA_START");
									break;
								default:
									TEST_DEBUG("NONE");
									break;
							}

						}
							break;
						default:
							TEST_DEBUG("NONE");
							break;
					}
				}
					break;

				case TAPI_EVENT_CALL_SETUP_CNF:
				{
					TEST_DEBUG("TAPI_EVENT_CALL_SETUP_CNF");
					int	free_id;

					if(event->Status == 0)
					{
						memcpy(&temp_handle, event->pData, sizeof(TS_UINT));
						TEST_DEBUG("Received Call Setup for call Handle [%d]", temp_handle);

						free_id = call_test_ret_first_free_index();

						if (free_id == -1)
						{
							TEST_DEBUG("No Free Context id ...");
						}

						else
						{
							memcpy(&ctxt[free_id].info.callNumber, _glob_setup_info.szNumber, sizeof( _glob_setup_info.szNumber));
							ctxt[free_id].info.callState = TAPI_CALL_STATE_SETUPIND;
							ctxt[free_id].info.activeState = TAPI_CALL_CONNECT_IDLE;
							ctxt[free_id].info.isMobileOriginated = true;
							ctxt[free_id].handle = temp_handle;

						}
					}
					else
						TEST_DEBUG("event->Status is not zero..");
				}
					break;

				case TAPI_EVENT_CALL_ALERT_IND:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_ALERT_IND");
					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));
					TEST_DEBUG("Received Alert for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.callState = TAPI_CALL_STATE_ALERT;

					AUTO_LOG("EVENT: ALERT_IND:- Handle [%d]", temp_handle);
				}
					break;

				case TAPI_EVENT_CALL_INCOM_IND:
				{
					TEST_DEBUG("\n$$$$$$$$$$$$$$	TAPI_EVENT_CALL_INCOM_IND	$$$$$$$$$$$$$$\n");
					call_test_process_incom_ind(event->pData);
				}
					break;

				case TAPI_EVENT_CALL_WAITING_IND:
					TEST_DEBUG("TAPI_EVENT_CALL_WAITING_IND");
					break;

				case TAPI_EVENT_CALL_END_IND:
				{
					int	index;
					TelCallEndInfo_t EndInfo = {0, };

					TEST_DEBUG("TAPI_EVENT_CALL_END_IND");

					memcpy(&EndInfo, event->pData, sizeof(TelCallEndInfo_t));

					TEST_DEBUG("Call Handle [%d], End Reason [%x]",
						EndInfo.pCallHandle, EndInfo.CallEndCause);

					index = call_test_ret_index_by_handle(EndInfo.pCallHandle);

					/*	Free this context..	*/
					ctxt[index].used = false;

					memset(&ctxt[index].info, 0, sizeof(TelTapiCallInfo_t));

					AUTO_LOG("EVENT: END_IND:- Handle [%d] CauseX [%x]",
						EndInfo.pCallHandle, EndInfo.CallEndCause);

					/*	Continue Auto MO Call	*/
					call_test_count_mo();
				}

				break;

				case TAPI_EVENT_CALL_CONNECTED_IND:
				{
					int	index;
					//TapiResult_t	api_err = TAPI_API_SUCCESS;

					TEST_DEBUG("TAPI_EVENT_CALL_CONNECTED_IND");

					/*	Stop the Timer..	*/
					//timer_reset();

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Connected Event  for call Handle [%d]", temp_handle);

					curr_handle =  temp_handle;

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.callState = TAPI_CALL_STATE_CONV;
					ctxt[index].info.activeState = TAPI_CALL_CONNECT_ACTIVE;

					AUTO_LOG("EVENT: CONNECTED_IND:- Handle [%d]", temp_handle);

					/*	Hold the Call after 10 sec	*/
					if(__call_test_auto_mo)
					{
						sleep(10);

						call_test_auto_hold();
					}

				}
					break;


				case TAPI_EVENT_CALL_RETRIEVE_CNF:
				{
					int		index;
					TapiResult_t	api_err = TAPI_API_SUCCESS;

					TEST_DEBUG("TAPI_EVENT_CALL_RETRIEVE_CNF");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Activate Cnf Event  for call Handle [%d]",	temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.activeState = TAPI_CALL_CONNECT_ACTIVE;

					AUTO_LOG("EVENT: RETRIEVE_CNF:- Handle [%d]", temp_handle);

					if(__call_test_auto_mo)
					{
						sleep(3);

						TEST_DEBUG("### Drop the Call with handle [%d]", incoming_handle);

						api_err = tel_release_call(temp_handle, &RequestId);

						TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
						AUTO_LOG("API: tel_release_call:- Handle [%d], Return [%d]", temp_handle, api_err);
					}

				}
					break;


				case TAPI_EVENT_CALL_HOLD_CNF:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_HOLD_CNF");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Hold Cnf Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.activeState = TAPI_CALL_CONNECT_HELD;

					AUTO_LOG("EVENT: HOLD_CNF:- Handle [%d]", temp_handle);

					if(__call_test_auto_mo)
					{
						sleep(5);

						call_test_auto_act();
					}

				}
					break;



				case TAPI_EVENT_CALL_SETUPCONFERENCE_CNF:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_SETUPCONFERENCE_CNF");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Conf Setup Cnf Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.conferenceState = TAPI_CALL_CONFERENCE_ACTIVE;
					ctxt[index].info.activeState = TAPI_CALL_CONNECT_HELD;

					AUTO_LOG("EVENT: SETUPCONFERENCE_CNF:- Handle [%d]", temp_handle);
				}
					break;



				case TAPI_EVENT_CALL_SPLITCONFERENCE_CNF:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_SPLITCONFERENCE_CNF");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));


					TEST_DEBUG("Received Conf Split Cnf Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.conferenceState = TAPI_CALL_CONFERENCE_IDLE;
					AUTO_LOG("EVENT: SPLITCONFERENCE_CNF:- Handle [%d]", temp_handle);
				}
					break;



				case TAPI_EVENT_CALL_TRANSFER_CNF:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_TRANSFER_CNF");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Tansfer Cnf Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.callState = TAPI_CALL_STATE_ENDING;
					AUTO_LOG("EVENT: TRANSFER_CNF:- Handle [%d]", temp_handle);
				}
					break;


				case TAPI_EVENT_CALL_FORWARD_IND:
				{
					int	index;
					TelCallForwardType_t	type;

					TEST_DEBUG("TAPI_EVENT_CALL_FORWARD_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					memcpy(&type, event->pData+sizeof(TS_UINT), sizeof(TelCallForwardType_t ));

					TEST_DEBUG("Received Fw Ind Event  for call Handle [%d]", temp_handle);
					__PrintCallFwType("CallFwType: ", type);

					index = call_test_ret_index_by_handle(temp_handle);
					AUTO_LOG("EVENT: FORWARD_IND::- Handle [%d]", temp_handle);
				}

					break;

				case TAPI_EVENT_CALL_RETRIEVE_IND:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_RETRIEVE_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Act Ind Event for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.activeState = TAPI_CALL_CONNECT_ACTIVE;
					AUTO_LOG("EVENT: RETRIEVE_IND:- Handle [%d]", temp_handle);
				}
					break;

				case TAPI_EVENT_CALL_HOLD_IND:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_HOLD_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));


					TEST_DEBUG("Received Held Ind Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);

					ctxt[index].info.activeState = TAPI_CALL_CONNECT_HELD;
					AUTO_LOG("EVENT: HOLD_IND:- Handle [%d]", temp_handle);
				}
					break;

				case TAPI_EVENT_CALL_TRANSFER_IND:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_TRANSFER_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Tansfer Ind Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);
					AUTO_LOG("EVENT: TRANSFER_IND:- Handle [%d]", temp_handle);
				}
					break;

				case TAPI_EVENT_CALL_SETUPCONFERENCE_IND:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_SETUPCONFERENCE_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Conf Setup Ind Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);
					AUTO_LOG("EVENT: SETUPCONFERENCE_IND:- Handle [%d]", temp_handle);

				}
					break;

				case TAPI_EVENT_CALL_BARRING_IND:
				{
					int	index;

					TEST_DEBUG("TAPI_EVENT_CALL_BARRING_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					TEST_DEBUG("Received Barr Ind Event  for call Handle [%d]", temp_handle);

					index = call_test_ret_index_by_handle(temp_handle);
					AUTO_LOG("EVENT: BARRING_IND:- Handle [%d]", temp_handle);
				}
					break;



				case TAPI_EVENT_CALL_SEND_DTMF_CNF:
				{
					TEST_DEBUG("Received TAPI_EVENT_CALL_SEND_DTMF_CNF");
					AUTO_LOG("EVENT: DTMF_CNF: Cnf Count [%d]", _auto_dtmf_cnf_count++);
					if((_auto_dtmf_send_count > 0) && (_auto_dtmf_send_count < 100))
						call_test_auto_dtmf();
				}
					break;

				case TAPI_EVENT_CALL_SET_LINEID_CNF:
				{
					TEST_DEBUG("Received TAPI_EVENT_CALL_SET_LINEID_CNF");
					AUTO_LOG("EVENT: SET_LINEID_CNF:");
				}
					break;

				case TAPI_EVENT_CALL_GET_LINEID_CNF:
				{
					TEST_DEBUG("Received TAPI_EVENT_CALL_GET_LINEID_CNF");
					AUTO_LOG("EVENT: GET_LINEID_CNF:");
				}
					break;


				case TAPI_EVENT_CALL_AOCINFO_IND:
				{
					TelCallAocInfo_t	info = {0, };

					TEST_DEBUG("TAPI_EVENT_CALL_AOCINFO_IND");
					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					memcpy(&info, event->pData+sizeof(TS_UINT), sizeof(TelCallAocInfo_t));

					TEST_DEBUG("Received AOC Info Ind Event for call Handle [%d], acm [%u], max acm [%u]",
						temp_handle, info.ACM, info.MaxAcm);
					AUTO_LOG("EVENT: AOCINFO_IND:- Handle [%d]", temp_handle);
				}
					break;

				case TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND:
				{
					TelCallConnectedNumberInfo_t	info = {0, };

					TEST_DEBUG("TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND");

					memcpy(&temp_handle, event->pData, sizeof(TS_UINT));

					memcpy(&info, event->pData+sizeof(TS_UINT), sizeof(TelCallConnectedNumberInfo_t));

					TEST_DEBUG("Received SS Info Ind Event for call Handle [%d],  cli cause [%d], name mode [%d]",
						temp_handle, info.no_cli_cause, info.name_mode);
					AUTO_LOG("EVENT: CONNECTEDNUMBERINFO_IND:- Handle [%d]", temp_handle);
				}
					break;


				default:
					TEST_DEBUG("Undefined Event Type [%d] Received", event->EventType);
					AUTO_LOG("EVENT: UNDEFINED EVENT:-TYPE [%d]", event->EventType);
					break;

			}

			break;

		default:
			TEST_DEBUG("*** Other TAPI EVENT received : class=%x type=%x ****",event->EventClass, event->EventType);
			break;

	}
	TEST_DEBUG("****************** End event *****************************");

}

int call_test_subscribe_tapi_events (void)
{
	int i = 0;
	int ret_val = TRUE;
	int iNumOfCALLEvt = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;


	int CALLEvtList[] =
	{
		TAPI_EVENT_CALL_SETUP_CNF,  /**<0x0000,	This event indicates the Call Setup request triggered */
		TAPI_EVENT_CALL_ALERT_IND,			/**<	This event indicates the Call is in Alerting stage*/
		TAPI_EVENT_CALL_CONNECTING_IND,	/**<	This event indicates the Call is in Connecting stage*/
		TAPI_EVENT_CALL_CONNECTED_IND,	/**<	This event indicates the Call is Connected */
		TAPI_EVENT_CALL_INCOM_IND    ,		/**<	This event indicates Incoming Call indication to Application*/
		TAPI_EVENT_CALL_ANSWER_CNF,	/**<	Incoming call was answered*/
		TAPI_EVENT_CALL_RETRIEVE_CNF,	/**<	This event indicates Call is retrieved from held state*/
		TAPI_EVENT_CALL_RETRIEVE_IND,	/**<	This event indicates that other party has retrieved the call from hold*/
		TAPI_EVENT_CALL_HOLD_IND,		/**<	This event indicates that other party is has put the call on hold*/
		TAPI_EVENT_CALL_HOLD_CNF ,		/**<	This event indicates is call is put on hold*/
		TAPI_EVENT_CALL_TRANSFER_CNF,	/**<	This event indicates is call is explicitly transferred*/
		TAPI_EVENT_CALL_TRANSFER_IND,	/**<	Call Transfer Indication*/
		TAPI_EVENT_CALL_SETUPCONFERENCE_CNF,		/**<	This event indicates that Call is joined in a conference*/
		TAPI_EVENT_CALL_SETUPCONFERENCE_IND,		/**<	Conference setup indication*/
		TAPI_EVENT_CALL_SPLITCONFERENCE_CNF,		/**<	This event indicates Call is split from conference*/
		TAPI_EVENT_CALL_SEND_DTMF_CNF,	/**<	This event indicates the confirmation for sending DTMF Digits to the network*/
		TAPI_EVENT_CALL_WAITING_IND,			/**<	Call waiting indication*/
		TAPI_EVENT_CALL_FORWARD_IND,		/**<	This event indication for Call is forwarded */
		TAPI_EVENT_CALL_RELEASE_CNF,		/**<	This event is indicated when a specific call is released requested by the application*/
		TAPI_EVENT_CALL_RELEASE_ALL_CNF,	/**<	This event is indicated when all the calls released requested by the application*/
		TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF,	/**<	This event is indicated when all the active calls released requested by the application*/
		TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF,		/**<	This event is indicated when all the held calls released requested by the application*/
		TAPI_EVENT_CALL_END_IND,			 /**<	This event indicates the Call is Ended. This event can be solicited/unsolicited.*/
		TAPI_EVENT_CALL_GET_LINEID_CNF,	/**<	Error in Get Line ID	*/
		TAPI_EVENT_CALL_SET_LINEID_CNF, /**<		Error in Set Line Id		*/
		TAPI_EVENT_CALL_SWAP_CNF, /**< Call Swap Confirmation */
		TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF,	/**<	Get Privacy mode	*/
		TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF, /**<		Set Privacy mode	*/
		TAPI_EVENT_CALL_PRIVACYMODE_IND,	/**<	Privacy mode Indication	*/
		TAPI_EVENT_CALL_FLASHINFO_CNF, /**<		Flash info	*/
		TAPI_EVENT_CALL_EXIT_EMERGENCYMODE_CNF, /**<		Exit Emergency mode	cnf */
		TAPI_EVENT_CALL_EMERGENCYMODE_IND, /**<		 Emergency mode	ind */
		TAPI_EVENT_CALL_TIME_CNF, /**<		 call time cnf */
		TAPI_EVENT_CALL_TIME_IND, /**<		 call time ind */
		TAPI_EVENT_CALL_OTA_PROGRESS_IND, /**<		 ota progress ind */
		TAPI_EVENT_CALL_DISPLAY_INFO_IND, /**<		 display info ind */
		TAPI_EVENT_CALL_BARRING_IND,			/**<	This event indicates the Call is Barred*/
		TAPI_EVENT_CALL_FORWARDED_INFO_IND,	/**<	This event indicates the MT call is Forwarded*/
		TAPI_EVENT_CALL_AOCINFO_IND,	/**<	This event indicates the AOC Information*/
		TAPI_EVENT_CALL_CALLINGNAMEINFO_IND,	/**<	This event indicates the Calling Name Information*/
		TAPI_EVENT_CALL_CONNECTEDNUMBERINFO_IND,	/**<	This event indicates the Connected Number Information*/
		TAPI_EVENT_CALL_CLISUPRESSIONREJ_IND,	/**<	This event indicates the CLI Suppression Indication*/
		TAPI_EVENT_CALL_DEFLECTED_IND,	/**<	This event indicates the Call is Deflected*/
		TAPI_EVENT_CALL_UNCOND_FORWARDING_IND,	/**<	This event indicates the Call is unconditionally forwarded*/
		TAPI_EVENT_CALL_COND_FORWARDING_IND,	/**<	This event indicates the Call is conditionally forwarded*/
		TAPI_EVENT_CALL_CONNECTED_ECT_REMOTE_IND,	/**<	This event indicates the ECT remote connection  */
		TAPI_EVENT_CALL_ON_HOLD_RELEASED_IND,	/**<	This event indicates the Call on hold is released*/
		TAPI_EVENT_CALL_ALERT_ECT_REMOTE_IND,	/**<	This event indicates the ECT remote alert*/
		TAPI_EVENT_CALL_SS_FORWARDED_MSG_RXED_IND,	/**<	This event indicates that the forwarded msg has been received*/
	};

	iNumOfCALLEvt = sizeof(CALLEvtList)/sizeof(int);
	CALL_subscription_id = (unsigned int *) calloc (iNumOfCALLEvt, sizeof (unsigned int));



	for( i=0 ; i< iNumOfCALLEvt; i++ )
	{
		api_err = tel_register_event(CALLEvtList[i], &CALL_subscription_id[i],(TelAppCallback)&call_async_event_callback,NULL );
		if(api_err != TAPI_API_SUCCESS)
		{
			TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", CALLEvtList[i],api_err);
			ret_val = FALSE;
			break;
		}
	}

	return ret_val;



#if 0
	char *videocallEvents[] = {
			"Telephony.Call.Video.SetupCnf",
			"Telephony.Call.Video.AlertInd",
			"Telephony.Call.Video.ConnectedInd",
			"Telephony.Call.Video.IncomingInd",
			"Telephony.Call.Video.AnswerCnf",
			"Telephony.Call.Video.ForwardInd",
			"Telephony.Call.Video.ReleaseCnf",
			"Telephony.Call.Video.EndInd",
			 "Telephony.Call.Video.BarringInd",
			 "Telephony.Call.Video.ForwardedInfoInd",
			 "Telephony.Call.Video.AocInfoInd",
			"Telephony.Call.Video.ConnectedNumberInfoInd",
			"Telephony.Call.Video.UncondforwardingActiveInd",
			 "Telephony.Call.Video.CondforwardingActiveInd",
			 "Telephony.Call.Video.ConnectedEctRemoteInd",
	};

	for(count =0;count  <  (sizeof(videocallEvents)/sizeof(*videocallEvents)) ; count++)
	{
		error_code = EvtSysEventSubscribe (
				EvtDeliveryHandle,
				videocallEvents[count],				/* notification ID */
				NULL,						/*	filter expression	*/
				(EvtSysEventCb)call_test_noti_cb,		/* notification handler */
				(void *)&sdata,				/* local data */
				&subscription_id2			/* subscription ID */
				);
		if (error_code )
		{
			TEST_DEBUG( " EvtSysEventSubscribe(): error [%#X] for event [%s]", error_code , videocallEvents[count]);
		}
	}


	return 0;
#endif
}


int call_test_exit(void)
{
	int result = 0;
	return result;
}



int call_test_get_request(void)
{
	//char ch;
	int ret;
	//char callId=0;
	//unsigned short len;
	char buf[256];
	char dtmfStr[16] = "1000103001";
	//char normal[16] = "+91";
	//unsigned short error;
	TapiResult_t		api_err = TAPI_API_SUCCESS;
	int index;
	int RequestID;
	_auto_dtmf_send_count = 0;
	_auto_dtmf_cnf_count = 0;

	for(index=0; index<TAPI_CONTEXT_CALL_SIZE_MAX; index++)
	{
		if(ctxt[index].used == true)
		{
			api_err = tel_get_call_status( ctxt[index].handle, &gcallStatus[index]);

			if(api_err != TAPI_API_SUCCESS)
			{
				TEST_DEBUG("Get Call Status Failed: returnStatus value is [%x] for handle[%d] ", api_err, ctxt[index].handle);
			}
		}
	}


	printf("\n -----------------------------------------------------------------------------");
	printf("\n ############  For gsm/wcdma/cdma ############ ");
	printf("\n tel_exe_call_mo		:       mo <Number>");
	printf("\n tel_exe_call_mo (SOS)				:       sos <Number>");
	printf("\n tel_answer_call (Accept)		:       ans");
	printf("\n tel_answer_call (Reject)			:       rej");
	printf("\n tel_send_call_dtmf				:       dtmf <String>");
	printf("\n tel_release_call				:       drop <Call handle>");
	printf("\n tel_release_call_all				:       rall");
	printf("\n tel_get_call_status  (SyncAPI)		:       status <Call handle>");
	printf("\n tel_get_call_duration (SyncAPI)		:       duration <Call handle>");
	printf("\n Get call state from NPS (SyncAPI)		:       state");

	printf("\n ############  For only gsm/wcdma ############ ");
	printf("\n tel_answer_call(Hold&Accept)			:       hna");
	printf("\n tel_answer_call(Replace)			:       rep");
	printf("\n tel_hold_call				:       hld <Call handle>");
	printf("\n tel_retrieve_call				:       act <Call handle>");
	printf("\n tel_swap_call		:       swap <Active Call handle> <Held Call handle>");
	printf("\n tel_exe_call_explicit_transfer			:       tran <Active Call handle>");
	printf("\n tel_join_call				:       join <Active Call handle> <Held Call handle>");
	printf("\n tel_split_call			:       split <Call handle>");
	printf("\n tel_release_call_all_active			:       ract");
	printf("\n tel_release_call_all_held			:       rhld");
	printf("\n tel_activate_call_ccbs (not supported)		:       ccbs <Call handle>");
	printf("\n tel_deflect_call (not supported)		:       defl <Number>");
	printf("\n tel_get_call_act_line		:       lineg");
	printf("\n tel_set_call_act_line		:       lines");
	printf("\n tel_get_call_conf_list(SyncAPI)		:       conflist <Call handle>");

	printf("\n ############  For only cdma ############ ");
	printf("\n tel_exe_call_flash_info				: 	flash1  ");
	printf("\n tel_exe_call_flash_info (with Number)		:	flash2 <Number>");
	printf("\n tel_get_call_privacy_mode			:	pmodeg <vp_type(1/2/3)>");
	printf("\n tel_set_call_privacy_mode		: 	pmodes <vp_type(1/2/3)> <vp_mode(0/1)>");
	printf("\n tel_exit_call_emergency_mode			: 	xemer ");
	printf("\n tel_get_call_time			: 	time <mask(count 1, total time 2 last time 4)>");

	printf("\n\n quit						:	 quit ");
	printf("\n -----------------------------------------------------------------------------");


	printf("\n########################################################\n\n");

	for(index=0; index<TAPI_CONTEXT_CALL_SIZE_MAX; index++)
	{
		if(ctxt[index].used == true)
		{
			fprintf(stderr, " Index:[%d], handle: [%d], [%s], [%s],[%s],",
				index, ctxt[index].handle, gcallStatus[index].pNumber, (gcallStatus[index].bMoCall)?"MO":"MT", (gcallStatus[index].bConferenceState)?"Conference":"Single");
			__PrintCallType(" ", gcallStatus[index].CallType);
			__PrintCallState("\n Call State:", gcallStatus[index].CallState);
			__PrintActiveCallState(" Active Call State:", gcallStatus[index].CallActiveState);
			fprintf(stderr, "\n");
		}
	}
	printf("\n########################################################\n");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0) {
		if (errno == EINTR)
		perror("read(1)");
		return -1;
	} else if (ret == 0) return ret;



	TEST_DEBUG("Enter the API code: ");


	/******************************************************************************
	*
	*								C A L L  A P I
	*
	*******************************************************************************/

	if(strncmp(buf, "flash1", 6) == 0)
	{
		TEST_DEBUG("###flash1 .... ");

		api_err = tel_exe_call_flash_info(NULL,&RequestID);

		TEST_DEBUG(" \n tel_exe_call_flash_info Tapi return returnStatus value is [%x]  RequestID : %d \n ", api_err, RequestID);
      }


	if(strncmp(buf, "flash2", 6) == 0)
	{
	        char		tempNum[TAPI_CALL_DIALDIGIT_LEN_MAX+1];
	        char	DialNum[TAPI_CALL_DIALDIGIT_LEN_MAX+1];
	        int		length = 0;
	        strncpy(tempNum, &buf[7], TAPI_CALL_DIALDIGIT_LEN_MAX);
	        length = strlen(tempNum);
	        strncpy(DialNum,tempNum, length-1);  // removing '/n' form the end of the string
	        TEST_DEBUG("### dial number :[%s]", DialNum);

		api_err = tel_exe_call_flash_info(DialNum, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if(strncmp(buf, "pmodeg", 6) == 0)
	{
		char temp[2];
		int  vp_type= -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[7], 1);
		vp_type = atoi(temp);

		TEST_DEBUG("### tel_get_call_privacy_mode: vp_type [%d]", vp_type);

		api_err = tel_get_call_privacy_mode(vp_type,&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if(strncmp(buf, "pmodes", 6) == 0)
	{
		TEST_DEBUG("### tel_set_call_privacy_mode");

		char temp[2];
		TelCallVoicePrivacyInfo_t vp_info = { 0, };

		memset(temp, 0, 2);
		memcpy(&temp, &buf[7], 1);
		vp_info.PrivacyType = atoi(temp);

		memset(temp, 0, 2);
		memcpy(&temp, &buf[9], 1);
		vp_info.PrivacyMode = atoi(temp);

		TEST_DEBUG("### tel_set_call_privacy_mode: vp_type [%d] vp_mode [%d]", vp_info.PrivacyType, vp_info.PrivacyMode);

		api_err = tel_set_call_privacy_mode(vp_info,&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if(strncmp(buf, "xemer", 5) == 0)
	{
		TEST_DEBUG("###tel_exit_call_emergency_mode");

		api_err = tel_exit_call_emergency_mode(&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

	if(strncmp(buf, "time", 4) == 0)
	{
		TEST_DEBUG("###tel_get_call_time");
		char temp[2];
		unsigned short mask= -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[5], 1);
		mask = atoi(temp);

		TEST_DEBUG("###tel_get_call_time.. req mask type:[0x%x]",mask );

		api_err = tel_get_call_time(mask, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

      }
//######################################################################
	if(strncmp(buf, "sos", 3) == 0)
	{
//		int	free_id;
		char	tempNum[TAPI_CALL_DIALDIGIT_LEN_MAX+1];
		int		length = 0;

		strncpy(tempNum, &buf[4], TAPI_CALL_DIALDIGIT_LEN_MAX);
		length = strlen(tempNum);

		memset(&_glob_setup_info, 0, sizeof(_glob_setup_info));

		strncpy(_glob_setup_info.szNumber,tempNum, length-1);  // removing '/n' form the end of the string

	_glob_setup_info.CallType=TAPI_CALL_TYPE_E911;

		TEST_DEBUG("Request to Setup call with Number [%s], type %d",	_glob_setup_info.szNumber, _glob_setup_info.CallType);

		api_err = tel_exe_call_mo(&_glob_setup_info, &curr_handle, &RequestID);

		TEST_DEBUG(" after calling tel_exe_call_mo : Handle [%d]", curr_handle);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

#if 0 // move to call setup cnf event
		if(api_err == TAPI_API_SUCCESS)
		{
			free_id = call_test_ret_first_free_index();

			if (free_id == -1)
			{
				TEST_DEBUG("No Free Context ...");
			}

			else
			{
				memcpy(&ctxt[free_id].info.callNumber, _glob_setup_info.szNumber, sizeof( _glob_setup_info.szNumber));
				ctxt[free_id].info.callState = TAPI_CALL_STATE_ORIG;
				ctxt[free_id].info.activeState = TAPI_CALL_CONNECT_IDLE;
				ctxt[free_id].info.isMobileOriginated = true;
				ctxt[free_id].handle = curr_handle;

			}
		}
#endif


      }

#if 0
	if(strncmp(buf, "automo", 6) == 0)
	{
		char temp[16] = {0, };

		memcpy(temp, &buf[7], _country_spc_num_);


		memset(&_glob_setup_info, 0, sizeof(_glob_setup_info));

		_glob_setup_info.CallType=TAPI_CALL_TYPE_VOICE;
		strncpy(_glob_setup_info.szNumber, pre_string, sizeof(pre_string));

		strncat(_glob_setup_info.szNumber, temp, _country_spc_num_);

		_glob_setup_info.IdentityMode = TAPI_CALL_IDENTITY_SHOW;

		call_test_auto_mo();

	}

	if(strncmp(buf, "autoans", 7) == 0)
	{
		call_test_auto_ans();
	}

#endif
	if(strncmp(buf, "vmo", 2) == 0)
	{
//		int	free_id;
		char	tempNum[TAPI_CALL_DIALDIGIT_LEN_MAX+1];
		int		length = 0;

		strncpy(tempNum, &buf[4], TAPI_CALL_DIALDIGIT_LEN_MAX);
		length = strlen(tempNum);

		memset(&_glob_setup_info, 0, sizeof(_glob_setup_info));

		strncpy(_glob_setup_info.szNumber,tempNum, length-1);  // removing '/n' form the end of the string

		_glob_setup_info.CallType=TAPI_CALL_TYPE_DATA;

		TEST_DEBUG("Request to Setup call with Number [%s], type %d",	_glob_setup_info.szNumber, _glob_setup_info.CallType);

		api_err = tel_exe_call_mo(&_glob_setup_info, &curr_handle, &RequestID);

		TEST_DEBUG(" after calling tel_exe_call_mo : Handle [%d]", curr_handle);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
#if 0 // move to call setup cnf event
		if(api_err == TAPI_API_SUCCESS)
		{
			free_id = call_test_ret_first_free_index();

			if (free_id == -1)
			{
				TEST_DEBUG("No Free Context ...");
			}

			else
			{
				memcpy(&ctxt[free_id].info.callNumber, _glob_setup_info.szNumber, sizeof( _glob_setup_info.szNumber));
				ctxt[free_id].info.callState = TAPI_CALL_STATE_ORIG;
				ctxt[free_id].info.activeState = TAPI_CALL_CONNECT_IDLE;
				ctxt[free_id].info.isMobileOriginated = true;
				ctxt[free_id].handle = curr_handle;

			}
		}
#endif

      }

	if(strncmp(buf, "mo", 2) == 0)
	{
//		int	free_id;
		char	tempNum[TAPI_CALL_DIALDIGIT_LEN_MAX+1];
		int		length = 0;

		strncpy(tempNum, &buf[3], TAPI_CALL_DIALDIGIT_LEN_MAX);
		length = strlen(tempNum);

		memset(&_glob_setup_info, 0, sizeof(_glob_setup_info));

		strncpy(_glob_setup_info.szNumber,tempNum, length-1);  // removing '/n' form the end of the string

		_glob_setup_info.CallType=TAPI_CALL_TYPE_VOICE;
		//_glob_setup_info.Ecc=TAPI_CALL_ECC_DEFAULT;

		TEST_DEBUG("Request to Setup call with Number [%s], type %d",	_glob_setup_info.szNumber, _glob_setup_info.CallType);

		api_err = tel_exe_call_mo(&_glob_setup_info, &curr_handle, &RequestID);

		TEST_DEBUG(" after calling tel_exe_call_mo : Handle [%d]", curr_handle);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

#if 0 // move to call setup cnf event
		if(api_err == TAPI_API_SUCCESS)
		{
			free_id = call_test_ret_first_free_index();

			if (free_id == -1)
			{
				TEST_DEBUG("No Free Context ...");
			}

			else
			{
				memcpy(&ctxt[free_id].info.callNumber, _glob_setup_info.szNumber, sizeof( _glob_setup_info.szNumber));
				ctxt[free_id].info.callState = TAPI_CALL_STATE_ORIG;
				ctxt[free_id].info.activeState = TAPI_CALL_CONNECT_IDLE;
				ctxt[free_id].info.isMobileOriginated = true;
				ctxt[free_id].handle = curr_handle;

			}
		}
#endif

      }


	if( strncmp(buf, "ans", 3) == 0 )
	{
		TEST_DEBUG("###answer incoming call");

		api_err = tel_answer_call(incoming_handle, TAPI_CALL_ANSWER_ACCEPT, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

	if( strncmp(buf, "hna", 3) == 0 )
	{
//		TS_UINT active_handle = -1;

//		active_handle = call_test_ret_handle_by_active_state();

//		TEST_DEBUG("### hold active call [%d] and accept waiting call", active_handle);

		TEST_DEBUG("### hold active call and accept waiting call");

		api_err = tel_answer_call(incoming_handle, TAPI_CALL_ANSWER_HOLD_AND_ACCEPT, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

	if( strncmp(buf, "rep", 3) == 0 )
	{
//		TS_UINT active_handle = -1;

//		active_handle = call_test_ret_handle_by_active_state();

//		TEST_DEBUG("### release active call [%d] and accept waiting call", active_handle);

		TEST_DEBUG("### release active call and accept waiting call");

		api_err = tel_answer_call(incoming_handle, TAPI_CALL_ANSWER_REPLACE, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }


	if( strncmp(buf, "rej", 3) == 0 )
	{
		TEST_DEBUG("### reject waiting call ");

		api_err = tel_answer_call(incoming_handle, TAPI_CALL_ANSWER_REJECT, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

//+++++++++++ Release
	if( strncmp(buf, "rall", 4) == 0 )
	{
		TEST_DEBUG("### release all calls. ");

		api_err = tel_release_call_all(&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

	if( strncmp(buf, "rall.e", 6) == 0 )
	{
		TEST_DEBUG("### release all calls.with err option ");

		api_err = tel_release_call(curr_handle, &RequestID);

		TEST_DEBUG(" Tapi returned api_err value is [%x] ", api_err);
      }

	if( strncmp(buf, "ract", 4) == 0 )
	{
		TEST_DEBUG("### release all active calls. ");

		api_err = tel_release_call_all_active( &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

	if( strncmp(buf, "rhld", 4) == 0 )
	{
		TEST_DEBUG("### release all held calls. ");

		api_err = tel_release_call_all_held( &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }

	if( strncmp(buf, "drop", 4) == 0 )
	{
		char temp[2];
		TS_UINT handle= -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[5], 1);
		handle = atoi(temp);

		TEST_DEBUG("### Drop the Call with handle [%d]", handle);

		api_err = tel_release_call(handle,  &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

      }



	if( strncmp(buf, "hld", 3) == 0 )
	{
		char temp[2];
		TS_UINT handle= -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[4], 1);
		handle = atoi(temp);

		TEST_DEBUG("### Hold the call No. %d !!", handle);

		api_err = tel_hold_call(handle, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

	}


	if( strncmp(buf, "act", 3) == 0 )
	{
		char temp[2];
		TS_UINT handle= -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[4], 1);
		handle = atoi(temp);

		TEST_DEBUG("### Activate the call No. %d !!", handle);

		api_err = tel_retrieve_call(handle, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if( strncmp(buf, "swap", 4) == 0 )
	{
		char temp[2];
		TS_UINT held_handle, active_handle = -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[5], 1);
		active_handle = atoi(temp);

		memset(temp, 0, 2);
		memcpy(&temp, &buf[7], 1);
		held_handle = atoi(temp);

		TEST_DEBUG("### Swap! Active call No, [%d], Hold the call No. [%d]",
			active_handle, held_handle);

		api_err = tel_swap_call(active_handle, held_handle, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }


	if( strncmp(buf, "join", 4) == 0 )
	{
		char temp[2];
		TS_UINT held_handle, active_handle = -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[5], 1);
		active_handle = atoi(temp);

		memset(temp, 0, 2);
		memcpy(&temp, &buf[7], 1);
		held_handle = atoi(temp);

		TEST_DEBUG("Setup a Conference Call with Active & Held Handles [%d] [%d]",
			active_handle, held_handle);

		api_err = tel_join_call(active_handle,	held_handle, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

	}


	if( strncmp(buf, "split", 5) == 0 )
	{
		char temp[2];
		TS_UINT  handle = -1;

		memset(temp, 0, 2);
		memcpy(temp, &buf[6], 1);
		handle = atoi(temp);

		TEST_DEBUG("Split Call Handle [%d] from Conference call", handle);

		api_err = tel_split_call(handle, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if(strncmp(buf, "conflist", 8) == 0)
	{
		unsigned int  callList[10];
		int  noOfCalls;
		char temp[2];
		int i=0;
		TS_UINT  handle = -1;

		memset(temp, 0, 2);
		memcpy(temp, &buf[9], 1);
		handle = atoi(temp);

		TEST_DEBUG("Get Conference List for call handle [%d]", handle);

		api_err = tel_get_call_conf_list(handle, callList, &noOfCalls);

		if(api_err == TAPI_API_SUCCESS)
		{
		    TEST_DEBUG(" Get Conference List () - call handle : %d   No.of Calls %d",handle, noOfCalls );

		    for(i=0;i<noOfCalls;i++)
		    {
		        TEST_DEBUG("Voice Test: Call Handle %d  : %d\n", i+1, callList[i]);
		    }
		}
		else
		{
		    TEST_DEBUG("Voice Test: Get Conference List  Failed: returnStatus value is [%d]", api_err);
		}

	}


	if(strncmp(buf, "status", 6) == 0)
	{
		TelCallStatus_t	callStatus;
		char temp[2];
		TS_UINT  handle = -1;

		memset(temp, 0, 2);
		memcpy(temp, &buf[7], 1);
		handle = atoi(temp);

		TEST_DEBUG(" Get Call Status informaiton for call handle %d", handle);

		api_err = tel_get_call_status(handle, &callStatus);

		if(api_err == TAPI_API_SUCCESS)
		{
			fprintf(stderr, "\n\t CallNumber             : %s",callStatus.pNumber);
			fprintf(stderr, "\n\t Is Mobile Originated   : %d(1:true, 0:false)", callStatus.bMoCall);
			__PrintCallType("\n\t CallType               :", callStatus.CallType);
			__PrintActiveCallState("\n\t CallActiveState        :", callStatus.CallActiveState);
			__PrintCallState("\n\t CallState              :", callStatus.CallState);
			fprintf(stderr, "\n\t isConferenceState	: %d(1:true, 0:false)\n",callStatus.bConferenceState);
		}
		else
		{
		    TEST_DEBUG("Get Call Status Failed: returnStatus value is [%x] ", api_err);
		}

	}

	if( strncmp(buf, "duration", 8) == 0 )
	{
		char temp[2];
		unsigned int duration;
		TS_UINT  handle = -1;

		memset(temp, 0, 2);
		memcpy(temp, &buf[9], 1);
		handle = atoi(temp);


		TEST_DEBUG("### Get Duration for  the call handle. %d !!", handle);

		api_err = tel_get_call_duration(handle, &duration);
		TEST_DEBUG("Call Duration for the Call handle [%d] is: [%d] sec  \n", handle, duration);
	}


	if(strncmp(buf, "tran", 4) == 0)
	{
		char temp[2];
		TS_UINT active_handle = -1;

		memset(temp, 0, 2);
		memcpy(&temp, &buf[5], 1);
		active_handle = atoi(temp);


		TEST_DEBUG("Transfer calls with handles [%d]", active_handle);

		api_err = tel_exe_call_explicit_transfer(active_handle,  &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

      }

	if(strncmp(buf, "dtmf", 4) == 0)
	{
	        char		tempStr[MAX_BURST_DTMF_STRLEN] ={0,};
	        char	str[MAX_BURST_DTMF_STRLEN] = {0,};
	        int		length = 0;
	        strncpy(tempStr, &buf[5], MAX_BURST_DTMF_STRLEN);
	        length = strlen(tempStr);
	        strncpy(str,tempStr, length-1);  // removing '/n' form the end of the string
	        TEST_DEBUG("### Send DTMF - [%s]", str);

		api_err = tel_send_call_dtmf(str, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}
	if(strncmp(buf, "clir", 4) == 0)
	{
//		int	free_id;
		char	tempNum[TAPI_CALL_DIALDIGIT_LEN_MAX+1];
		int		length = 0;

		strncpy(tempNum, &buf[5], TAPI_CALL_DIALDIGIT_LEN_MAX);
		length = strlen(tempNum);

		memset(&_glob_setup_info, 0, sizeof(_glob_setup_info));

		strncpy(_glob_setup_info.szNumber,tempNum, length-1);  // removing '/n' form the end of the string

		_glob_setup_info.CallType=TAPI_CALL_TYPE_VOICE;

		TEST_DEBUG("Request to Setup CLIR call with Number [%s], type %d",	_glob_setup_info.szNumber, _glob_setup_info.CallType);

		api_err = tel_exe_call_mo(&_glob_setup_info, &curr_handle, &RequestID);

		TEST_DEBUG(" after calling tel_exe_call_mo : Handle [%d]", curr_handle);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

      }

	if(strncmp(buf, "adtmf", 5) == 0)
	{
		char DtmfString[10] = {0, };
		unsigned char length;

		 strcpy(DtmfString, "1234\0");
		 length = strlen(DtmfString);

		 TEST_DEBUG("Send DTMF digits [%s], length %d", DtmfString, length);

		api_err = tel_send_call_dtmf(DtmfString, &RequestID);
		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
		_auto_dtmf_send_count++;
	}

	if(strncmp(buf, "ds", 2) == 0)
	{
		strncat(dtmfStr, &buf[3], 16);
		strncpy(_glob_setup_info.szNumber, dtmfStr, TAPI_CALL_DIALDIGIT_LEN_MAX);
		TEST_DEBUG("### make a call for DTMF test....%s", _glob_setup_info.szNumber);

		api_err=tel_exe_call_mo( &_glob_setup_info, &curr_handle,&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
      }


	if(strncmp(buf, "lineg", 5) == 0)
	{
		TEST_DEBUG("### Get Current Active Line.");

		api_err = tel_get_call_act_line(&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

		TEST_DEBUG("### Current Active Line : %d", _glob_test_curr_line);
	}

	if(strncmp(buf, "lines", 5) == 0)
	{
		TEST_DEBUG("### Set Current Active Line.");

		if(_glob_test_curr_line == TAPI_CALL_ACTIVE_LINE1)
			_glob_test_curr_line = TAPI_CALL_ACTIVE_LINE2;

		else
			_glob_test_curr_line = TAPI_CALL_ACTIVE_LINE1;

		api_err = tel_set_call_act_line(_glob_test_curr_line,&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if(strncmp(buf, "ccbs", 4) == 0)
	{
		TEST_DEBUG("### Activate ccbs  call handle[%d]", curr_handle);

		api_err = tel_activate_call_ccbs(curr_handle,&RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);
	}

	if(strncmp(buf, "defl", 4) == 0)
	{
		int		len1;
		char	str[TAPI_CALL_DIALDIGIT_LEN_MAX] = {0, };
		TelCallDeflectDstInfo_t	dst;

		memset(&dst, 0, sizeof(dst));

		strncpy(str, &buf[5], TAPI_CALL_DIALDIGIT_LEN_MAX);

		len1 = strlen(str);

		strncpy((char*)dst.number, &str[0], (len1-1));

		TEST_DEBUG("Deflect Call Handle [%d] to [%s]", incoming_handle, dst.number);

		api_err = tel_deflect_call(incoming_handle,&dst, &RequestID);

		TEST_DEBUG(" Tapi return api_err value is [%x] ", api_err);

	}

	if(strncmp(buf, "quit", 4) == 0)
	{
		TEST_DEBUG("quit..... ");

		return -1;
      }

	/******************************************************************************
	*
	*					C O M M O N  A P I
	*
	*******************************************************************************/

	if(strncmp(buf, "tapi", 4) == 0)
        {
	TS_BOOL	status = FALSE;

		api_err = tel_check_service_ready(&status);

		TEST_DEBUG(" Current status of TAPI is:%d, api_err value is [%x]", status, api_err);
        }


	if(strncmp(buf, "state", 5) == 0)
        {
	unsigned char		call_state = 0;
	api_err = vconf_get_int(VCONFKEY_TELEPHONY_CALL_STATE, (int*)&call_state);
	TEST_DEBUG(" Current call state is:%s, api_err value is [%x]", call_state? "CONV" : "IDLE", api_err);
        }

	if(strncmp(buf, "menu", 4) == 0)
        {
		TEST_DEBUG("Finished Testing this module, Going back to Main Menu\n");
		//call_test_exit();
		return -1;
		//exit(0);
        }

	memset(buf, '\0', 256);

    return ret;

}


void call_test_loop(void)
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
				  if(call_test_get_request()<0)
					break;
		          }
		}
	}

}

void call_test_init(void)
{
	int count;

	TEST_DEBUG("Please press enter...\n");

	for( count = 0; count < TAPI_CONTEXT_CALL_SIZE_MAX; count++)
	{
		ctxt[count].handle = -1;
		ctxt[count].used = false;
	}

	memset(&_glob_setup_info, 0, sizeof(_glob_setup_info));

	_glob_setup_info.CallType=TAPI_CALL_TYPE_VOICE;
	strncpy(_glob_setup_info.szNumber, pre_string, sizeof(pre_string));

	call_test_loop();

}

