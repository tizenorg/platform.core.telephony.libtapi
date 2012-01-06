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
* @addtogroup UTS_TAPI_SIM_PB_GET_ENTRY_INFO tel_get_sim_pb_meta_info
*
* @brief Unit test code for tel_get_sim_pb_meta_info()
* @par Test function Prototype:
* 	TapiResult_t tel_get_sim_pb_meta_info(TelSimPbFileType_t  StorageFileType, int* pRequestId);
* @par Test function description:
* 	This API is used to get max text length and max number length supported by SIM phone book elementary file.
* @par Important Notes:
* 	The max number length includes the storage space provided by the corresponding EXT file for a given Dialling Number file.
* @param [in] StorageFileType    Different storage types to be selected in SIM.
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
* @pre
*	- Phonebook supported SIM used for test
* @post
*	None
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_get_sim_pb_meta_info.c
* @brief	Test for the tel_get_sim_pb_meta_info() API that is used to get phonebook entry info
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/


#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_get_sim_pb_meta_info.h"
#include "sim_common.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_get_sim_pb_meta_info_01, 1},
	{utc_tel_get_sim_pb_meta_info_02, 2},
	{utc_tel_get_sim_pb_meta_info_03, 3},
	{utc_tel_get_sim_pb_meta_info_04, 4},
	{utc_tel_get_sim_pb_meta_info_05, 5},
	{utc_tel_get_sim_pb_meta_info_06, 6},
	{utc_tel_get_sim_pb_meta_info_07, 7},
	{utc_tel_get_sim_pb_meta_info_08, 8},
	{utc_tel_get_sim_pb_meta_info_09, 9},
	{utc_tel_get_sim_pb_meta_info_10, 10},
	{utc_tel_get_sim_pb_meta_info_11, 11},
	{utc_tel_get_sim_pb_meta_info_12, 12},
	{utc_tel_get_sim_pb_meta_info_13, 13},
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
    	case TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF:
    	{
                tet_printf("\n[SIM]*****************TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF***************************\n");

//                TelSimPbEntryInfo_t *entryInfo = (TelSimPbEntryInfo_t*)sim_event->pData ;
                tet_printf("SIM PB entry info event status = [%x]\n", sim_event->Status);
        //        tet_printf("[SIM APP]STORAGE TYPE [%d]\n", entryInfo->StorageFileType);
           //     tet_printf("[SIM APP]PB_INDEX_MIN %d\n", entryInfo->PbIndexMin);
             //   tet_printf("[SIM APP]PB_INDEX_MAX = %d\n", entryInfo->PbIndexMax);
               // tet_printf("[SIM APP]PB_NUM_LEN_MAX =  %d\n", entryInfo->PbNumLenMax);
              //  tet_printf("[SIM APP]PB_TEXT_LEN_MAX =  %d\n", entryInfo->PbTextLenMax);
               // tet_printf("********************************************\n");
                gAsyncResult = TRUE;
                bReceived = TRUE;
        }
        break;

        default:
        {
            bReceived = TRUE;
            gAsyncResult = FALSE;
            tet_printf("Info TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF = %d",TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF);
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
    tet_infoline("utc_tel_get_sim_pb_meta_info Test Start\n");
	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("utc_tel_get_sim_pb_meta_info Test Finished\n");
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
* @brief tel_get_sim_pb_meta_info() API : Negative condition(Low boundary test)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_01
* @param [in] StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_FDN-1;
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_get_sim_pb_meta_info_01()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT test 1");
	TelSimPbFileType_t  StorageFileType=(TelSimPbFileType_t)TAPI_SIM_PB_FDN-1;
	int pRequestId=0;

	TET_CHECK(TAPI_API_INVALID_INPUT, tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
	TET_STAMP_PASS();
}

/**
* @brief tel_get_sim_pb_meta_info() API : Negative condition(Upper boundary test)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_02
* @param [in] StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_UNKNOWNN+1;
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_get_sim_pb_meta_info_02()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT test 2");
	TelSimPbFileType_t  StorageFileType=(TelSimPbFileType_t)TAPI_SIM_PB_UNKNOWNN+1;
	int pRequestId=0;

	TET_CHECK(TAPI_API_INVALID_INPUT, tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
	TET_STAMP_PASS();
}

/**
* @brief tel_get_sim_pb_meta_info() API : Negative condition(NULL Parameter test)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_03
* @param [in] StorageFileType = TAPI_SIM_PB_GAS
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_get_sim_pb_meta_info_03()
{
	TET_STAMP_START("TAPI_API_INVALID_PTR test");
	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_GAS;

	TET_CHECK(TAPI_API_INVALID_PTR,tel_get_sim_pb_meta_info(StorageFileType,NULL));
	TET_STAMP_PASS();
}

/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook FDN record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_04
* @param [in] StorageFileType = TAPI_SIM_PB_FDN
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_04()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_FDN");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_FDN;
	int pRequestId=0;

	gAsyncResult = FALSE;

	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));

	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}

/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook LDN record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_05
* @param [in] StorageFileType = TAPI_SIM_PB_LDN
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_05()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_LDN");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_LDN;
	int pRequestId=0;

	gAsyncResult = FALSE;
	
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
	
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook MSISDN record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_06
* @param [in] StorageFileType = TAPI_SIM_PB_MSISDN
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_06()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_MSISDN");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_MSISDN;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
		
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook ADN record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_07
* @param [in] StorageFileType = TAPI_SIM_PB_ADN
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_07()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_ADN");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_ADN;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
		
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook SDN record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_08
* @param [in] StorageFileType = TAPI_SIM_PB_SDN
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_08()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_SDN");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_SDN;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
		
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook EN record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_09
* @param [in] StorageFileType = TAPI_SIM_PB_EN
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_09()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_EN");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_EN;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
	
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook 3GSIM record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_10
* @param [in] StorageFileType = TAPI_SIM_PB_3GSIM
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_10()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_3GSIM");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_3GSIM;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
		
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook AAS record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_11
* @param [in] StorageFileType = TAPI_SIM_PB_AAS
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_11()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_AAS");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_AAS;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
		
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}



/**
* @brief tel_get_sim_pb_meta_info() API : Positive condition(Get Entry info for a Phonebook GAS record)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_12
* @param [in] StorageFileType = TAPI_SIM_PB_GAS
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbEntryInfo_t. \n
* 	- TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF
*/
void utc_tel_get_sim_pb_meta_info_12()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_GAS");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_GAS;
	int pRequestId=0;

	gAsyncResult = FALSE;
		
	TET_CHECK(TAPI_API_SUCCESS,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
		
	G_MAIN_LOOP_RUN(5);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_get_sim_pb_meta_info() API : Negative condition( StorageFileType=TAPI_SIM_PB_UNKNOWNN)
* @par ID: UTC_TEL_GET_SIM_PB_META_INFO_13
* @param [in] StorageFileType = TAPI_SIM_PB_UNKNOWNN
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_get_sim_pb_meta_info_13()
{
	TET_STAMP_START("StorageFileType = TAPI_SIM_PB_UNKNOWNN");
	TelSimPbFileType_t  StorageFileType=TAPI_SIM_PB_UNKNOWNN;
	int pRequestId=0;

	TET_CHECK(TAPI_API_INVALID_INPUT,tel_get_sim_pb_meta_info(StorageFileType,&pRequestId));
	TET_STAMP_PASS();
	return;
}

/** @} */
