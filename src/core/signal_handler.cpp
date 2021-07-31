#include <signal_handler.hpp>

#include <cassert>
#include <iostream>
#include <cstring>

#include <core.hpp>
#include <logger.hpp>

namespace core {

std::atomic< signal_handler_t::signal_e > signal_handler_t::g_signal = signal_e::none;

signal_handler_t & signal_handler_t::get_instance() noexcept
{
    static signal_handler_t handler;
    return handler;
}

void signal_handler_t::add_observer( signal_handler_t::observer_t && observer ) noexcept
{
    std::lock_guard< std::mutex > guard{ m_mutex };
    m_observers.emplace_back( std::move( observer ) );
}

signal_handler_t::signal_handler_t() noexcept
{
    sigemptyset( &m_signals );
    sigaddset( &m_signals, SIGUSR2 );
    
    auto ok = pthread_sigmask( SIG_BLOCK, &m_signals, nullptr );
    assert( ok == 0 );
}

signal_handler_t::~signal_handler_t() noexcept
{
    //const thread_join_guard guard{ m_thread };
}

void signal_handler_t::run() noexcept
{
    std::lock_guard< std::mutex > guard{ m_mutex };
    if( m_running )
    {
        return;
    }
    m_thread = std::thread{
        [ this ]() noexcept
        {
            m_running = true;
            pthread_setname_np( pthread_self(), "signal-handler" );
    
            try
            {
                while( await_result_e::resume == synchronous_signal_await() )
                {
                
                }
            }
            catch( const std::exception & exception )
            {
                logger_t{} << "exception occurred on signal await:" << exception.what();
                std::terminate();
            }
        }
    };
    
    m_thread.detach();
    while( !m_running )
    {
        std::this_thread::yield();
    }
}

signal_handler_t::signal_e signal_handler_t::get_status() const noexcept
{
    return g_signal.load( std::memory_order_acquire );
}

signal_handler_t::await_result_e signal_handler_t::synchronous_signal_await() noexcept
{
    int signal_value = 0;
    if( auto error = sigwait( &m_signals, &signal_value )
        ; error != 0 )
    {
        logger_t{} << "signal error occurred:" << std::strerror( error );
        return await_result_e::resume;
    }
    logger_t{} << "signal occurred:" << signal_value;
    
    switch ( static_cast< signal_e >( signal_value ) )
    {
        default:
            return await_result_e::resume;
            break;
    
        case signal_e::usr2:
        {
            g_signal.store( signal_e::usr2 );
            for( const auto & observer : m_observers )
            {
                observer( action_e::shutdown );
            }
            return await_result_e::shutdown;
        }
    }
}

}