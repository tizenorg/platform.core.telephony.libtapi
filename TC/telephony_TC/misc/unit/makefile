CC=gcc -Wall

TS1  =  uts_tapi_misc_NAM_info_get
TS2  =  uts_tapi_misc_NAM_info_set
TS3  =  uts_tapi_misc_me_serial_num_get
TS4  =  uts_tapi_misc_timezone_getinfo
TS5  =  uts_tapi_misc_timezone_setinfo

LIBS  = `pkg-config --libs  glib-2.0 gconf-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi
	
INCS  =  -I. `pkg-config --cflags  glib-2.0 gconf-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS  += -I$(TET_ROOT)/inc/tet3 

CFLAGS += $(INCS) -D__arm__ -D_ARM_TOUCH -DTELEPHONY_TIMER 
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

all : $(TS1)  $(TS2) $(TS3) $(TS4) $(TS5)

clean :
	rm -rf *~ *.o tet_lock tet_captured $(TS1)  $(TS2) $(TS3) $(TS4) $(TS5) 

$(TS1): $(TS1).c
	$(CC) -o $(TS1) $(TS1).c misc_callback_util.c  $(LDFLAGS)
$(TS2): $(TS2).c
	$(CC) -o $(TS2) $(TS2).c misc_callback_util.c  $(LDFLAGS)

$(TS3): $(TS3).c
	$(CC) -o $(TS3) $(TS3).c misc_callback_util.c  $(LDFLAGS)

$(TS4): $(TS4).c
	$(CC) -o $(TS4) $(TS4).c misc_callback_util.c  $(LDFLAGS)

$(TS5): $(TS5).c
	$(CC) -o $(TS5) $(TS5).c misc_callback_util.c  $(LDFLAGS)

