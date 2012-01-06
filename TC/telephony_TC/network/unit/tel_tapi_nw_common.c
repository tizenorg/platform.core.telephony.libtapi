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

#include <tet_api.h>
#include "tel_tapi_nw_common.h"

TelNetworkList_t gNetworkList = { 0, };
GMainLoop *mainloop;

void GetNetworkInfoCB(void *data)
{
	TelNetworkInfo_t tNetworkInfo;
	memcpy(&tNetworkInfo, data, sizeof(TelNetworkInfo_t));

	printf("GetNetworkInfoCB() : PLMN is %ld\n", tNetworkInfo.SysId.SysId);
	printf("GetNetworkInfoCB() : PLMN is %s\n", tNetworkInfo.SysId.szSystemName);

	if (tNetworkInfo.SysId.bPlmnForbidden == TRUE) {
		printf("GetNetworkInfoCB(): Network Forbidden\n");
	}

	if (tNetworkInfo.SysId.bHome == TRUE) {
		printf("GetNetworkInfoCB(): Home Network \n");
	}

	if (tNetworkInfo.bRoaming == TRUE) {
		printf("GetNetworkInfoCB(): Roaming Network \n");
	}

	/*SystemType*/
	switch (tNetworkInfo.SysId.SystemType) {
		case TAPI_NETWORK_SYSTEM_GSM:
			printf("SystemType: TAPI_NETWORK_SYSTEM_GSM\n");
			break;

		case TAPI_NETWORK_SYSTEM_UMTS:
			printf("SystemType: TAPI_NETWORK_SYSTEM_UMTS\n");
			break;

		case TAPI_NETWORK_SYSTEM_GPRS:
			printf("SystemType: TAPI_NETWORK_SYSTEM_GPRS\n");
			break;

		default:
			printf("SystemType: Other than GSM,GPRS,UMTS System type \n");
			break;
	}

	/*ServiceType*/
	switch (tNetworkInfo.SysId.ServiceType) {
		case TAPI_NETWORK_SERVICE_TYPE_COMBINED:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_COMBINED\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_CS:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_CS\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_PS:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_PS\n");
			break;

		default:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_NO_SVE\n");
			break;
	}

	/*NetworkMode*/
	switch (tNetworkInfo.SysId.NetworkMode) {
		case TAPI_NETWORK_MODE_AUTOMATIC:
			printf("NetworkMode: TAPI_NETWORK_MODE_AUTOMATIC\n");
			break;

		case TAPI_NETWORK_MODE_GSM:
			printf("NetworkMode: TAPI_NETWORK_MODE_GSM\n");
			break;

		case TAPI_NETWORK_MODE_CDMA:
			printf("NetworkMode: TAPI_NETWORK_MODE_CDMA\n");
			break;

		case TAPI_NETWORK_MODE_EMERGENCY:
			printf("NetworkMode: TAPI_NETWORK_MODE_EMERGENCY\n");
			break;

		case TAPI_NETWORK_MODE_WCDMA:
			printf("NetworkMode: TAPI_NETWORK_MODE_WCDMA\n");
			break;

		case TAPI_NETWORK_MODE_WLAN:
			printf("NetworkMode: TAPI_NETWORK_MODE_WLAN\n");
			break;

		default:
			printf("NetworkMode: UnKnown\n");
			break;
	}

	/*service level */
	switch (tNetworkInfo.ServiceLevel) {
		case TAPI_NETWORK_SERVICE_LEVEL_NO:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_NO\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_FULL:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_FULL\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_SEARCH:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_SEARCH\n");
			break;

		default:
			printf("ServiceLevel: Unknown value\n");
			break;
	}
}

void SearchNetworkCB(void *data)
{
	TelNetworkList_t tmpNetworkList;
	int idx = 0;

	memcpy(&tmpNetworkList, data, sizeof(TelNetworkList_t));
	memcpy(&gNetworkList, data, sizeof(TelNetworkList_t));

	printf("[PHONE CLIENT] ReceiveNetworkList() - entryCount:[%d]\n", tmpNetworkList.NumOfNwAvailable);

	for (idx = 0; idx < tmpNetworkList.NumOfNwAvailable; idx++) {
		printf("[PHONE CLIENT] NetworkList.systemList[idx].SysId =>%ld\n", tmpNetworkList.SystemList[idx].SysId);
	}

	if (tmpNetworkList.SystemList[0].ServiceType == TAPI_NETWORK_SERVICE_TYPE_COMBINED) {
		printf("TAPI_NETWORK_SERVICE_TYPE_COMBINED\n");
	}
	else if (tmpNetworkList.SystemList[0].ServiceType == TAPI_NETWORK_SERVICE_TYPE_CS) {
		printf("TAPI_NETWORK_SERVICE_TYPE_CS\n");
	}
	else if (tmpNetworkList.SystemList[0].ServiceType == TAPI_NETWORK_SERVICE_TYPE_PS) {
		printf("TAPI_NETWORK_SERVICE_TYPE_PS\n");
	}
}

void SelectNetworkCB(void *data)
{
	TelNetworkInfo_t tNetworkInfo;
	memcpy(&tNetworkInfo, data, sizeof(TelNetworkInfo_t));

	printf("SelectNetworkCB() : PLMN is %ld\n", tNetworkInfo.SysId.SysId);

	if (tNetworkInfo.SysId.bPlmnForbidden == TRUE) {
		printf("SelectNetworkCB(): Network Forbidden\n");
	}

	if (tNetworkInfo.SysId.bHome == TRUE) {
		printf("SelectNetworkCB(): Home Network \n");
	}

	if (tNetworkInfo.bRoaming == TRUE) {
		printf("SelectNetworkCB(): Roaming Network \n");
	}


	/*SystemType*/
	switch (tNetworkInfo.SysId.SystemType) {
		case TAPI_NETWORK_SYSTEM_GSM:
			printf("SystemType: TAPI_NETWORK_SYSTEM_GSM\n");
			break;

		case TAPI_NETWORK_SYSTEM_UMTS:
			printf("SystemType: TAPI_NETWORK_SYSTEM_UMTS\n");
			break;

		case TAPI_NETWORK_SYSTEM_GPRS:
			printf("SystemType: TAPI_NETWORK_SYSTEM_GPRS\n");
			break;

		default:
			printf("SystemType: Other than GSM,GPRS,UMTS System type \n");
			break;
	}


	/*ServiceType*/
	switch (tNetworkInfo.SysId.ServiceType) {
		case TAPI_NETWORK_SERVICE_TYPE_COMBINED:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_COMBINED\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_CS:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_CS\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_PS:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_PS\n");
			break;

		default:
			printf("ServiceType: TAPI_NETWORK_SERVICE_TYPE_NO_SVE\n");
			break;
	}


	/*NetworkMode*/
	switch (tNetworkInfo.SysId.NetworkMode) {
		case TAPI_NETWORK_MODE_AUTOMATIC:
			printf("NetworkMode: TAPI_NETWORK_MODE_AUTOMATIC\n");
			break;

		case TAPI_NETWORK_MODE_GSM:
			printf("NetworkMode: TAPI_NETWORK_MODE_GSM\n");
			break;

		case TAPI_NETWORK_MODE_CDMA:
			printf("NetworkMode: TAPI_NETWORK_MODE_CDMA\n");
			break;

		case TAPI_NETWORK_MODE_EMERGENCY:
			printf("NetworkMode: TAPI_NETWORK_MODE_EMERGENCY\n");
			break;

		case TAPI_NETWORK_MODE_WCDMA:
			printf("NetworkMode: TAPI_NETWORK_MODE_WCDMA\n");
			break;

		case TAPI_NETWORK_MODE_WLAN:
			printf("NetworkMode: TAPI_NETWORK_MODE_WLAN\n");
			break;

		default:
			printf("NetworkMode: UnKnown\n");
			break;
	}

	/*service level */
	switch (tNetworkInfo.ServiceLevel) {
		case TAPI_NETWORK_SERVICE_LEVEL_NO:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_NO\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_FULL:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_FULL\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_SEARCH:
			printf("ServiceLevel: TAPI_NETWORK_SERVICE_LEVEL_SEARCH\n");
			break;

		default:
			printf("ServiceLevel: Unknown value\n");
			break;
	}
}


void GetSelectionModeCB(void *data)
{
	TelNetworkSelectionMode_t tSelectionMode;
	memcpy(&tSelectionMode, data, sizeof(TelNetworkSelectionMode_t));

	if (tSelectionMode == TAPI_NETWORK_SELECTIONMODE_AUTOMATIC) {
		printf("GetSelectionModeCB(): TAPI_NETWORK_SELECTIONMODE_AUTOMATIC\n");
	}
	else if (tSelectionMode == TAPI_NETWORK_SELECTIONMODE_MANUAL) {
		printf("GetSelectionModeCB(): TAPI_NETWORK_SELECTIONMODE_MANUAL\n");
	}
	else {
		printf("GetSelectionModeCB()???");
	}
}

int tapi_callback(const TelTapiEvent_t *tapievent, void *data)
{
	int tmp;
	int i = 0;
	TelNetworkRadioInfo_t radioInfo;

	tmp = tapievent->RequestId;

	//tapievent->RequestId =  tapievent->Status;
	//tapievent->Status = tapievent->EventType;
	//tapievent->EventType = tmp;
	tet_printf("\n Inside tapi_callback \n");
	tet_printf("Request iD is %d\n", tapievent->RequestId);
	tet_printf("Status  is %d\n", tapievent->Status);
	tet_printf("Event type  is %d\n", tapievent->EventType);

	if (tapievent->EventClass != TAPI_EVENT_CLASS_NETWORK) {
		return FALSE;
	}

	switch (tapievent->EventType) {
		case TAPI_EVENT_NETWORK_RADIO_INFO_IND:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_RADIO_INFO_IND $$$$$$ \n");

			memcpy(&radioInfo, tapievent->pData, sizeof(TelNetworkRadioInfo_t));

			printf("\n Radio InfoRssiBar:[%d] \n", radioInfo.RssiBar);
			printf("\n Radio InfoRxLevel:[%d] \n", radioInfo.RxLevel);
			printf("\n Radio InfoRxQuality:[%d] \n", radioInfo.RxQuality);

			break;

		case TAPI_EVENT_NETWORK_CHANGE_IND:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_CHANGE_IND $$$$$$ \n");

			if (tapievent->pData != NULL)
				SelectNetworkCB(tapievent->pData);
			else
				printf("Event Data is NULL\n");

			break;

		case TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF $$$$$$ \n");

			if (tapievent->pData != NULL)
				GetSelectionModeCB(tapievent->pData);
			else
				printf("Event Data is NULL\n");

			break;

		case TAPI_EVENT_NETWORK_SETNWMODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SETNWMODE_CNF $$$$$$ \n");
			break;

		case TAPI_EVENT_NETWORK_SETNWBAND_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SETNWBAND_CNF $$$$$$ \n");
			break;

		case TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF $$$$$$ \n");
			break;

		case TAPI_EVENT_NETWORK_SEARCH_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SEARCH_CNF $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkList_t networlist;
				memcpy(&networlist, tapievent->pData, sizeof(TelNetworkList_t));
				//memcpy(&gNetworkList, 0, sizeof(TelNetworkList_t));
				memcpy(&gNetworkList, tapievent->pData, sizeof(TelNetworkList_t));
				printf("\n Network Count:[%d] \n", networlist.NumOfNwAvailable);
				for (i = 0; i < networlist.NumOfNwAvailable; i++) {
					printf("Network Name:[%ld] \n", networlist.SystemList[i].SysId);
					printf("Network Name:[%c] \n", networlist.SystemList[i].szSystemName[0]);
					printf("System Type:[%d] \n", networlist.SystemList[i].SystemType);
					printf("Service Type:[%d] \n", networlist.SystemList[i].ServiceType);
					printf("Home PLMN:[%d] \n", networlist.SystemList[i].bHome);
					printf("Forbidden PLMN:[%d] \n", networlist.SystemList[i].bPlmnForbidden);
				}
			}
			else {
				printf("Event Data is NULL\n");
			}

			break;

		case TAPI_EVENT_NETWORK_SELECT_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SELECT_CNF $$$$$$ \n");

			if (tapievent->pData != NULL)
				GetNetworkInfoCB(tapievent->pData);
			else
				printf("Event Data is NULL\n");

			break;

		case TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETPREFFPLMN_CNF $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkPrefferedPlmnList_t preff_plmn_list;
				memcpy(&preff_plmn_list, tapievent->pData, sizeof(TelNetworkPrefferedPlmnList_t));
				unsigned char plmn_count;
				unsigned char counter;
				plmn_count = preff_plmn_list.NumOfPreffPlmns;
				counter = 0;
				while (counter < plmn_count) {
					printf(" INDEX:[%d]\n", preff_plmn_list.PreffPlmnRecord[counter].Index);
					printf(" PLMN ID:[%s]\n", preff_plmn_list.PreffPlmnRecord[counter].Plmn);
					printf(" SYSTEM TYPE:[%d]\n", preff_plmn_list.PreffPlmnRecord[counter].SystemType);
					printf("\n");
					counter++;
				}
			}
			else {
				printf("Event Data is NULL\n");
			}

			break;

		case TAPI_EVENT_NETWORK_GETNWBAND_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETNWBAND_CNF $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkBand_t band_type;
				memcpy(&band_type, tapievent->pData, sizeof(TelNetworkBand_t));
				printf("\n BAND TYPE:[%d] \n", band_type);
				if (band_type == TAPI_NETWORK_BAND_TYPE_GSM)
					printf("\n$$$$$$ TAPI_NETWORK_BAND_TYPE_GSM $$$$$$ \n");

			}
			else {
				printf("Event Data is NULL\n");
			}

			break;

		case TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkServiceDomain_t service_type;
				memcpy(&service_type, tapievent->pData, sizeof(TelNetworkServiceDomain_t));
				printf("\n SERVICE DOMAIN:[%d] \n", service_type);

				if (service_type == TAPI_NETWORK_SERVICE_DOMAIN_PS)
					printf("\n$$$$$$TAPI_NETWORK_SERVICE_DOMAIN_PS$$$$$$ \n");
				else if (service_type == TAPI_NETWORK_SERVICE_DOMAIN_CS)
					printf("\n$$$$$$TAPI_NETWORK_SERVICE_DOMAIN_CS$$$$$$ \n");
				else if (service_type == TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC)
					printf("\n$$$$$$ TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC$$$$$$ \n");
				else if (service_type == TAPI_NETWORK_SERVICE_DOMAIN_COMBINED)
					printf("\n$$$$$$ TAPI_NETWORK_SERVICE_DOMAIN_COMBINED \n");
			}
			else {
				printf("Event Data is NULL\n");
			}

			break;

		case TAPI_EVENT_NETWORK_GETNWMODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETNWMODE_CNF $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkMode_t network_mode;
				memcpy(&network_mode, tapievent->pData, sizeof(TelNetworkMode_t));
				printf("\n NETWORK MODE:[%d] \n", network_mode);
				if (network_mode == TAPI_NETWORK_MODE_AUTOMATIC)
					printf("\n$$$$$$ TAPI_NETWORK_MODE_AUTOMATIC $$$$$$ \n");
				else if (network_mode == TAPI_NETWORK_MODE_GSM)
					printf("\n$$$$$$ TAPI_NETWORK_MODE_GSM $$$$$$ \n");
				else if (network_mode == TAPI_NETWORK_MODE_WCDMA)
					printf("\n$$$$$$ TAPI_NETWORK_MODE_WCDMA $$$$$$ \n");
			}
			else {
				printf("Event Data is NULL\n");
			}

			break;

		case TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_LOCANDCELLINFO $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkLocationCellInfo_t network_loc_cell_info;
				memcpy(&network_loc_cell_info, tapievent->pData, sizeof(TelNetworkLocationCellInfo_t));
				printf("\n LAC:[%d] \n", network_loc_cell_info.LocationAreaCode);
				printf("\n CID:[%ld] \n", network_loc_cell_info.CellId);
			}
			else {
				printf("Event Data is NULL\n");
			}

			break;

		case TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SETPREFPLMN_CNF $$$$$$ \n");
			break;

		case TAPI_EVENT_NETWORK_SPN_INFO:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SPN_INFO $$$$$$ \n");

			if (tapievent->pData != NULL) {
				TelNetworkSpnMccMncInfo_t short_network_name;
				int ret_val = 0;
				char short_net_name[20];

				memcpy(&short_network_name, tapievent->pData, sizeof(TelNetworkSpnMccMncInfo_t));
				fprintf(stderr, "SERVICE PROVIDER NAME ####	[%s]	 ####\n", short_network_name.short_network_name);
				fprintf(stderr, "MCC ####	[%d] ####	MNC #### [%d]\n", short_network_name.mcc, short_network_name.mnc);

				printf("ret val from status to get network name=%d", ret_val);
				printf("NETWORK NAME FORM STATUS = %s \n", short_net_name);
			}
			break;

		default:
			printf("\n$$$$$$ UNKNOWN Event has Received $$$$$$ \n");
			break;
	}

	return TRUE;
}

gboolean TimeOutFunc(void *data)
{
	g_main_loop_quit(mainloop);
	printf("out of loop\n");
	return FALSE;
}

int gmain_loop_for_tapi_events()
{
	mainloop = g_main_loop_new(NULL, FALSE);
	g_timeout_add(3000, TimeOutFunc, NULL);
	g_main_loop_run(mainloop);
	printf("receive_tapi_event:finish\n");
	return 0;
}
