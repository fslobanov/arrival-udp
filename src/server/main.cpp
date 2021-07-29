#include <event_stream.hpp>
#include <signal_handler.hpp>
#include <iostream>
#include <future>

struct observer_t : public core::event_stream_t< std::string >::subscriber_t
{
    void on_event( std::string && event ) noexcept override
    {
        std::cerr << "event: " << event << std::endl;
    }
};

signed main( int argument_count, char ** arguments ) noexcept
{
    ( void )argument_count;
    ( void  )arguments;
    
    pthread_setname_np( pthread_self(), "arrival-server" );
    
    auto & signals = core::signal_handler_t::get_instance();
    
    observer_t observer;
    core::event_stream_t< std::string > stream{ "event-stream", observer };
    
    signals.add_observer(
        [ &stream ]( core::signal_handler_t::action_e ) noexcept
        {
            std::cerr << "shutting down event stream from signal handler: " << stream.get_name() << std::endl;
            stream.shutdown();
        }
    );
    
    stream.push( "foo" );
    stream.push( "bar" );
    std::thread dummy{
        [ &stream ]() noexcept
        {
            std::this_thread::sleep_for( std::chrono::seconds{ 3 } );
            stream.push( "baz" );
        }
    };
    dummy.detach();
    
    signals.run();
    stream.run();
    
    return EXIT_SUCCESS;
}