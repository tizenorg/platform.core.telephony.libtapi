CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(tapitest C)

SET(tapitest
	main.c
	menu.c
	network.c
	sat.c
	sim.c
	sms.c
	sms_util.c
	sms_util_cdma.c
	phonebook.c
	modem.c
	ss.c
	call.c
)

ADD_EXECUTABLE(tapitest ${tapitest})
TARGET_LINK_LIBRARIES(tapitest ${pkgs_LDFLAGS} "-L${CMAKE_BINARY_DIR} -lSLP-tapi")
INSTALL(TARGETS tapitest RUNTIME DESTINATION bin/)
ADD_DEPENDENCIES(tapitest SLP-tapi)
