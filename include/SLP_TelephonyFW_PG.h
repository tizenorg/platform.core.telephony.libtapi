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
 * @ingroup   SLP_PG
 * @defgroup   TELEPHONY Telephony
@{

<h1 class="pg">Introduction</h1>
	<h2 class="pg">Purpose</h2>
The purpose of this document is to describe how applications can use Telephony Framework APIs. This document gives programming guidelines to application engineers.

	<h2 class="pg">Scope</h2>
The scope of this document is limited to Samsung Linux platform Telephony Framework API usage.

	<h2 class="pg">Abbreviations</h2>
<table>
<tr><td>OEM</td><td>Original Equipment Manufacturer</td></tr>
<tr><td>RPC</td><td>Remote Procedure Call</td></tr>
<tr><td>SAP</td><td>Sim Access Profile</td></tr>
<tr><td>SAT</td><td>SIM Application Toolkit</td></tr>
<tr><td>SIM</td><td>Subscriber Identity Module</td></tr>
<tr><td>SVC</td><td>Service</td></tr>
<tr><td>TAPI</td><td>Telephony API</td></tr>
<tr><td>EFL</td><td>Enlightenment Foundation Libraries</td></tr>
</table>

	<h2 class="pg">Restriction Modules</h2>
CALL, SMS, GPRS, SAT have the permission. Refer to each module API description
@}
@defgroup Telephony_Architecture 1.Architecture
@ingroup TELEPHONY
@{
<h1 class="pg">Telephony Framework Architecture</h1>
@image html SLP_TelephonyFW_PG_image001.png

@image html SLP_TelephonyFW_PG_image002.png System Architecture of Telephony Framework

Telephony is a middleware component which acts as an interface layer between applications and the OEM.
@n Telephony Framework provides interfaces to applications in the form of a library and to the OEM providers in the form of OEM plug-in.
@n The applications make use of the library of exported APIs, which uses a RPC mechanism for making telephony service requests.
@n On the other side, OEM-Plug-in library is the OEM layer, which is the wireless protocol stack specific implementation. This layer acts as a plug-in giving flexibility for any wireless protocol stack to be used avoiding any changes to be made in the Telephony clients.
@n Telephony has certain synchronous and asynchronous exported APIs. Synchronous APIs provide data as the function parameter passed to Telephony. Asynchronous APIs provide data using an event based mechanism.
@image html SLP_TelephonyFW_PG_image003.png Asynchronous service request (response Use-Case Diagram)
@image html SLP_TelephonyFW_PG_image003.png Synchronous service request (response Use-Case Diagram)
@}
@defgroup Telephony_Feature 2.SubModules & Services
@ingroup TELEPHONY
@{

<h1 class="pg">Telephony Framework SubModules & Services</h1>
	<h2 class="pg">Call & Call-dependent SS</h2>
	Also see Use Cases of @ref Use_Cases2_CALL
- Initiate, accept, reject and end calls.
- Call supplementary service ( Hold,CLI, Multiparty, Call Waiting, Forwarding, Barring)
- Get Call Time, Status, Duration, Conference List
- Get and Set Active Line
- Retrieve, swap, and transfer call.
- For CDMA
	- Call Flash Information
	- Get and Set voice privacy mode

	<h2 class="pg">SMS</h2>
	Also see Use Cases of @ref Use_Cases4_SMS
SMS service in Telephony Framework only provides the interface to the modem and doesn’t handle Transport layer, storing and deleting the SMS in the storage except SIM. We recommend you use MAPI of Message Framework.
- Send, save, receive, read and delete network texts.
- Receive Cell-Broadcast Message.
- Set and Get Cell broadcast configuration.
- Set message status, memory status.
- Set and Get sms parameters

	<h2 class="pg">Supplementary Service</h2>
- Interrogation and activation for Call Barring, forwarding and waiting.
- USSD services, AOC Services.

	<h2 class="pg">Network Registration/Configuration</h2>
- Search and Select Network.
- Set and Get Selection Mode, Service Domain, network Mode and Band.
- Get Network information.
- For CDMA
	- Get and Set CDMA Hybrid Mode
	- Get and Set Roaming Mode, Preferred PLMN

	<h2 class="pg">SIM</h2>
- Handling SIM security procedure (PIN, PUK).
- Get / Update / Delete SIM EF files.
- Support 2G/3G Phonebook data.
- SIM Lock Personalisation enable or disable
- SIM lock enable or disable
- SAP

	<h2 class="pg">SAT (SIM Application Tool Kit)</h2>
- Get main menu information from the SIM application
- Send envelope commands to the SIM application
- Send the execution results of which the SIM application requests to applications
- Send UI / User confirmation to the SIM application

	<h2 class="pg">Sound</h2>
Sound that is related with voice call is used to control modem configuration
- Sound user configuration
- Voice path control
- Volume control
- Mic mute control

	<h2 class="pg">Common</h2>

- TAPI initialization & De-initialization
- Register, Deregister event
- TAPI Initialization & De-initialization for Ecore
- Register, Deregister Ecore event
- Request Connection name

@}

<h1 class="pg">Feature Description</h1>
<h2 class="pg">COMMON</h2>

@defgroup Use_Cases1_1 TAPI Initialization
@ingroup Use_Cases1_COMMON
@{
<h3 class="pg">TAPI Initialization</h3>
This API shall be used to initialize the TAPI library. This API internally performs initialization routines related to Event delivery.
@n This API should be called before any other TAPI APIs. This API should be used by GTK/Glib applications.

@code
int tel_init (void);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void tapiinit()
{
	int api_err;

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf ("tel_init Failed - api_err = %d\n", api_err);
	}
}
@endcode
@}
@defgroup Use_Cases1_2 TAPI De-initialization
@ingroup Use_Cases1_COMMON
@{
		<h3 class="pg">TAPI De-initialization</h3>
This API shall be used to de initialize the Tapi library.
@n It shall perform the deinitializing routines related to event delivery.
@n The API shall be called finally, as a part of clean up routines. This API should be used by GTK/Glib applications.

@code
int tel_deinit (void);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void tapideinit()
{
	int api_err;

	api_err = tel_deinit();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf ("tel_deinit Failed - api_err = %d\n", api_err);
	}
}
@endcode
@}
@defgroup Use_Cases1_3 Register Event
@ingroup Use_Cases1_COMMON
@{

		<h3 class="pg">Register Event</h3>
API is used to register the callback function for a specific TAPI Event. Upon successful registration of the event, the function shall return a valid subscription id.

If Application registers multiple callbacks for a specific event, then all the callbacks will be called when an event indication is received.
@n This API should be used by GTK/Glib applications.

@code
int tel_register_event (int EventType , unsigned int * SubscriptionId, TelAppCallback AppCallBack);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void app_callback(TelTapiEvent_t *event);

// REGISTER EVENT
void registerevent()
{
	unsigned int subscription_id = 0;
	int api_err;

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf ("tel_init Failed - api_err = %d\n", api_err);
	}

	api_err = tel_register_event (TAPI_EVENT_CALL_SETUP_CNF, &subscription_id, (TelAppCallback)&app_callback);

	printf("MISC Event registeration is Done: sub id is %d, api_err is %d\n", subscription_id, api_err);

	tel_register_app_name("org.tizen.appname");
}

void app_callback(TelTapiEvent_t *event)
{
	//Callback function to be called
}
@endcode
@}

@defgroup Use_Cases1_4 Deregister Event
@ingroup Use_Cases1_COMMON
@{

		<h3 class="pg">Deregister Event</h3>
API shall de-register the pre-registered TAPI event and the associated application callback, based on the subscription id.
@n This identifier is the same as the subscription identifier returned to the application during the TAPI registration procedure. If the de-register API is called during any service requests, the registered callback will be cleared based on the subscription id. No application callback shall be called when any confirmation/indication events are received after de-registration.
@n This API should be used by GTK/Glib applications.

@code
int tel_deregister_event ( unsigned int  SubscriptionId);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void app_callback(TelTapiEvent_t *event);

void registerevent()
{
	unsigned int subscription_id = 0;
	int api_err;

	api_err = tel_init();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf ("tel_deregister_event Failed - api_err = %d\n", api_err);
	}

	// REGISTER EVENT
	api_err = tel_register_event (TAPI_EVENT_CALL_SETUP_CNF, &subscription_id, (TelAppCallback)&app_callback);
	printf("MISC Event registeration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);

	tel_register_app_name("org.tizen.appname");

	// DEREGISTER EVENT
	api_err = tel_deregister_event (subscription_id);
	if (api_err != TAPI_API_SUCCESS)
	{
		printf("Event Deregistration Failed\n");
	}
}

void app_callback(TelTapiEvent_t *event)
{
	// callback function to be called
}
@endcode
@}


@defgroup Use_Cases1_5 TAPI Ecore Initialization
@ingroup Use_Cases1_COMMON
@{

	<h3 class="pg">TAPI Ecore Initialization</h3>
This API shall be used by Ecore based applications, to initialize TAPI library. This API internally performs event delivery related initialization routines. This API shall be called before any other TAPI APIs.
@n This API is for applications using Ecore library.

@code
int tel_init_ecore_loop (void);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void tapiinitecore()
{
	int api_err;

	api_err = tel_init_ecore_loop();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf("tel_init_ecore_loop Failed - api_err = %d\n", api_err);
	}
}
@endcode

5		<h3 class="pg">TAPI Ecore De-Initialization</h3>
API shall be used to de-initialize the TAPI library. It shall be used by Ecore based applications.
@n This API shall be called finally, as a part of cleanup routine. This API is for applications using Ecore library.

@code
int tel_deinit_ecore_loop (void);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void tapideinitecore()
{
	int api_err;

	api_err = tel_deinit_ecore_loop();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf("tel_deinit_ecore_loop Failed - api_err = %d\n", api_err);
	}
}
@endcode
@}

@defgroup Use_Cases1_6 TAPI RegisterEvent Ecore
@ingroup Use_Cases1_COMMON
@{

		<h3 class="pg">TAPI RegisterEvent Ecore</h3>
API shall be used to register for TAPI events (for application that use EFL).
@n Upon successful registration of the event, the API shall return a valid subscription id.
@n If Application registers multiple callbacks for a specific event, then all the callbacks will be called when an event indication is received
@n This API is for applications using Ecore library.

@code
int tel_register_event_at_ecore_loop(int EventType, unsigned int * SubscriptionId, TelAppCallback AppCallBack);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

void app_callback(TelTapiEvent_t *event);

void registereventecore()
{
	int api_err;
	unsigned int subscription_id = 0;

	api_err = tel_register_event_at_ecore_loop (TAPI_EVENT_CALL_SETUP_CNF, &subscription_id, (TellAppCallback)&app_callback);
	if (api_err != TAPI_API_SUCCESS)
	{
		printf ("tel_register_event_at_ecore_loop Failed - api_err = %d \n", api_err);
	}
	printf("RegisterEventEcore done with subscription_id:%d and return Status:%d\n", subscription_id, api_err);

	tel_register_app_name("org.tizen.appname");
}

void app_callback(TelTapiEvent_t *event)
{
	//Callback function to be called
}
@endcode
@}


@defgroup Use_Cases1_7 TAPI DeRegisterEvent Ecore
@ingroup Use_Cases1_COMMON
@{

	<h3 class="pg">TAPI DeRegisterEvent Ecore</h3>
API shall de-register the pre-registered TAPI event and the associated application callback, based on the subscription id.
@n This identifier is the same as the subscription identifier returned to the application during the TAPI registration procedure.
@n If the de-register API is called during any service requests, the registered callback will be cleared based on the subscription id. No application callback shall be called when any confirmation/indication events are received after de-registration.
@n This API is for applications based on Ecore library.

@code
int tel_deregister_event_at_ecore_loop (unsigned int SubscriptionId);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

Void deregistereventecore()
{
	int api_err;
	unsigned int subscription_id = 0;

	// REGISTER EVENT
	api_err = tel_register_event_at_ecore_loop (TAPI_EVENT_CALL_SETUP_CNF, &subscription_id, (TellAppCallback)&app_callback);
	printf("RegisterEventEcore done with subscription_id:%d and return Status:%d\n", subscription_id, api_err);

	tel_register_app_name("org.tizen.appname");

	// DEREGISTER EVENT
	api_err = tel_deregister_event_at_ecore_loop (subscription_id);
	if (api_err != TAPI_API_SUCCESS)
	{
		printf("tel_deregister_event_at_ecore_loop Failed - api_err = %d\n", api_err);
	}
}
@endcode
@}

@defgroup Use_Cases1_8 Register Application Name
@ingroup Use_Cases1_COMMON
@{

		<h3 class="pg">Register Application Name</h3>
This function registers the application name which requests any telephony service. Telephony Server uses this name as the destination name of response message upon the service request. If the name is not set, the request will fails. Before an application invoke this function, it shall initialize a connection to Telephony Sever with #tel_init or #tel_init_ecore_loop. And the application name shall be registered prior to start a event loop.

@code
int tel_register_app_name(char *name)
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>

Void register_app_name()
{
	tel_init();
	//register telephony event
	tel_register_event(...);
	//...
	tel_register_app_name("org.tizen.appname");
	GMainLoop *loop = g_main_loop_new(NULL, false);
	g_main_loop_run(loop);
}
@endcode
@}

@defgroup Use_Cases1_9 Get Application Name
@ingroup Use_Cases1_COMMON
@{

		<h3 class="pg">Get Application Name</h3>
This function get the application name which was registered with #tel_register_application_name.

@code
int tel_get_app_name(tapi_dbus_connection_name *app_name)
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>

void get_app_name()
{
	tapi_dbus_connection_name app_name;
	tel_get_app_name(&app_name);
	printf("app_name = %s\n", app_name.name);
}
@endcode
@}
@defgroup Use_Cases1_10 TAPI Check service ready
@ingroup Use_Cases1_COMMON
@{

	<h3 class="pg">TAPI Check service ready </h3>
API is used to get the modem status i.e. whether modem is on or off.

@code
int tel_check_service_ready (int* bStatus)
@endcode

SAMPLE CODE:
@code
#include <stdio.h>
#include <TapiCommon.h>

void getmodemstatus()
{
	int bStatus = 0;
	int err_code;

	err_code = tel_check_service_ready(&bStatus);
	if (err_code != TAPI_API_SUCCESS) {
		printf("Error Code [%x]\n", err_code);
	}
	else {
		if (bStatus == 0)
			printf("Telephony Service Not Ready";
		else if (bStatus == 1)
			printf("Telephony Service Ready";
	}
}
@endcode
@}


<h2 class="pg">CALL</h2>

@defgroup Use_Cases2_1 Event Register and Deregister
@ingroup Use_Cases2_CALL
@{

	<h3 class="pg">Event Register and Deregister</h3>
To receive asynchronous event notifications for the call APIs, the registration of related events with their associated call backs is required. After these are registered, when an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

SAMPLE CODE:
@code
// REGISTER EVENT
int api_err;
unsigned int subscription_id = 0;

api_err = tel_init();
if (api_err != TAPI_API_SUCCESS)
{
	printf("tel_init Failed - api_err = %d \n", api_err);
}

api_err = tel_register_event (TAPI_EVENT_CALL_CALL_SETUP_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("VOICE CALL Event registration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);

tel_register_app_name("org.tizen.appname");

// DEREGISTER EVENT
api_err = tel_deregister_event (subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Unregistration Fail\n");
}
@endcode
@}

@defgroup Use_Cases2_2 Call Setup
@ingroup Use_Cases2_CALL
@{

<h3 class="pg">Call Setup</h3>
This API originates an MO call setup.

@code
int  tel_exe_call_mo _t (const TelCallSetupParams_t* const pParams, unsigned int  * pCallHandle, int * pRequestID);
@endcode

On success, this API returns a valid call handle and the request ID as out parameters.
@n The MO call setup procedure continues in background. There are multiple states involved in the call setup procedure. Upon completion of the setup request, the event notifications and associated event data are sent to the client, based on the indication or response received by the Telephony Server.

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callsetup()
{
	int ret_status;
	int pRequestID = 0;
	unsigned int pCallHandle;
	TelCallSetupParams_t pParams;
	char normal[16] = "1234"; //Called party number

	memset(&pParams, 0, sizeof(TelCallSetupParams_t));
	pParams.CallType = TAPI_CALL_TYPE_VOICE;
	strcpy(pParams.szNumber, normal);

	ret_status = tel_exe_call_mo (&pParams, &pCallHandle, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status); //WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	unsigned int temp_handle = 0;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType)
	{
		case TAPI_EVENT_CALL_SETUP_CNF:
			printf("TAPI_EVENT_CALL_SETUP_CNF");
			memcpy(&temp_handle, EventData, sizeof(unsigned int));
			TEST_DEBUG("Received setup cnf for call Handle [%d]", temp_handle);

			break;

		case TAPI_EVENT_CALL_ALERT_IND:
			int index;
			TEST_DEBUG("TAPI_EVENT_CALL_ALERT_IND");
			memcpy(&temp_handle, event->pData, sizeof(unsigned int));
			TEST_DEBUG("Received Alert for call Handle [%d]", temp_handle);
			break;

		case TAPI_EVENT_CALL_CONNECTED_IND:
			int index;
			TEST_DEBUG("TAPI_EVENT_CALL_CONNECTED_IND");
			memcpy(&temp_handle, event->pData, sizeof(TS_UINT));
			TEST_DEBUG("Received Connected Event  for call Handle [%d]", temp_handle);
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases2_3 Call Answer
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Answer</h3>
This API responds to an incoming call either by accepting or rejecting the call.

Pre Conditions:
@n Call associated with the call handle should be in TAPI_CALL_STATE_INCOM state.
@n There should be no more than one existing call.

@code
int  tel_answer_call (unsigned int CallHandle,TelCallAnswerType_t AnsType, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"
//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

void callsetup(unsigned int CallHandle)
{
	int ret_status;
	int pRequestID = 0;

	ret_status = tel_answer_call (CallHandle, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status); //WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		//GET CALL HANDLE FROM INCOMING CALL EVENT DATA
		case TAPI_EVENT_CALL_INCOM_IND:
			unsigned int CallHandle;
			TelCallIncomingCallInfo_t IncomingInfo;
			memcpy(&IncomingInfo, event->pData, sizeof(TelCallIncomingCallInfo_t));
			CallHandle = IncomingInfo.CallHandle;
			break;

		case TAPI_EVENT_CALL_ANSWER_CNF:
			memcpy(&CallHandle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_ANSWER_CNF\n");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases2_4 Call Release
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Release</h3>
This API is used to release the call.
@n The call to be released can be a single call based on the call handle, or all active calls or all calls.

Pre Conditions:
@n Call handle should be valid and there should be an existing call in Active/hold state.

@code
int tel_release_call (unsigned int  CallHandle, int * pRequestID);
int tel_release_call_all (int * pRequestId);
int tel_release_call_all_active (int *pRequestID);
int tel_release_call_all_held (int *pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

void callrelease(unsigned int CallHandle)
{
	int ret_status;
	int pRequestID = 0;

	ret_status = tel_release_call (CallHandle, &pRequestID);
	//OR
	ret_status = tel_release_call_all (&pRequestID);
	//OR
	ret_status = tel_release_call_all_active (&pRequestID);
	//OR
	ret_status = tel_release_call_all_held (&pRequestID);

	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
} //WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback(TelTapiEvent_t *event)
{
	//...
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		//GET CALL HANDLE FROM INCOMING CALL EVENT DATA
		case TAPI_EVENT_CALL_INCOM_IND:
			TelCallIncomingCallInfo_t IncomingInfo;
			memcpy(&IncomingInfo, event->pData, sizeof(TelCallIncomingCallInfo_t));
			CallHandle = IncomingInfo.CallHandle;
			break;

		case TAPI_EVENT_CALL_RELEASE_CNF:
		case TAPI_EVENT_CALL_RELEASE_ALL_CNF:
		case TAPI_EVENT_CALL_RELEASE_ALL_ACTIVE_CNF:
		case TAPI_EVENT_CALL_RELEASE_ALL_HELD_CNF:
		case TAPI_EVENT_CALL_END_IND_IND:
			unsigned int handle;
			memcpy(&handle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_RELEASE_CNF\n");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases2_5 Call Hold
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Hold</h3>
This API puts the specified call on hold. The call identified by Call Handle should be in the active state.

@code
int tel_hold_call (unsigned int CallHandle, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callhold(unsigned int CallHandle)
{
	int ret_status;
	int pRequestID = 0;

	ret_status = tel_hold_call(CallHandle, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
} //WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_HOLD_CNF:
			unsigned int CallHandle;
			memcpy(&CallHandle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_HOLD_CNF\n");
			break;
		//...
	}
	//...
}
@endcode
@}
@defgroup Use_Cases2_6  Call Retrieve
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Retrieve</h3>
This API can be used to retrieve a held call.

Pre Conditions:
@n Call should be in held state in order to return to the active state, and no other call should be active.

@code
int tel_retrieve_call (unsigned int CallHandle, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"


//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callretrieve(unsigned int CallHandle)
{
	int ret_status;
	int pRequestID=0;

	ret_status = tel_retrieve_call(CallHandle, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
} //WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback(TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_HOLD_CNF:
			unsigned int CallHandle;
			memcpy(&CallHandle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_HOLD_CNF\n");
			break;

		case TAPI_EVENT_CALL_RETRIEVE_CNF:
			unsigned int CallHandle;
			memcpy(&CallHandle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_RETRIEVE_CNF ");
			break;
	}
	//...
}
//...
@endcode
@}
@defgroup Use_Cases2_7  Call Swap
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Swap</h3>
This API can be used to swap calls. This feature is only available for the calls that have been setup and answered using the Telephony service. The swap functionality is only available for voice calls.

Pre Conditions:
@n Two calls should be present, one of them in the Active state and the other in the held state

@code
int tel_swap_call (unsigned int CallHandle1, const TS_UINT CallHandle2, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"
//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

void callswap(unsigned int CallHandle1, unsigned int CallHandle2)
{
	int ret_status;
	int pRequestID = 0;

	ret_status = tel_swap_call(CallHandle1, CallHandle2, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//..
} //WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_SWAP_CNF:
			printf("TAPI_EVENT_CALL_RETRIEVE_CNF\n");
			break;
	}
	//..
}
//..
@endcode
@}
@defgroup Use_Cases2_8 Send DTMF
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Send DTMF</h3>
This API can be used for sending one or more DTMF digits during the call.

Pre Conditions:
@n Active call should be present.

@code
int tel_send_call_dtmf (char const * const pDtmfString, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void senddtmf()
{
	int ret_status;
	int pRequestID = 0;
	char DtmfString[10];

	strcpy(DtmfString, "123456789"); //Sample DTMF digits that need to be sent

	ret_status = tel_send_call_dtmf(DtmfString, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE
static void app_callback(TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_SEND_DTMF_CNF:
			printf("TAPI_EVENT_CALL_SEND_DTMF_CNF\n");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases2_9 Call Join
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Join</h3>
This API joins the specified two calls (one call which is in the active state and other call which is in the held state) into a conference call.

Pre Conditions:
@n For a Multiparty call or for joining two calls into conference, there should be one call in active state and another call in held state.

@code
int tel_join_call (unsigned int CallHandle1, const unsigned int CallHandle2,int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void calljoin(unsigned int CallHandle1, unsigned int CallHandle2)
{
	int ret_status;
	int pRequestID = 0;

	ret_status= tel_join_call(CallHandle1, CallHandle2, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
} //WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_SETUPCONFERENCE_CNF:
			unsigned int temp_handle;
			memcpy(&temp_handle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_SETUPCONFERENCE_CNF ");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases2_10 Call Split
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call Split</h3>
This API triggers the split of a single call from multiparty call. This can be used for creating a private communication with one of remote parties in a multiparty session.
@n TAPI client application has to pass the call handle which should be split from the multiparty call.

Pre Conditions:
@n Call should be in multiparty conference call.

Post Conditions:
@n Split call will be the active call and the conference will be held call.

@code
int tel_split_call (unsigned int CallHandle, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callsplit(unsigned int CallHandle)
{
	int ret_status;
	int pRequestID = 0;

	ret_status = tel_split_call(CallHandle, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI TAPI_EVENT_CALL_SPLITCONFERENCE_CNF:
			unsigned int temp_handle;
			memcpy(&temp_handle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_SPLITCONFERENCE_CNF ");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases2_11 Active Line
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Active Line </h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n Telephony provides APIs for setting and getting the current calling line identification number.

@code
int tel_set_call_act_line const TelCallActiveLine_t active_line, int * pRequestID);
int tel_get_call_act_line ( int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SET ACTIVE LINE
void activeline()
{
	int ret_status;
	int pRequestID = 0;
	TelCallActiveLine_t g_curLine = TAPI_CALL_ACTIVE_LINE1;
	TelCallActiveLine_t active_line;

	printf (" Set Active Line with TAPI_ACTIVE_LINE1.");

	ret_status = tel_set_call_act_line (g_curLine, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);

	//GET ACTIVE LINE
	ret_status = tel_get_call_act_line (&pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//..
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_SET_LINEID_CNF:
			//TEST_DEBUG("Received TAPI_EVENT_CALL_SET_LINEID_CNF");
			//AUTO_LOG("EVENT: SET_LINEID_CNF:");
			break;

		case TAPI_EVENT_CALL_GET_LINEID_CNF:
			TelCallActiveLine_t active_line;
			memcpy(&active_line, event->pData, sizeof(TelCallActiveLine_t));
			TEST_DEBUG("Received TAPI_EVENT_CALL_GET_LINEID_CNF");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases2_12  Call transfer
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call transfer</h3>
This API triggers an explicit call transfer by connecting two parties, one of which is in the active state with the other in the held state.

Pre Conditions:
@n In order to call transfer, 2 calls should be present. One in the active state and the other in the Held state.

Post Conditions:
@n When the request has been completed successfully, Call end indication will be sent to both the calls (active and held).

@code
int tel_exe_call_explicit_transfer (unsigned int CallHandle, int * pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE:
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void calltransfer(unsigned int CallHandle)
{
	int ret_status;
	int pRequestID = 0;

	ret_status = tel_exe_call_explicit_transfer (CallHandle, &pRequestID);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	//..
	switch (eventType) //REFER Call Setup TO GET eventType
	{
		case TAPI_EVENT_CALL_TRANSFER_CNF:
			unsigned int temp_handle = 0;
			memcpy(&temp_handle, event->pData, sizeof(unsigned int));
			printf("TAPI_EVENT_CALL_TRANSFER_CNF. ");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases2_13 Get Call Status
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Get Call Status</h3>
When call status is requested by the application, this API gets the status of the call associated with the input Call Handle. Call status information consists of destination number, call direction (MO or MT), call type (voice or data etc), multiparty state (whether the call is in conference state or not) and present call state.
@n Call handle must be valid.

@code
int tel_get_call_status (unsigned int CallHandle, TelCallStatus_t * pCallStatus)
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

void getcallstatus(unsigned int CallHandle)
{
	int ret_status;
	TelCallStatus_t callStatus;

	ret_status = tel_get_call_status(CallHandle, &callStatus);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
		printf("CallNumber: %s\n", callStatus.pNumber);
		printf("Is Mobile Originated:%d n\t CallType : %d \n\t CallActiveState: %d\n\t CallState : %d\n\t isConferenceState:%d \n",
				callStatus.bMoCall,
				callStatus.CallType,
				callStatus.CallActiveState,
				callStatus.CallState,
				callStatus.bConferenceState);
	}
	else {
		printf("error=%d\n", ret_status);
	}
}
@endcode
@}
@defgroup Use_Cases2_14  Get Call Duration
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Get Call Duration</h3>
This API gets the duration of the specified call.
@n This is a synchronous API. Duration is timed from the moment the connection is established, i.e. call goes into active state for first time. The total call duration is provided in seconds, from the call connecting time to the current time.

@code
int tel_get_call_duration unsigned int CallHandle, unsigned int * pDurationInSecs);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

void getcallduration(unsigned int CallHandle)
{
	int ret_status;
	unsigned int duration;

	ret_status = tel_get_call_duration (CallHandle, &duration);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
		printf("Call Duration : %d Seconds\n", duration);
	}
	else {
		printf("error=%d\n", ret_status);
	}
}
@endcode
@}
@defgroup Use_Cases2_15 Get Call Conference List
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Get Call Conference List</h3>
This is a synchronous API which returns information about all the call handles and the total number of call handles, within the specified conference call.

Pre Conditions:
@n Call should be a multi-party conference call in order to return actual number of calls and call handles in the conference call.

@code
int tel_get_call_conf_list(unsigned int  CallHandle,  unsigned int * pCallList, int * pNoOfCalls);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

void getconferencelist(unsigned int CallHandle)
{
	int ret_status ;
	unsigned int callList[5];
	int  noOfCalls;

	ret_status = tel_get_call_conf_list(CallHandle, callList, &noOfCalls);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
		printf("\n No.of Calls %d, \n", noOfCalls);
		for (i = 0; i < noOfCalls; i++)
		{
			printf("\n Call Handle in conference calls %d \n", callList[i]);
		}
	}
	else {
		printf("error=%d\n", ret_status);
	}
}
@endcode
@}
@defgroup Use_Cases2_16 Voice Privacy Mode
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Voice Privacy Mode</h3>
Telephony provides two APIs which can be used to set and get the CDMA voice privacy mode .

@code
int tel_get_call_privacy_mode (TelCallVoicePrivacyInfo_t PrivacyInfo, int *pRequestID);
int tel_set_call_privacy_mode (TelCallPrivacyType_t PrivacyType, int *pRequestID);
@endcode

This API is restricted for direct use.  Use the Voice call engine API instead.

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

//SetVoicePrivacyMode
void setvoiceprivacymode()
{
	int ReqId = -1;
	int ret_status = -1;
	int api_err;
	TelCallPrivacyMode_t PrivacyMode_info;

	pPrivacyMode_info = TAPI_CALL_PRIVACY_MODE_STANDARD;

	ret_status = tel_set_call_privacy_mode (pPrivacyMode_info, &reqId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("Successful\n");
	else
		printf("error:%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED

bool call_app_callback (TelTapiEvent_t *Event)
{
	int ret = -1;
	int eventClass = Event->EventClass;
	int eventType = Event->EventType;
	int RequestId = Event->RequestId;
	int Status = Event->Status;

	if (eventClass == TAPI_EVENT_CLASS_CALL_VOICE)
	{
		switch (eventType) {
			case TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF:
				if (RequestId != reqId) {
					printf("Invalid RequestID received %d \n", reqId);
					return false;
				}

				if (Status != TAPI_CAUSE_SUCCESS) {
					printf("Invalid Status value received");
					return false;
				}
				break;
		}
	}
}

//GetVoicePrivacyMode
void getprivacymode()
{
	Int ReqId = -1;
	int ret_status = -1;
	int api_err;
	TelCallPrivacyType_t privacyType;

	privacyType = TAPI_CALL_PRIVACY_TYPE_MS;

	ret_status = tel_get_call_privacy_mode (privacyType, &req_id);
	if (ret_status == TAPI_API_SUCCESS)
		printf("Successful\n");
	else
		printf("error:%d\n", ret_status);
}

//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED

bool call_app_callback (TelTapiEvent_t *Event)
{
	int ret = -1;
	int eventClass = Event->EventClass;
	int eventType = Event->EventType;
	int RequestId = Event->RequestId;
	int Status = Event->Status;

	if (eventClass == TAPI_EVENT_CLASS_CALL_VOICE)
	{
		switch (eventType) {
			case TAPI_EVENT_CALL_GET_PRIVACYMODE_CNF:
				TelCallVoicePrivacyInfo_t vp_info;
				memcpy(&vp_info, event->pData, sizeof(TelCallVoicePrivacyInfo_t));
				if (Status != TAPI_CAUSE_SUCCESS)
				{
					printf("Invalid Status value received");
					return false;
				}
				break;
		}
	}
}
@endcode
@}
@defgroup Use_Cases2_17 Call FlashInformation
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Call FlashInformation</h3>
The user makes an additional outgoing call in CDMA, this API is used instead of the CALL Outgoing API because there is no additional voice call transaction in CDMA. So the voice call id is only one in CDMA. The multiparty call is not supported in CDMA.

@code
int tel_exe_call_flash_info (const char* pDialNumber, int *pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
//CallFlashInfo
void callflashinfo()
{
	int ret_status = -1;
	int api_err;
	const char *pDialNumber = "9999900000";

	ret_status = tel_exe_call_flash_info(pDialNumber, &reqId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("Successful\n");
	else
		printf("error:%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
bool call_app_callback(TelTapiEvent_t *Event)
{
	int ret = -1;
	int eventClass = Event->EventClass;
	int eventType = Event->EventType;
	int RequestId = Event->RequestId;
	int Status = Event->Status;

	if (eventClass == TAPI_EVENT_CLASS_CALL_VOICE)
	{
		switch (eventType)
		{
			case TAPI_EVENT_CALL_FLASHINFO_CNF:
				if (RequestId != reqId)
				{
					printf("Invalid RequestID received %d \n", reqId);
					return false;
				}

				if (Status != TAPI_CAUSE_SUCCESS)
				{
					printf("Invalid Status value received");
					return false;
				}
				printf("\n Received TAPI_EVENT_CALL_FLASHINFO_CNF \n");
				break;

		}
	}
	return true;
}
@endcode
@}
@defgroup Use_Cases2_18 Getting Call Time
@ingroup Use_Cases2_CALL
@{

		<h3 class="pg">Getting Call Time</h3>
This API is used to query information about a call time and call count.

@code
int tel_get_call_time (const TS_WORD req_mask, int *pRequestID);
@endcode

<strong>This API is restricted for direct use.  Use the Voice call engine API instead.</strong>

SAMPLE CODE
@code
#include <ITapiCall.h>
#include <stdio.h>
#include "TapiCommon.h"

void getcalltime()
{
	int ret_status = -1;
	int api_err;
	TelCallTimeMaskType_t mask_byte1;
	TelCallTimeMaskType_t mask_byte2;
	unsigned short mask_type;

	mask_byte1 = TAPI_CALL_TIME_TYPE_TOTAL_CALL_CNT;
	mask_byte2 = TAPI_CALL_TIME_TYPE_TOTAL_CALL_TIME;

	mask_type = (mask_byte1 | mask_byte2);

	ret_status = tel_get_call_time(mask_type, &reqId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("Successful\n");
	else
		printf("error:%d\n", ret_status);
}
//WAIT FOR EVENT HERE

bool call_app_callback(TelTapiEvent_t *Event)
{
	int ret = -1;
	int eventClass = Event->EventClass;
	int eventType = Event->EventType;
	int RequestId = Event->RequestId;
	int Status = Event->Status;

	if (eventClass == TAPI_EVENT_CLASS_CALL_VOICE)
	{
		switch (eventType)
		{
			case TAPI_EVENT_CALL_SET_PRIVACYMODE_CNF:
				if (RequestId != reqId)
				{
					printf("Invalid RequestID received %d \n", reqId);
					return false;
				}

				if (Status != TAPI_CAUSE_SUCCESS)
				{
					printf("Invalid Status value received");
					return false;
				}
				break;
		}
	}
}
@endcode
@}

<h2 class="pg">SUPPLEMENTARY SERVICE </h2>
@defgroup Use_Cases3_1 Event register and deregister
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{
		<h3 class="pg">Event register and deregister</h3>
To receive asynchronous event notifications for the SS APIs, the registration of related events with their associated call backs is required. After these are registered, when an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

SAMPLE CODE
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
int api_err;

api_err = tel_init();
if (api_err != TAPI_API_SUCCESS)
{
	TEST_DEBUG("tel_init Failed - api_err = %d \n", api_err);
}

api_err = tel_register_event (TAPI_EVENT_SS_WAITING_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("SS Event registration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);

tel_register_app_name("org.tizen.appname");

// DEREGISTER EVENT
api_err = tel_deregister_event(subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Unregisteration Fail\n");
}
@endcode
@}
@defgroup Use_Cases3_2 Call Barring
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{

		<h3 class="pg">Call Barring </h3>
Telephony provides APIs to activate/deactivate call barring, get the barring status and change the barring password. It also provides APIs for applications to provide barring password requested by network for Call Barring Request or Change barring password request.
@n This service makes it possible for a mobile subscriber to activate barring for certain categories of outgoing or incoming calls. It also allows the deactivation of call barring based on categories set.
@n The mobile subscriber may also determine the type of barring preferrred for incoming/outgoing calls, by subscribing to a set of one or more unique barring programs .

@code
int   tel_set_ss_barring (const  TelSsCallBarringInfo_t *pBarringInfo, int * pRequestId);
int   tel_get_ss_barring_status (TelSsCallBarType_t  BarType, TelSsCallType_t  CallType,  int * pRequestId )
int   tel_change_ss_barring_password ( const char*  pOldPassword,const char*  pNewPassword,const char * pNewPasswordAgain, int *  pRequestId);
@endcode

SAMPLE CODE FOR CALL BARRING
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callbarring()
{
	int ret_status = -1;
	TelSsCallBarringInfo_t BarringInfo;
	int RequestId = 0;
	char *bar_pwd = "0000";

	BarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	BarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;
	BarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strcpy(BarringInfo.szPassword, bar_pwd);

	ret_status = tel_set_ss_barring (&BarringInfo, &RequestId);
	if (ret_status == TAPI_API_SUCCESS) {
		printf("successful\n");
	}
	else {
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType)
	{
		case TAPI_EVENT_SS_BARRING_CNF:
			TelSsInfo_t SsInfo;
			memcpy(&SsInfo, (TelSsInfo_t*)event->pData, sizeof(TelSsInfo_t));
			printf("Received Event :TAPI_EVENT_SS_BARRING_CNF\n");
			break;
	}
}
@endcode

SAMPLE CODE FOR BARRING STATUS
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

void callbarringstatus()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	TelSsCallBarType_t BarType = TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL;
	TelSsCallType_t CallType = TAPI_CALL_TYPE_VOICE_EV;
	int pRequestId = 0;

	ret_status = tel_get_ss_barring_status (BarType, CallType, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId, i;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType)
	{
		case TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF:
			printf(" Received Event :TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF \n");
			TelSsInfo_t SsInfo;
			memcpy(&SsInfo, (TelSsInfo_t*)EventData, sizeof(TelSsInfo_t));

			for (i=0; i<SsInfo.NumberOfRecords; i++) {
				printf("Number of records %d \n", SsInfo.NumberOfRecords);
				printf("Record:%d Status:%d Teleservice:%d \n", i,
						SsInfo.SsRecord.BarringRecord.rec_class[i].Status,
						SsInfo.SsRecord.BarringRecord.rec_class[i].TeleCommService);

				if (SsInfo.SsRecord.BarringRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
					printf("  Status: Active \n");
				else
					printf("Status: Not Active \n");
			}

			break;
	}
}
@endcode

SAMPLE CODE FOR CHANGING BARRING PASSWORD
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void changebarpwd()
{
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_change_ss_barring_password ("0000", "0000", "0000", &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}

//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	switch (eventType)
	{
		case TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF:
			printf("Received Event : TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF\n");
			//check data
			//
			printf("Call barring Password Changed successfully\n");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases3_3 Call Forward
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">Call Forward</h3>
These APIs are used to set (activate/deactivate) the call forwarding option at the Network and to get call forward status. Call forwarding allows a user to forward incoming calls to another party. This service permits a called mobile subscriber to have the network send all incoming calls, or just those associated with a specific Basic service group meeting various call FORWARD conditions, to another directory number. The ability of the served mobile subscriber to originate calls is unaffected. If the service is activated, a call is forwarded only if the enabled forward condition is satisfied.
@n Application has to call this API in order to explicitly register/erase/activate/deactivate the call forwarding. Request will be sent irrespective of whether another SS call forward is already ongoing.

@code
int   tel_set_ss_forward ( const  TelSsForwardInfo_t *pForwardInfo, int * pRequestId);
int   tel_get_ss_forward_status (TelSsForwardType_t Type, TelSsForwardWhen_t Condition, int *  pRequestId);
@endcode

SAMPLE CODE FOR CALL FORWARD
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED INEvent Register and Deregister
void callfwd()
{
	int ret_status = 0;
	int pRequestId = 0;
	TelSsForwardInfo_t pForwardInfo;

	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
	pForwardInfo.NoReplyConditionTimer = 10;

	int len;
	char *cf_number = "9986529874"; //number to which calles need to be forwarded.

	len = strlen(cf_number);
	printf("Length of CF number :%d \n", len);
	memcpy(&(pForwardInfo.szPhoneNumber), cf_number, len);

	ret_status = tel_set_ss_forward (&pForwardInfo, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId,i;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//REFER Call Barring TO GET eventType AND EventData
	switch (eventType)
	{
		case TAPI_EVENT_SS_FORWARD_CNF:
			printf("Received Event :TAPI_EVENT_SS_FORWARD_CNF \n");
			TelSsInfo_t SsInfo;

			if (EventData != NULL) {
				memcpy(&SsInfo, (TelSsInfo_t*)EventData, sizeof(TelSsInfo_t));
				printf("Number of records %d\n", SsInfo.NumberOfRecords);
			}

			for (i=0; i<SsInfo.NumberOfRecords; i++)
			{
				printf("Number of records %d\n", SsInfo.NumberOfRecords);
				printf("Record:%d Status:%d Teleservice:%d \n",
						i,
						SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status,
						SsInfo.SsRecord.ForwardingRecord.rec_class[i].TeleCommService);

				if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_REGISTERED)
					printf("Status: Registered \n");
				else if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
					printf("Status: Active \n");
				else
					printf("Status: Not Registered /Not Active\n");
			}
			break;
	}
}
@endcode

SAMPLE CODE FOR CALL FORWARD STATUS
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callfwdstatus()
{
	int ret_status = 0;
	TelSsForwardType_t Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	TelSsForwardWhen_t Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
	int pRequestId = 0;

	ret_status = tel_get_ss_forward_status(Type, Condition, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}

//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId, i;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	//...
	switch (eventType)
	{
		case TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF:
			// REFER Call Barring TO GET eventType AND EventData
			TelSsInfo_t SsInfo;

			if (EventData != NULL)
				memcpy(&SsInfo, (TelSsInfo_t*)EventData, sizeof(TelSsInfo_t));

			for (i=0; i<SsInfo.NumberOfRecords; i++)
			{
				printf("Number of records %d \n",SsInfo.NumberOfRecords);
				printf("Record:%d Status:%d Teleservice:%d \n",
						i,
						SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status,
						SsInfo.SsRecord.ForwardingRecord.rec_class[i].TeleCommService);

				if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_REGISTERED)
					printf(" Status: Registered      \n");
				else if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
					printf("  Status: Active   \n");
				else
					printf("    Status: Not Registered /Not Active     \n");
			}
			break;
	}
}
//...
@endcode
@}
@defgroup Use_Cases3_4 Call Waiting
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">Call Waiting</h3>
These APIs activate/deactivate the call waiting service and provide the status of call wait feature. The call waiting service permits a mobile to be notified of an incoming call (as per basic call procedures) whilst the traffic channel is not available for the incoming call or when the mobile subscriber is engaged in an active or held call. Subsequently, the subscriber can accept, reject, or ignore the incoming call.

@code
int   tel_set_ss_waiting ( const TelSsWaitingInfo_t *waitInfo, int * RequestID);
int   tel_get_ss_waiting_status ( const TelSsCallType_t CallType, int * RequestID);
@endcode

SAMPLE CODE FOR CALL WAITING
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callwaiting()
{
	int ret_status;
	int RequestID;
	TelSsWaitingInfo_t waitInfo;

	waitInfo.Mode = TAPI_SS_CW_ACTIVATE;
	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;

	ret_status = tel_set_ss_waiting (&waitInfo, &RequestID);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId, i;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	switch (eventType)
	{
		case TAPI_EVENT_SS_WAITING_CNF:
			printf("Received Event : TAPI_EVENT_SS_WAITING_CNF  \n");
			//REFER Call Barring TO GET eventType AND EventData

			TelSsInfo_t SsInfo;

			memcpy(&SsInfo, (TelSsInfo_t*)EventData, sizeof(TelSsInfo_t));

			for (i=0; i<SsInfo.NumberOfRecords; i++)
			{
				printf("Number of records %d \n", SsInfo.NumberOfRecords);
				printf("Record:%d Status:%d Teleservice:%d \n",
						i,
						SsInfo.SsRecord.WaitingRecord.rec_class[i].Status,
						SsInfo.SsRecord.WaitingRecord.rec_class[i].TeleCommService);

				if (SsInfo.SsRecord.WaitingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
					printf("Status: Active      \n");
				else
					printf(" Status: Not Active   \n");
			}
			break;
		//...
	}
}
@endcode

SAMPLE CODE FOR CALL WAITING STATUS
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void callwaiting()
{
	int ret_status;
	TelSsCallType_t CallType = TAPI_CALL_TYPE_VOICE_EV;
	int RequestID=0;

	ret_status = tel_get_ss_waiting_status(CallType, &RequestID);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId, i;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	switch (eventType)
	{
		case TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF:
			printf("Received Event :TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF\n");
			//REFER Call Barring TO GET eventType AND EventData

			TelSsInfo_t SsInfo;
			if (EventData != NULL)
				memcpy(&SsInfo, (TelSsInfo_t*)EventData, sizeof(TelSsInfo_t));

			for (i=0; i<SsInfo.NumberOfRecords; i++)
			{
				printf("Number of records %d \n", SsInfo.NumberOfRecords);
				printf("Record:%d Status:%d Teleservice:%d \n",
						i,
						SsInfo.SsRecord.WaitingRecord.rec_class[i].Status,
						SsInfo.SsRecord.WaitingRecord.rec_class[i].TeleCommService);

				if (SsInfo.SsRecord.WaitingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
					printf(" Status: Active      \n");
				else
					printf("Status: Not Active   \n");
			}
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases3_5 CLI Service Status
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">CLI Service Status</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

Telephony provides an API to query the status of calling line identity service. The CLI service permits a mobile to show or hide his identity to the called party.

@code
int   tel_get_ss_cli_status (TelSsCliType_t CliType,int * pRequestId );
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void cliservicestatus()
{
	int ret_status;
	TelSsCliType_t CliType = TAPI_SS_CLI_CDIP;
	int pRequestId;

	ret_status = tel_get_ss_cli_status (CliType, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	Int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	// TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	switch (eventType)
	{
		//REFER Call Barring TO GET eventType AND EventData

		case TAPI_EVENT_SS_IDENTIFY_IND:
			TelSsInfo_t SsInfo;
			memcpy(&SsInfo, (TelSsInfo_t*)EventData, sizeof(TelSsInfo_t));
			printf("Received Event : TAPI_EVENT_SS_IDENTIFY_IND \n");
			printf("Status :%d   CLI Service : %d\n",
					SsInfo.SsRecord.CliRecord.CliStatus,
					SsInfo.SsRecord.CliRecord.IdentificationType);
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases3_6 Send USSD Request
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">Send USSD Request </h3>
This API is used to send a USSD string to the Network. This is an Asynchronous API.

Pre Conditions:
@n No SS and USSD Transaction should be ongoing. If there an ongoing transaction, new USSD request will be returned to the Application with an error (USSD_BUSY)

@code
int  tel_send_ss_ussd_request( const char* szUssdString, int * pRequestId);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void sendussdreq()
{
	char ussdString[TAPI_SS_USSD_DATA_SIZE_MAX];
	int ret;

	memset(ussdString, 0, TAPI_SS_USSD_DATA_SIZE_MAX);
	strcpy(ussdString, "*124#");

	ret = tel_send_ss_ussd_request (ussdString, &request_id);

	ret = tel_send_ss_ussd_request (pAppMsg, &pRequestId);
}
//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//..
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//..

	switch (eventType)
	{
		case TAPI_EVENT_SS_USSD_CNF:
			TelSsUssdMsgIndInfo_t UssdRecord;
			memcpy(&UssdRecord, (TelSsInfo_t*)EventData, sizeof(TelSsUssdMsgIndInfo_t));
			printf("got event : TAPI_EVENT_SS_USSD_CNF:\n");
			//...
			break;
		//..
	}
}
@endcode
@}
@defgroup Use_Cases3_7 Send USSD Response
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">Send USSD Response</h3>
This API is used to send a User response to the Network. This is an Asynchronous API.

Pre Conditions:
No SS and USSD Transaction should be ongoing. If there an ongoing transaction, new USSD request will be returned to the Application with an error (USSD_BUSY)
@code
int  tel_send_ss_ussd_response( const TelSsUssdMsgRspInfo_t* ussdRspRecord, int * RequestID);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void sendussdresp()
{
	char *ussd_str = "*123*0#";
	int ret = 0;
	int pRequestId = 0;

	TelSsUssdMsgRspInfo_t *ussdRspRecord;

	ussdRspRecord = (TelSsUssdMsgRspInfo_t*)malloc(sizeof(TelSsUssdMsgRspInfo_t));
	ussdRspRecord->SsRespType = TAPI_USSD_RES_OK;
	ussdRspRecord->IndType = TAPI_USSD_IND_NOTIFY;
	ussdRspRecord->UssdInfo.Dcs = 0x0f;
	ussdRspRecord->UssdInfo.RequestedBySAT = 0;
	strncpy(ussdRspRecord->UssdInfo.szUssdString, ussd_str, TAPI_SS_USSD_DATA_SIZE_MAX);
	ussdRspRecord->UssdInfo.szUssdString[strlen(ussdRspRecord->UssdInfo.szUssdString)] = '\0';
	ussdRspRecord->UssdInfo.UssdStringLength = strlen(ussd_str);

	ret = tel_send_ss_ussd_response (ussdRspRecord, &pRequestId);
	printf ("The return value is %d\n", ret);
}
//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType)
	{
		case TAPI_EVENT_SS_USSD_CNF:
			TelSsUssdMsgIndInfo_t UssdRecord;
			memcpy(&UssdRecord, (TelSsInfo_t*)EventData, sizeof(TelSsUssdMsgIndInfo_t));
			printf("got event : TAPI_EVENT_SS_USSD_CNF:\n");
			//...
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases3_8 Send USSD Cancel Request
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">Send USSD Cancel Request </h3>
This API sends a USSD cancel request to the Network to release USSD session.

Pre Conditions:
@n SS and USSD Transaction should be ongoing.

@code
int  tel_send_ss_ussd_cancel ( int * pRequestId);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void sendussdcancelreq()
{
	char ussdString[TAPI_SS_USSD_DATA_SIZE_MAX];
	int ret;
	int requestId = 0;

	memset(ussdString, 0, TAPI_SS_USSD_DATA_SIZE_MAX);
	strcpy(ussdString, "*124#");

	ret = tel_send_ss_ussd_request(ussdString, &requestId);
	printf ("The return value is %d\n", ret);

	ret = tel_send_ss_ussd_cancel(&equestId);
	printf ("The return value is %d\n", ret);
}

//WAIT FOR THE EVENTS  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]          \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType)
	{
		case TAPI_EVENT_SS_USSD_CNF:
			printf("got event : TAPI_EVENT_SS_USSD_CNF:\n");
			//...
			break;

		case TAPI_EVENT_SS_USSD_CANCEL_CNF:
			printf("got event : TAPI_EVENT_SS_USSD_CANCEL_CNF:\n");
			//...
			break;

		//...
	}
}
@endcode
@}
@defgroup Use_Cases3_9 AOC Info
@ingroup Use_Cases3_SUPPLEMENTARY_SERVICE
@{


		<h3 class="pg">AOC Info</h3>
These APIs set and read AOC parameter values on the SIM.

Pre Conditions:
@n Advice of charge feature is dependent on the SIM for setting or reading aoc parameters. Setting of AOC requires PIN2 Verification

@code
int  tel_get_ss_aoc_info ( TelSsAocType_t AocType, int * pRequestId);
int  tel_set_ss_aoc_info ( TelCallAocInfo_t * pAocSetInfo, int * pRequestId);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSS.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

//Complete PIN2verification process as mentioned in SIM
void aocinforeq()
{
	//set aoc
	printf(" Resetting Total Cost  \n");

	TelCallAocInfo_t *pAocSetInfo;
	int request_id;
	int ret_status = 0;

	pAocSetInfo = (TelCallAocInfo_t *)calloc(1, sizeof(TelCallAocInfo_t));
	pAocSetInfo->AocType = TAPI_SS_AOC_TYPE_RESET;

	ret = tel_set_ss_aoc_info (pAocSetInfo, &request_id);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}

//WAIT FOR THE EVENT  HERE
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	switch (eventType)
	{
		case TAPI_EVENT_SS_SET_AOC_CNF:
			printf("Received Event : SS -- TAPI_EVENT_SS_SET_AOC_CNF\n");
			break;
	}//...
}

//read aoc
void readaoc()
{
	TelSsAocType_t AocType = TAPI_SS_AOC_TYPE_ACM;

	ret_status = tel_get_ss_aoc_info (AocType, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
}
//WAIT FOR THE EVENT  HERE

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType)
	{
		case TAPI_EVENT_SS_AOC_RSP:
			TelCallAocInfo_t aoc_info;
			memcpy(&aoc_info, (TelCallAocInfo_t*)EventData, sizeof(TelCallAocInfo_t));
			printf("Received Event : SS -- TAPI_EVENT_SS_AOC_RSP\n");
			printf(" AOC TYPE :%d\n", aoc_info.AocType);
			break;
	}

}//...
@endcode
@}

	<h2 class="pg">SMS</h2>

@defgroup Use_Cases4_1 Event register and deregister
@ingroup Use_Cases4_SMS
@{

		<h3 class="pg">Event register and deregister</h3>
To receive asynchronous event notifications for the SMS APIs, the registration of related events with their associated call backs is required. When an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.
@nSMS in the telephony framework only provides an interface to the modem and doesn’t handle Transport layer, storing and deleting the SMS in the storage except SIM. Because of this we recommend you shouldn’t access this API directly, and should use the MAPI of Message Framework instead.

Notification Events are as follows.
@code
TAPI_EVENT_NETTEXT_INCOM_IND
TAPI_EVENT_NETTEXT_CB_INCOM_IND
TAPI_EVENT_NETTEXT_INCOM_EX_IND
TAPI_EVENT_NETTEXT_CB_INCOM_EX_IND
TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND
TAPI_EVENT_NETTEXT_DEVICE_READY_IND
@endcode

SAMPLE CODE
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
int api_err;

api_err = tel_init();
if (api_err != TAPI_API_SUCCESS)
{
	TEST_DEBUG("tel_init Failed - api_err = %d \n", api_err);
}
api_err = tel_register_event(TAPI_EVENT_NETTEXT_SENTSTATUS_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("Nettext Class registration is Done: sub id is %d, api_err is %d\n", subscription_id, api_err);

tel_register_app_name("org.tizen.appname");

// DEREGISTER EVENT
api_err = tel_deregister_event(subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Unregistration Fail\n");
}

// SMS notification callback function
void app_callback(void)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void *EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	printf(" app _callback is called \n");

	print_events((int)eventType);
	printf("\n event came in power_callback [%d] \n", eventType);
	printf("\n expected event is  in callback [%d] \n", expected_event);

	switch (eventType)
	{
		case TAPI_EVENT_NETTEXT_INCOM_IND:
			TelSmsDatapackageInfo_t *ptrDataPackage;
			printf("SMS Incoming Message\n");
			ptrDataPackage = (TelSmsDatapackageInfo_t *)EventData;
			// Decoding Service Center Address
			// Decoding TPDU(SMS-DELIVER, SMS-STATUS)
			break;

		case TAPI_EVENT_NETTEXT_CB_INCOM_IND:
			TelSmsCbMsg_t *ptrCbMsgInfo;
			printf("SMS Cell Broadcasting Incoming Message\n");
			ptrCbMsgInfo = (TelSmsCbMsg_t *)EventData;
			// Decoding Cell broadcasting Message in szData [Refer to 3GPP TS23.041 9.4.1]
			break;

		case TAPI_EVENT_NETTEXT_INCOM_EX_IND:
			TelSmsMsgInfo_t *pMsgInfo;
			printf("CDMA SMS Incoming Message\n");
			pMsgInfo = (TelSmsMsgInfo_t *)EventData;
			// Decoding TPDU(SMS-DELIVER, SMS-STATUS)
			break;

		case TAPI_EVENT_NETTEXT_CB_INCOM_EX_IND:
			TelSmsMsgInfo_t *pMsgInfo = NULL;
			printf("CDMA SMS Incoming Message\n");
			pMsgInfo = (TelSmsMsgInfo_t *)EventData;
			// Decoding Cell broadcasting Message in szData
			break;

		case TAPI_EVENT_NETTEXT_MEMORY_STATUS_IND:
			int *status = NULL;
			printf("SIM Memory Status Notification\n");
			status = (TelSmsMemStatusType *)EventData;
			if (*status == TAPI_NETTEXT_PHONE_MEMORY_STATUS_AVAILABLE)
				printf("SIM Memory Available\n");
			else if (*status == TAPI_NETTEXT_PHONE_MEMORY_STATUS_FULL)
				printf("SIM Memory Full\n");
			else
				printf("Unknown Type\n");
			break;

		case TAPI_EVENT_NETTEXT_DEVICE_READY_IND:
			int *pDeviceReady = NULL;
			printf("SMS Device Ready Status Notification\n");
			pDeviceReady = (int*)EventData;
			if (pDeviceReady->Result == 1)
				printf("Device Ready\n");
			else
				printf("Unknown Type\n");
			break;
	}
	return;
}
@endcode
@}
@defgroup Use_Cases4_2 Send SMS
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Send SMS</h3>
Telephony provides an API to send an SMS to the network. This API allows the transmission of an SMS PDU as defined by 3GPP TS 23.040 (SMS-SUBMIT, SMS-COMMAND) to the lower layers. If there is a need to send multiple parts of a concatenated message, this API requests for a dedicated link, through the input parameter MoreMessageToSend, from the lower layers. This will ensure the lower layers do not release the dedicated connection if there are more messages expected.

@code
int tel_send_sms (const TelSmsDatapackageInfo_t* pDataPackage, int bMoreMsgToSend, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void send_sms()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	TelSmsDatapackageInfo_t *pDataPackage;
	int ret_status;
	int pRequestId = 0;

	pDataPackage = (TelSmsDatapackageInfo_t *)malloc(sizeof(TelSmsDatapackageInfo_t));

	//fill the structure appropriately
	//Sca parameter is optional. If you don't provide Service center address, you should fill "00" in sca array.
	//MsgLength is length of szData//fill the szData in TelSmsDatapackageInfo_t with SMS-SUBMIT or SMS-COMMAND TPDU.

	ret_status = tel_send_sms (pDataPackage, 0, &pRequestId);
	if  (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR THE EVENT  HERE
}

static void app_callback (TelTapiEvent_t* event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType) {
		case TAPI_EVENT_NETTEXT_SENTSTATUS_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ TAPI_EVENT_NETTEXT_SENTSTATUS_CNF $$$\n");
			// Extract the Event Data here

			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases4_3 Send SMS Message
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Send SMS Message</h3>
This API is used to send an SMS message to the network. This API allows the transmission of an SMS PDU as defined by 3GPP2 C.S0015 (SMS-SUBMIT, SMS-ACK, SMS-CANCEL) to the lower layers. If there is a need to send multiple parts of a concatenated message, this API requests for a dedicated link, through the input parameter MoreMessageToSend, from the lower layers. This will ensure the lower layers do not release the dedicated connection if there are more messages expected. This is currently supported in CDMA.

@code
int tel_send_sms_msg (const TelSmsMsgInfo_t* pSmsMsgInfo,unsigned int MoreMsgToSend, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void send_sms_ex()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int requestId = 0;
	TelSmsMsgInfo_t pMsgInfo;
	char* diallingNum;
	int dialNumLen;
	TelSmsMsgInfo_t *pDataPackage = malloc(sizeof(TelSmsMsgInfo_t));

	memset(pDataPackage, 0, sizeof(TelSmsMsgInfo_t));
	pDataPackage->MsgType = TAPI_NETTEXT_MESSAGETYPE_SUBMIT;
	pDataPackage->ParamMask |= TAPI_NETTEXT_PARAM_TELESERVICE_MASK;
	//fill structure appropriately

	ret_status = tel_send_sms_msg (pDataPackage, 0, &requestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
}
//WAIT FOR EVENT HERE
//CALLBACK FUNTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType) {
		case TAPI_EVENT_NETTEXT_SENTSTATUS_EX_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ TAPI_EVENT_NETTEXT_SENTSTATUS_EX_CNF $$$\n");
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_4 Read SMS
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Read SMS</h3>
This API reads a message from SIM card storage. You should not access this API directly; instead use the Message Framework API.

@code
int  tel_read_sms_in_sim(int Index, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void read_sms()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_read_sms_in_sim(0, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("Error  = %d \n", ret_status);
	}
	//WAIT FOR EVENT HERE
}

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_READ_SMS_CNF:
			TelSmsData_t *pReadData;
			printf("\n$$Read Msg Notification $$$\n");
			pReadData = (TelSmsData_t *) event->pData;
			printf("\n$$Read Msg index = [%d] $$$\n", pReadData->SimIndex);
			printf("\n$$Read Msg status = [%d] $$$\n", pReadData->MsgStatus);
			// TPDU
			// pReadData->SmsData.szData

			break;
	}
}
//...
@endcode
@}
@defgroup Use_Cases4_5 Write Sms
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Write Sms</h3>
This API writes a message to SIM storage area.
@n The parameter of this API is changed into TelSmsData_t.
@n If SIM index in TelSmsData_t is -1, SMS message is stored at the first empty SIM index . The contents of szData is SMS-SUBMIT, SMS-DELIVER or SMS-STATUS REPORT TPDU. You should not access this API directly; use the Message Framework API.

@code
int   tel_write_sms_in_sim(const TelSmsData_t *pWriteData, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void write_sms()
{
	// SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	TelSmsData_t *pWriteData;
	int pRequestId = 0;
	char *msg = "This is an Test SMS";
	char *SCA = "821091";

	pWriteData = malloc(sizeof(TelSmsData_t));
	if (pWriteData == NULL) {
		printf("Memory Allocation fails\n");
		return;
	}

	// conv_service_center_address(pWriteData->SmsData.Sca, SCA);
	// conv_msg_into_tpdu(pWriteData->SmsData.szData, pWriteData->SmsData.MsgLength, msg);
	pWrite->MsgStatus = TAPI_NETTEXT_STATUS_READ;
	pWrite-> SimIndex = -1;

	ret_status = tel_write_sms_in_sim (pWriteData, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//..
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//..
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_SAVE_STATUS_CNF:
			int *SimIndex = NULL;
			printf("\n***************************************\n");
			printf("\n$$Save Status Notification$$\n");
			SimIndex = (int *)EventData;
			printf("SIM Index stored SMS is [%d]\n", *SimIndex);
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases4_6 Delete Sms
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Delete Sms</h3>
This API deletes a message specified by the index (SIM index used when storing the sms) number or all the messages in the SIM storage area. If SIM index is -1, all SMS stored in SIM are deleted. You should not access this API directly; use the Message Framework API.

@code
int tel_delete_sms_in_sim(int Index, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void delete_sms()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_delete_sms_in_sim(1, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_DELETE_STATUS_CNF:
			printf("\n***************************************\n");
			printf("\n$$Delete Notification$$$\n");
			break;
	}
	//..
}
@endcode
@}
@defgroup Use_Cases4_7 Getting Sms Count
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Getting Sms Count</h3>
This API is used to retrieve message count information such as Total record count, used record count and the indices list for SIM Storage. You should not access this API directly; use the Message Framework API.

@code
int tel_get_sms_count (int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void get_count()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_get_sms_count (&pRequestId);
	if  (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//..
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_GET_COUNT_CNF:
			printf("\n***************************************\n");
			printf("\n$$$GetMsgCountNotification$$$$\n");
			MsgCountNotification(tapiEvent.pData);
			break;
	}
	//..
}
void MsgCountNotification (char *pData)
{
	static TelSmsStoredMsgCountInfo_t *countInfo;
	int loop_counter = 0;
	int returnStatus;
	countInfo = (TelSmsStoredMsgCountInfo_t *)pData;
	int requestId = 0;

	if (countInfo->UsedCount != 0x00)
	{
		fprintf(stderr, "Index LIST..........\n");
		for (loop_counter  = 0; loop_counter < countInfo->UsedCount; loop_counter++)
		{
			fprintf(stderr, "[%02x]", countInfo->IndexList[loop_counter]);
			returnStatus = tel_read_sms_in_sim (countInfo->IndexList[loop_counter], &requestId);
			printf("After read msg: returnstatus %d request id is %d\n", returnStatus, requestId);
		}
		printf("In MsgCountNotification total cnt is %d, usedcnt is %dapi_err %d\n",
				countInfo->TotalCount,
				countInfo->UsedCount,
				returnStatus);
	}

	printf("In MsgCountNotification total cnt is %d, usedcnt is %d\n", countInfo->TotalCount, countInfo->UsedCount);
}
@endcode
@}
@defgroup Use_Cases4_8 Setting Sms CB message
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Setting Sms CB message</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n This API is used to set the SMS CB Message Identifiers in the appropriate EF-CBMI/EF-CBMIR file in (U)SIM. This API also indicates the underlying provider to enable or disable CB Channel on which CB messages are received. You should not access this API directly; use the Message Framework API.

@code
int  tel_set_sms_cb_config(const TelSmsCbConfig_t *pCBConfig, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void set_cbconfig()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;
	TelSmsCbConfig_t *pCBConfig = NULL;

	pCBConfig = (TelSmsCbConfig_t *)malloc(sizeof(TelSmsCbConfig_t));
	pCBConfig->bCBEnabled = 1;
	pCBConfig->SelectedId = 0x01;
	pCBConfig->MsgIdCount = 0x02;

	pCBConfig->MsgIDs[0] = 0x0c;
	pCBConfig->MsgIDs[1] = 0x0c;
	pCBConfig->MsgIDs[2] = 0xdf;
	pCBConfig->MsgIDs[3] = 0x00;

	ret_status = tel_set_sms_cb_config(pCBConfig, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//..
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TelSmsSetResponse *RequestType;
			printf("\n***************************************\n");
			printf("\n$$$$$$   SET CONFIRMATION $$$$$$ \n");
			RequestType = (TelSmsSetResponse *)EventData;
			if (*RequestType == TAPI_NETTEXT_CBSETCONFIG_RSP) {
				printf("\n$$$$$$   SET CB CONFIRMATION $$$$$$ \n");
			}
			break;
	}
}
@endcode
@}
@defgroup Use_Cases4_9 Getting Sms CB configuration
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Getting Sms CB configuration</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n This API is used to retrieve SMS CB configuration options from EFcbmi file in the SIM. You should not access this API directly; use the Message Framework API.

@code
TapiResult_t    tel_get_sms_cb_config (int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:

@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void get_cbconfig()
{
	int ret_status;
	int pRequestId = 0;

	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	ret_status = tel_get_sms_cb_config (&pRequestId);

	printf ("The return value is %d\n", ret_status);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//..
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//..
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_GET_CB_CONFIG_CNF:
			printf("\n***************************************\n");
			printf("\n$$$GET CB Config$$$\n");
			GetCBNotification(tapiEvent.pData);
			break;
	}
	//..
}

void GetCBNotification (char *pData)
{
	TelSmsCbConfig_t *CBConfig;
	int i = 0;

	CBConfig = (TelSmsCbConfig_t *) pData;
	printf("=========CB Configuration=========\n");

	// CB Enable/Diable
	if (CBConfig->bCBEnabled == 0x01)
		printf("Cell Broadcast Msg Enabled...\n");
	else if (CBConfig->bCBEnabled == 0x02)
		printf("Cell Broadcast Msg Disabled...\n");

	// Selected ID
	if (CBConfig->SelectedId == 0x01)
		printf("All CBMI is selected... \n");
	else if (CBConfig->SelectedId == 0x02)
		printf("There are some selected CBMI...\n");

	// CBMI  Count
	printf("CBMI Count: %d \n\n", CBConfig->MsgIdCount);
	// CBMI  List
	if (CBConfig->MsgIdCount != 0)
	{
		printf("----- CBMI List -----\n");
		for (i=0; i < CBConfig->MsgIdCount; i++)
		{
			printf("No.%d - [0x%04x]\n", i, CBConfig->MsgIDs[i]);
		}
	}
}
//...
@endcode
@}
@defgroup Use_Cases4_10 Service Center Address
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Service Center Address</h3>
This API sets the SMS Service Centre Address information in order to send the SMS.
@n It is also possible to get current SMS Service Centre Address information. Currently Only index 0 is supported in Aquila’s Telephony OEM Plugin. You should not access this API directly; use the Message Framework API.

@code
int  tel_set_sms_sca(const TelSmsAddressInfo_t *pSCA, int Index, int *pRequestId);
int  tel_get_sms_sca (int Index, int *pRequestId)
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void set_sca()
{

	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;
	BYTE scabuf[512];
	char scaNum[TAPI_NETTEXT_SCADDRESS_LEN_MAX];
	BYTE scaNum_len = 0;
	TelSmsAddressInfo_t *sc_addr = NULL;
	BYTE packet[500];
	int idx = 0;
	int i;

	printf("***Setting the SCA(Api:SetSCA)****\n");
	sc_addr = (TelSmsAddressInfo_t *)malloc(sizeof(TelSmsAddressInfo_t));
	memset(sc_addr, 0, sizeof(sc_addr));
	memset(scaNum, 0, sizeof(scaNum));
	memset(scabuf, 0, sizeof(scabuf));
	sprintf(scaNum, "821091");
	scaNum_len = strlen(scaNum);
	scaNum_len = strlen(scaNum);
	scaNum[scaNum_len] = 0;
	scaNum_len = scaNum_len - 1;

	printf("==========================\n");
	printf("Sca Number :%s\n", scaNum);
	printf("sca  number length : %d \n", scaNum_len);
	printf("==========================\n");

	//idx = SmsUtilEncodeAddrField( packet, scaNum, scaNum_len, 0x01, 0x01);
	//get idx
	printf("idx =%d\n", idx);
	sc_addr->DialNumLen = idx ;
	memcpy(sc_addr->szDiallingNum, packet, (unsigned int)idx);

	ret_status = tel_set_sms_sca(sc_addr, 0, &pRequestId);
	//[Note] Telephony Emulator Not Support

	printf("returnstatus after scaddr set is %d requestId:%d \n", ret_status, pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//..
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//..
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TelSmsSetResponse *RequestType;
			printf("\n***************************************\n");
			RequestType = (TelSmsSetResponse *)EventData;
			if (*RequestType == TAPI_NETTEXT_SETSCADDR_RSP) {
				printf("\n$$$TAPI_EVENT_NETTEXT_SET_SCA_CNF$$$\n");
				// service center address decoding
			}
			break;
	}
}
//..

//get SCA
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void get_sca()
{
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_get_sms_sca(0, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}

	//WAIT FOR EVENT HERE
}

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_GET_SCA_CNF:
			printf("\n****************************************\n");
			printf("\n$$$ TAPI_EVENT_NETTEXT_GET_SCA_CNF$$$\n");
			// SCA address decoding
			printf("\n****************************************\n");
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_11 Sms Preferred Bearer
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Sms Preferred Bearer</h3>
<strong>[Note] Telephony Emulator and Aquila’s Telephony OEM Plugin do not support this feature.</strong>
@n Telephony provides APIs to set SMS preferred bearer on which SMS has to be transmitted. You should not access this API directly; use the Message Framework API.

@code
int  tel_set_sms_preferred_bearer (TelSmsBearerType_t    BearerType, int *pRequestId);
int  tel_get_sms_preferred_bearer(int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void set_pb()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;
	TelSmsBearerType_t BearerType = TAPI_NETTEXT_BEARER_PS_ONLY;

	ret_status = tel_set_sms_preferred_bearer(BearerType, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TelSmsSetResponse *RequestType;
			RequestType = (TelSmsSetResponse *)EventData;
			if (*RequestType == TAPI_NETTEXT_SETPREFERREDBEARER_RSP) {
				printf("\n$$$TAPI_EVENT_NETTEXT_SET_Preferred Bearer_CNF$$$\n");
			}
			break;
	}
	//...
}

//get preferred bearer
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>
void get_pb()
{
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_get_sms_preferred_bearer(&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_GET_SMSBEARER_CNF:
			printf("\n****************************************\n");
			printf("\n$$$ GET PREFERRED BEARER$$$\n");
			printf("\n****************************************\n");
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_12 Sms Parameters
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Sms Parameters</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n Telephony provides APIs to set the header parameters of SMS, which are used in the origination of MO messages. It also provides API to get the SMS parameters for a particular SMS (sent/received) based on the SIM index where it is stored. You should not access this API directly; use the Message Framework API.

@code
int  tel_set_sms_parameters(const TelSmsParams_t SmsSetParameters, int *pRequestId);
int  tel_get_sms_parameters(const int SmsRecordIndex, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void set_param()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;
	TelSmsParams_t SmsSetParameters;

	SmsSetParameters.RecordIndex = 0x01;
	SmsSetParameters.RecordLen = 28;
	memcpy(SmsSetParameters.TpDestAddr.szDiallingNum, "9986529874", 10);
	printf("\ndial no is %s\n", SmsSetParameters.TpDestAddr.szDiallingNum);

	SmsSetParameters.TpDestAddr.DialNumLen = 10;
	SmsSetParameters.TpDestAddr.Ton = TAPI_SIM_TON_NATIONAL;//national no
	SmsSetParameters.TpDestAddr.Npi = TAPI_SIM_NPI_NATIONAL;

	ret_status = tel_set_sms_parameters(SmsSetParameters, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TelSmsSetResponse *RequestType;
			RequestType = (TelSmsSetResponse *)EventData;
			if (*RequestType == TAPI_NETTEXT_SETPARAMETERS_RSP) {
				printf("\n$$$TAPI_EVENT_NETTEXT_SET_Parameter_CNF$$$\n");
			}
			break;
	}
}
//...

//get params
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void get_params()
{
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_get_sms_parameters(1, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_GET_PARAM_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ GET PARAM NOTI$$$\n");
			// decoding SMS parameter
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_13 Sending Delivery Report
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Sending Delivery Report</h3>
This API sends a SMS-deliver report to the network, after receiving an incoming SMS. You should not access this API directly.

@code
int  tel_send_sms_deliver_report(const TelSmsDatapackageInfo_t *pDataPackage, TelSmsResponse_t RPCause, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>
void send_delivery()
{
	int ret_status;
	int requestId = -1;
	TelSmsResponse_t RPCause;
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	TelSmsDatapackageInfo_t *del_report = NULL;

	del_report = malloc(sizeof(TelSmsDatapackageInfo_t));
	memset(del_report, 0, sizeof(TelSmsDatapackageInfo_t));
	//Encode Send Delivery Report here
	//EncodeSmsDeliveryReportTpdu();

	RPCause = TAPI_NETTEXT_SENDSMS_SUCCESS;
	printf("***receive message (Api:SendDeliverreport)****\n");

	ret_status = tel_send_sms_deliver_report(del_report, RPCause, &requestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_DELIVERY_REPORT_CNF:
			printf("\n***************************************\n");
			printf("\n$$$ DELIVER REPORT NOTI$$$\n");
			printf("\n***************************************\n");
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_14 Setting Memory Status
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Setting Memory Status</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n This API is used by applications to inform the modem about the memory status of PDA (whether FULL or AVAILABLE) so that modem can inform network. You should not access this API directly.

@code
int  tel_set_sms_memory_status      (int  MemoryStatus ,  int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>
void set_memstatus()
{

	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;
	int MemoryStatus = TAPI_NETTEXT_PDA_MEMORY_STATUS_AVAILABLE;

	ret_status = tel_set_sms_memory_status(MemoryStatus, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TelSmsSetResponse *RequestType;
			printf("\n***************************************\n");
			printf("\n$$$$$$   SET CONFIRMATION $$$$$$ \n");
			RequestType = (TelSmsSetResponse *)EventData;
			if (*RequestType == TAPI_NETTEXT_SETMEMORYSTATUS_RSP) {
				printf("\n$$$$$$   SET Memory Status $$$$$$ \n");
			}
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_15 Setting Message Status
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Setting Message Status</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
This API is used by applications to set the message status in the Sim file. You should not access this API directly; use the Message Framework API.

@code
int    tel_set_sms_message_status(int Index, TelSmsMsgStatus_t MsgStatus, int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void set_msgstatus()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int Index = 2;
	int pRequestId = 0;
	TelSmsMsgStatus_t MsgStatus = TAPI_NETTEXT_STATUS_READ;

	ret_status = tel_set_sms_message_status (Index, MsgStatus, &pRequestId);
	printf ("The return value is %d\n", ret_status);
	printf("RequestId is %d\n", pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType)
	{
		case TAPI_EVENT_NETTEXT_SET_REQUEST_CNF:
			TelSmsSetResponse *RequestType;
			printf("\n***************************************\n");
			printf("\n$$$$$$   SET CONFIRMATION $$$$$$ \n");
			RequestType = (TelSmsSetResponse *)EventData;
			if (*RequestType == TAPI_NETTEXT_SETMESSAGESTATUS_RSP) {
				printf("\n$$$$$$   SET Memory Status $$$$$$ \n");
			}
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases4_16 Getting SMS Parameter Count
@ingroup Use_Cases4_SMS
@{


		<h3 class="pg">Getting SMS Parameter Count</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n This API is used by applications to get the count of sms parameter records stored in Sim EF. You should not access this API directly.

@code
int  tel_get_sms_parameter_count (int *pRequestId);
@endcode

<strong>This API is restricted for direct use.  Use the Message Framework API instead.</strong>

SAMPLE CODE:
@code
#include <stdio.h>
#include <stdlib.h>
#include <TapiCommon.h>
#include <ITapiNetText.h>

void get_paramcount()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_get_sms_parameter_count (&pRequestId);
	printf ("The return value is %d\n", ret_status);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("successful\n");
	}
	else
	{
		printf("error = %d\n", ret_status);
	}
	//WAIT FOR EVENT HERE
}
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Send SMS TO GET eventType
	{
		case TAPI_EVENT_NETTEXT_PARAM_COUNT_IND:
			printf("\n***************************************\n");
			printf("\n$$$ PARAM COUNT NOTI$$$\n");
			printf("\n***************************************\n");
			break;
	}
	//...
}
@endcode
@}
	<h2 class="pg">NETWORK Registration and Configuration<h/h2>

@defgroup Use_Cases5_1 Event register and deregister
@ingroup Use_Cases5_NETWORK
@{

		<h3 class="pg">Event register and deregister</h3>
To receive asynchronous event notifications for the network APIs, the registration of related events with their associated call backs is required. When an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

Network Notification Events
@code
TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND
TAPI_EVENT_NETWORK_RADIO_INFO_IND
TAPI_EVENT_NETWORK_SPN_INFO
TAPI_EVENT_NETWORK_DISPLAYINFO_IND
@endcode

SAMPLE CODE:
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
int api_err;

api_err = tel_init();
if (api_err != TAPI_API_SUCCESS)
{
	TEST_DEBUG("TelTapiInit Failed - api_err = %d \n", api_err);
}

api_err = tel_register_event(TAPI_EVENT_NETWORK_GETNWBAND_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("Network Event registration is Done: sub id is %d, api_err is %d\n", subscription_id, api_err);

tel_register_app_name("org.tizen.appname");

// DEREGISTER EVENT
api_err = tel_deregister_event (subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Unregeration Fail\n");
}

// Network notification callback function
void app_callback(void)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void *EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;
	printf(" app _callback is called \n");

	print_events((int)eventType);
	printf("\n event came in power_callback [%d] \n", eventType);
	printf("\n expected event is  in callback [%d] \n", expected_event);

	switch (eventType)
	{
		case TAPI_EVENT_NETWORK_LOCATIONCELLINFO_IND:
			TelSmsDatapackageInfo_t *ptrDataPackage = NULL;
			printf("SMS Incoming Message\n");
			ptrDataPackage = (TelSmsDatapackageInfo_t *)EventData;
			// Decoding Service Center Address
			// Decoding TPDU(SMS-DELIVER, SMS-STATUS)
			break;

		case TAPI_EVENT_NETWORK_RADIO_INFO_IND:
			TelSmsDatapackageInfo_t *ptrDataPackage = NULL;
			printf("SMS Incoming Message\n");
			ptrDataPackage = (TelSmsDatapackageInfo_t *)EventData;
			// Decoding Service Center Address
			// Decoding TPDU(SMS-DELIVER, SMS-STATUS)
			break;

		case TAPI_EVENT_NETWORK_SPN_INFO:
			TelSmsDatapackageInfo_t *ptrDataPackage = NULL;
			printf("SMS Incoming Message\n");
			ptrDataPackage = (TelSmsDatapackageInfo_t *)EventData;
			// Decoding Service Center Address
			// Decoding TPDU(SMS-DELIVER, SMS-STATUS)
			break;

		case TAPI_EVENT_NETWORK_DISPLAYINFO_IND:
			TelSmsDatapackageInfo_t *ptrDataPackage = NULL;
			printf("SMS Incoming Message\n");
			ptrDataPackage = (TelSmsDatapackageInfo_t *)EventData;
			// Decoding Service Center Address
			// Decoding TPDU(SMS-DELIVER, SMS-STATUS)
			break;
	}
}
@endcode
@}
@defgroup Use_Cases5_2 Network Selection
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Network Selection</h3>
Telephony provides APIs which can request the lower layers use the network (PLMN) which has been selected by the user from the Network List displayed to the User.

@code
int tel_select_network_automatic(int *pRequestId)
int tel_select_network_manual(unsigned int Plmn, int *pRequestId)
@endcode

SAMPLE CODE:
@code
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"

void select_nw()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;
	int pRequestID;
	unsigned int plmn = 0;

	//CASE 1. MANUAL MODE
	plmn = 45454;
	ret_status = tel_select_network_manual(Plmn, &pRequestId);

	// CASE 2. AUTOMATIC MODE
	// selectionMode = TAPI_NETWORK_SELECTIONMODE_AUTOMATIC;
	ret_status = tel_select_network_automatic(&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);

	//WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType)
	{
		case TAPI_EVENT_NETWORK_SELECT_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SELECT_CNF $$$$$$ \n");
	}
}
@endcode
@}
@defgroup Use_Cases5_3 Network Search
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Network Search</h3>
This API is used to search for the available networks, and to provide the Network List to the User/Application.

@code
int     tel_search_network (int *pRequestId);
@endcode

SAMPLE CODE:
@code
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"

void searchnw()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;

	ret_status = tel_search_network(&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType)   //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SEARCH_CNF:
			int ui_entry_count = 0;
			TelNetworkPlmnList_t *pNetSearchCnf;
			pNetSearchCnf = (TelNetworkPlmnList_t *)EventData;
			while (ui_entry_count <  pNetSearchCnf->networks_count)
			{
				printf("Type_of_plmn[%d] 0:unkwon,1:home,2:available,3:forbidden, NetworkName[%s]",
						pNetSearchCnf->network_list[ui_entry_count].type_of_plmn,
						pNetSearchCnf->network_list[ui_entry_count].network_name);
				printf("ServiceProviderName[%s]", pNetSearchCnf->network_list[ui_entry_count].service_provider_name);
				printf("PLMN ID[%lu]", pNetSearchCnf->network_list[ui_entry_count].plmn_id);
				ui_entry_count++;
			}
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases5_4 Selection Mode
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Selection Mode</h3>
Telephony provides APIs to set and retrieve the current mode of network selection i.e. automatic or manual. Request for Automatic network selection or Manual network selection will allow OEM provider to do PLMN Selection based on network selection preference.

@code
int tel_get_network_selection_mode (int *pRequestId);
@endcode

SAMPLE CODE
@code
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void get_selmode()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;
	int pRequestId;
	//GET THE SELECTION MODE

	ret_status = tel_get_network_selection_mode (&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) // REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETSELECTIONMODE_CNF $$$$$$ \n");
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases5_5 Service Domain
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Service Domain</h3>
Telephony provides APIs so that User/application can configure the service domain and also retrieve the present network service domain.

@code
int tel_set_network_service_domain (TelNetworkServiceDomain_t ServiceDomain, int *pRequestId);
int tel_get_network_service_domain (int *pRequestId);
@endcode

SAMPLE CODE
@code
#include <stdio.h>
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"

void set_servicedomain()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;
	int pRequestId;
	//SET NETWORK SERVICE DOMAIN
	TelNetworkServiceDomain_t ServiceDomain = TAPI_NETWORK_SERVICE_DOMAIN_AUTOMATIC;

	ret_status = tel_set_network_service_domain (ServiceDomain, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	unsigned int temp_handle = -1;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SET_SVC_DOMAIN_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SET_SVC_DOMAIN_CNF $$$$$$ \n");
			break;
	}

	//...
}

//GET NETWORK SERVICE DOMAIN
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void get_servicedomain()
{
	int ret_status;
	ret_status = tel_get_network_service_domain (&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GET_SVC_DOMAIN_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GET_SVC_DOMAIN_CNF $$$$$$ \n");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases5_6 Network Mode
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Network Mode</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n The Network Mode (Automatic, GSM, and WCDMA) can be configured at OEM provider, based on which lower layers can scan only those radio access technologies, to register with network.

@code
int tel_set_network_mode (TelNetworkMode_t NwMode, int *pRequestId);
int     tel_get_network_mode (int *pRequestId);
@endcode

SAMPLE CODE
@code
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void setnwmode()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;
	int pRequestId;

	ret_status = tel_set_network_mode(NwMode, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SETNWMODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SETNWMODE_CNF $$$$$$ \n");
			break;
			//...
	}
}

//GET NETWORK MODE
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"

void getnwmode()
{
	int ret_status;
	int pRequestId;

	ret_status = tel_get_network_mode(&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GETNWMODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GETNWMODE_CNF $$$$$$ \n");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases5_7 Network Band
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Network Band</h3>
Telephony provides APIs to set and retrieve the network band and allows the underlying OEM provider to scan the set band.

@code
int tel_set_network_band (TelNetworkBandPreferred_t BandMode, TelNetworkBand_t Band, int *pRequestId);
int tel_get_network_band ( int *pRequestId);
@endcode

SAMPLE CODE:
@code
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void setnwband()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;
	int pRequestId;
	TelNetworkBandPreferred_t BandMode = TAPI_NETWORK_BAND_MODE_PREFERRED;
	TelNetworkBand_t Band = TAPI_NETWORK_BAND_TYPE_GSM_900_1800;
	int pRequestId = 0;

	ret_status = tel_set_network_band (BandMode,Band, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SETNWBAND_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SETNWBAND_CNF $$$$$$ \n");
			break;
			//...
	}
}

//GET NETWORK BAND
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void getnwband()
{
	int ret_status;
	int pRequestId;
	ret_status = tel_get_network_band (BandMode, Band, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) // REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GETNWBAND_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_GTNWBAND_CNF $$$$$$ \n");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases5_8 Preferred PLMN
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Preferred PLMN</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
@n Telephony provides APIs which can be used to set and retrieve the preferred plmn of network.

@code
int tel_set_network_preferred_plmn (TelNetworkPrefferedPlmnOp_t Operation, TelNetworkPrefferedPlmnInfo_t  PreffPlmnInfo, int *pRequestId);
int tel_get_network_preferred_plmn ( int *pRequestId);
@endcode

SAMPLE CODE
@code
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void set_preferredplmn()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister
	int ret_status;
	int pRequestId;

	//SET PLMN
	TelNetworkPrefferedPlmnOp_t Operation = TAPI_NETWORK_PREF_PLMN_ADD;
	TelNetworkPrefferedPlmnInfo_t PreffPlmnInfo;
	unsigned char *plmn = "45454";

	memset(&PreffPlmnInfo, 0, sizeof(TelNetworkPrefferedPlmnInfo_t));
	PreffPlmnInfo.Index = 0;
	PreffPlmnInfo.SystemType = TAPI_NETWORK_SYSTEM_GSM;
	memcpy(PreffPlmnInfo.Plmn, plmn, 6);

	ret_status = tel_set_network_preferred_plmn (Operation,PreffPlmnInfo, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}

	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SETPREFFPLMN_CNF:
			printf("\n$$$$$$ TAPI TAPI_EVENT_NETWORK_SETPREFFPLMN_CNF $$$$$$ \n");
			break;
	}
	//...
}
@endcode

@code
//GET PLMN
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void get_preferredplmn()
{
	int ret_status;
	int pRequestId;

	ret_status = tel_get_network_preferred_plmn (&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status); //WAIT FOR EVENT HERE
	//WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GETPREFFPLMN_CNF:
			printf("\n$$$$$$ TAPI TAPI_EVENT_NETWORK_GETPREFFPLMN_CNF $$$$$$ \n");
			break;
	}
@endcode
@}
@defgroup Use_Cases5_9 Network Roaming Mode
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Network Roaming Mode</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
Telephony provides APIs which can be used to set and retrieve the mode of network roaming. This is for CDMA.

@code
int tel_set_network_roaming (TelNetworkRoamingMode_t *RoamingMode, int *pRequestId);
int tel_get_network_roaming (int *pRequestId);
@endcode

SAMPLE CODE:
@code
//SetRoamingMode
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void set_roamingmode()
{
	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister

	TelNetworkRoamingMode_t roamingMode;
	int ret_status;

	roamingMode.pref_net_type = TAPI_NETWORK_PREF_NET_HOME_ONLY;
	roamingMode.prl_pref_only = TAPI_NETWORK_PRL_PREF_ONLY_ON;

	ret_status = tel_get_network_roaming (&roamingMode, &requestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("Successful\n");
	else
		printf("error:%d\n", ret_status);

	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF:
			printf("$$$$$TAPI_EVENT_NETWORK_SET_ROAMING_MODE_CNF$$$$$\n");
			//....
			break;
	}
	//...
}
@endcode

@code
//GetRoamingMode
#include<stdio.h>
#include<TapiCommon.h>
#include<ITapiNetwork.h>
void set_roamingmode()
{
	int ret_status;

	ret_status = tel_get_network_roaming(&requestId);
	if (ret_status == TAPI_API_SUCCESS)
		("Successful\n");
	else
		printf("error:%d\n", ret_status);

	//WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF:
			printf("$$$$TAPI_EVENT_NETWORK_GET_ROAMING_MODE_CNF$$$$\n");
			//...
			break;
	}
	//...
}
@endcode
@}
@defgroup Use_Cases5_10 Network CDMA Hybrid Mode
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Network CDMA Hybrid Mode</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
Telephony provides APIs which can be used to set and get the CDMA Hybrid Mode. This is for CDMA.
Hybrid means 1x only, EVDO only combined. we can set network mode separately also in CDMA network.

@code
int tel_set_network_hybrid_in_cdma (TelNetworkCDMAHybridMode_t CDMAHybridMode, int *pRequestId);
int tel_get_network_hybrid_in_cdma (int *pRequestId);
@endcode

SAMPLE CODE:
@code
//Set CDMA Hybrid Mode
#include <ITapiNetwork.h>
#include <stdio.h>
#include "TapiCommon.h"
void set_hybridmode()
{
	int ret_status;

	//SUBSCRIBE FOR EVENTS AS MENTIONED IN  Event Register and Deregister

	TelNetworkCDMAHybridMode_t cdmaHybridMode;
	cdmaHybridMode = TAPI_NETWORK_CDMA_HYBRID; // TAPI_NETWORK_CDMA_1X_ONLY, TAPI_NETWORK_CDMA_EVDO_ONLY, TAPI_NETWORK_CDMA_HYBRID(1X + EVDO)

	ret_status = tel_get_network_hybrid_in_cdma(cdmaHybridMode, &requestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("Successful\n");
	else
		printf("error:%d\n", ret_status);

	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF:
			printf("\n$$$$$$ TAPI_EVENT_NETWORK_SET_CDMA_HYBRID_MODE_CNF $$$$$$ \n");
			break;
			//...
	}
}
@endcode

@code
//Get CDMA Hybrid Mode
#include<stdio.h>
#include<TapiCommon.h>
#include<ITapiNetwork.h>
void get_hybridmode()
{
	int ret_status;

	ret_status = tel_get_network_hybrid_in_cdma (&requestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf(successful\n);
	else
		printf(error=%d\n, ret_status);
	//WAIT FOR EVENT HERE
}

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	//...

	eventClass = event->EventClass;

	eventType = event->EventType;

	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	//...
	//...
	switch (eventType) //REFER Network Selection TO GET eventType
	{
		case TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF:
			printf("$$$$$TAPI_EVENT_NETWORK_GET_CDMA_HYBRID_MODE_CNF$$$$\n");
			//...
			break;
	}
}
@endcode
@}
@defgroup Use_Cases5_11 SIM
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">SIM</h3>
Event Register and Deregister
To receive asynchronous event notifications for the SIM APIs, the registration of related events with their associated call backs is required. When an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

SAMPLE CODE:
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
TapiResult_t api_err;

api_err = TelTapiInit();
if (api_err != TAPI_API_SUCCESS)
{
	TEST_DEBUG("TelTapiInit Failed - api_err = %d \n", api_err);
}

api_err = TelTapiRegisterEvent (TAPI_EVENT_SIM_ENABLE_PERS_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("VSIM Event registration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);


// DEREGISTER EVENT
api_err = TelTapiDeRegister(subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Unregeration Fail\n");

}
@endcode
@}
@defgroup Use_Cases5_12 PIN, SIM Lock operations
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">PIN, SIM Lock operations</h3>
Telephony provides APIs to enable/disable pin, change pin, unblock pin, pin verification and sim lock enable/disable/verification. It is also possible to retrieve the pin/sim lock status.

@code
int tel_enable_sim_security(TelSimSecPw_t *sec_data, int *req_id);
int tel_disable_sim_security(TelSimSecPw_t *sec_data, int *req_id);
int  tel_change_sim_pins(const TelSimSecPw_t *old_pin , const TelSimSecPw_t *new_pin, int *req_id);
int tel_verify_sim_puks(const TelSimSecPw_t *puk_data, const TelSimSecPw_t *new_pin_data, int *req_id)
int  tel_verifiy_sim_pins(const TelSimSecPw_t *pin_data, int *req_id)
int  tel_get_sim_security_status(TelSimPinType_t type, TelSimPinStatus_t *status)
@endcode

SAMPLE CODE: PIN1/PIN2/SIM LOCK/PUK1 VERIFICATION AND CHANGE PIN1
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//  OPERATION REQUEST PIN1 VERIFICATION PART
#define PIN1_NORMAL "0000" //Sample PIN1 code

Void pin1_verify()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH + 1;
	char init_pin_val[length] ;
	TelSimSecPw_t pin_data;

	strcpy(init_pin_val, PIN1_NORMAL);

	pin_data.type = TAPI_SIM_PTYPE_PIN1;
	pin_data.pw_len = strlen(init_pin_val);

	printf("pw_len[%d]", pin_data.pw_len);

	pin_data.pw = (char*)malloc(length);
	memcpy(pin_data.pw, init_pin_val, length);

	err_code = tel_verifiy_sim_pins(&pin_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT FOR EVENT FROM HERE
}

//  OPERATION REQUEST PIN2 VERIFICATION PART
#define PIN2_NORMAL "0000" //Sample PIN2 code
Void verify_pin2()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length] ;
	TelSimSecPw_t pin_data;

	strcpy(init_pin_val, PIN2_NORMAL);

	pin_data.type = TAPI_SIM_PTYPE_PIN2;
	pin_data.pw_len = strlen(init_pin_val);

	printf("pw_len[%d]", pin_data.pw_len);

	pin_data.pw = (char*)malloc(length);
	memcpy(pin_data.pw, init_pin_val, length);

	err_code = tel_verifiy_sim_pins(&pin_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT FOR EVENT FROM HERE
}

//  OPERATION REQUEST CHANGE PIN1 PART
#define PIN1_ORG "0000" //Sample old PIN1 code
#define PIN1_NEW "1111" //Sample new PIN1 code
void change_pins()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_old_pin_val[length] ;
	char init_new_pin_val[length];
	TelSimSecPw_t old_pin;
	TelSimSecPw_t new_pin;

	memset(&init_old_pin_val, 0, length);
	memset(&init_new_pin_val, 0, length);

	strcpy(init_old_pin_val, PIN1_ORG);

	old_pin.type = TAPI_SIM_PTYPE_PIN1;
	old_pin.pw_len = strlen(init_old_pin_val);
	printf("pw_len[%d]", old_pin.pw_len);
	old_pin.pw = (char*)malloc(length);
	memcpy(old_pin.pw, init_old_pin_val, length);

	strcpy(init_new_pin_val, PIN1_NEW);

	new_pin.type = TAPI_SIM_PTYPE_PIN1;
	new_pin.pw_len = strlen(init_new_pin_val);
	printf("pw_len[%d]", new_pin.pw_len);
	new_pin.pw = (char*)malloc(length);
	memcpy(new_pin.pw, init_new_pin_val, length);

	err_code = tel_change_sim_pins(&old_pin, &new_pin, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  OPERATION REQUEST VERIFICATION PUK1 PART
#define PUK1_NORMAL "00000000" //Sample PUK1 code
#define PIN1_NORMAL "0000" //Sample PIN1 code

void verify_puks()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH + 1;

	char init_pin_val[length];
	char init_puk_val[length];

	TelSimSecPw_t puk_data;
	TelSimSecPw_t new_pin_data;

	strcpy(init_pin_val, PIN1_NORMAL);
	strcpy(init_puk_val, PUK1_NORMAL);

	puk_data.type = TAPI_SIM_PTYPE_PUK1;   // 0x00
	puk_data.pw_len = length;
	puk_data.pw_len = strlen(init_puk_val);
	printf("pw_len[%d]", puk_data.pw_len);
	memcpy(puk_data.pw, init_pin_val, length);

	new_pin_data.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
	new_pin_data.pw_len = strlen(init_pin_val);
	printf("pw_len[%d]", new_pin_data.pw_len);
	new_pin_data.pw = (char*)malloc(length);
	memcpy(new_pin_data.pw, init_pin_val, length);

	err_code = tel_verify_sim_puks(&puk_data, &new_pin_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}
//  OPERATION REQUEST SIM LOCK VERIFICATION PART
#define SIM_NORMAL "0000" //Sample SIM Lock code
Void verify_sim()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH + 1;
	char init_pin_val[length] ;
	TelSimSecPw_t pin_data;

	strcpy(init_pin_val, SIM_NORMAL);

	pin_data.type = TAPI_SIM_PTYPE_SIM;
	pin_data.pw_len = strlen(init_pin_val);

	printf("pw_len[%d]", pin_data.pw_len);

	pin_data.pw = (char*)malloc(length);
	memcpy(pin_data.pw, init_pin_val, length);

	err_code = tel_verifiy_sim_pins(&pin_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT FOR EVENT FROM HERE
}

//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED

static void app_callback(TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*)event;

	unsigned int temp_handle = -1;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	printf("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	switch (EventType)
	{
		case TAPI_EVENT_SIM_VERIFY_SEC_CNF:
			printf("********TAPI_EVENT_SIM_VERIFY_SEC_CNF**********");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*) EventData;

			if (status == TAPI_SIM_OPERATION_TIMEOUT )
			{
				printf("TAPI SIM Operation Timeout!!");
			}
			else if (sim_event->Status == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("PIN1 Verification Success!");
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM)
				{
					printf("SIM Lock Verification Success!");
				}
				else
				{
					printf("PIN2 Vefication Success!");
				}
			}
			else
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("PIN1 Verification Failed! - PIN Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1)
				{
					printf("PIN1 Verification Failed! - PUK Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("PIN2 Verification Failed! - PIN2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
				{
					printf("PIN2 Verification Failed! - PUK2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM)
				{
					printf("SIM Lock Verification Failed! - SIM Lock code Required");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
			}
			printf("********************************************");
			break;

		case TAPI_EVENT_SIM_VERIFY_PUK_CNF:
			printf("**********TAPI_EVENT_SIM_VERIFY_PUK_CNF********");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*)EventData;

			if (status == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Unblock PIN1 Success!");
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("Unblock PIN2 Success!");
				}
			}
			else
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1)
				{
					printf("PIN1 Verification Failed! - PUK Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
				{
					printf("PIN2 Verification Failed! - PUK2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
			}
			printf("********************************************");
			break;

		case TAPI_EVENT_SIM_CHANGE_PINS_CNF:
			printf("*********TAPI_EVENT_SIM_CHANGE_PINS_CNF*******************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*)EventData;

			printf("PinType[%d]", pPinInfo->type);

			if (status == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Change PIN1 Success!");
				}
				else
				{
					printf("Change PIN2 Success!");
				}

			}
			else
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Change PIN1 Failed! - PIN Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1)
				{
					printf("Change PIN1 Failed! - PUK Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("Change PIN2 Failed! - PIN2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
				{
					printf("Change PIN2 Failed! - PUK2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
			}
			printf("********************************************");
			break;
	}
	//...
}
@endcode

SAMPLE CODE: ENABLE/DISABLE PIN1 and SIM LOCK
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//  OPERATION REQUEST DISABLING PIN1 PART
#define PIN1_NORMAL "0000" //Sample PIN1 code
void disable_pin1()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length];

	strcpy(init_pin_val, PIN1_NORMAL);

	TelSimSecPw_t sec_data;
	sec_data.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
	sec_data.pw_len = strlen(init_pin_val);
	printf("pw_len[%d]", sec_data.pw_len);

	sec_data.pw = (char*)malloc(length);
	memcpy(sec_data.pw, init_pin_val, length);

	err_code = tel_disable_sim_security(&sec_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  OPERATION REQUEST ENABLING PIN1 PART
#define PIN1_NORMAL "0000" //Sample PIN1 code
void enable_pin1()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length];

	strcpy(init_pin_val, PIN1_NORMAL);

	TelSimSecPw_t sec_data;
	sec_data.type = TAPI_SIM_PTYPE_PIN1;   // 0x00
	sec_data.pw_len = strlen(init_pin_val);
	printf("pw_len[%d]", sec_data.pw_len);

	sec_data.pw = (char*)malloc(length);
	memcpy(sec_data.pw, init_pin_val, length);

	err_code = tel_enable_sim_security(&sec_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	//WAIT EVENT RESPONSE FROM HERE
}

//  OPERATION REQUEST DISABLING SIM LOCK PART
#define SIM_NORMAL "0000" //Sample SIM LOCK code
void disable_sim()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length];

	strcpy(init_pin_val, SIM_NORMAL);

	TelSimSecPw_t sec_data;
	sec_data.type = TAPI_SIM_PTYPE_SIM;   // 0x00
	sec_data.pw_len = strlen(init_pin_val);
	printf("pw_len[%d]", sec_data.pw_len);

	sec_data.pw = (char*)malloc(length);
	memcpy(sec_data.pw, init_pin_val, length);

	err_code = tel_disable_sim_security(&sec_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	//WAIT EVENT RESPONSE FROM HERE
}


//  OPERATION REQUEST ENABLING SIM LOCK PART
#define SIM_NORMAL "0000" //Sample SIM LOCK code
void enable_sim()
{
	int request_id = 0;
	int err_code;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length];

	strcpy(init_pin_val, SIM_NORMAL);

	TelSimSecPw_t sec_data;
	sec_data.type = TAPI_SIM_PTYPE_SIM;   // 0x00
	sec_data.pw_len = strlen(init_pin_val);
	printf("pw_len[%d]", sec_data.pw_len);

	sec_data.pw = (char*)malloc(length);
	memcpy(sec_data.pw, init_pin_val, length);

	err_code = tel_enable_sim_security(&sec_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	//WAIT EVENT RESPONSE FROM HERE
}


//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback(TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

	unsigned int temp_handle = -1;

	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	printf("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );

	switch (EventType)
	{
		case TAPI_EVENT_SIM_DISABLE_SEC_CNF:
			printf("****************TAPI_EVENT_SIM_DISABLE_SEC_CNF****************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*)EventData;

			if (status == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Disable PIN1 Success!");
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("Disable PIN2 Success!");
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM)
				{
					printf("Disable SIM LOCK Success!");
				}
			}
			else
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Disable PIN1 Failed! - PIN1Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("Disable PIN2 Failed! - PIN2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1)
				{
					printf("Disable PIN1 Failed! - PUK1 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
				{
					printf("Disable PIN2 Failed! - PUK2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM)
				{
					printf("Disable SIM LOCK Failed! - SIM LOCK CODE Required ");
				}
			}
			printf("********************************************");
			break;

		case TAPI_EVENT_SIM_ENABLE_SEC_CNF:
			printf("*****************TAPI_EVENT_SIM_ENABLE_SEC_CNF****************");

			TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*)EventData;

			if (status == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Enable PIN1 Success!");
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("Enable PIN2 Success!");
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM)
				{
					printf("Enable SIM LOCK Success!");
				}
			}
			else
			{
				if (pPinInfo->type == TAPI_SIM_PTYPE_PIN1)
				{
					printf("Enable PIN1 Failed! - PIN1Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
				{
					printf("Enable PIN2 Failed! - PIN2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK1)
				{
					printf("Enable PIN1 Failed! - PUK1 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}
				else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
				{
					printf("Enable PIN2 Failed! - PUK2 Required ");
					printf("Remainint attempts [%d]", pPinInfo->retry_count);
				}

				else if (pPinInfo->type == TAPI_SIM_PTYPE_SIM)
				{
					printf("Enable SIM LOCK Failed! - SIM LOCK CODE Required ");
				}
			}
			printf("********************************************");
			break;
	}
	//...
}
@endcode

SAMPLE CODE: GET PIN1/PIN2/SIM LOCK STATUS
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//  OPERATION REQUEST GET PIN1/PIN2/SIM LOCK STATUS PART
void get_security_status()
{
	int error_code;
	TelSimPinType_t type = TAPI_SIM_PTYPE_PIN1 // or TAPI_SIM_PTYPE_PIN2 or TAPI_SIM_PTYPE_SIM;
	TelSimPinStatus_t status = -1;
	printf("Get Security status");

	err_code = tel_get_sim_security_status(type, &status);
	if (err_code == TAPI_API_SUCCESS)
	{
		printf(" *****************************************************");
		switch (status)
		{
			case TAPI_SIM_PIN_STATUS_DISABLED:
				printf("TAPI_SIM_PIN_STATUS_DISABLED ");
				break;

			case TAPI_SIM_PIN_STATUS_ENABLED:
				printf("TAPI_SIM_PIN_STATUS_ENABLED ");
				break;

			case TAPI_SIM_PIN_STATUS_BLOCKED:
				printf("TAPI_SIM_PIN_STATUS_BLOCKED ");
				break;

			case TAPI_SIM_PIN_STATUS_PUK_BLOCKED:
				printf("TAPI_SIM_PIN_STATUS_PUK_BLOCKED ");
				break;

			case TAPI_SIM_PIN_STATUS_UNKNOWN:
				printf("TAPI_SIM_PIN_STATUS_UNKNOWN ");
				break;

			default:
				printf(" Default case statment: sim_status(%d)", status);
				break;
		}
	}
	else
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	printf("*****************************************************");
}
@endcode
@}
@defgroup Use_Cases5_13 Personalisation Lock
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Personalisation Lock</h3>
These APIs are used to enable/disable and get the status information about personalisation of network, network subset, corporate, service provider personalisation.

@code
int tel_enable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id);
int tel_disable_sim_personalization(TelSimPersPw_t *pers_data, int *req_id);
int tel_get_sim_personalization_status(TelSimPersType_t type, int *req_id);
@endcode

SAMPLE CODE: GET PERSONALIZATION STATUS
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister
//  OPERATION REQUEST GET NETWORK PERSONALIZATION PART
void get_net_pers()
{
	int request_id = 0;
	int err_code;

	TelSimPersType_t type = TAPI_SIM_PERS_NET;

	err_code = tel_get_sim_personalization_status(type, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_PERS_STATUS_CNF:
			int i = 0;
			TelSimPersStatus_t* pers_status = (TelSimPersStatus_t*)EventData;

			printf("[SIM APP]*********** TAPI_EVENT_SIM_PERS_STATUS_CNF************\n");
			printf("[SIM APP]SIM lock personalisation event status = [%x]\n", status);
			printf("[SIM APP]SIM lock personalisation status - net0-ns1-sp2-cp3  = [%d]\n", pers_status->type);
			printf("[SIM APP]SIM lock personalisation status - unlock0-lock1  = [%d]\n", pers_status->mode);
			printf("[SIM APP]***************************************************\n");
			break;
	}
}
@endcode

SAMPLE CODE: ENABLE/DISABLE PERSONALIZATION
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister
//  OPERATION REQUEST ENABLING NETWORK PERSONALIZATION PART
#define NET_ENCODE "54500112345678" //Sample Network Personalization code
void enable_net_pers()
{
	printf("Lock Personalisation MCC+MNC 5\n");
	printf("Plese input Network Lock CODE : length of MCC+MNC, MCC, MNC, NCK\n");
	printf("We need to use this for testing : length of MCC+MNC(5), MCC(450), MNC(01), NCK(12345678) => 54500112345678\n");

	int request_id = 0;
	int err_code;
	int length = 14+1;
	char init_pin_val[length];

	strcpy(init_pin_val, NET_ENCODE);

	TelSimPersPw_t lock_pers = {0,};
	lock_pers.type = TAPI_SIM_PERS_NET;
	lock_pers.pw_len = 14;
	lock_pers.pw = (char*)malloc(length);
	memcpy(lock_pers.pw, init_password_val, length);

	err_code = tel_enable_sim_personalization(&lock_pers, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  OPERATION REQUEST DISABLING NETWORK PERSONALIZATION PART
#define NET_DISCODE "12345678" //Sample Network Personalization code
void disable_net_pers()
{
	printf("de Personalisation key is 8 byte \n");
	printf("We need to use this for testing : 12345678\n");

	int request_id = 0;
	int err_code;
	int length = 8+1;
	char init_pin_val[length];

	strcpy(init_pin_val, NET_DISCODE);

	TelSimPersPw_t lock_pers = {0,};
	lock_pers.pw_len =  8; //control key
	lock_pers.pw = (char*)malloc(length);
	memcpy(lock_pers.pw,init_password_val,length);

	err_code = tel_disable_sim_personalization(&lock_pers, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_DISABLE_PERS_CNF:
			TelSimPinOperationResult_t opResult = (TelSimPinOperationResult_t)status;

			printf("[SIM APP]**** *TAPI_EVENT_SIM_DISABLE_PERS_CNF********\n");
			printf("[SIM APP]Eable Personalization event status = [%x]\n", opResult);

			if (opResult == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				("[SIM APP]Disable Personalization Success!\n");
			}
			else
			{
				printf("[SIM APP]Verification Failed! - Correct Password Required\n");
			}
			printf("\n***************************************************************\n");
			break;

		case TAPI_EVENT_SIM_ENABLE_PERS_CNF:
			TelSimPinOperationResult_t opResult = (TelSimPinOperationResult_t)status;

			printf("[SIM APP]****** *TAPI_EVENT_SIM_ENABLE_PERS_CNF*********\n");
			printf("[SIM APP]Eable Personalization event status = [%x]\n", opResult);

			if (opResult == TAPI_SIM_PIN_OPERATION_SUCCESS)
			{
				printf("[SIM APP]Enable Personalization Success!\n");
			}
			else
			{
				printf("[SIM APP]Verification Failed! - Correct Password Required\n");
			}
			printf("\n***************************************************************\n");
			break;
	}
}
@endcode
@}
@defgroup Use_Cases5_14 FDN
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">FDN</h3>
These APIs are used to enable/disable FDN and to retrieve the current FDN status.

@code
int tel_enable_sim_fdn(const unsigned char *pin2, int *pin2_len, int *req_id);
int tel_disable_sim_fdn(const unsigned char *pin2, int *pin2_len, int *req_id);
int tel_get_sim_fdn_status(int *fdn_status);
@endcode

SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister

//  OPERATION REQUEST ENABLE FDN MODE PART
#define PIN2_NORMAL "0000" //Sample PIN2 code
void enable_fdn()
{
	int request_id = 0;
	int api_ret;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length] ;

	strcpy(init_pin_val, PIN2_NORMAL);

	TelSimSecPw_t pin_data = {0,};

	pin_data.type = TAPI_SIM_PTYPE_PIN2;
	pin_data.pw_len = strlen(init_pin_val);

	printf("pw_len[%d]", pin_data.pw_len);

	pin_data.pw = (char*)malloc(length);
	memcpy(pin_data.pw, init_pin_val, length);

	err_code = tel_enable_sim_fdn(pin_data.pw, &pin_data.pw_len, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT FOR EVENT FROM HERE
}


//  OPERATION REQUEST DISABLE FDN MODE PART
#define PIN2_NORMAL "0000" //Sample PIN2 code
void disable_fdn()
{
	int request_id = 0;
	int api_ret;
	int length = TAPI_SIM_PIN_LENGTH+1;
	char init_pin_val[length] ;

	strcpy(init_pin_val, PIN2_NORMAL);

	TelSimSecPw_t pin_data = {0,};

	pin_data.type = TAPI_SIM_PTYPE_PIN2;
	pin_data.pw_len = strlen(init_pin_val);

	printf("pw_len[%d]", pin_data.pw_len);

	pin_data.pw = (char*)malloc(length);
	memcpy(pin_data.pw, init_pin_val, length);

	err_code = tel_disable_sim_fdn(pin_data.pw, &pin_data.pw_len, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT FOR EVENT FROM HERE


	//  EVENT RESPONSE PART
	//CALLBACK FUNCTION TO BE USED
	static void app_callback (TelTapiEvent_t *event)
	{
		int eventClass;
		int eventType;
		int requestId;
		int status;
		void* EventData = NULL;

		TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

		unsigned int temp_handle = -1;
		//
		eventClass = event->EventClass;
		eventType = event->EventType;
		requestId = event->RequestId;
		status = event->Status;
		EventData = event->pData;

		switch (eventType)
		{
			case TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF:
				printf("**********TAPI_EVENT_SIM_DISABLE_FDNMODE_CNF***********");

				TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*)EventData;

				if (status == TAPI_SIM_PIN_OPERATION_SUCCESS)
				{
					if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
					{
						printf("Disable FDN Success!");
					}
					else
					{
						printf("Unhandled type[%d]", pPinInfo->type);
					}
				}
				else
				{
					if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
					{
						printf("Disable FDN Failed! - PIN2 Required ");
						printf("Remainint attempts [%d]", pPinInfo->retry_count);
					}
					else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
					{
						printf("Disable FDN Failed! - PUK2 Required ");
						printf("Remainint attempts [%d]", pPinInfo->retry_count);
					}
					else
					{
						printf("Unhandled type[%d]", pPinInfo->type);
					}
				}
				printf("********************************************");
				break;

			case TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF:
				printf("*******TAPI_EVENT_SIM_ENABLE_FDNMODE_CNF*******");

				TelSimSecResult_t *pPinInfo = (TelSimSecResult_t*)EventData;

				if (status == TAPI_SIM_PIN_OPERATION_SUCCESS)
				{
					if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
					{
						printf("Enable FDN Success!");
					}
					else
					{
						printf("Unhandled type[%d]", pPinInfo->type);
					}
				}
				else
				{
					if (pPinInfo->type == TAPI_SIM_PTYPE_PIN2)
					{
						printf("Enable FDN Failed! - PIN2 Required ");
						printf("Remainint attempts [%d]", pPinInfo->retry_count);
					}
					else if (pPinInfo->type == TAPI_SIM_PTYPE_PUK2)
					{
						printf("Enable FDN Failed! - PUK2 Required ");
						printf("Remainint attempts [%d]", pPinInfo->retry_count);
					}
					else
					{
						printf("Unhandled type[%d]", pPinInfo->type);
					}
				}
				printf("********************************************");
				break;
		}
	}


	//  OPERATION REQUEST GET FDN STATUS PART
	void get_fdn_status()
	{
		printf("Get FDN status");
		int fdn_status = 0 ;

		err_code = tel_get_sim_fdn_status(&fdn_status);
		if (err_code == TAPI_API_SUCCESS)
		{
			if (fdn_status == 0)
			{
				printf("*************************");
				printf("FDN disabled ");
				printf("*************************");
			}
			else
			{
				printf("*************************");
				printf("FDN enabled");
				printf("*************************");
			}
		}
		else
		{
			printf("TAPI API FAIL: Error Code [0x%x]", err_code);
		}

	}
@endcode
@}
@defgroup Use_Cases5_15 IMSI Information
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">IMSI Information</h3>
This API retrieves the IMSI (International Mobile Subscriber Identity) information.

@code
int tel_get_sim_imsi(TelSimImsiInfo_t* imsi);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

void getimsiInfo()
{
	int err_code;
	TelSimImsiInfo_t sim_imsi_info;

	err_code = tel_get_sim_imsi (&sim_imsi_info);
	if (err_code == TAPI_API_SUCCESS)
	{
		printf("[SIM APP] IMSI [mcc,mnc,msin]= ");
		printf(" [%s]",sim_imsi_info.szMcc);
		printf(" [%s]",sim_imsi_info.szMnc);
		printf(" [%s]",sim_imsi_info.szMsin);
	}
	else
	{
		printf("Error Code [%x]\n", err_code);
	}
}
@endcode
@}
@defgroup Use_Cases5_16 ECC Information
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">ECC Information</h3>
This API retreieves the ECC (Emergency Call Codes) data.

@code
int tel_get_sim_ecc(TelSimEccData_t *ecc_data, int *ecc_count);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

void geteccinfo()
{
	int err_code;
	int ecc_rec_count = 0;
	TelSimEccData_t  ecc_info = {0,};

	err_code = tel_get_sim_ecc (&ecc_info, &ecc_rec_count);
	printf("The Retrun Status is %d", err_code);

	printf(" ECC count [%d]\n", ecc_rec_count);
	printf("ECC1 [%s]\n", ecc_info.EccInfo.szEcc1);
	printf("ECC2 [%s]\n", ecc_info.EccInfo.szEcc2);
	printf("ECC3 [%s]\n", ecc_info.EccInfo.szEcc3);
	printf("ECC4 [%s]\n", ecc_info.EccInfo.szEcc4);
	printf("ECC5 [%s]\n", ecc_info.EccInfo.szEcc5);
}

void get_uecc()
{
	int err_code;
	int i = 0;
	int uecc_rec_count = 0;

	printf(" Get USIM ECC  ");

	TelSimEccData_t uecc_info = {{{0,}}};

	err_code = tel_get_sim_ecc(&uecc_info, &uecc_rec_count);
	if (err_code == TAPI_API_SUCCESS)
	{
		printf("ECC count [%d]",uecc_rec_count );

		for (i=0; i<uecc_rec_count; i++)
		{
			printf("Loop(%d): ", i);
			printf(" ECC Used [%d]", uecc_info.UeccInfo[i].bUsed);
			printf(" ECC Len [%d]", uecc_info.UeccInfo[i].EccLen);

			if (NULL != uecc_info.UeccInfo[i].szEcc)
			{
				printf("ECC string [%s]", uecc_info.UeccInfo[i].szEcc);
			}
			else
			{
				printf("ECC string [null]");
			}

			printf("ECC alphaID [%s]",uecc_info.UeccInfo[i].szEccAlphaIdentifier);
		}
	}
	else
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
}
@endcode
@}
@defgroup Use_Cases5_17 Language Preference Information
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Language Preference Information</h3>
These APIs are used to get and set the language preference information stored in SIM.

@code
int tel_get_sim_language(TelSimLanguageInfo_t *sim_language);
int tel_set_sim_language(TelSimLanguagePreferenceCode_t language, int *req_id);
@endcode

SAMPLE CODE: GET SIM LANGUAGE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

void getlanguageprefinfo()
{
	int err_code;
	TelSimLanguageInfo_t li_info;
	int i;

	err_code = tel_get_sim_language (&li_info);
	printf("Error Code [%x]\n", err_code);

	for (i =0; i<li_info.LpCount; i++)
		printf(" LI[%d] --> Val  [%d]\n", i, li_info.Lp[i]);
}
@endcode

SAMPLE CODE: UPDATE SIM LANGUAGE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister
//  OPERATION REQUEST UPDATE SIM LANGUAGE PART
void get_net_pers()
{
	int err_code;
	int request_id = 0;
	TelSimLanguagePreferenceCode_t language = 0x00;

	language = TAPI_SIM_LP_ENGLISH; //0x01

	err_code = tel_set_sim_language(language, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}


//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_SET_LANGUAGE_CNF:
			printf("*****TAPI_EVENT_SIM_SET_LANGUAGE_CNF*********");
			printf("update event status = [0x%x]", status);
			printf("*********************************************");
			break;

	}
}
@endcode
@}
@defgroup Use_Cases5_18 Getting Card Type
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Getting Card Type</h3>
This API is a synchronous API which gets the Card Type i.e. whether the SIM Card is a GSM SIM or a 3G SIM.

@code
int tel_get_sim_type(TelSimCardType_t *card_type);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

void getcardtype()
{
	int err_code;
	TelSimCardType_t cardInfo;

	printf("Get card type !!!\n");
	err_code = tel_get_sim_type (&cardInfo);
	printf("card type is %d\n", cardInfo);
}
@endcode
@}
@defgroup Use_Cases5_19  Getting SIM Card Initialization Status
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Getting SIM Card Initialization Status</h3>
API is a synchronous API which gets the Card Init status and Identification.

@code
int tel_get_sim_init_info(TelSimCardStatus_t *sim_status, int *card_changed);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//  OPERATION REQUEST GET CARD INIT STATUS PART
void get_sim_init_status()
{
	int err_code;
	TelSimCardStatus_t status = 0x00;
	int b_card_changed = 0;

	err_code = tel_get_sim_init_info(&status, &b_card_changed);
	if (err_code == TAPI_API_SUCCESS)
	{
		printf("**********************************************");
		printf("CardStatus:[0x%x]",status);
		printf("b_card_changed[%d]\n",b_card_changed);
		printf("**********************************************");
	}
	else
	{
		printf("TAPI Fail: Error Code [%d]\n", err_code);
	}
}
@endcode
@}
@defgroup Use_Cases5_20 SIM ACCESS COMMANDS Interface
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">SIM ACCESS COMMANDS Interface</h3>
This API is used to handle SIM EF data directly according to sim access command (select-file info, read, update command). Use_Cases is defined in the 3GPP or ETSI SIM specifications.

@code
int tel_req_sim_access(const TelSimRSimReq_t *rsim_data, int *req_id);
@endcode

SAMPLE CODE: EF HANDLING ( SELECT-FILE INFO)
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister
//  OPERATION REQUEST GET SIM FILE INFO PART
void get_sim_file_info()
{
	int err_code;
	int request_id = 0;

	TelRSimReq_t rsim_data;
	rsim_data.file_id = TAPI_SIM_EFILE_SPN;      // rsim access : sim file id to access
	rsim_data.rsim_cmd = TAPI_SIM_GET_RESPONSE;  // rsim access : get response, read, update available

	err_code = tel_req_sim_access(&rsim_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;

	int status;

	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_RSIM_ACCESS_CNF:
			TelSimReadFileRaw_t* rsim_resp = (TelSimReadFileRaw_t*)EventData;

			printf("*******TAPI_EVENT_SIM_RSIM_ACCESS_CNF**********");
			printf("Read event status = [0x%x]", status);

			if ((rsim_resp->sw1 == 0x90 && rsim_resp->sw2 == 0x00) ||rsim_resp->sw1 == 0x91)
			{
				printf("**********GET FILE INFO************");
				printf("you should parse current data array value properly regarding 3GPP or ETSI TS 102 221 SPEC");
			}
			printf("************************************************");
			break;
	}
}
@endcode
@}
@defgroup Use_Cases5_21 SIM Mailbox EF management
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">SIM Mailbox EF management</h3>
These APIs are used to get and update the mailbox information stored in SIM.

@code
int tel_get_sim_mailbox_info(TelSimMsgWaitingGroup_t type, int *req_id);
int tel_set_sim_mailbox_info(TelSimMsgWaitingGroup_t type, const TelSimDialingNumberInfo_t *update_data, int *req_id);
@endcode

SAMPLE CODE: READ / UPDATE MAILBOX INFO
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister

void get_sim_mailbox()
{
	int err_code;
	int request_id = 0;
	TelSimMsgWaitingGroup_t type = 0x00;

	type = TAPI_SIM_MWG_VOICE;

	err_code = tel_get_sim_mailbox_info(type, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}
//  OPERATION REQUEST UPDATE SIM MAILBOX INFO PART
void update_sim_mailbox()
{
	int err_code;
	int request_id = 0;

	TelSimMsgWaitingGroup_t type = 0x00;
	TelSimDialingNumberInfo_t update_data;

	type = TAPI_SIM_MWG_VOICE;

	memcpy(update_data.AlphaId, "TestUpdate", 10);
	update_data.AlphaIdLength = strlen(update_data.AlphaId);
	update_data.CapaConfigId = 1;
	memcpy(update_data.DiallingNum, "101020203030", 12);
	update_data.DiallingnumLength = strlen(update_data.DiallingNum);
	update_data.Ext1RecordId = 1;
	update_data.NumberingPlanIdent =2;
	update_data.TypeOfNumber = 2;

	err_code = tel_set_sim_mailbox_info(type, &update_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_GET_MAILBOX_CNF:
			TelSimDialingNumberInfo_t* mailbox = (TelSimDialingNumberInfo_t*)EventData;

			printf("*******TAPI_EVENT_SIM_GET_MAILBOX_CNF*********");
			printf("Read event status = [0x%x]", status);
			printf("bUsed[%d]", mailbox->bUsed);
			printf("AlphaIDMaxLength[%d]", mailbox->AlphaIDMaxLength);
			printf("AlphaIdLength[%d]", mailbox->AlphaIdLength);
			printf("AlphaId[%s]", mailbox->AlphaId);
			printf("DiallingNumMaxLength[%d]", mailbox->DiallingNumMaxLength);
			printf("DiallingnumLength[%d]", mailbox->DiallingnumLength);
			printf("TypeOfNumber:0-unknown,1-international,2-national[%d]", mailbox->TypeOfNumber);
			printf("NumberingPlanIdent:0-unknown,1-isdn,3-data[%d]", mailbox->NumberingPlanIdent);
			printf("DiallingNum[%s]", mailbox->DiallingNum);
			printf("CapaConfigId[%c]", mailbox->CapaConfigId);
			printf("Ext1RecordId[%c]", mailbox->Ext1RecordId);
			printf("**********************************************************");
			break;

		case TAPI_EVENT_SIM_SET_MAILBOX_CNF:
			printf("*********NEW*TAPI_EVENT_SIM_SET_MAILBOX_CNF************");
			printf("update event status = [0x%x]", status);
			printf("**********************************************************");
			break;
	}
}
@endcode
@}
@defgroup Use_Cases5_22 SIM Indication Status EFs management
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">SIM Indication Status EFs management</h3>
These APIs are used to get and set the message indication status stored in SIM.

@code
int tel_get_sim_indicator_info(TelSimIndicationStatusType_t type, int *req_id);
int tel_set_sim_indicator_info(const TelSimIndicationStatusData_t *update_data, int *req_id);
@endcode

SAMPLE CODE:  READ / UPDATE INDICATION STATUS DATA
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister

//  OPERATION REQUEST READ SIM MESSAGE WAITING INDICATION STATUS INFO PART
void read_sim_mwis()
{
	int err_code;
	int request_id = 0;

	TelSimIndicationStatusType_t type = 0x00;

	type = TAPI_SIM_INDICATION_MW;

	err_code = tel_get_sim_indicator_info(type, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}

//  OPERATION REQUEST UPDATE SIM MESSAGE WAITING INDICATION STATUS INFO PART
//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister

void update_sim_mwis()
{
	int err_code;
	int request_id = 0;

	TelSimIndicationStatusType_t type = 0x00;
	TelSimIndicationStatusData_t update_data;
	memset(&update_data, 0x00, sizeof(TelSimIndicationStatusData_t));

	type = TAPI_SIM_INDICATION_MW;

	update_data.requested_type = type;
	update_data.data.Mwis.VoiceMailCount = 1;           // VoiceMail Count
	update_data.data.Mwis.FaxCount = 2;                 // Fax Count
	update_data.data.Mwis.EmailCount = 3;               // Email Count
	update_data.data.Mwis.OtherCount = 4;               // Other Count
	update_data.data.Mwis.VideoMailCount = 5;           // VideoMail Count

	err_code = tel_set_sim_indicator_info(&update_data, &request_id);
	if (err_code != TAPI_API_SUCCESS)
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
	// WAIT EVENT RESPONSE FROM HERE
}


//  EVENT RESPONSE PART
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_GET_INDICATION_CNF:
			TelSimIndicationStatusData_t* indi_data = (TelSimIndicationStatusData_t*)EventData;

			printf("*******TAPI_EVENT_SIM_GET_INDICATION_CNF**********");
			printf("Read event status = [0x%x]", status);

			if (indi_data->requested_type == TAPI_SIM_INDICATION_MW)
			{
				printf("**********MWIS************");
				printf("IndicatorType[%d]", indi_data->data.Mwis.IndicatorType);
				printf("VoiceMailCount[%d]", indi_data->data.Mwis.VoiceMailCount);
				printf("FaxCount[%d]", indi_data->data.Mwis.FaxCount);
				printf("EmailCount[%d]", indi_data->data.Mwis.EmailCount);
				printf("OtherCount[%d]", indi_data->data.Mwis.OtherCount);
				printf("VideoMailCount[%d]", indi_data->data.Mwis.VideoMailCount);
			}
			else if (indi_data->requested_type == TAPI_SIM_INDICATION_CF)
			{
				printf("**********CFIS************");
				printf("bUsed[%d]", indi_data->data.Cfis.bUsed);
				printf("MspNumber[%d]", indi_data->data.Cfis.MspNumber);
				printf("Status[%d]", indi_data->data.Cfis.Status);
				printf("DiallingnumLen[%d]", indi_data->data.Cfis.DiallingnumLen);
				printf("TypeOfNumber[%d]", indi_data->data.Cfis.TypeOfNumber);
				printf("NumberingPlanIdent[%d]", indi_data->data.Cfis.NumberingPlanIdent);
				printf("DiallingNum[%s]", indi_data->data.Cfis.DiallingNum);
				printf("CapaConfig2Id[%d]", indi_data->data.Cfis.CapaConfig2Id);
				printf("Ext7RecordId[%d]", indi_data->data.Cfis.Ext7RecordId);
			}
			else
			{
				printf("Unhandled indication status type[%d]",indi_data->requested_type );
			}
			printf("************************************************");
			break;

		case TAPI_EVENT_SIM_SET_INDICATION_CNF:
			printf("******TAPI_EVENT_SIM_SET_INDICATION_CNF*******");
			printf("update event status = [0x%x]", status);
			printf("**********************************************");
			break;
	}
}
@endcode
@}
@defgroup Use_Cases5_23 Phone Book
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Phone Book</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

Telephony provides APIs for various phone book related features, such as getting total records in phone book, retrieving phone book entry information, reading, updating and deleting phone book information, getting first valid index.

@code
int tel_get_sim_pb_count(TelSimPbFileType_t pb_type, int* req_id);
int tel_get_sim_pb_meta_info(TelSimPbFileType_t pb_type, int* req_id);
int tel_read_sim_pb_record(TelSimPbFileType_t pb_type, unsigned short index, int *req_id );
int tel_update_sim_pb_record(const TelSimPbRecordData_t *req_data, int *req_id );
int tel_delete_sim_pb_record(TelSimPbFileType_t pb_type, unsigned short index, int *req_id );
int tel_get_sim_pb_init_info(int *init_completed, TelSimPbList_t *pb_list, int *first_index );
@endcode

SAMPLE CODE FOR GETTING STORAGE COUNT
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void getstoragecount()
{
	int ret_status;
	TelSimPbFileType_t StorageFileType = TAPI_SIM_PB_LDN;
	int pRequestId = 0;

	ret_status = tel_get_sim_pb_count (StorageFileType, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF:
			printf("TAPI_EVENT_SIM_PB_STORAGE_COUNT_CNF");
			break;
	}
}
@endcode

SAMPLE CODE TO GET PHONE BOOK ENTRY INFO
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENIONED IN Event Register and Deregister
void getPBEntryInfo()
{
	int ret_status;
	TelSimPbFileType_t StorageFileType = TAPI_SIM_PB_FDN;
	int pRequestId = 0;

	ret_status = tel_get_sim_pb_meta_info (StorageFileType, &pRequestId);
	if (ret _status == TAPI_API_SUCCESS)
		printf(successful\n);
	else
		printf(error=%d\n, ret_status);
	//WAIT FOR EVENT HERE
}
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event;

	unsigned int temp_handle = -1;
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (EventType) {
		case TAPI_EVENT_SIM_PB_ENTRY_INFO_CNF:
			TelSimPbEntryInfo_t *entryInfo = (TelSimPbEntryInfo_t*)sim_event->pData ;
			printf("SIM PB entry info event status = [%x]\n", sim_event->Status);
			printf("[SIM APP]STORAGE TYPE [%d]\n", entryInfo->StorageFileType);
			printf("[SIM APP]PB_INDEX_MIN %d\n", entryInfo->PbIndexMin);
			printf("[SIM APP]PB_INDEX_MAX = %d\n", entryInfo->PbIndexMax);
			printf("[SIM APP]PB_NUM_LEN_MAX =  %d\n", entryInfo->PbNumLenMax);
			printf("[SIM APP]PB_TEXT_LEN_MAX =  %d\n", entryInfo->PbTextLenMax);
			printf("********************************************\n");
			break;
	}
}
@endcode

SAMPLE CODE FOR READING RECORD
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void simreadrecord()
{
	int ret_status;
	TelSimPbFileType_t StorageFileType = TAPI_SIM_PB_FDN;
	unsigned short Index = 4;
	int pRequestId = 0;

	ret_status = tel_read_sim_pb_record (StorageFileType,Index, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf(successful\n);
	else
		printf(error=%d\n, ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType) {
		case TAPI TAPI_EVENT_SIM_PB_ACCESS_READ_CNF:
			printf("TAPI_EVENT_SIM_PB_ACCESS_READ_CNF");
			//READ DATA
			break;
			//
	}
}
@endcode

SAMPLE CODE FOR UPDATING RECORD
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void simupdaterecord()
{
	int ret_status;
	TelSimPbRecordData_t pb_add;
	pb_add.StorageFileType = TAPI_SIM_PB_GAS;
	int pRequestId = 0;

	pb_add.Index = 1;

	ret_status = tel_update_sim_pb_record (&pb_add, &pRequestId);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (EventType)
	{
		case TAPI_EVENT_SIM_PB_UPDATE_CNF:
			printf("TAPI_EVENT_SIM_PB_UPDATE_CNF");
			break;
			//...
	}
}
@endcode

SAMPLE CODE FOR DELETING RECORD
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void contactdelete()
{
	int ret_status;
	//DELETE RECORD
	TelSimPbFileType_t StorageFileType = TAPI_SIM_PB_3GSIM;
	unsigned short RecordIndex = 1;
	int pRequestId = 0;

	ret_status = tel_delete_sim_pb_record (StorageFileType, RecordIndex, &pRequestId);
}//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (EventType)
	{
		case TAPI_EVENT_SIM_PB_DELETE_CNF:
			printf(TAPI_EVENT_SIM_PB_DELETE_CNF);
			//
			break;
	}
}
@endcode

SAMPLE CODE FOR GETTING PHONEBOOK INIT INFO
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//  OPERATION REQUEST GET SIM PHONEBOOK INIT STATUS PART
void get_sim_phonebook_init_status()
{
	int valid_index = 0;
	TelSimPbList_t pb_list;
	int pPbInitCompleted = 0;

	err_code = tel_get_sim_pb_init_info(&pPbInitCompleted, &pb_list, &valid_index);
	if (err_code == TAPI_API_SUCCESS)
	{
		if (pPbInitCompleted == 0)
		{
			printf(" SIM PB INIT NOT completed ");
		}
		else
		{
			printf("SIM PB INIT completed");
			// if no sim records at all then valid_index = 0xFF
			printf ("ADN First index is [%d]",valid_index);
			printf ("SIM phone book storage list Information ");
			printf ("********************************************");
			printf ("pb_list.b_fdn[%d]",pb_list.b_fdn);
			printf ("pb_list.b_msisdn[%d]",pb_list.b_msisdn);
			printf ("pb_list.b_adn[%d]",pb_list.b_adn);
			printf ("pb_list.b_sdn[%d]",pb_list.b_sdn);
			printf ("pb_list.b_3g[%d]",pb_list.b_3g);
			printf ("pb_list.b_aas[%d]",pb_list.b_aas);
			printf ("pb_list.b_gas[%d]",pb_list.b_gas);
		}
	}
	else
	{
		printf("TAPI API FAIL: Error Code [0x%x]", err_code);
	}
}
@endcode
@}
@defgroup Use_Cases5_24 Getting 3G Phone Book Information
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">Getting 3G Phone Book Information</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

Telephony provides APIs to retrieve SIM EFs supported by 3G phone , such as ANR, SNE, GRP, EMAIL etc and corresponding EFs max text length, number length and size.

@code
int tel_get_sim_pb_3g_meta_info(int *req_id);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void get3gcapainfo()
{
	int ret_status;
	int req_id = 0;

	ret_status = tel_get_sim_pb_3g_meta_info (&req_id);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId, i;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType) {
		case TAPI_EVENT_SIM_PB_CAPABILITY_INFO_CNF:
			printf("[SIM APP]SIM PB Capability Info event status = [%x]\n", sim_event->Status);
			TelSimPbCapabilityInfo_t *sim_capability_info = (TelSimPbCapabilityInfo_t*)sim_event->pData;
			int i;
			for (i=0; i<sim_capability_info->FileTypeCount; i++)
			{
				printf(" [SIM APP]Filed tag type[%d]", sim_capability_info->FileTypeInfo[i].FileType3G);
				printf(" [SIM APP]max index [%d]\n", sim_capability_info->FileTypeInfo[i].PbIndexMax);
				printf(" [SIM APP]max text [%d]\n", sim_capability_info->FileTypeInfo[i].PbTextMax);
				printf(" [SIM APP]used count [%d]\n",  sim_capability_info->FileTypeInfo[i].TotalRecordCount);
			}
			break;
			//...
	}
}
@endcode

@}
@defgroup Use_Cases5_25 SAP (Sim access Profile) features
@ingroup Use_Cases5_NETWORK
@{

		<h3 class="pg">SAP (Sim access Profile) features</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

The SIM Access Profile defines the protocols and procedures that shall be used to access data and services of GSM SIM card, a UICC card or a R-UIM card via a Bluetooth link.

The SIM Access Profile specifies the following features:
- Connection Management
- Transfer APDU
- Transfer ATR
- Power SIM off/on
- Transfer Card Reader Status
- Set Transport Protocol

<h4>Connection Management:</h4>
An established SIM Access Profile connection is the prerequisite for all other features.
Telephony exposes APIs to make a connection request to SIM and to get connection status information.

@code
int tel_req_sap_connection( TelSimSapConnect_t *req_data, int *req_id);
int tel_req_sap_connection_status( int *req_id);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"
//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void sapconnectreq()
{
	int ret_status;
	TelSimSapConnect_t pConnectionReq;
	int pRequestId = 0;

	pConnectionReq.MsgId = TAPI_SIM_SAP_CONNECT_REQ;
	pConnectionReq.MaxMsgSize = 10; //temp for testing only - not clear

	ret_status = tel_req_sap_connection (&pConnectionReq, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_SIM_EVENT_SAP_CONNECT_CNF:
			printf("TAPI_SIM_EVENT_SAP_CONNECT_CNF");
			break;
			//...
	}
}

//GET STATUS
void sapconnectstatusreq()
{
	int request_id = 0;
	int ret_status = tel_req_sap_connection_status (&request_id);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_SIM_EVENT_SAP_CONNECT_STATUS_CNF:
			printf("TAPI_SIM_EVENT_SAP_CONNECT_STATUS_CNF");
			break;
			//...
	}
}
@endcode

<h4>Transfer APDU:</h4>
The ability to send APDUs (Application Protocol Data Units) over the Bluetooth link
@n Telephony provides API to transfer APDU to sim.

@code
int tel_req_sap_transfer_apdu(TelSimSapApduData_t *apdu_data, int *req_id);
@endcode

SAMPLE CODE:
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void saptransferapdu()
{
	int err_code;
	int file_id = TAPI_SIM_EFILE_ICCID;

	// for selecting EF ICCID
	TelSimSapApduData_t apdu_data;
	int pRequestId = 0;

	apdu_data.ApduLength = 7;
	apdu_data.Apdu[0] = 0xA0; // class
	apdu_data.Apdu[1] = 0xA4; // instruction
	apdu_data.Apdu[2] = 0; // p1 = parameter 1
	apdu_data.Apdu[3] = 0; // p2 = parameter 2
	apdu_data.Apdu[4] = 2; // p3 - parameter 3

	memcpy(&apdu_data.Apdu[5], &file_id, sizeof(unsigned short));
	err_code = tel_req_sap_transfer_apdu (&apdu_data, NULL);
	if (err_code == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", err_code);
}
//WAIT FOR EVENT HERE
//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_SIM_EVENT_SAP_TRANSFER_APDU_CNF:
			printf("TAPI_SIM_EVENT_SAP_TRANSFER_APDU_CNF");
			break;
	}//...
}
@endcode

<h4>Transfer ATR:</h4>
The ability to send the content of the ATR (Answer to Reset) from the Server to the Client over the Bluetooth link. It contains information about the interface provided by the subscription module and the services on the GSM SIM, the UICC or the R-UIM.

@code
int tel_req_sap_transfer_atr( int *req_id);
@endcode

SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void saptransferatrreq()
{
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_req_sap_transfer_atr (&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (eventType)
	{
		case TAPI_SIM_EVENT_SAP_TRANSFER_ATR_CNF:
			printf("TAPI_SIM_EVENT_SAP_TRANSFER_ATR_CNF");
			break;
			//...
	}
}
@endcode

<h4>Power sim on/off:</h4>
The ability to power the subscription module on/off remotely.

@code
int tel_req_sap_power_operation(TelSimSapMsgId_t req_data, int *req_id );
@endcode

SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void sapsetpower()
{
	int ret_status;
	TelSimSapMsgId_t MsgId = TAPI_SIM_SAP_CONNECT_REQ;
	int pRequestId = 0;

	ret_status = tel_req_sap_power_operation (MsgId, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;


	switch (EventType)
	{
		case TAPI_SIM_EVENT_SAP_SET_SIM_POWER_CNF:
			printf("TAPI_SIM_EVENT_SAP_SET_SIM_POWER_CNF");
			break;
			//...
	}
}
@endcode

<h4>Transfer Card Reader Status:</h4>
The card reader status contains some basic information about the Card Reader and the subscription module (e.g. the size of the SIM or if the SIM is removable).

@code
int tel_req_sap_cardreader_status(int *req_id);
@endcode

SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void simsapcardreaderstatus()
{
	int ret_status;
	int pRequestId = 0;

	ret_status = tel_req_sap_cardreader_status (&pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;


	switch (eventType)
	{
		case TAPI_SIM_EVENT_SAP_CARD_READER_STATUS_CNF:
			printf("TAPI_SIM_EVENT_SAP_CARD_READER_STATUS_CNF");
			break;
			//...
	}
}
@endcode

<h4>Set Transport Protocol:</h4>
The server shall reset the subscription module and switch to the desired protocol if supported by subscription module and Server.

@code
int tel_req_sap_transport_protocol(TelSimSapProtocol_t protocol, int *req_id);
@endcode

SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void sapsettransportprotocol()
{
	int ret_status;
	TelSimSapProtocol_t Protocol = TAPI_SIM_SAP_PROTOCOL_T0;
	int pRequestId = 0;

	ret_status = tel_req_sap_transport_protocol (Protocol, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (EventType)
	{
		case TAPI_SIM_EVENT_SAP_SET_PROTOCOL_CNF:
			printf("TAPI_SIM_EVENT_SAP_SET_PROTOCOL_CNF");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases5_26 ISIM Features
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">ISIM Features</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

@code
int tel_req_sim_isim_authentication( TelSimIsimAuthenticationData_t *authentication_data, int *req_id);
@endcode

Application uses this API to send the ISIM authentication request to telephony.

SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister
void isimauthreq()
{
	int ret_status;
	TelSimIsimAuthenticationData_t authenticationData;
	authenticationData.RandomAccessLength = 100;
	authenticationData.RandomAccessLength = 100;
	authenticationData.RandomAccessData = "test data";
	authenticationData.AuthData = "test data";
	int pRequestId = 0;

	ret_status = tel_req_sim_isim_authentication (&authenticationData, &pRequestId);
	if (ret_status == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (EventType)
	{
		case TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF:
			printf("TAPI_EVENT_SIM_ISIM_AUTHENTICATION_CNF");
			break;
			//...
	}
}
@endcode
@}
@defgroup Use_Cases5_27 SIM NOTIFICATION MESSAGE
@ingroup Use_Cases5_NETWORK
@{


		<h3 class="pg">SIM NOTIFICATION MESSAGE </h3>
These are not APIs, but EVENTs from the low layer. User can receive the following events:
@code
TAPI_EVENT_SIM_STATUS_IND
TAPI_EVENT_SIM_FDN_STATUS_IND
@endcode

Application uses this event as follows.
SAMPLE CODE
@code
#include <ITapiSim.h>
#include <stdio.h>
#include "TapiCommon.h"

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

//CALLBACK FUNCTION TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	TelTapiEvent_t *sim_event = (TelTapiEvent_t*) event ;

	unsigned int temp_handle = -1;
	//...
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	switch (EventType)
	{
		case TAPI_EVENT_SIM_STATUS_IND:
			printf("***********TAPI_EVENT_SIM_STATUS_IND**************");

			if (status == TAPI_SIM_STATUS_CARD_ERROR)
			{
				printf("SIM CARD ERROR");
			}
			else if (status == TAPI_SIM_STATUS_CARD_NOT_PRESENT)
			{
				printf("SIM CARD NOT INSERTED");
			}
			else if (status == TAPI_SIM_STATUS_SIM_INITIALIZING)
			{
				printf("SIM INITITIALIZING");
			}
			else if (status == TAPI_SIM_STATUS_SIM_INIT_COMPLETED)
			{
				printf("SIM INIT COMPLETED");
			}
			else if (status == TAPI_SIM_STATUS_SIM_PIN_REQUIRED)
			{
				printf("SIM PIN REQUIRED");
			}
			else if (status == TAPI_SIM_STATUS_SIM_PUK_REQUIRED)
			{
				printf("SIM PUK REQUIRED");
			}
			else if (status == TAPI_SIM_STATUS_CARD_BLOCKED)
			{
				printf("SIM CARD BLOCKED");
			}
			printf("********************************************");
			break;

		case TAPI_EVENT_SIM_FDN_STATUS_IND:
			printf("*********TAPI_EVENT_SIM_FDN_STATUS_IND***********");
			printf("SIM FDN status event status = [0x%x]", status);
			printf("********************************************");
			break;
			//...
	}
}
@endcode
@}

	<h2 class="pg">SOUND</h2>
<strong>[Note] Telephony Emulator does not support this feature.</strong>
These APIs provide the interface for configuring sound related parameters.

@defgroup Use_Cases6_1 Event Register and Deregister
@ingroup Use_Cases6_SOUND
@{


		<h3 class="pg">Event Register and Deregister </h3>
To receive asynchronous event notifications for the Sound APIs, the registration of related events with their associated call backs is required. When an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

SAMPLE CODE:
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
int api_err;

api_err = tel_init ();
if (api_err != TAPI_API_SUCCESS)
{
	TEST_DEBUG("tel_init Failed - api_err = %d \n", api_err);
}

api_err = tel_register_event (TAPI_EVENT_SOUND_VOLUMECTRL_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("VSOUND Event registration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);

tel_register_app_name("org.tizen.appname");

// DEREGISTER EVENT
api_err = tel_deregister_event (subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Deregestration Fail\n");
}
@endcode

The main features provided by the Sound APIs include setting the volume,voice path and user configuration such as minute alert,Service change,Connet tone, Disconnect Tone...etc.
@}
@defgroup Use_Cases6_2 Voice path control
@ingroup Use_Cases6_SOUND
@{


		<h3 class="pg">Voice path control</h3>
This API provides an interface for controlling voice path via application.

@code
int tel_set_sound_path (tapi_sound_audio_path_t  vc_path, int *pRequestId);
@endcode

@}
@defgroup Use_Cases6_3 Volume control
@ingroup Use_Cases6_SOUND
@{


		<h3 class="pg">Volume control</h3>
These APIs provide interfaces for controlling volume via application.

@code
int     tel_set_sound_volume_info (tapi_sound_volume_control_t vol_control, int *pRequestId);
int     tel_get_sound_volume_info (tapi_sound_volume_type_t vol_control, int *pRequestId);
@endcode

@}
@defgroup Use_Cases6_4 Mic mute control
@ingroup Use_Cases6_SOUND
@{


		<h3 class="pg">Mic mute control</h3>
This API provides an interface for mute/unmute mic via application.

@code
int  tel_set_sound_mute_status(tapi_sound_mic_mute_t micmute_set, int * pRequestId);
@endcode


SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSound.h>

//SUBSCRIBE FOR EVENTS AS MENTIONED IN Event Register and Deregister

//VOLUME CONTROL
void vol_controlset()
{
	int ret_val;
	int requestId = -1;
	tapi_sound_volume_control_t vol_control;

	vol_control.volume = TAPI_SOUND_VOLUME_LEVEL_3;
	vol_control.vol_type = TAPI_SOUND_VOL_VOICE;

	ret_val = tel_set_sound_volume_info(vol_control, &requestId);
	if (ret_val == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_val);
	//WAIT FOR EVENT HERE
}
void vol_controlget()
{
	int ret_val;
	int requestId = -1;
	api_sound_volume_type_t vol_type;
	vol_type = TAPI_SOUND_VOL_VOICE;

	ret_val = tel_get_sound_volume_info (vol_type, &requestId);
	if (ret_val == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_val);
	//WAIT FOR EVENT HERE
}

//VOICE PATH AND USER CONFIGURATION
void voicepathset()
{
	int ret_val;
	int requestId = -1;

	tapi_sound_audio_path_t vc_path;
	//VOICE PATH
	vc_path = TAPI_SOUND_HANDSET;

	ret_val = tel_set_sound_path (vc_path, &requestId);
	if (ret_val == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}


//USER CONFIGURATION
void usr_configset()
{
	int ret_val;
	Int requestId = -1;
	tapi_sound_user_sound_mask_t snd_mask;
	tapi_sound_user_sound_value_t snd_value;

	snd_mask = TAPI_SOUND_USER_MASK_ONE_MIN_ALERT;
	snd_value = TAPI_SOUND_ON;
	printf("tapi_sound_usr_configuration_set\n");
	ret_val = tapi_sound_usr_configuration_set(snd_mask,snd_value, &requestId);
	if (ret_val == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}

void usr_configget()
{
	int ret_val;
	tapi_sound_user_sound_mask_t snd_mask;

	snd_mask = TAPI_SOUND_USER_MASK_SERVICE_CHANGE_ALERT;
	printf("tel_get_sound_mute_status \n");
	ret_val = tel_get_sound_mute_status(snd_mask, &requestId);
	if (ret_val == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}

//MIC MUTE CONT\ROL
void micmute_controlset()
{
	int ret_val;
	tapi_sound_mic_mute_t set_micmute;

	set_micmute = TAPI_SOUND_MIC_MUTE;

	printf(" tel_set_sound_mute_status \n");
	ret_val = tel_set_sound_mute_status (set_micmute, &requestId);
	if (ret_val == TAPI_API_SUCCESS)
		printf("successful\n");
	else
		printf("error=%d\n", ret_status);
	//WAIT FOR EVENT HERE
}


//CALL BACK TO BE USED
static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	unsigned int temp_handle = -1;
	//
	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;

	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d]\n", eventClass, eventType , requestId, status );

	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}


	switch (eventClass)
	{
		case TAPI_EVENT_CLASS_SOUND:
			switch (eventType)
			{
				case TAPI_EVENT_SOUND_VOLUMECTRL_RSP :
					printf("***  TAPI_EVENT_SOUND_VOLUMECTRL_RSP received : %x ****\n", eventType);
					tapi_sound_volumn_ctrl_res vlm_ctrl_info;
					int index;

					memcpy(&vlm_ctrl_info, event->pData,
							sizeof(tapi_sound_volumn_ctrl_res));
					break;

				case TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI:
					tapi_sound_audio_path_t AudioPath = 0;
					int OldSndOutPath = MM_SOUND_PATH_NONE;
					AudioPath = *((tapi_sound_audio_path_t *)(event->pData));
					printf("***  TAPI_EVENT_SOUND_AUDIOPATHCTRL_NOTI received : %x ****\n", eventType);
					break;

				case TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI:
					tapi_sound_audio_source_type_t *pAudioSrcCtrl = NULL;
					pAudioSrcCtrl = (tapi_sound_audio_source_type_t*)(event->pData);
					printf("***  TAPI_EVENT_SOUND_AUDIOSOURCECTRL_NOTI received : %x ****\n", eventType);
					break;

				case TAPI_EVENT_SOUND_ERROR_IND:
					tapi_phone_err_t errorCause;
					errorCause = *(( tapi_phone_err_t *)(event->pData));
					printf("***  TAPI_EVENT_SOUND_ERROR_IND received : %x ****\n", eventType);
					break;

				case TAPI_EVENT_SOUND_MICMUTECTRL_RSP:
					tapi_sound_mic_mute_t *mute_status = NULL;
					mute_status = (tapi_sound_audio_source_type_t*)(event->pData);

					printf("***  TAPI_EVENT_SOUND_MICMUTECTRL_RSP received : %x ****\n", eventType);
					break;

				default:
					printf("***  Default Event  %x ****\n", eventType);
					break;
			}
			//...
			break;

		default:
			printf("*** Other TAPI EVENT received : class : %x ****\n", eventClass);
			break;
	}
}
@endcode
@}


	<h2 class="pg">SAT (SIM Application Toolkit)</h2>
@defgroup Use_Cases7_1 Event register and deregister
@ingroup Use_Cases7_SAT
@{

		<h3 class="pg">Event register and deregister</h3>
To receive asynchronous event notifications for SAT APIs, the registration of related events with their associated call backs is required. When an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

SAMPLE CODE
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
int api_err;

api_err = tel_init();
if (api_err != TAPI_API_SUCCESS)
{
	TEST_DEBUG("tel_init Failed - api_err = %d \n", api_err);
}

api_err = tel_register_event(TAPI_EVENT_SAT_MENU_SELECTION_CNF, &subscription_id, (TelAppCallback)&app_callback);
printf("SAT Event registration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);

tel_register_app_name("org.tizen.appname");

// DEREGISTER EVENT
api_err = tel_deregister_event(subscription_id);
if (api_err != TAPI_API_SUCCESS)
{
	printf("Event Class Unregeration Fail\n");
}

api_err = tel_deinit();
if (api_err != TAPI_API_SUCCESS)
{
	printf("tel_deinit Fail\n");
}

static void app_callback(TelTapiEvent_t *event)
{
	//CALL BACK FUNTION TO BE CALLED
	switch (event->EventType)
	{
		case TAPI_EVENT_SAT_DISPLAY_TEXT_IND:
			TelSatDisplayTextInd_t *event_data;
			event_data = (TelSatDisplayTextInd_t*)event->pData;
			break;
		case TAPI_EVENT_SAT_GET_INKEY_IND:
			TelSatGetInkeyIndInfo_t *event_data;
			event_data = (TelSatGetInkeyIndInfo_t *)event->pData;
			break;
		case TAPI_EVENT_SAT_GET_INPUT_IND:
			TelSatGetInputInd_t *event_data;
			event_data = (TelSatGetInputInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_PLAY_TONE_IND:
			TelSatPlayToneInd_t *event_data;
			event_data = (TelSatPlayToneInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_PLAY_TONE_IND:
			TelSatPlayToneInd_t *event_data;
			event_data = (TelSatPlayToneInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_REFRESH_IND:
			TelSatRefreshIndUiInfo_t *event_data;
			event_data = (TelSatRefreshIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SETUP_MENU_IND:
			TelSatSetupMenuInfo_t *event_data;
			event_data = (TelSatSetupMenuInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_SETUP_MENU_IND:
			TelSatSetupMenuInfo_t *event_data;
			event_data = (TelSatSetupMenuInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SELECT_ITEM_IND:
			TelSatSelectItemInd_t *event_data;
			event_data = (TelSatSelectItemInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_SEND_SMS_IND:
			TelSatSendSmsIndUiInfo_t *event_data;
			event_data = (TelSatSendSmsIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_SEND_SS_IND:
			TelSatSendSsIndUiInfo_t *event_data;
			event_data = (TelSatSendSsIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_SEND_USSD_IND:
			TelSatSendUssdIndUiInfo_t *event_data;
			event_data = (TelSatSendUssdIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_SETUP_CALL_IND:
			TelSatSetupCallIndUiInfo_t *event_data;
			event_data = (TelSatSetupCallIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_PROVIDE_LOCAL_INFO_IND:
			TelSatProvideLocalInfoInd_t *event_data;
			event_data = (TelSatProvideLocalInfoInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SETUP_IDLE_MODE_TEXT_IND:
			TelSatSetupIdleModeTextInd_t *event_data;
			event_data = (TelSatSetupIdleModeTextInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_SEND_DTMF_IND:
			TelSatSendDtmfIndUiInfo_t *event_data;
			event_data = (TelSatSendDtmfIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_LANGUAGE_NOTIFICATION_IND:
			TelSatLanguageNotiInfoInd_t *event_data;
			event_data = (TelSatLanguageNotiInfoInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_UI_LAUNCH_BROWSER_IND:
			TelSatLaunchBrowserIndUiInfo_t *event_data;
			event_data = (TelSatLaunchBrowserIndUiInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_END_PROACTIVE_SESSION_IND:
			TelSatEndProactiveSessionIndInfo_t *event_data;
			event_data = (TelSatEndProactiveSessionIndInfo_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_MENU_SELECTION_CNF:
			TelSatEnvelopeResp_t *event_data;
			event_data = (TelSatEnvelopeResp_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_CALL_CONTROL_IND:
			TelSatCallCtrlIndData_t *event_data;
			event_data = (TelSatCallCtrlIndData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_MO_SMS_CONTROL_IND:
			TelSatMoSmCtrlIndData_t *event_data;
			event_data = (TelSatMoSmCtrlIndData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF:
			TelSatEnvelopeResp_t *event_data;
			event_data = (TelSatEnvelopeResp_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SMS_REFRESH_IND:
		case TAPI_EVENT_SAT_PB_REFRESH_IND:
		case TAPI_EVENT_SAT_IDLE_REFRESH_IND:
			TelSatRefreshInd_t *event_data;
			event_data = (TelSatRefreshInd_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SEND_SMS_IND:
			TelSatSendSmsIndSmsData_t *event_data;
			event_data = (TelSatSendSmsIndSmsData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SETUP_CALL_IND:
			TelSatSetupCallIndCallData_t *event_data;
			event_data = (TelSatSetupCallIndCallData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SEND_SS_IND:
			TelSatSendSsIndSsData_t *event_data;
			event_data = (TelSatSendSsIndSsData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SEND_USSD_IND:
			TelSatSendUssdIndUssdData_t *event_data;
			event_data = (TelSatSendUssdIndUssdData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_SEND_DTMF_IND:
			TelSatSendDtmfIndDtmfData_t *event_data;
			event_data = (TelSatSendDtmfIndDtmfData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_LAUNCH_BROWSER_IND:
			TelSatLaunchBrowserIndBrowserData_t *event_data;
			event_data = (TelSatLaunchBrowserIndBrowserData_t *)event->pData;
			break;

		case TAPI_EVENT_SAT_END_OF_APP_EXEC_IND:
			TelSatProactiveCmdEndIndInfo_t *event_data;
			event_data = (TelSatProactiveCmdEndIndInfo_t *)event->pData;
			break;

		default:
			break;
	}
}
@endcode
@}
@defgroup Use_Cases7_2 Main Menu Information
@ingroup Use_Cases7_SAT
@{


		<h3 class="pg">Main Menu Information</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

Telephony provides APIs for retreiving SAT Main Menu Information.


@code
int tel_get_sat_main_menu_info(TelSatSetupMenuInfo_t* pMainMenu)
@endcode

SAMPLE CODE:
@code
//SAT MAIN MENU INFORMATION
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSat.h>

void mainmenuinformation()
{
	int i = 0;
	int ReturnStatus;
	TelSatSetupMenuInfo_t SatMainMenuInfo;

	ReturnStatus = tel_get_sat_main_menu_info(&SatMainMenuInfo);
	if (ReturnStatus == TAPI_API_SUCCESS)
	{
		printf(" SAT Main Menu Information \n");
		printf("Main Menu Title  :%s\n", satMainMenuInfo.satMainTitle);
		printf("Number of SAT Menu Items :%d\n", satMainMenuNum);
		printf("Sat Menu Items are:\n");
		for (i=0; i<SatMainMenuInfo.satMainMenuNum; i++)
			printf(" %s \n", SatMainMenuInfo.satMainMenuItem[i]);
	}
	else
		printf("ReturnStatus =%d\n", ReturnStatus);
}
@endcode
@}
@defgroup Use_Cases7_3 Envelope Commands
@ingroup Use_Cases7_SAT
@{


		<h3 class="pg">Envelope Commands</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

This API sends a USIM application toolkit command to the USIM, using the envelope mechanism of USIM application toolkit. (See GSM TS 31.111)


@code
int tel_select_sat_menu(TelSatMenuSelectionReqInfo_t* pMenuSelect, int* pRequestId);
int tel_download_sat_event(TelSatEventDownloadReqInfo_t*  pEventData, int* pRequestId);
@endcode

SAMPLE CODE:
@code
//MENU SELECTION ENVELOPE
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSat.h>

//Precondition
//SUBSCRIBE TO SAT EVENT(TAPI_EVENT_SAT_MENU_SELECTION_CNF) AS MENTIONED IN //Event Register and Deregister

void satmenuselectionenvelope()
{
	int RequestId = -1;
	int ReturnStatus;
	TelSatMenuSelectionReqInfo_t SatMenuSelect;

	SatMenuSelect.itemIdentifier = 1;
	SatMenuSelect.bIsHelpRequested = FALSE;

	ReturnStatus = tel_select_sat_menu(&SatMenuSelect, &RequestId);
	if (ReturnStatus != TAPI_API_SUCCESS)
	{
		printf("ReturnStatus =%d\n", ReturnStatus);
	}
	else
	{
		//WAIT FOR TAPI_EVENT_SAT_MENU_SELECTION_CNF EVENT HERE
	}
}

//CALLBACK FUNCTION TO BE USED
static void SatAppCallback (TelTapiEvent_t *event)
{
	int EventClass = event->EventClass;
	int EventType = event->EventType;
	int RequestId = event->RequestId;
	void* EventData = event->pData;
	int Status = event->Status;

	if (EventType == TAPI_EVENT_SAT_MENU_SELECTION_CNF)
	{
		printf(" TAPI_EVENT_SAT_MENU_SELECTION_CNF Event Received \n");
	}
}
@endcode

@code
//EVENT DOWNLOAD ENVELOPE
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSat.h>


//Precondition
//SUBSCRIBE TO SAT EVENT(TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF) AS MENTIONED IN //Event Register and Deregister
void eventdownloadenvelope()
{
	int RequestId = -1;
	int ReturnStatus;
	TelSatEventDownloadReqInfo_t SatEventData;

	SatEventData.eventDownloadType = TAPI_EVENT_SAT_DW_TYPE_IDLE_SCREEN_AVAILABLE;

	SatEventData.u.bIdleScreenAvailable = TRUE;

	ReturnStatus = tel_download_sat_event(&SatEventData, &RequestId);
	if (ReturnStatus != TAPI_API_SUCCESS)
	{
		printf("ReturnStatus =%d\n", ReturnStatus);
	}
	else
	{
		//WAIT FOR TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF EVENT HERE
	}
}

//CALLBACK FUNCTION TO BE USED
static void  SatAppCallback  (TelTapiEvent_t *event)
{
	int EventClass = event->EventClass;
	int EventType = event->EventType;
	int RequestId = event->RequestId;
	void* EventData = event->pData;
	int Status = event->Status;

	if (EventType == TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF)
	{
		printf("TAPI_EVENT_SAT_EVENT_DOWNLOAD_CNF Event Received \n");
		//Extract Event Data here
	}
}
@endcode
@}
@defgroup Use_Cases7_4 Application Execution result
@ingroup Use_Cases7_SAT
@{


		<h3 class="pg">Application Execution result</h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

Telephony provides this API to get the Application execution result.


@code
Int tel_send_sat_app_exec_result(TelSatAppsRetInfo_t* pAppRetInfo);
@endcode

SAMPLE CODE:
@code
//SAT MAIN MENU TITLE
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSat.h>

void sendappexecutionresult()
{
	int ReturnStatus;
	TelSatAppsRetInfo_t SatAppRetInfo;

	SatAppRetInfo.commandType = TAPI_SAT_CMD_TYPE_SETUP_IDLE_MODE_TEXT;
	SatAppRetInfo.appsRet.setupIdleModeText.resp = TAPI_SAT_R_SUCCESS;

	ReturnStatus = tel_send_sat_app_exec_result(&SatAppRetInfo);
	if (ReturnStatus == TAPI_API_SUCCESS)
	{
		printf(" Sending Application Execution Result Successful \n");
	}
	else
		printf("ReturnStatus =%d\n", ReturnStatus);
}
@endcode
@}
@defgroup Use_Cases7_5  UI /User Confirmation
@ingroup Use_Cases7_SAT
@{


		<h3 class="pg">UI /User Confirmation </h3>
<strong>[Note] Telephony Emulator does not support this feature.</strong>

Send user’s confirmation message to the USIM application


@code
int tel_send_sat_ui_display_status(int commandId, TelSatUiDisplayStatusType_t status);
int tel_send_sat_ui_user_confirm(TelSatUiUserConfirmInfo_t * pUserConfirmData) ;
@endcode

SAMPLE CODE:
@code
//UI DISPLAY STATUS
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSat.h>

void senduidisplaystatus()
{
	int ReturnStatus;
	TelSatUiDisplayStatusType_t DisplatStatus;
	int CommandId;

	DisplatStatus = TAPI_SAT_DISPLAY_SUCCESS;
	CommandId = commandId; //Command Id of the Proactive Command

	ReturnStatus = tel_send_ui_display_status(CommandId, DisplayStatus);
	if (ReturnStatus == TAPI_API_SUCCESS)
		printf(" Sending UI Display Status Successful \n");
	else
		printf("ReturnStatus =%d\n", ReturnStatus);
}
@endcode

@code
//USER CONFIRMATION
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiSat.h>

void senduiuserconfirm()
{
	int ReturnStatus;
	TelSatUiUserConfirmInfo_t UserConfirmData;

	UserConfirmData.commandId = commandId ; //Command Id of the Proactive Command

	UserConfirmData.commandType = TAPI_SAT_CMD_TYPE_PLAY_TONE;
	UserConfirmData.keyType = TAPI_SAT_USER_CONFIRM_YES;

	UserConfirmData.pAdditionalData = NULL;
	UserConfirmData.dataLen = 0;

	ReturnStatus = tel_send_sat_ui_user_confirm(&UserConfirmData);
	if (ReturnStatus == TAPI_API_SUCCESS)
		printf(" Sending User Confirmation Data Successful \n");
	else
		printf("ReturnStatus =%d\n", ReturnStatus);
}
@endcode
@}


	<h2 class="pg">GPRS</h2>
@defgroup Use_Cases8_1 Event Register and Deregister
@ingroup Use_Cases8_GPRS
@{

		<h3 class="pg">Event Register and Deregister</h3>
To receive asynchronous event notifications for GPRS APIs, the registration of related events with their associated call backs is required. When an asynchronous API is called, the confirmation is returned as an asynchronous event notification which invokes the callback function along with the event data.

SAMPLE CODE:
@code
// REGISTER EVENT
unsigned int subscription_id = 0;
int api_err;

api_err = tel_init();
if (api_err != 0)
{
      printf ("tel_init Failed - api_err = %d \n", api_err);
}

api_err = tel_register_event(TAPI_EVENT_PS_PDP_ACT_RSP, &subscription_id , (TelAppCallback)&app_callback);
printf("Power Event registeration is Done: sub id is %d, api_err is %d\n", subscription_id, api_err);

tel_register_app_name("org.tizen.appname");

void app_callback(TelTapiEvent_t *event)
{
	//callback function to be called
}
@endcode
@}
@defgroup Use_Cases8_2 Pdp Activation
@ingroup Use_Cases8_GPRS
@{


		<h3 class="pg">Pdp Activation</h3>
This API is used to start a GPRS session successfully.

@code
int tel_activate_gprs_pdp (const tapi_ps_net_start_req_t *pNet_start_req_info,int * pRequestID);
@endcode

<strong>This API is restricted for direct use. Use the Data Network API instead.</strong>

SAMPLE CODE
@code
#include<stdio.h>
#include <TapiCommon.h>
#include <ITapiPS.h>

int ret_status;
tapi_ps_net_start_req_t start_req;
int request_id = 0;

#define CONTEXT_ID 1
#define APN_ADDRESS "nate.sktelecom.com"

void gprsstart()
{
	int ret_status;
	tapi_ps_net_start_req_t start_req;
	int request_id = 0;

	start_req.cont_id = CONTEXT_ID;
	strcpy(start_req.pdp_info.apn , APN_ADDRESS);

	ret_status = tel_activate_gprs_pdp (&start_req, &request_id);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("/n/  gprs mode Started successfully \\n");
	}
	else
	{
		printf("/n/ gprs mode not started \\n");
	}
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
void  ps_app_callback  (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void *EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;
	printf(" app _callback is called \n");

	switch (eventClass)
	{
		case TAPI_EVENT_CLASS_DATA:
			switch (eventType)
			{
				case TAPI_EVENT_PS_PDP_ACT_RSP:
					tapi_ps_net_activation_rsp_t  pdp_ip_resp;
					memcpy(&pdp_ip_resp, psEvent->pData, sizeof(tapi_ps_net_activation_rsp_t));
					printf("TAPI_EVENT_PS_PDP_ACT_RSP\n");
					break;

				case TAPI_EVENT_PS_PDP_ACT_IPCONFIG_INFO:
					tapi_ps_net_start_rsp_t  pdp_ip_resp;
					memcpy(&pdp_ip_resp, psEvent->pData, sizeof(tapi_ps_net_start_rsp_t));

				case TAPI_EVENT_PS_STATUS:
					tapi_ps_attach_response_t res;
					memcpy(&res, psEvent->pData, sizeof(tapi_ps_attach_response_t));
			}
	}
}
@endcode
@}
@defgroup Use_Cases8_3 Check PDP Signal line On
@ingroup Use_Cases8_GPRS
@{


		<h3 class="pg">Check PDP Signal line On</h3>
This API is used to check whether the PDP siganl line is on or not.

Precondition:
@n GPRS session should be started before running this API.

@code
int tel_control_gprs_btdun_pin (tapi_ps_btdun_pincontrol pincontrol,int * pRequestID);
@endcode

<strong>This API is restricted for direct use. Use the Data Network API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiPS.h>

void pdpbtduncontrolget()
{
	int ret_status;
	int request_id = 0;
	tapi_ps_btdun_pincontrol bt_control;

	bt_control.signal = GPRS_SIGNAL_DCD ;
	bt_control.status = GPRS_SIGNAL_STATUS_OFF;

	ret_status = tel_control_gprs_btdun_pin(bt_control, &request_id) ;
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("/n/  Successful \\n");
	}
	else
	{
		printf("/n/error:%d \n", ret_status);
	}
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
void  ps_app_callback  (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void *EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;
	printf(" app _callback is called \n");

	switch (eventClass)
	{
		case TAPI_EVENT_CLASS_DATA:
			switch (eventType)
			{
				case TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI:
					tapi_ps_btdun_pincontrol pincontrol;
					memcpy(&pincontrol, psEvent->pData, sizeof(tapi_ps_btdun_pincontrol));
					printf("TAPI_EVENT_PS_BTDUN_PINCONTROL_NOTI\n");
					break;
			}
	}
}
@endcode
@}
@defgroup Use_Cases8_4 Set PDP deactivation
@ingroup Use_Cases8_GPRS
@{


		<h3 class="pg">Set PDP deactivation</h3>
This API is used to deactivate the PDP service.

Precondition:
@n GPRS session must be started before executing this API.

@code
int     tel_deactivate_gprs_pdp (const tapi_ps_net_stop_req_t *net_stop_req_info,int * pRequestID);
@endcode

<strong>This API is restricted for direct use. Use the Data Network API instead.</strong>

SAMPLE CODE
@code
#include <stdio.h>
#include <TapiCommon.h>
#include <ITapiPS.h>

#define CONTEXT_ID 1

void setpdpdeactivation()
{
	int ret_status;
	tapi_ps_net_stop_req_t end_req;
	int request_id = 0;

	memset(&end_req, 0, sizeof(tapi_ps_net_stop_req_t));

	end_req.cont_id = CONTEXT_ID;

	ret_status = tel_deactivate_gprs_pdp (&end_req, &request_id);
	if (ret_status == TAPI_API_SUCCESS)
	{
		printf("/n/ gprs active session ended successfully \\n");
	}
	else
	{
		printf("/n/  gprs active session not ended  \\n");
	}
}
//WAIT FOR EVENT HERE

//CALLBACK FUNCTION TO BE USED
void  ps_app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void *EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;
	printf(" app _callback is called \n");

	switch (eventClass)
	{
		case TAPI_EVENT_CLASS_DATA:
			switch (eventType)
			{
				case TAPI_EVENT_PS_PDP_DEACT_IND:
					tapi_ps_net_stop_rsp_t* pdp_stop_resp;
					memcpy(pdp_stop_resp, psEvent->pData, sizeof(tapi_ps_net_stop_rsp_t));
					printf("TAPI_EVENT_PS_PDP_DEACT_IND\n");
					break;

				case TAPI_EVENT_PS_CALL_STATISTICS:
					tapi_ps_call_statistics_t call_stats;
					memcpy(&call_stats, psEvent->pData,
							sizeof(tapi_ps_call_statistics_t));
					break;
			}
			break;
	}
	break;
}
@endcode
@}
@defgroup Appendixes1 Appendix A. Sample Codes
@ingroup Appendix
@{
<h1 class="pg">Appendixes</h1>
	<h2 class="pg">Appendix A. Sample Codes</h2>
Some reference sample code is attached to help the application developer understand the telephony Framework Module

How to use supplementary service APIs and handle SS events
@code
#include <ITapiSs.h>
#include <TapiEvent.h>
#include <TapiCommon.h>
#include <TelUtility.h>

GMainLoop *nw_context_loop = NULL;

typedef struct
{
	int group;
	int type;
	int len;
	void *dst;
} data_t;

static int SS_Callback(int eventClass, int eventType, void * param2)
{
	int i;
	printf ("\n\n\n SS_Callback is called! eventClass : %d\n, eventType : %d\n", eventClass, eventType);
	fflush(stdout);
	switch (eventType)
	{
		case TAPI_EVENT_SS_REL_COMP_MSG_NOTIFY:
			if (param2 != NULL) {
				int x = 0;
				TelSsRelCompMsgInfo_t rel_comp_msg;
				memcpy(&rel_comp_msg, (TelSsRelCompMsgInfo_t *)param2,
						sizeof(TelSsRelCompMsgInfo_t));
				printf("Printing the Release Complete Message \n");

				for (x=0; x<rel_comp_msg.RelCompMsgLen; x++)
				{
					printf("[%.2x]", rel_comp_msg.szRelCompMsg[x]);
				}
			}// end if
			break;

		case TAPI_EVENT_SS_WAITING_CNF:
			printf("####    Received Event : TAPI_EVENT_SS_WAITING_CNF  ##### \n");
			if (param2 == NULL) {
				printf("param2 is NULL\n");
			}
			else {
				TelSsInfo_t SsInfo;
				memcpy(&SsInfo, (TelSsInfo_t*)param2, sizeof(TelSsInfo_t));
				for (i=0; i<SsInfo.NumberOfRecords; i++)
				{
					printf("Number of records %d \n", SsInfo.NumberOfRecords);
					printf("Record:%d Status:%d Teleservice:%d \n",
							i,
							sInfo.SsRecord.WaitingRecord.rec_class[i].Status,
							SsInfo.SsRecord.WaitingRecord.rec_class[i].TeleCommService);
					if (SsInfo.SsRecord.WaitingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
						printf("####    Status: Active      #### \n");
					else
						printf("####     Status: Not Active   #### \n");
				} //end for
			}//end else
			break;

		case TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF:
			printf("####    Received vent :TAPI_EVENT_SS_WAITING_QUERYSTATUS_CNF  ##### \n");
			if (param2 == NULL) {
				printf("\n paran 2 is NULL\n");
				break;
			}
			TelSsInfo_t SsInfo;
			memcpy(&SsInfo, (TelSsInfo_t*)param2, sizeof(TelSsInfo_t));
			for (i=0; i<SsInfo.NumberOfRecords; i++) {
				printf("Number of records %d \n",SsInfo.NumberOfRecords);
				printf("Record:%d Status:%d Teleservice:%d \n",
						i,
						SsInfo.SsRecord.WaitingRecord.rec_class[i].Status,
						SsInfo.SsRecord.WaitingRecord.rec_class[i].TeleCommService);

				if (SsInfo.SsRecord.WaitingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
					printf("####    Status: Active      #### \n");
				else
					printf("####     Status: Not Active   #### \n");
			}//end for
			break;

		case TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF:
			printf("####    Received Event :TAPI_EVENT_SS_BARRING_QUERYSTATUS_CNF  ##### \n");
			break;

		case TAPI_EVENT_SS_BARRING_CNF:
			printf("####    Received Event :TAPI_EVENT_SS_BARRING_CNF   ##### \n");
			break;

		case TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF:
			printf("####    Received Event : TAPI_EVENT_SS_BARRING_CHANGEPASS_CNF  ##### \n");
			printf("Call barring Password Changed successfully\n");
			break;

		case TAPI_EVENT_SS_FORWARD_CNF:
			printf("####    Received Event :TAPI_EVENT_SS_FORWARD_CNF  ##### \n");
			if (param2 != NULL) {
				TelSsInfo_t SsInfo;
				memset(&SsInfo, 0, sizeof(TelSsInfo_t));
				memcpy(&SsInfo, (TelSsInfo_t*)param2, sizeof(TelSsInfo_t));
				printf("Number of records %d \n", SsInfo.NumberOfRecords);
				for (i=0; i<SsInfo.NumberOfRecords; i++)
				{
					printf("Number of records %d\n", SsInfo.NumberOfRecords);
					printf("Record:%d Status:%d Teleservice:%d \n",
							i,
							SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status,
							SsInfo.SsRecord.ForwardingRecord.rec_class[i].TeleCommService);
					if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_REGISTERED)
						printf("####    Status: Registered      #### \n");
					else if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
						printf("####     Status: Active   #### \n");
					else
						printf("####    Status: Not Registered /Not Active     ####\n");
				}//end for
			}
			else
			{
				printf("param2 is NULL\n");
			}
			break;

		case TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF:
			printf("####    Received Event : TAPI_EVENT_SS_FORWARD_QUERYSTATUS_CNF  ##### \n");
			if (param2 != NULL) {
				TelSsInfo_t SsInfo;
				memcpy(&SsInfo, (TelSsInfo_t*)param2, sizeof(TelSsInfo_t));
				for (i=0; i<SsInfo.NumberOfRecords; i++)
				{
					printf("Number of records %d \n", SsInfo.NumberOfRecords);
					printf("Record:%d Status:%d Teleservice:%d \n",
							i,
							SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status,
							SsInfo.SsRecord.ForwardingRecord.rec_class[i].TeleCommService);
					if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_REGISTERED)
						printf("####    Status: Registered      #### \n");
					else if (SsInfo.SsRecord.ForwardingRecord.rec_class[i].Status == TAPI_SS_STATUS_ACTIVE)
						printf("####     Status: Active   #### \n");
					else
						printf("####    Status: Not Registered /Not Active     #### \n");
				}//end for
			}//end if
			else
			{
				printf("param 2 is NULL\n");
			}
			break;

		case TAPI_EVENT_SS_CLI_QUERYSTATUS_CNF:
			printf("####    Received Event : TAPI_EVENT_SS_IDENTIFY_IND #### \n");
			if (param2 != NULL) {
				TelSsInfo_t SsInfo;
				memcpy(&SsInfo, (TelSsInfo_t*)param2, sizeof(TelSsInfo_t));
				printf("Status :%d   CLI Service : %d \n",
						SsInfo.SsRecord.CliRecord.CliStatus,
						SsInfo.SsRecord.CliRecord.IdentificationType);
				if (SsInfo.SsRecord.CliRecord.CliStatus == 1)
					printf(" #####    Status : Not Provisioned   #####\n");
				else if (SsInfo.SsRecord.CliRecord.CliStatus == 2)
					printf(" #####    Status :  Provisioned  but Deactivated #####\n");
				else if (SsInfo.SsRecord.CliRecord.CliStatus == 3)
					printf(" #####    Status : Provisioned  and Activated   #####\n");
				else if (SsInfo.SsRecord.CliRecord.CliStatus == 4)
					printf(" #####    Status : Unknown   #####\n");
				else if (SsInfo.SsRecord.CliRecord.CliStatus == 5)
					printf(" #####    Status : Temporarily Restricted  #####\n");
				else if (SsInfo.SsRecord.CliRecord.CliStatus == 6)
					printf(" #####    Status : Temporarily Allowed  #####\n");
			}
			else
			{
				printf("param 2 is NULL\n");
			}
			break;

		default:
			printf("\n\nDID NOT GET THE EVENT\n");
			break;
	}//end switch
	g_main_loop_quit(nw_context_loop);
	return 0;
}//end call back

static void app_callback (TelTapiEvent_t *event)
{
	int eventClass;
	int eventType;
	int requestId;
	int status;
	void* EventData = NULL;

	eventClass = event->EventClass;
	eventType = event->EventType;
	requestId = event->RequestId;
	status = event->Status;
	EventData = event->pData;
	//TEST_DEBUG("Class:[%d], Event Type:[%d], RequestId:[%d], Status:[%d] \n", eventClass, eventType , requestId, status );
	if (status != 0)
	{
		//TEST_DEBUG("******This is nonzero status. ******\n");
		return;
	}
	SS_Callback((int)eventClass, (int)eventType, EventData);
}

ss_test_subscribe_tapi_events()
{
	//REGISTER EVENT
	unsigned int subscription_id = 0;
	TapiResult_t api_err;

	api_err = TelTapiInit();
	if (api_err != TAPI_API_SUCCESS)
	{
		printf("TelTapiInit Failed - api_err = %d \n", api_err);
	}
	api_err = TelTapiRegisterEvent(TAPI_EVENT_SS_WAITING_CNF, &subscription_id, (TelAppCallback)&app_callback);
	printf("SS Class registeration is Done: sub id is %d, api_err is %d\n",subscription_id,api_err);
	fflush(stdout);
}

noti_finish()
{
	//DEREGISTER EVENT
	unsigned int subscription_id = 0;
	TapiResult_t api_err;

	api_err = tel_deregister_event(subscription_id);
	if (api_err != TAPI_API_SUCCESS)
	{
		printf("Event Class Unregeration Fail\n");
	}
}

tapi_ss_call_barr()
{
	TapiResult_t ret_status;
	TelSsCallBarringInfo_t pBarringInfo;
	int pRequestId = 0;
	char *bar_pwd = "0000";

	printf("Before copying ,,,length is %d\n", strlen(pBarringInfo.szPassword));
	printf("len initially is %d\n", strlen(bar_pwd));

	pBarringInfo.Mode = TAPI_SS_CALL_BAR_ACTIVATE;
	pBarringInfo.Type = TAPI_CALL_BARRING_ALL_OUTGOING_CALLS;//TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL;
	pBarringInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;
	strcpy(pBarringInfo.szPassword, bar_pwd);
	printf("length is %d\n", strlen(pBarringInfo.szPassword));
	ret_status = tel_set_ss_barring(&pBarringInfo, &pRequestId);
	if (ret_status != TAPI_API_SUCCESS) {
		return;
	}

	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);


	//now , get the status
	TelSsCallBarType_t BarType = TAPI_CALL_BARRING_ALL_OUTGOING_INTERN_CALL;
	TelSsCallType_t CallType = TAPI_CALL_TYPE_VOICE_EV;
	ret_status = tel_get_ss_barring_status(BarType,CallType, &pRequestId);
	if (ret_status != TAPI_API_SUCCESS) {
		return ;
	}

	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);
}

tapi_ss_call_fwd()
{
	TapiResult_t ret_status;
	TelSsForwardInfo_t pForwardInfo;
	int len;
	int pRequestId = 0;
	char *cf_number = "9986529874";

	pForwardInfo.Mode = TAPI_CALL_FORWARD_MODE_ENABLE_EV;
	pForwardInfo.Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	pForwardInfo.Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;
	pForwardInfo.NoReplyConditionTimer = 10;

	len = strlen(cf_number);
	printf("Length of CF number :%d \n", len);
	memcpy(&(pForwardInfo.szPhoneNumber), cf_number, len);
	ret_status = tel_set_ss_forward(&pForwardInfo, &pRequestId);
	if (ret_status != TAPI_API_SUCCESS) {
		return ;
	}

	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);

	//now , get call forward status
	TelSsForwardType_t Type = TAPI_CS_FORWARD_TYPE_VOICE_EV;
	TelSsForwardWhen_t Condition = TAPI_SS_FORWARD_WHEN_UNCONDITIONAL_EV;

	// Calling TAPI SS Call Frwd Query Req Status Api
	ret_status = tel_get_ss_forward_status(Type,Condition, &pRequestId);
	if (ret_status != TAPI_API_SUCCESS) {
		return ;
	}

	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);
}

tapi_ss_call_wait()
{
	//set and get call waiting
	TapiResult_t ret_status;
	TelSsWaitingInfo_t waitInfo;
	int RequestID;

	waitInfo.Mode = TAPI_SS_CW_ACTIVATE;
	waitInfo.CallType = TAPI_CALL_TYPE_VOICE_EV;

	//set call wait
	ret_status = tel_set_ss_waiting(&waitInfo, &RequestID);
	if (ret_status != TAPI_API_SUCCESS) {
		return;
	}

	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);

	sleep(4);

	//now get call wait
	TelSsCallType_t CallType = TAPI_CALL_TYPE_VOICE_EV;
	ret_status = tel_get_ss_waiting_status(CallType, &RequestID);
	if (ret_status != TAPI_API_SUCCESS) {
		return;
	}
	nw_context_loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(nw_context_loop);
}

ss_test()
{
	ss_test_subscribe_tapi_events();
	tapi_ss_call_barr();
	tapi_ss_call_fwd();
	tapi_ss_call_wait();
	noti_finish();
}

int main(int argc, char *argv[])
{
	ss_test();
	return 0;
}
@endcode
@}
@defgroup Appendixes2 Appendix   B. Error Code
@ingroup Appendix
@{

	<h2 class="pg">Appendix B. Error Code</h2>
<table>
<tr><td>Error Num.</td>
<td>Error Code</td>
<td>Error Description</td></tr>
<tr><td>0</td>
<td>TAPI_API_SUCCESS</td>
<td>No Error occurred</td></tr>
<tr><td>-1</td>
<td>TAPI_API_INVALID_INPUT</td>
<td>Input values are not correct in TAPI Library</td></tr>
<tr><td>-2</td>
<td>TAPI_API_INVALID_PTR</td>
<td>invalid pointer</td></tr>
<tr><td>-3</td>
<td>TAPI_API_NOT_SUPPORTED</td>
<td>The feature corresponding to requested API is not supported. This may be due to market/network/vendor reasons such as the feature is not available in the network.</td></tr>
<tr><td>-4</td>
<td>TAPI_API_DEPRICATED </td>
<td>This API is deprecated and will be so in future also </td></tr>
<tr><td>-5</td>
<td>TAPI_API_SYSTEM_OUT_OF_MEM</td>
<td>Out of memory</td></tr>
<tr><td>-6</td>
<td>TAPI_API_SYSTEM_RPC_LINK_DOWN</td>
<td>RPC link down</td></tr>
<tr><td>-7</td>
<td>TAPI_API_SERVICE_NOT_READY          </td>
<td>Phone was powered on, but yet to receive the power up completed notification </td></tr>
<tr><td>-8</td>
<td>TAPI_API_SERVER_FAILURE</td>
<td>error occurred in Telephony server</td></tr>
<tr><td>-9</td>
<td>TAPI_API_OEM_PLUGIN_FAILURE</td>
<td>Plug-in layer failure</td></tr>
<tr><td>-10</td>
<td>TAPI_API_TRANSPORT_LAYER_FAILURE</td>
<td>Transport layer Failure</td></tr>
<tr><td>-11</td>
<td>TAPI_API_INVALID_DATA_LEN</td>
<td>Invalid data length</td></tr>
</table>
@}
*/
/**
* @defgroup  TELEPHONY
 @{
*	@defgroup Appendix Reference
*	@defgroup TELEPHONY_USECASES Use Cases
	@{
*		@defgroup Use_Cases1_COMMON COMMON
*		@defgroup Use_Cases2_CALL CALL
*		@defgroup Use_Cases3_SUPPLEMENTARY_SERVICE SUPPLEMENTARY_SERVICE
*		@defgroup Use_Cases4_SMS SMS
*		@defgroup Use_Cases5_NETWORK NETWORK_Registration_and_Configuration
*		@defgroup Use_Cases6_SOUND SOUND
*		@defgroup Use_Cases7_SAT SAT
*		@defgroup Use_Cases8_GPRS GPRS
	@}
 @}
*/
