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
 * @file TelSatObj.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_SAT
 * @{
 */

#ifndef _TEL_SAT_OBJ_H_
#define _TEL_SAT_OBJ_H_

#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define EXTENDED_ASCII 1

/**
 * @brief Definition for the maximum length of the dialing number.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DIALLING_NUMBER_LEN_MAX				200

/**
 * @brief Definition for the maximum length of the alpha identifier.
 * @since_tizen 2.3
 */
#define TAPI_SAT_ALPHA_ID_LEN_MAX						255

/**
 * @brief Definition for the maximum length of the text string.
 * @details When the string data is in the 7bit packed format, this length is not enough to support the maximum size so the value should be increased to a value > 275.
 * @since_tizen 2.3
 */
#define TAPI_SAT_TEXT_STRING_LEN_MAX					500

/**
 * @brief Definition for the maximum length of a sub address.
 * @since_tizen 2.3
 */
#define TAPI_SAT_SUB_ADDR_LEN_MAX						30

/**
 * @brief Definition for the maximum length of CCP data.
 * @since_tizen 2.3
 */
#define TAPI_SAT_CCP_DATA_LEN_MAX						30

/**
 * @brief Definition for the maximum length of the item text.
 * @since_tizen 2.3
 */
#define TAPI_SAT_ITEM_TEXT_LEN_MAX						255

/**
 * @brief Definition for the maximum length of SMS TPDU data.
 * @since_tizen 2.3
 */
#define TAPI_SAT_SMS_TPDU_SMS_DATA_LEN_MAX				175

/**
 * @brief Definition for the maximum length of the SS string.
 * @since_tizen 2.3
 */
#define TAPI_SAT_SS_STRING_LEN_MAX						160

/**
 * @brief Definition for the maximum length of the USSD string.
 * @since_tizen 2.3
 */
#define TAPI_SAT_USSD_STRING_LEN_MAX					255

/**
 * @brief Definition for the maximum count of the file ID list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_FILE_ID_LIST_MAX_COUNT					255

/**
 * @brief Definition for the maximum count of the items next action indication list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_ITEMS_NEXT_ACTION_INDI_LIST_MAX_COUNT	50

/**
 * @brief Definition for the maximum count of the sat event list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_EVENT_LIST_MAX_COUNT					17

/**
 * @brief Definition for the maximum length of image instant raw data.
 * @since_tizen 2.3
 */
#define TAPI_SAT_IMG_INSTANT_RAW_DATA_LEN_MAX			256

/**
 * @brief Definition for the maximum length of clut data.
 * @since_tizen 2.3
 */
#define TAPI_SAT_CLUT_DATA_LEN_MAX						256

/**
 * @brief Definition for the maximum length of the image data file name (icon, clut).
 * @since_tizen 2.3
 */
#define TAPI_SAT_IMG_DATA_FILE_PATH_LEN_MAX				50

/**
 * @brief Definition for the maximum count of the icon list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_ICON_LIST_MAX_COUNT					50

/**
 * @brief Definition for the maximum length of the DTMF string.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DTMF_STRING_LEN_MAX					30

/**
 * @brief Definition for the maximum length of date time and time zone.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DATE_TIME_AND_TIME_ZONE_LEN			7

/**
 * @brief Definition for the maximum length of the URL.
 * @since_tizen 2.3
 */
#define TAPI_SAT_URL_LEN_MAX							129

/**
 * @brief Definition for the maximum count of the bearer list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_BEARER_LIST_MAX_COUNT					50

/**
 * @brief Definition for the maximum length of the provisioning file path.
 * @since_tizen 2.3
 */
#define TAPI_SAT_PROVISIONING_FILE_PATH_LEN_MAX			50

/**
 * @brief Definition for the maximum length of the bearer parameters.
 * @since_tizen 2.3
 */
#define TAPI_SAT_BEARER_PARAMS_LEN_MAX					10

/**
 * @brief Definition for the maximum length of the channel data string.
 * @since_tizen 2.3
 */
#define TAPI_SAT_CHANNEL_DATA_STRING_LEN_MAX			255

/**
 * @brief Definition for the maximum length of the channel status.
 * @since_tizen 2.3
 */
#define TAPI_SAT_CHANNEL_STATUS_LEN						2

/**
 * @brief Definition for the maximum length of the channel ID.
 * @since_tizen 2.3
 */
#define TAPI_SAT_CHANNEL_ID_LEN							3

/**
 * @brief Definition for the maximum length of other addresses.
 * @since_tizen 2.3
 */
#define TAPI_SAT_OTHER_ADDR_LEN_MAX						30

/**
 * @brief Definition for the maximum length of the port number.
 * @since_tizen 2.3
 */
#define TAPI_SAT_PORT_NUMBER_LEN						2

/**
 * @brief Definition for the maximum length of the net account name.
 * @since_tizen 2.3
 */
#define TAPI_SAT_NET_ACC_NAM_LEN_MAX					30

/**
 * @brief Definition for the maximum length of the aid.
 * @since_tizen 2.3
 */
#define TAPI_SAT_AID_LEN_MAX							128

/**
 * @brief Definition for the maximum length of the remote entity address.
 * @since_tizen 2.3
 */
#define TAPI_SAT_REMOTE_ENTITY_ADDR_LEN_MAX				50

/**
 * @brief Definition for the maximum count of the item text attributes list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_ITEM_TEXT_ATTRIBUTES_LIST_MAX_COUNT	50

/**
 * @brief Definition for the maximum length of MCC.
 * @since_tizen 2.3
 */
#define TAPI_SAT_MCC_CODE_LEN							3

/**
 * @brief Definition for the maximum length of MNC.
 * @since_tizen 2.3
 */
#define TAPI_SAT_MNC_CODE_LEN							3

/**
 * @brief Definition for the maximum length of LAC.
 * @since_tizen 2.3
 */
#define TAPI_SAT_LAC_CODE_LEN							2

/**
 * @brief Definition for the maximum length of the cell ID.
 * @since_tizen 2.3
 */
#define TAPI_SAT_CELL_ID_LEN							2

/**
 * @brief Enumeration for alphabet format.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_ALPHABET_FORMAT_SMS_DEFAULT = 0x00, /**< ALPHABET FORMAT SMS DEFAULT */
	TAPI_SAT_ALPHABET_FORMAT_8BIT_DATA = 0x01, /**< ALPHABET FORMAT 8BIT DATA */
	TAPI_SAT_ALPHABET_FORMAT_UCS2 = 0x02, /**< ALPHABET FORMAT UCS2 */
	TAPI_SAT_ALPHABET_FORMAT_RESERVED = 0x03 /**< ALPHABET FORMAT RESERVED */
} TelSatAlphabetFormatType_t;

/**
 * @brief Enumeration for the message class.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_MSG_CLASS_NONE = 0x00, /**< none */
	TAPI_SAT_MSG_CLASS_0 = 0x01, /**< class 0 */
	TAPI_SAT_MSG_CLASS_1, /**< class 1 Default meaning:ME-specific */
	TAPI_SAT_MSG_CLASS_2, /**< class 2 SIM specific message */
	TAPI_SAT_MSG_CLASS_3, /**< class 3 Default meaning: TE specific */
	TAPI_SAT_MSG_CLASS_RESERVED = 0xFF /**<	class reserved */
} TelSatMsgClassType_t;

/**
 * @brief Enumeration for the type of command and the next action indicator.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_CMD_TYPE_NONE = 0x00, /**< command type - None */

	TAPI_SAT_CMD_TYPE_REFRESH = 0x01, /**< command type - refresh */
	TAPI_SAT_CMD_TYPE_MORE_TIME = 0x02, /**< command type - more time */
	TAPI_SAT_CMD_TYPE_SETUP_EVENT_LIST = 0x05, /**< command type - setup event list */
	TAPI_SAT_CMD_TYPE_SETUP_CALL = 0x10, /**< command type - setup call */
	TAPI_SAT_CMD_TYPE_SEND_SS = 0x11, /**< command type - send SS */
	TAPI_SAT_CMD_TYPE_SEND_USSD = 0x12, /**< command type - send USSD */
	TAPI_SAT_CMD_TYPE_SEND_SMS = 0x13, /**< command type - send SMS */
	TAPI_SAT_CMD_TYPE_SEND_DTMF = 0x14, /**< command type - send DTMF */
	TAPI_SAT_CMD_TYPE_LAUNCH_BROWSER = 0x15, /**< command type - launch browser */
	TAPI_SAT_CMD_TYPE_PLAY_TONE = 0x20, /**< command type - play tone */
	TAPI_SAT_CMD_TYPE_DISPLAY_TEXT = 0x21, /**< command type - display text */
	TAPI_SAT_CMD_TYPE_GET_INKEY = 0x22, /**< command type - get inkey */
	TAPI_SAT_CMD_TYPE_GET_INPUT = 0x23, /**< command type - get input */
	TAPI_SAT_CMD_TYPE_SELECT_ITEM = 0x24, /**< command type - select item */
	TAPI_SAT_CMD_TYPE_SETUP_MENU = 0x25, /**< command type - setup menu */
	TAPI_SAT_CMD_TYPE_PROVIDE_LOCAL_INFO = 0x26, /**< command type - provide local info */
	TAPI_SAT_CMD_TYPE_SETUP_IDLE_MODE_TEXT = 0x28, /**<	command type - setup idle mode text */
	TAPI_SAT_CMD_TYPE_LANGUAGE_NOTIFICATION = 0x35, /**< command type - language notification */
	TAPI_SAT_CMD_TYPE_OPEN_CHANNEL = 0x40, /**< command type - open channel - class e */
	TAPI_SAT_CMD_TYPE_CLOSE_CHANNEL = 0x41, /**< command type - close channel - class e */
	TAPI_SAT_CMD_TYPE_RECEIVE_DATA = 0x42, /**< command type - receive data -class e */
	TAPI_SAT_CMD_TYPE_SEND_DATA = 0x43, /**< command type - send data */
	TAPI_SAT_CMD_TYPE_GET_CHANNEL_STATUS = 0x44, /**< command type - get channel status - class e */
	TAPI_SAT_CMD_TYPE_END_OF_APP_EXEC = 0xFD, /**< inform to End the execution of a Proactive Command */
	TAPI_SAT_CMD_TYPE_END_PROACTIVE_SESSION = 0xFE, /**< inform end proactive session */
	TAPI_SAT_CMD_TYPE_RESERVED = 0xFF /**< command type - reserved */
} TelSatCommandType_t;

/**
 * @brief Enumeration for the command qualifier values of the refresh command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_REFRESH_SIM_INIT_AND_FULL_FCN = 0x00, /**< command qualifier for REFRESH SIM INIT AND FULL FILE CHANGE_NOTIFICATION */
	TAPI_SAT_REFRESH_FCN = 0x01, /**< command qualifier for REFRESH FILE CHANGE NOTIFICATION */
	TAPI_SAT_REFRESH_SIM_INIT_AND_FCN = 0x02, /**< command qualifier for REFRESH SIM INIT AND FILE CHANGE NOTIFICATION */
	TAPI_SAT_REFRESH_SIM_INIT = 0x03, /**< command qualifier for REFRESH SIM INIT */
	TAPI_SAT_REFRESH_SIM_RESET = 0x04, /**< command qualifier for REFRESH SIM RESET */
	TAPI_SAT_REFRESH_3G_APPLICATION_RESET = 0x05, /**< command qualifier for REFRESH 3G APPLICATION RESET */
	TAPI_SAT_REFRESH_3G_SESSION_RESET = 0x06, /**< command qualifier for REFRESH 3G SESSION RESET */
	TAPI_SAT_REFRESH_RESERVED = 0xFF /**< command qualifier for REFRESH RESERVED */
} TelSatCmdQualiRefresh_t;

/**
 * @brief Enumeration for the command qualifier values of the setup call command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_SETUP_CALL_IF_ANOTHER_CALL_NOT_BUSY = 0x00, /**< command qualifier for SETUP CALL IF ANOTHER CALL IS NOT BUSY */
	TAPI_SAT_SETUP_CALL_IF_ANOTHER_CALL_NOT_BUSY_WITH_REDIAL = 0x01, /**< command qualifier for SETUP CALL IF ANOTHER CALL IS NOT BUSY WITH REDIAL */
	TAPI_SAT_SETUP_CALL_PUT_ALL_OTHER_CALLS_ON_HOLD = 0x02, /**< command qualifier for SETUP CALL PUTTING ALL OTHER CALLS ON HOLD */
	TAPI_SAT_SETUP_CALL_PUT_ALL_OTHER_CALLS_ON_HOLD_WITH_REDIAL = 0x03, /**< command qualifier for SETUP CALL PUTTING ALL OTHER CALLS ON HOLD WITH REDIAL */
	TAPI_SAT_SETUP_CALL_DISCONN_ALL_OTHER_CALLS = 0x04, /**< command qualifier for SETUP CALL DISCONNECTING ALL OTHER CALLS */
	TAPI_SAT_SETUP_CALL_DISCONN_ALL_OTHER_CALLS_WITH_REDIAL = 0x05, /**< command qualifier for SETUP CALL DISCONNECTING ALL OTHER CALLS WITH REDIAL */
	TAPI_SAT_SETUP_CALL_RESERVED = 0xFF /**< command qualifier for SETUP CALL RESERVED */
} TelSatCmdQualiSetupCall_t;

/**
 * @brief Enumeration for the message priority.
 *
 * @details This is associated with the command qualifier of display text.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_MSG_PRIORITY_NORMAL = 0, /**< MSG PRIORITY NORMAL */
	TAPI_SAT_MSG_PRIORITY_HIGH = 1 /**< MSG PRIORITY HIGH */
} TelSatDisplayTextPriorityType_t;

/**
 * @brief Enumeration for the message clear type.
 *
 * @details This is associated with the command qualifier of display text.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_AUTO_CLEAR_MSG_AFTER_A_DELAY = 0, /**< message clear type - AUTO CLEAR MSG AFTER A DELAY */
	TAPI_SAT_WAIT_FOR_USER_TO_CLEAR_MSG = 1 /**< message clear type - WAIT FOR USER TO CLEAR MSG */
} TelSatDisplayTextMsgClearType_t;

/**
 * @brief Enumeration for the inkey type.
 *
 * @details This is associated with the command qualifier of get inkey.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_INKEY_TYPE_CHARACTER_SET_ENABLED = 0, /**< command qualifier for INKEY TYPE CHARACTER SET ENABLED */
	TAPI_SAT_INKEY_TYPE_YES_NO_REQUESTED = 1 /**< command qualifier for INKEY TYPE YES NO REQUESTED */
} TelSatInkeyType_t;

/**
 * @brief Enumeration for the user input type.
 *
 * @details This is associated with the command qualifier of get input.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_USER_INPUT_ALPHABET_TYPE_SMS_DEFAULT = 1, /**< command qualifier for ALPHABET TYPE SMS DEFAULT */
	TAPI_SAT_USER_INPUT_ALPHABET_TYPE_UCS2 = 2 /**< command qualifier for ALPHABET TYPE UCS2 */
} TelSatUseInputAlphabetType_t;

/**
 * @brief Enumeration for the vibrate alert type.
 *
 * @details This is associated with the command qualifier of the play tone command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_VIBRATE_ALERT_OPTIONAL = 0, /**< VIBRATE ALERT UPTO THE TERMINAL */
	TAPI_SAT_VIBRATE_ALERT_REQUIRED = 1 /**< VIBRATE, IF AVAILABLE, WITH TONE */
} TelSatDisplayVibrateAlertType_t;

/**
 * @brief Enumeration for the presentation type.
 *
 * @details This is associated with the command qualifier of the select item command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_PRESENTATION_TYPE_NOT_SPECIFIED = 0x00, /**< command qualifier for PRESENTATION TYPE NOT SPECIFIED */
	TAPI_SAT_PRESENTATION_TYPE_DATA_VALUE = 0x01, /**< command qualifier for PRESENTATION TYPE DATA VALUE */
	TAPI_SAT_PRESENTATION_TYPE_NAVIGATION_OPTION = 0x02 /**< command qualifier for PRESENTATION TYPE NAVIGATION OPTION */
} TelSatPresentationType_t;

/**
 * @brief Enumeration for the selection preference.
 *
 * @details This is associated with the command qualifier of the select item command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_SELECTION_PREFERENCE_NONE_REQUESTED = 0, /**< command qualifier for SELECTION PREFERENCE NONE REQUESTED */
	TAPI_SAT_SELECTION_PREFERENCE_USING_SOFT_KEY = 1 /**< command qualifier for SELECTION PREFERENCE USING SOFT KEY */
} TelSatSelectionPreferenceType_t;

/**
 * @brief Enumeration for the local info type.
 *
 * @details This enum defines the command qualifier values of the local info command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_PROVIDE_DATE_TIME_AND_TIME_ZONE = 0x03, /**< command qualifier for PROVIDE DATE TIME AND TIME ZONE */
	TAPI_SAT_PROVIDE_LANGUAGE_SETTING = 0x04, /**< command qualifier for PROVIDE LANGUAGE SETTING */
	TAPI_SAT_PROVIDE_IMEISV = 0x08, TAPI_SAT_PROVIDE_RESERVED = 0xFF /**< reserved */
} TelSatCmdQualiProvideLocalInfo_t;

/**
 * @brief Enumeration for the browser launching mode.
 *
 * @details This enum defines the command qualifier values of the launch browser command.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_LAUNCH_BROWSER = 0, /**< command qualifier for LAUNCH BROWSER */
	TAPI_SAT_NOT_USED = 1, /**< command qualifier for NOT USED */
	TAPI_SAT_USE_EXISTING_BROWSER = 2, /**< command qualifier for USE EXISTING BROWSER. If secure session, do not use it */
	TAPI_SAT_CLOSE_AND_LAUNCH_NEW_BROWSER = 3, /**< command qualifier for CLOSE AND LAUNCH NEW BROWSER */
	TAPI_SAT_NOT_USED2 = 4, /**< command qualifier for NOT USED2 */
	TAPI_SAT_LB_RESERVED = 0xFF /**< reserved */
} TelSatCmdQualiLaunchBrowser_t;

/**
 * @brief Enumeration for the device ID type.
 *
 * @details This enum lists the device identity tag value IDs.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_DEVICE_ID_KEYPAD = 0x01, /**< DEVICE ID KEYPAD */
	TAPI_SAT_DEVICE_ID_DISPLAY = 0x02, /**< DEVICE ID DISPLAY */
	TAPI_SAT_DEVICE_ID_EARPIECE = 0x03, /**< DEVICE ID EARPIECE */

	TAPI_SAT_DEVICE_ID_SIM = 0x81, /**< DEVICE ID SIM */
	TAPI_SAT_DEVICE_ID_ME = 0x82, /**< DEVICE ID ME */
	TAPI_SAT_DEVICE_ID_NETWORK = 0x83, /**< DEVICE ID NETWORK */

	TAPI_SAT_DEVICE_ID_RESERVED = 0XFF /**< reserved */
} TelSatDeviceIdentitiesTagType_t;

/**
 * @brief Enumeration for the time unit type.
 *
 * @details This enum lists the time units for the duration data object.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_TIME_UNIT_MINUTES = 0x0, /**< time unit - minutes */
	TAPI_SAT_TIME_UNIT_SECONDS = 0x01, /**< time unit - second */
	TAPI_SAT_TIME_UNIT_TENTHS_OF_SECONDS = 0x02, /**< time unit - tenths of seconds */
	TAPI_SAT_TIME_UNIT_RESERVED = 0xFF /**< reserved */
} TelSatTimeUnitType_t;

/**
 * @brief Enumeration for the command execution result type.
 *
 * @details This enum lists the values for the RESULT data object - General Response.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_R_SUCCESS = 0x0, /**< command performed successfully */
	TAPI_SAT_R_SUCCESS_WITH_PARTIAL_COMPREHENSION = 0x01, /**< command performed with partial comprehension */
	TAPI_SAT_R_SUCCESS_WITH_MISSING_INFO = 0x02, /**< command performed, with missing information */

	TAPI_SAT_R_REFRESH_PERFORMED_WITH_ADDITIONAL_EFS_READ = 0x03, /**< REFRESH PERFORMED WITH ADDITIONAL EFS READ */
	TAPI_SAT_R_SUCCESS_BUT_REQUESTED_ICON_NOT_DISPLAYED = 0x04, /**< command performed but REQUESTED ICON NOT DISPLAYED */
	TAPI_SAT_R_SUCCESS_BUT_MODIFIED_BY_CALL_CONTROL_BY_SIM = 0x05, /**< command performed but MODIFIED BY CALL CONTROL BY SIM */
	TAPI_SAT_R_SUCCESS_LIMITED_SERVICE = 0x06, /**< command performed with LIMITED SERVICE */
	TAPI_SAT_R_SUCCESS_WITH_MODIFICATION = 0x07, /**< command performed with MODIFICATION */
	TAPI_SAT_R_REFRESH_PRFRMD_BUT_INDICATED_USIM_NOT_ACTIVE = 0x08, /**< REFRESH PERFORMED BUT INDICATED USIM NOT ACTIVE */

	TAPI_SAT_R_PROACTIVE_SESSION_TERMINATED_BY_USER = 0x10, /**< proactive SIM application session terminated by the user */
	TAPI_SAT_R_BACKWARD_MOVE_BY_USER = 0x11, /**< backward move in the proactive SIM application session request by the user */
	TAPI_SAT_R_NO_RESPONSE_FROM_USER = 0x12, /**< no response from the user */

	TAPI_SAT_R_HELP_INFO_REQUIRED_BY_USER = 0x13, /**< HELP INFO REQUIRED BY USER */
	TAPI_SAT_R_USSD_OR_SS_TRANSACTION_TERMINATED_BY_USER = 0x14, /**< USSD OR SS TRANSACTION TERMINATED BY USER	*/

	TAPI_SAT_R_ME_UNABLE_TO_PROCESS_COMMAND = 0x20, /**< ME currently unable to process the command */
	TAPI_SAT_R_NETWORK_UNABLE_TO_PROCESS_COMMAND = 0x21, /**< Network currently unable to process the command */
	TAPI_SAT_R_USER_DID_NOT_ACCEPT_CALL_SETUP_REQ = 0x22, /**< User did not accept the call setup request */
	TAPI_SAT_R_USER_CLEAR_DOWN_CALL_BEFORE_CONN = 0x23, /**< User cleared down the call before connection or released the network */

	TAPI_SAT_R_INTERACTION_WITH_CC_BY_SIM_IN_TMP_PRBLM = 0x25, /**<	INTERACTION WITH CALL CONTROL BY SIM IN TEMPORARY PROBLEM */
	TAPI_SAT_R_LAUNCH_BROWSER_GENERIC_ERROR_CODE = 0x26, /**< LAUNCH BROWSER GENERIC ERROR CODE */

	TAPI_SAT_R_BEYOND_ME_CAPABILITIES = 0x30, /**< command beyond ME's capabilities */
	TAPI_SAT_R_COMMAND_TYPE_NOT_UNDERSTOOD_BY_ME = 0x31, /**< command type not understood by ME */
	TAPI_SAT_R_COMMAND_DATA_NOT_UNDERSTOOD_BY_ME = 0x32, /**< command data not understood by ME */
	TAPI_SAT_R_COMMAND_NUMBER_NOT_KNOWN_BY_ME = 0x33, /**< command number not known by ME */
	TAPI_SAT_R_SS_RETURN_ERROR = 0x34, /**<	SS return error */
	TAPI_SAT_R_SMS_RP_ERROR = 0x35, /**< SMS rp-error */
	TAPI_SAT_R_ERROR_REQUIRED_VALUES_ARE_MISSING = 0x36, /**< Error, required values are missing */

	TAPI_SAT_R_USSD_RETURN_ERROR = 0x37, /**< USSD_RETURN_ERROR */
	TAPI_SAT_R_INTRCTN_WITH_CC_OR_SMS_CTRL_PRMNT_PRBLM = 0x39, /**< INTERACTION WITH CALL CONTROL OR SMS CONTROL PERMANENT PROBLEM */
	TAPI_SAT_R_BEARER_INDEPENDENT_PROTOCOL_ERROR = 0x3A, /**< BEARER INDEPENDENT PROTOCOL ERROR */
	TAPI_SAT_R_FRAMES_ERROR = 0x3C /**< FRAMES ERROR */
} TelSatResultType_t;

/**
 * @brief Enumeration for the ME problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object and specifies a particular ME PROBLEM.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_ME_PROBLEM_NO_SPECIFIC_CAUSE = 0x0, /**< ME problem with NO SPECIFIC CAUSE */
	TAPI_SAT_ME_PROBLEM_SCREEN_BUSY = 0x01, /**< ME problem with SCREEN BUSY */
	TAPI_SAT_ME_PROBLEM_ME_BUSY_ON_CALL = 0x02, /**< ME problem with ME BUSY ON CALL */
	TAPI_SAT_ME_PROBLEM_ME_BUSY_ON_SS = 0x03, /**< ME problem with ME_BUSY ON SS */
	TAPI_SAT_ME_PROBLEM_NO_SERVICE = 0x04, /**< ME problem with NO SERVICE */
	TAPI_SAT_ME_PROBLEM_ACCESS_CONTROL_CLASS_BAR = 0x05, /**< ME problem with ACCESS CONTROL CLASS BAR */
	TAPI_SAT_ME_PROBLEM_RADIO_RES_NOT_GRANTED = 0x06, /**< ME problem with RADIO RES NOT GRANTED */
	TAPI_SAT_ME_PROBLEM_NOT_IN_SPEECH_CALL = 0x07, /**< ME problem with NOT IN SPEECH CALL */
	TAPI_SAT_ME_PROBLEM_ME_BUSY_ON_USSD = 0x08, /**< ME problem with ME BUSY ON USSD */
	TAPI_SAT_ME_PROBLEM_ME_BUSY_ON_SEND_DTMF_CMD = 0x09, /**< ME problem with ME BUSY ON SEND DTMF CMD */
	TAPI_SAT_ME_PROBLEM_NO_USIM_ACTIVE = 0x0A, /**< ME problem with NO USIM ACTIVE */
	TAPI_SAT_ME_PROBLEM_INVALID = 0xFF /**< ME problem with INVALID */
} TelSatMeProblemType_t;

/**
 * @brief Enumeration for the network problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object and specifies a particular network PROBLEM.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_NETWORK_PROBLEM_NO_SPECIFIC_CAUSE = 0x0, /**< Network problem with no specific cause */
	TAPI_SAT_NETWORK_PROBLEM_USER_BUSY = 0x91 /**< Network problem with USER BUSY */
} TelSatNetworkProblemType_t;

/**
 * @brief Enumeration for the SS problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object related to SEND SS.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_SS_PROBLEM_NO_SPECIFIC_CAUSE = 0, /**< SS problem with no specific cause */
	TAPI_SAT_SS_PROBLEM_FACILITY_NOT_SUPPORTED = 0x15 /**< SS problem with FACILITY NOT SUPPORTED */
} TelSatSsProblemType_t;

/**
 * @brief Enumeration for the SMS problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object related to SEND SMS.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_SMS_PROBLEM_NO_SPECIFIC_CAUSE = 0 /**< SMS problem with no specific cause */
} TelSatSmsProblemType_t;

/**
 * @brief Enumeration for the USSD problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object related to SEND USSD STRING.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_USSD_PROBLEM_NO_SPECIFIC_CAUSE = 0, /**< USSD problem with no specific cause */
	TAPI_SAT_USSD_PROBLEM_UNKNOWN_ALPHABET = 0x47 /**< USSD problem with UNKNOWN ALPHABET */
} TelSatUssdProblemType_t;

/**
 * @brief Enumeration for the CC problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object related to CALL CONTROL or MO SMS CONTROL.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_CC_PROBLEM_NO_SPECIFIC_CAUSE = 0, /**< Call Control problem with no specific cause */
	TAPI_SAT_CC_PROBLEM_ACTION_NOT_ALLOWED = 1, /**< Call Control problem with action not allowed */
	TAPI_SAT_CC_PROBLEM_REQUEST_TYPE_HAS_CHANGED = 2 /**< Call Control problem in which request type has changed */
} TelSatCallCtrlProblemType_t;

/**
 * @brief Enumeration for the browser problem type.
 *
 * @details This enum lists the values for the additional response of the RESULT object related to LAUNCH BROWSER PROBLEM.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BROWSER_PROBLEM_NO_SPECIFIC_CAUSE = 0, /**< Browser problem with no specific cause */
	TAPI_SAT_BROWSER_PROBLEM_BEARER_UNAVAILABLE = 1, /**< Browser problem with bearer unavailable */
	TAPI_SAT_BROWSER_PROBLEM_BROWSER_UNAVAILABLE = 2, /**< Browser problem with browser unavailable */
	TAPI_SAT_BROWSER_PRBLM_ME_UNABLE_TO_READ_PROV_DATA = 3 /**< Browser problem with ME unable to read provisioning data */
} TelSatLaunchBrowserProblemType_t;

/**
 * @brief Enumeration for the #TelSatBipProblemType_t enum.
 *
 * @details This enum lists the values for the additional response of the RESULT object.
 * Permanent Problems  ::: 12.12.11 ADDITIONAL INFORMATION :  BEARER INDEPENDENT PROTOCOL.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_PROBLEM_NO_SPECIFIC_CAUSE = 0x00, /**< BIP problem with no specific cause */
	TAPI_SAT_BIP_PROBLEM_NO_CHANNEL_AVAILABLE = 0x01, /**< BIP problem with no channel available */
	TAPI_SAT_BIP_PROBLEM_CHANNEL_CLOSED = 0x02, /**< BIP problem with channel closed */
	TAPI_SAT_BIP_PROBLEM_CHANNEL_ID_NOT_VALID = 0x03, /**< BIP problem with channel ID not valid */
	TAPI_SAT_BIP_PROBLEM_BUF_SIZE_NOT_AVAILABLE = 0x04, /**< BIP problem with buffer size not available */
	TAPI_SAT_BIP_PROBLEM_SECURITY_ERROR = 0x05, /**< BIP problem with security error */
	TAPI_SAT_BIP_PRBLM_SIM_ME_IF_TRNSPRT_LEVEL_NOT_AVL = 0x06, /**< BIP problem with SIM ME interface transport level not available */
	TAPI_SAT_BIP_REMOTE_DEV_NOT_REACHABLE = 0x07, /**< BIP problem with remote device not reachable */
	TAPI_SAT_BIP_SERVICE_ERROR = 0x08, /**< BIP service error */
	TAPI_SAT_BIP_SERVICE_IDENTIFIER_UNKNOWN = 0x09 /**< BIP service identifier unknown */
} TelSatBipProblemType_t;

/**
 * @brief Enumeration for the SMS TPDU type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_SMS_TPDU_TYPE_DELIVER_TPDU = 0, /**< SMS TPDU type - DELIVER TPDU */
	TAPI_SAT_SMS_TPDU_TYPE_DELIVER_RPT = 1, /**< SMS TPDU type - DELIVER RPT */
	TAPI_SAT_SMS_TPDU_TYPE_SUBMIT_TPDU = 2, /**< SMS TPDU type - SUBMIT TPDU */
	TAPI_SAT_SMS_TPDU_TYPE_SUBMIT_RPT = 3, /**< SMS TPDU type - SUBMIT RPT */
	TAPI_SAT_SMS_TPDU_TYPE_STATUS_RPT = 4, /**< SMS TPDU type - STATUS RPT */
	TAPI_SAT_SMS_TPDU_TYPE_TPDU_CMD = 5 /**< SMS TPDU type - TPDU CMD */
} TelSatSmsTpduType_t;

/**
 * @brief Enumeration for the tone type.
 * @since_tizen 2.3
 */
typedef enum {
	// standard supervisory tones
	TAPI_SAT_DIAL_TONE = 0x01, /**< TONE TYPE DIAL TONE */
	TAPI_SAT_CALLED_SUBSCRIBER_BUSY = 0x02, /**< TONE TYPE CALLED SUBSCRIBER BUSY */
	TAPI_SAT_CONGESTION = 0x03, /**< TONE TYPE CONGESTION */
	TAPI_SAT_RADIO_PATH_ACK = 0x04, /**< TONE TYPE RADIO PATH ACK */
	TAPI_SAT_RADIO_PATH_NOT_AVAILABLE_CALL_DROPPED = 0x05, /**< TONE TYPE RADIO PATH NOT AVAILABLE CALL DROPPED */
	TAPI_SAT_ERR_SPECIAL_INFO = 0x06, /**< TONE TYPE ERR SPECIAL INFO */
	TAPI_SAT_CALL_WAITING_TONE = 0x07, /**< TONE TYPE CALL WAITING TONE	 */
	TAPI_SAT_RINGING_TONE = 0x08, /**< TONE TYPE RINGING TONE */

	// ME proprietary tones
	TAPI_SAT_GENERAL_BEEP = 0x10, /**< TONE TYPE GENERAL BEEP */
	TAPI_SAT_POSITIVE_ACK_TONE = 0x11, /**< TONE TYPE POSITIVE ACK TONE */
	TAPI_SAT_NEGATIVE_ACK_OR_ERROR_TONE = 0x12, /**< TONE TYPE NEGATIVE ACK OR ERROR TONE */
	TAPI_SAT_RINGING_TONE_SLCTD_BY_USR_FOR_INCOM_SPEECH_CALL = 0x13, /**< TONE TYPE RINGING TONE SELECTED BY USER FOR INCOMING SPEECH CALL */
	TAPI_SAT_ALERT_TONE_SELECTED_BY_USER_FOR_INCOMING_SMS = 0x14, /**< TONE TYPE ALERT TONE SELECTED BY USER FOR INCOMING SMS */
	TAPI_SAT_CRITICAL_ALERT = 0x15, /**< TONE TYPE CRITICAL ALERT */

	//Themed tones
	TAPI_SAT_HAPPY_TONE = 0x30, /**< TONE TYPE HAPPY TONE */
	TAPI_SAT_SAD_TONE = 0x31, /**< TONE TYPE SAD TONE */
	TAPI_SAT_URGENT_ACTION_TONE = 0x32, /**< TONE TYPE URGENT ACTION TONE */
	TAPI_SAT_QUESTION_TONE = 0x33, /**< TONE TYPE QUESTION TONE */
	TAPI_SAT_MESSAGE_RECEIVED_TONE = 0x34, /**< TONE TYPE MESSAGE RECEIVED TONE */

	//Melody tones
	TAPI_SAT_MELODY_1 = 0x40, /**< TONE TYPE MELODY 1 */
	TAPI_SAT_MELODY_2 = 0x41, /**< TONE TYPE MELODY 2 */
	TAPI_SAT_MELODY_3 = 0x42, /**< TONE TYPE MELODY 3 */
	TAPI_SAT_MELODY_4 = 0x43, /**< TONE TYPE MELODY 4 */
	TAPI_SAT_MELODY_5 = 0x44, /**< TONE TYPE MELODY 5 */
	TAPI_SAT_MELODY_6 = 0x45, /**< TONE TYPE MELODY 6 */
	TAPI_SAT_MELODY_7 = 0x46, /**< TONE TYPE MELODY 7 */
	TAPI_SAT_MELODY_8 = 0x47, /**< TONE TYPE MELODY 8 */

	TAPI_SAT_TONE_TYPE_RESERVED = 0xFF /**< TONE TYPE RESERVED */
} TelSatToneType_t;

/**
 * @brief Enumeration for the event types required by ME to monitor and report to the SIM.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_EVENT_SAT_DW_TYPE_MT_CALL = 0,	/**< data download type - MT CALL */
	TAPI_EVENT_SAT_DW_TYPE_CALL_CONNECTED = 1,	/**< data download type - CALL CONNECTED */
	TAPI_EVENT_SAT_DW_TYPE_CALL_DISCONNECTED = 2,	/**< data download type - CALL DISCONNECTED */
	TAPI_EVENT_SAT_DW_TYPE_LOCATION_STATUS = 3,	/**< data download type - LOCATION STATUS */
	TAPI_EVENT_SAT_DW_TYPE_USER_ACTIVITY = 4, /**< data download type - USER_ACTIVITY */
	TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE = 5, /**< data download type - IDLE SCREEN AVAILABLE */
	TAPI_EVENT_SAT_DW_TYPE_CARD_READER_STATUS = 6,	/**< data download type - CARD READER STATUS */
	TAPI_EVENT_SAT_DW_TYPE_LANGUAGE_SELECTION = 7, /**<	data download type - LANGUAGE SELECTION */
	TAPI_EVENT_SAT_DW_TYPE_BROWSER_TERMINATION = 8, /**< data download type - BROWSER TERMINATION */
	TAPI_EVENT_SAT_DW_TYPE_DATA_AVAILABLE = 9, /**<	data download type - DATA AVAILABLE */
	TAPI_EVENT_SAT_DW_TYPE_CHANNEL_STATUS = 0x0A, /**< data download type - CHANNEL STATUS */
	TAPI_EVENT_SAT_DW_TYPE_ACCESS_TECHNOLOGY_CHANGED = 0x0B, /**< data download type - ACCESS TECHNOLOGY CHANGED */
	TAPI_EVENT_SAT_DW_TYPE_DISPLAY_PARAMETERS_CHANGED = 0x0C, /**< data download type - DISPLAY PARAMETERS CHANGED */
	TAPI_EVENT_SAT_DW_TYPE_LOCAL_CONNECTION = 0x0D,	/**< data download type - LOCAL CONNECTION */
	TAPI_EVENT_SAT_DW_TYPE_NW_SEARCH_MODE_CHANGED = 0X0E,	/**< data download type - NW SEARCH MODE CHANGED */
	TAPI_EVENT_SAT_DW_TYPE_BROWSING_STATUS = 0X0F,	/**< data download type - BROWSING STATUS */
	TAPI_EVENT_SAT_DW_TYPE_FRAMES_INFORMATION_CHANGED = 0X10,	/**< data download type - FRAMES INFORMATION CHANGED */
	TAPI_EVENT_SAT_DW_TYPE_RESERVED_FOR_3GPP = 0X11,	/**< data download type - RESERVED FOR 3GPP */
	TAPI_EVENT_SAT_DW_TYPE_UNKNOWN = 0xFF /**< data download type - unknown */
} TelSatEventDownloadType_t;

/**
 * @brief Enumeration for the SIM image coding scheme type.
 *
 * @details This enum lists image coding scheme types required by ME to show.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_SIM_IMAGE_CODING_SCHEME_BASIC = 0x11, /**<	IMAGE CODING SCHEME BASIC */
	TAPI_SAT_SIM_IMAGE_CODING_SCHEME_COLOUR = 0x21, /**< IMAGE CODING SCHEME COLOUR */
	TAPI_SAT_SIM_IMAGE_CODING_SCHEME_RESERVED = 0xFF /**< RESERVED */
} TelSatImageCodingSchemeType_t;

/**
 * @brief Enumeration for the icon qualifier.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_ICON_QUALI_SELF_EXPLANATORY = 0, /**< ICON QUALI SELF EXPLANATORY */
	TAPI_SAT_ICON_QUALI_NOT_SELF_EXPLANATORY = 1, /**< ICON QUALI NOT SELF EXPLANATORY */
	TAPI_SAT_ICON_QUALI_RESERVED = 0xFF /**< RESERVED */
} TelSatIconQualifierType_t;

/**
 * @brief Enumeration for the SIM ATK BC repeat indicator type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BC_REPEAT_INDI_ALTERNATE_MODE = 0x01, /**< BC REPEAT ALTERNATE MODE */
	TAPI_SAT_BC_REPEAT_INDI_SEQUENTIAL_MODE = 0x03, /**< BC REPEAT SEQUENTIAL MODE */
	TAPI_SAT_BC_REPEAT_INDI_RESERVED = 0xFF /**< RESERVED */
} TelSatBcRepeatIndicatorType_t;

/**
 * @brief Enumeration for the call control string type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_CC_VOICE = 0x00, /**< Call Control String Type - voice */
	TAPI_SAT_CC_SS = 0x01, /**< Call Control String Type - SS */
	TAPI_SAT_CC_USSD = 0x02, /**< Call Control String Type - USSD */
	TAPI_SAT_CC_NONE = 0xFF /**< Call Control String Type - none */
} TelSatCallCtrlStringType_t;

/**
 * @brief Enumeration for the supported language values.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_LP_GERMAN = 0x00, /**< GERMAN */
	TAPI_SAT_LP_ENGLISH = 0x01, /**< ENGLISH */
	TAPI_SAT_LP_ITALIAN = 0x02, /**< ITALIAN */
	TAPI_SAT_LP_FRENCH = 0x03, /**< FRENCH */
	TAPI_SAT_LP_SPANISH = 0x04, /**< SPANISH */
	TAPI_SAT_LP_DUTCH = 0x05, /**< DUTCH */
	TAPI_SAT_LP_SWEDISH = 0x06, /**< SWEDISH */
	TAPI_SAT_LP_DANISH = 0x07, /**< DANISH */
	TAPI_SAT_LP_PORTUGUESE = 0x08, /**< PORTUGUESE */
	TAPI_SAT_LP_FINNISH = 0x09, /**< FINNISH */
	TAPI_SAT_LP_NORWEGIAN = 0x0A, /**< NORWEGIAN */
	TAPI_SAT_LP_GREEK = 0x0B, /**< GREEK */
	TAPI_SAT_LP_TURKISH = 0x0C, /**< TURKISH */
	TAPI_SAT_LP_HUNGARIAN = 0x0D, /**< HUNGARIAN */
	TAPI_SAT_LP_POLISH = 0x0E, /**< POLISH */
	TAPI_SAT_LP_LANG_UNSPECIFIED = 0x0F /**< LANGUAGE UNSPECIFIED */
} TelSatLanguageType_t;

/**
 * @brief Enumeration for the SAT browser identity type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BROWSER_ID_DEFAULT = 0x0, /**<	DEFAULT BROWSER */
	TAPI_SAT_BROWSER_ID_WML, /**< BROWSER WML */
	TAPI_SAT_BROWSER_ID_HTML, /**< BROWSER HTML */
	TAPI_SAT_BROWSER_ID_XHTML, /**< BROWSER XHTML */
	TAPI_SAT_BROWSER_ID_CHTML, /**< BROWSER CHTML */
	TAPI_SAT_BROWSER_ID_RESERVED = 0xFF /**< RESERVED */
} TelSatBrowserIdentityType_t;

/**
 * @brief Enumeration for the SAT bearer type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BEARER_TYPE_SMS = 0x0, /**< BEARER SMS */
	TAPI_SAT_BEARER_TYPE_CSD = 0x1, /**< BEARER CSD */
	TAPI_SAT_BEARER_TYPE_USSD = 0x2, /**< BEARER USSD */
	TAPI_SAT_BEARER_TYPE_GPRS = 0x3, /**< BEARER GPRS */
	TAPI_SAT_BEARER_TYPE_RESERVED = 0xFF /**< BEARER RESERVED */
} TelSatBearerType_t;

/**
 * @brief Enumeration for the SAT browser termination cause type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BROWSER_TERMINATED_BY_USER = 0, /**< BROWSER TERMINATED BY USER */
	TAPI_SAT_BROWSER_TERMINATED_BY_ERROR = 1, /**< BROWSER TERMINATED BY ERROR */
} TelSatBrowserTerminationCauseType_t;

/**
 * @brief Enumeration for the SAT bearer destination type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BEARER_CSD = 0x1, /**< BEARER DESC CSD */
	TAPI_SAT_BEARER_GPRS = 0x2, /**< BEARER DESC GPRS */
	TAPI_SAT_BEARER_DEFAULT_BEARER_FROM_TRANSPORT_LAYER = 0x3, /**<	BEARER DESC DEFAULT BEARER FROM TRANSPORT LAYER */
	TAPI_SAT_BEARER_LOCAL_LINK_TECHNOLOGY_INDEPENDENT = 0x4, /**< BEARER DESC LOCAL LINK TECHNOLOGY INDEPENDENT */
	TAPI_SAT_BEARER_BLUETOOTH = 0x5, /**< BEARER DESC BLUETOOTH */
	TAPI_SAT_BEARER_IrDA = 0x6, /**< BEARER DESC IrDA */
	TAPI_SAT_BEARER_RS232 = 0x7, /**< BEARER DESC RS232 */
	TAPI_SAT_BEARER_USB = 0x10, /**< BEARER DESC USB */
	TAPI_SAT_BEARER_RESERVED = 0xFF /**< RESERVED */
} TelSatBearerDescType_t;

/**
 * @brief Enumeration for the SAT bearer CSD data rate.
 *
 * @details Refer TS 27.007.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_DR_AUTOBAUDING = 0, /**< CSD data rate - AUTOBAUDING */
	TAPI_SAT_BIP_DR_300BPS_V21 = 1, /**< CSD data rate - 300BPS V21 */
	TAPI_SAT_BIP_DR_1200BPS_V22 = 2, /**< CSD data rate - 1200BPS V22 */
	TAPI_SAT_BIP_DR_1200_75BPS_V23 = 3, /**< CSD data rate - 1200 75BPS V23 */
	TAPI_SAT_BIP_DR_2400BPS_V22 = 4, /**< CSD data rate - 2400BPS V22 */
	TAPI_SAT_BIP_DR_2400BPS_V26 = 5, /**< CSD data rate - 2400BPS V26 */
	TAPI_SAT_BIP_DR_4800BPS_V32 = 6, /**< CSD data rate - 4800BPS V32 */
	TAPI_SAT_BIP_DR_9600BPS_V32 = 7, /**< CSD data rate - 9600BPS V32 */
	TAPI_SAT_BIP_DR_9600BPS_V34 = 12, /**< CSD data rate - 9600BPS V34 */
	TAPI_SAT_BIP_DR_14400BPS_V34 = 14, /**<	CSD data rate - 14400BPS V34 */
	TAPI_SAT_BIP_DR_19200BPS_V34 = 15, /**<	CSD data rate - 19200BPS V34 */
	TAPI_SAT_BIP_DR_28800BPS_V34 = 16, /**<	CSD data rate - 28800BPS V34 */
	TAPI_SAT_BIP_DR_33600BPS_V34 = 17, /**<	CSD data rate - 33600BPS V34 */
	TAPI_SAT_BIP_DR_1200BPS_V120 = 34, /**<	CSD data rate - 1200BPS V120 */
	TAPI_SAT_BIP_DR_2400BPS_V120 = 36, /**<	CSD data rate - 2400BPS V120 */
	TAPI_SAT_BIP_DR_4800BPS_V120 = 38, /**<	CSD data rate - 4800BPS V120 */
	TAPI_SAT_BIP_DR_9600BPS_V120 = 39, /**<	CSD data rate - 9600BPS V120 */
	TAPI_SAT_BIP_DR_14400BPS_V120 = 43, /**< CSD data rate - 14400BPS V120 */
	TAPI_SAT_BIP_DR_19200BPS_V120 = 47, /**< CSD data rate - 19200BPS V120 */
	TAPI_SAT_BIP_DR_28800BPS_V120 = 48, /**< CSD data rate - 28800BPS V120 */
	TAPI_SAT_BIP_DR_38400BPS_V120 = 49, /**< CSD data rate - 38400BPS V120 */
	TAPI_SAT_BIP_DR_48000BPS_V120 = 50, /**< CSD data rate - 48000BPS V120 */
	TAPI_SAT_BIP_DR_56000BPS_V120 = 51, /**< CSD data rate - 56000BPS V120 */
	TAPI_SAT_BIP_DR_300BPS_V110 = 65, /**< CSD data rate - 300BPS V110 */
	TAPI_SAT_BIP_DR_1200BPS_V110 = 66, /**<	CSD data rate - 1200BPS V110 */
	TAPI_SAT_BIP_DR_2400BPS_V110_OR_X31_FALG_STUFFING = 68, /**< CSD data rate - 2400BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_4800BPS_V110_OR_X31_FALG_STUFFING = 70, /**< CSD data rate - 4800BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_9600BPS_V110_OR_X31_FALG_STUFFING = 71, /**< CSD data rate - 9600BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_14400BPS_V110_OR_X31_FALG_STUFFING = 75, /**< CSD data rate - 14400BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_19200BPS_V110_OR_X31_FALG_STUFFING = 79, /**< CSD data rate - 19200BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_28800BPS_V110_OR_X31_FALG_STUFFING = 80, /**< CSD data rate - 28800BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_38400BPS_V110_OR_X31_FALG_STUFFING = 81, /**< CSD data rate - 38400BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_48000BPS_V110_OR_X31_FALG_STUFFING = 82, /**< CSD data rate - 48000BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_56000BPS_V110_OR_X31_FALG_STUFFING = 83, /**< CSD data rate - 56000BPS V110 OR X31 FALG STUFFING */
	TAPI_SAT_BIP_DR_64000BPS = 84, /**<	CSD data rate - 64000BPS */
	TAPI_SAT_BIP_DR_56000BPS_BIT_TRANSPERENT = 115, /**< CSD data rate - 56000BPS BIT TRANSPERENT */
	TAPI_SAT_BIP_DR_64000BPS_BIT_TRANSPERENT = 116, /**< CSD data rate - 64000BPS BIT TRANSPERENT */
	TAPI_SAT_BIP_DR_32000BPS_PIAFS32K = 120, /**< CSD data rate - 32000BPS PIAFS32K */
	TAPI_SAT_BIP_DR_64000BPS_PIAFS64K = 121, /**< CSD data rate - 64000BPS PIAFS64K	*/
	TAPI_SAT_BIP_DR_28800BPS_MULTIMEDIA = 130, /**<	CSD data rate - 28800BPS MULTIMEDIA */
	TAPI_SAT_BIP_DR_32000BPS_MULTIMEDIA = 131, /**<	CSD data rate - 32000BPS MULTIMEDIA */
	TAPI_SAT_BIP_DR_33600BPS_MULTIMEDIA = 132, /**<	CSD data rate - 33600BPS MULTIMEDIA */
	TAPI_SAT_BIP_DR_56000BPS_MULTIMEDIA = 133, /**<	CSD data rate - 56000BPS MULTIMEDIA */
	TAPI_SAT_BIP_DR_64000BPS_MULTIMEDIA = 134 /**< CSD data rate - 64000BPS MULTIMEDIA */
} TelSatBearerParamCsdDataRateType_t;

/**
 * @brief Enumeration for the CSD bearer service type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_CSD_BS_DATA_CIRCUIT_ASYNC_UDI = 0, /**< CSD Bearer service - DATA CIRCUIT ASYNCHRONOUS UDI */
	TAPI_SAT_BIP_CSD_BS_DATA_CIRCUIT_SYNC = 1, /**<	CSD Bearer service - DATA CIRCUIT SYNCHRONOUS UDI */
	TAPI_SAT_BIP_CSD_BS_PAD_ACCESS_ASYNC_UDI = 2, /**< CSD Bearer service - PAD ACCESS ASYNCHRONOUS UDI	*/
	TAPI_SAT_BIP_CSD_BS_PACKET_ACCESS_SYNC = 3, /**< CSD Bearer service - PACKET ACCESS SYNCHRONOUS UDI */
	TAPI_SAT_BIP_CSD_BS_DATA_CIRCUIT_ASYNC_RDI = 4, /**< CSD Bearer service - DATA CIRCUIT ASYNCHRONOUS RDI */
	TAPI_SAT_BIP_CSD_BS_DATA_CIRCUIT_SYNC_RDI = 5, /**<	CSD Bearer service - DATA CIRCUIT SYNCHRONOUS RDI */
	TAPI_SAT_BIP_CSD_BS_PAD_ACCESS_ASYNC_RDI = 6, /**< CSD Bearer service - PAD ACCESS ASYNCHRONOUS RDI */
	TAPI_SAT_BIP_CSD_BS_PACKET_ACCESS_SYNC_RDI = 7 /**<	CSD Bearer service - PACKET ACCESS SYNCHRONOUS RDI */
} TelSatBearerParamCsdBearerServiceType_t;

/**
 * @brief Enumeration for the SAT bearer CSD connection element.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_CSD_CONN_ELEM_TRANSPARENT = 0, /**< CSD connection element - TRANSPARENT */
	TAPI_SAT_BIP_CSD_CONN_ELEM_NON_TRANSPARENT = 1, /**< CSD connection element - NON TRANSPARENT */
	TAPI_SAT_BIP_CSD_CONN_ELEM_BOTH_TRANSPARENT_PREF = 2, /**< CSD connection element - BOTH TRANSPARENT PREFFERED */
	TAPI_SAT_BIP_CSD_CONN_ELEM_BOTH_NON_TRANSPARENT_PREF = 3 /**< CSD connection element - NON TRANSPARENT PREFFERED */
} TelSatBearerParamCsdConnectionElementType_t;

/**
 * @brief Enumeration for the SAT bearer GPRS precedence class.
 *
 * @details Refer TS 23.107.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_GPRS_PRECED_CLASS_HIGH_PRIORITY = 0x01, /**< GPRS precedence class - HIGH PRIORITY	*/
	TAPI_SAT_BIP_GPRS_PRECED_CLASS_NORM_PRIORITY = 0x02, /**< GPRS precedence class - NORM PRIORITY	*/
	TAPI_SAT_BIP_GPRS_PRECED_CLASS_LOW_PRIORITY = 0x03 /**<	GPRS precedence class - LOW PRIORITY */
} TelSatBearerParamGprsPrecedenceClassType_t;

/**
 * @brief Enumeration for the SAT bearer GPRS delay class.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_GPRS_DELAY_CLASS_1 = 0x01, /**< GPRS delay class - 1 */
	TAPI_SAT_BIP_GPRS_DELAY_CLASS_2 = 0x02, /**< GPRS delay class -	2 */
	TAPI_SAT_BIP_GPRS_DELAY_CLASS_3 = 0x03, /**< GPRS delay class -	3 */
	TAPI_SAT_BIP_GPRS_DELAY_CLASS_4 = 0x04 /**< GPRS delay class - 4 */
} TelSatBearerParamGprsDelayClassType_t;

/**
 * @brief Enumeration for the SAT bearer GPRS Reliability class.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_GPRS_RELIABILITY_CLASS_1 = 0x01, /**< GPRS Reliability class - 1 */
	TAPI_SAT_BIP_GPRS_RELIABILITY_CLASS_2 = 0x02, /**< GPRS Reliability class - 2 */
	TAPI_SAT_BIP_GPRS_RELIABILITY_CLASS_3 = 0x03, /**< GPRS Reliability class - 3 */
	TAPI_SAT_BIP_GPRS_RELIABILITY_CLASS_4 = 0x04, /**< GPRS Reliability class - 4 */
	TAPI_SAT_BIP_GPRS_RELIABILITY_CLASS_5 = 0x05 /**< GPRS Reliability class - 5 */
} TelSatBearerParamGprsReliabilityClassType_t;

/**
 * @brief Enumeration for the SAT bearer GPRS peak throughput class.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_8KBPS = 0x01, /**<	GPRS peak throughput class - UPTO 8KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_16KBPS = 0x02, /**< GPRS peak throughput class - UPTO 16KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_32KBPS = 0x03, /**< GPRS peak throughput class - UPTO 32KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_64KBPS = 0x04, /**< GPRS peak throughput class - UPTO 64KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_128KBPS = 0x05, /**< GPRS peak throughput class - UPTO 128KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_256KBPS = 0x06, /**< GPRS peak throughput class - UPTO 256KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_512KBPS = 0x07, /**< GPRS peak throughput class - UPTO 512KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_1024KBPS = 0x08, /**< GPRS peak throughput class - UPTO 1024KBPS */
	TAPI_SAT_BIP_GPRS_PEAK_THROUGHPUT_CLASS_UPTO_2048KBPS = 0x09 /**< GPRS peak throughput class - UPTO 2048KBPS */
} TelSatBearerParamGprsPeakThroughputClassType_t;

/**
 * @brief Enumeration for the SAT bearer GPRS mean throughput class.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_DOT_22_BPS = 0x01, /**<	GPRS mean throughput class - DOT 22 BPS	*/
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_DOT_44_BPS = 0x02, /**<	GPRS mean throughput class - DOT 44 BPS	*/
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_1_DOT_11_BPS = 0x03, /**< GPRS mean throughput class - 1 DOT 11 BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_2_DOT_2_BPS = 0x04, /**< GPRS mean throughput class - 2 DOT 2 BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_2_DOT_4_BPS = 0x05, /**< GPRS mean throughput class - 2 DOT 4 BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_11_DOT_1_BPS = 0x06, /**< GPRS mean throughput class - 11 DOT 1 BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_22BPS = 0x07, /**< GPRS mean throughput class - 22BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_44BPS = 0x08, /**< GPRS mean throughput class - 44BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_111BPS = 0x09, /**<	GPRS mean throughput class - 111BPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_DOT_22_KBPS = 0x0A, /**< GPRS mean throughput class - DOT 22 KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_DOT_44_KBPS = 0x0B, /**< GPRS mean throughput class -DOT 44 KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_1_DOT_11_KBPS = 0x0C, /**< GPRS mean throughput class -1 DOT 11 KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_2_DOT_2_KBPS = 0x0D, /**< GPRS mean throughput class -2 DOT 2 KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_4_DOT_4_KBPS = 0x0E, /**< GPRS mean throughput class - 4 DOT 4 KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_11_DOT_1_KBPS = 0x0F, /**< GPRS mean throughput class -11 DOT 1 KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_22KBPS = 0x10, /**<	GPRS mean throughput class - 22KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_44KBPS = 0x11, /**<	GPRS mean throughput class - 44KBPS	*/
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_111KBPS = 0x12, /**<	GPRS mean throughput class -111KBPS */
	TAPI_SAT_BIP_GPRS_MEAN_THROUGHPUT_CLASS_BEST_EFFORT = 0x13 /**<	GPRS mean throughput class - BEST EFFORT */
} TelSatBearerParamGprsMeanThroughputClassType_t;

/**
 * @brief Enumeration for the SAT bearer GPRS PDP type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_GPRS_PDP_TYPE_IP = 0x02, /**< bearer parameter GPRS PDP type - IP */
	TAPI_SAT_BIP_GPRS_PDP_TYPE_RESERVED = 0xff /**<	reserved */
} TelSatBearerParamGprsPdpType_t;

/**
 * @brief Enumeration for the SAT bearer local links service identity.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_BIP_LL_SERVICE_IDENT_VALUE_ASSIGNED_BY_USIM = 0x00, /**< local links service identity - value assigned by USIM */
	TAPI_SAT_BIP_LL_SERVICE_IDENT_VALUE_ASSIGNED_BY_REMOTED_DEVICE = 0xFF /**< local links service identity - value assigned by remote service */
} TelSatBearerParamLocalLinksServiceIdentityType_t;

/**
 * @brief Enumeration for the SAT channel status type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_CS_LINK_ESTABLISHED_OR_PDP_CTX_NOT_ACTIVATED, /**<	channel status type - CS LINK ESTABLISHED OR PDP CTX NOT ACTIVATED */
	TAPI_SAT_CS_LINK_ESTABLISHED_OR_PDP_CTX_ACTIVATED, /**<	channel status type - CS LINK ESTABLISHED OR PDP CTX ACTIVATED */
	TAPI_SAT_UICC_SERVER_MODE_TCP_IN_CLOSED_STATE, /**<	channel status type - UICC SERVER MODE TCP IN CLOSED STATE */
	TAPI_SAT_UICC_SERVER_MODE_TCP_IN_LISTEN_STATE, /**<	channel status type - UICC SERVER MODE TCP IN LISTEN STATE */
	TAPI_SAT_UICC_SERVER_MODE_TCP_IN_ESTABLISHED_STATE, /**< channel status type - UICC SERVER MODE TCP IN ESTABLISHED STATE */
	TAPI_SAT_UICC_SERVER_MODE_RESERVED /**<	reserved */
} TelSatChannelStatusType_t;

/**
 * @brief Enumeration for the SAT channel status info type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_CHANNEL_STATUS_NO_FURTHER_INFO_AVAILABLE = 0, /**< CHANNEL STATUS NO FURTHER INFO AVAILABLE */
	TAPI_SAT_CHANNEL_STATUS_NOT_USED = 1, /**< CHANNEL STATUS NOT USED */
	TAPI_SAT_CHANNEL_STATUS_LINK_DROPPED = 5 /**< CHANNEL STATUS LINK DROPPED */
} TelSatChannelStatusInfoType_t;

/**
 * @brief Enumeration for the SAT address type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_ADDR_TYPE_IPv4 = 0x21, /**< address type - IPv4 */
	TAPI_SAT_ADDR_TYPE_IPv6 = 0x57, /**< address type - IPv6 */
	TAPI_SAT_ADDR_RESERVED = 0xFF /**< reserved	*/
} TelSatAddressType_t;

/**
 * @brief Enumeration for the SAT transport protocol type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_TP_TYPE_UDP_UICC_CLIENT = 0x01, /**< transport protocol type - UDP UICC CLIENT */
	TAPI_SAT_TP_TYPE_TCP_UICC_CLIENT = 0x02, /**< transport protocol type - TCP UICC CLIENT */
	TAPI_SAT_TP_TYPE_TCP_UICC_SERVER = 0x03 /**< transport protocol type - TCP UICC SERVER */
} TelSatTransportProtocolType_t;

/**
 * @brief Enumeration for the SAT remote entity address coding type.
 * @since_tizen 2.3
 */
typedef enum {
	TAPI_SAT_REMOTE_ENTITY_ADDR_CODING_TYPE_IEEE802_48BIT = 0, /**<	remote entity address coding type - IEEE802 48BIT */
	TAPI_SAT_REMOTE_ENTITY_ADDR_CODING_TYPE_IRDA_32BIT = 1, /**< remote entity address coding type - IRDA 32BIT	*/
	TAPI_SAT_REMOTE_ENTITY_ADDR_CODING_TYPE_RESERVED = 0xFF /**< reserved */
} TelSatRemoteEntityAddrCodingType_t;

/**
 * @brief The structure type defining the address data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimTypeOfNum_t ton; /**<	Type of number */
	TelSimNumberingPlanIdentity_t npi; /**<	Number plan identity */
	unsigned char diallingNumberLen; /**< Dialing the number length */
	char diallingNumber[TAPI_SAT_DIALLING_NUMBER_LEN_MAX]; /**<	Dialing the number */
} TelSatAddressInfo_t;

/**
 * @brief The structure type defining the data coding scheme object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsCompressedFormat; /**< Flag to verify the compressed format */
	TelSatAlphabetFormatType_t alphabetFormat; /**<	Alphabet format type */
	TelSatMsgClassType_t msgClass; /**<	Type of message class */
	unsigned char rawDcs; /**< Raw DCS info */
} TelSatDataCodingSchemeInfo_t;

/**
 * @brief The structure type defining the Alpha ID data object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsPresent; /**< Flag for checking the existence of the alpha identifier */
	TelSatDataCodingSchemeInfo_t dcs; /**< DCS info */
	unsigned char stringLen; /**< Alpha identifier length */
	char string[TAPI_SAT_ALPHA_ID_LEN_MAX]; /**< Alpha identifier info */
} TelSatAlphaIdentifierInfo_t;

/**
 * @brief The structure type defining the Sub Address data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char subAddressLen; /**< Sub address length */
	char subAddress[TAPI_SAT_SUB_ADDR_LEN_MAX]; /**< Sub address */
} TelSatSubAddressInfo_t;

/**
 * @brief The structure type defining the Capability Configuration Parameters data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char dataLen; /**<	Capability configuration parameter length */
	char data[TAPI_SAT_CCP_DATA_LEN_MAX]; /**< Capability configuration parameter */
} TelSatCapaConfigParamInfo_t;

/**
 * @brief The structure type defining command qualifier values for the send SMS command.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsPackingByMeRequired; /**< Flag to verify packing requirement, if FALSE packing by ME is not required */
} TelSatCmdQualiSendSms_t;

/**
 * @brief The structure type defining command qualifier values for the display text command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatDisplayTextPriorityType_t msgPriority; /**< Message priority */
	TelSatDisplayTextMsgClearType_t msgClear; /**< Message clear type */
} TelSatCmdQualiDisplayText_t;

/**
 * @brief The structure type defining command qualifier values for the get inkey command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatInkeyType_t inkeyType; /**< Inkey type */
	int bIsUsingAlphabetSet; /**< Flag for checking whether alphabet set is being used. If FALSE, digits(0-9,*,and+) only */
	TelSatUseInputAlphabetType_t alphabetType; /**<	Alphabet type */
	int bIsImmediateResponseRequired;/**< Flag for checking whether immediate response is required */
	int bIsHelpInfoAvailable; /**< Flag for checking whether help info is available. If FALSE, no help information is available */
} TelSatCmdQualiGetInkey_t;

/**
 * @brief The structure type defining command qualifier values for the get input command.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsUsingAlphabetSet; /**< Flag for checking whether alphabet set is being used. If FALSE, digits(0-9,*,and+) only */
	TelSatUseInputAlphabetType_t alphabetType; /**<	Alphabet type. Only using alphabet set case */
	int bIsMeEchoUserInput; /**< Flag for checking whether ME should echo user input. If FALSE, user input shall not be displayed */
	int bIsUserInputUnpackedFormat; /**< Flag for checking whether user input is in unpacked format. If FALSE, user input is in the SMS packed format */
	int bIsHelpInfoAvailable; /**< Flag to verify if help info is available. If FALSE, no help information is available	*/
} TelSatCmdQualiGetInput_t;

/**
 * @brief The structure type defining command qualifier values for the play tone command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatDisplayVibrateAlertType_t vibrateAlert; /**< Type of vibrate alert */
} TelSatCmdQualiPlayTone_t;

/**
 * @brief The structure type defining command qualifier values for the select item command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatPresentationType_t presentationType; /**<	Type of presentation. Only presentation type specified */
	TelSatSelectionPreferenceType_t selectionPreference; /**< Type of selection preference */
	int bIsHelpInfoAvailable; /**< Flag for checking whether help info is available. If FALSE, no help information is available	*/
} TelSatCmdQualiSelectItem_t;

/**
 * @brief The structure type defining command qualifier values for the setup menu command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatSelectionPreferenceType_t selectionPreference; /**< Type of selection preference */
	int bIsHelpInfoAvailable; /**< Flag to verify whether help info is available. If FALSE, no help information is available */
} TelSatCmdQualiSetupMenu_t;

/**
 * @brief The structure type defining command qualifier values for the language notification command.
 * @since_tizen 2.3
 */
typedef struct {
	int bSpecificLanguageNotification; /**<	Flag for a specific language notification. If FALSE, it is a non-specific language notification */
} TelSatCmdQualiLanguageNotification_t;

/**
 * @brief The structure type defining the SAT command qualifier open channel.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsEstablishImmediateLink; /**< Flag to verify whether immediate link is establishing. If FALSE, link establishment	on demand */
	int bIsAutomaticReconnection; /**< Flag to verify whether automatic reconnection is establishing. If FALSE, no automatic reconnection */
	int bIsModeBackground; /**<	Flag to verify whether background mode is activated */
} TelSatCmdQualiOpenChannel_t;

/**
 * @brief The structure type containing SAT command qualifier send data.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsSendDataImmediately; /**< Flag to verify whether to send data immediately. If FALSE, store data in the Tx buffer */
} TelSatCmdQualiSendData_t;

/**
 * @brief The structure type containing the command number, type, and the qualifier objects of a SATK command.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char commandNumber; /**< Specific command number */
	TelSatCommandType_t commandType; /**< Proactive command type */

	union {
		TelSatCmdQualiRefresh_t cmdQualiRefresh; /**< Refresh command qualifier info */
		TelSatCmdQualiSetupCall_t cmdQualiSetupCall; /**< Setup call command qualifier info	*/
		TelSatCmdQualiSendSms_t cmdQualiSendSms; /**< Send SMS command qualifier info */
		TelSatCmdQualiDisplayText_t cmdQualiDisplayText; /**< Display text command qualifier info */
		TelSatCmdQualiGetInkey_t cmdQualiGetInkey; /**< Get inkey command qualifier info */
		TelSatCmdQualiGetInput_t cmdQualiGetInput; /**< Get input command qualifier info */
		TelSatCmdQualiPlayTone_t cmdQualiPlayTone; /**< Play tone command qualifier info */
		TelSatCmdQualiSelectItem_t cmdQualiSelectItem; /**< Select item command qualifier info */
		TelSatCmdQualiSetupMenu_t cmdQualiSetupMenu; /**< Setup menu command qualifier info	*/
		TelSatCmdQualiProvideLocalInfo_t cmdQualiProvideLocalInfo;/**< Provide local info command qualifier info */
		TelSatCmdQualiLanguageNotification_t cmdQualiLanguageNotification;/**< Language notification command qualifier info	*/
		TelSatCmdQualiLaunchBrowser_t cmdQualiLaunchBrowser; /**< Launch browser command qualifier info	*/
		TelSatCmdQualiOpenChannel_t cmdQualiOpenChannel; /**< Open channel command qualifier info */
		TelSatCmdQualiSendData_t cmdQualiSendData; /**< Send data command qualifier info */
	} u; /**<	Union  	*/
} TelSatCommandDetailInfo_t;

/**
 * @brief The structure type defining device identity values.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatDeviceIdentitiesTagType_t source; /**< Device identity tag for source	*/
	TelSatDeviceIdentitiesTagType_t destination; /**< Device identity tag for destination */
} TelSatDeviceIdentitiesInfo_t;

/**
 * @brief The structure type defining the duration data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatTimeUnitType_t timeUnit; /**<	Time units for the duration data */
	unsigned char timeInterval; /**< Time interval */
} TelSatDurationInfo_t;

/**
 * @brief The structure type defining the menu item data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char itemId; /**< Item identifier */
	unsigned char textLen; /**< Text length */
	unsigned char text[TAPI_SAT_ITEM_TEXT_LEN_MAX + 1]; /**< Text information */
} TelSatMenuItemInfo_t;

/**
 * @brief The structure type defining the item identifier object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char selectedItemIdentifier; /**< Selected item identifier	*/
} TelSatItemIdentifierInfo_t;

/**
 * @brief The structure type defining expected user response length.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char min; /**<	User response length's minimum value */
	unsigned char max; /**<	User response length's maximum value */
} TelSatRespLenInfo_t;

/**
 * @brief The structure type defining the result data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultType_t generalResult; /**< General result */
	TelSatMeProblemType_t meProblem; /**< Additional information on the general result */
} TelSatResultInfo_t;

/**
 * @brief The structure type defining the RESULT data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatSmsTpduType_t tpduType; /**< SMS TPDU TYPE */
	unsigned char dataLen; /**<	SMS TPDU DATA LENGTH */
	unsigned char data[TAPI_SAT_SMS_TPDU_SMS_DATA_LEN_MAX]; /**< SMS TPDU DATA */
} TelSatSmsTpduInfo_t;

/**
 * @brief The structure type defining the SS STRING data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSimTypeOfNum_t ton; /**<	Type of number */
	TelSimNumberingPlanIdentity_t npi; /**<	Number plan identity */
	unsigned char stringLen; /**< SS string length */
	char string[TAPI_SAT_SS_STRING_LEN_MAX]; /**< SS string */
} TelSatSsStringInfo_t;

/**
 * @brief The structure type defining the TEXT STRING data object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsDigitOnly; /**< Flag for checking whether only digits are used */
	TelSatDataCodingSchemeInfo_t dcs; /**< Data coding scheme */
	unsigned short stringLen; /**< Text length */
	char string[TAPI_SAT_TEXT_STRING_LEN_MAX + 1]; /**<	Text string */
} TelSatTextTypeInfo_t;

/**
 * @brief The structure type defining the menu item text object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsDigitOnly; /**< Flag for checking whether only digits are used */
	TelSatDataCodingSchemeInfo_t dcs; /**< Data coding scheme */
	unsigned char stringLen; /**< Menu item string length */
	char* pString; /**<	Menu item string */
} TelSatMenuItemTextInfo_t;

/**
 * @brief The structure type defining the tone object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatToneType_t type; /**<	Tone type */
} TelSatToneInfo_t;

/**
 * @brief The structure type defining the USSD string data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatDataCodingSchemeInfo_t dcs; /**< Data coding scheme */
	unsigned char ussdStringLen; /**< USSD string length */
	char ussdString[TAPI_SAT_USSD_STRING_LEN_MAX]; /**<	USSD string	*/
} TelSatUssdStringInfo_t;

/**
 * @brief The structure type defining the file list data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char fileCount; /**< File count */
//	TelSimFileName_t fileId[TAPI_SAT_FILE_ID_LIST_MAX_COUNT]; /**< File identifier */
} TelSatFileListInfo_t;

/**
 * @brief The structure type defining the default text data object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsPresent; /**< Flag for checking whether default text exists */
	int bIsDigitOnly; /**< Flag for checking whether only digits are used */
	TelSatDataCodingSchemeInfo_t dcs; /**< Data coding scheme */
	unsigned char stringLen; /**< Default text string length */
	char string[TAPI_SAT_TEXT_STRING_LEN_MAX]; /**<	Default text */
} TelSatDefaultTextInfo_t;

/**
 * @brief The structure type defining the Next Action Indicator List data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char listCount; /**< Next action identifier count */
	unsigned char list[TAPI_SAT_ITEMS_NEXT_ACTION_INDI_LIST_MAX_COUNT]; /**< Next action identifier list */
} TelSatItemsNextActionIndiListInfo_t;

/**
 * @brief The structure type defining the event list data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char eventListCount; /**< Event list count	*/
	TelSatEventDownloadType_t list[TAPI_SAT_EVENT_LIST_MAX_COUNT]; /**<	Event list */
} TelSatEventListInfo_t;

/**
 * @brief The structure type defining the icon info object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char width; /**< Icon width */
	unsigned char height; /**< Icon height */
	TelSatImageCodingSchemeType_t ics; /**< Image coding scheme */
	unsigned short iconDataLen; /**< Icon data length */
	unsigned short clutDataLen; /**< Clut data length */
	char iconFile[TAPI_SAT_IMG_DATA_FILE_PATH_LEN_MAX];	/**< Icon File */
	char clutFile[TAPI_SAT_IMG_DATA_FILE_PATH_LEN_MAX];	/**< Clut File */
} TelSatIconInfo_t;

/**
 * @brief The structure type defining the icon data object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsPresent; /**< Flag for checking whether the icon identifier exists */
	TelSatIconQualifierType_t iconQualifier; /**< Icon qualifier type */
	unsigned char iconIdentifier; /**< Icon identifier */
	TelSatIconInfo_t iconInfo; /**< Icon info */
} TelSatIconIdentifierInfo_t;

/**
 * @brief The structure type defining the icon identifier data object.
 * @since_tizen 2.3
 */
typedef struct {
	int bIsPresent; /**< Flag for checking whether the icon identifier exists */
	TelSatIconQualifierType_t iconListQualifier; /**< Icon list qualifier */
	unsigned char iconCount; /**< Icon count */
	unsigned char iconIdentifierList[TAPI_SAT_ICON_LIST_MAX_COUNT]; /**< Icon identifier list */
	TelSatIconInfo_t iconInfo[TAPI_SAT_ICON_LIST_MAX_COUNT]; /**< Icon list info */
} TelSatIconIdentifierListInfo_t;

/**
 * @brief The structure type defining SAT BC repeat indicator info.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBcRepeatIndicatorType_t indType; /**<	BC repeat indicator type */
} TelSatBcRepeatIndicatorInfo_t;

/**
 * @brief The structure type defining call control strings.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCallCtrlStringType_t callCtrlStringType; /**<	Call control type */
	union {
		TelSatAddressInfo_t voiceString; /**< Voice call string	*/
		TelSatSsStringInfo_t ssString; /**<	SS string */
		TelSatUssdStringInfo_t ussdString; /**<	USSD string	*/
	} u; /**< Union */
} TelSatCallCtrlAddressStringInfo_t;

/**
 * @brief The structure type defining action requested call control data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCallCtrlAddressStringInfo_t callCtrlAddString; /**< Call control address string */
	TelSatCapaConfigParamInfo_t ccp1; /**< Capability configuration parameter 1 */
	TelSatSubAddressInfo_t subAddress; /**< Subaddress */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier */
	TelSatBcRepeatIndicatorInfo_t bcRepeatIndicator; /**< BC repeat indicator */
	TelSatCapaConfigParamInfo_t ccp2; /**< Capability configuration parameter 2 */
} TelSatCallCtrlRequestedActionInfo_t;

/**
 * @brief The structure type defining the DTMF string data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char stringLen; /**< DTMF string length */
	char dtmfTone[TAPI_SAT_DTMF_STRING_LEN_MAX]; /**< DTMF tone data */
} TelSatDtmfStringInfo_t;

/**
 * @brief The structure type defining the language data object.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatLanguageType_t language; /**< Language type */
} TelSatLanguageInfo_t;

/**
 * @brief The structure type defining the date time and time zone data object.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char year; /**< Year */
	unsigned char month; /**< Month	*/
	unsigned char day; /**< Day	*/
	unsigned char hour; /**< Hour */
	unsigned char minute; /**< Minute */
	unsigned char second; /**< Second */
	unsigned char timeZone; /**< TimeZone */
} TelSatDataTimeZoneInfo_t;

/**
 * @brief The structure type defining SAT browser identities.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBrowserIdentityType_t browserIdentity; /**< Browser identity */
} TelSatBrowserIdentitiesInfo_t;

/**
 * @brief The structure type defining the SAT browser URL data object.
 * @since_tizen 2.3
 */
typedef struct {
	char string[TAPI_SAT_URL_LEN_MAX + 1]; /**< URL string */
} TelSatUrlInfo_t;

/**
 * @brief The structure type defining the SAT bearer type.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char listLen; /**<	Bearer list length	*/
	TelSatBearerType_t bearerList[TAPI_SAT_BEARER_LIST_MAX_COUNT]; /**<	Bearer list	*/
} TelSatBearerInfo_t;

/**
 * @brief The structure type defining the SAT provisioning reference.
 * @since_tizen 2.3
 */
typedef struct {
	char provisioningFilePath[TAPI_SAT_PROVISIONING_FILE_PATH_LEN_MAX]; /**< Provisioning file path	*/
} TelSatProvisioningRefInfo_t;

/**
 * @brief The structure type defining the SAT browser termination cause.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBrowserTerminationCauseType_t cause; /**<	Browser termination cause */
} TelSatBrowserTerminationCauseInfo_t;

/**
 * @brief The structure type defining SAT CSD bearer parameters.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBearerParamCsdDataRateType_t dataRate; /**< Bearer CSD data rate */
	TelSatBearerParamCsdBearerServiceType_t bearerService; /**< Bearer CSD service type	*/
	TelSatBearerParamCsdConnectionElementType_t connectionElement; /**<	Bearer connection element type */
} TelSatBearerParametersCsdInfo_t;

/**
 * @brief The structure type defining SAT GPRS bearer parameters.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBearerParamGprsPrecedenceClassType_t precedenceClass; /**< Bearer GPRS precedence class */
	TelSatBearerParamGprsDelayClassType_t delayClass; /**< Bearer GPRS delay */
	TelSatBearerParamGprsReliabilityClassType_t reliabilityClass; /**< Bearer GPRS reliability */
	TelSatBearerParamGprsPeakThroughputClassType_t peakThroughputClass;/**<	Bearer peak throughput */
	TelSatBearerParamGprsMeanThroughputClassType_t meanThroughputClass;/**<	Bearer GPRS mean throughput	*/
	TelSatBearerParamGprsPdpType_t pdpType; /**<	bearer gprs pdp type	*/
} TelSatBearerParametersGprsInfo_t;

/**
 * @brief The structure type defining SAT local links bearer parameters.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBearerParamLocalLinksServiceIdentityType_t serviceIdentifier; /**< Bearer local link service identifier */
	char serviceRecord[TAPI_SAT_BEARER_PARAMS_LEN_MAX]; /**< Bearer local link service record */
} TelSatBearerParametersLocalLinksInfo_t;

/**
 * @brief The structure type defining the SAT bearer description.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBearerDescType_t bearerType; /**<	Bearer type	*/
	union {
		TelSatBearerParametersCsdInfo_t bearerParamsCsd; /**< CSD */
		TelSatBearerParametersGprsInfo_t bearerParamsGprs; /**<	GPRS */
		TelSatBearerParametersLocalLinksInfo_t bearerParamsLocalLinks; /**<	Local link */
	} bearer_params; /**< Union */
} TelSatBearerDescriptionInfo_t;

/**
 * @brief The structure type defining SAT channel data.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char channelDataStringLen; /**< Channel data length */
	char channelDataString[TAPI_SAT_CHANNEL_DATA_STRING_LEN_MAX]; /**< Channel data */
} TelSatChannelDataInfo_t;

/**
 * @brief The structure type defining the SAT channel data length.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char channelDataLen; /**< Channel data length */
} TelSatChannelDataLengthInfo_t;

/**
 * @brief The structure type defining the SAT buffer size.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char bufferSize[2]; /**< Channel data buffer size */
} TelSatBufferSizeInfo_t;

/**
 * @brief The structure type defining the SAT channel status.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char channelId; /**< Channel ID */
	TelSatChannelStatusType_t status; /**< Channel status */
	TelSatChannelStatusInfoType_t channelInfo; /**<	Channel status info */
} TelSatChannelStatusInfo_t;

/**
 * @brief The structure type defining another SAT address.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatAddressType_t addressType; /**< Channel address type */
	unsigned char addressLen; /**< Channel address length */
	char address[TAPI_SAT_OTHER_ADDR_LEN_MAX]; /**< Channel address	*/
} TelSatOtherAddressInfo_t;

/**
 * @brief The structure type defining the SIM ME interface transport level.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatTransportProtocolType_t transportProtocolType; /**< Transport protocol type */
	unsigned short portNumber; /**<	Port number	*/
} TelSatSimMeInterfaceTransportLevelInfo_t;

/**
 * @brief The structure type defining the SAT network access name.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char length; /**< Network access name length */
	unsigned char netAccName[TAPI_SAT_NET_ACC_NAM_LEN_MAX]; /**< Network access name */
} TelSatnetworkAccessNameInfo_t;

/**
 * @brief The structure type defining the SAT aid.
 * @since_tizen 2.3
 */
typedef struct {
	char aid[TAPI_SAT_AID_LEN_MAX]; /**< Application ID	*/
} TelSatAidInfo_t;

/**
 * @brief The structure type defining the SAT remote entity address.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatRemoteEntityAddrCodingType_t codingType; /**<	Remote entity address coding type */
	unsigned short length; /**<	Remote entity address length */
	unsigned char remoteEntityAddress[TAPI_SAT_REMOTE_ENTITY_ADDR_LEN_MAX]; /**< Remote entity address data	*/
} TelSatRemoteEntityAddressInfo_t;

/**
 * @brief The structure type defining the SAT text attribute.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char textFormatting[4]; /**< Text attribute - e.g. bold, center align, and so on */
} TelSatTextAttributeInfo_t;

/**
 * @brief The structure type defining the SAT text attribute list.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned int listCount; /**< Text attribute list count */
	TelSatTextAttributeInfo_t list[TAPI_SAT_ITEM_TEXT_ATTRIBUTES_LIST_MAX_COUNT]; /**< Text attribute list info	*/
} TelSatTextAttributeListInfo_t;

#ifdef __cplusplus
}
#endif

#endif	/* _TEL_SAT_OBJ_H_ */

/**
 * @}
 */
