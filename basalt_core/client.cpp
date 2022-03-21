#include <net/Message.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    net::Message msg(net::PULL_REQ);
    msg << "Hello there";

    asio::io_context ctx;
    using namespace asio::ip;
    tcp::endpoint ep(address_v4(0x7f000001), 1337);
    tcp::socket sock(ctx);
    asio::error_code ec;
    sock.connect(ep, ec);
    if(ec){
        std::cerr << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Connected" << '\n';
    sock << msg;
    net::Message resp;
    resp << sock;
    std::cout << (char*)resp.data() << '\n';
    return 0;
}
