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
        Message& Message::operator>>(uint8_t& out) {
            out = _payload[--_header.size];
            _payload.pop_back();
            return *this;
        }

    } // namespace net
    
} // namespace Basalt
