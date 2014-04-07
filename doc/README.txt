LIBTAPI documentation:
======================

AD stands for Application Developper.

*-------------*
|A- TelHandle|
*-------------*

1- AD must create TelHandle to be able to use libtapi:

	TelHandle *handle;

	handle = tapi_init(NULL);

[refer to tapi.h for more info on tapi_init]


2- AD can also precise the CP he wants to work with:

	char **cp_list;
	TelReturn ret;

	ret = tapi_get_cp_list(&cp_list);

	if (ret != TEL_RETURN_SUCCESS)
		return;

	/* Use first CP from the list */
	if (cp_list[0])
		handle = tapi_init(cp_list[0]);

	g_strfreev(cp_list);

[refer to tapi.h for more info on tapi_get_cp_list]

3- AD must destroy TelHandle when closing application.

	tapi_deinit(handle);


*---------------------*
|B- Event registration|
*---------------------*

1- AD can register callback by evt_id:

void on_modem_power(TelHandle *handle, const char *evt_id,
					void *data, void *user_data)
{
	gboolean *enable = data;

	printf("modem power state %d", *enable);
}

int main(void)
{
	TelReturn ret;
	TelHandle *handle;

	handle = tapi_init(NULL);

	ret = tapi_register_event_id(handle, TEL_NOTI_MODEM_POWER,
					on_modem_power, NULL);

	/* Fail to register on event_id */
	if (ret != TEL_RETURN_SUCCESS) {
		tapi_deinit(handle);
		return -1;
	}

	...
}

[refer to tapi.h for TapiEvtCb prototype and for more info on
tapi_register_event_id()]

[refer to tapi_events.h to see possible event per modules and data structure
associated to event]

/* TapiEvtCb data: 'gooblean *' */
#define TAPI_PROP_MODEM_POWER				...
/* TapiEvtCb data: 'gooblean *' */
#define TAPI_PROP_MODEM_FLIGHT_MODE_STATUS		...
/* TapiEvtCb data: 'char *' */
#define TAPI_PROP_MODEM_IMEI				...
/* TapiEvtCb data: 'TelModemVersion *' */
#define TAPI_PROP_MODEM_VERSION				...

2- AD can unregister callback by evt_id:

	ret = tapi_deregister_event_id(handle, TEL_NOTI_MODEM_POWER);
	/* Fail to unregister event_id */
	if (ret != TEL_RETURN_SUCCESS)
		return -1;


*--------------*
|C- Module APIs|
*--------------*

libtapi is exposing APIs for following modules:
	- modem
	- sim
	- network
	- call
	- sms
	- phonebook
	- SS (Supplementary Services)
	- sat
	- sap
	- gps

Each module are exposing synchronous and Asynchronous APIs.

1- Synchronous APIs can only return TelReturn values, they are system results.
output argument for synchronous APIs has to be freed when it is specified in
header of API protoype.

[refer to tel_return.h for more details]

2- Asynchronous APIs have necessarily one input 'TapiResponseCb *cb', if not
it is a synchronous APIs.

Asynchronous APIs call can fail. In this case it will immediately return a
system error 'TelReturn' listed in tel_return.h, callback will never be called.

If API call doesn't fail, result will be sent through the response callback
within the result parameter.

typedef void (*TapiResponseCb)(TelHandle *handle, int result, void *data,
					void *user_data);

--> int result: it is specific for each module:
	* modem: TelReturn		[refer to tel_return.h]
	* sim: TelSimResult		[refer to tel_sim.h]
	* network: TelNetworkResult	[refer to tel_network.h]
	* call: TelCallResult		[refer to tel_call.h]
	* ...

--> void *data: it is specific for each callback, it is specified in header of
API prototype:
	* tapi_modem.h
	* tapi_sim.h
	* tapi_network.h
	* tapi_call.h
	* ...

See tapi_modem.h:
/* TapiResponseCb data: NULL pointer */
TelReturn tapi_modem_set_power_status(TelHandle *handle, TelModemPowerStatus status,
				TapiResponseCb callback, void *user_data);
/* TapiResponseCb data: NULL pointer */
TelReturn tapi_modem_set_flight_mode(TelHandle *handle, gboolean enable,
				TapiResponseCb callback, void *user_data);
