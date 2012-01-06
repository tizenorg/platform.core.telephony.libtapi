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

/**
 * @open
 * @ingroup		TelephonyAPI
 * @addtogroup	COMMON_TAPI	COMMON
 * @{
 *
 * @file TapiCommon.h
 * @brief  This file serves as a "C" header file defines functions for Tapi Common Interfaces like Callback Registration / De-registration. \n
 It contains a sample set of function prototypes that would be required by applications.


 */

#ifndef _TAPI_COMMON_H_
#define _TAPI_COMMON_H_

/*==================================================================================================
 INCLUDE FILES
 ==================================================================================================*/
#include <TelDefines.h>
#include <TelUtility.h>
#include <TapiEvent.h>

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 CONSTANTS
 ==================================================================================================*/

/*==================================================================================================
 MACROS
 ==================================================================================================*/

#define TAPI_EVENT_NAME_MAX_LEN			128		/**< Maximum length of theevent name */
#define TAPI_REQUEST_NOTI_ID			0xff	/**< Request id for notification message */
#define TAPI_REQUEST_INVALID_ID			-1		/**< Invalid request id */
#define TAPI_DBUS_CONNECTION_NAME_LEN_MAX	255

/*==================================================================================================
 ENUMS
 ==================================================================================================*/

/*==================================================================================================
 STRUCTURES AND OTHER TYPEDEFS
 ==================================================================================================*/

/**
 *This structure retrieves current application name registered in DBUS.
 */
typedef struct {
	char name[TAPI_DBUS_CONNECTION_NAME_LEN_MAX]; /**<application name*/
	unsigned int length_of_name; /**<length of application name*/
} tapi_dbus_connection_name;

/**
 *This structure holds information like length and actual tunneling data.
 */
typedef struct {
	unsigned int TunnelDataLen; /**<Length of tunnel data*/
	void *pTunnelData; /**<Tunneling information exchanged between OEM and application(Not Null terminated string)*/
} TelTunnelInfo_t;

/*==================================================================================================
 FUNCTION PROTOTYPES
 ==================================================================================================*/

/**
 * @brief This API shall be used to initialize TAPI library.
 *  This API internally initiate a D-bus shared connection to communicate with Telephony Server.
 *  This API should be called before any other TAPI APIs.
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  None.
 *
 * @warning
 *
 * @par Async Response Message:
 * - NA
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
 * #include <TapiCommon.h>
 *
 * int api_err;
 * api_err = tel_init(); //initialize telephony api if application use gmain loop
 * @endcode
 *
 * @see tel_register_event tel_deinit
 *
 * @remarks
 *  - None.
 *
 */
/*================================================================================================*/
int tel_init(void);

/**
 * @brief  This function is used to register the callback function for a specific TAPI Event.
 *           If Application tries to register a callback for a specific event multiple times, then multiple callbacks will be registered.
 *           When an event indication is received, then registered callbacks for the specific event will be invoked multiple times.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  Incase Application has registered for multiple callbacks for a event and it wants to have only one callback to be registered
 *	for that particular event, then Application needs to explicitly deregister the callbacks based on subscription id.
 *	If Application wants to modify the already registered event with a different callback, then it needs to deregister the already
 *	registered TAPI event with subscription id received and Application has to register again with a new callback for that TAPI event.
 *
 * @warning
 *
 * @param[in] EventType
 * - TAPI Event for which the callback registration is required.
 *
 * @param [out] SubscriptionId
 *   - An unique identifier identifying the callback registration.
 *
 * @param[in] AppCallBack
 * - Callback which will be invoked when an event occurs for that particular event class.
 *
 * @param[in] data
 * - User data which is passed as the callback parameter
 *
 * @par Async Response Message:
 * - None.
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
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <TapiCommon.h>
 * #include <ITapiCall.h>
 *
 * void app_callback(TelTapiEvent_t *event);
 * // REGISTER EVENT
 * void registerevent()
 * {
 *	unsigned int subscription_id = 0;
 *	int api_err = 0;
 *
 *	api_err = tel_init();
 *	if(api_err != 0){
 *		printf ("tel_init Failed - api_err = %d \n",api_err);
 *	}
 *	api_err = tel_register_event (TAPI_EVENT_CALL_SETUP_CNF ,
 *	                 &subscription_id, (TelAppCallback)&app_callback);
 *	printf("MISC Event registeration is Done: sub id is %d, api_err is %d\n", subscription_id, api_err);
 * }
 *
 * void callsetup()
 * {
 *	int ret_status = 0;
 *	int RequestID = 0;
 *	unsigned int CallHandle;
 *	TelCallSetupParams_t Params;
 *	char *normal = "1234";//Called party number
 *
 *	memset(&Params, 0, sizeof(TelCallSetupParams_t));
 *	Params.CallType = TAPI_CALL_TYPE_VOICE;
 *	strcpy(Params.szNumber, normal);
 *
 *	ret_status = tel_exe_call_mo (&Params, &CallHandle, &RequestID);
 *	printf ("The return value is %d\n", ret_status);
 *
 *	if (ret_status == 0)
 *		printf("successful\n");
 *	else
 *		printf("error=%d\n", ret_status); //WAIT FOR EVENT HERE
 * }
 *
 * static void app_callback(TelTapiEvent_t *event)
 * {
 *	int eventClass;
 *	int eventType;
 *  int requestId;
 *	int status;
 *	void *EventData = NULL;
 *
 *	unsigned int temp_handle = 0;
 *	eventClass = event->EventClass;
 *	eventType = event->EventType;
 *	requestId = event->RequestId;
 *	status = event->Status;
 *	EventData = event->pData;
 *
 *
 *	if (status != 0) {
 *		return;
 *	}
 *
 *	switch (eventType) {
 *		case TAPI_EVENT_CALL_SETUP_CNF:
 *			memcpy(&temp_handle, EventData, sizeof(unsigned int));
 *			break;
 *		case TAPI_EVENT_CALL_ALERT_IND:
 *			memcpy(&temp_handle, event->pData, sizeof(unsigned int));
 *			break;
 *		case TAPI_EVENT_CALL_CONNECTED_IND:
 *			memcpy(&temp_handle, event->pData, sizeof(unsigned int));
 *			break;
 *	}
 *
 *	// Handle the event data
 *	...
 * }
 * @endcode
 *
 * @see None.
 *
 * @remarks
 *  - None.
 *
 */
/*================================================================================================*/
int tel_register_event(int EventType, unsigned int *SubscriptionId, TelAppCallback AppCallBack, void *data);

/**
 * @brief  This function will de-register the application callback registered for the specific event or event class based on the
 *		subscription id. This identifier is the same subscription identifier which is returned back to the application during
 *		TAPI registration procedure.If de-register API is called during any service requests, Application registered callback
 *		will be cleared based on the subscription id. No application callback shall be invoked in case any confirmations/indications
 *		are received from TS.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  Application has to de-register each registration using subscription id explicitly as there is no overriding of callback registration.
 *
 * @warning
 *
 * @param[in] SubscriptionId
 * - An unique identifier identifying the callback registration.
 *
 * @par Async Response Message:
 * - None.
 *
 * @pre
 *  - None.
 *
 * @post
 *  - None.
 *
 * @return Return Type (TapiResult_t) \n
 * - TAPI_API_SUCCESS - indicating that the operation has completed successfully. \n
 * - Else it will return failure and error code (Refer #TapiResult_t)
 *
 * @par Prospective Clients:
 * External Apps.
 *
 * @code
 * #include <TapiCommon.h>
 * #include <ITapiCall.h>
 *
 * unsigned int subscription_id = 0;
 *
 * void registerevent()
 * {
 *	int api_err = 0;
 *	api_err = tel_init();
 *	if (api_err != 0) {
 *		printf ("tel_init Failed - api_err = %d \n", api_err);
 *		return;
 *	}
 *	api_err = tel_register_event (TAPI_EVENT_CALL_SETUP_CNF,
 *	                 &subscription_id, (TelAppCallback)&app_callback);
 *	printf("MISC Event registeration is Done: sub id is %d, api_err is %d\n", subscription_id, api_err);
 * }
 *
 * void deregisterevent()
 * {
 *	int api_err = 0;
 *  api_err = tel_deregister_event (subscription_id); //should be used subscription id which is generated at register time
 * }
 * @endcode
 *
 * @see None.
 *
 * @remarks
 *  - None.
 *
 */
/*================================================================================================*/
int tel_deregister_event(unsigned int SubscriptionId);

/**
 * @brief This API shall be used to de-initialize TAPI library.
 *  It shall handle the reinitializing routines related to Event delivery mechanism.
 *  This API internally shall handle Event delivery related de-initialization routines .
 *  This API should be called before any other TAPI API.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  None.
 *
 * @warning
 *
 * @par Async Response Message:
 * - NA
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
 * #include <TapiCommon.h>
 *
 * int api_err = 0;
 * api_err = tel_deinit(); //no in,out param required
 * @endcode
 *
 * @see None.
 *
 * @remarks
 *  - None.
 *
 */
/*================================================================================================*/
int tel_deinit(void);


/**
 * @brief This API shall be used to check whether telephony service is ready or not.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  None.
 *
 * @warning
 *
 *
 * @param[out] bStatus
 * - Status of the telephony service
 *
 *
 * @par Async Response Message:
 * - NA
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
 * #include <TapiCommon.h>
 *
 * int bStatus = 0;
 * int err_code;
 * err_code = tel_check_service_ready(&bStatus); //bStatus value is set 1 if ready
 *
 * @endcode
 *
 * @see None.
 *
 * @remarks
 *  - None.
 *
 */
/*================================================================================================*/
int tel_check_service_ready(int *bStatus);

/**
 * @brief This API shall be used to register the application name which listen
 *        the async response upon the TAPI requests
 *
 * @par Notes:
 * This function sets the application name. It is to determine the destination
 * which the Telephony Server send the asynchronous response upon TAPI request.
 * The TAPI callee can receive the response event.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  Before register an application name, tel_init() has to be called.
 *    Otherwise it will fail in registering the name.
 *
 * @warning
 *
 *
 * @param[in] name
 * - Application name.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - It should be invoked after registering telephony events and before starting a event loop.
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
 * #include <TapiCommon.h>
 * #include <glib.h>
 *
 * GMainLoop *loop = g_main_loop_new(NULL, false);
 *
 * tel_init();
 * tel_register_app_name("org.tizen.test"); //register application name. the format of name is not fixed
 * g_main_loop_run(loop);
 *
 * @endcode
 *
 * @see tel_get_app_name
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int tel_register_app_name(char *name);

/**
 * @brief This API shall be used to get the registered application name.
 *
 *
 * @par Sync (or) Async:
 * This is a Synchronous API.
 *
 * @par Important Notes:
 * -  None.
 *
 * @warning
 *
 *
 * @param[out] app_name
 * - Application name which is registered in telephony server.
 *
 *
 * @par Async Response Message:
 * - NA
 *
 * @pre
 *  - It should be invoked after registering telephony events and before starting a event loop.
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
 * #include <TapiCommon.h>
 *
 * tapi_dbus_connection_name app_name;
 * tel_get_app_name(&app_name); //if application want to get registered name itself
 *
 * @endcode
 *
 * @see tel_register_app_name
 *
 * @remarks
 *  - None.
 */
/*================================================================================================*/
int tel_get_app_name(tapi_dbus_connection_name *app_name);

/**
 * @brief  This API allows Application to send any proprietary information to OEM.
 *
 * @par Notes:
 * This API shall be used when any proprietary information what telephony server is transparent is needed to send the OEM layer.
 *
 * @par Sync (or) Async:
 * This is an Asynchronous API.
 *
 * @par Important Notes:
 * -  None.
 *
 * @warning
 *
 * @param[in] pTunnelInfo
 * - This information holds the length of the tunneling data and actual tunneling data.
 *
 * @param[out] pRequestId
 * - Callback which will be invoked when an event occurs for that particular event class.
 *
 * @par Async Response Message:
 * - This Asynchronous event TAPI_EVENT_TUNNEL_INFO_CNF is returned in response and associated event
 *    data is #TelTunnelInfo_t with this event. Asynchronous return status will be indicated by tunnel_status
 *    which is a Boolean indicating success(1-True) or Failure(0-False)
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
 * #include <TapiCommon.h>
 *
 * int requestId = 0;
 * TelTunnelInfo_t ti;
 * int test = 1234;
 *
 * ti.pTunnelData = (void *)test;
 * ti.TunnelDataLen = sizeof(test); //should be set correct size
 * ret = tel_request_tunnel (&ti, &requestId);
 *
 * @endcode
 *
 * @see None.
 *
 * @remarks
 *  - None.
 *
 */
/*================================================================================================*/
int tel_request_tunnel(const TelTunnelInfo_t *pTunnelInfo, int *pRequestId);

#ifdef __cplusplus
}
#endif

#endif /* _TAPI_COMMON_H_ */

/**
 *  @}
 */

