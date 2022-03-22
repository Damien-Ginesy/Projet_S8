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
    net::Message msg(net::PULL_REQ);
    msg << "Hello";

    net::CallbackMap callbacks {{net::PULL_RESP, on_resp}};
    net::net_init(callbacks, 2112);
    
    using namespace asio::ip;
    net::send_request(tcp::endpoint(address_v4(0x7f000001), 1337), msg);
    
    std::getchar();
    net::net_finish();
    std::cout << "\033[A";
    
    return 0;
}
