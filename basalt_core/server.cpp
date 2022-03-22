#include <iostream>
#include <net/Session.hpp>


using namespace Basalt;
using namespace asio::ip;

void on_pull(net::Message& msg){
    for(char b: msg) std::cout << b;
    std::cout << '\n';
    msg = net::Message(net::PULL_RESP);
    msg << "Hey!";
}
net::CallbackMap callbacks = {{net::PULL_REQ, on_pull}};

void accept_connections(tcp::acceptor& acceptor){
    auto handle = [&](asio::error_code ec, tcp::socket peer){
        if(!ec){
            new net::Session(std::move(peer), callbacks);
            accept_connections(acceptor);
        }
    };
    acceptor.async_accept(handle);
}

int main(int argc, char const *argv[])
{
    tcp::endpoint ep(tcp::v4(), 1337);
    asio::io_context ctx;

    tcp::acceptor acceptor(ctx, ep);
    asio::error_code ec;
    tcp::endpoint remote;
    std::cout << "Listening on " << ep.address().to_string()
        << ":1337\n";
    accept_connections(acceptor);
    std::thread runner([&ctx](){ ctx.run(); });
    std::getchar();
    std::cout << "\033[AStopping now" << '\n';
    ctx.stop();
    runner.join();
    return 0;
}
