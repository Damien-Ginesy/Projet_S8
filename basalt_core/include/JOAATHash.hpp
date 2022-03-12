#include "Hash.hpp"

struct JOAATHash: public Hash<4>
{
    private:
    uint32_t& _hash = *(uint32_t*)_data;
    public:
    JOAATHash(const void* input, unsigned long long size){
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
    bool operator<(const Hash<4>& other) const override {
        return _hash < *(uint32_t*)other.begin();
    }
    uint32_t value() const { return _hash; }
    const byte* begin() const = delete;
    const byte* end() const = delete;
    const byte operator[](unsigned long long) const = delete;
    void fillFromBuffer(const byte* data) = delete;
    bool operator==(const JOAATHash& other) const {
        return _hash == other._hash;
    }
    friend std::ostream& operator<<(std::ostream& s, const JOAATHash& h){
        const char* sym = "0123456789abcdef";
        for(int i=24; i>=0; i-=8)
            s << sym[(h._hash>>(i+4)) & 15] << sym[(h._hash >> i) & 15];
        return s;
    }
};