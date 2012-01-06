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

#include "uts_tel_set_sound_loopback.h"


static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;


static TapiResult_t gStartupSuccess;

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module */

static void startup()
{
	tapi_dbus_connection_name dname;
	int bStatus;
	
	gStartupSuccess =  tel_init();
	if(gStartupSuccess != TAPI_API_SUCCESS)
	{
		printf("tel_init() Failed gStartupSuccess = %d \n",gStartupSuccess);
	}

	register_all_events();
	
	tel_register_app_name("org.samsung.test");
	if(tel_get_app_name(&dname))
		printf("Dbus Get app name error\n");
	else 
		printf("Dbus Name is %s\n",dname.name);
		
	tel_check_service_ready(&bStatus);
	printf("Telephony service  %d\n",bStatus);

}

static void cleanup()
{
	deregister_all();
	tel_deinit();
	return;

}


struct tet_testlist tet_testlist[] = {
	{utc_tel_set_sound_loopback_000, 1},
	{utc_tel_set_sound_loopback_001, 2},
	{utc_tel_set_sound_loopback_002, 3},
	{utc_tel_set_sound_loopback_003, 4},
	{utc_tel_set_sound_loopback_004, 5},
	{utc_tel_set_sound_loopback_005, 6},
        {utc_tel_set_sound_loopback_006, 7},
	{NULL, 0}
};






/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_000
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_set_sound_loopback_000()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_HANDSET;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_OFF;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);
	
	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_set_sound_loopback_000 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_000 has failed\n");
		tet_result(TET_FAIL);
	}     
}


/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_001
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_set_sound_loopback_001()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_STEREO_BLUETOOTH;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_OFF;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);
	
	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_set_sound_loopback_001 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_001 has failed\n");
		tet_result(TET_FAIL);
	}
}


/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_002
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_set_sound_loopback_002()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_HEADSET_3_5PI;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_OFF;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);

	
	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_set_sound_loopback_002 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_002 has failed\n");
		tet_result(TET_FAIL);
	}
}


/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_003
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_set_sound_loopback_003()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_HANDSET;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_ON;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);

	
	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_set_sound_loopback_003 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_003 has failed\n");
		tet_result(TET_FAIL);
	}
}


/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_004
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_set_sound_loopback_004()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_STEREO_BLUETOOTH;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_ON;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);

	
	if(returnStatus == TAPI_API_SUCCESS)
	{
		tet_printf("utc_tel_set_sound_loopback_004 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_004 has failed\n");
		tet_result(TET_FAIL);
	}
}


/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_005
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/

void utc_tel_set_sound_loopback_005()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_STEREO_BLUETOOTH;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_MAX + 1;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);
	
	if(returnStatus == TAPI_API_INVALID_PTR)
	{
		tet_printf("utc_tel_set_sound_loopback_005 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_005 has failed\n");
		tet_result(TET_FAIL);
	}
}


/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_SET_SOUND_LOOPBACK_005
 * @param [in]  vc_loopback
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vc_loopback
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_set_sound_loopback_006()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	TelAudioLoopbackMode_t Mode;
	Mode.path = TAPI_SOUND_HEADSET_3_5PI + 1;
	Mode.status =  TAPI_SOUND_AUDIO_LOOPBACK_MAX + 1;

	returnStatus = tel_set_sound_loopback(Mode, &pRequestId);

	tet_printf ("The return value is %d\n", returnStatus);
	tet_printf ("The pRequestId value is %d\n",pRequestId);

	if(returnStatus == TAPI_API_INVALID_PTR)
	{
		tet_printf("utc_tel_set_sound_loopback_006 pass\n");
		tet_result(TET_PASS);
	}
	else
	{
		tet_printf("utc_tel_set_sound_loopback_006 has failed\n");
		tet_result(TET_FAIL);
	}
}

/** @} */
