CC=gcc -Wall

COMM = tel_tapi_nw_common
TS1 = uts_tel_get_network_band
TS2 = uts_tel_get_network_hybrid_in_cdma
TS3 = uts_tel_get_network_preferred_plmn
TS4 = uts_tel_get_network_roaming
TS5 = uts_tel_get_network_selection_mode
TS6 = uts_tel_get_network_service_domain
TS7 = uts_tel_search_network
TS8 =  uts_tel_set_network_band
TS10 = uts_tel_set_network_hybrid_in_cdma
TS11 = uts_tel_set_network_preferred_plmn
TS13 = uts_tel_set_network_roaming
TS14 = uts_tel_set_network_service_domain
TS15 = uts_tel_select_network_automatic
TS16 = uts_tel_set_network_mode
TS17 = uts_tel_get_network_mode
TS18 = uts_tel_select_network_manual

LIBS  = `pkg-config --libs  glib-2.0 gconf-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`

LIBS +=$(TET_ROOT)/lib/tet3/tcm.o
LIBS +=-L$(TET_ROOT)/lib/tet3 -lapi
	
INCS  =  -I. `pkg-config --cflags  glib-2.0 gconf-2.0 dbus-1 dbus-glib-1 gthread-2.0 tapi`
INCS  += -I$(TET_ROOT)/inc/tet3
INCS  += -I../include

CFLAGS += $(INCS) -D__arm__ -D_ARM_TOUCH -DTELEPHONY_TIMER
CC +=$(CFLAGS)
LDFLAGS = $(LIBS)

all : $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS10) $(TS11) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18)

clean :
	rm -rf *~ *.o tet_lock tet_captured $(TS1) $(TS2) $(TS3) $(TS4) $(TS5) $(TS6) $(TS7) $(TS8) $(TS10) $(TS11) $(TS13) $(TS14) $(TS15) $(TS16) $(TS17) $(TS18)

$(TS1): $(TS1).c
	$(CC) -o $(TS1) tel_tapi_nw_common.c $(TS1).c  $(LDFLAGS)

$(TS2): $(TS2).c
	$(CC) -o $(TS2) tel_tapi_nw_common.c $(TS2).c  $(LDFLAGS)

$(TS3): $(TS3).c
	$(CC) -o $(TS3) tel_tapi_nw_common.c $(TS3).c  $(LDFLAGS)

$(TS4): $(TS4).c
	$(CC) -o $(TS4) tel_tapi_nw_common.c $(TS4).c  $(LDFLAGS)

$(TS5): $(TS5).c
	$(CC) -o $(TS5) tel_tapi_nw_common.c $(TS5).c  $(LDFLAGS)

$(TS6): $(TS6).c
	$(CC) -o $(TS6) tel_tapi_nw_common.c $(TS6).c  $(LDFLAGS)

$(TS7): $(TS7).c
	$(CC) -o $(TS7) tel_tapi_nw_common.c $(TS7).c  $(LDFLAGS)

$(TS8): $(TS8).c
	$(CC) -o $(TS8) tel_tapi_nw_common.c $(TS8).c  $(LDFLAGS)

$(TS10): $(TS10).c
	$(CC) -o $(TS10) tel_tapi_nw_common.c $(TS10).c  $(LDFLAGS)

$(TS11): $(TS11).c
	$(CC) -o $(TS11) tel_tapi_nw_common.c $(TS11).c  $(LDFLAGS)

$(TS13): $(TS13).c
	$(CC) -o $(TS13) tel_tapi_nw_common.c $(TS13).c  $(LDFLAGS)

$(TS14): $(TS14).c
	$(CC) -o $(TS14) tel_tapi_nw_common.c $(TS14).c  $(LDFLAGS)
$(TS15): $(TS15).c
	$(CC) -o $(TS15) tel_tapi_nw_common.c $(TS15).c  $(LDFLAGS)

$(TS16): $(TS16).c
	$(CC) -o $(TS16) tel_tapi_nw_common.c $(TS16).c  $(LDFLAGS)

$(TS17): $(TS17).c
	$(CC) -o $(TS17) tel_tapi_nw_common.c $(TS17).c  $(LDFLAGS)

$(TS18): $(TS18).c
	$(CC) -o $(TS18) tel_tapi_nw_common.c $(TS18).c  $(LDFLAGS)
