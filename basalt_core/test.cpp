#include <basalt.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <random>
#include <algorithm>
#include <HTTPLogger.hpp>
#include <SHA256Hash.hpp>
#include <misc.h>

#define ERROR_EXIT(x)   { std::cerr << x << '\n'; exit(EXIT_FAILURE); }

#define ESC             "\033["
#define STYLE_DEFAULT   ESC "0m"
#define STYLE_BOLD      ESC "1m"
#define STYLE_RED       ESC "31m"
#define STYLE_GREEN     ESC "32m"

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
        }
        else{
            id._addr = make_address_v4(ip);
            id._port = port;
        }
    }
    inputFile.close();
    std::random_device rng;
    std::sample(ids.begin(), ids.end(), bs.begin(), bs.size(), rng);
    for(const NodeId& id: bs)
        std::cout << id.to_string() << '\n';
}


Basalt::Node::Hash_t hashFunc(const Basalt::NodeId& id, uint32_t seed) {
        uint8_t data[8];
        Basalt::toLittleEndian(id.id, 4, data);
        Basalt::toLittleEndian(seed, 4, data+4);
        return SHA256Hash(data, 8);
}

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    asio::io_context ctx;
    NodeId id {address_v4(0x7F000001), 3000, 0};
    Array<NodeId> bs(2);
    bs[0] = NodeId {address_v4(0x7F000001), 3001, 1};
    bs[1] = NodeId {address_v4(0x7F000001), 3002, 2};
    Node n(id, bs, 1, hashFunc, false, false);
    try
    {
        HTTPLogger logger(1, "localhost", 3000, "/");
        logger.setCallback([](const llhttp_t& parser, net::HTTPClient::BufferView body){
            std::cout << "Received code " << parser.status_code << '\n';
            for(char b: body)
                std::cout << b;
        });
        logger << n.to_string();
        std::getchar();
    }
    catch(const std::runtime_error& ec)
    {
        std::cerr << ec.what() << '\n';
    }
}
