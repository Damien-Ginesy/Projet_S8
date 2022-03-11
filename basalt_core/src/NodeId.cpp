#include "NodeId.hpp"

namespace Basalt
{
    void NodeId::deserialize(byte* output) const{
        toLittleEndian(id, 4, output);
    }
} // namespace Basalt
