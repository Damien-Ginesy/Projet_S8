#include <basalt.hpp>
#include <exchange_protocol.h>
// #include <endian.hpp>
#include <thread>

asio::error_code resolve(asio::ip::tcp::resolver& resolver, std::string hostname, std::string service, asio::ip::tcp::endpoint& out)
{
    asio::error_code ec;
    auto results =  resolver.resolve(hostname, service, ec);
    if(ec) return ec;
    for(const auto& r: results){
        if(r.endpoint().address().is_v4()) {
            out = r.endpoint();
            break;
        }
    }
    return asio::error_code();
}

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    using namespace std::chrono_literals;
    constexpr int attackId = 0;
    if(argc < 8){
        std::cout << "Usage: ./honest_node " 
        "<view_size> <listen_port> <cycles_before_reset> <n_nodes_per_reset> <cycles_per_second> <bootstrap_hostname> "
        "<bootstrap_port> [log server hostname] [log server port (defaults to 80]" << '\n';
        return 1;
    }
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
    tcp::endpoint bootstrapServer;
    asio::error_code ec = resolve(resolver, argv[6], argv[7], bootstrapServer);
    tcp::socket bsServerSock(ctx);

    bsServerSock.connect(bootstrapServer, ec);
    if(ec){
        std::cerr << "Couldn't connect to bootstrap server: " << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    bootstrap_req req {viewSize, port, 0};
    Basalt::write_n(bsServerSock, sizeof(req), &req);
    bootstrap_res resp;
    Basalt::read_n(bsServerSock, sizeof(resp), &resp);
    Basalt::NodeId id { make_address_v4(resp.real_ip), port, resp.ip};
    std::cout << id.to_string() << '\n';
    std::cout << sizeof(node_network_info) << '\n';
    Basalt::Array<Basalt::NodeId> bootstrap(viewSize);
    std::cout << "Bootstrap:\n==================\n";
    for (auto& p: bootstrap)
    {
        node_network_info info;
        Basalt::read_n(bsServerSock, sizeof(info), &info);
        p = Basalt::NodeId {make_address_v4(info.ip), info.port, info.virtual_ip};
        std::cout << p.to_string() << '\n';
    }
    

    // init here
    const std::chrono::milliseconds mainDelay(1000 / cyclesPerSec);
    const std::chrono::milliseconds resetDelay(cyclesBeforeReset * mainDelay);
    const char *logServerHostname = NULL;
    uint16_t logServerPort = 80;
    switch (argc)
    {
    case 10: logServerPort = (uint16_t)atoi(argv[9]);
    case 9: logServerHostname = argv[8];
    Basalt::basalt_set_logger(k, logServerHostname, logServerPort, "/infoNoeud");
    }
    
    Basalt::basalt_init(id, bootstrap, k, mainDelay, resetDelay);

    while(1) 
        std::this_thread::sleep_for(10s);
    Basalt::basalt_stop();

    return 0;
}
