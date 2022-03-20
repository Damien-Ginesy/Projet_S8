#pragma once
#include <asio.hpp>
#include <net/Message.hpp>
#include <NodeId.hpp>

namespace Basalt{
    namespace net
    {
        void net_init(void (*callbacks[])(Message&), uint16_t port);
        void net_finish();

        void send_request(NodeId& dest, const Message& req);
    } // namespace net
    
}