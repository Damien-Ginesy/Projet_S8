#pragma once
#include <asio/ip/address_v4.hpp>

namespace Basalt
{
    /* Represents an node's id */
    struct NodeId
    {
        asio::ip::address_v4 _addr; // address of the remote node
        uint16_t _port; // port the remote node is listening on
        uint16_t id; // virtual id value, used for hashing
        /* Dump the raw bytes of the id */
        void to_bytes(uint8_t* output) const;
        /* Construct an id from raw bytes */
        static NodeId from_bytes(const uint8_t* input);
        bool operator==(const NodeId& other) const;
        /* The size of the actual data, once serialized to raw bytes */
        static constexpr size_t dataSize = 8;
        /* Represents a default invalid id */
        static inline NodeId null() { return NodeId{asio::ip::address_v4(0), 0, 0xffff}; }
        /* Represents the NodeId object, in JSON format */
        std::string to_string() const;
    };
} // namespace Basalt

