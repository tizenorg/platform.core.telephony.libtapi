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
 * @internal
 * @ingroup		TelephonyAPI
 * @addtogroup	MISC_TAPI	MISC
 * @{
 *
 * @file TelMisc.h

 @brief This file serves as a "C" header file defines structures for Tapi Misc Services. \n
 It contains a sample set of constants, enums, structs that would be required by applications.

 */

#ifndef _TEL_MISC_H_
#define _TEL_MISC_H_

#include <sys/time.h>
#include <TelDefines.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VERSION_LEN	32    /**< Maximum version length *///sw version, hw version, rf cal date, me sn
#define FUS_DUMMY_DATA_SIZE 2     /**< Data Size*/

#define	MMICHECK_CALL_RELEASE_NOTI	"User.LauncherUi.RequestTermination"
#define	MMICHECK_END_ALL_NOTI		"User.LauncherUi.RequestTermination"
#define	MMICHECK_DIALER_KEY_NOTI	"User.LauncherUi.DialerKeyInsert"
#define	MMICHECK_ACTIVITY_VIEW_NOTI	"User.MMICheck.ActivityView.ChangeStatus"

#define TAPI_MISC_ME_SN_LEN_MAX				32
#define TAPI_MISC_PRODUCT_CODE_LEN_MAX		32
#define TAPI_MISC_MODEL_ID_LEN_MAX			17
#define TAPI_MISC_PRL_ERI_VER_LEN_MAX		17

#define TAPI_MISC_MCC_LEN_MAX				3
#define TAPI_MISC_MNC_LEN_MAX				2
#define TAPI_MISC_MIN_LEN_MAX				10
#define TAPI_MISC_MDN_LEN_MAX				15
#define TAPI_MISC_NAME_LEN_MAX				17

/**
 * @enum TelMiscApplicationKey_t
 * This enumeration defines mmi check key values
 */
typedef enum {
	MMI_CHECK_DIAL_STAR		=	0x2A,	//	20 00 2A AF 4B 7E
	MMI_CHECK_DIAL_SHARP	=	0x23,	//	20 00 23 6E D6 7E
	MMI_CHECK_DIAL_0		=	0x30,	//	20 00 30 74 F4 7E
	MMI_CHECK_DIAL_1,					//	20 00 31 FD E5 7E
	MMI_CHECK_DIAL_2,					//	20 00 32 66 D7 7E
	MMI_CHECK_DIAL_3,					//	20 00 33 EF C6 7E
	MMI_CHECK_DIAL_4,					//	20 00 34 50 B2 7E
	MMI_CHECK_DIAL_5,					//	20 00 35 D9 A3 7E
	MMI_CHECK_DIAL_6,					//	20 00 36 42 91 7E
	MMI_CHECK_DIAL_7,					//	20 00 37 CB 80 7E
	MMI_CHECK_DIAL_8,					//	20 00 38 3C 78 7E
	MMI_CHECK_DIAL_9,					//	20 00 39 B5 69 7E

	MMI_CHECK_END_ALL		=	0x41,	//

	MMI_CHECK_CALL			=	0x50,	//	20 00 50 72 97 7E
	MMI_CHECK_END			=	0x51,	//	20 00 51 FB 86 7E
	MMI_CHECK_CANCEL		=	0x52,	//	20 00 52 60 B4 7E
	MMI_CHECK_RIGHT			=	0x53,	//	20 00 5C 1E 5D 7E
	MMI_CHECK_SIDE_UP		=	0x54,	//	20 00 54 56 D1 7E
	MMI_CHECK_SIDE_DOWN		=	0x55,	//	20 00 55 DF C0 7E

	MMI_CHECK_LEFT			=	0x5B,	//	20 00 5B A1 29 7E
	MMI_CHECK_ARROW_UP		=	0x63,	//	20 00 63 6A 94 7E
	MMI_CHECK_ARROW_DOWN	=	0x64,	//	20 00 64 D5 E0 7E
	MMI_CHECK_ARROW_LEFT	=	0x65,	//	20 00 65 5C F1 7E
	MMI_CHECK_ARROW_RIGHT	=	0x66,	//	20 00 66 C7 C3 7E

	MMI_CHECK_WAP			=	0x8C,	//	20 00 53 E9 A5 7E
	MMI_CHECK_CAMERA		=	0x8D,	//	20 01 8D C2 83 7E
	MMI_CHECK_MP3			=	0xDD,	//	20 00 DD 9F C8 7E
	MMI_CHECK_POWER			=	0x95,	//	20 00 95 D3 06 7E
	MMI_CHECK_LONG_PRESS	=	0xFF
} TelMiscMMICHECK_t;

/**
 * @enum TelMiscApplicationKey_t
 * This enumeration defines application key type
 */
typedef enum {
	KEY_EXT_DIAL_0 = 0,
	KEY_EXT_DIAL_1,
	KEY_EXT_DIAL_2,
	KEY_EXT_DIAL_3,
	KEY_EXT_DIAL_4,
	KEY_EXT_DIAL_5,
	KEY_EXT_DIAL_6,
	KEY_EXT_DIAL_7,
	KEY_EXT_DIAL_8,
	KEY_EXT_DIAL_9,
	KEY_EXT_DIAL_STAR,	// 10 : *
	KEY_EXT_DIAL_SHARP,	// 11 : #

	KEY_EXT_DIAL_BACK,	// 12 : <- (backkey)
	KEY_EXT_DIAL_HIDE,	// 13 :�� (hidekey)
	KEY_EXT_DIAL_P,		// 14 : p
	KEY_EXT_DIAL_PLUS,	// 15 : +
	KEY_EXT_DIAL_MUTE,	// 16 : mute

	KEY_EXT_DIAL_CALL			=	100,		// 100 : BTN1 (call)
	KEY_EXT_DIAL_MESSAGE,						// 101 : BTN2 (message)
	KEY_EXT_DIAL_VIDEO_CALL,					// 102 : BTN3 (video call)
	KEY_EXT_DIAL_SAVE,							// 103 : BTN4 (save)
	KEY_EXT_DIAL_CALL_END,						// noti publish
	KEY_EXT_TERMINATE_ALL,

	KEY_EXT_PAGEDOWN			=	114,		// Volume down
	KEY_EXT_PAGEUP				=	115,		// Volume up
	KEY_EXT_POWER				=	116,		// Power short
	KEY_EXT_PEOPLE_VIEW			=	132,		// People view
	KEY_EXT_TIMELINE			=	169,		// Lifedrive
	KEY_EXT_PREPARE_CAMERA		=	171,		// Prepare Camera
	KEY_EXT_ACTIVITY_VIEW		=	174,		// Activity view
	KEY_EXT_CAMERA				=	212,		// Camera
	KEY_EXT_SEARCH				=	217,		// Phone Search
	KEY_EXT_POWER_LONG			=	356,		// Power off

	KEY_EXT_INVALID_KEY			=	500
} TelMiscApplicationKey_t;

/**
 * @enum TelMiscDiagTouchEventType_t
 * This enumeration defines the touch event type
 */
typedef enum {
	TAPI_DIAG_TOUCH_CLICK_DOWN = 0x000F, /**< 0x0F00: Click Down */
	TAPI_DIAG_TOUCH_MOVE = 0x0010, /**< 0x1000: Move  */
	TAPI_DIAG_TOUCH_CLICK_UP = 0x0011, /**< 0x1100: Click Up */
	TAPI_DIAG_TOUCH_HOLD = 0x0012 /**< 0x1200: Hold */
} TelMiscDiagTouchEventType_t;

/**
 * @enum  TelMiscAlarmMode_t
 * Enum type for Validity of the Alarm booting
 */
typedef enum {
	TAPI_MISC_ALARMBOOT_INACTIVATE, /**< 0x00: Inactivated */
	TAPI_MISC_ALARMBOOT_ACTIVATE /**< 0x00: Activated */
} TelMiscAlarmMode_t;

typedef enum {
	TAPI_MISC_ME_IMEI = 0x00, /**< 0x00: IMEI, GSM/UMTS device */
	TAPI_MISC_ME_ESN = 0x01, /**< 0x01: ESN(Electronic Serial Number), It`s essentially run out. CDMA device */
	TAPI_MISC_ME_MEID = 0x02, /**< 0x02: MEID, This value can have hexa decimal digits. CDMA device */
	TAPI_MISC_ME_MAX = 0xff /**< 0xff: reserved */
} TelMiscSNIndexType_t;

/**
 * NAM Information MASK fiel
 */
typedef enum {
	TAPI_MISC_NAM_INFO_MASK_TOTAL			= 0x0001,
	TAPI_MISC_NAM_INFO_MASK_CURRENT_INDEX	= 0x0002,
	TAPI_MISC_NAM_INFO_MASK_REQUEST_INDEX	= 0x0004,
	TAPI_MISC_NAM_INFO_MASK_AUTONAM		= 0x0008,
	TAPI_MISC_NAM_INFO_MASK_MCC			= 0x0010,
	TAPI_MISC_NAM_INFO_MASK_MNC			= 0x0020,
	TAPI_MISC_NAM_INFO_MASK_MIN			= 0x0040,
	TAPI_MISC_NAM_INFO_MASK_MDN			= 0x0080,
	TAPI_MISC_NAM_INFO_MASK_NAM_NAME		= 0x0100,
	TAPI_MISC_NAM_INFO_MASK_NAM_CHANGE		= 0x0200,
	TAPI_MISC_NAM_INFO_MASK_NAM_NUM		= 0x0400,
	TAPI_MISC_NAM_INFO_MASK_ALL			= 0x8000
} TelMiscNamInfoMaskType_t;

/**
 * NAMIndex Field
 */
typedef enum {
	TAPI_MISC_NAM_1 = 0x00,
	TAPI_MISC_NAM_2,
	TAPI_MISC_NAM_3,
	TAPI_MISC_NAM_4,
	TAPI_MISC_NAM_5
} TelMiscNamIndex_t;


/**
 * @enum  tapi_misc_nitz_dayofweek_t
 * Enum type for day of week
 */
typedef enum {
	TAPI_MISC_NITZ_DOW_SUN = 0x00, /**< Sunday */
	TAPI_MISC_NITZ_DOW_MON = 0x01, /**< Monday */
	TAPI_MISC_NITZ_DOW_TUE = 0x02, /**< Tuesday */
	TAPI_MISC_NITZ_DOW_WED = 0x03, /**< Wednesday */
	TAPI_MISC_NITZ_DOW_THU = 0x04, /**< Thursday */
	TAPI_MISC_NITZ_DOW_FRI = 0X05, /**< Friday */
	TAPI_MISC_NITZ_DOW_SAT = 0x06  /**< Saturday */
} tapi_misc_nitz_dayofweek_t;

/**
 * @enum  tapi_misc_nitz_daylight_t
 * Enum type for daylight
 */
typedef enum {
	TAPI_MISC_NITZ_NOADJUST = 0x00, /**< No daylight adjustment */
	TAPI_MISC_NITZ_ONE_HR = 0x01, /**< One hour */
	TAPI_MISC_NITZ_TW0_HR = 0x02  /**< two hours */
} tapi_misc_nitz_daylight_t;

/**
 * @enum  tapi_misc_nitz_timeinfo_t
 * Enum type for types of data
 */
typedef enum {
	TAPI_MISC_NITZ_INVALID			= 0x00, /**< Invalid */
	TAPI_MISC_NITZ_TIMEINFO_NW_CDMA = 0x01, /**< Time info network */
	TAPI_MISC_NITZ_TIMEINFO_NW_GSM	= 0x02, /**< Time info network */
	TAPI_MISC_NITZ_RTC_INFO_LINE	= 0x03, /**< RTC Info line */
	TAPI_MISC_NITZ_RTC_BKUP_PHONE	= 0x04, /**< RTC Backup phone */
	TAPI_MISC_NITZ_NO_TIME_INFO	= 0x05  /**< No time info */
} tapi_misc_nitz_timeinfo_t;

/**
 * Mobile Equipment Version Information
 */
typedef struct {
	unsigned char ver_mask; /**< version mask  - 0x01:SW_ver, 0x02:HW_ver, 0x04:RF_CAL_date, 0x08:Product_code, 0x10:Model_ID, 0x20:PRL, 0x04:ERI, 0xff:all */
	unsigned char szSwVersion[MAX_VERSION_LEN]; /**< Software version, null termination */
	unsigned char szHwVersion[MAX_VERSION_LEN]; /**< Hardware version, null termination */
	unsigned char szRfCalDate[MAX_VERSION_LEN]; /**< Calculation Date, null termination */
	unsigned char szProductCode[TAPI_MISC_PRODUCT_CODE_LEN_MAX]; /**< product code, null termination */
	unsigned char szModelId[TAPI_MISC_MODEL_ID_LEN_MAX]; /**< model id (only for CDMA), null termination */
	unsigned char prl_nam_num; /**< number of PRL NAM fields */
	unsigned char szPrlVersion[TAPI_MISC_PRL_ERI_VER_LEN_MAX * 3];/**< prl version (only for CDMA), null termination */
	unsigned char eri_nam_num; /**< number of PRL NAM fields */
	unsigned char szEriVersion[TAPI_MISC_PRL_ERI_VER_LEN_MAX * 3];/**< eri version (only for CDMA), null termination */
} TelMiscVersionInformation;

/**
 * Mobile Equipment Serial Number Information
 */
typedef struct {
	TelMiscSNIndexType_t sn_index; /**< serial number index */
	int sn_len; /**< Length */
	unsigned char szNumber[TAPI_MISC_ME_SN_LEN_MAX]; /**< Number */
} TelMiscSNInformation;

/**
 * Event delivery - event type
 */
typedef struct {
	struct timeval time; /**< Time information */
	unsigned short type; /**< Type of event */
	unsigned short code; /**< Code */
	long value; /**< Value */
} input_event_t;

/**
 * Diagnostic Key Code Information
 */
typedef struct {
	unsigned char keycode; /**< Key Code */
	unsigned char keyhold; /**< Key Hold */
} TelMiscDiagKeyCodeInfomation;

/**
 * Struct for alarm info
 */
typedef struct {
	unsigned char year; /**< year */
	unsigned char month; /**< month */
	unsigned char day; /**< day */
	unsigned char hour; /**< hour */
	unsigned char minute; /**< minute */
	unsigned char second; /**< second */
	unsigned char time_zone; /**< time zone */
	tapi_misc_nitz_dayofweek_t day_of_week; /**< day of week */
} TelMiscAlarmInfo_t;

/**
 * Struct for alarm response
 */
typedef struct {
	TelMiscAlarmMode_t validity; /**< validity */
	TelMiscAlarmInfo_t alarm_info; /**< alarm info */
} TelMiscAlarmStatus_t;

/**
 * @struct  TelMiscNamInfo_t
 * struct type for nam info
 */
typedef struct {
	unsigned short nam_info_mask;
	unsigned char nam_total; // this field is not used in case of nam set req.
	unsigned char nam_current_index;
	unsigned char nam_req_index;
	unsigned char nam_auto;
	unsigned char nam_mcc[TAPI_MISC_MCC_LEN_MAX+1];
	unsigned char nam_mnc[TAPI_MISC_MCC_LEN_MAX+1];
	unsigned char nam_min[TAPI_MISC_MIN_LEN_MAX+1];
	unsigned char nam_mdn[TAPI_MISC_MDN_LEN_MAX+1];
	unsigned char nam_name[TAPI_MISC_NAME_LEN_MAX+1];
} TelMiscNamInfo_t;

/**
 * Struct for time info
 */
typedef struct {
	tapi_misc_nitz_timeinfo_t time_zone_valid; /**< time zone */
	int bDaylight_valid; /**< day light */
	unsigned char year; /**< year */
	unsigned char month; /**< month */
	unsigned char day; /**< day */
	unsigned char hour; /**< hour */
	unsigned char minute; /**< minute */
	unsigned char second; /**< second */
	unsigned char time_zone; /**< time zone */
	tapi_misc_nitz_daylight_t daylight_adjust; /**< day light adjust */
	tapi_misc_nitz_dayofweek_t day_of_week; /**< day of week */
	unsigned long plmn; /**< day of week */
} tapi_misc_time_zone_info_type;

#ifdef __cplusplus
}
#endif

#endif // _TEL_MISC_H_
/**
 *  @}
 */
