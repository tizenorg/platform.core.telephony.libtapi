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
	phonebook.c
	modem.c
	ss.c
	call.c
	gps.c
)

ADD_EXECUTABLE(tapitest ${tapitest})
TARGET_LINK_LIBRARIES(tapitest ${pkgs_LDFLAGS} "-L${CMAKE_BINARY_DIR} -ltapi-3.0")
INSTALL(TARGETS tapitest RUNTIME DESTINATION bin/)
ADD_DEPENDENCIES(tapitest tapi-3.0)
