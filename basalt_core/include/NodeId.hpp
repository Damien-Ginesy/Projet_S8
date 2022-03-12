#pragma 
#include "misc.h"

namespace Basalt
{
    struct NodeId
    {
        uint32_t id;
        void deserialize(uint8_t* output) const;
        bool operator==(const NodeId& other) const;

        static NodeId null() { return NodeId{0}; }
    };
    
} // namespace Basalt

