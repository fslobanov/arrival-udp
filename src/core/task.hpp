#pragma once


#include <ostream>
#include <cstring>

#include <endian.h>

#include <timestamp.hpp>
#include <datagram.hpp>
#include "logger.hpp"

namespace core {

struct task_t
{
public:
    using msec_count_t = std::uint64_t;
    constexpr static auto k_msec_count_size = sizeof( msec_count_t );
    static_assert( sizeof( timestamp_t::value_type::rep ) == k_msec_count_size );
    
public:
    task_t( timestamp_t timestamp, address_t address ) noexcept;
    task_t( datagram_t && datagram ) noexcept( false );
    
    datagram_t to_datagram() const noexcept( false );
    friend std::ostream & operator <<( std::ostream & os, const task_t & task ) noexcept;
    
    timestamp_t m_timestamp;
    address_t m_address;
};

}