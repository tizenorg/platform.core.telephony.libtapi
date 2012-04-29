/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Ja-young Gu <jygu@samsung.com>
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

#ifndef _TAPI_UTILITY_H_
#define _TAPI_UTILITY_H_

#include <dlog.h>
#include <TapiEvent.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LEVEL_DEBUG		LOG_DEBUG
#define LEVEL_INFO		LOG_DEBUG
#define LEVEL_NOTICE	LOG_DEBUG
#define LEVEL_WARNING	LOG_DEBUG
#define LEVEL_ERR		LOG_WARN
#define LEVEL_CRIT		LOG_WARN
#define LEVEL_ALERT		LOG_WARN
#define LEVEL_LOGGING	LOG_WARN
#define LEVEL_EMERG		LOG_FATAL

#define LOG_TELEPHONY	"TELEPHONY"

/**
 * Default we are disabling the logging. If we want enable logging based on our
 * requirement whether to log on to STD or to syslog we have to compile telephony
 * code with flags as mentioned below
 *
 * DISABLE LOGGING				----> No Flag
 * ENABLE LOGGING(to SYSLOG)	----> Enable Flag  _TAPI_DEBUG_ON_SYSLOG_ in Sconscript
 * ENABLE LOGGING(to STD)		----> Enable Flag  _TAPI_DEBUG_ON_SCREEN_ in Sconscript
 */

#define TAPI_LIB_DEBUG(level, frmt, args...)	do {RLOG(level,LOG_TELEPHONY, "[TAPI] %s; "frmt"\n", __func__, ##args);} while (0)
#define TAPI_PRINT_TIME(str, args...)			do { } while (0)

/*
#define TAPI_LIB_DEBUG(level, frmt, args...)  do { fprintf(stderr, "\n[TAPI Library] [PID: %d, %s:%d] " frmt , getpid(), __func__,  __LINE__, ##args); } while (FALSE)
#define TAPI_PRINT_TIME(str, args...)	 do { } while(0)
#define TAPI_SYSLOG_DEBUG(args...)   {\
	syslog(LOG_USER , "\n[Telephony Server] [%s:%d] ",  __func__, __LINE__);  \
	syslog(LOG_USER , args); }
*/

/**
 * This will check the 'scalar_exp' for TRUE, if failed then return 'err_value' from function.
 */
#define TAPI_RET_ERR_NUM_IF_FAIL(scalar_exp, err_num) {\
	if (!(scalar_exp)) \
	{ \
		TAPI_LIB_DEBUG(LEVEL_ERR, "TAPI_RET_ERR_NUM_IF_FAIL Failed. returning [%d]", err_num);\
		return err_num; \
	} \
}

/**
 * Returns from the current function if the expression is not true. If the expression evaluates to FALSE,
 * a critical message is logged and the function returns. This can only be used in functions which do not
 * return a value.
 *
 * expr : the expression to check.
 */
#define TAPI_RETURN_IF_FAIL(scalar_exp) {\
	if (!scalar_exp) \
	{ \
		TAPI_LIB_DEBUG(LEVEL_ERR, "TAPI_RETURN_IF_FAIL: Failed: Returning from here."); \
		return;	\
	} \
}

/**
 * Returns from the current function, returning the value ret, if the expression is not true.
 * If the expression evaluates to FALSE, a critical message is logged and ret is returned.
 */
#define TAPI_RETURN_VAL_IF_FAIL(scalar_exp, ret) { \
	if (!scalar_exp) \
	{ \
		TAPI_LIB_DEBUG(LEVEL_ERR, "TAPI_RETURN_VAL_IF_FAIL: Failed: Returning [%d]", ret); \
		return ret; \
	} \
}

/**
 * Returns from the current function, returning the value ret, if the expression is not true.
 * If the expression evaluates to FALSE, a critical message is logged and ret is returned.
 */
#define TAPI_RETURN_VAL_IF_SUCCESS(scalar_exp, ret) { \
	if (scalar_exp) \
	{ \
		TAPI_LIB_DEBUG(LEVEL_DEBUG, "TAPI_RETURN_VAL_IF_SUCCESS: Failed Returning [%d]", ret); \
		return ret; \
	} \
}

/**
 * This assert will log an err on stderr and abort,  if assertion failed,
 * Use this only if the telephpny sevrer needs to be exited, if asertion fails
 */
#define TAPI_ASSERT(scalar_exp)	assert(scalar_exp);

/**
 * Returns from the current function, returning the value ret, if the exp1 and exp2 are of same value.
 * If the equal expression evaluates to TRUE, a critical message is logged and ret is returned.
 */
#define TAPI_RETURN_VAL_IF_EQUAL(exp1, exp2, ret) {	\
	if (exp1 == exp2) \
	{ \
		TAPI_LIB_DEBUG(LEVEL_ALERT, "TAPI_RETURN_VAL_IF_EQUAL: FAILED, Returning [%d]", ret); \
		return ret; \
	} \
}

/**
 * void *calloc(size_t nelements, size_t bytes);
 * which allocates a region of memory large enough to hold nelements of size bytes each.
 * The allocated region is initialized to zero.
 */
#define TAPI_CALLOC(ptr, no_elements, type)	\
	ptr = (type *) calloc (no_elements , sizeof (type)); \
	if (ptr == NULL) { \
		perror("NULL is returned"); \
		TAPI_LIB_DEBUG(LEVEL_ERR, "calloc error -NULL, errno is [%d]", errno); \
		fprintf(stderr,"Calloc ERRNO %d\n", errno); \
		TAPI_ASSERT(ptr); \
	}

/**
 * why ptr = NULL; after free()?.
 * If the same pointer is passed to free twice,	known as a double free. To avoid this, set pointers to
 * NULL after passing them to free: free(NULL) is safe (it does nothing).
 */
#define TAPI_FREE(ptr) \
	if (ptr != NULL) { \
		free(ptr); \
		ptr = NULL; \
	} \
	else { \
		TAPI_LIB_DEBUG(LEVEL_ALERT, "Double Free or NULL"); \
	}

/**
 * check for available destination buffer size before copying source buffer data to destination buffer.
 * Assert if destination buffer size less than source buffer size.
 */
#define TAPI_MEMCPY(dest, src, dest_len, src_len) \
	if (dest_len >= src_len) { \
		memcpy(dest, src, src_len); \
	} \
	else { \
		TAPI_LIB_DEBUG(LEVEL_ERR, "TAPI_MEMCPY FAILURE - dest_len(%d) < src_len(%d)", dest_len, src_len); \
		assert(0); \
	}

/**
 * Encode a sequence of binary data into its Base-64 stringified representation.
 *
 * data : the binary data to encode.
 * len : the length of data.
 * Returns : a newly allocated, zero-terminated Base-64 encoded string representing data.
 * gchar *g_base64_encode (const guchar *data, gsize len);
 */
#define TAPI_BASE64_ENCODE(data, len, encoded_data) \
	encoded_data = g_base64_encode((const guchar *)data, (gsize) len);

/**
 * Decode a sequence of Base-64 encoded text into binary data
 *
 * text : zero-terminated string with base64 text to decode.
 * out_len : The length of the decoded data is written here.
 * Returns : a newly allocated buffer containing the binary data that text represents
 * guchar *g_base64_decode  (const gchar *text, gsize *out_len);
 */
#define TAPI_BASE64_DECODE(text, decoded_data, ptr_out_len)	\
	decoded_data = g_base64_decode((const gchar *)text, (gsize *)ptr_out_len);

/**
 * Define Macro for calculate the length of SCA in BCD type.
 */
#define TAPI_GET_LENGTH_FROM_BCD(nLength, rtn) { \
	if (nLength <= 0) { \
		rtn = 0; \
	} \
	else if ((nLength) % 2) { \
		rtn = ((nLength) / 2) + 1; \
	} \
	else { \
		rtn = (nLength) / 2; \
	} \
}


#define TAPI_VALIDATE_EVENTTYPE(val_min ,val_max, type, ret) { \
	if (type < val_min || type > val_max ) { \
		return ret; \
	} \
}

/**
 * Initialize the in-put & out-put params for the TAPI API's
 */
#define TAPI_GLIB_INIT_PARAMS() \
	GArray*	in_param1  = NULL; \
	GArray*	in_param2  = NULL; \
	GArray*	in_param3  = NULL; \
	GArray*	in_param4  = NULL; \
	GArray*	out_param1 = NULL; \
	GArray*	out_param2 = NULL; \
	GArray*	out_param3 = NULL; \
	GArray*	out_param4 = NULL;


/**
 * If alloc fails any where in between IP1 to OP4 then free allocated params,
 * and return FLASE to Application who ever called our API.
 *
 * GArray *g_array_new(gboolean zero_terminated, gboolean clear_ ,guint element_size);
 */
#define TAPI_GLIB_ALLOC_PARAMS(IP1,IP2,IP3,IP4,OP1,OP2,OP3,OP4) \
        IP1 = g_array_new(FALSE, FALSE, sizeof(gchar));	\
        IP2 = g_array_new(FALSE, FALSE, sizeof(gchar));	\
        IP3 = g_array_new(FALSE, FALSE, sizeof(gchar));	\
        IP4 = g_array_new(FALSE, FALSE, sizeof(gchar)); \
        if(IP1 && IP2 && IP3 && IP4) \
        { \
        } \
        else \
        { \
			TAPI_GLIB_FREE_PARAMS(IP1,IP2,IP3,IP4,OP1,OP2,OP3,OP4);	\
			return TAPI_API_SYSTEM_OUT_OF_MEM; \
        }


/**
 * Free any allocated in-put & out-put params
 */
#define TAPI_GLIB_FREE_PARAMS(IP1,IP2,IP3,IP4,OP1,OP2,OP3,OP4) \
	if (IP1) \
		g_array_free(IP1, TRUE); \
	if (IP2) \
		g_array_free(IP2, TRUE); \
	if (IP3) \
		g_array_free(IP3, TRUE); \
	if (IP4) \
		g_array_free(IP4, TRUE); \
	if (OP1) \
		g_array_free(OP1, TRUE); \
	if (OP2) \
		g_array_free(OP2, TRUE); \
	if (OP3) \
		g_array_free(OP3, TRUE); \
	if (OP4) \
		g_array_free(OP4, TRUE);

#define TAPI_SIGNAL_OBJPATH "/org/projectx/telephony_event"
#define TAPI_SIGNAL_SENDER "org.projectx.telephony_event"
#define TAPI_SIGNAL_MEMBER_ARG0 "ARG0"
#define TAPI_SIGNAL_MEMBER_ARG1 "ARG1"
#define TAPI_SIGNAL_MEMBER_ARG2 "ARG2"
#define TAPI_SIGNAL_MEMBER_ARG3 "ARG3"
#define TAPI_SIGNAL_MEMBER_ARG4 "ARG4"
#define TAPI_SIGNAL_MEMBER_ARG5 "ARG5"
#define TAPI_SIGNAL_MEMBER_GENRESP "GENRESP"

#define IF_NAME_MAX	100
/* Generate a unique signal interface name for each dbus message */
#ifndef TAPI_GET_EVENT_NAME
#define TAPI_GET_EVENT_NAME(TYPE, SIG_IF_NAME) { \
	char *_class = NULL; \
	char *_type = NULL; \
	switch((TYPE >>8)&0xFF)\
	{\
		case TAPI_EVENT_CLASS_CALL_VOICE:\
			_class = "Voicecall"; \
			break;\
		case TAPI_EVENT_CLASS_CALL_DATA:\
			_class = "Datacall"; \
			break;\
		case TAPI_EVENT_CLASS_SIM:\
			_class = "Sim"; \
			break;\
		case TAPI_EVENT_CLASS_NETTEXT:\
			_class = "Nettext"; \
			break;\
		case TAPI_EVENT_CLASS_NETWORK:\
			_class = "Network"; \
			break;\
		case TAPI_EVENT_CLASS_SS:\
			_class = "Ss"; \
			break;\
		case TAPI_EVENT_CLASS_SAT:\
			_class = "Sat"; \
			break;\
		case TAPI_EVENT_CLASS_DATA:\
			_class = "Data"; \
			break;\
		case TAPI_EVENT_CLASS_SOUND:\
			_class = "Sound"; \
			break;\
		case TAPI_EVENT_CLASS_UTIL:\
			_class = "Util"; \
			break;\
		case TAPI_EVENT_CLASS_ERR:\
			_class = "Error"; \
			break;\
		case TAPI_EVENT_CLASS_POWER:\
			_class = "Power"; \
			break;\
		case TAPI_EVENT_CLASS_DISPLAY:\
			_class = "Display"; \
			break;\
		case TAPI_EVENT_CLASS_MISC:\
			_class = "Misc"; \
			break;\
		case TAPI_EVENT_CLASS_CFG:\
			_class = "Cfg"; \
			break;\
		case TAPI_EVENT_CLASS_IMEI:\
			_class = "Imei"; \
			break;\
		case TAPI_EVENT_CLASS_GPS:\
			_class = "Gps"; \
			break;\
		case TAPI_EVENT_CLASS_OMADM:\
			_class = "Omadm"; \
			break;\
		case TAPI_EVENT_CLASS_SVCMODE:\
			_class = "Svcmode"; \
			break;\
		case TAPI_EVENT_CLASS_FACTORY:\
			_class = "Factory"; \
			break;\
		case TAPI_EVENT_CLASS_CALL_E911:\
			_class = "EmergencyCall"; \
			break;\
		case TAPI_EVENT_CLASS_CDMA_DATA:\
			_class = "CdmaData"; \
			break;\
		default:\
			_class = "Unknown"; \
			TAPI_LIB_DEBUG(LEVEL_ERR,"Invalid event class = 0lx%x",(TYPE >>8)&0xFF);\
	}\
	switch(TYPE&0xFF0000)\
	{\
		case TAPI_EVENT_TYPE_NOTIFICATION:\
			_type = "Noti"; \
			break;\
		case TAPI_EVENT_TYPE_CONFIRMATION:\
			_type = "Resp"; \
			break;\
		default:\
			_type = "Unknown"; \
			TAPI_LIB_DEBUG(LEVEL_ERR, "Invalid event type = 0x%lx",TYPE&0xFF0000);\
	}\
	snprintf(SIG_IF_NAME, IF_NAME_MAX, "Telephony.%s.%s_0x%x", _class, _type, TYPE);\
	TAPI_LIB_DEBUG(LEVEL_DEBUG, "event_string = %s", SIG_IF_NAME);\
}
#endif

#define TAPI_BACKTRACE  { \
			void *buffer[128]; \
			char **strings; \
			int i, nptrs; \
			nptrs = backtrace(buffer, 128); \
			TAPI_LIB_DEBUG(LEVEL_INFO, "backtrace() returned %d addresses", nptrs); \
			strings = backtrace_symbols(buffer, nptrs); \
			if (strings == NULL) { \
				TAPI_LIB_DEBUG(LEVEL_INFO, "backtrace_symbols() failed. (errno=%d)", errno); \
			} \
			else { \
				for (i=0; i<nptrs; i++) { \
					TAPI_LIB_DEBUG(LEVEL_INFO, "[%2d] %s", i, strings[i]); \
				} \
				free(strings); \
			} \
		}

#ifdef __cplusplus
}
#endif

#endif /* _TAPI_UTILITY_H_	*/
