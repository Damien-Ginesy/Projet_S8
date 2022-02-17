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
    constexpr uint32_t V = 4;
    uint32_t N = atoi(argv[1]);
    rps::Node n(V, R);
    std::random_device rng;
    std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
    rps::Array<uint32_t> bs(V*N);
    using namespace std::chrono;
    n.init(dist(rng), false, bs.view());
    microseconds d;
    {
        std::cout << "Starting timer..." << std::endl;
        ScopeTimer t(&d);
        for(size_t i=V; i<N; i+=V){
            n.updateView(bs.sub(i, V));
        }
    }
    std::cout << "\x1b[32;1m" << d.count() << " microseconds (" << (double)d.count()/1000 << "ms)" << "\x1b[0m\n";
}
