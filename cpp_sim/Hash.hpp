#pragma once
#include <inttypes.h>
#include "sha256.h"
#include <iostream>

namespace rps
{
    template<uint32_t S>
    struct Hash
    {
        uint8_t data[S] = {0};
        Hash(void (*hashFunc)(const void*, size_t, uint8_t*), const void* input, size_t dataSize)
        {
            hashFunc(input, dataSize, this->data);
        }
        uint8_t* begin(){ return data; }
        uint8_t* end(){ return data+S; }
        const uint8_t* begin() const { return data; }
        const uint8_t* end() const { return data+S; }
        bool operator==(const Hash& other) const
        {
            for(uint32_t i=0; i<S; i++)
                if(data[i] != other.data[i]) return false;
            return true;
        }
        bool operator!=(const Hash& other) const { return !(*this==other); }
        bool operator>(const Hash& other) const {
            uint32_t i=0;
            while(data[i] == other.data[i]) ++i;
            return (i<S && data[i]>other.data[i]);
        }
        bool operator>=(const Hash& other) const { return (*this == other) || (*this > other); }
        bool operator<(const Hash& other) const { return !(*this>=other); }
        bool operator<=(const Hash& other) const { return !(*this>other); }

        friend std::ostream& operator<<(std::ostream& s, const Hash<S>& h)
        {
            const char* hexSyms = "0123456789abcdef";
            for(uint8_t b: h)
                s << hexSyms[b>>4] << hexSyms[b&15];
            return s;
        }
        protected: Hash(){}
    };
    struct SHA256Hash: public Hash<32>
    {
        SHA256Hash(const void* input, uint32_t dataSize)
        {
            sha256_hash(input, dataSize, this->data);
        }
    };
} // namespace basalt

