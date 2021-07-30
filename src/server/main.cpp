#include <event_stream.hpp>
#include <signal_handler.hpp>
#include <socket.hpp>
#include <timer.hpp>
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
    
    std::atomic_bool application{ true };
    
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
    
    signals.add_observer(
        [ &application ]( core::signal_handler_t::action_e ) noexcept
        {
           application = false;
        }
    );
    
    net::socket_sender_t sender{net::address_t{ "127.0.0.1", 5555 } };
    
    core::timer_t timer{
        std::chrono::milliseconds{ 1000 },
        [ &sender ]() noexcept
        {
            net::datagram_t::bytes_t bytes{ std::byte{ 1}, std::byte{ 2 } }; //TODO
            net::datagram_t datagram{ net::address_t{ "127.0.0.1", 6666 }, std::move( bytes ) };
            sender.send( std::move( datagram ) );
        }
    };
    signals.add_observer(
        [ &timer ]( core::signal_handler_t::action_e ) noexcept
        {
            std::cerr << "shutting down timer" << std::endl;
            timer.shutdown();
        }
    );
    timer.run();
    
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
    
    while(  application )
    {
        std::this_thread::yield();
    }
    
    return EXIT_SUCCESS;
}