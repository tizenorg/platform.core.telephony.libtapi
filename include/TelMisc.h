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
 * @file TelMisc.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_MODEM
 * @{
 */

#ifndef _TEL_MISC_H_
#define _TEL_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Definition for the maximum length of version.
 * @since_tizen 2.3
 */
#define MAX_VERSION_LEN 32

/**
 * @brief Definition for the maximum length of serial number.
 * @since_tizen 2.3
 */
#define TAPI_MISC_ME_SN_LEN_MAX 32

/**
 * @brief Definition for the maximum length of product code.
 * @since_tizen 2.3
 */
#define TAPI_MISC_PRODUCT_CODE_LEN_MAX 32

/**
 * @brief Definition for the maximum length of model ID.
 * @since_tizen 2.3
 */
#define TAPI_MISC_MODEL_ID_LEN_MAX 17

/**
 * @brief Definition for the maximum length of PRL version.
 * @since_tizen 2.3
 */
#define TAPI_MISC_PRL_ERI_VER_LEN_MAX 17

/**
 * @brief Definition for the maximum length of device name.
 * @since_tizen 2.3
 */
#define TAPI_MISC_ME_DEVICE_NAME_LEN_MAX 32

/**
 * @brief The structure type for Mobile Equipment Version Information.
 * @since_tizen 2.3
 * @see tel_get_misc_me_version()
 * @see tel_get_misc_me_version_sync()
 */
typedef struct {
	unsigned char ver_mask; /**< Version mask - 0x01:SW_ver, 0x02:HW_ver, 0x04:RF_CAL_date, 0x08:Product_code, 0x10:Model_ID, 0x20:PRL, 0x04:ERI, 0xff:all */
	unsigned char szSwVersion[MAX_VERSION_LEN]; /**< Software version, null termination */
	unsigned char szHwVersion[MAX_VERSION_LEN]; /**< Hardware version, null termination */
	unsigned char szRfCalDate[MAX_VERSION_LEN]; /**< Calculation Date, null termination */
	unsigned char szProductCode[TAPI_MISC_PRODUCT_CODE_LEN_MAX]; /**< Product code, null termination */
	unsigned char szModelId[TAPI_MISC_MODEL_ID_LEN_MAX]; /**< Model ID (only for CDMA), null termination */
	unsigned char prl_nam_num; /**< Number of PRL NAM fields */
	unsigned char szPrlVersion[TAPI_MISC_PRL_ERI_VER_LEN_MAX * 3];/**< PRL version (only for CDMA), null termination */
	unsigned char eri_nam_num; /**< Number of PRL NAM fields */
	unsigned char szEriVersion[TAPI_MISC_PRL_ERI_VER_LEN_MAX * 3];/**< ERI version (only for CDMA), null termination */
} TelMiscVersionInformation;

/**
 * @brief The structure type for Mobile Equipment Serial Number Information.
 * @since_tizen 2.3
 * @see tel_get_misc_me_sn()
 * @see tel_get_misc_me_sn_sync()
 */
typedef struct {
	unsigned char szEsn[TAPI_MISC_ME_SN_LEN_MAX]; /**< Number */
	unsigned char szMeid[TAPI_MISC_ME_SN_LEN_MAX]; /**< Number */
	unsigned char szImei[TAPI_MISC_ME_SN_LEN_MAX]; /**< Number */
	unsigned char szImeiSv[TAPI_MISC_ME_SN_LEN_MAX]; /**< Number */
} TelMiscSNInformation;

/**
 * @brief The structure type for device info of cellular dongle.
 * @since_tizen 2.4
 * @see tel_get_device_info()
 */
typedef struct {
	unsigned char vendor_name[TAPI_MISC_ME_DEVICE_NAME_LEN_MAX + 1]; /**< Vendor name */
	unsigned char device_name[TAPI_MISC_ME_DEVICE_NAME_LEN_MAX + 1]; /**< Device name */
} TelMiscDeviceInfo_t;

#ifdef __cplusplus
}
#endif

#endif /* _TEL_MISC_H_ */

/**
 * @}
 */
