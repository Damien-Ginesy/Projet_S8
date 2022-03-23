#pragma once
#include <inttypes.h>
#include <iostream>

using byte=uint8_t;

/* Rempresents a generic comparable Hash object of S bytes */
template<uint32_t S>
struct Hash
{
    using Hash_t = Hash<S>;
    protected:
    byte _data[S];
    Hash(){};
    public:
    /* Returns an iterator to the beginning of the data */
    const byte* begin() const { return _data; }
    /* Returns an iterator to past the end of the data */
    const byte* end() const { return _data+S; }
    /* const data access */
    byte operator[](uint32_t i) const { return _data[i]; }
    Hash(byte* hash){
        for(uint32_t i=0; i<S; i++) _data[i] = hash[i];
    }
    /* Builds the Hash object from a given hash function, fed with some input data */
    static Hash<S> compute(void (*func)(const void* in, uintmax_t len, byte* out), const void* in, uintmax_t len){
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
    virtual void toString(char* output) const {
        const char* hexSymbols = "0123456789abcdef";
        for(int i=0; i<S; ++i)
        {
            output[2*i] = hexSymbols[_data[i] >> 4];
            output[2*i+1] = hexSymbols[_data[i] & 15];
        }
    }
};
template<uint32_t S>
std::ostream& operator<<(std::ostream& s, const Hash<S>& h){
    char hex[2*S+1] = {};
    h.toString(hex);
    s << hex;
    return s;
}