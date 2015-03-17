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

#ifndef __TIZEN_TELEPHONY_SERVICE_DOC_H__
#define __TIZEN_TELEPHONY_SERVICE_DOC_H__

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_FRAMEWORK
 * @defgroup CAPI_TELEPHONY_SERVICE Telephony Service
 * @brief The Telephony Service module provides call, modem, network, phonebook, SAT, SIM, SMS, and SS services.
 *
 * @section CAPI_TELEPHONY_SERVICE_HEADER Required Header
 *   \#include <tapi_common.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_OVERVIEW Overview
 * The Telephony Service module provides call, modem, network, phonebook, SAT, SIM, SMS, and SS services.
 * @section CAPI_TELEPHONY_SERVICE_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_CALL Call
 * @brief
 * @section CAPI_TELEPHONY_SERVICE_CALL_HEADER Required Header
 *   \#include <ITapiCall.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_CALL_OVERVIEW Overview
 * Call APIs allow an application to accomplish the following services:
 * - Make a mobile originated Emergency call, Voice call, or Video call.
 * - Accept or Reject incoming calls.
 * - Release Calls.
 * - Call dependent supplementary services such as call hold, retrieve, DTMF.
 * - Multi-party call management, setup conference call, split conference call.
 * - Call information such as get call information, get line identification number.
 * - To verify whether a call (voice / video) is possible at any given time.
 *
 * @section CAPI_TELEPHONY_SERVICE_CALL_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_MODEM Modem
 * @brief
 *
 * @section CAPI_TELEPHONY_SERVICE_MODEM_HEADER Required Header
 *   \#include <ITapiModem.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_MODEM_OVERVIEW Overview
 * Modem APIs allow an application to accomplish the following services:
 * - Process power command
 * - Reset the phone power (on / off)
 * - Process airplane mode
 *
 * @section CAPI_TELEPHONY_SERVICE_MODEM_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_NETWORK Network
 * @brief
 * @section CAPI_TELEPHONY_SERVICE_NETWORK_HEADER Required Header
 *   \#include <ITapiNetwork.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_NETWORK_OVERVIEW Overview
 * Network APIs allow an application to accomplish the following services:
 * - Search and Select Network
 * - Get Network information
 * - Get and Set Preferred PLMN
 *
 * @section CAPI_TELEPHONY_SERVICE_NETWORK_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_PHONEBOOK Phonebook
 * @brief
 *
 * @section CAPI_TELEPHONY_SERVICE_PHONEBOOK_HEADER Required Header
 *   \#include <ITapiPhonebook.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_PHONEBOOK Overview
 * Telephony provides APIs for various phonebook related features such as getting SIM initialization status,
 * supported phonebook types, total records and used records in phonebook, phonebook entry information,
 * and reading, updating, deleting of SIM phonebook records.
 *
 * @section CAPI_TELEPHONY_SERVICE_PHONEBOOK_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_SAT SAT
 * @brief
 *
 * @section CAPI_TELEPHONY_SERVICE_SAT_HEADER Required Header
 *   \#include <ITapiSat.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_SAT_OVERVIEW Overview
 * SAT APIs allow an application to accomplish the following services:
 * - Get main menu information from the SIM application
 * - Send envelope commands to the SIM application
 * - Send the execution results of which the SIM application requests from other applications
 * - Send UI / User confirmation to the SIM application
 * Note: Telephony SAT functionality allows message relaying from a USIM application to SAT related applications.
 *
 * @section CAPI_TELEPHONY_SERVICE_SAT_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_SIM SIM
 * @brief
 *
 * @section CAPI_TELEPHONY_SERVICE_SIM_HEADER Required Header
 *   \#include <ITapiSim.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_SIM_OVERVIEW Overview
 * SIM APIs allow an application to accomplish the following services:
 * - Get Card Init status and Identification
 * - Get the Card Type i.e. whether the SIM Card is a GSM SIM or a 3G SIM
 * - Set and Get the language preference information stored in the SIM
 * - Get the ECC (Emergency Call Codes) data
 * - Get the IMSI (International Mobile Subscriber Identity) information
 * - Set and Get the mailbox information stored in the SIM
 * - Set and Get the message indication status stored in the SIM
 * - Enable, Disable, Change, and Verify the SIM Lock status
 *
 * @section CAPI_TELEPHONY_SERVICE_SIM_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_SMS SMS
 * @brief
 * @section CAPI_TELEPHONY_SERVICE_SMS_HEADER Required Header
 *   \#include <ITapiNetText.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_SMS_OVERVIEW Overview
 * SMS APIs allow an application to accomplish the following services:
 * - Send, save, receive, read, and delete network texts
 * - Receive Cell-Broadcast Message
 * - Set and Get Cell broadcast configuration
 * - Set message status, memory status
 * - Set and Get SMS parameters
 *
 * @section CAPI_TELEPHONY_SERVICE_SMS_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

/**
 * @internal
 * @ingroup CAPI_TELEPHONY_SERVICE
 * @defgroup CAPI_TELEPHONY_SERVICE_SS Supplementary Service
 * @brief
 *
 * @section CAPI_TELEPHONY_SERVICE_SS_HEADER Required Header
 *   \#include <ITapiSs.h>
 *
 * @section CAPI_TELEPHONY_SERVICE_SS_OVERVIEW Overview
 * Supplementary Service APIs allow an application to accomplish the following services:
 * - Activate, Deactivate, and Change the password for call barring
 * - Register, Deregister, Activate, and Deactivate call forwarding and call waiting
 * - Set and Get the CLI(Calling Line Identification) status
 * - Send USSD request
 *
 * Telephony provides APIs to activate/deactivate call barring, get the barring status, and change the barring password.\n
 * This service makes it possible for a mobile subscriber to activate barring for certain categories of outgoing or incoming calls.
 * It also allows the deactivation of call barring based on categories that are set.\n
 * The mobile subscriber may also determine the type of barring preferred for incoming/outgoing calls, by subscribing to a set of one or more unique barring programs.\n
 * Telephony provides APIs to activate/deactivate the call forwarding option at the Network and to get the call forward status.\n
 * Call forwarding allows a user to forward incoming calls to another party.
 * This service permits a called mobile subscriber to have the network send all incoming calls, or just those associated with a specific Basic service group meeting various call FORWARD conditions, to another directory number.\n
 * The ability of the served mobile subscriber to originate calls is unaffected. If the service is activated, a call is forwarded only if the enabled forward condition is satisfied.\n
 * An application has to call this API in order to explicitly register/deregister/activate/deactivate the call forwarding option. A request will be sent irrespective of whether another SS call forward is already in progress.\n
 *
 * @section CAPI_TELEPHONY_SERVICE_SS_FEATURE Related Features
 * This API is related with the following feature:\n
 * - http://tizen.org/feature/network.telephony
 *
 * It is recommended to design feature related codes in your application for reliability.\n
 *
 * You can check if a device supports the related features for this API by using @ref CAPI_SYSTEM_SYSTEM_INFO_MODULE, thereby controlling the procedure of your application.\n
 *
 * To ensure your application is only running on the device with specific features, please define the features in your manifest file using the manifest editor in the SDK.\n
 *
 * More details on featuring your application can be found from <a href="../org.tizen.mobile.native.appprogramming/html/ide_sdk_tools/feature_element.htm"><b>Feature Element</b>.</a>
 */

#endif /* __TIZEN_TELEPHONY_SERVICE_DOC_H__ */
