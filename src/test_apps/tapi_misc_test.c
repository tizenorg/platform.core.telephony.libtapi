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
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <glib.h>
#include <string.h>
#include <unistd.h>

#include "ITapiMisc.h"
#include "TapiCommon.h"
#include "TelErr.h"
#include "vconf.h"
#include "TelUtility.h"
#include "TapiEvent.h"

typedef struct {
	int group;
	int type;
	int len;
	void *dst;
} edata_t;

#define TEST_DEBUG(frmt, args...) \
{do { fprintf(stderr, "[MISC Test][%s:%04d] "frmt "\n", __func__,__LINE__, ##args); } while (FALSE); }

int error_code;
extern int EvtDeliveryHandle;

TelMiscAlarmInfo_t alarm_info;
static unsigned int * MISC_subscription_id;

void __PrintNAMInfoMASK(char* str, TS_UINT16 nam_info_mask) {
	switch (nam_info_mask) {
	case TAPI_MISC_NAM_INFO_MASK_TOTAL:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_TOTAL [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_CURRENT_INDEX:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_CURRENT_INDEX [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_REQUEST_INDEX:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_REQUEST_INDEX [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_AUTONAM:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_AUTONAM [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_MCC:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_MCC [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_MNC:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_MNC [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_MIN:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_MIN [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_MDN:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_MDN [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_NAM_NAME:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_NAM_NAME [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_NAM_CHANGE:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_NAM_CHANGE [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_NAM_NUM:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_NAM_NUM [%d]", str, nam_info_mask)
		;
		break;
	case TAPI_MISC_NAM_INFO_MASK_ALL:
		TEST_DEBUG("%s TAPI_MISC_NAM_INFO_MASK_ALL [%d]", str, nam_info_mask)
		;
		break;
	default:
		TEST_DEBUG("%s Undefined MASK [0x%x]", str, nam_info_mask)
		;
		break;
	}
}

//static int  misc_async_event_callback (TelTapiEventClass_t	eventClass,int	eventType, void * param2)
static void misc_async_event_callback(TelTapiEvent_t *event) {
	TEST_DEBUG("EVENT TYPE:[0x%x], STATUS:[0x%x]", event->EventType, event->Status);

	switch (event->EventClass) {
	case TAPI_EVENT_CLASS_MISC: {
		switch (event->EventType) {
		case TAPI_EVENT_MISC_NITZ_GETCNF: {
			tapi_misc_time_zone_info_type zone_info;
			memcpy(&zone_info, (tapi_misc_time_zone_info_type*) event->pData,
					sizeof(tapi_misc_time_zone_info_type));

			TEST_DEBUG("$$$$$$ TAPI_EVENT_MISC_NITZ_GETCNF $$$$$$ ");

			TEST_DEBUG("time info valid : [%d](0:invalid, 1:nw, 2:RTC, 3:RTC BACKUP) ",zone_info.time_zone_valid);
			TEST_DEBUG("Daylight_valid : [%d] ",zone_info.bDaylight_valid);
			TEST_DEBUG("Year : [%d] ",zone_info.year);
			TEST_DEBUG("Month : [%d] ",zone_info.month);
			TEST_DEBUG("Day : [%d] ",zone_info.day);
			TEST_DEBUG("Hour : [%d] ",zone_info.hour);
			TEST_DEBUG("Minute : [%d] ",zone_info.minute);
			TEST_DEBUG("Second : [%d] ",zone_info.second);
			TEST_DEBUG("Time_zone :[%d] ",zone_info.time_zone);
			TEST_DEBUG("Daylight_adjust : [%d] ",zone_info.daylight_adjust);
			TEST_DEBUG("Day_of_week :[%d] ",zone_info.day_of_week);

			alarm_info.year = zone_info.year;
			alarm_info.month = zone_info.month;
			alarm_info.day = zone_info.day;
			alarm_info.hour = zone_info.hour;
			alarm_info.minute = zone_info.minute;
			alarm_info.second = zone_info.second;
			alarm_info.time_zone = zone_info.time_zone;
			alarm_info.day_of_week = zone_info.day_of_week;

			break;
		}
		case TAPI_EVENT_MISC_SERIAL_NUMBER_IND: {
			TelMiscSNInformation SN = { 0, };

			int index;

			memcpy(&SN, (char*) event->pData, sizeof(TelMiscSNInformation));
			TEST_DEBUG("$$$$$$ TAPI_EVENT_MISC_SERIAL_NUMBER_IND $$$$$$ ");
			TEST_DEBUG("Number Len :[%d] ", SN.sn_len);
			switch (SN.sn_index) {
			case TAPI_MISC_ME_ESN:
				TEST_DEBUG("SN Index: ME ESN");
				for (index = 0; index < 4; index++) {
					TEST_DEBUG("Number :[0x%x]", SN.szNumber[index]);
				}
				break;
			case TAPI_MISC_ME_MEID:
				TEST_DEBUG("SN Index: ME MEID");
				for (index = 0; index < 8; index++) {
					TEST_DEBUG("Number :[0x%x]", SN.szNumber[index]);
				}
				break;
			default:
				break;
			}
			break;
		}

		case TAPI_EVENT_MISC_NAM_INFO_NOTI:
		case TAPI_EVENT_MISC_GET_NAM_INFO_CNF: {
			TelMiscNamInfo_t NAM = { 0, };

			memcpy(&NAM, (char*) event->pData, sizeof(TelMiscNamInfo_t));
			TEST_DEBUG("$$$$$$ TAPI_EVENT_MISC_GET_NAM_INFO_CNF or NAM_INFO_NOTI $$$$$$ ");
			__PrintNAMInfoMASK("Nam Info Mask: ", NAM.nam_info_mask);
			TEST_DEBUG("Num of Total Nam : %d ", NAM.nam_total);
			TEST_DEBUG("Index of Current Selected NAM : %d ", NAM.nam_current_index);
			TEST_DEBUG("Index of Request Nam : %d ", NAM.nam_req_index);
			TEST_DEBUG("NAM Auto(0:disable / 1:enable) : %d ", NAM.nam_auto);
			TEST_DEBUG("NAM MCC : [%s] ", NAM.nam_mcc);
			TEST_DEBUG("NAM MNC : [%s] ", NAM.nam_mnc);
			TEST_DEBUG("NAM MIN : [%s] ", NAM.nam_min);
			TEST_DEBUG("NAM MDN : [%s] ", NAM.nam_mdn);
			TEST_DEBUG("NAM NAME : [%s] ", NAM.nam_name);

			break;
		}

		default:
			TEST_DEBUG(" MISC TEST -----TAPI_MISC EVENT TYPE DEFAULT CASE : [%d] ", event->EventType)
			;
			break;
		}
		break;
	}

	default:
		break;

	}

	TEST_DEBUG(" ########## End of MISC EVENT  ##################");

	return;
}

int misc_read_key_input(int option) {
	int ret;
	char buf[512] = { 0, };
	char msg[512];
	char diallingNum[256];
	int diallingNum_len = 0;
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	memset(buf, 0, sizeof(buf));
	memset(diallingNum, 0, sizeof(diallingNum));
	memset(msg, 0, sizeof(msg));
	diallingNum_len = 0;

	if (option == 10) {
		TEST_DEBUG("*************** MISC TEST MENU     ***************");
		TEST_DEBUG("1g.tel_get_misc_timezone_info	(UMTS/CDMA)");
		TEST_DEBUG("1s.tapi_misc_timezone_setinfo	(UMTS/CDMA)");
		TEST_DEBUG("4.tel_get_misc_alarm_info	(IPCv3.0)");
		TEST_DEBUG("5.ME serial number		(UMTS/CDMA)");
		TEST_DEBUG("6.ME version info	(UMTS/CDMA)");
		TEST_DEBUG("7.ts_get_misc_nam_info	(CDMA)");
		TEST_DEBUG("8.emergency mode control (UMTS/CDMA)");
		TEST_DEBUG("0.exit");
		TEST_DEBUG("*****************************************************\n");

		ret = read(0, buf, sizeof(buf));
		if (ret < 0) {
			if (errno == EINTR)
				perror("read(1)");
			return -1;
		} else if (ret == 0)
			return ret;

		if (memcmp(buf, "0", sizeof("0") - 1) == 0) {
			TEST_DEBUG("Finished Testing this module, Going back to Main Menu");
			return -1;
		} else if (memcmp(buf, "1g", sizeof("1g") - 1) == 0) {
			TEST_DEBUG("Get time info");
			ret_val = tel_get_misc_timezone_info();
			TEST_DEBUG("API RETURN VALUE : [%d]" , ret_val);
		} else if (memcmp(buf, "1s", sizeof("1s") - 1) == 0) {
			TEST_DEBUG("Set time info");
			tapi_misc_time_zone_info_type timezone_info = { 0, };
			timezone_info.bDaylight_valid = 1;
			timezone_info.year = 10;
			timezone_info.month = 1;
			timezone_info.day = 1;
			timezone_info.hour = 1;
			timezone_info.minute = 1;
			timezone_info.second = 1;
			timezone_info.time_zone = 36;
			timezone_info.time_zone_valid = TAPI_MISC_NITZ_RTC_BKUP_PHONE;
			timezone_info.daylight_adjust = TAPI_MISC_NITZ_NOADJUST;
			timezone_info.day_of_week = TAPI_MISC_NITZ_DOW_FRI;

			ret_val = tel_set_misc_timezone_info(&timezone_info);

			TEST_DEBUG("API RETURN VALUE : [%d]" , ret_val);
		} else if (memcmp(buf, "2", sizeof("2") - 1) == 0) {
			if (memcmp(buf + 1, "a", sizeof("a") - 1) == 0)
				alarm_info.minute += 1;
			else if (memcmp(buf + 1, "b", sizeof("b") - 1) == 0)
				alarm_info.minute += 2;
			else if (memcmp(buf + 1, "c", sizeof("c") - 1) == 0)
				alarm_info.minute += 3;
			else if (memcmp(buf + 1, "d", sizeof("d") - 1) == 0)
				alarm_info.minute += 10;
			else {
				TEST_DEBUG("Wrong Selection!!");
				return 0;
			}
		} else if (memcmp(buf, "4", sizeof("4") - 1) == 0) {
			TEST_DEBUG("Get current alarm status");
			//ret_val = TelTapiMiscGetAlarmInfo();
			TEST_DEBUG("API RETURN VALUE : [%d]" , ret_val);
		} else if (memcmp(buf, "5", sizeof("5") - 1) == 0) {
			TelMiscSNIndexType_t sn_index;
			TelMiscSNInformation sn_info = { 0, };

			int iSelect = 0;
			TEST_DEBUG("1. IMEI (UMTS)");
			TEST_DEBUG("2. ME_ESN (CDMA)");
			TEST_DEBUG("3. ME_MEID (CDMA)");
			TEST_DEBUG("Select: ");
			scanf("%d", &iSelect);

			switch (iSelect) {
			case 1:
				sn_index = TAPI_MISC_ME_IMEI;
				TEST_DEBUG("Get ME IMEI...!\n")
				;
				break;
			case 2:
				sn_index = TAPI_MISC_ME_ESN;
				TEST_DEBUG("Get ME ESN...!\n")
				;
				break;
			case 3:
				sn_index = TAPI_MISC_ME_MEID;
				TEST_DEBUG("Get ME MEID...!\n")
				;
				break;
			default:
				TEST_DEBUG("Wrong Selection!!\n")
				;
				return 0;
				break;
			}
			ret_val = tel_get_misc_me_sn(sn_index, &sn_info);

			if (ret_val == TAPI_API_SUCCESS) {
				TEST_DEBUG("sn_index[%d]",sn_info.sn_index);
				TEST_DEBUG("sn_len[%d]",sn_info.sn_len);
				TEST_DEBUG("szNumber[%s]",sn_info.szNumber);
			} else {
				TEST_DEBUG("FAIL API RETURN:[%d]",ret_val);
			}
		} else if (memcmp(buf, "6", sizeof("6") - 1) == 0) {
			TelMiscVersionInformation ver_info = { 0, };
			TEST_DEBUG("ME version info..!");
			ret_val = tel_get_misc_me_version(&ver_info);
			if (ret_val == TAPI_API_SUCCESS) {
				TEST_DEBUG("ver_mask[0x%02x]",ver_info.ver_mask);
				TEST_DEBUG("szHwVersion[%s]",ver_info.szHwVersion);
				TEST_DEBUG("szSwVersion[%s]",ver_info.szSwVersion);
				TEST_DEBUG("szRfCalDate[%s]",ver_info.szRfCalDate);
				TEST_DEBUG("szProductCode[%s]",ver_info.szProductCode);
			} else {
				TEST_DEBUG("FAIL API RETURN:[%d]",ret_val);
			}
		} else if (memcmp(buf, "7", sizeof("7") - 1) == 0) {
			TS_WORD nam_info_mask;
			TelMiscNamIndex_t nam_index;

			int iSelect = 0;
			TEST_DEBUG("1. The Number of Total NAM");
			TEST_DEBUG("2. The Index of current selected NAM");
			TEST_DEBUG("3. The Index of request NAM");
			TEST_DEBUG("4. Auto NAM");
			TEST_DEBUG("5. MCC");
			TEST_DEBUG("6. MNC");
			TEST_DEBUG("7. MIN");
			TEST_DEBUG("8. MDN");
			TEST_DEBUG("9. Nam Name");
			TEST_DEBUG("10. Nam Change");
			TEST_DEBUG("11. Current Nam num");
			TEST_DEBUG("12. All parameter");

			TEST_DEBUG("Select: ");
			scanf("%d", &iSelect);

			switch (iSelect) {
			case 1:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_TOTAL;
				break;
			case 2:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_CURRENT_INDEX;
				break;
			case 3:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_REQUEST_INDEX;
				break;
			case 4:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_AUTONAM;
				break;
			case 5:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_MCC;
				break;
			case 6:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_MNC;
				break;
			case 7:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_MIN;
				break;
			case 8:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_MDN;
				break;
			case 9:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_NAM_NAME;
				break;
			case 10:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_NAM_CHANGE;
				break;
			case 11:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_NAM_NUM;
				break;
			case 12:
				nam_info_mask = TAPI_MISC_NAM_INFO_MASK_ALL;
				break;
			default:
				TEST_DEBUG("Wrong Selection!!")
				;
				return 0;
				break;
			}

			TEST_DEBUG("Select MISC Nam Index(1~5): ");
			scanf("%d", &iSelect);
			switch (iSelect) {
			case 1:
				nam_index = TAPI_MISC_NAM_1;
				break;
			case 2:
				nam_index = TAPI_MISC_NAM_2;
				break;
			case 3:
				nam_index = TAPI_MISC_NAM_3;
				break;
			case 4:
				nam_index = TAPI_MISC_NAM_4;
				break;
			case 5:
				nam_index = TAPI_MISC_NAM_5;
				break;
			default:
				TEST_DEBUG("Wrong Selection!!")
				;
				return 0;
				break;
			}

			TEST_DEBUG("Current NAM index(1) or Request NAM index(2): ");
			scanf("%d", &iSelect);
			switch (iSelect) {
			case 1:
				nam_info_mask |= TAPI_MISC_NAM_INFO_MASK_CURRENT_INDEX;
				break;
			case 2:
				nam_info_mask |= TAPI_MISC_NAM_INFO_MASK_REQUEST_INDEX;
				break;
			default:
				TEST_DEBUG("Wrong Selection!!")
				;
				return 0;
				break;
			}

			TEST_DEBUG("You selected nam infomask[0x%x] & index[0x%x]", nam_info_mask, nam_index);
			TEST_DEBUG("Get ME NAM Info...!");
			ret_val = tel_get_misc_nam_info(nam_info_mask, nam_index);
			TEST_DEBUG("API RETURN VALUE : [%d]" , ret_val);
		}

		else if (memcmp(buf, "8", sizeof("8") - 1) == 0) {
			int selection = 0;
			int mode = 0;
			char dummy;
			TEST_DEBUG("device emergency mode control");
			TEST_DEBUG("Please select operation: ");
			TEST_DEBUG("1:get status, 2:enable, 3:disable");
			scanf("%d%c",&selection,&dummy);

			switch (selection) {
			case 1:
				ret_val = tel_get_misc_emergency_mode(&mode);
				TEST_DEBUG("ret_val[%d], mode[%d]",ret_val,mode);
				break;
			case 2:
				ret_val = tel_set_misc_emergency_mode(1);
				TEST_DEBUG("enable ret_val[%d]",ret_val);
				break;
			case 3:
				ret_val = tel_set_misc_emergency_mode(0);
				TEST_DEBUG("disable ret_val[%d]",ret_val);
				break;
			}
		} else {
			TEST_DEBUG("Invalid Selection!\n");
		}

	}

	return 0;
}

void misc_select_loop(int option) {
	int ret;
	fd_set readfds;

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret) {
			if (FD_ISSET(0, &readfds)) {
				if (misc_read_key_input(option) < 0)
					break;
			}
		}
	}
}

int misc_test_subscribe_tapi_events(void) {
	int i = 0;
	int ret_val = TRUE;
	int iNumOfMISCEvt = 0;
	TapiResult_t api_err = TAPI_API_SUCCESS;

	int MISCEvtList[] = { TAPI_EVENT_MISC_NITZ_GETCNF, /**< 0x0A00, */
			TAPI_EVENT_MISC_NITZ_NOTIFY, /**<     */
			TAPI_EVENT_MISC_NITZ_IND, /**<     */
			TAPI_EVENT_MISC_AT_FUS, /**<     */
			//		TAPI_EVENT_MISC_ALARM_SETCNF,      /**< alarm set confirm message */
			//		TAPI_EVENT_MISC_ALARM_GETCNF,	/**< alarm message */
			TAPI_EVENT_MISC_SERIAL_NUMBER_IND,
			TAPI_EVENT_MISC_GET_NAM_INFO_CNF, TAPI_EVENT_MISC_SET_NAM_INFO_CNF,
			TAPI_EVENT_MISC_NAM_INFO_NOTI };

	iNumOfMISCEvt = sizeof(MISCEvtList) / sizeof(int);
	MISC_subscription_id = (unsigned int *) calloc(iNumOfMISCEvt,
			sizeof(unsigned int));

	for (i = 0; i < iNumOfMISCEvt; i++) {
		api_err = tel_register_event(MISCEvtList[i], &MISC_subscription_id[i],
				(TelAppCallback) &misc_async_event_callback, NULL);
		if (api_err != TAPI_API_SUCCESS) {
			TEST_DEBUG("isn't subscribed. event id is %d, api_err is %d\n", MISCEvtList[i],api_err);
			ret_val = FALSE;
			break;
		}
	}

	return ret_val;
}
