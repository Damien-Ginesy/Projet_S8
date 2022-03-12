#include <iostream>
#include "Node.hpp"
#include "joaat_hash.h"
#include "ScopeTimer.hpp"

uint32_t R(uint32_t seed, uint32_t id){
    struct{uint32_t x, y;} input = {seed, id};
    return joaat_hash(&input, sizeof(input));
}

int main(int argc, char const *argv[])
{
    constexpr uint32_t V = 50;
    uint32_t N = atoi(argv[1]);
    float f = atof(argv[2]);
    uint32_t mCount = f * N; uint32_t hCount = N-mCount;
    std::cout << "Creating nodes..." << std::endl;
    rps::Array<rps::Node> nodes(N);
    for(uint32_t i=0; i<N; i++)
    {
        nodes.emplace(i, V, R);
        nodes[i].id = i;
        nodes[i].isByzantine = i>=hCount;
    }
    
    rps::ArrayView<rps::Node> honestNodes = nodes.sub(0, hCount);
    rps::ArrayView<rps::Node> maliciousNodes = nodes.sub(hCount);
    std::cout << "Initializing nodes..." << std::endl;
    for(auto& h: honestNodes)
        h.init(rps::sample_n(V, nodes.view()).view());
    for(auto& m: honestNodes)
        m.init(rps::sample_n(V, maliciousNodes).view());
}
