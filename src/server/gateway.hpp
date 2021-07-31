#pragma once

#include <socket.hpp>
#include <task.hpp>

namespace server {

class gateway_t final : public core::event_stream_t< core::task_t >::subscriber_t
{
public:
    using sender_t = std::unique_ptr< core::datagram_sender_t >;

public:
    explicit gateway_t( sender_t && sender ) noexcept;
    ~gateway_t() noexcept final;

public:
    void post( core::task_t task ) noexcept;

private:
    core::event_stream_t< core::task_t > m_tasks_stream;
    sender_t m_sender;

private:
    void on_event( core::task_t && task ) noexcept override;
};

}



