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
* @addtogroup UTS_TAPI_SIM_GET_3G_CAPABILLITY_INFO tel_get_sim_pb_3g_meta_info
*
* @brief Unit test code for tel_get_sim_pb_3g_meta_info()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_pb_3g_meta_info (int *pRequestId);
* @par Test function description:
* 	This API is used to Get the 3G phone book capability information
* @par Important Notes:
*	The index ranges from 1 through to a maximum of 254 for a Linear fixed file and 255 for a cyclic file.
*	This API is applicable only for 3G phonebook.
* @param [out] pRequestId  Unique identifier for a particular request.
*   - pRequestId value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbCapabilityInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF
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
* @file		uts_tel_get_sim_pb_3g_meta_info.c
* @brief	Test for the tel_get_sim_pb_3g_meta_info() API that is used to get phonebook capability info
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/


#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_get_sim_pb_3g_meta_info.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_pb_3g_meta_info_01, 1},
	{utc_tel_get_sim_pb_3g_meta_info_02, 2},
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
	  	case TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF:
    	{
            tet_printf("\n[GOT THE EVENT]*******TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF,******\n");
            tet_printf("[SIM]SIM PB Capability Info event status = [%x]\n", sim_event->Status);

			//TelSimPbCapabilityInfo_t *sim_capability_info = (TelSimPbCapabilityInfo_t*)sim_event->pData;
			//int i;
			// for(i = 0;i<sim_capability_info->FileTypeCount;i++)
			//{
             //   tet_printf(" [SIM APP]Filed tag type[%d]", sim_capability_info->FileTypeInfo[i].FileType3G);
               // tet_printf(" [SIM APP]max index [%d]\n", sim_capability_info->FileTypeInfo[i].PbIndexMax);
               // tet_printf(" [SIM APP]max text [%d]\n", sim_capability_info->FileTypeInfo[i].PbTextMax);
               // tet_printf(" [SIM APP]used count [%d]\n",  sim_capability_info->FileTypeInfo[i].TotalRecordCount);
			//}
            gAsyncResult = TRUE;
            bReceived = TRUE;
        }
        break;

        default:
        {
            bReceived = TRUE;
            gAsyncResult = FALSE;
            tet_printf("Info : TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF = %d",TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF);
            tet_printf("[SIM]***************** Undhandled event type [%d] *****************\n",sim_event->EventType);
            tet_printf("[SIM]Undhandled event state [%d]\n",sim_event->Status);
            break;
	}
    }//end switch

    if (bReceived == TRUE)
      G_MAIN_LOOP_CLOSE();

    return 0;
}

static void startup()
{
    tet_infoline("uts_tel_get_sim_pb_3g_meta_info Test Start\n");
	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("uts_tel_get_sim_pb_3g_meta_info Test Finished\n");

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
* @brief tel_get_sim_pb_3g_meta_info() API : Negative condition(Parameter is Null)
* @par ID: UTC_TEL_GET_SIM_PB_3G_META_INFO_01
* @param [out] &req_id = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_pb_3g_meta_info_01()
{
    TET_STAMP_START("Null parameter test");

    /* calling PB capability info api */
    TET_CHECK(TAPI_API_INVALID_PTR, tel_get_sim_pb_3g_meta_info(NULL));
    TET_STAMP_PASS();
    return;
}

/**
* @brief tel_get_sim_pb_3g_meta_info() API : Positive condition
* @par ID: UTC_TEL_GET_SIM_PB_3G_META_INFO_02
* @param [out] &req_id
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event received is below and the event data is #TelSimPbCapabilityInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF
*/
void utc_tel_get_sim_pb_3g_meta_info_02()
{
    TET_STAMP_START("utc_tel_get_sim_pb_3g_meta_info_02");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    int req_id=0;
	
    gAsyncResult = FALSE;

	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_3g_meta_info(&req_id));

    G_MAIN_LOOP_RUN(2);

	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
    TET_STAMP_PASS();
	return;
}


/** @} */
