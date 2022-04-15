#pragma once
#include <stdint.h>

class xoshiro256ss {
private:
	uint64_t _s[4];
public:
    struct iterator
    {
        xoshiro256ss* _rng;
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
    
    xoshiro256ss(uint64_t seed);
    uint64_t operator()();
    generator operator()(uintmax_t n);
};