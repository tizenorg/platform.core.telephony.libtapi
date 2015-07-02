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
 * @file TelSat.h
 */

/**
 * @addtogroup CAPI_TELEPHONY_SERVICE_SAT
 * @{
 */

#ifndef _TEL_SAT_H_
#define _TEL_SAT_H_

#include <TelCall.h>
#include <TelSs.h>
#include <TelSatObj.h>
#include <TelSatProactvCmd.h>
#include <TelSatEnvelope.h>
#include <TelSim.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Definition for the maximum length of a menu title.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DEF_TITLE_LEN_MAX			255

/**
 * @brief Definition for the maximum length of a menu item.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DEF_ITEM_STR_LEN_MAX		50

/**
 * @brief Definition for the maximum length of a text string.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DEF_TEXT_STRING_LEN_MAX	500

/**
 * @brief Definition for the bit mask of a contact.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DEF_BIT_MASK_CONTACT		0x01

/**
 * @brief Definition for the bit mask of a message.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DEF_BIT_MASK_MSG			0x02

/**
 * @brief Definition for the bit mask of others.
 * @since_tizen 2.3
 */
#define TAPI_SAT_DEF_BIT_MASK_OTHER			0x04

/**
 * @brief Definition for the refresh file list.
 * @since_tizen 2.3
 */
#define TAPI_SAT_REFRESH_FILE_LIST			20
#define TAPI_SAT_DEF_SS_LEN_MAX             250
#define TAPI_SAT_DEF_USSD_LEN_MAX           250

//	Telephony UI USER CONFIRM TYPE
/**
 * @brief Enumeration for the UI user confirm type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_USER_CONFIRM_YES,			/**< Informs that the user confirms yes */
	TAPI_SAT_USER_CONFIRM_NO_OR_CANCEL,	/**< Informs that the user confirms no/cancel */
	TAPI_SAT_USER_CONFIRM_HELP_INFO,	/**< Informs that the user wants help information */
	TAPI_SAT_USER_CONFIRM_END,			/**< Informs that the user confirms end */
	TAPI_SAT_USER_CONFIRM_TIMEOUT,		/**< Informs that the user did not respond */
}TelSatUiUserConfirmType_t;

//	Telephony UI INKEY TYPE
/**
 * @brief Enumeration for the UI inkey type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_INKEY_CONFIRM_NO				= 0x00,		/**< No */
	TAPI_SAT_INKEY_CONFIRM_YES				= 0x01,		/**< Yes */
}TelSatUiInkeyYesNoCaseType_t;

//	Telephony UI DISPLAY STATUS
/**
 * @brief Enumeration for the UI display status.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_DISPLAY_SUCCESS				= 0x00,		/**< Informs about UI display success */
	TAPI_SAT_DISPLAY_FAIL					= 0x01,		/**< Informs about UI display failure */
}TelSatUiDisplayStatusType_t;

//	TELEPHONY REFRESH APPLICATION TYPE
/**
 * @brief Enumeration for the refresh application type.
 * @since_tizen 2.3
 */
 typedef enum
{
	TAPI_SAT_REFRESH_CONTACT = 0x00,				/**< Refresh application type - Phonebook */
	TAPI_SAT_REFRESH_MSG,							/**< Refresh application type - SMS */
	TAPI_SAT_REFRESH_OTHER,							/**< Refresh application type - Other */
	TAPI_SAT_REFRESH_MAX,							/**< Maximum Enumeration Value */
}TelSatRefreshAppType_t;

//	Telephony COMMAND PERFORMED RESULT
/**
 * @brief Enumeration for the result of proactive command execution.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_CMD_PERFORM_SUCCESS = 0x00,		/**< Command performed successfully */
	TAPI_SAT_CMD_PERFORM_FAIL,					/**< Command execution failed */
	TAPI_SAT_CMD_PERFORM_MAX,					/**< Maximum Enumeration Value */
}TelSatCommandPerformResultType_t;

//	Telephony CALL CONTROL TYPE
/**
 * @brief Enumeration for the call control type.
 * @since_tizen 2.3
 */
typedef enum
{
	TAPI_SAT_CALL_CNF_NONE	= 0x00,		/**< Call control confirm type - None */
	TAPI_SAT_CALL_CNF_CALL,			    /**< Call control confirm type - Call */
	TAPI_SAT_CALL_CNF_SS,				/**< Call control confirm type - SS */
	TAPI_SAT_CALL_CNF_USSD,				/**< Call control confirm type - USSD */
	TAPI_SAT_CALL_CNF_MAX,				/**< Maximum Enumeration Value */
}TelSatCallCtrlType_t;

//	SAT UI USER CONFIRMATION INFO
/**
 * @brief The structure type defining user confirmation data.
 * @since_tizen 2.3
 */
typedef struct
{
	int						commandId;				/**< Proactive Command Number sent by USIM */
	TelSatCommandType_t		commandType;			/**< Proactive Command Type */
	TelSatUiUserConfirmType_t	keyType;			/**< User Response Type */
	unsigned char*				pAdditionalData;	/**< Additional Data */
	int							dataLen;			/**< Additional Data Length */
}TelSatUiUserConfirmInfo_t;

//	Telephony TEXT INFO
/**
 * @brief The structure type defining character data for the SAT engine data structure.
 * @since_tizen 2.3
 */
typedef struct
{
	unsigned short	stringLen;										/**< Character data length */
	unsigned char		string[TAPI_SAT_DEF_TEXT_STRING_LEN_MAX+1];	/**< Character data */
}TelSatTextInfo_t;

//	Telephony MAIN MENU TITLE INFO
/**
 * @brief The structure type defining the main menu title to check the SAT menu.
 * @since_tizen 2.3
 */
typedef struct
{
	int				bIsMainMenuPresent;		/**< Flag to check SAT main menu existence */
	TelSatTextInfo_t	mainMenuTitle;		/**< Main menu title data */
}TelSatMainMenuTitleInfo_t;

//	Telephony DISPLAY TEXT DATA
/**
 * @brief The structure type defining the display text proactive command for SAT UI.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;					/**< Proactive Command Number sent by USIM */
	TelSatTextInfo_t				text;					/**< Character data to display on screen */
	unsigned int					duration;				/**< The duration of the display */
	int							bIsPriorityHigh;			/**< Flag that indicates whether text is to be displayed if some other app is using the screen */
	int							bIsUserRespRequired;		/**< Flag that indicates whether user response is required */
	int							b_immediately_resp;		    /**< TBD */
	TelSatIconIdentifierInfo_t	iconId;						/**< Icon Identifier */
}TelSatDisplayTextInd_t;

//	Telephony INKEY DATA
/**
 * @brief The structure type defining get inkey proactive command data for SAT UI.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;				/**< Proactive Command Number sent by USIM */
	TelSatInkeyType_t				keyType;			/**< Input Type: Character Set or Yes/No */
	TelSatUseInputAlphabetType_t	inputCharMode;		/**< Input character mode(SMS default, UCS2) */
	int							bIsNumeric;				/**< Is input character numeric(0-9, *, # and +) */
	int							bIsHelpInfoAvailable;	/**< Help info request flag */
	TelSatTextInfo_t				text;				/**< Character data to display on the screen */
	unsigned int					duration;			/**< Duration of the display */
	TelSatIconIdentifierInfo_t		iconId;				/**< Icon Identifier */
}TelSatGetInkeyInd_t;

//	Telephony GET INPUT DATA
/**
 * @brief The structure type defining get input proactive command data for SAT UI.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;				/**< Proactive Command Number sent by USIM */
	TelSatUseInputAlphabetType_t	inputCharMode;		/**< Input character mode(SMS default, UCS2) */
	int							bIsNumeric;				/**< Is input character numeric(0-9, *, # and +) */
	int							bIsHelpInfoAvailable;	/**< Help info request flag */
	int							bIsEchoInput;			/**< Flag that indicates whether to show input data on the screen */
	TelSatTextInfo_t				text;				/**< Character data to display on the screen*/
	TelSatRespLenInfo_t				respLen;			/**< Input data min, max length */
	TelSatTextInfo_t				defaultText;		/**< Default input character data */
	TelSatIconIdentifierInfo_t		iconId;				/**< Icon Identifier */
}TelSatGetInputInd_t;

//	Telephony PLAY TONE DATA
/**
 * @brief The structure type defining play tone proactive command data for an application.
 * @since_tizen 2.3
 */
typedef struct
{
	int						commandId;	    /**< Proactive Command Number sent by USIM */
	TelSatTextInfo_t			text;		/**< Character data to display on the screen */
	TelSatToneInfo_t			tone;		/**< Tone info */
	unsigned int				duration;	/**< Duration for playing the tone */
	TelSatIconIdentifierInfo_t	iconId;		/**< Icon Identifier */
}TelSatPlayToneInd_t;

//	Telephony UI info
/**
 * @brief The structure type defining data for SAT UI.
 * @since_tizen 2.3
 */
typedef struct
{
	int						commandId;		/**< Proactive Command Number sent by USIM */
	int						user_confirm;	/**< TBD */
	TelSatTextInfo_t			text;		/**< Character data to display on the screen */
	TelSatIconIdentifierInfo_t	iconId;		/**< Icon Identifier */
}TelSatSendUiInfo_t;

//	Telephony MENU ITEM DATA FOR SETUP MENU
/**
 * @brief The structure type defining menu item info for the setup menu.
 * @since_tizen 2.3
 */
typedef struct
{
	char itemString[TAPI_SAT_DEF_ITEM_STR_LEN_MAX + 6];	/**< Menu item character data */
	char itemId;										/**< Identifies the item on the menu that the user selected */
}TelSatMenuInfo_t;

//	Telephony SETUP MENU INFO
/**
 * @brief The structure type defining SAT main menu info.
 * @since_tizen 2.3
 */
typedef struct
{
	int commandId;	/**< Proactive Command Number sent by USIM */
	int	bIsMainMenuPresent;		/**< TBD */
	char satMainTitle[TAPI_SAT_DEF_TITLE_LEN_MAX + 1];	/**< Menu title text */
	TelSatMenuInfo_t satMainMenuItem[TAPI_SAT_MENU_ITEM_COUNT_MAX];	/**< Menu items */
	unsigned short satMainMenuNum;	/**< Number of menu items */
	int	bIsSatMainMenuHelpInfo;	/**< Flag for a help information request */
	int	bIsUpdatedSatMainMenu;		/**< TBD */
	TelSatIconIdentifierInfo_t iconId;	/**< Icon Identifier */
	TelSatIconIdentifierListInfo_t iconIdList;	/**< List of Icon Identifiers */
}TelSatSetupMenuInfo_t;

//	Telephony SELECT ITEM DATA
/**
 * @brief The structure type defining select item proactive command data for SAT UI.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;									    /**< Proactive Command Number sent by USIM */
	int							bIsHelpInfoAvailable;						    /**< Flag for a help information request */
	TelSatTextInfo_t				text;									    /**< Menu title text */
	char							defaultItemIndex;							/**< Selected default item - default value is @c 0 */
	char							menuItemCount;								/**< Number of menu items */
	TelSatMenuItemInfo_t			menuItem[TAPI_SAT_MENU_ITEM_COUNT_MAX];		/**< Menu items */
	TelSatIconIdentifierInfo_t		iconId;										/**< Icon Identifier */
	TelSatIconIdentifierListInfo_t	iconIdList;									/**< List of Icon Identifiers */

}TelSatSelectItemInd_t;

//	Telephony IDLE MODE TEXT DATA
/**
 * @brief The structure type defining the setup idle mode text proactive command for the idle application.
 * @since_tizen 2.3
 */
typedef struct
{
	int						commandId;			/**< Proactive Command Number sent by USIM */
	TelSatTextInfo_t			text;			/**< Character data to display on the screen */
	TelSatIconIdentifierInfo_t	iconId;		    /**< TBD */
} TelSatSetupIdleModeTextInd_t;

//	Telephony REFRESH DATA
/**
 * @brief The structure type defining refresh proactive command data for SAT UI.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;			/**< Proactive Command Number sent by USIM */
	unsigned int				duration;			/**< Duration of the display */
	TelSatCmdQualiRefresh_t		refreshType;		/**< Refresh mode */
}TelSatRefreshIndUiInfo_t;

//	Telephony REFRESH DATA
/**
 * @brief The structure type defining refresh proactive command data for applications that are concerned with files residing on USIM.
 * @since_tizen 2.3
 */
typedef struct
{
	int						commandId;								    /**< Proactive Command Number sent by USIM */
	TelSatRefreshAppType_t		appType;								/**< Concerned application type */
	TelSatCmdQualiRefresh_t		refreshMode;							/**< Refresh mode */
	unsigned char					fileCount;							/**< Refresh file count */
	TelSimFileID_t				fileId[TAPI_SAT_REFRESH_FILE_LIST];		/**< Refresh file identifier */
}TelSatRefreshInd_t;

//	Telephony END PROACTIVE COMMAND INFO
/**
 * @brief The structure type defining data objects to indicate to SAT UI, the end of execution of a specific proactive command by another application.
 * @since_tizen 2.3
 */
typedef struct
{
	int									commandId;			/**< Proactive Command Number sent by USIM */
	char									commandType;	/**< Command Type */
	TelSatCommandPerformResultType_t		result;			/**< Result of command execution by the application */
}TelSatProactiveCmdEndIndInfo_t;

//	Telephony SEND SMS DATA
/**
 * @brief The structure type defining send SMS proactive command data for the SMS application.
 * @since_tizen 2.3
 */
typedef struct
{
	int					commandId;				/**< Proactive Command Number sent by USIM */
	int					bIsPackingRequired;		/**< Flag to check if packing is required for SMS TPDU */
	TelSatAddressInfo_t	address;				/**< Destination address */
	TelSatSmsTpduInfo_t	smsTpdu;				/**< SMS TPDU data */
} TelSatSendSmsIndSmsData_t;

//	Telephony SEND SS DATA
/**
 * @brief The structure type defining send SS proactive command data for the SS application.
 * @since_tizen 2.3
 */
typedef struct
{
	int					commandId;		/**< Proactive Command Number sent by USIM */
	TelSimTypeOfNum_t ton;              /**< Type of number */
	TelSimNumberingPlanIdentity_t npi;  /**< Number plan identity */
	unsigned short		ssStringLen;	/**< TBD */
	unsigned char		ssString[TAPI_SAT_DEF_SS_LEN_MAX+1]; /**< TBD */
}TelSatSendSsIndSsData_t;

//	Telephony USSD DATA
/**
 * @brief The structure type defining send USSD proactive command data for the USSD application.
 * @since_tizen 2.3
 */
typedef struct
{
	int			commandId;		            /**< Proactive Command Number sent by USIM */
	unsigned char		rawDcs;			    /**< Data coding scheme */
	unsigned short		ussdStringLen;		/**< TBD */
	unsigned char		ussdString[TAPI_SAT_DEF_USSD_LEN_MAX+1];		/**< TBD */
}TelSatSendUssdIndUssdData_t;

//	Telephony SEND DTMF DATA
/**
 * @brief The structure type defining send DTMF proactive command data for the DTMF application.
 * @since_tizen 2.3
 */
typedef struct
{
	int					commandId;			/**< Proactive Command Number sent by USIM */
	int					bIsHiddenMode;		/**< Hidden mode flag */
	TelSatTextInfo_t		dtmfString;		/**< DTMF string data */
}TelSatSendDtmfIndDtmfData_t;

//	Telephony SETUP CALL  DATA
/**
 * @brief The structure type defining setup call proactive command data for the call application.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;		    /**< Proactive Command Number sent by USIM */
	TelSatCmdQualiSetupCall_t		calltype;		/**< Call type */
	TelSatTextInfo_t				dispText;		/**< Display data for calling */
	TelSatTextInfo_t				callNumber;		/**< Call number */
	unsigned int					duration;		/**< Maximum repeat duration */
	TelSatIconIdentifierInfo_t		iconId;			/**< Icon identifier for the call application */
}TelSatSetupCallIndCallData_t;

//	Telephony LAUNCH BROWSER DATA
/**
 * @brief The structure type defining launch browser proactive command data for the browser application.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;		/**< Proactive Command Number sent by USIM */
	TelSatUrlInfo_t					url;		/**< URL to connect */
	TelSatCmdQualiLaunchBrowser_t	launchType;	/**< Launch type */
	TelSatBrowserIdentityType_t	IdentityType;	/**< Browser Identity - default, HTML, and so on */
}TelSatLaunchBrowserIndBrowserData_t;

//	Telephony PROVIDE LOCAL INFO DATA
/**
 * @brief The structure type defining provide local info proactive command data for the application.
 * @since_tizen 2.3
 */
typedef struct
{
	int commandId;										/**< Proactive Command Number sent by USIM */
	TelSatCmdQualiProvideLocalInfo_t localInfoType;		/**< Provide Local Information Type */
}TelSatProvideLocalInfoInd_t;

//	Telephony LANGUAGE NOTIFICATION DATA
/**
 * @brief The structure type defining language notification proactive command data for the application.
 * @since_tizen 2.3
 */
typedef struct
{
	int							commandId;						/**< Proactive Command Number sent by USIM */
	int							bSpecificLanguageNotification;  /**< Flag for checking a specific language notification. If FALSE, non-specific language notification */
	TelSatLanguageInfo_t			language;					/**< Language info from the USIM application */
}TelSatLanguageNotiInfoInd_t;

//	Telephony PLAY TONE RETURN INFO
/**
 * @brief The structure type defining result data for the Play Tone proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t	resp;			/**< Result response value */
}TelSatSetupMenuRetInfo_t;

//	Telephony REFRESH RETURN INFO
/**
 * @brief The structure type defining result data for the refresh proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatRefreshAppType_t	appType;			/**< Application type */
	TelSatResultType_t		resp;				/**< Result response value */
}TelSatRefreshRetInfo_t;

//	Telephony CALL RETURN INFO
/**
 * @brief The structure type defining result data for the setup call proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t				resp;						    /**< Result response value */
	int							bIsTapiCauseExist;				    /**< Flag for checking tapi error cause */
	TelCallCause_t					tapiCause;						/**< Tapi call error cause */
	TelSsCause_t						ssCause;					/**< Tapi SS error cause */
	TelSatMeProblemType_t				meProblem;					/**< ME problem error cause */
	int								bIsOtherInfoExist;				/**< Call control result existence flag */
	TelSatCallCtrlProblemType_t		permanentCallCtrlProblem;		/**< Call control problem type */
	TelSatCallCtrlRequestedActionInfo_t	callCtrlRequestedAction;	/**< Call control request data */
	TelSatResultInfo_t			result2;						    /**< Call control envelope result value */
	TelSatTextTypeInfo_t			text;							/**< Call control envelope display data */
}TelSatCallRetInfo_t;

//	Telephony SS RETURN INFO
/**
 * @brief The structure type defining result data for the send SS proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t				resp;							    /**< Result response value */
	TelSsCause_t						ssCause;						/**< Error - SS cause */
	TelSatTextInfo_t					ssString;						/**< SS result string */
	TelSatMeProblemType_t			meProblem;						    /**< Error - ME problem */
	int								bIsOtherInfoExist;				    /**< Call control result exist flag */
	TelSatCallCtrlProblemType_t			additionalCallCtrlProblemInfo;	/**< Call control problem */
	TelSatCallCtrlRequestedActionInfo_t	callCtrlRequestedAction;		/**< Call control request data */
	TelSatResultInfo_t					result2;						/**< Call control envelope result value */
}TelSatSsRetInfo_t;

//	Telephony USSD RETURN INFO
/**
 * @brief The structure type defining result data for the send USSD proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t					resp;							/**< Result response value */
	TelSsCause_t						ssCause;						/**< Error - SS cause */
	TelSatTextInfo_t					ussdString;						/**< USSD result string */
	TelSatDataCodingSchemeInfo_t		dcsUssdString;					/**< DCS of USSD result string */
	TelSatMeProblemType_t				meProblem;						/**< Error - ME problem */
	int								bIsOtherInfoExist;				    /**< Call control result exist flag */
	TelSatCallCtrlProblemType_t			additionalCallCtrlProblemInfo;	/**< Call control problem */
	int								bCallCtrlHasModification;		    /**< Call control request modification flag */
	TelSatCallCtrlRequestedActionInfo_t	callCtrlRequestedAction;		/**< Call control request data */
	TelSatResultInfo_t					result2;						/**< Call control envelope result value */
	TelSatTextTypeInfo_t				text2;							/**< CC envelope display data */
}TelSatUssdRetInfo_t;

//	Telephony SMS RETURN INFO
/**
 * @brief The structure type defining result data for the send SMS proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t	resp;			/**< Result response value */
}TelSatSmsRetInfo_t;

//	Telephony DTMF RETUEN INFO
/**
 * @brief The structure type defining result data for the send DTMF proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t	resp;			/**< Result response value */
}TelSatDtmfRetInfo_t;

//	Telephony BROWSER RETURN INFO
/**
 * @brief The structure type defining result data for the launch browser proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t					resp;			/**< Result response value */
	TelSatLaunchBrowserProblemType_t    browserProblem;	/**< Specific browser problem */
}TelSatBrowserRetInfo_t;

//	Telephony SETUP IDLE MODE TEXT RETURN INFO
/**
 * @brief The structure type defining result data for the setup idle mode text proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t	resp;			/**< Result response value */
}TelSatIdleTextRetInfo_t;

//	Telephony PLAY TONE RETURN INFO
/**
 * @brief The structure type defining result data for the Play Tone proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t	resp;			/**< Result response value */
}TelSatPlayToneRetInfo_t;

//	Telephony  PROVIDE LOCAL INFO RETURN INFO
/**
 * @brief The structure type defining result data for the setup idle mode text proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatResultType_t					resp;			/**< Result response value */
	int								bOtherInfo;		    /**< Flag to check whether other information is required */
	TelSatCmdQualiProvideLocalInfo_t	infoType;		/**< Local info type - e.g. time zone, language info, and so on */
	union
	{
		TelSatDataTimeZoneInfo_t		timeZoneInfo;	/**< Current time zone info	*/
		TelSatLanguageInfo_t			languageInfo;	/**< Current ME language setting info */
	}u;													/**< Union */
}TelSatProvideLocalRetInfo_t;

//	Telephony LANGUAGE NOTI RETURN INFO
/**
 * @brief The structure type defining result data for the setup idle mode text proactive command.
 * @since_tizen 2.3
 */
typedef struct
	{
		TelSatResultType_t		 resp;				/**< Result response value */
	}TelSatLanguageNotiRetInfo_t;

//	Telephony DISPLAY TEXT RETURN INFO
/**
 * @brief The structure type defining result data for the setup idle mode text proactive command.
 * @since_tizen 2.3
 */
typedef struct
	{
		TelSatResultType_t		 resp;				/**< Result response value */
		TelSatMeProblemType_t	 meProblem;			/**< ME Problem Type */
	}TelSatDiplayTextRetInfo_t;

//	Telephony APPLICATIONS RETURN DATA
/**
 * @brief The structure type defining common result data for the applications proactive command.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatCommandType_t	commandType;						/**< Proactive Command type */
	int commandId;											/**< Proactive Command Number sent by USIM */
	union
		{
			TelSatSetupMenuRetInfo_t	setupMenu;			    /**< Result response value to setup menu */
			TelSatRefreshRetInfo_t		refresh;				/**< Result response value to refresh */
			TelSatCallRetInfo_t			setupCall;				/**< Result response value to setup call */
			TelSatSsRetInfo_t			sendSs;					/**< Result response value to send SS */
			TelSatUssdRetInfo_t			sendUssd;				/**< Result response value to send USSD */
			TelSatSmsRetInfo_t			sendSms;				/**< Result response value to send SMS */
			TelSatDtmfRetInfo_t			sendDtmf;				/**< Result response value to send DTMF */
			TelSatBrowserRetInfo_t		launchBrowser;			/**< Result response value to launch the browser */
			TelSatIdleTextRetInfo_t		setupIdleModeText;		/**< Result response value to setup idle mode text */
			TelSatLanguageNotiRetInfo_t	languageNoti;			/**< Result response value for language notification */
			TelSatProvideLocalRetInfo_t provideLocalInfo;		/**< Result response value to provide local info */
			TelSatDiplayTextRetInfo_t   displayText;			/**< Result response value to display text */
			TelSatPlayToneRetInfo_t     playTone;				/**< Result response value to play a tone */
		}appsRet;												/**< Common union result value */
}TelSatAppsRetInfo_t;

//	Telephony CALL CONTROL CONFIRM  DATA FOR CALL
/**
 * @brief The structure type defining call control confirm data for the call.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatTextInfo_t				address;					/**< Call destination address */
	TelSatTextInfo_t				subAddress;					/**< Call SUB address */
	TelSatBcRepeatIndicatorType_t	bcRepeatIndicator;			/**< BC repeat indicator */
	TelSatTextInfo_t				ccp1;						/**< Configuration Capability Parameter 1 */
	TelSatTextInfo_t				ccp2;						/**< Configuration Capability Parameter 2 */
} TelSatCallCtrlIndCallData_t;

//	Telephony CALL CONTROL CONFIRM  DATA FOR SS
/**
 * @brief The structure type defining call control confirm data for SS.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatTextInfo_t				ssString;					/**< SS destination address */
	TelSatTextInfo_t				subAddress;					/**< Call SUB address */
	TelSatBcRepeatIndicatorType_t	bcRepeatIndicator;			/**< BC repeat indicator */
	TelSatTextInfo_t				ccp1;						/**< Configuration Capability Parameter 1 */
	TelSatTextInfo_t				ccp2;						/**< Configuration Capability Parameter 2 */
} TelSatCallCtrlIndSsData_t;

//	Telephony CALL CONTROL CONFIRM  DATA FOR USSD
/**
 * @brief The structure type defining call control confirm data for USSD.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatTextInfo_t		ussdString;			/**< USSD destination address */
} TelSatCallCtrlIndUssdData_t;

//	Telephony READ FILE REQUEST DATA
/**
 * @brief The structure type defining common call control confirm data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatCallType_t			callCtrlCnfType;				/**< Call control confirm type - call, SS, or USSD */
	TelSatCallCtrlResultType_t	callCtrlResult;					/**< Call control result */
	TelSatTextInfo_t			dispData;						/**< Call control display data */
	int						bIsUserInfoDisplayEnabled;		    /**< Flag for checking existence of call control display */

	union
		{
			TelSatCallCtrlIndCallData_t		callCtrlCnfCallData;	/**< Call control call address */
			TelSatCallCtrlIndSsData_t		callCtrlCnfSsData;		/**< Call control SS string */
			TelSatCallCtrlIndUssdData_t		callCtrlCnfUssdData;	/**< Call control USSD string */
		}u;															/**< Union */
} TelSatCallCtrlIndData_t;

//	Telephony MO SMS CONTROL CONFIRMATION DATA
/**
 * @brief The structure type defining MO SMS control confirm data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatCallCtrlResultType_t		moSmsCtrlResult;			/**< Envelope result */
	int								bIsUserInfoDisplayEnabled;	/**< Display present flag */
	TelSatTextTypeInfo_t			dispData;					/**< Display data for sending SMS */
	TelSatTextTypeInfo_t			rpDestAddr;				    /**< The RP_Destination_Address of the Service Center */
	TelSatTextTypeInfo_t			tpDestAddr;					/**< The TP_Destination_Address */
} TelSatMoSmCtrlIndData_t;

//	Telephony EVENT LIST INFO
/**
 * @brief The structure type defining Event List Info.
 * @since_tizen 2.3
 */
typedef struct
{
	int bIsEventDownloadActive;			/**< Is Event Download Active */
	int bIsMtCallEvent;					/**< Is Mt Call Event */
	int bIsCallConnected;				/**< Is Call Connected */
	int bIsCallDisconnected;			/**< Is Call Disconnected */
	int bIsLocationStatus;				/**< Is Location Status */
	int bIsUserActivity;				/**< Is User Activity */
	int bIsIdleScreenAvailable;			/**< Is Idle Screen Available */
	int bIsCardReaderStatus;			/**< Is Card Reader Status */
	int bIsLanguageSelection;			/**< Is Language Selection */
	int bIsBrowserTermination;			/**< Is Browser Termination */
	int bIsDataAvailable;				/**< Is Data Available */
	int bIsChannelStatus;				/**< Is Channel Status */
}	TelSatEventListData_t;

/**
 * @brief The structure type containing the data structures to be used to send any envelope/event download data.
 *
 * @details This structure type is used by the Application to send envelope/event download data.
 * @since_tizen 2.3
 */
typedef struct
{
	TelSatEventDownloadType_t					eventDownloadType;				/**< Event Download Type	*/

	union
	{
		int									bIdleScreenAvailable;			    /**< Flag to specify whether Idle Screen is Available */
		TelSatLanguageSelectionEventReqInfo_t	languageSelectionEventReqInfo;	/**< Selected Language Information */
		TelSatBrowserTerminationEventReqInfo_t	browserTerminationEventReqInfo;	/**< Browser Termination Event Information */
		TelSatDataAvailableEventReqInfo_t		dataAvailableEventReqInfo;		/**< dataAvailableEventReqInfo */
		TelSatChannelStatusEventReqInfo_t		channelStatusEventReqInfo;		/**< channelStatusEventReqInfo */
	} u;																		/**< Union */
} TelSatEventDownloadReqInfo_t;

#ifdef __cplusplus
}
#endif

#endif /* _TEL_SAT_H_ */

/**
* @}
*/
