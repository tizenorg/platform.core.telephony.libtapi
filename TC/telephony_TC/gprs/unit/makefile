CC=gcc -Wall

LB1  =  gprs_common
TS1  =  uts_tel_activate_gprs_pdp
TS2  =  uts_tel_control_gprs_btdun_pin
TS3  =  uts_tel_deactivate_gprs_pdp

LIBS =$(LB1).o
LIBS += `pkg-config --libs  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi

INCS  =  -I. `pkg-config --cflags  gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS  += -I$(TET_ROOT)/inc/tet3

CFLAGS += $(INCS) -D__arm__ -D_ARM_TOUCH -DTELEPHONY_TIMER
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

all : $(LB1) $(TS1) $(TS2) $(TS3)

clean :
	rm -rf *~ *.o tet_lock tet_captured $(LB1) $(TS1) $(TS2) $(TS3)

$(LB1): $(LB1).c
	$(CC) -c $(LB1).c

$(TS1): $(TS1).c $(LB1)
	$(CC) -o $(TS1) $(TS1).c $(LDFLAGS)

$(TS2): $(TS2).c $(LB1)
	$(CC) -o $(TS2) $(TS2).c $(LDFLAGS)

$(TS3): $(TS3).c $(LB1)
	$(CC) -o $(TS3) $(TS3).c $(LDFLAGS)
