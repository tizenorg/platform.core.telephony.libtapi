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
 * @open
 * @ingroup		TelephonyAPI
 * @addtogroup	SIM_TAPI	SIM
 * @{
 *
 * @file TelSim.h

 @brief This file serves as a "C" header file defines structures for Tapi SIM Services. \n
 It contains a sample set of constants, enums, structs that would be required by applications.
 */

#ifndef _TELSIM_H_
#define _TELSIM_H_

#include <TelDefines.h>
#include <TapiCommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** max length for PIN */
#define TAPI_SIM_PIN_LENGTH	8

/** maximum MCC code length*/
#define TAPI_SIM_MCC_CODE_LEN	3

/** maximum MNC Code length*/
#define TAPI_SIM_MNC_CODE_LEN	2

/** Maximum MSIN code length */
#define TAPI_SIM_MSIN_CODE_LEN	10

/** Maximum ICCID Length */
#define TAPI_SIM_ICCID_LEN_MAX 20

/** Maximum Text length for number */
#define TAPI_SIM_NUM_TEXT_LEN_MAX	256

/** Maximum ECC code length */
#define TAPI_SIM_ECC_CODE_LEN	3

/** Maximum ECC Alpha string length */
#define TAPI_SIM_ECC_ALPHA_STRING_LEN_MAX	50

/** Maximum Language indication length */
#define TAPI_SIM_LP_LEN_MAX	30

/** Maximum group identifier length */
#define TAPI_SIM_GROUP_IDENTIFIER_LEN_MAX	10

/** Maximum UECC Record count */
#define TAPI_SIM_UECC_RECORD_COUNT_MAX	15

/** Defines the maximum number of 3G phone book records at a time  */
#define TAPI_SIM_3G_PB_MAX_RECORD_COUNT 10

/** SIM 3G Phone book EF Max count */
#define TAPI_SIM_PB_3G_FILE_MAX_COUNT 13

/** Operator Name max length */
#define TAPI_SIM_CPHS_OPERATOR_NAME_LEN_MAX 25

/** Operator Name short form max length */
#define TAPI_SIM_CPHS_OPERATOR_NAME_SHORT_FORM_LEN_MAX 10

/** Alpha Id  max length */
#define TAPI_SIM_XDN_ALPHA_ID_MAX_LEN 30

/** Dialing number max length */
#define TAPI_SIM_XDN_DIALING_NUMBER_LEN	20

/** CSP profile entry count max length */
#define TAPI_SIM_CPHS_CUSTOMER_SERVICE_PROFILE_ENTRY_COUNT_MAX	11

/** Authentication code max length */
#define TAPI_SIM_AUTH_MAX_REQ_DATA_LEN 256

/** Authentication response data max length */
#define TAPI_SIM_AUTH_MAX_RESP_DATA_LEN 128

/** SAP Answer to Reset data max length */
#define TAPI_SIM_SAP_ATR_DATA	256

/** SAP APDU max length */
#define TAPI_SIM_APDU_MAX_LEN 256+2 // to be fine tuned according to lower layers, 2bytes for SW1 & SW2 should be added
/** EF-SMSP digit length */
#define TAPI_SIM_SMSP_ADDRESS_LEN 20

/** EF-SMSP alpha id length */
#define TAPI_SIM_SMSP_ALPHA_ID_LEN_MAX  128

/**< Maximum OPLMNWACT length*/
#define TAPI_SIM_OPLMNWACT_MAX_LEN 100

/**< Maximum rsim access data length for 1 time*/
#define TAPI_SIM_DATA_LEN_MAX	256

/** Request Id for Notification */
#define TAPI_SIM_REQUEST_ID_FOR_NOTI	0xFF

/**< Event status for operation timeout */
#define TAPI_SIM_OPERATION_TIMEOUT  1234

#define TAPI_SIM_LOCK_TYPE_INFO_MAX  8

/**
 * @enum TelSimCardType_t
 * This enumeration defines the card type.
 */
typedef enum {
	TAPI_SIM_CARD_TYPE_UNKNOWN, /**< Unknown card */
	TAPI_SIM_CARD_TYPE_GSM, /**< SIm(GSM) card*/
	TAPI_SIM_CARD_TYPE_USIM, /**< USIM card */
} TelSimCardType_t;

/**
 * @enum TelSimFileName_t
 * This enumeration defines the file id.
 */
typedef enum {
	/* common Elementary files */
	TAPI_SIM_EFILE_DIR = 0x2F00, /**< Root Directory for the USIM */
	TAPI_SIM_EFILE_ICCID = 0x2FE2, /**< the ICC Identification file	*/
	TAPI_SIM_EFILE_ADN = 0x6F3A, /**< the Abbrv. Dialing Number file */
	TAPI_SIM_EFILE_FDN = 0x6F3B, /**< the Fixed Dialing Number file  */
	TAPI_SIM_EFILE_SMS = 0x6F3C, /**< the Short Messages file      */
	TAPI_SIM_EFILE_CCP = 0x6F3D, /**< the Capab. Config Parameters file */
	TAPI_SIM_EFILE_IMSI = 0x6F07, /**< the IMSI file                */
	TAPI_SIM_EFILE_SST = 0x6F38, /**< the SIM Service Table file   */
	TAPI_SIM_EFILE_EST = 0x6F56, /**< the Enabled Service Table file   */
	TAPI_SIM_EFILE_ACM = 0x6F39, /**< the Accumul. Call Meter file */
	TAPI_SIM_EFILE_OPLMN_ACT = 0x6F61, /**< the OPLMN List file*/
	TAPI_SIM_EFILE_MBDN = 0x6FC7, /** < SIM Mail Box Dialing Number file */

	TAPI_SIM_EFILE_GID1 = 0x6F3E, /**< the Group Identifier Level 1 */
	TAPI_SIM_EFILE_GID2 = 0x6F3F, /**< the Group Identifier Level 2 */

	/* For PHASE 2 only */
	TAPI_SIM_EFILE_MSISDN = 0x6F40, /**< the MSISDN file              */
	TAPI_SIM_EFILE_SMSP = 0x6F42, /**< the Short Msg Srv. Parameters file*/
	TAPI_SIM_EFILE_SMSS = 0x6F43, /**< the SMS Status               */
	TAPI_SIM_EFILE_EXT1 = 0x6F4A, /**< the Extension 1 file         */
	TAPI_SIM_EFILE_EXT2 = 0x6F4B, /**< the Extension 2 file         */
	TAPI_SIM_EFILE_ELP = 0x2F05, /**< the Extended Language Preference file */
	TAPI_SIM_EFILE_LP = 0x6F05, /**< SIM: Language preference */
	TAPI_SIM_EFILE_ACM_MAX = 0x6F37, /**< the ACM maximum value file   */
	TAPI_SIM_EFILE_PUCT = 0x6F41, /**< the Price per Unit and. file */
	TAPI_SIM_EFILE_CBMI = 0x6F45, /**< the Cell Brd msg Id sel file */
	TAPI_SIM_EFILE_PHASE = 0x6FAE, /**< the PHASE identification file*/

	/* For Phase 2+ */
	TAPI_SIM_EFILE_SDN = 0x6F49, /**< the Service Dialing Numbers */
	TAPI_SIM_EFILE_EXT3 = 0x6F4C, /**< the Extension 3 file         */
	TAPI_SIM_EFILE_BDN = 0x6F4D, /**< the BDN file                 */
	TAPI_SIM_EFILE_EXT4 = 0x6F4E, /**< the Extension 4 file         */
	TAPI_SIM_EFILE_ECC = 0x6FB7, /**< the Emergency Call Codes     */
	TAPI_SIM_EFILE_SPN = 0x6F46, /**< the Service Provider Name    */
	TAPI_SIM_EFILE_SPDI = 0x6FCD, /**< the Service provider display information*/
	TAPI_SIM_EFILE_PNN = 0x6FC5, /**< the PLMN Network Name File*/
	TAPI_SIM_EFILE_OPL = 0x6FC6, /**< the Operator PLMN List File*/

	/* Contents of files at the DF GRAPHICS level */
	TAPI_SIM_EFILE_IMG = 0x4F20, /**< image */

	/* 4.2 Contents of files at the SIM ADF (Application DF) level */
	TAPI_SIM_EFILE_USIM_LI = 0x6A05, /**< USIM: Language Indication */
	TAPI_SIM_EFILE_USIM_PL = 0x2A05, /**< the Language Preference file of USIM */
	TAPI_SIM_EFILE_USIM_IMSI = 0x6A07, /**< IMSI */
	TAPI_SIM_EFILE_USIM_ACM_MAX = 0x6A37, /**< ACM maximum value */
	TAPI_SIM_EFILE_USIM_UST = 0x6A38, /**< USIM Service Table */
	TAPI_SIM_EFILE_USIM_ACM = 0x6A39, /**< Accumulated Call Meter */
	TAPI_SIM_EFILE_USIM_SPN = 0x6A46, /**< Service Provider Name */
	TAPI_SIM_EFILE_USIM_PUCT = 0x6A41, /**< Price per Unit and Currency Table */
	TAPI_SIM_EFILE_USIM_CBMI = 0x6A45, /**< Cell Broadcaster Message Identifier selection */
	TAPI_SIM_EFILE_USIM_ECC = 0x6AB7, /**< Emergency Call Codes */
	TAPI_SIM_EFILE_USIM_FDN = 0x6A3B, /**< Fixed Dialing Number */
	TAPI_SIM_EFILE_USIM_SMS = 0x6A3C, /**< SMS */
	TAPI_SIM_EFILE_USIM_MSISDN = 0x6A40, /**< My own number */
	TAPI_SIM_EFILE_USIM_SMSP = 0x6A42, /** < SMS parameter */
	TAPI_SIM_EFILE_USIM_SMSS = 0x6A43, /** < SMS status */
	TAPI_SIM_EFILE_USIM_SDN = 0x6A49, /** < Service dialing number */
	TAPI_SIM_EFILE_USIM_EXT1 = 0X6A4A, /** < EXT1 record file */
	TAPI_SIM_EFILE_USIM_EXT2 = 0x6A4B, /** < EXT2 record file */
	TAPI_SIM_EFILE_USIM_EXT3 = 0x6A4C, /** < EXT3 record file */
	TAPI_SIM_EFILE_USIM_EXT4 = 0x6A55, /** < EXT4 record file */
	TAPI_SIM_EFILE_USIM_EXT5 = 0x6A4E, /** < EXT5 record file */
	TAPI_SIM_EFILE_USIM_EXT6 = 0x6FC8, /** < EXT6 record file */
	TAPI_SIM_EFILE_USIM_EXT7 = 0x6FCC, /** < EXT7 record file */

	TAPI_SIM_EFILE_USIM_CCP2 = 0x6A4F, /**< Capability Configuration Parameters 2 */
	TAPI_SIM_EFILE_USIM_HIDDEN_KEY = 0x6FC3, /**< Key for hidden phonebook entries */
	TAPI_SIM_EFILE_USIM_BDN = 0x6A4D, /**< Barred Dialing Numbers */

	TAPI_SIM_EFILE_USIM_MBDN = 0x6FC7, /** < Mailbox dialing number -linear fixed*/
	TAPI_SIM_EFILE_USIM_MBI = 0x6FC9, /** < Mailbox Identifier -linear fixed*/

	TAPI_SIM_EFILE_USIM_MWIS = 0x6FCA, /** < Message Waiting Indication Status -linear fixed*/
	TAPI_SIM_EFILE_USIM_CFIS = 0x6FCB, /** < Call forward indication status -linear fixed*/

	TAPI_SIM_EFILE_USIM_APN_ACL = 0x6F57, /** < Access point name control list */

	/* Contents of files at USIM ADF PHONEBOOK level */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_PBR = 0x4FF0, /**< Phonebook Reference File */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_CCP1 = 0x4FFE, /**< Capability Configuration Parameters 1 */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_UID = 0x4FF8, /**< unique identifier */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_PSC = 0x4FFF, /**< phonebook synchronization counter */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_CC = 0x4FEF, /**< change counter */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_PUID = 0x4FEE, /**< previous unique identifier */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_ADN = 0x4FF1, /**< Abbreviated Dialing Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_IAP = 0x4FF2, /**< Index Administration Phonebook */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_PBC = 0x4FF3, /**< Phonebook Control */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_EXT1 = 0x4FFB, /**< Extension 1 */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_GRP = 0x4FF7, /**< Grouping File */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_AAS = 0x4FFC, /**< Additional number Alpha String */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_AAS1 = 0x4FFD, /**< Additional number Alpha String */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_GAS = 0x4F0E, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_GAS1 = 0x4F0F, /**< Second Name Entry */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANR = 0x4FF4, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_SNE = 0x4FF9, /**< Second Name Entry */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_EMAIL = 0x4FFA, /**< e-mail address */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANRA = 0x4F13, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANRB = 0x4F14, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANRC = 0x4F15, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANR1 = 0x4F16, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANRA1 = 0x4F17, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANRB1 = 0x4F18, /**< Additional Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_ANRC1 = 0x4F21, /**< Additional Number */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_ADN1 = 0x4F1A, /**< Abbreviated Dialing Number */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_PBC1 = 0x4F1B, /**< Phonebook Control */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_GRP1 = 0x4F1C, /**< Grouping File */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_SNE1 = 0x4F1D, /**< Second Name Entry */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_UID1 = 0x4F1E, /**< unique identifier */

	TAPI_SIM_EFILE_USIM_PHONEBOOK_EMAIL1 = 0x4F1F, /**< e-mail address */
	TAPI_SIM_EFILE_USIM_PHONEBOOK_IAP1 = 0x4FF5, /**< Index Administration Phonebook */

	/* Contents of files at TELECOM DF PHONEBOOK level */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PBR = 0x4F30, /**< Phonebook Reference File */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PBR1 = 0x4F29, /**< Phonebook Reference File 1 */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PBR2 = 0x4F28, /**< Phonebook Reference File 1 */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_CCP1 = 0x4F3D, /**< Capability Configuration Parameters 1 */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_UID = 0x4F00, /**< unique identifier */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PSC = 0x4F22, /**< Phonebook synchronization counter */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_CC = 0x4F23, /**< change counter */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PUID = 0x4F24, /**< previous unique identifier */

	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ADN = 0x4F3A, /**< Abbreviated Dialing Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_IAP = 0x4F25, /**< Index Administration Phonebook */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PBC = 0x4F09, /**< Phonebook Control */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_EXT1 = 0x4F4A, /**< Extension 1 */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_GRP = 0x4F26, /**< Grouping File */

	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_AAS = 0x4F4B, /**< Additional number Alpha String */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_GAS = 0x4F4C, /**< Additional Number */

	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANR = 0x4F11, /**< Additional Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_SNE = 0x4F19, /**< Second Name Entry */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_EMAIL = 0x4F50, /**< e-mail address */

	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANRA = 0x4F31, /**< Additional Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANRB = 0x4F32, /**< Additional Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANRC = 0x4F40, /**< Additional Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANR1 = 0x4F33, /**< Additional Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANRA1 = 0x4F34, /**< Additional Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ANRB1 = 0x4F35, /**< Additional Number */

	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_ADN1 = 0x4F36, /**< Abbreviated Dialing Number */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_PBC1 = 0x4F37, /**< Phonebook Control */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_GRP1 = 0x4F38, /**< Grouping File */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_SNE1 = 0x4F39, /**< Second Name Entry */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_UID1 = 0x4F3B, /**< unique identifier */

	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_EMAIL1 = 0x4F3C, /**< e-mail address */
	TAPI_SIM_EFILE_TELECOM_PHONEBOOK_IAP1 = 0x4F3E, /**< Index Administration Phonebook */

	TAPI_SIM_EFILE_MSG_WAITING_INDICATION_STATUS = 0x6FCA, /** < MSG waiting indication  */ //WILL BE REMOVED

	/* CPHS FILE ID */
	TAPI_SIM_EFILE_CPHS_VOICE_MSG_WAITING = 0x6F11, /** < CPHS voice MSG waiting indication  */
	TAPI_SIM_EFILE_CPHS_SERVICE_STRING_TABLE = 0x6F12, /** < CPHS service string table  */
	TAPI_SIM_EFILE_CPHS_CALL_FORWARD_FLAGS = 0x6F13, /** < CPHS call forward flags  */
	TAPI_SIM_EFILE_CPHS_OPERATOR_NAME_STRING = 0x6F14, /** < CPHS operator name string  */
	TAPI_SIM_EFILE_CPHS_CUSTOMER_SERVICE_PROFILE = 0x6F15, /** < CPHS customer service profile  */
	TAPI_SIM_EFILE_CPHS_CPHS_INFO = 0x6F16, /** < CPHS information  */
	TAPI_SIM_EFILE_CPHS_MAILBOX_NUMBERS = 0x6F17, /** < CPHS mail box numbers  */
	TAPI_SIM_EFILE_CPHS_OPERATOR_NAME_SHORT_FORM_STRING = 0x6F18, /** < CPHS operator name short form string  */
	TAPI_SIM_EFILE_CPHS_INFORMATION_NUMBERS = 0x6F19, /** < CPHS information numbers  */

	/*  CPHS ALS FILE ID */
	TAPI_SIM_CPHS_DYNAMICFLAGS = 0x6F9F, /** < CPHS Dynamics flags  */
	TAPI_SIM_CPHS_DYNAMIC2FLAG = 0x6F92, /** < CPHS Dynamics2 flags  */
	TAPI_SIM_CPHS_CUSTOMER_SERVICE_PROFILE_LINE2 = 0x6F98, /** < CPHS CSP2  */

	/* ISIM files */
	TAPI_SIM_EFILE_ISIM_IMPI = 0x6F02, /** < IMS private user identity   */
	TAPI_SIM_EFILE_ISIM_DOMAIN = 0x6F03, /** < IMS Home Network Domain Name   */
	TAPI_SIM_EFILE_ISIM_IMPU = 0x6F04, /** < IMS public user identity    */
	TAPI_SIM_EFILE_ISIM_AD = 0x6FAD, /** < Administrative data     *//* NOT implemented */
	TAPI_SIM_EFILE_ISIM_ARR = 0x6F06, /** < Access control file     *//* NOT implemented */

	/* ID clashes with IMSI file Id so changed to  6FFF - actual Id is 0x6F07 *//* modem no support ?*/
	TAPI_SIM_EFILE_ISIM_IST = 0x6FFF, /** <ISIM keys     */
	TAPI_SIM_EFILE_ISIM_KEYS = 0x6F08, /** < Access control file     */
	TAPI_SIM_EFILE_ISIM_PCSCF = 0x6F09, /** < PCSCF address     */
	TAPI_SIM_EFILE_ISIM_GBABP = 0x6FD5, /* support from IPC - NO RSIM access */

	/* Root Certificates */
	TAPI_SIM_EFILE_CA_TOKENINFO = 0x5032,
	TAPI_SIM_EFILE_CA_ODF = 0x5031,
	TAPI_SIM_EFILE_CA_CDFTRUSTED = 0x4404,
	TAPI_SIM_EFILE_CA_TRUSTCERT1 = 0x4424,
	TAPI_SIM_EFILE_CA_TRUSTCERT2 = 0x4425,
	TAPI_SIM_EFILE_CA_TRUSTCERT3 = 0x4426,
	TAPI_SIM_EFILE_CA_DODFPROV = 0x4407,
	TAPI_SIM_EFILE_CA_BOOTSTRAP = 0x4440,
	TAPI_SIM_EFILE_CA_CONFIG1 = 0x4441,

	/* Invalid File ID, All the file ID are less than this Value*/
	TAPI_SIM_EFILE_INVALID = 0xFFFF, /**< Invalid file.*/
	TAPI_SIM_EFILE_OTHERS, /**< Element to indicate an unknown file.*/
} TelSimFileName_t;

/**
 * @enum TelSimPinStatus_t
 * This enumeration defines the pin status.
 */
typedef enum {
	TAPI_SIM_PIN_STATUS_DISABLED = 0x00, /**< Pin1 or Pin2 Disabled */
	TAPI_SIM_PIN_STATUS_ENABLED = 0x01, /**< Pin1 or Pin2 Enabled */
	TAPI_SIM_PIN_STATUS_BLOCKED = 0x02, /**< SIM is present, but PIN1 or PIN2  is blocked. need unblocking by PUK or PUK2 */
	TAPI_SIM_PIN_STATUS_PUK_BLOCKED = 0x03, /**< SIM is present, but PUK is blocked. */
	TAPI_SIM_PIN_STATUS_UNKNOWN = 0xFF /**< SIM is in unknown state */
} TelSimPinStatus_t;

/**
 * @enum TelSimPinOperationResult_t
 * This enumeration defines the pin operation result from the lower layers.
 */
typedef enum {
	TAPI_SIM_PIN_OPERATION_SUCCESS, /**< Operation involving PIN (verification/change/enable/disable, etc) is successful.  */
	TAPI_SIM_BUSY, /**< SIM is busy  */
	TAPI_SIM_CARD_ERROR, /**< SIM card error - Permanently blocked and general errors   */
	TAPI_SIM_INCOMPATIBLE_PIN_OPERATION, /**< SIM Incompatible pin operation that is in case when invalid SIM command is given or incorrect parameters are supplied to the SIM. */
	TAPI_SIM_PIN_INCORRECT_PASSWORD, /**< SIM PIN  Incorrect password */
	TAPI_SIM_PUK_INCORRECT_PASSWORD, /**< SIM PUK Incorrect Password */
	TAPI_SIM_PUK_REQUIRED, /**< PUK Required */
	TAPI_SIM_PIN_REQUIRED, /**< PIN Required */
	TAPI_SIM_NCK_REQUIRED, /**< Network Control Key Required */
	TAPI_SIM_NSCK_REQUIRED, /**< Network Subset Control Key Required */
	TAPI_SIM_SPCK_REQUIRED, /**< Service Provider Control Key Required */
	TAPI_SIM_CCK_REQUIRED, /**< Corporate Control Key Required */
	TAPI_SIM_LOCK_REQUIRED, /**<  PH-SIM (phone-SIM) locked state **/
} TelSimPinOperationResult_t;

/**
 * @enum TelSimAccessResult_t
 * This enumeration defines the SIM access result from the lower layers.
 */
typedef enum {
	TAPI_SIM_ACCESS_SUCCESS, /**< Access to file successful.  */
	TAPI_SIM_ACCESS_CARD_ERROR, /**< SIM card error    */
	TAPI_SIM_ACCESS_FILE_NOT_FOUND, /**< File not found  */
	TAPI_SIM_ACCESS_ACCESS_CONDITION_NOT_SATISFIED, /**< Access condition is not fulfilled  */
	TAPI_SIM_ACCESS_FAILED, /**< Access failed.  */
} TelSimAccessResult_t;

/**
 * @enum TelSimPinType_t
 * This enumeration defines the pin type.
 */
typedef enum {
	TAPI_SIM_PTYPE_PIN1 = 0x00, /**< PIN 1 code */
	TAPI_SIM_PTYPE_PIN2 = 0x01, /**< PIN 2 code */
	TAPI_SIM_PTYPE_PUK1 = 0x02, /**< PUK 1 code */
	TAPI_SIM_PTYPE_PUK2 = 0x03, /**< PUK 2 code */
	TAPI_SIM_PTYPE_UPIN = 0x04, /**< Universal PIN - Unused now */
	TAPI_SIM_PTYPE_ADM = 0x05, /**< Administrator - Unused now */
	TAPI_SIM_PTYPE_SIM = 0x06 /**< SIM Lock code */
} TelSimPinType_t;

/**
 * @enum TelSimPersType_t
 *	This structure gives security lock type information enum values
 */
typedef enum {
	TAPI_SIM_PERS_NET = 0x00, /**< Network Lock */
	TAPI_SIM_PERS_NS = 0x01, /**< Network Subset Lock */
	TAPI_SIM_PERS_SP = 0x02, /**< Service Provider Lock */
	TAPI_SIM_PERS_CP = 0x03, /**< Corporate Lock */
} TelSimPersType_t;

/**
 * @enum TelSimTypeOfNum_t
 * This enumeration defines the type of number.
 */
typedef enum {
	TAPI_SIM_TON_UNKNOWN = 0, /**< unknown */
	TAPI_SIM_TON_INTERNATIONAL = 1, /**< international number */
	TAPI_SIM_TON_NATIONAL = 2, /**< national number */
	TAPI_SIM_TON_NETWORK_SPECIFIC = 3, /**< network specific number */
	TAPI_SIM_TON_DEDICATED_ACCESS = 4, /**< subscriber number */
	TAPI_SIM_TON_ALPHA_NUMERIC = 5, /**< alphanumeric, GSM 7-bit default alphabet) */
	TAPI_SIM_TON_ABBREVIATED_NUMBER = 6, /**< abbreviated number */
	TAPI_SIM_TON_RESERVED_FOR_EXT = 7 /**< reserved for extension */
} TelSimTypeOfNum_t;

/**
 *  @enum TelSimTextEncrypt_t
 *   This enumeration defines the text encryption types
 */
typedef enum {
	TAPI_SIM_TEXT_ENC_ASCII, /**< ASCII Encoding */
	TAPI_SIM_TEXT_ENC_GSM7BIT, /**< GSM 7 Bit Encoding */
	TAPI_SIM_TEXT_ENC_UCS2, /**< UCS2 Encoding */
	TAPI_SIM_TEXT_ENC_HEX, /**< HEX Encoding */
} TelSimTextEncrypt_t;

/**
 * @enum TelSimPbFileType_t
 * This enumeration defines  different storage types to be selected in SIM or USIM
 */
typedef enum {
	TAPI_SIM_PB_FDN, /**< Fixed Dialing Number */
	TAPI_SIM_PB_LDN, /**< last dialed number  - WILL BE REMOVED */
	TAPI_SIM_PB_MSISDN, /**< my own number - MSISDN list */
	TAPI_SIM_PB_ADN, /**< SIM - ADN	 */
	TAPI_SIM_PB_SDN, /**< Service Dialing Number  */
	TAPI_SIM_PB_EN, /**< Emergency number  - WILL BE REMOVED*/
	TAPI_SIM_PB_3GSIM, /**< USIM - 3G phone book */
	TAPI_SIM_PB_AAS, /**< Additional number Alpha String */
	TAPI_SIM_PB_GAS, /**< Grouping identifier Alpha String */
	TAPI_SIM_PB_UNKNOWNN = 0xFF, /**< Unknown file type */
} TelSimPbFileType_t;

/**
 * @enum tapi_pb_storage_field_tag_type_t
 *  This enumeration defines the different storage field types in 3G Phone book.
 */
typedef enum {
	/* for 3G phone storage field type */
	TAPI_PB_3G_NAME = 0x01, /**< Name */
	TAPI_PB_3G_NUMBER, /**< Number */
	TAPI_PB_3G_ANR, /**< Another number */
	TAPI_PB_3G_EMAIL, /**< Email */
	TAPI_PB_3G_SNE, /**< Second name entry */
	TAPI_PB_3G_GRP, /**< Group file */
	TAPI_PB_3G_PBC, /** <1 byte control info and 1 byte hidden info*/
	TAPI_PB_3G_ANRA, /**< Another number */
	TAPI_PB_3G_ANRB, /**< Another number */
} TelSimPb3GFileType_t;

/**
 * @enum TelSimNumberingPlanIdentity_t
 * This enumeration defines the numbering plan identifier.
 */
typedef enum {
	TAPI_SIM_NPI_UNKNOWN = 0, /**< Unknown */
	TAPI_SIM_NPI_ISDN_TEL = 1, /**< ISDN/Telephone numbering plan */
	TAPI_SIM_NPI_DATA_NUMBERING_PLAN = 3, /**< Data numbering plan */
	TAPI_SIM_NPI_TELEX = 4, /**< Telex numbering plan */
	TAPI_SIM_NPI_SVC_CNTR_SPECIFIC_PLAN = 5, /**< Service Center Specific plan */
	TAPI_SIM_NPI_SVC_CNTR_SPECIFIC_PLAN2 = 6, /**< Service Center Specific plan */
	TAPI_SIM_NPI_NATIONAL = 8, /**< National numbering plan */
	TAPI_SIM_NPI_PRIVATE = 9, /**< Private numbering plan */
	TAPI_SIM_NPI_ERMES_NUMBERING_PLAN = 10, /**< ERMES numbering plan */
	TAPI_SIM_NPI_RESERVED_FOR_EXT = 0xF /**< Reserved for extension */
} TelSimNumberingPlanIdentity_t;

/**
 * @enum TelSimEccEmergencyServiceInfo_t
 * This enumeration defines the emergency service type.
 */
typedef enum {
	TAPI_SIM_ECC_ESC_POLICE = 0x01, /**< Police */
	TAPI_SIM_ECC_ESC_AMBULANCE = 0x02, /**< Ambulance */
	TAPI_SIM_ECC_ESC_FIREBRIGADE = 0x04, /**< Fire brigade */
	TAPI_SIM_ECC_ESC_MARAINEGUARD = 0x08, /**< Marine guard */
	TAPI_SIM_ECC_ESC_MOUTAINRESCUE = 0x10, /**< Mountain rescue */
	TAPI_SIM_ECC_ESC_SPARE = 0x00 /**< Spare */
} TelSimEccEmergencyServiceInfo_t;

/**
 * @enum TelSimPbAccessResult_t
 * This enumeration defines the Phone book access result
 */
typedef enum {
	TAPI_SIM_PB_SUCCESS, /**< SIM phonebook operation successful. */
	TAPI_SIM_PB_FAIL, /**< SIM phonebook operation failure. */
	TAPI_SIM_PB_INVALID_INDEX, /**< The index passed was not a valid index. */
	TAPI_SIM_PB_INVALID_NUMBER_LENGTH, /**< The number length is exceeds the max length allowed (or 0). */
	TAPI_SIM_PB_INVALID_NAME_LENGTH, /**< The name length is exceeds the max length allowed (or 0). */
	TAPI_SIM_PB_ACCESS_CONDITION_NOT_SATISFIED, /**< Access condition for PB file is not satisfied. */
} TelSimPbAccessResult_t;

/**
 * @enum TelSimLanguagePreferenceCode_t
 * This enumeration defines the language indication code.
 */
typedef enum {
	TAPI_SIM_LP_GERMAN = 0x00, /**< German */
	TAPI_SIM_LP_ENGLISH = 0x01, /**< English */
	TAPI_SIM_LP_ITALIAN = 0x02, /**< Italian */
	TAPI_SIM_LP_FRENCH = 0x03, /**< French */
	TAPI_SIM_LP_SPANISH = 0x04, /**< Spanish */
	TAPI_SIM_LP_DUTCH = 0x05, /**< Dutch */
	TAPI_SIM_LP_SWEDISH = 0x06, /**< Swedish */
	TAPI_SIM_LP_DANISH = 0x07, /**< Danish */
	TAPI_SIM_LP_PORTUGUESE = 0x08, /**< Portuguese */
	TAPI_SIM_LP_FINNISH = 0x09, /**< Finnish */
	TAPI_SIM_LP_NORWEGIAN = 0x0A, /**< Norwegian */
	TAPI_SIM_LP_GREEK = 0x0B, /**< Greek */
	TAPI_SIM_LP_TURKISH = 0x0C, /**< Turkish */
	TAPI_SIM_LP_HUNGARIAN = 0x0D, /**< Hungarian */
	TAPI_SIM_LP_POLISH = 0x0E, /**< Polish */
	TAPI_SIM_LP_KOREAN = 0x0F, /**< Korean */
	TAPI_SIM_LP_CHINESE = 0x10, /**< Chinese */
	TAPI_SIM_LP_RUSSIAN = 0x11, /**< Russian */
	TAPI_SIM_LP_JAPANESE = 0x12, /**< Japanese */
	TAPI_SIM_LP_LANG_UNSPECIFIED = 0xFF /**< Unspecified */
} TelSimLanguagePreferenceCode_t;

/**
 * @enum TelSimAccessType_t
 * This enumeration defines the access type information.
 */
typedef enum {
	TAPI_SIM_READ_BINARY, /**< Read binary file */
	TAPI_SIM_READ_RECORD, /**< Read record */
	TAPI_SIM_READ_FILE_RAW, /**< Read file and return raw data */
	TAPI_SIM_GET_RESPONSE, /**< Get response */
	TAPI_SIM_UPDATE_BINARY, /**< Update binary */
	TAPI_SIM_UPDATE_RECORD, /**< Update record */
} TelSimAccessType_t;

/**
 * @enum TelSimCardStatus_t
 * This enumeration defines the SIM card status
 */
typedef enum {
	TAPI_SIM_STATUS_CARD_ERROR = 0x00, /**< Bad card / On the fly SIM gone bad **/
	TAPI_SIM_STATUS_CARD_NOT_PRESENT = 0x01, /**<  Card not present **/
	TAPI_SIM_STATUS_SIM_INITIALIZING = 0x02, /**<  SIM is Initializing state **/
	TAPI_SIM_STATUS_SIM_INIT_COMPLETED = 0x03, /**<  SIM Initialization ok **/
	TAPI_SIM_STATUS_SIM_PIN_REQUIRED = 0x04, /**<  PIN  required state **/
	TAPI_SIM_STATUS_SIM_PUK_REQUIRED = 0x05, /**<  PUK required state **/
	TAPI_SIM_STATUS_CARD_BLOCKED = 0x06, /**<  PIN/PUK blocked(permanently blocked- All the attempts for PIN/PUK failed) **/
	TAPI_SIM_STATUS_SIM_NCK_REQUIRED = 0x07, /**<  Network Control Key required state **/
	TAPI_SIM_STATUS_SIM_NSCK_REQUIRED = 0x08, /**<  Network Subset Control Key required state **/
	TAPI_SIM_STATUS_SIM_SPCK_REQUIRED = 0x09, /**<  Service Provider Control Key required state **/
	TAPI_SIM_STATUS_SIM_CCK_REQUIRED = 0x0a, /**<  Corporate Control Key required state **/
	TAPI_SIM_STATUS_CARD_REMOVED = 0x0b, /**<  Card removed **/
	TAPI_SIM_STATUS_SIM_LOCK_REQUIRED = 0x0c /**<  PH-SIM (phone-SIM) locked state **/
} TelSimCardStatus_t;

/**
 * @enum TelSimCphsPhaseType_t
 * This enum gives the current CPHS phase of SIM card.
 */
typedef enum {
	TAPI_SIM_CPHS_PHASE1 = 0x01, /**< phase1  */
	TAPI_SIM_CPHS_PHASE2 = 0x02, /**< phase2  */
	TAPI_SIM_CPHS_PHASE_RFU = 0xff /**< RFU  */
} TelSimCphsPhaseType_t;

/**
 * @enum TelSimCphsIndexLevelIndicator_t
 * This struct gives CPHS index level indication.
 */
typedef enum {
	TAPI_SIM_CPHS_INDEX_LEVEL_ONE = 0x01, /**< SIM cphs index level one */
	TAPI_SIM_CPHS_INDEX_LEVEL_TWO = 0x02, /**< SIM cphs index level two */
	TAPI_SIM_CPHS_INDEX_LEVEL_THREE = 0x03, /**< SIM cphs index level three */
	TAPI_SIM_CPHS_INDEX_LEVEL_RFU = 0xff /**< SIM cphs index level rfu */
} TelSimCphsIndexLevelIndicator_t;

/**
 * @enum TelSimCphsCustomerServiceGroup_t
 * This struct gives CPHS group service type information .
 */
typedef enum {
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CALL_OFFERING = 0x01, /**< Group csp offering*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CALL_RESTRICTION = 0x02, /**< Group csp restriction*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_OTHER_SUPP_SERVICES = 0x03, /**< Group csp supplementary services*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CALL_COMPLETION = 0x04, /**< Group csp completion*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_TELESERVICES = 0x05, /**< Group csp teleservices*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CPHS_TELESERVICES = 0x06, /**< Group csp cphs teleservies*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_CPHS_FEATURES = 0x07, /**< Group csp cphs features*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_NUMBER_IDENTIFIERS = 0x08, /**< Group csp number identifiers*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_PHASE_SERVICES = 0x09, /**< Group csp phase services*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_VALUE_ADDED_SERVICES = 0xC0, /**< Group csp value added services*/
	TAPI_SIM_CPHS_CSP_SERVICE_GROUP_INFORMATION_NUMBERS = 0xD5 /**< Group csp information numbers*/
} TelSimCphsCustomerServiceGroup_t;

/**
 * @enum TelSimIndicationStatusType_t
 * This struct gives SIM Indication service information.
 */
typedef enum {
	TAPI_SIM_INDICATION_MW = 0x00, /**< Message Waiting indication status */
	TAPI_SIM_INDICATION_CF = 0x01, /**< Call Forwarding indication status */
} TelSimIndicationStatusType_t;

/**
 * @enum TelSimMsgWaitingGroup_t
 * This struct gives message waiting group type for mailbox,mwis and cfis.
 */
typedef enum {
	TAPI_SIM_MWG_NONE = 0x00, /**< none*/
	TAPI_SIM_MWG_VOICE = 0x01, /**< voice*/
	TAPI_SIM_MWG_FAX = 0x02, /**< fax*/
	TAPI_SIM_MWG_EMAIL = 0x03, /**< email*/
	TAPI_SIM_MWG_OTHER = 0x04, /**< other*/
	TAPI_SIM_MWG_VIDEO = 0x05, /**< video*/
	TAPI_SIM_MWG_RFU = 0xff /**< rfu*/
} TelSimMsgWaitingGroup_t;

/**
 * @enum TelSimCallForwardUiIndicatorStatus_t
 * This struct gives call forward ui indicator status.
 */
typedef enum {
	TAPI_SIM_CFIS_NONE = 0x00, /**< CFIS none*/
	TAPI_SIM_CFIS_VOICE = 0x01, /**< CFIS voice*/
	TAPI_SIM_CFIS_FAX = 0x02, /**< CFIS fax*/
	TAPI_SIM_CFIS_DATA = 0x04, /**< CFIS data*/
	TAPI_SIM_CFIS_RFU = 0x08 /**< CFIS rfu*/
} TelSimCallForwardUiIndicatorStatus_t;

/**
 * @enum TelSimDynamicFlagsSelectedLineId_t
 * This enum gives dynamics flag selected line  information.
 */
typedef enum {
	TAPI_SIM_DYNAMIC_FLAGS_LINE1 = 0x01, /**< line 1 */
	TAPI_SIM_DYNAMIC_FLAGS_LINE2 = 0x00, /**< line 2*/
	TAPI_SIM_DYNAMIC_FLAGS_RFU = 0xff /**< rfu*/
} TelSimDynamicFlagsSelectedLineId_t;

/**
 * @enum tapi_sim_dynamic2_flag_als_status_t
 * This enum gives dynamics2 flag selected line  information.
 */
typedef enum {
	TAPI_SIM_PIN2_ACCESSIBLE_FLAGS_LOCKED = 0x01, /**< Dynamic flags locked */
	TAPI_SIM_PIN2_ACCESSIBLE_FLAGS_UNLOCKED = 0x00, /**< Dynamic flags unlocked */
	TAPI_SIM_PIN2_ACCESSIBLE_FLAGS_RFU = 0xff /**< rfu */
} TelSimDynamic2FlagAlsStatus_t;

/**
 * @enum TelSimSapMsgId_t
 * This enum gives the SAP message Ids between SAP client and SAP server.
 */
typedef enum {
	TAPI_SIM_SAP_CONNECT_REQ = 0x00, /**< SAP connect request from client to server*/
	TAPI_SIM_SAP_CONNECT_RESP = 0x01, /**< SAP connect response from server to client*/
	TAPI_SIM_SAP_DISCONNECT_REQ = 0x02, /**< SAP disconnect request from client to server */
	TAPI_SIM_SAP_DISCONNECT_RESP = 0x03, /**< SAP disconnect response from server to client */
	TAPI_SIM_SAP_DISCONNECT_IND = 0x04, /**< SAP disconnect indication from server to client */
	TAPI_SIM_SAP_TRANSFER_APDU_REQ = 0x05, /**< SAP transfer APDU(application protocol data unit) from client to server */
	TAPI_SIM_SAP_TRANSFER_APDU_RESP = 0x06, /**< SAP response of transfer APDU */
	TAPI_SIM_SAP_TRANSFER_ATR_REQ = 0x07, /**< SAP Client request about Transfer ATR(Answer to Reset) procedure */
	TAPI_SIM_SAP_TRANSFER_ATR_RESP = 0x08, /**< SAP Server response after execution of ATR */
	TAPI_SIM_SAP_POWER_SIM_OFF_REQ = 0x09, /**< SAP Client request about power SIM off in Server */
	TAPI_SIM_SAP_POWER_SIM_OFF_RESP = 0x0A, /**< SAP Server response the result about power SIM off in Server */
	TAPI_SIM_SAP_POWER_SIM_ON_REQ = 0x0B, /**< SAP Client request about power SIM on in Server */
	TAPI_SIM_SAP_POWER_SIM_ON_RESP = 0x0C, /**< SAP Server response the result about power SIM on in Server */
	TAPI_SIM_SAP_RESET_SIM_REQ = 0x0D, /**< SAP Client request about SIM reset in Server */
	TAPI_SIM_SAP_RESET_SIM_RESP = 0x0E, /**< SAP Server response the result about SIM reset in Server */
	TAPI_SIM_SAP_TRANSFER_CARD_READER_STATUS_REQ = 0x0F, /**< SAP request current status about card reader(Client or Server) */
	TAPI_SIM_SAP_TRANSFER_CARD_READER_STATUS_RESP = 0x10, /**< SAP response current status about card reader(Client or Server) */
	TAPI_SIM_SAP_STATUS_IND = 0x11, /**< SAP indication of Server`s current status*/
	TAPI_SIM_SAP_ERROR_RESP = 0x12, /**< SAP Server send error message when error occur */
	TAPI_SIM_SAP_SET_TRANSPORT_PROTOCOL_REQ = 0x13, /**< SAP Client request Transport Protocol type(T0 or T1)*/
	TAPI_SIM_SAP_SET_TRANSPORT_PROTOCOL_RESP = 0x14 /**< SAP Server response the result after changing Transport Protocol*/
} TelSimSapMsgId_t;

/**
 * @enum TelSimSapConnectionStatus_t
 * This enum gives the SAP connection status information .
 */
typedef enum {
	TAPI_SIM_SAP_CONNECTION_STATUS_OK = 0x00, /**< connect successfully */
	TAPI_SIM_SAP_CONNECTION_STATUS_UNABLE_TO_ESTABLISH, /**< unable to establish connection */
	TAPI_SIM_SAP_CONNECTION_STATUS_NOT_SUPPORT_MAX_SIZE, /**< when server does not support message length that client want send */
	TAPI_SIM_SAP_CONNECTION_STATUS_TOO_SMALL_MAX_SIZE /**< when client want to connect with very small message length which is not supported by Server */
} TelSimSapConnectionStatus_t;

/**
 * @enum TelSimSapDissconnectType_t
 * This enum gives the SAP  disconnect type information.
 */
typedef enum {
	TAPI_SIM_SAP_DISCONNECT_TYPE_GRACEFUL = 0x00, /**< disconnection procedure ends after finishing current work */
	TAPI_SIM_SAP_DISCONNECT_TYPE_IMMEDIATE /**<  disconnection procedure ends immediately*/
} TelSimSapDissconnectType_t;

/**
 * @enum TelSimSapStatusInfo_t
 * This enum gives the SAP current connection status information
 */
typedef enum {
	TAPI_SIM_SAP_STATUS_UNKNOWN = 0x00, /**<  SAP server connection status - unknown*/
	TAPI_SIM_SAP_STATUS_NO_SIM, /**<  SAP server connection status - no SIM*/
	TAPI_SIM_SAP_STATUS_NOT_READY, /**<  SAP server connection status - not ready*/
	TAPI_SIM_SAP_STATUS_READY, /**<  SAP server connection status - ready*/
	TAPI_SIM_SAP_STATUS_CONNECTED /**<  SAP server connection status - connected*/
} TelSimSapStatusInfo_t;

/**
 * @enum TelSimSapCardStatus_t
 * This enum gives the SIM card status if server`s status changed about connection with subscription module
 */
typedef enum {
	TAPI_SIM_SAP_CARD_STATUS_UNKNOWN = 0x00, /**<  SAP server status(card reader status) - unknown*/
	TAPI_SIM_SAP_CARD_STATUS_RESET, /**<  SAP server status(card reader status) - reset*/
	TAPI_SIM_SAP_CARD_STATUS_NOT_ACCESSIBLE, /**<  SAP server status(card reader status) - not accessible*/
	TAPI_SIM_SAP_CARD_STATUS_REMOVED, /**<  SAP server status(card reader status) - removed*/
	TAPI_SIM_SAP_CARD_STATUS_INSERTED, /**<  SAP server status(card reader status) - inserted*/
	TAPI_SIM_SAP_CARD_STATUS_RECOVERED /**<  SAP server status(card reader status) - recovered*/
} TelSimSapCardStatus_t;

/**
 * @enum TelSimSapResultCode_t
 * This enum gives the SAP result information.
 */
typedef enum {
	TAPI_SIM_SAP_RESULT_CODE_OK = 0x00, /**<  SAP operation result - ok*/
	TAPI_SIM_SAP_RESULT_CODE_NO_REASON, /**<  SAP operation result - no reason*/
	TAPI_SIM_SAP_RESULT_CODE_CARD_NOT_ACCESSIBLE, /**<  SAP operation result - not accessible*/
	TAPI_SIM_SAP_RESULT_CODE_CARD_ALREADY_POWER_OFF, /**<  SAP operation result - card already power off*/
	TAPI_SIM_SAP_RESULT_CODE_CARD_REMOVED, /**<  SAP operation result - card removed*/
	TAPI_SIM_SAP_RESULT_CODE_CARD_ALREADY_POWER_ON, /**<  SAP operation result - card already power on*/
	TAPI_SIM_SAP_RESULT_CODE_DATA_NOT_AVAILABLE, /**<  SAP operation result - data not available*/
	TAPI_SIM_SAP_RESULT_CODE_NOT_SUPPORT /**<  SAP operation result - not support*/
} TelSimSapResultCode_t;

/**
 * @enum TelSimSapProtocol_t
 * This enum gives SAP transport protocol type
 */
typedef enum {
	TAPI_SIM_SAP_PROTOCOL_T0, /**< T = 0, character*/
	TAPI_SIM_SAP_PROTOCOL_T1 /**< T = 1, block*/
} TelSimSapProtocol_t;

/**
 * @enum TelSimAuthenticationType_t
 * This is used for Authentication Procedure by using SIM.
 */
typedef enum {
	TAPI_SIM_AUTH_TYPE_IMS = 0x00, /**< IMS Authentication */
	TAPI_SIM_AUTH_TYPE_GSM, /**< GSM Authentication */
	TAPI_SIM_AUTH_TYPE_3G, /**< 3G Authentication */
	TAPI_SIM_AUTH_TYPE_MAX /**< TBD */
} TelSimAuthenticationType_t;

/**
 * @enum TelSimAuthenticationResult_t
 * This is used for Authentication Procedure.
 */
typedef enum {
	TAPI_SIM_AUTH_NO_ERROR = 0x00, /**< ISIM no error */
	TAPI_SIM_AUTH_CANNOT_PERFORM, /**< status - can't perform authentication */
	TAPI_SIM_AUTH_SKIP_RESPONSE, /**< status - skip authentication response */
	TAPI_SIM_AUTH_MAK_CODE_FAILURE, /**< status - MAK(Multiple Activation Key) code failure */
	TAPI_SIM_AUTH_SQN_FAILURE, /**< status - SQN(SeQuenceNumber) failure */
	TAPI_SIM_AUTH_SYNCH_FAILURE, /**< status - synch failure */
	TAPI_SIM_AUTH_UNSUPPORTED_CONTEXT, /**< status - unsupported context */
	TAPI_SIM_AUTH_MAX /**< TBD */
} TelSimAuthenticationResult_t;

/**
 * @enum TelSimFileType_t
 * This enumeration defines the file type.
 */
typedef enum {
	TAPI_SIM_FTYPE_DEDICATED = 0x00, /**< Dedicated */
	TAPI_SIM_FTYPE_TRANSPARENT = 0x01, /**< Transparent -binary type*/
	TAPI_SIM_FTYPE_LINEAR_FIXED = 0x02, /**< Linear fixed - record type*/
	TAPI_SIM_FTYPE_CYCLIC = 0x04, /**< Cyclic - record type*/
	TAPI_SIM_FTYPE_INVALID_TYPE = 0xFF /**< Invalid type */
} TelSimFileType_t;

/**
 * @enum TelSimLockType_t
 *	This structure gives security lock type enum values
 */
typedef enum {
	TAPI_SIM_LOCK_READY = 0x00, /**< ME Not locked - No pin verifications for any lock*/

	TAPI_SIM_LOCK_PS, /** < PH-SIM (phone-SIM) locked.Lock Phone to SIM/UICC card
	 *	(MT asks password when other than current SIM/UICC card inserted;
	 *	MT may remember certain amount of previously used cards thus not
	 *	requiring password when they are inserted
	 */

	TAPI_SIM_LOCK_PF, /** < PH-FSIM (phone-first-SIM) Lock Phone to the very
	 * First inserted SIM/UICC card(MT asks password when other than the first SIM/UICC
	 * card is inserted
	 */

	TAPI_SIM_LOCK_SC, /** < SIM Lock (PIN, PIN2, PUK, PUK2) Lock SIM/UICC card ( SIM asks password in ME power-up and
	 *	when this command is issued
	 */

	TAPI_SIM_LOCK_FD, /** < FDN - SIM card or active application in the UICC (GSM or USIM)
	 *	fixed dialing memory feature */
	TAPI_SIM_LOCK_PN, /**< Network Personalization */
	TAPI_SIM_LOCK_PU, /** < network subset Personalization */
	TAPI_SIM_LOCK_PP, /** < service Provider Personalization */
	TAPI_SIM_LOCK_PC, /** < Corporate Personalization */
	TAPI_SIM_LOCK_SC2, /**< PIN2 lock*/
	TAPI_SIM_LOCK_SCPUK2 = 0x0A, /** < PIN2 Unblocking Key */
	TAPI_SIM_LOCK_ACL = 0x0B, /** < ACL lock */

	TAPI_SIM_NO_SIM = 0x80, /**< SIM not inserted */
	TAPI_SIM_UNAVAIL = 0x81, /** < SIM interface error - SIM inserted but cannot communicate */
	TAPI_SIM_INIT_COMPLETE = 0x82, /** < SIM initialization completed */
	TAPI_SIM_PB_INIT_COMPLETE = 0x83, /**< PB initialization completed */
} TelSimLockType_t;

/**
 *	This will be used to get or set locks - PIN, FDN etc.
 */
typedef enum {
	TAPI_SIM_LOCK_MODE_UNLOCK, /**< unlock */
	TAPI_SIM_LOCK_MODE_LOCK, /**< lock */
	TAPI_SIM_LOCK_MODE_GET /**< get */
} tapi_sim_sec_lock_mode_t;

/**
 * @enum tapi_sim_acl_t
 * This enumeration defines the ACL( Access Condition Level).
 */
typedef enum {
	TAPI_SIM_ACL_ALW = 0x00, /**< Allowed */
	TAPI_SIM_ACL_PIN1 = 0x01, /**< CHV 1 */
	TAPI_SIM_ACL_PIN2 = 0x02, /**< CHV 2 */
	TAPI_SIM_ACL_RFU = 0x04, /**< RFU */
	TAPI_SIM_ACL_ADM = 0x08, /**< Administration */
	TAPI_SIM_ACL_NEVER = 0xFF /** Never */
} TelSimAcl_t;

/**
 * @enum TelSimRecordMode_t
 * This enumeration defines the record mode.
 */
typedef enum {
	TAPI_SIM_RECORD_NONE = 0x00, /**< N/A, meaning the target file isn't a record type */
	TAPI_SIM_RECORD_ABSOLUTE = 0x01, /**< The specified record will be read or updated */
	TAPI_SIM_RECORD_CURRENT = 0x02, /**< The current record will be read or updated */
	TAPI_SIM_RECORD_NEXT = 0x03, /**< The next record will be read or updated */
	TAPI_SIM_RECORD_PREVIOUS = 0x04, /**< The previous record will be read or updated */
	TAPI_SIM_RECORD_TO_THE_LAST = 0x05, /**< All records from the record specified by record index to the last one will be read to the last */
	TAPI_SIM_RECORD_FROM_THE_FIRST = 0x06 /**< All records from the first record to the one specified by record_index will be read from the first */
} TelSimRecordMode_t;

/**
 * @enum TelSimLockKey_t
 *	This structure gives security lock key information enum values
 */
typedef enum {
	TAPI_SIM_LOCK_KEY_NOT_NEED = 0x00, /**< key not need */
	TAPI_SIM_LOCK_KEY_PIN = 0x01, /**< PIN required */
	TAPI_SIM_LOCK_KEY_PUK = 0x02, /**< PUK required */
	TAPI_SIM_LOCK_KEY_PIN2 = 0x03, /**< PIN2 required */
	TAPI_SIM_LOCK_KEY_PUK2 = 0x04, /**< PUK2 required */
	TAPI_SIM_LOCK_PERM_BLOCKED = 0x05, /**< Permanent block SIM */
	TAPI_SIM_LOCK_DISABLE = 0x06, /**< Lock disable */
	TAPI_SIM_LOCK_KEY_SIMLOCK = 0x07, /**< SIM Lock key required */
} TelSimLockKey_t;

/**
 * @enum TelSimPbAccessMode_t
 * This enumeration defines  modes to add, delete, edit for phone book access
 */
typedef enum {
	TAPI_SIM_PB_ACCESS_MODE_ADD = 0x01, /**< Phone Book Access mode - Add entry */
	TAPI_SIM_PB_ACCESS_MODE_DEL = 0x02, /**< Phone Book Access mode - Delete entry */
	TAPI_SIM_PB_ACCESS_MODE_EDIT = 0x03, /**< Phone Book Access mode - edit entry */
} TelSimPbAccessMode_t;


/**
 * This structure contains information about pin data.
 * SIM PIN DATA. For PIN handling (Change, UnBlock) & for Type of PIN information.
 */
typedef struct {
	TelSimPinType_t type; /**< Pin type */
	unsigned char* pw; /**< PIN code */
	unsigned int pw_len; /**< PIN code length*/
} TelSimSecPw_t;

/**
 * This structure contains information about personalization data.
 */
typedef struct {
	TelSimPersType_t type; /**< Personalization type */
	unsigned char* pw; /**< Password */
	unsigned int pw_len; /**< Password length*/
} TelSimPersPw_t;

/**
 * This data structure defines the data for the Read file result information.
 */
typedef struct {
	TelSimFileName_t file_id; /**< rsim access : SIM file id to access */
	TelSimAccessType_t rsim_cmd; /**< rsim access : get response, read, update available*/
	unsigned char p1; /**< rsim access : offset(bin), record num(record)*/
	unsigned char p2; /**< rsim access : offset low(bin), mode(record)*/
	unsigned char p3; /**< Number of bytes to be read(read case) or Length of the subsequent data field(update case) */
	char* data; /**< Only update case. Optional*/
} TelSimRSimReq_t;

/**
 * This data structure defines the data for the Read file with raw format.
 */
typedef struct {
	unsigned short file_id;
	char data_len;
	unsigned char pData[1];
} TelSimReadFileRawData_t;

/**
 * This data structure defines the data for the Response of request raw format it.
 */
typedef struct {
	unsigned char sw1;
	unsigned char sw2;
	unsigned char data_len;
	unsigned char *pData;
} TelSimReadFileRawResonse_t;

/**
 * This data structure defines the data for the apdu.
 */
typedef struct {
	unsigned char apdu_len;
	unsigned char* apdu;
} TelSimApdu_t;

/**
 * This data structure defines the data for the Response of sending apdu.
 */
typedef struct {
	unsigned char apdu_resp_len;
	unsigned char apdu_resp[TAPI_SIM_APDU_MAX_LEN];
} TelSimApduResp_t;

/**
 * This data structure defines the data for the Response of sending apdu.
 */
typedef struct {
	unsigned char atr_resp_len;
	unsigned char atr_resp[TAPI_SIM_APDU_MAX_LEN];
} TelSimAtrResp_t;

/**
 * This structure contains information about File name and path.
 */
typedef struct {
	TelSimFileName_t FileName; /**< Elementary FileName enumeration */
	unsigned char szPath[128]; /**< Absolute szPath of the elementary file. [Null Terminated] */
} TelSimFileID_t;

/**
 * This data structure defines the response for SIM update file.
 */
typedef struct {
	TelSimFileID_t FileId; /**< SIM file ID */
	TelSimAccessType_t AccessType; /**< Access type */
} TelSimFileAccessGeneralResp_t;

/**
 * This structure defines the SIM device Elementary file information read
 */
typedef struct {
	int bValid; /**< Valid or not */
	TelSimFileID_t FileId; /**< File identifier */
	TelSimFileType_t FileType; /**< File type and structure */
	TelSimAcl_t AcRead; /**< Access condition for read operation */
	TelSimAcl_t AcUpdate; /**< Access condition for update operation */
	TelSimAcl_t AcInvalidate; /**< Access condition for invalidate operation */
	TelSimAcl_t AcRehabilitate; /**< Access condition for rehabilitate operation */
	unsigned long RecordLen; /**< Length of one record in file */
	unsigned long RecordCount; /**< Number of records in file */
	unsigned long FileSize; /**< File size */
} TelSimFileInfo_t;

/**
 * This data structure defines the data for the Read file result information.
 */
typedef struct {
	TelSimFileID_t FileID; /**< SIM file id to read */
	TelSimFileType_t FileType; /**< SIM file type */
	unsigned long Offset; /**< Offset if read binary */
	unsigned long RecordIndex; /**< record index if read record */
	unsigned long RecordIndexFrom; /**< start index from */
	unsigned long RecordIndexTo; /**< end index to */
	TelSimRecordMode_t RecordMode; /**< record mode if read record */
	unsigned long DataLength; /**< data length to read */
} TelSimReadFileRequestData_t;

/*
 * This is the RAW data sent to Porting Layer after converting it to SIM specific data by Codecs
 */
typedef struct {
	TelSimFileID_t file_id; /* SIM file id to update */
	TelSimFileType_t file_type; /* SIM file type */
	unsigned long offset; /* offsset if update binary */
	unsigned long record_index; /* record index if update record */
	TelSimRecordMode_t record_mode; /* record mode if update record */
	unsigned long data_len; /* data length to update */
	unsigned char pData[1];
} TelSimUpdateFileReqRawData_t;

/**
 *This data structure defines the data which is provided a unique identification number for the (U)ICC.
 */
typedef struct {
	int icc_length; /**< Integrated Circuit Card number length */
	char icc_num[TAPI_SIM_ICCID_LEN_MAX]; /**< Integrated Circuit Card number */
} TelSimIccIdInfo_t;

/**
 * This data structure defines the data for the Imsi information.
 */
typedef struct {
	int bValid; /**< Valid(ACTIVATE) IMSI (TRUE) or Deactivated (FALSE) */
	char szMcc[TAPI_SIM_MCC_CODE_LEN + 1]; /**< mobile country code */
	char szMnc[TAPI_SIM_MNC_CODE_LEN + 1]; /**< mobile network code */
	char szMsin[TAPI_SIM_MSIN_CODE_LEN + 1]; /**< mobile station identification number */
} TelSimImsiInfo_t;

/**
 * This data structure defines the data for the PIN Information.
 */
typedef struct {
	TelSimPinType_t type; /**< Specifies the PIN or PUK type.*/
	int retry_count; /**< Number of attempts remaining for PIN/PUK verification.*/
} TelSimSecResult_t;

/**
 * This data structure defines the data for the PIN Information.
 */
typedef struct {
	TelSimPinType_t PinType; /**< Specifies the PIN or PUK type.*/
	int RemainingAttempts; /**< Number of attempts remaining for PIN/PUK verification.*/
} TelSimPinInfo_t;

/**
 *
 *	This will be used to get / set lock status of PIN or FDN etc.
 */
typedef struct {
	TelSimLockType_t LockType;
	tapi_sim_sec_lock_mode_t LockMode;
} LockStatus_t;

/**
 *
 * This structure is used to get information about LOCK_TYPE
 */
typedef struct {
	TelSimLockType_t LockType; /**< Lock type */
	TelSimLockKey_t LockKey; /**< Lock key */
	unsigned char RemainingAttempts; /**< retry counts */
} TelSimLockInfoElement_t;

/**
 *
 *This structure is used to get information about LOCK_TYPE
 */
typedef struct {
	unsigned char LockCount; /**< requested lock count */
	int ErrorCode; /**< error code */
	TelSimLockInfoElement_t LockTypeInfo[TAPI_SIM_LOCK_TYPE_INFO_MAX]; /**< Lock info according to Requested Lock Count *//* max of 8 supported */
} TelSimLockInfo_t;

/**
 * This data structure gives the phone book availability of current SIM.
 */
typedef struct {
	int b_fdn; /**< Fixed Dialing Number */
	int b_msisdn; /**< my own number - MSISDN list */
	int b_adn; /**< SIM - ADN(2G phonebook	 */
	int b_sdn; /**< Service Dialing Number  */
	int b_3g; /**< USIM - 3G phonebook */
	int b_aas; /**< Additional number Alpha String phonebook */
	int b_gas; /**< Grouping information Alpha String phonebook */
} TelSimPbList_t;

/**
 *  This data struture defines the 2G phone book data storage.
 */
typedef struct {
	unsigned short NumLen; /**< Number length*/
	unsigned short NumType; /**< Number type [refer to TelSimTypeOfNum_t]*/
	unsigned short NameLen; /**< Name string length*/
	unsigned short NameEncryptType; /**< Name string encryption type [refer to TelSimTextEncrypt_t]*/
	unsigned short Number[TAPI_SIM_NUM_TEXT_LEN_MAX]; /**< Number data*/
	unsigned short Name[TAPI_SIM_NUM_TEXT_LEN_MAX]; /**< Name string*/
} TelSimPb2GData_t;

/**
 *
 *   This is union defines either type of number or defines the text encryption types
 */
typedef union {
	TelSimTypeOfNum_t NumType; /**< Type of number */
	TelSimTextEncrypt_t EncryptionType; /**< Encryption type */
} TelSimPbFileDataType_t;

/**
 *
 *  if tapi_sim_pb_storage_type_t value is TAPI_SIM_PB_3GSIM(0x0C)
 *  SUB_PARAMETERs are as follows
 */
typedef struct {
	TelSimPb3GFileType_t FileType; /**< SM PB File */
	TelSimPbFileDataType_t FileDataType; /**< Type of num or encryption type */
	unsigned short FileDataLength; /**< Length of data*/
	unsigned char AasRecordIdentifier; /**< To be used only for ANR tag type*/
	unsigned char FileData[TAPI_SIM_NUM_TEXT_LEN_MAX]; /**< file data */
} TelSimPb3GFileDataInfo_t;

/**
 *
 * This structure is used for tapi_sim_pb_storage_type_t value is TAPI_SIM_PB_AAS(0x0F) or TAPI_SIM_PB_GAS(0x10)
 *  SUB PRAMETERs are as follows
 */
typedef struct {
	unsigned short TextLength; /**< text length */
	TelSimTextEncrypt_t EncryptionType; /**< Encryption type */
	unsigned char TextData[TAPI_SIM_NUM_TEXT_LEN_MAX]; /**< text data */
} TelSimPbAdditionalNumString_t;

/**
 *
 * This structure contains the information about 3g phonebook
 */
typedef struct {
	unsigned char FileTypeCount; /**<Number of SIM PB EFs */
	TelSimPb3GFileDataInfo_t PbFileDataInfo[TAPI_SIM_PB_3G_FILE_MAX_COUNT]; /**< max of 13 array of structures */
} TelSimPb3GData_t;

/**
 *
 * This structure contains the information about phonebook contact
 */
typedef union {
	TelSimPb2GData_t Pb2GData; /**<2G PB*/
	TelSimPb3GData_t Pb3GData; /**<3G PB*/
	TelSimPbAdditionalNumString_t PbAdditionalNumStringData; /**<Additional number*/
} TelSimPbContactInfo_t;

/**
 *
 * This data structure defines the main structure for adding,edit, delete SIM phone book entry
 */
typedef struct {
	TelSimPbFileType_t StorageFileType; /**< Storage file type */
	unsigned short Index; /**< Index of the record being read/updated. */
	unsigned short NextIndex; /**<  used only in read; ignored for write */
	TelSimPbContactInfo_t ContactInfo; /**< Contact Info can be 2G data or 3G data or AAS, GAS*/
} TelSimPbRecordData_t;


typedef struct {
	TelSimPbFileType_t phonebook_type;
	unsigned short index;
	unsigned short next_index;

	unsigned char name[60];
	TelSimTextEncrypt_t dcs;

	unsigned char number[40];
	TelSimTypeOfNum_t ton;

	/* following field is valid in only USIM*/
	unsigned char anr1[40];
	TelSimTypeOfNum_t anr1_ton;
	unsigned char anr2[40];
	TelSimTypeOfNum_t anr2_ton;
	unsigned char anr3[40];
	TelSimTypeOfNum_t anr3_ton;

	unsigned char email1[60];
	unsigned char email2[60];
	unsigned char email3[60];
	unsigned char email4[60];

	unsigned short group_index; //GRP
	unsigned short pb_control; //PBC
} TelSimPbRecord_t;

/**
 * This enumeration defines the Ecc information.
 */
typedef struct {
	char szEcc1[TAPI_SIM_ECC_CODE_LEN * 2 + 1]; /**< Emergency Call Code 1 info. ECC is coded in BCD format. Unused bytes is set to 0xFF. */
	char szEcc2[TAPI_SIM_ECC_CODE_LEN * 2 + 1]; /**< Emergency Call Code  2 info ECC is coded in BCD format. Unused bytes is set to 0xFF. */
	char szEcc3[TAPI_SIM_ECC_CODE_LEN * 2 + 1]; /**< Emergency Call Code  3 info ECC is coded in BCD format. Unused bytes is set to 0xFF. */
	char szEcc4[TAPI_SIM_ECC_CODE_LEN * 2 + 1]; /**< Emergency Call Code  4 info ECC is coded in BCD format. Unused bytes is set to 0xFF. */
	char szEcc5[TAPI_SIM_ECC_CODE_LEN * 2 + 1]; /**< Emergency Call Code  5 info ECC is coded in BCD format. Unused bytes is set to 0xFF. */
} TelSimEccInfo_t;

/**
 * This data structure defines the data for the USim emergency call code information.
 */
typedef struct {
	int bUsed; /**< Used or not */
	char szEcc[TAPI_SIM_ECC_CODE_LEN * 2 + 1]; /**< Emergency Call Code info-ECC is coded in BCD format. Unused bytes is set to 0xFF.*/
	unsigned short EccLen; /**< Ecc length */
	char szEccAlphaIdentifier[TAPI_SIM_ECC_ALPHA_STRING_LEN_MAX]; /**< Alphabet identifier */
	TelSimEccEmergencyServiceInfo_t EccEmergencyServiceInfo; /**< ECC emergency service information */
} TelUsimEccInfo_t;

/**
 *	This data structure defines the phone book storage count information.
 */
typedef struct {
	TelSimPbFileType_t StorageFileType; /**< Storage  file type */
	unsigned short TotalRecordCount; /**< Total record count */
	unsigned short UsedRecordCount; /**< Used record count */
} TelSimPbStorageInfo_t;

/**
 * This data structure gives the phone book entry information.
 */
typedef struct {
	TelSimPbFileType_t StorageFileType; /**< Storage  file type */
	unsigned short PbIndexMin; /**< Phone book minimum index*/
	unsigned short PbIndexMax; /**< Phone book maximum index */
	unsigned short PbNumLenMax; /**< Phone number maximum length */
	unsigned short PbTextLenMax; /**< Text maximum length */
} TelSimPbEntryInfo_t;

/**
 * This data structure gives the phone book update(add/edit/delete) response.
 */
typedef struct {
	TelSimPbStorageInfo_t PbStorageInfo; /**< Indicates the total and used phonebook entries for the updated storage type (ADN/FDN, etc). */
	unsigned short PbIndex; /**< Phone book record index that was updated/deleted. */
} TelSimPbUpdateResp_t;

/**
 * This data structure defines the data for the Language information.
 */
typedef struct {
	unsigned long LpCount; /**< Language preference count; zero indicates that the LP file is not available or it doesn't have any data.*/
	TelSimLanguagePreferenceCode_t Lp[TAPI_SIM_LP_LEN_MAX]; /**< Language preference code */
} TelSimLanguageInfo_t;

/**
 * This data structure defines the data for the group identifier information.
 */
typedef struct {
	unsigned short GroupIdentifierLen; /**< Group identifier length */
	char szGroupIdentifier[TAPI_SIM_GROUP_IDENTIFIER_LEN_MAX]; /**< Group identifier */
} TelSimGroupIdentifierInfo_t;

/**
 * This data structure defines the data for the ECC information.
 */
typedef struct {
	TelUsimEccInfo_t UeccInfo[TAPI_SIM_UECC_RECORD_COUNT_MAX]; /**< USIM Emergency Call Code information structure*/
	TelSimEccInfo_t EccInfo; /**< Emergency Call Code information*/
} TelSimEccData_t;

/**
 * This structure contains all read SIM elementary file information.
 */
typedef struct {
	unsigned char sw1;
	unsigned char sw2;
	unsigned char DataLen; /**<Data Length*/
	unsigned char Data[TAPI_SIM_DATA_LEN_MAX];
} TelSimReadFileRaw_t;

/**
 *	This will be used to get / set lock status of PIN or FDN etc.
 */
typedef struct {
	TelSimPersType_t type;
	int mode;
} TelSimPersStatus_t;

/**
 *
 This structure gives 3G phone book capability information.
 */
typedef struct {
	TelSimPb3GFileType_t FileType3G; /**< 3G phonebook file type */
	unsigned short PbIndexMax; /**< max index */
	unsigned short PbTextMax; /**< max text length */
	unsigned short TotalRecordCount; /**< total record count */
} TelSimPb3GFileTypeCapabiltyInfo_t;

/**
 *
 * This data structure defines the data for the SIM PHONEBOOK & ITS CAPABILITIES information.
 * It refers to EF_PBR
 */
typedef struct {
	unsigned char FileTypeCount; /**< phonebook file type count */
	TelSimPb3GFileTypeCapabiltyInfo_t FileTypeInfo[TAPI_SIM_PB_3G_FILE_MAX_COUNT]; /**< phonebook file type information */
} TelSimPbCapabilityInfo_t;

/**CPHS related structs **/

/**
 *	This sturcture gives information of available optional CPHS SIM files.
 */
typedef struct {
	/* Byte 2 - bit1 & 2*/
	int bCustomerServiceProfile; /**< Customer Service Profile (CSP)  */
	/* Byte 2 - bit3 & 4*/
	int bServiceStringTable; /**< Service String Table (SST) */
	/* Byte 2 - bit5 & 6*/
	int bMailBoxNumbers; /**< MailBoxNumbers */
	/* Byte 2 - bit7 & 8*/
	int bOperatorNameShortForm; /**< Short form of operator name */
	/* Byte 3 - bit1 & 2*/
	int bInformationNumbers; /**< Information numbers */
} TelSimCphsServiceTable_t;

/*
 These requirements are additional to the GSM 900 and DCS1800 recommendations.
 They apply to all products which are to be compliant with the CPHS specification.

 In addition to those SIM storage fields previously defined in DCS1800 to support
 existing MS features and services, the Association has defined the following fields  :-

 1)	Call Forwarding flag						(mandatory)
 2)	Voice message waiting flag					(mandatory)
 3)	PLMN operator name						(mandatory)
 4)	Customer Service Profile (CSP)				(optional)
 5)	CPHS Information							(mandatory)
 6)	Mailbox Numbers							(optional)
 7)	Information Numbers						(optional)

 */

/*
 DATA FIELD - 6F 16: CPHS INFORMATION
 Access Conditions:
 READ	CHV1
 UPDATE	ADM
 */
/**
 *
 *This structure gives CPHS information data.
 */
typedef struct {
	TelSimCphsPhaseType_t CphsPhase; /**< CPHS phase type */
	TelSimCphsServiceTable_t CphsServiceTable; /**< CPHS service table */
} TelSimCphsInfo_t;

/*
 DATA FIELD - 6F 14: OPERATOR NAME STRING
 Access Conditions:
 READ	CHV1
 UPDATE	ADM
 */
/**
 *
 * This struct gives CPHS operator name information.
 */
typedef struct {
	unsigned char NameLength; /**< CPHS operator name length */
	unsigned char OperatorName[TAPI_SIM_CPHS_OPERATOR_NAME_LEN_MAX + 1]; /**< CPHS operator name */
} TelSimCphsOperatorName_t;

/*
 DATA FIELD - 6F 18: OPERATOR NAME SHORTFORM
 Access Conditions:
 READ	CHV1
 UPDATE	ADM
 */
/**
 *
 * This struct gives CPHS operator short form name information.
 */
typedef struct {
	unsigned char ShortNameLength; /**< CPHS operator short name length */
	unsigned char OperatorShortName[TAPI_SIM_CPHS_OPERATOR_NAME_SHORT_FORM_LEN_MAX + 1]; /**< CPHS short operator name */
} TelSimCphsOperatorShortName_t;

/*
 Data field 6F 17 in CPHS, 6F C7 in 3GPP : Mailbox Numbers
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */

/**
 * This data structure defines the data for the dialing number information.
 */
typedef struct {
	int bUsed; /**< Used or not */
	unsigned long AlphaIDMaxLength; /**< alpha max length in SIM - READ ONLY*/
	unsigned long AlphaIdLength; /**< length of alpha identifier */
	unsigned char AlphaId[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN + 1]; /**< Alpha Identifier */
	unsigned long DiallingNumMaxLength; /**< max length of dialing number filed in SIM - READ ONLY */
	unsigned long DiallingnumLength; /**< Length of BCD number/SSC contents */
	TelSimTypeOfNum_t TypeOfNumber; /**< TON */
	TelSimNumberingPlanIdentity_t NumberingPlanIdent; /**< NPI */
	char DiallingNum[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1]; /**< Dialing Number/SSC String */
	unsigned char CapaConfigId; /**< Capability/Configuration Identifier */
	unsigned char Ext1RecordId; /**< Extensiion1 Record Identifier */
} TelSimDialingNumberInfo_t;

/**
 * This struct gives mailbox number(including CPHS case).
 */
typedef struct {
	int b_cphs; /**< current data is from CPHS SIM or not*/
	TelSimDialingNumberInfo_t voice_line1; /**< voice line1 mailbox number */
	TelSimDialingNumberInfo_t voice_line2; /**< voice line2 mailbox number (only available in CPHS)*/
	TelSimDialingNumberInfo_t video; /**< video mailbox number */
	TelSimDialingNumberInfo_t fax; /**< fax mailbox number */
	TelSimDialingNumberInfo_t email; /**< email mailbox number */
} TelSimMailboxNumbers_s;

/*
 DATA FIELD -6F 11: Voice message waiting flag
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */
/**
 *
 * This struct gives CPHS voice message waiting flag information .
 */
typedef struct {
	int bWaitVoiceMsgLine1; /**< VoiceMsgLine 1 */
	int bWaitVoiceMsgLine2; /**< VoiceMsgLine 2 */
	int bWaitFaxMsg; /**< FAX Msg */
	int bWaitDataMsg; /**< Data Msg */
} TelSimCphsVoiceMsgWaitFlagInfo_t;

/*
 DATA FIELD -6F 13: Call forwarding flags
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */
/**
 * This struct gives CPHS call forwarding flag information.
 */
typedef struct {
	int bCallForwardUnconditionalLine1; /**< CallForwardUnconditionalLine 1 */
	int bCallForwardUnconditionalLine2; /**< CallForwardUnconditionalLine 2 */
	int bCallForwardUnconditionalFax; /**< CallForwardUnconditional FAX */
	int bCallForwardUnconditionalData; /**<CallForwardUnconditional data*/
	int bCallForwardUnconditionalSms; /**< CallForwardUnconditional SMS */
	int bCallForwardUnconditionalBearer; /**< CallForwardUnconditional bearer*/
} TelSimCphsCallForwardingFlagInfo_t;

/*
 DATA FIELD -6F 19: Information Numbers
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */
/**
 * This struct gives CPHS information numbers data.
 */
typedef struct {
	int bUsed; /**< SIM CPHS index level one */
	unsigned char AlphaIdLength; /**< length of alpha identifier */

	TelSimCphsIndexLevelIndicator_t IndexLevelIndicator; /**< SIM CPHS index level one */
	int PremiumServiceIndicator; /**< SIM CPHS index level one */
	int NetworkSpecificIndicator; /**< SIM CPHS index level one */
	unsigned char Alpha_id[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN + 1]; /**<  Alpha Identifier */

	unsigned long DiallingnumLength; /**< Length of BCD number/SSC contents */
	TelSimTypeOfNum_t TypeOfNumber; /**< TON */
	TelSimNumberingPlanIdentity_t NumberingPlanIdentity; /**< NPI */
	char DiallingNum[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1]; /**< dialing Number/SSC String */
	unsigned char Ext1RecordId; /**< Extensiion1 Record Identifier */
} TelSimCphsInformationNum_t;

/*
 DATA FIELD- 6F 15: Customer Service Profile (Storing a list of service options which are relevant to that specific customer)
 Access Conditions:
 READ	CHV1
 UPDATE	CHV1
 */
/**
 *
 * This struct gives CPHS service call offering information.
 */
typedef struct {
	int bCallForwardingUnconditional; /**< CallForwarding Unconditional */
	int bCallForwardingOnUserBusy; /**< CallForwarding On UserBusy */
	int bCallForwardingOnNoReply; /**< CallForwarding On NoReply */
	int bCallForwardingOnUserNotReachable; /**< CallForwarding On User Not Reachable */
	int bCallTransfer; /**< Call Transfer */
} TelSimCphsServiceCallOffering_t;

/**
 *
 * This struct gives CPHS service call restriction information.
 */
typedef struct {
	int bBarringOfAllOutgoingCalls; /**< Barring Of All Outgoing Calls*/
	int bBarringOfOutgoingInternationalCalls; /**< Barring Of Outgoing International Calls */
	int bBarringOfOutgoingInternationalCallsExceptHplmn; /**< Barring Of Outgoing International Calls Except HPLMN */
	int bBarringOfAllIncomingCallsRoamingOutsideHplmn; /**< Barring Of All Incoming Calls Roaming Outside HPLMN */
	int bBarringOfIncomingCallsWhenRoaming; /**< Barring Of IncomingCalls When Roaming */
} TelSimCphsServiceCallRestriction_t;

/**
 *
 * This struct gives CPHS service SS  information.
 */
typedef struct {
	int bMultiPartyService; /**< MultiPartyService*/
	int bClosedUserGroup; /**< ClosedUserGroup*/
	int bAdviceOfCharge; /**< AdviceOfCharge*/
	int bPreferentialClosedUserGroup; /**< PreferentialClosedUserGroup*/
	int bClosedUserGroupOutgoingAccess; /**< ClosedUserGroupOutgoingAccess*/
} TelSimCphsServiceOtherSupplimentaryService_t;

/**
 *
 * This struct gives CPHS service call complete information.
 */
typedef struct {
	int bCallHold; /**< Call Hold*/
	int bCallWaiting; /**< Call Waiting*/
	int bCompletionOfCallToBusySubscriber; /**< Completion Of Call To Busy Subscriber*/
	int bUserUserSignalling; /**< User User Signaling*/
} TelSimCphsServiceCallComplete_t;

/**
 *
 * This struct gives CPHS service teleservices  information.
 */
typedef struct {
	int bShortMessageMobileOriginated; /**< Short Message Mobile Originated*/
	int bShortMessageMobileTerminated; /**< Short Message Mobile Terminated*/
	int bShortMessageCellBroadcast; /**< Short Message Cell Broadcast*/
	int bShortMessageReplyPath; /**< Short  Message Reply Path*/
	int bShortMessageDeliveryConf; /**< Short Message Delivery Conf*/
	int bShortMessageProtocolIdentifier; /**< Short Message Protocol Identifier*/
	int bShortMessageValidityPeriod; /**< Short Message Validity Period*/
} TelSimCphsServiceTeleservices_t;

/**
 *
 * This struct gives CPHS alternative line service  information.
 */
typedef struct {
	int bAlternativeLineService; /**< Alternative Line Service*/
} TelSimCphsServiceCphsTeleservices_t;

/**
 *
 * This struct gives CPHS string service table information.
 */
typedef struct {
	int bStringServiceTable; /**< String Service Table*/
} TelSimCphsServiceCphsFeatures_t;

/**
 *
 * This struct gives CPHS service number identifier  information.
 */
typedef struct {
	int bCallingLineIdentificationPresent; /**< Calling Line Identification Present*/
	int bConnectedLineIdentificationRestrict; /**< Connected Line Identification Restrict*/
	int bConnectedLineIdentificationPresent; /**< Connected Line Identification Present*/
	int bMaliciousCallIdentifier; /**< Malicious Call Identifier*/
	int bCallingLineIdentificationSend; /**< Calling Line Identification Send*/
	int bCallingLineIdentificationBlock; /**< Calling Line Identification Block*/
} TelSimCphsServiceNumberIdentifier_t;

/**
 *
 * This struct gives CPHS service phase services information.
 */
typedef struct {
	int bMenuForGprs; /**< Menu For GPRS*/
	int bMenuForHighSpeedCsd; /**< Menu For HighSpeedCsd*/
	int bMenuForVoiceGroupCall; /**< Menu For VoiceGroupCall*/
	int bMenuForVoiceBroadcastService; /**< Menu For VoiceBroadcastService*/
	int bMenuForMultipleSubscriberProfile; /**< Menu For MultipleSubscriberProfile*/
	int bMenuForMultipleBand; /**< Menu For MultipleBand*/
} TelSimCphsServicePhaseServices_t;

/**
 *
 * This struct gives CPHS value added service   information.
 */
typedef struct {
	int bRestrictMenuForManualSelection; /**< RestrictMenu For ManualSelection*/
	int bRestrictMenuForVoiceMail; /**< RestrictMenu For VoiceMail*/
	int bRestrictMenuForMoSmsAndPaging; /**< RestrictMenu For MoSmsAndPaging*/
	int bRestrictMenuForMoSmsWithEmialType; /**< RestrictMenu For MoSmsWithEmialType*/
	int bRestrictMenuForFaxCalls; /**< RestrictMenu For FaxCalls*/
	int bRestrictMenuForDataCalls; /**< RestrictMenu For DataCalls*/
	int bRestrictMenuForChangeLanguage; /**< RestrictMenu For ChangeLanguage*/
} TelSimCphsServiceValueAddedServices_t;

/**
 *
 * This struct gives CPHS service information number data.
 */
typedef struct {
	int bInformationNumbers; /**< Information Numbers*/
} TelSimCphsServiceInformationNumbers_t;

/**
 *
 * This struct gives CPHS service profile entry  information.
 */
typedef struct {
	TelSimCphsCustomerServiceGroup_t CustomerServiceGroup; /**< customer service group*/
	union {
		TelSimCphsServiceCallOffering_t CallOffering; /**< call offering*/
		TelSimCphsServiceCallRestriction_t CallRestriction; /**< call restriction*/
		TelSimCphsServiceOtherSupplimentaryService_t OtherSuppServices; /**< other SS services*/
		TelSimCphsServiceCallComplete_t CallComplete; /**< call complete*/
		TelSimCphsServiceTeleservices_t Teleservices; /**< teleservices*/
		TelSimCphsServiceCphsTeleservices_t CphsTeleservices; /**< CPHS teleservices*/
		TelSimCphsServiceCphsTeleservices_t CphsFeatures; /**< CPHS features*/
		TelSimCphsServiceNumberIdentifier_t NumberIdentifiers; /**< number identifiers*/
		TelSimCphsServicePhaseServices_t PhaseServices; /**< phase services*/
		TelSimCphsServiceValueAddedServices_t ValueAddedServices; /**< value added services*/
		TelSimCphsServiceInformationNumbers_t InformationNumbers; /**< information numbers*/
	} u;
} TelSimCphsCustomerServiceProfileEntry_t;

/**
 *
 * This struct gives CPHS service profile information.
 */
typedef struct {
	TelSimCphsCustomerServiceProfileEntry_t ServiceProfileEntry[TAPI_SIM_CPHS_CUSTOMER_SERVICE_PROFILE_ENTRY_COUNT_MAX]; /**< service profile entry*/
} TelSimCphsCustomerServiceProfileInfo_t;

/*
 Identifier: '6FCA'	Structure: linear fixed	Optional
 Record length: X bytes, X>=5	Update activity: low
 Access Conditions:
 READ	PIN
 UPDATE	PIN
 */
/**
 *
 * This struct gives message waiting indication status information.
 */
typedef struct {
	unsigned char IndicatorType; /**< Indicator Type*/
	unsigned char VoiceMailCount; /**< VoiceMail Count*/
	unsigned char FaxCount; /**< FAX Count*/
	unsigned char EmailCount; /**< Email Count*/
	unsigned char OtherCount; /**< Other Count*/
	unsigned char VideoMailCount; /**< VideoMail Count*/
} TelSimMsgWaitingIndicationStatus_t;

/*
 Identifier: '6FCB'	Structure: linear fixed	Optional
 Record length: 16 bytes,	Update activity: low
 Access Conditions:
 READ	PIN
 UPDATE	PIN
 */
/**
 *
 * This struct gives call forwarding indication  information.
 */
typedef struct {
	int bUsed; /**< used or not*/
	unsigned char MspNumber; /**< MSP number*/
	unsigned char Status; /**< call forwarding indication status*/
	unsigned long DiallingnumLen; /**< Length of BCD number/SSC contents*/
	TelSimTypeOfNum_t TypeOfNumber; /**< TON*/
	TelSimNumberingPlanIdentity_t NumberingPlanIdent; /**< NPI*/
	char DiallingNum[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1];/**< Dialing Number/SSC String*/
} TelSimCallForwardingIndicationStatus_t;

/**
 * This struct gives Call Forwarding Indication information including CPHS case.
 */
typedef struct {
	int b_cphs; /**< current data is from CPHS SIM or not*/
	union {
		TelSimCphsCallForwardingFlagInfo_t cphs_cf; /**< information from CPHS SIM*/
		TelSimCallForwardingIndicationStatus_t cf; /**< information from 3GPP SIM*/
	} cf_data_u;
} TelSimCallForwarding_s;

/**
 * This struct gives Message Waiting Indication information including CPHS case.
 */
typedef struct {
	int b_cphs; /**< current data is from CPHS SIM or not*/
	union {
		TelSimCphsVoiceMsgWaitFlagInfo_t cphs_mw; /**< information from CPHS SIM*/
		TelSimMsgWaitingIndicationStatus_t mw; /**< information from 3GPP SIM*/
	} mw_data_u;
} TelSimMessageWaiting_s;

/**
 *
 * This struct gives dynamics flag selected line  information.
 */
typedef struct {
	TelSimDynamicFlagsSelectedLineId_t DynamicFlags; /**< Dynamic flags information */
} TelSimDynamicFlagsInfo_t;

/**
 *
 * This struct gives dynamics flag selected line  information.
 */
typedef struct {
	TelSimDynamic2FlagAlsStatus_t Dynamic2Flag; /**< Dynamic flags status */
} TelSimDynamic2FlagsInfo_t;

/**
 *	This data structure represents the CPHS specific information
 */
typedef struct {
	int b_used; /**< used or not */
	TelSimCphsInfo_t cphsinfo; /**< CPHS information */
	TelSimCphsCustomerServiceProfileInfo_t csp; /**< customer service profile */
	TelSimCphsInformationNum_t infn; /**< information numbers */
	TelSimCphsOperatorName_t opname; /**< operator name */
	TelSimCphsOperatorShortName_t opshortform; /**< operator short form */
	TelSimDynamicFlagsInfo_t dflagsinfo; /**< Dynamic flags */
	TelSimDynamic2FlagsInfo_t d2flagsinfo; /**< Dynamic2 flags */
} TelSimCphsLocalInfo_t;

/**
 *	This data structure represents MSISDN information
 */
typedef struct {
	char num[TAPI_SIM_XDN_DIALING_NUMBER_LEN + 1]; /**< MSISDN number. If not exist, Null string will be returned*/
	char name[TAPI_SIM_XDN_ALPHA_ID_MAX_LEN + 1]; /**< MSISDN name. If not exist, Null string will be returned*/
} TelSimSubscriberInfo_t;

/**
 *This is used for authentication request procedure.
 */
typedef struct {
	TelSimAuthenticationType_t auth_type; /**< Authentication type */
	int rand_length; /**< the length of RAND */
	int autn_length; /**< the length of AUTN. it is not used in case of GSM AUTH */
	char rand_data[TAPI_SIM_AUTH_MAX_REQ_DATA_LEN]; /**< RAND data */
	char autn_data[TAPI_SIM_AUTH_MAX_REQ_DATA_LEN]; /**< AUTN data. it is not used in case of GSM AUTH */
} TelSimAuthenticationData_t;

/**
 * This is used for result data of authentication.
 */
typedef struct {
	TelSimAuthenticationType_t auth_type; /**< authentication type */
	TelSimAuthenticationResult_t auth_result; /**< authentication result */
	int resp_length; /**< response length. IMS and 3G case, it stands for RES_AUTS. GSM case, it stands for SRES. */
	char resp_data[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< response data. IMS and 3G case, it stands for RES_AUTS. GSM case, it stands for SRES. */
	int authentication_key_length; /**< the length of authentication key, Kc*/
	char authentication_key[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< the data of of authentication key, Kc*/
	int cipher_length; /**< the length of cipher key length */
	char cipher_data[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< cipher key */
	int integrity_length; /**< the length of integrity key length */
	char integrity_data[TAPI_SIM_AUTH_MAX_RESP_DATA_LEN]; /**< integrity key */
} TelSimAuthenticationResponse_t;

/**OPLMN list**/
typedef struct {
	char szMcc[TAPI_SIM_MCC_CODE_LEN + 1]; /**< mobile country code */
	char szMnc[TAPI_SIM_MNC_CODE_LEN + 1]; /**< mobile network code */

} TelSimOplmnlistInfo_t;

/*OPLMN Access technology Identifiers*/
typedef struct {
	int bACT_RFU1; /**<act rfu1 */
	int bACT_RFU2; /**<act rfu2 */
	int bACT_RFU3; /**<act rfu3 */
	int bACT_RFU4; /**<act rfu4 */
	int bACT_RFU5; /**<act rfu5 */
	int bACT_RFU6; /**<act rfu6 */
	int bACT_RFU7; /**<act rfu7 */
	int bACT_UTRAN; /**<act UTRAN */
	int bACT_RFU9; /**<act rfu9 */
	int bACT_RFU10; /**<act rfu10 */
	int bACT_RFU11; /**<act rfu11 */
	int bACT_RFU12; /**<act rfu12 */
	int bACT_RFU13; /**<act rfu13 */
	int bACT_RFU15; /**<act rfu15 */
	int bACT_GSMCOMPACT; /**<act GSM compact */
	int bACT_GSM; /**<act GSM */
} TelSimOplmnActInfo_t;

/*OpLMNWACT  File data*/
typedef struct {
	TelSimOplmnlistInfo_t OplmnList; /**<oplmn list information */
	TelSimOplmnActInfo_t ActIdentifier; /**<act identifier*/
} TelSimOplmnWactInfo_t;

/* OPLMNWACT  list */
typedef struct {
	TelSimOplmnWactInfo_t OplmnList[TAPI_SIM_OPLMNWACT_MAX_LEN]; /**<oplmnwact list information */
	int OplmnListCnt; /**<oplmnwact list count */
} TelSimOplmnWactList_t;

/* SAP (SIM Access Profile) related interface structures and enum */
/**
 *
 * This struct gives the SAP ATR response data information.
 */
typedef struct {
	TelSimSapResultCode_t AtrResult; /**<  SAP result of transfer ATR request */
	unsigned long AtrLength; /**<  SAP ATR response data length */
	unsigned char AtrData[TAPI_SIM_SAP_ATR_DATA]; /**<  SAP ATR response data */
} TelSimSapAtrInfo_t;

/**
 *
 * This data structure gives the SAP ATR response data information.
 */

typedef struct {
	TelSimSapMsgId_t MsgId; /**< connect or disconnect request /response *//* 1 byte  at lower level */
	TelSimSapConnectionStatus_t ConnectionStatus; /**< used only in response from tapi */
	unsigned short MaxMsgSize; /**< request from app or set by lower layer*/

} TelSimSapConnect_t;

/* SAP transfer APDU request */

/**
 *
 * This data structure gives the SAP APDU data information.
 */
typedef struct {
	unsigned char ApduLength; /**<  SAP APDU length */
	unsigned char Apdu[TAPI_SIM_APDU_MAX_LEN]; /**<  SAP APDU data */
} TelSimSapApduData_t;

/**
 *
 * This data structure gives SAP SIM power response
 */
typedef struct {
	TelSimSapMsgId_t MsgId; /**<  SAP operation Id */
	TelSimSapResultCode_t SimPowerResult; /**<  SAP result of operation */
} TelSimSapPower_t;

/**
 *
 * This data structure gives SAP SIM card reader status
 */
typedef struct {
	TelSimSapResultCode_t CardReaderResult; /**< SAP result of operation */
	char CardReaderStatus; /**< bit wise encoded information */
} TelSimCardReaderStatus_t;

#ifdef __cplusplus
}
#endif

#endif // _TELSIM_H_
/**
 * @}
 */
