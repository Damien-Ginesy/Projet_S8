#include "rng.hpp"

#define ROL64(x, k) (((x) << (k)) | ((x) >> (64 - (k))))
#define splitmix64(state, result){\
    result = (state += 0x9E3779B97f4A7C15);\
    result = (result ^ (result >> 30)) * 0xBF58476D1CE4E5B9;\
    result = (result ^ (result >> 27)) * 0x94D049BB133111EB;\
}

xoshiro256s::xoshiro256s(uint64_t seed){
    splitmix64(seed, _s[0]);
    splitmix64(seed, _s[1]);
    splitmix64(seed, _s[2]);
    splitmix64(seed, _s[3]);
    this->operator()();
}

uint64_t xoshiro256s::operator()(){
    _val = ROL64(_s[1] * 5, 7) * 9;
    uint64_t const t = _s[1] << 17;

	_s[2] ^= _s[0];
	_s[3] ^= _s[1];
	_s[1] ^= _s[2];
	_s[0] ^= _s[3];

	_s[2] ^= t;
	_s[3] = ROL64(_s[3], 45);
    return _val;
}
using iter_t = xoshiro256s::iterator;
bool iter_t::operator==(const iter_t& other) const{
    return (_rng == other._rng) && (_c == other._c);
}
bool iter_t::operator!=(const iter_t& other) const{
    return !(*this == other);
}
uint64_t iter_t::operator*() const {
    return _rng->_val;
}
iter_t iter_t::operator++(int){
    iter_t old = *this;
    (*_rng)();
    _c++;
    return old;
}
iter_t& iter_t::operator++(){
    (*_rng)();
    _c++;
    return *this;
}

xoshiro256s::generator xoshiro256s::operator()(uintmax_t n){
    return xoshiro256s::generator {{this,0}, {this, n}};
}