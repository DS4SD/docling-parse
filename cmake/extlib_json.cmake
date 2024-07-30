cmake_minimum_required(VERSION 3.5)

message(STATUS "entering in extlib_json.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(JSON_URL https://github.com/nlohmann/json.git)
#set(JSON_TAG v3.10.5)

ExternalProject_Add(extlib_json
    PREFIX extlib_json

    GIT_REPOSITORY ${JSON_URL}
    #GIT_TAG ${JSON_TAG}

    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""

    BUILD_COMMAND ""
    BUILD_ALWAYS OFF

    INSTALL_DIR     ${EXTERNALS_PREFIX_PATH}
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include/ ${EXTERNALS_PREFIX_PATH}/include/
    )

add_library(json INTERFACE)
add_custom_target(install_extlib_json DEPENDS extlib_json)
add_dependencies(json install_extlib_json)