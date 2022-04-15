#include <NodeId.hpp>
#include <array>
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    NodeId id = {asio::ip::address_v4(0x7f000001), 2048, 0x00010002};
    NodeId::bytes_t d1 = {};
    id.to_bytes(d1);
    for(unsigned char b: d1)
        printf("%02x ", b);
    printf("\n");
    NodeId::bytes_t d2 = {
        127, 0, 0, 1,
        0, 8,
        2, 0, 1, 0
    };
    NodeId id2 = NodeId::from_bytes(d2);
    std::cout << id2.to_string() << '\n';
    return 0;
}
