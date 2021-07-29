#include <timestamp.hpp>

#include <cassert>

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
    assert( false && "impl" );
    return {};
}

timestamp_t timestamp_t::now() noexcept
{
    return std::chrono::system_clock::now();
}

}


