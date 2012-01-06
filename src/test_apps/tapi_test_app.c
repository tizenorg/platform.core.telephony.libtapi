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
#include <string.h>
#include <sys/time.h>
#include <pthread.h>     /* pthread functions and data structures */
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <glib.h>

#include "tapi_test_utility.h"
#include "TapiCommon.h"

#define TAPI_TEST_MAX_LIST		10

#define TAPI_TEST_COMMON_ID		1
#define TAPI_TEST_PWR_ID		2
#define TAPI_TEST_CALL_ID		3
#define TAPI_TEST_SIM_ID		4
#define TAPI_TEST_SS_ID			5
#define TAPI_TEST_SMS_ID		6
#define TAPI_TEST_NW_ID			7
#define TAPI_TEST_SAT_ID		8
#define TAPI_TEST_DATA_ID		9
#define TAPI_TEST_MISC_ID		10
#define TAPI_TEST_SVC_ID		11
#define TAPI_TEST_GPS_ID		12
#define TAPI_TEST_SOUND_ID		13
#define TAPI_TEST_CDMA_DATA_ID	14
#define TAPI_TEST_CFG_ID		15
#define TAPI_TEST_QUIT_ID		0

int EvtDeliveryHandle = 0;

/******************************************************************************
*******************************************************************************
*
*							S U B  M O D U L E   A P I's
*
******************************************************************************
******************************************************************************/
/*	Add the Keyboard Loop Routines here...	*/
void power_select_loop(void);
int call_test_init(void);
void sim_select_loop(void);
void ss_select_loop(void);
void sms_select_loop();
void nw_select_loop(int option);
void gps_select_loop();
void misc_select_loop(int option);
void sat_select_loop(void);
void data_select_loop(void);
int svc_select_loop(void);
void sound_select_loop(void);
void cdma_data_select_loop(void);
void cfg_select_loop(void);


/*	Add the Subscribe routines here....	*/
int power_test_subscribe_tapi_events (void);
int call_test_subscribe_tapi_events (void);
int nw_test_subscribe_tapi_events(void);
int gps_test_subscribe_tapi_events(void);
int misc_test_subscribe_tapi_events(void);
int cfg_test_subscribe_tapi_events(void);
int data_test_subscribe_tapi_events (void);
int sat_test_subscribe_tapi_events(void);
int sim_test_subscribe_tapi_events (void);
int svc_test_subscribe_tapi_events(void);
int ss_test_subscribe_tapi_events(void);
int sms_subscribe_tapi_events();
void cdma_data_test_subscribe_tapi_events (void);
int sound_test_subscribe_tapi_events(void);

/******************************************************************************
*******************************************************************************
*
*							G L O B A L   V A R I A B L E S
*
******************************************************************************
******************************************************************************/



/******************************************************************************
*******************************************************************************
*
*						T A P I   T E S T   A P I's
*
*
******************************************************************************
******************************************************************************/


//1 GMain Loop Routine.



/*	Wait in the GMain Loop	to receive notifications   */
void  tapi_test_app_noti_loop(void)
{
	GMainLoop *mainloop;

	mainloop = g_main_loop_new (NULL, FALSE);

	TAPI_TEST_DEBUG("Entering GMain Loop to Receive Notifications....\n");

	g_main_loop_run (mainloop);

	TAPI_TEST_DEBUG("EXITING GMAIN LOOP\n");

	g_main_loop_unref(mainloop);

	tel_deinit();
}




//2 Call the sub module Key Event Loop sub routines on demand by the User.


/*	Call the selected sub module routine..	*/
int tapi_test_app_process_choice(int sub_mod_id)
{
	int ret = 1;

	switch(sub_mod_id)
	{
		case TAPI_TEST_COMMON_ID:
		case TAPI_TEST_PWR_ID:
			TAPI_TEST_DEBUG("Loading Power Module Test Routines...\n");
			power_select_loop();
			break;
		case TAPI_TEST_CALL_ID:
			TAPI_TEST_DEBUG("Loading Common/Power&Display/Call Module Test Routines...\n");
			call_test_init();
			TAPI_TEST_DEBUG(" sub-module testing done.. \n");
			break;
		case TAPI_TEST_SIM_ID:
			TAPI_TEST_DEBUG("Loading SIM Module Test Routines...\n");
			sim_select_loop();
			TAPI_TEST_DEBUG(" SIM sub-module testing done.. \n");
			break;

		case TAPI_TEST_SS_ID:
			TAPI_TEST_DEBUG("Loading SS Module Test Routines...\n");
			//ss_select_loop();
			TAPI_TEST_DEBUG(" SS sub-module testing done.. \n");
			break;

		case TAPI_TEST_SMS_ID:
			TAPI_TEST_DEBUG("Loading SMS module test routines...\n");
			sms_select_loop();
			TAPI_TEST_DEBUG(" SMS sub-module testing done.. \n");
			break;

		case TAPI_TEST_NW_ID:
			TAPI_TEST_DEBUG("Loading NW Module Test Routines...\n");
			nw_select_loop(TAPI_TEST_NW_ID);
			TAPI_TEST_DEBUG("NW sub-module testing done.. \n");
			break;
#if 0
		case TAPI_TEST_SAT_ID:
			TAPI_TEST_DEBUG("Loading SAT Module Test Routines...\n");
			sat_select_loop();
			TAPI_TEST_DEBUG("SAT sub-module testing done.. \n");
			break;
#endif
		case TAPI_TEST_DATA_ID:
			TAPI_TEST_DEBUG("Loading Data Module Test Routines...\n");
			data_select_loop();
			TAPI_TEST_DEBUG("Data sub-module testing done.. \n");
			break;

		case TAPI_TEST_MISC_ID:
			TAPI_TEST_DEBUG("Loading MISC Module Test Routines...\n");
			misc_select_loop(TAPI_TEST_MISC_ID);
			TAPI_TEST_DEBUG("MISC sub-module testing done.. \n");
			break;

		case TAPI_TEST_SVC_ID:
			TAPI_TEST_DEBUG("Loading SVC Module Test Routines...\n");
			svc_select_loop();
			TAPI_TEST_DEBUG("SVC sub-module testing done.. \n");
			break;

		case TAPI_TEST_GPS_ID:
			TAPI_TEST_DEBUG("Loading GPS Module Test Routines...\n");
			gps_select_loop();
			TAPI_TEST_DEBUG("GPS sub-module testing done.. ");
			break;

		case TAPI_TEST_SOUND_ID:
			TAPI_TEST_DEBUG("Loading Sound Module Test Routines...\n");
			sound_select_loop();
			TAPI_TEST_DEBUG("Sound sub-module testing done.. ");
			break;

		case TAPI_TEST_CDMA_DATA_ID:
			TAPI_TEST_DEBUG("Loading CDMA Data Module Test Routines...\n");
			cdma_data_select_loop();
			TAPI_TEST_DEBUG("CDMA Data sub-module testing done.. ");
			break;

		case TAPI_TEST_CFG_ID:
			TAPI_TEST_DEBUG("Loading CFG Module Test Routines...\n");
			TAPI_TEST_DEBUG("CFG sub-module testing done.. ");
			break;

		case TAPI_TEST_QUIT_ID:
			TAPI_TEST_DEBUG("Quit option selected. Aborting the application execution");
			tel_deinit();
			exit(0);
			break;

		default:
			TAPI_TEST_DEBUG("Oops!Invalid Sub Module Id Pressed. Try Again");
			ret = -1;
			break;

	}


	return ret;

}


//2 Display the Main Menu Options for the user


void tapi_test_app_process_keyboard(void)
{
	int	choice = 0;
	char buf[255];

	while (1)
	{
		sleep(1);

		choice = 0;

		printf("\n\nMain Menu: \n\n");

		printf("Avaliable Sub-Modules for API testing...\n");

		printf("\n\n *********************************************************\n\n");

		printf(" 1:\tCommon\n 2:\tPower & Display\n 3:\tCall\n 4:\tSIM\n 5:\tSS\n 6:\tSMS\n 7:\tNW\n 8:\tSAT\n 9:\tGPRS\n 10:\tMISC\n 11:\tSVC\n 12:\tGPS & NPS\n 13:\tSOUND\n 14:\tCDMA-DATA\n 15:\tCFG\n\n");

		printf("\n **********************************************************\n\n");

		printf("Enter the selected sub-module <ID> shown above or <0> to Quit \n");

		memset(buf, 0, 255);
		fgets(buf, 255, stdin);
		choice = atoi(buf);

		printf("You have selected %d\n", choice);

		tapi_test_app_process_choice(choice);

	}

}





//2  This function calls the subroutines of the sub modules which will subscribe for the
//2 Notifications. Note that GMain Loop should not be created in the called sub-routines.



/*	Initialize Notification Manager, Subscribe for Events and then Wait in GMain Loop	*/
void tapi_test_app_noti_thread(void)
{
	if(tel_init() == TAPI_API_SUCCESS)
	{
		TAPI_TEST_DEBUG("tel_init() succeeded");
	}
	else
	{
		TAPI_TEST_DEBUG("tel_init() failed");
	}

	power_test_subscribe_tapi_events ();

	/*	Subscribe for Call, Common, Power & Display Events	*/
	call_test_subscribe_tapi_events();

	nw_test_subscribe_tapi_events();

	gps_test_subscribe_tapi_events();

	misc_test_subscribe_tapi_events();

	data_test_subscribe_tapi_events();

	//sat_test_subscribe_tapi_events();

	sim_test_subscribe_tapi_events();

	svc_test_subscribe_tapi_events();

//	ss_test_subscribe_tapi_events();

	sms_subscribe_tapi_events();

	sound_test_subscribe_tapi_events ();

	tel_register_app_name("org.tizen.tel_test");

	cdma_data_test_subscribe_tapi_events();

	/*	Enter to GMain Loop	*/
	tapi_test_app_noti_loop();

	tel_deinit();
	fprintf(stderr,"tel_deinit success\n");

}



/*	Entry point for the Integrated Test Application...	*/
int main(void)
{
	pthread_t		p_thread;
	int			noti_thread_id;

	/*	Step 1: Version Information */
	TAPI_TEST_DEBUG("******* Integrated TAPI Test Application: Version 0.4  *****");
	TAPI_TEST_DEBUG("******* Buid On: %s  %s  ********", __DATE__, __TIME__);

	/*	Step 2: Create a thread for Notifications	*/
	noti_thread_id = pthread_create(&p_thread, NULL,(void *)& tapi_test_app_noti_thread, NULL);

	if(noti_thread_id != 0)
	{
		TAPI_TEST_DEBUG( "Failed to create Noti Thread");
		return -1;
	}
	else
	{
		TAPI_TEST_DEBUG( "Successfully Created Noti Thread");
	}

	/*	Step 3: Create and Process Loop for Key Events	*/
	tapi_test_app_process_keyboard();


	/*	Main Thread shall wait for the worker (Noti) Thread	*/
	pthread_join(noti_thread_id, NULL);

	return 0;

}
/*	EOF		*/

