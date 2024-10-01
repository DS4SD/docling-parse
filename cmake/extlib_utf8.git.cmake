
message(STATUS "entering in extlib_utf8.cmake")

set(ext_name "utf8")

if(USE_SYSTEM_DEPS)
    # this will define the utf8cpp target
    find_package(utf8cpp REQUIRED)

    add_library(${ext_name} INTERFACE IMPORTED)
    add_dependencies(${ext_name} utf8cpp)

else()

    include(ExternalProject)
    include(CMakeParseArguments)

    set(UTF8_URL https://github.com/nemtrif/utfcpp.git)
    set(UTF8_TAG v4.0.5)

    ExternalProject_Add(extlib_utf8
        PREFIX extlib_utf8

        GIT_REPOSITORY ${UTF8_URL}
        GIT_TAG ${UTF8_TAG}

        UPDATE_COMMAND ""
        CONFIGURE_COMMAND ""

        BUILD_COMMAND ""
        BUILD_ALWAYS OFF

        INSTALL_DIR     ${EXTERNALS_PREFIX_PATH}
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/source ${EXTERNALS_PREFIX_PATH}/include
    )

    add_library(${ext_name} INTERFACE IMPORTED)
    add_dependencies(${ext_name} extlib_utf8)
    set_target_properties(${ext_name} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${EXTERNALS_PREFIX_PATH}/include)

endif()
