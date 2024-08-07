
message(STATUS "entering in extlib_qpdf.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(QPDF_URL https://github.com/qpdf/qpdf.git)
set(QPDF_TAG release-qpdf-10.0.4)

if(APPLE)
  set(QPDF_EXTRA_CONFIGURE_COMMAND "--target=${CMAKE_OSX_ARCHITECTURES}")
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

    CONFIGURE_COMMAND ./configure --enable-shared=no \\
      --with-pic \\
      ${QPDF_EXTRA_CONFIGURE_COMMAND} \\
      --enable-implicit-crypto=false \\
      --enable-crypto-native=yes\\
      --prefix=${EXTERNALS_PREFIX_PATH} \\
      --libdir=${EXTERNALS_PREFIX_PATH}/lib \\
      CPPFLAGS=-I${EXTERNALS_PREFIX_PATH}/include \\
      LDFLAGS=-L${EXTERNALS_PREFIX_PATH}/lib 

    BUILD_IN_SOURCE ON
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

add_library(qpdf STATIC IMPORTED)
set_target_properties(qpdf PROPERTIES IMPORTED_LOCATION ${EXTERNALS_PREFIX_PATH}/lib/libqpdf.a)
add_dependencies(qpdf extlib_qpdf)
