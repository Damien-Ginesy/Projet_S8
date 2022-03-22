#pragma once
#include <asio.hpp>
#include <iostream>
#include <net/Message.hpp>
#include <map>

namespace Basalt
{
    namespace net
    {   
        using CallbackMap = std::unordered_map<MessageType, void(*)(Message&)>;

        struct Session final
        {
        private:
            asio::ip::tcp::socket _peer;
            Message _msg;
            CallbackMap _callbacks;
            void close();
            asio::error_code read_n(size_t n, uint8_t* dest);
            void on_message();
            void async_wait();
        public:
            Session(asio::ip::tcp::socket&& peer, CallbackMap& callbacks);
            void send_message(const Message& msg);
            ~Session();
        };
    } // namespace net
    
} // namespace Basalt

