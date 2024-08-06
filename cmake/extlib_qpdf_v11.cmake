cmake_minimum_required(VERSION 3.5)

message(STATUS "entering in extlib_qpdf.cmake")

include(ExternalProject)
include(CMakeParseArguments)

#set(PCRE2_URL https://github.com/PCRE2Project/pcre2.git)
#set(PCRE2_TAG pcre2-10.40)

#set(QPDF_URL git@github.ibm.com:CognitiveCore/qpdf-multipart-stream-patched.git)
set(QPDF_URL https://github.com/qpdf/qpdf.git)
set(QPDF_TAG v11.9.1 )

if(APPLE)
  set(QPDF_EXTRA_CONFIGURE_COMMAND "--target=${CMAKE_OSX_ARCHITECTURES} --host=arm64 --build=arm64")
else()
  set(QPDF_EXTRA_CONFIGURE_COMMAND " ")
endif()

ExternalProject_Add(extlib_qpdf
    PREFIX extlib_qpdf

    DEPENDS extlib_jpeg

    UPDATE_COMMAND ""

    GIT_REPOSITORY ${QPDF_URL}
    GIT_TAG        ${QPDF_TAG}
    
    BUILD_ALWAYS OFF

    INSTALL_DIR ${EXTERNALS_PREFIX_PATH}

    CMAKE_ARGS \\
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON \\
    -DBUILD_SHARED_LIBS=OFF \\
    -DUSE_IMPLICIT_CRYPTO=OFF \\
    -DREQUIRE_CRYPTO_NATIVE=ON \\
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} \\
    -DCMAKE_INSTALL_LIBDIR=${EXTERNALS_PREFIX_PATH}/lib \\
    -DCMAKE_INSTALL_PREFIX=${EXTERNALS_PREFIX_PATH}

    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

add_library(qpdf STATIC IMPORTED)
set_target_properties(qpdf PROPERTIES IMPORTED_LOCATION ${EXTERNALS_PREFIX_PATH}/lib/libqpdf.a)
add_dependencies(qpdf extlib_qpdf)
