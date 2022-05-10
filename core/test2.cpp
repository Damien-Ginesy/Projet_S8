#include <basalt.hpp>

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace std::chrono_literals;

    uint16_t port = atoi(argv[1]);

    NodeId id {asio::ip::make_address_v4((uint32_t)0), port, (uint32_t)(port % 10)};
    port = (uint16_t)atoi(argv[2]);
    Array<NodeId> bs(1);
    bs[0] = NodeId { asio::ip::make_address_v4(0x7F000001), port, (uint32_t)(port % 10) };

    basalt_init(id, bs, 1, 1s, 3s);
    std::this_thread::sleep_for(10s);
    basalt_stop();
    
    return 0;
}
