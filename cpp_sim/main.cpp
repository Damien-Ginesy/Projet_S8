#include <iostream>
#include "Node.hpp"
#include "joaat_hash.h"
#include "ScopeTimer.hpp"


#define N_LOOPS         200

uint32_t R(uint32_t seed, uint32_t id){
    struct{uint32_t x, y;} input = {seed, id};
    return joaat_hash(&input, sizeof(input));
}

int main(int argc, char const *argv[])
{
    if(argc < 7){
        std::cerr << "Usage: ./sim <node count> <view size> <malicious count> <attack Force>"
        " <reset rate> <attack start>\n";
        return EXIT_FAILURE;
    }
    uint32_t N = atoi(argv[1]);
    uint32_t V = atoi(argv[2]);
    uint32_t mCount = atoi(argv[3]); uint32_t hCount = N-mCount;
    unsigned F = atoi(argv[4]);
    uint32_t attackStart = atoi(argv[6]);
    uint32_t resetRate = atoi(argv[5]);

    std::cout << "Creating nodes..." << std::endl;
    rps::Array<rps::Node> nodes(N);
    for(uint32_t i=0; i<N; i++)
    {
        nodes.emplace(i, V, R);
        nodes[i].id = i;
        nodes[i].isByzantine = i>=hCount;
    }
    /* init */ 
    rps::ArrayView<rps::Node> honestNodes = nodes.sub(0, hCount);
    rps::ArrayView<rps::Node> maliciousNodes = nodes.sub(hCount);
    std::cout << "Initializing nodes..." << std::endl;
    for(auto& h: honestNodes)
        h.init(rps::sample_n(V, nodes.view()).view());
    for(auto& m: maliciousNodes)
        m.init(rps::sample_n(V, maliciousNodes).view());

    /* mainloop */
    uintmax_t maliciousProp;
    std::cout << "Starting simulation..." << std::endl;
    for (size_t i = 1; i <= N_LOOPS; i++)
    {
        maliciousProp = 0;
        for(auto& n: honestNodes){
            n.step(nodes.view());
            maliciousProp += n.maliciousCount();
        }
        if(i>=attackStart)
            for(auto& m: maliciousNodes)
                m.step(honestNodes, 10);
        if(i%resetRate == 0){
            for(auto& n: nodes)
                n.reset(V>>1);
        }
        std::cout << maliciousProp/hCount << std::endl;
    }
    
}
