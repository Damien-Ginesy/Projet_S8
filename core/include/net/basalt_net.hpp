#pragma once
#include <net/Session.hpp>
#include <NodeId.hpp>

namespace Basalt{
    namespace net
    {
        /* Inits the network engine of the Basalt RPS service */
        void net_init(CallbackMap& callbacks, const asio::ip::tcp::endpoint& port);
        /* Stops all network operations, clears the state */
        void net_finish();

        /* Attempts to send create a session with the remote endpoint, 
        and to send a message */
        asio::error_code send_request(const asio::ip::tcp::endpoint& remote, const Message& req);
        /* Attemps to dump the message in the socket */
        asio::ip::tcp::socket& operator<<(asio::ip::tcp::socket&, const Message&);
        /* Attemps to read a message from the socket */
        asio::ip::tcp::socket& operator>>(asio::ip::tcp::socket&, Message&);
        /* dumps a NodeId to a Message object */
        net::Message& operator<<(net::Message& m, const NodeId& id);
        net::Message& operator>>(net::Message& m, NodeId& id);
        net::Message& operator<<(net::Message& m, const asio::ip::address_v4& addr);
        Message& operator>>(net::Message& m, asio::ip::address_v4& addr);
        Message& operator<<(net::Message& m, const asio::ip::address_v4& addr);


    } // namespace net
    
}