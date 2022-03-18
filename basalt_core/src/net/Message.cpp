#include <net/Message.hpp>

namespace Basalt
{
    namespace net
    {
        Message::Message(MessageType t): _header({t, 0}) {}
        Message& Message::operator<<(uint8_t b){
            _payload.push_back(b);
            ++_header.size;
            return *this;
        }
        Message& Message::operator<<(uint16_t x) {
            *this << (uint8_t)(x&0xff) << (uint8_t)(x>>8);
            return *this;
        }
        Message& Message::operator<<(uint32_t x) {
            *this << (uint16_t)(x&0xffff) << (uint16_t)(x>>16);
            return *this;
        }
        Message& Message::operator<<(uint64_t x) {
            *this << (uint32_t)(x&0xffffffff) << (uint32_t)(x>>32);
            return *this;
        }
        Message& Message::operator>>(uint8_t& out) {
            out = _payload[--_header.size];
            _payload.pop_back();
            return *this;
        }
        Message& Message::operator>>(uint16_t& out) {
            uint8_t a, b;
            *this >> a >> b;
            out = ((uint16_t)a << 8) | b;
            return *this;
        }
        Message& Message::operator>>(uint32_t& out) {
            uint16_t a, b;
            *this >> a >> b;
            out = ((uint32_t)a << 16) | b;
            return *this;
        }
        Message& Message::operator>>(uint64_t& out) {
            uint32_t a, b;
            *this >> a >> b;
            out = ((uint64_t)a << 32) | b;
            return *this;
        }
        Message& Message::operator<<(const NodeId& id){
            _payload.resize(_header.size+NodeId::dataSize);
            id.serialize(_payload.data() + _header.size);
            _header.size = _payload.size();
            return *this;
        }
        Message& Message::operator>>(NodeId& id){
            _header.size -= NodeId::dataSize;
            id = NodeId::deserialize(_payload.data()+_header.size);
            _payload.resize(_header.size);
            return *this;
        }

    } // namespace net
    
} // namespace Basalt
