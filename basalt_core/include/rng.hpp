#pragma once
#include <stdint.h>

struct xoshiro256s {
private:
	uint64_t _s[4];
    uint64_t _val;
public:
    struct iterator
    {
        xoshiro256s* _rng;
        uintmax_t _c;
        bool operator==(const iterator&) const;
        bool operator!=(const iterator&) const;
        iterator& operator++();
        iterator operator++(int);
        uint64_t operator*() const;
    };
    struct generator
    {
        iterator _start, _end;
        iterator begin() const { return _start; }
        iterator end() const { return _end; }
    };
    
    xoshiro256s(uint64_t seed);
    uint64_t operator()();
    generator operator()(uintmax_t n);
};