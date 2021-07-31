enable_testing()

add_executable( address address.cpp )
target_link_libraries( address
                       gtest gtest_main
                       gmock gmock_main
                       core )

add_executable( server_gateway server_gateway.cpp )
target_link_libraries(
		server_gateway
		gtest gtest_main
		gmock gmock_main
		arrival-server-impl )

include( GoogleTest )
gtest_discover_tests( address )
gtest_discover_tests( server_gateway )