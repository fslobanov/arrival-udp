#pragma once

#include <functional>
#include <stdexcept>

#include <event_stream.hpp>

#include <address.hpp>
#include <datagram.hpp>

namespace net {

using socket_descriptor_t = int;
constexpr socket_descriptor_t k_invalid_socket = -1;

class sender_t
{
public:
    virtual ~ sender_t() noexcept = default;

public:
    virtual void send( datagram_t && datagram ) noexcept = 0;
};

class receiver_t
{
public:
    class subscriber_t
    {
    public:
        virtual ~subscriber_t() noexcept = default;
        virtual void on_receive( datagram_t && datagram ) noexcept = 0;
    };

public:
    explicit receiver_t( subscriber_t & subscriber ) noexcept
        : m_subscriber( subscriber )
    {
    
    }
    virtual ~receiver_t() noexcept = default;

protected:
    void notify( datagram_t && datagram ) noexcept
    {
        m_subscriber.on_receive( std::move( datagram ) );
    }

private:
    subscriber_t & m_subscriber;
};

class socket_sender_t final : public sender_t
                            , public core::event_stream_t< datagram_t >::subscriber_t
{
public:
    explicit socket_sender_t( address_t address ) noexcept ( false );
    ~socket_sender_t() noexcept final;
    
    void send( datagram_t && datagram ) noexcept override;

private:
    address_t m_address;
    ::core::event_stream_t< datagram_t > m_datagrams_stream;
    socket_descriptor_t m_socket = k_invalid_socket;
    
private:
    void on_event( datagram_t && event ) noexcept override;
};

class socket_receiver_t final : public receiver_t
{
public:
    ~socket_receiver_t() noexcept override = default;

private:
    socket_descriptor_t m_socket = k_invalid_socket;
};

}



