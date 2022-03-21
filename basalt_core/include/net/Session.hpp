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
                for(uint8_t x: _msg)
                    std::cout << x;
                std::cout << '\n';
                // call the callback (if defined)
                MessageType t = _msg.get_type();
                bool respond = _handlers[t] &&
                    _handlers[t](_msg);
                // depending on the message type, send back a response
                if(respond)
                    this->_peer << this->_msg;
                close();
            }
        public:
            Session(asio::ip::tcp::socket&& peer, bool (**f)(Message&)): _peer(std::move(peer))
            {
                for(int i=0; i<N_MSG_TYPES; ++i)
                    _handlers[i] = f[i];
                auto waitHandler = [this](asio::error_code ec){
                    if(ec) {
                        std::cerr << "Wait: " << ec.message() << std::endl;
                        return;
                    }
                    try
                    {
                        this->_msg << this->_peer;
                        this->on_message();
                    }
                    catch(const std::string& e)
                    {
                        std::cerr << "Error reading message: " << e << '\n';
                    }
                    
                };
                _peer.async_wait(_peer.wait_read, waitHandler);
            }
            // void* operator new(size_t) { return std::malloc(sizeof(Session)); }
            ~Session() { }
        };
    } // namespace net
    
} // namespace Basalt

