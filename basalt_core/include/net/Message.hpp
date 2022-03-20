#pragma once
#pragma once


#include <vector>
#include <inttypes.h>
#include <string.h>

namespace Basalt
{
    namespace net
    {
        #define N_MSG_TYPES 4
        typedef enum {
            PULL_REQ,
            PUSH_REQ,
            PULL_RESP=0x10, // the left hex digit indicates it's a response
            PUSH_RESP
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

            Message& operator<<(uint8_t b);
            Message& operator<<(uint16_t b);
            Message& operator<<(uint32_t b);
            Message& operator<<(uint64_t b);
               
            Message& operator>>(uint8_t& b);
            Message& operator>>(uint16_t& b);
            Message& operator>>(uint32_t& b);
            Message& operator>>(uint64_t& b);

            uint32_t size() const { return _payload.size() + sizeof(Header); }
            uint32_t payloadSize() const { return _payload.size(); }
            auto begin() { return _payload.begin(); }
            auto end() { return _payload.end(); }
        };
    } // namespace net
} // namespace Basalt