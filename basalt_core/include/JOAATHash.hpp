#include "Hash.hpp"

struct JOAATHash: public Hash<4>
{
    private:
    byte _data[4] = {0};
    uint32_t& _value = *(uint32_t*)_data;
    public:
    JOAATHash(const byte* input, unsigned long long size){
        for(unsigned long long i=0; i<size; ++i)
        {
            _value += input[i];
            _value += _value << 10;
            _value ^= _value >> 6;
        }
        _value += _value << 3;
        _value ^= _value >> 11;
        _value += _value << 15;
    }
    bool operator<(const Hash<4>& other) const {
        return _value < *(uint32_t*)other.begin();
    }
    uint32_t value() const { return _value; }
    const byte* begin() const = delete;
    const byte* end() const = delete;
    const byte operator[](unsigned long long) const = delete;
    void fillFromBuffer(const byte* data) = delete;
    bool operator==(const JOAATHash& other) const {
        return _value == other._value;
    }
    friend std::ostream& operator<<(std::ostream& s, const JOAATHash& h){
        const char* sym = "0123456789abcdef";
        for(int i=24; i>=0; i-=8)
            s << sym[(h._value>>(i+4)) & 15] << sym[(h._value >> i) & 15];
        return s;
    }
};