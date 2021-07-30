#include <event_stream.hpp>
#include <signal_handler.hpp>
#include <socket.hpp>
#include <application.hpp>
#include <timer.hpp>
#include <iostream>
#include <future>

struct receiver_observer_t : public net::socket_receiver_t::subscriber_t
{
    void on_receive( net::datagram_t && datagram ) noexcept override
    {
        std::cerr << datagram << std::endl;
    }
};

signed main( int argument_count, char ** arguments ) noexcept
{
    core::application_t app{ argument_count, arguments };
    pthread_setname_np( pthread_self(), "arrival-server" );
    
    const net::address_t broadcast{ "127.255.255.255", 6666 };
    
    receiver_observer_t observer{};
    net::socket_receiver_t receiver{ broadcast, observer };
    
    net::socket_sender_t sender{ net::address_t{ "127.255.255.255", 4444 } };
    
    core::timer_t timer{
        std::chrono::milliseconds{ 1 },
        [ &sender, &broadcast ]() noexcept
        {
            core::bytes_t bytes{ std::byte{ 1 }, std::byte{ 2 } }; //TODO
            net::datagram_t datagram{ broadcast, std::move( bytes ) };
            sender.send( std::move( datagram ) );
        }
    };
    
    core::signal_handler_t::get_instance().add_observer(
        [ &timer ]( core::signal_handler_t::action_e ) noexcept
        {
            std::cerr << "shutting down timer" << std::endl;
            timer.shutdown();
        }
    );
   
    timer.run();
    app.run();
    
    return EXIT_SUCCESS;
}