#include <net/Message.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
    Basalt::net::Message m(Basalt::net::PULL_REQ);

    m << (uint8_t)0 << (uint8_t)1;
    uint8_t x, y;
    m >> y >> x;
    std::cout << (int)x << ' ' << (int)y << '\n';
    return 0;
}
