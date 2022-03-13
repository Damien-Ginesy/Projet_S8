#pragma once
#include <Hash.hpp>
#include <iostream>

class SpookyHash: public Hash<16>
{
private:
    uint64_t& _hash_1 = ((uint64_t*)_data)[0];
    uint64_t& _hash_2 = ((uint64_t*)_data)[1];

    void hex(uint64_t x, char* s) const {
        const char *hexSymbols = "0123456789abcdef";
        for (int i = 15; i >=0 ; i--, x>>=4)
            s[i] = hexSymbols[x&15];        
    }
public:
    SpookyHash(const void* data, size_t len, uint64_t seed1 = 0, uint64_t seed2 = 0);
    uint64_t hash1() const { return _hash_1; }
    uint64_t hash2() const { return _hash_2; }

    void toString(char* out) const {
        hex(_hash_1, out);
        hex(_hash_2, out+16);
    }
    bool operator==(const SpookyHash&) const;
    bool operator<(const SpookyHash&) const;

};
std::ostream& operator<<(std::ostream& s, const SpookyHash& h);