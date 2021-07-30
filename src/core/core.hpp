#pragma once

namespace core {

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