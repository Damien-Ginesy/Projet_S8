#pragma once
#pragma once


#include <vector>
#include <inttypes.h>
#include <string.h>

namespace Basalt
{
    namespace net
    {
        typedef enum {
            PULL_REQ,
            PUSH_REQ
        } MessageType;

        struct Header
        {
            MessageType type;
            uint32_t size;
        };

        struct Message
        {
        protected:
            Header _header;
            std::vector<uint8_t> _payload;
        public:
            Message(MessageType type);
            // shift operator for numbers
            Message& operator<<(uint8_t b);
            Message& operator<<(uint16_t b);
            Message& operator<<(uint32_t b);
            Message& operator<<(uint64_t b);
            Message& operator>>(uint8_t& b);
            Message& operator>>(uint16_t& b);
            Message& operator>>(uint32_t& b);
            Message& operator>>(uint64_t& b);

            uint32_t size() const { return _payload.size()+sizeof(_header); }
            auto begin() { return _payload.begin(); }
            auto end() { return _payload.end(); }
        };
    } // namespace net
} // namespace Basalt