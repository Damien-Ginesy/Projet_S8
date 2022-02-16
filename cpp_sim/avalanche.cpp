#include <stdio.h>
#include "joaat_hash.h"
#include <random>

#define FLIP_NTH(x, n)      ((x) ^ ((uint32_t)1 << (n)))

unsigned bitSum(uint32_t x)
{
    unsigned s=0;
    for(; x; x>>=1)
        s+=x&1;
    return s;
}

int main(int argc, char const *argv[])
{
    std::random_device rng;
    std::uniform_int_distribution<uint32_t> dist(0,UINT32_MAX);

    uint32_t x = dist(rng);
    uint8_t n = (uint8_t)dist(rng)&31;
    uint32_t y = FLIP_NTH(x, n);

    uint32_t h1=joaat_hash(&x, 4), h2=joaat_hash(&y, 4);
    printf("%u\n", bitSum(h1 ^ h2));
    return 0;
}
