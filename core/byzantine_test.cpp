#define IS_BYZANTINE    1
#include <basalt.hpp>
#include <iostream>
#include <net/basalt_net.hpp>


void on_pull_req(asio::ip::tcp::endpoint sender, Basalt::net::Message& req){
    std::cout << "Received pull from " << sender << '\n';
    req.set_type(Basalt::net::SESSION_END);
}

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    address_v4 localhost(0x7F000001);
    Array<NodeId> bs(2);
    bs[0] = NodeId {localhost, 3000, 0};
    bs[1] = NodeId {localhost, 3001, 1};

    Array<NodeId> friends(4);
    bs[0] = NodeId {localhost, 3002, 0x10000};
    bs[1] = NodeId {localhost, 3003, 0x10001};
    bs[2] = NodeId {localhost, 3004, 0x10002};
    bs[3] = NodeId {localhost, 3005, 0x10003};

    Node b(NodeId {address_v4(0x7F000001), 3006, 0x10004}, bs, friends);

    net::CallbackMap cbk {
        {net::PULL_REQ, on_pull_req}
    };
    tcp::endpoint ep(localhost, 3006);
    // net::net_init(cbk, ep);


    // std::this_thread::sleep_for(std::chrono::seconds(1));

    // net::net_finish();

    return 0;
}
