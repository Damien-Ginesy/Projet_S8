#pragma once
#include "NodeId.hpp"
#include "Array.hpp"
#include "Hash.hpp"

namespace Basalt
{
    struct ViewEntry{
        NodeId id;
        uint32_t seed;
        uint16_t hits;
        void deserialize(uint8_t* out) const;
    };

    class Node
    {
    private:
        Array<ViewEntry> _view;
        NodeId _id;
        
        bool _isByzantine = false;
        bool _isSGX = false;
        uint32_t _r = 0;
        uint32_t _k;
        void updateSamples(const Array<NodeId>& candidates); // update our view
        /* leave empty for now */
        void pull(NodeId); // pull the view from another node
        void push(NodeId); // push our view to someone else
        /* =================== */
        NodeId selectPeer(); // select someone in our view based on its hit counter
        uint32_t generateSeed(); // gen a pseudo random seed

    public:
        Node(const Array<NodeId>& bootstrap, uint32_t numSamplesPerReset,bool isByzantine=false, bool isSGX=false);
        void reset();
        template<uint32_t HashSize>
        void update(Hash<HashSize> (*h)(const NodeId&, uint32_t seed));
    };
    
} // namespace Basalt
