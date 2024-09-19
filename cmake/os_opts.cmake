
message(STATUS "entering in 'os_opts.cmake'")

if(WIN32)
   message(STATUS "compiling on windows")

   set(LIB_LINK qpdf jpeg utf8 z)

elseif(APPLE)
   message(STATUS "compiling on mac-osx")

   #set(CMAKE_MACOSX_RPATH 1)

   find_library(FoundationLib Foundation)
   #message("LIB: ${FoundationLib}")

   find_library(SystemConfigurationLib SystemConfiguration)
   #message("LIB: ${SystemConfigurationLib}")

   set(LIB_LINK qpdf jpeg utf8 z)	

   list(APPEND LIB_LINK ${FoundationLib} ${SystemConfigurationLib})
   
elseif(UNIX)
   message(STATUS "compiling on linux")

   set(LIB_LINK qpdf jpeg utf8 z)
   
   list(APPEND LIB_LINK dl m pthread rt resolv)

endif()
