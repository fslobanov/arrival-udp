#include <application.hpp>
#include <gateway.hpp>
#include <task_processor.hpp>

signed main( int argument_count, char ** arguments ) noexcept
{
    core::application_t app{ "arrival-endpoint", argument_count, arguments };
    
    endpoint::task_processor task_processor;
    
    endpoint::gateway_t gateway{
        [ &task_processor ]( core::task_t && task ) noexcept
        {
            task_processor( std::move( task ) );
        },
        std::make_unique< core::socket_receiver_t >( core::address_t{ "127.255.255.255", 6666 } )
    };
    
    gateway.run();
    app.await();
    
    return EXIT_SUCCESS;
}