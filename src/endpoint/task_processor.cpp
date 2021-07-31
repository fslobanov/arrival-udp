#include <task_processor.hpp>

namespace endpoint {

task_processor::task_processor() noexcept
    : m_tasks_stream( "task-processor", *this )
{
    m_tasks_stream.run();
}

task_processor::~task_processor() noexcept
{
    m_tasks_stream.shutdown();
}

void task_processor::operator ()( core::task_t && task ) noexcept
{
    m_tasks_stream.push( std::move( task ) );
}

void task_processor::on_event( core::task_t && event ) noexcept
{
    auto endpoint = core::timestamp_t::now();
    auto server = event.m_timestamp;
    assert( server && endpoint );
    
    const auto diff = std::chrono::duration_cast< std::chrono::milliseconds >( *endpoint - *server ).count();
    
    const bool server_greater = *server > *endpoint;
    const auto msec = ( server_greater ? '-' : '+' ) + std::to_string( diff );
    
    core::logger_t{} << "## Got task from server:\nEndpoint time is" << endpoint
                     << "\nServer time is" << server
                     << "\nDifference is"
                     << msec << "milliseconds from server";
}

}



