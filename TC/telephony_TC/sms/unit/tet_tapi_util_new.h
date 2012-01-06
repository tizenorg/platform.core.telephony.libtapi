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


#ifndef _TET_UTIL_NEW_
#define _TET_UTIL_NEW_

#include <TelUtility.h>

// TET_Error Code string return macro
typedef struct {
	TapiResult_t err;
	char * string;
} TAPI_Error_String;

#ifndef BOOL
typedef int BOOL;
#endif

#ifndef TRUE
#define TRUE 	1
#endif

#ifndef FALSE
#define FALSE 	0
#endif

#define TAPI_ERROR_MAX TAPI_API_MISC_VALIDITY_ERROR+1

TAPI_Error_String g_ErrorString[TAPI_ERROR_MAX] = {

	/* #################### COMMON START ################ */

	{TAPI_API_SUCCESS, "No Error occured"},
	{TAPI_API_INVALID_INPUT, "Input values are not correct"},
	{TAPI_API_INVALID_PTR, "TAPI_API_INVALID_PTR"},
	{TAPI_API_API_DEPRICATED, "This API is depricated and will be so in future also."},
	{TAPI_API_SYSTEM_OUT_OF_MEM, "TAPI_API_SYSTEM_OUT_OF_MEM"},
	{TAPI_API_SYSTEM_RPC_LINK_DOWN, "TAPI_API_SYSTEM_RPC_LINK_DOWN"},
	{TAPI_API_SYSTEM_RPC_LINK_NOT_EST, "TAPI_API_SYSTEM_RPC_LINK_NOT_EST"},
	{TAPI_API_SERVICE_NOT_READY, "Phone was powered on, but yet to receive the power up completed noti"},
	{TAPI_API_SERVER_LAYER_FAILURE, "Erros happend in executing the following layer fucntions"},
	{TAPI_API_TRANSPORT_LAYER_FAILURE, "TAPI_API_TRANSPORT_LAYER_FAILURE"},

	/* #################### COMMON END ################ */

	/* CALL */
	{TAPI_API_INVALID_CALL_HANDLE, "TAPI_API_INVALID_CALL_HANDLE"},
	{TAPI_API_INVALID_CALL_STATE, "TAPI_API_INVALID_CALL_STATE"},
	{TAPI_API_INVALID_LINE_ID, "TAPI_API_INVALID_LINE_ID"},

	/* POWER */
	{TAPI_API_MODEM_ALREADY_ON, "TAPI_API_MODEM_ALREADY_ON"},
	{TAPI_API_MODEM_ALREADY_OFF, "TAPI_API_MODEM_ALREADY_OFF"},

	/* SAT */
	{TAPI_API_SAT_PARSER_ENCODING_ERROR, "TAPI_API_SAT_PARSER_ENCODING_ERROR"},
	{TAPI_API_SAT_COMMAND_NUMBER_INVALID, "TAPI_API_SAT_COMMAND_NUMBER_INVALID"},
	{TAPI_API_SAT_COMMAND_NUMBER_MISMATCH, "TAPI_API_SAT_COMMAND_NUMBER_MISMATCH"},
	{TAPI_API_SAT_COMMAND_TYPE_MISMATCH, "TAPI_API_SAT_COMMAND_TYPE_MISMATCH"},

	/* Misc */
	{TAPI_API_MISC_RETURN_NULL, "TAPI_API_MISC_RETURN_NULL"},
	{TAPI_API_MISC_INPUTPARM_ERROR, "TAPI_API_MISC_INPUTPARM_ERROR"},
	{TAPI_API_MISC_OUTPARAM_NULL, "TAPI_API_MISC_OUTPARAM_NULL"},
	{TAPI_API_MISC_VALIDITY_ERROR, "TAPI_API_MISC_VALIDITY_ERROR"},
};

static const char *TET_ERROR_TO_STRING(TapiResult_t err_code)
{
	int index;
	for (index = 0; g_ErrorString[index].err != TAPI_ERROR_MAX; index++) {
		if (g_ErrorString[index].err == err_code || g_ErrorString[index].err == TAPI_ERROR_MAX)
			return g_ErrorString[index].string;
	}
	return NULL;
}

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


#endif //_TET_UTIL_
