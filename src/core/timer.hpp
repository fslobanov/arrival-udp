#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <mutex>

namespace core {

class timer_t final
{
public:
    using callback_t = std::function< void( void ) >;
    using duration_t = std::chrono::milliseconds;

public:
    explicit timer_t( std::string name, duration_t period, callback_t && callback ) noexcept;
    ~timer_t() noexcept;
    
    void run() noexcept;
    void shutdown() noexcept;

private:
    const std::string m_name;
    const duration_t m_duration;
    const callback_t m_callback;
    
    std::thread m_thread;
    
    std::once_flag m_run_once;
    std::atomic_bool m_running;
    std::once_flag m_shutdown_once;

private:
    void run_loop() noexcept;
};

}



