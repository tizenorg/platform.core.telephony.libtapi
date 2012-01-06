CC=gcc -Wall

LB1  = tet_tapi_util
TS1  = uts_tel_change_ss_barring_password
TS2  = uts_tel_get_ss_aoc_info
TS3  = uts_tel_get_ss_barring_status
TS4  = uts_tel_get_ss_cli_status
TS5  = uts_tel_get_ss_forward_status
TS6  = uts_tel_get_ss_waiting_status
TS7  = uts_tel_send_ss_ussd_cancel
TS8  = uts_tel_send_ss_ussd_request
TS9 = uts_tel_send_ss_ussd_response
TS10 = uts_tel_set_ss_barring
TS11 = uts_tel_set_ss_forward
#TS12 = uts_tapi_ussd_send_rsp
TS12  = uts_tel_set_ss_aoc_info
TS13 = uts_tel_set_ss_waiting

LIBS =$(LB1).o
LIBS +=`pkg-config --libs gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi

INCS = -I. `pkg-config --cflags gconf-2.0 glib-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS += -I$(TET_ROOT)/inc/tet3 
INCS += -I../

CFLAGS += $(INCS) -D__arm__ -D_ARM_TOUCH -DTELEPHONY_TIMER
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

#ifeq ($(ARCH),target)
#CC += -g -mabi=aapcs-linux
#endif

all : $(LB1) $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) $(TS11) $(TS12)  $(TS13) 

clean :
	rm -rf *~ *.o tet_lock tet_captured $(LB1) $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS9) $(TS10) $(TS11) $(TS12)  $(TS13) 

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
