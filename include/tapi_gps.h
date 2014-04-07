/*
 * libtapi
 *
 * Copyright (c) 2013 Samsung Electronics Co. Ltd. All rights reserved.
 * Copyright (c) 2013 Intel Corporation. All rights reserved.
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
*  @addtogroup TAPI_GPS
*  @{
*
*  @file tapi_gps.h
*  @brief Telephony GPS interface
*/

#ifndef __TAPI_GPS_H__
#define __TAPI_GPS_H__

#include <tapi.h>
#include <tel_gps.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  @brief This function sends the confirmation data for measure position message
 *
 *  @par Sync (or) Async:
 *  Synchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_FAILURE</b>: Operation Failed\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] gps_data GPS data info
 */
TelReturn tapi_gps_confirm_measure_pos(TelHandle *handle, TelGpsDataInfo *gps_data);


/**
 *  @brief This function is invoked to set the gps frequency aiding and allows the underlying OEM provider to scan the set band
 *
 *  @par Sync (or) Async:
 *  Asynchronous API
 *
 *  @return Possible #TelReturn values:\n
 *  <b>TEL_RETURN_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_RETURN_INVALID_PARAMETER</b>: Invalid input parameters
 *
 *  @param[in] handle #TelHandle obtained from tapi_init()
 *  @param[in] state <b>TRUE</b>: Frequency aiding ON\n <b>FALSE</b>: Frequency aiding OFF
 *  @param[in] callback Response callback
 *  @param[in] user_data Callback user data
 *
 *  @par Async Response:
 *  \em result Possible #TelModemResult values:\n
 *  <b>TEL_GPS_RESULT_SUCCESS</b>: Operation completed successfully\n
 *  <b>TEL_GPS_RESULT_INVALID_PARAMETER</b>: Invalid input parameters\n
 *  <b>TEL_GPS_RESULT_OPERATION_NOT_SUPPORTED</b>: Operation not supported\n
 *  \em data NULL
 */
TelReturn tapi_gps_set_frequency_aiding(TelHandle *handle, gboolean state, TapiResponseCb callback, void *user_data);


#ifdef __cplusplus
}
#endif

#endif	/* __TAPI_GPS_H__ */

/**
 * @}
 */
