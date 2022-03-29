#include <NodeId.hpp>
#include <misc.h>
#include <sstream>

namespace Basalt
{
    void NodeId::to_bytes(byte* output) const{
        using namespace asio::ip;
        memcpy(output, _addr.to_bytes().data(), 4);
        toLittleEndian(_port, 2, output+4);
        toLittleEndian(id, 2, output+6);
    }
    NodeId NodeId::from_bytes(const byte* input) {
        using namespace asio::ip;
        address_v4::bytes_type addrBytes
            {input[0], input[1], input[2], input[3]};
        address_v4 addr = make_address_v4(addrBytes);
        uint16_t port = input[4] | (input[5]<<8);
        uint16_t id = input[6] | (input[7]<<8);
        return NodeId {addr, port, id};
    }
    bool NodeId::operator==(const NodeId& other) const { return id == other.id; }
    std::string NodeId::to_string() const {
        std::stringstream s;
        s << "{\"ip\": \"" << _addr.to_string() << 
            "\", \"port\": " << _port << 
            ", \"virtual_id\": " << id << "}";
        return s.str();
    }
} // namespace Basalt
