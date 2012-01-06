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
 * @internal
 * @ingroup		TelephonyAPI
 * @addtogroup	GPS_TAPI		GPS
 * @{
 *
 * @file tapi_gps_interface.h

 @brief This file serves as a "C" header file defines functions for Tapi GPS\n
 It contains a sample set of function prototypes,enums and structs that would be required by applications.
 * GPS APIs allow an application to accomplish the following services: @n
 * - Start a GPS session. @n
 * - End a GPS session. @n
 * - Request to initiate XTRA client download. @n
 * - To set XTRA Auto download. @n
 * - Set time information for XTRA. @n
 * - Set SSL for AGPS. @n
 * - Set AGPS bearer information. @n
 */

#ifndef _ITAPI_GPS_H_
#define _ITAPI_GPS_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/
#include <TapiCommon.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 MACROS
 ==================================================================================================*/
#define	TAPI_GPS_FIX_PARAM_TAG_MAX		0x0F
#define	TAPI_GPS_PARAMETERS_TAG_MAX		0x11
#define	TAPI_GPS_ADDR_LEN_MAX			16
#define	TAPI_GPS_URL_LEN_MAX			256
#define	TAPI_GPS_SECURITY_DATA_MAX		20
#define	TAPI_GPS_APN_LEN_MAX			101
#define	TAPI_GPS_SSD_LEN_MAX			32
#define	TAPI_GPS_SSL_LEN_MAX			32
#define	TAPI_GPS_XTRA_DATA_LEN_MAX		256
#define	TAPI_GPS_MAX_SATELLITES			12
#define	TAPI_GPS_DNS_LEN_MAX			256

/*==================================================================================================
 ENUMS
 ==================================================================================================*/
typedef enum {
	TAPI_GPS_FIX_REQ_SESSION_TYPE			= 0x01,
	TAPI_GPS_FIX_REQ_SESSION_OPERATION,
	TAPI_GPS_FIX_REQ_NUM_FIXES,
	TAPI_GPS_FIX_REQ_TIME_BTW_FIXES,
	TAPI_GPS_FIX_REQ_ACCURACY,
	TAPI_GPS_FIX_REQ_SESSION_TIMEOUT,
	TAPI_GPS_FIX_REQ_SERVER_OPTION,
	TAPI_GPS_FIX_REQ_SERVER_ADDR_TYPE,
	TAPI_GPS_FIX_REQ_SERVER_ADDR_IP,
	TAPI_GPS_FIX_REQ_SERVER_PORT,
	TAPI_GPS_FIX_REQ_SERVER_URL,
	TAPI_GPS_FIX_REQ_SEC_DATA_ID,
	TAPI_GPS_FIX_REQ_SEC_DATA_LEN,
	TAPI_GPS_FIX_REQ_SEC_DATA
} tapi_gps_fix_req_param_type_t;

typedef enum {
	TAPI_GPS_GET_LATEST_PD_INFO,
	TAPI_GPS_GET_NEW_POSITION,
	TAPI_GPS_TRACKING_MODE,
	TAPI_GPS_DATA_DOWNLOAD
} tapi_gps_session_type_t;

typedef enum {
	TAPI_GPS_STANDALONE_ONLY	= 0x01,
	TAPI_GPS_MSBASED,
	TAPI_GPS_MSASSISTED,
	TAPI_GPS_OPTIMAL_SPEED,
	TAPI_GPS_OPTIMAL_ACCURACY,
	TAPI_GPS_OPTIMAL_DATA,
	TAPI_GPS_REF_POSITION
} tapi_gps_session_operation_t;

typedef enum {
	TAPI_GPS_SERVER_DEFAULT,
	TAPI_GPS_SERVER_LOCAL
} tapi_gps_server_option_t;

typedef enum {
	TAPI_GPS_SERVER_UMTS,
	TAPI_GPS_SERVER_1X_PDE,
	TAPI_GPS_SERVER_1X_MPC
} tapi_gps_server_type_t;

typedef enum {
	TAPI_GPS_ADDR_IPV4,
	TAPI_GPS_ADDR_URL,
	TAPI_GPS_ADDR_IPV6
} tapi_gps_server_addr_type_t;

typedef enum {
	TAPI_GPS_PDSM_PD_EVENT_POSITION		= 0x00000001,
	TAPI_GPS_PDSM_PD_EVENT_VELOCITY		= 0x00000002,
	TAPI_GPS_PDSM_PD_EVENT_HEIGHT			= 0x00000004,
	TAPI_GPS_PDSM_PD_EVENT_DONE			= 0x00000008,
	TAPI_GPS_PDSM_PD_EVENT_END				= 0x00000010,
	TAPI_GPS_PDSM_PD_EVENT_BEGIN			= 0x00000020,
	TAPI_GPS_PDSM_PD_EVENT_COMM_BEGIN		= 0x00000040,
	TAPI_GPS_PDSM_PD_EVENT_COMM_CONNECTED	= 0x00000080,
	TAPI_GPS_PDSM_PD_EVENT_COMM_FAILURE	= 0x00000100,
	TAPI_GPS_PDSM_PD_EVENT_COMM_DONE		= 0x00000200,
	TAPI_GPS_PDSM_PD_EVENT_WARM_PROC_BEGIN  = 0x00000400,
	TAPI_GPS_PDSM_PD_EVENT_WARM			= 0x00000800,
	TAPI_GPS_PDSM_PD_EVENT_NOT_WARM		= 0x00001000,
	TAPI_GPS_PDSM_PD_EVENT_WARM_PROC_DONE	= 0x00002000,
	TAPI_GPS_PDSM_PD_EVENT_GPS_BEGIN		= 0x00004000,
	TAPI_GPS_PDSM_PD_EVENT_GPS_DONE		= 0x00008000,
	TAPI_GPS_PDSM_PD_EVENT_SECURITY		= 0x00100000,
	TAPI_GPS_PDSM_PD_EVENT_JGPS_PROGRESS	= 0x00200000,
	TAPI_GPS_PDSM_PD_EVENT_UPDATE_FAILURE	= 0x01000000,
	TAPI_GPS_PDSM_PD_EVENT_DLOAD_BEGIN		= 0x10000000,
	TAPI_GPS_PDSM_PD_EVENT_DLOAD			= 0x20000000,
	TAPI_GPS_PDSM_PD_EVENT_DLOAD_DONE		= 0x40000000,
	TAPI_GPS_PDSM_PD_EVENT_DLOAD_END		= 0x80000000
} tapi_gps_pd_event_t;

typedef enum {
	TAPI_GPS_END_PHONE_OFFLINE,
	TAPI_GPS_END_NO_SERVICE,
	TAPI_GPS_END_NO_CONNECTION,			/* no connection with PDE */
	TAPI_GPS_END_NO_DATA_AVAILABLE,
	TAPI_GPS_END_SESSION_MANAGER_BUSY,
	TAPI_GPS_END_CDMA_LOCKED,			/* phone is CDMA locked. */
	TAPI_GPS_END_GPS_LOCKED,			/* phone is GPS locked. */
	TAPI_GPS_END_CONNECTION_FAILURE,	/* connection failure with PDE */
	TAPI_GPS_END_BY_PDSM,				/* PDMS ended the session because of error condition. */
	TAPI_GPS_END_BY_USER,				/* User ended the session. */
	TAPI_GPS_END_BY_END_KEY,			/* End key pressed from UI */
	TAPI_GPS_END_NETWORK_SESSION_ENDED,
	TAPI_GPS_END_TIMEOUT,
	TAPI_GPS_END_CONFLICT,
	TAPI_GPS_END_NETWORK_CONNECTION_FAIL,
	TAPI_GPS_END_ERROR_IN_FIX,
	TAPI_GPS_END_REJECTED,
	TAPI_GPS_END_BY_TC_EXIT,
	TAPI_GPS_END_BY_E911_CALL,
	TAPI_GPS_END_ADDED_PROTOCOL_ERROR,
	TAPI_GPS_END_STALE_BS_INFO,
	TAPI_GPS_END_VX_LCS_AGENT_AUTH_FAIL,
	TAPI_GPS_END_UNKNOWN_SYSTEM_ERROR,
	TAPI_GPS_END_UNKNOWN_SERVICE_ERROR,
	TAPI_GPS_END_UNSUPPORTED_SERVICE,
	TAPI_GPS_END_SUBSCRIPTION_VIOLATION,
	TAPI_GPS_END_FIX_METHOD_FIALED,
	TAPI_GPS_END_ANTENNA_SWITCH,
	TAPI_GPS_END_NO_FIX_REPORTED,
	TAPI_GPS_END_NORMAL_ENDING,
	TAPI_GPS_END_NO_ERR_SPECIFIED_BY_NETWORK,
	TAPI_GPS_END_NO_RESOURCES_ON_NETWORK,
	TAPI_GPS_END_POSITION_SERVER_UNAVAILABLE,
	TAPI_GPS_END_UNSUPPORTED_PROTOCOL,
	TAPI_GPS_END_SS_MOLR_ERROR,
	TAPI_GPS_END_SS_UNEXPECT_DATA,
	TAPI_GPS_END_SS_DATA_MISSING,
	TAPI_GPS_END_SS_NO_FACILITY,
	TAPI_GPS_END_SS_SUBSCRPIPTION_VIOLATION,
	TAPI_GPS_END_SS_POSITION_METHOD_FAIL,
	TAPI_GPS_END_SS_UNDEFINED,
	TAPI_GPS_END_CP_SMLC_TIMEOUT,
	TAPI_GPS_END_CP_MT_GUARDTIME_EXPIRATION,
	TAPI_GPS_END_WAITING_ASSISTANCE
} tapi_gps_end_status_code;

typedef enum {
	TAPI_GPS_MASK_ALTITUDE_VALID			= 0x01,
	TAPI_GPS_MASK_VELOCITY_VALID			= 0x02,
	TAPI_GPS_MASK_UTC_OFFSET_VALID			= 0x04,
	TAPI_GPS_MASK_UNCERTAINTY_VAILD			= 0x08,
	TAPI_GPS_MASK_EXT_STATUS_VALID			= 0x10,
	TAPI_GPS_MASK_MILLISECOND_VALID			= 0x20,
	TAPI_GPS_MASK_PM_DATA_AVAILABLE			= 0x40,
	TAPI_GPS_MASK_ADDITIONAL_MEASUREMENT	= 0x80
} tapi_gps_opt_field_mask_t;

typedef enum {
	TAPI_GPS_POS_SRC_GPS		= 0x01,
	TAPI_GPS_POS_SRC_CELLID		= 0x02,
	TAPI_GPS_POS_SRC_AFLT		= 0x04,
	TAPI_GPS_POS_SRC_HYBRID		= 0x08,
	TAPI_GPS_POS_SRC_CELL_ERROR	= 0x10,
	TAPI_GPS_POS_SRC_DEFAULT	= 0x20,
	TAPI_GPS_POS_SRC_UNKNOWN	= 0x40
} tapi_gps_pos_source_type_t;

typedef enum {
	TAPI_GPS_POS_TYPE_INVAILD,
	TAPI_GPS_POS_TYPE_OLD,
	TAPI_GPS_POS_TYPE_NEW
} tapi_gps_pos_type_t;

typedef enum {
	TAPI_GPS_POS_MODE_MSBASED			= 0x01,
	TAPI_GPS_POS_MODE_MSASSISTED,
	TAPI_GPS_POS_MODE_STANDALONE,
	TAPI_GPS_POS_MODE_UNKNOWN,
	TAPI_GPS_POS_MODE_2G_CP_MS_BASED	= 0x08,
	TAPI_GPS_POS_MODE_2G_CP_MS_ASSISTED	= 0x10,
	TAPI_GPS_POS_MODE_3G_CP_UE_BASED	= 0x20,
	TAPI_GPS_POS_MODE_3G_CP_UE_ASSISTED	= 0x40
} tapi_gps_pos_mode_t;

typedef enum {
	TAPI_GPS_FIX_TYPE_UNKNOWN,
	TAPI_GPS_FIX_TYPE_2D,
	TAPI_GPS_FIX_TYPE_3D
} tapi_gps_fix_type_t;

typedef enum {
	TAPI_GPS_FIX_SELECTION_UNKNOWN,
	TAPI_GPS_FIX_SELECTION_AUTO,
	TAPI_GPS_FIX_SELECTION_MANUAL
} tapi_gps_fix_selection_type_t;

typedef enum {
	TAPI_GPS_FIX_QUALITY_UNKNOWN,
	TAPI_GPS_FIX_QUALITY_GPS,
	TAPI_GPS_FIX_QUALITY_DGPS
} tapi_gps_fix_quality_t;

typedef enum {
	TAPI_GPS_ENCRYPTION_DISABLED,
	TAPI_GPS_ENCRYPTION_AES128
} tapi_gps_encryption_algorithm_t;

typedef enum {
	TAPI_GPS_SV_STATE_IDLE,
	TAPI_GPS_SV_STATE_SEARCH,
	TAPI_GPS_SV_STATE_SEARCH_VERIFY,
	TAPI_GPS_SV_STATE_BIT_EDGE,
	TAPI_GPS_SV_STATE_TRACK
} tapi_gps_sv_state_t;

typedef enum {
	TAPI_GPS_GPSLOCK_NONE,
	TAPI_GPS_GPSLOCK_MI,
	TAPI_GPS_GPSLOCK_MT,
	TAPI_GPS_GPSLOCK_ALL
} tapi_gps_gpslock_mode_t;

typedef enum {
	TAPI_GPS_EPHEMERIS				= 0x00000001,
	TAPI_GPS_ALMANAC				= 0x00000002,
	TAPI_GPS_POSITION				= 0x00000004,
	TAPI_GPS_TIME					= 0x00000008,
	TAPI_GPS_IONO					= 0x00000010,
	TAPI_GPS_UTC					= 0x00000020,
	TAPI_GPS_HEALTH				= 0x00000040,
	TAPI_GPS_SVDIR					= 0x00000080,
	TAPI_GPS_SVSTEER				= 0x00000100,
	TAPI_GPS_SADATA				= 0x00000200,
	TAPI_GPS_RTI					= 0x00000400,
	TAPI_GPS_TIME_UNCERTAINTY		= 0x00000800,
	TAPI_GPS_POSITION_UNCERTAINTY	= 0x00001000,
	TAPI_GPS_TIME_OFFSET			= 0x00002000,
	TAPI_GPS_POSITION_OFFSET		= 0x00004000,
	TAPI_GPS_ALMANAC_CORP			= 0x00008000,
	TAPI_GPS_FREQ_BIAS				= 0x00010000
} tapi_gps_aiding_data_t;

typedef enum {
	TAPI_GPS_MT_LR_DISABLE,
	TAPI_GPS_MT_LR_ENABLE
} tapi_gps_mt_lr_t;

typedef enum {
	TAPI_GPS_MO_CP,
	TAPI_GPS_MO_UP,
} tapi_gps_mo_method_t;

typedef enum {
	TAPI_GPS_SSL_DISABLE,
	TAPI_GPS_SSL_ENABLE
} tapi_gps_ssl_on_off_t;

typedef enum {
	TAPI_GPS_SSL_VERISIGN				= 0x00,	/* 0x00: Orange France/UK/Spain, SFR, TIM, Telstra */
	TAPI_GPS_SSL_THAWTE				= 0x01,	/* 0x01: Vodafone Global */
	TAPI_GPS_SSL_CMCC_OFFICIAL			= 0x02,	/* 0x02: CMCC OFFICIAL */
	TAPI_GPS_SSL_SPIRENT_TEST_EQUIPMENT = 0x03,	/* 0x03: Spirent Equipment */
	TAPI_GPS_SSL_THALES_TEST_SERVER	= 0x04,	/* 0x04: THALES IOT TEST  */
	TAPI_GPS_SSL_CMCC_TEST_LAB			= 0x05,	/* 0x05: CMCC test lab */
	TAPI_GPS_SSL_C3_PCA_G3V2			= 0x06,	/* 0x06: DocoMo Test 1 */
	TAPI_GPS_SSL_CLASS3_PCA_G2_V2		= 0x07,	/* 0x07: DocoMo Test 2 */
	TAPI_GPS_SSL_GOOGLE_CA				= 0x08,	/* 0x08: GOOGLE Server */
} tapi_gps_cert_type_t;

typedef enum {
	TAPI_GPS_PRM_MSG_SERVER_TYPE		 = 0x01,
	TAPI_GPS_PRM_MSG_SERVER_ADDR_TYPE,
	TAPI_GPS_PRM_MSG_SERVER_ADDR_IP,
	TAPI_GPS_PRM_MSG_SERVER_PORT,
	TAPI_GPS_PRM_MSG_SERVER_URL,
	TAPI_GPS_PRM_MSG_GPS_LOCK,
	TAPI_GPS_PRM_MSG_DEL_AIDING_DATA,
	TAPI_GPS_PRM_MSG_POSITION_MODE,
	TAPI_GPS_PRM_MSG_MT_LR,
	TAPI_GPS_PRM_MSG_MO_METHOD,
	TAPI_GPS_PRM_MSG_SSD,
	TAPI_GPS_PRM_MSG_SEC_UPDATE_RATE,
	TAPI_GPS_PRM_MSG_SSL_ON_OFF,
	TAPI_GPS_PRM_MSG_SSL,
} tapi_gps_param_msg_param_type_t;

typedef enum {
	TAPI_GPS_PD_CMD_GET_POSITION,
	TAPI_GPS_PD_CMD_END_SESSION,
	TAPI_GPS_PD_CMD_INJECT_TIME
} tapi_gps_pd_cmd_t;

typedef enum {
	TAPI_GPS_PDP_CONNECT,
	TAPI_GPS_PDP_DISCONNECT
} tapi_gps_pdp_connection_t;

typedef enum {
	TAPI_GPS_PDP_TYPE_IP,
	TAPI_GPS_PDP_TYPE_PPP,
	TAPI_GPS_PDP_TYPE_IPV6
} tapi_gps_pdp_type_t;

typedef enum {
	TAPI_GPS_PDP_CONNECT_SUCESS,
	TAPI_GPS_PDP_CONNECT_FAIL,
	TAPI_GPS_PDP_DISCONNECT_SUCCESS,
	TAPI_GPS_PDP_DISCONNECT_FAIL
} tapi_gps_pdp_result_t;

typedef enum {
	TAPI_GPS_DNS_LOOKUP_SUCCESS,
	TAPI_GPS_DNS_LOOKUP_FAIL
} tapi_gps_dns_lookup_result_t;

typedef enum {
	TAPI_GPS_IP_ADDR_TYPE_IPV4,
	TAPI_GPS_IP_ADDR_TYPE_IPV6,
} tapi_gps_ip_addr_type_t;

typedef enum {
	TAPI_GPS_NOTIFY_ONLY,
	TAPI_GPS_NOTIFY_ALLOW_NORESPONSE,
	TAPI_GPS_NOTIFY_NOTALLOW_NORESPONSE
} tapi_gps_verification_notify_t;

typedef enum {
	TAPI_GPS_SUPL_FORMAT_LOGICAL_NAME,
	TAPI_GPS_SUPL_FORMAT_EMAIL_ADDRESS,
	TAPI_GPS_SUPL_FORMAT_MSISDN,
	TAPI_GPS_SUPL_FORMAT_URL,
	TAPI_GPS_SUPL_FORMAT_SIP_URL,
	TAPI_GPS_SUPL_FORMAT_MIN,
	TAPI_GPS_SUPL_FORMAT_MDN,
	TAPI_GPS_SUPL_FORMAT_UNKNOWN
} tapi_gps_supl_format_t;

typedef enum {
	TAPI_GPS_DCS_ISO646IRV,
	TAPI_GPS_DCS_ISO8859,
	TAPI_GPS_DCS_UTF8,
	TAPI_GPS_DCS_UTF16,
	TAPI_GPS_DCS_UCS2,
	TAPI_GPS_DCS_GSM_DEFAULT,
	TAPI_GPS_DCS_SHIFT_JIS,
	TAPI_GPS_DCS_JIS,
	TAPI_GPS_DCS_EUC,
	TAPI_GPS_DCS_GB2312,
	TAPI_GPS_DCS_CNS11643,
	TAPI_GPS_DCS_KSC1001,
	TAPI_GPS_DCS_GERMAN,
	TAPI_GPS_DCS_ENGLISH,
	TAPI_GPS_DCS_ITALIAN,
	TAPI_GPS_DCS_FRENCH,
	TAPI_GPS_DCS_SPANISH,
	TAPI_GPS_DCS_DUTCH,
	TAPI_GPS_DCS_SWEDISH,
	TAPI_GPS_DCS_DANISH,
	TAPI_GPS_DCS_PORTUGUESE,
	TAPI_GPS_DCS_FINNISH,
	TAPI_GPS_DCS_NORWEGIAN,
	TAPI_GPS_DCS_GREEK,
	TAPI_GPS_DCS_TURKISH,
	TAPI_GPS_DCS_HUNGARIAN,
	TAPI_GPS_DCS_POLISH,
	TAPI_GPS_DCS_LANGUAGE_UNSPEC
} tapi_gps_data_coding_scheme_t;

typedef enum {
	TAPI_GPS_RIE_OCTET,
	TAPI_GPS_RIE_EXN_PROTOCOL_MSG,
	TAPI_GPS_RIE_ASCII,
	TAPI_GPS_RIE_IA5,
	TAPI_GPS_RIE_UNICODE,
	TAPI_GPS_RIE_SHIFT_JIS,
	TAPI_GPS_RIE_KOREAN,
	TAPI_GPS_RIE_LATIN_HEBREW,
	TAPI_GPS_RIE_LATIN,
	TAPI_GPS_RIE_GSM
} tapi_gps_requestor_id_encoding_t;

typedef enum {
	TAPI_GPS_VER_CNF_YES = 0x00,	/**< Specifies Confirmation yes. */
	TAPI_GPS_VER_CNF_NO = 0x01,	/**< Specifies Confirmation no. */
	TAPI_GPS_VER_CNF_NORESPONSE = 0x02, /**< Specifies Confirmation no response. */
} tapi_gps_verification_cnf_t;

typedef enum {
	TAPI_GPS_SUPL_INIT			= 0x01,
	TAPI_GPS_SUPL_START,
	TAPI_GPS_SUPL_RESPONSE,
	TAPI_GPS_SUPL_POS_INIT,
	TAPI_GPS_SUPL_POS,
	TAPI_GPS_SUPL_END,
	TAPI_GPS_SUPL_AUTH_REQ,
	TAPI_GPS_SUPL_AUTH_RESP,
	TAPI_GPS_SUPL_TLS_FAILURE,
	TAPI_GPS_SUPL_FALLBACK_TO_STANDALONE,
	TAPI_GPS_SUPL_RESPONSE_TIMEOUT,
	TAPI_GPS_SUPL_OTA_MSG_TIMEOUT
} tapi_gps_supl_msg_t;

typedef enum {
	//[Error Indicator]
	TAPI_GPS_ERR_IND_UNSPECIFIED				 = 0x01,
	TAPI_GPS_ERR_IND_SYSTEM_FAILURE,
	TAPI_GPS_ERR_IND_PROTOCOL_ERR,
	TAPI_GPS_ERR_IND_DATA_MISSING,
	TAPI_GPS_ERR_IND_UNEXPECTED_DATA_VALUE,
	TAPI_GPS_ERR_IND_POS_METHOD_FAILURE,
	TAPI_GPS_ERR_IND_POS_METHOD_MISMATCH,
	TAPI_GPS_ERR_IND_POS_PROTOCOL_MISMATCH,
	TAPI_GPS_ERR_IND_TARGETSET_NOT_REACHABLE,
	TAPI_GPS_ERR_IND_VERSION_NOTSUPPORTED,
	TAPI_GPS_ERR_IND_RESOURCE_SHORTAGE,
	TAPI_GPS_ERR_IND_INVALID_SESSION_ID,
	//[Unexpected Message]
	TAPI_GPS_UNEXP_MSG_NONPROXYMODE_NOTSUPPORTED,
	TAPI_GPS_UNEXP_MSG_PROXYMODE_NOTSUPPORTED,
	TAPI_GPS_UNEXP_MSG_POS_NOT_PERMITTED,
	TAPI_GPS_UNEXP_MSG_AUTH_NET_FAILURE,
	TAPI_GPS_UNEXP_MSG_AUTH_SUPLINIT_FAILURE,
	//[Information Indicator]
	TAPI_GPS_INFO_IND_CONSENT_DENIED_BY_USER	= 0x65,
	TAPI_GPS_INFO_IND_CONSENT_GRANTED_BY_USER,
	TAPI_GPS_ERROR_NONE						= 0xFF
} tapi_gps_supl_status_t;

typedef enum {
	TAPI_GPS_XTRA_STATUS_FAIL,
	TAPI_GPS_XTRA_STATUS_SUCCESS
} tapi_gps_xtra_status_t;

/**
 * @enum tapi_gps_error_code_t
 * This enumeration defines error types of telephony gps.
 */
typedef enum {
	TAPI_GPS_ERR_NONE						= 0,	/**< Error None	*/
	TAPI_GPS_ERR_INIT_FAILED,						/**< Session opening failed	*/
	TAPI_GPS_ERR_DEINIT_FAILED,						/**< Option setting failed	*/
	TAPI_GPS_ERR_FIX_REQ_FAILED,					/**< Session starting failed */
	TAPI_GPS_ERR_STOP_SESSION_FAILED,				/**< Serial opening failed	*/
	TAPI_GPS_ERR_PARAMETER_SET_FAILED,				/**< Creating DPRAM thread failed */
	TAPI_GPS_ERR_XTRA_INIT_FAILED,					/**< Session Closing failed	*/
	TAPI_GPS_ERR_XTRA_DEINIT_FAILED,				/**< XTRA enable/disable setting failed	*/
	TAPI_GPS_ERR_XTRA_ENABLE_FAILED,				/**< XTRA auto download enable/disable setting failed */
	TAPI_GPS_ERR_XTRA_SET_TIME_INFO_FAILED,			/**< SNTP time info injection failed */
	TAPI_GPS_ERR_XTRA_SET_DATA_INFO_FAILED,			/**< Query validity of XTRA data failed	*/
	TAPI_GPS_ERR_AGPS_SSL_SET_FAILED,				/**< SSL security setting failed */
	TAPI_GPS_ERR_AGPS_BEARER_SET_FAILED,			/**< AGPS bearer setting failed	*/
} tapi_gps_error_code_t;


/*****************************************************************************************************/
/*                                    GPS Chipset on AP - START                                      */
/*****************************************************************************************************/

//GPS CP MO Location

/* MOLR Type */
typedef enum {
	TAPI_GPS_MOLR_INVALID,
	TAPI_GPS_MOLR_LOCATION_ESTIMATE,
	TAPI_GPS_MOLR_ASSISTANCE_DATA,
	TAPI_GPS_MOLR_DECIPHERING_KEY
} tapi_gps_molr_e_type;

/* Response Time Type */
typedef enum {
	TAPI_GPS_RESPONSE_TIME_INVALID,
	TAPI_GPS_RESPONSE_TIME_LOW_DELAY,
	TAPI_GPS_RESPONSE_TIME_DELAY_TOLERANT
} tapi_gps_response_time;

/* Location Method Type */
typedef enum {
	TAPI_GPS_LOCATION_METHOD_INVALID,
	TAPI_GPS_LOCATION_METHOD_MSBASED_EOTD,
	TAPI_GPS_LOCATION_METHOD_MSASSISTED_EOTD,
	TAPI_GPS_LOCATION_METHOD_ASSISTED_GPS
} tapi_gps_location_method_e_type;

/* GAD Shape Type */
typedef enum {
	TAPI_GPS_SHAPE_ELLIPSOIDPOINT,
	TAPI_GPS_SHAPE_ELLIPSOIDPOINT_WITH_UNCERTAINTY_CIRCLE,
	TAPI_GPS_SHAPE_ELLIPSOIDPOINT_WITH_UNCERTAINTY_ELLIPSE,
	TAPI_GPS_SHAPE_POLYGON,
	TAPI_GPS_SHAPE_ELLIPSOIDPOINT_WITH_ALTITUDE,
	TAPI_GPS_SHAPE_ELLIPSOIDPOINT_WITH_ALTITUDE_AND_UNCERTAINTYELI,
	TAPI_GPS_SHAPE_ELLIPSOIDPOINT_ARC
} tapi_gps_gad_shape_e_type;

/* GPS Assistance Data Flags */
#define GPS_ASSISTANCE_ALMANAC                  0x00000001
#define GPS_ASSISTANCE_UTC_MODEL                0x00000002
#define GPS_ASSISTANCE_IONO_MODEL               0x00000004
#define GPS_ASSISTANCE_NAVI_MODEL               0x00000008
#define GPS_ASSISTANCE_DGPS_CORRECTION          0x00000010
#define GPS_ASSISTANCE_REF_LOCATION             0x00000040
#define GPS_ASSISTANCE_REF_TIME                 0x00000080
#define GPS_ASSISTANCE_ACQUISITION_ASSISTANCE   0x00000100
#define GPS_ASSISTANCE_REAL_TIME_INTEGRITY      0x00000200

//GPS Assist Data Message

/* GPS Crrier Type */
typedef enum {
	TAPI_GPS_CARRIER_INVALID,
	TAPI_GPS_CARRIER_GSM,
	TAPI_GPS_CARRIER_UMTS
} tapi_gps_carrier_e_type;

/* Utran Sfn Uncertainty */
typedef enum {
	TAPI_GPS_UTRAN_SFN_UNCERTAINTY_INVALID,
	TAPI_GPS_UTRAN_SFN_UNCERTAINTY_LESSTHAN_10,
	TAPI_GPS_UTRAN_SFN_UNCERTAINTY_MORETHAN_10
} tapi_gps_sfn_unc_e_type;

/* UTRAN Choice mode Type */
typedef enum {
	TAPI_GPS_UTRAN_CHOICE_INVALID,
	TAPI_GPS_UTRAN_CHOICE_FDD,
	TAPI_GPS_UTRAN_CHOICE_TDD
} tapi_gps_utrn_choice_mode_e_type;

/* DGPS Status Type */
typedef enum {
	TAPI_GPS_DGPS_INVALID,
	TAPI_GPS_DGPS_UDRE_SCALE_1_0,
	TAPI_GPS_DGPS_UDRE_SCALE_0_75,
	TAPI_GPS_DGPS_UDRE_SCALE_0_5,
	TAPI_GPS_DGPS_UDRE_SCALE_0_3,
	TAPI_GPS_DGPS_UDRE_SCALE_0_2,
	TAPI_GPS_DGPS_UDRE_SCALE_0_1,
	TAPI_GPS_DGPS_NO_DATA
} tapi_gps_dgps_status_e_type;

/* Navigation Satellite Status Type*/
typedef enum {
	TAPI_GPS_NAVIGATION_MODEL_NEW_SATELLITE_NEW_NAVIGATION,
	TAPI_GPS_NAVIGATION_MODEL_EXIST_SATELLITE_EXIST_NAVIGATION,
	TAPI_GPS_NAVIGATION_MODEL_EXIST_SATELLITE_NEW_NAVIGATION,
	TAPI_GPS_NAVIGATION_MODEL_RESERVED
} tapi_gps_navigation_sat_status_e_type;

//GPS Measure Position Message
/* Method Type */
typedef enum {
	TAPI_GPS_METHODTYPE_INVALID,
	TAPI_GPS_METHODTYPE_MS_ASSISTED,
	TAPI_GPS_METHODTYPE_MS_BASED,
	TAPI_GPS_METHODTYPE_MS_BASED_PREF,
	TAPI_GPS_METHODTYPE_MS_ASSISTED_PREF
} tapi_gps_method_e_type;

/* Use Multiple Sets Type */
typedef enum {
	TAPI_GPS_MULTIPLESETS_INVALID,
	TAPI_GPS_MULTIPLESETS_MULTIPLESETS,
	TAPI_GPS_MULTIPLESETS_ONESET
} tapi_gps_use_multi_sets_e_type;

/* Environment Char Type */
typedef enum {
	TAPI_GPS_ENVIRONMENT_INVALID,
	TAPI_GPS_ENVIRONMENT_BAD_AREA,
	TAPI_GPS_ENVIRONMENT_NOT_BAD_AREA,
	TAPI_GPS_ENVIRONMENT_MIXED_AREA
} tapi_gps_env_char_e_type;

/* Cell Timing Wanted Type */
typedef enum {
	TAPI_GPS_CELLTIMING_INVALID,
	TAPI_GPS_CELLTIMING_WANTED,
	TAPI_GPS_CELLTIMING_NOT_WANTED
} tapi_gps_cell_timing_wnt_e_type;

/* Additional Assist Request Type */
typedef enum {
	TAPI_GPS_ADDITIONAL_ASSISREQ_INVALID,
	TAPI_GPS_ADDITIONAL_ASSISREQ_REQ,
	TAPI_GPS_ADDITIONAL_ASSISREQ_NOT_REQ
} tapi_gps_add_assit_req_e_type;

/* Measure Position Response Type */
typedef enum {
	TAPI_GPS_MSR_POS_RES_LOCATION,
	TAPI_GPS_MSR_POS_RES_GPS_MEASUREMENTS,
	TAPI_GPS_MSR_POS_RES_AID_REQ,
	TAPI_GPS_MSR_POS_RES_ERROR
} tapi_gps_msr_pos_res_e_type;

/* GPS element multi path Type */
typedef enum {
	TAPI_GPS_MULTIPATH_NOT_MEASURED,
	TAPI_GPS_MULTIPATH_LOW,
	TAPI_GPS_MULTIPATH_MEDIUM,
	TAPI_GPS_MULTIPATH_HIGH
}tapi_gps_multi_path_e_type;

/* Accuracy Flag Value */
#define QOS_PRESENT_HORIZONTAL_ACCURACY 0x01
#define QOS_PRESENT_VERTICAL_ACCURACY   0x02

//GPS MTLR Notification Message
/* MTLR Notification Type */
typedef enum {
	TAPI_GPS_NO_NOTIFY_NO_VERIFY,
	TAPI_GPS_USER_NOTIFY_ONLY,     /* Location Notification Allowed */
	TAPI_GPS_USER_NOTIFY_VERIFY_ALLOW_NO_RESP, /* notify and verify, if no response, Location Notification Allowed */
	TAPI_GPS_USER_NOTIFY_VERIFY_NOT_ALLOW_NO_RESP, /* notify and verify, if no response, Location Notification Not Allowed */
	TAPI_GPS_PRIVACY_NEEDED,
	TAPI_GPS_PRIVACY_OVERRIDE
} tapi_gps_mtlr_notify_e_type;

/* location estimate Type */
typedef enum {
	TAPI_GPS_ESTIMATE_TYPE_CURRENT_LOCATION,	///< Current Location
	TAPI_GPS_ESTIMATE_TYPE_CURRENT_OR_LAST_KNOWN_LOCATION,	///< Current or Last Known Location
	TAPI_GPS_ESTIMATE_TYPE_INITIAL_LOCATION,		///< Initial Location
	TAPI_GPS_ESTIMATE_TYPE_ACTIVATE_DEFERRED_LOC,	///< Activate Deferred Location
	TAPI_GPS_ESTIMATE_TYPE_CANCEL_DEFERRED_LOC,	///< Cancel Deferred Location
	TAPI_GPS_ESTIMATE_TYPE_NOTIFY_VERIFY_ONLY,		///< Notification and Verification Only
	TAPI_GPS_ESTIMATE_TYPE_INVALID
} tapi_gps_loc_estimate_e_type;

/* ID Format Type */
typedef enum {
	TAPI_GPS_FORMAT_IND_LOGICAL_NAME,
	TAPI_GPS_FORMAT_IND_EMAIL_ADDRESS,
	TAPI_GPS_FORMAT_IND_MSISDN,
	TAPI_GPS_FORMAT_IND_URL,
	TAPI_GPS_FORMAT_IND_SIPURL,
	TAPI_GPS_FORMAT_IND_IMS_PUBLIC_ID
} tapi_gps_format_ind_e_type;

/* Verify Response Type */
typedef enum {
	TAPI_GPS_VERIFY_RSP_TYPE_DENIED,
	TAPI_GPS_VERIFY_RSP_TYPE_GRANTED,
	TAPI_GPS_VERIFY_RSP_TYPE_INVALID
} tapi_gps_verify_rsp_e_type;

typedef enum {
	TAPI_GPS_FREQ_AIDING_DISABLE = 0,
	TAPI_GPS_FREQ_AIDING_ENABLE,
} tapi_gps_freq_aiding_e_type;


/*****************************************************************************************************/
/*                                     GPS Chipset on AP - END                                       */
/*****************************************************************************************************/


/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/
typedef struct {
	unsigned char NumOfRecord;
	tapi_gps_fix_req_param_type_t ParamType[TAPI_GPS_FIX_PARAM_TAG_MAX];
	tapi_gps_session_type_t SessionType;
	tapi_gps_session_operation_t SessionOperation;
	int TotalFixes;
	int TimeBtwFixes;
	int Accuracy;
	unsigned char GpsSessionTimeout; /* 0~255 secs. */
	tapi_gps_server_option_t ServerOption;
	tapi_gps_server_addr_type_t ServerAddressType;
	char ServerAddressIP[TAPI_GPS_ADDR_LEN_MAX];
	int ServerPort;
	char URL[TAPI_GPS_URL_LEN_MAX];
	unsigned char SecurityDataId;
	unsigned char SecurityDataLength;
	char SecurityData[TAPI_GPS_SECURITY_DATA_MAX];
} tapi_gps_fix_request_msg_t;

typedef struct {
	unsigned char SpaceVehicleNumber;
	unsigned char ElevationOfMeasuredSatellite;
	short AzimuthOfMeasuredSatellite;
} tapi_gps_sv_info_inview_t;

typedef struct {
	int sv_num;
	int elev;
	int azimuth_snr;
} tapi_sv_in_view_type_t;

typedef struct {
	tapi_gps_pd_event_t pd_evnet;
	tapi_gps_end_status_code end_status;
	tapi_gps_opt_field_mask_t opt_field_mask;
	long time_stamp;
	int utc_offset;
	tapi_gps_pos_source_type_t positioning_source;
	tapi_gps_pos_type_t position_type;
	tapi_gps_pos_mode_t position_mode;
	int loc_uncertainty_ang;
	int loc_uncertainty_a;
	int loc_uncertainty_p;
	int loc_uncertainty_v;
	int loc_uncertainty_conf;
	long long latitude;
	long long longitude;
	int altitude;
	int speed_knot;
	int speed_kmh;
	int heading_true;
	int velocity_include;
	tapi_gps_fix_type_t fix_type;
	int hour;
	int minute;
	int sec;
	int date;
	int h_dop;
	int p_dop;
	int v_dop;
	int sv_in_use;
	long long magnetic_variation;
	tapi_gps_fix_selection_type_t selection_type;
	tapi_gps_fix_quality_t fix_quality;
	int num_sv_in_view;
	tapi_sv_in_view_type_t sv[TAPI_GPS_MAX_SATELLITES];
	int hash_alg;
	int sec_data_id;
	int sec_data_len;
	char sec_data[20];
	tapi_gps_encryption_algorithm_t crypt_alg;
	int crypt_data_len;
	char crypt_data[16];
} tapi_gps_position_data_noti_t;

typedef struct {
	int sv_id;
	int c_no;
	int latency_ms;
	int azimuth;
	int elevation;
	tapi_gps_sv_state_t sv_state;
} tapi_meas_report_type_t;

typedef struct {
	int num_svs;
	int eph_svmask;
	int alm_svmask;
	tapi_meas_report_type_t meas_report[16];
} tapi_gps_ext_measurement_noti_t;

typedef struct {
	unsigned char NumOfRecord;
	tapi_gps_param_msg_param_type_t ParamType[TAPI_GPS_PARAMETERS_TAG_MAX];
	tapi_gps_server_type_t ServerType;
	tapi_gps_server_addr_type_t ServerAddressType;
	char ServerAddressIP[TAPI_GPS_ADDR_LEN_MAX];
	int ServerPort;
	char ServerURL[TAPI_GPS_URL_LEN_MAX];
	tapi_gps_gpslock_mode_t GpsLock;
	int DelAidingData;
	tapi_gps_pos_mode_t PositionMode;
	tapi_gps_mt_lr_t MT_LR;
	tapi_gps_mo_method_t MO_Method;
	unsigned char SharedSecretData[TAPI_GPS_SSD_LEN_MAX];
	int SecurityUpdateRate;
	tapi_gps_ssl_on_off_t SslOnOff;
	tapi_gps_cert_type_t SSL;
} tapi_gps_param_t;

typedef struct {
	tapi_gps_pdp_result_t Result;
	unsigned char APN[TAPI_GPS_APN_LEN_MAX];
} tapi_gps_data_conn_cnf_t;

typedef struct {
	tapi_gps_pdp_connection_t Connection;
	tapi_gps_pdp_type_t PdpType;
} tapi_gps_data_conn_indi_t;

typedef struct {
	tapi_gps_dns_lookup_result_t Result;
	tapi_gps_ip_addr_type_t IP_Type;
	unsigned char IP_Address[TAPI_GPS_ADDR_LEN_MAX];
	int port;
} tapi_gps_dns_lookup_cnf_t;

typedef struct {
	int len_addr;
	char url[TAPI_GPS_DNS_LEN_MAX];
} tapi_gps_dns_lookup_indi_t;

typedef struct {
	tapi_gps_verification_notify_t notify_type; /**< Specifies Notify type refer enum tapi_gps_notify_type_t */
	tapi_gps_supl_format_t supl_format;
	tapi_gps_data_coding_scheme_t dcs;
	tapi_gps_requestor_id_encoding_t requestor_id_encoding; /**< Specifies encoding type refer enum tapi_gps_encoding_type_t */
	char requestor_id[50]; /**< Specifies Client name */
	char client_name[50]; /**< Specifies Client name */
	int resp_time;
} tapi_gps_verification_indi_t;

/**
 * This structure defines the values for agps SUPL flow parameter.
 */
typedef struct {
	tapi_gps_supl_msg_t message;
	tapi_gps_supl_status_t status_code;
} tapi_gps_suplflow_noti_t;

typedef struct {
	unsigned long long time_msec; /**< Commulative GPS Time */
	unsigned long time_unc_msec;
	unsigned char ref_toutc_Time; /**< Referenced to UTC GPS Time  False :0 True: 1 */
	unsigned char force_flag; /**< Force Acceptance of Data  False :0 True: 1 */
	unsigned char status;
} tapi_gps_xtra_time_info_t;

/**
 * This structure defines the values for gps xtra time information notification data.
 */
typedef struct {
	unsigned int oneway_delay_failover_thresh; /**< One-way delay threshold for failover backup servers */
} tapi_gps_xtra_time_noti_t;

typedef struct {
	unsigned int data_len;
	unsigned char xtra_data[TAPI_GPS_XTRA_DATA_LEN_MAX];
	unsigned char part_num;
	unsigned char total_part;
	tapi_gps_xtra_status_t status;
} tapi_gps_xtra_data_info_t;

typedef struct {
	int max_file_part_size;
	int max_file_size;
} tapi_gps_xtra_data_noti_t;

typedef struct {
	tapi_gps_pd_cmd_t pd_cmd;
	int pd_cmd_err;
} tapi_gps_pd_cmd_callback_t;

/*****************************************************************************************************/
/*                                    GPS Chipset on AP - START                                      */
/*****************************************************************************************************/

typedef struct {
	unsigned long int qosFlag;
	unsigned char horizontalAccuracy;
	unsigned char verticalCoordinateRequest;
	unsigned char verticalAccuracy;
	unsigned char responseTime; //0x01:low delay, 0x02: delay tolerant
}__attribute__ ((packed)) tapi_gps_qos_t;

typedef struct {
	unsigned char sat_id;
	unsigned char iode;
}__attribute__ ((packed)) tapi_gps_sat_info_t;

typedef struct {
	unsigned char beginWeek;
	unsigned char endWeek;
	unsigned char beginTow;
	unsigned char endTow;
}__attribute__ ((packed)) tapi_gps_ext_ephe_chk_t;

typedef struct {
	unsigned long int assistanceFlag;
	unsigned short gpsWeek;
	unsigned char gpsToe;
	unsigned char nSat;
	unsigned char toeLimit;
	tapi_gps_sat_info_t satInfo[15];
	unsigned char gpsExtendedEphemeris;
	tapi_gps_ext_ephe_chk_t extEphemerisChk;
}__attribute__ ((packed)) tapi_gps_assistance_data_t;

typedef struct {
	tapi_gps_molr_e_type molr_type;
	tapi_gps_location_method_e_type location_method;
	tapi_gps_qos_t qos;
	unsigned char client_id[82];
	unsigned char mlc_num[82];
	tapi_gps_assistance_data_t assistance_data;
	tapi_gps_gad_shape_e_type gad_shape;
	unsigned char serviceTypeID;
	unsigned char pseudonymIndicator;
}__attribute__ ((packed)) tapi_gps_cp_mo_loc_set_t; //APGPS - Control Plane Mo Location - Set

//Measure Position message
typedef struct {
	unsigned char satId; //Satellite ID
	unsigned char cno; // 0~63, unit of dB-Hz
	signed short doppler; // -32768~32767, Hz and scale factor 0.2
	unsigned short wholeChips; // 0~1022
	unsigned short fracChips; // 0~1024
	unsigned char lcsMultiPath;
	unsigned char pseuRangeRmsErr; // 0~63
}__attribute__ ((packed)) tapi_gps_measuremet_element_t;

typedef struct {
	unsigned long int gpsTow; ///< GPS time of week [msec]
	unsigned short gpsWeek; ///< GPS week [0 .. 1023]
	unsigned char nrOfSats; ///< number of satellites [1 .. 16]
	tapi_gps_measuremet_element_t GpsMeasure[16];
}__attribute__ ((packed)) tapi_gps_measure_t;

typedef struct {
	signed long int latitude;
	signed long int longitude;
}__attribute__ ((packed)) tapi_gps_ellipsoid_po_t;

typedef struct {
	tapi_gps_ellipsoid_po_t point;
	unsigned char uncertainRadius;
}__attribute__ ((packed)) tapi_gps_po_unc_circle_t;

typedef struct {
	tapi_gps_ellipsoid_po_t point;
	unsigned char semiMajorAxis;
	unsigned char semiMinorAxis;
	unsigned char orientationAngle;
	unsigned char confidence;
}__attribute__ ((packed)) tapi_gps_po_unc_ellipse_t;

typedef struct {
	tapi_gps_ellipsoid_po_t point;
	signed short altitude;
	unsigned char semiMajorAxis;
	unsigned char semiMinorAxis;
	unsigned char orientationAngle;
	unsigned char uncertainAltitude;
	unsigned char confidence;
}__attribute__ ((packed)) tapi_gps_po_alt_unc_ellipse_t;

typedef struct {
	tapi_gps_ellipsoid_po_t point;
	unsigned short innerRadius;
	unsigned char uncertainRadius;
	unsigned char offsetAngle;
	unsigned char includedAngle;
	unsigned char confidence;
}__attribute__ ((packed)) tapi_gps_ellipsoid_arc_t;

typedef struct {
	tapi_gps_ellipsoid_po_t point;
	signed short altitude;
}__attribute__ ((packed)) tapi_gps_ellipsoid_alt_t;

typedef struct {
	unsigned char noOfPoints;
	tapi_gps_ellipsoid_po_t points[15];
}__attribute__ ((packed)) tapi_gps_polygon_t;

typedef struct {
	unsigned char shape_type;
	tapi_gps_po_unc_circle_t p_unc_clrcle;
	tapi_gps_po_unc_ellipse_t p_unc_ellipse;
	tapi_gps_po_alt_unc_ellipse_t p_alt_unc_ellipse;
	tapi_gps_ellipsoid_arc_t ellipsoid_arc;
	tapi_gps_ellipsoid_po_t ellipsoid_po;
	tapi_gps_ellipsoid_alt_t ellipsoid_alt;
	tapi_gps_polygon_t polygon;
}__attribute__ ((packed)) tapi_gps_loc_info_t;

typedef struct {
	unsigned long int gpsTow; ///< GPS time of week [msec]
	unsigned short gpsWeek; ///< GPS week [0 .. 1023]
	unsigned char fixType; ///< Fix type. 2D(0x01) or 3D(0x02)
	tapi_gps_loc_info_t measured_loc_info;
}__attribute__ ((packed)) tapi_gps_measure_loc_info_t;

typedef struct {
	unsigned char valid;
	unsigned long int cellFrames;
	unsigned char choice_mode;
	unsigned long int UtranFdd; //FDD Primary Scrambling Code
	unsigned long int UtranTdd; // TDD Cell Parameter ID
	unsigned long int sfn; //SFN
}__attribute__ ((packed)) tapi_gps_utran_gps_ref_time_t;

typedef struct {
	unsigned char result; // 0x00 : SUCCESS, 0x01 : Fail
	tapi_gps_msr_pos_res_e_type response_type; //should be 4 byte
	tapi_gps_measure_t gps_measure;
	tapi_gps_measure_loc_info_t loc_info;
	tapi_gps_assistance_data_t measured_assit_data;
	tapi_gps_utran_gps_ref_time_t UtranGpsRefTime; // only for 3G
}__attribute__ ((packed)) tapi_gps_measure_position_confirm_t; //APGPS - Measure Position message - confirm

typedef struct {
	unsigned char reqId;
	tapi_gps_verify_rsp_e_type response;
}__attribute__ ((packed)) tapi_gps_mtlr_notification_confirm_t; //AGPS - MTLR Notification message - confirm


/**********************************************************************/
//RX part
/**********************************************************************/
typedef struct {
	unsigned short arfcn; //ARFCN of GSM network
	unsigned char bsic; //Base Station Identity Code
	unsigned char rx_lev; // RX signal level
	unsigned char ta; //Timing Advance
}__attribute__ ((packed)) tapi_gps_gsm_extended_radio_signal_info_t; //APGPS - GSM Extended Radio Signal Info - Resp

typedef struct {
	unsigned long int flag;
	unsigned char horizontalAccuracy;
	unsigned char vertcalAccuracy;
}__attribute__ ((packed)) tapi_gps_accuracy_t;

typedef struct {
	unsigned char method_type;
	tapi_gps_accuracy_t accuracy;
	unsigned char rsp_time;
	unsigned char use_multi_sets;
	unsigned char environment_char;
	unsigned char cell_timing_wnt;
	unsigned char add_assist_req;
}__attribute__ ((packed)) tapi_gps_measure_position_indi_t; //APGPS - Measure Position message -indication

typedef struct {
	unsigned short deferredLocEventType;
	tapi_gps_mtlr_notify_e_type locEstimateType;
}__attribute__ ((packed)) tapi_gps_mtlr_loc_t;

typedef struct {
	unsigned long int length; //Length of APDU
	unsigned char val[63]; //APDU data
}__attribute__ ((packed)) tapi_gps_string_t;

typedef struct {
	unsigned char dcs;
	tapi_gps_string_t str;
	tapi_gps_format_ind_e_type format_indicator;
}__attribute__ ((packed)) tapi_gps_dcs_string_t;

typedef struct {
	unsigned char dcs;
	tapi_gps_string_t str;
}__attribute__ ((packed)) tapi_gps_code_word_t;

typedef struct {
	unsigned char req_id;
	tapi_gps_mtlr_notify_e_type notify_type;
	tapi_gps_mtlr_loc_t loc;
	unsigned char client_id[82];
	tapi_gps_dcs_string_t client_name;
	tapi_gps_dcs_string_t requestor_id;
	tapi_gps_code_word_t code_word;
	unsigned char svc_type_id;
}__attribute__ ((packed)) tapi_gps_mtlr_notification_indi_t; //AGPS - MTLR Notification message - Indication

typedef struct {
	unsigned char cipherKeyFlag;
	unsigned char currentDecipherKey[7];
	unsigned char nextDecipherKey[7];
}__attribute__ ((packed)) tapi_gps_deciphering_keys_t;

typedef struct {
	tapi_gps_loc_info_t loc_info;
	unsigned char no_loc;
	tapi_gps_deciphering_keys_t decper_keys;
}__attribute__ ((packed)) tapi_gps_cp_mo_loc_noti_t; //APGPS - Control Plane Mo Location - Notification

typedef struct {
	unsigned char valid;
	unsigned short bcchCarrier;
	unsigned short bsic;
	unsigned long int frameNumber;
	unsigned short timeSlot;
	unsigned short bitNumber;
}__attribute__ ((packed)) tapi_gps_gsm_time_t;

typedef struct {
	unsigned char valid;
	unsigned long int gpsTimeUncertainty;
}__attribute__ ((packed)) tapi_gps_utran_gps_unc_t;

typedef struct {
	unsigned char valid;
	signed long int driftRate;
}__attribute__ ((packed)) tapi_gps_drift_rate_t;

typedef struct {
	tapi_gps_utran_gps_ref_time_t UtranGpsRefTime;
	tapi_gps_utran_gps_unc_t UtranGpsUncertainty;
	unsigned char UtranSfnUncertainty;
	tapi_gps_drift_rate_t UtranDriftRate;
}__attribute__ ((packed)) tapi_gps_utran_time_t;

typedef struct {
	unsigned short satID;
	unsigned short tlmWord;
	unsigned char antiSpoofFlag;
	unsigned char alertFlag;
	unsigned char tmlReservedBits;
}__attribute__ ((packed)) tapi_gps_gps_tow_assist_t;

typedef struct {
	unsigned long int gpsTow;
	unsigned long int gpsWeek;
	unsigned char nrOfSats;
	union {
		tapi_gps_gsm_time_t gsm_time;
		tapi_gps_utran_time_t UtranTime;
	} networkTimeInfo;
	tapi_gps_gps_tow_assist_t GpsTowAssist[12];
}__attribute__ ((packed)) tapi_gps_ref_time_t;

typedef struct {
	unsigned char shapeType;
	unsigned char hemisphere;
	unsigned short altitude;
	unsigned long int latitude;
	signed long int longitude;
	unsigned char directionOfAlt;
	unsigned char semiMajorUncert;
	unsigned char semiMinorUncert;
	unsigned char majorAxis;
	unsigned char altUncert;
	unsigned char confidence;
}__attribute__ ((packed)) tapi_gps_ref_loc_t;

typedef struct {
	unsigned char satId; //Satellite ID
	unsigned short iode;
	unsigned char udre;
	signed short pseudoRangeCor;
	signed short rangeRateCor;
}__attribute__ ((packed)) tapi_gps_dgps_sat_list_t;

typedef struct {
	unsigned long int gpsTow;
	unsigned char status;
	unsigned long int numberOfSat;
	tapi_gps_dgps_sat_list_t seqOfSatElement[16];
}__attribute__ ((packed)) tapi_gps_dgps_correction_t;

typedef struct {
	unsigned long int rsv1; // 0~838860
	unsigned long int rsv2; // 0~16777215
	unsigned long int rsv3; // 0~16777215
	unsigned long int rsv4; // 0~65535
}__attribute__ ((packed)) tapi_gps_navi_subframe_rsv_t;

typedef struct {
	unsigned char ephemCodeOnL2; // 0~3
	unsigned char ephemUra; // 0~15
	unsigned char ephemSvHealth; // 0~63
	unsigned short ephemIodc; // 0~1023
	unsigned char ephemL2PFlag; // 0~1
	tapi_gps_navi_subframe_rsv_t NavigationSubFrameRsv;
	signed char ephemTgd; // -128~127
	unsigned short ephemToc; // 0~37799
	signed char ephemAf2; // -128~12
	signed short ephemAf1; // -32768~32767
	signed long int ephemAf0; // -2097152~2097151
	signed short ephemCrs; // -32768~32767
	signed short ephemDeltaN; // -32768~32767
	signed long int ephemM0; // -2147483648~2147483647
	signed short ephemCuc; // -32768~32767
	unsigned long int ephemE; // 0~4294967295
	signed short ephemCus; // -32768~32767
	unsigned long int ephemAPowrHalf; // 0~4294967295
	unsigned short ephemToe; // 0~37799
	signed char ephemFitFlag; // 0~1
	unsigned char ephemAoda; // 0~31
	signed short ephemCic; // -32768~32767
	signed long int ephemOmegaA0; // -2147483648~2147483647
	signed short ephemCis; // -32768~32767
	signed long int ephemI0; // -2147483648~2147483647
	signed short ephemCrc; // -32768~32767
	signed long int ephemW; // -2147483648~2147483647
	signed long int ephemOmegaADot; // -8388608~8388607
	signed short ephemIDot; // -8192~8191
}__attribute__ ((packed)) tapi_gps_navi_ephe_t;

typedef struct {
	unsigned char satId;
	unsigned char NavigationSatStatus;
	tapi_gps_navi_ephe_t NavigationEphemeris;
}__attribute__ ((packed)) tapi_gps_navi_sat_info_t;

typedef struct {
	unsigned long int numberOfSat;
	tapi_gps_navi_sat_info_t NavigationSatInfo[16];
}__attribute__ ((packed)) tapi_gps_navi_model_t;

typedef struct {
	signed char alfa0; // -128~127
	signed char alfa1; // -128~127
	signed char alfa2; // -128~127
	signed char alfa3; // -128~127
	signed char beta0; // -128~127
	signed char beta1; // -128~127
	signed char beta2; // -128~127
	signed char beta3; // -128~127
}__attribute__ ((packed)) tapi_gps_iono_model_t;

typedef struct {
	signed long int utcA1; // -8388608~8388607
	signed long int utcA0; // -2147483648~2147483647
	unsigned char utcTot; // 0~255
	unsigned char utcWNt; // 0~255
	signed char utcDeltaTls; // -128~127
	unsigned char utcWNlsf; // 0~255
	signed char utcDN; // -128~127
	signed char utcDeltaTlsf; // -128~127
}__attribute__ ((packed)) tapi_gps_utc_model_t;

typedef struct {
	signed char dataId; // only for 3G, 0~3, if this value is -1, it means this value is invalid
	unsigned char satId;
	unsigned short almanacE; // 0~65536
	unsigned char almanacToa; // 0~255
	signed short almanacKsii; // -32768~3276
	signed short almanacOmegaDot; // -32768~3276
	unsigned char almanacSvHealth; // 0~255
	unsigned long int almanacAPowerHalf; // 0~16777215
	signed long int almanacOmega0; // -8388608~8388607
	signed long int almanacW; // -8388608~8388607
	signed long int almanacM0; // -8388608~8388607
	signed short almanacAf0; // -1024~1023
	signed short almanacAf1; // -1024~1023
}__attribute__ ((packed)) tapi_gps_almanac_sat_info_t;

typedef struct {
	unsigned char almanacWNa; // 0~255
	unsigned long int numberOfSat;
	tapi_gps_almanac_sat_info_t AlmanacSatInfo[64];
}__attribute__ ((packed)) tapi_gps_almanac_model_t;

typedef struct {
	tapi_gps_utran_gps_ref_time_t AcqUtranGpsRefTime;
	tapi_gps_utran_gps_unc_t AcqUtranGpsUncertainty;
}__attribute__ ((packed)) tapi_gps_acq_utran_time_t;

typedef struct {
	unsigned char satId;
	signed short doppler0; // -2048~2047 (real value is from -5120 to 5117.5 by step of 2.5)
	unsigned char doppler1; // 0~63 (real value is from -0.966 to 0.483 by step of 0.023)
	unsigned char dopplerUncertainty; // 0~7 (12.5, 25, 50, 100, 200)
	unsigned short codePhase; // 0~1022
	unsigned char intCodePhase; // 0~19
	unsigned char gpsBitNumber; // 0~3
	unsigned char codePhaseSearchWindow; // 0~15 (1023, 1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192)
	unsigned char azimuth; // 0~31, 11.25 degree resolution
	unsigned char elevation; // 0~7, 11.25 degree resolution
}__attribute__ ((packed)) tapi_gps_acq_sat_info_t;

typedef struct {
	unsigned long int gpsTow;
	union {
		tapi_gps_gsm_time_t gsm_time;
		tapi_gps_acq_utran_time_t AcqUtranTime;
	} acquisitionTimeInfo;
	unsigned long int numberOfSat;
	tapi_gps_acq_sat_info_t lcsAcquisitionSatInfo[16];
}__attribute__ ((packed)) tapi_gps_acq_assist_t;

typedef struct {
	unsigned char satId[16];
	unsigned char numOfSat;
}__attribute__ ((packed)) tapi_gps_r_time_int_t;

typedef struct {
	unsigned long int flag;
//	tapi_gps_carrier_e_type cr_type;
	tapi_gps_ref_time_t ref_time;
	tapi_gps_ref_loc_t ref_loc;
	tapi_gps_dgps_correction_t dgps_corrections;
	tapi_gps_navi_model_t navi_model;
	tapi_gps_iono_model_t iono_model;
	tapi_gps_utc_model_t utc_model;
	tapi_gps_almanac_model_t almanac;
	tapi_gps_acq_assist_t acq_assist;
	tapi_gps_r_time_int_t r_time_int;
}__attribute__ ((packed)) tapi_gps_assist_data_noti_t; //APGPS -  GPS Assist Data Message - Notification


/*****************************************************************************************************/
/*                                     GPS Chipset on AP - END                                       */
/*****************************************************************************************************/

/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

/**
 *
 * @brief  This function is invoked to initialize GPS.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_init_gps(void);

/**
 *
 * @brief  This function is invoked to deinitialize GPS.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_deinit_gps(void);

/**
 *
 * @brief  This function is invoked to set GPS prefix information.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  type
 *    - tapi_gps_fix_request_msg_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_gps_prefix_info(tapi_gps_fix_request_msg_t *type);

/**
 *
 * @brief  This function is invoked to stop GPS session.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_stop_gps_session(void);

/**
 *
 * @brief  This function is invoked to get GPS parameters.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_gps_parameters(void);

/**
 *
 * @brief  This function is invoked to set GPS parameters.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  gps_params
 *    - tapi_gps_param_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_gps_parameters(tapi_gps_param_t *gps_params);

/**
 *
 * @brief  This function is invoked to confirm GPS data connection.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  pdp_cnf
 *    - tapi_gps_data_conn_cnf_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_confirm_gps_data_connection(tapi_gps_data_conn_cnf_t *pdp_cnf);

/**
 *
 * @brief  This function is invoked to confirm GPS DNS lookup.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  dns_cof
 *    - tapi_gps_dns_lookup_cnf_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_confirm_gps_dns_lookup(tapi_gps_dns_lookup_cnf_t *dns_cof);

/**
 *
 * @brief  This function is invoked to confirm GPS verification.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  result
 *    - tapi_gps_verification_cnf_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_confirm_gps_verification(tapi_gps_verification_cnf_t result);

/**
 *
 * @brief  This function is invoked to initialize GPS XTRA.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_init_gps_xtra(void);

/**
 *
 * @brief  This function is invoked to deinit GPS XTRA.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_deinit_gps_xtra(void);

/**
 *
 * @brief  This function is invoked to enable GPS XTRA.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_enable_gps_xtra(void);

/**
 *
 * @brief  This function is invoked to disable GPS XTRA
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_disable_gps_xtra(void);

/**
 *
 * @brief  This function is invoked to set GPS XTRA time
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  time_info
 *    - tapi_gps_xtra_time_info_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_gps_xtra_time(tapi_gps_xtra_time_info_t *time_info);

/**
 *
 * @brief  This function is invoked to set GPS XTRA data.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  data_info
 *    - tapi_gps_xtra_data_info_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Refer #int for failure and error code
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_gps_xtra_data(tapi_gps_xtra_data_info_t *data_info);


int tel_confirm_gps_measure_pos(tapi_gps_measure_position_confirm_t *data);

int tel_set_gps_frequency_aiding(unsigned char state);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_GPS_H_ */

/**
 * @}
 */

