#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gateway.hpp>

using namespace core;
using namespace server;

TEST( ServerGateway, Construct )
{
    class mock_t : public datagram_sender_t
    {
        MOCK_METHOD( void, send,( datagram_t && datagram ), ( noexcept, override ) );
    };
    EXPECT_NO_THROW( gateway_t{ std::make_unique< mock_t >() } );
    ASSERT_DEATH( gateway_t{ nullptr }, "null sender" );
}

TEST( ServerGateway, BadTs )
{
    class mock_t : public datagram_sender_t
    {
    public:
        MOCK_METHOD( void, send, ( datagram_t && datagram ), ( noexcept, override ) );
    };
    
    auto m = std::make_shared< mock_t >();
    gateway_t gw{ m };
    EXPECT_CALL( *m, send( ::testing::An< datagram_t && >() ) ).Times( 0 );
    
    gw.post( task_t{ {},  address_t{ "127.0.0.1", 6666 } } );
    
    std::this_thread::sleep_for( std::chrono::milliseconds{ 100 } );
}

TEST( ServerGateway, GoodTs )
{
    constexpr const static auto c = 10;
    class mock_t : public datagram_sender_t
    {
    public:
        MOCK_METHOD( void, send, ( datagram_t && datagram ), ( noexcept, override ) );
    };
    
    auto m = std::make_shared< mock_t >();
    gateway_t g{ m };
    EXPECT_CALL( *m, send( ::testing::An< datagram_t && >() ) ).Times( c );
    
    for( auto i = 0; i < c; ++ i )
    {
        g.post( task_t{ timestamp_t::now(),  address_t{ "127.0.0.1", 6666 } } );
    }
    
    std::this_thread::sleep_for( std::chrono::milliseconds{ 100 } );
}

TEST( ServerGateway, SameAddress )
{
    struct mock_t : public datagram_sender_t
    {
        mock_t( address_t a ) : a( a ) {}
        address_t a;
        void send( datagram_t && d ) noexcept override
        {
            EXPECT_EQ( d.get_address(),a );
        }
    };
    
    address_t a{ "127.0.0.1", 6666 };
    auto m = std::make_shared< mock_t >( a );
    gateway_t gw{ m };
    gw.post( task_t{ timestamp_t::now(), a } );
    
    std::this_thread::sleep_for( std::chrono::milliseconds{ 100 } );
}
