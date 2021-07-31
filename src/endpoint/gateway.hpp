#pragma once

#include <functional>

#include <task.hpp>
#include <socket.hpp>

namespace endpoint {

class gateway_t final : public core::datagram_receiver_t::subscriber_t
{
public:
    using callback_t = std::function< void( core::task_t && ) >;
    using receiver_t = std::unique_ptr< core::datagram_receiver_t >;

public:
    gateway_t( callback_t && callback, receiver_t && receiver ) noexcept;

    void run() noexcept;

private:
    callback_t m_callback;
    receiver_t m_receiver;

private:
    void on_receive( core::datagram_t && datagram ) noexcept override;
};
}



