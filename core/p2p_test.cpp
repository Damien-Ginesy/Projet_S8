#include <basalt.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <random>

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
}
void on_logger_response(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    std::cout << "[LOGGER] Received code " << parser.status_code << '\n';
}

//doc p2p_test.cpp id viewSize bootstrap
int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    NodeId id = {address_v4(0), 0, (uint16_t)atoi(argv[1])};
    size_t viewSize = atoll(argv[2]);
    std::cout << "View size: " << viewSize << '\n';
    Array<NodeId> bs(viewSize);
    init(id, argv[3], bs);
    HTTPLogger log(5, "localhost", 8080);
    log.setCallback(on_logger_response);
    std::cout << "Starting basalt..." << '\n';
    using namespace std::chrono_literals;
    basalt_set_logger(&log);
    basalt_init(id, bs, 1s, 5s);
    std::this_thread::sleep_for(2min);
    basalt_stop();
    return 0;
}
