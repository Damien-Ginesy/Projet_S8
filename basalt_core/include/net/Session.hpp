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
        /* A type that binds message types to callback functions */
        using CallbackMap = std::unordered_map<MessageType, void(*)(Message&)>;

        /* Represents a communication session between two peers */
        struct Session final
        {
        private:
            /* Stores the contents of the exchanged messages */
            Message _msg;
            /* The callback functions to call when receiving a message */
            CallbackMap _callbacks;
            /* Asynchronous callback for message reception */
            void on_message();
            /* Asynchronously awaits for data to arrive */
            void async_wait();
        public:
            /* The socket to the remote peer */
            asio::ip::tcp::socket _peer;
            /* Opens the Session with an open socket, and a set of callback functions */
            Session(asio::ip::tcp::socket&& peer, CallbackMap& callbacks);
            /* Attempts to send a message to the remote peer */
            asio::error_code send_message(const Message& msg);
            /* Is the Session still active */
            bool isOpen() const { return _peer.is_open(); }
            ~Session();
        };
        /* Manages sessions within the execution context */
        struct SessionManager{
        private:
            /* List of current sessions */
            std::list<Session> _sessions;
            /* Removes all closed sessions */
            void clean();
        public:
            using iterator = std::list<Session>::iterator;
            /* Open a new session, and returns an iterator to the newly create object */
            iterator open_new(asio::ip::tcp::socket&&, CallbackMap& callbacks);
        };
    } // namespace net
    
} // namespace Basalt

