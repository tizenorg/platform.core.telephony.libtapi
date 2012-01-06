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
* @addtogroup UTS_TAPI_SIM_PB_DELETE_RECORD tel_delete_sim_pb_record
*
* @brief Unit test code for tel_delete_sim_pb_record()
* @par Test function Prototype:
* 	TapiResult_t tel_delete_sim_pb_record(TelSimPbFileType_t StorageFileType, unsigned short RecordIndex, int* pRequestId);
* @par Test function description:
* 	This API is used to delete a SIM phone book record.
* @par Important Notes:
* 	The index ranges from 1 through to a maximum of 254 for a Linear fixed file and 255 for a cyclic file.
* @param [in] StorageFileType    Different storage types to be selected in SIM.
* @param [in]  RecordIndex  Index of the record to be deleted.
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* (Asynchronous return status is indicated by #TelSimPinOperationResult_t.)
* 	- TAPI_EVENT_SIM_PB_DELETE_CNF
*	- TAPI_EVENT_SIM_PB_ACCESS_READ_CNF
* @pre
*	- Phonebook supported SIM used for test
*	- SIM Phonebook data backup is required(After test, some phonebook record is deleted)
* @post
*	- Restore SIM phonebook data
* @return
* Return Type : (TapiResult_t)
* - TAPI_API_SUCCESS - indicating that the operation has completed successfully.
* - Otherwise it provides an error code (Refer #TapiResult_t)
* @{
*/

/**
* @file		uts_tel_delete_sim_pb_record.c
* @brief	Test for the tel_delete_sim_pb_record() API that is used to access for delete a SIM phonebook record
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/

#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_delete_sim_pb_record.h"
#include "sim_common.h"


/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_delete_sim_pb_record_01, 1},
	{utc_tel_delete_sim_pb_record_02, 2},
	{utc_tel_delete_sim_pb_record_03, 3},
	{utc_tel_delete_sim_pb_record_04, 4},
	{utc_tel_delete_sim_pb_record_05, 5},
	{utc_tel_delete_sim_pb_record_06, 6},
	{utc_tel_delete_sim_pb_record_07, 7},
	{utc_tel_delete_sim_pb_record_08, 8},
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
    	case TAPI_EVENT_SIM_PB_ACCESS_READ_CNF:
    	{
                tet_printf("[SIM APP]*****************TAPI_EVENT_SIM_PB_ACCESS_READ_CNF***************************");
                tet_printf("[SIM APP]SIM PB access read event status = [%x]", sim_event->Status);

                TelSimPbRecordData_t *sim_acces_info = (TelSimPbRecordData_t*)sim_event->pData;
                tet_printf("[SIM APP]storaget type is [%x]",sim_acces_info->StorageFileType);
                tet_printf(" SIM phone book access error [%d]", sim_event->Status);
                tet_printf("********************************************");
                gAsyncResult = TRUE;
                bReceived = TRUE;
        }
        break;

        case TAPI_EVENT_SIM_PB_DELETE_CNF:
        {
                tet_printf("[SIM APP]*****************TAPI_EVENT_SIM_PB_DELETE_CNF***************************");
                tet_printf("SIM Delete PB event status = [%x]", sim_event->Status);

               // TelSimPbUpdateResp_t *pDeleteResp = (TelSimPbUpdateResp_t*)sim_event->pData;
               // tet_printf(" Storage File Type [%d]", pDeleteResp->PbStorageInfo.StorageFileType);
               // tet_printf(" Storage File Index [%d]",  pDeleteResp->PbIndex);
                tet_printf("*******************************************");
                gAsyncResult = TRUE;
                bReceived = TRUE;
        }
        break;

        default:
        {
            bReceived = TRUE;
            gAsyncResult = FALSE;
            tet_printf("Info : TAPI_EVENT_SIM_PB_DELETE_CNF = %d", TAPI_EVENT_SIM_PB_DELETE_CNF);
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
    tet_infoline("utc_tel_read_sim_pb_record Test Start");
	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_PB_DELETE_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL) )
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
    tet_infoline("utc_tel_read_sim_pb_record Finished");

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
* @brief tel_delete_sim_pb_record() API : Negative condition(Low boundary test)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_01
* @param [in] StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_FDN-1;
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_delete_sim_pb_record_01()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT test 1");
	TelSimPbFileType_t StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_FDN-1;
	unsigned short Index=1;
	int pRequestId=0;

	TET_CHECK(TAPI_API_INVALID_INPUT, tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId));
	TET_STAMP_PASS();
	return;
}

/**
* @brief tel_delete_sim_pb_record() API : Negative condition(Upper boundary test)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_02
* @param [in] StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_UNKNOWNN+1;
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_INVALID_INPUT
*/
void utc_tel_delete_sim_pb_record_02()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT test 2");
	TelSimPbFileType_t StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_UNKNOWNN+1;
	unsigned short Index=1;
	int pRequestId=0;

	TET_CHECK(TAPI_API_INVALID_INPUT, tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId));
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_delete_sim_pb_record() API : Negative condition(NULL parameter)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_03
* @param [in] StorageFileType = TAPI_SIM_PB_FDN
* @param [in] Index = 1
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_delete_sim_pb_record_03()
{
	TET_STAMP_START("TAPI_API_INVALID_INPUT test 3");
	TelSimPbFileType_t StorageFileType = TAPI_SIM_PB_FDN;
	unsigned short Index=1;

	TET_CHECK(TAPI_API_INVALID_PTR, tel_delete_sim_pb_record(StorageFileType,Index,NULL));   //<-------------------- Invalid value
	TET_STAMP_PASS();
	return;	
}


/**
* @brief tel_delete_sim_pb_record() API : Positive condition(Delete for a Phonebook FDN record)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_04
* @param [in] StorageFileType = TAPI_SIM_PB_FDN
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_DELETE_CNF
*/
void utc_tel_delete_sim_pb_record_04()
{
    TET_STAMP_START("StorageFileType=TAPI_SIM_PB_FDN");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);
	TapiResult_t ret;
    TelSimPbFileType_t StorageFileType=TAPI_SIM_PB_FDN;
    unsigned short Index = 1;
    int pRequestId=0;

	gAsyncResult =FALSE;

    ret = tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId);
	
	G_MAIN_LOOP_RUN(2);
	
    TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
    TET_STAMP_PASS();
	return;
}


/**
* @brief tel_delete_sim_pb_record() API : Positive condition(Delete for a Phonebook LDN record)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_05
* @param [in] StorageFileType = TAPI_SIM_PB_LDN
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_DELETE_CNF
*/
void utc_tel_delete_sim_pb_record_05()
{
    TET_STAMP_START("StorageFileType=TAPI_SIM_PB_LDN");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    TelSimPbFileType_t StorageFileType=TAPI_SIM_PB_LDN;
    unsigned short Index = 1;
    int pRequestId=0;
	TapiResult_t ret;

	gAsyncResult =FALSE;
	
	ret = tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId);
		
	G_MAIN_LOOP_RUN(2);
		
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}



/**
* @brief tel_delete_sim_pb_record() API : Positive condition(Delete for a Phonebook MSISDN record)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_06
* @param [in] StorageFileType = TAPI_SIM_PB_MSISDN
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_DELETE_CNF
*/
void utc_tel_delete_sim_pb_record_06()
{
    TET_STAMP_START("StorageFileType=TAPI_SIM_PB_MSISDN");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    TelSimPbFileType_t StorageFileType=TAPI_SIM_PB_MSISDN;
	unsigned short Index = 1;
	int pRequestId=0;
	TapiResult_t ret;
	
	gAsyncResult =FALSE;
		
	ret = tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId);
			
	G_MAIN_LOOP_RUN(2);
			
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_delete_sim_pb_record() API : Positive condition(Delete for a Phonebook ADN record)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_07
* @param [in] StorageFileType = TAPI_SIM_PB_ADN
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_DELETE_CNF
*/
void utc_tel_delete_sim_pb_record_07()
{
    TET_STAMP_START("StorageFileType=TAPI_SIM_PB_ADN");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    TelSimPbFileType_t StorageFileType=TAPI_SIM_PB_ADN;
	unsigned short Index = 1;
	int pRequestId=0;
	TapiResult_t ret;
	
	gAsyncResult =FALSE;
		
	ret = tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId);
			
	G_MAIN_LOOP_RUN(2);
			
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_delete_sim_pb_record() API : Positive condition(Delete for a Phonebook SDN record)
* @par ID: UTC_TEL_DELETE_SIM_PB_RECORD_08
* @param [in] StorageFileType = TAPI_SIM_PB_SDN
* @param [in] Index = 1
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_DELETE_CNF
*/
void utc_tel_delete_sim_pb_record_08()
{
    TET_STAMP_START("StorageFileType=TAPI_SIM_PB_SDN");
    TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);

    TelSimPbFileType_t StorageFileType=TAPI_SIM_PB_SDN;
	unsigned short Index = 1;
	int pRequestId=0;
	TapiResult_t ret;
	
	gAsyncResult =FALSE;
		
	ret = tel_delete_sim_pb_record(StorageFileType,Index,&pRequestId);
			
	G_MAIN_LOOP_RUN(2);
			
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}

/** @} */
