#include <NodeId.hpp>

namespace Basalt
{
    void NodeId::serialize(byte* output) const{
        toLittleEndian(id, NodeId::dataSize, output);
    }
    NodeId NodeId::deserialize(const byte* input) {
        return NodeId {(uint32_t)input[0] | (input[1] << 8) | (input[2] << 16) | (input[3] << 24)};
    }
    bool NodeId::operator==(const NodeId& other) const { return id == other.id; }

} // namespace Basalt
