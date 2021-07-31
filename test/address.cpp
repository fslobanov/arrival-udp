#include <gtest/gtest.h>

#include <address.hpp>

using namespace core;

TEST( Address, UnknownEnum )
{
    using vt = std::underlying_type< address_t::known_e >::type;
    const auto v = static_cast< vt >( address_t::known_e::__max ) + 1;
    const auto u = static_cast< address_t::known_e >( v );
    EXPECT_ANY_THROW( address_t( u, 0 )  );
}

TEST( Address, KnownEnum )
{
    EXPECT_NO_THROW( address_t( address_t::known_e::localhost, 0 ) );
    EXPECT_EQ( "127.0.0.1", address_t( address_t::known_e::localhost, 0 ).get_ip() );
}

TEST( Address, BadIP )
{
    EXPECT_ANY_THROW( address_t( "", 0 ) );
    EXPECT_ANY_THROW( address_t( "a", 0 ) );
    EXPECT_ANY_THROW( address_t( "127.0.0.A", 0 ) );
    EXPECT_ANY_THROW( address_t( "127.О.0.1", 0 ) );
    EXPECT_ANY_THROW( address_t( "127,О,0,1", 0 ) );
    EXPECT_ANY_THROW( address_t( "256.256.256.256", 0 ) );      ;
    EXPECT_ANY_THROW( address_t( std::string( std::size_t{ 1000 }, 'c' ), 0 ) );
}

TEST( Address, GoodIP )
{
    EXPECT_NO_THROW( address_t( "0.0.0.0", 0 ) );
    EXPECT_NO_THROW( address_t( "1.1.1.1", 0 ) );
    EXPECT_NO_THROW( address_t( "192.168.0.1", 0 ) );
    EXPECT_NO_THROW( address_t( "255.255.255.255", 0 ) );
}

TEST( Address, GetIP )
{
    EXPECT_EQ( "127.0.0.1", address_t( "127.0.0.1", 0 ).get_ip() );
    EXPECT_EQ( "0.0.0.0", address_t( "0.0.0.0", 0 ).get_ip() );
    EXPECT_EQ( "255.255.255.255", address_t( "255.255.255.255", 0 ).get_ip() );
}

TEST( Address, GetPort )
{
    EXPECT_EQ( 65535, address_t( address_t::known_e::localhost, 65535 ).get_port() );
    EXPECT_EQ( 666, address_t( address_t::known_e::localhost, 666 ).get_port() );
    EXPECT_EQ( 0, address_t( address_t::known_e::localhost, 0 ).get_port() );
    EXPECT_NE( 9215809281509, address_t( address_t::known_e::localhost, 666 ).get_port() );
}

TEST( Address, Equals )
{
    EXPECT_EQ( address_t( address_t::known_e::localhost, 65535 ), address_t( address_t::known_e::localhost, 65535 ) );
    EXPECT_EQ( address_t( "127.0.0.1", 65535 ), address_t( address_t::known_e::localhost, 65535 ) );
    EXPECT_NE( address_t( "127.0.0.1", 65535 ), address_t( "127.0.0.1", 65534 ) );
    EXPECT_NE( address_t( "127.0.0.2", 65535 ), address_t( address_t::known_e::localhost, 65535 ) );
    EXPECT_NE( address_t( "127.0.0.2", 65535 ), address_t( "127.0.0.1", 65535 ) );
}

TEST( Address, OStream )
{
    using oss = std::ostringstream ;
    
    {
        oss s;
        s << address_t{ "127.0.0.1", 65535 };
        EXPECT_EQ( "127.0.0.1:65535", s.str() );
    }
    
    {
        oss s;
        s << address_t{ "127.0.0.1", 0 };
        EXPECT_EQ( "127.0.0.1:0", s.str() );
    }
    
    {
        oss s;
        s << address_t{ "255.255.255.255", 65535 };
        EXPECT_EQ( "255.255.255.255:65535", s.str() );
    }
}