#pragma once


#include <cstdint>
#include <vector>
#include <thread>

namespace core {

using byte_t = std::byte;
using bytes_t = std::vector< byte_t >;

struct thread_join_guard final
{
    std::thread & m_thread;
    
    ~thread_join_guard() noexcept
    {
        if( m_thread.joinable() )
        {
            m_thread.join();
        }
    }
};



}