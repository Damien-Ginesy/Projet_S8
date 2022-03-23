#pragma once
#include <net/Session.hpp>

namespace Basalt{
    namespace net
    {
        void net_init(CallbackMap& callbacks, const asio::ip::tcp::endpoint& port);
        void net_finish();

        asio::error_code send_request(const asio::ip::tcp::endpoint& remote, const Message& req);
        asio::ip::tcp::socket& operator<<(asio::ip::tcp::socket&, const Message&);
        asio::ip::tcp::socket& operator>>(asio::ip::tcp::socket&, Message&);

    } // namespace net
    
}