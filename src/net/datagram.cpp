/*
 * \file datagram.cpp
 * \copyright (C) 2021 Special Technological Center Ltd
 * \author : Lobanov F.S.
 * \date : 28.07.2021
 * \time : 18:21
 * \brief : datagram implementation
 */

#include "datagram.hpp"

namespace net {

datagram_t::datagram_t( const address_t & m_address, datagram_t::bytes_t && m_bytes ) noexcept
    : m_address( m_address )
    , m_bytes( m_bytes )
{

}

const address_t & datagram_t::get_address() const noexcept
{
    return m_address;
}

const datagram_t::bytes_t & datagram_t::get_bytes() const noexcept
{
    return m_bytes;
}

}

