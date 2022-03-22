#include <net/Session.hpp>

namespace Basalt{
    namespace net
    {
        void Session::close(){
            _peer.close();
            delete this;
        }
        asio::error_code Session::read_n(size_t n, uint8_t* dest){
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
        void Session::on_message(){
            MessageType t = _msg.get_type();
            if(t==SESSION_END){ close(); return; }
            auto handler = _callbacks.find(t);
            if(handler != _callbacks.end())
                handler->second(_msg);
            if(_msg.get_type() & 0xf0) /* if _msg is a reponse message */
                _peer << _msg;
            async_wait();
        }
        void Session::async_wait(){
            auto waitHandler = [this](asio::error_code ec){
                if(!ec){
                    try
                    {
                        this->_msg << this->_peer;
                    }
                    catch(const asio::error_code& e)
                    {
                        if(e.value() == 2){ // eof error: socket closed on the other end
                            close(); return;
                        }
                        else throw e;
                    }
                    
                    this->on_message();
                }
                else
                throw ec;
            };
            _peer.async_wait(_peer.wait_read, waitHandler);
        }
        Session::Session(asio::ip::tcp::socket&& peer, CallbackMap& callbacks): 
            _peer(std::move(peer)), _callbacks(callbacks)
        {
            async_wait(); // async wait for data to arrive on the socket
        }
        void Session::send_message(const Message& msg){
            this->_peer << msg;
        }
        Session::~Session() { }
    } // namespace net
    
}

