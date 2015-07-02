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
 * @file TelSim.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_SIM
 * @{
 */

#ifndef _TELSIM_H_
#define _TELSIM_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Definition for the maximum length of ICCID.
 * @since_tizen 2.3
 */
#define TAPI_SIM_ICCID_LEN_MAX 20

/**
 * @brief Definition for the maximum length of the alpha ID.
 * @since_tizen 2.3
 */
#define TAPI_SIM_XDN_ALPHA_ID_MAX_LEN 30

/**
 * @brief Definition for the maximum length of the dialing number.
 * @since_tizen 2.3
 */
#define TAPI_SIM_XDN_DIALING_NUMBER_LEN	20

/**
 * @brief Definition for the maximum length of the MSISDN number.
 * @since_tizen 2.3
 */
#define TAPI_SIM_MSISDN_DIALING_NUMBER_LEN	26

/**
 * @brief Definition for the maximum length of the CSP profile entry count.
 * @since_tizen 2.3
 */
#define TAPI_SIM_CPHS_CUSTOMER_SERVICE_PROFILE_ENTRY_COUNT_MAX	11

/**
 * @brief Definition for the maximum length of the authentication code.
 * @since_tizen 2.3
 */
#define TAPI_SIM_AUTH_MAX_REQ_DATA_LEN 256

/**
 * @brief Definition for the maximum length of authentication response data.
 * @since_tizen 2.3
 */
#define TAPI_SIM_AUTH_MAX_RESP_DATA_LEN 128

/**
 * @brief Definition for the maximum length of SAP APDU.
 * @details 2048 is for QMI definition. (1024 : general length, 1024 : extra length for additional response).
 * @since_tizen 2.3
 */
#define TAPI_SIM_APDU_MAX_LEN 2048

/**
 * @brief Definition for the maximum length of the SIM 3G Phone book EF.
 * @since_tizen 2.3
 */
#define TAPI_SIM_PB_3G_FILE_MAX_COUNT 13

/**
 * @brief Definition for the maximum length of the SIM Phone book Record name.
 * @since_tizen 2.3
 */
#define TAPI_SIM_PB_RECORD_NAME_MAX_LEN 255

/**
 * @brief Definition for the maximum length of the SIM Phone book Record number.
 * @since_tizen 2.3
 */
#define TAPI_SIM_PB_RECORD_NUMBER_MAX_LEN 255

/**
 * @brief Definition for the maximum length of the SIM Phone book Record email.
 * @since_tizen 2.3
 */
#define TAPI_SIM_PB_RECORD_EMAIL_MAX_LEN 255

/**
 * @brief Definition for the maximum length of the SAP answer to reset data.
 * @since_tizen 2.3
 */
#define TAPI_SIM_SAP_ATR_DATA	256

#define TAPI_SIM_NET_FULL_NAME_MAX_LEN 40

#define TAPI_SIM_NET_SHORT_NAME_MAX_LEN 10

#define	TAPI_SIM_MSP_CNT_MAX 2

#define TAPI_SIM_OPERATION_TIMEOUT 1234

#define TAPI_SIM_SST_SERVICE_CNT_MAX 56

#define TAPI_SIM_UST_SERVICE_CNT_MAX 64

#define TAPI_SIM_IST_SERVICE_CNT_MAX 11

#define TAPI_SIM_CDMA_ST_SERVICE_CNT_MAX 47

#define TAPI_SIM_CSIM_ST_SERVICE_CNT_MAX 41

/**
 * @brief Definition for the type of application on UICC
 * @since_tizen 2.4
 */
#define TAPI_SIM_APP_TYPE_SIM 0x01
#define TAPI_SIM_APP_TYPE_USIM 0x02
#define TAPI_SIM_APP_TYPE_CSIM 0x04
#define TAPI_SIM_APP_TYPE_ISIM 0x08

#define TAPI_SIM_GROUP_IDENTIFIER_LEN_MAX 10

#define TAPI_SIM_OPLMNWACT_LEN_MAX 100

/**
 * @brief Enumeration for the card type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_CARD_TYPE_UNKNOWN, /**< Unknown card */
	TAPI_SIM_CARD_TYPE_GSM, /**< SIM(GSM) card */
	TAPI_SIM_CARD_TYPE_USIM, /**< USIM card */
	TAPI_SIM_CARD_TYPE_RUIM, /**< CDMA card */
	TAPI_SIM_CARD_TYPE_NVSIM, /**< CDMA NV SIM */
	TAPI_SIM_CARD_TYPE_IMS, /**< will be deprecated */
} TelSimCardType_t;

/**
 * @brief Enumeration for the file ID.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_EF_DIR = 0x2F00, /**< Root Directory for the USIM */
	TAPI_SIM_EF_ICCID = 0x2FE2, /**< The ICC Identification file */
	TAPI_SIM_EF_IMSI = 0x6F07, /**< The IMSI file */
	TAPI_SIM_EF_SST = 0x6F38, /**< The SIM Service Table file */
	TAPI_SIM_EF_EST = 0x6F56, /**< The Enabled Service Table file */
	TAPI_SIM_EF_OPLMN_ACT = 0x6F61, /**< The OPLMN List file */
	TAPI_SIM_EF_GID1 = 0x6F3E, /**< The Group Identifier Level 1 */
	TAPI_SIM_EF_GID2 = 0x6F3F, /**< The Group Identifier Level 2 */

	TAPI_SIM_EF_ELP = 0x2F05, /**< The Extended Language Preference file */
	TAPI_SIM_EF_LP = 0x6F05, /**< SIM: Language preference */
	TAPI_SIM_EF_ECC = 0x6FB7, /**< The Emergency Call Codes */
	TAPI_SIM_EF_SPN = 0x6F46, /**< The Service Provider Name */
	TAPI_SIM_EF_SPDI = 0x6FCD, /**< The Service provider display information */
	TAPI_SIM_EF_PNN = 0x6FC5, /**< The PLMN Network Name File */
	TAPI_SIM_EF_OPL = 0x6FC6, /**< The Operator PLMN List File */
	TAPI_SIM_EF_MSISDN = 0x6F40, /**< MSISDN */

	TAPI_SIM_EF_SMS = 0x6F3C, /**< Short Messages file */
	TAPI_SIM_EF_SMSP = 0x6F42, /**< SMS Parameter */
	TAPI_SIM_EF_SMSS = 0x6F43, /**< SMS Status */
	TAPI_SIM_EF_CBMI = 0x6F45, /**< Cell Broadcast Message Identifier */
	TAPI_SIM_EF_MBDN = 0x6FC7, /**< SIM Mail Box Dialing Number file */

	TAPI_SIM_EF_USIM_MBI = 0x6FC9, /**< Mailbox Identifier - linear fixed */
	TAPI_SIM_EF_USIM_MWIS = 0x6FCA, /**< Message Waiting Indication Status - linear fixed */
	TAPI_SIM_EF_USIM_CFIS = 0x6FCB, /**< Call forward indication status - linear fixed */

	/* CPHS FILE ID */
	TAPI_SIM_EF_CPHS_VOICE_MSG_WAITING = 0x6F11, /**< CPHS voice MSG waiting indication */
	TAPI_SIM_EF_CPHS_SERVICE_STRING_TABLE = 0x6F12, /**< CPHS service string table */
	TAPI_SIM_EF_CPHS_CALL_FORWARD_FLAGS = 0x6F13, /**< CPHS call forward flags */
	TAPI_SIM_EF_CPHS_OPERATOR_NAME_STRING = 0x6F14, /**< CPHS operator name string */
	TAPI_SIM_EF_CPHS_CUSTOMER_SERVICE_PROFILE = 0x6F15, /**< CPHS customer service profile */
	TAPI_SIM_EF_CPHS_CPHS_INFO = 0x6F16, /**< CPHS information */
	TAPI_SIM_EF_CPHS_MAILBOX_NUMBERS = 0x6F17, /**< CPHS mail box numbers */
	TAPI_SIM_EF_CPHS_OPERATOR_NAME_SHORT_FORM_STRING = 0x6F18, /**< CPHS operator name short form string */
	TAPI_SIM_EF_CPHS_INFORMATION_NUMBERS = 0x6F19, /**< CPHS information numbers */
	/*  CPHS ALS FILE ID */
	TAPI_SIM_EF_CPHS_DYNAMICFLAGS = 0x6F9F, /**< CPHS Dynamics flags */
	TAPI_SIM_EF_CPHS_DYNAMIC2FLAG = 0x6F92, /**< CPHS Dynamics2 flags */
	TAPI_SIM_EF_CPHS_CUSTOMER_SERVICE_PROFILE_LINE2 = 0x6F98, /**< CPHS CSP2 */

	/* Invalid File ID, All the file IDs are less than this Value */
	TAPI_SIM_EF_INVALID = 0xFFFF, /**< Invalid file */
	TAPI_SIM_EF_OTHERS, /**< Element to indicate an unknown file */
}TelSimFileID_t;

/**
 * @brief Enumeration for the PIN status.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_FACILITY_DISABLED = 0x00,	/**< TBD */
	TAPI_SIM_FACILITY_ENABLED = 0x01,	/**< TBD */
	TAPI_SIM_FACILITY_UNKNOWN = 0xFF	/**< TBD */
} TelSimFacilityStatus_t;

/**
 * @brief Enumeration for the PIN operation result from the lower layers.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_PIN_OPERATION_SUCCESS, /**< Operation involving PIN (verification/change/enable/disable, and so on) is successful */
	TAPI_SIM_BUSY, /**< SIM is busy */
	TAPI_SIM_CARD_ERROR, /**< SIM card error - General errors */
	TAPI_SIM_INCOMPATIBLE_PIN_OPERATION, /**< SIM Incompatible PIN operation when an invalid SIM command is given or incorrect parameters are supplied to the SIM */
	TAPI_SIM_PIN_INCORRECT_PASSWORD, /**< SIM PIN incorrect password */
	TAPI_SIM_PUK_INCORRECT_PASSWORD, /**< SIM PUK incorrect password */
	TAPI_SIM_PUK_REQUIRED, /**< PUK Required */
	TAPI_SIM_PIN_REQUIRED, /**< PIN Required */
	TAPI_SIM_NCK_REQUIRED, /**< Network Control Key Required */
	TAPI_SIM_NSCK_REQUIRED, /**< Network Subset Control Key Required */
	TAPI_SIM_SPCK_REQUIRED, /**< Service Provider Control Key Required */
	TAPI_SIM_CCK_REQUIRED, /**< Corporate Control Key Required */
	TAPI_SIM_LOCK_REQUIRED, /**< PH-SIM (phone-SIM) locked state */
	TAPI_SIM_PERM_BLOCKED /**< Permanently Blocked */
} TelSimPinOperationResult_t;

/**
 * @brief Enumeration for the SIM access result from the lower layers.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_ACCESS_SUCCESS, /**< Access to file is successful */
	TAPI_SIM_ACCESS_CARD_ERROR, /**< SIM card error */
	TAPI_SIM_ACCESS_FILE_NOT_FOUND, /**< File not found */
	TAPI_SIM_ACCESS_ACCESS_CONDITION_NOT_SATISFIED, /**< Access condition is not fulfilled */
	TAPI_SIM_ACCESS_FAILED, /**< Access failed */
} TelSimAccessResult_t;

/**
 * @brief Enumeration for the SIM power set result from the lower layers.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_POWER_SET_SUCCESS, /**< Power Set is successful */
	TAPI_SIM_POWER_SET_FAILURE /**< Power Set failure */
} TelSimPowerSetResult_t;

/**
 * @brief Enumeration for the PIN type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_PTYPE_PIN1 = 0x00, /**< PIN 1 code */
	TAPI_SIM_PTYPE_PIN2 = 0x01, /**< PIN 2 code */
	TAPI_SIM_PTYPE_PUK1 = 0x02, /**< PUK 1 code */
	TAPI_SIM_PTYPE_PUK2 = 0x03, /**< PUK 2 code */
	TAPI_SIM_PTYPE_UPIN = 0x04, /**< Universal PIN - Unused now */
	TAPI_SIM_PTYPE_ADM = 0x05, /**< Administrator - Unused now */
	TAPI_SIM_PTYPE_SIM = 0x06 /**< SIM Lock code */
} TelSimPinType_t;

/**
 * @brief Enumeration for the number type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_TON_UNKNOWN = 0, /**< Unknown */
	TAPI_SIM_TON_INTERNATIONAL = 1, /**< International number */
	TAPI_SIM_TON_NATIONAL = 2, /**< National number */
	TAPI_SIM_TON_NETWORK_SPECIFIC = 3, /**< Network specific number */
	TAPI_SIM_TON_DEDICATED_ACCESS = 4, /**< Subscriber number */
	TAPI_SIM_TON_ALPHA_NUMERIC = 5, /**< Alphanumeric, GSM 7-bit default alphabet */
	TAPI_SIM_TON_ABBREVIATED_NUMBER = 6, /**< Abbreviated number */
	TAPI_SIM_TON_RESERVED_FOR_EXT = 7 /**< Reserved for extension */
} TelSimTypeOfNum_t;

/**
 * @brief Enumeration for the text encryption type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_TEXT_ENC_ASCII, /**< ASCII Encoding */
	TAPI_SIM_TEXT_ENC_GSM7BIT, /**< GSM 7 Bit Encoding */
	TAPI_SIM_TEXT_ENC_UCS2, /**< UCS2 Encoding */
	TAPI_SIM_TEXT_ENC_HEX, /**< HEX Encoding */
} TelSimTextEncrypt_t;

/**
 * @brief Enumeration for the numbering plan identifier.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_NPI_UNKNOWN = 0, /**< Unknown */
	TAPI_SIM_NPI_ISDN_TEL = 1, /**< ISDN/Telephone numbering plan */
	TAPI_SIM_NPI_DATA_NUMBERING_PLAN = 3, /**< Data numbering plan */
	TAPI_SIM_NPI_TELEX = 4, /**< Telex numbering plan */
	TAPI_SIM_NPI_SVC_CNTR_SPECIFIC_PLAN = 5, /**< Service Center Specific plan */
	TAPI_SIM_NPI_SVC_CNTR_SPECIFIC_PLAN2 = 6, /**< Service Center Specific plan */
	TAPI_SIM_NPI_NATIONAL = 8, /**< National numbering plan */
	TAPI_SIM_NPI_PRIVATE = 9, /**< Private numbering plan */
	TAPI_SIM_NPI_ERMES_NUMBERING_PLAN = 10, /**< ERMES numbering plan */
	TAPI_SIM_NPI_RESERVED_FOR_EXT = 0xF /**< Reserved for extension */
} TelSimNumberingPlanIdentity_t;

/**
 * @brief Enumeration for the emergency service type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_ECC_ESC_POLICE = 0x01, /**< Police */
	TAPI_SIM_ECC_ESC_AMBULANCE = 0x02, /**< Ambulance */
	TAPI_SIM_ECC_ESC_FIREBRIGADE = 0x04, /**< Fire brigade */
	TAPI_SIM_ECC_ESC_MARAINEGUARD = 0x08, /**< Marine guard */
	TAPI_SIM_ECC_ESC_MOUTAINRESCUE = 0x10, /**< Mountain rescue */
	TAPI_SIM_ECC_ESC_SPARE = 0x00 /**< Spare */
} TelSimEccEmergencyServiceInfo_t;

/**
 * @brief Enumeration for the language preference code.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_LP_GERMAN = 0x00, /**< German */
	TAPI_SIM_LP_ENGLISH = 0x01, /**< English */
	TAPI_SIM_LP_ITALIAN = 0x02, /**< Italian */
	TAPI_SIM_LP_FRENCH = 0x03, /**< French */
	TAPI_SIM_LP_SPANISH = 0x04, /**< Spanish */
	TAPI_SIM_LP_DUTCH = 0x05, /**< Dutch */
	TAPI_SIM_LP_SWEDISH = 0x06, /**< Swedish */
	TAPI_SIM_LP_DANISH = 0x07, /**< Danish */
	TAPI_SIM_LP_PORTUGUESE = 0x08, /**< Portuguese */
	TAPI_SIM_LP_FINNISH = 0x09, /**< Finnish */
	TAPI_SIM_LP_NORWEGIAN = 0x0A, /**< Norwegian */
	TAPI_SIM_LP_GREEK = 0x0B, /**< Greek */
	TAPI_SIM_LP_TURKISH = 0x0C, /**< Turkish */
	TAPI_SIM_LP_HUNGARIAN = 0x0D, /**< Hungarian */
	TAPI_SIM_LP_POLISH = 0x0E, /**< Polish */
	TAPI_SIM_LP_KOREAN = 0x0F, /**< Korean */
	TAPI_SIM_LP_CHINESE = 0x10, /**< Chinese */
	TAPI_SIM_LP_RUSSIAN = 0x11, /**< Russian */
	TAPI_SIM_LP_JAPANESE = 0x12, /**< Japanese */
	TAPI_SIM_LP_LANG_UNSPECIFIED = 0xFF /**< Unspecified */
} TelSimLanguagePreferenceCode_t;

/**
 * @brief Enumeration for the SIM card status.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_STATUS_CARD_ERROR = 0x00, /**< Bad card / On the fly SIM gone bad */
	TAPI_SIM_STATUS_CARD_NOT_PRESENT = 0x01, /**< Card not present */
	TAPI_SIM_STATUS_SIM_INITIALIZING = 0x02, /**< SIM is in the Initializing state */
	TAPI_SIM_STATUS_SIM_INIT_COMPLETED = 0x03, /**< SIM Initialization ok */
	TAPI_SIM_STATUS_SIM_PIN_REQUIRED = 0x04, /**< PIN  required state */
	TAPI_SIM_STATUS_SIM_PUK_REQUIRED = 0x05, /**< PUK required state */
	TAPI_SIM_STATUS_CARD_BLOCKED = 0x06, /**< PIN/PUK blocked(permanently blocked- All the attempts for PIN/PUK failed) */
	TAPI_SIM_STATUS_SIM_NCK_REQUIRED = 0x07, /**< Network Control Key required state */
	TAPI_SIM_STATUS_SIM_NSCK_REQUIRED = 0x08, /**< Network Subset Control Key required state */
	TAPI_SIM_STATUS_SIM_SPCK_REQUIRED = 0x09, /**< Service Provider Control Key required state */
	TAPI_SIM_STATUS_SIM_CCK_REQUIRED = 0x0a, /**< Corporate Control Key required state */
	TAPI_SIM_STATUS_CARD_REMOVED = 0x0b, /**< Card removed */
	TAPI_SIM_STATUS_SIM_LOCK_REQUIRED = 0x0c, /**< PH-SIM (phone-SIM) locked state */
	TAPI_SIM_STATUS_CARD_CRASHED = 0x0d, /**< Runtime SIM card error */
	TAPI_SIM_STATUS_CARD_POWEROFF = 0x0e, /**< SIM card Powered OFF */
	TAPI_SIM_STATUS_UNKNOWN = 0xff /**< Unknown status. It can be the initial status */
} TelSimCardStatus_t;

/**
 * @brief The structure type for the SIM card status.
 * @details This structure has information on the card_status and whether it is changed.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimCardStatus_t card_status;	/**< TBD */
	unsigned char b_is_changed;	/**< TBD */
} TelSimInitInfo_t;

/**
 * @brief Enumeration for the current CPHS phase of the SIM card.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_CPHS_PHASE1 = 0x01, /**< Phase 1 */
	TAPI_SIM_CPHS_PHASE2 = 0x02, /**< Phase 2 */
	TAPI_SIM_CPHS_PHASE_RFU = 0xff /**< RFU */
} TelSimCphsPhaseType_t;

/**
 * @brief The structure type for the CPHS index level indication.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_CPHS_INDEX_LEVEL_ONE = 0x01, /**< SIM CPHS index level one */
	TAPI_SIM_CPHS_INDEX_LEVEL_TWO = 0x02, /**< SIM CPHS index level two */
	TAPI_SIM_CPHS_INDEX_LEVEL_THREE = 0x03, /**< SIM CPHS index level three */
	TAPI_SIM_CPHS_INDEX_LEVEL_RFU = 0xff /**< SIM CPHS index level RFU */
} TelSimCphsIndexLevelIndicator_t;

/**
 * @brief The structure type for CPHS group service type information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CALL_OFFERING = 0x01, /**< Group CSP offering */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CALL_RESTRICTION = 0x02, /**< Group CSP restriction */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_OTHER_SUPP_SERVICES = 0x03, /**< Group CSP supplementary services */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CALL_COMPLETION = 0x04, /**< Group CSP completion */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_TELESERVICES = 0x05, /**< Group CSP teleservices */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CPHS_TELESERVICES = 0x06, /**< Group CSP CPHS teleservices */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CPHS_FEATURES = 0x07, /**< Group CSP CPHS features */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_NUMBER_IDENTIFIERS = 0x08, /**< Group CSP number identifiers */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_PHASE_SERVICES = 0x09, /**< Group CSP phase services */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_VALUE_ADDED_SERVICES = 0xC0, /**< Group CSP value added services */
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_INFORMATION_NUMBERS = 0xD5 /**< Group CSP information numbers */
} TelSimCphsCustomerServiceGroup_t;

/**
 * @brief Enumeration for the mailbox type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_MAILBOX_VOICE = 0x01, /**< Voicemail */
	TAPI_SIM_MAILBOX_FAX = 0x02, /**< Fax */
	TAPI_SIM_MAILBOX_EMAIL = 0x03, /**< Email */
	TAPI_SIM_MAILBOX_OTHER = 0x04, /**< Other */
	TAPI_SIM_MAILBOX_VIDEO = 0x05, /**< Videomail */
	TAPI_SIM_MAILBOX_DATA = 0x06, /**< Data */
} TelSimMailboxType_t;

/**
 * @brief Enumeration for the dynamics flag selected line information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_DYNAMIC_FLAGS_LINE1 = 0x01, /**< line 1 */
	TAPI_SIM_DYNAMIC_FLAGS_LINE2 = 0x00, /**< line 2 */
	TAPI_SIM_DYNAMIC_FLAGS_RFU = 0xff /**< RFU */
} TelSimDynamicFlagsSelectedLineId_t;

/**
 * @brief Enumeration for the dynamics2 flag selected line information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_PIN2_ACCESSIBLE_FLAGS_LOCKED = 0x01, /**< Dynamic flags locked */
	TAPI_SIM_PIN2_ACCESSIBLE_FLAGS_UNLOCKED = 0x00, /**< Dynamic flags unlocked */
	TAPI_SIM_PIN2_ACCESSIBLE_FLAGS_RFU = 0xff /**< RFU */
} TelSimDynamic2FlagAlsStatus_t;


/**
 * @brief Enumeration for the SIM Authentication type.
 * @details This is used in the SIM-based Authentication Procedure.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_AUTH_TYPE_IMS = 0x00, /**< IMS Authentication */
	TAPI_SIM_AUTH_TYPE_GSM, /**< GSM Authentication */
	TAPI_SIM_AUTH_TYPE_3G, /**< 3G Authentication */
	TAPI_SIM_AUTH_RUIM_CAVE, /**< CDMA CAVE Authentication */
	TAPI_SIM_AUTH_RUIM_CHAP, /**< CDMA CHAP Authentication */
	TAPI_SIM_AUTH_RUIM_MNHA, /**< CDMA MNHA Authentication */
	TAPI_SIM_AUTH_RUIM_MIPRRQ, /**< CDMA MIPRRQ Authentication */
	TAPI_SIM_AUTH_RUIM_MNAAA, /**< CDMA MNAAA Authentication */
	TAPI_SIM_AUTH_RUIM_HRPD,	/**< CDMA HRPD Authentication */
	TAPI_SIM_AUTH_TYPE_MAX /**< TBD */
} TelSimAuthenticationType_t;

/**
 * @brief Enumeration for the SIM Authentication result.
 * @details This is used for the Authentication Procedure.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_AUTH_NO_ERROR = 0x00, /**< ISIM no error */
	TAPI_SIM_AUTH_CANNOT_PERFORM, /**< Status - can't perform authentication */
	TAPI_SIM_AUTH_SKIP_RESPONSE, /**< Status - skip authentication response */
	TAPI_SIM_AUTH_MAK_CODE_FAILURE, /**< Status - MAK(Multiple Activation Key) code failure */
	TAPI_SIM_AUTH_SQN_FAILURE, /**< Status - SQN(SeQuenceNumber) failure */
	TAPI_SIM_AUTH_SYNCH_FAILURE, /**< Status - synch failure */
	TAPI_SIM_AUTH_UNSUPPORTED_CONTEXT, /**< Status - unsupported context */
	TAPI_SIM_AUTH_MAX /**< TBD */
} TelSimAuthenticationResult_t;

/**
 * @brief Enumeration for the security lock type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_LOCK_PS = 0x01, /**< PH-SIM (phone-SIM) locked. Lock Phone to SIM/UICC card
	 *	(MT asks for the password when a SIM/UICC card other than current SIM/UICC card is inserted;
	 *	MT may remember a certain amount of previously used cards thus not
	 *	requiring the password when they are inserted
	 */
	TAPI_SIM_LOCK_PF, /**< PH-FSIM (phone-first-SIM) Lock Phone to the very
	 * First inserted SIM/UICC card(MT asks for the password when a SIM/UICC card other than the first SIM/UICC
	 * card is inserted
	 */
	TAPI_SIM_LOCK_SC, /**< SIM Lock (PIN, PIN2, PUK, PUK2) Lock SIM/UICC card ( SIM asks for the password in ME power-up and
	 * when this command is issued
	 */
	TAPI_SIM_LOCK_FD, /**< FDN - SIM card or active application in the UICC (GSM or USIM)
	 * fixed dialing memory feature */
	TAPI_SIM_LOCK_PN, /**< Network Personalization */
	TAPI_SIM_LOCK_PU, /**< Network subset Personalization */
	TAPI_SIM_LOCK_PP, /**< Service Provider Personalization */
	TAPI_SIM_LOCK_PC, /**< Corporate Personalization */
} TelSimLockType_t;

/**
 * @brief Enumeration for the security lock key information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_LOCK_KEY_NOT_NEED = 0x00, /**< Key not needed */
	TAPI_SIM_LOCK_KEY_PIN = 0x01, /**< PIN required */
	TAPI_SIM_LOCK_KEY_PUK = 0x02, /**< PUK required */
	TAPI_SIM_LOCK_KEY_PIN2 = 0x03, /**< PIN2 required */
	TAPI_SIM_LOCK_KEY_PUK2 = 0x04, /**< PUK2 required */
	TAPI_SIM_LOCK_PERM_BLOCKED = 0x05, /**< Permanent block SIM */
} TelSimLockStatus_t;

/**
 * @brief Enumeration for the SAP message IDs between the SAP client and SAP server.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_POWER_SIM_ON_REQ, /**< SAP Client request about power SIM on in the Server */
	TAPI_SIM_SAP_POWER_SIM_OFF_REQ, /**< SAP Client request about power SIM off in the Server */
	TAPI_SIM_SAP_RESET_SIM_REQ, /**< SAP Client request about SIM reset in the Server */
} TelSimSapPowerMode_t;

/**
 * @brief Enumeration for the SAP connection status information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_CONNECTION_STATUS_OK = 0x00, /**< Connected successfully */
	TAPI_SIM_SAP_CONNECTION_STATUS_UNABLE_TO_ESTABLISH, /**< Unable to establish connection */
	TAPI_SIM_SAP_CONNECTION_STATUS_NOT_SUPPORT_MAX_SIZE, /**< When server does not support the message length of the message that the client wants to send */
	TAPI_SIM_SAP_CONNECTION_STATUS_TOO_SMALL_MAX_SIZE /**< When client wants to connect with a very small message length that is not supported by the Server */
} TelSimSapConnectionStatus_t;

/**
 * @brief Enumeration for the SAP disconnect type information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_DISCONNECT_TYPE_GRACEFUL = 0x00, /**< Disconnection procedure ends after finishing the current work */
	TAPI_SIM_SAP_DISCONNECT_TYPE_IMMEDIATE /**<  Disconnection procedure ends immediately */
} TelSimSapDissconnectType_t;

/**
 * @brief Enumeration for the SAP current connection status information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_STATUS_UNKNOWN = 0x00, /**< SAP server connection status - unknown */
	TAPI_SIM_SAP_STATUS_NO_SIM, /**< SAP server connection status - no SIM */
	TAPI_SIM_SAP_STATUS_NOT_READY, /**< SAP server connection status - not ready */
	TAPI_SIM_SAP_STATUS_READY, /**< SAP server connection status - ready */
	TAPI_SIM_SAP_STATUS_CONNECTED /**< SAP server connection status - connected */
} TelSimSapStatusInfo_t;

/**
 * @brief Enumeration for the SIM card status if the server's status about connection with subscription module has changed.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_CARD_STATUS_UNKNOWN = 0x00, /**< SAP server status(card reader status) - unknown */
	TAPI_SIM_SAP_CARD_STATUS_RESET, /**< SAP server status(card reader status) - reset */
	TAPI_SIM_SAP_CARD_STATUS_NOT_ACCESSIBLE, /**< SAP server status(card reader status) - not accessible */
	TAPI_SIM_SAP_CARD_STATUS_REMOVED, /**< SAP server status(card reader status) - removed */
	TAPI_SIM_SAP_CARD_STATUS_INSERTED, /**< SAP server status(card reader status) - inserted */
	TAPI_SIM_SAP_CARD_STATUS_RECOVERED /**< SAP server status(card reader status) - recovered */
} TelSimSapCardStatus_t;

/**
 * @brief Enumeration for the SAP result information.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_RESULT_CODE_OK = 0x00, /**< SAP operation result - ok */
	TAPI_SIM_SAP_RESULT_CODE_NO_REASON, /**< SAP operation result - no reason */
	TAPI_SIM_SAP_RESULT_CODE_CARD_NOT_ACCESSIBLE, /**< SAP operation result - not accessible */
	TAPI_SIM_SAP_RESULT_CODE_CARD_ALREADY_POWER_OFF, /**< SAP operation result - card already powered off */
	TAPI_SIM_SAP_RESULT_CODE_CARD_REMOVED, /**< SAP operation result - card removed */
	TAPI_SIM_SAP_RESULT_CODE_CARD_ALREADY_POWER_ON, /**< SAP operation result - card already powered on */
	TAPI_SIM_SAP_RESULT_CODE_DATA_NOT_AVAILABLE, /**< SAP operation result - data not available */
	TAPI_SIM_SAP_RESULT_CODE_NOT_SUPPORT /**< SAP operation result - not supported */
} TelSimSapResultCode_t;

/**
 * @brief Enumeration for the SAP transport protocol type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_SAP_PROTOCOL_T0, /**< T = 0, character */
	TAPI_SIM_SAP_PROTOCOL_T1 /**< T = 1, block */
} TelSimSapProtocol_t;

/**
 * @brief Enumeration for the phonebook access result.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_PB_SUCCESS, /**< SIM phonebook operation successful */
	TAPI_SIM_PB_FAIL, /**< SIM phonebook operation failure */
	TAPI_SIM_PB_INVALID_INDEX, /**< The index passed is not a valid index */
	TAPI_SIM_PB_INVALID_NUMBER_LENGTH, /**< The number length exceeds the max length allowed (or 0) */
	TAPI_SIM_PB_INVALID_NAME_LENGTH, /**< The name length exceeds the max length allowed (or 0) */
	TAPI_SIM_PB_ACCESS_CONDITION_NOT_SATISFIED, /**< Access condition for PB file is not satisfied */
} TelSimPbAccessResult_t;

/**
 * @brief Enumeration for the storage types to be selected in the SIM or USIM.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_PB_FDN, /**< Fixed Dialing Number */
	TAPI_SIM_PB_ADN, /**< SIM - ADN */
	TAPI_SIM_PB_SDN, /**< Service Dialing Number */
	TAPI_SIM_PB_3GSIM, /**< USIM - 3G phone book */
	TAPI_SIM_PB_AAS, /**< Additional number Alpha String */
	TAPI_SIM_PB_GAS, /**< Grouping identifier Alpha String */
	TAPI_SIM_PB_UNKNOWNN = 0xFF, /**< Unknown file type */
} TelSimPbType_t;

/**
 * @brief Enumeration for the storage field types in the 3G Phone book.
 * @since_tizen 2.3
 */
typedef enum {
	/* for 3G phone storage field type */
	TAPI_PB_3G_NAME = 0x01, /**< Name */
	TAPI_PB_3G_NUMBER, /**< Number */
	TAPI_PB_3G_ANR1, /**< First Another number */
	TAPI_PB_3G_ANR2, /**< Second Another number */
	TAPI_PB_3G_ANR3, /**< Third Another number */
	TAPI_PB_3G_EMAIL1, /**< First Email */
	TAPI_PB_3G_EMAIL2, /**< Second Email */
	TAPI_PB_3G_EMAIL3, /**< Third Email */
	TAPI_PB_3G_EMAIL4, /**< Fourth Email */
	TAPI_PB_3G_SNE, /**< Second name entry of the main name */
	TAPI_PB_3G_GRP, /**< Group */
	TAPI_PB_3G_PBC, /**< 1 byte control info and 1 byte hidden info */
} TelSimPb3GFileType_t;

/**
 * @brief Enumeration for the phonebook operation types.
 * @since_tizen 2.4
 */
typedef enum {
	TAPI_PB_UPDATE, /**< Contact added or updated */
	TAPI_PB_DELETE, /**< Existing contact deleted */
	TAPI_PB_MAX, /**< MAX value */
} TelSimPbOp_t;

/**
 * @brief Enumeration for the list of SST services in the SIM Service Table (GSM).
 * @since_tizen 2.3
 */
typedef enum {
	// service 1 ~ 8
	TAPI_SIM_SST_CHV1_DISABLE_FUNC = 0,	/**< CHV1 disable function */
	TAPI_SIM_SST_ADN,					/**< Abbreviated Dialing number */
	TAPI_SIM_SST_FDN,					/**< Fixed Dialing number */
	TAPI_SIM_SST_SMS,					/**< Short message storage */
	TAPI_SIM_SST_AOC,					/**< Advice of charge */
	TAPI_SIM_SST_CCP,					/**< Capability configuration parameters */
	TAPI_SIM_SST_PLMN_SELECTOR,			/**< PLMN selector */
	TAPI_SIM_SST_RFU1,					/**< RFU */

	// service 9 ~ 16
	TAPI_SIM_SST_MSISDN = 8,			/**< MSISDN */
	TAPI_SIM_SST_EXT1,					/**< Extension 1 */
	TAPI_SIM_SST_EXT2,					/**< Extension 2 */
	TAPI_SIM_SST_SMS_PARAMS,				/**< SMS parameters */
	TAPI_SIM_SST_LND,					/**< Last number dialed */
	TAPI_SIM_SST_CELL_BROADCAST_MSG_ID,	/**< Cell broadcast message identifier */
	TAPI_SIM_SST_GID_LV1,				/**< Group identifier level 1 */
	TAPI_SIM_SST_GID_LV2,				/**< Group identifier level 2 */

	// service 17 ~ 24
	TAPI_SIM_SST_SPN = 16,				/**< Service provider name */
	TAPI_SIM_SST_SDN,					/**< Service Dialing number */
	TAPI_SIM_SST_EXT3,					/**< Extension3 */
	TAPI_SIM_SST_RFU2,					/**< RFU */
	TAPI_SIM_SST_VGCS_GID_LIST,			/**< VGCS group identifier (EF-VGCS, EF-VGCSS) */
	TAPI_SIM_SST_VBS_GID_LIST,			/**< VBS group identifier (EF-VBS, EF-VBSS) */
	TAPI_SIM_SST_ENHANCED_MULTI_LV_PRECEDENCE_PREEMPTION_SRVC,	/**< Enhanced multi-level precedence and pre-emption service */
	TAPI_SIM_SST_AUTO_ANSWER_FOR_EMLPP,	/**< Automatic answer for EMLPP */

	// service 25 ~ 32,
	TAPI_SIM_SST_DATA_DOWNLOAD_VIA_SMS_CB = 24,	/**< Data download via SMS-CB */
	TAPI_SIM_SST_DATA_DOWNLOAD_VIA_SMS_PP,		/**< Data download via SMS-PP */
	TAPI_SIM_SST_MENU_SELECTION,					/**< Menu selection */
	TAPI_SIM_SST_CALL_CTRL,						/**< Call control */
	TAPI_SIM_SST_PROACTIVE_SIM,					/**< Proactive SIM command */
	TAPI_SIM_SST_CELL_BROADCAST_MSG_ID_RANGES,	/**< Cell broadcast message identifier ranges */
	TAPI_SIM_SST_BDN,							/**< Barred Dialing numbers */
	TAPI_SIM_SST_EXT4,							/**< Extension 4 */

	// service 33 ~ 40
	TAPI_SIM_SST_DEPERSONALIZATION_CTRL_KEYS = 32,	/**< De-personalization control keys */
	TAPI_SIM_SST_COOPERATIVE_NETWORK_LIST,			/**< Co-operative network list */
	TAPI_SIM_SST_SMS_STATUS_REPORTS,					/**< Short message status reports */
	TAPI_SIM_SST_NIA,					/**< Network's indication of alerting in the MS (NIA) */
	TAPI_SIM_SST_MO_SMS_CTRL_BY_SIM,		/**< Mobile-originated short message control by SIM */
	TAPI_SIM_SST_GPRS,					/**< GPRS */
	TAPI_SIM_SST_IMG,					/**< Image */
	TAPI_SIM_SST_SOLSA,					/**< Support of local service area */

	// service 41 ~ 48
	TAPI_SIM_SST_USSD_STR_DATA_OBJECT_SUPPORTED_IN_CALL_CTRL = 40,	/**< USSD string data object supported in call control */
	TAPI_SIM_SST_RUN_AT_CMD_CMD,							/**< RUN AT COMMAND command */
	TAPI_SIM_SST_USER_CTRLED_PLMN_SELECTOR_WACT,			/**< User controlled PLMN selector with Access technology */
	TAPI_SIM_SST_OPERATOR_CTRLED_PLMN_SELECTOR_WACT,		/**< Operator controlled PLMN selector with Access technology */
	TAPI_SIM_SST_HPLMN_SELECTOR_WACT,			/**< HPLMN selector with access technology */
	TAPI_SIM_SST_CPBCCH_INFO,					/**< CPBCCH information */
	TAPI_SIM_SST_INVESTIGATION_SCAN,				/**< Investigation scan */
	TAPI_SIM_SST_EXTENDED_CAPA_CONF_PARAMS,		/**< Extended capability configuration parameters */

	//	service 49 ~ 56
	TAPI_SIM_SST_MEXE = 48,				/**< MExE */
	TAPI_SIM_SST_RPLMN_LAST_USED_ACCESS_TECH,	/**< RPLMN last used access technology */
	TAPI_SIM_SST_PLMN_NETWORK_NAME,		/**< PLMN Network Name */
	TAPI_SIM_SST_OPERATOR_PLMN_LIST,		/**< Operator PLMN List */
	TAPI_SIM_SST_MBDN,					/**< Mailbox Dialling Numbers */
	TAPI_SIM_SST_MWIS,					/**< Message Waiting Indication Status */
	TAPI_SIM_SST_CFIS,					/**< Call Forwarding Indication Status */
	TAPI_SIM_SST_SPDI,					/**< Service Provider Display Information */
} TelSimSSTService_t;

/**
 * @brief Enumeration for the list of UST services in the SIM Service Table (USIM).
 * @since_tizen 2.3
 */
typedef enum {
	// service 1 ~ 8
	TAPI_SIM_UST_LOCAL_PB = 0,		/**< Local phone book */
	TAPI_SIM_UST_FDN,				/**< Fixed Dialing number */
	TAPI_SIM_UST_EXT2,				/**< Extension 2 */
	TAPI_SIM_UST_SDN,				/**< Service Dialing number */
	TAPI_SIM_UST_EXT3,				/**< Extension 3 */
	TAPI_SIM_UST_BDN,				/**< Barred Dialing numbers */
	TAPI_SIM_UST_EXT4,				/**< Extension 4 */
	TAPI_SIM_UST_OUTGOING_CALL_INFO,	/**< Outgoing call information */

	// service 9 ~ 16
	TAPI_SIM_UST_INCOMING_CALL_INFO = 8,		/**< Incoming call information */
	TAPI_SIM_UST_SMS,						/**< Short message storage */
	TAPI_SIM_UST_SMS_STATUS_REPORTS,			/**< Short message status reports */
	TAPI_SIM_UST_SMS_PARAMS,					/**< SMS parameters */
	TAPI_SIM_UST_AOC,						/**< Advice of charge */
	TAPI_SIM_UST_CCP,						/**< Capability configuration parameters */
	TAPI_SIM_UST_CELL_BROADCAST_MSG_ID,		/**< Cell broadcast message identifier */
	TAPI_SIM_UST_CELL_BROADCAST_MSG_ID_RANGES,	/**< Cell broadcast message identifier ranges */

	// service 17 ~ 24
	TAPI_SIM_UST_GID_LV1 = 16,						/**< Group identifier level 1 */
	TAPI_SIM_UST_GID_LV2,							/**< Group identifier level 2 */
	TAPI_SIM_UST_SPN,								/**< Service provider name */
	TAPI_SIM_UST_USER_CTRLED_PLMN_SELECTOR_WACT,		/**< User controlled PLMN selector with Access technology */
	TAPI_SIM_UST_MSISDN,											/**< MSISDN */
	TAPI_SIM_UST_IMG,											/**< Image */
	TAPI_SIM_UST_SOLSA,											/**< Support of local service area */
	TAPI_SIM_UST_ENHANCED_MULTI_LV_PRECEDENCE_PREEMPTION_SRVC,	/**< Enhanced multi-level precedence and pre-emption service */

	// service 25 ~ 32
	TAPI_SIM_UST_AUTO_ANSWER_FOR_EMLPP = 24,	/**< Automatic answer for EMLPP */
	TAPI_SIM_UST_RFU1,						/**< RFU */
	TAPI_SIM_UST_GSM_ACCESS,					/**< GSM access */
	TAPI_SIM_UST_DATA_DOWNLOAD_VIA_SMS_PP,	/**< Data download via SMS-PP */
	TAPI_SIM_UST_DATA_DOWNLOAD_VIA_SMS_CB,	/**< Data download via SMS-CB */
	TAPI_SIM_UST_CALL_CTRL,					/**< Call control by USIM */
	TAPI_SIM_UST_MO_SMS_CTRL,				/**< Mobile-originated short message control by USIM */
	TAPI_SIM_UST_RUN_AT_CMD_CMD,				/**< RUN AT COMMAND command */

	// service 33 ~ 40
	TAPI_SIM_UST_SHALL_BE_SET_TO_ONE = 32,	/**< Shall be set to 1 */
	TAPI_SIM_UST_ENABLED_SRVC_TABLE,			/**< Enabled service table */
	TAPI_SIM_UST_ACL,						/**< APN control list */
	TAPI_SIM_UST_DEPERSONALIZATION_CTRL_KEYS,	/**< De-personalization control keys */
	TAPI_SIM_UST_COOPERATIVE_NETWORK_LIST,		/**< Co-operative network list */
	TAPI_SIM_UST_GSM_SEC_CONTEXT,				/**< GSM security context */
	TAPI_SIM_UST_CPBCCH_INFO,					/**< CPBCCH information */
	TAPI_SIM_UST_INVESTIGATION_SCAN,				/**< Investigation scan */

	// service 41 ~ 48
	TAPI_SIM_UST_MEXE = 40,								/**< MExE */
	TAPI_SIM_UST_OPERATOR_CTRLED_PLMN_SELECTOR_WACT,		/**< Operator controlled PLMN selector with Access technology */
	TAPI_SIM_UST_HPLMN_SELECTOR_WACT,	/**< HPLMN selector with access technology */
	TAPI_SIM_UST_EXT5,					/**< Extension 5 */
	TAPI_SIM_UST_PLMN_NETWORK_NAME,		/**< PLMN Network Name */
	TAPI_SIM_UST_OPERATOR_PLMN_LIST,		/**< Operator PLMN List */
	TAPI_SIM_UST_MBDN,					/**< Mailbox Dialling Numbers */
	TAPI_SIM_UST_MWIS,					/**< Message Waiting Indication Status */

	// service 49 ~ 56
	TAPI_SIM_UST_CFIS = 48,						/**< Call Forwarding Indication Status */
	TAPI_SIM_UST_RPLMN_LAST_USED_ACCESS_TECH,	/**< RPLMN last used access technology */
	TAPI_SIM_UST_SPDI,							/**< Service Provider Display Information */
	TAPI_SIM_UST_MMS,							/**< Multi media messaging service */
	TAPI_SIM_UST_EXT8,							/**< Extension 8 */
	TAPI_SIM_UST_CALL_CTRL_ON_GPRS,				/**< Call control on GPRS by USIM */
	TAPI_SIM_UST_MMS_USER_CONNECTIVITY_PARAMS,	/**< MMS user connectivity parameters */
	TAPI_SIM_UST_NIA,							/**< Network's indication of alerting in the MS (NIA) */

	// service 57 ~ 64
	TAPI_SIM_UST_VGCS_GID_LIST = 56,		/**< VGCS group identifier List (EF-VGCS, EF-VGCSS) */
	TAPI_SIM_UST_VBS_GID_LIST,			/**< VBS group identifier List (EF-VBS, EF-VBSS) */
	TAPI_SIM_UST_PSEUDONYM,
	TAPI_SIM_UST_USER_CTRLED_PLMN_SELECTOR_IWLAN,			/**< User controlled PLMN selector for I-WLAN access */
	TAPI_SIM_UST_OPERATOR_CTRLED_PLMN_SELECTOR_IWLAN,		/**< Operator controlled PLMN selector for I-WLAN access */
	TAPI_SIM_UST_USER_CTRLED_WSID_LIST,
	TAPI_SIM_UST_OPERATOR_CTRLED_WSID_LIST,
	TAPI_SIM_UST_VGCS_SEC,
} TelSimUSTService_t;

/**
 * @brief Enumeration for the list of CST services in the CDMA Service Table.
 * @since_tizen 2.3
 */
typedef enum {
	// service 1 ~ 8
	TAPI_SIM_CDMA_ST_CHV_DISABLE = 0,	/**< CHV Disable Option */
	TAPI_SIM_CDMA_ST_ADN,			/**< Abbreviated Dialing number */
	TAPI_SIM_CDMA_ST_FDN,			/**< Fixed Dialing number */
	TAPI_SIM_CDMA_ST_SMS,			/**< Short message storage */
	TAPI_SIM_CDMA_ST_HRPD,			/**< HRPD */
	TAPI_SIM_CDMA_ST_EPB,			/**< Enhanced Phone Book */
	TAPI_SIM_CDMA_ST_MMD,			/**< Multimedia domain */
	TAPI_SIM_CDMA_ST_EUIMID,		/**< SF_EUIMID- based EUIMID */

	// service 9 ~ 16
	TAPI_SIM_CDMA_ST_MEID = 8,		/**< MEID */
	TAPI_SIM_CDMA_ST_EXT1,			/**< Extension 1 */
	TAPI_SIM_CDMA_ST_EXT2,			/**< Extension 2 */
	TAPI_SIM_CDMA_ST_SMSP,			/**< SMS parameters */
	TAPI_SIM_CDMA_ST_LND,			/**< Last number dialled */
	TAPI_SIM_CDMA_ST_SCP,			/**< Service Category Program for BC-SMS */
	TAPI_SIM_CDMA_ST_RFU1,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU2,			/**< RFU */

	// service 17 ~ 24
	TAPI_SIM_CDMA_ST_HSPN = 16,		/**< CDMA Home Service Provider Name */
	TAPI_SIM_CDMA_ST_SDN,			/**< Service Dialing number */
	TAPI_SIM_CDMA_ST_EXT3,			/**< Extension 3 */
	TAPI_SIM_CDMA_ST_3GPDSIP,		/**< 3GPD-SIP */
	TAPI_SIM_CDMA_ST_RFU3,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU4,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU5,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU6,			/**< RFU */

	// service 25 ~ 32
	TAPI_SIM_CDMA_ST_DDSMSB = 24,		/**< Data download by SMS broadcast */
	TAPI_SIM_CDMA_ST_DDSMSPP,		/**< Data download by SMS PP */
	TAPI_SIM_CDMA_ST_MENU,			/**< Menu Selection */
	TAPI_SIM_CDMA_ST_CALLC,			/**< Call Control */
	TAPI_SIM_CDMA_ST_PROACTIVE,		/**< Proactive RUIM */
	TAPI_SIM_CDMA_ST_AKA,			/**< AKA */
	TAPI_SIM_CDMA_ST_RFU7,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU8,			/**< RFU */

	// service 33 ~ 40
	TAPI_SIM_CDMA_ST_RFU9 = 32,		/**< RFU */
	TAPI_SIM_CDMA_ST_RFU10,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU11,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU12,			/**< RFU */
	TAPI_SIM_CDMA_ST_RFU13,			/**< RFU */
	TAPI_SIM_CDMA_ST_3GPDMIP,		/**< 3GPD- MIP */
	TAPI_SIM_CDMA_ST_BCMCS,			/**< BCMCS */
	TAPI_SIM_CDMA_ST_MMS,			/**< Multimedia messaging service */

	// service 41 ~ 48
	TAPI_SIM_CDMA_ST_EXT8 = 40,		/**< Extension 8 */
	TAPI_SIM_CDMA_ST_MMSUCP,		/**< MMS User Connectivity Parameters */
	TAPI_SIM_CDMA_ST_AA,			/**< Application Authentication */
	TAPI_SIM_CDMA_ST_GIL1,			/**< Group Identifier Level 1 */
	TAPI_SIM_CDMA_ST_GIL2,			/**< Group Identifier Level 2 */
	TAPI_SIM_CDMA_ST_DEPERSO,		/**< Depersonalisation control keys */
	TAPI_SIM_CDMA_ST_CNL			/**< Co-operative Network List */
} TelSimCdmaService_t;

/**
 * @brief Enumeration for the list of CST services in the CSIM Service Table. (CSIM)
 * @since_tizen 2.3
 */
typedef enum {
	// service 1 ~ 8
	TAPI_SIM_CSIM_ST_LOCAL_PHONEBOOK = 0,	/**< Local Phone book */
	TAPI_SIM_CSIM_ST_FDN,			/**< Fixed Dialing Numbers (FDN) */
	TAPI_SIM_CSIM_ST_EXT2,			/**< Extension 2 */
	TAPI_SIM_CSIM_ST_SDN,			/**< Service Dialing Numbers (SDN) */
	TAPI_SIM_CSIM_ST_EXT3,			/**< Extension 3 */
	TAPI_SIM_CSIM_ST_SMS,			/**< Short Message Storage (SMS) */
	TAPI_SIM_CSIM_ST_SMSP,			/**< Short Message Parameters */
	TAPI_SIM_CSIM_ST_HRPD,			/**< HRPD */

	// service 9 ~ 16
	TAPI_SIM_CSIM_ST_SCP = 8,		/**< Service Category Program for BC-SMS */
	TAPI_SIM_CSIM_ST_HSPN,			/**< CDMA Home Service Provider Name */
	TAPI_SIM_CSIM_ST_DDSMSB,		/**< Data Download via SMS Broadcast */
	TAPI_SIM_CSIM_ST_DDSMSPP,		/**< Data Download via SMS-PP */
	TAPI_SIM_CSIM_ST_CALLC,			/**< Call Control */
	TAPI_SIM_CSIM_ST_3GPDSIP,		/**< 3GPD-SIP */
	TAPI_SIM_CSIM_ST_3GPDMIP,		/**< 3GPD-MIP */
	TAPI_SIM_CSIM_ST_AKA,			/**< AKA */

	// service 17 ~ 24
	TAPI_SIM_CSIM_ST_IP_LCS = 16,		/**< IP-based Location Services (LCS) */
	TAPI_SIM_CSIM_ST_BCMCS,			/**< BCMCS */
	TAPI_SIM_CSIM_ST_MMS,			/**< Multimedia Messaging Service (MMS) */
	TAPI_SIM_CSIM_ST_EXT8,			/**< Extension 8 */
	TAPI_SIM_CSIM_ST_MMSUCP,		/**< MMS User Connectivity Parameters */
	TAPI_SIM_CSIM_ST_AA,			/**< Application Authentication */
	TAPI_SIM_CSIM_ST_GIL1,			/**< Group Identifier Level 1 */
	TAPI_SIM_CSIM_ST_GIL2,			/**< Group Identifier Level 2 */

	// service 25 ~ 32
	TAPI_SIM_CSIM_ST_DEPERSO = 24,		/**< De-Personalization Control Keys */
	TAPI_SIM_CSIM_ST_CNL,			/**< Cooperative Network List */
	TAPI_SIM_CSIM_ST_OCI,			/**< Outgoing Call Information (OCI) */
	TAPI_SIM_CSIM_ST_ICI,			/**< Incoming Call Information (ICI) */
	TAPI_SIM_CSIM_ST_EXT5,			/**< Extension 5 */
	TAPI_SIM_CSIM_ST_MM_STORAGE,		/**< Multimedia Storage */
	TAPI_SIM_CSIM_ST_IMG,			/**< Image (EFIMG) */
	TAPI_SIM_CSIM_ST_EST,			/**< Enabled Services Table */

	// service 33 ~ 41
	TAPI_SIM_CSIM_ST_CCP = 32,		/**< Capability Configuration Parameters (CCP) */
	TAPI_SIM_CSIM_ST_EUIMIDl,		/**< SF_EUIMID-based EUIMID */
	TAPI_SIM_CSIM_ST_3GPD_EXT,		/**< Messaging and 3GPD Extensions */
	TAPI_SIM_CSIM_ST_ROOT_CERTI,		/**< Root Certificates */
	TAPI_SIM_CSIM_ST_WAP,			/**< WAP Browser */
	TAPI_SIM_CSIM_ST_JAVA,			/**< Java */
	TAPI_SIM_CSIM_ST_RSVD_CDG1,		/**< Reserved for CDG */
	TAPI_SIM_CSIM_ST_RSVD_CDG2,		/**< Reserved for CDG */
	TAPI_SIM_CSIM_ST_IPV6 = 40,		/**< IPv6 */
} TelSimCsimService_t;

typedef enum {
	TAPI_SIM_IST_PCSCF_ADDR = 0,		/**< P-CSCF address */
	TAPI_SIM_IST_GBA,			/**< Generic Bootstrapping Architecture */
	TAPI_SIM_IST_HTTP_DIGEST,		/**< HTTP Digest */
	TAPI_SIM_IST_GBA_LOCAL_KEY,		/**< GBA-based Local Key Establishment Mechanism */
	TAPI_SIM_IST_PCSCF_LOCAL_BREAK_OUT,	/**< Support of P-CSCF discovery for IMS Local Break Out */
	TAPI_SIM_IST_SMS,			/**< Short Message Storage */
	TAPI_SIM_IST_SMSR,			/**< Short Message Status Reports */
	TAPI_SIM_IST_SM_OVER_IP,		/**< Support for SM-over-IP including data download via SMS-PP as defined in TS 31.111 [31] */

	TAPI_SIM_IST_COMMUNICATION_CONTROL = 8,	/**< Communication Control for IMS by ISIM */
	TAPI_SIM_IST_ACCESS_TO_IMS,		/**< Support of UICC access to IMS */
	TAPI_SIM_IST_URI_SUPPORT,		/**< URI support by UICC */
} TelSimIsimService_t;

typedef enum {
	TAPI_SIM_CDMA_SVC_TABLE = 0,
	TAPI_SIM_CSIM_SVC_TABLE,
	TAPI_SIM_MAX_SVC_TABLE
} TelSimCdmaSvcTable_t;

typedef enum {
	TAPI_SIM_PCSCF_TYPE_FQDN, /**< Fully Qualified Domain Name */
	TAPI_SIM_PCSCF_TYPE_IPV4, /**< IPv4 */
	TAPI_SIM_PCSCF_TYPE_IPV6, /**< IPv6 */
} TelSimPcscfType;

typedef struct {
	char service[TAPI_SIM_SST_SERVICE_CNT_MAX];	/**< TBD */	// should access with 'enum TelSimSSTService_t' as index
} TelSimSST_t;

typedef struct {
	char service[TAPI_SIM_UST_SERVICE_CNT_MAX];	/**< TBD */	// should access with 'enum TelSimUSTService_t' as index
} TelSimUST_t;

typedef struct {
	TelSimCdmaSvcTable_t cdma_svc_table;	/* Member of service union is accessed based on the corresponding value of TelSimCdmaSvcTable_t */
	union {
		char cdma_service[TAPI_SIM_CDMA_ST_SERVICE_CNT_MAX];	// should access with 'enum TelSimCdmaService_t' as index
		char csim_service[TAPI_SIM_CSIM_ST_SERVICE_CNT_MAX];	// should access with 'enum TelSimCsimService_t' as index
	} service;
} TelSimCST_t;

typedef struct {
	char service[TAPI_SIM_IST_SERVICE_CNT_MAX];
} TelSimIST_t;

typedef struct {
	TelSimCardType_t sim_type;	/**< TBD */
	union {
		TelSimSST_t sst;	/**< TBD */
		TelSimUST_t ust;	/**< TBD */
		TelSimCST_t cst;
	} table;	/**< TBD */
} TelSimServiceTable_t;

/**
 * @brief The structure type defining data for IMSI information.
 * @since_tizen 2.3
 */
typedef struct {
	char szMcc[3 + 1]; /**< Mobile country code */
	char szMnc[3 + 1]; /**< Mobile network code */
	char szMsin[10 + 1]; /**< Mobile station identification number */
} TelSimImsiInfo_t;

/**
 * @brief The structure type defining data for ECC information of the GSM/USIM/CDMA SIM.
 * @since_tizen 2.3
 */
typedef struct {
	char name[30+1]; /**< Applicable only for USIM(3G) SIM */
	char number[6+1];
	TelSimEccEmergencyServiceInfo_t category;/**< Applicable only for USIM(3G) SIM */
}TelSimEcc_t;

typedef struct {
	int ecc_count;	/**< TBD */
	TelSimEcc_t list[15];	/**< TBD */
}TelSimEccList_t;

typedef struct {
	int icc_length; /**< Integrated Circuit Card number length */
	char icc_num[TAPI_SIM_ICCID_LEN_MAX]; /**< Integrated Circuit Card number */
} TelSimIccIdInfo_t;

typedef struct {
	int b_cphs;	/**< TBD */
	int rec_index; /**< Index which stands for the location where the record is saved in SIM */
	int profile_num; /**< SIM profile index */
	TelSimMailboxType_t mb_type;	/**< TBD */
	int alpha_id_max_len; /**< Alpha max length in SIM - READ ONLY */
	char alpha_id[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN + 1]; /**< Alpha Identifier */
	TelSimTypeOfNum_t ton; /**< Type Of Number */
	TelSimNumberingPlanIdentity_t npi; /**< Number Plan Identity */
	char num[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1]; /**< Dialing Number/SSC String */
	unsigned char cc_id; /**< Capability/Configuration Identifier */
	unsigned char ext1_id; /**< Extension 1 Record Identifier */
}TelSimMailBoxNumber_t;

typedef struct {
	int count;	/**< TBD */
	TelSimMailBoxNumber_t list[TAPI_SIM_MSP_CNT_MAX*5];	/**< TBD */ //max is 10
}TelSimMailboxList_t;

typedef struct {
	int rec_index;	/**< TBD */
	unsigned char msp_num; /**< MSP number*/
	unsigned char cfu_status; /**< Call forwarding unconditional indication status */
	TelSimTypeOfNum_t ton; /**< TON */
	TelSimNumberingPlanIdentity_t npi; /**< NPI */
	char cfu_num[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1];/**< Dialing Number/SSC String */
	unsigned char cc2_id; /**< Capability/Configuration 2 Record Identifier */
	unsigned char ext7_id; /**< Extension 7 Record Identifier */
}TelSimCfis_t;

typedef struct {
	int profile_count;	/**< TBD */
	TelSimCfis_t cf[TAPI_SIM_MSP_CNT_MAX];	/**< TBD */
}TelSimCfisList_t;

typedef struct {
	int b_line1; /**< CallForwardUnconditionalLine 1 */
	int b_line2; /**< CallForwardUnconditionalLine 2 */
	int b_fax; /**< CallForwardUnconditional FAX */
	int b_data; /**< CallForwardUnconditional data */
}TelSimCphsCf_t;

typedef struct {
	int b_cphs;	/**< TBD */
	TelSimCfisList_t cf_list;	/**< TBD */
	TelSimCphsCf_t cphs_cf;	/**< TBD */
}TelSimCallForwardingResp_t;

typedef struct {
	int b_cphs;	/**< TBD */
	union {
		TelSimCfis_t cf;	/**< TBD */
		TelSimCphsCf_t cphs_cf;	/**< TBD */
	} cf_data_u;	/**< TBD */
}TelSimCallForwardingReq_t;

typedef struct {
	int rec_index;	/**< TBD */
	unsigned char indicator_status; /**< Indicator Type */
	int voice_count; /**< VoiceMail Count */
	int fax_count; /**< FAX Count */
	int email_count; /**< Email Count */
	int other_count; /**< Other Count */
	int video_count; /**< VideoMail Count */
}TelSimMwis_t;

typedef struct {
	int profile_count;	/**< TBD */
	TelSimMwis_t mw[TAPI_SIM_MSP_CNT_MAX];	/**< TBD */
}TelSimMwisList_t;

typedef struct {
	int b_voice1; /**< VoiceMsgLine1 message waiting flag */
	int b_voice2; /**< VoiceMsgLine2 message waiting flag */
	int b_fax; /**< FAX message waiting flag */
	int b_data; /**< Data message waiting flag */
}TelSimCphsMw_t;

typedef struct {
	int b_cphs;	/**< TBD */
	TelSimMwisList_t mw_list;	/**< TBD */
	TelSimCphsMw_t cphs_mw;	/**< TBD */
}TelSimMessageWaitingResp_t;

typedef struct {
	int b_cphs;	/**< TBD */
	union {
		TelSimMwis_t mw;	/**< TBD */
		TelSimCphsMw_t cphs_mw;	/**< TBD */
	} mw_data_u;	/**< TBD */
}TelSimMessageWaitingReq_t;

/**
 * @brief The structure type for MSISDN information of the GSM/CDMA SIM.
 * @since_tizen 2.3
 */
typedef struct {
	char num[TAPI_SIM_MSISDN_DIALING_NUMBER_LEN + 1]; /**< MSISDN number. If it does not exist, a null string will be returned */
	char name[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN + 1]; /**< MSISDN name. If it does not exist, a null string will be returned. Not applicable for CDMA */
} TelSimSubscriberInfo_t;

typedef struct {
	int count;	/**< TBD */
	TelSimSubscriberInfo_t list[3];	/**< TBD */ //max is 3
}TelSimMsisdnList_t;

typedef struct {
	char plmn[6+1];	/**< TBD */
	int b_umts;	/**< TBD */
	int b_gsm;	/**< TBD */
}TelSimOplmnwact_t;

typedef struct {
	int count;	/**< TBD */
	TelSimOplmnwact_t list[TAPI_SIM_OPLMNWACT_LEN_MAX];	/**< TBD */ //max is 100
}TelSimOplmnwactList_t;

typedef struct {
	unsigned char display_condition; /**< Display condition (1 byte) */
	unsigned char spn[TAPI_SIM_NET_FULL_NAME_MAX_LEN + 1]; /**< SPN */
}TelSimSpn_t;

typedef struct {
	unsigned char full_name[TAPI_SIM_NET_FULL_NAME_MAX_LEN + 1];	/**< TBD */
	unsigned char short_name[TAPI_SIM_NET_SHORT_NAME_MAX_LEN + 1];	/**< TBD */
}TelSimCphsNetName_t;

typedef struct {
	int GroupIdentifierLen;	/**< Group identifier length */
	unsigned char szGroupIdentifier[TAPI_SIM_GROUP_IDENTIFIER_LEN_MAX]; /**< Group identifier */
}TelSimGid_t;

/**
 * @details The structure type for authentication request data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimAuthenticationType_t auth_type; /**< Authentication type */
	int rand_length; /**< The length of RAND */
	int autn_length; /**< The length of AUTN. It is not used in case of GSM AUTH */
	char rand_data[TAPI_SIM_AUTH_MAX_REQ_DATA_LEN]; /**< RAND data */
	char autn_data[TAPI_SIM_AUTH_MAX_REQ_DATA_LEN]; /**< AUTN data. It is not used in case of GSM AUTH */
} TelSimAuthenticationData_t;

/**
 * @details The structure type for authentication result data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimAuthenticationType_t auth_type; /**< Authentication type */
	TelSimAuthenticationResult_t auth_result; /**< Authentication result */
	int resp_length; /**< Response length. IMS and 3G case, it stands for RES_AUTS. GSM case, it stands for SRES */
	char resp_data[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< Response data. IMS and 3G case, it stands for RES_AUTS. GSM case, it stands for SRES */
	int authentication_key_length; /**< The length of the authentication key, Kc */
	char authentication_key[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< The data of the authentication key, Kc */
	int cipher_length; /**< The length of the cipher key */
	char cipher_data[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< Cipher key */
	int integrity_length; /**< The length of the integrity key */
	char integrity_data[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< Integrity key */
} TelSimAuthenticationResponse_t;


/**
 * @brief The structure type for information about PIN data.
 * @details The SIM PIN data. For PIN handling (Change, UnBlock) & for Type of PIN information.
 */
typedef struct {
	TelSimPinType_t type; /**< PIN type */
	unsigned char* pw; /**< PIN code */
	unsigned int pw_len; /**< PIN code length */
} TelSimSecPw_t;

/**
 * @brief The structure type for PIN information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimPinType_t type; /**< Specifies the PIN or PUK type */
	int retry_count; /**< Number of attempts remaining for PIN/PUK verification */
} TelSimSecResult_t;

/**
 * @details The structure type used to enable/disable facility.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimLockType_t lock_type; /**< Facility type */
	unsigned char *pw; /**< Password */
	int pw_len; /**< Password length */
} TelSimFacilityPw_t;

typedef struct {
	TelSimLockType_t type; /**< Specifies the PIN or PUK type */
	int retry_count; /**< Number of attempts remaining for PIN/PUK verification */
} TelSimFacilityResult_t;

typedef struct {
	TelSimLockType_t type;	/**< TBD */
	TelSimFacilityStatus_t f_status;	/**< TBD */
}TelSimFacilityInfo_t;

/**
 * @details The structure type for information about LOCK_TYPE.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimLockType_t lock_type; /**< Lock type */
	TelSimLockStatus_t lock_status; /**< Lock key */
	int retry_count; /**< Retry counts */
} TelSimLockInfo_t;

/**
 * @brief The structure type for APDU.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned short apdu_len;	/**< TBD */
	unsigned char* apdu;	/**< TBD */
} TelSimApdu_t;

/**
 * @brief The structure type for the response of sending APDU.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned short apdu_resp_len;	/**< TBD */
	unsigned char apdu_resp[TAPI_SIM_APDU_MAX_LEN];	/**< TBD */
} TelSimApduResp_t;

/**
 * @brief The structure type for the response of sending APDU.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned short atr_resp_len;	/**< TBD */
	unsigned char atr_resp[TAPI_SIM_APDU_MAX_LEN];	/**< TBD */
} TelSimAtrResp_t;

/**
 * @brief The structure type for available optional CPHS SIM files.
 * @since_tizen 2.3
 */
typedef struct {
	/* Byte 2 - bit1 & 2*/
	int bCustomerServiceProfile; /**< Customer Service Profile (CSP) */
	/* Byte 2 - bit3 & 4*/
	int bServiceStringTable; /**< Service String Table (SST) */
	/* Byte 2 - bit5 & 6*/
	int bMailBoxNumbers; /**< MailBoxNumbers */
	/* Byte 2 - bit7 & 8*/
	int bOperatorNameShortForm; /**< Short form of operator name */
	/* Byte 3 - bit1 & 2*/
	int bInformationNumbers; /**< Information numbers */
} TelSimCphsServiceTable_t;

/*
 These requirements are additional to the GSM 900 and DCS1800 recommendations.
 They apply to all products which are to be compliant with the CPHS specification.

 In addition to those SIM storage fields previously defined in DCS1800 to support
 existing MS features and services, the Association has defined the following fields  :-

 1)	Call Forwarding flag					(mandatory)
 2)	Voice message waiting flag				(mandatory)
 3)	PLMN operator name						(mandatory)
 4)	Customer Service Profile (CSP)			(optional)
 5)	CPHS Information						(mandatory)
 6)	Mailbox Numbers							(optional)
 7)	Information Numbers						(optional)

 */

/*
 DATA FIELD - 6F 16: CPHS INFORMATION
 Access Conditions:
 READ	CHV1
 UPDATE	ADM
 */
/**
 * @brief The structure type for CPHS information data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimCphsPhaseType_t CphsPhase; /**< CPHS phase type */
	TelSimCphsServiceTable_t CphsServiceTable; /**< CPHS service table */
} TelSimCphsInfo_t;

/*
 DATA FIELD -6F 19: Information Numbers
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */
/**
 * @brief The structure type for CPHS information numbers data.
 * @since_tizen 2.3
 */
typedef struct {
	int bUsed; /**< SIM CPHS index level one */
	unsigned char AlphaIdLength; /**< Length of alpha identifier */

	TelSimCphsIndexLevelIndicator_t IndexLevelIndicator; /**< SIM CPHS index level one */
	int PremiumServiceIndicator; /**< SIM CPHS index level one */
	int NetworkSpecificIndicator; /**< SIM CPHS index level one */
	unsigned char Alpha_id[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN + 1]; /**< Alpha Identifier */

	unsigned long DiallingnumLength; /**< Length of BCD number/SSC contents */
	TelSimTypeOfNum_t TypeOfNumber; /**< TON */
	TelSimNumberingPlanIdentity_t NumberingPlanIdentity; /**< NPI */
	char DiallingNum[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1]; /**< Dialing Number/SSC String */
	unsigned char Ext1RecordId; /**< Extension 1 Record Identifier */
} TelSimCphsInformationNum_t;

/*
 DATA FIELD- 6F 15: Customer Service Profile (Storing a list of service options which are relevant to that specific customer)
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */
/**
 * @brief The structure type for CPHS service call offering information.
 * @since_tizen 2.3
 */
typedef struct {
	int bCallForwardingUnconditional; /**< CallForwarding Unconditional */
	int bCallForwardingOnUserBusy; /**< CallForwarding On UserBusy */
	int bCallForwardingOnNoReply; /**< CallForwarding On NoReply */
	int bCallForwardingOnUserNotReachable; /**< CallForwarding On User Not Reachable */
	int bCallTransfer; /**< Call Transfer */
} TelSimCphsServiceCallOffering_t;

/**
 * @brief The structure type for CPHS service call restriction information.
 * @since_tizen 2.3
 */
typedef struct {
	int bBarringOfAllOutgoingCalls; /**< Barring Of All Outgoing Calls */
	int bBarringOfOutgoingInternationalCalls; /**< Barring Of Outgoing International Calls */
	int bBarringOfOutgoingInternationalCallsExceptHplmn; /**< Barring Of Outgoing International Calls Except HPLMN */
	int bBarringOfAllIncomingCallsRoamingOutsideHplmn; /**< Barring Of All Incoming Calls Roaming Outside HPLMN */
	int bBarringOfIncomingCallsWhenRoaming; /**< Barring Of Incoming Calls When Roaming */
} TelSimCphsServiceCallRestriction_t;

/**
 * @brief The structure type for CPHS service SS information.
 * @since_tizen 2.3
 */
typedef struct {
	int bMultiPartyService; /**< MultiPartyService */
	int bClosedUserGroup; /**< ClosedUserGroup */
	int bAdviceOfCharge; /**< AdviceOfCharge */
	int bPreferentialClosedUserGroup; /**< PreferentialClosedUserGroup*/
	int bClosedUserGroupOutgoingAccess; /**< ClosedUserGroupOutgoingAccess */
} TelSimCphsServiceOtherSupplimentaryService_t;

/**
 * @brief The structure type for CPHS service call complete information.
 * @since_tizen 2.3
 */
typedef struct {
	int bCallHold; /**< Call Hold */
	int bCallWaiting; /**< Call Waiting */
	int bCompletionOfCallToBusySubscriber; /**< Completion Of Call To Busy Subscriber */
	int bUserUserSignalling; /**< User User Signaling */
} TelSimCphsServiceCallComplete_t;

/**
 * @brief The structure type for CPHS service teleservices information.
 * @since_tizen 2.3
 */
typedef struct {
	int bShortMessageMobileOriginated; /**< Short Message Mobile Originated */
	int bShortMessageMobileTerminated; /**< Short Message Mobile Terminated */
	int bShortMessageCellBroadcast; /**< Short Message Cell Broadcast */
	int bShortMessageReplyPath; /**< Short  Message Reply Path */
	int bShortMessageDeliveryConf; /**< Short Message Delivery Conf */
	int bShortMessageProtocolIdentifier; /**< Short Message Protocol Identifier */
	int bShortMessageValidityPeriod; /**< Short Message Validity Period */
} TelSimCphsServiceTeleservices_t;

/**
 * @brief The structure type for CPHS alternative line service information.
 * @since_tizen 2.3
 */
typedef struct {
	int bAlternativeLineService; /**< Alternative Line Service */
} TelSimCphsServiceCphsTeleservices_t;

/**
 * @brief The structure type for CPHS string service table information.
 * @since_tizen 2.3
 */
typedef struct {
	int bStringServiceTable; /**< String Service Table */
} TelSimCphsServiceCphsFeatures_t;

/**
 * @brief The structure type for CPHS service number identifier information.
 * @since_tizen 2.3
 */
typedef struct {
	int bCallingLineIdentificationPresent; /**< Calling Line Identification Present */
	int bConnectedLineIdentificationRestrict; /**< Connected Line Identification Restrict */
	int bConnectedLineIdentificationPresent; /**< Connected Line Identification Present */
	int bMaliciousCallIdentifier; /**< Malicious Call Identifier */
	int bCallingLineIdentificationSend; /**< Calling Line Identification Send */
	int bCallingLineIdentificationBlock; /**< Calling Line Identification Block */
} TelSimCphsServiceNumberIdentifier_t;

/**
 * @brief The structure type for CPHS service phase services information.
 * @since_tizen 2.3
 */
typedef struct {
	int bMenuForGprs; /**< Menu For GPRS */
	int bMenuForHighSpeedCsd; /**< Menu For HighSpeedCsd */
	int bMenuForVoiceGroupCall; /**< Menu For VoiceGroupCall */
	int bMenuForVoiceBroadcastService; /**< Menu For VoiceBroadcastService */
	int bMenuForMultipleSubscriberProfile; /**< Menu For MultipleSubscriberProfile */
	int bMenuForMultipleBand; /**< Menu For MultipleBand */
} TelSimCphsServicePhaseServices_t;

/**
 * @brief The structure type for CPHS value added service information.
 * @since_tizen 2.3
 */
typedef struct {
	int bRestrictMenuForManualSelection; /**< RestrictMenu For ManualSelection */
	int bRestrictMenuForVoiceMail; /**< RestrictMenu For VoiceMail */
	int bRestrictMenuForMoSmsAndPaging; /**< RestrictMenu For MoSmsAndPaging */
	int bRestrictMenuForMoSmsWithEmialType; /**< RestrictMenu For MoSmsWithEmailType */
	int bRestrictMenuForFaxCalls; /**< RestrictMenu For FaxCalls */
	int bRestrictMenuForDataCalls; /**< RestrictMenu For DataCalls */
	int bRestrictMenuForChangeLanguage; /**< RestrictMenu For ChangeLanguage */
} TelSimCphsServiceValueAddedServices_t;

/**
 * @brief The structure type for CPHS service information number data.
 * @since_tizen 2.3
 */
typedef struct {
	int bInformationNumbers; /**< Information Numbers */
} TelSimCphsServiceInformationNumbers_t;

/**
 * @brief The structure type for CPHS service profile entry information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimCphsCustomerServiceGroup_t CustomerServiceGroup; /**< Customer service group */
	union {
		TelSimCphsServiceCallOffering_t CallOffering; /**< Call offering */
		TelSimCphsServiceCallRestriction_t CallRestriction; /**< Call restriction */
		TelSimCphsServiceOtherSupplimentaryService_t OtherSuppServices; /**< Other SS services */
		TelSimCphsServiceCallComplete_t CallComplete; /**< Call complete */
		TelSimCphsServiceTeleservices_t Teleservices; /**< Teleservices */
		TelSimCphsServiceCphsTeleservices_t CphsTeleservices; /**< CPHS teleservices */
		TelSimCphsServiceCphsTeleservices_t CphsFeatures; /**< CPHS features */
		TelSimCphsServiceNumberIdentifier_t NumberIdentifiers; /**< Number identifiers */
		TelSimCphsServicePhaseServices_t PhaseServices; /**< Phase services */
		TelSimCphsServiceValueAddedServices_t ValueAddedServices; /**< Value added services */
		TelSimCphsServiceInformationNumbers_t InformationNumbers; /**< Information numbers */
	} u;
} TelSimCphsCustomerServiceProfileEntry_t;

/**
 * @brief The structure type for CPHS service profile information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimCphsCustomerServiceProfileEntry_t ServiceProfileEntry[TAPI_SIM_CPHS_CUSTOMER_SERVICE_PROFILE_ENTRY_COUNT_MAX]; /**< Service profile entry */
} TelSimCphsCustomerServiceProfileInfo_t;

/**
 * @brief The structure type for dynamic flag selected line information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimDynamicFlagsSelectedLineId_t DynamicFlags; /**< Dynamic flags information */
} TelSimDynamicFlagsInfo_t;

/**
 * @brief The structure type for dynamic flag selected line information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimDynamic2FlagAlsStatus_t Dynamic2Flag; /**< Dynamic flags status */
} TelSimDynamic2FlagsInfo_t;

/**
 * @brief The structure type for phone book availability of the current SIM.
 * @since_tizen 2.3
 */
typedef struct {
	int b_fdn; /**< Fixed Dialing Number */
	int b_adn; /**< SIM - ADN(2G phonebook, Under DF phonebook */
	int b_sdn; /**< Service Dialing Number */
	int b_3g; /**< USIM - 3G phonebook */
	int b_aas; /**< Additional number Alpha String phonebook */
	int b_gas; /**< Grouping information Alpha String phonebook */
} TelSimPbList_t;

/**
 * @brief The structure type for phone book status of the current SIM.
 * @since_tizen 2.3
 */
typedef struct {
	int init_completed;	/**< TBD */
	TelSimPbList_t pb_list;	/**< TBD */
} TelSimPbStatus_t;

typedef struct {
	TelSimPbType_t phonebook_type;	/**< TBD */
	unsigned short index;	/**< TBD */
	unsigned short next_index;	/**< TBD */ //this field is not used in the add/update case

	unsigned char name[TAPI_SIM_PB_RECORD_NAME_MAX_LEN+1];	/**< TBD */
	TelSimTextEncrypt_t dcs;	/**< TBD */

	unsigned char number[TAPI_SIM_PB_RECORD_NUMBER_MAX_LEN+1];	/**< TBD */
	TelSimTypeOfNum_t ton;	/**< TBD */

	/* following field is valid in only USIM*/
	unsigned char sne[TAPI_SIM_PB_RECORD_NAME_MAX_LEN+1];	/**< TBD */
	TelSimTextEncrypt_t sne_dcs;	/**< TBD */
	unsigned char anr1[TAPI_SIM_PB_RECORD_NUMBER_MAX_LEN+1];	/**< TBD */
	TelSimTypeOfNum_t anr1_ton;	/**< TBD */
	unsigned char anr2[TAPI_SIM_PB_RECORD_NUMBER_MAX_LEN+1];	/**< TBD */
	TelSimTypeOfNum_t anr2_ton;	/**< TBD */
	unsigned char anr3[TAPI_SIM_PB_RECORD_NUMBER_MAX_LEN+1];	/**< TBD */
	TelSimTypeOfNum_t anr3_ton;	/**< TBD */

	unsigned char email1[TAPI_SIM_PB_RECORD_EMAIL_MAX_LEN+1];	/**< TBD */
	unsigned char email2[TAPI_SIM_PB_RECORD_EMAIL_MAX_LEN+1];	/**< TBD */
	unsigned char email3[TAPI_SIM_PB_RECORD_EMAIL_MAX_LEN+1];	/**< TBD */
	unsigned char email4[TAPI_SIM_PB_RECORD_EMAIL_MAX_LEN+1];	/**< TBD */

	unsigned short group_index;	/**< TBD */ //GRP
	unsigned short pb_control;	/**< TBD */ //PBC
} TelSimPbRecord_t;

/**
 * @brief The structure type for phone book storage count information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimPbType_t StorageFileType; /**< Storage file type */
	unsigned short TotalRecordCount; /**< Total record count */
	unsigned short UsedRecordCount; /**< Used record count */
} TelSimPbStorageInfo_t;

/**
 * @brief The structure type for phone book entry information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimPbType_t StorageFileType; /**< Storage file type */
	unsigned short PbIndexMin; /**< Phone book minimum index */
	unsigned short PbIndexMax; /**< Phone book maximum index */
	unsigned short PbNumLenMax; /**< Phone number's maximum length */
	unsigned short PbTextLenMax; /**< Text's maximum length */
	unsigned short PbUsedCount; /**< Phone book used record count */
} TelSimPbEntryInfo_t;

/**
 * @brief The structure type for 3G phone book capability information.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimPb3GFileType_t field_type; /**< 3G phonebook file type */
	unsigned short index_max; /**< Max index */
	unsigned short text_max; /**< Max text length */
	unsigned short used_count; /**< Used record count */
} TelSimPb3GFileTypeCapabiltyInfo_t;

/**
 * @brief The structure type for SIM PHONEBOOK & ITS CAPABILITIES information.
 * @details It refers to EF_PBR.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned short FileTypeCount; /**< Phonebook file type count */
	TelSimPb3GFileTypeCapabiltyInfo_t FileTypeInfo[TAPI_SIM_PB_3G_FILE_MAX_COUNT]; /**< Phonebook file type information */
} TelSimPbCapabilityInfo_t;

/**
 * @brief The structure type for phonebook contact change informarion.
 * @since_tizen 2.4
 */
typedef struct {
	TelSimPbType_t pb_type; /**< Storage file type */
	unsigned short index; /**< Changed index */
	TelSimPbOp_t operation; /**< Phonebook operation */
} TelSimPbContactChangeInfo_t;



/* SAP (SIM Access Profile) related interface structures and enum */
/**
 * @brief The structure type for SAP ATR response data information.
 * @since_tizen 2.3
 */
typedef struct {
	int atr_len; /**< SAP ATR response data length */
	unsigned char atr_data[TAPI_SIM_SAP_ATR_DATA]; /**< SAP ATR response data */
} TelSapAtrInfo_t;

/* SAP transfer APDU request */

/**
 * @brief The structure type for SAP APDU data information.
 * @since_tizen 2.3
 */
typedef struct {
	int apdu_len; /**< SAP APDU length */
	unsigned char apdu_data[TAPI_SIM_APDU_MAX_LEN]; /**< SAP APDU data */
} TelSapApduData_t;

/**
 * @brief Enumeration for TelSimPowerState_t.
 * @details This enumeration defines the power state of the SIM.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SIM_POWER_OFF = 0x00, /**< OFF */
	TAPI_SIM_POWER_ON = 0x01, /**< ON */
	TAPI_SIM_POWER_UNSPECIFIED = 0xFF /**< Unspecified */
} TelSimPowerState_t;

#ifdef __cplusplus
}
#endif

#endif // _TELSIM_H_
/**
 * @}
 */
