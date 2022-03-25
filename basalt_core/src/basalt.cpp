#include "basalt.hpp"
#include "SpookyHash.h"
#include <net/basalt_net.hpp>

namespace Basalt
{
    using std::chrono::duration;

    template<typename... Arg_t>
    class LoopedFunction{
        std::thread _thread;
        duration<double> _period;
        void (*_cbk)(Arg_t... args);
        bool _keepGoing = true;

        public:
        LoopedFunction(void (*f)(Arg_t...), duration<double> period, Arg_t... args):
            _cbk(f), _period(period)
        {
            auto threadFunc = [this](Arg_t... args){
                while(this->_keepGoing){
                    std::this_thread::sleep_for(this->_period);
                    this->_cbk(std::forward<Arg_t>(args)...);
                }
            };
            _thread = std::thread(threadFunc, std::forward<Arg_t>(args)...);
        }
        void finish() { _keepGoing = false; }
        ~LoopedFunction() { _thread.join(); }
    };
    Node *node;
    LoopedFunction<> *mainLoop, *resetLoop;

    uint32_t iterCount = 0;

    Hash<16> hashFunc(const NodeId& id, uint32_t seed) {
        byte data[sizeof(id)] = {0};
        id.to_bytes(data);

        return SpookyHash(data, NodeId::dataSize, seed);
    }
    void update(){
        iterCount++;
        node->update();
    }
    void reset(){
        std::cout << "Reset\n";
    }
    void on_pull_req(net::Message& req){
        node->on_pull(req);
    }
    void on_push_req(net::Message& req){
        node->on_push(req);
    }
    void on_pull_resp(net::Message& resp){

    }
    void on_push_resp(net::Message& resp){
        
    }
    void basalt_init(NodeId id, const Array<NodeId>& bs, duration<double> updateDelay, duration<double> resetDelay){
        /* init node here */
        node = new Node(id, bs, 1, hashFunc);
        net::CallbackMap callbacks {
            {net::PULL_REQ, on_pull_req},
            {net::PUSH_REQ, on_push_req},
            {net::PULL_RESP, on_pull_resp},
            {net::PUSH_RESP, on_push_resp}
        };
        using namespace asio::ip;
        tcp::endpoint ep(tcp::v4(), id._port);
        net::net_init(callbacks, ep);
        // init main update loop
        mainLoop = new LoopedFunction<>(update, updateDelay);
        resetLoop = new LoopedFunction<>(reset, resetDelay);
    }
    void basalt_stop(){
        net::net_finish();
        mainLoop->finish();
        resetLoop->finish();
        delete node;
        delete mainLoop;
        delete resetLoop;
    }
} // namespace Basalt
