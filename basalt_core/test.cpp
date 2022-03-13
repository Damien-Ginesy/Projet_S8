#include <iostream>
#include <basalt.hpp>
#include <SHA256Hash.hpp>
#include <JOAATHash.hpp>
#include <SpookyHash.h>
#include <random>

int hammingDistance(uint64_t x, uint64_t y){
    uint8_t s=0;
    for(uint64_t d=x^y; d; d>>=1)
        s += d & 1;
    return s;
}
int hammingDistance(const SpookyHash& h1, const SpookyHash& h2){
    return hammingDistance(h1.hash1(), h2.hash1()) + hammingDistance(h1.hash2(), h2.hash2());
}

int main(int argc, char const *argv[])
{
    std::random_device rng;
    std::uniform_int_distribution<uint64_t> dist;
    uint64_t s1 = dist(rng), s2 = dist(rng);
    printf("Seeds: %016lx %016lx\n", s1, s2);
    SpookyHash h1("\0", 1, s1, s2), h2("\x01", 1, s1, s2);
    std::cout << "SpookyHash(0x00) = " << h1 << '\n';
    std::cout << "SpookyHash(0x01) = " << h2 << '\n';
    std::cout << "Distance: " << hammingDistance(h1, h2) << std::endl;
}
