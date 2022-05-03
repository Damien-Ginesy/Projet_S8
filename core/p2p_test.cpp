#include <basalt.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <random>

std::stringstream bsStream;
Basalt::NodeId id;
bool bsOK = false;
std::binary_semaphore sem(0);

void init(Basalt::Array<Basalt::NodeId>& bs){
    
    using namespace Basalt;
    using namespace asio::ip;
    size_t i=0;
    while (!bsStream.eof())
    {
        std::string ip;
        uint16_t port;
        uint32_t idVal;
        bsStream >> ip >> port >> idVal;
        if(ip.empty()) continue;
        bs[i++] = NodeId { make_address_v4(ip), port, idVal};
    }
}
void on_logger_response(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    std::cout << "[LOGGER] Received code " << parser.status_code << '\n';
}
void on_POST_resp(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    std::stringstream s;
    using namespace asio::ip;
    std::cout << parser.status_code << '\n';
    for(char c: body) s << c;
    std::string ip;
    uint32_t idVal;
    s >> ip >> idVal;
    id._addr = make_address_v4(ip);
    id.id = idVal;

    sem.release();
}
void on_GET_resp(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    if(bsOK = (parser.status_code == 200))
        for(char c: body) bsStream << c;
    sem.release();
}



int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    id = {address_v4(0), (uint16_t)atoi(argv[1]), 0};
    size_t viewSize = atoll(argv[2]);

    std::cout << "View size: " << viewSize << '\n';
    Array<NodeId> bs(viewSize);

    net::HTTPClient cli(std::string(argv[3]), 8080);
    std::stringstream s;
    s << "{\"type\":0,\"port\":" << argv[1] << "}\n";
    std::string body = s.str();

    cli.POST("/log", net::HTTPClient::BufferView(body.data(), body.size()), on_POST_resp, "application/json");
    sem.acquire();
    std::cout << id.to_string() << "\n=================\n";

    do
    {
        cli.GET("/", on_GET_resp);
        sem.acquire();
    } while (!bsOK);

    init(bs);

    for(const auto& id: bs)
        std::cout << id.to_string() << '\n';

    
    return 0;
}
