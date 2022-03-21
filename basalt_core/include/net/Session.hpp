#pragma once
#include <asio.hpp>
#include <iostream>
#include <net/Message.hpp>

namespace Basalt
{
    namespace net
    {   
        struct Session final
        {
        private:
            asio::ip::tcp::socket _peer;
            Message _msg;
            bool (*_handlers[N_MSG_TYPES])(Message&) = {nullptr};
            void close(){
                _peer.close();
                delete this;
            }
            asio::error_code read_n(size_t n, uint8_t* dest){
                uint32_t nRead = 0;
                asio::error_code ec;
                while (nRead < n)
                {
                    _peer.wait(_peer.wait_read);
                    nRead += _peer.read_some(
                        asio::buffer(dest+nRead, n-nRead), ec);
                    if(ec) break;
                }
                return ec;
            }
            void on_message(){
                // call the callback (if defined)
                MessageType t = _msg.get_type();
                bool respond = _handlers[t] &&
                    _handlers[t](_msg);
                // depending on the message type, send back a response
                if(respond)
                    this->_peer << this->_msg;
                close();
            }
            void async_wait(){
                auto waitHandler = [this](asio::error_code ec){
                    if(!ec){
                        this->_msg << this->_peer;
                        this->on_message();
                    }
                    else
                    throw ec.message();
                };
                _peer.async_wait(_peer.wait_read, waitHandler);
            }
        public:
            Session(asio::ip::tcp::socket&& peer, bool (**f)(Message&)): _peer(std::move(peer))
            {
                for(int i=0; i<N_MSG_TYPES; ++i)
                    _handlers[i] = f[i];
                async_wait(); // async wait for data to arrive on the socket
            }
            void send_message(const Message& msg){
                this->_peer << msg;
            }
            // void* operator new(size_t) { return std::malloc(sizeof(Session)); }
            ~Session() { }
        };
    } // namespace net
    
} // namespace Basalt

