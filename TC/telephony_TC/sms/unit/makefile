CC=gcc -Wall

TS1  =  uts_tel_delete_sms_in_sim
TS2  =  uts_tel_get_sms_cb_config
TS3  =  uts_tel_get_sms_count
TS4  =  uts_tel_get_sms_parameter_count
TS5  =  uts_tel_get_sms_parameters
TS6  =  uts_tel_get_sms_preferred_bearer
TS7  =  uts_tel_get_sms_sca
TS8  =  uts_tel_read_sms_in_sim
TS9  =  uts_tel_send_sms
TS10 =  uts_tel_send_sms_deliver_report
TS11 =  uts_tel_send_sms_msg
TS12 =  uts_tel_set_sms_cb_config
TS13 =  uts_tel_set_sms_memory_status
TS14 =  uts_tel_set_sms_message_status
TS15 =  uts_tel_set_sms_parameters
TS16 =  uts_tel_set_sms_preferred_bearer
TS17 =  uts_tel_set_sms_sca
TS18 =  uts_tel_write_sms_in_sim

LIBS = `pkg-config --libs  glib-2.0 gconf-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi

INCS =  -I. `pkg-config --cflags  glib-2.0 gconf-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS += -I$(TET_ROOT)/inc/tet3 
INCS += -I../

CFLAGS = $(INCS) -DXTHREADS -D_REENTRANT -DXUSE_MTSAFE_API -DWITHOUT_APP_MGR -D_TETWARE_MODE_ 
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

all : $(TS1)  $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) $(TS11) $(TS12) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18) 


clean :
	rm -rf *~ *.o tet_lock tet_captured $(TS1)  $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) $(TS11) $(TS12) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18) 


$(TS1): $(TS1).c
	$(CC) -o $(TS1) $(TS1).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS2): $(TS2).c
	$(CC) -o $(TS2) $(TS2).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS3): $(TS3).c
	$(CC) -o $(TS3) $(TS3).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS4): $(TS4).c
	$(CC) -o $(TS4) $(TS4).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS5): $(TS5).c
	$(CC) -o $(TS5) $(TS5).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS6): $(TS6).c
	$(CC) -o $(TS6) $(TS6).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS7): $(TS7).c
	$(CC) -o $(TS7) $(TS7).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS8): $(TS8).c
	$(CC) -o $(TS8) $(TS8).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS9): $(TS9).c
	$(CC) -o $(TS9) $(TS9).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS10): $(TS10).c
	$(CC) -o $(TS10) $(TS10).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS11): $(TS11).c
	$(CC) -o $(TS11) $(TS11).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS12): $(TS12).c
	$(CC) -o $(TS12) $(TS12).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS13): $(TS13).c
	$(CC) -o $(TS13) $(TS13).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS14): $(TS14).c
	$(CC) -o $(TS14) $(TS14).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS15): $(TS15).c
	$(CC) -o $(TS15) $(TS15).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS16): $(TS16).c
	$(CC) -o $(TS16) $(TS16).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS17): $(TS17).c
	$(CC) -o $(TS17) $(TS17).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)
$(TS18): $(TS18).c
	$(CC) -o $(TS18) $(TS18).c sms_callback_util.c sms_util.c async_util.c $(LDFLAGS)


