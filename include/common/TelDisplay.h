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
 * @addtogroup 	DISPLAY_TAPI	DISPLAY
 * @{
 *
 * These APIs allow an application to accomplish the following services: @n
 * - Get the Display Icon Information. @n
 */

#ifndef _TEL_DISPLAY_H_
#define _TEL_DISPLAY_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/

#include <TelDefines.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 MACROS
 ==================================================================================================*/
#define TAPI_MAX_DISP_O2_HOMEZONE_TAG_LEN    	13	/** Maximum number of bytes Home Zone Tag name */
#define TAPI_MAX_DISP_ROAM_INFO_LEN				49	/* Roaming location information Maximum length is 32 bytes, But, 49 bytes in Modem code*/
#define TAPI_MAX_DISP_PHONE_FATAL_ERR_MSG_LEN	64	/* Phone fatal error message length */

/*==================================================================================================
 ENUMS
 ==================================================================================================*/

/**
 * This defines the RSSI levels.
 * In GSM, this field represent the radio signal strength in  dBm , but in the CDMA this field represents
 * the number of the antenna bar as follows
 */
typedef enum {
	TAPI_DISPLAY_RSSI_0 = 0x00,
	TAPI_DISPLAY_RSSI_1 = 0x01,
	TAPI_DISPLAY_RSSI_2 = 0x02,
	TAPI_DISPLAY_RSSI_3 = 0x03,
	TAPI_DISPLAY_RSSI_4 = 0x04,
	TAPI_DISPLAY_RSSI_5 = 0x05,
	TAPI_DISPLAY_RSSI_6 = 0x06
} tapi_display_rssi_level_t;

/**
 * This enum defines the strength of the battery in levels
 */
typedef enum {
	TAPI_POWER_PHONE_BATT_LEVEL0 = 0x00, /**< BLANK(Critical Low) */
	TAPI_POWER_PHONE_BATT_LEVEL1 = 0x01, /**< bar1 */
	TAPI_POWER_PHONE_BATT_LEVEL2 = 0x02, /**< bar2 */
	TAPI_POWER_PHONE_BATT_LEVEL3 = 0x03, /**< bar3 */
	TAPI_POWER_PHONE_BATT_LEVEL4 = 0x04, /**< bar4 */
	TAPI_POWER_PHONE_BATT_LEVEL5 = 0x05, /**< bar5 */
	TAPI_POWER_PHONE_BATT_LEVEL_INIT = 0x06 /**< init value */
} tapi_phone_battery_level_t;

/**
 * Home Zone UI Icon Indiction Type
 */
typedef enum {
	TAPI_DISPLAY_HMZ_O2_ZONE_INDI_NONE = 0x00, /**< None */
	TAPI_DISPLAY_HMZ_O2_ZONE_INDI_IND = 0x01, /**< Ind */
	TAPI_DISPLAY_HMZ_O2_ZONE_INDI_CLEAR = 0x02 /**< Clear */
} tapi_display_hmz_02_zone_ind_t;

/**
 * Home Zone Type
 */
typedef enum {
	TAPI_DISPLAY_HMZ_O2_ZONE_TYPE_NONE = 0x00, /**< None */
	TAPI_DISPLAY_HMZ_O2_ZONE_TYPE_HOME = 0x01, /**< Home Zone */
	TAPI_DISPLAY_HMZ_O2_ZONE_TYPE_CITY = 0x02 /**< City Zone */
} tapi_display_hmz_02_zone_type_t;

/**
 * ZONE_TAG_PRESENT
 */
typedef enum {
	TAPI_DISPLAY_HMZ_O2_ZONE_TAG_NONE = 0x00, /**< None */
	TAPI_DISPLAY_HMZ_O2_ZONE_TAG_PRESENT = 0x01, /**< Present */
	TAPI_DISPLAY_HMZ_O2_ZONE_TAG_ABSENT = 0x02 /**< Absent */
} tapi_display_hmz_02_zone_tag_t;

/**
 * This enumeration is used to identify what icon info the client is interested in..
 */
typedef enum {
	TAPI_DISPLAY_GET_ICON_RSSI = 0x01, /**< Signal Strength */
	TAPI_DISPLAY_GET_ICON_BATT = 0x02, /**< Battery Level */
	TAPI_DISPLAY_GET_ICON_HDR_RSSI = 0x04, /**< HDR(High Data Rate RSSI - CDMA */
	TAPI_DISPLAY_GET_ICON_ALL = 0xFF /**< All Values Request */
} tapi_display_get_icon_req_type_t;

/**
 * roam info Control Indication Type
 */
typedef enum {
	TAPI_DISPLAY_ROAM_INDI_NONE = 0x00, /**< None */
	TAPI_DISPLAY_ROAM_TEXT_OFF = 0x01, /**< Roam Text Off */
	TAPI_DISPLAY_ROAM_TEXT_ON = 0x02 /**< Roam Text On */
} tapi_display_roam_control_ind_t;

/**
 * roam info data roam guard Indication Type
 */
typedef enum {
	TAPI_DISPLAY_DATA_ROAM_GUARD_NONE = 0x00, /**< None */
	TAPI_DISPLAY_DATA_ROAM_GUARD_OFF = 0x01, /**< Data Roam Guard Off */
	TAPI_DISPLAY_DATA_ROAM_GUARD_ON = 0x02 /**< Data Roam Guard On */
} tapi_display_data_roam_guard_ind_t;

/**
 * roam info data roam guard Indication Type
 */
typedef enum {
	TAPI_DISPLAY_USER_IND_NORMAL_MODE = 0x01, /**< Normal Mode */
	TAPI_DISPLAY_USER_IND_INITIAL_PROGRAMMING = 0x02, /**< Initial Programming needed */
	TAPI_DISPLAY_USER_IND_MAINTENANCE_REQUIRED = 0x03, /**< Maintenance Required */
	TAPI_DISPLAY_USER_IND_AUTHENTICATION_REQUIRED = 0x04 /**< Authentication Required */
} tapi_display_user_ind_message_t;

/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/**
 * This structure defines the display icon information
 */
typedef struct {
	tapi_display_rssi_level_t rssi; /**< RSSI Levels */
	tapi_display_rssi_level_t hdrRssi; /**< HDR(High Data Rate) RSSI Levels -CDMA */
	tapi_phone_battery_level_t batt; /**< Battery Level */
} tapi_display_icon_info_t;

/**
 * HomeZone Information Notification Message
 *
 * This service specifies the implementation of o2 DE's HomeZone Indication (HZI) service
 * in 3G or 2G handsets. The HZI service consists of displaying a text and, optionally depending
 * on the zone, an icon in the handset (UE) when the UE is located within a zone which is defined
 * individually for each customer. The zone is defined by a set of cells, i. e. for each cell the
 * indication is either displayed or not. If the display of a MS is too small to display the complete
 * operator name together with the complete tag, it will be allowed to display the operator
 * name and the HomeZone/CityZone-tag in the same line ('o2 - de city' or 'o2 - de home').
 *
 * Reference O2 Home Zone spec.
 */
typedef struct {
	tapi_display_hmz_02_zone_ind_t ind;
	tapi_display_hmz_02_zone_type_t type;
	tapi_display_hmz_02_zone_tag_t tag_presnet;

	/**
	 * The coding is 12-character string coded in the short message alphabet given
	 * In GSM 03.38 with bit 8 set to zero. Default FF. FF indicates no display.
	 * If ZONE_TAG_PRESENT, then this is 13 bytes.
	 * If ZONE_TAG_ABSENT, then this is zero byte.
	 */
	unsigned char tag_name[TAPI_MAX_DISP_O2_HOMEZONE_TAG_LEN];
} tapi_display_hmz_o2_ind_t;

/**
 * Notify the TS of the occurrence of the phone fatal in the phone
 */
typedef struct {
	unsigned char Msg_len;
	unsigned char Message[TAPI_MAX_DISP_PHONE_FATAL_ERR_MSG_LEN];
} tapi_display_phone_fatal_info_noti_t;

/**
 * Notify the TS of the occurrence of the ext roam info in the phone
 */
typedef struct {
	tapi_display_roam_control_ind_t control;
	tapi_display_data_roam_guard_ind_t data_roam_guard;
	unsigned char txt_len;
	unsigned char text[TAPI_MAX_DISP_ROAM_INFO_LEN];
} tapi_display_ext_roam_info_noti_t;

/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif	/* _TEL_DISPLAY_H_ */

/**
 *  @}
 */

