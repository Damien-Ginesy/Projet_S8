#pragma 
#include "misc.h"
#include <net/Message.hpp>

namespace Basalt
{
    /* Represents an node's id */
    struct NodeId
    {
        asio::ip::address_v4 _addr; // address of the remote node
        uint16_t _port; // port the remote node is listening on
        uint16_t id; // virtual id value, used for hashing
        /* Dump the raw bytes of the id */
        void serialize(uint8_t* output) const;
        /* Construct an id from raw bytes */
        static NodeId deserialize(const uint8_t* input);
        bool operator==(const NodeId& other) const;
        /* The size of the actual data, once serialized to raw bytes */
        static constexpr size_t dataSize = 8;
        /* Represents a default invalid id */
        static NodeId null() { return NodeId{asio::ip::address_v4(0), 0, 0}; }
    };
    /* dumps a NodeId to a Message object */
    net::Message& operator<<(net::Message& m, const NodeId& id){
        m << id.id;
        return m;
    }    
} // namespace Basalt

