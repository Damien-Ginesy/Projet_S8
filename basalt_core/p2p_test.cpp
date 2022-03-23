#include <thread>
#include <iostream>
#include <mutex>
#include <time.h>
#include <random>
#include <net/basalt_net.hpp>

using namespace asio::ip;

std::vector<uint16_t> peers; // local view, containing listening ports of other known peers
uint16_t port; // our listening port
std::mutex m;
std::random_device rng;
std::atomic_bool keepGoing = true;

// adds candidate to the view if it wasn't in there yet
void update_peers(uint16_t candidate){
    bool isKnown = false;
    for(uint16_t p: peers) isKnown |= (p==candidate);
    if(!isKnown) peers.push_back(candidate);
}
void on_push_req(Basalt::net::Message& req){
    uint16_t p;
    req >> p;
    std::cout << "PUSH from " << p << '\n';
    update_peers(p);
    if(req.payloadSize()){
        req >> p;
        update_peers(p);
    }
    req.set_type(Basalt::net::PUSH_RESP);
}
void on_push_resp(Basalt::net::Message& resp){
    resp.set_type(Basalt::net::SESSION_END);
}
size_t selectPeer(){
    std::uniform_int_distribution<size_t> dist(0, peers.size() - 1);
    return dist(rng);
}
// the main loop of the program
void loop_cbk(){
    std::cout << "Entering main loop..." << '\n';
    while (keepGoing)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        address_v4 addr(0x7F000001);
        Basalt::net::Message msg(Basalt::net::PUSH_REQ);
        msg << port;
        size_t p, q;
        switch (peers.size())
        {
        case 0: // if know nobody, do nothing
            break;
        case 1:
            p = selectPeer();
            std::cout << "PUSH to " << peers[p] << '\n';            
            Basalt::net::send_request(tcp::endpoint(addr, peers[p]), msg);
            break;
        default: 
            p = selectPeer(); q = selectPeer();
            std::cout << "PUSH to " << peers[p] << '\n';
            msg << peers[q];
            Basalt::net::send_request(tcp::endpoint(addr, peers[p]), msg);
            break;
        }
    }
}
int main(int argc, char const *argv[])
{
    if(argc < 2){
        std::cerr << "Usage: ./p2p_test <port> [bootstrap...]" << std::endl;
        return EXIT_FAILURE;
    }
    for(size_t i=2; i<argc; ++i) peers.push_back(atoi(argv[i]));
    port = std::atoi(argv[1]); // read the listening port from the first argument
    tcp::endpoint localEp(tcp::v4(), port); // the local endpoint to listen on
    using namespace Basalt;
    net::CallbackMap callbacks {{net::PUSH_REQ, on_push_req}, 
            {net::PUSH_RESP, on_push_resp}};
    net::net_init(callbacks, localEp);
    std::cout << "Listening on " << localEp << '\n';
    std::thread mainLoop(loop_cbk);
    std::this_thread::sleep_for(std::chrono::seconds(60));
    keepGoing = false;
    mainLoop.join();
    net::net_finish();
    std::cout << "Stopping now" << '\n';
    return 0;
}