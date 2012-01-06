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
* @addtogroup UTS_TAPI_SIM_SAP_AUTH tel_req_sim_isim_authentication
*
* @brief Unit test code for tel_req_sim_isim_authentication()
* @par Test function Prototype:
* 	TapiResult_t tel_req_sim_isim_authentication(TelSimIsimAuthenticationData_t* AuthReqData, int *pRequestId);
* @par Test function description:
* 	This function is a used to do the authentication proceedure of ISIM for IMS application
* @par Important Notes:
* 	None
* @param [in] AuthReqData
* ISIM Authentication data.(auth_req_data - authentication data to be verified.)
* @code
// GBA(Generic Bootstrapping architecture) Boot strapping parameters.
// This is used for IMS Authentication proceedure.
typedef struct
{
	UINT8 RandomAccessLength;	// ISIM random access length
	UINT8 AuthDataLength; // ISIM Boot straping transaction ID len
	UINT8 RandomAccessData[TAPI_SIM_ISIM_AUTH_MAX_REQ_DATA_LEN];	// ISIM random data
	UINT8 AuthData[TAPI_SIM_ISIM_AUTH_MAX_REQ_DATA_LEN];	// ISIM autherisation data
}TelSimIsimAuthenticationData_t;
* @endcode
* @param [out] pRequestId
*   - Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimIsimAuthenticationResponse_t. \n
* 	- TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF
* @pre
*	SAP supported sim used to testing
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_req_sim_isim_authentication.c
* @brief	Test for the tel_req_sim_isim_authentication() API that is used to isim authentication request
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/


#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_req_sim_isim_authentication.h"
#include "sim_common.h"



/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_req_sim_isim_authentication_01, 1},
	{utc_tel_req_sim_isim_authentication_02, 2},
	{NULL, 0}
};


/*
  G_MAIN Utility ////////////////////////////////////////////////////////////////////////////
*/
// Prototype : User must add function body
gboolean g_timeout_callback (void *data);

// GMainLoop global handler
GMainLoop *nw_context_loop = NULL;

void G_MAIN_LOOP_CLOSE()
{
    g_main_loop_quit(nw_context_loop);
}

int G_MAIN_LOOP_RUN(int sec)
{
	//GMainLoop *mainloop;
	nw_context_loop = g_main_loop_new (NULL, FALSE);
	g_timeout_add ( (sec * 1000), g_timeout_callback, NULL);
	g_main_loop_run (nw_context_loop);

	return 0;
}
// End of  G_MAIN Utility//////////////////////////////////////////////////////////////////////


/*
  User Defined Handler
*/
// Global variable for user data
static BOOL gAsyncResult; //user defined data : Async callback is called or not

// G_MAIN Utility : time out callback function
gboolean g_timeout_callback (void *data)
{
	gAsyncResult = FALSE;
	tet_printf("[SIM APP][TIME OUT] g_timeout called .Close g_main_roop()");
	G_MAIN_LOOP_CLOSE();
	return FALSE;
}

static int  sim_app_callback  (TelTapiEvent_t *sim_event)
{
    tet_printf("\n\n\n***************[SIM APP]Callback data **********************\n");
    tet_printf("[SIM APP]Reques Id[%d]\n", sim_event->RequestId);
    tet_printf("[SIM APP]event status [%d]\n", sim_event->Status);

    gAsyncResult = FALSE;
    BOOL bReceived = FALSE;

    switch(sim_event->EventType)
      {

    	case TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF:
    	{
            tet_printf("\n[SIM APP]*****************TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF***************************\n");
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

        default:
        {
            bReceived = TRUE;
            gAsyncResult = FALSE;
            tet_printf(" TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF = %d", TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF);
            tet_printf("[SIM APP]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
            tet_printf("[SIM APP]Undhandled event state [%d]\n",sim_event->Status);
            break;
		}
	}//end switch

    if (bReceived == TRUE)
      G_MAIN_LOOP_CLOSE();

    return 0;
}


/* Startup & Clean function : It is not test code, only for
   initialize and finalize test module
*/
static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;

static void startup()
{
    tet_infoline("uts_tel_req_sim_isim_authentication Test Start");
	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("uts_tel_req_sim_isim_authentication Finished");
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
* @brief tel_req_sim_isim_authentication() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_REQ_SIM_ISIM_AUTHENTICATION_01
* @param [in] pAuthenticationData
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sim_isim_authentication_01()
{
    TET_STAMP_START("NULL Test 1");
    TelSimIsimAuthenticationData_t pAuthenticationData;

    tet_printf(" Enter  RAND data \n");
    strcpy((char*)pAuthenticationData.RandomAccessData, "username@operator.com");
    pAuthenticationData.RandomAccessLength= strlen("username@operator.com");

    tet_printf(" Enter Authentication  data \n");
    strcpy((char*)pAuthenticationData.AuthData,  "+1-212-555-12345");
    pAuthenticationData.AuthDataLength = strlen("+1-212-555-12345");

    /* Calling TAPI API */
    TapiResult_t err = tel_req_sim_isim_authentication(&pAuthenticationData,NULL);
    tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
    if (err == TAPI_API_INVALID_PTR )
        TET_STAMP_PASS();
    else if ( err == TAPI_API_NOT_SUPPORTED )
        TET_STAMP(TET_NOTINUSE);
    else
        TET_STAMP_FAIL();
}

/**
* @brief tel_req_sim_isim_authentication() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_REQ_SIM_ISIM_AUTHENTICATION_02
* @param [in] pAuthenticationData = NULL
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_req_sim_isim_authentication_02()
{
    TET_STAMP_START("NULL Test 2");
    int pRequestId=0;

    /* Calling TAPI API */
    TapiResult_t err = tel_req_sim_isim_authentication(NULL,&pRequestId);
    tet_printf("%s[%d] %s(): error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) );
    if (err == TAPI_API_INVALID_PTR )
        TET_STAMP_PASS();
    else if ( err == TAPI_API_NOT_SUPPORTED )
        TET_STAMP(TET_NOTINUSE);
    else
        TET_STAMP_FAIL();
}

/** @} */
