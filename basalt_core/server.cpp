#include <iostream>
#include <net/basalt_net.hpp>


using namespace Basalt;

void on_pull(net::Message& msg){
    for(char b: msg) std::cout << b;
    std::cout << '\n';
    msg = net::Message(net::PULL_RESP);
    msg << "Hey!";
}
net::CallbackMap callbacks = {{net::PULL_REQ, on_pull}};


int main(int argc, char const *argv[])
{
    net::net_init(callbacks, 1337);
    std::getchar();
    net::net_finish();
    return 0;
}
