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
 * @addtogroup	GPRS_TAPI	GPRS
 * @{
 *
 * GPRS APIs allow an application to accomplish the following services: @n
 * - activate , deactivate PDP service @n
 *
 */

#ifndef	_ITAPI_PS_H_
#define _ITAPI_PS_H_

#include <TelDefines.h>
#include <TelErr.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Defining macros for PSMAN profiles */
#define TAPI_PDP_ADDR_LEN_MAX		20
#define TAPI_PDP_APN_LEN_MAX		101
#define TAPI_PDP_NAME_LEN_MAX		16
#define TAPI_PDP_MAX_USERNAME_LEN	32
#define TAPI_PDP_MAX_PASSWORD_LEN	32
#define TAPI_PDP_MAX_DNS_LEN		16
#define MAX_PDP_CONTEXTS_ALLOWED	3
#define MAX_GPRS_PORT_LIST			200

typedef enum {
     TAPI_GPRS_DEFINE_PDP_CONTEXT=0x01,		/* 0x01 : Define PDP Context */
     TAPI_GSM_GPRS_QOS,						/* 0x02 : Quality of Service Profile */
     TAPI_GSM_GPRS_PS,						/* 0x03 : PS attach or detach */
     TAPI_GSM_GPRS_PDP_CONTEXT,				/* 0x04 : PDP context activate or deactivate */
     TAPI_GSM_GPRS_ENTER_DATA,				/* 0x05 : Enter data */
     TAPI_GSM_GPRS_SHOW_PDP_ADDR,			/* 0x06 : Show PDP address*/
     TAPI_GSM_GPRS_MS_CLASS,					/* 0x07 : GPRS mobile station class*/
     TAPI_GSM_GPRS_3G_QUAL_SRVC_PROFILE,		/* 0x08 : 3G Quality of service profile */
     TAPI_GSM_GPRS_IP_CONFIGURATION,			/* 0x09 : Multiple PDP context IP configuration */
     TAPI_GSM_GPRS_DEFINE_SEC_PDP_CONTEXT,		/* 0X0A : AT+CGDSCONT	05.10.07 ky.doo EDIT */
     TAPI_GSM_GPRS_TFT,							/* 0X0B : AT+CGTFT	05.10.07 ky.doo EDIT */
     TAPI_GSM_GPRS_HSDPA_STATUS,				/* 0x0C : HSDPA status */
     TAPI_GSM_GPRS_CURRENT_SESSION_DATA_COUNTER,/* 0x0D : Current data session tx/rx total bytes */
     TAPI_GSM_GPRS_DATA_DORMANT,                /* 0x0E : Force to set Dormant */
     TAPI_GSM_GPRS_DUN_PIN_CTRL ,               /* 0x0F : Dial up Networking Pin Control Message */
     TAPI_GSM_GPRS_CALL_STATUS,			    /* 0x10 : DS TE2 Data Call result(Multiple PDP) */
     TAPI_GSM_GPRS_PORT_SEL,				/* 0x11 : PORT SEL */
     TAPI_GSM_GPRS_MAX
} tapi_gprs_type_t;

typedef enum{
	TAPI_PDP_MODE_ADD = 0x01,				/*ADD*/
	TAPI_PDP_MODE_DEL = 0x02					/*Delete*/
} tapi_pdp_mode_type_t;

typedef enum{
	TAPI_PDP_AUTH_NONE = 0x0,		/* No authentication           */
	TAPI_PDP_AUTH_PAP  = 0x1,		/* PAP authentication          */
	TAPI_PDP_AUTH_CHAP = 0x2,		/* CHAP authentication       */
	TAPI_PDP_AUTH_MAX  = 0xff

} tapi_pdp_auth_type_t;

typedef enum {
	GPRS_PDP_TYPE_UNKNOWN	= 0x00,
	GPRS_PDP_TYPE_X25		= 0x01,
	GPRS_PDP_TYPE_IP		= 0x02,
	GPRS_PDP_TYPE_IHOSP	= 0x03,
	GPRS_PDP_TYPE_PPP		= 0x04,
	GPRS_PDP_TYPE_IPV6		= 0x05,
} tapi_pdp_type_t;

/* HSDPA Status Variable */
typedef enum {
  HSDPA_INACTIVE,
  HSDPA_ACTIVE
} tapi_hspda_status_type;

typedef enum {
	GPRS_SIGNAL_DCD		= 0x01,
	GPRS_DATA_SIGNAL_DTR	= 0x04,
	GPRS_DATA_SIGNAL_DSR	= 0x06,
	GPRS_DATA_SIGNAL_RTS	= 0x07,
	GPRS_DATA_SIGNAL_CTS	= 0x08,
	GPRS_DATA_SIGNAL_RI	= 0x09,
} tapi_ps_btdun_pincontrol_signal;

typedef enum {
	GPRS_SIGNAL_STATUS_OFF	= 0x00,
	GPRS_SIGNAL_STATUS_ON	= 0x01,
} tapi_ps_btdun_pincontrol_status;

typedef enum {
	TAPI_GPRS_PS_STATUS_DETACH = 0x00,
	TAPI_GPRS_PS_STATUS_ATTACH = 0x01,
	TAPI_GPRS_PS_STATUS_MAX
} tapi_gprs_pdp_status;

typedef enum {
	TAPI_GPRS_MO_SMS_SVC_CIRCUIT			= 0x00,
	TAPI_GPRS_MO_SMS_SVC_GPRS				= 0x01,
	TAPI_GPRS_MO_SMS_SVC_CIRCUITPREFERRED	= 0x02,
	TAPI_GPRS_MO_SMS_SVC_GPRSPREFERRED		= 0x03,
} tapi_gprs_pdp_mo_sms_service;

typedef enum {
	TAPI_GPRS_3G_QOS_TYPE_REQUEST			= 0x01,
	TAPI_GPRS_3G_QOS_TYPE_MIN_ACCEPT		= 0x02,
	TAPI_GPRS_3G_QOS_TYPE_NEGOTIATE		= 0x03,
} tapi_gprs_pdp_3g_qos_type;

typedef enum {
	TAPI_GPRS_3G_QOS_DELI_ORDER_NO			= 0x00,
	TAPI_GPRS_3G_QOS_DELI_ORDER_YES			= 0x01,
	TAPI_GPRS_3G_QOS_DELI_ORDER_SUBSCRIBE	= 0x02,
} tapi_gprs_pdp_3g_qos_deli_order;

typedef enum {
	DSTATE_OK			= 0x00,
	DSTATE_CONNECT,
	DSTATE_RING,
	DSTATE_NO_CARRIER,
	DSTATE_ERROR,
	DSTATE_NO_DIALTONE,
	DSTATE_BUSY,
	DSTATE_NO_ANSWER,
	DSTATE_RESULT_FDN,
	DSTATE_MAX,
} tapi_gprs_data_status;

typedef enum {
	PDP_ACT_FAILURE_CAUSE_NORMAL					= 0x00,			// 0x00 : Normal Process ( no problem )
	PDP_ACT_FAILURE_CAUSE_REL_BY_USER				= 0x01,			// Call Released by User
	PDP_ACT_FAILURE_CAUSE_REGULAR_DEACTIVATION		= 0x02,			// Regular deactivation
	PDP_ACT_FAILURE_CAUSE_LLC_SNDCP				= 0x03,			// LLC SNDCP failure
	PDP_ACT_FAILURE_CAUSE_INSUFFICIENT_RESOURCE	= 0x04,			// Insufficient resources
	PDP_ACT_FAILURE_CAUSE_UNKNOWN_APN				= 0x05,			// Missing or unknown apn
	PDP_ACT_FAILURE_CAUSE_UNKNOWN_PDP_ADDRESS		= 0x06,         // Unknown PDP address or type
	PDP_ACT_FAILURE_CAUSE_USER_AUTH_FAILED			= 0x07,		// Unknown PDP address or type
	PDP_ACT_FAILURE_CAUSE_ACT_REJ_GGSN				= 0x08,			// Unknown PDP address or type
	PDP_ACT_FAILURE_CAUSE_ACT_REJ_UNSPECIFIED		= 0x09,			// Unknown PDP address or type
	PDP_ACT_FAILURE_CAUSE_SVC_OPTION_NOT_SUPPORTED	= 0x0A,		// Service option not supported
	PDP_ACT_FAILURE_CAUSE_SVC_NOT_SUBSCRIBED		= 0x0B,		// Requested service option not subscribed
	PDP_ACT_FAILURE_CAUSE_SVC_OPT_OUT_ORDER		= 0x0C,			// Service out of order
    PDP_ACT_FAILURE_CAUSE_NSAPI_USED				= 0x0D,		// NSAPI already used
	PDP_ACT_FAILURE_CAUSE_QOS_NOT_ACCEPTED			= 0x0E,		// QoS not accepted
	PDP_ACT_FAILURE_CAUSE_NETWORK_FAILURE			= 0x0F,		// Network Failure
    PDP_ACT_FAILURE_CAUSE_REACT_REQUIRED			= 0x10,		// Reactivation Required
	PDP_ACT_FAILURE_CAUSE_REATURE_NOT_SUPPORTED		= 0x11,		// Feature not supported
	PDP_ACT_FAILURE_CAUSE_TFT_FILTER_ERROR			= 0x12,		// TFT or filter error
	PDP_ACT_FAILURE_CAUSE_UNKOWN_PDP_CONTEXT		= 0x13,		// Unknown PDP context
	PDP_ACT_FAILURE_CAUSE_INVALID_MSG				= 0x14,		// Invalid MSG
	PDP_ACT_FAILURE_CAUSE_PROTOCOL_ERROR			= 0x15,		// Protocol error
	PDP_ACT_FAILURE_CAUSE_MOBILE_FAILURE_ERROR		= 0x16,		// Mobile failure error
	PDP_ACT_FAILURE_CAUSE_TIMEOUT_ERROR				= 0x17,		// Timeout error
	PDP_ACT_FAILURE_CAUSE_UNKNOWN_ERROR				= 0x18,		// Unknown error
	PDP_ACT_FAILURE_CAUSE_MAX ,
} tapi_gprs_ds_status_reason;

typedef enum {
	INTERNAL_PS_CALL = 0x00,
	EXTERNAL_PS_CALL = 0x01
} tapi_gprs_external_status;

typedef enum {
	PDP_DEVICE_INFO_GET = 0x00,
	PDP_DEVICE_INFO_SET = 0x01
} tapi_gprs_device_info;

typedef enum{
    TAPI_GPRS_PORT_LIST_NOT_USE,    // 0x00
	TAPI_GPRS_PORT_LIST_WHITE,      // 0x01
	TAPI_GPRS_PORT_LIST_BLACK,      // 0x02
} tapi_gprs_port_list_type;

typedef enum {
	TAPI_PDP_STATE_NONE,             /* 0x00 Default state for a given PDP context*/
    TAPI_PDP_STATE_ACTIVATING,		 /* 0x01 TS will be in this state when there is a START PDP Request until it is  activated by modem - This state PDP is not yet UP*/
    TAPI_PDP_STATE_ACTIVE,           /* 0x02 After PDP activation is completed by modem and sent to PDA - This state PDP is fully UP*/
    TAPI_PDP_STATE_MODIFYING,        /* 0x03 This state is reserved for future use - When TS support modifying existing PDP*/
    TAPI_PDP_STATE_DEACTIVATING,     /* 0x04 TS will be in this state when there is STOP PDP request from App until it is deactivated by modem */
} tapi_gprs_state;

typedef enum {
	TAPI_PDP_DATA_DORMANCY_SUCCESS,
	TAPI_PDP_DATA_DORMANCY_FAILED
} TelTapiGprsDataDormancy_t;

/* Suspend Reason Type */
typedef enum {
  TAPI_PDP_SUSPEND_NO_SVC			= 0x01,
  TAPI_PDP_SUSPEND_RAT_CHANGE		= 0x02,
  TAPI_PDP_SUSPEND_GPRS_HOLD		= 0x03,
  TAPI_PDP_SUSPEND_FLOW_CONTROL	= 0x04,
  TAPI_PDP_SUSPEND_GPRS_SUSPEND	= 0x05,
  TAPI_PDP_SUSPEND_RAU			= 0x06,
  TAPI_PDP_SUSPEND_LU			    = 0x07,
  TAPI_PDP_TRANSFER_STATUS_MAX
} tapi_gprs_suspend_type;

typedef struct {
	tapi_gprs_type_t type;
	tapi_phone_err_t cause;
} tapi_gprs_error_info_t;

typedef struct {
	int cont_id;
	tapi_pdp_err_t err;
} tapi_ps_pdp_info_t;

typedef struct {
	int cont_id;
	tapi_pdp_err_t err;
} tapi_ps_net_stop_rsp_t;

typedef struct {
	int cont_id;
	tapi_pdp_err_t err;
} tapi_ps_net_activation_rsp_t;

typedef struct {
	int cont_id;
	tapi_pdp_err_t err;
} tapi_ps_net_stop_ind_t;

/*	ps attach response	*/
typedef struct {
	int	state;
	int	transfer_status;
} tapi_ps_attach_response_t;

/*	ps call statistics	*/
typedef struct {
	unsigned char	    cid;	//	call id . for proper bit alignment prefer int than short
	unsigned long int	rx;	//	total number of bytes received for this cid
	unsigned long int	tx;		//	total number of bytes transmitted for this cid
} tapi_ps_call_statistics_t;

typedef struct {
	tapi_ps_btdun_pincontrol_signal signal;
	tapi_ps_btdun_pincontrol_status status;
} tapi_ps_btdun_pincontrol;

/**
 * This structure defines PDP information.
 * @see	ps_net_info_t
 */
typedef struct {
   unsigned short	is_primary_cont;		/*Primary or secondary*/
   int				primary_cont;			/*Primary context id, if the is the context is secondary*/
   char			apn[TAPI_PDP_APN_LEN_MAX];
   unsigned char	pdp_address[TAPI_PDP_ADDR_LEN_MAX];
   unsigned char	username[TAPI_PDP_MAX_USERNAME_LEN ];
   unsigned char	password[TAPI_PDP_MAX_PASSWORD_LEN ];
   unsigned char	dns1[TAPI_PDP_MAX_DNS_LEN ];               // @field Context activation DNS1
   unsigned char	dns2[TAPI_PDP_MAX_DNS_LEN ];               // @field Context activation DNS2
   tapi_pdp_auth_type_t auth_type;
   tapi_pdp_type_t		pdp_type;
} tapi_pdp_info_t;

typedef struct {
	int cont_id;
	tapi_pdp_info_t pdp_info;
} tapi_ps_net_start_req_t;

typedef struct {
	unsigned short	is_primary_cont;  /*Primary or secondary*/
	unsigned char		username[TAPI_PDP_MAX_USERNAME_LEN ];
	unsigned char	password[TAPI_PDP_MAX_PASSWORD_LEN ];
	unsigned char	dns1[TAPI_PDP_MAX_DNS_LEN ];               // @field Context activation DNS1
	unsigned char	dns2[TAPI_PDP_MAX_DNS_LEN ];               // @field Context activation DNS2
    tapi_pdp_auth_type_t auth_type;
} tapi_pdpdeact_info_t;

typedef struct {
	int cont_id;
	tapi_pdpdeact_info_t pdp_info;
} tapi_ps_net_stop_req_t;

typedef struct {
	unsigned char field_flag;
	unsigned char ip_address[4];
	unsigned char primary_dns[4];
	unsigned char secondary_dns[4];
	unsigned char gateway[4];
	unsigned char subnet_mask[4];
} tapi_pdp_resp_info_t;

typedef struct {
	int cont_id;
	tapi_pdp_err_t err;
	tapi_pdp_resp_info_t pdp_info;
	char devname[TAPI_PDP_NAME_LEN_MAX];
} tapi_ps_net_start_rsp_t;

typedef struct tagSetPDPActDeact {
	int cid;
	unsigned char username[TAPI_PDP_MAX_USERNAME_LEN ];	// @field Context activation user name
	unsigned char passwd[TAPI_PDP_MAX_PASSWORD_LEN ];		// @field Context activation password
	unsigned char dns1[TAPI_PDP_MAX_DNS_LEN ];				// @field Context activation DNS1
	unsigned char dns2[TAPI_PDP_MAX_DNS_LEN ];				// @field Context activation DNS2
	tapi_pdp_auth_type_t	auth;
} tapi_gprs_pdp_act_deact_set;

typedef struct {
	char cid;
	tapi_gprs_pdp_status state;
} tapi_gprs_pdp_status_info;

typedef struct {
	char num_act;
	tapi_gprs_pdp_status_info info[3];
} tapi_gprs_pdp_status_res;

typedef struct {
	char cid;
	char address[20];
} tapi_gprs_pdp_address_res;

typedef struct {
	unsigned char				cid;
	tapi_gprs_data_status		status;
	tapi_gprs_ds_status_reason	reason;
	unsigned char				external;
} tapi_gprs_pdp_status_reason_info;

typedef struct {
    tapi_gprs_port_list_type type;
    unsigned char len;
    unsigned short list[MAX_GPRS_PORT_LIST];
} tapi_gprs_port_list;

typedef struct {
    tapi_gprs_port_list tcp_list;
    tapi_gprs_port_list udp_list;
} tapi_gprs_pdp_port_list_setting_info_t;

/**
 * @brief  This API is used to request to Telephony Server to set PDP context define info, PDP activation.
 *  Access to this API is limited, we recommend you use Data Network API
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param[in] pNet_start_req_info
 * - This input has the PDP context define info, PDP activation info #tapi_ps_net_start_req_t
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_EVENT_PS_PDP_ACT_RSP  and the event data is #tapi_ps_pdp_info_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiPS.h>
 *
 * int ret_status;
 * int request_id = 0;
 * tapi_ps_net_start_req_t start_req;
 *
 * start_req.cont_id = CONTEXT_ID; //use proper id regarding upper layer usage
 * strcpy(start_req.pdp_info.apn, APN_ADDRESS);
 *
 * ret_status = tel_activate_gprs_pdp (&start_req, &request_id);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int	tel_activate_gprs_pdp(const tapi_ps_net_start_req_t *pNet_start_req_info, int *pRequestID);

/**
 * @brief  This API is used to request to Telephony Server to deactivate PDP service
 *  Access to this API is limited, we recommend you use Data Network API
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param[in] net_stop_req_info
 * - This input has the PDP context define info, PDP activation info #tapi_ps_net_start_req_t
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_EVENT_PS_PDP_DEACT_RSP  and the event data is #tapi_ps_pdp_info_t.
 *
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiPS.h>
 *
 * int ret_status;
 * int request_id = 0;
 * tapi_ps_net_stop_req_t end_req;
 *
 * memset(&end_req, 0, sizeof(tapi_ps_net_stop_req_t));
 *
 * end_req.cont_id = CONTEXT_ID;
 *
 * ret_status = tel_deactivate_gprs_pdp (&end_req, &request_id); //use context id which is used in activate time
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int	tel_deactivate_gprs_pdp(const tapi_ps_net_stop_req_t *net_stop_req_info, int *pRequestID);

/**
 * @brief  This API is used to send modem data channels to dormant state for power saving when there is no data flowing for certain time
 *         Access to this API is limited, we recommend you use Data Network API
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  -The event associated is TAPI_EVENT_PS_DATA_DORMANT_RESP  and the event data is #TelTapiGprsDataDormancy_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiPS.h>
 *
 * int ret_status;
 * int request_id = 0;
 *
 * ret_status = tel_set_gprs_dormant(&request_id); //if user want to dormant state
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int	tel_set_gprs_dormant(int *pRequestID);

/**
 * @brief  This API is used to get port list
 *         Access to this API is limited, we recommend you use Data Network API
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  -The event associated is TAPI_EVENT_PS_PDP_GET_PORTLIST_RSP  and the event data is #tapi_gprs_pdp_port_list_setting_info_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiPS.h>
 *
 * int ret_val;
 * int requestId = 0;
 *
 * ret_val = tel_get_gprs_port_list(&requestId); //get port list with async response. current return value means whether request send low layer properly or not
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int	tel_get_gprs_port_list(int *pRequestID);

/**
 * @brief  This API is used to set port list
 *         Access to this API is limited, we recommend you use Data Network API
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param[in] info
 * - This input has the TCP/UDP port list information info #tapi_gprs_pdp_port_list_setting_info_t
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  -None
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiPS.h>
 *
 * int ret_val;
 * int requestId = 0;
 * tapi_gprs_pdp_port_list_setting_info_t test;
 *
 * memset(&test, 0, sizeof(tapi_gprs_pdp_port_list_setting_info_t));
 * test.tcp_list.type = TAPI_GPRS_PORT_LIST_NOT_USE;
 * test.tcp_list.len = 1;
 * test.tcp_list.list[0] = 1004;
 * test.udp_list.type = TAPI_GPRS_PORT_LIST_NOT_USE;
 * test.udp_list.len = 1;
 * test.udp_list.list[i] = 1111;
 *
 * ret_val = tel_set_gprs_port_list(&test, &requestId); //get operation result with async response. current return value means whether request send low layer properly or not
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int	tel_set_gprs_port_list(tapi_gprs_pdp_port_list_setting_info_t *info, int *pRequestID);


/**
 * @brief  This API is used to request BT Dun enable or disable
 *         Access to this API is limited, we recommend you use Data Network API
 *
 * This function makes Dbus method call to Telephony Sever and returns immediate value.
 * However it just means that the API request has been transfered to the CP successfully.
 * The actual operation result is being delivered in the corresponding event asynchronously.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -NONE
 *
 * @warning
 * - None.
 *
 * @param[in] pincontrol
 * - This input has the BT Dun pin control information #tapi_ps_btdun_pincontrol
 *
 * @param [out] pRequestID
 *   - Unique identifier for a particular request.
 *   - request_id value can be any value from 0 to 255 if the API is returned successfully
 *   - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * -The event associated is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI or TAPI_EVENT_PS_ERROR_IND.
 * -In success, the event associated is TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI  and the event data is #tapi_ps_btdun_pincontrol.
 *   In fail, the event associated is TAPI_EVENT_PS_ERROR_IND  and the event data is #tapi_gprs_error_info_t.
 *
 * @pre
 *  - A dbus connection is established with #tel_init
 *  - The application name is registered with #tel_register_app_name
 *  - The application is registered events to listen asynchronous response with #tel_register_event
 *  - A event loop is running to listen events
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiPS.h>
 *
 * int ret_status;
 * int request_id = 0;
 * tapi_ps_btdun_pincontrol bt_control;
 *
 * bt_control.signal = GPRS_SIGNAL_DCD;
 * bt_control.status = GPRS_SIGNAL_STATUS_OFF;
 *
 * ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id); //get control result with async response. current return value means whether request send low layer properly or not
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int tel_control_gprs_btdun_pin(tapi_ps_btdun_pincontrol pincontrol, int *pRequestID);

#ifdef __cplusplus
}
#endif

#endif	 //_ITAPI_PS_H_

/**
   *  @}
   */
