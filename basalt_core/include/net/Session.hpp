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
            Message _msg;
            CallbackMap _callbacks;
            void close();
            void on_message();
            void async_wait();
        public:
            asio::ip::tcp::socket _peer;
            // static void* operator new(size_t s) { 
            //     std::cout << "Alloc" << '\n';
            //     return std::malloc(s); }
            // static void operator delete(void *p){
            //     std::cout << "Dealloc" << '\n';
            //     std::free(p);
            // }
            Session(asio::ip::tcp::socket&& peer, CallbackMap& callbacks);
            asio::error_code send_message(const Message& msg);
            ~Session();
        };
    } // namespace net
    
} // namespace Basalt

