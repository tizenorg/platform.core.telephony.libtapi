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

/**
 * @open
 * @ingroup		TelephonyAPI
 * @addtogroup	COMMON_TAPI	COMMON
 * @{
 *
 * @file TelUtility.h

 @brief This file serves as a "C" header file defines structures for Utility Services. \n
 It contains a sample set of constants, enums, structs that would be required by applications.
 */

#ifndef _TEL_UTILITY_H_
#define _TEL_UTILITY_H_

__BEGIN_DECLS

#define INVALID_REQUEST_ID	-1    /**< Invalid RequestId Value */
#define DBUS_DEFAULT_PATH	"/org/tizen/telephony"

/**
 * @enum TapiResult_t
 * Below enumerations are the return codes of TAPI API's
 */
typedef enum {
	TAPI_API_SUCCESS = 0, /**<No Error occurred */
	TAPI_API_INVALID_INPUT = -1, /**<input values are not correct in TAPI Library */
	TAPI_API_INVALID_PTR = -2, /**<invalid pointer */
	TAPI_API_NOT_SUPPORTED = -3, /**<The feature corresponding to requested API is not supported. This may be due to market/network/vendor reasons such as the feature is not available in the network. */
	TAPI_API_DEPRICATED = -4, /**<This API is deprecated and will be so in future also */
	TAPI_API_SYSTEM_OUT_OF_MEM = -5, /**<Out of memory */
	TAPI_API_SYSTEM_RPC_LINK_DOWN = -6, /**<RPC link down */
	TAPI_API_SERVICE_NOT_READY = -7, /**<Phone was powered on, but yet to receive the power up completed notification */
	TAPI_API_SERVER_FAILURE = -8, /**<error occurred in Telephony server  */
	TAPI_API_OEM_PLUGIN_FAILURE = -9, /**<Plug-in layer failure */
	TAPI_API_TRANSPORT_LAYER_FAILURE = -10, /**<Transport layer Failure*/
	TAPI_API_INVALID_DATA_LEN = -11, /**<Invalid data length */
	TAPI_API_REQUEST_MAX_IN_PROGRESS = -12, /**<Maximum number of API Request for the same service are already in progress */
	TAPI_API_OFFLINE_MODE_ERROR = -13, /**<OEM Telephony Provider is in Offline mode. */
	TAPI_EVENT_CLASS_UNKNOWN = -14, /**<Event class specified is not present in Event Class list. - 20*/
	TAPI_EVENT_UNKNOWN = -15, /**<Event specified is not present in TAPI Unsolicited Event list. */
	TAPI_REGISTRATION_OP_FAILED = -16, /**<Callback Registration/De-registration failed */
	TAPI_API_OPERATION_FAILED = -17, /**<API operation failed*/
	TAPI_API_INVALID_OPERATION = -18, /**<API Invalid Operation */

	TAPI_API_SYSTEM_RPC_LINK_NOT_EST = -100, /**< RPC link down */
	TAPI_API_API_NOT_SUPPORTED = -101, /**<API not supported */
	TAPI_API_SERVER_LAYER_FAILURE = -102, /**< Server layer failure*/

	/*	CALL */
	TAPI_API_INVALID_CALL_ID = -200, /**< Invalid call ID*/
	TAPI_API_CALL_CTXT_OVERFLOW = -201, /**<Call context overflow */
	TAPI_API_COULD_NOT_GET_CALL_CTXT = -202, /**< Could not get call  context*/
	TAPI_API_CTXT_SEARCH_RET_NON_CALL_CTXT = -203, /**< Context search returned non-call context*/
	TAPI_API_COULD_NOT_DESTROY_CTXT = -204, /**< could not destroy context*/
	TAPI_API_INVALID_LINE_ID = -205, /**< invalid line ID*/
	TAPI_API_INVALID_CALL_HANDLE = -206, /**<Invalid call handle  - 35*/
	TAPI_API_INVALID_CALL_STATE = -207, /**<Invalid call state- Newly added. Need to raise a CR for this */
	TAPI_API_CALL_PRE_COND_FAILED = -208, /**<Pre condition like MO call can not be established now.*/
	TAPI_API_CALL_SAME_REQ_PENDING = -209, /**<  Can not accept same request multiple times  */

	/*	POWER	*/
	TAPI_API_MODEM_POWERED_OFF = -300, /**<The Modem is powered off */
	TAPI_API_MODEM_ALREADY_ON = -301, /**<Modem already on */
	TAPI_API_MODEM_ALREADY_OFF = -302, /**<Modem already off */

	/* NETTEXT */
	TAPI_API_NETTEXT_DEVICE_NOT_READY = -400, /**<Nettext device not ready */
	TAPI_API_NETTEXT_SCADDR_NOT_SET = -401, /**<Nettext SCA address not set */
	TAPI_API_NETTEXT_INVALID_DATA_LEN = -402, /**<Nettext Invalid data length */
	TAPI_NETTEXT_SCADDRESS_NOT_SET = -403, /**<Nettext SCA address not set*/

	/* SIM  */
	TAPI_API_SIM_CARD_ERROR = -500, /**<SIM error/blocked state */
	TAPI_API_SIM_NOT_FOUND = -501, /**<SIM is not present / removed */
	TAPI_API_SIM_NOT_INITIALIZED = -502, /**<SIM has not initialized yet (waiting for PIN verification, etc) */
	TAPI_API_SIM_LOCKED = -503,

	/* SAT  */
	TAPI_API_SAT_INVALID_COMMAND_ID = -600, /**<Command Number Invalid	*/
	TAPI_API_SAT_COMMAND_TYPE_MISMATCH = -601, /**<	Command Type Mismatch	*/
	TAPI_API_SAT_EVENT_NOT_REQUIRED_BY_USIM = -602, /**< Event Not Requested by USIM*/

	/* Network */
	TAPI_API_NETWORK_INVALID_CTXT = -700, /**< Network invalid context*/

	/*Misc */
	TAPI_API_MISC_RETURN_NULL = -800, /**< MISC return NULL*/
	TAPI_API_MISC_VALIDITY_ERROR = -801, /**< MISC validity error*/
	TAPI_API_MISC_INPUTPARM_ERROR = -802, /**< MISC input parameter error*/
	TAPI_API_MISC_OUTPARAM_NULL = -803, /**< MISC output parameter null*/
} TapiResult_t;

/**
 * This will check the 'scalar_exp' for TRUE, if failed then return 'err_value' from function.
 */
#define TAPI_RET_ERR_NUM_IF_FAIL(scalar_exp, err_num) {\
	if (!(scalar_exp)) \
	{ \
		err("TAPI_RET_ERR_NUM_IF_FAIL Failed. returning [%d]", err_num);\
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
		err("TAPI_RETURN_IF_FAIL: Failed: Returning from here."); \
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
		err("TAPI_RETURN_VAL_IF_FAIL: Failed: Returning [%d]", ret); \
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
		err("TAPI_RETURN_VAL_IF_SUCCESS: Failed Returning [%d]", ret); \
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
		err("TAPI_RETURN_VAL_IF_EQUAL: FAILED, Returning [%d]", ret); \
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
		err("calloc error -NULL, errno is [%d]", errno); \
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
		err("Double Free or NULL"); \
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
		err("TAPI_MEMCPY FAILURE - dest_len(%d) < src_len(%d)", dest_len, src_len); \
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

__END_DECLS

#endif //_TEL_UTILITY_H_
/**
 * @}
 */
