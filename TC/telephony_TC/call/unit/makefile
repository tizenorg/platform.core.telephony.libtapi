CC=gcc -Wall

TS1  = uts_tel_exe_call_mo
TS2  = uts_tel_answer_call
TS3  = uts_tel_release_call
TS4  = uts_tel_release_call_all
TS5  = uts_tel_retrieve_call
TS6  = uts_tel_hold_call
TS7  = uts_tel_swap_call
TS8  = uts_tel_join_call
TS9  = uts_tel_split_call
TS10  = uts_tel_exe_call_explicit_transfer
TS11  = uts_tel_get_call_act_line
TS12  = uts_tel_set_call_act_line
TS13  = uts_tel_get_call_conf_list
TS14  = uts_tel_get_call_status
TS15  = uts_tel_get_call_duration
TS16  = uts_tel_send_call_dtmf
TS17  = uts_tel_release_call_all_active
TS18  = uts_tel_release_call_all_held
//TS19  = uts_tapi_check_status
TS20  = uts_tel_exit_call_emergency_mode
TS21  = uts_tel_exe_call_flash
TS22  = uts_tel_get_call_privacy_mode
TS23  = uts_tel_set_call_privacy_mode
TS24  = uts_tel_get_call_time
TS25  = uts_tel_activate_call_ccbs
TS26  = uts_tel_deflect_call

LB1  = tapi_callback

LIBS =$(LB1).o
LIBS += `pkg-config --libs  gconf-2.0 dbus-1 dbus-glib-1 glib-2.0 gthread-2.0 tapi` 

LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi

INCS =  -I. `pkg-config --cflags  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS += -I$(TET_ROOT)/inc/tet3 
INCS += -I..

CFLAGS += $(INCS) -D__arm__ -D_ARM_TOUCH -DTELEPHONY_TIMER
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

all : $(LB1) $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) $(TS11) $(TS12) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18) $(TS20)   $(TS21)   $(TS22) $(TS23) $(TS24) $(TS25) $(TS26)  


clean :
	rm -rf *~ *.o tet_lock tet_captured $(LB1) $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) $(TS11) $(TS12)  $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18) $(TS20)   $(TS21)   $(TS22) $(TS23) $(TS24) $(TS25) $(TS26)  


$(LB1): $(LB1).c
	$(CC) -c $(LB1).c

$(TS1): $(TS1).c $(LB1)
	$(CC) -o $(TS1) $(TS1).c  $(LDFLAGS)

$(TS2): $(TS2).c $(LB1)
	$(CC) -o $(TS2) $(TS2).c  $(LDFLAGS)

$(TS3): $(TS3).c $(LB1)
	$(CC) -o $(TS3) $(TS3).c  $(LDFLAGS)

$(TS4): $(TS4).c $(LB1)
	$(CC) -o $(TS4) $(TS4).c  $(LDFLAGS)

$(TS5): $(TS5).c $(LB1)
	$(CC) -o $(TS5) $(TS5).c  $(LDFLAGS)

$(TS6): $(TS6).c $(LB1)
	$(CC) -o $(TS6) $(TS6).c  $(LDFLAGS)

$(TS7): $(TS7).c $(LB1)
	$(CC) -o $(TS7) $(TS7).c  $(LDFLAGS)

$(TS8): $(TS8).c $(LB1)
	$(CC) -o $(TS8) $(TS8).c  $(LDFLAGS)

$(TS9): $(TS9).c $(LB1)
	$(CC) -o $(TS9) $(TS9).c  $(LDFLAGS)

$(TS10): $(TS10).c $(LB1)
	$(CC) -o $(TS10) $(TS10).c  $(LDFLAGS)

$(TS11): $(TS11).c $(LB1)
	$(CC) -o $(TS11) $(TS11).c  $(LDFLAGS)

$(TS12): $(TS12).c $(LB1)
	$(CC) -o $(TS12) $(TS12).c  $(LDFLAGS)

$(TS13): $(TS13).c $(LB1)
	$(CC) -o $(TS13) $(TS13).c  $(LDFLAGS)


$(TS14): $(TS14).c $(LB1)
	$(CC) -o $(TS14) $(TS14).c  $(LDFLAGS)

$(TS15): $(TS15).c $(LB1)
	$(CC) -o $(TS15) $(TS15).c  $(LDFLAGS)

$(TS16): $(TS16).c $(LB1)
	$(CC) -o $(TS16) $(TS16).c  $(LDFLAGS)

$(TS17): $(TS17).c $(LB1)
	$(CC) -o $(TS17) $(TS17).c  $(LDFLAGS)

$(TS18): $(TS18).c $(LB1)
	$(CC) -o $(TS18) $(TS18).c  $(LDFLAGS)

#$(TS19): $(TS19).c $(LB1)
#	$(CC) -o $(TS19) $(TS19).c  $(LDFLAGS)

$(TS20): $(TS20).c $(LB1)
	$(CC) -o $(TS20) $(TS20).c  $(LDFLAGS)

$(TS21): $(TS21).c $(LB1)
	$(CC) -o $(TS21) $(TS21).c  $(LDFLAGS)

$(TS22): $(TS22).c $(LB1)
	$(CC) -o $(TS22) $(TS22).c  $(LDFLAGS)

$(TS23): $(TS23).c $(LB1)
	$(CC) -o $(TS23) $(TS23).c  $(LDFLAGS)

$(TS24): $(TS24).c $(LB1)
	$(CC) -o $(TS24) $(TS24).c  $(LDFLAGS)

$(TS25): $(TS25).c $(LB1)
	$(CC) -o $(TS25) $(TS25).c  $(LDFLAGS)

$(TS26): $(TS26).c $(LB1)
	$(CC) -o $(TS26) $(TS26).c  $(LDFLAGS)
