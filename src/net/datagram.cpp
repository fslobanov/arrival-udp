#include <datagram.hpp>

#include <ostream>

namespace net {

datagram_t::datagram_t( const address_t & m_address, core::bytes_t && m_bytes ) noexcept( false )
    : m_address( m_address )
    , m_bytes( m_bytes )
{
    if( m_bytes.size() > k_max_size )
    {
        throw std::invalid_argument( "datagram size should be less than 512 bytes, got " + std::to_string( m_bytes.size() ) );
    }
}

const address_t & datagram_t::get_address() const noexcept
{
    return m_address;
}

const core::bytes_t & datagram_t::get_bytes() const noexcept
{
    return m_bytes;
}

std::ostream & operator <<( std::ostream & os, const datagram_t & datagram ) noexcept
{
    os << "datagram: " << datagram.m_address.get_ip()
       << ":" << datagram.get_address().get_port()
       << ", size - " << datagram.get_bytes().size() << " bytes";
    
    return os;
}

}

