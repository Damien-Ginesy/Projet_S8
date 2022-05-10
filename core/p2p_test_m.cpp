#include <basalt.hpp>
#include <iostream>
#include <chrono>
#include <random>
#include <string.h>

std::stringstream bsStream;
std::stringstream bsmStream;
using namespace std::literals::chrono_literals;
Basalt::NodeId id;

bool bsOK = false;
bool bsmOK = false;

std::binary_semaphore sem(0);

int countLines(std::stringstream& str){
  int l = 0;
  std::string ln;
  while(std::getline(str, ln)) {l++;}
  str.clear();
  str.seekg(0);
  return l;
}

void init(Basalt::Array<Basalt::NodeId>& bs, std::stringstream& str){

    using namespace Basalt;
    using namespace asio::ip;
    size_t i=0;
    while (!str.eof())
    {
        std::string ip;
        uint16_t port;
        uint32_t idVal;
        str >> ip >> port >> idVal;
        if(idVal == 0){std::cout << "================================\n";}
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
    if (bsOK = (parser.status_code == 200))
        for(char c: body) bsStream << c;
    sem.release();
}
void on_GET_respM(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    if (bsmOK = (parser.status_code == 200))
        for(char c: body) bsmStream << c;
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

//bin port viewSize k delay_cycle delay_reset bootstrap_url http_url
//00  1111 22222222 3 44444444444 55555555555 6666666666666 77777777
int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace asio::ip;
    id = {address_v4(0), (uint16_t)atoi(argv[1]), 0};
    size_t viewSize = atoll(argv[2]);

    std::cout << "View size: " << viewSize << '\n';


    //Enregistrement sur le serveur bootstrap
    char bs_host[128] = "";
    char bs_route[128] = "/";
    char bs_port[10] = "";
    urlParser(argv[6], bs_host, bs_route, bs_port);

    net::HTTPClient cli(bs_host, atoi(bs_port));
    std::stringstream s;
    s << "{\"type\":1,\"port\":" << argv[1] << "}\n";
    std::string body = s.str();
    asio::error_code err =
    cli.POST("/log", net::HTTPClient::BufferView(body.data(), body.size()), on_POST_resp, "application/json");
    if(err){
        std::cout << "Couldn't send request: " << err.message() << '\n';
        return EXIT_FAILURE;
    }
    sem.acquire();




    //Recup bootstrap
    do
    {
        cli.GET("/?bs_size=" + std::string(argv[2]), on_GET_resp);
        sem.acquire();
        std::this_thread::sleep_for(1s);
    } while (!bsOK);

    Array<NodeId> bs(viewSize);
    init(bs, bsStream);

    //Recup malicieux
    do
    {
        cli.GET("/m", on_GET_respM);
        sem.acquire();
        std::this_thread::sleep_for(1s);
    } while (!bsmOK);

    int nbM = countLines(bsmStream);
    Array<NodeId> friends(nbM);
    init(friends, bsmStream);


    /*
    std::cout << "=== noeud courant: " << id.to_string() << " ===\n";

    //Print friends
    std::cout << "---------------- friends ----------------\n";
    for(const auto& id: friends)
        std::cout << id.to_string() << "\n";
    std::cout << "-----------------------------------------\n";



    //Print bootstrap
    std::cout << "---------------- bs ----------------\n";
    for(const auto& id: bs)
        std::cout << id.to_string() << '\n';
    std::cout << "------------------------------------\n";
    */

    HTTPLogger* log = NULL;
    if(argc>7){
    //Logger HTTP
    char logger_host[128] = "";
    char logger_route[128] = "/";
    char logger_port[10] = "";
    urlParser(argv[7], logger_host, logger_route, logger_port);

    log = new HTTPLogger(10, logger_host, atoi(logger_port), logger_route);
    log->setCallback(on_logger_response);
    basalt_set_logger(log);
    }

    //BASALT
    std::cout << "Starting basalt..." << '\n';
    basalt_init(id, bs, friends, atoi(argv[3]),std::chrono::milliseconds(atoi(argv[4])), std::chrono::milliseconds(atoi(argv[5])));
    std::this_thread::sleep_for(10min);
    basalt_stop();

    if(log != NULL) delete(log);

    return 0;
}
