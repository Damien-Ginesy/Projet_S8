#include <basalt.hpp>
#include <fasthash.h>
#include <rng.hpp>
#include <time.h>
#include <algorithm>
#include <random>

using namespace Basalt;


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
    return 0;
}
