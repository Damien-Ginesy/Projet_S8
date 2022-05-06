#include <basalt.hpp>
#include <exchange_protocol.h>
#include <endian.hpp>
#include <thread>

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    using namespace std::chrono_literals;
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

    Basalt::Array<Basalt::NodeId> bootstrap(viewSize);
    std::cout << "Bootstrap:\n=========" << '\n';
    for (auto& p: bootstrap)
    {
        node_network_info info;
        Basalt::read_n(sock, sizeof(resp), &info);
        p = Basalt::NodeId {make_address_v4(info.ip), info.port, info.virtual_ip};
        std::cout << p.to_string() << '\n';
    }
    

    // init here


    // while(true) std::this_thread::sleep_for(10s);
    return 0;
}
