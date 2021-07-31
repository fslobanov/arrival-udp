#pragma once

#include <task.hpp>
#include <event_stream.hpp>

namespace endpoint {

class task_processor final : public core::event_stream_t< core::task_t >::subscriber_t
{
public:
    task_processor() noexcept;
    ~task_processor() noexcept;
    
    void operator()( core::task_t && task ) noexcept;
    
private:
    core::event_stream_t< core::task_t > m_tasks_stream;

private:
    void on_event( core::task_t && event ) noexcept override;
};

}



