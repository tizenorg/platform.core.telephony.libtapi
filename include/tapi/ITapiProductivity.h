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
 * @addtogroup	PRODUCTIVITY_TAPI	PRODUCTIVITY_TAPI
 * @{
 *
 * @file ITapiProductivity.h

 @brief This file serves as a "C" header file defines functions for Tapi productivity Services.\n
 It contains a sample set of function prototypes that would be required by applications.
 */

#ifndef _ITAPI_SERVICE_MODE_H_
#define _ITAPI_SERVICE_MODE_H_

#include <TelDefines.h>
#include <TelErr.h>
#include <TelUtility.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TAPI_MAX_SERVICE_LINES		16	/** service information display max length */
#define TAPI_MAX_LCD_WIDTH			31	/** LCD max width for display */

#define MAX_FACTORY_OMISSION_AVOIDANCE_DATA_CNT	90
#define TAPI_FACTORY_WIFI_DATA_PDA_MAX	13	/** wifi data pda max length */
#define TAPI_FACTORY_WIFI_DATA_PC_MAX	16	/** wifi data pc max length */
#define TAPI_FACTORY_BT_MACADDRESS_MAX	6	/** BT mac address max length */
#define TAPI_FACTORY_PIN_CODE_MAX		4	/** pin code max length */
#define TAPI_FACTORY_LINE_SMS_ADDR_MAX	20	/** line sms address max length */
#define TAPI_FACTORY_LINE_SMS_DATA_MAX	256	/** line sms data max length */
#define TAPI_FACTORY_AXIS_MAX			4	/** axis max length */
#define TAPI_FACTORY_MEM_TOTAL_SIZE_MAX	8	/** max of memory total size */
#define TAPI_FACTORY_MEM_USED_SIZE_MAX	8	/** max of used memory size */
#define TAPI_FACTORY_MEM_FREE_SIZE_MAX	8	/** max of free memory size */
#define TAPI_FACTORY_TOUCH_STATUS_MAX	4	/** max if touch status */

#define SMS_NUM_SIZE	64	/**< sms number max size for factory dft*/
#define SMS_DATA_SIZE	255	/**< sms data max size for factory dft*/

/**
 * @enum tapi_service_mode_t
 * This enumeration defines the service mode type for test mode.
 */
typedef enum {
	TAPI_SVC_MODE_TEST_MANUAL = 0x01, /**< 0x01 : Manual test mode */
	TAPI_SVC_MODE_NAM_EDIT, /**< 0x02 : Full NAM edit mode */
	TAPI_SVC_MODE_MONITOR, /**<0x03 : Monitor screen mode */
	TAPI_SVC_MODE_TEST_AUTO, /**< 0x04 : Auto test mode */
	TAPI_SVC_MODE_NAM_BASIC_EDIT, /**< 0x05 : Basic NAM edit mode */
	TAPI_SVC_MODE_PHONE_TEST, /**< 0x06 : Phone test mode ( just for the debugging ) */
	TAPI_SVC_MODE_OPERATOR_SPECIFIC_TEST /**< 0x07: Specific test mode required by operator*/
} tapi_service_mode_t;

/**
 * @enum tapi_test_mode_sub_t
 * This enumeration defines the test mode sub type.
 */
typedef enum {
	TAPI_TESTMODE_ENTER	=	0x1000,				/**< 0x1000 : Testmode enter */
	TAPI_TESTMODE_SW_VERSION_ENTER,				/**< 0x1001 : SW_version enter */
	TAPI_TESTMODE_FTA_SW_VERSION_ENTER,			/**< 0x1002 : FTA SW version enter */
	TAPI_TESTMODE_FTA_HW_VERSION_ENTER,			/**< 0x1003 : FTA HW version enter */
	TAPI_TESTMODE_ALL_VERSION_ENTER,			/**< 0x1004 : All version enter  */
	TAPI_TESTMODE_BATTERY_INFO_ENTER,			/**< 0x1005 : Battery Information enter  */
	TAPI_TESTMODE_CIPHERING_PROTECTION_ENTER,	/**< 0x1006 : Ciphering protection enter */
	TAPI_TESTMODE_INTEGRITY_PROTECTION_ENTER,	/**< 0x1007 : Integrity protection enter */
	TAPI_TESTMODE_IMEI_READ_ENTER,				/**< 0x1008 : IMEI enter */
	TAPI_TESTMODE_BLUETOOTH_TEST_ENTER,			/**< 0x1009 : Bluetooth test enter */
	TAPI_TESTMODE_VIBRATOR_TEST_ENTER,			/**< 0x100A : Vibrator test enter */
	TAPI_TESTMODE_MELODY_TEST_ENTER,			/**< 0x100B : Melody test enter */
	TAPI_TESTMODE_MP3_TEST_ENTER,				/**< 0x100C : MP3 test enter */
	TAPI_TESTMODE_FACTORY_RESET_ENTER,			/**< 0x100D : Factory test enter */
	TAPI_TESTMODE_FACTORY_PRECONFIG_ENTER,		/**< 0x100E : Factory preconfig enter */
	TAPI_TESTMODE_TFS4_EXPLORE_ENTER,			/**< 0x100F : TFS4 explore enter */
	TAPI_TESTMODE_RTC_TIME_DISPLAY_ENTER,		/**< 0x1010 : RTC time display enter */
	TAPI_TESTMODE_RSC_FILE_VERSION_ENTER,		/**< 0x1011 : RSC file version enter */
	TAPI_TESTMODE_USB_DRIVER_ENTER,				/**< 0x1012 : USB driver enter */
	TAPI_TESTMODE_USB_UART_DIAG_CONTROL_ENTER,	/**< 0x1013 : USB UART diag control enter*/
	TAPI_TESTMODE_RRC_VERSION_ENTER,			/**< 0x1014 : RRC Version enter */
	TAPI_TESTMODE_GPSONE_SS_TEST_ENTER,	/**< 0x1015 : GPSone testmode enter */
	TAPI_TESTMODE_BAND_SEL_ENTER,				/**< 0x1016 : Band selection mode enter */
	TAPI_TESTMODE_GCF_TESTMODE_ENTER,			/**< 0x1017 : GCF Test mode enter */
	TAPI_TESTMODE_GSM_FACTORY_AUDIO_LB_ENTER,	/**< 0x1018 : GSM Factory Audio Loopback enter */
	TAPI_TESTMODE_FACTORY_VF_TEST_ENTER,		/**< 0x1019 : VF ADC Test mode enter */
	TAPI_TESTMODE_TOTAL_CALL_TIME_INFO_ENTER,	/**< 0x101A : Display Total Call time info enter */
	TAPI_TESTMODE_SELLOUT_SMS_ENABLE_ENTER,		/**< 0x101B : Sell_Out_SMS_Enable_enter */
	TAPI_TESTMODE_SELLOUT_SMS_DISABLE_ENTER,	/**< 0x101C : Sell_Out_SMS_Disable_enter */
	TAPI_TESTMODE_SELLOUT_SMS_TEST_MODE_ON,		/**< 0x101D : Sell_Out_SMS_Test_Mode_ON */
	TAPI_TESTMODE_SELLOUT_SMS_PRODUCT_MODE_ON,	/**< 0x101E : Sell_Out_SMS_Product_Mode_ON */
	TAPI_TESTMODE_GET_SELLOUT_SMS_INFO_ENTER,	/**< 0x101F Get Sellout SMS info enter  */
	TAPI_TESTMODE_WCDMA_SET_CHANNEL_ENTER,		/**< 0x1020 WCDMA Set Channel Enter */
	TAPI_TESTMODE_GPRS_ATTACH_MODEM_ENTER,		/**< 0x1021 GPRS Attach mode Enter */

	/*	In case of  SVC_MODE = SM_NAM  in CDMA	*/
	TAPI_SVC_MODE_NAM_CDMA_EDIT	=	0x2001,		/**< 0x2001 : NAM Full Edit Mode */
	TAPI_SVC_MODE_NAM_CDMA_BASIC_EDIT,			/**< 0x2002 : NAM Basic Edit Mode */
	TAPI_SVC_MODE_NAM_CDMA_ADVANCED_EDIT,		/**< 0x2003 : NAM Advanced Edit Mode */

	TAPI_TESTMODE_SUB_MODE_MAX
} tapi_test_mode_sub_t;

/**
 * @enum tapi_service_mode_reset_t
 * This enumeration defines the reset mode type for test mode.
 */
typedef enum {
	TAPI_SVC_MODE_END_NO_ACTION = 0x00, /**< 0x00 : RESET_NO_ACTION */
	TAPI_SVC_MODE_END_PHONE_ONLY_RESET, /**< 0x01 : RESET_PHONE_ONLY_REQUIRED */
	TAPI_SVC_MODE_END_BOTH_RESET /**< 0x02 : RESET_BOTH_REQUIRED */
} tapi_service_mode_reset_t;

/**
 * @enum tapi_service_mode_debug_cp_dump_t
 * This enumeration defines the cp dump type.
 */
typedef enum {
	TAPI_SVC_DBG_CP_DUMP_TARGET_ALL = 0x00, /**< cp dump option - all */
	TAPI_SVC_DBG_CP_DUMP_TARGET_MSG, /**< cp dump option - message */
	TAPI_SVC_DBG_CP_DUMP_TARGET_LOG, /**< cp dump option - log */
	TAPI_SVC_DBG_CP_DUMP_TARGET_LOG2, /**< cp dump option - log2 */
	TAPI_SVC_DBG_CP_DUMP_TARGET_RAM_DUMP, /**< cp dump option - ram dump */
	TAPI_SVC_DBG_CP_DUMP_TARGET_MAX /**< RFU */
} tapi_service_mode_debug_cp_dump_t;

/**
 * @enum tapi_service_mode_debug_ap_dump_t
 * This enumeration defines the ap dump type.
 */
typedef enum {
	TAPI_SVC_DBG_AP_DUMP_TARGET_RIL = 0x01, /**< ap dump option - ril */
	TAPI_SVC_DBG_AP_DUMP_TARGET_SYSTEM, /**< ap dump option - system */
	TAPI_SVC_DBG_AP_DUMP_TARGET_MAX /**< ap dump -RFU */
} tapi_service_mode_debug_ap_dump_t;

/**
 * @enum tapi_device_id_t
 * This enumeration defines the device information for test mode.
 */
typedef enum {
	TAPI_DEVICE_VIBRATION = 0x01, /**< vibration */
	TAPI_DEVICE_BACKLIGHT, /**< back light */
	TAPI_DEVICE_FRONTLCD, /**< front lcd */
	TAPI_DEVICE_KEYLED, /**< key led */
	TAPI_DEVICE_ALERTLED /**< alerted */
} tapi_device_id_t;

/**
 * @enum tapi_device_status_t
 * This enumeration defines the device status.
 */
typedef enum {
	TAPI_DEVICE_OFF = 0x00, /**< off */
	TAPI_DEVICE_ON, /**< on */
	TAPI_DEVICE_AUTO /**< auto */
} tapi_device_status_t;

/**
 * @enum tapi_keypad_t
 * This enumeration defines the keypad requirement.
 */
typedef enum {
	KEYPAD_NOT_NEEDED = 0x00, /**< keypad required */
	KEYPAD_NEEDED /**< keypad not required */
} tapi_keypad_t;

/**
 * @enum tapi_svcmode_event_t
 * This enumeration defines the service mode event type for test mode.
 */
typedef enum {
	TAPI_SVCMODE_EVENT_SVCMODE_START = 0, /**< start */
	TAPI_SVCMODE_EVENT_SVCMODE_END, /**< end */
	TAPI_SVCMODE_EVENT_SVCMODE_CHANGE, /** changed */
	TAPI_SVCMODE_EVENT_SVCMODE_SCREEN_CFG, /**< screen config */
	TAPI_SVCMODE_EVENT_SVCMODE_DISP_SCREEN, /**< display screen */
	TAPI_SVCMODE_EVENT_SVCMODE_DEVICE_TEST /**< device test */
} tapi_svcmode_event_t;

/**
 * @enum TelFactoryDftSmsOption_t
 * This enumeration defines the service mode type for test mode.
 */
typedef enum {
	TAPI_FACTORY_DFT_SMS_INBOX, /**< sms inbox */
	TAPI_FACTORY_DFT_SMS_DRAFTS, /**< sms draft */
	TAPI_FACTORY_DFT_SMS_OUTBOX, /**< sms outbox */
	TAPI_FACTORY_DFT_SMS_SENTBOX /**< sent box */
} TelFactoryDftSmsOption_t;

typedef enum {
	TAPI_FACTORY_OMISSION_AVOIDANCE_ITEM_WRITE = 0x01,
	TAPI_FACTORY_OMISSION_AVOIDANCE_ITEM_WRITE_FULL,
	TAPI_FACTORY_OMISSION_AVOIDANCE_LOG_ERASE_FULL,
	TAPI_FACTORY_OMISSION_AVOIDANCE_FAILDATA_WRITE,
	TAPI_FACTORY_OMISSION_AVOIDANCE_FAILDATA_WRITE_FULL,
} tapi_factory_omission_avoidance_write_cmd_t;

typedef enum {
	TAPI_FACTORY_OMISSION_AVOIDANCE_ITEM_READ = 0x01,
	TAPI_FACTORY_OMISSION_AVOIDANCE_ITEM_READ_FULL,
	TAPI_FACTORY_OMISSION_AVOIDANCE_LOG_READ_FULL,
	TAPI_FACTORY_OMISSION_AVOIDANCE_FAILDATA_READ,
	TAPI_FACTORY_OMISSION_AVOIDANCE_FAILDATA_READ_FULL,
} tapi_factory_omission_avoidance_read_cmd_t;

/**
 * This structure contains service mode end information.
 */
typedef struct {
	tapi_service_mode_t service_mode; /**< service mode */
	tapi_service_mode_reset_t reset; /**< service reset mode */
} tapi_service_mode_end_t;

/**
 * This structure contains screen configuration information.
 */
typedef struct {
	int line_number; /**< line number */
	tapi_keypad_t keypad; /**< keypad requirement */
} tapi_screen_config_t;

/**
 * This structure contains string information to display.
 */
typedef struct {
	char line; /**< line to display */
	char string[TAPI_MAX_LCD_WIDTH + 1 ]; /**< string information */
	int	reverse; /**< reverse mode */
} tapi_service_string_info_t;

/**
 * This structure contains display information.
 */
typedef struct
{
	char num_of_lines; /**< number of line to display */
	tapi_service_string_info_t display[ TAPI_MAX_SERVICE_LINES ]; /**< string information */
} tapi_service_display_info_t;

/**
 * This structure contains device information for test mode.
 */
typedef struct
{
	tapi_device_id_t device_id; /**< device id */
	tapi_device_status_t device_status; /**< device status */
} tapi_device_info_t;

/**
 * This structure contains factory dft sms information.
 */
typedef struct {
	TelFactoryDftSmsOption_t option; /**< factory dft sms option */
	char number[SMS_NUM_SIZE]; /**< number */
	char data[SMS_DATA_SIZE]; /**< data */
 }TelFactoryDftSmsInfo_t;

typedef union {
	tapi_factory_omission_avoidance_write_cmd_t write_cmd;
	tapi_factory_omission_avoidance_read_cmd_t read_cmd;
} tapi_factory_omission_avoidance_cmd_t;

typedef struct {
	unsigned char test_id;
	unsigned char test_result;
} tapi_factory_omission_avoidance_data_t;

typedef struct {
	tapi_factory_omission_avoidance_cmd_t cmd;
	unsigned int data_cnt;
	tapi_factory_omission_avoidance_data_t data[MAX_FACTORY_OMISSION_AVOIDANCE_DATA_CNT];
} tapi_factory_omission_avoidance_info_t;

/**
 *
 * @brief  This function is invoked to start service mode.
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  svc_mode, test_sub
 * -  svc_mode, test_sub. refer #tapi_service_mode_t, #tapi_test_mode_sub_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
TapiResult_t tel_start_svcmode(tapi_service_mode_t svc_mode, tapi_test_mode_sub_t test_sub);

/**
 *
 * @brief  This function is invoked to end service mode.
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  svc_mode
 * -  svc_mode. refer #tapi_service_mode_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_end_svcmode(tapi_service_mode_t svc_mode);

/**
 *
 * @brief  This function is invoked to process key code in service mode.
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  key_code
 * -  key_code.
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_send_svcmode_keycode(char key_code);

/**
 *
 * @brief  This function is invoked to dump cp debug.
 *
 * @par Sync (or) Async:
 *  This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  type
 * -  cp dump type. refer #tapi_service_mode_debug_cp_dump_t
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 * - None.
 *
 * @post
 * - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 *  External Apps.
 *
 */
int tel_request_svcmode_dump(tapi_service_mode_debug_cp_dump_t type);

/**
 * @brief  This function is used to indicate start factory process to modem,
 * Telephont will send Get operation to modem to get the items from Modem.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param void
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.(Current, Launcher is)
 *
 */
int tel_start_imei_process(void);

/**
 * @brief  This function is used to Indicate result for comparison item between PDA and Modem.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param nItemID, bResult
 * - Index of ItemID and Result after comparison.
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_compare_imei_item(unsigned char nItemID, unsigned char bResult);

/**
 * @brief  This function is used in factory process omission avoidance.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param
 * - cmd : cmd type
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_get_factory_omission_avoidance(tapi_factory_omission_avoidance_read_cmd_t cmd);

/**
 * @brief  This function is used in factory process omission avoidance.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous function.
 *
 * @par Important Notes:
 * - None
 *
 * @warning
 * - None.
 *
 * @param
 * - info : comman type, test id, test result
 *
 * @par Async Response Message:
 * - None
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - Integer '0' ( same with TAPI_API_SUCCESS )  - indicating that the operation has completed successfully. \n
 * - Negative integer : it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_factory_omission_avoidance(tapi_factory_omission_avoidance_info_t *info);

#ifdef __cplusplus
}
#endif

#endif /*_ITAPI_SERVICE_MODE_H_*/

/**
 * @}
 */
