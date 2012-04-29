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
 * @ingroup              TelephonyAPI
 * @addtogroup	  MISC_TAPI       MISC
 * @{
 *
 * @file ITapiMisc.h

 @brief This file serves as a "C" header file defines functions for Tapi Misc Services.\n
 It contains a sample set of function prototypes that would be required by applications.
 This header file defines the Telephony - MISC - API's.  These API's will be used by the
 processing Nitz and RTC time and some MISC functions.
 */
#ifndef _ITAPI_MISC_H_
#define _ITAPI_MISC_H_

#include <TelDefines.h>
#include <TelUtility.h>
#include <TelMisc.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * @brief  This function is invoked to Confirm Time zone Info.
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
 *
 * @param  [in]  timezone_info
 * -  timezone_info.
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
int tel_confirm_misc_timezone_Info(tapi_misc_time_zone_info_type *timezone_info);

/**
 *
 * @brief  This function is invoked to Get Time zone Mode.
 *
 * @par Sync (or) Async:
 *  This is a Synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [out]  bTimezone_valid
 * -  specifies whether true or false.
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
int tel_get_misc_timezone_mode(int *bTimezone_valid);

/**
 *
 * @brief  This function is invoked to Set Time zone Information
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [in]  timezone_info
 *    -  specifies the time zone information
 *
 * @par Async Response Message:
 * - NA
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
int tel_set_misc_timezone_info(tapi_misc_time_zone_info_type *timezone_info);

/**
 *
 * @brief  This function is invoked to  Get Time zone Information
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param  [in]
 * - None.
 *
 * @par Async Response Message:
 * - NA
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
int tel_get_misc_timezone_info(void);

/**
 *
 * @brief  This function is invoked to Get ME version information.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [out]  ver_info
 *    - SW,HW version, RF calibration date, product code, model id(CDMA only), PRL/ERI information(CDMA only)
 *	refer #TelMiscVersionInformation
 *
 * @par Async Response Message:
 * - NA
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
int tel_get_misc_me_version(TelMiscVersionInformation *ver_info);

/**
 *
 * @brief  This function is invoked to Get ME IMEI/ESN/MEID for each phone type.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  sn_index
 *    - serial type which user wants to get from current device. Refer #TelMiscSNIndexType_t
 *
 * @param  [out]  sn_info
 *    - Device Serial number information. IMEI for GSM/UMTS device, ESN or MEID for CDMA device. Refer #TelMiscSNInformation
 *
 * @par Async Response Message:
 * - NA
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
int tel_get_misc_me_sn(TelMiscSNIndexType_t sn_index, TelMiscSNInformation *sn_info);

/**
 *
 * @brief  This function is invoked to Get NAM info.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  nam_info_mask, nam_index
 *    - nam info mask & index
 *
 * @par Async Response Message:
 * - NA
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
int tel_get_misc_nam_info(unsigned short nam_info_mask, TelMiscNamIndex_t nam_index);

/**
 *
 * @brief  This function is invoked to Set NAM info.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  nam_info
 *    - nam info
 *
 * @par Async Response Message:
 * - NA
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
int tel_set_misc_nam_info(TelMiscNamInfo_t *nam_info);

/**
 *
 * @brief  This function is used to enable emergency mode
 * when device goes into emergency mode, user can not use normal call, SMS, and MMS service excepting emergency call.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [in]  mode
 *    - emergency mode value to request. 0:disable, 1:enable
 *
 * @par Async Response Message:
 * - NA
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
int tel_set_misc_emergency_mode(int mode);

/**
 *
 * @brief  This function is used to get current device emergency mode status.
 *
 * @par Sync (or) Async:
 * This is a synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param  [out] mode
 *    - current emergency mode
 *
 * @par Async Response Message:
 * - NA
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
int tel_get_misc_emergency_mode(int *mode);

#ifdef __cplusplus
}
#endif

#endif // _ITAPI_MISC_H_
/**
 * @}
 */
