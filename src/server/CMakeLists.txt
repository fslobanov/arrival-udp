set( target_name arrival-server-impl )
add_library( ${target_name} STATIC )

set( executable_target_name arrival-server )
add_executable( ${executable_target_name} )

target_link_libraries( ${target_name} core )
target_link_libraries( ${executable_target_name} PUBLIC ${target_name} )

target_include_directories( ${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )

set_target_properties( ${target_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON )
set_target_properties( ${executable_target_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON )

target_sources(
		${target_name} PUBLIC

		"${CMAKE_CURRENT_SOURCE_DIR}/gateway.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/gateway.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/task_generator.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/task_generator.cpp"
)

target_sources(
		${executable_target_name} PUBLIC

		"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)