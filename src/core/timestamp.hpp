#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace core {

class timestamp_t final
{
public:
    using value_type = std::chrono::system_clock::time_point;
    
public:
    timestamp_t() noexcept;
    timestamp_t( value_type && value ) noexcept;
  
public:
    explicit operator bool() const noexcept;
    bool is_valid() const noexcept;
    
    std::string to_string() const noexcept;

public:
    static timestamp_t now() noexcept;

private:
    std::optional< value_type > m_value;
};

}



