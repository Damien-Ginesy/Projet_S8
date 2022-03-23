#include <NodeId.hpp>
#include <array>

namespace Basalt
{
    void NodeId::serialize(byte* output) const{
        toLittleEndian(id, NodeId::dataSize, output);
    }
    NodeId NodeId::deserialize(const byte* input) {
        using namespace asio::ip;
        std::array<byte, 4> addrBytes
            {input[0], input[1], input[2], input[3]};
        address_v4 addr = make_address_v4(addrBytes);
        uint16_t port = input[4] | (input[5]<<8);
        uint16_t id = input[6] | (input[7]<<8);
        return NodeId {addr, port, id};
    }
    bool NodeId::operator==(const NodeId& other) const { return id == other.id; }

} // namespace Basalt
