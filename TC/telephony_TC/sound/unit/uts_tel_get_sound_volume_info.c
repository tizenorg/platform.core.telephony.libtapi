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

#include "uts_tel_get_sound_volume_info.h"

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
	{utc_tel_get_sound_volume_info_000, 1},
	{utc_tel_get_sound_volume_info_001, 2},
	{utc_tel_get_sound_volume_info_002, 3},
	{utc_tel_get_sound_volume_info_003, 4},
	{utc_tel_get_sound_volume_info_004, 5},
	{utc_tel_get_sound_volume_info_005, 6},
	{utc_tel_get_sound_volume_info_006, 7},
	{NULL, 0}
};



/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_000
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/


void utc_tel_get_sound_volume_info_000()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_SPK_PHONE, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);

	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_get_sound_volume_info_000 pass\n");
}

/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_001
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_get_sound_volume_info_001()
{

	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_BT_VOICE, &pRequestId);

	gmain_loop_for_tapi_events(TIME_DELAY);
	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_get_sound_volume_info_001 pass\n");

}

/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_002
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_get_sound_volume_info_002()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_TYPE_ALL, &pRequestId);

	gmain_loop_for_tapi_events(TIME_DELAY);
	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_SUCCESS);
	if(returnStatus == TAPI_API_SUCCESS)
		printf("utc_tel_get_sound_volume_info_002 pass\n");
}

/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_003
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/


void utc_tel_get_sound_volume_info_003()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_VOICE - 1, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);
	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if(returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_get_sound_volume_info_003 pass\n");
}

/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_004
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_get_sound_volume_info_004()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_VOICE - 1, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);
	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if(returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_get_sound_volume_info_004 pass\n");
}

/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_005
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_get_sound_volume_info_005()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_TYPE_ALL + 1, &pRequestId);
	gmain_loop_for_tapi_events(TIME_DELAY);
	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_INPUT);
	if(returnStatus == TAPI_API_INVALID_INPUT)
		printf("utc_tel_get_sound_volume_info_005 pass\n");
}

/**
 * @brief This tests
 * @par ID:
 *  UTC_TEL_GET_SOUND_VOLUME_INFO_006
 * @param [in]  vol_control_type
 * @param [in]  *pRequestId
 * @code
 * @endcode
 * @param [out]  vol_control_type
 * @param [out]  *pRequestId
 * @return TapiResult_t
*/
void utc_tel_get_sound_volume_info_006()
{
	TapiResult_t  returnStatus = TAPI_API_SUCCESS;
	int pRequestId = -1;
	gmain_loop_for_tapi_events(TIME_DELAY);
	returnStatus = tel_get_sound_volume_info(TAPI_SOUND_VOL_VOICE, NULL);
	gmain_loop_for_tapi_events(TIME_DELAY);
	printf ("The return value is %d\n", returnStatus);
	printf ("The pRequestId value is %d\n",pRequestId);

	TET_CHECK(returnStatus == TAPI_API_INVALID_PTR);
	if(returnStatus == TAPI_API_INVALID_PTR)
	printf("utc_tel_get_sound_volume_info_006 pass\n");
}

/** @} */
