#include <application.hpp>
#include <signal_handler.hpp>

namespace core {

application_t::application_t( signed argument_count, char ** arguments ) noexcept
    : m_state( state_e::standby )
{
    ( void ) argument_count;
    ( void ) arguments;
    
    core::signal_handler_t::get_instance().add_observer(
        [ this ]( core::signal_handler_t::action_e ) noexcept
        {
            shutdown();
        }
    );
}

void application_t::run() noexcept
{
    std::unique_lock< std::mutex > lock{ m_mutex };
    
    core::signal_handler_t::get_instance().run();
    assert( m_state == state_e::standby );
    
    m_notifier.wait(
        lock,
        [ this ]() noexcept
        {
            return state_e::shutdown == m_state;
        }
    );
}

void application_t::shutdown() noexcept
{
    std::unique_lock< std::mutex > lock{ m_mutex };
    m_state = state_e::shutdown;
    lock.unlock();
    m_notifier.notify_one();
}

}

