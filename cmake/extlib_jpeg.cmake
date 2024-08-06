cmake_minimum_required(VERSION 3.5)

message(STATUS "entering in extlib_jpeg.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(JPEG_URL https://github.com/libjpeg-turbo/libjpeg-turbo.git)
set(JPEG_TAG 3.0.0)

ExternalProject_Add(extlib_jpeg
    PREFIX extlib_jpeg

    UPDATE_COMMAND ""
    GIT_REPOSITORY ${JPEG_URL}
    GIT_TAG ${JPEG_TAG}

    BUILD_ALWAYS OFF

    INSTALL_DIR ${EXTERNALS_PREFIX_PATH}

    CMAKE_ARGS \\
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \\
    -DCMAKE_INSTALL_LIBDIR=${EXTERNALS_PREFIX_PATH}/lib \\
    -DCMAKE_INSTALL_PREFIX=${EXTERNALS_PREFIX_PATH}

    BUILD_IN_SOURCE ON
    LOG_DOWNLOAD ON
    LOG_BUILD ON)

add_library(jpeg STATIC IMPORTED)
set_target_properties(jpeg PROPERTIES IMPORTED_LOCATION ${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a)
add_dependencies(jpeg extlib_jpeg)
