#include <net/basalt_net.hpp>
#include <iostream>
#include <thread>
#include <random>

#define VIEW_SIZE       16

using namespace Basalt;
using namespace std::chrono_literals;
std::random_device rng;
std::uniform_int_distribution dist(0, VIEW_SIZE-1);

void on_pull_req(net::Message& msg){
    std::cout << "Received request" << '\n';
    msg.set_type(net::PULL_RESP);
}
void on_pull_resp(net::Message& resp){
    std::cout << "Received response" << '\n';
    resp.set_type(net::SESSION_END);
}

int main(int argc, char const *argv[])
{
    net::CallbackMap callbacks {
        {net::PULL_REQ, on_pull_req},
        {net::PULL_RESP, on_pull_resp}
    };
    net::net_init(callbacks, 1337);
    
    asio::ip::tcp::endpoint ep(asio::ip::address_v4(0x7f000001), 1337);
    net::Message req(net::PULL_REQ);
    net::send_request(ep, req);

    std::getchar();
    net::net_finish();
    return 0;
}
