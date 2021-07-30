/*
 * \file datagram.hpp
 * \copyright (C) 2021 Special Technological Center Ltd
 * \author : Lobanov F.S.
 * \date : 28.07.2021
 * \time : 18:21
 * \brief : 
 */

#pragma once

#include <cstdint>
#include <vector>

#include <address.hpp>
#include <ostream>

namespace net {

class datagram_t final
{
public:
    using byte_t = std::byte;
    using bytes_t = std::vector< byte_t >;

public:
    datagram_t( const address_t & m_address, bytes_t && m_bytes ) noexcept;
    
    datagram_t( const datagram_t & ) = default;
    datagram_t & operator=( const datagram_t & ) = default;
    
    datagram_t( datagram_t && ) = default;
    datagram_t & operator=( datagram_t && ) = default;

public:
    const address_t & get_address() const noexcept;
    const bytes_t & get_bytes() const noexcept;
    
    friend std::ostream & operator <<( std::ostream & os, const datagram_t & datagram ) noexcept;

private:
    address_t m_address;
    bytes_t m_bytes;
};

}



