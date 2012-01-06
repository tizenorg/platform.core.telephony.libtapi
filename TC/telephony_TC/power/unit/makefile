CC=gcc -Wall

LB1  =  power_common
TS1  =  uts_tel_process_power_command
TS2  =  uts_tel_set_flight_mode
TS3  =  uts_tapi_power_reboot_modem
TS4  =  
TS5  =   
TS6  =  
TS7  =  

LIBS =$(LB1).o
LIBS += `pkg-config --libs  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi` 
LIBS +=$(TET_ROOT)/lib/tet3/tcm_s.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -ltcm_s
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi_s

INCS  += -I$(TET_ROOT)/inc/tet3 
INCS  +=  -I. `pkg-config --cflags  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

CFLAGS = $(INCS) -DXTHREADS -D_REENTRANT -DXUSE_MTSAFE_API -DWITHOUT_APP_MGR 
CFLAGS += -D_TETWARE_MODE_ -D_XO_TELEPHONY_INTEGRATION_
CFLAGS += -DXO20_EVENT_DELIVERY_MODIFICATION
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)


all : $(LB1)  $(TS1)  $(TS2) $(TS3)  $(TS4)  $(TS5)  $(TS6) $(TS7)
clean :
	rm -rf *~ *.o tet_lock tet_captured $(LB1)  $(TS1)  $(TS2) $(TS3)  $(TS4)  $(TS5)  $(TS6) $(TS7)

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

