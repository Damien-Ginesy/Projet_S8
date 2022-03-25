#include <net/basalt_net.hpp>
#include <thread>
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    using namespace Basalt;
    net::Message msg;
    NodeId id { make_address_v4(0x7F000001), 1337, 0x0102 };
    msg << id;
    NodeId id2;
    msg >> id2;
    std::cout << id2.to_string() << '\n';
}
