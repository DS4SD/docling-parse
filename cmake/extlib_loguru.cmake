
message(STATUS "entering in extlib_loguru.cmake")

set(ext_name_loguru "loguru")

if(USE_SYSTEM_DEPS)
    message(STATUS "using system-deps in extlib_loguru.cmake")

    # this will define the loguru target
    find_package(loguru REQUIRED)

    add_library(${ext_name_loguru} INTERFACE IMPORTED)
    add_dependencies(${ext_name_loguru} loguru)

else()
    message(STATUS "ignoring system-deps extlib_loguru.cmake")

    include(ExternalProject)
    include(CMakeParseArguments)

    set(LOGURU_INCLUDE_DIR ${EXTERNALS_PREFIX_PATH}/include/loguru)
    execute_process(COMMAND mkdir -p ${LOGURU_INCLUDE_DIR})

    set(LOGURU_URL https://github.com/emilk/loguru)
    #set(LOGURU_TAG v2.1.0)

    ExternalProject_Add(extlib_loguru
	PREFIX extlib_loguru

    	GIT_REPOSITORY ${LOGURU_URL}
    	#GIT_TAG ${LOGURU_TAG}

    	UPDATE_COMMAND ""
    	CONFIGURE_COMMAND ""

    	BUILD_COMMAND ""
    	BUILD_ALWAYS OFF

    	INSTALL_DIR     ${EXTERNALS_PREFIX_PATH}
    	INSTALL_COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/loguru.hpp ${LOGURU_INCLUDE_DIR} <SOURCE_DIR>/loguru.cpp ${LOGURU_INCLUDE_DIR}

    	LOG_DOWNLOAD ON
    	LOG_BUILD ON	
    )

	# add_library(loguru INTERFACE)
	# add_custom_target(install_extlib_loguru DEPENDS extlib_loguru)
	# add_dependencies(loguru install_extlib_loguru)

    add_library(${ext_name_loguru} INTERFACE IMPORTED)
    add_dependencies(${ext_name_loguru} extlib_loguru)
    set_target_properties(${ext_name_loguru} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${EXTERNALS_PREFIX_PATH}/include)

endif()