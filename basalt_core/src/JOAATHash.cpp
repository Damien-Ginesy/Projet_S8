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