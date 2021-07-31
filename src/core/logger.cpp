#include <logger.hpp>
#include <mutex>

namespace core {

logger_t::logger_t() noexcept
{
    const auto now = timestamp_t::now();
    m_content << now.to_string();
    m_content << ( now.millis_since_second() < 100 ? " :" : ":" );
}

logger_t::~logger_t() noexcept
{
    static std::mutex g_mutex;
    
    auto string = m_content.str();
    if( !string.empty() && string.back() == ' ' )
    {
        string.pop_back();
    }
    
    std::lock_guard< std::mutex > guard{ g_mutex };
    std::cerr << string << std::endl;
}

}



