#include <basalt.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include <stdio.h>
#include <random>
#include <semaphore>

std::binary_semaphore sem(0);
stringstream fichier_bs;

void init(Basalt::NodeId& id,Basalt::Array<Basalt::NodeId>& bs){

    using namespace Basalt;
    using namespace asio::ip;
    std::vector<NodeId> ids;
    while (!fichier_bs.eof())
    {
        std::string ip;
        uint16_t port, idVal;
        fichier_bs >> ip >> port >> idVal;
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
    std::random_device rng;
    std::sample(ids.begin(), ids.end(), bs.begin(), bs.size(), rng);
}
void on_logger_response(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    std::cout << "[LOGGER] Received code " << parser.status_code << '\n';
}

void on_POST_response(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    stringstream vId;
    std::cout << "[LOGGER] Received code " << parser.status_code << '\n';
    for(char c: body)
        vId << c;
    sem.release();
}



void on_GET_response(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    std::cout << "Code: " << parser.status_code << '\n';
    for(char c: body)
        fichier_bs << c;
    sem.release();
}

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    using namespace std::chrono_literals;

    net::HTTPClient cli(argv[3]);
    std::string req;
    std::stringstream body;
    body << "{\"type\":0,\"port\":" << argv[1] << "}" CRLF;
    req = body.str();
    cli.POST("/log", net::HTTPClient::BufferView(req.data(), req.size()),on_POST_response,"application/json");
    sem.acquire();

    std::this_thread::sleep_for(2s);

    cli.GET("/", on_GET_response);
    sem.acquire();

    NodeId id = {address_v4(0), 0, (uint16_t)atoi(argv[1])};
    size_t viewSize = atoll(argv[2]);
    std::cout << "View size: " << viewSize << '\n';
    Array<NodeId> bs(viewSize);
    init(id, bs);
    HTTPLogger log(5, "localhost", 8080);
    log.setCallback(on_logger_response);
    std::cout << "Starting basalt..." << '\n';
    basalt_set_logger(&log);
    basalt_init(id, bs, (uint32_t)(viewSize>>1),1s, 5s);
    std::this_thread::sleep_for(2min);
    basalt_stop();
    return 0;
}
