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
 * @file TelOem.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_OEM
 * @{
 */

#ifndef _TEL_OEM_H_
#define _TEL_OEM_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief OEM data Request & Response structure - typedef #struct oem_data
 * @since_tizen 2.4
 */
typedef struct oem_data TelOemData_t;

/**
 * @brief OEM data Notification structure - typedef #struct oem_data
 * @since_tizen 2.4
 */
typedef struct oem_data TelOemNotiData_t;

/**
 * @brief OEM data base structure
 * @since_tizen 2.4
 * @see tel_send_oem_data_sync()
 * @see tel_send_oem_data_async()
 */
struct oem_data {
	unsigned int oem_id; /**< Oem ID */
	unsigned int data_len; /**< Oem data length */
	unsigned char *data; /**< Oem data */
};

#ifdef __cplusplus
}
#endif

#endif  /* _TEL_OEM_H_ */
/**
 * @}
 */
