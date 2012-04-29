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
* @addtogroup	NetText_TAPI    NetText(SMS)
* @{
 *
* @file ITapiNettext.h

     @brief This file serves as a "C" header file defines functions for Tapi Nettext\n
      It contains a sample set of function prototypes that would be required by applications.

 */

#ifndef _ITAPI_NETTEXT_H_
#define _ITAPI_NETTEXT_H_

#include <TapiCommon.h>
#include <TelNetText.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @enum TelSmsDeviceStatus
 * This defines the enumeration for device ready value
 */
typedef enum {
	TAPI_NETTEXT_DEVICE_READY = 0x01, /**< device is ready */
	TAPI_NETTEXT_DEVICE_NOT_READY = 0x02 /**< Device not ready */
} TelSmsDeviceStatus;

 /**
 * @brief  This API is used to send an SMS message to the network.  This API interface allows transmitting SMS PDU as defined by
 *		3GPP TS 23.040 (SMS-SUBMIT, SMS-COMMAND) to lower layers. This API allows keeping the dedicated link at lower layers
 *		by passing information as more messages to send in parameters. This will enable not to release if a dedicated connection is
 *		used for transmission. Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pDataPackage
 *   - Contains SMS-SUBMIT TPDU
 *
 * @param [in] bMoreMsgToSend
 *   - will be set to TRUE when there are more than one message to be send or Multiple segmented concatenated
 *      message to be send, otherwise FALSE. This flag indicates OEM that more messages to send.
 *
 * @param [out] pRequestId
 * - Unique identifier for a particular request.
 * - pRequestId value can be any value from 0 to 255 if the API is returned successfully
 * - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 *  SMS sent status is returned in this event TAPI_EVENT_NETTEXT_SENTSTATUS_CNF and the event data associated with this event
 *  is #TelSmsDatapackageInfo_t that has SMS PDU as defined by 3GPP TS 23.040(SMS-SUBMIT-REPORT). Asynchronous return status is indicated by #TelSmsResponse_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * TelSmsDatapackageInfo_t *pDataPackage;
 * int ret_status;
 * int RequestId = 0;
 *
 * pDataPackage = (TelSmsDatapackageInfo_t*) malloc(sizeof(TelSmsDatapackageInfo_t));
 * // fill the structure appropriately
 * // Sca parameter is optional. If you don't provide Service center address, you should fill "00" in sca array.
 * //  MsgLength is length of szData//fill the szData in TelSmsDatapackageInfo_t with SMS-SUBMIT or SMS-COMMAND TPDU.
 * ret_status = tel_send_sms (pDataPackage, 0, &RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_send_sms(const TelSmsDatapackageInfo_t *pDataPackage, int bMoreMsgToSend, int *pRequestId);

 /**
 * @brief  This API is used to send an SMS message to the network.  This API interface allows transmitting SMS PDU as defined by
 *		3GPP2 C.S0015 (SMS-SUBMIT, SMS-ACK, SMS-CANCEL) to lower layers. This API allows keeping the dedicated link at lower layers
 *		by passing information as more messages to send in parameters. This will enable not to release if a dedicated connection is
 *		used for transmission. Access to this API is limited to in-house application and  we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pSmsMsgInfo
 *   - Contains parameter mask, message type and data.
 *
 * @param [in] MoreMsgToSend
 *   - will be set to TRUE when there are more than one message to be send or Multiple segmented concatenated
 *      message to be send, otherwise FALSE. This flag indicates OEM that more messages to send.
 *
 * @param [out] pRequestId
 * - Unique identifier for a particular request.
 * - pRequestId value can be any value from 0 to 255 if the API is returned successfully
 * - -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 *
 * @par Async Response Message:
 *  SMS sent status is returned in this event TAPI_EVENT_NETTEXT_SENTSTATUS_EX_CNF and the event data associated with this event
 *  is #TelSmsIs637CauseCode_t. Asynchronous return status is indicated by #TelSmsResponse_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 * int ret_status = TAPI_API_SUCCESS;
 * int requestId = 0;
 * TelSmsMsgInfo_t pMsgInfo={0,};
 * char* diallingNum;
 * int dialNumLen;
 * TelSmsMsgInfo_t *pDataPackage = malloc(sizeof(TelSmsMsgInfo_t));
 *
 * memset(pDataPackage, 0, sizeof(TelSmsMsgInfo_t));
 * pDataPackage->MsgType = TAPI_NETTEXT_MESSAGETYPE_SUBMIT; //fill structure appropriately
 * pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_TELESERVICE_MASK;
 *
 * ret_status = tel_send_sms_msg(pDataPackage, 0 ,&requestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_send_sms_msg(const TelSmsMsgInfo_t *pSmsMsgInfo, int MoreMsgToSend, int *pRequestId);

 /**
 * @brief  This function reads a message from SIM card storage. SIM index (index used when storing the SMS) number to be passed to in order
 *		to read particular SMS message. Total number of messages can be retrieved by #TelTapiSmsGetCount function.
  *		Access to this API is limited to in-house application and  we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] Index
 *  -  Index number of the message to be read
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_NETTEXT_READ_SMS_CNF and the event data is #TelSmsData_t. Asynchronous return status is indicated
 *  by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_read_sms_in_sim(0, &RequestId); //index value which is located in SIM
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_read_sms_in_sim(int Index, int* pRequestId);

 /**
 * @brief  This function writes a message to SIM storage area.
 *           Access to this API is limited to in-house application and  we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pWriteData
 *  - SMS message structure.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  TAPI library will send TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF asynchronous event for this request when received from TS.
 *  The associated data is SIM Index in which the message stored. Asynchronous return status is indicated by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 * TelSmsData_t *pWriteData;
 * char *msg = "This is an Test SMS";
 * char *SCA = "821091";
 *
 * pWriteData = malloc(sizeof(TelSmsData_t));
 *
 * conv_service_center_address(pWriteData->SmsData.Sca, SCA); //convert service center address
 * conv_msg_into_tpdu(pWriteData->SmsData.szData, pWriteData->SmsData.MsgLength, msg); // convert message into TPDU
 * pWrite->MsgStatus = TAPI_NETTEXT_STATUS_READ;
 * pWrite->SimIndex = -1;
 *
 * ret_status = tel_write_sms_in_sim (pWriteData, &RequestId);
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_write_sms_in_sim(const TelSmsData_t *pWriteData, int *pRequestId);

 /**
 * @brief  This function deletes a message specified by index (SIM index used when storing the SMS)
 *           number from the SIM storage area. Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] Index
 *  - Message index to delete.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The asynchronous event associated is TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF and the event data is SIM index from which
 * the message has been deleted. Asynchronous return status is indicated by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t).\n
 * @par Prospective Clients:
 *  External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_delete_sms_in_sim(1, &RequestId); //index which is located in SIM
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_delete_sms_in_sim(int Index, int *pRequestId);

 /**
 * @brief  This function is used to retrieve message count information like Total record count,
 *  used record count and the indices list for SIM Storage. This is an asynchronous function and
 *  the response will be sent back by event notification. Access to this API is limited, we recommend you use Message Framework API(MAPI) besides.
 *  Access to this API is limited to in-house application.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_GET_COUNT_CNF and the event data is #TelSmsStoredMsgCountInfo_t.
 * Asynchronous return status is indicated by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * - External Apps.\n
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_get_sms_count (&RequestId); //get sms count in SIM
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_get_sms_count(int *pRequestId);

 /**
 * @brief  This function is used to set SMS CB Message Identifiers in the appropriate EF-CBMI/EF-CBMIR file in (U)SIM.
 *		This function also indicates underlying provider to enable or disable CB Channel on which CB messages are received.
 *		Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI) besides.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pCBConfig
 *  - This structure contains parameters to set SMS CB configuration \n
 *      (i)  Cell broadcast service state. (Enabled/disabled.) \n
 *      (ii) Cell broadcast message identifier, which denotes whether all cell broadcast message identifiers are selected or only
 *           some of those.\n
 *	  (iii) The list count of message identifiers to be set.\n
 *	  (iv) The actual list of message identifiers to be set. Each identifier occupies 2 bytes. And the list ranges from 0x00 to 0xff. \n
 *    These message identifiers denote the source and type of the CB message. Based on the message identifiers set in the SIM,
 *    the ME receives CB messages with selected message identifiers.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_SET_REQUEST_CNF and there is no data associated with this event.
 * Asynchronous return status is indicated by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.\n
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 * TelSmsCbConfig_t *pCBConfig = NULL;
 *
 * pCBConfig = (TelSmsCbConfig_t *)malloc(sizeof(TelSmsCbConfig_t));
 * pCBConfig->bCBEnabled = 1; //fill structure appropriately
 * pCBConfig->SelectedId = 0x01;
 * pCBConfig->MsgIdCount = 0x02;
 *
 * pCBConfig->MsgIDs[0] = 0x0c;
 * pCBConfig->MsgIDs[1] = 0x0c;
 * pCBConfig->MsgIDs[2] = 0xdf;
 * pCBConfig->MsgIDs[3] = 0x00;
 *
 * ret_status = tel_set_sms_cb_config(pCBConfig, &RequestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_set_sms_cb_config(const TelSmsCbConfig_t *pCBConfig, int *pRequestId);

 /**
 * @brief  This function is used to retrieve SMS CB configuration options from EFcbmi file in the SIM.
 *		Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF and the event data is #TelSmsCbConfig_t.
 * Asynchronous return status is indicated by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * - External Apps.\n
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_get_sms_cb_config (&RequestId); //result will come with async response
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_get_sms_cb_config(int *pRequestId);


 /**
 * @brief This function is used to set SMS preferred bearer on which SMS has to be transmitted.
 *	      Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] BearerType
 *  - Specifies the bearer type.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_SET_REQUEST_CNF and there is no data associated with this event.
 * Asynchronous return status is indicated by #TelSmsCause_t
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 * TelSmsBearerType_t BearerType = TAPI_NETTEXT_BEARER_PS_ONLY;
 *
 * ret_status = tel_set_sms_preferred_bearer(BearerType, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_set_sms_preferred_bearer(TelSmsBearerType_t BearerType, int *pRequestId);

 /**
 * @brief  This function is used to get SMS preferred bearer on which SMS has to be transmitted.
 *  This is an asynchronous function. Access to this API is limited to in-house application and , we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF and the associated event data is #TelSmsBearerType_t.
 * Asynchronous return status is indicated by #TelSmsCause_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_get_sms_preferred_bearer(&RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_get_sms_preferred_bearer(int *pRequestId);

 /**
 * @brief This function is used to get the SMS parameters for a particular SMS (sent/received)
 *  based on the SIM index where it is stored. This is an asynchronous function. Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] index
 *  - SIM index where the SMS is stored
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequest Id value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_GET_PARAM_CNF and the event data is #TelSmsParams_t. Asynchronous return status
 * is indicated by #TelSmsCause_t
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_get_sms_parameters(1, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_get_sms_parameters(int index, int *pRequestId);

 /**
 * @brief  This function is used to set the short message service header parameters which are
 *  used in the origination of MO messages. Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - This API updates/sets SMS Parameters in EFsmsp file in the SIM storage
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pSmsSetParameters
 *  - Contains SMS header Parameter information like service center address, destination address, validity period, protocol identifier,
 *     data coding scheme.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_SET_REQUEST_CNF and there is no data associated with this event. Asynchronous
 * return status is indicated by #TelSmsCause_t
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 * TelSmsParams_t SmsSetParameters = {0,};
 *
 * SmsSetParameters.RecordIndex = 0x01;
 * SmsSetParameters.RecordLen = 28;
 * memcpy(SmsSetParameters.TpDestAddr.szDiallingNum, "9986529874", 10);
 * printf("\ndial no is %s\n", SmsSetParameters.TpDestAddr.szDiallingNum);
 * SmsSetParameters.TpDestAddr.DialNumLen = 10;
 * SmsSetParameters.TpDestAddr.Ton = TAPI_SIM_TON_NATIONAL;//national no
 * SmsSetParameters.TpDestAddr.Npi = TAPI_SIM_NPI_NATIONAL;
 *
 * ret_status = tel_set_sms_parameters(SmsSetParameters, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_set_sms_parameters(const TelSmsParams_t *pSmsSetParameters, int *pRequestId);

 /**
 * @brief This function is used to send a delivery report for the received incoming SMS to the network.
 *          Access to this API is limited to in-house application.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pDataPackage
 *  - Contains SMS Data package..
 *
 * @param [in] RPCause
 *  - Indicates the reason for SMS-DELIVER failure
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF and there is no associated event data. Asynchronous return status
 * is indicated by #TelSmsResponse_t
 *
 * @pre
 *  - SMS-DELIVER-REPORT message structure should be in TPDU format as specified by 3GPP TS 23.040 SMS TPDU.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = 0;
 * Int requestId = -1;
 * TelSmsResponse_t RPCause;
 * TelSmsDatapackageInfo_t *del_report = NULL;
 *
 * del_report = malloc(sizeof(TelSmsDatapackageInfo_t));
 * memset(del_report, 0, sizeof(TelSmsDatapackageInfo_t));
 * //Encode Send Delivery Report here
 * //EncodeSmsDeliveryReportTpdu();
 *
 * RPCause = TAPI_NETTEXT_SENDSMS_SUCCESS;
 * printf("***receive message (Api:SendDeliverreport)****\n");
 *
 * ret_status = tel_send_sms_deliver_report(del_report, RPCause, &requestId);
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_send_sms_deliver_report(const TelSmsDatapackageInfo_t *pDataPackage, TelSmsResponse_t RPCause, int *pRequestId);

 /**
 * @brief  This function is used to set SMS Service Centre Address information in order to send the SMS.
 *           Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * - This API updates/sets service center address in EFsmsp file in the SIM storage.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] pSCA
 * - Contains the service centre address informations.
 *
 * @param [in] Index
 * - Contains the record index of the Service center address information in the EF.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_SET_REQUEST_CNF and there is no data associated with this event.
 * Asynchronous return status is indicated by #TelSmsResponse_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t).
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status;
 * int RequestId = 0;
 * BYTE	scabuf[512];
 * char	scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
 * BYTE	scaNum_len =0;
 * TelSmsAddressInfo_t *sc_addr = NULL;
 * BYTE	packet[500];
 * int idx = 0;
 * int i;
 *
 * printf("***Setting the SCA(Api:SetSCA)****\n");
 * sc_addr = (TelSmsAddressInfo_t *)malloc(sizeof(TelSmsAddressInfo_t));
 * memset(sc_addr, 0, sizeof(sc_addr));
 * memset(scaNum, 0, sizeof(scaNum));
 * memset(scabuf, 0, sizeof(scabuf));
 * sprintf(scaNum, "821091");
 * scaNum_len = strlen(scaNum);
 * scaNum_len = strlen(scaNum);
 * scaNum[scaNum_len] = 0;
 * scaNum_len = scaNum_len - 1;
 *
 * //idx = SmsUtilEncodeAddrField(packet, scaNum, scaNum_len, 0x01, 0x01);
 * //get idx
 * sc_addr->DialNumLen = idx ;
 * memcpy(sc_addr->szDiallingNum, packet, (unsigned int)idx);
 * ret_status = tel_set_sms_sca(sc_addr, 0, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_set_sms_sca(const TelSmsAddressInfo_t *pSCA, int Index, int *pRequestId);

 /**
 * @brief  This function is used to get current SMS Service Centre Address information.
 *           Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [in] Index
 * - Contains the record index of the Service center address information in the EF.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 * The event associated is TAPI_EVENT_NETTEXT_GET_SCA_CNF and there is no data associated with this event.
 * Asynchronous return status is indicated by #TelSmsResponse_t.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_get_sms_sca(0, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_get_sms_sca(int Index, int *pRequestId);

 /**
 * @brief This function is used by the applications to inform modem about  the memory status of PDA (whether FULL or AVAILABLE) so that modem can inform network .
 *           Access to this API is limited to in-house application.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] memoryStatus
 *  - Contains Memory Status
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_NETTEXT_SET_REQUEST_CNF and there is no associated event data. Asynchronous return status
 *  is indicated by #TelSmsResponse_t
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 * int MemoryStatus = TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE;
 *
 * ret_status = tel_set_sms_memory_status(MemoryStatus, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int  tel_set_sms_memory_status(int memoryStatus, int *pRequestId);

 /**
 * @brief  This function is used by the applications to set the message status in the SIM EF.
 *           Access to this API is limited to in-house application and we recommend you use Message Framework API(MAPI).
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in] Index
 *  - Index of the message whose status has to be set.
 *
 *
 * @param [in] msgStatus
 *  - Status of the message to be set.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_NETTEXT_SET_REQUEST_CNF and there is no associated event data. Asynchronous return status
 *  is indicated by #TelSmsResponse_t
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 * int index = 1;
 * int MemoryStatus = TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE;
 *
 * ret_status = tel_set_sms_memory_status(index, MemoryStatus, &RequestId); //result will come with async response
 *
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_set_sms_message_status(int Index, TelSmsMsgStatus_t msgStatus, int *pRequestId);

 /**
 * @brief This function is used by the applications to get the count of SMS parameter records stored in SIM EF.
 *           Access to this API is limited to in-house application.
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 * @param [out] pRequestId
 *	-  Unique identifier for a particular request.
 *	-  pRequestId value can be any value from 0 to 255 if the API is returned successfully
 *	-  -1 (INVALID_REQUEST_ID) will be sent in case of failure.
 *
 * @par Async Response Message:
 *  The event associated is TAPI_EVENT_NETTEXT_PARAM_COUNT_IND and there is no associated event data. Asynchronous return status
 *  is indicated by #TelSmsResponse_t
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <ITapiNetText.h>
 *
 * int ret_status = TAPI_API_SUCCESS;
 * int RequestId = 0;
 *
 * ret_status = tel_get_sms_parameter_count (&RequestId); //result will come with async response
 * @endcode
 *
 * @see
 *  - None.
 *
 * @remarks
 *  - None.
 *
 *
 */
/*================================================================================================*/
int tel_get_sms_parameter_count(int *pRequestId);


/**
 * @brief This function is used by the applications check the device status (whether ready or not) .
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in]
 * - None
 *
 * @param [out] pDeviceStatus
 * - Contains Device Status
 *
 * @par Async Response Message:
 *  None.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_check_sms_device_status(int *pDeviceStatus);

/**
 * @brief This function is used by the applications to inform modem about the PDA status(whether PDA is ready to
 * recieve messages from network or not).
 *
 *
 * @par Sync (or) Async:
 * This is a Asynchronous API.
 *
 * @par Important Notes:
 * - None.
 *
 * @warning
 * - None.
 *
 *
 * @param [in]
 *  - None
 *
 * @param [out]
 *  - None
 *
 * @par Async Response Message:
 *  None.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (int) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 * @par Prospective Clients:
 * External Apps.
 *
 */
int tel_set_sms_device_status(void);

#ifdef __cplusplus
}
#endif

#endif	/* _ITAPI_NETTEXT_H_ */

/**
* @}
*/
