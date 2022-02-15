#include <iostream>
#include "Node.hpp"
#include <thread>
#include <chrono>
#include "joaat_hash.h"

#define N               10000
#define V               50
#define N_LOOPS         200
#define RESET_RATE      5
#define K               25

bool R(uint32_t seed, uint32_t i1, uint32_t i2){
    typedef struct{ uint32_t seed; uint32_t id; } rankInput;
    rankInput x1 = {seed, i1};
    rankInput x2 = {seed, i2};
    return joaat_hash(&x1, sizeof(x1)) < joaat_hash(&x2, sizeof(x2));
} 
rps::Array<uint32_t> sampleBootstrap(size_t n, const rps::Array<rps::Node>& nodes){
    rps::Array<uint32_t> bs(n);
    std::random_device rng;
    std::uniform_int_distribution<uint32_t> dist(0, nodes.size()-1);
    bs[0] = dist(rng);
    SAMPLE: for(size_t i=1; i<n; i++){
        size_t x=dist(rng);
        for(size_t j=0; j<i; j++)
            if(bs[j] == x) goto SAMPLE;
        bs[i] = x;
    }
    return bs;
}
int main(int argc, char const *argv[])
{
    rps::Array<rps::Node> nodes(N);
    for(size_t i=0; i<N; ++i)
        nodes.emplace(i, V, i, R);

    std::random_device rng;
    std::uniform_int_distribution<uint32_t> dist(0, N-1);
    rps::Array<uint32_t> bs(V);
    for (auto& n: nodes)
        n.init(sampleBootstrap(V, nodes));
    std::cout << "Entering main loop" << std::endl;
    // main loop
    for(size_t i=1; i<=N_LOOPS; ++i){
        std::cout << "Iter " << i << std::endl;
        bool reset = (i%RESET_RATE)==0;
        for(auto& n: nodes){
            n.update(nodes);
            if(reset) n.reset(K);
        }
    }
    
    return 0;
}
