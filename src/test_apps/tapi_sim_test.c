/*
 * libslp-tapi
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Kyeongchul Kim <kyeongchul.kim@samsung.com>
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


/* standard header files*/
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/stat.h>
#include <assert.h>

/*Dbus header files*/
#include <glib.h>

#include "ITapiSim.h"
#include "TelErr.h"
#include "TapiCommon.h"
#include "TelUtility.h"
#include "TapiEvent.h"

#define SIM_APPLICATION_ID  10
#define _TAPI_DEBUG_ON_SCREEN_
#define TEST_DEBUG(frmt, args...) \
{do { fprintf(stderr, "[SIM Test][%s:%04d] "frmt "\n", __func__,__LINE__, ##args); } while (FALSE); }

#ifndef false
#define false 0
#endif
#ifndef true
#define true  !0
#endif
#ifndef bool
#define bool  char
#endif

//extern int EvtDeliveryHandle ;
int request_id;
int iNumOfSIMEvt = 0;
int error_code;
static unsigned int* SIM_subscription_id;

void sim_async_event_callback(TelTapiEvent_t* sim_event);

// same as tapievent
typedef struct {
	int group;
	int type;
	int ID;
	int status;
	void *dst;
} data_t;

int strlen2(register const char * s) {
	int count = 0;
	while (*s++)
		count++;
	return count;
}

int _fgets(register char * digit, int n) {

	if (!fgets(digit, n, stdin)) {
		return -1;
	}
	if (*(digit + (n = strlen2(digit) - 1)) ^ '\n') {
		n = 0;
		while (getchar() ^ '\n')
			n--;
		if (n)
			return 1;

		return 0;
	} else
		*(digit + n) = 0;
	return 0;
}

TelSimPbFileType_t __InputStorageType() {
	TelSimPbFileType_t storage_type = 0;

	int iStorageType = 0;
	char dummy = 0;

	TEST_DEBUG(" storage types are below : ");
	TEST_DEBUG(" 2 - Emergency numbers ");
	TEST_DEBUG(" 3 - Fixed dialing numbers ");
	TEST_DEBUG(" 4-  Last dialled numbers ");
	TEST_DEBUG(" 8 - Own numbers ");
	TEST_DEBUG(" 10 - 2G SIM numbers ");
	TEST_DEBUG(" 11 - Service dialing numbers ");
	TEST_DEBUG(" 12 - 3G SIM numbers ");
	TEST_DEBUG(" 15 - Additional string numbers ");
	TEST_DEBUG(" 16 - Grouping text string ");

	TEST_DEBUG("Please input storage type: ");
	scanf("%d%c", &iStorageType, &dummy);

	switch (iStorageType) {
	case 2: {
		storage_type = TAPI_SIM_PB_EN;
	}
		break;

	case 3: {
		storage_type = TAPI_SIM_PB_FDN;
	}
		break;

	case 4: {
		storage_type = TAPI_SIM_PB_LDN;
	}
		break;

	case 8: {
		storage_type = TAPI_SIM_PB_MSISDN;
	}
		break;

	case 10: {
		storage_type = TAPI_SIM_PB_ADN;
	}
		break;

	case 11: {
		storage_type = TAPI_SIM_PB_SDN;
	}
		break;

	case 12: {
		storage_type = TAPI_SIM_PB_3GSIM;
	}
		break;

	case 15: {
		storage_type = TAPI_SIM_PB_AAS;
	}
		break;

	case 16: {
		storage_type = TAPI_SIM_PB_GAS;
	}
		break;

	default: {
		storage_type = TAPI_SIM_PB_UNKNOWNN;
	}
		break;
	}
	return storage_type;

}

void __PrintStorageType(TelSimPbFileType_t tSimPbFileType) {
	switch (tSimPbFileType) {
	case TAPI_SIM_PB_EN: {
		TEST_DEBUG("TAPI_SIM_PB_EN [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_FDN: {
		TEST_DEBUG("TAPI_SIM_PB_FDN [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_LDN: {
		TEST_DEBUG("TAPI_SIM_PB_LDN [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_MSISDN: {
		TEST_DEBUG("TAPI_SIM_PB_MSISDN [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_ADN: {
		TEST_DEBUG("TAPI_SIM_PB_ADN [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_SDN: {
		TEST_DEBUG("TAPI_SIM_PB_SDN [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_3GSIM: {
		TEST_DEBUG("TAPI_SIM_PB_3GSIM [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_AAS: {
		TEST_DEBUG("TAPI_SIM_PB_AAS [0x%x]", tSimPbFileType);
	}
		break;

	case TAPI_SIM_PB_GAS: {
		TEST_DEBUG("TAPI_SIM_PB_GAS [0x%x]", tSimPbFileType);
	}
		break;

	default: {
		TEST_DEBUG("Undefined Storage Type returnted [0x%x]", tSimPbFileType);
	}
		break;
	}
}

void __PrintPbFiledType(TelSimPb3GFileType_t tSimPb3GFileType) {
	switch (tSimPb3GFileType) {
	case TAPI_PB_3G_NAME: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_NAME [0x%x]", tSimPb3GFileType);
		break;
	}

	case TAPI_PB_3G_NUMBER: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_NUMBER [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_ANR: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_ANR [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_EMAIL: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_EMAIL [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_SNE: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_SNE [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_GRP: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_GRP [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_PBC: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_PBC [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_ANRA: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_ANRA [0x%x]", tSimPb3GFileType);
		break;
	}
	case TAPI_PB_3G_ANRB: {
		TEST_DEBUG("PB Type: TAPI_PB_3G_ANRB [0x%x]", tSimPb3GFileType);
		break;
	}
	default: {
		TEST_DEBUG("PB Type: Undefined PB 3G File Type [0x%x]", tSimPb3GFileType);
	}
	}
}

#if 0
void __PrintLockType(TelSimLockType_t tSimLockType)
{
	switch(tSimLockType)
	{
		case TAPI_SIM_LOCK_READY:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_READY [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_PS:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_PS (Phone-Sim Lock) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_PF:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_PF (Phone first SIM) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_SC:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_SC (PIN, PIN2, PUK, PUK2 Lock)  [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_FD:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_FD (FDN) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_PN:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_PN (Network Personalization) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_PU:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_PU (Network Subset Personalization) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_PP:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_PP (Service Provider Personalization) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_PC:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_PC (Corporate Personalization) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_SC2:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_SC2 (PIN2 Lock) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_SCPUK2:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_SCPUK2 (PIN2 Unblocking Key) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_LOCK_ACL:
		TEST_DEBUG("Lock Type: TAPI_SIM_LOCK_ACL (ACL Lock) [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_NO_SIM:
		TEST_DEBUG("Lock Type: TAPI_SIM_NO_SIM [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_UNAVAIL:
		TEST_DEBUG("Lock Type: TAPI_SIM_UNAVAIL [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_INIT_COMPLETE:
		TEST_DEBUG("Lock Type: TAPI_SIM_INIT_COMPLETE [0x%x]", tSimLockType);
		break;
		case TAPI_SIM_PB_INIT_COMPLETE:
		TEST_DEBUG("Lock Type: TAPI_SIM_PB_INIT_COMPLETE [0x%x]", tSimLockType);
		break;
		default:
		TEST_DEBUG("Lock Type: Undefined Lcok Type [0x%x]", tSimLockType);
	}
}

void __PrintLockKey(TelSimLockKey_t tSimLockKey)
{
	switch(tSimLockKey)
	{
		case TAPI_SIM_LOCK_KEY_NOT_NEED:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_KEY_NOT_NEED [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_KEY_PIN:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_KEY_PIN [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_KEY_PUK:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_KEY_PUK [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_KEY_PIN2:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_KEY_PIN2 [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_KEY_PUK2:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_KEY_PUK2 [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_PERM_BLOCKED:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_PERM_BLOCKED [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_DISABLE:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_DISABLE [0x%x]", tSimLockKey);
		break;
		case TAPI_SIM_LOCK_KEY_SIMLOCK:
		TEST_DEBUG(" Lock Key:  TAPI_SIM_LOCK_KEY_SIMLOCK [0x%x]", tSimLockKey);
		break;
		default:
		TEST_DEBUG(" Lock Key:  Undefined Lock key [0x%x]", tSimLockKey);
	}
}
#endif

void __PrintCardStatus(char* str, TelSimCardStatus_t tSimCardStatus) {
	switch (tSimCardStatus) {
	case TAPI_SIM_STATUS_CARD_ERROR:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_CARD_ERROR [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_CARD_NOT_PRESENT:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_CARD_NOT_PRESENT [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_INITIALIZING:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_INITIALIZING [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_INIT_COMPLETED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_INIT_COMPLETED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_PIN_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_PIN_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_PUK_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_PUK_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_CARD_BLOCKED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_CARD_BLOCKED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_NCK_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_NCK_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_NSCK_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_NSCK_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_SPCK_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_SPCK_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_CCK_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_CCK_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_CARD_REMOVED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_CARD_REMOVED [0x%x]", str, tSimCardStatus)
		;
		break;
	case TAPI_SIM_STATUS_SIM_LOCK_REQUIRED:
		TEST_DEBUG("[%s]TAPI_SIM_STATUS_SIM_LOCK_REQUIRED [0x%x]", str, tSimCardStatus)
		;
		break;
	default:
		TEST_DEBUG("[%s]Undefined Card Status [0x%x]", str, tSimCardStatus)
		;
	}
}

void __PrintPinStatus(char* str, TelSimPinStatus_t tSimPinStatus) {
	switch (tSimPinStatus) {
	case TAPI_SIM_PIN_STATUS_DISABLED:
		TEST_DEBUG("%sTAPI_SIM_PIN_STATUS_DISABLED [0x%x]", str, tSimPinStatus)
		;
		break;
	case TAPI_SIM_PIN_STATUS_ENABLED:
		TEST_DEBUG("%sTAPI_SIM_PIN_STATUS_ENABLED [0x%x]", str, tSimPinStatus)
		;
		break;
	case TAPI_SIM_PIN_STATUS_BLOCKED:
		TEST_DEBUG("%sTAPI_SIM_PIN_STATUS_BLOCKED [0x%x]", str, tSimPinStatus)
		;
		break;
	case TAPI_SIM_PIN_STATUS_PUK_BLOCKED:
		TEST_DEBUG("%sTAPI_SIM_PIN_STATUS_PUK_BLOCKED [0x%x]", str, tSimPinStatus)
		;
		break;
	case TAPI_SIM_PIN_STATUS_UNKNOWN:
		TEST_DEBUG("%sTAPI_SIM_PIN_STATUS_UNKNOWN [0x%x]", str, tSimPinStatus)
		;
		break;
	default:
		TEST_DEBUG("%sUndefined Pin Status [0x%x]", str, tSimPinStatus)
		;
		break;

	}
}

void __PrintCardType(char* str, TelSimCardType_t tSimCardType) {
	switch (tSimCardType) {
	case TAPI_SIM_CARD_TYPE_UNKNOWN:
		TEST_DEBUG("%sTAPI_SIM_CARD_TYPE_UNKNOWN [0x%x]", str, tSimCardType)
		;
		break;
	case TAPI_SIM_CARD_TYPE_GSM:
		TEST_DEBUG("%sTAPI_SIM_CARD_TYPE_GSM [0x%x]", str, tSimCardType)
		;
		break;
	case TAPI_SIM_CARD_TYPE_USIM:
		TEST_DEBUG("%sTAPI_SIM_CARD_TYPE_USIM [0x%x]", str, tSimCardType)
		;
		break;
	default:
		TEST_DEBUG("%sUndefined Card Type [0x%x]", str, tSimCardType)
		;
	}
}

void __PrintECCSvcInfo(TelSimEccEmergencyServiceInfo_t tSimECCSvcInfo) {
	switch (tSimECCSvcInfo) {
	case TAPI_SIM_ECC_ESC_POLICE:
		TEST_DEBUG("TAPI_SIM_ECC_ESC_POLICE [0x%x]", tSimECCSvcInfo)
		;
		break;
	case TAPI_SIM_ECC_ESC_AMBULANCE:
		TEST_DEBUG("TAPI_SIM_ECC_ESC_AMBULANCE [0x%x]", tSimECCSvcInfo)
		;
		break;
	case TAPI_SIM_ECC_ESC_FIREBRIGADE:
		TEST_DEBUG("TAPI_SIM_ECC_ESC_FIREBRIGADE [0x%x]", tSimECCSvcInfo)
		;
		break;
	case TAPI_SIM_ECC_ESC_MARAINEGUARD:
		TEST_DEBUG("TAPI_SIM_ECC_ESC_MARAINEGUARD [0x%x]", tSimECCSvcInfo)
		;
		break;
	case TAPI_SIM_ECC_ESC_MOUTAINRESCUE:
		TEST_DEBUG("TAPI_SIM_ECC_ESC_MOUTAINRESCUE [0x%x]", tSimECCSvcInfo)
		;
		break;
	case TAPI_SIM_ECC_ESC_SPARE:
		TEST_DEBUG("TAPI_SIM_ECC_ESC_SPARE [0x%x]", tSimECCSvcInfo)
		;
		break;
	default:
		TEST_DEBUG("Undefined ECC Sercie Info [0x%x]", tSimECCSvcInfo)
		;
	}
}

void __PrintUSTData(unsigned long UstLen, unsigned short UstData[]) {
	int i = 0;
	fprintf(stderr, "UST Data: ");
	for (i = 0; i < UstLen; i++) {
		fprintf(stderr, "0x%x ", UstData[i]);
	}
	fprintf(stderr, "\n");
}

#if 0
void __PrintUSimSvcTable(TelUSimServiceTable_t tUSimSVCTable)
{
	TEST_DEBUG("-------- UST Table -------- (1: On, 0:Off)");
	TEST_DEBUG("Local Phonebook [%d] ", tUSimSVCTable.bLocalPhoneBook);
	TEST_DEBUG("FDN [%d] ", tUSimSVCTable.bFdn);
	TEST_DEBUG("EXT2 [%d] ", tUSimSVCTable.bExtension2);
	TEST_DEBUG("SDN [%d] ", tUSimSVCTable.bSdn);
	TEST_DEBUG("EXT3 [%d] ", tUSimSVCTable.bExtension3);
	TEST_DEBUG("BDN [%d] ", tUSimSVCTable.bBdn);
	TEST_DEBUG("EXT4 [%d] ", tUSimSVCTable.bExtension4);
	TEST_DEBUG("OUT Going Call Info. [%d] ", tUSimSVCTable.bOCIOCT);
	TEST_DEBUG("Call control [%d] ", tUSimSVCTable.bUsimCallControl);
	TEST_DEBUG("Added more Table as you need");
	TEST_DEBUG("---------------------------");
}

void __PrintSimSvcTable(TelSimServiceTable_t tSimSVCTable)
{
	TEST_DEBUG("-------- UST Table --------");
	TEST_DEBUG("FDN [%d] ", tSimSVCTable.bFdn);
	TEST_DEBUG("EXT2 [%d] ", tSimSVCTable.bExtension2);
	TEST_DEBUG("SDN [%d] ", tSimSVCTable.bSdn);
	TEST_DEBUG("EXT3 [%d] ", tSimSVCTable.bExtension3);
	TEST_DEBUG("BDN [%d] ", tSimSVCTable.bBdn);
	TEST_DEBUG("EXT4 [%d] ", tSimSVCTable.bExtension4);
	TEST_DEBUG("PIN1 Disable func. [%d] ", tSimSVCTable.bPIN1DisableFunction );
	TEST_DEBUG("Adn [%d] ", tSimSVCTable.bAdn);
	TEST_DEBUG("Added more Table as you need");
	TEST_DEBUG("---------------------------");
}
#endif

void __PrintSSTData(unsigned long SstLen, unsigned short SstData[]) {
	int i = 0;
	TEST_DEBUG("SST Data: ");
	for (i = 0; i < SstLen; i++) {
		fprintf(stderr, "0x%x ", SstData[i]);
	}
}

void __PrintLangInfo(char *str, TelSimLanguagePreferenceCode_t tSimLP) {
	switch (tSimLP) {
	case TAPI_SIM_LP_GERMAN:
		TEST_DEBUG("%s TAPI_SIM_LP_GERMAN", str)
		;
		break;

	case TAPI_SIM_LP_ENGLISH:
		TEST_DEBUG("%s TAPI_SIM_LP_ENGLISH", str)
		;
		break;

	case TAPI_SIM_LP_ITALIAN:
		TEST_DEBUG("%s TAPI_SIM_LP_ITALIAN", str)
		;
		break;

	case TAPI_SIM_LP_FRENCH:
		TEST_DEBUG("%s TAPI_SIM_LP_FRENCH", str)
		;
		break;

	case TAPI_SIM_LP_SPANISH:
		TEST_DEBUG("%s TAPI_SIM_LP_SPANISH", str)
		;
		break;

	case TAPI_SIM_LP_DUTCH:
		TEST_DEBUG("%s TAPI_SIM_LP_DUTCH", str)
		;
		break;

	case TAPI_SIM_LP_SWEDISH:
		TEST_DEBUG("%s TAPI_SIM_LP_SWEDISH", str)
		;
		break;

	case TAPI_SIM_LP_DANISH:
		TEST_DEBUG("%s TAPI_SIM_LP_DANISH", str)
		;
		break;

	case TAPI_SIM_LP_PORTUGUESE:
		TEST_DEBUG("%s TAPI_SIM_LP_PORTUGUESE", str)
		;
		break;

	case TAPI_SIM_LP_FINNISH:
		TEST_DEBUG("%s TAPI_SIM_LP_FINNISH", str)
		;
		break;

	case TAPI_SIM_LP_NORWEGIAN:
		TEST_DEBUG("%s TAPI_SIM_LP_NORWEGIAN", str)
		;
		break;

	case TAPI_SIM_LP_GREEK:
		TEST_DEBUG("%s TAPI_SIM_LP_GREEK", str)
		;
		break;

	case TAPI_SIM_LP_TURKISH:
		TEST_DEBUG("%s TAPI_SIM_LP_TURKISH", str)
		;
		break;

	case TAPI_SIM_LP_HUNGARIAN:
		TEST_DEBUG("%s TAPI_SIM_LP_HUNGARIAN", str)
		;
		break;

	case TAPI_SIM_LP_POLISH:
		TEST_DEBUG("%s TAPI_SIM_LP_POLISH", str)
		;
		break;

	case TAPI_SIM_LP_KOREAN:
		TEST_DEBUG("%s TAPI_SIM_LP_KOREAN", str)
		;
		break;

	case TAPI_SIM_LP_CHINESE:
		TEST_DEBUG("%s TAPI_SIM_LP_CHINESE", str)
		;
		break;

	case TAPI_SIM_LP_RUSSIAN:
		TEST_DEBUG("%s TAPI_SIM_LP_RUSSIAN", str)
		;
		break;

	case TAPI_SIM_LP_JAPANESE:
		TEST_DEBUG("%s TAPI_SIM_LP_JAPANESE", str)
		;
		break;

	case TAPI_SIM_LP_LANG_UNSPECIFIED:
	default:
		TEST_DEBUG("%s Undefined Language Preference Code", str)
		;
		break;
	}
}

void __PrintPBResult(char *str, TelSimPbAccessResult_t tPBResult) {
	switch (tPBResult) {
	case TAPI_SIM_PB_SUCCESS:
		TEST_DEBUG("%s TAPI_SIM_PB_SUCCESS", str)
		;
		break;

	case TAPI_SIM_PB_FAIL:
		TEST_DEBUG("%s TAPI_SIM_PB_FAIL", str)
		;
		break;

	case TAPI_SIM_PB_INVALID_INDEX:
		TEST_DEBUG("%s TAPI_SIM_PB_INVALID_INDEX", str)
		;
		break;

	case TAPI_SIM_PB_INVALID_NUMBER_LENGTH:
		TEST_DEBUG("%s TAPI_SIM_PB_INVALID_NUMBER_LENGTH", str)
		;
		break;

	case TAPI_SIM_PB_INVALID_NAME_LENGTH:
		TEST_DEBUG("%s TAPI_SIM_PB_INVALID_NAME_LENGTH", str)
		;
		break;

	case TAPI_SIM_PB_ACCESS_CONDITION_NOT_SATISFIED:
		TEST_DEBUG("%s TAPI_SIM_PB_ACCESS_CONDITION_NOT_SATISFIED", str)
		;
		break;

	default:
		TEST_DEBUG("%s Undefined PB Access Result", str)
		;
		break;
	}
}

void __PrintPBStorageList(char *str, TS_UINT16 StorageList) {
	if (StorageList & (0x01 << TAPI_SIM_PB_FDN)) {
		TEST_DEBUG("%s PB_FDN is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_LDN)) {
		TEST_DEBUG("%s PB_LDN is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_MSISDN)) {
		TEST_DEBUG("%s PB_MSISDN is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_ADN)) {
		TEST_DEBUG("%s PB_ADN is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_SDN)) {
		TEST_DEBUG("%s PB_SDN is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_EN)) {
		TEST_DEBUG("%s PB_EN is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_3GSIM)) {
		TEST_DEBUG("%s PB_3GSIM is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_AAS)) {
		TEST_DEBUG("%s PB_AAS is in the StorageList", str);
	}

	if (StorageList & (0x01 << TAPI_SIM_PB_GAS)) {
		TEST_DEBUG("%s PB_GAS is in the StorageList", str);
	}
}

#if 0
TelSimReadFileRequestData_t __SelectBinary()
{
	int iSelect = 0;
	TelSimReadFileRequestData_t ReadFileReqData = { {0,}};

	ReadFileReqData.Offset = 0; /**< offset if read binary */

	ReadFileReqData.FileType = TAPI_SIM_FTYPE_TRANSPARENT;
	// if record
	ReadFileReqData.RecordIndex =0; /**< record index if read record */
	ReadFileReqData.RecordIndexFrom=0;
	ReadFileReqData.RecordIndexTo=0;
	ReadFileReqData.RecordMode=TAPI_SIM_RECORD_NONE; /**< record index if read record */

	TEST_DEBUG("---- EF File List ----");
	TEST_DEBUG("1: ICCID");
	TEST_DEBUG("2: SPN");
	TEST_DEBUG("----------------------");
	TEST_DEBUG("Select EF File: ");

	scanf("%d", &iSelect);
	if(iSelect == 1)
	{
		ReadFileReqData.FileID.FileName = TAPI_SIM_EFILE_ICCID;
		ReadFileReqData.DataLength = 0x0A; /* hard code 10 bytes */;
	}
	else if(iSelect == 2)
	{
		ReadFileReqData.FileID.FileName = TAPI_SIM_EFILE_SPN;
		ReadFileReqData.DataLength = 17;
	}
	else if(iSelect == 2)
	{
		ReadFileReqData.FileID.FileName = TAPI_SIM_EFILE_LP;
	}
	else
	{
		TEST_DEBUG("Invalid Selection!");
	}

	return ReadFileReqData;

}
#endif

/**
 * This function  is used for encoding a ASCII text into UNICODE text
 *
 * @return		This function returns a TRUE on Success and FALSE on failure
 * @param[in]		text_len -input text lenght
 * @param[in]		text_data - input text data
 * @param[in]	unicode_text_len - unicode converted tex lenght
 * @param[in]	unicode_text - unicode text
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 */
TS_BOOL __encode_text_unicode(char text_len, char *text_data,
		char *unicode_text_len, char *unicode_text) {
	int j, i;
	i = j = 0;

	if (text_len == 0) {
		TEST_DEBUG("Unicode encoding failed - input text string length is 0!");
		return FALSE;
	}
	/*UNicode conversion*/
	/* a  = Ascii is 61 and in Unicode it is 0061 but modem expects 6100 */
	for (j = 0, i = 0; i < text_len * 2; i++) {
		unicode_text[i++] = text_data[j++];
		unicode_text[i] = 0x00;
	}
	*unicode_text_len = text_len * 2;
	return TRUE;

}

/**
 * This function  is used for encoding a ASCII text into GSM7 bit text
 *
 * @return		This function returns a TRUE on Success and FALSE on failure
 * @param[in]		text_len - input text lenght
 * @param[in]		text_data - input text data
 * @param[in]	out_len_max - gsm7bit converted text lenght
 * @param[in]	gsm7bit_text - gsm7bit converted tex data
 * @return		This function returns a TRUE on Success and FALSE on failure
 */
TS_BOOL __encode_gsm_7_bit_chars(char text_len, const char* text_data,
		char *out_len_max, char* gsm7bit_text) {
	int i;
	int pos;
	TS_UINT16 shift = 0;

	for (pos = 0, i = 0; i < text_len; pos++, i++) {
		/* pack the low bits - for single char */
		gsm7bit_text[pos] = text_data[i] >> shift;

		if (i + 1 < text_len) {
			/* pack the high bits using the low bits of the next character */
			gsm7bit_text[pos] |= text_data[i + 1] << (7 - shift);

			shift++;

			if (shift == 7) {
				shift = 0;
				i++;
			}
		}
	}
	*out_len_max = (text_len * 7) / 8;

	/* done */
	return pos;

}

/**
 * This function converts unicode to ascii text
 *
 * @return		This function returns a 1 on Success and -1 on failure
 * @param[in]		phone_pb_type
 * @Interface		Asynchronous.
 * @remark
 * @Refer
 */
TS_BOOL __decode_unicode_to_asci_str(char text_len, char *text_data,
		char *ascii_text_len, char *ascii_text) {
	int i, j;
	i = j = 0;
	if (text_len == 0 || text_data == NULL || ascii_text == NULL) {
		TEST_DEBUG("Unicode Decode Failed as text length is 0");
		return FALSE;
	}

	if (0 != (text_len % 2)) {
		TEST_DEBUG(" ##### Unicode decoding failed due to invalid text length [%d]", text_len );
		return FALSE;
	}

	for (i = 0; i < text_len; i++) {
		ascii_text[i] = text_data[j];
		j = j + 2;
	}
	*ascii_text_len = (text_len / 2);
	ascii_text[i] = '\0';
	return TRUE;
}

int sim_test_subscribe_tapi_events() {
	int i = 0;
	int ret_val = TRUE;
	TapiResult_t api_err = TAPI_API_SUCCESS;

	int SIMEvtList[] = { TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF,
			TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF,
			TAPI_EVENT_SIM_PB_ACCESS_READ_CNF,

			TAPI_EVENT_SIM_PB_UPDATE_CNF, TAPI_EVENT_SIM_PB_DELETE_CNF,
			TAPI_EVENT_SIM_STATUS_IND, TAPI_EVENT_SIM_FDN_STATUS_IND,

			TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF,
			TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF,
			TAPI_EVENT_SIM_SAP_CONNECT_NOTI, TAPI_EVENT_SIM_SAP_CONNECT_CNF,
			TAPI_EVENT_SIM_SAP_CONNECT_STATUS_CNF,
			TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF,
			TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF,
			TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF,
			TAPI_EVENT_SIM_SAP_SET_SIM_POWER_CNF,
			TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF, TAPI_EVENT_SIM_APDU_CNF,
			TAPI_EVENT_SIM_ATR_CNF,

			TAPI_EVENT_SIM_GET_INDICATION_CNF,
			TAPI_EVENT_SIM_SET_INDICATION_CNF, TAPI_EVENT_SIM_GET_MAILBOX_CNF,
			TAPI_EVENT_SIM_SET_MAILBOX_CNF, TAPI_EVENT_SIM_SET_LANGUAGE_CNF,
			TAPI_EVENT_SIM_RSIM_ACCESS_CNF, TAPI_EVENT_SIM_VERIFY_SEC_CNF,
			TAPI_EVENT_SIM_VERIFY_PUK_CNF, TAPI_EVENT_SIM_CHANGE_PINS_CNF,
			TAPI_EVENT_SIM_DISABLE_SEC_CNF, TAPI_EVENT_SIM_ENABLE_SEC_CNF,
			TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF,
			TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF, TAPI_EVENT_SIM_PERS_STATUS_CNF,
			TAPI_EVENT_SIM_DISABLE_PERS_CNF, TAPI_EVENT_SIM_ENABLE_PERS_CNF };

	iNumOfSIMEvt = sizeof(SIMEvtList) / sizeof(int);
	TEST_DEBUG("Number of SIM Event List to be registered: %d", iNumOfSIMEvt);
	//TAPI_CALLOC(SIM_subscription_id, iNumOfSIMEvt, unsigned int)
	//#define TAPI_CALLOC(ptr, no_elements, type)	s
	SIM_subscription_id = (unsigned int *) calloc(iNumOfSIMEvt,
			sizeof(unsigned int));
	if (SIM_subscription_id == NULL) {
		TEST_DEBUG("calloc error -NULL, errno is [%d]", errno);
	}

	for (i = 0; i < iNumOfSIMEvt; i++) {
		api_err = tel_register_event(SIMEvtList[i], &SIM_subscription_id[i],
				(TelAppCallback) &sim_async_event_callback, NULL);

		if (api_err != TAPI_API_SUCCESS) {
			TEST_DEBUG("isn't subscribed. sub id is [%d], api_err is [%d]\n", SIM_subscription_id[i],api_err);
			ret_val = FALSE;
			break;
		}
	}

	return ret_val;

}

int sim_test_unsubscribe_tapi_events(void) {
	int i = 0;
	TapiResult_t api_err = TAPI_API_SUCCESS;
	int ret_val = TRUE;

	for (i = 0; i < iNumOfSIMEvt; i++) {
		api_err = tel_deregister_event(SIM_subscription_id[i]);
		if (api_err != TAPI_API_SUCCESS) {
			TEST_DEBUG("TelTapiDeRegister isn't unsubscribed. sub id is [%d] api_err is [%d]\n",SIM_subscription_id[i],api_err);
			ret_val = FALSE;
			break;
		}

	}

	return ret_val;
}

void sim_async_event_callback(TelTapiEvent_t* sim_event)
{
	//int i;
	//static unsigned int fileSize = 0;

	TEST_DEBUG("Func Entrance");
	TEST_DEBUG("Request ID : [0x%x]\n", sim_event->RequestId);

	if (sim_event->pData == NULL) {
		TEST_DEBUG("No Event Data!!");
		//		return;
	}

	TEST_DEBUG("*************** Callback data **********************");
	TEST_DEBUG("EventClass [%d],EventType [0x%x], Reques Id[%d], Event status [%d], sim_event->pDataLen[%d]",
			sim_event->EventClass, sim_event->EventType, sim_event->RequestId, sim_event->Status,sim_event->pDataLen);
#if 0
	if(sim_event->Status != TAPI_SIM_PB_SUCCESS)
	{
		__PrintPBResult("SIM Result", sim_event->Status);
		TEST_DEBUG("TAPI Fail: EventStatus [%d]", sim_event->Status);
		return;
	}
#endif

	if (sim_event->EventClass == TAPI_EVENT_CLASS_SIM) {
		switch (sim_event->EventType) {
		case TAPI_EVENT_SIM_STATUS_IND: {
			TEST_DEBUG("***********TAPI_EVENT_SIM_STATUS_IND**************");

			//	TEST_DEBUG("Event status [%d]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_STATUS_CARD_ERROR) {
				TEST_DEBUG("SIM CARD ERROR");
			} else if (sim_event->Status == TAPI_SIM_STATUS_CARD_NOT_PRESENT) {
				TEST_DEBUG("SIM CARD NOT INSERTED");
			} else if (sim_event->Status == TAPI_SIM_STATUS_SIM_INITIALIZING) {
				TelSimCardType_t CardType =
						*((TelSimCardType_t*) (sim_event->pData));

				TEST_DEBUG("SIM INITITIALIZING");
				__PrintCardType("CardType: ", CardType);
			} else if (sim_event->Status == TAPI_SIM_STATUS_SIM_INIT_COMPLETED) {
				TEST_DEBUG("SIM INIT COMPLETED");
			} else if (sim_event->Status == TAPI_SIM_STATUS_SIM_PIN_REQUIRED) {
				TEST_DEBUG("SIM PIN REQUIRED");
			} else if (sim_event->Status == TAPI_SIM_STATUS_SIM_PUK_REQUIRED) {
				TEST_DEBUG("SIM PUK REQUIRED");
			} else if (sim_event->Status == TAPI_SIM_STATUS_CARD_BLOCKED) {
				TEST_DEBUG("SIM CARD BLOCKED");
			}
			TEST_DEBUG("********************************************");

		}
			break;

		case TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF: {
			TEST_DEBUG("*****************TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF***************************");

			//TEST_DEBUG("SIM PB Capability Info event status = [0x%x]", sim_event->Status);

			TelSimPbCapabilityInfo_t *sim_capability_info =
					(TelSimPbCapabilityInfo_t*) sim_event->pData;

			int i;
			for (i = 0; i < sim_capability_info->FileTypeCount; i++) {
				__PrintPbFiledType(
						sim_capability_info->FileTypeInfo[i].FileType3G);
				// TEST_DEBUG(" Filed tag type [%d]", sim_capability_info->FileTypeInfo[i].FileType3G);
				TEST_DEBUG(" max index [%d]", sim_capability_info->FileTypeInfo[i].PbIndexMax);
				TEST_DEBUG(" max entry [%d]", sim_capability_info->FileTypeInfo[i].PbTextMax);
				TEST_DEBUG(" used count [%d]", sim_capability_info->FileTypeInfo[i].TotalRecordCount);
			}

			TEST_DEBUG("****************************************************");

		}
			break;

		case TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF: {
			TEST_DEBUG("*****TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF*****");

			TelSimPbStorageInfo_t *storageInfo =
					(TelSimPbStorageInfo_t*) sim_event->pData;

			//TEST_DEBUG("SIM PB storage count event status = [0x%x]", sim_event->Status);


			__PrintStorageType(storageInfo->StorageFileType);
			//TEST_DEBUG("PB_STORAGE_TYPE 0x%x", storageInfo->StorageFileType);
			TEST_DEBUG("TOTAL_COUNT = %d", storageInfo->TotalRecordCount);
			TEST_DEBUG("USED_COUNT =  %d", storageInfo->UsedRecordCount);

			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF: {
			TEST_DEBUG("**********TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF********");

			TelSimPbEntryInfo_t *entryInfo =
					(TelSimPbEntryInfo_t*) sim_event->pData;

			//TEST_DEBUG("SIM PB entry info event status = [0x%x]", sim_event->Status);

			//TEST_DEBUG("STORAGE TYPE [%d]", entryInfo->StorageFileType);
			__PrintStorageType(entryInfo->StorageFileType);
			TEST_DEBUG("PB_INDEX_MIN %d", entryInfo->PbIndexMin);
			TEST_DEBUG("PB_INDEX_MAX = %d", entryInfo->PbIndexMax);
			TEST_DEBUG("PB_NUM_LEN_MAX =  %d", entryInfo->PbNumLenMax);
			TEST_DEBUG("PB_TEXT_LEN_MAX =  %d", entryInfo->PbTextLenMax);

			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_PB_ACCESS_READ_CNF: {
			TEST_DEBUG("*********TAPI_EVENT_SIM_PB_ACCESS_READ_CNF*********");

			//TEST_DEBUG("SIM PB access read event status = [0x%x]", sim_event->Status);

			TelSimPbRecordData_t *sim_acces_info =
					(TelSimPbRecordData_t*) sim_event->pData;

			__PrintStorageType(sim_acces_info->StorageFileType);
			//TEST_DEBUG("storaget type is [0x%x]",sim_acces_info->StorageFileType);

			if (sim_event->Status != TAPI_SIM_PB_SUCCESS) {
				TEST_DEBUG(" SIM phone book access error [%d]", sim_event->Status);
				break;
			}

			int ascii_text_len = 0;
			char ascii_text[256] = { 0, };

			if (sim_acces_info->StorageFileType == TAPI_SIM_PB_EN
					|| sim_acces_info->StorageFileType == TAPI_SIM_PB_FDN
					|| sim_acces_info->StorageFileType == TAPI_SIM_PB_LDN
					|| sim_acces_info->StorageFileType == TAPI_SIM_PB_MSISDN
					|| sim_acces_info->StorageFileType == TAPI_SIM_PB_ADN
					|| sim_acces_info->StorageFileType == TAPI_SIM_PB_SDN) {
				TEST_DEBUG(" 2G Type SIM phone book access");
				TEST_DEBUG(" sim_acces_info->StorageFileType[%d](0-fdn,1-ldn,2-msisdn,3-adn,4-sdn,5-en), number[%s]",sim_acces_info->StorageFileType, (char *)sim_acces_info->ContactInfo.Pb2GData.Number);

				/* unicode decoding */
				if (sim_acces_info->ContactInfo.Pb2GData.NameEncryptType
						== TAPI_SIM_TEXT_ENC_UCS2) {
					TEST_DEBUG("Decoding UCS2 to ASCII");
					if (FALSE
							== __decode_unicode_to_asci_str(
									sim_acces_info->ContactInfo.Pb2GData.NameLen,
									(char *) &sim_acces_info->ContactInfo.Pb2GData.Name,
									(char *) &ascii_text_len, ascii_text)) {
						TEST_DEBUG(" Deocing to ascii failed ");
						break;
					}
				} else if (sim_acces_info->ContactInfo.Pb2GData.NameEncryptType
						== TAPI_SIM_TEXT_ENC_ASCII
						|| sim_acces_info->ContactInfo.Pb2GData.NameEncryptType
								== TAPI_SIM_TEXT_ENC_GSM7BIT) {
					//TAPI_MEMCPY(ascii_text, sim_acces_info->ContactInfo.Pb2GData.Name, 256, TAPI_SIM_NUM_TEXT_LEN_MAX);
					//#define TAPI_MEMCPY(dest , src , dest_len , src_len)
					if (256 >= TAPI_SIM_NUM_TEXT_LEN_MAX) {
						memcpy(ascii_text,
								sim_acces_info->ContactInfo.Pb2GData.Name,
								TAPI_SIM_NUM_TEXT_LEN_MAX);
					} else {
						TEST_DEBUG("memcpy FAILURE - dest_len(%d) < src_len(%d)", 256, TAPI_SIM_NUM_TEXT_LEN_MAX);
					}
				} else {
					TEST_DEBUG("Name Encryption is not UCS2 / ASCII / unpacked GSM7bit");
					break;
				}
				TEST_DEBUG("text[%s]", ascii_text);
			} else if (sim_acces_info->StorageFileType == TAPI_SIM_PB_3GSIM) {
				int i, k;
				int tmp_size;
				int num_of_pb_rec = 0;

				TEST_DEBUG(" sim_event->pDataLen [%d]",sim_event->pDataLen);
				TEST_DEBUG(" sizeof(TelSimPbRecordData_t)[%d]",sizeof(TelSimPbRecordData_t));
				TEST_DEBUG(" strlen(sim_event->pData)[%d]", strlen(sim_event->pData));

				num_of_pb_rec = (sim_event->pDataLen
						/ sizeof(TelSimPbRecordData_t));

				TEST_DEBUG("Number of Saved Records [%d]",num_of_pb_rec);

				char * temp = (char*) malloc(sim_event->pDataLen);
				memcpy(temp, (char*) sim_event->pData, sim_event->pDataLen);

				TelSimPbRecordData_t *sim_3g_access_info;

				for (k = 0; k < num_of_pb_rec; k++) {
					tmp_size = k * sizeof(TelSimPbRecordData_t);
					sim_3g_access_info = (TelSimPbRecordData_t*) (temp
							+ tmp_size);

					TEST_DEBUG("****************************************************");
					TEST_DEBUG(" Record Num = [%d], index = [%d], next index = [%d] ",k , sim_3g_access_info->Index,sim_3g_access_info->NextIndex);

					TEST_DEBUG(" Num of Fields = [%d]",sim_3g_access_info->ContactInfo.Pb3GData.FileTypeCount);
					for (i = 0; i
							< sim_3g_access_info->ContactInfo.Pb3GData.FileTypeCount; i++) {
						TEST_DEBUG("-------------------------------------");
						TEST_DEBUG("Field_type = [%d], FiledLen = [%d]",
								sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileType,
								sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength);

						if (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
								== TAPI_PB_3G_GRP) {
							int grp_index;
							TEST_DEBUG("Number of groups [%d]",sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength );

							for (grp_index = 0; grp_index
									< (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength); grp_index++) {
								TEST_DEBUG(" Groupd Index [%d]",sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData[grp_index]); //ignore data_type byte

							}

						} else if (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
								== TAPI_PB_3G_NAME
								|| sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
										== TAPI_PB_3G_EMAIL
								|| sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
										== TAPI_PB_3G_SNE) {
							TEST_DEBUG("3G Name or Email or SNE");
							if (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.EncryptionType
									== TAPI_SIM_TEXT_ENC_UCS2) {
								TEST_DEBUG("EncryptionType: UCS2");
								if (FALSE
										== __decode_unicode_to_asci_str(
												sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength,
												(char *) &sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
												(char *) &ascii_text_len,
												ascii_text)) {
									TEST_DEBUG("Deocing ucs2 to ascii failed ");
									break;
								}
							} else if (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.EncryptionType
									== TAPI_SIM_TEXT_ENC_ASCII) {
								TEST_DEBUG("EncryptionType: ASCII");
								//TAPI_MEMCPY(ascii_text, sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData, 256, TAPI_SIM_NUM_TEXT_LEN_MAX);
								//#define TAPI_MEMCPY(dest , src , dest_len , src_len)
								if (256 >= TAPI_SIM_NUM_TEXT_LEN_MAX) {
									memcpy(
											ascii_text,
											sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
											TAPI_SIM_NUM_TEXT_LEN_MAX);
								} else {
									TEST_DEBUG("memcpy FAILURE - dest_len(%d) < src_len(%d)", 256, TAPI_SIM_NUM_TEXT_LEN_MAX);
								}

							} else if (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.EncryptionType
									== TAPI_SIM_TEXT_ENC_GSM7BIT) {
								TEST_DEBUG("EncryptionType: GSM7 bit");
								//TAPI_MEMCPY(ascii_text, sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData, 256, TAPI_SIM_NUM_TEXT_LEN_MAX);
								//#define TAPI_MEMCPY(dest , src , dest_len , src_len)
								if (256 >= TAPI_SIM_NUM_TEXT_LEN_MAX) {
									memcpy(
											ascii_text,
											sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
											TAPI_SIM_NUM_TEXT_LEN_MAX);
								} else {
									TEST_DEBUG("memcpy FAILURE - dest_len(%d) < src_len(%d)", 256, TAPI_SIM_NUM_TEXT_LEN_MAX);
								}

							} else {
								TEST_DEBUG("Name Encryption is not UCS2 / ASCII / unpacked GSM7bit");
								break;
							}

							TEST_DEBUG("Field_data[%s]", ascii_text);

						} else {
							TEST_DEBUG("Number Type[%d]", sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.NumType);
							TEST_DEBUG("AAS RecID [0x%x]", sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].AasRecordIdentifier);

							if (sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
									== TAPI_PB_3G_ANR) {
								TEST_DEBUG("3G ANR Type");
							} else {
								TEST_DEBUG("3G Number Type");
							}
							TEST_DEBUG("Number_data[%s]",sim_3g_access_info->ContactInfo.Pb3GData.PbFileDataInfo[i].FileData);
						}
					}
				}

			} else if (sim_acces_info->StorageFileType == TAPI_SIM_PB_AAS
					|| sim_acces_info->StorageFileType == TAPI_SIM_PB_GAS) {
				TEST_DEBUG(" AAS and GAS");
				TEST_DEBUG(" index = [%d]",sim_acces_info->Index);
				__PrintStorageType(sim_acces_info->StorageFileType);
				TEST_DEBUG(" str len = [%d]",sim_acces_info->ContactInfo.PbAdditionalNumStringData.TextLength);
				TEST_DEBUG(" encrypt type = [%d]",sim_acces_info->ContactInfo.PbAdditionalNumStringData.EncryptionType);
				TEST_DEBUG(" string = [%s]",sim_acces_info->ContactInfo.PbAdditionalNumStringData.TextData);
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_PB_UPDATE_CNF: {
			TEST_DEBUG("*****************TAPI_EVENT_SIM_PB_UPDATE_CNF***************************");

			TelSimPbUpdateResp_t *pUpdateResp =
					(TelSimPbUpdateResp_t*) sim_event->pData;

			__PrintStorageType(pUpdateResp->PbStorageInfo.StorageFileType);
			TEST_DEBUG("Storage Info Total Record cnt [%d]", pUpdateResp->PbStorageInfo.TotalRecordCount);
			TEST_DEBUG("Storage File Used Record Cnt [%d]", pUpdateResp->PbStorageInfo.UsedRecordCount);
			//TEST_DEBUG(" Storage File Type [%d] ", pUpdateResp->PbStorageInfo.StorageFileType);
			TEST_DEBUG("Storage File Index [%d]", pUpdateResp->PbIndex);
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_PB_DELETE_CNF: {
			TEST_DEBUG("*****************TAPI_EVENT_SIM_PB_DELETE_CNF***************************");

			TEST_DEBUG("SIM Delete PB event status = [0x%x]", sim_event->Status);

			TelSimPbUpdateResp_t *pDeleteResp =
					(TelSimPbUpdateResp_t*) sim_event->pData;
			TEST_DEBUG(" Storage File Type [%d] ", pDeleteResp->PbStorageInfo.StorageFileType);
			TEST_DEBUG(" Storage File Index [%d]", pDeleteResp->PbIndex);

			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_FDN_STATUS_IND: {
			TEST_DEBUG("*****************TAPI_EVENT_SIM_FDN_STATUS_IND***************************");

			TEST_DEBUG("SIM FDN status event status = [0x%x]", sim_event->Status);

			TEST_DEBUG("********************************************");

		}
			break;

			/** will be tested later **/
#if 1
		case TAPI_EVENT_SIM_AUTHENTICATION_CNF: {
			TEST_DEBUG("*****************IMS Authentication **********************");
#if 0
			TelSimAuthenticationResponse_t *auth_resp =
					(TelSimAuthenticationResponse_t*) sim_event->pData;

			TEST_DEBUG("SIM ISIM authentication event status = [0x%x]", sim_event->Status);

			TEST_DEBUG(" auth resp data [%s]",auth_resp->ResponeData);
			TEST_DEBUG("  auth string [%s]",auth_resp->AuthenticationString);
			TEST_DEBUG(" auth cipher key [%s]",auth_resp->CipherKey);
			TEST_DEBUG(" auth integrity key [%s]",auth_resp->IntegrityKey);
#endif
		}
			break;

		case TAPI_EVENT_SIM_SAP_CONNECT_CNF: {
			TEST_DEBUG("*********** SAP CONNECT RESP *************");

			TelSimSapConnect_t *sap_conn_resp =
					(TelSimSapConnect_t*) sim_event->pData;

			TEST_DEBUG("SIM SAP connect request event status  = [0x%x]", sim_event->Status);

			TEST_DEBUG(" message id [0x%x]", sap_conn_resp->MsgId);
			TEST_DEBUG("connection status [0x%x] ", sap_conn_resp->ConnectionStatus);
			TEST_DEBUG("max message size [%d]", sap_conn_resp->MaxMsgSize);

			TEST_DEBUG("*****************************************");
		}
			break;

		case TAPI_EVENT_SIM_SAP_CONNECT_STATUS_CNF: {
			TEST_DEBUG("*********** SAP CONNECT STATUS *************");

			TEST_DEBUG("SIM SAP connect event status = [0x%x]", sim_event->Status);

			TelSimSapStatusInfo_t *sap_conn_status =
					(TelSimSapStatusInfo_t*) sim_event->pData;
			TEST_DEBUG(" connection status info [0x%x]", *sap_conn_status);

			TEST_DEBUG("*****************************************");
		}
			break;

		case TAPI_EVENT_SIM_SAP_TRANSFER_ATR_CNF: {
			TEST_DEBUG("*********** SAP TRANSFER ATR *************");

			int i = 0;
			TelSimSapAtrInfo_t *sap_transfer_atr =
					(TelSimSapAtrInfo_t*) sim_event->pData;

			TEST_DEBUG("SIM SAP trasfer ATR event status  = [0x%x]", sim_event->Status);

			TEST_DEBUG(" ATR result [0x%x]", sap_transfer_atr->AtrResult);
			TEST_DEBUG(" ATR length [%lu]", sap_transfer_atr->AtrLength);
			TEST_DEBUG(" ATR data ---> " );
			for (i = 0; i < sap_transfer_atr->AtrLength; i++)
				TEST_DEBUG(" [0x%x]", sap_transfer_atr->AtrData[i]);

			TEST_DEBUG("*****************************************");
		}
			break;

		case TAPI_EVENT_SIM_SAP_TRANSFER_APDU_CNF: {
			TEST_DEBUG("*********** SAP TRANSFER APDU *************");

			int i = 0;
			TelSimSapApduData_t *sap_transfer_apdu =
					(TelSimSapApduData_t*) sim_event->pData;

			TEST_DEBUG("SIM SAP trasfer APDU event status  = [0x%x]", sim_event->Status);

			TEST_DEBUG(" APDU  length [0x%x]", sap_transfer_apdu->ApduLength);
			TEST_DEBUG(" APDU  data ---> " );
			for (i = 0; i < sap_transfer_apdu->ApduLength; i++)
				TEST_DEBUG(" [0x%x]", sap_transfer_apdu->Apdu[i]);

			TEST_DEBUG("*****************************************");
		}
			break;

		case TAPI_EVENT_SIM_SAP_SET_PROTOCOL_CNF: {
			TEST_DEBUG("*********** SAP SET PROTOCOL  *************");

			TelSimSapProtocol_t *sap_result_code =
					(TelSimSapProtocol_t*) sim_event->pData;

			TEST_DEBUG("SIM SAP set protocol event status  = [0x%x]", sim_event->Status);
			TEST_DEBUG("SAP SET protocol  result [0x%x]", *sap_result_code );

			TEST_DEBUG("*****************************************");
		}
			break;

		case TAPI_EVENT_SIM_SAP_SET_SIM_POWER_CNF: {
			TEST_DEBUG("*********** SAP SET SIM POWER ON/OFF  *************");

			TelSimSapPower_t *sap_sim_power =
					(TelSimSapPower_t*) sim_event->pData;

			TEST_DEBUG("SIM SAP set SIM power  event status  = [0x%x]", sim_event->Status);

			TEST_DEBUG("message id [0x%x]", sap_sim_power->MsgId);
			TEST_DEBUG(" sim power on/off result [0x%x]", sap_sim_power->SimPowerResult);

			TEST_DEBUG("*****************************************");
		}
			break;

		case TAPI_EVENT_SIM_SAP_CARD_READER_STATUS_CNF: {
			TEST_DEBUG("*********** SAP CARD READER STATUS *************");

			TelSimCardReaderStatus_t *sap_card_reader_status =
					(TelSimCardReaderStatus_t*) sim_event->pData;

			TEST_DEBUG("SIM SAP card reader event status  = [0x%x]", sim_event->Status);

			TEST_DEBUG(" card reader result [0x%x] ", sap_card_reader_status->CardReaderResult);
			TEST_DEBUG(" card reader status bitwise encoded data [%d]", sap_card_reader_status->CardReaderStatus);

			TEST_DEBUG("*****************************************");
		}
			break;
#endif

			/******************************************************************************************************************************/
			/*										NEW EVENT										*/
			/******************************************************************************************************************************/
		case TAPI_EVENT_SIM_RSIM_ACCESS_CNF: {
			TEST_DEBUG("********NEW*TAPI_EVENT_SIM_RSIM_ACCESS_CNF**********");

			TelSimReadFileRaw_t * rsim_resp =
					(TelSimReadFileRaw_t*) sim_event->pData;

			if (sim_event->Status == TAPI_SIM_ACCESS_SUCCESS) {
				TEST_DEBUG("rsim_resp->sw1[0x%x]", rsim_resp->sw1);
				TEST_DEBUG("rsim_resp->sw2[0x%x]", rsim_resp->sw2);
				TEST_DEBUG("rsim_resp->DataLen[%d]", rsim_resp->DataLen);
				TEST_DEBUG("rsim_resp->Data[%s]", rsim_resp->Data);
			} else {
				TEST_DEBUG("OPERATION FAILED");
			}

			TEST_DEBUG("********NEW*TAPI_EVENT_SIM_RSIM_ACCESS_CNF**********");
		}
			break;

		case TAPI_EVENT_SIM_VERIFY_SEC_CNF: {
			TEST_DEBUG("********NEW*TAPI_EVENT_SIM_VERIFY_SEC_CNF**********");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM Verify PIN event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_OPERATION_TIMEOUT) {
				TEST_DEBUG("TAPI SIM Operation Timeout!!");
			} else if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("PIN1 Verification Success!");
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM) {
					TEST_DEBUG("PIN1 Verification Success!");
				} else {
					TEST_DEBUG("PIN2 Vefication Success!");
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("PIN1 Verification Failed! - PIN Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1) {
					TEST_DEBUG("PIN1 Verification Failed! - PUK Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM) {
					TEST_DEBUG("SIM Lock Verification Failed! - SIM Lock code Required");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				}
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_VERIFY_PUK_CNF: {
			TEST_DEBUG("**********NEW*TAPI_EVENT_SIM_VERIFY_PUK_CNF********");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM Unblock PIN event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("Unblock PIN1 Success!");
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("Unblock PIN2 Success!");
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("PIN1 Verification Failed! - PIN Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1) {
					TEST_DEBUG("PIN1 Verification Failed! - PUK Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				}
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_CHANGE_PINS_CNF: {
			TEST_DEBUG("********NEW*TAPI_EVENT_SIM_CHANGE_PINS_CNF***************************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM Change PIN event status = [0x%x], pinType[%d]", sim_event->Status, pPinInfo->PinType);
			TEST_DEBUG("PinType[%d]", pPinInfo->type);

			if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("Change PIN1 Success!");
				} else {
					TEST_DEBUG("Change PIN2 Success!");
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("PIN1 Verification Failed! - PIN Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1) {
					TEST_DEBUG("PIN1 Verification Failed! - PUK Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				}
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_DISABLE_SEC_CNF: {
			TEST_DEBUG("****************NEW*TAPI_EVENT_SIM_DISABLE_SEC_CNF****************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM Disable SEC event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("Disable PIN1 Success!");
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("Disable PIN2 Success!");
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM) {
					TEST_DEBUG("Disable SIM LOCK Success!");
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("PIN1 Verification Failed! - PIN1Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1) {
					TEST_DEBUG("PIN1 Verification Failed! - PUK1 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM) {
					TEST_DEBUG("SIM LOCK Verification Failed! - SIM LOCK CODE Required ");
					TEST_DEBUG("Remainint attempts [%d] - Useless value", pPinInfo->retry_count);
				}
			}
			TEST_DEBUG("********************************************");

		}
			break;

		case TAPI_EVENT_SIM_ENABLE_SEC_CNF: {
			TEST_DEBUG("****************NEW*TAPI_EVENT_SIM_ENABLE_SEC_CNF****************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM ENABLE SEC event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("Enable PIN1 Success!");
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("Enable PIN2 Success!");
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM) {
					TEST_DEBUG("Enable SIM LOCK Success!");
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1) {
					TEST_DEBUG("PIN1 Verification Failed! - PIN1Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1) {
					TEST_DEBUG("PIN1 Verification Failed! - PUK1 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM) {
					TEST_DEBUG("SIM LOCK Verification Failed! - SIM LOCK CODE Required ");
					TEST_DEBUG("Remainint attempts [%d] - Useless value", pPinInfo->retry_count);
				}
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF: {
			TEST_DEBUG("****************NEW*TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF****************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM Disable FDN event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("Disable FDN Success!");
				} else {
					TEST_DEBUG("Unhandled type[%d]", pPinInfo->type);
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else {
					TEST_DEBUG("Unhandled type[%d]", pPinInfo->type);
				}
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF: {
			TEST_DEBUG("****************NEW*TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF****************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) sim_event->pData;

			//TEST_DEBUG("SIM Enable FDN event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("Enable FDN Success!");
				} else {
					TEST_DEBUG("Unhandled type[%d]", pPinInfo->type);
				}
			} else {
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2) {
					TEST_DEBUG("PIN2 Verification Failed! - PIN2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2) {
					TEST_DEBUG("PIN2 Verification Failed! - PUK2 Required ");
					TEST_DEBUG("Remainint attempts [%d]", pPinInfo->retry_count);
				} else {
					TEST_DEBUG("Unhandled type[%d]", pPinInfo->type);
				}
			}
			TEST_DEBUG("********************************************");
		}
			break;

		case TAPI_EVENT_SIM_PERS_STATUS_CNF: {
			TelSimPersStatus_t* pers_status =
					(TelSimPersStatus_t*) sim_event->pData;

			printf(
					"[SIM APP]**********NEW* TAPI_EVENT_SIM_PERS_STATUS_CNF************\n");
			printf("[SIM APP]SIM lock personalisation event status = [%x]\n",
					sim_event->Status);
			printf(
					"[SIM APP]SIM lock personalisation status - net0-ns1-sp2-cp3  = [%d]\n",
					pers_status->type);
			printf(
					"[SIM APP]SIM lock personalisation status - unlock0-lock1  = [%d]\n",
					pers_status->mode);
			printf(
					"[SIM APP]***************************************************\n");
		}
			break;

		case TAPI_EVENT_SIM_DISABLE_PERS_CNF: {
			TelSimPinOperationResult_t opResult =
					(TelSimPinOperationResult_t) sim_event->Status;

			printf(
					"[SIM APP]*********NEW*TAPI_EVENT_SIM_DISABLE_PERS_CNF************\n");
			printf("[SIM APP]Eable Personalization event status = [%x]\n",
					opResult);

			if (opResult == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				printf("[SIM APP]Disable Personalization Success!\n");
			} else {
				printf(
						"[SIM APP]Verification Failed! - Correct Password Required\n");
			}
			printf(
					"\n***************************************************************\n");
		}
			break;

		case TAPI_EVENT_SIM_ENABLE_PERS_CNF: {
			TelSimPinOperationResult_t opResult =
					(TelSimPinOperationResult_t) sim_event->Status;

			printf(
					"[SIM APP]*********NEW*TAPI_EVENT_SIM_ENABLE_PERS_CNF************\n");
			printf("[SIM APP]Eable Personalization event status = [%x]\n",
					opResult);

			if (opResult == TAPI_SIM_PIN_OPERATION_SUCCESS) {
				printf("[SIM APP]Enable Personalization Success!\n");
			} else {
				printf(
						"[SIM APP]Verification Failed! - Correct Password Required\n");
			}
			printf(
					"\n***************************************************************\n");
		}
			break;

		case TAPI_EVENT_SIM_SET_LANGUAGE_CNF: {
			TEST_DEBUG("*********NEW*TAPI_EVENT_SIM_SET_LANGUAGE_CNF************");
			TEST_DEBUG("update event status = [0x%x]", sim_event->Status);
			TEST_DEBUG("**********************************************************");
		}
			break;

		case TAPI_EVENT_SIM_APDU_CNF: {
			TelSimApduResp_t* apdu_resp = (TelSimApduResp_t*) sim_event->pData;
			int i = 0;
			TEST_DEBUG("*********NEW*TAPI_EVENT_SIM_APDU_CNF************");
			TEST_DEBUG("update event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == 1234) {
				TEST_DEBUG("timeout happended. this means, modem didn`t reply");
				return;
			}

			if (apdu_resp != NULL) {
				TEST_DEBUG("apdu_resp->apdu_resp_len[%d]",apdu_resp->apdu_resp_len);

				for (i = 0; i < apdu_resp->apdu_resp_len; i++)
					TEST_DEBUG("apdu_resp->apdu_resp[%d]=[0x%x]",i,apdu_resp->apdu_resp[i]);

			} else {
				TEST_DEBUG("atr_resp == NULL");
			}
			TEST_DEBUG("**********************************************************");;
		}
			break;

		case TAPI_EVENT_SIM_ATR_CNF: {
			TelSimAtrResp_t* atr_resp = (TelSimAtrResp_t*) sim_event->pData;
			int i = 0;

			TEST_DEBUG("*********NEW*TAPI_EVENT_SIM_ATR_CNF************");
			TEST_DEBUG("update event status = [0x%x]", sim_event->Status);

			if (sim_event->Status == 1234) {
				TEST_DEBUG("timeout happended. this means, modem didn`t reply");
				return;
			}

			if (atr_resp != NULL) {
				TEST_DEBUG("atr_resp->atr_resp_len[%d]",atr_resp->atr_resp_len);

				for (i = 0; i < atr_resp->atr_resp_len; i++)
					TEST_DEBUG("atr_resp->atr_resp[%d]=[0x%x]",i,atr_resp->atr_resp[i]);
			} else {
				TEST_DEBUG("atr_resp == NULL");
			}
			TEST_DEBUG("**********************************************************");
		}
			break;

#if 0

			case TAPI_SIM_EVENT_PB_GET_STORAGE_LIST:
			{
				TEST_DEBUG("****************TAPI_SIM_EVENT_PB_GET_STORAGE_LIST****************************");
				char *p = (char*)&sim_event->pData;

				TEST_DEBUG("SIM phone book storage list Information ");
				if(*p & 0x01)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_DC - ME Dailled Call list");
				if(*p & 0x02)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_EN - Emergency Number");
				if(*p & 0x03)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_FD - Fixed dialing phonebook");
				if(*p & 0x04)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_LD - Last dialled phonebook");
				if(*p & 0x05)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_MC -Missed call list");
				if(*p & 0x06)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_ME - ME phone book");
				if(*p & 0x07)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_MT - ME and sim combined phone book");
				if(*p & 0x08)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_ON - My own number - MSISDN");
				if(*p & 0x09)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_RC - ME Received calls");
				if(*p & 0x0A)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_SIM - SIM phone book");
				if(*p & 0x0B)
				TEST_DEBUG("IPC_PB_STORAGE_TYPE_SDN - Service dailing number");

				TEST_DEBUG("********************************************");
			}
			break;
#endif

		default:
			TEST_DEBUG("Undhandled event type [0x%x]",sim_event->EventType)
			;
		}
		printf("\n");
	}//end switch (Type)
	else {
		TEST_DEBUG("Undhandled EventClass [0x%x]",sim_event->EventClass);
	}
	return;

}// end func


int sim_read_key_input(void) {
	char buf[256] = { 0, };
	int ret = 0;
	TapiResult_t err_code = TAPI_API_SUCCESS;
	char ch;

	ret = read(0, buf, sizeof(buf));

	if (ret < 0) {
		if (errno == EINTR)
			perror("read(1)");
		return -1;

	} else if (ret == 0)
		return ret;

	printf("************************************\n");
	printf("op : show all OP | q : quit sim test app \n");
	printf("************************************\n");
	printf("SIM OP in : %s\n", buf);

	if (memcmp(buf, "op", sizeof("op") - 1) == 0) {
		printf("***************************** NEW Asynchronous APIs *******************************\n");
		printf("rsim : rsim \n");
		printf("pin1 : verify pin1 number \n");
		printf("pin2 : verify pin2 number\n");
		printf("sim : verify sim lock number\n");
		printf("puk1 : Unblocking CHV1 \n");
		printf("puk2 : Unblocking CHV2 \n");
		printf("chpin1 : change pin1 number \n");
		printf("chpin2 : change pin2 number \n");
		printf("enpin : CHV enable\n");
		printf("dispin : CHV disable\n");
		printf("enfdn : FDN enable\n");
		printf("disfdn : FDN disable\n");
		printf("ensim : enable sim lock\n");
		printf("dissim : disable sim lock\n");
		printf("getpers : Get lock personalisation information \n");
		printf("medepers : de-personalization test\n");
		printf("mepers : personalization test\n");
		printf("setlang : update sim language info\n");
		printf("apdu : send apdu\n");
		printf("atr : request atr\n");
		printf("***************************** Synchronous APIs *******************************\n");
		printf("siminit : Get SIM init Information\n");
		printf("getpin1 : Get CHV1 status info\n");
		printf("getfdn : Get FDN status info\n");
		printf("getpin2 : Get CHV2 status Info \n");
		printf("getsim : Get CHV2 status Info \n");
		printf("gettype : show Sim Card Type\n");
		printf("getimsi : show Sim IMSI\n");
		printf("geticcid : show Sim ICCID\n");
		printf("getlang:Language Indication Info\n");
		printf("getecc : get SIM ECC\n");
		printf("getuecc : get USIM ECC\n");
		printf("getmb : get mailbox number\n");
		printf("getmw : get message waiting information\n");
		printf("getcf : get callforwarding information\n");
		printf("getcphs : get cphs information\n");
		printf("getmsisdn : get msisdn information\n");
		printf("******************************* PHONE BOOK ***********************************\n");
		printf("pbinit : Get pb init info\n");
		printf("pbcapa : pb capability info \n");
		printf("pbcnt : phonebook used and total count\n");
		printf("pbentry : phonebook entry info\n");
		printf("pbadd : phonebook add\n");
		printf("pbdel : phonebook delete\n");
		printf("pbread:phonebook read\n");
		//		printf("****************************** SAP commands **********************************\n");
		//		printf("sapconnect: SAP Connect request\n");
		//		printf("sapstatus : SAP current connection status request \n");
		//		printf("sapatr: SAP transfer atr request\n");
		//		printf("sapapdu: SAP transfer apdu request\n");
		//		printf("sapsetproto: SAP set protocol request\n");
		//		printf("sapsimpwr: SAP SIM power on/off request\n");
		//		printf("sapreaderstat: SAP card reader status request\n");
		//		printf("*****************************************************************************\n");
	}

	/*NEW API -START*/
	if (memcmp(buf, "siminit", sizeof("siminit") - 1) == 0) {
		TelSimCardStatus_t status = 0x00;
		int b_card_changed = 0;

		err_code = tel_get_sim_init_info(&status, &b_card_changed);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("*************************************************************");
			__PrintCardStatus("CardStatus: ", status);
			TEST_DEBUG("b_card_changed[%d]\n",b_card_changed);
			TEST_DEBUG("*************************************************************");
		} else {
			TEST_DEBUG("TAPI Fail: Error Code [%d]\n",err_code);
		}
	} else if (memcmp(buf, "rsim", sizeof("rsim") - 1) == 0) {
		TEST_DEBUG("rsim....");

		TelSimRSimReq_t rsim = { 0, };

		rsim.file_id = 0x6F60;
		rsim.rsim_cmd = TAPI_SIM_GET_RESPONSE;

		err_code = tel_req_sim_access((const TelSimRSimReq_t*) &rsim,
				&request_id);
		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	} else if (memcmp(buf, "pin1", sizeof("pin1") - 1) == 0) {
		TEST_DEBUG("Pin1 Verification");
		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[TAPI_SIM_PIN_LENGTH + 1];

		getchar();

		TEST_DEBUG(" PUT PIN1 CODE: ");

		_fgets(init_pin_val, 9);

		TelSimSecPw_t pin_data = { 0, };

		pin_data.type = TAPI_SIM_PTYPE_PIN1; // 0x00
		pin_data.pw_len = strlen(init_pin_val);

		TEST_DEBUG("pw_len[%d]", pin_data.pw_len);

		pin_data.pw = (unsigned char*) malloc(length);
		memcpy(pin_data.pw, init_pin_val, length);

		err_code = tel_verifiy_sim_pins(&pin_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "pin2", sizeof("pin2") - 1) == 0) {
		TEST_DEBUG("Pin2 Verification");
		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[TAPI_SIM_PIN_LENGTH + 1];

		getchar();

		TEST_DEBUG(" PUT PIN2 CODE: ");

		_fgets(init_pin_val, 9);

		TelSimSecPw_t pin_data = { 0, };

		pin_data.type = TAPI_SIM_PTYPE_PIN2; // 0x00
		pin_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", pin_data.pw_len);

		pin_data.pw = (unsigned char*) malloc(length);
		memcpy(pin_data.pw, init_pin_val, length);

		err_code = tel_verifiy_sim_pins(&pin_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "sim", sizeof("sim") - 1) == 0) {
		TEST_DEBUG("sim lock verification");
		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[TAPI_SIM_PIN_LENGTH + 1];

		//puts("Flushing input - Enter \ to exit! ");
		//while ((ch = getchar()) != '\n' && ch != EOF);
		getchar();

		TEST_DEBUG(" PUT SIM LOCK CODE: 4 DIGIT ");

		_fgets(init_pin_val, 9);

		TelSimSecPw_t pin_data = { 0, };

		pin_data.type = TAPI_SIM_PTYPE_SIM; // 0x06
		pin_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", pin_data.pw_len);

		pin_data.pw = (unsigned char*) malloc(length);
		memcpy(pin_data.pw, init_pin_val, length);

		err_code = tel_verifiy_sim_pins(&pin_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}
	// Disable pin1
	else if (memcmp(buf, "dispin", sizeof("dispin") - 1) == 0) {
		getchar();

		TEST_DEBUG("Disable npin1");
		TEST_DEBUG("Plese input PIN1 CODE: ");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];

		_fgets(init_pin_val, 9);

		TelSimSecPw_t sec_data = { 0, };
		sec_data.type = TAPI_SIM_PTYPE_PIN1; // 0x00
		sec_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", sec_data.pw_len);

		sec_data.pw = (unsigned char*) malloc(length);
		memcpy(sec_data.pw, init_pin_val, length);

		err_code = tel_disable_sim_security(&sec_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	// Enable pin1
	else if (memcmp(buf, "enpin", sizeof("enpin") - 1) == 0) {
		//TEST_DEBUG("Flushing input - Enter \ to exit!");
		//while ((ch = getchar()) != '\n' && ch != EOF);
		getchar();

		TEST_DEBUG("Enable npin1");
		TEST_DEBUG("Plese input PIN1 CODE: ");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];

		_fgets(init_pin_val, 9);

		TelSimSecPw_t sec_data = { 0, };
		sec_data.type = TAPI_SIM_PTYPE_PIN1; // 0x00
		sec_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", sec_data.pw_len);

		sec_data.pw = (unsigned char*) malloc(length);
		memcpy(sec_data.pw, init_pin_val, length);

		err_code = tel_enable_sim_security(&sec_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	// Disable sim lock
	else if (memcmp(buf, "dissim", sizeof("dissim") - 1) == 0) {
		getchar();

		TEST_DEBUG("Disable ndissim");
		TEST_DEBUG("Plese input SIM LOCK CODE: ");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];

		_fgets(init_pin_val, 9);

		TelSimSecPw_t sec_data = { 0, };
		sec_data.type = TAPI_SIM_PTYPE_SIM; // 0x00
		sec_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", sec_data.pw_len);

		sec_data.pw = (unsigned char*) malloc(length);
		memcpy(sec_data.pw, init_pin_val, length);

		err_code = tel_disable_sim_security(&sec_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	// Enable pin1
	else if (memcmp(buf, "ensim", sizeof("ensim") - 1) == 0) {
		getchar();

		TEST_DEBUG("Enable nensim");
		TEST_DEBUG("Plese input SIM LOCK CODE: ");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];

		_fgets(init_pin_val, 9);

		TelSimSecPw_t sec_data = { 0, };
		sec_data.type = TAPI_SIM_PTYPE_SIM; // 0x00
		sec_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", sec_data.pw_len);

		sec_data.pw = (unsigned char*) malloc(length);
		memcpy(sec_data.pw, init_pin_val, length);

		err_code = tel_enable_sim_security(&sec_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	// get pin1 status
	else if (memcmp(buf, "getpin1", sizeof("getpin1") - 1) == 0) {
		TelSimPinType_t type = TAPI_SIM_PTYPE_PIN1;
		TelSimPinStatus_t status = -1;
		TEST_DEBUG("Get pin1 status");

		err_code = tel_get_sim_security_status(type, &status);

		if (err_code == TAPI_API_SUCCESS) {

			TEST_DEBUG(" *****************************************************");
			switch (status) {
			case TAPI_SIM_PIN_STATUS_DISABLED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_DISABLED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_ENABLED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_ENABLED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_BLOCKED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_BLOCKED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_PUK_BLOCKED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_PUK_BLOCKED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_UNKNOWN: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_UNKNOWN ");
			}
				break;

			default:
				TEST_DEBUG(" Default case statment: pin1_status(%d)", status)
				;
			}
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

		TEST_DEBUG("*****************************************************");
	}
	// get pin2 status
	else if (memcmp(buf, "getpin2", sizeof("getpin2") - 1) == 0) {
		TelSimPinType_t type = TAPI_SIM_PTYPE_PIN2;
		TelSimPinStatus_t status = -1;
		TEST_DEBUG("Get pin2 status");

		err_code = tel_get_sim_security_status(type, &status);

		if (err_code == TAPI_API_SUCCESS) {

			TEST_DEBUG(" *****************************************************");
			switch (status) {
			case TAPI_SIM_PIN_STATUS_DISABLED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_DISABLED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_ENABLED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_ENABLED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_BLOCKED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_BLOCKED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_PUK_BLOCKED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_PUK_BLOCKED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_UNKNOWN: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_UNKNOWN ");
			}
				break;

			default:
				TEST_DEBUG(" Default case statment: pin2_status(%d)", status)
				;
			}
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

		TEST_DEBUG("*****************************************************");
	} else if (memcmp(buf, "getsim", sizeof("getsim") - 1) == 0) {
		TelSimPinType_t type = TAPI_SIM_PTYPE_SIM;
		TelSimPinStatus_t status = -1;
		TEST_DEBUG("Get SIM LOCK status");

		err_code = tel_get_sim_security_status(type, &status);

		if (err_code == TAPI_API_SUCCESS) {

			TEST_DEBUG(" *****************************************************");
			switch (status) {
			case TAPI_SIM_PIN_STATUS_DISABLED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_DISABLED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_ENABLED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_ENABLED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_BLOCKED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_BLOCKED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_PUK_BLOCKED: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_PUK_BLOCKED ");
			}
				break;

			case TAPI_SIM_PIN_STATUS_UNKNOWN: {
				TEST_DEBUG("TAPI_SIM_PIN_STATUS_UNKNOWN ");
			}
				break;

			default:
				TEST_DEBUG(" Default case statment: sim_status(%d)", status)
				;
			}
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

		TEST_DEBUG("*****************************************************");
	}
	// Enable FDN
	else if (memcmp(buf, "enfdn", sizeof("enfdn") - 1) == 0) {
		//puts("Flushing input - Enter \ to exit! ");
		//while ((ch = getchar()) != '\n' && ch != EOF);
		getchar();

		TEST_DEBUG("Enable FDN");
		TEST_DEBUG("Plese input PIN2 CODE: ");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];

		_fgets(init_pin_val, 9);

		TelSimSecPw_t sec_data = { 0, };
		sec_data.type = TAPI_SIM_PTYPE_PIN2; // 0x00
		sec_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", sec_data.pw_len);

		sec_data.pw = (unsigned char*) malloc(length);
		memcpy(sec_data.pw, init_pin_val, length);

		err_code = tel_enable_sim_fdn(sec_data.pw, (int *)&sec_data.pw_len,
				&request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}
	// Disable FDN
	else if (memcmp(buf, "disfdn", sizeof("disfdn") - 1) == 0) {
		getchar();

		TEST_DEBUG("Disable FDN");
		TEST_DEBUG("Plese input PIN2 CODE");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];

		_fgets(init_pin_val, 9);

		TelSimSecPw_t sec_data = { 0, };
		sec_data.type = TAPI_SIM_PTYPE_PIN2; // 0x00
		sec_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", sec_data.pw_len);

		sec_data.pw = (unsigned char*) malloc(length);
		memcpy(sec_data.pw, init_pin_val, length);

		err_code = tel_disable_sim_fdn(sec_data.pw, (int *)&sec_data.pw_len,
				&request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}
	// get FDN status
	else if (memcmp(buf, "getfdn", sizeof("getfdn") - 1) == 0) {
		TEST_DEBUG("Get FDN status");
		int fdn_status = 0;

		err_code = tel_get_sim_fdn_status(&fdn_status);

		if (err_code == TAPI_API_SUCCESS) {
			if (fdn_status == 0) {
				TEST_DEBUG("*************************");
				TEST_DEBUG("FDN disabled ");
				TEST_DEBUG("*************************");
			} else {
				TEST_DEBUG("*************************");
				TEST_DEBUG("FDN enabled");
				TEST_DEBUG("*************************");
			}
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	} else if (memcmp(buf, "gettype", sizeof("gettype") - 1) == 0) {
		TelSimCardType_t cardInfo;
		TEST_DEBUG("Get card type !!!");
		err_code = tel_get_sim_type(&cardInfo);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("*************************************************************");
			__PrintCardType("", cardInfo);
			TEST_DEBUG("*************************************************************");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	} else if (memcmp(buf, "getimsi", sizeof("getimsi") - 1) == 0) {
		TelSimImsiInfo_t imsi;
		TEST_DEBUG("Get IMSI type !!!");
		err_code = tel_get_sim_imsi(&imsi);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("*************************************************************");
			TEST_DEBUG("* imsi.bValid[%d]",imsi.bValid);
			TEST_DEBUG("* imsi.szMcc[%s]",imsi.szMcc);
			TEST_DEBUG("* imsi.szMnc[%s]",imsi.szMnc);
			TEST_DEBUG("* imsi.szMsin[%s]",imsi.szMsin);
			TEST_DEBUG("*************************************************************");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	} else if (memcmp(buf, "geticcid", sizeof("geticcid") - 1) == 0) {
		TelSimIccIdInfo_t iccid;
		TEST_DEBUG("Get ICCID  !!!");
		err_code = tel_get_sim_iccid(&iccid);
		int i = 0;

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("*************************************************************");
			TEST_DEBUG("* iccid.icc_length[%d]",iccid.icc_length);
			for (i = 0; i < iccid.icc_length; i++) {
				TEST_DEBUG("* iccid.icc_num[%c]",iccid.icc_num[i]);
			}
			TEST_DEBUG("*************************************************************");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getlang", sizeof("getlang") - 1) == 0) {
		TEST_DEBUG("Language Preference INFO ");
		int i;
		TelSimLanguageInfo_t li_info = { 0, };

		err_code = tel_get_sim_language(&li_info);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");
			TEST_DEBUG("************ LI  INFO ***********************");
			TEST_DEBUG("LpCount: [%lu]", li_info.LpCount);
			for (i = 0; i < li_info.LpCount; i++) {
				fprintf(stderr, "Loop(%d), ", i);
				__PrintLangInfo("LpCode: ", li_info.Lp[i]);
			}
			TEST_DEBUG("============================================");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getecc", sizeof("getecc") - 1) == 0) {
		TEST_DEBUG(" Get SIM ECC  ");
		int ecc_rec_count = 0;
		TelSimEccData_t ecc_info = { { { 0, } } };
		TelSimEccInfo_t SimEccInfo = { { 0, } };

		err_code = tel_get_sim_ecc(&ecc_info, &ecc_rec_count);

		if (err_code == TAPI_API_SUCCESS) {
			SimEccInfo = ecc_info.EccInfo;
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" EEC count [%d]", ecc_rec_count);
			TEST_DEBUG(" ============================================");

			if (ecc_rec_count != 0) {
				TEST_DEBUG("ECC1 [%s]", SimEccInfo.szEcc1);
				TEST_DEBUG("ECC2 [%s]", SimEccInfo.szEcc2);
				TEST_DEBUG("ECC3 [%s]", SimEccInfo.szEcc3);
				TEST_DEBUG("ECC4 [%s]", SimEccInfo.szEcc4);
				TEST_DEBUG("ECC5 [%s]", SimEccInfo.szEcc5);
			}
			TEST_DEBUG(" ============================================");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getuecc", sizeof("getuecc") - 1) == 0) {
		int i = 0;
		int uecc_rec_count = 0;

		TEST_DEBUG(" Get USIM ECC  ");

		TelSimEccData_t uecc_info = { { { 0, } } };

		err_code = tel_get_sim_ecc(&uecc_info, &uecc_rec_count);

		if (err_code == TAPI_API_SUCCESS) {

			TEST_DEBUG("ECC count [%d]",uecc_rec_count );

			for (i = 0; i < uecc_rec_count; i++) {
				TEST_DEBUG("Loop(%d): ", i);
				TEST_DEBUG(" ECC Used [%d]", uecc_info.UeccInfo[i].bUsed);
				TEST_DEBUG(" ECC Len [%d]", uecc_info.UeccInfo[i].EccLen);

				if (NULL != uecc_info.UeccInfo[i].szEcc) {
					TEST_DEBUG("ECC string [%s]", uecc_info.UeccInfo[i].szEcc);
				} else {
					TEST_DEBUG("ECC string [null]");
				}

				TEST_DEBUG("ECC alphaID [%s]",uecc_info.UeccInfo[i].szEccAlphaIdentifier);
				__PrintECCSvcInfo(uecc_info.UeccInfo[i].EccEmergencyServiceInfo);
			}

		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getimsi", sizeof("getimsi") - 1) == 0) {
		TEST_DEBUG("Get IMSI INFO ");

		TelSimImsiInfo_t sim_imsi_info;

		err_code = tel_get_sim_imsi(&sim_imsi_info);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" IMSI [mcc,mnc,msin]= ");
			TEST_DEBUG(" [%s]",sim_imsi_info.szMcc);
			TEST_DEBUG(" [%s]",sim_imsi_info.szMnc);
			TEST_DEBUG(" [%s]",sim_imsi_info.szMsin);
			TEST_DEBUG(" ============================================");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getmb", sizeof("getmb") - 1) == 0) {
		TEST_DEBUG("Get MAILBOX INFO ");

		TelSimMailboxNumbers_s mbox;

		err_code = tel_get_sim_mailbox_info(&mbox);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");

			if (mbox.b_cphs) {
				TEST_DEBUG("Current SIM is CPHS");
			} else {
				TEST_DEBUG("Current SIM is not CPHS but 3GPP");
			}
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" voice_line1.bUsed[%d]",mbox.voice_line1.bUsed);
			TEST_DEBUG(" voice_line1.AlphaIdLength[%lu]",mbox.voice_line1.AlphaIdLength);
			TEST_DEBUG(" voice_line1.AlphaId[%s]",mbox.voice_line1.AlphaId);
			TEST_DEBUG(" voice_line1.DiallingnumLength[%lu]",mbox.voice_line1.DiallingnumLength);
			TEST_DEBUG(" voice_line1.DiallingNum[%s]",mbox.voice_line1.DiallingNum);
			TEST_DEBUG(" voice_line1.NumberingPlanIdent[%d]",mbox.voice_line1.NumberingPlanIdent);
			TEST_DEBUG(" voice_line1.TypeOfNumber[%u]",mbox.voice_line1.TypeOfNumber);
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" voice_line2.bUsed[%d]",mbox.voice_line2.bUsed);
			TEST_DEBUG(" voice_line2.AlphaIdLength[%lu]",mbox.voice_line2.AlphaIdLength);
			TEST_DEBUG(" voice_line2.AlphaId[%s]",mbox.voice_line2.AlphaId);
			TEST_DEBUG(" voice_line2.DiallingnumLength[%lu]",mbox.voice_line2.DiallingnumLength);
			TEST_DEBUG(" voice_line2.DiallingNum[%s]",mbox.voice_line2.DiallingNum);
			TEST_DEBUG(" voice_line2.NumberingPlanIdent[%d]",mbox.voice_line2.NumberingPlanIdent);
			TEST_DEBUG(" voice_line2.TypeOfNumber[%d]",mbox.voice_line2.TypeOfNumber);
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" video.bUsed[%d]",mbox.video.bUsed);
			TEST_DEBUG(" video.AlphaIdLength[%lu]",mbox.video.AlphaIdLength);
			TEST_DEBUG(" video.AlphaId[%s]",mbox.video.AlphaId);
			TEST_DEBUG(" video.DiallingnumLength[%lu]",mbox.video.DiallingnumLength);
			TEST_DEBUG(" video.DiallingNum[%s]",mbox.video.DiallingNum);
			TEST_DEBUG(" video.NumberingPlanIdent[%d]",mbox.video.NumberingPlanIdent);
			TEST_DEBUG(" video.TypeOfNumber[%d]",mbox.video.TypeOfNumber);
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" fax.bUsed[%d]",mbox.fax.bUsed);
			TEST_DEBUG(" fax.AlphaIdLength[%lu]",mbox.fax.AlphaIdLength);
			TEST_DEBUG(" fax.AlphaId[%s]",mbox.fax.AlphaId);
			TEST_DEBUG(" fax.DiallingnumLength[%lu]",mbox.fax.DiallingnumLength);
			TEST_DEBUG(" fax.DiallingNum[%s]",mbox.fax.DiallingNum);
			TEST_DEBUG(" fax.NumberingPlanIdent[%d]",mbox.fax.NumberingPlanIdent);
			TEST_DEBUG(" fax.TypeOfNumber[%d]",mbox.fax.TypeOfNumber);
			TEST_DEBUG(" ============================================");
			TEST_DEBUG(" email.bUsed[%d]",mbox.email.bUsed);
			TEST_DEBUG(" email.AlphaIdLength[%lu]",mbox.email.AlphaIdLength);
			TEST_DEBUG(" email.AlphaId[%s]",mbox.email.AlphaId);
			TEST_DEBUG(" email.DiallingnumLength[%lu]",mbox.email.DiallingnumLength);
			TEST_DEBUG(" email.DiallingNum[%s]",mbox.email.DiallingNum);
			TEST_DEBUG(" email.NumberingPlanIdent[%d]",mbox.email.NumberingPlanIdent);
			TEST_DEBUG(" email.TypeOfNumber[%d]",mbox.email.TypeOfNumber);
			TEST_DEBUG(" ============================================");

		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getmw", sizeof("getmw") - 1) == 0) {
		TEST_DEBUG("Get MESSAGE WAITING INFO ");

		TelSimMessageWaiting_s mw;

		err_code = tel_get_sim_messagewaiting_info(&mw);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");

			if (mw.b_cphs) {
				TEST_DEBUG("Current SIM is CPHS");
				TEST_DEBUG(" ============================================");
				TEST_DEBUG(" cphs_mw.bWaitVoiceMsgLine1[%d]",mw.mw_data_u.cphs_mw.bWaitVoiceMsgLine1);
				TEST_DEBUG(" cphs_mw.bWaitVoiceMsgLine2[%d]",mw.mw_data_u.cphs_mw.bWaitVoiceMsgLine2);
				TEST_DEBUG(" cphs_mw.bWaitFaxMsg[%d]",mw.mw_data_u.cphs_mw.bWaitFaxMsg);
				TEST_DEBUG(" cphs_mw.bWaitDataMsg[%d]",mw.mw_data_u.cphs_mw.bWaitDataMsg);

			} else {
				TEST_DEBUG("Current SIM is not CPHS but 3GPP");
				TEST_DEBUG(" ============================================");
				TEST_DEBUG(" mw_data_u.mw.IndicatorType[0x%x]",mw.mw_data_u.mw.IndicatorType);
				TEST_DEBUG(" mw_data_u.mw.VoiceMailCount[%d]",mw.mw_data_u.mw.VoiceMailCount);
				TEST_DEBUG(" mw_data_u.mw.FaxCount[%d]",mw.mw_data_u.mw.FaxCount);
				TEST_DEBUG(" mw_data_u.mw.EmailCount[%d]",mw.mw_data_u.mw.EmailCount);
				TEST_DEBUG(" mw_data_u.mw.OtherCount[%d]",mw.mw_data_u.mw.OtherCount);
				TEST_DEBUG(" mw_data_u.mw.VideoMailCount[%d]",mw.mw_data_u.mw.VideoMailCount);
			}
			TEST_DEBUG(" ============================================");

		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getcf", sizeof("getcf") - 1) == 0) {
		TEST_DEBUG("Get CALLFORWARDING INFO ");

		TelSimCallForwarding_s cf;

		err_code = tel_get_sim_callforwarding_info(&cf);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");

			if (cf.b_cphs) {
				TEST_DEBUG("Current SIM is CPHS");
				TEST_DEBUG(" ============================================");
				TEST_DEBUG(" cphs_cf.bCallForwardUnconditionalLine1[%d]",cf.cf_data_u.cphs_cf.bCallForwardUnconditionalLine1);
				TEST_DEBUG(" cphs_cf.bCallForwardUnconditionalLine2[%d]",cf.cf_data_u.cphs_cf.bCallForwardUnconditionalLine2);
				TEST_DEBUG(" cphs_cf.bCallForwardUnconditionalFax[%d]",cf.cf_data_u.cphs_cf.bCallForwardUnconditionalFax);
				TEST_DEBUG(" cphs_cf.bCallForwardUnconditionalData[%d]",cf.cf_data_u.cphs_cf.bCallForwardUnconditionalData);
				TEST_DEBUG(" cphs_cf.bCallForwardUnconditionalSms[%d]",cf.cf_data_u.cphs_cf.bCallForwardUnconditionalSms);
				TEST_DEBUG(" cphs_cf.bCallForwardUnconditionalBearer[%d]",cf.cf_data_u.cphs_cf.bCallForwardUnconditionalBearer);
			} else {
				TEST_DEBUG("Current SIM is not CPHS but 3GPP");
				TEST_DEBUG(" ============================================");
				TEST_DEBUG(" cf.bUsed[%d]",cf.cf_data_u.cf.bUsed);
				TEST_DEBUG(" cf.MspNumber[%d]",cf.cf_data_u.cf.MspNumber);
				TEST_DEBUG(" cf.Status[0x%x]",cf.cf_data_u.cf.Status);
				TEST_DEBUG(" cf.DiallingnumLen[%lu]",cf.cf_data_u.cf.DiallingnumLen);
				TEST_DEBUG(" cf.TypeOfNumber[%d]",cf.cf_data_u.cf.TypeOfNumber);
				TEST_DEBUG(" cf.NumberingPlanIdent[%d]",cf.cf_data_u.cf.NumberingPlanIdent);
				TEST_DEBUG(" cf.DiallingNum[%s]",cf.cf_data_u.cf.DiallingNum);
			}
			TEST_DEBUG(" ============================================");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "getcphs", sizeof("getcphs") - 1) == 0) {
		TEST_DEBUG("Get CPHS INFO ");

		TelSimCphsLocalInfo_t cphs;

		err_code = tel_get_sim_cphs_info(&cphs);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");

			if (cphs.b_used) {
				TEST_DEBUG("Current SIM is CPHS");
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.cphsinfo.CphsPhase[%d]",cphs.cphsinfo.CphsPhase);
				TEST_DEBUG("cphs.cphsinfo.CphsServiceTable.bCustomerServiceProfile[%d]",cphs.cphsinfo.CphsServiceTable.bCustomerServiceProfile);
				TEST_DEBUG("cphs.cphsinfo.CphsServiceTable.bServiceStringTable[%d]",cphs.cphsinfo.CphsServiceTable.bServiceStringTable);
				TEST_DEBUG("cphs.cphsinfo.CphsServiceTable.bMailBoxNumbers[%d]",cphs.cphsinfo.CphsServiceTable.bMailBoxNumbers);
				TEST_DEBUG("cphs.cphsinfo.CphsServiceTable.bOperatorNameShortForm[%d]",cphs.cphsinfo.CphsServiceTable.bOperatorNameShortForm);
				TEST_DEBUG("cphs.cphsinfo.CphsServiceTable.bInformationNumbers[%d]",cphs.cphsinfo.CphsServiceTable.bInformationNumbers);
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.csp.ServiceProfileEntry[0].CustomerServiceGroup[%d]",cphs.csp.ServiceProfileEntry[0].CustomerServiceGroup);
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.infn.bUsed[%d]",cphs.infn.bUsed);
				TEST_DEBUG("cphs.infn.AlphaIdLength[%d]",cphs.infn.AlphaIdLength);
				TEST_DEBUG("cphs.infn.IndexLevelIndicator[%d]",cphs.infn.IndexLevelIndicator);
				TEST_DEBUG("cphs.infn.PremiumServiceIndicator[%d]",cphs.infn.PremiumServiceIndicator);
				TEST_DEBUG("cphs.infn.NetworkSpecificIndicator[%d]",cphs.infn.NetworkSpecificIndicator);
				TEST_DEBUG("cphs.infn.Alpha_id[%s]",cphs.infn.Alpha_id);
				TEST_DEBUG("cphs.infn.DiallingnumLength[%lu]",cphs.infn.DiallingnumLength);
				TEST_DEBUG("cphs.infn.TypeOfNumber[%d]",cphs.infn.TypeOfNumber);
				TEST_DEBUG("cphs.infn.NumberingPlanIdentity[%d]",cphs.infn.NumberingPlanIdentity);
				TEST_DEBUG("cphs.infn.DiallingNum[%s]",cphs.infn.DiallingNum);
				TEST_DEBUG("cphs.infn.Ext1RecordId[%d]",cphs.infn.Ext1RecordId);
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.opname.NameLength[%d]",cphs.opname.NameLength);
				TEST_DEBUG("cphs.opname.OperatorName[%s]",cphs.opname.OperatorName);
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.opshortform.ShortNameLength[%d]",cphs.opshortform.ShortNameLength);
				TEST_DEBUG("cphs.opshortform.OperatorShortName[%s]",cphs.opshortform.OperatorShortName);
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.dflagsinfo.DynamicFlags[%d]",cphs.dflagsinfo.DynamicFlags);
				TEST_DEBUG(" ============================================");
				TEST_DEBUG("cphs.d2flagsinfo.Dynamic2Flag[%d]",cphs.d2flagsinfo.Dynamic2Flag);
			} else {
				TEST_DEBUG("Current SIM is not CPHS but 3GPP");
				TEST_DEBUG(" ============================================");

			}
			TEST_DEBUG(" ============================================");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "getmsisdn", sizeof("getmsisdn") - 1) == 0) {
		TEST_DEBUG("Get MSISDN INFO ");

		TelSimSubscriberInfo_t msisdn;

		err_code = tel_get_sim_msisdn(&msisdn);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" ============================================");
			TEST_DEBUG("name[%s]",msisdn.name);
			TEST_DEBUG("num[%s]",msisdn.num);
			if (msisdn.name == '\0')
				TEST_DEBUG("name is null string");

			if (msisdn.num == '\0')
				TEST_DEBUG("number is null string");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	} else if (memcmp(buf, "pbinit", sizeof("pbinit") - 1) == 0) {
		TEST_DEBUG("npbinit status");

		int valid_index = 0;
		TelSimPbList_t pb_list = { 0, };
		int pPbInitCompleted = 0;

		err_code = tel_get_sim_pb_init_info(&pPbInitCompleted, &pb_list,
				&valid_index);

		if (err_code == TAPI_API_SUCCESS) {
			if (pPbInitCompleted == 0) {
				TEST_DEBUG(" SIM PB INIT NOT completed ");
			} else {
				TEST_DEBUG("SIM PB INIT completed");
				/* if no sim records at all then valid_index = 0xFF */TEST_DEBUG("ADN First index is [%d]",valid_index);
				TEST_DEBUG("SIM phone book storage list Information ");
				TEST_DEBUG("********************************************");
				TEST_DEBUG("pb_list.b_fdn[%d]",pb_list.b_fdn);
				TEST_DEBUG("pb_list.b_msisdn[%d]",pb_list.b_msisdn);
				TEST_DEBUG("pb_list.b_adn[%d]",pb_list.b_adn);
				TEST_DEBUG("pb_list.b_sdn[%d]",pb_list.b_sdn);
				TEST_DEBUG("pb_list.b_3g[%d]",pb_list.b_3g);
				TEST_DEBUG("pb_list.b_aas[%d]",pb_list.b_aas);
				TEST_DEBUG("pb_list.b_gas[%d]",pb_list.b_gas);

				/*
				 if(pb_list & 0x01)
				 TEST_DEBUG("ME dialed calls list stored in NV");
				 if(pb_list & 0x02)
				 TEST_DEBUG("SIM(or ME) emergency number");
				 if(pb_list & 0x03)
				 TEST_DEBUG("SIM fixed-dialing phonebook");
				 if(pb_list & 0x04)
				 TEST_DEBUG("SIM last-dialing phonebook");
				 if(pb_list & 0x05)
				 TEST_DEBUG("ME missed calls list");
				 if(pb_list & 0x06)
				 TEST_DEBUG("ME phonebook");
				 if(pb_list & 0x07)
				 TEST_DEBUG("Combined ME and SIM phonebook");
				 if(pb_list & 0x08)
				 TEST_DEBUG("SIM(or ME) own numbers ( MSISDNs) list");
				 if(pb_list & 0x09)
				 TEST_DEBUG("ME received calls list stored in NV");
				 if(pb_list & 0x0A)
				 TEST_DEBUG("2G SIM phonebook");
				 if(pb_list & 0x0B)
				 TEST_DEBUG("Service Dialing Number");
				 if(pb_list & 0x0C)
				 TEST_DEBUG("3G SIM phonebook");
				 if(pb_list & 0x0D)
				 TEST_DEBUG("Incoming Call Information");
				 if(pb_list & 0x0E)
				 TEST_DEBUG("Outgoing Call Information");
				 if(pb_list & 0x0F)
				 TEST_DEBUG("Additional Number Alpha String");
				 if(pb_list & 0x10)
				 TEST_DEBUG("Grouping Information Alpha String");
				 */
				/*
				 PB_DC	0x01	ME dialed calls list stored in NV
				 PB_EN	0x02	SIM(or ME) emergency number
				 PB_FD	0x03	SIM fixed-dialing phonebook
				 PB_LD	0x04	SIM last-dialing phonebook
				 PB_MC	0x05	ME missed calls list
				 PB_ME	0x06	ME phonebook
				 PB_MT	0x07	Combined ME and SIM phonebook
				 PB_ON	0x08	SIM(or ME) own numbers ( MSISDNs) list
				 PB_RC	0x09	ME received calls list stored in NV
				 PB_SIM	0x0A	2G SIM phonebook
				 PB_SDN	0x0B	Service Dialing Number
				 PB_3GSIM	0x0C	3G SIM phonebook
				 PB_ICI	0x0D	Incoming Call Information
				 PB_OCI	0x0E	Outgoing Call Information
				 PB_AAS	0x0F	Additional Number Alpha String
				 PB_GAS	0x10	Grouping Information Alpha String
				 */

				TEST_DEBUG("********************************************");
			}
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	}

	//change pin1 number
	else if (memcmp(buf, "chpin1", sizeof("chpin1") - 1) == 0) {
		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_old_pin_val[length];
		char init_new_pin_val[length];

		memset(&init_old_pin_val, 0, length);
		memset(&init_new_pin_val, 0, length);

		TelSimSecPw_t old_pin = { 0, };
		TelSimSecPw_t new_pin = { 0, };

		getchar();

		TEST_DEBUG("Change PIN1 CODE");
		TEST_DEBUG("Input Current PIN1 Code: ");

		_fgets(init_old_pin_val, 9);

		old_pin.type = TAPI_SIM_PTYPE_PIN1; // 0x00
		old_pin.pw_len = strlen(init_old_pin_val);
		TEST_DEBUG("pw_len[%d]", old_pin.pw_len);
		old_pin.pw = (unsigned char*) malloc(length);
		memcpy(old_pin.pw, init_old_pin_val, length);

		TEST_DEBUG("Input New PIN1 Code: ");

		_fgets(init_new_pin_val, 9);

		new_pin.type = TAPI_SIM_PTYPE_PIN1; // 0x00
		new_pin.pw_len = strlen(init_new_pin_val);
		TEST_DEBUG("pw_len[%d]", new_pin.pw_len);
		new_pin.pw = (unsigned char*) malloc(length);
		memcpy(new_pin.pw, init_new_pin_val, length);

		err_code = tel_change_sim_pins(&old_pin, &new_pin, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	//change pin2 number
	else if (memcmp(buf, "chpin2", sizeof("chpin2") - 1) == 0) {
		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_old_pin_val[length];
		char init_new_pin_val[length];

		memset(&init_old_pin_val, 0, length);
		memset(&init_new_pin_val, 0, length);

		TelSimSecPw_t old_pin = { 0, };
		TelSimSecPw_t new_pin = { 0, };

		getchar();

		TEST_DEBUG("Change PIN2 CODE");
		TEST_DEBUG("Input Current PIN2 Code: ");

		_fgets(init_old_pin_val, 9);

		old_pin.type = TAPI_SIM_PTYPE_PIN2; // 0x00
		old_pin.pw_len = strlen(init_old_pin_val);
		TEST_DEBUG("pw_len[%d]", old_pin.pw_len);
		old_pin.pw = (unsigned char*) malloc(length);
		memcpy(old_pin.pw, init_old_pin_val, length);

		TEST_DEBUG("Input New PIN2 Code: ");

		_fgets(init_new_pin_val, 9);

		new_pin.type = TAPI_SIM_PTYPE_PIN2; // 0x00
		new_pin.pw_len = strlen(init_new_pin_val);
		TEST_DEBUG("pw_len[%d]", new_pin.pw_len);
		new_pin.pw = (unsigned char*) malloc(length);
		memcpy(new_pin.pw, init_new_pin_val, length);

		err_code = tel_change_sim_pins(&old_pin, &new_pin, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	// unblock CHV and enter new pin code
	else if (memcmp(buf, "puk1", sizeof("puk1") - 1) == 0) {
		getchar();

		TEST_DEBUG("PUK1 verification");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];
		char init_puk_val[length];

		TEST_DEBUG("Plese input PUK CODE: ");

		_fgets(init_puk_val, 9);

		TEST_DEBUG("NEW PIN1 CODE: ");
		_fgets(init_pin_val, 9);

		TelSimSecPw_t puk_data = { 0, };
		TelSimSecPw_t new_pin_data = { 0, };

		puk_data.type = TAPI_SIM_PTYPE_PUK1; // 0x00
		puk_data.pw_len = strlen(init_puk_val);
		TEST_DEBUG("pw_len[%d]", puk_data.pw_len);
		puk_data.pw = (unsigned char*) malloc(length);
		memcpy(puk_data.pw, init_puk_val, length);

		new_pin_data.type = TAPI_SIM_PTYPE_PIN1; // 0x00
		new_pin_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", new_pin_data.pw_len);
		new_pin_data.pw = (unsigned char*) malloc(length);
		memcpy(new_pin_data.pw, init_pin_val, length);

		//  sim pin init
		TEST_DEBUG("PUKtype Value %d",puk_data.type);
		TEST_DEBUG("PUK Value %s",puk_data.pw);
		TEST_DEBUG("New Pin Value %s",new_pin_data.pw);

		err_code = tel_verify_sim_puks(&puk_data, &new_pin_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	// unblock CHV2 and enter new pin2 code
	else if (memcmp(buf, "puk2", sizeof("puk2") - 1) == 0) {
		getchar();

		TEST_DEBUG("PUK2 verification");

		int length = TAPI_SIM_PIN_LENGTH + 1;
		char init_pin_val[length];
		char init_puk_val[length];

		TEST_DEBUG("Plese input PUK2 CODE: ");

		_fgets(init_puk_val, 9);

		TEST_DEBUG("NEW PIN2 CODE: ");
		_fgets(init_pin_val, 9);

		TelSimSecPw_t puk_data = { 0, };
		TelSimSecPw_t new_pin_data = { 0, };

		puk_data.type = TAPI_SIM_PTYPE_PUK2; // 0x00
		puk_data.pw_len = strlen(init_puk_val);
		TEST_DEBUG("pw_len[%d]", puk_data.pw_len);
		puk_data.pw = (unsigned char*) malloc(length);
		memcpy(puk_data.pw, init_puk_val, length);

		new_pin_data.type = TAPI_SIM_PTYPE_PIN2; // 0x00
		new_pin_data.pw_len = strlen(init_pin_val);
		TEST_DEBUG("pw_len[%d]", new_pin_data.pw_len);
		new_pin_data.pw = (unsigned char*) malloc(length);
		memcpy(new_pin_data.pw, init_pin_val, length);

		//  sim pin init
		TEST_DEBUG("PUKtype Value %d",puk_data.type);
		TEST_DEBUG("PUK Value %s",puk_data.pw);
		TEST_DEBUG("New Pin Value %s",new_pin_data.pw);

		err_code = tel_verify_sim_puks(&puk_data, &new_pin_data, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "mepers", sizeof("mepers") - 1) == 0) {
		int tempi;
		char dummy;

		TelSimPersPw_t lock_pers = { 0, };

		printf("------------------------------ \n");
		printf("Personalization Type : \n");
		printf("------------------------------ \n");
		printf("1 - Network  \n");
		printf("2 - Network Subset \n");
		printf("3 - Service Provider \n");
		printf("4 - Corporate \n");
		printf("------------------------------ \n");

		printf("\nPlease input type: \n");
		scanf("%d%c", &tempi, &dummy);

		switch (tempi) {
		case 1: {
			lock_pers.type = TAPI_SIM_PERS_NET;
		}
			break;

		case 2: {
			lock_pers.type = TAPI_SIM_PERS_NS;
		}
			break;

		case 3: {
			lock_pers.type = TAPI_SIM_PERS_SP;
		}
			break;

		case 4: {
			lock_pers.type = TAPI_SIM_PERS_CP;
		}
			break;

		default: {
			printf("wrong pers type! exit!\n");
			exit(1);
		}
			break;
		}

		switch (lock_pers.type) {
		case TAPI_SIM_PERS_NET: {
			puts("Flushing input - Enter \\n to exit! ");

			while ((ch = getchar()) != '\n' && ch != EOF)
				;

			printf("Lock Personalisation MCC+MNC 5\n");
			printf(
					"Plese input Network Lock CODE : length of MCC+MNC, MCC, MNC, NCK\n");
			printf(
					"We need to use this for testing : length of MCC+MNC(5), MCC(450), MNC(01), NCK(12345678) => 54500112345678\n");

			int length = 14 + 1;
			char init_password_val[length];
			_fgets(init_password_val, 15);

			lock_pers.pw_len = 14;
			lock_pers.pw = (unsigned char*) malloc(length);
			memcpy(lock_pers.pw, init_password_val, length);

		}
			break;

		case TAPI_SIM_PERS_NS: {
			puts("Flushing input - Enter \\n to exit! ");

			while ((ch = getchar()) != '\n' && ch != EOF)
				;

			printf(
					"Network Subset Personalisation MCC+MNC 5 byte, subset 2byte \n");
			printf(
					"We need to use this for testing : length of MCC+MNC(5), MCC(450), MNC(01), lengthof NSP(2), NSP(11) NSPCK(12345678) => 54500121112345678\n");

			int length = 17 + 1;
			char init_password_val[length];
			_fgets(init_password_val, 18);

			lock_pers.pw_len = 17;
			lock_pers.pw = (unsigned char*) malloc(length);
			memcpy(lock_pers.pw, init_password_val, length);
		}
			break;

		case TAPI_SIM_PERS_SP: {
			puts("Flushing input - Enter \\n to exit! ");

			while ((ch = getchar()) != '\n' && ch != EOF)
				;

			printf(
					"Service Provider Personalisation MCC+MNC 5 byte, SP 2byte \n");
			printf(
					"We need to use this for testing : length of MCC+MNC(5), MCC(450), MNC(01), SP(11) SPCK(12345678) => 5450011112345678\n");

			int length = 16 + 1;
			char init_password_val[length];
			_fgets(init_password_val, 17);

			lock_pers.pw_len = 16;
			lock_pers.pw = (unsigned char*) malloc(length);
			memcpy(lock_pers.pw, init_password_val, length);
		}
			break;

		case TAPI_SIM_PERS_CP: {
			puts("Flushing input - Enter \\n to exit! ");

			while ((ch = getchar()) != '\n' && ch != EOF)
				;

			printf(
					"Corporate Personalisation MCC+MNC 5 byte, SP 2byte CP 2byte \n");
			printf(
					"We need to use this for testing : length of MCC+MNC(5), MCC(450), MNC(01), SP(11) CP(11) CPCK(12345678) => 545001111112345678\n");

			int length = 18 + 1;
			char init_password_val[length];
			_fgets(init_password_val, 19);

			lock_pers.pw_len = 18;
			lock_pers.pw = (unsigned char*) malloc(length);
			memcpy(lock_pers.pw, init_password_val, length);
		}
			break;

		default: {
			printf("wrong condition exit\n");
			exit(1);
		}
			break;
		}

		err_code = tel_enable_sim_personalization(&lock_pers, &request_id);
		printf("Error Code [%x]\n", err_code);

	}

	if (memcmp(buf, "medepers", sizeof("medepers") - 1) == 0) {
		int tempi;
		char dummy;

		TelSimPersPw_t lock_pers = { 0, };

		printf("------------------------------ \n");
		printf("de-Personalization Type : \n");
		printf("------------------------------ \n");
		printf("1 - Network  \n");
		printf("2 - Network Subset \n");
		printf("3 - Service Provider \n");
		printf("4 - Corporate \n");
		printf("------------------------------ \n");

		printf("\nPlease input type: \n");
		scanf("%d%c", &tempi, &dummy);

		switch (tempi) {
		case 1: {
			lock_pers.type = TAPI_SIM_PERS_NET;
		}
			break;

		case 2: {
			lock_pers.type = TAPI_SIM_PERS_NS;
		}
			break;

		case 3: {
			lock_pers.type = TAPI_SIM_PERS_SP;
		}
			break;

		case 4: {
			lock_pers.type = TAPI_SIM_PERS_CP;
		}
			break;

		default: {
			printf("wrong pers type! exit!\n");
			exit(1);
		}
			break;
		}

		puts("Flushing input - Enter \\n to exit! ");

		while ((ch = getchar()) != '\n' && ch != EOF)
			;

		printf("de Personalisation key is 8 byte \n");
		printf("We need to use this for testing : 12345678\n");

		int length = 8 + 1;
		char init_password_val[length];
		_fgets(init_password_val, 9);

		lock_pers.pw_len = 8 /*control key*/;
		lock_pers.pw = (unsigned char*) malloc(length);
		memcpy(lock_pers.pw, init_password_val, length);

		err_code = tel_disable_sim_personalization(&lock_pers, &request_id);

		printf("Error Code [%x]\n", err_code);

	}

	if (memcmp(buf, "getpers", sizeof("getpers") - 1) == 0) {
		int tempi = 0;
		char dummy = 0;

		TelSimPersType_t type = 0;

		puts("Flushing input - Enter \\n to exit! ");

		while ((ch = getchar()) != '\n' && ch != EOF)
			;

		printf("getpers status\n");
		printf("Here - 1\n");
		printf(
				"================================================================ \n");
		printf("TAPI_SIM_PERS_NET		= 0x00,	/**< Network Lock */ \n");
		printf("TAPI_SIM_PERS_NS		= 0x01,	/**< Nettwork Subset Lock */\n");
		printf("TAPI_SIM_PERS_SP		= 0x02,	/**< Service Provider Lock */\n");
		printf("TAPI_SIM_PERS_CP		= 0x03,	/**< Coporate Lock */\n");
		printf(
				"================================================================ \n");
		printf("Plese input Lock type \n");

		scanf("%d%c", &tempi, &dummy);
		type = tempi;

		err_code = tel_get_sim_personalization_status(type, &request_id);
		printf("Error Code [%x]\n", err_code);

	} else if (memcmp(buf, "setlang", sizeof("setlang") - 1) == 0) {
		int tempi;
		char dummy;

		TelSimLanguagePreferenceCode_t language = 0x00;

		printf("------------------------------ \n");
		printf("select language which will be updated: \n");
		printf("------------------------------ \n");
		printf("should select inside here ( 0.DE / 1.EN / 2.IT / 3.FR ) \n");
		printf(
				"should select inside here ( 4.SPANISH / 5.DUTCH / 6.SWEDISH / 7.DANISH ) \n");
		printf(
				"should select inside here ( 8.PORTUGUESE / 9.FINNISH / 10.NORWEGIAN / 11.GREEK ) \n");
		printf(
				"should select inside here ( 12.TURKISH / 13.HUNGARIAN / 14.POLISH / 15.KOREAN ) \n");
		printf(
				"should select inside here ( 16.CHINESE / 17.RUSSIAN / 18.JAPANESE ) \n");
		printf("------------------------------ \n");

		printf("\nPlease input type: \n");
		scanf("%d%c", &tempi, &dummy);

		language = (TelSimLanguagePreferenceCode_t) tempi;

		err_code = tel_set_sim_language(language, &request_id);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API SUCCESS");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "apdu", sizeof("apdu") - 1) == 0) {
		printf("------------------------------ \n");
		printf("APDU\n");
		printf("------------------------------ \n");

		TelSimApdu_t apdu_data = { 0, };

		char tmp_apdu[4] = { 0x41, 0xC0, 0x05, 0x6f };

		apdu_data.apdu_len = 4;
		apdu_data.apdu = (unsigned char*) &tmp_apdu;

		err_code = tel_req_sim_apdu(&apdu_data, &request_id);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API SUCCESS");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "atr", sizeof("atr") - 1) == 0) {
		printf("------------------------------ \n");
		printf("ATR request: \n");
		printf("------------------------------ \n");

		err_code = tel_req_sim_atr(&request_id);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API SUCCESS");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	}

	////////////////////////////////////////////////////////////////////////
	///////////////////////  Phone Book ///////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	else if (memcmp(buf, "pbcapa", sizeof("pbcapa") - 1) == 0) {
		TEST_DEBUG("PB capability info");

		err_code = tel_get_sim_pb_3g_meta_info(&request_id);
		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "pbcnt", sizeof("pbcnt") - 1) == 0) {
		TEST_DEBUG("PB storage count info");

		TelSimPbFileType_t storage_type = __InputStorageType();

		__PrintStorageType(storage_type);

		err_code = tel_get_sim_pb_count(storage_type, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "pbentry", sizeof("pbentry") - 1) == 0) {
		TEST_DEBUG("PB Entry info");

		TelSimPbFileType_t storage_type = __InputStorageType();

		err_code = tel_get_sim_pb_meta_info(storage_type, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	///////////////////////////////////
	/*
	 0x01 : ADD
	 0x02 : DELETE
	 0x03 : EDIT
	 0x04 : WRITE (�ش� index�� data ������ ������ unconditional write)
	 0x05 : DELETE ALL (�ش� phone book id�� ���� �����ϴ� ��� �����͸� delete)
	 */
	else if (memcmp(buf, "pbadd", sizeof("pbadd") - 1) == 0) {
		TelSimPbRecordData_t pb_add = { 0, };
		char dummy;
		char unicode_text[256];
		int unicode_text_len = 0;
		int index = 0;

		TelSimPbFileType_t storage_type = __InputStorageType();
		pb_add.StorageFileType = storage_type;

		TEST_DEBUG("Plese input index : ");
		scanf("%d%c", &index, &dummy);
		pb_add.Index = index;

		if (pb_add.StorageFileType == TAPI_SIM_PB_EN || pb_add.StorageFileType
				== TAPI_SIM_PB_FDN || pb_add.StorageFileType
				== TAPI_SIM_PB_MSISDN || pb_add.StorageFileType
				== TAPI_SIM_PB_LDN || pb_add.StorageFileType == TAPI_SIM_PB_ADN
				|| pb_add.StorageFileType == TAPI_SIM_PB_SDN) {
			int tmpInput;
			TEST_DEBUG("2g phone book entry");

			TEST_DEBUG(" Number Types are below ");
			TEST_DEBUG(" 1 - INTERNATIONAL");
			TEST_DEBUG(" 2 - NATIONAL");
			TEST_DEBUG(" 3 - NETWORK");
			TEST_DEBUG(" 4 - DEDICATE");
			TEST_DEBUG("Select Numer Type: ");

			scanf("%d%c", &tmpInput, &dummy);
			pb_add.ContactInfo.Pb2GData.NumType = tmpInput;

			TEST_DEBUG("Plese input contact number : ");
			_fgets((char *) pb_add.ContactInfo.Pb2GData.Number, 15);
			pb_add.ContactInfo.Pb2GData.NumLen = strlen(
					(char*) pb_add.ContactInfo.Pb2GData.Number);

			TEST_DEBUG(" Encryption Types are below ");
			TEST_DEBUG(" 1 - ASCII - DO NOT SUPPORT");
			TEST_DEBUG(" 2 - GSM7 Bit");
			TEST_DEBUG(" 3 - UCS2");
			TEST_DEBUG(" 4 - HEX - DO NOT SUPPORT");
			TEST_DEBUG("Select Text Encryption Type: ");

			scanf("%d%c", &tmpInput, &dummy);
			pb_add.ContactInfo.Pb2GData.NameEncryptType = tmpInput - 1;

			TEST_DEBUG("Plese input contact name : ");
			_fgets((char *) pb_add.ContactInfo.Pb2GData.Name, 20);

			if (pb_add.ContactInfo.Pb2GData.NameEncryptType
					== TAPI_SIM_TEXT_ENC_UCS2) {
				TEST_DEBUG("TAPI_SIM_TEXT_ENC_UCS2 Encoding");
				/* unicode encoding */
				if (FALSE == __encode_text_unicode(
						strlen((char *) pb_add.ContactInfo.Pb2GData.Name),
						(char*) pb_add.ContactInfo.Pb2GData.Name,
						(char *) &unicode_text_len, unicode_text)) {
					TEST_DEBUG("Encoding failed ! ");

				} else {
					pb_add.ContactInfo.Pb2GData.NameLen = unicode_text_len;
					memcpy(pb_add.ContactInfo.Pb2GData.Name, unicode_text,
							unicode_text_len);
				}
			} else {
				pb_add.ContactInfo.Pb2GData.NameLen = strlen(
						(char *) pb_add.ContactInfo.Pb2GData.Name);
			}
		} else if (pb_add.StorageFileType == TAPI_SIM_PB_3GSIM) {
			int tempi;
			char num_field_type;
			int i = 0;

			TEST_DEBUG("3g phone book entry");

			TEST_DEBUG("Input Nuumber of fields to be entered: ");

			scanf("%d%c", &tempi, &dummy);

			num_field_type = tempi;

			pb_add.ContactInfo.Pb3GData.FileTypeCount = num_field_type;

			for (i = 0; i < num_field_type; i++) {
				TEST_DEBUG(" Field Types are below ");
				TEST_DEBUG(" 1 - Contact Name : (EF_ADN)");
				TEST_DEBUG(" 2 - Contact Mobile Number (EF_ADN)");
				TEST_DEBUG(" 3 - Contact Another Number (EF_ANR)");
				TEST_DEBUG(" 4 - Contact Email (EF_EMAIL)");
				TEST_DEBUG(" 5 - Contact Nick Name (EF_SNE)");
				TEST_DEBUG(" 6 - Contact Group (EF_GRP)");
				TEST_DEBUG(" 7 - Contact Hidden entry (EF_PBC) - NOT SUPPORT YET");
				TEST_DEBUG(" 8 - Contact Another Number A (EF_ANRA)");
				TEST_DEBUG(" 9 - Contact Another Number B (EF_ANRB)");

				TEST_DEBUG("Select Field type: ");

				scanf("%d%c", &tempi, &dummy);

				pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileType = tempi;

				if (tempi == 1 || tempi == 4 || tempi == 5 || tempi == 6) {
					TEST_DEBUG(" Encryption Types are below ");
					TEST_DEBUG(" 1 - ASCII");
					TEST_DEBUG(" 2 - GSM7 Bit");
					TEST_DEBUG(" 3 - UCS2");
					TEST_DEBUG(" 4 - HEX");
					TEST_DEBUG("Select Text Encryption Type: ");

					scanf("%d%c", &tempi, &dummy);
					pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.EncryptionType
							= tempi - 1;

				} else {
					TEST_DEBUG(" Number Types are below ");
					TEST_DEBUG(" 1 - INTERNATIONAL");
					TEST_DEBUG(" 2 - NATIONAL");
					TEST_DEBUG(" 3 - NETWORK");
					TEST_DEBUG(" 4 - DEDICATE");
					TEST_DEBUG("Select Numer Type: ");

					scanf("%d%c", &tempi, &dummy);
					pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.NumType
							= tempi;
				}

				if (pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
						== TAPI_PB_3G_GRP) {
					int c, count, k = 0;

					TEST_DEBUG("Please input number of index to add in GRP");
					scanf("%d%c", &tempi, &dummy);

					count = tempi;
					for (c = 0; c < count; c++) {
						TEST_DEBUG("Please input index of GRP: ");
						scanf("%d%c", &tempi, &dummy);

						pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData[k]
								= tempi;
						k++;
						//pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData[k] = 0;
						//k++;
					}
					pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength
							= count;
				} else if (pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
						== TAPI_PB_3G_NAME
						|| pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
								== TAPI_PB_3G_EMAIL
						|| pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileType
								== TAPI_PB_3G_SNE) {
					TEST_DEBUG("Plese input  field string data: ");
					_fgets(
							(char *) pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
							30);

					TEST_DEBUG("strlen about input data[%d]",strlen((char *)pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData) );

					if (pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataType.EncryptionType
							== TAPI_SIM_TEXT_ENC_UCS2) {
						/* unicode encoding */
						if (FALSE
								== __encode_text_unicode(
										strlen(
												(char *) pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData),
										(char *) pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
										(char *) &unicode_text_len,
										unicode_text)) {
							TEST_DEBUG("Encoding failed ! ");
						} else {
							memcpy(
									pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
									unicode_text, unicode_text_len);
							pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength
									= unicode_text_len;
						}
					} else {
						pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength
								= strlen(
										(char *) pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData);
					}
				} else {
					TEST_DEBUG("Plese input  field data: ");
					_fgets(
							(char *) pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData,
							20);
					pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileDataLength
							= strlen(
									(char *) pb_add.ContactInfo.Pb3GData.PbFileDataInfo[i].FileData);
				}

			}

		} else if (pb_add.StorageFileType == TAPI_SIM_PB_AAS
				|| pb_add.StorageFileType == TAPI_SIM_PB_GAS) {
			TEST_DEBUG("additional num Alpha string or Group Alpha string ");

			TEST_DEBUG("Plese input Alpha name : ");
			_fgets(
					(char *) pb_add.ContactInfo.PbAdditionalNumStringData.TextData,
					20);
#if 0
			/* GSM 7bit conversion */
			__encode_gsm_7_bit_chars(strlen((char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData) ,
					(char *)pb_add.ContactInfo.PbAdditionalNumStringData.TextData, (char *)&unicode_text_len, unicode_text);

			memcpy(pb_add.ContactInfo.PbAdditionalNumStringData.TextData, unicode_text, unicode_text_len );

			pb_add.ContactInfo.PbAdditionalNumStringData.TextLength = unicode_text_len;

			pb_add.ContactInfo.PbAdditionalNumStringData.EncryptionType = TAPI_SIM_TEXT_ENC_GSM7BIT; // always 2 for GAS and AAS
#endif
			pb_add.ContactInfo.PbAdditionalNumStringData.TextLength
					= strlen(
							(char *) pb_add.ContactInfo.PbAdditionalNumStringData.TextData);
			pb_add.ContactInfo.PbAdditionalNumStringData.EncryptionType
					= TAPI_SIM_TEXT_ENC_GSM7BIT; // But real type is ASCII (because current GSM7 is GSM7 with bit 8 set to '0')

		} else {
			TEST_DEBUG("Undefined Storage File Type [0x%x]", pb_add.StorageFileType);

		}

		err_code = tel_update_sim_pb_record(&pb_add, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "pbdel", sizeof("pbdel") - 1) == 0) {
		TelSimPbFileType_t storage_type = __InputStorageType();

		TEST_DEBUG(" PB delete ");

		unsigned short index = 0;
		char dummy = 0;
		TEST_DEBUG("Plese input index : ");
		scanf("%d%c", (int *)&index, &dummy);

		err_code = tel_delete_sim_pb_record(storage_type, index, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}

	} else if (memcmp(buf, "pbread", sizeof("pbread") - 1) == 0) {
		TEST_DEBUG("PB read");

		unsigned short index = 0;
		char dummy = 0;

		TelSimPbFileType_t storage_type = __InputStorageType();

		TEST_DEBUG("Plese input index : ");
		scanf("%d%c", (int *)&index, &dummy);

		err_code = tel_read_sim_pb_record(storage_type, index, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "isimauth", sizeof("isimauth") - 1) == 0) {
		TEST_DEBUG("ISIM Authentication ");
#if 0
		getchar();

		TelSimAuthenticationData_t auth_req = { 0, };

		memset(&auth_req, 0, sizeof(TelSimAuthenticationData_t));

		TEST_DEBUG(" Enter  RAND data ");
		_fgets((char *) &auth_req.rand_data, 20);

		auth_req.rand_length
				= strlen((char *) auth_req.rand_data);

		TEST_DEBUG(" Enter Authentication  data ");
		_fgets((char *) &auth_req.autn_data, 20);

		auth_req.autn_length = strlen((char *) auth_req.autn_data);

		TEST_DEBUG("TelTapiSimIsimAuthenticationRequest is not tested yet!");
		/*
		 err_code = TelTapiSimIsimAuthenticationRequest( &auth_req, &request_id);

		 if(err_code == TAPI_API_SUCCESS)
		 {
		 TEST_DEBUG("TAPI API SUCCESS");
		 }
		 else
		 {
		 TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		 }
		 */
#endif
	}

	else if (memcmp(buf, "sapcon", sizeof("sapcon") - 1) == 0) {
		TEST_DEBUG("SAP Connect request  ");
		TelSimSapConnect_t con_req = { 0, };
		con_req.MsgId = TAPI_SIM_SAP_CONNECT_REQ;
		con_req.MaxMsgSize = 10; //temp for testing only - not clear

		err_code = tel_req_sap_connection(&con_req, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "sapdiscon", sizeof("sapdiscon") - 1) == 0) {
		TEST_DEBUG("SAP Disconnect request  ");
		TelSimSapConnect_t con_req = { 0, };
		con_req.MsgId = TAPI_SIM_SAP_DISCONNECT_REQ;
		con_req.MaxMsgSize = 10; //temp for testing only - not clear

		err_code = tel_req_sap_connection(&con_req, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "sapstatus", sizeof("sapstatus") - 1) == 0) {
		TEST_DEBUG("SAP Status request  ");

		err_code = tel_req_sap_connection_status(&request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "sapatr", sizeof("sapatr") - 1) == 0) {
		TEST_DEBUG("SAP ATR request  ");

		err_code = tel_req_sap_transfer_atr(&request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "sapapdu", sizeof("sapapdu") - 1) == 0) {
		TEST_DEBUG("SAP APDU transfer  ");

		int file_id = TAPI_SIM_EFILE_ICCID;

		/* for selecting EF ICCID */
		TelSimSapApduData_t apdu_data;

		apdu_data.ApduLength = 7;
		apdu_data.Apdu[0] = 0xA0; // class
		apdu_data.Apdu[1] = 0xA4; // instruction
		apdu_data.Apdu[2] = 0; // p1 = parameter 1
		apdu_data.Apdu[3] = 0; // p2 = parameter 2
		apdu_data.Apdu[4] = 2; // p3 - parameter 3

		memcpy(&apdu_data.Apdu[5], &file_id, sizeof(unsigned short));

		err_code = tel_req_sap_transfer_apdu(&apdu_data, &request_id);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API SUCCESS");
		} else {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "sapsetproto", sizeof("sapsetproto") - 1) == 0) {
		TEST_DEBUG("SAP set protocol ");
		TelSimSapProtocol_t protocol = TAPI_SIM_SAP_PROTOCOL_T0;

		err_code = tel_req_sap_transport_protocol(protocol, &request_id);

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	else if (memcmp(buf, "sapsimpwr", sizeof("sapsimpwr") - 1) == 0) {
		int tempi = 0;
		TelSimSapMsgId_t msdId = 0;

		char dummy = 0;

		TEST_DEBUG("SAP SIM power  ");

		TEST_DEBUG("Please input SAP SIM power on(1) / off(!1): ");

		scanf("%d%c", &tempi, &dummy);
		if (tempi == 1)
			msdId = TAPI_SIM_SAP_POWER_SIM_ON_REQ;
		else
			msdId = TAPI_SIM_SAP_POWER_SIM_OFF_REQ;

		err_code = tel_req_sap_power_operation(msdId, &request_id);// only for testing

		if (err_code != TAPI_API_SUCCESS) {
			TEST_DEBUG("TAPI API FAIL: Error Code [0x%x]",err_code);
		}
	}

	if (memcmp(buf, "close", sizeof("close") - 1) == 0) {
		TEST_DEBUG(" close reuqest ");

		//	err_code = tapi_sim_close(92);

		//err_code = tapi_sim_close(g_test_hsim);
		TEST_DEBUG(" ret value from close [0x%x]", err_code);

		if (err_code == TAPI_API_SUCCESS) {
			TEST_DEBUG(" sim close OK!");
		} else {
			TEST_DEBUG(" SIM close request returned error [0x%x] !",err_code );
		}
		TEST_DEBUG("Request ID [0x%x]", request_id);
	}

	if (memcmp(buf, "q", sizeof("q") - 1) == 0) {
		TEST_DEBUG("Finished Testing this module, Going back to Main Menu");
		return -1;
		//exit(0);
	}

	return 0;
}

void sim_select_loop(void) {
	int ret;
	fd_set readfds;

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret) {

			if (FD_ISSET(0, &readfds)) {
				if (sim_read_key_input() < 0)
					break;
			}
		}
	}
}

void* sim_test_thread_loop(void* data) {
	TEST_DEBUG(" sim_test_thread_loop : called...");

	sim_select_loop();

	TEST_DEBUG(" sim_test_thread_loop : stdin Loop exited");

	pthread_exit(NULL);
}

int sim_main(int argc, char *argv[]) {
	pthread_t p_thread;

	p_thread = pthread_create(&p_thread, NULL, sim_test_thread_loop, NULL);

	if (p_thread != 0) {
		TEST_DEBUG("Failed while creating a thread to read from stdin");
		return -1;
	} else {
		TEST_DEBUG("A thread was created to read from stdin ..");
	}

	sim_test_subscribe_tapi_events();

	pthread_join(p_thread, NULL);

	TEST_DEBUG("Exiting sim test program bye bye !");

	return 0;
}

