cmake_minimum_required(VERSION 3.5)

message(STATUS "entering in extlib_utf8.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(UTF8_URL https://github.com/nemtrif/utfcpp.git)

ExternalProject_Add(extlib_utf8
    PREFIX extlib_utf8

    GIT_REPOSITORY ${UTF8_URL}
    #GIT_TAG ${UTF8_TAG}

    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""

    BUILD_COMMAND ""
    BUILD_ALWAYS OFF

    INSTALL_DIR     ${EXTERNALS_PREFIX_PATH}
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/source ${EXTERNALS_PREFIX_PATH}/include/utf8
    )

add_library(utf8 INTERFACE)
add_custom_target(install_extlib_utf8 DEPENDS extlib_utf8)
add_dependencies(utf8 install_extlib_utf8)