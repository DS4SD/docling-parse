
message(STATUS "entering in extlib_qpdf.cmake")

include(ExternalProject)
include(CMakeParseArguments)

set(QPDF_URL https://github.com/qpdf/qpdf.git)
set(QPDF_TAG v11.9.1 )

if(UNIX)
  set(QPDF_EXTRA_CXX_FLAGS "-fPIC ")
  set(QPDF_EXTRA_C_FLAGS "-fPIC ")
else()
  set(QPDF_EXTRA_CXX_FLAGS " ")
  set(QPDF_EXTRA_C_FLAGS " ")
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
    -DBUILD_SHARED_LIBS=ON \\
    -DUSE_IMPLICIT_CRYPTO=OFF \\
    -DREQUIRE_CRYPTO_NATIVE=ON \\
    -DCMAKE_CXX_FLAGS=${QPDF_EXTRA_CXX_FLAGS} \\
    -DCMAKE_C_FLAGS=${QPDF_EXTRA_C_FLAGS} \\
    -DLIBJPEG_LIBRARY=${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a \\
    -DLIBJPEG_LIBRARIES=${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a \\
    -DLIBJPEG_LIB_PATH=${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a \\
    -DLIBJPEG_LIBDIR=${EXTERNALS_PREFIX_PATH}/lib \\
    -DLIBJPEG_H_PATH=${EXTERNALS_PREFIX_PATH}/include \\
    -DLIBJPEG_INCLUDEDIR=${EXTERNALS_PREFIX_PATH}/include \\
    -Dpc_libjpeg_LIBRARY=${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a \\
    -Dpc_libjpeg_LIBRARIES=${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a \\
    -Dpc_libjpeg_LIB_PATH=${EXTERNALS_PREFIX_PATH}/lib/libjpeg.a \\
    -Dpc_libjpeg_LIBDIR=${EXTERNALS_PREFIX_PATH}/lib \\
    -Dpc_libjpeg_H_PATH=${EXTERNALS_PREFIX_PATH}/include \\
    -Dpc_libjpeg_INCLUDEDIR=${EXTERNALS_PREFIX_PATH}/include \\
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES} \\
    -DINSTALL_EXAMPLES=OFF \\
    -DBUILD_DOC_DIST=OFF \\
    -DCMAKE_INSTALL_LIBDIR=${EXTERNALS_PREFIX_PATH}/lib \\
    -DCMAKE_INSTALL_PREFIX=${EXTERNALS_PREFIX_PATH}
    
    LOG_DOWNLOAD ON
    # LOG_CONFIGURE ON
    # LOG_BUILD ON
  )

add_library(qpdf STATIC IMPORTED)
set_target_properties(qpdf PROPERTIES IMPORTED_LOCATION ${EXTERNALS_PREFIX_PATH}/lib/libqpdf.a)
add_dependencies(qpdf extlib_qpdf)
