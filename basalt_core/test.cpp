#include <asio.hpp>
#include <thread>
#include <iostream>
int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    asio::io_context ctx;
    tcp::acceptor a(ctx, tcp::endpoint(tcp::v4(), 2000));
    std::thread t([&](){
        a.accept();
    });
    std::thread t2([&]() { ctx.run(); });

    std::getchar();
    ctx.stop();
    a.~basic_socket_acceptor();
    t2.join();
    std::cout << "Stopping" << '\n';
    t.join();
}
