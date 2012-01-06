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
* @addtogroup UTS_TAPI_SIM_PB_UPDATE_RECORD tel_update_sim_pb_record
*
* @brief Unit test code for tel_update_sim_pb_record()
* @par Test function Prototype:
* 	TapiResult_t tel_update_sim_pb_record(const TelSimPbRecordData_t* pPbRecordData, int* pRequestId);
* @par Test function description:
* 	This API is used to add or edit SIM phone book record entry information.
* @par Important Notes:
*	GSM 7 Bit or Unicode conversion required to make record data
*	- GSM 7 Bit conversion
* @code
BOOL oem_encode_gsm_7_bit_chars (char text_len,  const char* text_data,char *out_len_max, char* gsm7bit_text )
{
	int i;
	int pos;
	 UINT16 shift = 0;

	for( pos = 0, i = 0; i < text_len; pos++, i++ )
	{
		// pack the low bits - for single char
		gsm7bit_text[pos] = text_data[i] >> shift;

		if ( i + 1 < text_len )
		{
			// pack the high bits using the low bits of the next character
			gsm7bit_text[pos] |= text_data[i+1] << ( 7 - shift );
			shift++;
			if( shift == 7 )
			{
				shift = 0;
				i++;
			}
		}
	}
	*out_len_max  = (text_len* 7 )/8;
	return pos;
}
* @endcode
*	- Unicode conversion
* @code
BOOL
oem_encode_text_unicode(char text_len, char *text_data,char *unicode_text_len,char *unicode_text)
{
	int j,i;
	i = j = 0;

	if(text_len == 0)
	{
		tet_printf(LEVEL_ERR,"Unicode encoding failed - input text string length is 0!");
		return FALSE;
	}
	// UNicode conversion
	// a  = Ascii is 61 and in Unicode it is 0061 but modem expects 6100
	for(j= 0,i = 0;i<text_len*2;i++)
	{
		unicode_text[i++] = text_data[j++];
		unicode_text[i] = 0x00;
	}
	*unicode_text_len = text_len * 2;
	return TRUE;
}
@endcode
* @param [in] pPbRecordData  Record data
* @param [out] pRequestId   Unique identifier for a particular request.
*   - request_id value can be any value from 0 to 255 if the API is returned successfully
*   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_UPDATE_CNF
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
* @file		uts_tapi_pb_rec_modify_access.c
* @brief	Test for the tel_update_sim_pb_record() API that is used to modify phonebook record
* @author	
* @version	Initial Creation V0.1
* @date		2008.07.30
*/


#include <tet_api.h>
#include <ITapiSim.h>
#include <TapiCommon.h>

#include "tet_tapi_util.h"
#include "uts_tel_update_sim_pb_record.h"
#include "sim_common.h"

/* Initialize TCM data structures */
static void startup();
static void cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] = {
	{utc_tel_update_sim_pb_record_01, 1},
	{utc_tel_update_sim_pb_record_02, 2},
	{utc_tel_update_sim_pb_record_03, 3},
	{utc_tel_update_sim_pb_record_04, 4},
	{utc_tel_update_sim_pb_record_05, 5},
	{NULL, 0}
};

/*
  User Defined Handler
*/
// Global variable for user data
static BOOL gAsyncResult; //user defined data : Async callback is called or not

static BOOL gStartupSuccess = FALSE;
static unsigned int subscription_id = 0;



/*
  G_MAIN Utility ////////////////////////////////////////////////////////////////////////////
*/

// Prototype : User must add function body
gboolean g_timeout_callback(void *data);

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
        case TAPI_EVENT_SIM_PB_UPDATE_CNF:
        {
			tet_printf("[SIM]*****************TAPI_EVENT_SIM_PB_UPDATE_CNF***************************");
			tet_printf("SIM Delete PB event status = [%x]", sim_event->Status);

			//TelSimPbUpdateResp_t *pUpdateResp = (TelSimPbUpdateResp_t *)sim_event->pData;
			//  tet_printf(" index [%d]\n",pUpdateResp->PbIndex);
			// tet_printf(" storage type [%d]\n",pUpdateResp->PbStorageInfo.StorageFileType);
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
    tet_infoline("utc_tel_update_sim_pb_record Test Start");

	if ( tel_init() == TAPI_API_SUCCESS)
	{
		if (TAPI_API_SUCCESS == tel_register_event(TAPI_EVENT_SIM_PB_UPDATE_CNF,&subscription_id,(TelAppCallback)sim_app_callback,NULL))
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
    tet_infoline("utc_tel_update_sim_pb_record Finished");

    if (gStartupSuccess == TRUE)
	{
		tel_deregister_event(subscription_id);
	}
	tel_deinit();

    return;
}


/* Utility function
*/
BOOL oem_encode_gsm_7_bit_chars (char text_len,  const char* text_data,char *out_len_max, char* gsm7bit_text )
{
	int i;
	int pos;
	 UINT16 shift = 0;

	for( pos = 0, i = 0; i < text_len; pos++, i++ )
	{
		/* pack the low bits - for single char */
		gsm7bit_text[pos] = text_data[i] >> shift;

		if ( i + 1 < text_len )
		{
			/* pack the high bits using the low bits of the next character */
			gsm7bit_text[pos] |= text_data[i+1] << ( 7 - shift );

			shift++;

			if( shift == 7 )
			{
				shift = 0;
				i++;
			}
		}
	}
	*out_len_max  = (text_len* 7 )/8;

	/* done */
	return pos;

}

BOOL oem_encode_text_unicode(char text_len, char *text_data,char *unicode_text_len,char *unicode_text)
{
	int j,i;
	i = j = 0;

	if(text_len == 0)
	{
		tet_printf("Unicode encoding failed - input text string length is 0!");
		return FALSE;
	}
	/*UNicode conversion*/
	/* a  = Ascii is 61 and in Unicode it is 0061 but modem expects 6100 */
	for(j= 0,i = 0;i<text_len*2;i++)
	{
		unicode_text[i++] = text_data[j++];
		unicode_text[i] = 0x00;
	}
	*unicode_text_len = text_len * 2;
	return TRUE;

}



/*
  Unit Test Case functions
*/

/**
* @brief tel_update_sim_pb_record() API : Negative condition(NULL parameter test)
* @par ID: UTC_TEL_UPDATE_SIM_PB_RECORD_01
* @param [in] pb_add = NULL
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_update_sim_pb_record_01()
{
	int  pRequestId=0;

	TET_STAMP_START("TAPI_API_INVALID_PTR");
	/* calling PB capability info api */
	TET_CHECK(TAPI_API_INVALID_PTR, tel_update_sim_pb_record(NULL,&pRequestId));
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_update_sim_pb_record() API : Negative condition(NULL parameter test)
* @par ID: UTC_TEL_UPDATE_SIM_PB_RECORD_02
* @param [in] pb_add
* @param [out] &pRequestId = NULL
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_update_sim_pb_record_02()
{
	TelSimPbRecordData_t  pb_add;
	pb_add.StorageFileType=TAPI_SIM_PB_GAS;
	pb_add.Index=1;
	char unicode_text[256];
	int unicode_text_len = 0;

	TET_STAMP_START("TAPI_API_INVALID_PTR");

	unsigned char name[]="user";
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData,name,20);

	/* GSM 7bit conversion */
	oem_encode_gsm_7_bit_chars(strlen((char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData) ,
				(char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData, (char *)&unicode_text_len, unicode_text);
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData, unicode_text, unicode_text_len );
	pb_add.ContactInfo.PbAdditionalNumStringData.TextLength =  unicode_text_len;
	pb_add.ContactInfo.PbAdditionalNumStringData.EncryptionType = TAPI_SIM_TEXT_ENC_GSM7BIT; // always 2 for GAS and AAS

	TET_CHECK(TAPI_API_INVALID_PTR, tel_update_sim_pb_record(&pb_add, NULL)); //<----Invalid PTR
	TET_STAMP_PASS();
	return;
}



/**
* @brief tel_update_sim_pb_record() API : Negative condition(Low boundry test)
* @par ID: UTC_TEL_UPDATE_SIM_PB_RECORD_03
* @param [in] pb_add.StorageFileType = (TelSimPbFileType_t)-1;//<--------- invalid
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_update_sim_pb_record_03()
{
	TelSimPbRecordData_t  pb_add;
	pb_add.StorageFileType = -1;//<--------- invalid
	int  pRequestId=0;
	pb_add.Index=1;
	char unicode_text[256];
	int unicode_text_len = 0;

	TET_STAMP_START("TAPI_API_INVALID_INPUT");

	unsigned char name[]="user";
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData,name,20);

	/* GSM 7bit conversion */
	oem_encode_gsm_7_bit_chars(strlen((char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData) ,
				(char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData, (char *)&unicode_text_len, unicode_text);
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData, unicode_text, unicode_text_len );
	pb_add.ContactInfo.PbAdditionalNumStringData.TextLength =  unicode_text_len;
	pb_add.ContactInfo.PbAdditionalNumStringData.EncryptionType = TAPI_SIM_TEXT_ENC_GSM7BIT; // always 2 for GAS and AAS

	TET_CHECK(TAPI_API_INVALID_INPUT, tel_update_sim_pb_record(&pb_add, &pRequestId));
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_update_sim_pb_record() API : Negative condition(Upper boundry test)
* @par ID: UTC_TEL_UPDATE_SIM_PB_RECORD_04
* @param [in] pb_add.StorageFileType = (TelSimPbFileType_t)TAPI_SIM_PB_UNKNOWNN + 1;//<--------- invalid
* @param [out] &pRequestId
* @return TAPI_API_INVALID_PTR
*/
void utc_tel_update_sim_pb_record_04()
{
	TelSimPbRecordData_t  pb_add;
	pb_add.StorageFileType=(TelSimPbFileType_t)TAPI_SIM_PB_UNKNOWNN+1;//<--------- invalid
	int  pRequestId=0;
	pb_add.Index=1;
	char unicode_text[256];
	int unicode_text_len = 0;

	TET_STAMP_START("TAPI_API_INVALID_INPUT");

	unsigned char name[]="user";
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData,name,20);

	/* GSM 7bit conversion */
	oem_encode_gsm_7_bit_chars(strlen((char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData) ,
				(char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData, (char *)&unicode_text_len, unicode_text);
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData, unicode_text, unicode_text_len );
	pb_add.ContactInfo.PbAdditionalNumStringData.TextLength =  unicode_text_len;
	pb_add.ContactInfo.PbAdditionalNumStringData.EncryptionType = TAPI_SIM_TEXT_ENC_GSM7BIT; // always 2 for GAS and AAS

	TET_CHECK(TAPI_API_INVALID_INPUT, tel_update_sim_pb_record(&pb_add, &pRequestId));
	TET_STAMP_PASS();
	return;
}


/**
* @brief tel_update_sim_pb_record() API : Positive condition(Save to 3GSIM)
* @par ID: UTC_TEL_UPDATE_SIM_PB_RECORD_05
* @param [in] pb_add
* @code
	pb_add.StorageFileType=TAPI_SIM_PB_3GSIM;
	pb_add.Index=1;

	char unicode_text[256];
	int unicode_text_len = 0;

	unsigned char name[]="user";
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData,name,20);

	// Unicode conversion conversion
	BOOL ret =oem_encode_text_unicode(strlen((char *)pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileData) ,
				(char *)pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileData, (char *)&unicode_text_len, unicode_text);
	memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData, unicode_text, unicode_text_len );
	pb_add.ContactInfo.PbAdditionalNumStringData.TextLength =  unicode_text_len;
	pb_add.ContactInfo.PbAdditionalNumStringData.EncryptionType = TAPI_SIM_TEXT_ENC_GSM7BIT;
* @endcode
* @param [out] &pRequestId
* @return TAPI_API_SUCCESS
* @par Async Response Message:
* The event associated is below and the event data is #TelSimPbUpdateResp_t. \n
* 	- TAPI_EVENT_SIM_PB_UPDATE_CNF
*/
void utc_tel_update_sim_pb_record_05()
{
	TelSimPbRecordData_t  pb_add = {0,};
	pb_add.StorageFileType=TAPI_SIM_PB_3GSIM;
	int  pRequestId=0;
	pb_add.Index=1;
	char unicode_text[256];
	int unicode_text_len = 0;

	TET_STAMP_START("StorageFileType=TAPI_SIM_PB_3GSIM");
	TET_RESULT_NOT_EQUAL(TET_UNTESTED,gStartupSuccess, TRUE);


	pb_add.ContactInfo.Pb3GData.FileTypeCount = 1;
	pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileType = TAPI_PB_3G_NAME;
	pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileDataType.EncryptionType = TAPI_SIM_TEXT_ENC_UCS2;

	unsigned char name[]="user";
	memcpy(pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileData,name,20);

	/* Unicode conversion */
	BOOL ret =oem_encode_text_unicode(strlen((char *)pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileData) ,
				(char *)pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileData, (char *)&unicode_text_len, unicode_text);
	if (ret == FALSE)
	{
		tet_printf("unicode conversion failed");
		TET_STAMP_FAIL();
		return;
	}
	memcpy(pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileData, unicode_text, unicode_text_len );
	pb_add.ContactInfo.Pb3GData.PbFileDataInfo[0].FileDataLength =  unicode_text_len;

	gAsyncResult = FALSE;

	TET_CHECK(TAPI_API_SUCCESS, tel_update_sim_pb_record(&pb_add, &pRequestId));

	G_MAIN_LOOP_RUN(2);
	TET_RESULT_NOT_EQUAL(TET_FAIL,gAsyncResult,TRUE); // If gAsyncResult != TRUE, then TET_FAIL and return
	TET_STAMP_PASS();
	return;
}


/** @} */

