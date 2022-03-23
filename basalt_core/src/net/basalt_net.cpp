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


        static void accept_connections(tcp::acceptor& ac){
            auto handler = [&](asio::error_code ec, tcp::socket peer){
                manager.open_new(std::move(peer), handlers);
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
            auto s = manager.open_new(std::move(sock), handlers);
            return msg.writeTo(s->_peer);
        }
    } // namespace net
    
} // namespace Basalt
