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

#ifndef _TET_TAPI_UTIL_
#define _TET_TAPI_UTIL_

//#include <tapi_err.h>
#include "TapiCommon.h"

extern void G_MAIN_LOOP_CLOSE();
extern int G_MAIN_LOOP_RUN(int);
extern void  ss_utc_callback(TelTapiEvent_t *, void *data);
extern const char* TET_RESULT_TO_STRING(TapiResult_t err_code);

typedef enum{
	SS_NO_RESPONSE,
	SS_OK,
	SS_FAIL
} SsAsyncResponse_t;

#ifndef BOOL
    typedef int                 BOOL;
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef FALSE
#define FALSE           0
#endif

// TET_Error Code string return macro
typedef struct {
	TapiResult_t err;
	char * string;
}TAPI_Error_String;

// TET_Event Code string return macro
typedef struct {
	int event_class;
	char * string;
}TAPI_EventClass_String;


// TET_Event Code string return macro
typedef struct {
	int event;
	char * string;
}TAPI_Event_String;


/////////////////////////////////////////////////////////////////////////
//                      TETware Utility Function
/////////////////////////////////////////////////////////////////////////


// TET_STAMP ******************************************************************************
#define TET_STAMP_START(log) \
do {\
	tet_printf("\n###########################################\n## %s \n## %s ", __FUNCTION__,log);\
} while(0)

#define TET_STAMP_PASS() \
do {\
	tet_printf("%s : pass",__FUNCTION__);\
	tet_result(TET_PASS);\
} while(0)

#define TET_STAMP_FAIL() \
do {\
	tet_printf("%s : fail",__FUNCTION__);\
	tet_result(TET_FAIL);\
} while(0)

#define TET_STAMP(result) \
do {\
	tet_printf("%s[%d] %s() TET_STAMP(%d)",__FILE__,__LINE__,__FUNCTION__,result); \
	tet_result(result);\
} while(0)

// TET_RESULT Function *********************************************************************
#define TET_RESULT_IF_EQUAL(result,x1,x2) \
do { \
   if ((x1) == (x2)) \
	{ \
	   tet_printf("[line %d] Expected Result: %d  Actual Result: %d",__LINE__, x1,x2); \
	   tet_result(result); \
	   if(result==TET_PASS || result==TET_FAIL || result == TET_UNTESTED) return;\
	} \
 } while(0)

#define TET_RESULT_IF_EQUAL_THEN_GOTO(result,label,x1,x2) \
do { \
   if ((x1) == (x2)) \
	{ \
	    tet_result(result); \
	    goto label; \
	} \
   else \
   { \
	tet_printf("[line %d] Expected Result: %d  Actual Result: %d",__LINE__, x1,x2); \
   } \
 } while(0)

#define TET_RESULT_IF_NOT_EQUAL(result,x1,x2) \
do { \
   if ((x1) != (x2)) \
	{ \
	tet_printf("[line %d] Expected Result: %d  Actual Result: %d",__LINE__, x1,x2); \
	   tet_result(result); \
	   if(result==TET_PASS || result==TET_FAIL || result == TET_UNTESTED) return; \
	} \
 } while(0)


#define TET_RESULT_IF_NOT_EQUAL_THEN_GOTO(result,label,x1,x2) \
do { \
   if ((x1) != (x2)) \
	{ \
	tet_printf("[line %d] Expected Result: %d  Actual Result: %d",__LINE__, x1,x2); \
	   tet_result(result); \
	   tet_result(result); \
	   goto label;\
	} \
 } while(0)


// TET_CHECK Macro : TAPI api return value test macro. If it is not TAPI_API_SUCCESS, tet_retult(TET_FAIL) is called
#define TET_CHECK(x1, y...) \
do { \
   TapiResult_t err = y;\
   if (err != (x1)) \
	{ \
	tet_printf("[line %d] Expected Result: %d  Actual Result: %d",__LINE__, x1,x2); \
		tet_result(TET_FAIL); \
		return;\
	} \
 } while(0)


#define TET_PRECONDITION(x1, y...) \
do { \
   TapiResult_t err = y;\
   if (err != (x1)) \
	{ \
		tet_printf("%s[%d] %s : Test precondition fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_UNTESTED); \
		return;\
	} \
 } while(0)

#define TET_PRECONDITION2(x1, x2, y...) \
do { \
   TapiResult_t err = y;\
   if ( !(err == (x1) || err == (x2))) \
	{ \
		tet_printf("%s[%d] %s : Test precondition fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_UNTESTED); \
		return;\
	} \
 } while(0)


#define TET_CHECK_GOTO(x1,label, y...) \
do { \
   TapiResult_t err = y;\
   if (err != (x1)) \
	{ \
		tet_printf("%s[%d] %s() : test fail. error code =%x [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		goto label;\
	} \
 } while(0)

#define TET_PRECONDITION_GOTO(x1,label, y...) \
do { \
   TapiResult_t err = y;\
   if (err != (x1)) \
	{ \
		tet_printf("%s[%d] %s() : Test precondition fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_UNTESTED); \
		goto label;\
	} \
 } while(0)

#define TET_PRECONDITION2_GOTO(x1,x2,label, y...) \
do { \
   TapiResult_t err = y;\
   if ( !(err == (x1)||err == (x2))) \
	{ \
		tet_printf("%s[%d] %s() : Test precondition fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_UNTESTED); \
		goto label;\
	} \
 } while(0)


#define TET_CHECK2(x1,x2, y...) \
do { \
   TapiResult_t err = y;\
   if (!(err == (x1) || err == (x2))) \
	{ \
		tet_printf("%s[%d] %s() : test fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		return;\
	} \
 } while(0)

#define TET_CHECK2_GOTO(x1,x2, label, y...) \
do { \
   TapiResult_t err = y;\
   if (!(err == (x1) || err == (x2))) \
	{ \
		tet_printf("%s[%d] %s() : test fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		goto label;\
	} \
   else tet_printf("%s[%d] %s() : test success. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
 } while(0)


#define TET_CHECK3(x1,x2,x3, y...) \
do { \
   TapiResult_t err = y;\
   if (!(err == (x1) || err == (x2) || err == (x3))) \
	{ \
		tet_printf("%s[%d] %s() : test fail. error code =%d [%s] ",__FILE__,__LINE__,__FUNCTION__, err, TET_ERROR_TO_STRING(err) ); \
		tet_result(TET_FAIL); \
		return;\
	} \
 } while(0)
#endif //_TET_TAPI_UTIL_
