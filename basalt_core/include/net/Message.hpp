#pragma once
#pragma once


#include <vector>
#include <inttypes.h>
#include <string.h>
#include <asio.hpp>

namespace Basalt
{
    namespace net
    {
        #define N_MSG_TYPES 4
        typedef enum {
            PULL_REQ,
            PUSH_REQ,
            PULL_RESP, // the left hex digit indicates it's a response
            PUSH_RESP
        } MessageType;

        struct Header
        {
            MessageType type;
            uint32_t size;
            static Header fromBytes(const uint8_t* in){
                return {
                    .type = (MessageType)in[0],
                    .size = (uint32_t)(in[1] | (in[2] << 8) | (in[3] << 16) | (in[4]<<24))
                };
            }            
            static constexpr size_t dataSize = 5;

            size_t toBytes(uint8_t* out) const {
                out[0] = (uint8_t)type;
                out[1] = size & 0xff;
                out[2] = (size >> 8) & 0xff;
                out[3] = (size >> 16) & 0xff;
                out[4] = (size >> 24) & 0xff;
                return Header::dataSize;
            }
        };

        struct Message
        {
        protected:
            Header _header;
            std::vector<uint8_t> _payload;
        public:
            Message();
            Message(MessageType type);

            Message& operator<<(uint8_t b);
            Message& operator<<(uint16_t b);
            Message& operator<<(uint32_t b);
            Message& operator<<(uint64_t b);
            Message& operator<<(const char* str);
               
            Message& operator>>(uint8_t& b);
            Message& operator>>(uint16_t& b);
            Message& operator>>(uint32_t& b);
            Message& operator>>(uint64_t& b);

            Message& operator<<(asio::ip::tcp::socket& sock);

            uint32_t size() const { return _payload.size() + Header::dataSize; }
            uint32_t payloadSize() const { return _payload.size(); }
            MessageType get_type() const { return _header.type; }
            void set_type(MessageType t) { _header.type = t; }
            auto begin() { return _payload.begin(); }
            auto end() { return _payload.end(); }
            uint8_t* data() { return _payload.data(); }
            friend asio::ip::tcp::socket& operator<<(asio::ip::tcp::socket&, const Message&);
        };
    } // namespace net
} // namespace Basalt