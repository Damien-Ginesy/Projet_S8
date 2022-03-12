#include "NodeId.hpp"

namespace Basalt
{
    void NodeId::deserialize(byte* output) const{
        toLittleEndian(id, 4, output);
    }
    bool NodeId::operator==(const NodeId& other) const { return id == other.id; }

} // namespace Basalt
