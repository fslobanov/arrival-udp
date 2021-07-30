#include <timer.hpp>
#include <cassert>

namespace core {

timer_t::timer_t( timer_t::duration_t period, timer_t::callback_t && callback ) noexcept
    : m_duration( period )
    , m_callback( std::move( callback ) )
    , m_running( false )
{
    assert( m_duration.count() > 0 );
    assert( m_callback );
}

timer_t::~timer_t() noexcept
{
    shutdown();
}

void timer_t::run() noexcept
{
    assert( !m_running.load() );
    
    m_thread = std::thread{
        [ this ]() noexcept
        {
            m_running = true;
            run_loop();
        }
    };
    m_thread.detach();
    
    while( !m_running )
    {
        std::this_thread::yield();
    }
}

void timer_t::shutdown() noexcept
{
    m_running.store( false );
}

void timer_t::run_loop() noexcept
{
    while( true )
    {
        if( !m_running )
        {
            return;
        }
        
        std::this_thread::sleep_for( m_duration );
        
        if( m_running )
        {
            m_callback();
        }
        else
        {
            return;
        }
    }
}

}
