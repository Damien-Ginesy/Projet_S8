#include <iostream>
#include <net/basalt_net.hpp>
#include <list>
#include <mutex>
#include <net/Session.hpp>

namespace Basalt
{
    namespace net
    {
        using namespace asio::ip;

        static CallbackMap handlers; // callback functions to call on incoming messages        
        static std::atomic_bool keepGoing = true;
        static tcp::endpoint listeningEndpoint;
        static std::mutex mutex;
        static asio::io_context context;

        static std::thread contextRunner;
        static tcp::acceptor* acceptor;
        static SessionManager manager;

        tcp::socket& operator<<(tcp::socket& sock, const Message& msg){
            asio::error_code ec = msg.writeTo(sock);
            if(ec) throw ec;
            return sock;
        }
        asio::ip::tcp::socket& operator>>(asio::ip::tcp::socket& sock, Message& msg){
            asio::error_code ec = msg.readFrom(sock);
            if(ec) throw ec;
            return sock;
        }
        Message& operator<<(net::Message& m, const asio::ip::address_v4& addr){
            address_v4::bytes_type bytes = addr.to_bytes();
            m << bytes[0] << bytes[1] << bytes[2] << bytes[3];
            return m;
        }
        Message& operator>>(net::Message& m, asio::ip::address_v4& addr){
            address_v4::bytes_type bytes;
            m >> bytes[3] >> bytes[2] >> bytes[1] >> bytes[0];
            addr = make_address_v4(bytes);
            return m;
        }

        Message& operator<<(net::Message& m, const NodeId& id){
            m << id._addr << id._port << id.id;
            return m;
        }    
        net::Message& operator>>(net::Message& m, NodeId& id){
            m >> id.id >> id._port >> id._addr;
            return m;
        }

        static void accept_connections(tcp::acceptor& ac){
            auto handler = [&](asio::error_code ec, tcp::socket peer){
                mutex.lock();
                manager.open_new(std::move(peer), handlers);
                mutex.unlock();
                if(keepGoing) accept_connections(ac);
            };
            ac.async_accept(handler);
        }

        void net_init(CallbackMap& callbacks, const tcp::endpoint& ep){
            listeningEndpoint = ep;
            handlers = callbacks;
            acceptor = new tcp::acceptor(context, ep);

            accept_connections(*acceptor);
            contextRunner = std::thread([]() { context.run(); });
        }
        void net_finish(){
            keepGoing = false;
            context.stop();
            // acceptor->
            contextRunner.join();
            delete acceptor;
        }
        asio::error_code send_request(const tcp::endpoint& remote, const Message& msg){
            tcp::socket sock(context);
            asio::error_code ec;
            sock.connect(remote, ec);
            if(ec) return ec;
            mutex.lock();
            auto s = manager.open_new(std::move(sock), handlers);
            mutex.unlock();
            return msg.writeTo(s->_peer);
        }
    } // namespace net
    
} // namespace Basalt
