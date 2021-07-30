#include <socket.hpp>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>

namespace net {

socket_sender_t::~socket_sender_t() noexcept
{
    if( k_invalid_socket != m_socket )
    {
        close( m_socket );
    }
}

void socket_sender_t::send( datagram_t && datagram ) noexcept
{
    m_datagrams_stream.push( std::move( datagram ) );
}

socket_sender_t::socket_sender_t( address_t address ) noexcept( false )
    : m_address( address )
    , m_datagrams_stream( "socket-sender", *this )
{
    m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if( k_invalid_socket == m_socket )
    {
        throw std::runtime_error( "failed to create socket" );
    }
    
    if( const auto bound = bind( m_socket, m_address.as_generic(), sizeof( address_t::native_type ) ); bound < 0 )
    {
        close( m_socket );
        m_socket = k_invalid_socket;
        throw std::runtime_error( "failed to bind socket: " + m_address.get_ip() + ":" + std::to_string( m_address.get_port() ) );
    }
    
    std::atomic_bool running{ false };
    std::thread thread {
        [ this, &running ]() noexcept
        {
            running = true;
            m_datagrams_stream.run();
        }
    };
    thread.detach();
    while( !running.load() )
    {
        std::this_thread::yield();
    }
}

void socket_sender_t::on_event( datagram_t && datagram ) noexcept
{
    const auto & address = datagram.get_address();
    const auto & bytes = datagram.get_bytes();
    
    const auto ok = sendto(
        m_socket,
        bytes.data(), bytes.size(),
        MSG_CONFIRM,
        address.as_generic(), sizeof( address_t::native_type )
    );
    
    //TODO proper handle
    if( ok == -1 || std::size_t( ok ) != bytes.size() )
    {
        std::cerr << "sendto failed";
    }
}

}

