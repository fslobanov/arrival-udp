#include <task.hpp>

namespace core {

task_t::task_t( timestamp_t timestamp, address_t address ) noexcept
    : m_timestamp( timestamp )
      , m_address( address )
{

}

task_t::task_t( datagram_t && datagram ) noexcept( false )
    : m_address( datagram.get_address() )
{
    const auto & bytes = datagram.get_bytes();
    if( k_msec_count_size != bytes.size() )
    {
        throw std::invalid_argument(
            "broken datagram size, expected "
            + std::to_string( k_msec_count_size )
            + ", got " + std::to_string( bytes.size() )
        );
    }
    
    using namespace std::chrono;
    
    msec_count_t msec_since_epoch{ 0 };
    std::memcpy( &msec_since_epoch, &bytes.front(), k_msec_count_size );
    msec_since_epoch = be64toh( msec_since_epoch );
    
    m_timestamp = timestamp_t{ timestamp_t::value_type{ milliseconds { msec_since_epoch } } };
}

std::ostream & operator <<( std::ostream & os, const task_t & task ) noexcept
{
    os << "[" << task.m_timestamp << ", " << task.m_address << "]";
    return os;
}

datagram_t task_t::to_datagram() const
{
    bytes_t bytes;
    bytes.resize( k_msec_count_size );
    
    if( !m_timestamp )
    {
        throw std::invalid_argument( "timestamp is empty" );
    }
    
    using namespace std::chrono;
    
    const auto since_epoch = time_point_cast< milliseconds >( *m_timestamp ).time_since_epoch();
    auto msec_since_epoch = duration_cast< milliseconds >( since_epoch ).count();
    
    msec_since_epoch = htobe64( msec_since_epoch );
    std::memcpy( &bytes.front(), &msec_since_epoch, k_msec_count_size );
    
    return { m_address, std::move( bytes ) };
}
}