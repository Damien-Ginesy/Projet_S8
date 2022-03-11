#pragma once
#include <inttypes.h>
#include <iostream>

using byte=uint8_t;
template<uint32_t S>
struct Hash
{
    using Hash_t = Hash<S>;
    protected:
    byte _data[S];
    Hash(){};
    public:
    const byte* begin() const { return _data; }
    const byte* end() const { return _data+S; }
    byte operator[](uint32_t i) const { return _data[i]; }
    Hash(byte* hash){
        for(uint32_t i=0; i<S; i++) _data[i] = hash[i];
    }
    static Hash<S> compute(void (*func)(byte* in, uintmax_t len, byte* out), byte* in, uintmax_t len){
        Hash<S> h;
        func(in, len, h._data);
        return h;
    }
    virtual bool operator==(const Hash_t& other) const {
        for(uint32_t i=0; i<S; i++)
            if(_data[i] != other._data[i]) return false;
        return true;
    }
    virtual bool operator<(const Hash_t& other) const {
        for(uint32_t i=0; i<S; ++i)
            if(_data[i] < other._data[i]) return true;
        return false;
    }
    bool operator<=(const Hash_t& other) const{
        return (*this < other) || (*this == other);
    }
    bool operator>(const Hash_t& other) const {
        return !(*this <= other);
    }
    bool operator>=(const Hash_t& other) const {
        return (*this > other) || (*this == other);
    }
};
template<uint32_t S>
std::ostream& operator<<(std::ostream& s, const Hash<S>& h){
    const char* hexSym = "0123456789abcdef";
    for(uint8_t x: h)
        s << hexSym[x>>4] << hexSym[x&15];
    return s;
}