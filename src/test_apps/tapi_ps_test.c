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
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <glib.h>

#include "ITapiPS.h"
#include "TapiCommon.h"
#include "TapiEvent.h"

#define TEST_DEBUG(frmt, args...)	\
{do { fprintf(stderr, "[PS Test][%s:%04d] "frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}

#ifndef false
	#define false 0
#endif
#ifndef true
	#define true  !0
#endif
#ifndef bool
	#define bool  char
#endif

int tapi_thread_id;
//extern int EvtDeliveryHandle ;
int request_id;
int iNumOfPSEvt = 0;
int error_code;
static unsigned int * PS_subscription_id;

// same as tapievent
typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}data_t;


void data_async_event_callback(TelTapiEvent_t* psEvent)
{
	if(psEvent->pData==NULL)
	{
		TEST_DEBUG("No Event Data!!");
		return;
	}

	TEST_DEBUG("*************** Callback data **********************");
	TEST_DEBUG("EventClass [%d],EventType [0x%x], Reques Id[%d], Event status [%d], psEvent->pDataLen[%d]",
		psEvent->EventClass, psEvent->EventType, psEvent->RequestId, psEvent->Status,psEvent->pDataLen);

	switch(psEvent->EventClass)
	{
		case TAPI_EVENT_CLASS_DATA:
			switch(psEvent ->EventType)
			{
				case TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO:
					{
						tapi_ps_net_start_rsp_t  pdp_ip_resp ={0,};
						memcpy(&pdp_ip_resp, psEvent->pData, sizeof(tapi_ps_net_start_rsp_t));
						TEST_DEBUG("**************	TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO	***************");
						TEST_DEBUG("ERR:[%d]",pdp_ip_resp.err);
						TEST_DEBUG("CONTEXT ID:[%d]",pdp_ip_resp.cont_id);
						TEST_DEBUG("PDP INFO FIELD FLAG:[%d]",pdp_ip_resp.pdp_info.field_flag);
						TEST_DEBUG("PDP INFO GATEWAY:[%d.%d.%d.%d]",pdp_ip_resp.pdp_info.gateway[0],pdp_ip_resp.pdp_info.gateway[1],pdp_ip_resp.pdp_info.gateway[2],pdp_ip_resp.pdp_info.gateway[3]);
						TEST_DEBUG("PDP INFO IP ADDRESS:[%d.%d.%d.%d]",pdp_ip_resp.pdp_info.ip_address[0],pdp_ip_resp.pdp_info.ip_address[1],pdp_ip_resp.pdp_info.ip_address[2],pdp_ip_resp.pdp_info.ip_address[3]);
						TEST_DEBUG("PDP INFO PRIMARY DNS:[%d.%d.%d.%d]",pdp_ip_resp.pdp_info.primary_dns[0],pdp_ip_resp.pdp_info.primary_dns[1],pdp_ip_resp.pdp_info.primary_dns[2],pdp_ip_resp.pdp_info.primary_dns[3]);
						TEST_DEBUG("PDP INFO SECONDARY DNS:[%d.%d.%d.%d]",pdp_ip_resp.pdp_info.secondary_dns[0],pdp_ip_resp.pdp_info.secondary_dns[1],pdp_ip_resp.pdp_info.secondary_dns[2],pdp_ip_resp.pdp_info.secondary_dns[3]);
						TEST_DEBUG("PDP INFO SUBNET MASK:[%d.%d.%d.%d]",pdp_ip_resp.pdp_info.subnet_mask[0],pdp_ip_resp.pdp_info.subnet_mask[1],pdp_ip_resp.pdp_info.subnet_mask[2],pdp_ip_resp.pdp_info.subnet_mask[3]);
						TEST_DEBUG("PDP INFO DEVICE NAME:****[%s]****",pdp_ip_resp.devname);
						TEST_DEBUG("*******************************************************");
					}
					break;

				case TAPI_EVENT_PS_PDP_ACT_RSP:
					{
						tapi_ps_pdp_info_t  pdp_ip_resp ={0,};
						memcpy(&pdp_ip_resp, psEvent->pData, sizeof(tapi_ps_pdp_info_t));
						TEST_DEBUG("**************	TAPI_EVENT_PS_PDP_ACT_RSP	***************");
						TEST_DEBUG("CONTEXT ID:[%d]",pdp_ip_resp.cont_id);
						TEST_DEBUG("ERR:[0x%x](0:normal)",pdp_ip_resp.err);
						TEST_DEBUG("*******************************************************");
					}
					break;

				case TAPI_EVENT_PS_PDP_DEACT_RSP:
					{
						tapi_ps_pdp_info_t		stop_resp ={0,};
						TEST_DEBUG("**************	TAPI_EVENT_PS_PDP_DEACT_RSP	***************");
						memcpy(&stop_resp, psEvent->pData, sizeof(tapi_ps_pdp_info_t));
						TEST_DEBUG("CONTEXT ID:[%d]",stop_resp.cont_id);
						TEST_DEBUG("ERR:[0x%x](0:normal)",stop_resp.err);
						TEST_DEBUG("*******************************************************");
					}
					break;

				case TAPI_EVENT_PS_PDP_DEACT_IND:
					{
						TEST_DEBUG("**************	TAPI_EVENT_PS_PDP_DEACT_IND	***************");
					}
					break;

				case TAPI_EVENT_PS_CALL_STATISTICS:
					{
						tapi_ps_call_statistics_t  call_stats ={0,};
						memcpy(&call_stats, psEvent->pData, sizeof(tapi_ps_call_statistics_t));
						TEST_DEBUG("**************	TAPI_EVENT_PS_CALL_STATISTICS	***************");
						TEST_DEBUG("CALL STATS FOR CONTEXT ID:[%d]",call_stats.cid);
						TEST_DEBUG("CALL STATS RX BYTES:[%lu]",call_stats.rx);
						TEST_DEBUG("CALL STATS TX BYTES:[%lu]",call_stats.tx);
						TEST_DEBUG("*******************************************************");
					}
					break;

				case TAPI_EVENT_PS_ERROR_IND:
					{
						tapi_gprs_error_info_t error_rsp ={0,};
						memcpy(&error_rsp, psEvent->pData, sizeof(tapi_gprs_error_info_t));
						TEST_DEBUG("**************	TAPI_EVENT_PS_ERROR_IND	***************");
						TEST_DEBUG("ERROR CAUSE:[%d]",error_rsp.cause);
						TEST_DEBUG("ERROR TYPE:[%d]",error_rsp.type);
						TEST_DEBUG("*******************************************************");
					}
					break;

				case TAPI_EVENT_PS_HSDPA_STATUS_NOTI:
					{
						tapi_hspda_status_type hsdpa_status =0x00;
						memcpy(&hsdpa_status, psEvent->pData, sizeof(tapi_hspda_status_type));
						TEST_DEBUG("**************	TAPI_EVENT_PS_HSDPA_STATUS_NOTI	***************");
						TEST_DEBUG("HSDPA STATUS - (0 -INACTIVE)  (1 -ACTIVE)  (2 -SUSPEND)");
						TEST_DEBUG("CURRENT HSDPA STATUS:[%d]",hsdpa_status);
						TEST_DEBUG("*******************************************************");
					}
					break;

				case TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI:
					{
						tapi_ps_btdun_pincontrol pincontrol ={0,};
						memcpy(&pincontrol, psEvent->pData, sizeof(tapi_ps_btdun_pincontrol));
						TEST_DEBUG("**************	TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI	***************");
						TEST_DEBUG("SIGNAL :[%d]",pincontrol.signal);
						TEST_DEBUG("STATUS:[%d]",pincontrol.status);
						TEST_DEBUG("*******************************************************");
					}
                    break;

				default:
					TEST_DEBUG("***  Default Event  psEvent ->EventType[0x%x] ****", psEvent ->EventType);
					break;

			}

			break;


		default:
			TEST_DEBUG("*** Other TAPI EVENT received : psEvent->EventClass : [0x%x] ****", psEvent->EventClass);
			break;

	}

}

int data_test_subscribe_tapi_events()
{
	int i = 0;
	int ret_val = TRUE;
	TapiResult_t	api_err = TAPI_API_SUCCESS;

	int PSEvtList[] =
	{
		TAPI_EVENT_PS_PDP_ACT_RSP,
		TAPI_EVENT_PS_PDP_DEACT_RSP,
		TAPI_EVENT_PS_PDP_DEACT_IND,
		TAPI_EVENT_PS_STATUS,
		TAPI_EVENT_PS_CALL_STATISTICS,
		TAPI_EVENT_PS_HSDPA_STATUS_NOTI,
		TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI,
		TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO,
		TAPI_EVENT_PS_ERROR_IND,
		TAPI_EVENT_PS_EXTERNAL_CALL_IND,
	};

	iNumOfPSEvt = sizeof(PSEvtList)/sizeof(int);
	TEST_DEBUG("Number of PS Event List to be registered: %d", iNumOfPSEvt);
	PS_subscription_id = (unsigned int *)calloc(iNumOfPSEvt, sizeof(unsigned int));

		if(tel_init() == TAPI_API_SUCCESS)
		{
			for( i=0 ; i< iNumOfPSEvt; i++ )
			{
				api_err = tel_register_event(PSEvtList[i], &PS_subscription_id[i],(TelAppCallback)&data_async_event_callback,NULL);

				if(api_err != TAPI_API_SUCCESS)
				{
					TEST_DEBUG("isn't subscribed. sub id is [%d], api_err is [%d]\n", PS_subscription_id[i],api_err);
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

}


int data_test_unsubscribe_tapi_events(void)
{
	int i = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;
	int ret_val = TRUE;

	for(i=0;i<iNumOfPSEvt; i++){
		api_err = tel_deregister_event(PS_subscription_id[i]);
		if(api_err != TAPI_API_SUCCESS){
			TEST_DEBUG("tel_deregister_event isn't unsubscribed. sub id is [%d] api_err is [%d]\n",PS_subscription_id[i],api_err);
			ret_val = FALSE;
			break;
		}
	}
	tel_deinit();
	return ret_val;
}


void HandleGeneralResponseError(int error)
{
	if(error == 0x8000 || error == 0x8100)
	{
		TEST_DEBUG("!!Success!!");
	}
	else
	{
		TEST_DEBUG("!!Fail!! error value : 0x%x", error);
	}
}


/*
int get_handle()
{
    _handle = TapiRegisterClient(TAPI_EVENT_CLASS_DATA, app_callback, TRUE);

    if( _handle == NULL )
    {

	TEST_DEBUG("handle get error");
	return -1;
    }

    if(!ProtoMgrRegisterLPC())
    {
	TEST_DEBUG("LPC  get error - Call");
	return -1;
    }

}



// ravi 10 20 #add server die handler
int read_phoneserver_pkt(void)
{
  int ret;

  ret = lxt_callback(_handle);

  if (ret == -1 )
  {
	TEST_DEBUG("Tapi Client: Phone Server Died, Exiting Gracefully");
	exit(0);
  }

    return 0;
}


int call_quit(void)
{
	int result = 0;

	if (!noti_finish(&error_code))
	{
		TEST_DEBUG( "Error: noti_finish() %d", error_code);
	}
	else
	{
		TEST_DEBUG("### noti was closed");
	}

	if (!TapiUnregisterClient(_handle))
	{
		TEST_DEBUG( "Error: TapiUnregisterClient failed");
	}
	else
	{
		TEST_DEBUG("### TapiUnregisterClient success");
	}

	return result;

}

*/


int data_read_key_input(void)
{
	int ret;
	char buf[256];
//	char cid;
	int requestId;
	TapiResult_t ret_val = TAPI_API_SUCCESS;

    ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			 perror("read(1)");
		return -1;
       }
	else if (ret == 0)
		return ret;


	TEST_DEBUG("Enter string in square braces: %s", buf);

	TEST_DEBUG("###################	PS TEST MENU	###################");
	TEST_DEBUG("gprs start		:	[start]");
	TEST_DEBUG("pdp deactivate	:	[deact]");
	TEST_DEBUG("BT PIN CTRL		:	[pinon]");
	TEST_DEBUG("BT PIN CTRL		:	[pinoff]");
	TEST_DEBUG("quit		:	[quit]");

	TEST_DEBUG("######################################################\n");


	 if(strncmp(buf, "start", 5) == 0)
	 {
		tapi_ps_net_start_req_t start_req;
		int  ctxt_id;
		memset(&start_req, 0, sizeof(tapi_ps_net_start_req_t));

		TEST_DEBUG("ENTER Context id....");
		scanf("%d", &ctxt_id);
		start_req.cont_id = ctxt_id;
		start_req.pdp_info.pdp_type = GPRS_PDP_TYPE_IP;

		TEST_DEBUG("ENTER APN choice  1. [nate.sktelecom.com],   2. [annam.dyn], 3. [web.vodafone.de], 4. [airtelwap.es]");
		scanf("%d", &ctxt_id);
		if(ctxt_id == 1)
			strcpy(start_req.pdp_info.apn , "nate.sktelecom.com");
		if(ctxt_id == 2)
		{
			//strcpy(start_req.pdp_info.apn , "annam.dyn");
			strcpy(start_req.pdp_info.apn , "annah.dyn");
			//strcpy(start_req.pdp_info.dns1 , "10.8.15.249");
			//strcpy(start_req.pdp_info.dns2 , "10.8.15.250");
			//strcpy(start_req.pdp_info.password, "mms");
			//strcpy(start_req.pdp_info.username, "mms");
		}
		if(ctxt_id == 3)
		{
			//strcpy(start_req.pdp_info.apn , "annam.dyn");
			strcpy(start_req.pdp_info.apn , "web.vodafone.de");
			//strcpy(start_req.pdp_info.dns1 , "10.8.15.249");
			//strcpy(start_req.pdp_info.dns2 , "10.8.15.250");
			//strcpy(start_req.pdp_info.password, "mms");
			//strcpy(start_req.pdp_info.username, "mms");
		}
		if(ctxt_id == 4)
		{
			strcpy(start_req.pdp_info.apn, "airtelwap.es");
		}

		start_req.pdp_info.pdp_type= GPRS_PDP_TYPE_IP;

		ret_val= tel_activate_gprs_pdp(&start_req,&requestId);
		TEST_DEBUG(" API RETURN VAL:[%d]",ret_val);
	}


	else if(strncmp(buf, "deact", 5) == 0)
	{
		tapi_ps_net_stop_req_t stop_req;
		memset(&stop_req, 0, sizeof(tapi_ps_net_stop_req_t));

		TEST_DEBUG("ENTER Context id....");
		int  ctxt_id;
		scanf("%d", &ctxt_id);
		stop_req.cont_id = ctxt_id;

		ret_val=tel_deactivate_gprs_pdp(&stop_req,&requestId);
		TEST_DEBUG(" API RETURN VAL:[%d]",ret_val);
	}
	else if(strncmp(buf, "pinon", 5) == 0)
	{
		tapi_ps_btdun_pincontrol pincontrol;

		memset(&pincontrol, 0, sizeof(tapi_ps_btdun_pincontrol));

		pincontrol.signal=GPRS_DATA_SIGNAL_DTR;
		pincontrol.status=GPRS_SIGNAL_STATUS_ON;

		ret_val=  tel_control_gprs_btdun_pin(pincontrol,&requestId);
		TEST_DEBUG(" API RETURN VAL:[%d]",ret_val);
	}
	else if(strncmp(buf, "pinoff", 6) == 0)
	{
		tapi_ps_btdun_pincontrol pincontrol;

		memset(&pincontrol, 0, sizeof(tapi_ps_btdun_pincontrol));

		pincontrol.signal=GPRS_DATA_SIGNAL_DTR;
		pincontrol.status=GPRS_SIGNAL_STATUS_OFF;

		ret_val=  tel_control_gprs_btdun_pin(pincontrol,&requestId);
		TEST_DEBUG(" API RETURN VAL:[%d]",ret_val);
	}

	/*	Go back to main Menu.	*/
	else if(strncmp(buf, "quit", 4) == 0)
	{
		TEST_DEBUG("Finished Testing this module, Going back to Main Menu");
		return -1;
	}

	memset(buf, '\0', 256);

	return 0;

}


void data_select_loop(void)
{
	int ret;
	fd_set readfds;


    while (1)
		{
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);


	 ret = select(0 + 1, &readfds, NULL, NULL, NULL);
        if (ret) {

	    if (FD_ISSET(0, &readfds)) {
		  if(data_read_key_input()<0)
			break;
            }
        }
    }
}



void* data_test_thread_loop(void* data)
{
	TEST_DEBUG( " data_test_thread_loop : called...");

	data_select_loop();

	TEST_DEBUG(" data_test_thread_loop : stdin Loop exited");

	pthread_exit(NULL);
}


int data_main(int argc, char *argv[])
{

//	int ret = 0;
//	int count;
	pthread_t  p_thread;

//	ret = get_handle();


	tapi_thread_id = pthread_create(&p_thread, NULL, data_test_thread_loop, NULL);

	if(tapi_thread_id != 0)
	{
		TEST_DEBUG( "Failed while creating a thread to read from stdin");
		return -1;
	}
	else
	{
		TEST_DEBUG( "a thread was created to read from stdin ..");
	}

	data_test_subscribe_tapi_events();

	pthread_join(p_thread, NULL);

	TEST_DEBUG("Exiting PS test program bye bye !");

	return 0;

}

