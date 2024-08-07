cmake_minimum_required(VERSION 3.12)

project(docling_parse VERSION 1.0.0 LANGUAGES CXX C)
#set(CMAKE_VERBOSE_MAKEFILE off)

message(STATUS "cmake osx-deployment: " ${CMAKE_OSX_DEPLOYMENT_TARGET})

# minimum macosx, ignored on other platforms
if(APPLE)
    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        message(STATUS "cmake is not set")            
    elseif(CMAKE_OSX_DEPLOYMENT_TARGET VERSION_LESS 10.15)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum macOS version" FORCE)
    endif()
endif()

message(STATUS "cmake system-version: " ${CMAKE_SYSTEM_VERSION})
message(STATUS "cmake osx-deployment: " ${CMAKE_OSX_DEPLOYMENT_TARGET})

# ************************
# ***     paths        ***
# ************************

if(NOT DEFINED TOPLEVEL_PREFIX_PATH)
    set(TOPLEVEL_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX ${TOPLEVEL_PREFIX_PATH}/install_dir)
endif()

if(NOT DEFINED EXTERNALS_PREFIX_PATH)
    set(EXTERNALS_PREFIX_PATH "${TOPLEVEL_PREFIX_PATH}/externals" CACHE INTERNAL "")
endif()

if(NOT DEFINED RESOURCES_PREFIX_PATH)
    set(RESOURCES_PREFIX_PATH "${TOPLEVEL_PREFIX_PATH}/resources" CACHE INTERNAL "")
endif()

if(NOT "${TOPLEVEL_PREFIX_PATH}/cmake" IN_LIST CMAKE_MODULE_PATH)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${TOPLEVEL_PREFIX_PATH}/cmake")
endif()

if(NOT DEFINED CMAKE_PDF_DATA_DIR)
    set(CMAKE_PDF_DATA_DIR "${TOPLEVEL_PREFIX_PATH}/docling_parse/pdf_resources")
endif()

set(ENV_ARCH $ENV{CIBW_ARCHS})
message(STATUS "cibw arch: " ${ENV_ARCH})

set(CMAKE_OSX_ARCHITECTURES $ENV{CIBW_ARCHS})
#message(STATUS "cibw arch: " ${ENV_ARCH})

set(ENV_ARCHFLAGS $ENV{ARCHFLAGS})
message(STATUS "arch flags: " ${ENV_ARCHFLAGS})

message(STATUS "      top path: " ${TOPLEVEL_PREFIX_PATH})
message(STATUS "      lib path: " ${EXTERNALS_PREFIX_PATH})
message(STATUS "  install path: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "    cmake path: " ${CMAKE_MODULE_PATH})
message(STATUS "  cmake system: " ${CMAKE_SYSTEM_PROCESSOR})
message(STATUS "cmake osx arch: " ${CMAKE_OSX_ARCHITECTURES})

# ************************
# ***  cxx-flags       ***
# ************************

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -g3 -DROOT_PATH='\"${TOPLEVEL_PREFIX_PATH}\"' ${ENV_ARCHFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare -O3 -DROOT_PATH='\"${TOPLEVEL_PREFIX_PATH}\"' ${ENV_ARCHFLAGS}")

message(STATUS "cxx-compiler: " ${CMAKE_CXX_COMPILER})
message(STATUS "cxx-flags   : " ${CMAKE_CXX_FLAGS})
message(STATUS "cxx-standard: " ${CMAKE_CXX_STANDARD})

# ************************
# *** libraries        ***
# ************************

if(NOT EXISTS ${EXTERNALS_PREFIX_PATH})
  execute_process(COMMAND mkdir ${EXTERNALS_PREFIX_PATH})
  execute_process(COMMAND mkdir ${EXTERNALS_PREFIX_PATH}/bin)
  execute_process(COMMAND mkdir ${EXTERNALS_PREFIX_PATH}/lib)
  execute_process(COMMAND mkdir ${EXTERNALS_PREFIX_PATH}/include)
  execute_process(COMMAND mkdir ${EXTERNALS_PREFIX_PATH}/resources)
endif()

include(cmake/subdirlist.cmake)

include(cmake/extlib_json.cmake)
include(cmake/extlib_utf8.git.cmake)
include(cmake/extlib_jpeg.cmake)
# include(cmake/extlib_qpdf_v10.cmake)
include(cmake/extlib_qpdf_v11.cmake)
include(cmake/extlib_pybind11.cmake)

include(cmake/os_opts.cmake)

# ***********************
# ***  includes       ***
# ***********************

include_directories(${EXTERNALS_PREFIX_PATH}/include)

subdirlist(EXTERNALS_INCLUDES ${EXTERNALS_PREFIX_PATH}/include)	
include_directories(${EXTERNALS_INCLUDES})

message(STATUS "subdirs: " ${SUBDIRS})

include_directories(${TOPLEVEL_PREFIX_PATH}/src)

subdirlist(SUBDIRS ${TOPLEVEL_PREFIX_PATH}/src)
include_directories(${SUBDIRS})

# **********************
# ***  Executables   ***
# **********************

add_executable(parse.exe "${TOPLEVEL_PREFIX_PATH}/app/parse.cpp")

set_property(TARGET parse.exe PROPERTY CXX_STANDARD 20)

target_link_libraries(parse.exe ${LIB_LINK})

# **********************
# ***  Libraries     ***
# **********************

add_library(parse STATIC "${TOPLEVEL_PREFIX_PATH}/app/parse.cpp")

set_target_properties(parse PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(parse ${LIB_LINK})

# ***************************
# ***  Python-binding     ***
# ***************************

# inspiration from
# 1. https://github.com/pybind/cmake_example/blob/master/CMakeLists.txt`
# 2. https://blog.devgenius.io/calling-python-and-c-code-using-pybind-99ab7fefa685
# 3. https://github.com/pybind/cmake_example/issues/11

include(FetchContent)
FetchContent_Declare(pybind11
  FETCHCONTENT_BASE_DIR extlib_pybind11 
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.10.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(docling_parse "${TOPLEVEL_PREFIX_PATH}/app/pybind_parse.cpp")

add_dependencies(docling_parse json utf8)  # we have to list the headers-only deps here
target_compile_definitions(docling_parse PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

target_link_libraries(docling_parse PRIVATE parse)

# *****************
# ***  Install  ***
# *****************

install(TARGETS docling_parse DESTINATION "${TOPLEVEL_PREFIX_PATH}/docling_parse")

# *****************
# ***  Testing  ***
# *****************

#function(do_test target arg result)
#  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
#  set_tests_properties(Comp${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
# endfunction()

# do a bunch of result based tests
# do_test(Tutorial 4 "4 is 2")

