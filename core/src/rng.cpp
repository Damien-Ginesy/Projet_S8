#include "rng.hpp"

#define ROL64(x, k) (((x) << (k)) | ((x) >> (64 - (k))))

static inline uint64_t splitmix64(uint64_t& state){
    uint64_t result = (state += 0x9E3779B97f4A7C15);
    result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;
    return (result ^ (result >> 27)) * 0x94D049BB133111EB;
}
static inline uint64_t xorshift64(uint64_t& x){
    x ^= x<<13;
    x ^= x>>7;
    return (x ^ x<<17);
}

xoshiro256ss::xoshiro256ss(uint64_t seed){
    _s[0] = splitmix64(seed);
    _s[1] = splitmix64(seed);
    _s[2] = splitmix64(seed);
    _s[3] = splitmix64(seed);
}

uint64_t xoshiro256ss::operator()(){
    uint64_t r = ROL64(_s[1] * 5, 7) * 9;
    uint64_t const t = _s[1] << 17;

	_s[2] ^= _s[0];
	_s[3] ^= _s[1];
	_s[1] ^= _s[2];
	_s[0] ^= _s[3];

	_s[2] ^= t;
	_s[3] = ROL64(_s[3], 45);
    return r;
}
using iter_t = xoshiro256ss::iterator;
bool iter_t::operator==(const iter_t& other) const{
    return (_rng == other._rng) && (_c == other._c);
}
bool iter_t::operator!=(const iter_t& other) const{
    return !(*this == other);
}
uint64_t iter_t::operator*() const {
    return (*_rng)();
}
iter_t iter_t::operator++(int){
    iter_t old = *this;
    _c++;
    return old;
}
iter_t& iter_t::operator++(){
    _c++;
    return *this;
}

xoshiro256ss::generator xoshiro256ss::operator()(uintmax_t n){
    return xoshiro256ss::generator {{this,0}, {this, n}};
}