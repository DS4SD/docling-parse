cmake_minimum_required (VERSION 3.5)

message(STATUS "entering in 'os_opts.cmake'")

if(WIN32)
   message(STATUS "compiling on windows")

elseif(APPLE)
   message(STATUS "compiling on mac-osx")

   #set(CMAKE_MACOSX_RPATH 1)

   find_library(FoundationLib Foundation)
   #message("LIB: ${FoundationLib}")

   find_library(SystemConfigurationLib SystemConfiguration)
   #message("LIB: ${SystemConfigurationLib}")

   #set(LIB_LINK json json_schema loguru cxxopts pcre2 fasttext utf8 andromeda_pos ${OPENSSL_LIBRARIES} ldap z)
   #set(LIB_LINK json loguru cxxopts pcre2 fasttext utf8 andromeda_pos ${OPENSSL_LIBRARIES} ldap z)
   #set(LIB_LINK json loguru cxxopts pcre2 fasttext utf8 ${OPENSSL_LIBRARIES} ldap z)
   #set(LIB_LINK fmt json loguru cxxopts pcre2 sentencepiece sentencepiece_train fasttext utf8 ldap z)
   set(LIB_LINK qpdf jpeg utf8 ldap z)	

   list(APPEND LIB_LINK ${FoundationLib} ${SystemConfigurationLib})
   
elseif(UNIX)
   message(STATUS "compiling on linux")

   #set(LIB_LINK json json_schema loguru cxxopts pcre2 fasttext utf8 andromeda_pos ${OPENSSL_LIBRARIES} z)
   #set(LIB_LINK json loguru cxxopts pcre2 fasttext utf8 andromeda_pos ${OPENSSL_LIBRARIES} z)
   #set(LIB_LINK json loguru cxxopts pcre2 fasttext utf8 ${OPENSSL_LIBRARIES} z)
   #set(LIB_LINK fmt json loguru cxxopts pcre2 sentencepiece sentencepiece_train fasttext utf8 z)
   set(LIB_LINK qpdf jpeg utf8 ldap z)
   
   list(APPEND LIB_LINK dl m pthread rt resolv)

endif()


