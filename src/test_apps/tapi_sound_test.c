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
#include <string.h>
#include <unistd.h>

#include "assert.h"
#include "glib.h"

#include "ITapiSound.h"
#include "TelUtility.h"
#include "TapiEvent.h"
#include "TapiCommon.h"

#define TEST_DEBUG(frmt, args...)	\
	{do { fprintf(stderr, "[SND Test][%s:%04d] " frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}


#ifndef false
	#define false 0
#endif
#ifndef true
	#define true  !0
#endif
#ifndef bool
	#define bool  char
#endif

#define MAX_CALL_INDEX				6

int data_status_;
int data_service_;
int callId;

int tapi_thread_id;


// this contains the tapi noti error info
int error_code;


// same as tapievent
typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}data_t;

static unsigned int *	SOUND_subscription_id;

extern int  EvtDeliveryHandle;

//static int  sound_app_callback (int group, int action, void* data, int length)
//static int  sound_async_event_callback  (TelTapiEventClass_t	eventClass, int	eventType, int  param1,void * param2)
static int  sound_async_event_callback (TelTapiEvent_t	*event)
{
	TEST_DEBUG("EVENT TYPE:[0x%x], STATUS:[0x%x]", event->EventType, event->Status);

	switch (event->EventClass)
	{
		case TAPI_EVENT_CLASS_SOUND:
			switch (event->EventType)
			{
				case TAPI_EVENT_SOUND_VOLUMECTRL_RSP: {
					TEST_DEBUG("***  TAPI_EVENT_SOUND_MICGAINCTRL_RSP received  ****");

					tapi_sound_volumn_ctrl_res vlm_ctrl_info;
					int index;

					memcpy(&vlm_ctrl_info, event->pData, sizeof(tapi_sound_volumn_ctrl_res));

					TEST_DEBUG("num record:[%d]",vlm_ctrl_info.num_record);

					for(index=0;index<vlm_ctrl_info.num_record;index++)
					{
						TEST_DEBUG("index:[%d], vol type:[0x%x]",index, vlm_ctrl_info.pinfo[index].type);
						TEST_DEBUG("index:[%d], vol level:[0x%x]",index, vlm_ctrl_info.pinfo[index].level);
					}
				}
					break;

				case TAPI_EVENT_SOUND_MICMUTECTRL_RSP:
					TEST_DEBUG("***  TAPI_EVENT_SOUND_MICMUTECTRL_RSP received  ****");
					tapi_sound_mic_mute_t *mute_status = NULL;
					mute_status = (tapi_sound_mic_mute_t *)(event->pData);
					if(*mute_status == TAPI_SOUND_MIC_UNMUTE){
						TEST_DEBUG("MIC Unmute");
					}
					else if(*mute_status == TAPI_SOUND_MIC_MUTE){
						TEST_DEBUG("MIC Mute");
					}
					else{
						TEST_DEBUG("Unknown state");
					}
					break;

				case TAPI_EVENT_SOUND_ERROR_IND:
					TEST_DEBUG("***  TAPI_EVENT_SOUND_ERROR_IND received  ****");
					break;

				default:
					TEST_DEBUG("***  Default Event  %x ****", event->EventType);
					break;

			}

			break;

		default:
			TEST_DEBUG("*** Other TAPI EVENT received : class : %x ****", event->EventClass);
			break;

	}

	return TRUE;
}

int sound_test_subscribe_tapi_events (void)
{
	int i = 0;
	int ret_val = TRUE;
	int iNumOfSOUNDEvt = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;


	int SOUNDEvtList[] =
	{
		TAPI_EVENT_SOUND_VOLUMECTRL_RSP,	      /**<     */
		TAPI_EVENT_SOUND_ERROR_IND,      /**<     */
		TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI,      /**<     */
		TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI,      /**<     */
		TAPI_EVENT_SOUND_MICMUTECTRL_RSP,      /**<     */
	};

	iNumOfSOUNDEvt = sizeof(SOUNDEvtList)/sizeof(int);
	//TAPI_CALLOC(SOUND_subscription_id, iNumOfSOUNDEvt, unsigned int)
	//#define TAPI_CALLOC(ptr, no_elements, type)
	SOUND_subscription_id = (unsigned int *) calloc (iNumOfSOUNDEvt , sizeof (unsigned int));
	if(SOUND_subscription_id == NULL)\
	{
		TEST_DEBUG("calloc error -NULL, errno is [%d]", errno);
	}

	for( i=0 ; i< iNumOfSOUNDEvt; i++ )
	{
		api_err = tel_register_event(SOUNDEvtList[i], &SOUND_subscription_id[i],(TelAppCallback)&sound_async_event_callback,NULL);
		if(api_err != TAPI_API_SUCCESS)
		{
			TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", SOUNDEvtList[i],api_err);
			ret_val = FALSE;
			break;
		}
	}

	return ret_val;
}


int sound_read_key_input(void)
{
	int ret;
	char buf[256];
	int requestId;
	TapiResult_t ret_val = TAPI_API_SUCCESS;


	tapi_sound_volume_type_t vol_type;
	tapi_sound_volume_control_t vol_control;
	tapi_sound_mic_mute_t micmute_set;
	tapi_sound_audio_path_t  vc_path;
	tapi_sound_voice_recording_status_t voice_record ;


	memset(&vol_control, 0, sizeof(tapi_sound_volume_type_t));
	memset(&vol_control, 0, sizeof(tapi_sound_volume_control_t));
	memset(&micmute_set, 0, sizeof(tapi_sound_mic_mute_t));
	memset(&vc_path, 0, sizeof(tapi_sound_audio_path_t));


	ret = read(0, buf, sizeof(buf));

	if (ret < 0)
	{
		if (errno == EINTR)
			 perror("read(1)");
		return -1;
	 }
	else if (ret == 0)
		return ret;

	TEST_DEBUG("*************** SOUND TEST MENU     ***************");
	TEST_DEBUG("VOLUME CTRL GET:[vlmget]");
	TEST_DEBUG("VOLUME CTRL SET:[vlmset]");
	TEST_DEBUG("MIC_MUTE_SET:[mmset] ");
	TEST_DEBUG("MIC_MUTE_GET:[mmget] ");
	TEST_DEBUG("VOICE_PATH_SET:[vpathset] ");
	TEST_DEBUG("AUDIO_LOOPBACK:[loopback] ");
	TEST_DEBUG("quit Sound test:[quit] ");
	TEST_DEBUG("Voice Call Recording on test:[vcre] ");
	TEST_DEBUG("Voice Call Recording off test:[vcrd] ");
	TEST_DEBUG("*************************************************");

	if(strncmp(buf, "vlmget", 6) == 0)
	{
		vol_type =TAPI_SOUND_VOL_VOICE;
		ret_val= tel_get_sound_volume_info(vol_type,&requestId);
		TEST_DEBUG("tapi_sound_volume_control_get return value is %d ",ret_val);
	}
	else if(strncmp(buf, "vlmset", 6) == 0)
	{
		vol_control.volume=TAPI_SOUND_VOLUME_LEVEL_7;
		vol_control.vol_type=TAPI_SOUND_VOL_VOICE;

		ret_val= tel_set_sound_volume_info(vol_control,&requestId);
		TEST_DEBUG("tapi_sound_volume_control_set return value is %d ",ret_val);
	 }
	 else if(strncmp(buf, "mmset", 5) == 0)
	{
		micmute_set=TAPI_SOUND_MIC_UNMUTE;
		TEST_DEBUG("micmute set value = TAPI_SOUND_MIC_UNMUTE");
		ret_val=tel_set_sound_mute_status(micmute_set,&requestId);
		TEST_DEBUG("tapi_sound_micmute_control_set return value is %d ",ret_val);
	}
	 else if(strncmp(buf, "mmget", 5) == 0)
	{
		micmute_set=TAPI_SOUND_MIC_UNMUTE;
		TEST_DEBUG("micmute set value = TAPI_SOUND_MIC_UNMUTE");
		ret_val=tel_get_sound_mute_status(&requestId);
		TEST_DEBUG("tapi_sound_micmute_control_get return value is %d ",ret_val);
	}
	else if(strncmp(buf, "vcre", 4) == 0)
	{
		voice_record=TAPI_SOUND_VOICE_RECORDING_ON;
		TEST_DEBUG("voice_record = TAPI_SOUND_VOICE_RECORDING_ON");

		ret_val=tel_control_sound_voice_recording(TAPI_SOUND_VOICE_RECORDING_ON,&requestId);

		TEST_DEBUG("tel_control_sound_voice_recording on return value is %d ",ret_val);
	}
	else if(strncmp(buf, "vcrd", 4) == 0)
	{
		voice_record=TAPI_SOUND_VOICE_RECORDING_OFF;
		TEST_DEBUG("voice_record = TAPI_SOUND_VOICE_RECORDING_OFF");
		ret_val=tel_control_sound_voice_recording(TAPI_SOUND_VOICE_RECORDING_OFF,&requestId);

		TEST_DEBUG("tel_control_sound_voice_recording off return value is %d ",ret_val);
	}
	else if(strncmp(buf, "vpathset", 8) == 0)
	{
		int iSelect = 0;
		TEST_DEBUG("1. TAPI_SOUND_HANDSET");
		TEST_DEBUG("2. TAPI_SOUND_HEADSET");
		TEST_DEBUG("3. TAPI_SOUND_HANDSFREE");
		TEST_DEBUG("4. TAPI_SOUND_BLUETOOTH");
		TEST_DEBUG("5. TAPI_SOUND_STEREO_BLUETOOTH");
		TEST_DEBUG("6. TAPI_SOUND_SPK_PHONE");
		TEST_DEBUG("7. TAPI_SOUND_HEADSET_3_5PI");

		TEST_DEBUG("Select: ");
		scanf("%d", &iSelect);

		switch(iSelect)
		{
			case 1:
				vc_path = TAPI_SOUND_HANDSET;
				TEST_DEBUG("TAPI_SOUND_HANDSET");
				break;
			case 2:
				vc_path = TAPI_SOUND_HEADSET;
				TEST_DEBUG("TAPI_SOUND_HEADSET");
				break;
			case 3:
				vc_path = TAPI_SOUND_HANDSFREE;
				TEST_DEBUG("TAPI_SOUND_HANDSFREE");
				break;
			case 4:
				vc_path = TAPI_SOUND_BLUETOOTH;
				TEST_DEBUG("TAPI_SOUND_BLUETOOTH");
				break;
			case 5:
				vc_path = TAPI_SOUND_STEREO_BLUETOOTH;
				TEST_DEBUG("TAPI_SOUND_STEREO_BLUETOOTH");
				break;
			case 6:
				vc_path = TAPI_SOUND_SPK_PHONE;
				TEST_DEBUG("TAPI_SOUND_SPK_PHONE");
				break;
			case 7:
				vc_path = TAPI_SOUND_HEADSET_3_5PI;
				TEST_DEBUG("TAPI_SOUND_HEADSET_3_5PI");
				break;
			default:
				TEST_DEBUG("Wrong Selection!!\n");
				return 0;
				break;
		}

		ret_val=tel_set_sound_path(vc_path,&requestId);
		TEST_DEBUG("vc_path:[%d],  return value [%d] ",vc_path, ret_val);
	}

	else if(strncmp(buf, "loopback", 8) == 0)
	{
		TelAudioLoopbackMode_t lpMode;
		int iSelect = 0;

		TEST_DEBUG("Select Audio Loopback Path");
		TEST_DEBUG("1. TAPI_SOUND_HANDSET");
		TEST_DEBUG("2. TAPI_SOUND_HEADSET");
		TEST_DEBUG("3. TAPI_SOUND_HANDSFREE");
		TEST_DEBUG("4. TAPI_SOUND_BLUETOOTH");
		TEST_DEBUG("5. TAPI_SOUND_STEREO_BLUETOOTH");
		TEST_DEBUG("6. TAPI_SOUND_SPK_PHONE");
		TEST_DEBUG("7. TAPI_SOUND_HEADSET_3_5PI");

		TEST_DEBUG("Select: ");
		scanf("%d", &iSelect);

		switch(iSelect)
		{
			case 1:
				lpMode.path = TAPI_SOUND_HANDSET;
				TEST_DEBUG("TAPI_SOUND_HANDSET");
				break;
			case 2:
				lpMode.path = TAPI_SOUND_HEADSET;
				TEST_DEBUG("TAPI_SOUND_HEADSET");
				break;
			case 3:
				lpMode.path = TAPI_SOUND_HANDSFREE;
				TEST_DEBUG("TAPI_SOUND_HANDSFREE");
				break;
			case 4:
				lpMode.path = TAPI_SOUND_BLUETOOTH;
				TEST_DEBUG("TAPI_SOUND_BLUETOOTH");
				break;
			case 5:
				lpMode.path = TAPI_SOUND_STEREO_BLUETOOTH;
				TEST_DEBUG("TAPI_SOUND_STEREO_BLUETOOTH");
				break;
			case 6:
				lpMode.path = TAPI_SOUND_SPK_PHONE;
				TEST_DEBUG("TAPI_SOUND_SPK_PHONE");
				break;
			case 7:
				lpMode.path = TAPI_SOUND_HEADSET_3_5PI;
				TEST_DEBUG("TAPI_SOUND_HEADSET_3_5PI");
				break;
			default:
				TEST_DEBUG("Wrong Selection!!\n");
				return 0;
				break;
		}

		TEST_DEBUG("Enable[1] Disable[0]");
		scanf("%d", &iSelect);

		if(iSelect == 1){
			lpMode.status = TAPI_SOUND_AUDIO_LOOPBACK_PCM_ON;
		}
		else if(iSelect == 0){
			lpMode.status = TAPI_SOUND_AUDIO_LOOPBACK_OFF;
		}
		else{
			TEST_DEBUG("Unknown Type loopback status!\n");
			return 0;
		}

		ret_val=tel_set_sound_loopback( &lpMode, &requestId);
		TEST_DEBUG("audio loopback return value [%d] \n", ret_val);
	}
	else if(strncmp(buf, "quit", 4) == 0)
	{
		TEST_DEBUG("sound test done ");
		return -1;
	}
	memset(buf, '\0', 256);

	return 0;

}


void sound_select_loop(void)
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
		  if(sound_read_key_input()<0)
			break;
            }
        }
    }
}



void* sound_test_thread_loop(void* data)
{
	TEST_DEBUG(" sound_test_thread_loop : called...");

	sound_select_loop();

	TEST_DEBUG(" sound_test_thread_loop : stdin Loop exited");

	pthread_exit(NULL);
}

#if 0
int sound_main(int argc, char *argv[])
{

	pthread_t  p_thread;

//	ret = get_handle();


	tapi_thread_id = pthread_create(&p_thread, NULL, sound_test_thread_loop, NULL);

	if(tapi_thread_id != 0)
	{
		TEST_DEBUG("Failed while creating a thread to read from stdin");
		return -1;
	}
	else
	{
		TEST_DEBUG("a thread was created to read from stdin ..");
	}

	sound_test_subscribe_tapi_events();

	pthread_join(p_thread, NULL);

	TEST_DEBUG("Exiting PS test program bye bye !");

	return 0;

}
#endif

