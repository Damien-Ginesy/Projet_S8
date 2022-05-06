#include <basalt.hpp>

#include <bootstrap_server_structs.h>
#include <endian.hpp>

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    constexpr int attackId = 0;
    asio::io_context ctx;
    /* params
    - view size
    - port
    - cycles before reset
    - n nodes per reset
    - cycles per second
    - bs server ip
    - bs server port
    - log server hostname (if applicable)
    - log server port (if not 80)
     */
    const unsigned viewSize = atol(argv[1]);
    const uint16_t port = atoi(argv[2]);
    const unsigned cyclesBeforeReset = atol(argv[3]);
    const unsigned k = atol(argv[4]);
    const unsigned cyclesPerSec = atoi(argv[5]);
    tcp::resolver resolver(ctx);
    asio::error_code ec;
    auto results =  resolver.resolve(argv[6], argv[7], ec);
    if(ec){
        std::cerr << "Couldn't resolve " << argv[6] << ": " << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    tcp::endpoint bootstrapServer;
    for(const auto& r: results){
        if(r.endpoint().address().is_v4()) {
            bootstrapServer = r.endpoint();
            break;
        }
    }
    std::cout << bootstrapServer << '\n';
    tcp::socket sock(ctx);

    sock.connect(bootstrapServer, ec);
    if(ec){
        std::cerr << "Couldn't connect to bootstrap server: " << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    bootstrap_req req {viewSize, port, 0};
    Basalt::write_n(sock, sizeof(req), &req);
    bootstrap_res resp;
    std::cout << resp.malicious_view_size << '\n';
    Basalt::read_n(sock, sizeof(resp), &resp);
    Basalt::NodeId id { make_address_v4(resp.real_ip), port, resp.ip};
    std::cout << id.to_string() << '\n';

    for (int i = 0; i < viewSize; i++)
    {
        node_network_info resp;
        Basalt::read_n(sock, sizeof(resp), &resp);
        std::cout << resp.ip << " " << resp.port << " " << resp.virtual_ip << '\n';
    }
    
    return 0;
}
