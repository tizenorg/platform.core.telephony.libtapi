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



#ifndef _UTS_TAPI_SIM_LOCK_PERSONALIZATION_H_
#define _UTS_TAPI_SIM_LOCK_PERSONALIZATION_H_

	static void utc_tel_enable_sim_personalization01();
 	static void utc_tel_enable_sim_personalization02();
 	static void utc_tel_enable_sim_personalization03();
	static void utc_tel_enable_sim_personalization04();
	static void utc_tel_disable_sim_personalization01();
 	static void utc_tel_disable_sim_personalization02();
	static void utc_tel_disable_sim_personalization03();
	static void utc_tel_disable_sim_personalization04();
typedef enum 
{
	TAPI_SIM_LOCK_READY		= 0x00,	/**< ME Not locked - No pin verifications for any lock*/
		
	TAPI_SIM_LOCK_PS,          			/** < PH-SIM (phone-sim) locked.Lock Phone to SIM/UICC card 
										   *	(MT asks password when other than current SIM/UICC card inserted; 
										   *	MT may remember certain amount of previously used cards thus not 
										   *	requiring password when they are inserted 
										   */ 
										/* This is currently managed by launcher in using gconf - NOT useful from modem 
										    * as of now.
										   */
										
	TAPI_SIM_LOCK_PF,					/** < PH-FSIM (phone-first-sim) Lock Phone to the very 
										   * First inserted SIM/UICC card(MT asks password when other than the first SIM/UICC 
										   * card is inserted
										   */
										 /* This should managed by launcher in using gconf - NOT useful from modem 
										    * as of now.
										    
									   	    */
	TAPI_SIM_LOCK_SC,					/** < SIM Lock (PIN, PIN2, PUK, PUK2) Lock SIM/UICC card ( SIM asks password in ME power-up and
										   *	when this command is issued 
										   */
									   
	TAPI_SIM_LOCK_FD,					/** < FDN - SIM card or active application in the UICC (GSM or USIM)
										   *	fixed dialing memory feature */
	TAPI_SIM_LOCK_PN,					/**< Network Personalization */
	TAPI_SIM_LOCK_PU,					/** < network subset Personalization */
	TAPI_SIM_LOCK_PP,					/** < service Provider Personalization */
	TAPI_SIM_LOCK_PC,					/** < Coporate Personalization */
	TAPI_SIM_LOCK_SC2,					/**< PIN2 lock*/
	TAPI_SIM_LOCK_SCPUK2		= 0x0A, 	/** < PIN2 Unblocking Key */
	TAPI_SIM_LOCK_ACL			= 0x0B,	/** < ACL lock */
	
	TAPI_SIM_NO_SIM			= 0x80, 	/**< SIM not inserted */
	TAPI_SIM_UNAVAIL			= 0x81,	/** < SIM interface error - SIM inserted but cannot communicate */
	TAPI_SIM_INIT_COMPLETE	= 0x82,	/** < SIM initialization completed */
	TAPI_SIM_PB_INIT_COMPLETE = 0x83, /**< PB init completed */
} TelSimLockType_t;
#endif

