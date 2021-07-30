set( target_name net )
add_library( ${target_name} STATIC )

target_include_directories( ${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

set( build_flags ${CMAKE_CXX_FLAGS} )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
	set( build_flags "-march=native -Wall -Wextra ${build_flags}" )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
	set( build_flags "-march=native -Wall -Wextra ${build_flags}" )
else()
	message( SEND_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}" )
endif()
set_target_properties( ${target_name} PROPERTIES COMPILE_FLAGS ${build_flags} )
set_target_properties( ${target_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON )

target_link_libraries( ${target_name} core )

target_sources(
		${target_name} PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/socket.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/socket.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/address.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/address.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/datagram.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/datagram.cpp"

)

