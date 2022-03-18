#include <net/Message.hpp>
#include <iostream>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    Basalt::NodeId i = {0xAABBCCDD};
    Basalt::net::Message m(Basalt::net::PULL_REQ);
    m << i;
    for(uint8_t b: m) printf("%x", b);
    std::cout << '\n'; 
    Basalt::NodeId i2;
    m >> i2;
    printf("%x\n", i2.id);
    std::cout << m.payloadSize() << '\n';

    return 0;
}
