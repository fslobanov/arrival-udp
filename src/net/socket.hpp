#pragma once

#include <functional>

#include <address.hpp>
#include <datagram.hpp>

namespace net {

class abstract_socket_t
{
public:
    using receive_callback_t = std::function< void( datagram_t && datagram ) >;
    using error_callback_t = std::function< void( std::string && error ) >;
    
public:
    virtual ~abstract_socket_t() noexcept = default;

public:
    virtual void send( datagram_t && datagram ) noexcept = 0;
};

class socket_t final : public abstract_socket_t
{


};

}



