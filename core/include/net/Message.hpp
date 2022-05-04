#pragma once
#pragma once

#include <vector>
#include <inttypes.h>
#include <string.h>
#include <asio.hpp>
#include <misc.h>
#include <Array.hpp>

namespace Basalt
{
    namespace net
    {
        /* All the different types of message */
        typedef enum {
            PULL_REQ,
            PUSH_REQ,
            PULL_RESP       = 0x10, // the left hex digit indicates it's a response
            PUSH_RESP,
            SESSION_END     = 0x80
        } MessageType;
        /* Represents the message header:
        the type
        the size of the payload in bytes
         */
        struct Header
        {
            MessageType type;
            uint32_t size;
            /* Constructs the Header object from raw bytes */
            static Header fromBytes(const uint8_t* in){
                return {
                    .type = (MessageType)in[0],
                    .size = lendian32(*(uint32_t*)(in+1))
                };
            }
            /* The raw data size of a serialized Header object */
            static constexpr size_t dataSize = 5;
            /* Dumps a Header object to raw bytes */
            void toBytes(uint8_t out[]) const {
                out[0] = (uint8_t)      type;
                out[1] = (uint8_t)      (size & 0xff);
                out[2] = (uint8_t)     ((size >> 8) & 0xff);
                out[3] = (uint8_t)     ((size >> 16) & 0xff);
                out[4] = (uint8_t)     ((size >> 24) & 0xff);
            }
        };
        /* Represents a message */
        struct Message
        {
            using iterator = std::vector<uint8_t>::iterator;
        protected:
            /* Header of the message */
            Header _header;
            /* The actual data */
            std::vector<uint8_t> _payload;
        public:
            Message();
            Message(MessageType type);

            /* Append operators
               *************** */
            Message& operator<<(uint8_t b); /* Appends a single byte to the Message */
            Message& operator<<(uint16_t b); /* Appends a 16-bit int to the Message in little endian order */
            Message& operator<<(uint32_t b); /* Appends a 32-bit int to the Message in little endian order */
            Message& operator<<(uint64_t b); /* Appends a 64-bit int to the Message in little endian order */
            Message& operator<<(const char* str); /* Appends a string to the message */
            Message& operator<<(const std::string& str); /* Appends a string to the message */
            template<typename T, size_t N>
            Message& operator<<(const std::array<T, N>& in){
                for(const T& x: in)
                    *this << x;
                return *this;
            }
            template<typename T, size_t N>
            Message& operator<<(const std::array<T, N>&& in){
                for(const T& x: in)
                    *this << x;
                return *this;
            }
            template<size_t N>
            Message& operator>>(std::array<byte, N>& out){
                _header.size -= N;
                for(size_t i=0; i<N; i++)
                    out[i] = _payload[_header.size+i];
                _payload.resize(_header.size);
                return *this;
            }
            Message& operator>>(Array<byte>& out);
            template<typename T>
            Message& operator<<(const Array<T>& in){
                for(const T& x: in)
                    *this << x;
                return *this;
            }
            template<typename T>
            Message& operator<<(const Array<T>&& in){
                for(const T& x: in)
                    *this << x;
                return *this;
            }
            /*
            template<typename T>
            Message& operator<<(Array<T>::View in){
                for(const T& x: in)
                    *this << x;
                return *this;
            }*/
            Message& operator>>(uint8_t& b);
            Message& operator>>(uint16_t& b);
            Message& operator>>(uint32_t& b);
            Message& operator>>(uint64_t& b);
            void operator>>(char* out);
            void operator>>(std::string& out);
            /* Returns the total size of the message (header included) */
            uint32_t size() const { return (uint32_t)_payload.size() + Header::dataSize; }
            /* Returns the size of the payload */
            uint32_t payloadSize() const { return (uint32_t)_payload.size(); }
            /* Returns the type of the message */
            MessageType get_type() const { return _header.type; }
            /* Sets the type of the message */
            void set_type(MessageType t) { _header.type = t; }
            /* Returns an iterator to the beginning of the data */
            iterator begin() { return _payload.begin(); }
            /* Returns an iterator to past the end of the data */
            iterator end() { return _payload.end(); }
            /* Pointer to the raw data */
            uint8_t* data() { return _payload.data(); }
            /* Attempts to write the message data into the socket */
            asio::error_code writeTo(asio::ip::tcp::socket&) const;
            /* Attempts to write the message data from the socket */
            asio::error_code readFrom(asio::ip::tcp::socket&);

            void append(const uint8_t *input, size_t n);
            void pop(uint8_t *output, size_t n);

        };
    } // namespace net
} // namespace Basalt
