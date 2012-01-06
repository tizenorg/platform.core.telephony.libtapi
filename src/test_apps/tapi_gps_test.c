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
#include <assert.h>
#include <glib.h>
#include <unistd.h>
#include <string.h>

#include "TapiCommon.h"
#include "ITapiGps.h"
#include "TelErr.h"
#include "TelUtility.h"
#include "TapiEvent.h"

static unsigned int GPS_subscription_id;

#define TEST_DEBUG(frmt, args...) \
{do { fprintf(stderr, "[GPS Test][%s:%04d] "frmt "\n", __func__,__LINE__, ##args); } while (FALSE); }


void gps_async_event_callback(TelTapiEvent_t* pGpsEvent)
{
	if(pGpsEvent->pData == NULL)
	{
		TEST_DEBUG("No Event Data!!");
		return;
	}

	TEST_DEBUG("*************** Callback data **********************");
	TEST_DEBUG("EventClass [%d],EventType [0x%x], Reques Id[%d], Event status [%d], psEvent->pDataLen[%d]",
		pGpsEvent->EventClass, pGpsEvent->EventType, pGpsEvent->RequestId, pGpsEvent->Status,pGpsEvent->pDataLen);

	// TAPI_EVENT_NONE:
	switch(pGpsEvent->EventType)
	{
		case TAPI_EVENT_GPS_INIT_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_INIT_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_DEINIT_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_DEINIT_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_FIX_REQUEST_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_FIX_REQUEST_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_STOP_SESSION_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_STOP_SESSION_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_PARAMETERS_SET_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_PARAMETERS_SET_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_INIT:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_INIT $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_DEINIT:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_DEINIT $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_ENABLE_SET_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_ENABLE_SET_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_SET_TIME_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_SET_TIME_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_SET_DATA_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_SET_DATA_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_POSITION_NOTI:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_POSITION_NOTI $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_EXT_POSITION_NOTI:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_EXT_POSITION_NOTI $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_PARAMETERS_GET_RESP:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_PARAMETERS_GET_RESP $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_PD_CMD_CB_NOTI:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_PD_CMD_CB_NOTI $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_PDP_CONNECTION_IND:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_PDP_CONNECTION_IND $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_DNS_LOOKUP_IND:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_DNS_LOOKUP_IND $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_VERIFICATION_IND:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_VERIFICATION_IND $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_SUPL_NOTI:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_SUPL_NOTI $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_TIME_NOTI:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_TIME_NOTI $$$$$$ ");
			}
			break;
		case TAPI_EVENT_GPS_XTRA_DATA_NOTI:
			{
				TEST_DEBUG("$$$$$$ TAPI_EVENT_GPS_XTRA_DATA_NOTI $$$$$$ ");
			}
			break;

		default:
			TEST_DEBUG("Unhandled pGpsEvent->EventType[0x%x]",pGpsEvent->EventType);
			break;
	}
	return;
}


int gps_read_key_input()
{
	int ret;
	char buf[512] = {0, };
	char msg[512];
	char  diallingNum[256];
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	memset(buf,0, sizeof(buf));
	memset(diallingNum,0,sizeof(diallingNum));
	memset(msg,0,sizeof(msg));

	fprintf(stderr,"*************** GPS TEST MENU     ***************\n");
	fprintf(stderr,"\n  1. INIT GPS\n");
	fprintf(stderr,"\n  2. DEINIT GPS\n");
	fprintf(stderr,"\n  3. SEND PARAMETERS\n");
	fprintf(stderr,"\n  4. SEND FIX REQUEST\n");
	fprintf(stderr,"\n  5. STOP SESSION\n");
	fprintf(stderr,"\n  6. GET PARAMETERS\n");
	fprintf(stderr,"\n  7. XTRA INIT\n");
	fprintf(stderr,"\n  8. XTRA DEINIT\n");
	fprintf(stderr,"\n  9. XTRA ENABLE\n");
	fprintf(stderr,"\n  a. XTRA DISABLE\n");
	fprintf(stderr,"\n  b. XTRA TIME INFO REQUEST\n");
	fprintf(stderr,"\n  c. XTRA DATA INFO REQUEST\n");
	fprintf(stderr,"\n  q. QUIT\n");
	fprintf(stderr,"*****************************************************\n");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;

	if(memcmp(buf, "1", 1) == 0)
	{
		ret_val = tel_init_gps();
		TEST_DEBUG("GPS INIT API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "2", 1) == 0)
	{
		ret_val = tel_deinit_gps();
		TEST_DEBUG("GPS DEINIT API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "3", 1) == 0)
	{
		tapi_gps_param_t  param = {0, };
		int i = 0;

		param.ParamType[i++] = TAPI_GPS_PRM_MSG_SERVER_TYPE;
		param.ServerType = TAPI_GPS_SERVER_UMTS;


		param.ParamType[i++] = TAPI_GPS_PRM_MSG_POSITION_MODE;
		param.PositionMode = TAPI_GPS_POS_MODE_3G_CP_UE_BASED;

		param.ParamType[i++] = TAPI_GPS_PRM_MSG_MT_LR;
		param.SslOnOff = TAPI_GPS_MT_LR_DISABLE;

		param.ParamType[i++] = TAPI_GPS_PRM_MSG_MO_METHOD;
		param.SslOnOff = TAPI_GPS_MO_UP;

		param.ParamType[i++] = TAPI_GPS_PRM_MSG_SSL_ON_OFF;
		param.SslOnOff = TAPI_GPS_SSL_DISABLE;


		param.NumOfRecord = i;

		ret_val = tel_set_gps_parameters(&param);
		TEST_DEBUG("SET PARAMETERS API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "4", 1) == 0)	// fix request
	{
		tapi_gps_fix_request_msg_t fix_req = {0,};
		int i = 0;

		fix_req.ParamType[i++] = TAPI_GPS_FIX_REQ_SESSION_TYPE;
		fix_req.SessionType = TAPI_GPS_TRACKING_MODE;

		fix_req.ParamType[i++] = TAPI_GPS_FIX_REQ_SESSION_OPERATION;
		fix_req.SessionOperation = TAPI_GPS_STANDALONE_ONLY;

		fix_req.ParamType[i++] = TAPI_GPS_FIX_REQ_NUM_FIXES;
		fix_req.TotalFixes = 99999999;

		fix_req.ParamType[i++] = TAPI_GPS_FIX_REQ_TIME_BTW_FIXES;
		fix_req.TimeBtwFixes = 1;

		fix_req.ParamType[i++] = TAPI_GPS_FIX_REQ_ACCURACY;
		fix_req.Accuracy = 50;

		fix_req.ParamType[i++] = TAPI_GPS_FIX_REQ_SESSION_TIMEOUT;
		fix_req.GpsSessionTimeout = 2;
		fix_req.NumOfRecord = i;

		ret_val = tel_set_gps_prefix_info(&fix_req);
		TEST_DEBUG("GPS FIX REQUEST RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "5", 1) == 0)
	{
		ret_val = tel_stop_gps_session();
		TEST_DEBUG("GPS STOP SESSION RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "6", 1) == 0)
	{
		ret_val = tel_get_gps_parameters();
		TEST_DEBUG("GPS GET PARAMS API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "7", 1) == 0)
	{
		ret_val = tel_init_gps_xtra();
		TEST_DEBUG("GPS XTRA INIT API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "8", 1) == 0)
	{
		ret_val = tel_deinit_gps_xtra();
		TEST_DEBUG("GPS XTRA DEINIT API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "9", 1) == 0)
	{
		ret_val = tel_enable_gps_xtra();
		TEST_DEBUG("GPS XTRA ENABLE API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "a", 1) == 0)
	{
		ret_val = tel_disable_gps_xtra();
		TEST_DEBUG("GPS XTRA DISABLE API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "b", 1) == 0)
	{
		tapi_gps_xtra_time_info_t time_info = {0,};

		ret_val = tel_set_gps_xtra_time(&time_info);
		TEST_DEBUG("GPS XTRA SET TIME API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "c", 1) == 0)
	{
		tapi_gps_xtra_data_info_t data_info = {0,};

		ret_val = tel_set_gps_xtra_data(&data_info);
		TEST_DEBUG("GPS XTRA SET DATA API RETURN VALUE : [%d]\n" , ret_val);
	}
	if(memcmp(buf, "q", 1) == 0)
	{
		return -1;
	}
	return 0;
}

void gps_select_loop()
{
	int ret;
	fd_set readfds;

	while(1)
	{
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if(ret)
		{
			if(FD_ISSET(0, &readfds))
			{
				if(gps_read_key_input()<0)
					break;
			}
		}
	}
}

int gps_test_subscribe_tapi_events()
{
	int ret_val = TRUE;
	TapiResult_t	api_err = TAPI_API_SUCCESS;

	int GpsEvt = TAPI_EVENT_GPS_INIT_RESP;
	if(tel_init() == TAPI_API_SUCCESS)
	{
		api_err = tel_register_event(GpsEvt, &GPS_subscription_id,(TelAppCallback)&gps_async_event_callback, NULL);
		if(api_err != TAPI_API_SUCCESS)
		{
			TEST_DEBUG("isn't subscribed. sub id is [%d], api_err is [%d]\n", GPS_subscription_id,api_err);
			ret_val = FALSE;
		}
	}
	else
	{
		TEST_DEBUG("tel_init() failed");
	}
	return ret_val;
}

void gps_thread_loop()
{
	TEST_DEBUG("start gps_thread_loop\n");
	gps_select_loop();
	fprintf (stderr, "gps exited\n");
	pthread_exit(NULL);
}

void gps_main(int argc, char *argv[])
{
	int ret=0;
	pthread_t gps_thread_id;

	ret = pthread_create(&gps_thread_id, NULL, (void*)&gps_thread_loop, NULL);

	if(ret != 0)
	{
		TEST_DEBUG("Thread for TAPI Events Failed\n");
	}

	pthread_join(gps_thread_id, NULL);
}


