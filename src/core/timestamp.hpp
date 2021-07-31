#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <ostream>

namespace core {

class timestamp_t final
{
public:
    using value_type = std::chrono::system_clock::time_point;
    
public:
    static timestamp_t now() noexcept;
    timestamp_t() noexcept;
    timestamp_t( value_type && value ) noexcept;
  
public:
    explicit operator bool() const noexcept;
    bool is_valid() const noexcept;
    
    std::string to_string() const noexcept;
    friend std::ostream & operator <<( std::ostream & os, const timestamp_t & timestamp ) noexcept;

    const value_type & get_value() const;
    const value_type & operator *() const;
    
private:
    std::optional< value_type > m_value;
};

}



