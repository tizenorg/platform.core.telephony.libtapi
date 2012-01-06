export PKG_CONFIG_PATH=/usr/lib/pkgconfig

TARGET	:= TAPITest

SRCS	:= \
	tapi_call_test.c \
	tapi_test_app.c \
	tapi_nw_test.c \
	tapi_power_test.c \
	tapi_gps_test.c \
	tapi_misc_test.c \
	tapi_ps_test.c \
	tapi_svc_test.c \
	tapi_sms_util.c \
	tapi_sim_test.c \
	tapi_sms_test.c \
	tapi_cdma_data_test.c \
	tapi_cfg_test.c \
	tapi_sound_test.c

OBJS	:= $(SRCS:.c=.o)
CC		:= gcc
PKGS	:= glib-2.0 dbus-1 dbus-glib-1 tapi dlog
CFLAGS	:= -Wall -g `pkg-config --cflags $(PKGS)`
LIBS	:= `pkg-config --libs $(PKGS)`


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

.c.o: $(SRCS)
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(OBJS) $(TARGET)
