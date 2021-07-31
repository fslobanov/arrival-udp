#pragma once

#include <optional>
#include <string_view>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <ostream>

namespace core {

class address_t final
{
public:
    using generic_type = sockaddr;
    using native_type = sockaddr_in;
    using port_type = uint16_t;
    
    enum class known_e : uint32_t
    {
        localhost
    };
    
public:
    address_t( known_e ip, port_type port ) noexcept( false );
    address_t( std::string_view ip, port_type port ) noexcept( false );
    address_t( native_type address ) noexcept;
    
    address_t( const address_t & ) = default;
    address_t & operator=( const address_t & ) = default;
    
    address_t( address_t && ) = default;
    address_t & operator=( address_t && ) = default;

public:
    const native_type * as_native() const noexcept;
    const generic_type * as_generic() const noexcept;
    
    std::string get_ip() const noexcept;
    port_type get_port() const noexcept;
    
    friend std::ostream & operator <<( std::ostream & os, const address_t & address ) noexcept;

private:
    native_type m_address;
};

}



