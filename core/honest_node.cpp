#include <basalt.hpp>
#include <semaphore>
#include <bootstrap_server_structs.h>

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    constexpr int attackId = 0;
    asio::io_context ctx;
    /* params
    - view size
    - cycles before reset
    - n nodes per reset
    - cycles per second
    - bs server ip
    - bs server port
    - log server hostname (if applicable)
    - log server port (if not 80)
     */
    const unsigned viewSize = atol(argv[1]);
    const unsigned cyclesBeforeReset = atol(argv[2]);
    const unsigned k = atol(argv[3]);
    const unsigned cyclesPerSec = atoi(argv[4]);
    tcp::resolver resolver(ctx);
    asio::error_code ec;
    auto results =  resolver.resolve(argv[5], argv[6], ec);
    if(ec){
        std::cerr << "Couldn't resolve " << argv[5] << ": " << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    tcp::endpoint bootstrapServer = *results.begin();
    tcp::socket sock(ctx);

    sock.connect(bootstrapServer, ec);
    if(ec){
        std::cerr << "Couldn't connect to bootstrap server: " << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    

    
    return 0;
}
