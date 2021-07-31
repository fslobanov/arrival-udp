#include <task_generator.hpp>
#include <cassert>

namespace server {

task_generator_t::task_generator_t( core::timer_t::duration_t duration,
                                    core::address_t destination,
                                    callback_t && callback ) noexcept
    : m_callback( std::move( callback ) )
    , m_destination( destination )
    , m_timer( "generator-timer", duration, [ this ]()noexcept { on_timer(); } )
{
    assert( m_callback );
}

void task_generator_t::run() noexcept
{
    m_callback( core::task_t{ core::timestamp_t::now(), m_destination } );
    m_timer.run();
}

void task_generator_t::on_timer() noexcept
{
    m_callback( core::task_t{ core::timestamp_t::now(), m_destination } );
}

}
