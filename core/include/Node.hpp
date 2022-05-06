#pragma once
#include "NodeId.hpp"
#include "Array.hpp"
#include "Hash.hpp"
#include <net/Message.hpp>
#include <rng.hpp>

#ifndef IS_BYZANTINE
#define IS_BYZANTINE    0 // redefine this macro at compile time
#endif

namespace Basalt
{
    struct Hash_t{
        uint64_t _x[4];
        bool operator<(const Hash_t& other) const {
            int i;
            for(i=0; (i<4) && (_x[i]==other._x[i]); ++i);
            return (i<4) && (_x[i] < other._x[i]);
        }
        bool operator==(const Hash_t& other) const {
            return (_x[0]==other._x[0]) &&(_x[1]==other._x[1]) &&(_x[2]==other._x[2]) &&(_x[3]==other._x[3]);
        }
    };
    using RankFunc_t = Hash_t (*) (uint32_t, uint32_t);
    /* An entry in the Basalt Node's view */
    struct ViewEntry{
        NodeId id;
        uint64_t seed;
        uint16_t hits;
        std::string to_string() const;
    };
    /* Represents a Basalt protocol Node */
    class Node
    {
    private:
        Array<ViewEntry> _view; /* Local view of the network */
        NodeId _id; /* The node's id */
        bool _isSGX = false; /* Whether or not the node is running in a trusted environment */
        #if IS_BYZANTINE==0
        RankFunc_t _rankingFunc; /* The function used to rank nodes */
        uint32_t _r = 0; /* Local round robin counter */
        uint32_t _k;
        #else
        Array<NodeId> _friends; // malicious friends to advertiste to the honest nodes
        #endif
        uint32_t _iter=0;
        xoshiro256ss _rng;

        /* =================== */
        NodeId selectPeer(); // selects someone in our view based on its hit counter
        uint32_t generateSeed(); // gen a pseudo random seed

        #if IS_BYZANTINE==0
        void pull(NodeId); // pulls the view from another node
        #endif
        void push(NodeId); // pushes our view to someone else

    public:
        #if IS_BYZANTINE==0
        /* Constructs the node */
        Node(NodeId id, const Array<NodeId>& bootstrap, uint32_t numSamplesPerReset, RankFunc_t,
            bool isByzantine=false, bool isSGX=false);
        /* Resets this->_k nodes seeds in the view */
        Array<NodeId> reset();
        /* Updates the current view with the candidates */
        void updateSamples(const Array<NodeId>& candidates); // update our view
        #else
        Node(NodeId, const Array<NodeId>& bootstrap, const Array<NodeId>& friends);
        #endif
        void update(); // performs a Basalt pull/push update round
        /* Message handlers */
        void on_pull_req(net::Message&) const; /* pull request handler */
        #if IS_BYZANTINE==0
        void on_pull_resp(net::Message&, const asio::ip::tcp::endpoint& sender); /* pull response handler */
        void on_push_req(net::Message&, const asio::ip::tcp::endpoint& sender); /* push request handler */
        #endif
        constexpr int is_byzantine() const { return IS_BYZANTINE; }
        std::string to_string() const;
        Array<ViewEntry>::View view() const;
    };
    
} // namespace Basalt
