#include <address.hpp>

#include <stdexcept>
#include <cassert>
#include <tuple>

namespace core {

namespace {
constexpr const std::string_view k_localhost = "127.0.0.1";
constexpr const auto k_protocol = AF_INET;
constexpr static auto k_length = INET_ADDRSTRLEN;
}

address_t::address_t( address_t::known_e ip, address_t::port_type port ) noexcept( false )
{
    m_address.sin_family = k_protocol;
    m_address.sin_port = htons( port );
    
    switch ( ip )
    {
        default:
            throw std::invalid_argument( "unknown ip value" );
    
        case known_e::localhost:
        {
            const auto ok = inet_pton( AF_INET, k_localhost.data(), &m_address.sin_addr.s_addr );
            assert( ok > 0 );
        }
        break;
    }
}

address_t::address_t( std::string_view ip, address_t::port_type port ) noexcept( false )
{
    m_address.sin_family = k_protocol;
    m_address.sin_port = htons( port );
    
    if( const auto ok = inet_pton( AF_INET, ip.data(), &m_address.sin_addr.s_addr )
        ; ok <= 0 )
    {
        throw std::invalid_argument( std::string{ "invalid ip: " }.append( ip ) );
    }
}

address_t::address_t( address_t::native_type value ) noexcept
    : m_address( value )
{

}

const address_t::native_type * address_t::as_native() const noexcept
{
    return &m_address;
}

const address_t::generic_type * address_t::as_generic() const noexcept
{
    return reinterpret_cast< const generic_type * >( &m_address );
}

std::string address_t::get_ip() const noexcept
{
    char buffer[ k_length ];
    auto ok = inet_ntop( k_protocol, &( m_address.sin_addr ), buffer, k_length );
    assert( ok );
    return std::string{ buffer };
}

address_t::port_type address_t::get_port() const noexcept
{
    return ntohs( m_address.sin_port );
}

bool address_t::operator ==( const address_t & other ) const noexcept
{
    const auto static make_tie = []( const address_t & a ) noexcept
    {
        return std::tie( /*a.m_address.sin_family,*/
                         a.m_address.sin_addr.s_addr,
                         a.m_address.sin_port );
    };
    return make_tie( *this ) == make_tie( other );
}

bool address_t::operator !=( const address_t & other ) const noexcept
{
    return !( *this == other );
}

std::ostream & operator <<( std::ostream & os, const address_t & address ) noexcept
{
    os << ( address.get_ip() + ":" + std::to_string( address.get_port() ) );
    return os;
}

}
