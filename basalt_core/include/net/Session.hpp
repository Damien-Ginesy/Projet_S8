#pragma once
#include <asio.hpp>
#include <iostream>
#include <net/Message.hpp>
#include <map>
#include <list>

namespace Basalt
{
    namespace net
    {   
        using CallbackMap = std::unordered_map<MessageType, void(*)(Message&)>;

        struct Session final
        {
        private:
            Message _msg;
            CallbackMap _callbacks;
            bool _isOpen = true;
            void close();
            void on_message();
            void async_wait();
        public:
            asio::ip::tcp::socket _peer;
            Session(asio::ip::tcp::socket&& peer, CallbackMap& callbacks);
            asio::error_code send_message(const Message& msg);
            bool isOpen() const { return _isOpen; }
            ~Session();
        };
        struct SessionManager{
        private:
            std::list<Session> _sessions;
            void clean();
        public:
            std::list<Session>::iterator open_new(asio::ip::tcp::socket&&, CallbackMap& callbacks);
        };
    } // namespace net
    
} // namespace Basalt

