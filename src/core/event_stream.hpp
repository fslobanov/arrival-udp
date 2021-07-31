#pragma once

#include <cassert>
#include <queue>
#include <variant>
#include <mutex>
#include <atomic>
#include <thread>
#include <tuple>
#include <condition_variable>
#include <iostream>

#include <core.hpp>
#include <logger.hpp>

namespace core {

template< typename Event >
class event_queue_t final
{
public:
    enum class state_e : uint32_t
    {
        standby, running, shutdown
    };
    
    using event_type = Event;
    using entry_type = std::variant< state_e, event_type >;
    using lock_type = std::unique_lock< std::mutex >;
    
    static_assert( !std::is_convertible< state_e, event_type >::value
                   && !std::is_convertible< event_type, state_e >::value,
                   "Events should not be convertible from each other" );
    
public:
    explicit event_queue_t( std::string name ) noexcept
        : m_name( std::move( name ) )
    {
    
    }
    
    std::string_view get_name() const noexcept
    {
        return m_name;
    }
    
    state_e run() noexcept
    {
        lock_type lock{ m_mutex };
        
        assert( m_state == state_e::standby && "Only standby satisfies" );
        m_state = state_e::running;
        m_queue.push( m_state );
        
        lock.unlock();
        
        notifier.notify_one();
        return m_state;
    }
    
    state_e shutdown() noexcept
    {
        lock_type lock{ m_mutex };
        logger_t{} << m_name << "- shutting down event queue";
        
        auto current_state = std::exchange( m_state, state_e::shutdown );
        switch ( current_state )
        {
            default:
                assert( false );
    
            case state_e::shutdown:
                return m_state;
    
            case state_e::standby:
                break;
    
            case state_e::running:
                break;
        }
        
        current_state = m_state;
        decltype( m_queue ) empty_queue{};
        m_queue.swap( empty_queue );
        m_queue.push( m_state );
        
        lock.unlock();
        notifier.notify_one();
        
        return current_state;
    }
    
    void push( Event && entry ) noexcept
    {
        lock_type lock{ m_mutex };
        
        if( state_e::running != m_state )
        {
            return;
        }
        m_queue.push( std::move( entry ) );
        
        lock.unlock();
        
        notifier.notify_one();
    }
    
    entry_type pull() noexcept
    {
        lock_type lock{ m_mutex };
        notifier.wait(
            lock,
            [ this ]() noexcept
            {
                return !m_queue.empty();
            }
        );
        
        auto value = std::move( m_queue.front() );
        m_queue.pop();
        return value;
    }

private:
    const std::string m_name;
    state_e m_state = state_e::standby;
    std::queue< entry_type > m_queue;
    std::mutex m_mutex;
    std::condition_variable notifier;
};

template< typename Event >
class event_stream_t final
{
public:
    using event_type = Event;
    using queue_type = event_queue_t< event_type >;
    using state_type = typename queue_type::state_e;
    
    class subscriber_t
    {
    public:
        virtual ~subscriber_t() noexcept = default;
        virtual void on_event( event_type && event ) noexcept = 0;
    };

public:
    explicit event_stream_t( std::string name, subscriber_t & subscriber ) noexcept
        : m_queue( std::move( name ) )
          , m_subscriber( subscriber )
    {
    
    }
    
    ~event_stream_t() noexcept
    {
        shutdown();
    }

public:
    void run() noexcept
    {
        logger_t{} << get_name() << "- running event stream";
        m_queue.run();
        std::atomic_bool running{ false };
        m_thread = std::thread{
            [ this, &running ]() noexcept
            {
                running = true;
                logger_t{} << get_name() << "- running event stream loop";
                pthread_setname_np( pthread_self(), get_name().data() );
                event_loop();
                logger_t{} << get_name() << "- event stream loop finished";
            }
        };
        
        //m_thread.detach();
        while( !running )
        {
            std::this_thread::yield();
        }
    }
    
    void shutdown() noexcept
    {
        logger_t{} << get_name() << "- shutting down event stream";
        m_queue.shutdown();
        const thread_join_guard guard{ m_thread };
    }
    
    void push( event_type && event ) noexcept
    {
        m_queue.push( std::move( event ) );
    }
    
    std::string_view get_name() const noexcept
    {
        return m_queue.get_name();
    }

private:
    struct entry_visitor_t final
        {
        event_stream_t & self;
        state_type operator ()( event_type && event ) noexcept
        {
            self.m_subscriber.on_event( std::move( event ) );
            return queue_type::state_e::running;
        }
    
        state_type operator ()( state_type && state ) noexcept
        {
            return state;
        }
        };
    
private:
    event_queue_t< event_type > m_queue;
    std::thread m_thread;
    subscriber_t & m_subscriber;
    
private:
    void event_loop() noexcept
    {
        try
        {
            while( true )
            {
                // FIXME error: incomplete type ‘std::tuple_size<std::variant<core::event_queue_t<int>::state_e, int> >’ used in nested name specifier
                // https://stackoverflow.com/questions/63616709/incomplete-type-stdvariant-used-in-nested-name-specifier
                // entry_visitor_t visitor{ *this };
                // if( state_type::shutdown == std::apply( visitor, m_queue.pull() ) )
                // {
                //     return;
                // }
                
                auto && entry = m_queue.pull();
                if( auto state = std::get_if< state_type >( &entry ) )
                {
                    if( state_type::shutdown == *state )
                    {
                        logger_t{} << get_name() << "- event stream got shutdown from queue";
                        return;
                    }
                }
                else if( auto event = std::get_if< event_type >( &entry ) )
                {
                    m_subscriber.on_event( std::move( *event ) );
                }
                else
                {
                    logger_t{} << "index: " << entry.index()
                              << ", valueless: " << std::boolalpha << entry.valueless_by_exception();
                    
                    assert( false );
                }
            }
        }
        catch ( const std::exception & exception )
        {
            logger_t{} << "event stream internal exception occurred: " << get_name() << "- "<< exception.what();
            m_queue.shutdown();
        }
        catch( ... )
        {
            logger_t{} << "event stream internal unknown error occurred: " << get_name();
            m_queue.shutdown();
        }
    }
};

}



