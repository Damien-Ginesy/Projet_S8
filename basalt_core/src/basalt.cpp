#include "basalt.hpp"
#include "SpookyHash.h"
#include <net/basalt_net.hpp>
#include <asio/steady_timer.hpp>

namespace Basalt
{
    using std::chrono::duration;
    class LoopedFunction
    {
        asio::chrono::milliseconds delay;
        void (*cbk)();
        asio::steady_timer t;
        bool stopped = false;
        static void async_run(LoopedFunction* func){
            auto handler = [=](asio::error_code ec){
                if(!ec){
                    func->cbk();
                    async_run(func);
                }
            };
            func->t.expires_at(asio::chrono::steady_clock::now() + func->delay);
            func->t.async_wait(handler);
        }
    public:
        LoopedFunction(asio::io_context& ctx, asio::chrono::milliseconds delay, void (*cbk)()):
            cbk(cbk), delay(delay), t(asio::steady_timer(ctx))
        {
            async_run(this);
        }
        void stop(){
            stopped = true;
            t.cancel();
        }
        ~LoopedFunction() {
            if(!stopped) stop();
        }
    };

    static asio::io_context ctx;
    Node *node;
    LoopedFunction *mainLoop, *resetLoop;
    static std::thread runner;

    uint32_t iterCount = 0;

    Hash<16> hashFunc(const NodeId& id, uint32_t seed) {
        byte data[NodeId::dataSize] = {0};
        id.to_bytes(data);

        return SpookyHash(data, NodeId::dataSize, seed);
    }
    void update(){
        iterCount++;
        node->update();
    }
    void reset(){
        node->reset();
    }
    // message handlers
    void on_pull_req(net::Message& req){
        node->on_pull_req(req);
    }
    void on_push_req(net::Message& req){
        node->on_push_req(req);
    }
    void on_pull_resp(net::Message& resp){
        node->on_pull_resp(resp);
    }
    void on_push_resp(net::Message& resp){
        resp.set_type(net::SESSION_END);
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
        using namespace asio::chrono;

        mainLoop = new LoopedFunction(ctx, duration_cast<milliseconds>(updateDelay), update);
        resetLoop = new LoopedFunction(ctx, duration_cast<milliseconds>(resetDelay), reset);
        runner = std::thread([](){ ctx.run(); });
    }
    void basalt_stop(){
        net::net_finish();
        mainLoop->stop();
        resetLoop->stop();
        ctx.stop();
        runner.join();
        delete mainLoop;
        delete resetLoop;
        delete node;
    }
} // namespace Basalt
