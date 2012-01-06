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
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <glib.h>

#include "TelUtility.h"
#include "TapiEvent.h"
#include "TapiCommon.h"
#include "ITapiData.h"



#ifndef false
#define false 0
#endif
#ifndef true
#define true  !0
#endif
#ifndef bool
#define bool  char
#endif

typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
}data_t;

#define TEST_DEBUG(frmt, args...)	\
{do { fprintf(stderr, "[CDMA-DATA Test][%s:%04d] "frmt "\n", __func__, __LINE__, ##args); } while (FALSE) ;}


#define TAPI_EVENT_CLASS_DATA_MAX_NUM 2
#define	NUM_OF_CDMA_DATA_EVENT			39

// Global Variable
int error_code;
unsigned int cdma_data_subscription_id[NUM_OF_CDMA_DATA_EVENT] = {0, };

void PrintNaiIndexType(TelDataNAIIndex_t NaiIndex)
{
	if(NaiIndex == TAPI_DATA_FACTORY_NAI)
		printf(" NAI type : FACTORY \n");
	else if(NaiIndex == TAPI_DATA_CUSTOM_NAI)
		printf(" NAI type : Custom \n");
	else
		printf(" NAI type : Unknown [%d]\n", NaiIndex);

	return;
}

void PrintPinControlInfo(TelDataPinControlInfo_t *pDsPin)
{
	printf(" Signal Type : ");
	switch(pDsPin->signaltype)
	{
		case TAPI_DATA_SIGNAL_DCD:
			printf("DCD \n");
			break;

		case TAPI_DATA_SIGNAL_DTR:
			printf("DTR \n");
			break;

		case TAPI_DATA_SIGNAL_DSR:
			printf("DSR \n");
			break;

		case TAPI_DATA_SIGNAL_RTS:
			printf("RTS \n");
			break;

		case TAPI_DATA_SIGNAL_CTS:
			printf("CTS \n");
			break;

		case TAPI_DATA_SIGNAL_RI:
			printf("RI \n");
			break;
		default:
			printf("Unknown \n");
			break;
	}

	printf(" Signal Status : ");
	switch(pDsPin->signalstatus)
	{
		case TAPI_DATA_SIGNAL_STATUS_OFF:
			printf("OFF \n");
			break;

		case TAPI_DATA_SIGNAL_STATUS_ON:
			printf("ON \n");
			break;

		default:
			printf("Unknown \n");
			break;
	}

	return;
}

void ComposeMobileIPNaiParametr(TelDataMipNaiParam_t *MipNaiParam)
{
	MipNaiParam->naiIndex = TAPI_DATA_FACTORY_NAI;
	MipNaiParam->paramMask =	TAPI_DATA_NAI_MIP_MASK_REVERSE_TUNNEL + TAPI_DATA_NAI_MIP_MASK_HOME_ADDRESS + \
					TAPI_DATA_NAI_MIP_MASK_PRIMARY_HOME_AGENT_IP;

	MipNaiParam->genericInfo.revTunPreference = 0x3;
	MipNaiParam->genericInfo.homeAddress = 0xA5D5E3E9;
	MipNaiParam->genericInfo.primaryHaAddress = 0x5A5D3E9E;

	return;
}

void EvtCurrentNaiIndexGetCnfCB(void *pData)
{
	TelDataNAIIndex_t *pNaiIndex;
	pNaiIndex = (TelDataNAIIndex_t *)pData;

	PrintNaiIndexType(*pNaiIndex);

	return;
}

void EvtMipNaiGetCnfCB(void *pData)
{
	TelDataMipNaiParam_t *pMipNaiParam;
	int i = 0;
	pMipNaiParam = (TelDataMipNaiParam_t *)pData;

	PrintNaiIndexType(pMipNaiParam->naiIndex);

	printf(" Mask Value = %x\n", pMipNaiParam->paramMask);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_USER_NAI){
		printf(" : User NAI Value");
		for(i=0; i< pMipNaiParam->genericInfo.usrNaiLength; i++)
			printf("[0x%x] ", pMipNaiParam->genericInfo.usrNai[i]);
		printf("\n");
	}

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_SPI_MN_HA)
		printf(" Mobile Node Home Agent Secure Parameter Index = [0x%x]\n", pMipNaiParam->genericInfo.mnHaSpi);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_SPI_MN_AAA)
		printf(" Mobile Node AAA Secure Parameter Index = [0x%x]\n", pMipNaiParam->genericInfo.mnAaaSpi);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_REVERSE_TUNNEL)
		printf(" Reverse Tunneling Preference = [0x%x]\n", pMipNaiParam->genericInfo.revTunPreference);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_HOME_ADDRESS)
		printf(" Home Address = [0x%x]\n", pMipNaiParam->genericInfo.homeAddress);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_PRIMARY_HOME_AGENT_IP)
		printf(" Primary Home Agent IP Address = [0x%x]\n", pMipNaiParam->genericInfo.primaryHaAddress);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_SECONDARY_HOME_AGENT_IP)
		printf(" Secondary Home Agent IP Address = [0x%x]\n", pMipNaiParam->genericInfo.secondaryHaAddr);

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_MN_AAA_SS){
		printf(" Mobile Node to AAA Shared Secret\n");
		for(i=0; i< pMipNaiParam->ssInfo.mnHaSharedSecretLength; i++)
			printf("[0x%x] ", pMipNaiParam->ssInfo.mnHaSharedSecret[i]);
		printf("\n");
	}

	if(pMipNaiParam->paramMask & TAPI_DATA_NAI_MIP_MASK_MN_HA_SS){
		printf(" Mobile Node to Home Agent Shared Secret\n");
		for(i=0; i< pMipNaiParam->ssInfo.mnAaaSharedSecretLength; i++)
			printf("[0x%x] ", pMipNaiParam->ssInfo.mnAaaSharedSecret[i]);
		printf("\n");
	}

	return;
}

void EvtMipNaiChangedNotiCB(void *pData)
{
	TelDataNaiChangedItem_t *pChangedItem;
	pChangedItem = (TelDataNaiChangedItem_t *)pData;

	if(*pChangedItem == TAPI_DATA_NAI_GENERIC_INFO_CHANGED)
		printf("[NAI Information]\n");
	else if(*pChangedItem == TAPI_DATA_SHARED_SECRET_CHANGED)
		printf("[Shared Screte]\n");

	return;
}

void EvtTe2StatusDisplay(void *pData)
{
	TelDataTE2Status_t *pTe2Status;
	pTe2Status= (TelDataTE2Status_t *)pData;

	printf("Type[");
	if(pTe2Status->type == TAPI_DATA_TE2_TYPE_UART)
		printf("UART]\t");
	else if(pTe2Status->type == TAPI_DATA_TE2_TYPE_USB)
		printf("USB]\t");
	else if(pTe2Status->type == TAPI_DATA_TE2_TYPE_BLUETOOTH)
		printf("Bluetooth]\t");
	else
		printf("Unknown]\t");

	printf("Status[");

	if(pTe2Status->cable_status == TAPI_DATA_TE2_DETACHED)
		printf("Detached]\n");
	else if(pTe2Status->cable_status == TAPI_DATA_TE2_ATTACHED)
		printf("Atached]\n");
	else
		printf("Unknown]\n");

	return;
}

void EvtBytesCountDisplay(void *pData)
{
	TelDataPacketByteCounter_t *pByteCnt = NULL;
	pByteCnt= (TelDataPacketByteCounter_t *)pData;

	printf("Received Current Packet Bytes[%u]", pByteCnt->rx_curr_packet_bytes);
	printf("Transmitted Current Packet Bytes[%u]", pByteCnt->tx_curr_packet_bytes);
	printf("Received Total Packet Bytes[%llu]", pByteCnt->rx_total_packet_bytes);
	printf("Transmitted Total Packet Bytes[%llu]", pByteCnt->tx_total_packet_bytes);

	return;
}

void EvtIncomingCallTypeDisplay(void *pData)
{
	TelDataCallType_t *pCallType = NULL;
	pCallType= (TelDataCallType_t *)pData;

	printf("Incoming Call Type :[");
	switch(*pCallType){
		case TAPI_DATA_CALL_TYPE_DEFAULT:
			printf("Default]");
			break;
		case TAPI_DATA_CALL_TYPE_FAX_FOR_NEXT_CALL:
			printf("FAX For Next Call]");
			break;
		case TAPI_DATA_CALL_TYPE_FAX_FOR_ALL_CALLS:
			printf("FAX For All Calls]");
			break;
		case TAPI_DATA_CALL_TYPE_ASYNC_FOR_NEXT_CALL:
			printf("Async For Next Call]");
			break;
		case TAPI_DATA_CALL_TYPE_ASYNC_FOR_ALL_CALLS:
			printf("Async For All Call]");
			break;
		default:
			printf("Unknown Incoming Call Type\n");
			break;
	}
	return;
}

void EvtDataRateInfoDisplay(void*pData)
{
	TelDataTE2DataRateInfo_t *pDataRate = NULL;
	pDataRate= (TelDataTE2DataRateInfo_t *)pData;

	printf("Transmitted Bytes[%u]", pDataRate->tx_bytes);
	printf("Received Bytes[%u]", pDataRate->rx_bytes);
	printf("Report Period [%d]", pDataRate->report_period);
	printf("Channel Number [%d]", pDataRate->channel_num);
	printf("Tx Number of Bar Level during the reporting period[%d]", pDataRate->tx_bar_level);
	printf("Rx Number of Bar Level during the reporting period[%d]", pDataRate->rx_bar_level);

	return;

}

void EvtBaudRateDisplay(void*pData)
{
	TelDataBaudRate_t *pBaudRate = NULL;
	pBaudRate= (TelDataBaudRate_t *)pData;

	printf("Baud Rate :[");
	switch(*pBaudRate){
		case TAPI_DATA_BAUDRATE_19200_BPS:
			printf("19200]");
			break;
		case TAPI_DATA_BAUDRATE_38400_BPS:
			printf("38400]");
			break;
		case TAPI_DATA_BAUDRATE_57600_BPS:
			printf("57600]");
			break;
		case TAPI_DATA_BAUDRATE_115200_BPS:
			printf("115200]");
			break;
		case TAPI_DATA_BAUDRATE_230400_BPS:
			printf("230400]");
			break;
		default:
			printf("Unknown Baud Rate\n");
			break;
	}
	return;
}

void EvtRoamGuardDisplay(void *pData)
{
	TelDataRoamGuardMode_t *pMode = NULL;
	pMode= (TelDataRoamGuardMode_t *)pData;

	printf("Roam Guard Mode:[");
	switch(*pMode){
		case TAPI_DATA_ROAM_GUARD_DEFAULT:
			printf("Default]");
			break;
		case TAPI_DATA_ROAM_GUARD_ALWAYS_ASK:
			printf("Always ASK]");
			break;
		case TAPI_DATA_ROAM_ROAM_GUARD_NEVER_ASK:
			printf("Never ASK]");
			break;
		case TAPI_DATA_ROAM_ROAM_GUARD_OFF:
			printf("OFF]");
			break;
		case TAPI_DATA_ROAM_ROAM_GUARD_ON:
			printf("ON]");
			break;
		default:
			printf("Unknown Roam Guard\n");
			break;
	}
	return;

}

void EvtModemNaiDisplay(void *pData)
{
	TelDataModemNAIMode_t *pMode = NULL;
	pMode= (TelDataModemNAIMode_t *)pData;

	printf("Modem NAI Mode:[");
	switch(*pMode){
		case TAPI_DATA_MODEM_NAI_OFF:
			printf("OFF]");
			break;
		case TAPI_DATA_MODEM_NAI_ON:
			printf("ON]");
			break;
		default:
			printf("Unknown Modem NAI Mode\n");
			break;
	}
	return;

}

void EvtModemBtModeDisplay(void *pData)
{
	TelDataDSType_t *pType = NULL;
	pType= (TelDataDSType_t *)pData;

	printf("Data Service Type :");
	switch(*pType){
		case TAPI_DATA_DS_TYPE_NORMAL:
			printf("DS TYPE : NORMAL");
			break;
		case TAPI_DATA_DS_TYPE_DUN:
			printf("DS TYPE : DUN");
			break;
		case TAPI_DATA_DS_TYPE_BTDUN:
			printf("DS TYPE : BTDUN");
			break;
		case TAPI_DATA_DS_TYPE_IS:
			printf("DS TYPE : IS");
			break;
		case TAPI_DATA_DS_TYPE_MMS:
			printf("DS TYPE : MMS");
			break;
		default:
			printf("Unknown DS Type\n");
			break;
	}
	return;
}

void EvtForceRevAModeDisplay(void *pData)
{
	TelDataRevConfig_t *pMode = NULL;
	pMode= (TelDataRevConfig_t *)pData;

	printf("Forced RevA Mode :[");
	switch(*pMode){
		case TAPI_DATA_REVA:
			printf("REV-A]");
			break;
		case TAPI_DATA_REV0:
			printf("REV-0]");
			break;
		default:
			printf("Unknown Forced Rev Mode\n");
			break;
	}
	return;
}

void EvtPacketDataCallConfigDisplay(void *pData)
{
	TelDataPacketDataCallConfig_t *pPacketData = NULL;
	TelDataPacketConfigType_t type;
	TelDataPacketParam_t param;
	pPacketData= (TelDataPacketDataCallConfig_t *)pData;

	type = pPacketData->type;
	memcpy(&param, &pPacketData->param, sizeof(TelDataPacketParam_t));

	printf("Packet Data Call Type :");
	switch(type){
		case TAPI_DATA_PACKET_DATA_TYPE_QNC:
			printf("QNC");
			break;
		case TAPI_DATA_PACKET_DATA_TYPE_MIP:
			printf("MIP");
			break;
		case TAPI_DATA_PACKET_DATA_TYPE_CALL_TYPE:
			printf("Call Type");
			break;
		case TAPI_DATA_PACKET_DATA_TYPE_PKT_ORIGIN_STRING:
			printf("ORG STRING");
			break;
		case TAPI_DATA_PACKET_DATA_TYPE_PAP_USER_ID:
			printf("USER ID");
			break;
		case TAPI_DATA_PACKET_DATA_TYPE_PAP_USER_PASSWORD:
			printf("USER PASSWORD");
			break;
		default:
			printf("\nUnknown Type\n");
			break;
	}

	return;
}

void  cdma_data_async_event_callback(TelTapiEvent_t* event)
{
	printf("\n data_async_event_callback enter\n");

	printf("Request ID : [0x%x]\n", event->RequestId);

	printf("Status : [");
	switch(event->Status)
	{
		case TAPI_DATA_NO_ERR:
			printf("No Err]\n");
			break;

		case TAPI_DATA_PARAMETER_ERR:
			printf("Parameter Err]\n");
			break;

		case TAPI_DATA_OFFLINE_ERR:
			printf("Phone Off]\n");
			break;

		case TAPI_DATA_OTHER_ERR:
		default:
			printf("Unknown Err]\n");
			break;
	}

	printf("event->EventType : %d\n", event->EventType);
	printf("event->EventType : %d\n", TAPI_EVENT_DATA_PACKET_DATA_CALL_CONFIGURATION_GET_CNF);

	switch(event->EventType)
	{
		case	TAPI_EVENT_DATA_MIP_NAI_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_MIP_NAI_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtMipNaiGetCnfCB(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_CURRENT_NAI_INDEX_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_CURRENT_NAI_INDEX_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtCurrentNaiIndexGetCnfCB(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_TE2_STATUS_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_TE2_STATUS_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtTe2StatusDisplay(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_TE2_STATUS_NOTI:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_TE2_STATUS_NOTI]\n");
				EvtTe2StatusDisplay(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_BYTES_COUNTER_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_BYTES_COUNTER_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtBytesCountDisplay(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_DS_TE2_DATA_RATE_INFO_NOTI:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_DS_TE2_DATA_RATE_INFO_NOTI]\n");
				EvtDataRateInfoDisplay(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_ROAM_GUARD_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_ROAM_GUARD_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtRoamGuardDisplay(event->pData);
			}
			break;
		case	TAPI_EVENT_DATA_MODEM_NAI_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_MODEM_NAI_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtModemNaiDisplay(event->pData);
			}
			break;

		case	TAPI_EVENT_DATA_EVDO_REV_CONFIG_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_EVDO_REV_CONFIG_GET_CNF]\n");
				if(event->Status ==TAPI_DATA_NO_ERR)
					EvtForceRevAModeDisplay(event->pData);
			}
			break;
		case	TAPI_EVENT_DATA_DDTM_CONFIGURATION_GET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_DDTM_CONFIGURATION_GET_CNF]\n");
			}
			break;
		case	TAPI_EVENT_DATA_MIP_NAI_SET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_MIP_NAI_SET_CNF]\n");
			}
			break;
		case	TAPI_EVENT_DATA_BYTES_COUNTER_RESET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_BYTES_COUNTER_RESET_CNF]\n");
			}
			break;

		case	TAPI_EVENT_DATA_HDR_SESSION_CLEAR_SET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_HDR_SESSION_CLEAR_SET_CNF]\n");
			}
			break;
		case	TAPI_EVENT_DATA_ROAM_GUARD_SET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_ROAM_GUARD_SET_CNF]\n");
			}
			break;

		case	TAPI_EVENT_DATA_EVDO_REV_CONFIG_SET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_FORCE_REVA_MODE_SET_CNF]\n");
			}
			break;
		case	TAPI_EVENT_DATA_DDTM_CONFIGURATION_SET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_DDTM_CONFIGURATION_SET_CNF]\n");
			}
			break;
		case	TAPI_EVENT_DATA_RESTORE_NAI_SET_CNF:
			{
				printf("Event Type:  [TAPI_EVENT_DATA_RESTORE_NAI_SET_CNF]\n");
			}
			break;
		case TAPI_EVENT_DATA_PACKET_DATA_CALL_CONFIGURATION_GET_CNF:
			{
				printf("Event Type: [TAPI_EVENT_DATA_PACKET_DATA_CALL_CONFIGURATION_GET_CNF]");
				EvtPacketDataCallConfigDisplay(event->pData);
			}
			break;

		default:
			printf("Event Type: unknown event:[%d]\n", event->EventType);

			break;
	}

}


int cdma_data_read_key_input(void)
{
	int ret;
	char buf[512] = {0, };
	char msg[512];
	TapiResult_t ret_val = TAPI_API_SUCCESS;
	int RequestID;

	memset(buf,0, sizeof(buf));
	memset(msg,0,sizeof(msg));

	printf("*************** CDMA DATA TEST MENU ******************\n");
	printf("0.  DSPin Control \n");
	printf("3.  Get Mobile IP NAI \n");
	printf("4.  Set Mobile IP NAI \n");
	printf("5.  Get Current NAI Index \n");
	printf("6.  Get TE2 Status \n");
	//printf("7.  Reset Data bytes counter \n");
	//printf("8.  Get Data bytes counter \n");
	//printf("9.  Get Incoming Call Type \n");
	//printf("10. Set Incoming Call Type \n");
	printf("11. Get Data Service Baud Rate\n");
	printf("12. Set Data Service Baud Rate\n");
	//printf("13. Set HDR Session Clear\n");
	printf("14. Get Roam Guard Mode\n");
	printf("15. Set Roam Guard Mode\n");
	printf("16. Get Modem NAI\n");
	printf("17. Set Modem NAI\n");
	printf("18. Get DS Type\n");
	printf("19. Set DS Type\n");
	printf("20. Get EVDO Rev Config\n");
	printf("21. Set EVDO Rev Config(Rev-A or Rev-0)\n");
	printf("22. Get Singed In State Mode\n");
	printf("23. Set Singed In State Mode\n");
	printf("24. Set the NAI value to factory setting\n");
	printf("25. Get Dedicated Data Transmission Mode\n");
	printf("26. Set Dedicated Data Transmission Mode\n");
	printf("27. Get DSWorking Mode\n");
	printf("28. Set DSWorking Mode\n");
	printf("\n  BONUS!! ^^;;\n");
	printf("29. Get Packet Data Call Configuration\n");
	printf("29. Set Packet Data Call Configuration\n");
	printf("q. quit\n");

	ret = read(0, buf, sizeof(buf));
	if (ret < 0)
	{
		if (errno == EINTR)
			perror("read(1)");
		return -1;
	}
	else if (ret == 0)
		return ret;


	// Get Mobile IP NAI
	if( memcmp(buf,"3",sizeof("3")-1) == 0 )
	{
		TelDataNAIIndex_t MipNaiIdx = TAPI_DATA_FACTORY_NAI;
		unsigned short ParamMask = TAPI_DATA_NAI_MIP_MASK_REVERSE_TUNNEL + TAPI_DATA_NAI_MIP_MASK_HOME_ADDRESS + \
					   TAPI_DATA_NAI_MIP_MASK_PRIMARY_HOME_AGENT_IP;//TAPI_DATA_NAI_MIP_MASK_USER_NAI + TAPI_DATA_NAI_MIP_MASK_SPI_MN_HA + TAPI_DATA_NAI_MIP_MASK_SPI_MN_AAA;
		ret_val = tel_get_data_mobile_ipnai (&MipNaiIdx, &ParamMask, &RequestID);
		printf(" \n tel_get_data_mobile_ipnai Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set Mobile IP NAI
	if( memcmp(buf,"4",sizeof("4")-1) == 0 )
	{
		TelDataMipNaiParam_t MipNaiParam;
		ComposeMobileIPNaiParametr(&MipNaiParam);
		ret_val = tel_set_data_mobile_ipnai (&MipNaiParam, &RequestID);
		printf(" \n tel_set_data_mobile_ipnai Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get Current NAI Index
	if( memcmp(buf,"5",sizeof("5")-1) == 0 )
	{
		ret_val = tel_get_data_current_nai_index(&RequestID);
		printf(" \n tel_get_data_current_nai_index Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get TE2 Status
	if( memcmp(buf,"6",sizeof("6")-1) == 0 )
	{
		ret_val = tel_get_data_te2_status(&RequestID);
		printf(" \n tel_get_data_te2_status Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get Roam Guard Mode
	if( memcmp(buf,"14",sizeof("14")-1) == 0 )
	{
		ret_val = tel_get_data_roam_guard(&RequestID);
		printf(" \n tel_get_data_roam_guard Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set Roam Guard Mode
	if( memcmp(buf,"15",sizeof("15")-1) == 0 )
	{
		TelDataRoamGuardMode_t RoamGuard = TAPI_DATA_ROAM_GUARD_DEFAULT;
		ret_val = tel_set_data_roam_guard(&RoamGuard, &RequestID);
		printf(" \n tel_set_data_roam_guard Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get Modem NAI
	if( memcmp(buf,"16",sizeof("16")-1) == 0 )
	{
		ret_val = tel_get_data_nai(&RequestID);
		printf(" \n tel_get_data_nai Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set Modem NAI
	if( memcmp(buf,"17",sizeof("17")-1) == 0 )
	{
		TelDataModemNAIMode_t ModemNai = TAPI_DATA_MODEM_NAI_OFF;
		ret_val = tel_set_data_nai(&ModemNai, &RequestID);
		printf(" \n tel_set_data_nai Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// 20. Get EVDO Rev Config
	if( memcmp(buf,"20",sizeof("20")-1) == 0 )
	{
		ret_val = tel_get_data_evdo_rev_config(&RequestID);
		printf(" \n tel_get_data_evdo_rev_config Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set EVDO Rev Config(Rev-A or Rev-0)
	if( memcmp(buf,"21",sizeof("21")-1) == 0 )
	{
		//TelDataRevConfig_t RevConfig = TAPI_DATA_REVA;
		//ret_val = TelTapiDataSetEVDORevConfig (&RevConfig, &RequestID);
		//printf(" \n TelTapiDataSetForceRevAMode Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get Singed In State Mode
	if( memcmp(buf,"22",sizeof("22")-1) == 0 )
	{
		//ret_val = tel_get_data_signed_in_sate (&RequestID);
		printf(" \n tel_get_data_signed_in_sate Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set Singed In State Mode
	if( memcmp(buf,"23",sizeof("23")-1) == 0 )
	{
		//TelDataSignStatus_t SignStatus = TAPI_DATA_PACKET_NAI_SIGNIN_OUT;
		//ret_val = tel_set_data_signed_in_sate (&SignStatus, &RequestID);
		//printf(" \n tel_set_data_signed_in_sate Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set the NAI value to factory setting
	if( memcmp(buf,"24",sizeof("24")-1) == 0 )
	{
		ret_val = tel_restore_data_nai(&RequestID);
		printf(" \n tel_restore_data_nai Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get Dedicated Data Transmission Mode
	if( memcmp(buf,"25",sizeof("25")-1) == 0 )
	{
		ret_val = tel_get_data_ddtm_config(&RequestID);
		printf(" \n tel_get_data_ddtm_cofig Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set Dedicated Data Transmission Mode
	if( memcmp(buf,"26",sizeof("26")-1) == 0 )
	{
		TelDataDDTMStatus_t DDTMStatus= TAPI_DATA_DDTM_MODE_OFF;
		ret_val = tel_set_data_ddtm_config (&DDTMStatus, &RequestID);
		printf(" \n tel_set_data_ddtm_cofig Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set DSWorking Mode
	if( memcmp(buf,"28",sizeof("28")-1) == 0 )
	{
		//TelDataDSWorkingMode_t DSWorkingMode = TAPI_DATA_DSWORKING_MODE_OFF;
		//ret_val = tel_set_data_ds_working_mode (&DSWorkingMode, &RequestID);
		//printf(" \n tel_set_data_ds_working_mode Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Get Packet Data Call Config
	if( memcmp(buf,"29",sizeof("29")-1) == 0 )
	{
		TelDataPacketConfigType_t Type;
		int num;

		printf("Select Config Type\n");
		printf("0. QNC\n");
		printf("1. Mobile IP\n");
		printf("2. Packet Data Call Type\n");
		printf("3. Packet Orig Str\n");
		printf("4. Pap User ID\n");
		printf("5. Pap Password\n");
		printf(">> ");
		scanf("%d", &num);

		Type = num;

		//ret_val = TelTapiDataGetPacketDataCallConfiguration(&Type, &RequestID);
		printf(" \n TelTapiDataGetPacketDataCallConfiguration Tapi return returnStatus value is [%x]  RequestID : [%d] \n ", ret_val, RequestID);
	}

	// Set Packet Data Call Config
	if( memcmp(buf,"30",sizeof("28")-1) == 0 )
	{
		printf("NOT YET\n");
	}
	// Exit
	if( memcmp(buf,"q",sizeof("q")-1) == 0)
	{
		return -1;
	}

	return 0;
}


void cdma_data_select_loop(void)
{
	int ret;
	fd_set readfds;

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);

		ret = select(0 + 1, &readfds, NULL, NULL, NULL);
		if (ret)
		{
			if (FD_ISSET(0, &readfds))
			{
				if(cdma_data_read_key_input() < 0)
					break;
			}
		}
	}
}

void cdma_data_test_unsubscribe_tapi_event(void)
{
	int i = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;

	for(i=0;i<NUM_OF_CDMA_DATA_EVENT; i++){
		api_err = tel_deregister_event(cdma_data_subscription_id[i]);
		if(api_err != TAPI_API_SUCCESS){
			TEST_DEBUG("tel_deregister_event isn't unsubscribed. sub id is %d api_err is %d\n",cdma_data_subscription_id[i],api_err);
			break;
		}

	}

	tel_deinit();
}

void cdma_data_test_subscribe_tapi_events (void)
{
	int i = 0;
	TapiResult_t	api_err = TAPI_API_SUCCESS;

	int CdmaDataEvt[NUM_OF_CDMA_DATA_EVENT] = {	TAPI_EVENT_DATA_MIP_NAI_GET_CNF,
		TAPI_EVENT_DATA_MIP_NAI_SET_CNF,
		TAPI_EVENT_DATA_CURRENT_NAI_INDEX_GET_CNF,
		TAPI_EVENT_DATA_TE2_STATUS_GET_CNF,
		TAPI_EVENT_DATA_TE2_STATUS_NOTI,
		TAPI_EVENT_DATA_BYTES_COUNTER_RESET_CNF,
		TAPI_EVENT_DATA_BYTES_COUNTER_GET_CNF,
		TAPI_EVENT_DATA_DS_TE2_DATA_RATE_INFO_NOTI,
		TAPI_EVENT_DATA_HDR_SESSION_CLEAR_SET_CNF,
		TAPI_EVENT_DATA_ROAM_GUARD_GET_CNF,
		TAPI_EVENT_DATA_ROAM_GUARD_SET_CNF,
		TAPI_EVENT_DATA_MODEM_NAI_GET_CNF,
		TAPI_EVENT_DATA_EVDO_REV_CONFIG_GET_CNF,
		TAPI_EVENT_DATA_EVDO_REV_CONFIG_SET_CNF,
		TAPI_EVENT_DATA_RESTORE_NAI_SET_CNF,
		TAPI_EVENT_DATA_DDTM_CONFIGURATION_GET_CNF,
		TAPI_EVENT_DATA_DDTM_CONFIGURATION_SET_CNF,
		TAPI_EVENT_DATA_PACKET_DATA_CALL_CONFIGURATION_GET_CNF,

	};

	//TEST_DEBUG("########### CDMA DATA TEST #############\n");
	if(tel_init() == TAPI_API_SUCCESS)
	{
		for(i=0;i<NUM_OF_CDMA_DATA_EVENT; i++){
			api_err = tel_register_event(CdmaDataEvt[i], &cdma_data_subscription_id[i],(TelAppCallback)&cdma_data_async_event_callback, NULL);

			if(api_err != TAPI_API_SUCCESS){
				TEST_DEBUG("tel_register_event isn't subscribed. sub id is %d api_err is %d\n",cdma_data_subscription_id[i],api_err);
				break;
			}
		}
	}
	else {

		TEST_DEBUG("tel_init() failed");
	}

}
