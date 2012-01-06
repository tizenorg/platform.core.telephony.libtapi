CC=gcc -Wall

TS1  =  uts_tel_get_sound_volume_info
TS2  =  uts_tel_set_sound_volume_info
TS3  =  uts_tel_set_sound_path
TS4  =  uts_tel_set_sound_mute_status
TS5  =  uts_tel_get_sound_mute_status
#TS6  =  uts_tel_control_sound_voice_recording
#TS7  =  uts_tel_set_sound_loopback
#TS8  =  uts_tel_set_sound_clock

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

all : $(TS1) $(TS2) $(TS3) $(TS4) $(TS5)

clean :
	rm -rf *~ *.o tet_lock tet_captured $(TS1)  $(TS2) $(TS3) $(TS4) $(TS5)

$(TS1): $(TS1).c
	$(CC) -o $(TS1) $(TS1).c sound_callback_util.c  $(LDFLAGS)
$(TS2): $(TS2).c
	$(CC) -o $(TS2) $(TS2).c sound_callback_util.c  $(LDFLAGS)
$(TS3): $(TS3).c
	$(CC) -o $(TS3) $(TS3).c sound_callback_util.c  $(LDFLAGS)
$(TS4): $(TS4).c
	$(CC) -o $(TS4) $(TS4).c sound_callback_util.c  $(LDFLAGS)
$(TS5): $(TS5).c
	$(CC) -o $(TS5) $(TS5).c sound_callback_util.c  $(LDFLAGS)
