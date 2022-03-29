#include <basalt.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <random>
#include <algorithm>

#define ERROR_EXIT(x)   { std::cerr << x << '\n'; exit(EXIT_FAILURE); }

int main(int argc, char const *argv[])
{
    using namespace asio::ip;
    if(argc < 2) ERROR_EXIT("Usage: ./test <id> <init file>");
    using namespace Basalt;
    Basalt::NodeId id;
    id.id = atoi(argv[1]);
    std::ifstream inputFile(argv[2]);
    if(!inputFile.is_open()) {
        perror(argv[2]); return 1;
    }
    NodeId localId;
    std::vector<NodeId> ids;
    while (!inputFile.eof())
    {
        std::string ip;
        uint16_t port, idVal;
        inputFile >> ip >> port >> idVal;
        if(ip.empty()) continue;
        if(idVal != id.id){
            NodeId id { make_address_v4(ip), port, idVal };
            ids.push_back(id);
            std::cout << id.to_string() << '\n';
        }
        else{
            id._addr = make_address_v4(ip);
            id._port = port;
        }
    }
    size_t N = ids.size();
    std::cout << "Read " << N << " IDs\n";
    std::cout << "Local id: " << id.to_string() << '\n';

    Array<NodeId> bs(N / 2);
    std::random_device rng;
    std::sample(ids.begin(), ids.end(), bs.begin(), bs.size(), rng);

    std::cout << "Chosen bootstrap: " << '\n';
    for(auto& id: bs)
        std::cout << id.to_string() << '\n';

    inputFile.close();
    std::cout << "Initalizing BASALT..." << '\n';
    using namespace std::chrono_literals;
    basalt_init(id, bs, 3s, 1h);

    std::getchar();
    basalt_stop();
}
