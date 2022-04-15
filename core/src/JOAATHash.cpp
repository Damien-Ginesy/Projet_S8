#include <JOAATHash.hpp>

JOAATHash::JOAATHash(const void* input, unsigned long long size){
    _hash = 0;
    for(unsigned long long i=0; i<size; ++i)
    {
        _hash += ((const byte*)input)[i];
        _hash += _hash << 10;
        _hash ^= _hash >> 6;
    }
    _hash += _hash << 3;
    _hash ^= _hash >> 11;
    _hash += _hash << 15;
}
bool JOAATHash::operator<(const Hash<4>& other) const {
    return _hash < *(uint32_t*)other.begin();
}
bool JOAATHash::operator==(const JOAATHash& other) const {
    return _hash == other._hash;
}
void JOAATHash::toString(char* output) const {
    const char* hexSym = "0123456789abcdef";
    uint32_t x = _hash;
    for(int i=7; i>=0; --i, x>>=4)
        output[i] = hexSym[x & 15];
}