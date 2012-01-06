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
 * @addtogroup	CFG_TAPI	CFG
 * @{
 *
 * CFG APIs allow an application to accomplish the following services: @n
 * - set or get modem configuration. @n
 */

#ifndef _ITAPI_CFG_H_
#define _ITAPI_CFG_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/
#include <TelDefines.h>
#include <TelErr.h>
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
#define TAPI_CFG_A_KEY_DIGITS_MAX			26	/**< Maximum length of A-Key Digits */
#define TAPI_CFG_MSL_CODE_MAX				6	/**< Maximum length of MSL codes */

/*==================================================================================================
 ENUMS
 ==================================================================================================*/

/* DEFAULT_MODE field */
typedef enum {
	TAPI_CFG_DEFAULT_FULL = 0x01, /* 0x01 : Full reset */
	TAPI_CFG_DEFAULT_FACTORY, /* 0x02 : Factory reset */
	TAPI_CFG_DEFAULT_SERVICE, /* 0x03 : Service reset */
	TAPI_CFG_DEFAULT_CUSTOM, /* 0x04 : Custom reset */
	TAPI_CFG_DEFAULT_MODE_MAX
} TelDefaultCfg_t;

/**
 * @enum TelCfgAKeyVerifyResult_t
 *  This enumeration defines the results for verification of A-Key.
 */
typedef enum {
	TAPI_CFG_AKEY_VERIFY_FAIL = 0x00, /**< A-Key verify fail */
	TAPI_CFG_AKEY_VERIFY_OK /**< A-Key verify success */
} TelCfgAKeyVerifyResult_t;

/**
 * @enum TelCfgOtkslFlag_t
 *  This enumeration defines OTKSL(One Time Key Subsidy Lock) flag.
 */
typedef enum {
	TAPI_CFG_OTKSL_FLAG_OFF = 0x00, /**< otksl flag off */
	TAPI_CFG_OTKSL_FLAG_ON /**< otksl flag on */
} TelCfgOtkslFlag_t;

/**
 * @enum TelCfgVocOption_t
 *  This enumeration defines vocoder option.
 */
typedef enum {
	TAPI_CFG_VOC_OPTION_DEFAULT = 0x00, /**< vocoder option is default */
	TAPI_CFG_VOC_OPTION_EVRC, /**< vocoder option is evrc */
	TAPI_CFG_VOC_OPTION_13K, /**< vocoder option is 13k */
	TAPI_CFG_VOC_OPTION_8K, /**< vocoder option is 8k */
	TAPI_CFG_VOC_OPTION_RESERVED /**< vocoder option is reserved */
} TelCfgVocOption_t;

/**
 * @enum TelCfgHiddenMenuAccessType_t
 *  This enumeration defines hidden menu access type.
 */
typedef enum {
	TAPI_CFG_HIDDEN_MENU_DISABLE = 0x00, /**< hidden menu disabled */
	TAPI_CFG_HIDDEN_MENU_ENABLE /**< hidden menu enabled */
} TelCfgHiddenMenuAccessType_t;

/**
 * @enum TelCfgCurrentCarrier_t
 *  This enumeration defines current carrier code.
 */
typedef enum {
	TAPI_CFG_CARRIER_TEST = 0x00,	/**< TEST */
	TAPI_CFG_CARRIER_SKT,			/**< SKT */
	TAPI_CFG_CARRIER_KTF,			/**< KTF */
	TAPI_CFG_CARRIER_LGT,			/**< LGT */
	TAPI_CFG_CARRIER_VERIZON,		/**< VERIZON */
	TAPI_CFG_CARRIER_SPRINT,		/**< SPRINT */
	TAPI_CFG_CARRIER_ALLTEL,		/**< ALLTEL */
	TAPI_CFG_CARRIER_TELUS,			/**< TELUS */
	TAPI_CFG_CARRIER_BMC,			/**< BMC */
	TAPI_CFG_CARRIER_ALIANT,		/**< ALIANT */
	TAPI_CFG_CARRIER_SASKTEL,		/**< SASKTEL */
	TAPI_CFG_CARRIER_MTS,			/**< MTS */
} TelCfgCurrentCarrier_t;

/**
 * @enum TelCfgSIOMode_t
 *  This enumeration defines a SIO mode.
 */
typedef enum {
	TAPI_CFG_SIO_U1_DIAG_USB2_GPS = 0x00, /**< U1_DIAG_USB2_GPS */
	TAPI_CFG_SIO_U1_HFK_USB2_GPS, /**< U1_HFK_USB2_GPS */
	TAPI_CFG_SIO_U1_HFK_USB2_DIAG /**< U1_HFK_USB2_DIAG */
} TelCfgSIOMode_t;

/**
 * @enum TelCfgUSBPath_t
 *  This enumeration defines a USB path.
 */
typedef enum {
	TAPI_CFG_USB_PATH_TO_PDA = 0x00, /**< 0x00 : PDA uses USB */
	TAPI_CFG_USB_PATH_TO_PHONE /**< 0x01 : Phone uses USB */
} TelCfgUSBPath_t;

/**
 * @enum TelCfgTTYMode_t
 *  This enumeration defines a TTY mode
 */
typedef enum {
	TAPI_CFG_TTY_MODE_DISABLE = 0x00, /**< 0x00 : TTY mode off */
	TAPI_CFG_TTY_MODE_ENABLE_TALK, /**< 0x01 : TTY mode talk */
	TAPI_CFG_TTY_MODE_ENABLE_HEAR, /**< 0x01 : TTY mode hear */
	TAPI_CFG_TTY_MODE_ENABLE_FULL /**< 0x01 : TTY mode full */
} TelCfgTTYMode_t;

/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/**
 * This structure contains A-Key information.
 */
typedef struct {
	char A_key[TAPI_CFG_A_KEY_DIGITS_MAX + 1]; /**< A-Key */
} TelCfgAKeyInfo_t;

/**
 * This structure contains MSL information.
 */
typedef struct {
	char MslCode[TAPI_CFG_MSL_CODE_MAX+1]; /**< MSL(Master Subsidy Lock) code */
	char OtkslCode[TAPI_CFG_MSL_CODE_MAX+1]; /**< OTKSL(One Time Key Subsidy Lock) code */
	int bOtksl_flag; /**< default is FALSE. Whenever the one time key is used , it can be changed to TRUE*/
} TelCfgMSLInfo_t;

/**
 * This structure contains activation date information.
 */
typedef struct {
	unsigned short year; /**< Year (actual year - 1980) */
	unsigned char month; /**< Month (1 ~ 12) */
	unsigned char day; /**< Day (1 ~ 31) */
	unsigned char hour; /**< Hour (0 ~ 23) */
	unsigned char minute; /**< Minute (0 ~ 59) */
	unsigned char second; /**< Second (0 ~ 59) */
} TelCfgActivationDateInfo_t;

/**
 * This structure contains reconditioned date information.
 */
typedef struct {
	unsigned char status; /**< Reconditioned Status (0x00 : NO, 0x01 : YES) */
	unsigned short year; /**< Year (actual year - 1980) */
	unsigned char month; /**< Month (1 ~ 12) */
	unsigned char day; /**< Day (1 ~ 31) */
	unsigned char hour; /**< Hour (0 ~ 23) */
	unsigned char minute; /**< Minute (0 ~ 59) */
	unsigned char second; /**< Second (0 ~ 59) */
} TelCfgReconditionedInfo_t;

/*==================================================================================================
  FUNCTION PROTOTYPES
  ==================================================================================================*/

/**
 * @brief  This function is used to set Modem default configuration,
 *           Telephony will send Get operation to modem to get the items from Modem.
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
 * @param[in] bType
 * - type of default configuration
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 * - The event associated is TAPI_CFG_EVENT_DEFAULTCFG_IND  and the event data is default configuration mode.(Refer #TelDefaultCfg_t)
 *    Asynchronous return status is always TAPI_API_SUCCESS.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 */
int tel_set_cfg_default_configuration(TelDefaultCfg_t bType);

/**
 * @brief  This function is used to get A-Key.(CDMA only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - It can be used in NAM mode.
 *
 * @warning
 * - None.
 *
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 * - The event associated is TAPI_EVENT_CFG_GET_A_KEY_CNF. The event data is A-key.(Refer #TelCfgAKeyInfo_t)
 *    Asynchronous return status is always TAPI_API_SUCCESS.
 *
 * @return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_get_cfg_akey(void);

/**
 * @brief  it is used to verify A-Key.(CDMA only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - It can be used in NAM mode.
 *
 * @warning
 * - None.
 *
 * @param[in] pAKey_info
 * - A-Key info. Maximum length of A-Key digit is 26.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_VERIFY_A_KEY_CNF. The event data is verification result which is indicated success or not.(Refer #TelCfgAKeyVerifyResult_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_verify_cfg_akey( const TelCfgAKeyInfo_t* pAKey_info);

/**
 * @brief  It is used to get MSL and OTKSL(CDMA Only).
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - MSL is used to password in NAM mode.
 * - MSL means Master subside Key code, which is the developer always enter the NAM mode using this code.
 * - OTKSL means One time Key code, which is the carrier representative only enter the NAM mode one time using this code.
 * - OTKSL_FLAG default value is false, whenever the one time key is used , it can be changed to TRUE.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_MSL_INFO_CNF. The event data is MSL information.(Refer #TelCfgMSLInfo_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 *
 */
int tel_get_cfg_msl_info(void);

/**
 * @brief  It is used to get vocoder option. (CDMA Only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - It can be used to need a current vocoder option in phone.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_VOCODER_OPTION_CNF. The event data is vocoder option.(Refer #TelCfgVocOption_t)
 *
 * @return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_get_cfg_vocoder_option(void);

/**
 * @brief  It is used to set vocoder option. (CDMA Only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in] option
 * - vocoder option.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_SET_VOCODER_OPTION_CNF. There is no event data.
 *
 * @return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_set_cfg_vocoder_option(TelCfgVocOption_t option);

/**
 * @brief  It informs whether is possible or impossible to access hidden menu.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_HIDDEN_MENU_ACCESS_CNF. The event data is hidden menu access mode.(Refer #TelCfgHiddenMenuAccessType_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_get_cfg_hidden_menu_access(void);

/**
 * @brief  It is used to set whether to access hidden menu or not.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in] type
 * - hidden menu access type(disable or enable).
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_SET_HIDDEN_MENU_ACCESS_CNF. There is no event data.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_set_cfg_hidden_menu_access(TelCfgHiddenMenuAccessType_t type);

/**
 * @brief  It requests a current carrier code.(CDMA only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - none.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_CURRENT_CARRIER_CNF. The event data is type of current carrier.(Refer #TelCfgCurrentCarrier_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 *
 */
int tel_get_cfg_serving_carrier(void);

/**
 * @brief  It sets a current carrier code.(CDMA only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]  Carrier
 * - current carrier.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_SET_CURRENT_CARRIER_CNF. There is no event data.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_set_cfg_serving_carrier(TelCfgCurrentCarrier_t Carrier);

/**
 * @brief  It requests a current SIO mode.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - none.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_SIO_MODE_CNF. The event data is sio mode.(Refer #TelCfgSIOMode_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_get_cfg_sio_mode(void);

/**
 * @brief  It sets a current SIO mode.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]   Mode
 * - SIO mode.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_SET_SIO_MODE_CNF. There is no event data.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 *
 *
 */
int tel_set_cfg_sio_mode(TelCfgSIOMode_t Mode);

/**
 * @brief  It requests activation date.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_ACTIVATION_DATE_CNF. The event data is activated date of phone.(Refer #TelCfgActivationDateInfo_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_get_cfg_activated_date_of_phone(void);

/**
 * @brief  It requests reconditioned status and date.
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_RECONDITIONED_DATE_CNF. The event is reconditioned date of phone.(Refer #TelCfgReconditionedInfo_t)
 *
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 *
 */
int tel_get_cfg_reconditioned_data_of_phone(void);

/**
 * @brief  It requests to get tty(text telephone) mode.(CDMA only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]
 * - None.
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_GET_TTY_MODE_CNF. The event data is tty mode.(Refer #TelCfgTTYMode_t)
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_get_cfg_tty_mode(void);

/**
 * @brief  It requests to set tty(text telephone) mode.(CDMA only)
 *
 * @par Sync (or) Async:
 * - This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param[in]  Mode
 * - TTY mode
 *
 * @param[out]
 * - None
 *
 * @par Async Response Message:
 *  - The event associated is TAPI_EVENT_CFG_SET_TTY_MODE_CNF. There is no event data.
 *
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Otherwise it provides an error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * - External Apps.
 */
int tel_set_cfg_tty_mode(TelCfgTTYMode_t Mode);

#ifdef __cplusplus
}
#endif

#endif // _ITAPI_CFG_H_
/**
 * @}
 */

