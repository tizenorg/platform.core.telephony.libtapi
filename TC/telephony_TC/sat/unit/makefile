CC=gcc -Wall

LB1 = async_util
LB2 = tet_tapi_util 
TS1 = uts_tel_download_sat_event
TS2 = uts_tel_get_sat_main_menu_info
TS3 = uts_tel_select_sat_menu
TS4 = uts_tel_send_sat_app_exec_result
TS5 = uts_tel_send_sat_ui_user_confirm
TS6 = uts_tel_send_ui_display_status

LIBS  = `pkg-config --libs  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi ` 
LIBS +=$(TET_ROOT)/lib/tet3/tcm_s.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -ltcm_s
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi_s

INCS  =  -I. `pkg-config --cflags  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS  += -I$(TET_ROOT)/inc/tet3

CFLAGS += $(INCS) -D__arm__ -D_ARM_TOUCH -DTELEPHONY_TIMER
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

all : $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) 

clean :
	rm -rf *~ *.o target i386 tet_lock tet_captured *.pyc *.log .scon*
	rm -rf *~ *.o $(TS1)
	rm -rf *~ *.o $(TS2)
	rm -rf *~ *.o $(TS3)
	rm -rf *~ *.o $(TS4)
	rm -rf *~ *.o $(TS5)
	rm -rf *~ *.o $(TS6)



$(LB1): $(LB1).c
	$(CC) -c $(LB1).c  

$(LB2): $(LB2).c
	$(CC) -c $(LB2).c  

$(TS1): $(TS1).c
	$(CC) -o $(TS1) $(TS1).c  $(LDFLAGS)

$(TS2): $(TS2).c
	$(CC) -o $(TS2) $(TS2).c  $(LDFLAGS)

$(TS3): $(TS3).c
	$(CC) -o $(TS3) $(TS3).c  $(LDFLAGS)

$(TS4): $(TS4).c
	$(CC) -o $(TS4) $(TS4).c  $(LDFLAGS)

$(TS5): $(TS5).c
	$(CC) -o $(TS5) $(TS5).c  $(LDFLAGS)

$(TS6): $(TS6).c
	$(CC) -o $(TS6) $(TS6).c  $(LDFLAGS)




