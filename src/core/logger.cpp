#include <logger.hpp>
#include <mutex>

namespace core {

logger_t::logger_t() noexcept
{
    m_content << timestamp_t::now().to_string() << ": ";
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



