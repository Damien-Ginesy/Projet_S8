#include "Message.hpp"
#include <NodeId.hpp>

namespace Basalt
{
    namespace net{
        struct PullRequestMessage: public Message
        {
            PullRequestMessage& operator<<(const NodeId& id)
            {
                size_t s = _payload.size();
                _payload.resize(s + id.dataSize());
                id.deserialize(_payload.data() + s);
                _header.size = _payload.size();
                return *this;
            }
        };
        
    }
} 
