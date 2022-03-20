#pragma 
#include "misc.h"
#include <net/Message.hpp>

namespace Basalt
{
    struct NodeId
    {
        uint32_t id;
        void serialize(uint8_t* output) const;
        static NodeId deserialize(const uint8_t* input);
        bool operator==(const NodeId& other) const;
        static constexpr size_t dataSize = 4;
        static NodeId null() { return NodeId{0}; }
    };

    net::Message& operator<<(net::Message& m, const NodeId& id){
        m << id.id;
        return m;
    }    
} // namespace Basalt

