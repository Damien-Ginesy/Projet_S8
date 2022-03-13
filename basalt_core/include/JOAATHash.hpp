#include "Hash.hpp"

struct JOAATHash: public Hash<4>
{
    private:
    uint32_t& _hash = *(uint32_t*)_data;
    public:
    JOAATHash(const void* input, unsigned long long size);
    bool operator<(const Hash<4>& other) const override;

    uint32_t value() const { return _hash; }
    const byte* begin() const = delete;
    const byte* end() const = delete;
    const byte operator[](unsigned long long) const = delete;
    void fillFromBuffer(const byte* data) = delete;
    bool operator==(const JOAATHash& other) const;

    friend std::ostream& operator<<(std::ostream& s, const JOAATHash& h)
    {
        const char* sym = "0123456789abcdef";
        for(int i=24; i>=0; i-=8)
            s << sym[(h._hash>>(i+4)) & 15] << sym[(h._hash >> i) & 15];
        return s;
    }
};