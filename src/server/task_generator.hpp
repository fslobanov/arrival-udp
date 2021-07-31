#pragma once

#include <functional>
#include <timer.hpp>
#include <task.hpp>
#include <address.hpp>

namespace server {

class task_generator_t final
{
public:
    using callback_t = std::function< void( core::task_t && ) >;
    
public:
    task_generator_t( core::timer_t::duration_t duration,
                      core::address_t destination,
                      callback_t && callback ) noexcept;
    
public:
    void run() noexcept;

private:
    callback_t m_callback;
    core::address_t m_destination;
    core::timer_t m_timer;
    //TODO use event stream

private:
    void on_timer() noexcept;
};

}




