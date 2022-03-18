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
            uint32_t size() const { return _payload.size()+sizeof(_header); }
        };
    } // namespace net
} // namespace Basalt