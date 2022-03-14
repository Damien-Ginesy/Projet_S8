#include "NodeId.hpp"

namespace Basalt
{
    size_t NodeId::deserialize(byte* output) const{
        toLittleEndian(id, 4, output);
        return 4;
    }
    bool NodeId::operator==(const NodeId& other) const { return id == other.id; }

} // namespace Basalt
