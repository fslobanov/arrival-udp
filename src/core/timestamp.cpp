#include <timestamp.hpp>

#include <cassert>
#include <sstream>
#include <iomanip>
#include <mutex>

namespace core {

timestamp_t::timestamp_t() noexcept
{

}

timestamp_t::timestamp_t( timestamp_t::value_type && value ) noexcept
    : m_value( value )
{

}

timestamp_t::operator bool() const noexcept
{
    return is_valid();
}

bool timestamp_t::is_valid() const noexcept
{
    return m_value.has_value();
}

std::string timestamp_t::to_string() const noexcept
{
    std::ostringstream oss;
    if( m_value )
    {
        const auto milliseconds = std::chrono::duration_cast< std::chrono::milliseconds >( m_value->time_since_epoch() )
            - std::chrono::duration_cast< std::chrono::seconds >( m_value->time_since_epoch() );
        
        std::time_t time_t_value = std::chrono::system_clock::to_time_t( *m_value );
        std::tm tm_value{};
        
        {
            static std::mutex g_mutex;
            std::lock_guard< std::mutex > guard{ g_mutex };
            tm_value = *std::localtime( &time_t_value );
        }
        
        oss << std::put_time( &tm_value, "%Y-%m-%d %H:%M:%S.") << std::to_string( milliseconds.count() );
    }
    else
    {
        oss << "{...}";
    };
    return oss.str();
}

timestamp_t timestamp_t::now() noexcept
{
    return std::chrono::system_clock::now();
}

std::ostream & operator <<( std::ostream & os, const timestamp_t & timestamp ) noexcept
{
    os << timestamp.to_string();
    return os;
}

const timestamp_t::value_type & timestamp_t::get_value() const
{
    assert( is_valid() );
    return *m_value;
}

const timestamp_t::value_type & timestamp_t::operator *() const
{
    return get_value();
}

}


