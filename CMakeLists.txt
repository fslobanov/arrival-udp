cmake_minimum_required( VERSION 3.11 )
project( arrival-udp
         VERSION 1.0
         DESCRIPTION "Multicast time server and client"
         HOMEPAGE_URL "https://github.com/fslobanov/arrival-udp"
         LANGUAGES "CXX" )

if( NOT CMAKE_CXX_STANDARD )
	set( CMAKE_CXX_STANDARD 17 )
	set( CMAKE_CXX_STANDARD_REQUIRED ON )
endif()

if( NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
	message( SEND_ERROR "arrival-udp: Only x86_64 arch supported, but current arch is '${CMAKE_SYSTEM_PROCESSOR}'" )
endif()

if( NOT UNIX OR NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" )
	message( SEND_ERROR "arrival-udp: Only Linux OS supported, but current OS is '${CMAKE_SYSTEM_NAME}'" )
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/artifacts" CACHE STRING "runtime output directory" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE STRING "release runtime output directory" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE STRING "debug runtime output directory" )

add_subdirectory( src )