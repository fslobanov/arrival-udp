#include <gateway.hpp>

namespace server {

gateway_t::gateway_t( gateway_t::sender_t && sender ) noexcept
    : m_tasks_stream( "gateway-tasks-stream", *this )
    , m_sender( std::move( sender ) )
{
    assert( m_sender && "null sender" );
    m_tasks_stream.run();
}

gateway_t::~gateway_t() noexcept
{
    m_tasks_stream.shutdown();
}

void gateway_t::post( core::task_t task ) noexcept
{
    m_tasks_stream.push( std::move( task ) );
}

void gateway_t::on_event( core::task_t && task ) noexcept
{
    try {
        auto datagram = task.to_datagram();
        m_sender->send( std::move( datagram ) );
        core::logger_t{} << "task send successful:" << task;
    }
    catch ( const std::exception & exception )
    {
        core::logger_t{} << "failed to serialize task to datagram:" << exception.what();
    }
}

}



