
message(STATUS "entering in extlib_cxxopts.cmake")

set(ext_name_cxxopts "cxxopts")

if(USE_SYSTEM_DEPS)
    message(STATUS "using system-deps in extlib_cxxopts.cmake")

    # this will define the cxxopts target
    find_package(cxxopts REQUIRED)

    add_library(${ext_name_cxxopts} INTERFACE IMPORTED)
    add_dependencies(${ext_name_cxxopts} cxxopts)

else()
    message(STATUS "ignoring system-deps extlib_utf8.cmake")

    include(ExternalProject)
    include(CMakeParseArguments)

    set(CXXOPTS_TAG v2.2.0)
    set(CXXOPTS_URL https://github.com/jarro2783/cxxopts.git)

    ExternalProject_Add(extlib_cxxopts
	PREFIX extlib_cxxopts

    	GIT_REPOSITORY ${CXXOPTS_URL}
    	GIT_TAG ${CXXOPTS_TAG}

    	UPDATE_COMMAND ""
    	CONFIGURE_COMMAND ""

    	BUILD_COMMAND ""
    	BUILD_ALWAYS OFF

	INSTALL_DIR     ${EXTERNALS_PREFIX_PATH}
    	INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include/ ${EXTERNALS_PREFIX_PATH}/include/

    	LOG_DOWNLOAD ON
    	LOG_BUILD ON
    )

    add_library(${ext_name_cxxopts} INTERFACE IMPORTED)
    add_dependencies(${ext_name_cxxopts} extlib_cxxopts)
    set_target_properties(${ext_name_cxxopts} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${EXTERNALS_PREFIX_PATH}/include)

endif()