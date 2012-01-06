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

/**
* @ingroup TELEPHONY_API
* @addtogroup TAPI_API_SIM_SAT SIM/SAT
*/

/**
* @ingroup  TAPI_API_SIM_SAT
* @addtogroup UTS_SIM_SAT Unit
*/

/**
* @ingroup  UTS_SIM_SAT
* @addtogroup UTS_TAPI_SIM_LOCK_ENABLE_PERSONALISATION tel_enable_sim_personalization
*
* @brief Unit test code for tel_enable_sim_personalization()
* @par Test function Prototype:
*	TapiResult_t tel_enable_sim_personalization(TelSimLockPersonalisationInfo_t *PersonalisationPwd, int *pRequestId);
* @par Test function description:
*	This API is used to enable the network personalisation feature.
* @par Important Notes:
*	None
* @param [in] PersonalisationPwd
*	The information required to disable network personalisation.
*	@code
	typedef struct
	{
		TelSimLockType_t PersonalisationLock;
		UINT8	Password_length;
		UINT8	Personalisation_password[TAPI_SEC_LOCK_PERSONALISATION_PASSWD_LEN];
	}TelSimLockPersonalisationInfo_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is TAPI_EVENT_SIM_ENABLE_PERSONALIZATION_CNF
* @pre
*	None
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tapi_sim_lock_personalization.c
* @brief
	Test for the tel_disable_sim_personalization() API that is used to disable the network personalisation feature \n
	Test for the tel_enable_sim_personalization() API that is used to enable the network personalisation feature
* @author
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "sim_common.h"
#include "uts_tel_lock_sim_personalization.h"
#include "async_util.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_enable_sim_personalization01, 1},
	{utc_tel_enable_sim_personalization02, 2},
	{utc_tel_enable_sim_personalization03, 3},
	{utc_tel_enable_sim_personalization04, 4},
	{utc_tel_disable_sim_personalization01, 5},
	{utc_tel_disable_sim_personalization02, 6},
	{utc_tel_disable_sim_personalization03, 7},
	{utc_tel_disable_sim_personalization04, 8},
	{NULL, 0}
};

/*
  User Defined Handler
*/
// Global variable for user data
static BOOL gAsyncResult; //user defined data : Async callback is called or not

/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;


/*
  G_MAIN Utility ////////////////////////////////////////////////////////////////////////////
*/
// Prototype : User must add function body
gboolean g_timeout_callback (void *data);

// GMainLoop global handler
GMainLoop *sim_loop = NULL;

void G_MAIN_LOOP_CLOSE()
{
    g_main_loop_quit(sim_loop);
}

int G_MAIN_LOOP_RUN(int sec)
{
	//GMainLoop *mainloop;
	sim_loop = g_main_loop_new (NULL, FALSE);
	g_timeout_add ( (sec * 1000), g_timeout_callback, NULL);
	g_main_loop_run (sim_loop);

	return 0;
}

gboolean g_timeout_callback (void *data)
{
	gAsyncResult = FALSE;
	tet_printf("[TIME OUT] g_timeout called .Close g_main_roop()");
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}
// End of  G_MAIN Utility//////////////////////////////////////////////////////////////////////


static int  sim_app_callback  (TelTapiEvent_t *sim_event)
{
    tet_printf("\n***************[SIM APP]Callback data **********************\n");
    tet_printf("[SIM]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    BOOL bReceived = FALSE;

    switch(sim_event->EventType)
    {
#if 0
	case TAPI_EVENT_SIM_LOCKTYPE_INFO_CNF:
	{
		tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_LOCKTYPE_INFO_CNF******\n");
		tet_printf("[SIM APP]SIM SAP trasfer ATR event status  = [0x%x]\n", sim_event->Status);
		bReceived = TRUE;
		gAsyncResult = TRUE;
	}
	break;
#endif
	case TAPI_EVENT_SIM_ENABLE_PERS_CNF:
	{
		tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_DISABLE_PERS_CNF******\n");
		tet_printf("[SIM]SIM SAP trasfer ATR event status  = [0x%x]\n", sim_event->Status);
		bReceived = TRUE;
		gAsyncResult = TRUE;
	}
	break;

	case TAPI_EVENT_SIM_DISABLE_PERS_CNF:
	{
		tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_DISABLE_PERS_CNF******\n");
		tet_printf("[SIM]SIM SAP trasfer ATR event status  = [0x%x]\n", sim_event->Status);
		bReceived = TRUE;
		gAsyncResult = TRUE;
	}
	break;

        default:
        {
		bReceived = TRUE;
		gAsyncResult = FALSE;
		tet_printf("Info : TAPI_EVENT_SIM_ENABLE_PERS_CNF = %d", TAPI_EVENT_SIM_ENABLE_PERS_CNF);
		tet_printf("[SIM]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
		tet_printf("[SIM]Undhandled event state [%d]\n",sim_event->Status);
	}
	break;
    }//end switch

    if (bReceived == TRUE)
      G_MAIN_LOOP_CLOSE();

    return 0;
}


static void startup()
{
    tet_infoline("uts_tapi_sim_lock_personalization Test Start\n");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_ENABLE_PERS_CNF, &subscription_id,(TelAppCallback)sim_app_callback,NULL) )
			gStartupSuccess = TRUE;
		else
			tet_infoline("startup fail. tel_register_event() failed");
	} else {
		tet_infoline("startup fail. tel_init() failed");
	}

 tel_register_app_name("org.samsung.test");
    return;
}


static void cleanup()
{
    tet_infoline("uts_tapi_sim_lock_personalization Test Finished\n");

    if (gStartupSuccess == TRUE)
	{
		tel_deregister_event(subscription_id);
	}
	tel_deinit();

    return;
}



/*
  Unit Test Case functions
*/
/**
* @brief tel_enable_sim_personalization() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_ENABLE_SIM_PERSONALIZATION01
* @param [in] &lock_pers = NULL
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_enable_sim_personalization01()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;

	TapiResult_t err = tel_enable_sim_personalization(NULL,&pRequestId);
	tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
	if (err == TAPI_API_INVALID_PTR )
		TET_STAMP_PASS();
	else if ( err == TAPI_API_NOT_SUPPORTED )
		TET_STAMP(TET_NOTINUSE);
	else
		TET_STAMP_FAIL();

}


/**
* @brief tel_enable_sim_personalization() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_ENABLE_SIM_PERSONALIZATION02
* @param [in] &lock_pers
* @param [out]&pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_enable_sim_personalization02()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;
	TelSimPersPw_t lock_pers;// = {0,};

	lock_pers.type = TAPI_SIM_LOCK_PN;  /* only for testing */

	lock_pers.pw_len = 8 /*control key*/;

	TelSimImsiInfo_t pSimImsiInfo;
	TET_PRECONDITION(TAPI_API_SUCCESS,tel_get_sim_imsi(&pSimImsiInfo));
	memcpy(&lock_pers.pw[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.pw[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(lock_pers.pw, "12345678\0", 9); /* only for testing - random password */

	tet_printf("check");
	TapiResult_t err = tel_enable_sim_personalization(&lock_pers,NULL);
	tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
	if (err == TAPI_API_INVALID_PTR )
		TET_STAMP_PASS();
	else if ( err == TAPI_API_NOT_SUPPORTED )
		TET_STAMP(TET_NOTINUSE);
	else
		TET_STAMP_FAIL();
}

/**
* @brief tel_enable_sim_personalization() API : Positive condition(Normal operation)
* @par ID: UTC_TEL_ENABLE_SIM_PERSONALIZATION03
* @pre Successfully get IMSI information using tel_get_sim_imsi()
* @param [in] &lock_pers
*	@code
	TelSimLockPersonalisationInfo_t lock_pers;
	TelSimImsiInfo_t pSimImsiInfo;
	tel_get_sim_imsi(&pSimImsiInfo);

	lock_pers.PersonalisationLock = TAPI_SIM_LOCK_PN;  // only for testing
	lock_pers.Password_length = 1 // 1 byte length of mnc+mcc
	                          + 5  // 5 or 6 bytes mnc+mcc
	                          + 6; //control key;
	lock_pers.Personalisation_password[0] = 5; // mcc + mnc length can be 5 or 6
	memcpy(&lock_pers.Personalisation_password[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.Personalisation_password[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(&lock_pers.Personalisation_password[1 + TAPI_SIM_MCC_CODE_LEN + TAPI_SIM_MNC_CODE_LEN], "123456", 6); // only for testing - random password
*	@endcode
* @param [out]&pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is TAPI_EVENT_SIM_ENABLE_PERSONALIZATION_CNF
*/
void utc_tel_enable_sim_personalization03()
{
	TET_STAMP_START("Normal operation ");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int pRequestId=0;
	TelSimPersPw_t lock_pers;// = {0,};

	lock_pers.type = TAPI_SIM_LOCK_PN;  /* only for testing */

	lock_pers.pw_len = 8 /*control key*/;

	TelSimImsiInfo_t pSimImsiInfo;
	TET_PRECONDITION(TAPI_API_SUCCESS,tel_get_sim_imsi(&pSimImsiInfo));
	memcpy(&lock_pers.pw[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.pw[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(lock_pers.pw, "12345678\0", 9); /* only for testing - random password */

	TapiResult_t err =  tel_enable_sim_personalization(&lock_pers,&pRequestId);
	if ( err == TAPI_API_NOT_SUPPORTED )
	{
		tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
		TET_STAMP(TET_NOTINUSE);
		return;
	}
	TET_CHECK(TAPI_API_SUCCESS,err);

	gAsyncResult = FALSE;
	G_MAIN_LOOP_RUN(55);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
}

void utc_tel_enable_sim_personalization04()
{
	TET_STAMP_START("Normal operation ");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	int pRequestId=0;
	TelSimPersPw_t lock_pers;// = {0,};

	lock_pers.type = TAPI_SIM_LOCK_PN + 12;  /* only for testing */

	lock_pers.pw_len = 8 /*control key*/;

	TelSimImsiInfo_t pSimImsiInfo;
	TET_PRECONDITION(TAPI_API_SUCCESS,tel_get_sim_imsi(&pSimImsiInfo));
	memcpy(&lock_pers.pw[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.pw[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(lock_pers.pw, "12345678\0", 9); /* only for testing - random password */

	TapiResult_t err =  tel_enable_sim_personalization(&lock_pers,&pRequestId);
	if ( err == TAPI_API_NOT_SUPPORTED )
	{
		tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
		TET_STAMP(TET_NOTINUSE);
		return;
	}
	TET_CHECK(TAPI_API_SUCCESS,err);

	gAsyncResult = FALSE;
	G_MAIN_LOOP_RUN(55);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
}

/** @} */


/**
* @ingroup  UTS_SIM_SAT
* @addtogroup UTS_TAPI_SIM_LOCK_DISABLE_PERSONALISATION tel_disable_sim_personalization
*
* @brief Unit test code for tel_disable_sim_personalization()
* @par Test function Prototype:
*	TapiResult_t tel_disable_sim_personalization(TelSimLockPersonalisationInfo_t *PersonalisationPwd, int *pRequestId);
* @par Test function description:
*	This API is used to disable the network personalisation feature.
* @par Important Notes:
*	None
* @param [in] PersonalisationPwd
*	The information required to disable network personalisation.
*	@code
	typedef struct
	{
		TelSimLockType_t PersonalisationLock;
		UINT8	Password_length;
		UINT8	Personalisation_password[TAPI_SEC_LOCK_PERSONALISATION_PASSWD_LEN];
	}TelSimLockPersonalisationInfo_t;
*	@endcode
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is TAPI_EVENT_SIM_DISABLE_PERSONALIZATION_CNF
* @pre
*	None
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/


/**
* @file		uts_tapi_sim_lock_personalization.c
* @brief
	Test for the tel_disable_sim_personalization() API that is used to disable the network personalisation feature \n
	Test for the tel_enable_sim_personalization() API that is used to enable the network personalisation feature
* @author
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

/**
* @brief tel_disable_sim_personalization() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_DISABLE_SIM_PERSONALIZATION01
* @param [in] &lock_pers = NULL
* @param [out]&pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_disable_sim_personalization01()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;

	TapiResult_t err =  tel_disable_sim_personalization(NULL,&pRequestId);
	tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
	if (err == TAPI_API_INVALID_PTR )
		TET_STAMP_PASS();
	else if ( err == TAPI_API_NOT_SUPPORTED )
		TET_STAMP(TET_NOTINUSE);
	else
		TET_STAMP_FAIL();

}

/**
* @brief tel_disable_sim_personalization() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_DISABLE_SIM_PERSONALIZATION02
* @param [in] &lock_pers
* @param [out]&pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_disable_sim_personalization02()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;
	TelSimPersPw_t lock_pers;// = {0,};

	lock_pers.type = TAPI_SIM_LOCK_PN;  /* only for testing */
	lock_pers.pw_len = 8 /*control key*/;

	TelSimImsiInfo_t pSimImsiInfo;
	tel_get_sim_imsi(&pSimImsiInfo);
	memcpy(&lock_pers.pw[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.pw[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(lock_pers.pw, "12345678\0", 9); /* only for testing - random password */

	TapiResult_t err = tel_disable_sim_personalization(&lock_pers,NULL);
	tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
	if (err == TAPI_API_INVALID_PTR )
		TET_STAMP_PASS();
	else if ( err == TAPI_API_NOT_SUPPORTED )
		TET_STAMP(TET_NOTINUSE);
	else
		TET_STAMP_FAIL();
}

void utc_tel_disable_sim_personalization03()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;
	TelSimPersPw_t lock_pers;// = {0,};

	lock_pers.type = TAPI_SIM_LOCK_PN;  /* only for testing */
	lock_pers.pw_len = 8 /*control key*/;

	TelSimImsiInfo_t pSimImsiInfo;
	tel_get_sim_imsi(&pSimImsiInfo);
	memcpy(&lock_pers.pw[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.pw[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(lock_pers.pw, "12345678\0", 9); /* only for testing - random password */

	TapiResult_t err = tel_disable_sim_personalization(&lock_pers,&pRequestId);
	tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
	if (err == TAPI_API_SUCCESS )
		TET_STAMP_PASS();
	else
		TET_STAMP_FAIL();
}


void utc_tel_disable_sim_personalization04()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR ");
	int pRequestId=0;
	TelSimPersPw_t lock_pers;// = {0,};

	lock_pers.type = TAPI_SIM_LOCK_PN-12;  /* only for testing */
	lock_pers.pw_len = 8 /*control key*/;

	TelSimImsiInfo_t pSimImsiInfo;
	tel_get_sim_imsi(&pSimImsiInfo);
	memcpy(&lock_pers.pw[1], pSimImsiInfo.szMcc, TAPI_SIM_MCC_CODE_LEN);
	memcpy(&lock_pers.pw[1 + TAPI_SIM_MCC_CODE_LEN], pSimImsiInfo.szMnc, TAPI_SIM_MNC_CODE_LEN);
	memcpy(lock_pers.pw, "12345678\0", 9); /* only for testing - random password */

	TapiResult_t err = tel_disable_sim_personalization(&lock_pers,&pRequestId);
	tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
	if (err != TAPI_API_SUCCESS )
		TET_STAMP_PASS();
	else
		TET_STAMP_FAIL();
}

/** @} */
