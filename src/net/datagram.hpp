#pragma once

#include <core.hpp>
#include <address.hpp>

namespace std {
template< typename, typename >
class basic_ostream;
}

namespace net {

class datagram_t final
{
public:
    constexpr const static std::size_t k_max_size = 512;

public:
    datagram_t( const address_t & m_address, core::bytes_t && m_bytes ) noexcept( false );
    
    datagram_t( const datagram_t & ) = default;
    datagram_t & operator=( const datagram_t & ) = default;
    
    datagram_t( datagram_t && ) = default;
    datagram_t & operator=( datagram_t && ) = default;

public:
    const address_t & get_address() const noexcept;
    const core::bytes_t & get_bytes() const noexcept;
    
    friend std::ostream & operator <<( std::ostream & os, const datagram_t & datagram ) noexcept;

private:
    address_t m_address;
    core::bytes_t m_bytes;
};

}



