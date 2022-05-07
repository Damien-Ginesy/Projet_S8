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






void urlParser(const char * const_url, char * host, char * route, char * port){
  char url[128];
  strcpy(url, const_url);
  char * f_url = strtok(url,":");
  char * f_port = strtok(NULL ,":");

  char * f_host = strtok(f_url ,"/");
  char * f_route = strtok(NULL,"/");

  if(f_port != NULL) strcpy(port , f_port);
  if(f_host != NULL) strcpy(host , f_host);
  if(f_route != NULL) strcpy(route , f_route);
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


    //Enregistrement aupres du srv bootstrap
    char bs_host[128] = "";
    char bs_route[128] = "/";
    char bs_port[10] = "";
    urlParser(argv[3], bs_host, bs_route, bs_port);

    net::HTTPClient cli(bs_host, atoi(bs_port));
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


    //Recup bootstrap
    do
    {
        cli.GET("/?bs_size=" + std::string(argv[2]), on_GET_resp);
        sem.acquire();
        std::this_thread::sleep_for(1s);
    } while (!bsOK);

    init(bs);

    //Print bootstrap
    /*
    for(const auto& id: bs)
        std::cout << id.to_string() << '\n';
    */


    //Logger HTTP
    char logger_host[128] = "";
    char logger_route[128] = "/";
    char logger_port[10] = "";
    urlParser(argv[4], logger_host, logger_route, logger_port);

    HTTPLogger log(1, logger_host, atoi(logger_port), logger_route);
    log.setCallback(on_logger_response);


    //BASALT
    std::cout << "Starting basalt..." << '\n';
    basalt_set_logger(&log);
    basalt_init(id, bs, (uint32_t)(viewSize>>2),1s, 5s);
    std::this_thread::sleep_for(10min);
    basalt_stop();

    return 0;
}
