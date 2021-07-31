cmake_minimum_required( VERSION 3.11 )
project( ArrivalUdp
         VERSION 1.0
         DESCRIPTION "Multicast time server and client"
         HOMEPAGE_URL "https://github.com/fslobanov/arrival-udp"
         LANGUAGES "CXX" )

option( ArrivalUdp_BUILD_TESTS "Build tests" ON )


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

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -Wall -Wextra" )
else()
	message( SEND_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}" )
endif()

set( ArrivalUdp_AVAILABLE_SANITIZERS none address thread leak undefined )
set( ArrivalUdp_SANITIZER none CACHE STRING "Current portal sanitizer" )
set_property( CACHE ArrivalUdp_SANITIZER PROPERTY STRINGS ${ArrivalUdp_AVAILABLE_SANITIZERS} )
if( NOT ArrivalUdp_SANITIZER IN_LIST ArrivalUdp_AVAILABLE_SANITIZERS )
	message( FATAL_ERROR "ArrivalUdp_SANITIZER must be one of ${ArrivalUdp_AVAILABLE_SANITIZERS}" )
endif()

if( none STREQUAL ArrivalUdp_SANITIZER )
	message( WARNING "arrival-udp: no sanitizer will be used, you can use one of ${ArrivalUdp_AVAILABLE_SANITIZERS}" )
else()
	message( WARNING "arrival-udp: '${ArrivalUdp_SANITIZER}' sanitizer will be used" )
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=${ArrivalUdp_SANITIZER}" )
	set( CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=${ArrivalUdp_SANITIZER}" )
endif()

message( STATUS "arrival-udp: compiler flags are '${CMAKE_CXX_FLAGS}'" )

add_subdirectory( googletest-release-1.11.0 )
add_subdirectory( src )

if( ArrivalUdp_BUILD_TESTS )
	include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} )
	add_subdirectory( test )
endif()