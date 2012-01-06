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
 * @ingroup           TELEPHONY_API
 * @addtogroup	Data_Service Data Services
 * @{
 * @file TelData.h
 *
 * Data APIs allow an application to accomplish the following services: @n
 * - process data pin control. @n
 */

#ifndef _TEL_DATA_H
#define _TEL_DATA_H

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/
#include <TelDefines.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 MACROS
 ==================================================================================================*/
#define TAPI_DATA_MAX_DIAL_NUM_LEN				32		/** Maximum length of dial number */
#define TAPI_DATA_MIP_CONNECT_SUCCESS			0xFF	/** For Sprint */
#define TAPI_DATA_MAX_USER_NAI_LEN				72		/** Maximum length of user NAI */
#define TAPI_DATA_MAX_SHARED_SECRET_LEN			16		/** Maximum length of shared secret */

#define TAPI_DATA_MAX_PKT_DATA_ORIGINSTRING_LEN	16		/** Maximum length of Packet data origin string */
#define TAPI_DATA_MAX_PAP_USER_ID_LEN			64		/** Maximum length of Packet PAP user ID */
#define TAPI_DATA_MAX_PAP_USER_PASSWORD_LEN		14		/** Maximum length of Packet PAP password */

/*==================================================================================================
 ENUMS
 ==================================================================================================*/

typedef enum {
	TAPI_DATA_NO_ERR = 0x00, /**< There is no error for any data operation. */
	TAPI_DATA_PARAMETER_ERR, /**< There is no error for any data operation. */
	TAPI_DATA_OFFLINE_ERR,   /**< This error is sent when any network option is tried when phone is in offline mode */
	TAPI_DATA_OTHER_ERR      /**< This error to indicate other failure */
} TelDataOperationCause_t;

/**
 * @enum TelDataSignalType
 * Data Signal Type
 */
typedef enum {
	TAPI_DATA_SIGNAL_DCD = 0x01, /**< DCD Signal */
	TAPI_DATA_SIGNAL_DTR = 0x04, /**< DTR Signal */
	TAPI_DATA_SIGNAL_DSR = 0x06, /**< DSR Signal */
	TAPI_DATA_SIGNAL_RTS = 0x07, /**< RTS Signal */
	TAPI_DATA_SIGNAL_CTS = 0x08, /**< CTS Signal */
	TAPI_DATA_SIGNAL_RI = 0x09,  /**< RI Signal */
} TelDataSignalType_t;

/**
 * @enum TelDataSignalStatus
 * Data Signal Status
 */
typedef enum {
	TAPI_DATA_SIGNAL_STATUS_OFF = 0x00, /**< Signal Status Off */
	TAPI_DATA_SIGNAL_STATUS_ON = 0x01   /**< Signal Status On */
} TelDataSignalStatus_t;

typedef enum {
	TAPI_DATA_TE2_TYPE_UART = 0x00,
	TAPI_DATA_TE2_TYPE_USB = 0x01,
	TAPI_DATA_TE2_TYPE_BLUETOOTH = 0x02
} TelDataTE2Type_t;

typedef enum {
	TAPI_DATA_TE2_DETACHED = 0x00,
	TAPI_DATA_TE2_ATTACHED = 0x01
} TelDataTE2CableStatus_t;

typedef enum {
	TAPI_DATA_CALL_TYPE_DEFAULT = 0x00,
	TAPI_DATA_CALL_TYPE_FAX_FOR_NEXT_CALL = 0x01,
	TAPI_DATA_CALL_TYPE_FAX_FOR_ALL_CALLS = 0x02,
	TAPI_DATA_CALL_TYPE_ASYNC_FOR_NEXT_CALL = 0x03,
	TAPI_DATA_CALL_TYPE_ASYNC_FOR_ALL_CALLS = 0x04
} TelDataCallType_t;

typedef enum {
	TAPI_DATA_CALL_STATUS_ALLOWED = 0x00,
	TAPI_DATA_CALL_STATUS_NOT_ALLOWED_NO_SERVICE = 0x01,
	TAPI_DATA_CALL_STATUS_NOT_ALLOWED_CALLING_VOICE = 0x02,
	TAPI_DATA_CALL_STATUS_NOT_ALLOWED_NOT_CDMA = 0x03,
	TAPI_DATA_CALL_STATUS_NOT_ALLOWED_LOCKED = 0x04,
	TAPI_DATA_CALL_STATUS_NOT_CALL_GUARD = 0x05
} TelDataDialingStatus_t;

typedef enum {
	TAPI_DATA_BAUDRATE_19200_BPS = 0x00,
	TAPI_DATA_BAUDRATE_38400_BPS = 0x01,
	TAPI_DATA_BAUDRATE_57600_BPS = 0x02,
	TAPI_DATA_BAUDRATE_115200_BPS = 0x03,
	TAPI_DATA_BAUDRATE_230400_BPS = 0x04
} TelDataBaudRate_t;

typedef enum {
	TAPI_DATA_FACTORY_NAI = 0x00,
	TAPI_DATA_CUSTOM_NAI = 0x01
} TelDataNAIIndex_t;

typedef enum {
	TAPI_DATA_NAI_MIP_MASK_NONE = 0x0000,
	TAPI_DATA_NAI_MIP_MASK_USER_NAI = 0x0001,
	TAPI_DATA_NAI_MIP_MASK_SPI_MN_HA = 0x0002,
	TAPI_DATA_NAI_MIP_MASK_SPI_MN_AAA = 0x0004,
	TAPI_DATA_NAI_MIP_MASK_REVERSE_TUNNEL = 0x0008,
	TAPI_DATA_NAI_MIP_MASK_HOME_ADDRESS = 0x0010,
	TAPI_DATA_NAI_MIP_MASK_PRIMARY_HOME_AGENT_IP = 0x0020,
	TAPI_DATA_NAI_MIP_MASK_SECONDARY_HOME_AGENT_IP = 0x0040,
	TAPI_DATA_NAI_MIP_MASK_MN_AAA_SS = 0x0080,
	TAPI_DATA_NAI_MIP_MASK_MN_HA_SS = 0x0100,
	TAPI_DATA_NAI_MIP_MASK_ALGO_SIP_SS = 0x0200,
	TAPI_DATA_NAI_MIP_MASK_ALGO_MIP_MN_AAA_SS = 0x0400,
	TAPI_DATA_NAI_MIP_MASK_ALGO_MIP_MN_HA_SS = 0x0800,
	TAPI_DATA_NAI_MIP_MASK_ALL = 0xFFFF
} TelDataNaiMipParamMask_t;

typedef enum {
	TAPI_DATA_NAI_GENERIC_INFO_CHANGED = 0x0, /**< NAI, MN-HA SPI Set, MN-HA SPI Value, MN-AAA SPI Set, MN-AAA SPI Value, Rev Tunnel Preferred, Mobile IP Address, Primary HA Address, Secondary HA Address */
	TAPI_DATA_SHARED_SECRET_CHANGED /**< HA Shared Secret, AAA Shared Secret */
} TelDataNaiChangedItem_t;

typedef enum {
	TAPI_DATA_MIP_CHANGED_ITEM_NAI_INFO = 0x00,
	TAPI_DATA_MIP_CHANGED_ITEM_SHARED_SECRET = 0x01
} TelDataNAIItem_t;

typedef enum {
	TAPI_DATA_PACKET_NAI_SIGNIN_OUT = 0x00,
	TAPI_DATA_PACKET_NAI_SIGNIN_IN = 0x01
} TelDataSignStatus_t;

typedef enum {
	TAPI_DATA_ROAM_GUARD_DEFAULT = 0x00,
	TAPI_DATA_ROAM_GUARD_ALWAYS_ASK = 0x01,
	TAPI_DATA_ROAM_ROAM_GUARD_NEVER_ASK = 0x02,
	TAPI_DATA_ROAM_ROAM_GUARD_OFF = 0x03,
	TAPI_DATA_ROAM_ROAM_GUARD_ON = 0x04
} TelDataRoamGuardMode_t;

typedef enum {
	TAPI_DATA_MODEM_NAI_OFF = 0x00,
	TAPI_DATA_MODEM_NAI_ON = 0x01
} TelDataModemNAIMode_t;

typedef enum {
	TAPI_DATA_DS_TYPE_NORMAL = 0x01,
	TAPI_DATA_DS_TYPE_DUN = 0x02,
	TAPI_DATA_DS_TYPE_BTDUN = 0x03,
	TAPI_DATA_DS_TYPE_IS = 0x04,
	TAPI_DATA_DS_TYPE_MMS = 0x05
} TelDataDSType_t;

typedef enum {
	TAPI_DATA_REVA = 0x00,
	TAPI_DATA_REV0 = 0x01
} TelDataRevConfig_t;

typedef enum {
	TAPI_DATA_COMMERCIAL_NETWORK = 0x00,
	TAPI_DATA_DOMESTIC_NETWORK = 0x01,
	TAPI_DATA_RESERVED = 0x02
} TelDataNAISettingMode_t;

typedef enum {
	TAPI_DATA_DDTM_MODE_OFF = 0x00,
	TAPI_DATA_DDTM_MODE_ON = 0x01
} TelDataDDTMStatus_t;

typedef enum {
	TAPI_DATA_DSWORKING_MODE_OFF = 0x00,
	TAPI_DATA_DSWORKING_MODE_ON = 0x01
} TelDataDSWorkingMode_t;

typedef enum {
	TAPI_DATA_PACKET_DATA_TYPE_QNC = 0x00,
	TAPI_DATA_PACKET_DATA_TYPE_MIP = 0x01,
	TAPI_DATA_PACKET_DATA_TYPE_CALL_TYPE = 0x02,
	TAPI_DATA_PACKET_DATA_TYPE_PKT_ORIGIN_STRING = 0x03,
	TAPI_DATA_PACKET_DATA_TYPE_PAP_USER_ID = 0x04,
	TAPI_DATA_PACKET_DATA_TYPE_PAP_USER_PASSWORD = 0x05
} TelDataPacketConfigType_t;

typedef enum {
	TAPI_DATA_QNC_DISABLE = 0x00,
	TAPI_DATA_QNC_ENABLE = 0x01
} TelDataQncType_t;

typedef enum {
	TAPI_DATA_MIP_SIMPLE_IP_ONLY = 0x00,
	TAPI_DATA_MIP_IF_AVAIL = 0x01,
	TAPI_DATA_MIP_ONLY = 0x02
} TelDataMipType_t;

typedef enum {
	TAPI_DATA_CALL_TYPE_ONLY_MDR_SVC = 0x00,
	TAPI_DATA_CALL_TYPE_MDR_SVC = 0x01,
	TAPI_DATA_CALL_TYPE_ONLY_LSPD = 0x02,
	TAPI_DATA_CALL_TYPE_HSPD = 0x02
} TelDataPacketDataCallType_t;


/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/**
 * This structure contains Data pin control information.
 *
 */
typedef struct {
	TelDataSignalType_t signaltype; /**<Signal Type*/
	TelDataSignalStatus_t signalstatus; /**<Signal Status*/
} TelDataPinControlInfo_t;

typedef struct {
	TelDataTE2Type_t type;
	TelDataTE2CableStatus_t cable_status;
} TelDataTE2Status_t;

typedef struct {
	unsigned int rx_curr_packet_bytes;
	unsigned int tx_curr_packet_bytes;
	unsigned long long rx_total_packet_bytes;
	unsigned long long tx_total_packet_bytes;
} TelDataPacketByteCounter_t;

typedef struct {
	unsigned int tx_bytes;
	unsigned int rx_bytes;
	unsigned char report_period;
	unsigned char channel_num;
	unsigned char tx_bar_level;
	unsigned char rx_bar_level;
} TelDataTE2DataRateInfo_t;

typedef struct {
	unsigned char usrNaiLength;
	char usrNai[TAPI_DATA_MAX_USER_NAI_LEN + 1];
	unsigned int mnHaSpi;
	unsigned int mnAaaSpi;
	unsigned char revTunPreference;
	unsigned int homeAddress;
	unsigned int primaryHaAddress;
	unsigned int secondaryHaAddr;
} TelDataMipNaiGenericInfoType_t;

typedef struct {
	unsigned char mnHaSharedSecretLength;
	char mnHaSharedSecret[TAPI_DATA_MAX_SHARED_SECRET_LEN];
	unsigned char mnAaaSharedSecretLength;
	char mnAaaSharedSecret[TAPI_DATA_MAX_SHARED_SECRET_LEN];
}TelDataMipNaiSsInfoType_t;

typedef struct {
	TelDataNAIIndex_t naiIndex;
	unsigned short paramMask;
	TelDataMipNaiGenericInfoType_t genericInfo; /** NAI Generic Information */
	TelDataMipNaiSsInfoType_t ssInfo; /** <Shared Secret */
}TelDataMipNaiParam_t;

typedef struct {
	TelDataQncType_t qnc;
	TelDataMipType_t mip;
	TelDataPacketDataCallType_t pkt_data_call_type;
	char pkt_orig_str[TAPI_DATA_MAX_PKT_DATA_ORIGINSTRING_LEN+1];
	char pap_user_id[TAPI_DATA_MAX_PAP_USER_ID_LEN+1];
	char pap_password[TAPI_DATA_MAX_PAP_USER_PASSWORD_LEN+1];
}TelDataPacketParam_t;

typedef struct {
	TelDataPacketConfigType_t type;
	TelDataPacketParam_t param;
}TelDataPacketDataCallConfig_t;


/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif // _TEL_DATA_H
/**
 * @}
 */

