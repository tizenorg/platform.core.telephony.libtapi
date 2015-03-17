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
 * @file TelNetwork.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_NETWORK
 * @{
 */

#ifndef _TEL_NETWORK_H_
#define _TEL_NETWORK_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Definition for the maximum network order.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_ORDER_MAX					5

/**
 * @brief Definition for the maximum network list.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_LIST_MAX					15

/**
 * @brief Definition for the maximum length of the network PLMN.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_PLMN_LEN_MAX				6

/**
 * @brief Definition for the maximum length of the network name.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_NAME_LEN_MAX				40

/**
 * @brief Definition for the maximum length of the network service provider's name.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_SPN_LEN_MAX				20
/**
 * @brief Definition for the maximum records of the preferred PLMN list.
 * @since_tizen 2.3
 */
#define TAPI_PREFERRED_PLMN_RECORDS_MAX			150

/**
 * @brief Definition for the maximum network display info elements.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_DISPLAY_INFO_ELEMENTS		4

/**
 * @brief Definition for the maximum length of the network home zone tag.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_HOMEZONE_TAG_MAX_LEN		13

/**
 * @brief Definition for the maximux network CS display index.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_CS_DISP_INDEX				0

/**
 * @brief Definition for the maximum network PS display index.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_PS_DISP_INDEX				1

/**
 * @brief Definition for the maximum network SVC display index.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_SVC_DISP_INDEX				2

/**
 * @brief Definition for the maximum network roaming display index.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_ROAM_DISP_INDEX			3

/**
 * @brief Definition for the maximum number of subscriber services.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_SUBS_SVC_NUM_MAX			6

/**
 * @brief Definition for the maximum length of the subscriber number.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_SUBS_NUM_LEN_MAX			40

/**
 * @brief Definition for the maximum length of the subscriber name.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_SUBS_ALPHA_LEN_MAX			16

/**
 * @brief Definition for the maximum GERAN neighboring cell list.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_GERAN_NEIGHBORING_CELL_LIST_MAX 6

/**
 * @brief Definition for the maximum UMTS neighboring cell list.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_UMTS_NEIGHBORING_CELL_LIST_MAX 24

/* This value defines the network mode */
/**
 * @brief Definition for the "set network mode to automatic" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_AUTO		0x00

/**
 * @brief Definition for "set network mode to GSM" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_GSM		0x01

/**
 * @brief Definition for the "set network mode to WCDMA" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_WCDMA		0x02

/**
 * @brief Definition for the "set network mode to 1XRTT" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_1XRTT		0x04

/**
 * @brief Definition for the "set network mode to LTE" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_LTE		0x08

/**
 * @brief Definition for the "set network mode to EVDO" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_EVDO		0x10

/**
 * @brief Definition for the "set network mode to CDMA" value.
 * @since_tizen 2.3
 */
#define TAPI_NETWORK_MODE_CDMA		0x14

/**
 * @brief Enumeration for the network selection modes.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_SELECTIONMODE_AUTOMATIC,	/**< Network selection is in the Automatic selection mode */
	TAPI_NETWORK_SELECTIONMODE_MANUAL	/**< Network selection is in the Manual selection mode (Not applicable to CDMA) */
} TelNetworkSelectionMode_t;

/**
 * @brief Enumeration for the network emergency callback mode states (3GPP2 specific enum).
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_EMERGENCY_CALLBACK_MODE_ENTER,	/**< Enter emergency callback mode */
	TAPI_NETWORK_EMERGENCY_CALLBACK_MODE_EXIT	/**< Exit emergency callback mode */
} TelNetworkEmergencyCallbackMode_t;

/**
 * @brief Enumeration for the status of the network service.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_SERVICE_LEVEL_NO,			/**< No service available in the network ME is camped */
	TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY,	/**< Only emergency service available in the network ME is camped */
	TAPI_NETWORK_SERVICE_LEVEL_FULL,		/**< FULL service available in the network ME is camped */
	TAPI_NETWORK_SERVICE_LEVEL_SEARCH		/**< Searching for service */
} TelNetworkServiceLevel_t;

/**
 * @brief Enumeration for the service types of TAPI.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_SERVICE_TYPE_UNKNOWN = 0x0,	/**< Service type is Unknown */
	TAPI_NETWORK_SERVICE_TYPE_NO_SERVICE,		/**< No Service available */
	TAPI_NETWORK_SERVICE_TYPE_EMERGENCY,		/**< Service type is Emergency */
	TAPI_NETWORK_SERVICE_TYPE_SEARCH,			/**< Service type is Searching */
	TAPI_NETWORK_SERVICE_TYPE_2G,				/**< Service type is 2G. In case of CDMA, service type is set to 2G when System Type is IS95A/IS95B/CDMA_1X */
	TAPI_NETWORK_SERVICE_TYPE_2_5G,				/**< Service type is 2.5G */
	TAPI_NETWORK_SERVICE_TYPE_2_5G_EDGE,		/**< Service type is 2.5G (EDGE) */
	TAPI_NETWORK_SERVICE_TYPE_3G,				/**< Service type is 3G. In case of CDMA, service type is set to 3G when System Type is EVDO_REV_0/REV_A/REV_B/EHRPD */
	TAPI_NETWORK_SERVICE_TYPE_HSDPA,			/**< Service type is HSDPA */
	TAPI_NETWORK_SERVICE_TYPE_LTE				/**< Service type is LTE */
} TelNetworkServiceType_t;

/**
 * @brief Enumeration for the system types of TAPI.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_SYSTEM_NO_SRV,				/**< No Service available */
	TAPI_NETWORK_SYSTEM_GSM,				/**< Available service is GSM */
	TAPI_NETWORK_SYSTEM_GPRS,				/**< Available service is GPRS */
	TAPI_NETWORK_SYSTEM_EGPRS,				/**< Available service is EGPRS */
	TAPI_NETWORK_SYSTEM_PCS1900,			/**< Available service is PCS1900 band */
	TAPI_NETWORK_SYSTEM_UMTS,				/**< Available service is UMTS */
	TAPI_NETWORK_SYSTEM_GSM_AND_UMTS,		/**< Both GSM and UMTS systems available */
	TAPI_NETWORK_SYSTEM_HSDPA,				/**< Available service is HSDPA */
	TAPI_NETWORK_SYSTEM_IS95A,				/**< Available service is IS95A */
	TAPI_NETWORK_SYSTEM_IS95B,				/**< Available service is IS95B */
	TAPI_NETWORK_SYSTEM_CDMA_1X,			/**< Available service is CDMA 1X */
	TAPI_NETWORK_SYSTEM_EVDO_REV_0,	/**< Available service is EV-DO rev0 */
	TAPI_NETWORK_SYSTEM_1X_EVDO_REV_0_HYBRID, /**< Available service is 1X and EV-DO rev0 */
	TAPI_NETWORK_SYSTEM_EVDO_REV_A,	/**< Available service is EV-DO revA */
	TAPI_NETWORK_SYSTEM_1X_EVDO_REV_A_HYBRID, /**< Available service is 1X and EV-DO revA */
	TAPI_NETWORK_SYSTEM_EVDO_REV_B,	/**< Available service is EV-DO revB */
	TAPI_NETWORK_SYSTEM_1X_EVDO_REV_B_HYBRID, /**< Available service is 1X and EV-DO revB */
	TAPI_NETWORK_SYSTEM_EVDV,		/**< Available service is EV-DV */
	TAPI_NETWORK_SYSTEM_EHRPD,		/**< Available service is EHRPD */
	TAPI_NETWORK_SYSTEM_LTE			/**< Available service is LTE */
} TelNetworkSystemType_t;

/**
 * @brief Enumeration for the different network bands.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_BAND_TYPE_ANY = 0x00,		/**< All bands can be tried */
	TAPI_NETWORK_BAND_TYPE_GSM850,			/**< This member sets the band to GSM 850 */
	TAPI_NETWORK_BAND_TYPE_GSM_900_1800,	/**< This member sets the band to GSM 900_1800 */
	TAPI_NETWORK_BAND_TYPE_GSM1900,			/**< GSM 1900 */
	TAPI_NETWORK_BAND_TYPE_GSM,				/**< This member sets all GSM bands */
	TAPI_NETWORK_BAND_TYPE_WCDMA,			/**< This member sets all WCDMA bands */
	TAPI_NETWORK_BAND_TYPE_WCDMA850,		/**< This member sets the band to WCDMA 850 */
	TAPI_NETWORK_BAND_TYPE_WCDMA1900,		/**< This member sets the band to WCDMA 1900 */
	TAPI_NETWORK_BAND_TYPE_WCDMA2100,		/**< This member sets the band to WCDMA 2100 */
	TAPI_NETWORK_BAND_TYPE_GSM900,			/**< GSM 900 */
	TAPI_NETWORK_BAND_TYPE_GSM1800,			/**< This member sets the band to GSM 1800 */
	TAPI_NETWORK_BAND_TYPE_GSM_850_1900,	/**< This member sets the band to GSM 850_1900 */
	TAPI_NETWORK_BAND_TYPE_LTE_BAND_4,		/**< This member sets the band to LTE BAND 4 */
	TAPI_NETWORK_BAND_TYPE_LTE_BAND_17		/**< This member sets the band to LTE BAND 17 */
} TelNetworkBand_t;

/**
 * @brief Enumeration for the different network operation causes.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_NO_ERR = 0x00,					/**< There is no error for any network operation */
	TAPI_NETWORK_OPERATION_ABORTED,				/**< This error is sent when an operation is aborted */
	TAPI_NETWORK_OPERATION_FAILED,				/**< There is an error for a network operation */
	TAPI_NETWORK_PHONE_IN_USE_ERR,				/**< This error will be returned when a network operation is tried while the phone is in use(eg: Voice / Data call in progress due to which network operation failed) */
	TAPI_NETWORK_OFFLINE_ERR,					/**< This error is sent when a network option is tried when the phone is in the offline mode */
	TAPI_NETWORK_CONFIG_SETTINGS_FAILURE_ERR,	/**< This error is sent when a modem is unable to process the config settings information */
	TAPI_NETWORK_INTERNAL_FAILURE_ERR,			/**< This error is used to indicate an internal failure */
	TAPI_NETWORK_MEMORY_FULL_ERR				/**< This error is caused when the memory is full */
} TelNetworkOperationCause_t;

/**
 * @brief Enumeration for the operation that can be done on a preferred PLMN.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_PREF_PLMN_ADD=0x01,			/**< Addition to the Network Preferred PLMN list */
	TAPI_NETWORK_PREF_PLMN_EDIT,				/**< Edit the network Preferred PLMN list */
	TAPI_NETWORK_PREF_PLMN_DELETE				/**< Delete the entry to the network Preferred PLMN list */
} TelNetworkPreferredPlmnOp_t;

/**
 * @brief Enumeration for the network PLMN type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_UNKNOWN_PLMN=0,				/**< Network PLMN type - Unknown */
	TAPI_HOME_PLMN ,					/**< Network PLMN type - Home PLMN */
	TAPI_AVAILABLE_PLMN,				/**< Network PLMN type - Available */
	TAPI_FORBIDDEN_PLMN,				/**< Network PLMN type - Forbidden */
}TelNetworkPlmnType_t;

/**
 * @brief Enumeration for the network home zone command.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_ZONEINFO_NONE,				/**< Network Zone info none */
	TAPI_NETWORK_ZONEINFO_IND,				/**< Network Zone info indication */
	TAPI_NETWORK_ZONEINFO_CLEAR				/**< Network Zone info clear */
}TelNetworkHomeZoneCommand_t;

/**
 * @brief Enumeration for the network home zone type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_ZONE_NONE,					/**< Network Zone Type none */
	TAPI_NETWORK_ZONETYPE_HOMEZONE,		    /**< Network Zone Type Home zone */
	TAPI_NETWORK_ZONETYPE_CITYZONE,			/**< Network Zone Type City zone */
}TelNetworkHomeZone_t;

/**
 * @brief Enumeration for the network name display condition type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_NETWORK_DISP_INVALID= 0x00,		/**< Invalid Display Condition */
	TAPI_NETWORK_DISP_SPN = 0x01,			/**< Display Condition is SPN */
	TAPI_NETWORK_DISP_PLMN = 0x02,		    /**< Display Condition is PLMN */
	TAPI_NETWORK_DISP_SPN_PLMN = 0x03,	    /**< Display Condition is SPN or PLMN */
}TelNetworkNameDispCondition_t;

/**
 * @brief Enumeration for the CDMA Preferred Network Type(CDMA only).
 * @since_tizen 2.3
 */
typedef enum
{
    TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY = 0,
    TAPI_NETWORK_PREF_NET_TYPE_AFFILIATED = 1,
    TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC = 2,
    TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A = 3,
    TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_B = 4,
    TAPI_NETWORK_PREF_NET_TYPE_ROAM_DOMESTIC = 5,
    TAPI_NETWORK_PREF_NET_TYPE_ROAM_INTERNATIONAL = 6,
    TAPI_NETWORK_PREF_NET_TYPE_ROAM_DUAL = 7,
    TAPI_NETWORK_PREF_NET_TYPE_BLANK = 8,
} TelNetworkPrefNetType_t;

/**
 * @brief Enumeration for the CDMA PRL Preferred Only field stored in the PRL(CDMA only).
 * @since_tizen 2.3
 */
typedef enum
{
    TAPI_NETWORK_PRL_PREF_ONLY_OFF,			/**< Preferred Only is FALSE */
    TAPI_NETWORK_PRL_PREF_ONLY_ON,			/**< Preferred Only is TRUE */
    TAPI_NETWORK_PRL_PREF_ONLY_NONE			/**< There isn't any Preferred Only Option */
} TelNetworkPrlPrefOnly_t;

/**
 * @brief Enumeration for the packet service protocol type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_PS_TYPE_UNKNOWN,
	TAPI_NETWORK_PS_TYPE_HSDPA,
	TAPI_NETWORK_PS_TYPE_HSUPA,
	TAPI_NETWORK_PS_TYPE_HSPA,
	TAPI_NETWORK_PS_TYPE_HSPAP
} TelNetworkPsType_t;

/**
 * @brief Enumeration for the network name option.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_NAME_OPTION_NONE,
	TAPI_NETWORK_NAME_OPTION_SPN,
	TAPI_NETWORK_NAME_OPTION_OPERATOR,
	TAPI_NETWORK_NAME_OPTION_ANY
} TelNetworkNameOption_t;

/**
 * @brief Enumeration for the possible 'default' Data Subscriptions.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_DEFAULT_DATA_SUBS_UNKNOWN = -1,  /**< Unknown status */
	TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM1 = 0,      /**< SIM 1 */
	TAPI_NETWORK_DEFAULT_DATA_SUBS_SIM2           /**< SIM 2 */
} TelNetworkDefaultDataSubs_t;

/**
 * @brief TelNetworkDefaultSubs_t
 * This enumeration defines possible 'default' Subscription for CS (Voice)
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_NETWORK_DEFAULT_SUBS_UNKNOWN = -1,  /**<  Unknown status **/
	TAPI_NETWORK_DEFAULT_SUBS_SIM1 = 0, /**< SIM 1 network **/
	TAPI_NETWORK_DEFAULT_SUBS_SIM2 /**<  SIM 2 network **/
} TelNetworkDefaultSubs_t;

/**
 * @brief The structure type for the preferred PLMN information.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char Index; /**< Network Preferred PLMN list index */
	char Plmn[TAPI_NETWORK_PLMN_LEN_MAX + 1]; /**< Network Preferred PLMN */
	char network_name[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< Network Name */
	char service_provider_name[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< Service provider name */
	TelNetworkSystemType_t SystemType; /**< Network system type */
} TelNetworkPreferredPlmnInfo_t;

/**
 * @brief The structure type for the preferred PLMN list.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int NumOfPrefPlmns; /**< Number of preferred PLMNs in the list */
	TelNetworkPreferredPlmnInfo_t PrefPlmnRecord[TAPI_PREFERRED_PLMN_RECORDS_MAX]; /**< Preferred PLMNs list */
} TelNetworkPreferredPlmnList_t;

/**
 * @brief The structure type for the location area code and cell ID.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned short LocationAreaCode; /**< Network Location area code */
	unsigned long CellId;            /**< Network Cell ID */
} TelNetworkLocationCellInfo_t;

/**
 * @brief The structure type for the network status.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char svctype; /**< Network SVC type */
	unsigned char roaming; /**< Network roaming */
} TelNetworkStatus_t;

/**
 * @brief The structure type for the network CS, PS status.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char cstype; /**< Network CS type */
	unsigned char pstype; /**< Network PS type */
} TelNetworkCsPsStatus_t;

/**
 * @brief The structure type for the network identity.
 * @since_tizen 2.3
 */
typedef struct {
	char network_name[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< Network Name */
	char service_provider_name[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< Service provider name */
	char plmn[TAPI_NETWORK_PLMN_LEN_MAX+1]; /**< Network PLMN */
	unsigned int plmn_id; /**< PLMN ID */
	TelNetworkPlmnType_t type_of_plmn; /**< PLMN type */
	TelNetworkSystemType_t access_technology; /**< Access technology */
} TelNetworkIdentity_t;

/**
 * @brief The structure type for the network PLMN list.
 * @since_tizen 2.3
 */
typedef struct {
	char networks_count; /**< Network PLMN count */
	TelNetworkIdentity_t network_list[TAPI_NETWORK_LIST_MAX]; /**< Network list */
} TelNetworkPlmnList_t;

/**
 * @brief The structure type for network display info.
 * @since_tizen 2.3
 */
typedef struct {
	int DisplayInfo[TAPI_NETWORK_DISPLAY_INFO_ELEMENTS]; /**< Network display info */
} TelNetworkDisplayInfo_t;

typedef struct {
	TelNetworkNameDispCondition_t DispCondition; /**< Display condition of SPN & PLMN */
	unsigned char Spn[TAPI_NETWORK_SPN_LEN_MAX + 1]; /**< SPN value */
	unsigned char Plmn[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< PLMN value, in 2G case if DispPlmn is @c true and the PLMN value is @c NULL then the network module has to display a registered PLMN */
} TelNetworkSpnPlmnDisplayInfo_t;

/**
 * @brief The structure type for the network's SPN, MCC, and MNC info.
 * @since_tizen 2.3
 */
typedef struct {
	int mcc; /**< Network MCC */
	int mnc; /**< Network MNC */
	char bDispCondition; /**< Display condition */
	char short_network_name[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< Network short name */
	char spn_name[TAPI_NETWORK_NAME_LEN_MAX + 1]; /**< Network SPN name */
} TelNetworkSpnMccMncInfo_t;

/**
 * @brief The structure type for information related to a CDMA system.
 * @since_tizen 2.3
 */
typedef struct {
	int  carrier; /**< CDMA Carrier */
	unsigned int system_id; /**< System ID */
	unsigned int network_id; /**< Network ID */
	unsigned int base_station_id; /**< Base station ID */
	int base_station_latitude; /**< Latitude of the current base station */
	int base_station_longitude; /**< Longitude of the current base station */
	unsigned int registration_zone; /**< Registration Zone */
	unsigned int pilot_offset; /**< Pilot Offset */
} TelNetworkCdmaSysInfo_t;

typedef struct {
	TelNetworkSystemType_t act; /**< Access technology */
	char plmn[TAPI_NETWORK_PLMN_LEN_MAX+1]; /**< Network PLMN */
	union {
		int lac; /**< Location Area Code - 3GPP Only */
		TelNetworkCdmaSysInfo_t cdma_info; /**< CDMA Only */
	}info;
} TelNetworkServing_t;

typedef struct {
	int cell_id;	/**< Cell ID (@c -1 indicates that cell ID information is not present) */
	int lac;		/**< Location area code (this field is ignored when @a cell_id is not present) */
	int bcch;		/**< Broadcast Control Channel Frequency number */
	int bsic;		/**< Base Station Identification Code */
	int rxlev;		/**< Received Signal Strength level. Valid values are (0-63, 99)
					 * Reference: 33GPP TS 45.008 [20] subclause 8.1.4
					   - Rxlev 0 is a signal strength less than -110 dBm
				       - Rxlev 1 is -110 dBm to -109 dBm
				       - Rxlev 2 is -109 dBm to -108 dBm
				       - ...
				       - Rxlev 62 is -49 dBm to -48 dBm
				       - Rxlev 63 is greater than -48 dBm
				       - Rxlev 99 is not known or detectable
				     */
} TelNetworkGeranCellInfo_t;

typedef struct {
	int cell_id;	/**< UCID (@c -1 indicates that cell ID information is not present) */
					/* UCID : RNCID + Cell ID(16 bit) */
	int lac;		/**< Location area code (this field is ignored when @a cell_id is not present) */
	int arfcn;	/**< UTRA Absolute RF Channel Number */
	int psc;		/**< Primary scrambling Code */
	int rscp;		/**< Received Signal Code Power. Valid values are (0-96, 255)
					 * Reference : 3GPP TS 25.133 [95] subclause 9.1.1.3 and 3GPP TS 25.123 [96] subclause 9.1.1.1.3)
					   - Rscp 0 is a signal strength less than -120 dBm
				       - Rscp 1 is -120 dBm to -119 dBm
				       - Rscp 2 is -119 dBm to -118 dBm
				       - ...
				       - Rscp 95 is -26 dBm to -25 dBm
				       - Rscp 96 is greater than -25 dBm
				       - Rscp 255 is not known or detectable
			         */
} TelNetworkUmtsCellInfo_t;

typedef struct {
	int cell_id;	/**< Cell ID (@c -1 indicates that cell ID information is not present) */
	int lac;		/**< Location area code (this field is ignored when @a cell_id is not present) */
	int earfcn;	/**< E-UTRA Absolute RF Channel Number */
	int tac;		/**< Tracking area code	*/
	int rssi;		/**< RSSI in dBm(signed) */
} TelNetworkLteCellInfo_t;

typedef struct {
	unsigned int system_id;	/**< System ID */
	unsigned int network_id;	/**< Network ID */
	unsigned int base_station_id; /**< Base station ID */
	unsigned int reference_pn;	 /**< Reference PN */
	int base_station_latitude; /**< Latitude of the current base station */
	int base_station_longitude; /**< Longitude of the current base station */
} TelNetworkCdmaCellInfo_t;

typedef struct {
	TelNetworkSystemType_t act;	/**< Access technology */
	int mcc;	/**< Mobile Country Code */
	int mnc;	/**< Mobile Network Code */
	union {
		TelNetworkGeranCellInfo_t geran;	/**< GERAN cell info */
		TelNetworkUmtsCellInfo_t umts;	/**< UMTS cell info */
		TelNetworkLteCellInfo_t lte;	/**< LTE cell info */
		TelNetworkCdmaCellInfo_t cdma;	/**< CDMA cell info */
	} cell;		/**< TBD */
} TelNetworkServingCellInfo_t;

typedef struct {
	TelNetworkServingCellInfo_t serving;	/**< Serving cell info */
	int geran_list_count;/**< GERAN cell info list count */
	TelNetworkGeranCellInfo_t geran_list[TAPI_NETWORK_GERAN_NEIGHBORING_CELL_LIST_MAX];/**< GERAN cell info list */
	int umts_list_count;/**< UMTS cell info list count */
	TelNetworkUmtsCellInfo_t umts_list[TAPI_NETWORK_UMTS_NEIGHBORING_CELL_LIST_MAX];/**< UMTS cell info list */
} TelNetworkNeighboringCellInfo_t;

/**
 * NEW NEW NEW
 */
typedef struct {
	TelNetworkServiceLevel_t cs; /**< Circuit Switched status */
	TelNetworkServiceLevel_t ps; /**< Packet Swithced status */
	TelNetworkServiceType_t type; /**< Registration service type */
	int is_roaming; /**< Roaming status 1: TRUE; FALSE otherwise */
} TelNetworkRegistrationStatus_t;

struct tel_noti_network_signal_strength {
	int dbm; /**< Signal Strenght in dBm */
};

struct tel_noti_network_cell_info {
	int lac; /**< Location Area Code */
	int cell_id; /**< Cell ID */
};

struct tel_noti_network_change {
	TelNetworkSystemType_t act; /**< Access technology */
	char plmn[TAPI_NETWORK_PLMN_LEN_MAX+1]; /**< Network PLMN */
};

struct tel_noti_network_time_info {
	int year;		/**< TBD */
	int month;		/**< TBD */
	int day;		/**< TBD */
	int hour;		/**< TBD */
	int minute;		/**< TBD */
	int second;		/**< TBD */
	int wday;		/**< TBD */
	int gmtoff;		/**< TBD */
	int dstoff;		/**< TBD */
	int isdst;		/**< TBD */
	char plmn[TAPI_NETWORK_PLMN_LEN_MAX+1]; /**< Network PLMN */
};

struct  tel_noti_network_identity {
	char plmn[TAPI_NETWORK_PLMN_LEN_MAX+1]; /**< Network PLMN */
	char short_name[17]; /**< Short network name */
	char full_name[33]; /**< Full network name */
};

struct tel_noti_network_emergency_callback_mode {
	TelNetworkEmergencyCallbackMode_t mode; /**< Emergency Callback Mode status */
};

struct tel_noti_network_neighboring_cell_info {
	TelNetworkNeighboringCellInfo_t info; /**< Neighboring Cell Information */
};

struct tel_noti_network_default_data_subs {
	TelNetworkDefaultDataSubs_t default_subs; /**< 'default' Data Subscription */
};

struct tel_noti_network_default_subs {
	TelNetworkDefaultSubs_t default_subs; /**< 'default' Subscription */
};

#ifdef __cplusplus
}
#endif

#endif // _TEL_NETWORK_H_

/**
* @}
*/
