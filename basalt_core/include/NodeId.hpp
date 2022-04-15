#pragma once
#include <asio/ip/address_v4.hpp>
#include <array>

namespace Basalt
{
    /* Represents an node's id */
    struct NodeId
    {
        static constexpr size_t dataSize = 10;
        using bytes_t = std::array<unsigned char, NodeId::dataSize>;
        asio::ip::address_v4 _addr; // address of the remote node
        uint16_t _port; // port the remote node is listening on
        uint32_t id; // virtual id value, used for hashing
        /* Dump the raw bytes of the id */
        void to_bytes(bytes_t& out) const;
        /* Construct an id from raw bytes */
        static NodeId from_bytes(const bytes_t& input);
        bool operator==(const NodeId& other) const;
        /* The size of the actual data, once serialized to raw bytes */
        /* Represents a default invalid id */
        static inline NodeId null() { return NodeId{asio::ip::address_v4(0), 0, 0xffff}; }
        /* Represents the NodeId object, in JSON format */
        std::string to_string() const;
    };
} // namespace Basalt

