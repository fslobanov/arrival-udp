set( target_name net )
add_library( ${target_name} STATIC )

target_link_libraries( ${target_name} core )

target_include_directories( ${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
set_target_properties( ${target_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON )

target_sources(
		${target_name} PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/socket.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/socket.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/address.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/address.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/datagram.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/datagram.cpp"

)

