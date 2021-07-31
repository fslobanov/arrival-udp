#include <socket.hpp>
#include <sys/socket.h>
#include <unistd.h>
#include <atomic>
#include <logger.hpp>

namespace core {

namespace {
constexpr static const int k_enabled = 1;
constexpr static const timeval k_ten_milliseconds{
    .tv_sec = 0,
    .tv_usec = 10000
};

}

using namespace core;

void datagram_receiver_t::notify( datagram_t && datagram ) noexcept
{
    assert( m_subscriber );
    m_subscriber.load()->on_receive( std::move( datagram ) );
}

void datagram_receiver_t::run( subscriber_t * subscriber ) noexcept
{
    assert( subscriber );
    assert( !m_subscriber );
    m_subscriber = subscriber;
    
    on_run();
}

socket_sender_t::socket_sender_t( std::optional< address_t > bind_address ) noexcept( false )
    : m_bind_address( std::move( bind_address ) )
    , m_datagrams_stream( "socket-sender", *this )
    , m_socket( k_invalid_socket )
{
    m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if( k_invalid_socket == m_socket )
    {
        throw std::runtime_error( "failed to create socket" );
    }
    
    if( const auto ok = setsockopt( m_socket, SOL_SOCKET, SO_BROADCAST, &k_enabled, sizeof( k_enabled ) )
        ; ok < 0 )
    {
        close( m_socket );
        m_socket = k_invalid_socket;
        throw std::runtime_error( "failed set broadcast option on socket"  );
    }
    
    if( m_bind_address )
    {
        if( const auto ok = bind( m_socket, m_bind_address->as_generic(), sizeof( address_t::native_type ) )
            ; ok < 0 )
        {
            close( m_socket );
            m_socket = k_invalid_socket;
            throw std::runtime_error( "failed to bind socket: " + m_bind_address->get_ip() + ":" + std::to_string( m_bind_address->get_port() ) );
        }
    }
    m_datagrams_stream.run();
}

socket_sender_t::~socket_sender_t() noexcept
{
    m_datagrams_stream.shutdown();
    
    if( k_invalid_socket != m_socket )
    {
        close( m_socket );
    }
}

void socket_sender_t::send( datagram_t && datagram ) noexcept
{
    m_datagrams_stream.push( std::move( datagram ) );
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
        logger_t{} << "sendto failed";
    }
}

socket_receiver_t::socket_receiver_t( address_t address ) noexcept( false )
    : m_bind_address( address )
      , m_socket( k_invalid_socket )
      , m_running( false )
{
    m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if( k_invalid_socket == m_socket )
    {
        throw std::runtime_error( "failed to create socket" );
    }
    
    if( const auto ok = setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO, &k_ten_milliseconds, sizeof( k_ten_milliseconds ) )
        ; ok < 0 )
    {
        close( m_socket );
        m_socket = k_invalid_socket;
        throw std::runtime_error( "failed to set socket receive timeout" );
    }
    
    if( const auto ok = bind( m_socket, m_bind_address.as_generic(), sizeof( address_t::native_type ) )
        ; ok < 0 )
    {
        close( m_socket );
        m_socket = k_invalid_socket;
        throw std::runtime_error( "failed to bind socket: " + m_bind_address.get_ip() + ":" + std::to_string( m_bind_address.get_port() ) );
    }
}

socket_receiver_t::~socket_receiver_t() noexcept
{
    m_running = false;
    const core::thread_join_guard guard{ m_thread };
}

void socket_receiver_t::receive_loop() noexcept
{
    std::array< core::byte_t, datagram_t::k_max_size > buffer{};
    
    try
    {
        while( m_running )
        {
            sockaddr_in sender_address{};
            int sender_address_length = sizeof( sender_address );
            const auto bytes_received = recvfrom(
                m_socket,
                buffer.data(), buffer.size(),
                0,
                reinterpret_cast< sockaddr * >( &sender_address ),
                reinterpret_cast< socklen_t * >( &sender_address_length )
            );
            
            if( bytes_received < 0 )
            {
                continue;
            }
            
            if( static_cast< std::size_t >( bytes_received ) > buffer.size() )
            {
                logger_t{} << "datagram is too big";
                continue;
            }
    
            try
            {
                notify(
                    datagram_t{
                        address_t{ sender_address },
                        core::bytes_t{ buffer.begin(), buffer.begin() + bytes_received }
                    }
                );
            }
            catch ( const std::exception & exception )
            {
                logger_t{} << "notification failed:" << exception.what();
                continue;
            }
        }
    }
    catch ( const std::exception & exception )
    {
        logger_t{} << "unexpected exception occurred:" << exception.what();
        std::terminate();
    }
    catch ( ... )
    {
        logger_t{} << "unexpected error occurred: ";
        std::terminate();
    }
}

void socket_receiver_t::on_run() noexcept
{
    m_thread = std::thread{
        [ this ]() noexcept
        {
            pthread_setname_np( pthread_self(), "socket-receiver" );
            m_running = true;
            receive_loop();
        }
    };
    
    while( !m_running )
    {
        std::this_thread::yield();
    }
}

}