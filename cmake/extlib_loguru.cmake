cmake_minimum_required(VERSION 3.5)

message(STATUS "entering in extlib_loguru.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(LOGURU_INCLUDE_DIR ${EXTERNALS_PREFIX_PATH}/include/loguru)
execute_process(COMMAND mkdir -p ${LOGURU_INCLUDE_DIR})

set(LOGURU_URL https://github.com/emilk/loguru)
#set(LOGURU_TAG )

ExternalProject_Add(extlib_loguru
    PREFIX extlib_loguru

    GIT_REPOSITORY ${LOGURU_URL}
    #GIT_TAG ${LOGURU_TAG}

    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""

    BUILD_COMMAND ""
    BUILD_ALWAYS OFF

    INSTALL_DIR ${EXTERNALS_PREFIX_PATH}
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/loguru.hpp ${LOGURU_INCLUDE_DIR} <SOURCE_DIR>/loguru.cpp ${LOGURU_INCLUDE_DIR}
    )

add_library(loguru INTERFACE)
add_custom_target(install_extlib_loguru DEPENDS extlib_loguru)
add_dependencies(loguru install_extlib_loguru)