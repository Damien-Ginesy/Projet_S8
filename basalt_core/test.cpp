#include <asio.hpp>
#include <thread>
#include <iostream>

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    std::cout << address_v4(1) << '\n';
}
