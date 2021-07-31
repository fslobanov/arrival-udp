#include <application.hpp>
#include <task_generator.hpp>
#include <gateway.hpp>

signed main( int argument_count, char ** arguments ) noexcept
try
{
    core::application_t app{ "arrival-server", argument_count, arguments };
    server::gateway_t gateway{
        std::make_shared< core::socket_sender_t >( core::address_t{ "127.255.255.255", 4444 } )
    };
    
    server::task_generator_t generator{
        std::chrono::milliseconds{ 100 },
        core::address_t{ "127.255.255.255", 6666 },
        [ &gateway ]( core::task_t && task ) noexcept
        {
            gateway.post( std::move( task ) );
        }
    };
    
    generator.run();
    app.await();
    
    return EXIT_SUCCESS;
    
} catch ( const std::exception & exception )
{
    core::logger_t{} << "exception occurred:" << exception.what();
    return EXIT_FAILURE;
}
