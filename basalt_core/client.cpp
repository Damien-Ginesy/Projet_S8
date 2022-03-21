#include <net/Message.hpp>
#include <net/Session.hpp>
#include <iostream>

bool on_resp(Basalt::net::Message& resp){
    for(char b: resp) std::cout << b;
    std::cout << '\n';
    return false;
}

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    net::Message msg(net::PULL_REQ);
    msg << "Hello there";

    asio::io_context ctx;
    using namespace asio::ip;
    tcp::endpoint ep(address_v4(0x7f000001), 1337);
    tcp::socket sock(ctx);
    asio::error_code ec;
    sock.connect(ep, ec);
    if(ec){
        std::cerr << ec.message() << std::endl;
        return EXIT_FAILURE;
    }
    bool (*f[N_MSG_TYPES])(net::Message&) = {nullptr};
    f[net::PULL_RESP] = on_resp;
    std::cout << "Connected" << '\n';
    net::Session *session;
    try
    {
        session = new net::Session(std::move(sock), f); 
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    std::thread t([&ctx](){ ctx.run(); });
    try
    {
        session->send_message(msg);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    std::getchar();
    ctx.stop();
    t.join();
    
    return 0;
}
