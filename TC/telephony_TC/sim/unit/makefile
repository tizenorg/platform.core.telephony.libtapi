CC=gcc -Wall

LB1 = async_util
TS1 = uts_tel_get_sim_type
TS2 = uts_tel_get_sim_init_info
TS3 = uts_tel_get_sim_ecc
TS4 = uts_tel_get_sim_imsi
TS5 = uts_tel_get_sim_language
TS6 = uts_tel_set_sim_language
TS7 = uts_tel_req_sim_access
TS8 = uts_tel_verify_sim_puks
TS9 = uts_tel_verifiy_sim_pins
TS10 = uts_tel_change_sim_pins
TS11 = uts_tel_disable_sim_security
TS12 = uts_tel_disable_sim_fdn
TS13 = uts_tel_enable_sim_security
TS14 = uts_tel_enable_sim_fdn
TS15 = uts_tel_get_sim_fdn_status
TS16 = uts_tel_get_sim_security_status
TS17 = uts_tel_get_sim_personalization_status
TS18 = uts_tel_disable_sim_personalization
TS19 = uts_tel_enable_sim_personalization
TS20 = uts_tel_get_sim_mailbox_info
TS21 = uts_tel_get_sim_callforwarding_info
TS22 = uts_tel_get_sim_messagewaiting_info
TS23 = uts_tel_get_sim_cphs_info
TS24 = uts_tel_get_sim_msisdn
TS25 = uts_tel_get_sim_pb_init_info
TS26 = uts_tel_get_sim_pb_meta_info
TS27 = uts_tel_get_sim_pb_3g_meta_info
TS28 = uts_tel_get_sim_pb_count
TS29 = uts_tel_read_sim_pb_record
TS30 = uts_tel_update_sim_pb_record
TS31 = uts_tel_delete_sim_pb_record
TS32 = uts_tel_req_sim_isim_authentication
TS33 = #uts_tel_req_sap_connection
TS34 = #uts_tel_req_sap_connection_status
TS35 = #uts_tel_req_sap_transfer_atr
TS36 = #uts_tel_req_sap_transfer_apdu
TS37 = #uts_tel_req_sap_transport_protocol
TS38 = #uts_tel_req_sap_power_operation
TS39 = #uts_tel_req_sap_cardreader_status
TS40 =
TS41 =
TS42 =

LIBS  = `pkg-config --libs  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi ` 
LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi

INCS = -I. `pkg-config --cflags  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi `
INCS += -I$(TET_ROOT)/inc/tet3 
INCS += -I../

CFLAGS = $(INCS) -DXTHREADS -D_REENTRANT -DXUSE_MTSAFE_API -DWITHOUT_APP_MGR 
CFLAGS += -D_TETWARE_MODE_ 
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

all :  $(LB1) $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) \
       $(TS11) $(TS12) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18) $(TS19) $(TS20) \
       $(TS21) $(TS22) $(TS23) $(TS24) $(TS25) $(TS26) $(TS27) $(TS28) $(TS29) $(TS30) \
       $(TS31) $(TS32) $(TS33) $(TS34) $(TS35) $(TS36) $(TS37) $(TS38) $(TS39) $(TS40) $(TS41) $(TS42)
      
      
clean :
	rm -rf *~ *.o target i386 tet_lock tet_captured *.pyc *.log .scon*
	rm -rf $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10)
	rm -rf $(TS11) $(TS12) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18) $(TS19) $(TS20)
	rm -rf $(TS21) $(TS22) $(TS23) $(TS24) $(TS25) $(TS26) $(TS27) $(TS28) $(TS29) $(TS30)
	rm -rf $(TS31) $(TS32) $(TS33) $(TS34) $(TS35) $(TS36) $(TS37) $(TS38) $(TS39) $(TS40) $(TS41) $(TS42)


$(LB1): $(LB1).c
	$(CC) -c $(LB1).c

$(TS1): $(TS1).c $(LB1)
	$(CC) -o $(TS1) $(TS1).c  $(LDFLAGS) $(LB1).o

$(TS2): $(TS2).c $(LB1)
	$(CC) -o $(TS2) $(TS2).c  $(LDFLAGS) $(LB1).o

$(TS3): $(TS3).c $(LB1)
	$(CC) -o $(TS3) $(TS3).c  $(LDFLAGS) $(LB1).o

$(TS4): $(TS4).c $(LB1)
	$(CC) -o $(TS4) $(TS4).c  $(LDFLAGS) $(LB1).o

$(TS5): $(TS5).c $(LB1)
	$(CC) -o $(TS5) $(TS5).c  $(LDFLAGS) $(LB1).o

$(TS6): $(TS6).c $(LB1)
	$(CC) -o $(TS6) $(TS6).c  $(LDFLAGS) $(LB1).o

$(TS7): $(TS7).c $(LB1)
	$(CC) -o $(TS7) $(TS7).c  $(LDFLAGS) $(LB1).o

$(TS8): $(TS8).c $(LB1)
	$(CC) -o $(TS8) $(TS8).c  $(LDFLAGS) $(LB1).o

$(TS9): $(TS9).c $(LB1)
	$(CC) -o $(TS9) $(TS9).c  $(LDFLAGS) $(LB1).o

$(TS10): $(TS10).c $(LB1)
	$(CC) -o $(TS10) $(TS10).c  $(LDFLAGS) $(LB1).o

$(TS11): $(TS11).c $(LB1)
	$(CC) -o $(TS11) $(TS11).c  $(LDFLAGS) $(LB1).o

$(TS12): $(TS12).c $(LB1)
	$(CC) -o $(TS12) $(TS12).c  $(LDFLAGS) $(LB1).o

$(TS13): $(TS13).c $(LB1)
	$(CC) -o $(TS13) $(TS13).c  $(LDFLAGS) $(LB1).o

$(TS14): $(TS14).c $(LB1)
	$(CC) -o $(TS14) $(TS14).c  $(LDFLAGS) $(LB1).o

$(TS15): $(TS15).c $(LB1)
	$(CC) -o $(TS15) $(TS15).c  $(LDFLAGS) $(LB1).o

$(TS16): $(TS16).c $(LB1)
	$(CC) -o $(TS16) $(TS16).c  $(LDFLAGS) $(LB1).o

$(TS17): $(TS17).c $(LB1)
	$(CC) -o $(TS17) $(TS17).c  $(LDFLAGS) $(LB1).o

$(TS18): $(TS18).c $(LB1)
	$(CC) -o $(TS18) $(TS18).c  $(LDFLAGS) $(LB1).o

$(TS19): $(TS19).c $(LB1)
	$(CC) -o $(TS19) $(TS19).c  $(LDFLAGS) $(LB1).o

$(TS20): $(TS20).c $(LB1)
	$(CC) -o $(TS20) $(TS20).c  $(LDFLAGS) $(LB1).o

$(TS21): $(TS21).c $(LB1)
	$(CC) -o $(TS21) $(TS21).c  $(LDFLAGS) $(LB1).o

$(TS22): $(TS22).c $(LB1)
	$(CC) -o $(TS22) $(TS22).c  $(LDFLAGS) $(LB1).o

$(TS23): $(TS23).c $(LB1)
	$(CC) -o $(TS23) $(TS23).c  $(LDFLAGS) $(LB1).o

$(TS24): $(TS24).c $(LB1)
	$(CC) -o $(TS24) $(TS24).c  $(LDFLAGS) $(LB1).o

$(TS25): $(TS25).c $(LB1)
	$(CC) -o $(TS25) $(TS25).c  $(LDFLAGS) $(LB1).o

$(TS26): $(TS26).c $(LB1)
	$(CC) -o $(TS26) $(TS26).c  $(LDFLAGS) $(LB1).o

$(TS27): $(TS27).c $(LB1)
	$(CC) -o $(TS27) $(TS27).c  $(LDFLAGS) $(LB1).o

$(TS28): $(TS28).c $(LB1)
	$(CC) -o $(TS28) $(TS28).c  $(LDFLAGS) $(LB1).o

$(TS29): $(TS29).c $(LB1)
	$(CC) -o $(TS29) $(TS29).c  $(LDFLAGS) $(LB1).o

$(TS30): $(TS30).c $(LB1)
	$(CC) -o $(TS30) $(TS30).c  $(LDFLAGS) $(LB1).o

$(TS31): $(TS31).c $(LB1)
	$(CC) -o $(TS31) $(TS31).c  $(LDFLAGS) $(LB1).o 

$(TS32): $(TS32).c $(LB1)
	$(CC) -o $(TS32) $(TS32).c  $(LDFLAGS) $(LB1).o

$(TS33): $(TS33).c $(LB1)
	$(CC) -o $(TS33) $(TS33).c  $(LDFLAGS) $(LB1).o

$(TS34): $(TS34).c $(LB1)
	$(CC) -o $(TS34) $(TS34).c  $(LDFLAGS) $(LB1).o

$(TS35): $(TS35).c $(LB1)
	$(CC) -o $(TS35) $(TS35).c  $(LDFLAGS) $(LB1).o

$(TS36): $(TS36).c $(LB1)
	$(CC) -o $(TS36) $(TS36).c  $(LDFLAGS) $(LB1).o

$(TS37): $(TS37).c $(LB1)
	$(CC) -o $(TS37) $(TS37).c  $(LDFLAGS) $(LB1).o

$(TS38): $(TS38).c $(LB1)
	$(CC) -o $(TS38) $(TS38).c  $(LDFLAGS) $(LB1).o

$(TS39): $(TS39).c $(LB1)
	$(CC) -o $(TS39) $(TS39).c  $(LDFLAGS) $(LB1).o

$(TS40): $(TS40).c $(LB1)
	$(CC) -o $(TS40) $(TS40).c  $(LDFLAGS) $(LB1).o

$(TS41): $(TS41).c $(LB1)
	$(CC) -o $(TS41) $(TS41).c  $(LDFLAGS) $(LB1).o

$(TS42): $(TS42).c $(LB1)
	$(CC) -o $(TS42) $(TS42).c  $(LDFLAGS) $(LB1).o

