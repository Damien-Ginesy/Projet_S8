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
                // write the received message back into the socket
                this->_peer << this->_msg;
                close();
            }
        public:
            Session(asio::ip::tcp::socket&& peer): _peer(std::move(peer))
            {
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

