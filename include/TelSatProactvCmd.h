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
 * @file TelSatProactvCmd.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SAT
 * @{
 */

#ifndef _TEL_SAT_PROACTV_CMD_H_
#define _TEL_SAT_PROACTV_CMD_H_

#include <TelSatObj.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Definition for the max count of SAT menu items.
 * @since_tizen 2.3
 */
#define TAPI_SAT_MENU_ITEM_COUNT_MAX			40
/**
 * @brief Definition for the max count of SAT provisioning reference.
 * @since_tizen 2.3
 */
#define TAPI_SAT_PROVISIONING_REF_MAX_COUNT		10

/**
 * @brief The structure type containing the data objects for DISPLAY TEXT proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
} TelSatMoreTimeIndInfo_t;

/**
 * @brief The structure type containing the data objects for DISPLAY TEXT proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatTextTypeInfo_t text; /**<	Display text info */
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	int bImmediateRespRequested; /**< Flag for checking whether immediate response is required */
	TelSatDurationInfo_t duration; /**<	Duration for which text should be displayed	*/
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align, and so on */
} TelSatDisplayTextIndInfo_t;

/**
 * @brief The structure type containing the data objects for GET INKEY proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatTextTypeInfo_t text; /**<	Display text info */
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatDurationInfo_t duration; /**<	Duration for which text should be displayed	*/
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatGetInkeyIndInfo_t;

/**
 * @brief The structure type containing the data objects for GET INPUT proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatTextTypeInfo_t text; /**<	Display text info */
	TelSatRespLenInfo_t respLen; /**< Input response length	*/
	TelSatTextTypeInfo_t defaultText; /**< Default text info */
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatGetInputIndInfo_t;

/**
 * @brief The structure type containing the data objects for PLAY TONE proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatToneInfo_t tone; /**<	Tone info */
	TelSatDurationInfo_t duration; /**<	Duration for which tone should be played */
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatPlayToneIndInfo_t;

/**
 * @brief The structure type containing the data objects for SETUP MENU proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	unsigned char menuItemCount; /**< Count of menu items */
	TelSatMenuItemInfo_t menuItem[TAPI_SAT_MENU_ITEM_COUNT_MAX]; /**< Menu item data */
	TelSatItemsNextActionIndiListInfo_t itemNextActionIndList; /**<	Next action indication list	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatIconIdentifierListInfo_t iconIdList; /**<	Icon identifier list info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
	TelSatTextAttributeListInfo_t itemTextAttributeList; /**< Item text attribute list */
} TelSatSetupMenuIndInfo_t;

/**
 * @brief The structure type containing the data objects for SELECT ITEM proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatItemsNextActionIndiListInfo_t itemNextActionIndList; /**<	Next action indication list	*/
	unsigned char defaultItemIdentifier; /**< Default item identifier(default selected item ID)	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatIconIdentifierListInfo_t iconIdList; /**<	Icon identifier list info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
	TelSatTextAttributeListInfo_t itemTextAttributeList; /**< Item text attribute list */
	unsigned char menuItemCount; /**< Count of menu items */
	TelSatMenuItemInfo_t menuItem[TAPI_SAT_MENU_ITEM_COUNT_MAX]; /**< Menu item data */
} TelSatSelectItemIndInfo_t;

/**
 * @brief The structure type containing the data objects for SEND SHORT MESSAGE proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatAddressInfo_t address; /**< Address for sending SMS */
	TelSatSmsTpduInfo_t smsTpdu; /**< SMS TPDU info	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatSendSmsIndInfo_t;

/**
 * @brief The structure type containing the data objects for SEND SS proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatSsStringInfo_t ssString; /**< SS string */
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatSendSsIndInfo_t;

/**
 * @brief The structure type containing the data objects for SEND USSD proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatUssdStringInfo_t ussdString; /**<	USSD string info */
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatSendUssdIndInfo_t;

/**
 * @brief The structure type containing the data objects for SETUP CALL proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatAddressInfo_t address; /**< Setup call address info */
	TelSatCapaConfigParamInfo_t ccp; /**< Capability configuration parameter */
	TelSatSubAddressInfo_t subAddress; /**<	Setup call sub address */
	TelSatDurationInfo_t duration; /**<	Command execution time duration	*/
	TelSatAlphaIdentifierInfo_t userConfirmPhaseAlphaId;/**< User Confirmation Phase Alpha ID */
	TelSatAlphaIdentifierInfo_t callSetupPhaseAlphaId; /**<	Call Setup Phase Alpha ID */
	TelSatIconIdentifierInfo_t userConfirmPhaseIconId; /**<	User Confirmation Phase Icon ID	*/
	TelSatIconIdentifierInfo_t callSetupPhaseIconId; /**< Call Setup Phase Icon ID */
	TelSatTextAttributeInfo_t userConfirmPhaseTextAttribute; /**< User Confirmation Phase Text Attribute */
	TelSatTextAttributeInfo_t callSetupPhaseTextAttribute; /**<	Call Setup Phase Text Attribute	*/
} TelSatSetupCallIndInfo_t;

/**
 * @brief The structure type containing the data objects for REFRESH proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatFileListInfo_t fileList; /**<	File list for refresh */
	TelSatAidInfo_t aid; /**< Application ID */
} TelSatRefreshIndInfo_t;

/**
 * @brief The structure type containing the data objects for PROVIDE LOCAL INFO proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/

} TelSatProvideLocalInfoIndInfo_t;

/**
 * @brief The structure type containing the data objects for SETUP EVENT LIST proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatEventListInfo_t eventList; /**< Event list structure type containing events which are required by the USIM application */

} TelSatSetupEventListIndInfo_t;

/**
 * @brief The structure type containing the data objects for SETUP IDLE MODE TEXT proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatTextTypeInfo_t text; /**<	Text to be shown on an idle screen	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatSetupIdleModeTextIndInfo_t;

/**
 * @brief The structure type containing the data objects for SEND DTMF COMMAND proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatDtmfStringInfo_t dtmfString; /**<	DTMF string	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatSendDtmfIndInfo_t;

/**
 * @brief The structure type containing the data objects for LANGUAGE NOTIFICATION proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatLanguageInfo_t language; /**<	Language info from the USIM application	*/
} TelSatLanguageNotificationIndInfo_t;

/**
 * @brief The structure type containing the data objects for LAUNCH BROWSER proactive command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. it includes command number, type, and qualifier	*/
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices	*/
	TelSatBrowserIdentitiesInfo_t browserId; /**< Browser identity	*/
	TelSatUrlInfo_t url; /**< URL */
	TelSatBearerInfo_t bearer; /**<	Bearer which is used by the browser	*/
	unsigned char provisioningRefCount; /**< Provisioning reference count */
	TelSatProvisioningRefInfo_t provisioningRef[TAPI_SAT_PROVISIONING_REF_MAX_COUNT]; /**< Provisioning reference data */
	TelSatTextTypeInfo_t text; /**<	Display text info */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatLaunchBrowserIndInfo_t;

/**
 * @brief The structure type containing the data objects for OPEN CHANNEL CSB proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatAddressInfo_t address; /**< Channel address */
	TelSatSubAddressInfo_t subAddress; /**<	Channel sub address	*/
	TelSatDurationInfo_t duration1; /**< Connection require time */
	TelSatDurationInfo_t duration2; /**< Connection require time 2 */
	TelSatBearerParametersCsdInfo_t bearerParamsCsd; /**< CSD info */
	TelSatBufferSizeInfo_t bufferSize; /**<	Buffer size	*/
	TelSatOtherAddressInfo_t otherAddress; /**<	Other address */
	TelSatTextTypeInfo_t userLogin; /**< User login	*/
	TelSatTextTypeInfo_t userPassword; /**<	User password */
	TelSatSimMeInterfaceTransportLevelInfo_t simMeInterfaceTransportLevel; /**<	SIM ME interface transport level */
	TelSatOtherAddressInfo_t dataDestinationAddress; /**< Data destination address	*/
} TelSatOpenChannelCsbInfo_t;

/**
 * @brief The structure type containing the data objects for OPEN CHANNEL (packet) proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBearerParametersGprsInfo_t bearerParamsGprs; /**<	GPRS info */
	TelSatBufferSizeInfo_t bufferSize; /**<	Buffer size	*/
	TelSatnetworkAccessNameInfo_t networkAccessName; /**< Network access name */
	TelSatOtherAddressInfo_t otherAddress; /**<	Other address */
	TelSatTextTypeInfo_t userLogin; /**< User login	*/
	TelSatTextTypeInfo_t userPassword; /**<	User password */
	TelSatSimMeInterfaceTransportLevelInfo_t simMeInterfaceTransportLevel; /**<	SIM ME interface transport level */
	TelSatOtherAddressInfo_t dataDestinationAddress; /**< Data destination address	*/
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatOpenChannelpdbInfo_t;

/**
 * @brief The structure type containing the data objects for OPEN CHANNEL LOCAL LINK proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatDurationInfo_t duration1; /**< Command execution time duration1 */
	TelSatDurationInfo_t duration2; /**< Command execution time duration2 */
	TelSatBearerParametersLocalLinksInfo_t bearerParamsLocalLinks; /**<	Local link info	*/
	TelSatBufferSizeInfo_t bufferSize; /**<	Buffer size	*/
	TelSatTextTypeInfo_t userPassword; /**<	User password */
	TelSatSimMeInterfaceTransportLevelInfo_t simMeInterfaceTransportLevel; /**<	SIM ME interface transport level */
	TelSatOtherAddressInfo_t dataDestinationAddress; /**< Data destination address */
	TelSatRemoteEntityAddressInfo_t remoteEntityAddress; /**< Remote entity address	*/
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatOpenChannelLocalBearerInfo_t;

/**
 * @brief The structure type containing the data objects for OPEN CHANNEL (DEFAULT BEARER) proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBufferSizeInfo_t bufferSize; /**<	Buffer size	*/
	TelSatOtherAddressInfo_t otherAddress; /**<	Other address */
	TelSatTextTypeInfo_t userLogin; /**< User login	*/
	TelSatTextTypeInfo_t userPassword; /**<	User password */
	TelSatSimMeInterfaceTransportLevelInfo_t simMeInterfaceTransportLevel;/**< SIM ME interface transport level	*/
	TelSatOtherAddressInfo_t dataDestinationAddress; /**< Data destination address */
} TelSatOpenChannelDefaultBearerInfo_t;

/**
 * @brief The structure type containing the data objects for OPEN CHANNEL (UICC Server Mode) proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatBufferSizeInfo_t bufferSize; /**<	Buffer size	*/
	TelSatSimMeInterfaceTransportLevelInfo_t simMeInterfaceTransportLevel; /**<	SIM ME interface transport level */
} TelSatOpenChannelUiccServerModeInfo_t;

/**
 * @brief The structure type containing the data objects for OPEN CHANNEL proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	int bIsUiccServerMode; /**<	Flag that indicates whether UICC server mode is active */
	TelSatBearerDescType_t bearerType; /**<	Bearer destination type	*/
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	union {
		TelSatOpenChannelCsbInfo_t csBearer; /**< CS info */
		TelSatOpenChannelpdbInfo_t pdBearer; /**< PBD info */
		TelSatOpenChannelLocalBearerInfo_t locBearer; /**< Local link info */
		TelSatOpenChannelDefaultBearerInfo_t defaultBearer; /**< Default bearer	*/
		TelSatOpenChannelUiccServerModeInfo_t uiccServerMode; /**< UICC server mode	*/
	} details; /**< Open Channel Details */
} TelSatOpenChannelIndInfo_t;

/**
 * @brief The structure type containing the data objects for CLOSE CHANNEL proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatTextAttributeInfo_t textAttribute; /**< Text attribute info - e.g. bold, center align	*/
} TelSatCloseChannelIndInfo_t;

/**
 * @brief The structure type containing the data objects for RECEIVE DATA proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatChannelDataLengthInfo_t channelDataLen; /**< Channel data length */
} TelSatReceiveDataIndInfo_t;

/**
 * @brief The structure type containing the data objects for SEND DATA proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
	TelSatAlphaIdentifierInfo_t alphaId; /**< Alpha identifier(string) info	*/
	TelSatIconIdentifierInfo_t iconId; /**<	Icon identifier info */
	TelSatChannelDataInfo_t channel_data; /**< Channel data for sending	*/
} TelSatSendDataIndInfo_t;

/**
 * @brief The structure type containing the data objects for GET CHANNEL STATUS proactive command indication data.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandDetailInfo_t commandDetail; /**< Command detail info. It includes command number, type, and qualifier */
	TelSatDeviceIdentitiesInfo_t deviceIdentities; /**<	Device identities info. It includes source and destination devices */
} TelSatGetChannelStatusIndInfo_t;

/**
 * @brief The structure type containing the data object for END PROACTIVE SESSION command indication.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatCommandType_t commandType; /**<	proactive command type	*/
} TelSatEndProactiveSessionIndInfo_t;

/**
 * @brief The structure type containing the data objects for PROACTIVE command indication union data.
 * @since_tizen 2.3
 */
typedef struct {
	union {
		TelSatMoreTimeIndInfo_t moreTime;	/**< TBD */
		TelSatDisplayTextIndInfo_t displayText; /**< Parsed proactive command info from TLV to Telephony data type - display text */
		TelSatGetInkeyIndInfo_t getInkey; /**< Parsed proactive command info from TLV to Telephony data type - getInkey	*/
		TelSatGetInputIndInfo_t getInput; /**< Parsed proactive command info from TLV to Telephony data type - getInput	*/
		TelSatPlayToneIndInfo_t playTone; /**< Parsed proactive command info from TLV to Telephony data type - play tone */
		TelSatSetupMenuIndInfo_t setupMenu; /**< Parsed proactive command info from TLV to Telephony data type - setup menu	*/
		TelSatSelectItemIndInfo_t selectItem; /**< Parsed proactive command info from TLV to Telephony data type - select item */
		TelSatSendSmsIndInfo_t sendSms; /**< Parsed proactive command info from TLV to Telephony data type - send SMS */
		TelSatSendSsIndInfo_t sendSs; /**< Parsed proactive command info from TLV to Telephony data type - send SS */
		TelSatSendUssdIndInfo_t sendUssd; /**< Parsed proactive command info from TLV to Telephony data type - send USSD */
		TelSatSetupCallIndInfo_t setupCall; /**< Parsed proactive command info from TLV to Telephony data type - setup call	*/
		TelSatRefreshIndInfo_t refresh; /**< Parsed proactive command info from TLV to Telephony data type - refresh */
		TelSatProvideLocalInfoIndInfo_t provideLocInfo; /**< Parsed proactive command info from TLV to Telephony data type - provide local info	*/
		TelSatLaunchBrowserIndInfo_t launchBrowser; /**< Parsed proactive command info from TLV to Telephony data type - launch browser	*/
		TelSatSetupIdleModeTextIndInfo_t idleText; /**<	Parsed proactive command info from TLV to Telephony data type - setup idle mode text */
		TelSatSendDtmfIndInfo_t sendDtmf; /**< Parsed proactive command info from TLV to Telephony data type - send DTMF */
		TelSatLanguageNotificationIndInfo_t languageNotification;/**< Parsed proactive command info from TLV to Telephony data type - language notification	*/
		TelSatSetupEventListIndInfo_t setupEventList; /**< Parsed proactive command info from TLV to Telephony data type - setup event list	*/
		TelSatOpenChannelIndInfo_t openChannel; /**< Parsed proactive command info from TLV to Telephony data type - open channel */
		TelSatCloseChannelIndInfo_t closeChannel; /**< Parsed proactive command info from TLV to Telephony data type - close channel */
		TelSatReceiveDataIndInfo_t receiveData; /**< Parsed proactive command info from TLV to Telephony data type - receive data */
		TelSatSendDataIndInfo_t sendData; /**< Parsed proactive command info from TLV to Telephony data type - send data */
		TelSatGetChannelStatusIndInfo_t getChannelStatus; /**< Parsed proactive command info from TLV to Telephony data type - get channel status */
	} cmdInfo; /**<	Union	*/
} TelSatProactiveCmdData_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the DISPLAY TEXT proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatDisplayTextRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the GET INKEY proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatTextTypeInfo_t text; /**<	Inserted key info */
	TelSatDurationInfo_t duration;	/**< TBD */
} TelSatGetInkeyRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the PLAY TONE proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatPlayToneRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the MORE TIME proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that whether the current proactive command request is executed successfully */
} TelSatMoreTimeRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SETUP MENU proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatSetupMenuRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the GET INPUT proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatTextTypeInfo_t text; /**<	Inserted string info */
} TelSatGetInputRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SELECT ITEM proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	unsigned char itemIdentifier; /**< Item identifier */
} TelSatSelectItemRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the PROVIDE LOCAL INFORMATION proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatCmdQualiProvideLocalInfo_t infoType; /**<	Local info type - e.g. time zone or language info, and so on */
	union {
		TelSatDataTimeZoneInfo_t timeZoneInfo; /**<	Current time zone info */
		TelSatLanguageInfo_t languageInfo; /**<	Current ME language setting info */
	} u; /**<	Union	*/
} TelSatProvideLocalInfoRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SETUP EVENT LIST proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatSetupEventListRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SEND SMS proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatCallCtrlProblemType_t additionalCallCtrlProblemInfo; /**<	Call control problem */
} TelSatSendSmsRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SET UP CALL proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatNetworkProblemType_t networkProblem; /**<	Network problem during setup call */
	TelSatCallCtrlProblemType_t permanentCallCtrlProblem; /**<	Permanent call control problem	*/
	TelSatCallCtrlRequestedActionInfo_t callCtrlRequestedAction; /**< Call control requested action info */
	TelSatResultInfo_t result2; /**< Additional response on general result */
	TelSatTextTypeInfo_t text; /**<	Text string info */
	int bIsTapiCause; /**< Flag to check whether tapi causes problems */
	unsigned long tapiCause; /**< Tapi call level cause */
	unsigned long tapiSsCause; /**< Tapi SS level cause */
} TelSatSetupCallRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SEND SS proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatSsProblemType_t additionalSsProblemInfo; /**<	Additional SS problem */
	TelSatCallCtrlProblemType_t additionalCallCtrlProblemInfo; /**<	Additional call control problem	*/
	TelSatCallCtrlRequestedActionInfo_t callCtrlRequestedAction; /**< Call control requested action info */
	TelSatResultInfo_t result2; /**< Additional response on general result */
	TelSatTextTypeInfo_t text; /**< Text string info */
} TelSatSendSsRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SEND USSD proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatUssdProblemType_t additionalUssdProblemInfo; /**<	Additional USSD problem	*/
	TelSatCallCtrlProblemType_t additionalCallCtrlProblemInfo; /**<	Additional call control problem	*/
	int bCallCtrlHasModification; /**< Flag to check whether modification happens during call control */
	TelSatTextTypeInfo_t text; /**<	Text string info */
	TelSatCallCtrlRequestedActionInfo_t callCtrlRequestedAction; /**< Call control requested action info */
	TelSatResultInfo_t result2; /**< Additional response on general result */
	TelSatTextTypeInfo_t text2; /**< Text string info */
} TelSatSendUssdRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the REFRESH proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatRefreshRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the GET CHANNEL STATUS proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatBipProblemType_t additionalProblemInfo; /**< BIP specific problem info */
	TelSatChannelStatusInfo_t channelStatus; /**< Channel Status */
} TelSatGetChannelStatusRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the CLOSE CHANNEL proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatBipProblemType_t additionalProblemInfo; /**< BIP specific problem info */
} TelSatCloseChannelRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the OPEN CHANNEL proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatBearerDescriptionInfo_t bearerDescription; /**< Bearer description */
	TelSatBipProblemType_t additionalProblemInfo; /**< BIP specific problem info */
	TelSatChannelStatusInfo_t channelStatus; /**< Channel status */
	TelSatBufferSizeInfo_t bufferSize; /**< Buffer size */
} TelSatOpenChannelRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the LANGAUGE NOTIFICATION proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatLanguageNotificationRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the LAUNCH BROWSER proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatLaunchBrowserProblemType_t additionalProblemInfo; /**< Browser specific problem info */
} TelSatLaunchBrowserRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the RECEIVE DATA proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatBipProblemType_t additionalProblemInfo; /**< BIP specific problem info */
	int bOtherInfo; /**< Flag to check whether other information is required */
	TelSatChannelDataInfo_t channel_data; /**< Channel data	*/
	unsigned char channelDataLen; /**< Channel data length */
} TelSatReceiveDataRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SEND DATA proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
	TelSatBipProblemType_t additionalProblemInfo; /**< BIP specific problem info */
	unsigned char channelDataLen; /**< Channel data length */
} TelSatSendDataRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SETUP IDLE MODE TEXT proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatSetupIdlemodeTextRespInfo_t;

/**
 * @brief The structure type containing the data objects for the Terminal Response of the SEND DTMF proactive command.
 * @since_tizen 2.3
 */
typedef struct {
	TelSatResultInfo_t result; /**<	Result that indicates whether the current proactive command request is executed successfully */
} TelSatSendDtmfRespInfo_t;

/**
 * @brief The structure type containing the data structures to be used to send the proactive command response.
 * @since_tizen 2.3
 */
typedef struct {
	unsigned char commandNumber; /**< Proactive command number */
	TelSatCommandType_t commandType; /**< Proactive command type */
	union {
		TelSatMoreTimeRespInfo_t moreTime;	/**< TBD */
		TelSatDisplayTextRespInfo_t displayText; /**< Terminal response info from the displayText proactive command	*/
		TelSatGetInkeyRespInfo_t getInkey; /**<	Terminal response info from the getInkey proactive command */
		TelSatGetInputRespInfo_t getInput; /**<	Terminal response info from the getInput proactive command */
		TelSatPlayToneRespInfo_t playTone; /**<	Terminal response info from the playTone proactive command */
		TelSatSetupMenuRespInfo_t setupMenu; /**< Terminal response info from the setupMenu proactive command */
		TelSatSelectItemRespInfo_t selectItem; /**<	Terminal response info from the selectItem proactive command */
		TelSatSendSmsRespInfo_t sendSms; /**< Terminal response info from the sendSms proactive command	*/
		TelSatSendSsRespInfo_t sendSs; /**< Terminal response info from the sendSs proactive command */
		TelSatSendUssdRespInfo_t sendUssd; /**< Terminal response info from the sendUssd proactive command */
		TelSatSetupCallRespInfo_t setupCall; /**< Terminal response info from the setupCall proactive command */
		TelSatRefreshRespInfo_t refresh; /**< Terminal response info from the refresh proactive command	*/
		TelSatProvideLocalInfoRespInfo_t provideLocInfo; /**< Terminal response info from the provide Local Info proactive command */
		TelSatLaunchBrowserRespInfo_t launchBrowser; /**< Terminal response info from the launch Browser proactive command */
		TelSatSetupIdlemodeTextRespInfo_t idleText; /**< Terminal response info from the setup idle mode text proactive command	*/
		TelSatSendDtmfRespInfo_t sendDtmf; /**<	Terminal response info from the send DTMF proactive command	*/
		TelSatLanguageNotificationRespInfo_t languageNotification; /**<	Terminal response info from the language Notification proactive command	*/
		TelSatSetupEventListRespInfo_t setupEventList; /**<	Terminal response info from the setup Event List proactive command */
		TelSatOpenChannelRespInfo_t openChannel; /**< Terminal response info from the openChannel proactive command	*/
	} terminalRespInfo; /**< Union */
} TelSatRespInfo_t;

/*
 * SAT Icon Data
 */

typedef struct {
	unsigned char iconId;	/**< TBD */
	unsigned char imgType;	/**< TBD */
	unsigned char imgLen;	/**< TBD */
	unsigned char imgData[256];	/**< TBD */
} TelSatIconDataResp_t;

typedef struct {
	unsigned char iconId;	/**< TBD */
	unsigned char imgType;	/**< TBD */
	unsigned char fileId[2];	/**< TBD */
	unsigned char reqDataLen[2];	/**< TBD */
	unsigned char offset[2];	/**< TBD */
} TelsatIconDataGet_t;

#ifdef __cplusplus
}
#endif

#endif	/* _TEL_SAT_PROACTV_CMD_H_ */

/**
 * @}
 */
