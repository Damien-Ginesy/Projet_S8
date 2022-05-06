#include <basalt.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <string.h>

std::stringstream bsStream;
using namespace std::literals::chrono_literals;

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


//bin port viewSize bootstrap_url
int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    id = {address_v4(0), (uint16_t)atoi(argv[1]), 0};
    size_t viewSize = atoll(argv[2]);

    std::cout << "View size: " << viewSize << '\n';
    Array<NodeId> bs(viewSize);

    char bs_url[64];
    strcpy(bs_url, argv[3]);
    char * bs_domain = strtok(bs_url,":");
    char * bs_port = strtok(NULL,":");

    net::HTTPClient cli(bs_domain, atoi(bs_port));
    std::stringstream s;
    s << "{\"type\":0,\"port\":" << argv[1] << "}\n";
    std::string body = s.str();

    asio::error_code err =
    cli.POST("/log", net::HTTPClient::BufferView(body.data(), body.size()), on_POST_resp, "application/json");
    if(err){
        std::cout << "Couldn't send request: " << err.message() << '\n';
        return EXIT_FAILURE;
    }
    sem.acquire();
    std::cout << id.to_string() << "\n=================\n";

    do
    {
        cli.GET("/?bs_size=" + std::string(argv[2]), on_GET_resp);
        sem.acquire();
        std::this_thread::sleep_for(1s);
    } while (!bsOK);

    init(bs);

    for(const auto& id: bs)
        std::cout << id.to_string() << '\n';



    char logger_url[64];
    strcpy(logger_url, argv[4]);
    char * logger_domain = strtok(logger_url,":");
    char * logger_port = strtok(NULL,":");

    HTTPLogger log(5, logger_domain, atoi(logger_port),"/infoNoeud");
    log.setCallback(on_logger_response);
    std::cout << "Starting basalt..." << '\n';
    basalt_set_logger(&log);
    basalt_init(id, bs, (uint32_t)(viewSize>>1),1s, 5s);
    std::this_thread::sleep_for(2min);
    basalt_stop();

    return 0;
}
