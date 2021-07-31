set( target_name core )
add_library( ${target_name} STATIC )

find_package( Threads REQUIRED )
target_link_libraries( ${target_name} PUBLIC Threads::Threads )

target_include_directories( ${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
set_target_properties( ${target_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON )

target_sources(
		${target_name} PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/timestamp.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/timestamp.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/signal_handler.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/signal_handler.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/timer.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/application.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/application.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/logger.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/socket.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/socket.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/address.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/address.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/datagram.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/datagram.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/task.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/task.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/event_stream.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/core.hpp"
)

