#pragma once
#include "NodeId.hpp"
#include "Array.hpp"
#include "Hash.hpp"
#include <random>

namespace Basalt
{
    /* An entry in the Basalt Node's view */
    struct ViewEntry{
        NodeId id;
        uint32_t seed;
        uint16_t hits;
    };
    /* Represents a Basalt protocol Node */
    class Node
    {
    private:
        Array<ViewEntry> _view; /* Local view of the network */
        NodeId _id; /* The node's id */
        
        bool _isByzantine = false; /* Whether or not the node is malicious */
        bool _isSGX = false; /* Whether or not the node is running in a trusted environment */
        Hash<16> (*_rankingFunc) (const NodeId&, uint32_t); /* The function used to rank nodes */
        uint32_t _r = 0; /* Local round robin counter */
        uint32_t _k; 
        std::random_device _rng;

        /* =================== */
        NodeId selectPeer(); // selects someone in our view based on its hit counter
        uint32_t generateSeed(); // gen a pseudo random seed

    public:
        /* Constructs the node */
        Node(const Array<NodeId>& bootstrap, uint32_t numSamplesPerReset, Hash<16> (*)(NodeId, uint32_t),
            bool isByzantine=false, bool isSGX=false);
        /* Resets this->_k nodes seeds in the view */
        Array<NodeId> reset();
        /* Updates the current view with the candidates */
        void updateSamples(const Array<NodeId>& candidates); // update our view
        void pull(NodeId); // pulls the view from another node
        void push(NodeId); // pushes our view to someone else
        void update(); // performs a Basalt pull/push update round
    };
    
} // namespace Basalt
