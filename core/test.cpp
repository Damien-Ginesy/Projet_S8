#include <basalt.hpp>
#include <thread>
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <random>
#include <algorithm>
#include <HTTPLogger.hpp>
#include <misc.h>
#include <semaphore>

std::binary_semaphore sem(0);

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

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    asio::io_context ctx;
    NodeId id {address_v4(0x7F000001), 3000, 0};
    Array<NodeId> bs(2);
    bs[0] = NodeId {address_v4(0x7F000001), 3001, 1};
    bs[1] = NodeId {address_v4(0x7F000001), 3002, 2};
    const char* host = argv[1];
    const char* url = argc>2?argv[2]: "/";
    uint16_t port = argc>3? atoi(argv[3]):80;
    Node n(id, bs, 1, rank, false, false);
    
    try
    {
        HTTPLogger logger(1, host, port, url);
        logger.setCallback([](const llhttp_t& parser, net::HTTPClient::BufferView body){
            std::cout << "\nLOG SERVER RESPONSE" << '\n';
            std::cout << "Received code " << parser.status_code << '\n';
            for(char b: body)
                std::cout << b;
            sem.release();
        });
        std::string msg = n.to_string();
        std::cout << msg << '\n';
        logger << msg;
        sem.acquire();
    }
    catch(const std::runtime_error& ec)
    {
        std::cerr << ec.what() << '\n';
    }
}
