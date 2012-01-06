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

#include "ITapiProductivity.h"
#include "TapiCommon.h"
#include "TapiEvent.h"

#define TEST_DEBUG(frmt, args...) \
{do { fprintf(stderr, "[SVC Test][%s:%04d] "frmt "\n", __func__,__LINE__, ##args); } while (FALSE); }


#define SVC_TEST_KEY_END				0x51
#define SVC_TEST_KEY_SOFT_RIGHT		0x5C
#define SVC_TEST_KEY_VALUE_0			0x30
#define SVC_TEST_KEY_VALUE_1			0x31
#define SVC_TEST_KEY_VALUE_2			0x32
#define SVC_TEST_KEY_VALUE_3			0x33
#define SVC_TEST_KEY_VALUE_4			0x34
#define SVC_TEST_KEY_VALUE_5			0x35
#define SVC_TEST_KEY_VALUE_6			0x36
#define SVC_TEST_KEY_VALUE_7			0x37
#define SVC_TEST_KEY_VALUE_8			0x38
#define SVC_TEST_KEY_VALUE_9			0x39

extern int EvtDeliveryHandle ;

typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}data_t;

static unsigned int *	SVC_subscription_id;

int error_code;

//static int  svc_async_event_callback  (tapi_svcmode_event_t event ,void * data)
static int  svc_async_event_callback(TelTapiEvent_t	*event)
{

	switch(event->EventType)
	{
		case TAPI_EVENT_SVCMODE_START_NOTI:
		{
			tapi_service_mode_t  svc_mode;
			memcpy(&svc_mode , event->pData , sizeof(tapi_service_mode_t));
			TEST_DEBUG("$$$$$$ TAPI_SVCMODE_EVENT_SVCMODE_START $$$$$$ ");
			TEST_DEBUG("svc_mode:[%d]", svc_mode);
		}
			break;

		case TAPI_EVENT_SVCMODE_END_NOTI:
		{
			tapi_service_mode_end_t reset_mode;
			memcpy(&reset_mode , event->pData , sizeof(tapi_service_mode_end_t));
			TEST_DEBUG("$$$$$$ TAPI_SVCMODE_EVENT_SVCMODE_END $$$$$$ ");
			TEST_DEBUG("svc_mode:[%d], reset:[%d]", reset_mode.service_mode, reset_mode.reset);
		}
			break;

		case TAPI_EVENT_SVCMODE_CHANGE_NOTI:
		{
			tapi_service_mode_t  svc_mode;
			memcpy(&svc_mode , event->pData , sizeof(tapi_service_mode_t));
			TEST_DEBUG("$$$$$$ TAPI_SVCMODE_EVENT_SVCMODE_CHANGE $$$$$$ ");
			TEST_DEBUG("svc_mode:[%d]", svc_mode);
		}
			break;

		case TAPI_EVENT_SVCMODE_SCREEN_CFG_NOTI:
		{
			tapi_screen_config_t screen_config;
			memcpy(&screen_config , event->pData , sizeof(tapi_screen_config_t));
			TEST_DEBUG("$$$$$$ TAPI_SVCMODE_EVENT_SVCMODE_SCREEN_CFG $$$$$$ ");
			TEST_DEBUG("line num:[%d], keypad:[%d]", screen_config.line_number, screen_config.keypad);
		}
			break;

		case TAPI_EVENT_SVCMODE_DISP_SCREEN_NOTI:
		{
			tapi_service_display_info_t display_screen;
			int num_of_lines, line_count;

			memcpy(&display_screen , event->pData , sizeof(tapi_service_display_info_t));

			num_of_lines = display_screen.num_of_lines;
			TEST_DEBUG("$$$$$$ TAPI_SVCMODE_EVENT_SVCMODE_DISP_SCREEN $$$$$$ ");
			TEST_DEBUG("num_of_lines :[%d]", display_screen.num_of_lines);

			line_count =0;

			TEST_DEBUG("===================================================");
			while(num_of_lines > 0)
			{
				/* LINE COUNT - LINE NUMBER - LINE REVRES - DISPLAY STRING */
				TEST_DEBUG("#[%d-%d-%d]:[%s]", line_count, display_screen.display[line_count].line, display_screen.display[line_count].reverse, display_screen.display[line_count].string);
				num_of_lines--;
				line_count++;
			}
			TEST_DEBUG("===================================================");

		}
			break;

		case	TAPI_EVENT_SVCMODE_DEVICE_TEST_NOTI:
		{
			tapi_device_info_t  device_info;
			memcpy(&device_info , event->pData , sizeof(tapi_device_info_t));
			TEST_DEBUG("$$$$$$ TAPI_SVCMODE_EVENT_SVCMODE_DEVICE_TEST $$$$$$ ");
			TEST_DEBUG("dev id:[%d], dev status:[%d]",device_info.device_id, device_info.device_status);
		}
			break;

		default:
		   break;
	}

    return TRUE;
}

int svc_read_key_input(void)
{
	int ret;
	char buf[512];

	static tapi_service_mode_t  svc_mode = -1; // staic. (for svc mode end)
	tapi_test_mode_sub_t test_sub = -1;


	memset(buf,0, sizeof(buf));

	printf("\n########################################################\n");
	printf("1. tel_start_svcmode (main)\n");
	printf("2a. tel_send_svcmode_keycode (END)\n");
	printf("2b. tel_send_svcmode_keycode (SOFT RIGHT)\n");
	printf("2c. tel_send_svcmode_keycode (1~9)\n");
	printf("3. tel_end_svcmode \n");
	printf("4. tel_start_svcmode (monitor)\n");
	printf("quit. QUIT\n");
	printf("For escape : [2a] -> [3]\n");
	printf("########################################################\n\n");

	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
		perror("read(1)");
		return -1;
	}
	else if (ret == 0) return ret;


	if( memcmp(buf,"1",sizeof("1")-1) == 0 )
	{
		TEST_DEBUG("START SERVICE MODE (MAIN)");

		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_ENTER;

		tel_start_svcmode(svc_mode, test_sub);
	}

	if( memcmp(buf,"2a",sizeof("2a")-1) == 0 )
	{
		TEST_DEBUG("PROCESS KEY CODE (END)");

		tel_send_svcmode_keycode(SVC_TEST_KEY_END);
	}

	if( memcmp(buf,"2b",sizeof("2b")-1) == 0 )
	{
		TEST_DEBUG("PROCESS KEY CODE (SOFT RIGHT)");

		tel_send_svcmode_keycode(SVC_TEST_KEY_SOFT_RIGHT);
	}

	if( memcmp(buf,"2c",sizeof("2c")-1) == 0 )
	{
		int iKey = 0;
		TEST_DEBUG("INPUT KEY NUMBER:");
		scanf("%d", &iKey);
		tel_send_svcmode_keycode(iKey+SVC_TEST_KEY_VALUE_0);
		getchar();
	}

	if( memcmp(buf,"3",sizeof("3")-1) == 0 )
	{
		TEST_DEBUG("END SERVICE MODE");

		tel_end_svcmode(svc_mode);
	}

	if( memcmp(buf,"4",sizeof("4")-1) == 0 )
	{
		TEST_DEBUG("START SERVICE MODE : MONITOR");

		svc_mode	=	TAPI_SVC_MODE_MONITOR;
		test_sub	=	TAPI_TESTMODE_ENTER;

		tel_start_svcmode(svc_mode, test_sub);
	}


	if( memcmp(buf,"a",sizeof("a")-1) == 0 )
	{
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_SW_VERSION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_SW_VERSION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	}

	 if( memcmp(buf,"b",sizeof("b")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_FTA_SW_VERSION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_FTA_SW_VERSION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"c",sizeof("c")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_FTA_HW_VERSION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_FTA_HW_VERSION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"d",sizeof("d")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_ALL_VERSION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_ALL_VERSION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
#if 1
	 if( memcmp(buf,"e",sizeof("e")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_BATTERY_INFO_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_BATTERY_INFO_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"f",sizeof("f")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub		=	TAPI_TESTMODE_CIPHERING_PROTECTION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_CIPHERING_PROTECTION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"g",sizeof("g")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_INTEGRITY_PROTECTION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_INTEGRITY_PROTECTION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"h",sizeof("h")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_IMEI_READ_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_IMEI_READ_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"i",sizeof("i")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_BLUETOOTH_TEST_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_BLUETOOTH_TEST_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"j",sizeof("j")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_VIBRATOR_TEST_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_VIBRATOR_TEST_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"k",sizeof("k")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub		=	TAPI_TESTMODE_MELODY_TEST_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_MELODY_TEST_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"l",sizeof("l")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_MP3_TEST_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_MP3_TEST_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"m",sizeof("m")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_FACTORY_RESET_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_FACTORY_RESET_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"n",sizeof("n")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_FACTORY_PRECONFIG_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_FACTORY_PRECONFIG_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"o",sizeof("o")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_TFS4_EXPLORE_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_TFS4_EXPLORE_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"p",sizeof("p")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_RTC_TIME_DISPLAY_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_RTC_TIME_DISPLAY_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"q",sizeof("q")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_RSC_FILE_VERSION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_RSC_FILE_VERSION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	}

	 if( memcmp(buf,"r",sizeof("r")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_USB_DRIVER_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_USB_DRIVER_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"s",sizeof("s")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_USB_UART_DIAG_CONTROL_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_USB_UART_DIAG_CONTROL_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }

	 if( memcmp(buf,"t",sizeof("t")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_RRC_VERSION_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_RRC_VERSION_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"u",sizeof("u")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_GPSONE_SS_TEST_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_GPSONE_SS_TEST_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"v",sizeof("v")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_BAND_SEL_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_BAND_SEL_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"w",sizeof("w")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_GCF_TESTMODE_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_GCF_TESTMODE_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"x",sizeof("x")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_GSM_FACTORY_AUDIO_LB_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_GSM_FACTORY_AUDIO_LB_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"y",sizeof("y")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_FACTORY_VF_TEST_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_FACTORY_VF_TEST_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
	 if( memcmp(buf,"z",sizeof("z")-1) == 0 )
	 {
		svc_mode	=	TAPI_SVC_MODE_TEST_MANUAL;
		test_sub	=	TAPI_TESTMODE_TOTAL_CALL_TIME_INFO_ENTER;

		TEST_DEBUG("START SERVICE MODE: TAPI_TESTMODE_TOTAL_CALL_TIME_INFO_ENTER");

		tel_start_svcmode(svc_mode, test_sub);
	 }
#endif
	if(strncmp(buf, "quit", 4) == 0)
        {
		TEST_DEBUG("Finished Testing this module, Going back to Main Menu\n");
		return -1;
        }

    return 0;
}


void svc_select_loop(void)
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
			      if(svc_read_key_input()<0)
					break;
		        }
	        }
	 }
}

int svc_test_subscribe_tapi_events(void)
{
	int i = 0;
	int ret_val = TRUE;
	int iNumOfSVCEvt = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;


	int SVCEvtList[] =
	{
        TAPI_EVENT_SVCMODE_START_NOTI,
        TAPI_EVENT_SVCMODE_END_NOTI,
        TAPI_EVENT_SVCMODE_CHANGE_NOTI,
        TAPI_EVENT_SVCMODE_SCREEN_CFG_NOTI,
        TAPI_EVENT_SVCMODE_DISP_SCREEN_NOTI,
        TAPI_EVENT_SVCMODE_DEVICE_TEST_NOTI,

	}; // TODO: event updated

	iNumOfSVCEvt = sizeof(SVCEvtList)/sizeof(int);
	SVC_subscription_id = (unsigned int *)calloc(iNumOfSVCEvt, sizeof(unsigned int));


	for( i=0 ; i< iNumOfSVCEvt; i++ )
	{
		api_err = tel_register_event(SVCEvtList[i], &SVC_subscription_id[i],(TelAppCallback)&svc_async_event_callback,NULL);
		if(api_err != TAPI_API_SUCCESS)
		{
			TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", SVCEvtList[i],api_err);
			ret_val = FALSE;
			break;
		}
	}

	return ret_val;
}
