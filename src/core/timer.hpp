#pragma once

#include <thread>
#include <atomic>
#include <functional>

namespace core {

class timer_t final
{
public:
    using callback_t = std::function< void( void ) >;
    using duration_t = std::chrono::milliseconds;

public:
    explicit timer_t( duration_t period, callback_t && callback ) noexcept;
    ~timer_t() noexcept;
    
    void run() noexcept;
    void shutdown() noexcept;

private:
    const duration_t m_duration;
    const callback_t m_callback;
    std::atomic_bool m_running;
    std::thread m_thread;

private:
    void run_loop() noexcept;
};

}



