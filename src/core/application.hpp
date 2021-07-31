#pragma once

#include <cassert>
#include <mutex>
#include <condition_variable>

namespace core {

class application_t final
{
public:
    application_t( std::string name, signed argument_count, char ** arguments ) noexcept;

public:
    void await() noexcept;
    void shutdown() noexcept;

private:
    enum class state_e : uint32_t
    {
        standby, running, shutdown
    };
    
    state_e m_state;
    std::mutex m_mutex;
    std::condition_variable m_notifier;
};

}



