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
#include <unistd.h>
#include <string.h>

#include "ITapiNetwork.h"
#include "TapiCommon.h"
#include "ITapiPS.h"
#include "TelUtility.h"
#include "TelSim.h"
#include "TelErr.h"
#include "vconf.h"
#include "TapiEvent.h"

#if 0 //ED based
typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}edata_t;
#endif //ED based

int error_code;
int iNumOfNwEvt;
static unsigned int * NW_subscription_id;
//extern int EvtDeliveryHandle ;

#define SZ_IMEI "user/telephony/szMobileEquipmentSerialNumber"

#define TEST_DEBUG(frmt, args...) \
{do { fprintf(stderr, "[NW Test][%s:%04d] "frmt "\n", __func__,__LINE__, ##args); } while (FALSE); }


void __PrintNWBand(char* str, TelNetworkBand_t tNWBandType)
{
	switch(tNWBandType)
	{
		case TAPI_NETWORK_BAND_TYPE_ANY:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_ANY [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_GSM850:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM850 [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_GSM_900_1800:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM_900_1800 [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_GSM1900:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM1900 [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_GSM:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_WCDMA:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_WCDMA [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_WCDMA850:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_WCDMA850 [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_WCDMA1900:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_WCDMA1900 [0x%x]", str, tNWBandType);
			}
			break;

		case TAPI_NETWORK_BAND_TYPE_WCDMA2100:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_WCDMA2100 [0x%x]", str, tNWBandType);
			}
			break;
		case TAPI_NETWORK_BAND_TYPE_GSM900:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM900 [0x%x]", str, tNWBandType);
			}
			break;
		case TAPI_NETWORK_BAND_TYPE_GSM1800:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM1800 [0x%x]", str, tNWBandType);
			}
			break;
		case TAPI_NETWORK_BAND_TYPE_GSM_850_1900:
			{
				TEST_DEBUG("%s TAPI_NETWORK_BAND_TYPE_GSM_850_1900 [0x%x]", str, tNWBandType);
			}
			break;
		default:
			{
				TEST_DEBUG("%s Undefined Storage Type returnted [0x%x]", str, tNWBandType);
			}
			break;
	}
	return;
}


void __PrintGetRoamingModeCnfCB(void *pData)
{
	TelNetworkRoamingMode_t* pRoamingMode = NULL;

	pRoamingMode = (TelNetworkRoamingMode_t *)pData;

	switch(pRoamingMode->pref_net_type)
	{
		case TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC:
			TEST_DEBUG("TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC\n");
			break;
		case TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A:
			TEST_DEBUG("TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A\n");
			break;
		case TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_B:
			TEST_DEBUG("TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_B\n");
			break;
		case TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY:
			TEST_DEBUG("TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY\n");
			break;
		case TAPI_NETWORK_PREF_NET_TYPE_ROAM_ONLY:
			TEST_DEBUG("TAPI_NETWORK_PREF_NET_TYPE_ROAM_ONLY\n");
			break;
		default:
			break;
	}

	switch(pRoamingMode->prl_pref_only)
	{
		case TAPI_NETWORK_PRL_PREF_ONLY_OFF:
			TEST_DEBUG("TAPI_NETWORK_PRL_PREF_ONLY_OFF\n");
			break;
		case TAPI_NETWORK_PRL_PREF_ONLY_ON:
			TEST_DEBUG("TAPI_NETWORK_PRL_PREF_ONLY_ON\n");
			break;
		default:
			break;
	}

	return;
}

void __PrintGetHybridModeCnfCB(void *pData)
{
	TelNetworkCDMAHybridMode_t HybridMode = 0x00;

	memcpy(&HybridMode, pData, sizeof(TelNetworkCDMAHybridMode_t));

	switch(HybridMode){
		case TAPI_NETWORK_CDMA_HYBRID:
			TEST_DEBUG("TAPI_NETWORK_CDMA_HYBRID \n");
			break;
		case TAPI_NETWORK_CDMA_1X_ONLY:
			TEST_DEBUG("TAPI_NETWORK_CDMA_1X_ONLY\n");
			break;
		case TAPI_NETWORK_CDMA_EVDO_ONLY:
			TEST_DEBUG("TAPI_NETWORK_CDMA_EVDO_ONLY\n");
			break;
		default:
			TEST_DEBUG("Unknown Mode\n");
			break;
	}
	return;
}

void __PrintGetNetworkInfoCB(void* pData)
{
	TelNetworkInfo_t tNetworkInfo ={0,};
	memcpy(&tNetworkInfo, pData, sizeof(TelNetworkInfo_t));

	TEST_DEBUG("__PrintGetNetworkInfoCB() : PLMN is %lu\n",tNetworkInfo.SysId.SysId);

	if(tNetworkInfo.SysId.bPlmnForbidden== TRUE)
	{
		TEST_DEBUG("__PrintGetNetworkInfoCB(): Network Forbidden\n");
	}

	TEST_DEBUG("Network Service Type : ");
	switch(tNetworkInfo.SysId.SystemType)
	{

		case TAPI_NETWORK_SYSTEM_GSM: // GSM
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_GSM\n");
			break;

		case TAPI_NETWORK_SYSTEM_GPRS: // GPRS
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_GPRS\n");
			break;

		case TAPI_NETWORK_SYSTEM_UMTS: // UTRAN
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_UMTS\n");
			break;

		case TAPI_NETWORK_SYSTEM_EGPRS: // EGPRS
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_EGPRS\n");
			break;

		default:
			TEST_DEBUG(" DEFAULT IN SYSTEM TYPE\n");
			break;

	}

	TEST_DEBUG("Network Service Level : ");
	switch(tNetworkInfo.ServiceLevel)
	{
		case TAPI_NETWORK_SERVICE_LEVEL_NO:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_NO\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_FULL:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_FULL\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_SEARCH:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_SEARCH\n");
			break;

		default:
			TEST_DEBUG(" Not Defined\n");
			break;

	}
	return;
}


void __PrintSearchNetworkCB(void* pData)
{
	TelNetworkList_t tmpNetworkList ={0,};
	int idx=0;

	memcpy(&tmpNetworkList, pData, sizeof(TelNetworkList_t));
	TEST_DEBUG("[PHONE CLIENT] ReceiveNetworkList() - entryCount:[%d]\n",tmpNetworkList.NumOfNwAvailable);

	for(idx=0;idx<tmpNetworkList.NumOfNwAvailable;idx++)
	{
		TEST_DEBUG("[PHONE CLIENT] NetworkList.systemList[idx].sysid =>%lu\n", tmpNetworkList.SystemList[idx].SysId);
		//			TEST_DEBUG("[PHONE CLIENT] NetworkList.systemList[idx].systemType =>%x\n", NetworkList.systemList[idx].systemType  );
	}

	if(tmpNetworkList.SystemList[0].ServiceType== TAPI_NETWORK_SERVICE_TYPE_COMBINED)
	{
		TEST_DEBUG("TAPI_NETWORK_SERVICE_TYPE_COMBINED\n");

	}
	else if(tmpNetworkList.SystemList[0].ServiceType== TAPI_NETWORK_SERVICE_TYPE_CS)
	{
		TEST_DEBUG("TAPI_NETWORK_SERVICE_TYPE_CS\n");

	}
	else if(tmpNetworkList.SystemList[0].ServiceType== TAPI_NETWORK_SERVICE_TYPE_PS)
	{
		TEST_DEBUG("TAPI_NETWORK_SERVICE_TYPE_PS\n");
	}

}

void __PrintSelectNetworkCB(void* pData)
{
	TelNetworkInfo_t tNetworkInfo = {0,};
	memcpy(&tNetworkInfo, pData, sizeof(TelNetworkInfo_t));

	switch(tNetworkInfo.SysId.SystemType)
	{
		case TAPI_NETWORK_SYSTEM_GSM: // GSM
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_GSM\n");
			break;

		case TAPI_NETWORK_SYSTEM_GPRS: // GPRS
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_GPRS\n");
			break;

		case TAPI_NETWORK_SYSTEM_UMTS: // UTRAN
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_UMTS\n");
			break;

		case TAPI_NETWORK_SYSTEM_EGPRS: // EGPRS
			TEST_DEBUG(" TAPI_NETWORK_SYSTEM_EGPRS\n");
			break;

		default:
			TEST_DEBUG(" DEFAULT IN SYSTEM TYPE\n");
			break;

	}

	switch(tNetworkInfo.SysId.SystemType)
	{

		case TAPI_NETWORK_SERVICE_TYPE_NO_SVE:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_TYPE_NO_SVE\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_LIMITED:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_TYPE_LIMITED\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_CS:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_TYPE_CS\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_PS:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_TYPE_PS\n");
			break;

		case TAPI_NETWORK_SERVICE_TYPE_COMBINED:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_TYPE_COMBINED\n");
			break;

		default:
			TEST_DEBUG(" DEFAULT IN SERVICE TYPE\n");
			break;

	}

	switch(tNetworkInfo.ServiceLevel)
	{

		case TAPI_NETWORK_SERVICE_LEVEL_NO:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_NO\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_EMERGENCY\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_FULL:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_FULL\n");
			break;

		case TAPI_NETWORK_SERVICE_LEVEL_SEARCH:
			TEST_DEBUG(" TAPI_NETWORK_SERVICE_LEVEL_SEARCH\n");
			break;

		default:
			TEST_DEBUG(" DEFAULT IN SERVICE LEVEL\n");
			break;

	}


}

void __PrintGetSelectionModeCB(void* pData)
{
	TelNetworkSelectionMode_t tSelectionMode = 0x00;
	memcpy(&tSelectionMode, pData, sizeof(TelNetworkSelectionMode_t));

	if(tSelectionMode == TAPI_NETWORK_SELECTIONMODE_AUTOMATIC)
	{
		TEST_DEBUG("__PrintGetSelectionModeCB(): TAPI_MODE_AUTOMATIC\n");
	}
	else if(tSelectionMode == TAPI_NETWORK_SELECTIONMODE_MANUAL)
	{
		TEST_DEBUG("__PrintGetSelectionModeCB(): TAPI_MODE_MANUAL\n");
	}
	else TEST_DEBUG("__PrintGetSelectionModeCB()???");

	return;
}


void nw_async_event_callback(TelTapiEvent_t* pNwEvent, void *data)
{

	TEST_DEBUG("Func Entrance");
	TEST_DEBUG("Request ID : [0x%x]\n", pNwEvent->RequestId);

	if(pNwEvent->pData==NULL)
	{
		TEST_DEBUG("No Event Data!!");
//		return;
	}

	TEST_DEBUG("*************** Callback data **********************");
	TEST_DEBUG("EventClass [%d],EventType [0x%x], Reques Id[%d], Event status [%d], psEvent->pDataLen[%d]",
		pNwEvent->EventClass, pNwEvent->EventType, pNwEvent->RequestId, pNwEvent->Status,pNwEvent->pDataLen);

	switch(pNwEvent->EventClass)
	{
		case TAPI_EVENT_CLASS_NETWORK:
			{
				switch(pNwEvent->EventType)
				{
					case TAPI_EVENT_NETWORK_RADIO_INFO_IND :
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_RADIO_INFO_IND $$$$$$ \n");
							//system("cat bark-2.wav > /dev/dsp");
						}
						break;

					case TAPI_EVENT_NETWORK_SELECT_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SELECT_CNF $$$$$$");
						}
						break;

					case TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF $$$$$$");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR)
								__PrintGetSelectionModeCB(pNwEvent->pData);
						}
						break;


					 case TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF $$$$$$");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR){
								TelNetworkPrefferedPlmnList_t	preff_plmn_list;
								memcpy(&preff_plmn_list, pNwEvent->pData, sizeof(TelNetworkPrefferedPlmnList_t));
								unsigned int plmn_count;
								unsigned int counter ;
								plmn_count = preff_plmn_list.NumOfPreffPlmns;
								counter = 0;
								TEST_DEBUG(" PREFF PLMN COUNT :[%d]\n",preff_plmn_list.NumOfPreffPlmns);
								while(counter <plmn_count)
								{
									TEST_DEBUG(" INDEX:[%d]\t\t",preff_plmn_list.PreffPlmnRecord[counter].Index);
									TEST_DEBUG(" PLMN ID:[%s]\t\t",preff_plmn_list.PreffPlmnRecord[counter].Plmn);
									TEST_DEBUG(" SYSTEM TYPE:[%d]\n",preff_plmn_list.PreffPlmnRecord[counter].SystemType);
									TEST_DEBUG("");
									counter++;
								}
							}

						}
						break;

					 case TAPI_EVENT_NETWORK_GETPOWERONATTACH_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETPOWERONATTACH_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR){
								TelNetworkPowerOnAttach_t	poweron_attach;
								memcpy(&poweron_attach, pNwEvent->pData, sizeof(TelNetworkPowerOnAttach_t));
								if(poweron_attach == TAPI_NETWORK_POWERON_ATTACH_DISABLE)
								{
									TEST_DEBUG(" Power On Attach diable");
								}
								else if(poweron_attach == TAPI_NETWORK_POWERON_ATTACH_ENABLE)
								{
									TEST_DEBUG(" Power On Attach enable");
								}
								else
								{
									TEST_DEBUG(" Power On Attach status Error");
								}
							}
						}
						break;

					 case TAPI_EVENT_NETWORK_SETPOWERONATTACH_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SETPOWERONATTACH_CNF $$$$$$ ");
						}
						break;

					 case TAPI_EVENT_NETWORK_GETNWBAND_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETNWBAND_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR){
								TelNetworkBand_t band_type;
								memcpy(&band_type, pNwEvent->pData, sizeof(TelNetworkBand_t));
								__PrintNWBand(" BAND TYPE: ", band_type);
							}
						}
						break;

					case TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR){
								TelNetworkServiceDomain_t service_type;
								memcpy(&service_type, pNwEvent->pData, sizeof(TelNetworkServiceDomain_t));
								TEST_DEBUG(" SERVICE DOMAIN:[%d] 0:combined,1:ps,2:cs,3:auto ", service_type);
							}
						}
						break;

					 case TAPI_EVENT_NETWORK_GETNWORDER_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETNWORDER_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR){
								TelNetworkMode_t	network_order[TAPI_NETWORK_ORDER_MAX];
								memcpy(&network_order, pNwEvent->pData, (sizeof(TelNetworkMode_t)* TAPI_NETWORK_ORDER_MAX));
								TEST_DEBUG(" NETWORK ORDER:[%d] 0:none,1:em,2:auto,3:cdma,4:gsm,5:wcdma,6:wlan ", network_order[0]);
							}
						}
						break;

					 case TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF $$$$$$ ");
						}
						break;

					 case TAPI_EVENT_NETWORK_GETNWMODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GETNWMODE_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR){
								TelNetworkMode_t	network_mode;
								memcpy(&network_mode, pNwEvent->pData, sizeof(TelNetworkMode_t));
								TEST_DEBUG(" NETWORK MODE:[%d] 0:none,1:em,2:auto,3:cdma,4:gsm,5:wcdma,6:wlan", network_mode);
							}
						}
						break;

					 case TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND $$$$$$ ");
							TelNetworkLocationCellInfo_t  network_loc_cell_info;
							memcpy(&network_loc_cell_info, pNwEvent->pData, sizeof(TelNetworkLocationCellInfo_t));
							TEST_DEBUG(" LAC:[%d] ", network_loc_cell_info.LocationAreaCode);
							TEST_DEBUG(" CID:[%ld]", network_loc_cell_info.CellId);
						}
						break;

					 case TAPI_EVENT_NETWORK_DISPLAYINFO_IND:
						{
							TelNetworkDisplayInfo_t   dispInfo;
							memcpy(&dispInfo, pNwEvent->pData, sizeof(TelNetworkDisplayInfo_t));

							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_DISPLAYINFO_IND $$$$$$ ");

							if(dispInfo.DisplayInfo[TAPI_NETWORK_CS_DISP_INDEX] != -1)
							{
								TEST_DEBUG("CSTYPE = %d ",dispInfo.DisplayInfo[TAPI_NETWORK_CS_DISP_INDEX]);
							}
							if(dispInfo.DisplayInfo[TAPI_NETWORK_PS_DISP_INDEX] != -1)
							{
								TEST_DEBUG("PSTYPE = %d ",dispInfo.DisplayInfo[TAPI_NETWORK_PS_DISP_INDEX]);
							}
							if(dispInfo.DisplayInfo[TAPI_NETWORK_SVC_DISP_INDEX] != -1)
							{
								TEST_DEBUG("SVCTYPE = %d ",dispInfo.DisplayInfo[TAPI_NETWORK_SVC_DISP_INDEX] );
							}
							if(dispInfo.DisplayInfo[TAPI_NETWORK_ROAM_DISP_INDEX] != -1)
							{
								TEST_DEBUG("ROAM TYPE = %d ", dispInfo.DisplayInfo[TAPI_NETWORK_ROAM_DISP_INDEX]);
							}

						}
						break;

					case TAPI_EVENT_NETWORK_SEARCH_CNF:
						{

							TelNetworkPlmnList_t* ui_network_info;
							int ui_entry_count=0;
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SEARCH_CNF $$$$$$ ");

							ui_network_info = (TelNetworkPlmnList_t *)calloc(1, sizeof(TelNetworkPlmnList_t));

							memcpy(ui_network_info, pNwEvent->pData, sizeof(TelNetworkPlmnList_t));

							while(ui_entry_count<ui_network_info->networks_count)
							{

								TEST_DEBUG( "Type_of_plmn[%d] 0:unkwon,1:home,2:available,3:forbidden,  NetworkName[%s]",ui_network_info->network_list[ui_entry_count].type_of_plmn,ui_network_info->network_list[ui_entry_count].network_name);
								TEST_DEBUG( "ServiceProviderName[%s]",ui_network_info->network_list[ui_entry_count].service_provider_name);
								TEST_DEBUG( "PLMN ID[%u]",ui_network_info->network_list[ui_entry_count].plmn_id);
								ui_entry_count++;
							}
						}
						break;

					case TAPI_EVENT_NETWORK_SPN_INFO:
						{
							TelNetworkSpnMccMncInfo_t	short_network_name;
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SPN_INFO $$$$$$ ");
							memcpy(&short_network_name, pNwEvent->pData, sizeof(TelNetworkSpnMccMncInfo_t));
							TEST_DEBUG(" Display Condition ####	[%d]	 ####",short_network_name.bDispCondition);
							TEST_DEBUG(" RPLMN  NAME ####	[%s]	 ####",short_network_name.short_network_name);
							TEST_DEBUG("SERVICE PROVIDER NAME ####	 [%s]	 ####",short_network_name.spn_name);
							TEST_DEBUG("MCC ####	[%d] ####	MNC #### [%d]",short_network_name.mcc , short_network_name.mnc);
						}
						break;

					case TAPI_EVENT_NETWORK_SETNWORDER_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SETNWORDER_CNF $$$$$$ ");
						}
						break;

					case TAPI_EVENT_NETWORK_SETNWMODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SETNWMODE_CNF $$$$$$ ");
						}
						break;

					case TAPI_EVENT_NETWORK_SETNWBAND_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SETNWBAND_CNF $$$$$$ ");
						}
						break;

					case TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF $$$$$$ ");
						}
						break;

					case TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF $$$$$$ ");
						}
						break;

					case TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR)
								__PrintGetRoamingModeCnfCB(pNwEvent->pData);
						}
						break;

					case TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF $$$$$$ ");
						}
						break;

					case TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF:
						{
							TEST_DEBUG("$$$$$$ TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF $$$$$$ ");
							if(pNwEvent->Status == TAPI_NETWORK_NO_ERR)
								__PrintGetHybridModeCnfCB(pNwEvent->pData);
						}
						break;

					default:
						TEST_DEBUG("Unhandled pNwEvent->EventType[0x%x]",pNwEvent->EventType);
						break;
				}
			}
			break;

		default:
			TEST_DEBUG("Unhandled pNwEvent->EventClass[0x%x]",pNwEvent->EventClass);
			break;
	}

	return;
}


int nw_read_key_input(int option)
{
	int ret;
	char buf[512] = {0, };
	char msg[512];
	TapiResult_t ret_val = TAPI_API_SUCCESS;

	int reqid;

	memset(buf, 0x00, sizeof(buf));
	memset(msg, 0x00, sizeof(msg));

	fprintf(stderr,"*************** NETWORK TEST MENU     ***************\n");
	fprintf(stderr,"c.Cancel to Search Network\n");
	fprintf(stderr,"1.Search Network\n");
	fprintf(stderr,"2.Select Network [mode(automatic/manual) systemid]\n");
	fprintf(stderr,"3.Get Selection Mode\n");
	fprintf(stderr,"4.Set NETWORK MODE --> [a] AUTO \t [b] GSM \t[c] WCDMA\n");
	fprintf(stderr,"5.Get NETWORK MODE\n");
	fprintf(stderr,"6 Set NETWORK Preferred Order -->[a] GSM [b] WCDMA [c] AUTO \n");
	fprintf(stderr,"7 Get NETWORK Preferred Order\n");
	fprintf(stderr,"8.Set SERVICE DOMAIN -->[a] PS [b] CS [c] AUTO [d] COMBINED\n");
	fprintf(stderr,"9.Get SERVICE DOMAIN\n");
	fprintf(stderr,"10.Get Network Information\n");
	fprintf(stderr,"11.Set Preferred PLMN List \n");
	fprintf(stderr,"12.Get Preferred PLMN List \n");
	fprintf(stderr,"13.Set POWER ON ATTACH --> [a] DISABLE [b] ENABLE\n");
	fprintf(stderr,"14.Get POWER ON ATTACH\n");
	fprintf(stderr,"15.Set NETWORK BAND \n");
	fprintf(stderr,"16.Get NETWORK BAND \n");
	fprintf(stderr,"17.Get Phone Number (from gconf)-not used now(091130) \n");
	fprintf(stderr,"*****************************************************\n");
	fprintf(stderr,"1A.Set CDMA Roaming Mode [a]Auto [b]Auto_A [c]Auto_b [d]Home [e]Roam\n");
	fprintf(stderr,"1B.Get CDMA Roaming Mode \n");
	fprintf(stderr,"1C.Set CDMA Hybrid Mode [a]Hybrid [b]1x [c]EVDO\n");
	fprintf(stderr,"1D.Get CDMA Hybrid Mode \n");
	fprintf(stderr,"*************** VCONF / STATUS TEST MENU ********************\n");
	fprintf(stderr,"STATUS GET CLASS  : class0(Net), class1(Sim), class2(CPHS), class3(SimDevCtx)\n");
	fprintf(stderr,"VCONF /STATUS GET MEMBER : mem \n");
	fprintf(stderr,"*****************************************************\n");
	fprintf(stderr,"q Quit\n");
	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0) return ret;

	if(strncmp(buf, "menu", 4) == 0)
	{
		printf("Finished Testing this module, Going back to Main Menu\n");
		return -1;
	}

	if( ret == 2 && memcmp(buf,"c",sizeof("c")-1) == 0 )
	{
		ret_val = tel_cancel_network_manual_search(&reqid);
		TEST_DEBUG("TelTapiNwCancelManualSearch API RETURN VALUE : [%d]" , ret_val);
	}

	if( ret == 2 && memcmp(buf,"1",sizeof("1")-1) == 0 )
	{
		ret_val = tel_search_network(&reqid);
		TEST_DEBUG("tel_search_network API RETURN VALUE : [%d]" , ret_val);
	}

	if( memcmp(buf,"2",sizeof("2")-1) == 0 )
	{
		unsigned int plmn = 45001;
		TelNetworkSelectionMode_t mode;
		char tmpPLMN[6]={0, };

		if(memcmp(buf+2, "automatic", sizeof("automatic")-1) == 0){
			TEST_DEBUG("Automatic mode \n");
			ret_val = tel_select_network_automatic(&reqid);
		}
		else if(memcmp(buf+2, "manual", sizeof("manual")-1) == 0){
			TEST_DEBUG("Manual mode \t");
			mode = TAPI_NETWORK_SELECTIONMODE_MANUAL;
			memcpy(tmpPLMN,  buf+3 + sizeof("manual")-1, 6);
			plmn=(TS_UINT32)atoi(tmpPLMN);
			TEST_DEBUG("PLMN number is [%u]",plmn);
			ret_val = tel_select_network_manual(plmn, &reqid);
		}
		else{
			TEST_DEBUG("Unknown mode \n");
			return 0;
		}

		TEST_DEBUG("tel_select_network_manual API RETURN VALUE : [%d]\n" , ret_val);
	}

	else if(memcmp(buf,"3",sizeof("3")-1) == 0)
	{
		ret_val = tel_get_network_selection_mode(&reqid);
		TEST_DEBUG("tel_get_network_selection_mode API RETURN VALUE : [%d]\n" , ret_val);
	}

	if( memcmp(buf,"4",sizeof("4")-1) == 0 )
	{

		TelNetworkMode_t networkMode;
		if(memcmp(buf+1,"a",sizeof("a")-1) == 0 )
			networkMode =TAPI_NETWORK_MODE_AUTOMATIC;
		else if(memcmp(buf+1,"b",sizeof("b")-1) == 0 )
			networkMode =TAPI_NETWORK_MODE_GSM;
		else if(memcmp(buf+1,"c",sizeof("c")-1) == 0 )
			networkMode =TAPI_NETWORK_MODE_WCDMA;
		else{
			TEST_DEBUG("Unknown Network mode\n");
			return 0;
		}

		ret_val = tel_set_network_mode(networkMode, &reqid);
		TEST_DEBUG("tel_set_network_mode API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"5",sizeof("5")-1) == 0 )
	{
		ret_val = tel_get_network_mode(&reqid);
		TEST_DEBUG("tel_get_network_mode API RETURN VALUE : [%d]\n" , ret_val);
	}


	if( memcmp(buf,"6",sizeof("6")-1) == 0 )
	{
		TelNetworkMode_t networkOrder[5];

		if(memcmp(buf+1,"a",sizeof("a")-1) == 0 )
			networkOrder[0] =TAPI_NETWORK_MODE_GSM;
		else if(memcmp(buf+1,"b",sizeof("b")-1) == 0 )
			networkOrder[0] =TAPI_NETWORK_MODE_WCDMA;
		else if(memcmp(buf+1,"c",sizeof("c")-1) == 0 )
			networkOrder[0] =TAPI_NETWORK_MODE_AUTOMATIC;

		ret_val = tel_set_network_acquisition_order(networkOrder, &reqid);
		TEST_DEBUG("tel_set_network_acquisition_order API RETURN VALUE : [%d]\n" , ret_val);
	}

	if( memcmp(buf,"7",sizeof("7")-1) == 0 )
	{
		ret_val = tel_get_network_acquisition_order(&reqid);
		TEST_DEBUG("tel_get_network_acquisition_order API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"8",sizeof("8")-1) == 0)
	{
		TelNetworkServiceDomain_t serviceDomain;

		if(memcmp(buf+1,"a",sizeof("a")-1) == 0 )
			serviceDomain =TAPI_NETWORK_SERVICE_DOMAIN_PS ;
		else if(memcmp(buf+1,"b",sizeof("b")-1) == 0 )
			serviceDomain =TAPI_NETWORK_SERVICE_DOMAIN_CS ;
		else if(memcmp(buf+1,"c",sizeof("c")-1) == 0 )
			serviceDomain =TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC ;
		else if(memcmp(buf+1,"d",sizeof("d")-1) == 0 )
			serviceDomain =TAPI_NETWORK_SERVICE_DOMAIN_COMBINED ;

		ret_val = tel_set_network_service_domain(serviceDomain, &reqid);
		TEST_DEBUG("tel_set_network_service_domain API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"9",sizeof("9")-1) == 0)
	{
		ret_val = tel_get_network_service_domain(&reqid);
		TEST_DEBUG("tel_get_network_service_domain API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"11",sizeof("11")-1) == 0)
	{
		TelNetworkPrefferedPlmnInfo_t  PreffPlmnInfo;
		int index = 0;
		char dummy;
		TEST_DEBUG("Plese input index : ");
		scanf("%d%c",&index,&dummy);
		PreffPlmnInfo.Index = index;
		strcpy((char *)PreffPlmnInfo.Plmn, "45454");
		TEST_DEBUG("Add 45454# to Preferred PLMN");
		PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
		ret_val = tel_set_network_preferred_plmn(TAPI_NETWORK_PREF_PLMN_ADD, &PreffPlmnInfo, &reqid);
		TEST_DEBUG("tel_set_network_preferred_plmn API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"12",sizeof("12")-1) == 0)
	{
		ret_val = tel_get_network_preferred_plmn(&reqid);
		TEST_DEBUG("tel_get_network_preferred_plmn API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"13",sizeof("13")-1) == 0)
	{
		if(memcmp(buf+2,"a",sizeof("a")-1) == 0)
		{
			ret_val = tel_set_network_power_on_attach(TAPI_NETWORK_POWERON_ATTACH_DISABLE, &reqid );
			TEST_DEBUG("tel_set_network_power_on_attach API[Disable] RETURN VALUE : [%d]\n" , ret_val);
		}
		else if(memcmp(buf+2,"b",sizeof("b")-1) == 0)
		{
			ret_val = tel_set_network_power_on_attach(TAPI_NETWORK_POWERON_ATTACH_ENABLE, &reqid);
			TEST_DEBUG("tel_set_network_power_on_attach API[Enable] RETURN VALUE : [%d]\n" , ret_val);
		}
	}

	if(memcmp(buf,"14",sizeof("14")-1) == 0)
	{
		ret_val = tel_get_network_power_on_attach(&reqid);
		TEST_DEBUG("tel_get_network_power_on_attach API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"15",sizeof("15")-1) == 0)
	{
		int iBandMode = 1;
		int iBand = 1;
		fprintf(stderr, "1=Preferred, 2=Only\n");
		fprintf(stderr, "Input Band Mode: ");
		scanf("%d", &iBandMode);
		getchar();

		fprintf(stderr, "0=All,1=GSM850 2=GSM900_1800 3=GSM1900 4=GSM_ALL 5=WCDMA 6=WCDMA850 7=WCDMA1900 8=WCDMA2100 9=gsm900 10=gsm1800 11=gsm850_1900\n");
		fprintf(stderr, "Input Band: ");
		scanf("%d", &iBand);
		getchar();

		ret_val = tel_set_network_band(iBandMode, iBand, &reqid);
		//TelTapiNwSetNetworkBand(TAPI_NETWORK_BAND_MODE_ONLY , TAPI_NETWORK_BAND_TYPE_ANY,&reqid);
		TEST_DEBUG("tel_set_network_band API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"16",sizeof("16")-1) == 0)
	{
		ret_val = tel_get_network_band(&reqid);
		TEST_DEBUG("tel_get_network_band API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(memcmp(buf,"17",sizeof("17")-1) == 0)
	{
#if 0 //not use in slp2.0
		GConfClient*    gconf_client;
		char*       strval;

		gconf_client = gconf_client_get_default();

		// get Subscriber No from gconf
		strval= gconf_client_get_string(gconf_client, "/Services/Telephony/szSubscriberNumber", NULL);

		TEST_DEBUG("Subscriber Number [ %s ] from gconf", strval);

		g_free(strval); //g_free checks for the NULL, so no need to check seperately
		g_object_unref(gconf_client);
#endif //not use in slp2.0
	}

	if( memcmp(buf,"1A",sizeof("1A")-1) == 0 )
	{
		TelNetworkRoamingMode_t RoamingMode = {0, };
		if(memcmp(buf+3,"a",sizeof("a")-1) == 0 )
		{
			RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC;
		}
		else if(memcmp(buf+3,"b",sizeof("b")-1) == 0 )
		{
			RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_A;
		}
		else if(memcmp(buf+32,"c",sizeof("c")-1) == 0 )
		{
			RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_AUTOMATIC_B;
		}
		else if(memcmp(buf+3,"d",sizeof("d")-1) == 0 )
		{
			RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_HOME_ONLY;
		}
		else if(memcmp(buf+3,"e",sizeof("e")-1) == 0 )
		{
			RoamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_TYPE_ROAM_ONLY;
		}
		else
			TEST_DEBUG("unknown roam preferred network type \n");
		RoamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_OFF;

		TEST_DEBUG("tel_set_network_roaming ..... mode:[%d]\n" , ret_val);
		ret_val = tel_set_network_roaming(&RoamingMode, &reqid);
		TEST_DEBUG("tel_set_network_roaming API RETURN VALUE : [%d]\n" , ret_val);
	}

	if( memcmp(buf,"1B",sizeof("1B")-1) == 0 )
	{
		ret_val = tel_get_network_roaming(&reqid);
		TEST_DEBUG("API RETURN VALUE : [%d]\n" , ret_val);
	}


	if( memcmp(buf,"1C",sizeof("1C")-1) == 0 )
	{
		TelNetworkCDMAHybridMode_t CDMAHybridMode = 0;
		if(memcmp(buf+3,"a",sizeof("a")-1) == 0 )
		{
			CDMAHybridMode = TAPI_NETWORK_CDMA_HYBRID;
		}
		else if(memcmp(buf+3,"b",sizeof("b")-1) == 0 )
		{
			CDMAHybridMode = TAPI_NETWORK_CDMA_1X_ONLY;
		}
		else if(memcmp(buf+3,"c",sizeof("c")-1) == 0 )
		{
			CDMAHybridMode = TAPI_NETWORK_CDMA_EVDO_ONLY;
		}
		else
		{
			TEST_DEBUG("unknown roam preferred network type \n");
		}
		TEST_DEBUG("tel_set_network_hybrid_in_cdma .... mode:[%d]\n" , CDMAHybridMode);
		ret_val = tel_set_network_hybrid_in_cdma(CDMAHybridMode, &reqid);
		TEST_DEBUG("tel_set_network_hybrid_in_cdma API RETURN VALUE : [%d]\n" , ret_val);
	}

	if( memcmp(buf,"1D",sizeof("1D")-1) == 0 )
	{
		ret_val = tel_get_network_hybrid_in_cdma(&reqid);
		TEST_DEBUG("tel_get_network_hybrid_in_cdma API RETURN VALUE : [%d]\n" , ret_val);
	}

	if(strncmp(buf, "mem", 3) == 0)
	{
		int retValue=0;
		int apiRet =0;
		char* spnName = NULL;
		char* nwName = NULL;
		char* imei = NULL;
		char* szImei = NULL;
		char* szSubNum = NULL;
		char* szSubName = NULL;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_RSSI, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_RSSI api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_PLMN, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_PLMN api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_LAC, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_LAC api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_CELLID, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_CELLID api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_TAPI_STATE, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_TAPI_STATE api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SVC_ROAM, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SVC_ROAM api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SVC_CS, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SVC_CS api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SVC_PS, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SVC_PS api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_ZONE_TYPE, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_ZONE_TYPE api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SVCTYPE, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SVCTYPE api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_LOW_BATTERY, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_LOW_BATTERY api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SIM_INIT, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SIM_INIT api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SIM_CHV, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SIM_CHV api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SIM_SLOT, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SIM_SLOT api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SIM_PB_INIT, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SIM_PB_INIT api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_CALL_STATE, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_CALL_STATE api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_ZONE_ZUHAUSE, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_ZONE_ZUHAUSE api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SAT_IDLE_SCREEN_EVENT, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SAT_IDLE_SCREEN_EVENT api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		apiRet = vconf_get_int(VCONFKEY_TELEPHONY_SPN_DISP_CONDITION, &retValue);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SPN_DISP_CONDITION api ret[%d], return value[%d] ",ret_val,retValue);
		apiRet = 0;
		retValue = 0;

		spnName = vconf_get_str(VCONFKEY_TELEPHONY_SPN_NAME);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SPN_NAME:[%s] ",spnName);

		nwName = vconf_get_str(VCONFKEY_TELEPHONY_NWNAME);
		TEST_DEBUG("VCONFKEY_TELEPHONY_NWNAME:[%s] ",nwName);

		imei = vconf_get_str(VCONFKEY_TELEPHONY_IMEI);
		TEST_DEBUG("VCONFKEY_TELEPHONY_IMEI:[%s] ",imei);

		szImei = vconf_get_str(SZ_IMEI);
		TEST_DEBUG("SZ_IMEI:[%s] ",szImei);

		szSubNum= vconf_get_str(VCONFKEY_TELEPHONY_SUBSCRIBER_NUMBER);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SUBSCRIBER_NUMBER:[%s] ",szSubNum);

		szSubName= vconf_get_str(VCONFKEY_TELEPHONY_SUBSCRIBER_NAME);
		TEST_DEBUG("VCONFKEY_TELEPHONY_SUBSCRIBER_NAME:[%s] ",szSubName);
	}

	if( memcmp(buf,"q",sizeof("q")-1) == 0 )
	{
		return -1;
	}

	return 0;
}



void nw_select_loop(int option)
{
	int ret;
	fd_set readfds;

	while (1) {
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);


		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret) {

			if (FD_ISSET(0, &readfds)) {
				if(nw_read_key_input(option)<0)
					break;
			}
		}
	}
}

void nw_test_thread_loop()
{
	TEST_DEBUG("start nw_test_thread_loop\n");
	nw_select_loop(7);
	fprintf(stderr," network exited\n");
	pthread_exit(NULL);
}

int nw_test_subscribe_tapi_events()
{
	int i = 0;
	int ret_val = TRUE;
	TapiResult_t	api_err = TAPI_API_SUCCESS;

	int NwEvtList[] =
	{
		TAPI_EVENT_NETWORK_GETNWBAND_CNF,
		TAPI_EVENT_NETWORK_SETNWBAND_CNF,
		TAPI_EVENT_NETWORK_GETSERVICEDOMAIN_CNF,
		TAPI_EVENT_NETWORK_SETSERVICEDOMAIN_CNF,
		TAPI_EVENT_NETWORK_CANCELMANUALSELECTION_CNF,
		TAPI_EVENT_NETWORK_CANCELMANUALSEARCH_CNF,
		TAPI_EVENT_NETWORK_GETNWORDER_CNF,
		TAPI_EVENT_NETWORK_SETNWORDER_CNF,
		TAPI_EVENT_NETWORK_GETNWMODE_CNF,
		TAPI_EVENT_NETWORK_SETNWMODE_CNF,
		TAPI_EVENT_NETWORK_SELECT_CNF,
		TAPI_EVENT_NETWORK_SEARCH_CNF,
		TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF,
		TAPI_EVENT_NETWORK_GETPREFFEREDPLMN_CNF,
		TAPI_EVENT_NETWORK_SETPREFFEREDPLMN_CNF,
		TAPI_EVENT_NETWORK_GETPOWERONATTACH_CNF,
		TAPI_EVENT_NETWORK_SETPOWERONATTACH_CNF,
		TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND,
		TAPI_EVENT_NETWORK_CHANGE_IND,
		TAPI_EVENT_NETWORK_RADIO_INFO_IND,
		TAPI_EVENT_NETWORK_SPN_INFO,
		TAPI_EVENT_NETWORK_DISPLAYINFO_IND,
		TAPI_EVENT_NETWORK_SEARCH_POPUP_ENABLE,
		TAPI_EVENT_NETWORK_SEARCH_POPUP_DISABLE,
		TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF,
		TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF,
		TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF,
		TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF,
	};

	iNumOfNwEvt = sizeof(NwEvtList)/sizeof(int);
	TEST_DEBUG("Number of NW Event List to be registered: %d", iNumOfNwEvt);
	NW_subscription_id = (unsigned int *)calloc(iNumOfNwEvt, sizeof(unsigned int));

	if(tel_init() == TAPI_API_SUCCESS)
	{
		for( i=0 ; i< iNumOfNwEvt; i++ )
		{
			api_err = tel_register_event(NwEvtList[i], &NW_subscription_id[i],(TelAppCallback)&nw_async_event_callback, NULL);

			if(api_err != TAPI_API_SUCCESS)
			{
				TEST_DEBUG("isn't subscribed. sub id is [%d], api_err is [%d]\n", NW_subscription_id[i],api_err);
				ret_val = FALSE;
				break;
			}
		}
	}
	else
	{
		TEST_DEBUG("tel_init() failed");
	}
	return ret_val;

}


int nw_test_unsubscribe_tapi_events(void)
{
	int i = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;
	int ret_val = TRUE;

	for(i=0;i<iNumOfNwEvt; i++){
		api_err = tel_deregister_event(NW_subscription_id[i]);
		if(api_err != TAPI_API_SUCCESS){
			TEST_DEBUG("tel_deregister_event isn't unsubscribed. sub id is [%d] api_err is [%d]\n",NW_subscription_id[i],api_err);
			ret_val = FALSE;
			break;
		}

	}
	tel_deinit();
	return ret_val;
}

void nw_main(int argc, char *argv[])
{
	int ret=0;
	pthread_t network_thread_id;

	TEST_DEBUG("===== Network_test  ver 1.01 =====\n");

	ret = pthread_create(&network_thread_id, NULL, (void *)&nw_test_thread_loop, NULL);

	if(ret != 0)
	{
		TEST_DEBUG( "Thread for TAPI Events Failed\n");
	}

	//nw_test_subscribe_tapi_events();

	pthread_join(network_thread_id, NULL);
}


