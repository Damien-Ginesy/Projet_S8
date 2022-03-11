#pragma 
#include <inttypes.h>

namespace Basalt
{
    struct NodeId
    {
        uint32_t id;
        void deserialize(uint8_t* output) const;
    };
    
} // namespace Basalt

