#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <csignal>

namespace core {

class signal_handler_t final
{
public:
    enum class signal_e : int
    {
        none = 0,
        usr2 = SIGUSR2
    };
    
    enum class action_e : uint32_t
    {
        shutdown
    };
    using observer_t = std::function< void( action_e action ) >;
    constexpr static int k_empty_value = 0;

public:
    static signal_handler_t & get_instance() noexcept;
    
    signal_handler_t( const signal_handler_t & ) = delete;
    signal_handler_t & operator =( const signal_handler_t & ) = delete;
    
    signal_handler_t( signal_handler_t && ) = delete;
    signal_handler_t & operator =( signal_handler_t && ) = delete;

private:
    signal_handler_t();
    ~signal_handler_t() = default;
    
public:
    void add_observer( observer_t && observer ) noexcept;
    void run() noexcept;
    
    signal_e get_status() const noexcept;

private:
    std::thread m_thread;
    std::vector< observer_t > m_observers;
    
    sigset_t m_signals;
    static std::atomic< signal_e > g_signal;

private:
    enum class await_result_e : bool
    {
        shutdown, resume
    };
    await_result_e synchronous_signal_await() noexcept;
};

}


