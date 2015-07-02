/*
 * libslp-tapi
 *
 * Copyright (c) 2014 Samsung Electronics Co., Ltd. All rights reserved.
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
 * @file TapiUtility.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE
 * @{
 */

#ifndef _TEL_UTILITY_H_
#define _TEL_UTILITY_H_

__BEGIN_DECLS

/**
 * @brief Definition for Invalid Request ID value.
 */
#define INVALID_REQUEST_ID	-1
#define DBUS_DEFAULT_PATH	"/org/tizen/telephony"

/**
 * @brief Definition for negative error value
 * @remarks If result is negative value in callback function, it comply below error.
 */
#define TAPI_ERROR_OPERATION_FAILED -1
#define TAPI_ERROR_ACCESS_DENIED -2
#define TAPI_ERROR_OPERATION_NOT_SUPPORTED -3

/**
 * @brief Enumeration for TAPI return codes.
 */
typedef enum {
	TAPI_API_SUCCESS = 0, /**< No Error occurred */
	TAPI_API_INVALID_INPUT = -1, /**< Input values are not correct in the TAPI Library */
	TAPI_API_INVALID_PTR = -2, /**< Invalid pointer */
	TAPI_API_NOT_SUPPORTED = -3, /**< The feature corresponding to the requested API is not supported. This may be due to market/network/vendor reasons such as the feature is not available in the network */
	TAPI_API_DEPRICATED = -4, /**< This API is deprecated and will be so in the future also */
	TAPI_API_SYSTEM_OUT_OF_MEM = -5, /**< Out of memory */
	TAPI_API_SYSTEM_RPC_LINK_DOWN = -6, /**< RPC link down */
	TAPI_API_SERVICE_NOT_READY = -7, /**< Phone is powered on, but is still to receive the power up completed notification */
	TAPI_API_SERVER_FAILURE = -8, /**< Error occurred in the Telephony server */
	TAPI_API_OEM_PLUGIN_FAILURE = -9, /**< Plug-in layer failure */
	TAPI_API_TRANSPORT_LAYER_FAILURE = -10, /**< Transport layer Failure */
	TAPI_API_INVALID_DATA_LEN = -11, /**< Invalid data length */
	TAPI_API_REQUEST_MAX_IN_PROGRESS = -12, /**< Maximum number of API Requests for the same service are already in progress */
	TAPI_API_OFFLINE_MODE_ERROR = -13, /**< OEM Telephony Provider is in the Offline mode */
	TAPI_EVENT_CLASS_UNKNOWN = -14, /**< Event class specified is not present in the Event Class list */
	TAPI_EVENT_UNKNOWN = -15, /**< Event specified is not present in the TAPI Unsolicited Event list */
	TAPI_REGISTRATION_OP_FAILED = -16, /**< Callback Registration/De-registration failed */
	TAPI_API_OPERATION_FAILED = -17, /**< API operation failed */
	TAPI_API_INVALID_OPERATION = -18, /**< API Invalid Operation */
	TAPI_API_ACCESS_DENIED = -19, /**< Access Denied */

	TAPI_API_SYSTEM_RPC_LINK_NOT_EST = -100, /**< RPC link down */
	TAPI_API_API_NOT_SUPPORTED = -101, /**< API not supported */
	TAPI_API_SERVER_LAYER_FAILURE = -102, /**< Server layer failure */

	/*	CALL */
	TAPI_API_INVALID_CALL_ID = -200, /**< Invalid call ID */
	TAPI_API_CALL_CTXT_OVERFLOW = -201, /**< Call context overflow */
	TAPI_API_COULD_NOT_GET_CALL_CTXT = -202, /**< Could not get the call context */
	TAPI_API_CTXT_SEARCH_RET_NON_CALL_CTXT = -203, /**< Context search returned the non-call context */
	TAPI_API_COULD_NOT_DESTROY_CTXT = -204, /**< Could not destroy the context */
	TAPI_API_INVALID_LINE_ID = -205, /**< Invalid line ID */
	TAPI_API_INVALID_CALL_HANDLE = -206, /**< Invalid call handle */
	TAPI_API_INVALID_CALL_STATE = -207, /**< Invalid call state - Newly added. Need to raise a CR for this */
	TAPI_API_CALL_PRE_COND_FAILED = -208, /**< Pre condition like MO call cannot be established now */
	TAPI_API_CALL_SAME_REQ_PENDING = -209, /**< Cannot accept the same request multiple times */

	/*	POWER	*/
	TAPI_API_MODEM_POWERED_OFF = -300, /**< Modem is powered off */
	TAPI_API_MODEM_ALREADY_ON = -301, /**< Modem is already on */
	TAPI_API_MODEM_ALREADY_OFF = -302, /**< Modem is already off */

	/* NETTEXT */
	TAPI_API_NETTEXT_DEVICE_NOT_READY = -400, /**< Nettext device is not ready */
	TAPI_API_NETTEXT_SCADDR_NOT_SET = -401, /**< Nettext SCA address is not set */
	TAPI_API_NETTEXT_INVALID_DATA_LEN = -402, /**< Nettext Invalid data length */
	TAPI_NETTEXT_SCADDRESS_NOT_SET = -403, /**< Nettext SCA address is not set */

	/* SIM  */
	TAPI_API_SIM_CARD_ERROR = -500, /**< SIM general error */
	TAPI_API_SIM_NOT_FOUND = -501, /**< SIM is not present / removed */
	TAPI_API_SIM_NOT_INITIALIZED = -502, /**< SIM has not initialized yet (waiting for PIN verification) */
	TAPI_API_SIM_LOCKED = -503, /**< PIN/PUK/NCK/NSCK/SPCK/CCK required state */
	TAPI_API_SIM_PERM_BLOCKED = -504, /**< SIM Permanently blocked state (no more allowing PUK input) */
	TAPI_API_SIM_SERVICE_IS_DISABLED = -505, /**< Service is disabled in EF-SST due to a given operation */

	/* SAT  */
	TAPI_API_SAT_INVALID_COMMAND_ID = -600, /**< Command Number Invalid	*/
	TAPI_API_SAT_COMMAND_TYPE_MISMATCH = -601, /**<	Command Type Mismatch */
	TAPI_API_SAT_EVENT_NOT_REQUIRED_BY_USIM = -602, /**< Event Not Requested by the USIM */

	/* Network */
	TAPI_API_NETWORK_INVALID_CTXT = -700, /**< Network invalid contex t*/
	TAPI_API_NETWORK_PLMN_NOT_ALLOWED = -701,	  /** 3GPP Attache reject cause 11 */
	TAPI_API_NETWORK_ROAMING_NOT_ALLOWED = -702, /** 3GPP Attache reject cause 13 */

	/*Misc */
	TAPI_API_MISC_RETURN_NULL = -800, /**< MISC return NULL */
	TAPI_API_MISC_VALIDITY_ERROR = -801, /**< MISC validity error */
	TAPI_API_MISC_INPUTPARM_ERROR = -802, /**< MISC input parameter error */
	TAPI_API_MISC_OUTPARAM_NULL = -803, /**< MISC output parameter NULL */
} TapiResult_t;

/**
 * @brief Checks the 'scalar_exp' for TRUE, if failed then it returns 'err_value' from the function.
 */
#define TAPI_RET_ERR_NUM_IF_FAIL(scalar_exp, err_num) {\
	if (!(scalar_exp)) \
	{ \
		err("TAPI_RET_ERR_NUM_IF_FAIL Failed. returning [%d]", err_num);\
		return err_num; \
	} \
}

/**
 * @brief Returns from the current function if the expression is not true.
 * @details If the expression evaluates to FALSE, a critical message is logged and the function returns.
 *          This can only be used in functions which do not return a value.
 *
 * @param[in] scalar_exp The expression to check
 */
#define TAPI_RETURN_IF_FAIL(scalar_exp) {\
	if (!scalar_exp) \
	{ \
		err("TAPI_RETURN_IF_FAIL: Failed: Returning from here."); \
		return;	\
	} \
}

/**
 * @brief Returns from the current function, returning the value @a ret, if the expression is not true.
 * @details If the expression evaluates to FALSE, a critical message is logged and @a ret is returned.
 */
#define TAPI_RETURN_VAL_IF_FAIL(scalar_exp, ret) { \
	if (!scalar_exp) \
	{ \
		err("TAPI_RETURN_VAL_IF_FAIL: Failed: Returning [%d]", ret); \
		return ret; \
	} \
}

/**
 * @brief Returns from the current function, returning the value @a ret, if the expression is not true.
 * @details If the expression evaluates to FALSE, a critical message is logged and @a ret is returned.
 */
#define TAPI_RETURN_VAL_IF_SUCCESS(scalar_exp, ret) { \
	if (scalar_exp) \
	{ \
		err("TAPI_RETURN_VAL_IF_SUCCESS: Failed Returning [%d]", ret); \
		return ret; \
	} \
}

/**
 * @brief Logs an error on stderr and abort, if assertion failed.
 * @details Use this only if the telephpny sevrer needs to be exited, if asertion fails.
 */
#define TAPI_ASSERT(scalar_exp)	assert(scalar_exp);

/**
 * @brief Returns from the current function, returning the value @a ret, if @a exp1 and @a exp2 are of the same value.
 * @details If the equal expression evaluates to TRUE, a critical message is logged and @a ret is returned.
 */
#define TAPI_RETURN_VAL_IF_EQUAL(exp1, exp2, ret) {	\
	if (exp1 == exp2) \
	{ \
		err("TAPI_RETURN_VAL_IF_EQUAL: FAILED, Returning [%d]", ret); \
		return ret; \
	} \
}

/**
 * @brief Allocates a region of the memory, which is large enough to hold n elements of size bytes each.
 * @details The allocated region is initialized to zero.
 */
#define TAPI_CALLOC(ptr, no_elements, type)	\
	ptr = (type *) calloc (no_elements , sizeof (type)); \
	if (ptr == NULL) { \
		perror("NULL is returned"); \
		err("calloc error -NULL, errno is [%d]", errno); \
		TAPI_ASSERT(ptr); \
	}

/**
 * @brief Deletes a pointer with double free check.
 * @details If the same pointer is passed to free twice, known as a double free.
 *          To avoid this, set pointers to NULL after passing them to free: free(NULL) is safe (it does nothing).
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
 * @brief Checks for available destination buffer size before copying source buffer data to the destination buffer.
 * @details It asserts if the destination buffer size is less than the source buffer size.
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
 * @brief Encodes a sequence of binary data into its Base-64 stringified representation.
 *
 * @param[in] data The binary data to encode
 * @param[in] len The length of data
 * @param[out] encoded_data The encoded data
 * @return A newly allocated, zero-terminated Base-64 encoded string representing data
 */
#define TAPI_BASE64_ENCODE(data, len, encoded_data) \
	encoded_data = g_base64_encode((const guchar *)data, (gsize) len);

/**
 * @brief Decodes a sequence of Base-64 encoded text into binary data.
 *
 * @param[in] text The zero-terminated string with base64 text to decode
 * @param[out] decoded_data The decoded data
 * @param[in] ptr_out_len The length of the decoded data
 * @return A newly allocated buffer containing the binary data representing text
 */
#define TAPI_BASE64_DECODE(text, decoded_data, ptr_out_len)	\
	decoded_data = g_base64_decode((const gchar *)text, (gsize *)ptr_out_len);

/**
 * @brief Calculates the length of SCA in BCD type.
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
