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


#ifndef _TET_TAPI_UTIL_NEW_
#define _TET_TAPI_UTIL_NEW_

#include <TelUtility.h>
#include <TelErr.h>
#include "TapiCommon.h"


#define TIME_DELAY0 3000
#define TIME_DELAY1 20000
#define TIME_DELAY2 30000
#define TIME_DELAY3 30000

#define INVALID_CALL_HANDLE 0
#define VALID_CALL_HANDLE	1

#define MOB_NUM1 "01012345678"   //Used for MO call creation.Change the number according to your country.
#define MOB_NUM2 "01098765432"   //Used to divert a call to specified number.Change the number according to your country.

#define DTMF_STRING "12345"  // User can change as per their choice

#define LOCAL_MOB_NUM     "01012345678"


#define	MAX_NUMBER_SIZE		16
#define TEST_INVALID_VALUE_1	-1
#define TEST_INVALID_VALUE_2	5

// TET_Error Code string return macro
typedef struct {
	TapiResult_t err;
	char * string;
}TAPI_Error_String;

//#define TAPI_ERROR_MAX TAPI_API_MISC_OUTPARAM_NULL+1
#define TAPI_ERROR_MAX 200 

TAPI_Error_String g_TAPI_ErrorString[TAPI_ERROR_MAX] = {

	/* #################### COMMON START ################ */
	{TAPI_API_SUCCESS, 		"TAPI_API_SUCCESS : No Error occured"},
	{TAPI_API_INVALID_INPUT, 	"TAPI_API_INVALID_INPUT : Input values are not correct in TAPI Library"},
	{TAPI_API_INVALID_PTR, 		"TAPI_API_INVALID_PTR : Invalid pointer"},
	{TAPI_API_NOT_SUPPORTED, 	"TAPI_API_NOT_SUPPORTED: The feature corresponding to requested API is not supported. This may be due to market/network/vendor reasons such as the feature is not available in the network"},
	{TAPI_API_DEPRICATED, 		"TAPI_API_DEPRICATED : This API is deprecated and will be so in future also"},
	{TAPI_API_SYSTEM_OUT_OF_MEM, 	"TAPI_API_SYSTEM_OUT_OF_MEM : Out of memory"},
	{TAPI_API_SYSTEM_RPC_LINK_DOWN, "TAPI_API_SYSTEM_RPC_LINK_DOWN : RPC link down"},
	{TAPI_API_MODEM_POWERED_OFF, 	"TAPI_API_MODEM_POWERED_OFF : The Modem is powered off"},
	{TAPI_API_SERVICE_NOT_READY, 	"TAPI_API_SERVICE_NOT_READY : Phone was powered on, but yet to receive the power up completed notification"},
	{TAPI_API_SERVER_FAILURE , 	"TAPI_API_SERVER_FAILURE : error occurred in Telephony server"},
	{TAPI_API_OEM_PLUGIN_FAILURE , 	"TAPI_API_OEM_PLUGIN_FAILURE  : Plug-in layer failure"},
	{TAPI_API_TRANSPORT_LAYER_FAILURE, "TAPI_API_TRANSPORT_LAYER_FAILURE"},

	{TAPI_API_INVALID_CALL_HANDLE, 	"TAPI_API_INVALID_CALL_HANDLE : Invalid call handle"},
	{TAPI_API_INVALID_CALL_STATE, 	"TAPI_API_INVALID_CALL_STATE : Invalid call state- Newly added. Need to raise a CR for this"},

	{TAPI_API_INVALID_DATA_LEN, 	"TAPI_API_INVALID_DATA_LEN : Invalid data length"},
	{TAPI_API_SIM_NOT_FOUND, 	"TAPI_API_SIM_NOT_FOUND : Sim is not present / removed "},
	{TAPI_API_SIM_BUSY, 		"TAPI_API_SIM_BUSY : SIM is busy"},
	{TAPI_API_SIM_INVALID_CARD_TYPE, "TAPI_API_SIM_INVALID_CARD_TYPE : Sim card is unknown "},
	{TAPI_API_SIM_CARD_PERMANENT_ERROR, "TAPI_API_SIM_CARD_PERMANENT_ERROR : SIM error/blocked state"},
	{TAPI_API_SIM_NOT_INITIALIZED, 	"TAPI_API_SIM_NOT_INITIALIZED : SIM has not initialized yet (waiting for PIN verifcation, etc)"},
	{TAPI_API_REQUEST_MAX_IN_PROGRESS, "TAPI_API_REQUEST_MAX_IN_PROGRESS : Maximum number of API Request for the same service are already in progress "},
	{TAPI_API_OFFLINE_MODE_ERROR,	"TAPI_API_OFFLINE_MODE_ERROR : OEM Telephony Provider is in Offline mode"},
	{TAPI_EVENT_CLASS_UNKNOWN, 	"TAPI_EVENT_CLASS_UNKNOWN : Event class specified is not present in Event Class list"},
	{TAPI_EVENT_UNKNOWN, 		"TAPI_EVENT_UNKNOWN : Event specified is not present in TAPI Unsolicited Event list"},
	{TAPI_REGISTRATION_OP_FAILED, 	"TAPI_REGISTRATION_OP_FAILED : Callback Registration/De-registration failed"},
	{TAPI_NETTEXT_SCADDRESS_NOT_SET, "TAPI_NETTEXT_SCADDRESS_NOT_SET"},
	{TAPI_API_OPERATION_FAILED, 	"TAPI_API_OPERATION_FAILED"},
	{TAPI_API_INVALID_OPERATION,	"TAPI_API_INVALID_OPERATION"},

	/*    Samsung Specif   */
    {TAPI_API_SYSTEM_RPC_LINK_NOT_EST, "TAPI_API_SYSTEM_RPC_LINK_NOT_EST"},
	{TAPI_API_API_NOT_SUPPORTED,	"TAPI_API_API_NOT_SUPPORTED"},
	{TAPI_API_SERVER_LAYER_FAILURE,	"TAPI_API_SERVER_LAYER_FAILURE"},

	/*        CALL        */
	{TAPI_API_INVALID_CALL_ID,	"TAPI_API_INVALID_CALL_ID"},
	{TAPI_API_CALL_CTXT_OVERFLOW, 	"TAPI_API_CALL_CTXT_OVERFLOW"},
	{TAPI_API_COULD_NOT_GET_CALL_CTXT, "TAPI_API_COULD_NOT_GET_CALL_CTXT"},
	{TAPI_API_CTXT_SEARCH_RET_NON_CALL_CTXT, "TAPI_API_CTXT_SEARCH_RET_NON_CALL_CTXT"},
	{TAPI_API_COULD_NOT_DESTROY_CTXT,  "TAPI_API_COULD_NOT_DESTROY_CTXT"},
	{TAPI_API_INVALID_LINE_ID, 	"TAPI_API_INVALID_LINE_ID"},

	/*	POWER	*/
//	{TAPI_API_MODEM_ALREADY_ON,  "TAPI_API_MODEM_ALREADY_ON"},
//	{TAPI_API_MODEM_ALREADY_OFF, "TAPI_API_MODEM_ALREADY_OFF"},

	/* NETTEXT */
	{TAPI_API_NETTEXT_DEVICE_NOT_READY, 	"TAPI_API_NETTEXT_DEVICE_NOT_READY"},
	{TAPI_API_NETTEXT_SCADDR_NOT_SET, 	"TAPI_API_NETTEXT_SCADDR_NOT_SET"},
	{TAPI_API_NETTEXT_INVALID_DATA_LEN, 	"TAPI_API_NETTEXT_INVALID_DATA_LEN"},

	/* SIM  */
	//{TAPI_API_SIM_DEV_CTX_NULL, 		"TAPI_API_SIM_DEV_CTX_NULL"},
	//{TAPI_API_SIM_SIM_NOT_FOUND, 		"TAPI_API_SIM_SIM_NOT_FOUND"},
	{TAPI_API_SIM_SIM_SESSION_FULL,		"TAPI_API_SIM_SIM_SESSION_FULL"},
	//{TAPI_API_SIM_INVALID_APP_ID,		"TAPI_API_SIM_INVALID_APP_ID"},
	{TAPI_API_SIM_INVALID_SESSION,		"TAPI_API_SIM_INVALID_SESSION"},
	{TAPI_API_SIM_FILE_TYPE_WRONG,		"TAPI_API_SIM_FILE_TYPE_WRONG"},
	{TAPI_API_SIM_FILE_ENCODE_FAIL,		"TAPI_API_SIM_FILE_ENCODE_FAIL"},
	{TAPI_API_SIM_FILE_DECODE_FAIL,		"TAPI_API_SIM_FILE_DECODE_FAIL"},
	{TAPI_API_SIM_FILE_INVALID_ENCODE_LEN,	"TAPI_API_SIM_FILE_INVALID_ENCODE_LEN"},
	{TAPI_API_SIM_INVALID_RECORD_NUM,	"TAPI_API_SIM_INVALID_RECORD_NUM"},
	{TAPI_API_SIM_CASE_NOT_HANDLED,		"TAPI_API_SIM_CASE_NOT_HANDLED"},
	{TAPI_API_SIM_OEM_UNKNOWN_SIM_CARD,	"TAPI_API_SIM_OEM_UNKNOWN_SIM_CARD"},
	{TAPI_API_SIM_SEC_UKNOWN_PIN_TYPE,	"TAPI_API_SIM_SEC_UKNOWN_PIN_TYPE"},
	{TAPI_API_SIM_SEC_INVALID_PIN_TYPE,	"TAPI_API_SIM_SEC_INVALID_PIN_TYPE"},
	{TAPI_API_SIM_PB_INVALID_STORAGE_TYPE,	"TAPI_API_SIM_PB_INVALID_STORAGE_TYPE"},

	{TAPI_API_SAT_COMMAND_TYPE_MISMATCH,	"TAPI_API_SAT_COMMAND_TYPE_MISMATCH"},

	/* Network */
	{TAPI_API_NETWORK_INVALID_CTXT,		"TAPI_API_NETWORK_INVALID_CTXT"},

	/*Misc */
	{TAPI_API_MISC_RETURN_NULL,		"TAPI_API_MISC_RETURN_NULL"},
	{TAPI_API_MISC_VALIDITY_ERROR,		"TAPI_API_MISC_VALIDITY_ERROR"},
	{TAPI_API_MISC_INPUTPARM_ERROR,		"TAPI_API_MISC_INPUTPARM_ERROR"},
	{TAPI_API_MISC_OUTPARAM_NULL,		"TAPI_API_MISC_OUTPARAM_NULL"},
};

//#define TET_ERROR_TO_STRING(err) g_TAPI_ErrorString[(err)].string
static const char* TET_ERROR_TO_STRING(TapiResult_t err_code)
{
	int index;
	for( index=0; g_TAPI_ErrorString[index].err != TAPI_ERROR_MAX ; index++)
	{
		if (g_TAPI_ErrorString[index].err == err_code || g_TAPI_ErrorString[index].err == TAPI_ERROR_MAX)
			return g_TAPI_ErrorString[index].string;
	}
	return NULL;
}

// TET_CHECK Macro : TAPI api return value test macro. If it is not TAPI_API_SUCCESS, tet_retult(TET_FAIL) is called
#define TET_CHECK(x1, y...) \
 { \
   TapiResult_t err = y;\
   if (err != (x1)) \
	{ \
		tet_printf("%s[%d] : test fail. error code =%d [%s] ",__FILE__,__LINE__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		return;\
	} \
 }

#define TET_CHECK2(x1,x2, y...) \
 { \
   TapiResult_t err = y;\
   if (!(err == (x1) || err == (x2))) \
	{ \
		tet_printf("%s[%d] : test fail. error code =%d [%s] ",__FILE__,__LINE__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		return;\
	} \
 }

#define TET_CHECK3(x1,x2,x3, y...) \
 { \
   TapiResult_t err = y;\
   if (!(err == (x1) || err == (x2) || err == (x3))) \
	{ \
		tet_printf("%s[%d] : test fail. error code =%d [%s] ",__FILE__,__LINE__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		return;\
	} \
 }


#endif //_TET_TAPI_UTIL_
