cmake_minimum_required(VERSION 3.5)

message(STATUS "entering in subdirlist.cmake")

macro(subdirlist result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${curdir}/${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()