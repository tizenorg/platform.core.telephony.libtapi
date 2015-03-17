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
 * @file TelSatEnvelope.h
 */

/**
 * @internal
 * @addtogroup CAPI_TELEPHONY_SERVICE_SAT
 * @{
 */

#ifndef _TEL_SAT_ENVELOPE_H_
#define _TEL_SAT_ENVELOPE_H_

#include <TelSatObj.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Enumeration for the SAT call type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_CALL_TYPE_MO_VOICE = 0X00,	/**< Call type - MO voice */
	TAPI_SAT_CALL_TYPE_MO_SMS,			/**< Call type - MO SMS */
	TAPI_SAT_CALL_TYPE_SS,				/**< Call type - SS */
	TAPI_SAT_CALL_TYPE_USSD,			/**< Call type - USSD */
	TAPI_SAT_PDP_CNTXT_ACT,				/**< Call type - PDP context action */
	TAPI_SAT_CALL_TYPE_MAX				/**< Call type - max */
}TelSatCallType_t;

/**
 * @brief Enumeration for the result of call control by SIM.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_CALL_CTRL_R_ALLOWED_NO_MOD			= 0,		/**< Call control result type -  ALLOWED WITH NO MOD */
	TAPI_SAT_CALL_CTRL_R_NOT_ALLOWED			= 1,		/**< Call control result type -  NOT ALLOWED */
	TAPI_SAT_CALL_CTRL_R_ALLOWED_WITH_MOD		= 2,		/**< Call control result type -  ALLOWED WITH MOD */
	TAPI_SAT_CALL_CTRL_R_RESERVED				= 0xFF		/**< Call control result type -  RESERVED */

} TelSatCallCtrlResultType_t;

/**
 * @brief Enumeration for the general result of sending an envelope command to the USIM.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_ENVELOPE_SUCCESS,	/**< Envelope result - success */
	TAPI_SAT_ENVELOPE_SIM_BUSY,	/**< Envelope result - USIM busy */
	TAPI_SAT_ENVELOPE_FAILED	/**< Envelope result - failed */

}TelSatEnvelopeResp_t;

//	8.	MENU SELECTION
/**
 * @brief The structure type containing data objects for the MENU SELECTION envelope.
 * @since_tizen 2.3
 */
typedef struct
{
	unsigned char		itemIdentifier;		/**< Menu selection item identifier	*/
	int				bIsHelpRequested;	/**< Flag to check whether help information is required */
} TelSatMenuSelectionReqInfo_t;

//	9.1 CALL CONTROL BY SIM
/**
 * @brief Structure type containing data objects for Call Control result data sent by the USIM.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatAddressInfo_t				address;			/**< Call number */
	TelSatSubAddressInfo_t			subAddress;			/**< Call number sub address */
	TelSatBcRepeatIndicatorType_t	bcRepeatIndicator;	/**< BC repeat indicator */
	TelSatCapaConfigParamInfo_t		ccp1;				/**< Capability configuration parameter 1 */
	TelSatCapaConfigParamInfo_t		ccp2;				/**< Capability configuration parameter 2 */
}TelSatVoiceCallCtrlIndInfo_t;

/**
 * @brief The structure type containing SAT SS control result data sent by the USIM.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatSsStringInfo_t			ssString;			/**< SS number */
	TelSatSubAddressInfo_t			subAddress;			/**< SS sub address */
	TelSatBcRepeatIndicatorType_t	bcRepeatIndicator;	/**< BC repeat indicator */
	TelSatCapaConfigParamInfo_t		ccp1;				/**< Capability configuration parameter 1 */
	TelSatCapaConfigParamInfo_t		ccp2;				/**< Capability configuration parameter 2 */
}TelSatSsCtrlIndInfo_t;

/**
 * @brief The structure type containing SAT MO SMS control configuration data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatAddressInfo_t		rpDestAddress;	/**< SMS control RP destination address	*/
	TelSatAddressInfo_t		tpDestAddress;	/**< SMS control TP destination address	*/
} TelSatMoSmsCtrlIndInfo_t;

/**
 * @brief The structure type containing SAT call control configuration data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatCallType_t					callType;		/**< Call type */
	TelSatCallCtrlResultType_t			callCtrlResult;	/**< Call control result */
	TelSatAlphaIdentifierInfo_t			alphaIdentifier;/**< Alpha identifier */
	unsigned char						callId;			/**< Call ID */
	TelSatCallType_t					oldCallType;	/**< Old call type */
	union
	{
		TelSatVoiceCallCtrlIndInfo_t	voiceCallData;	/**< Voice call control data */
		TelSatSsCtrlIndInfo_t			ssData;			/**< SS control data */
		TelSatMoSmsCtrlIndInfo_t		smsData;
	}u;													/**< Union */
} TelSatCallCtrlIndInfo_t;

//	9.2 MO SHORT MESSAGE CONTROL BY SIM RESULT
/**
 * @brief The structure type containing SAT MO SS control request data.
 * @since_tizen 2.3
 */
 typedef struct
{
	TelSatCallCtrlResultType_t		callCtrlResult;		/**< Call control result */
	TelSatAlphaIdentifierInfo_t		alphaIdentifier;	/**< Alpha identifier */
	TelSatMoSmsCtrlIndInfo_t		smsData;			/**< SMS control data */
}TelSatMoSMCtrlResult_t;

//	11.5 EVENT DOWNLOAD - USER ACTIVITY EVENT
/**
 * @brief The structure type containing SAT user activity event request data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatEventListInfo_t				eventList;			/**< Event List	*/
	TelSatDeviceIdentitiesInfo_t		deviceIdentities;	/**< Device identities info	*/

} TelSatUserActivityEventReqInfo_t;

//	11.6 EVENT DOWNLOAD - IDLE SCREEN AVAILABLE EVENT
/**
 * @brief The structure type containing the data objects for the IDLE SCREEN AVAILABLE event download.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatEventDownloadType_t	eventData;	/**< Event type	*/
} TelSatIdleScreenAvailableEventReqInfo_t;

//	11.8 EVENT DOWNLOAD - LANGUAGE SELECTION EVENT
/**
 * @brief The structure type containing the data objects for the LANGUAGE SELECTION event download.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatLanguageType_t	 language;		/**< Selected language info	*/
} TelSatLanguageSelectionEventReqInfo_t;

//	11.9 EVENT DOWNLOAD - BROWSER TERMINATION EVENT
/**
 * @brief The structure type containing the data objects for the BROWSER TERMINATION event download.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatBrowserTerminationCauseType_t		browserTerminationCause;	/**< Browser Termination Cause */
} TelSatBrowserTerminationEventReqInfo_t;

//	11.10 EVENT DOWNLOAD - DATA AVAILABLE EVENT
/**
 * @brief The structure type containing SAT data available event request data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatEventListInfo_t			eventList;			/**< Event List	*/
	TelSatDeviceIdentitiesInfo_t	deviceIdentities;	/**< Device identities info	*/
	TelSatChannelStatusInfo_t		channelStatus;		/**< Channel status	*/
	TelSatChannelDataLengthInfo_t	channelDataLen;		/**< Channel data length */

} TelSatDataAvailableEventReqInfo_t;

//	11.11 EVENT DOWNLOAD - CHANNEL STATUS EVENT
/**
 * @brief The structure type containing SAT channel status even request data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatEventListInfo_t			eventList;			/**< Event list	*/
	TelSatDeviceIdentitiesInfo_t	deviceIdentities;	/**< Device identities info	*/
	TelSatChannelStatusInfo_t		channelStatus;		/**< Channel Status	*/
} TelSatChannelStatusEventReqInfo_t;

#ifdef __cplusplus
}
#endif

#endif	/* _TEL_SAT_ENVELOPE_H_ */

/**
 * @}
 */
