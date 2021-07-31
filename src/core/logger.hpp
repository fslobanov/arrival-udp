#pragma once

#include <timestamp.hpp>
#include <cstdint>
#include <ostream>
#include <vector>
#include <sstream>
#include <iostream>

namespace core {

class logger_t
{
public:
    logger_t() noexcept;
    ~logger_t() noexcept;
    
    template< typename T >
    logger_t & operator << ( const T & arg ) noexcept
    {
        m_content << arg << ' ';
        return *this;
    }

private:
    std::ostringstream m_content;
};

};



