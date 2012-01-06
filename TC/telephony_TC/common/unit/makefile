CC=gcc -Wall
CXX=g++ -Wall

#LB1 = async_util
LB1 = 
TS1 = uts_tel_check_service_ready
TS2 = uts_tel_deinit
#TS3 = uts_tel_deinit_ecore_loop
TS3 = 
TS4 = uts_tel_register_event
#TS5 = uts_tel_deregister_event_at_ecore_loop
TS5 = 
#TS6 = uts_tel_init_ecore_loop
TS6 = 
TS7 = uts_tel_request_tunnel
TS8 = uts_tel_init
TS9 = uts_tel_deregister_event
#TS10 = uts_tel_register_event_at_ecore_loop
TS10 = 

LIBS  = `pkg-config --libs  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi ` 
LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi

INCS  =  -I. `pkg-config --cflags  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi `
INCS  += -I$(TET_ROOT)/inc/tet3 

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

CFLAGS = $(INCS) -DXTHREADS -D_REENTRANT -DXUSE_MTSAFE_API -DWITHOUT_APP_MGR 
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)


all :  $(LB1) $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) 
      

      
clean :
	rm -rf *~ *.o tet_lock tet_captured 
	rm -rf $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6)  $(TS7) $(TS8) $(TS9) $(TS10)


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

