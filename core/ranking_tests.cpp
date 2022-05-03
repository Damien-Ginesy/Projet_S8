#include <Array.hpp>
#include <NodeId.hpp>
#include <Node.hpp>
#include <hash64.h>
#include <rng.hpp>
#include <time.h>
#include <algorithm>
#include <random>

using namespace Basalt;

uint64_t rank(uint32_t x, uint64_t seed){
    return hash64(x, seed);
}

void update(Array<ViewEntry>& view, const Array<NodeId>& candidates){
    for (size_t i = 0; i < view.size(); i++)
    {
        const uint32_t& seed = view[i].seed; 
        Hash_t currentHash = rank(view[i].id.id, seed);
        for(const NodeId& c: candidates){
            if(c == view[i].id)
            {
                view[i].hits++;
                continue;
            }
            Hash_t r=rank(c.id, seed);
            if(view[i].id==NodeId::null() || r < currentHash)
            {
                std::cout << "Match" << '\n';
                view[i] = ViewEntry {.id=c, .seed=seed, .hits=1};
                currentHash = r;
            }
        }
    }
    
}

int main(int argc, char const *argv[])
{
    xoshiro256ss rng(time(NULL));
    unsigned N = atoi(argv[1]), V = atoi(argv[2]);

    Array<NodeId> nodes(N, NodeId {asio::ip::address_v4(0x7f000001), 3000, 0});
    for (unsigned i = 1; i < N; i++)
        nodes[i].id += i;
    Array<NodeId> bs(V);
    std::sample(nodes.begin(), nodes.end(), bs.begin(), V, std::random_device());

    printf("Bootstrap:\n");

    for(const auto& i: bs) std::cout << i.to_string() << '\n';


    for (uint64_t seed: rng(V)) // generate V random seeds
    {
        printf("==== Seed %016" PRIX64 " ====\n", seed);
        uint64_t h = UINT64_MAX;
        uint32_t match;
        for (const NodeId& c: bs)
        {
            uint64_t r = rank(c.id, seed);
            printf("%03u: %016" PRIX64 "\n", c.id, r);
            if (r < h)
            {
                h = r;
                match = c.id;
            }
        }
        printf("Best match: %03d\n\n", match);
    }
    
    return 0;
}
