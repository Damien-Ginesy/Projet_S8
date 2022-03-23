#include <net/basalt_net.hpp>
#include <iostream>

void on_resp(Basalt::net::Message& resp){
    for(char b: resp) std::cout << b;
    std::cout << '\n';
    resp = Basalt::net::Message(Basalt::net::SESSION_END);
}

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    net::Message msg(net::PULL_REQ);
    msg << "Hello";

    net::CallbackMap callbacks {{net::PULL_RESP, on_resp}};
    tcp::endpoint ep(address_v4(0x7f000001), 1337);
    net::net_init(callbacks, tcp::endpoint(address_v4(0x7F000001), 16000));
    
    net::send_request(ep, msg);
    
    std::getchar();
    net::net_finish();
    std::cout << "\033[A";
    
    return 0;
}
