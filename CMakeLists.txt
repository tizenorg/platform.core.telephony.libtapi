CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(tapi-3.0 C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX ${CMAKE_INSTALL_BINDIR})
SET(LIBDIR ${CMAKE_INSTALL_LIBDIR})
SET(INCLUDEDIR ${INCLUDE_INSTALL_DIR})
SET(TAPI_VERSION ${TAPI_VERSION})

# Set required packages
INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED glib-2.0 gio-unix-2.0 dlog security-server)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -Wextra -Wall -Wno-unused-parameter -Wno-missing-field-initializers")

#Garbage collection - Unused code
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdata-sections -ffunction-sections -Wl,--gc-sections")

INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/common
	${CMAKE_SOURCE_DIR}/include/private
)

ADD_DEFINITIONS("-DFEATURE_DLOG_DEBUG")
ADD_DEFINITIONS("-DTAPI_LOG_TAG=\"LIBTAPI\"")
ADD_DEFINITIONS("-DEXPORT_API=__attribute__((visibility(\"default\")))")

MESSAGE(${CMAKE_C_FLAGS})
MESSAGE(${CMAKE_EXE_LINKER_FLAGS})

### Build ###
SET(SRCS
	src/tapi_common.c
	src/tapi_event.c
	src/tapi_modem.c
	src/tapi_call.c
	src/tapi_network.c
	src/tapi_sim.c
	src/tapi_phonebook.c
	src/tapi_sap.c
	src/tapi_ss.c
	src/tapi_sms.c
	src/tapi_sat.c
	src/tapi_gps.c
)

SET(INTROSPECTION_PATH ${CMAKE_SOURCE_DIR}/introspection)

ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_BINARY_DIR}/generated-code.c
	COMMAND gdbus-codegen --interface-prefix org.tizen.telephony.
	--generate-c-code generated-code
	--c-namespace Telephony
	--c-generate-object-manager
	--generate-docbook generated-docs
	${INTROSPECTION_PATH}/network.xml
	${INTROSPECTION_PATH}/sim.xml
	${INTROSPECTION_PATH}/phonebook.xml
	${INTROSPECTION_PATH}/sap.xml
	${INTROSPECTION_PATH}/sat.xml
	${INTROSPECTION_PATH}/sms.xml
	${INTROSPECTION_PATH}/call.xml
	${INTROSPECTION_PATH}/ss.xml
	${INTROSPECTION_PATH}/modem.xml
	${INTROSPECTION_PATH}/gps.xml
	COMMENT "Generating GDBus .c/.h"
)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${CMAKE_BINARY_DIR}/generated-code.c)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES VERSION 0.0.0 SOVERSION 0 OUTPUT_NAME ${PROJECT_NAME})

# pkgconfig file
CONFIGURE_FILE(tapi-3.0.pc.in tapi-3.0.pc @ONLY)

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/tapi-3.0)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/tapi-3.0.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR} COMPONENT Runtime)

ADD_SUBDIRECTORY(test_src)
