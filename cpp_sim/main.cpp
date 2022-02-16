#include <iostream>
#include "Node.hpp"
#include <thread>
#include <chrono>
#include "joaat_hash.h"

#define N               1000
#define V               50
#define N_LOOPS         200
#define K               ((V>>1) - 1)
#define RESET_RATE      K
#define f               .1
#define F               10

bool R(uint32_t seed, uint32_t i1, uint32_t i2){
    typedef struct{ uint32_t seed; uint32_t id; } rankInput;
    rankInput x1 = {seed, i1};
    rankInput x2 = {seed, i2};
    return joaat_hash(&x1, sizeof(x1)) < joaat_hash(&x2, sizeof(x2));
} 

int main(int argc, char const *argv[])
{
    
}
