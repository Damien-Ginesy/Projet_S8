#include "Hash.hpp"

/* Wrapper class for Jenkins' one-at-a-time hash function */
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

    void toString(char* output) const override;
};