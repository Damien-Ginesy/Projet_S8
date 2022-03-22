#include <net/Session.hpp>

namespace Basalt{
    namespace net
    {
        void Session::close(){
            _peer.close();
            delete this;
        }
        void Session::on_message(){
            MessageType t = _msg.get_type();
            if(t==SESSION_END){ close(); return; }
            auto handler = _callbacks.find(t);
            if(handler != _callbacks.end())
                handler->second(_msg);
            asio::error_code ec;
            if(_msg.get_type() & 0xf0) /* if _msg is a reponse message */
                if(ec = _msg.writeTo(_peer)) throw ec;
            async_wait();
        }
        void Session::async_wait(){
            auto waitHandler = [this](asio::error_code ec){
                if(!ec){
                    try
                    {
                        this->_msg.readFrom(this->_peer);
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
        asio::error_code Session::send_message(const Message& msg){
            return _msg.writeTo(_peer);
        }
        Session::~Session() { }
    } // namespace net
    
}

