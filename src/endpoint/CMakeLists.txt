set( target_name arrival-endpoint )
add_executable( ${target_name} )

target_link_libraries( ${target_name} net )

target_include_directories( ${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
set_target_properties( ${target_name} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON )

target_sources(
		${target_name} PRIVATE

		"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)