#include <timer.hpp>
#include <cassert>
#include <core.hpp>
#include <logger.hpp>

namespace core {

timer_t::timer_t( std::string name, timer_t::duration_t period, timer_t::callback_t && callback ) noexcept
    : m_name( std::move( name ) )
    , m_duration( period )
    , m_callback( std::move( callback ) )
    , m_running( false )
{
    assert( m_duration.count() > 0 );
    assert( m_callback );
}

timer_t::~timer_t() noexcept
{
    logger_t{} << m_name << "dtor entrance";
    shutdown();
    logger_t{} << m_name << "dtor done";
}

void timer_t::run() noexcept
{
    std::call_once(
        m_run_once,
        [ this ]() noexcept
        {
            m_thread = std::thread{
                [ this ]() noexcept
                {
                    logger_t{} << m_name << "thread started";
                    m_running = true;
                    run_loop();
                    logger_t{} << m_name << "thread quit";
                }
            };
            
            while( !m_running )
            {
                std::this_thread::yield();
            }
        }
    );
}

void timer_t::shutdown() noexcept
{
    std::call_once(
        m_shutdown_once,
        [ this ]() noexcept
        {
            logger_t{} << m_name << "shutting down timer";
            m_running = false;
            const thread_join_guard guard{ m_thread };
            logger_t{} << m_name << "timer shutdown done";
        }
    );
}

void timer_t::run_loop() noexcept
{
    while( m_running )
    {
        std::this_thread::sleep_for( m_duration );
        if( m_running )
        {
            m_callback();
        }
        else
        {
            break;
        }
    }
    logger_t{} << m_name << "loop quit";
}

}
