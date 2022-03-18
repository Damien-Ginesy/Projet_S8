#pragma 
#include "misc.h"

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
    
} // namespace Basalt

