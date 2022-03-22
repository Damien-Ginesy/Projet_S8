#include <net/Message.hpp>
#include <net/Session.hpp>
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
    std::string s;
    std::getline(std::cin, s);
    msg << s;

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
    net::CallbackMap callbacks({{net::PULL_RESP, on_resp}});
    std::cout << "Connected" << '\n';
    net::Session *session;
    try
    {
        session = new net::Session(std::move(sock), callbacks);
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
    std::cout << "\033[A";
    
    return 0;
}
