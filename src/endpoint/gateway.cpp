#include <gateway.hpp>

namespace endpoint {

gateway_t::gateway_t( gateway_t::callback_t && callback, gateway_t::receiver_t && receiver ) noexcept
    : m_callback( std::move( callback ) )
    , m_receiver( std::move( receiver ) )
{
    assert( m_receiver );
}

void gateway_t::run() noexcept
{
    m_receiver->run( this );
}

void gateway_t::on_receive( core::datagram_t && datagram ) noexcept
{
    try
    {
        core::task_t task{ std::move( datagram ) };
        m_callback( std::move( task ) );
    }
    catch ( const std::exception & exception )
    {
        core::logger_t{} << "failed to parse datagram:" << exception.what();
    }
}

}



