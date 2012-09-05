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
* @ingroup	TelephonyAPI
* @addtogroup  Network_TAPI	Network
* @{
*
* @file TelNetwork.h

     @brief This file serves as a "C" header file and defines structures for Tapi Network Services\n
      It contains a sample set of constants, enums, structs that would be required by applications.
 */

#ifndef _TEL_NETWORK_H_
#define _TEL_NETWORK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define TAPI_NETWORK_ORDER_MAX					5		/**< Maximum network order */
#define TAPI_NETWORK_LIST_MAX					15		/**< Maximum network list */
#define TAPI_NETWORK_NAME_LEN_MAX				40		/**< Maximum network name or PLMN name length*/
#define TAPI_NETWORK_SPN_LEN_MAX				20		/**< Maximum network service provider name length */
#define TAPI_PREFERRED_PLMN_RECORDS_MAX			150		/**< Maximum record length for preferred plmn list*/
#define TAPI_NETWORK_DISPLAY_INFO_ELEMENTS		4		/**< Maximum network Display info element length */
#define TAPI_NETWORK_HOMEZONE_TAG_MAX_LEN		13		/**< Maximum Network  Home zone tag  length */
#define TAPI_NETWORK_CS_DISP_INDEX				0		/**< Maximum   Network CS display index length */
#define TAPI_NETWORK_PS_DISP_INDEX				1		/**< Maximum  Network PS display index  length */
#define TAPI_NETWORK_SVC_DISP_INDEX				2		/**< Maximum  Network SVC display index length */
#define TAPI_NETWORK_ROAM_DISP_INDEX			3		/**< Maximum  Network roaming display index length */
#define TAPI_NETWORK_SUBS_SVC_NUM_MAX			6		/**< Maximum number of the Subscriber's services */
#define TAPI_NETWORK_SUBS_NUM_LEN_MAX			40		/**< Maximum Subscriber Number Length */
#define TAPI_NETWORK_SUBS_ALPHA_LEN_MAX			16		/**< Maximum Subscriber Name Length */

/**
 * @enum TelNetworkSelectionMode_t
 *  This enumeration defines the network selection modes.
 */
typedef enum {
	TAPI_NETWORK_SELECTIONMODE_AUTOMATIC,				/**< Network selection is in Automatic selection mode*/
	TAPI_NETWORK_SELECTIONMODE_MANUAL,				/**< Network selection is in Manual selection mode */
	TAPI_NETWORK_SELECTIONMODE_GLOBAL_AUTOMAITIC,	/**<Global Automatic */
	TAPI_NETWORK_SELECTIONMODE_CDMA,				/**<CDMA selection*/
	TAPI_NETWORK_SELECTIONMODE_LTE_AUTO,			/**<LTE Automatic*/
	TAPI_NETWORK_SELECTIONMODE_LTE_MANUAL			/**<LTE Manual */
} TelNetworkSelectionMode_t;

/**
 * @enum TelNetworkServiceDomain_t
 * This enumeration defines the network service domains .
 */
typedef enum {
	TAPI_NETWORK_SERVICE_DOMAIN_COMBINED,		/**< This member sets the Circuit switch   and Packet Switch domain */
	TAPI_NETWORK_SERVICE_DOMAIN_PS,				/**< This member sets the Packet Switch service domain */
	TAPI_NETWORK_SERVICE_DOMAIN_CS,				/**< This member sets the Circuit Switch service domain */
	TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC		/**< This member sets the service domain to Circuit Switch and Packet Switch or Circuit Switch or Packet Switch domain */
} TelNetworkServiceDomain_t;

/**
 * @enum TelNetworkMode_t
 * This enumeration defines the network modes.
 */
typedef enum {
	TAPI_NETWORK_MODE_AUTO = 0x00,		/**< This member sets the network mode to GSM */
	TAPI_NETWORK_MODE_GSM = 0x01,		/**< This member sets the network mode to GSM */
	TAPI_NETWORK_MODE_WCDMA = 0x02,		/**< This member sets the network mode to WCDMA  */
	TAPI_NETWORK_MODE_CDMA = 0x04,		/**< This member sets the network mode to CDMA */
	TAPI_NETWORK_MODE_LTE = 0x08,		/**< This member sets the network mode to LTE */
} TelNetworkMode_t;

/**
 * @enum TelNetworkServiceLevel_t
 * This enumeration defines the status of network service.
 */
typedef enum {
	TAPI_NETWORK_SERVICE_LEVEL_NO,			/**< No service available in the network ME is camped */
	TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY,	/**< Only emergency service available in the network ME is camped */
	TAPI_NETWORK_SERVICE_LEVEL_FULL,		/**< FULL service available in the network ME is camped */
	TAPI_NETWORK_SERVICE_LEVEL_SEARCH		/**< Searching for service */
} TelNetworkServiceLevel_t;

/**
 * @enum TelNetworkServiceLevel_t
 * This enumeration defines the service types of TAPI.
 */
typedef enum {
	TAPI_NETWORK_SERVICE_TYPE_NO_SVE,		/**< No service  (This enumeration is not applicable for OEM_TAPI_NW_select_network_req API) */
	TAPI_NETWORK_SERVICE_TYPE_LIMITED,		/**< Limited service available in the camped network  (This enumeration is not applicable for OEM_TAPI_NW_select_network_req API) */
	TAPI_NETWORK_SERVICE_TYPE_CS,			/**< CS service available in the camped network */
	TAPI_NETWORK_SERVICE_TYPE_PS,			/**< PS service available in the camped network */
	TAPI_NETWORK_SERVICE_TYPE_COMBINED		/**< Both CS and PS services available in the camped network */
} TelNetworkServiceType_t;

/**
 * @enum TelNetworkSystemType_t
 * This enumeration defines the system types of TAPI.
 */
typedef enum
{
	TAPI_NETWORK_SYSTEM_NO_SRV,				/**< No Service available */
	TAPI_NETWORK_SYSTEM_GSM,				/**< Available service is GSM  */
	TAPI_NETWORK_SYSTEM_GPRS,				/**< Available service is GPRS */
	TAPI_NETWORK_SYSTEM_EGPRS,				/**< Available service is EGPRS  */
	TAPI_NETWORK_SYSTEM_PCS1900,			/**< Available service is PCS1900 band */
	TAPI_NETWORK_SYSTEM_UMTS,				/**< Available service is UMTS  */
	TAPI_NETWORK_SYSTEM_GSM_AND_UMTS,		/**< Both GSM and UMTS systems available */
	TAPI_NETWORK_SYSTEM_HSDPA,				/**< Available service is hsdpa */
	TAPI_NETWORK_SYSTEM_IS95A,				/**< Available service is IS95A */
	TAPI_NETWORK_SYSTEM_IS95B,				/**< Available service is IS95B */
	TAPI_NETWORK_SYSTEM_CDMA_1X,			/**< Available service is CDMA 1X */
	TAPI_NETWORK_SYSTEM_EVDO_REV_0,	/**< Available service is EV-DO rev0 */
	TAPI_NETWORK_SYSTEM_1X_EVDO_REV_0_HYBRID, /**< Available service is  1X and EV-DO rev0 */
	TAPI_NETWORK_SYSTEM_EVDO_REV_A,	/**< Available service is  EV-DO revA */
	TAPI_NETWORK_SYSTEM_1X_EVDO_REV_A_HYBRID, /**< Available service is 1X and EV-DO revA */
	TAPI_NETWORK_SYSTEM_EVDV,		/**< Available service is EV-DV */
	TAPI_NETWORK_SYSTEM_LTE			/**< Available service is LTE */
} TelNetworkSystemType_t;

/**
 *  @enum TelNetworkBandPreferred_t
 * This enumeration defines the operations that can be done on network Band.
 */
typedef enum
{
	TAPI_NETWORK_BAND_MODE_PREFERRED = 0x01,		 /**<  Preferred band. (If preferred Band is not available, then underlying modem can search on other bands) */
	TAPI_NETWORK_BAND_MODE_ONLY = 0x02,				 /**<   This allows only particular band. */
} TelNetworkBandPreferred_t;

/**
 * @enum TelNetworkBand_t
 * This enumeration defines different network Bands.
 */
typedef enum
{
	TAPI_NETWORK_BAND_TYPE_ANY = 0x00,		/**< All bands can be tried. */
	TAPI_NETWORK_BAND_TYPE_GSM850,			/**< This member sets the band to GSM 850 */
	TAPI_NETWORK_BAND_TYPE_GSM_900_1800,	/**< This member sets the band to GSM 900_1800 */
	TAPI_NETWORK_BAND_TYPE_GSM1900,			/**< GSM 1900*/
	TAPI_NETWORK_BAND_TYPE_GSM,				/**< This member sets all GSM bands */
	TAPI_NETWORK_BAND_TYPE_WCDMA,			/**< This member sets all WCDMA bands */
	TAPI_NETWORK_BAND_TYPE_WCDMA850,		/**< This member sets the band to WCDMA 850 */
	TAPI_NETWORK_BAND_TYPE_WCDMA1900,		/**< This member sets the band to WCDMA 1900 */
	TAPI_NETWORK_BAND_TYPE_WCDMA2100,		/**< This member sets the band to WCDMA 2100 */
	TAPI_NETWORK_BAND_TYPE_GSM900,			/**< GSM 900*/
	TAPI_NETWORK_BAND_TYPE_GSM1800,			/**< This member sets the band to GSM 1800 */
	TAPI_NETWORK_BAND_TYPE_GSM_850_1900,	/**< This member sets the band to GSM 850_1900 */
	TAPI_NETWORK_BAND_TYPE_LTE_BAND_4,		/**< This member sets the band to LTE BAND 4*/
	TAPI_NETWORK_BAND_TYPE_LTE_BAND_17		/**< This member sets the band to LTE BAND 17 */
} TelNetworkBand_t;

/**
 * @enum TelNetworkOperationCause_t
 * This enumeration defines different network operation cause.
 */
typedef enum
{
	TAPI_NETWORK_NO_ERR = 0x00,					/**< There is no error for any network operation.  */
	TAPI_NETWORK_OPERATION_FAILED,				/**< There is an error for any network operation.  */
	TAPI_NETWORK_PHONE_IN_USE_ERR,				/**< This error will be returned when any network operation is tried while phone is in use(eg, Voice / Data call in progress due to which network operation failed) */
	TAPI_NETWORK_OFFLINE_ERR,					/**< This error is sent when any network option is tried when phone is in offline mode */
	TAPI_NETWORK_CONFIG_SETTINGS_FAILURE_ERR,	/**< This error is sent when modem is unable to process the config settings information. */
	TAPI_NETWORK_INTERNAL_FAILURE_ERR,			/**< This error to indicate internal failure */
	TAPI_NETWORK_MEMORY_FULL_ERR				/**< This error is caused by memory full */
} TelNetworkOperationCause_t;


/**
 * @enum TelNetworkPreferredPlmnOp_t
 * This enumeration defines the operation that can be done on preferred PLMN.
 */
typedef enum
{
	TAPI_NETWORK_PREF_PLMN_ADD=0x01,			/**< Addition to  Network Preferred Plmn list*/
	TAPI_NETWORK_PREF_PLMN_EDIT,				/**< Edit the network Preferred Plmn list */
	TAPI_NETWORK_PREF_PLMN_DELETE				/**< Delete the entry to network Preferred Plmn list*/
} TelNetworkPreferredPlmnOp_t;

/**
 * @enum TelNetworkPowerOnAttach_t
 * This enumeration defines operation that can be done on power on attach.
 * It is used on CDMA.
 */
typedef enum
{
	TAPI_NETWORK_SERVING_CARRIER_TEST = 0x00,			/**< TestBed */
	TAPI_NETWORK_SERVING_CARRIER_SKT,					/**< Korea SKT */
	TAPI_NETWORK_SERVING_CARRIER_KTF,					/**< Korea KTF */
	TAPI_NETWORK_SERVING_CARRIER_LGT,					/**< Korea LGT */
	TAPI_NETWORK_SERVING_CARRIER_VERIZON = 0x11,		/**< US VERIZON */
	TAPI_NETWORK_SERVING_CARRIER_SPRINT,				/**< US SPRINT */
	TAPI_NETWORK_SERVING_CARRIER_ALLTEL,				/**< US ALLTEL */
	TAPI_NETWORK_SERVING_CARRIER_METRO_PCS,			/**< US METRO_PCS */
	TAPI_NETWORK_SERVING_CARRIER_CELLULAR,				/**< US CELLULAR */
	TAPI_NETWORK_SERVING_CARRIER_CRIKET,				/**< US CRIKET */
	TAPI_NETWORK_SERVING_CARRIER_TELUS = 0x21,			/**< CANADA TELUS */
	TAPI_NETWORK_SERVING_CARRIER_BMC,					/**< CANADA BMC */
	TAPI_NETWORK_SERVING_CARRIER_BWA,					/**< CANADA BWA */
	TAPI_NETWORK_SERVING_CARRIER_CTC = 0x31,			/**< CHINA CTC */
	TAPI_NETWORK_SERVING_CARRIER_UNKNOWN = 0xFF		/**< SERVING NETWORK UNKNOWN */
} TelNetworkServCarrierType_t;

/**
 * @enum TelNetworkPlmnType_t
 * This structure defines Network Plmn type.
 */
typedef enum
{
	TAPI_UNKNOWN_PLMN=0,				/**< Network Plmn type Unknown */
	TAPI_HOME_PLMN ,					/**< Network Plmn type Home Plmn */
	TAPI_AVAILABLE_PLMN,				/**< Network Plmn Available */
	TAPI_FORBIDDEN_PLMN,				/**< Network Plmn Forbidden */
}TelNetworkPlmnType_t;

/**
 * @enum TelNetworkHomeZoneCommand_t
 * This enumeration defines network home zone command.
 */
typedef enum
{
	TAPI_NETWORK_ZONEINFO_NONE,				/**< Network Zone info none */
	TAPI_NETWORK_ZONEINFO_IND,				/**< Network Zone info indication */
	TAPI_NETWORK_ZONEINFO_CLEAR				/**< Network Zone info clear */
}TelNetworkHomeZoneCommand_t;

/**
 * @enum TelNetworkHomeZone_t
 * This enumeration defines network home zone type.
 */
typedef enum
{
	TAPI_NETWORK_ZONE_NONE,					/**< Network Zone Type none */
	TAPI_NETWORK_ZONETYPE_HOMEZONE,		/**< Network Zone Type Home zone */
	TAPI_NETWORK_ZONETYPE_CITYZONE,			/**< Network Zone Type City zone */
}TelNetworkHomeZone_t;

/**
 * @enum TelNetworkNameDispCondition_t
 * This enumeration defines network name display condition type.
 */
typedef enum
{
	TAPI_NETWORK_DISP_INVALID= 0x00,
	TAPI_NETWORK_DISP_SPN = 0x01,
	TAPI_NETWORK_DISP_PLMN = 0x02,
	TAPI_NETWORK_DISP_SPN_PLMN = 0x03,
}TelNetworkNameDispCondition_t;

/**
 * @enum TelNetworkPrefNetType_t
 * This enumeration defines CDMA Preferred Network Type(CDMA only).
 */
typedef enum
{
    TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC = 0x01,		/**< 0x01 : AUTOMATIC */
    TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A,			/**<  0x02 : AUTOMATIC A */
    TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_B,			/**<  0x03 : AUTOMATIC B */
    TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY,			/**<  0x04 : HOME ONLY */
    TAPI_NETWORK_PREF_NET_TYPE_ROAM_ONLY,			/**<  0x05 : ROAM ONLY */
} TelNetworkPrefNetType_t;

/**
 * @enum TelNetworkPrlPrefOnly_t
 * This enumeration defines CDMA PRL Preferred Only field stored in the PRL(CDMA only).
 */
typedef enum
{
    TAPI_NETWORK_PRL_PREF_ONLY_OFF,			/**< Preferred Only is FALSE */
    TAPI_NETWORK_PRL_PREF_ONLY_ON,			/**< Preferred Only is TRUE */
    TAPI_NETWORK_PRL_PREF_ONLY_NONE			/**< There isn't Preferred Only Option */
} TelNetworkPrlPrefOnly_t;

/**
 * @enum TelNetworkCDMAHybridMode_t
 * This enumeration defines CDMA Hybrid Mode State(CDMA only).
 */
typedef enum
{
    TAPI_NETWORK_CDMA_HYBRID = 0x01,	/**< 0x01 : HYBRID(1X + EVDO) */
    TAPI_NETWORK_CDMA_1X_ONLY,		/**< 0x02 : 1X ONLY */
    TAPI_NETWORK_CDMA_EVDO_ONLY,		/**< 0x03 : EVDO ONLY */
}TelNetworkCDMAHybridMode_t;


/**
 * This structure has information related to cdma system.
 */
typedef struct {
	TelNetworkServCarrierType_t carrier; /**< for CDMA, Optional */
	unsigned short system_id; /**< for CDMA, Mandatory */
	unsigned short network_id; /**< for CDMA, Mandatory */
	unsigned short base_station_id; /**< for CDMA, Mandatory */
	unsigned short base_station_latitude; /**< for CDMA, Mandatory */
	unsigned short base_station_longitude; /**< for CDMA, Mandatory */
	unsigned short registration_zone; /**< for CDMA, Optional */
	unsigned short pilot_offset; /**< for CDMA, Optional */
} TelNetworkCDMASysInfo_t;

/**
 *
 * This structure defines the preferred plmn information.
 */
typedef struct {
	unsigned char Index; /**< Network Preferred Plmn list index */
	char Plmn[7]; /**< Network Preferred Plmn */
	char network_name[TAPI_NETWORK_NAME_LEN_MAX]; /**< Network Name  */
	char service_provider_name[TAPI_NETWORK_NAME_LEN_MAX]; /**< Service provider name */
	TelNetworkSystemType_t SystemType; /**< Network system type */
} TelNetworkPreferredPlmnInfo_t;

/**
 *
 * This structure defines the preferred plmn list.
 */
typedef struct {
	unsigned int NumOfPrefPlmns; /**< Number of preferred plmns in the list */
	TelNetworkPreferredPlmnInfo_t PrefPlmnRecord[TAPI_PREFERRED_PLMN_RECORDS_MAX]; /**< preferred plmns list */
} TelNetworkPreferredPlmnList_t;

/**
 *
 * This structure defines the location area code and cell id.
 */
typedef struct {
	unsigned short LocationAreaCode; /**< Network Location area code */
	unsigned long CellId; /**< Network Cell Id */
} TelNetworkLocationCellInfo_t;

/**
 *
 * This structure defines the network status .
 */
typedef struct {
	unsigned char svctype; /**< Network svc type  */
	unsigned char roaming; /**< Network roaming  */
} TelNetworkStatus_t;

/**
 *
 * This structure defines the network Cs,Ps status.
 */
typedef struct {
	unsigned char cstype; /**< Network Cs type  */
	unsigned char pstype; /**< Network Ps type  */
} TelNetworkCsPsStatus_t;

/**
 *   TelNetworkIdentity_t
 * This structure defines the network identity.
 */
typedef struct {
	char network_name[TAPI_NETWORK_NAME_LEN_MAX]; /**< Network Name  */
	char service_provider_name[TAPI_NETWORK_NAME_LEN_MAX]; /**< Service provider name */
	unsigned int plmn_id; /**< PLMN ID */
	TelNetworkPlmnType_t type_of_plmn; /**< PLMN type */
	TelNetworkSystemType_t access_technology; /**< Access technology  */
} TelNetworkIdentity_t;

/**
 *
 * This structure defines the network Plmn list.
 */
typedef struct {
	char networks_count; /**< Network Plmn count  */
	TelNetworkIdentity_t network_list[TAPI_NETWORK_LIST_MAX]; /**< Network list  */
} TelNetworkPlmnList_t;

/**
 *
 * This structure defines the network display info.
 */
typedef struct {
	int DisplayInfo[TAPI_NETWORK_DISPLAY_INFO_ELEMENTS]; /**< Network display info  */
} TelNetworkDisplayInfo_t;

typedef struct {
	TelNetworkNameDispCondition_t DispCondition; /*Display condition of SPN & PLMN*/
	unsigned char Spn[TAPI_NETWORK_SPN_LEN_MAX + 1]; /*Spn value*/
	unsigned char Plmn[TAPI_NETWORK_NAME_LEN_MAX + 1]; /*PLMN value,  In 2G case if DispPlmn is true and the PLMN value is null then Network module has to display registered PLMN*/
} TelNetworkSpnPlmnDisplayInfo_t;

/**
 *  * @struct TelNetworkSpnMccMncInfo_t
 * This structure defines the network Spn, Mcc and Mnc info.
 */
typedef struct {
	int mcc; /**< Network mcc  */
	int mnc; /**< Network Mnc  */
	char bDispCondition; /**<Display condition */
	char short_network_name[TAPI_NETWORK_NAME_LEN_MAX]; /**< Network short Name  */
	char spn_name[TAPI_NETWORK_NAME_LEN_MAX]; /**< Network spn Name  */
} TelNetworkSpnMccMncInfo_t;


typedef struct {
	TelNetworkSystemType_t act; /**< Access technology  */
	char plmn[7];
	int lac;
} TelNetworkServing_t;



/**
 * NEW NEW NEW
 */
struct tel_noti_network_registration_status {
	int cs;
	int ps;
	int type;
	int is_roaming;
};

struct tel_noti_network_info {
	int rssi;
	int battery;
};

struct tel_noti_network_cell_info {
	int lac;
	int cell_id;
};

struct tel_noti_network_change {
	TelNetworkSystemType_t act; /**< Access technology  */
	char plmn[7];
	int lac;
};

struct tel_noti_network_time_info {
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int wday;
	int gmtoff;
	int dstoff;
	int isdst;
	char plmn[7];
};

struct  tel_noti_network_identity {
	char plmn[7];
	char short_name[17];
	char full_name[33];
};

#ifdef __cplusplus
}
#endif

#endif // _TEL_NETWORK_H_

/**
* @}
*/
