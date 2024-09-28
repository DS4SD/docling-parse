cmake_minimum_required (VERSION 3.5)

message(STATUS "entering in extlib_cxxopts.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(CXXOPTS_TAG v2.2.0)
set(CXXOPTS_URL https://github.com/jarro2783/cxxopts.git)

ExternalProject_Add(extlib_cxxopts
    PREFIX extlib_cxxopts

    GIT_TAG ${CXXOPTS_TAG}
    GIT_REPOSITORY ${CXXOPTS_URL}

    INSTALL_DIR ${CXXOPTS_PREFIX_INSTALL_DIR}

    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""

    BUILD_COMMAND ""
    BUILD_ALWAYS OFF

    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include/ ${EXTERNALS_PREFIX_PATH}/include/

    LOG_DOWNLOAD ON
    LOG_BUILD ON
    )

add_library(cxxopts INTERFACE)
add_custom_target(install_extlib_cxxopts DEPENDS extlib_cxxopts)
add_dependencies(cxxopts install_extlib_cxxopts)