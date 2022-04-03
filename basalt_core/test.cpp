#include <basalt.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <random>
#include <algorithm>
#include <HTTPLogger.hpp>
#include <SpookyHash.h>


#define ERROR_EXIT(x)   { std::cerr << x << '\n'; exit(EXIT_FAILURE); }

void init(Basalt::NodeId& id, const char *filename,Basalt::Array<Basalt::NodeId>& bs){
    
    using namespace Basalt;
    std::ifstream inputFile(filename);
    using namespace asio::ip;
    if(!inputFile.is_open()) {
        perror(filename); exit(EXIT_FAILURE);
    }
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
    inputFile.close();
    std::random_device rng;
    std::sample(ids.begin(), ids.end(), bs.begin(), bs.size(), rng);
}

Hash<16> hashFunc(const Basalt::NodeId& id, uint32_t seed) {
    byte data[Basalt::NodeId::dataSize] = {0};
    id.to_bytes(data);

    return SpookyHash(data, Basalt::NodeId::dataSize, seed);
}
int main(int argc, char const *argv[])
{
    asio::io_context ctx;
    using namespace Basalt;
    using namespace asio::ip;
    NodeId id {address_v4(0x7F000001), 3000, 0};
    Array<NodeId> bs(2);
    bs[0] = NodeId {address_v4(0x7F000001), 3001, 1};
    bs[1] = NodeId {address_v4(0x7F000001), 3002, 2};
    Node n(id, bs, 1, hashFunc, false, false);
    try
    {
        Basalt::HTTPLogger logger(2, ctx, argv[1], argc>2? atoi(argv[2]):80);
        std::cout << logger.endpoint() << '\n';
        logger << n.to_string() << n.to_string();
        ctx.run();
    }
    catch(const std::runtime_error& ec)
    {
        std::cerr << ec.what() << '\n';
    }
}
