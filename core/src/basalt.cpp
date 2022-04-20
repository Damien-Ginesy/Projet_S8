#include "basalt.hpp"
#include <hash64.h>
#include <net/basalt_net.hpp>
#include <asio/steady_timer.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/ip/address_v4.hpp>
#include <misc.h>

namespace Basalt
{
    using std::chrono::duration;
    class LoopedFunction
    {
        asio::chrono::milliseconds delay;
        void (*cbk)();
        asio::steady_timer t;
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
            this->~LoopedFunction();
        }
        ~LoopedFunction() {
            t.cancel();
        }
    };

    static asio::io_context ctx;
    Node *node;
    LoopedFunction *mainLoop, *resetLoop;
    static std::thread runner;
    std::mutex mutex;
    HTTPLogger *logger = nullptr;

    Hash_t rank(uint32_t id, uint32_t seed) {
        return hash64(id, seed);
    }
    void update(){
        std::lock_guard guard(mutex);
        node->update();
        if(logger)
        {
            try
            {
                *logger << node->to_string();
            }
            catch(const asio::error_code& ec)
            {
                std::cerr << "[LOGGER ERROR] " << ec.message() << '\n';
            }
        }
    }
    void reset(){
        std::lock_guard guard(mutex);
        node->reset();
    }
    // message handlers
    void on_pull_req(asio::ip::tcp::endpoint sender, net::Message& req){
        using namespace asio::ip;
        std::lock_guard guard(mutex);
        node->on_pull_req(req);
    }
    void on_push_req(asio::ip::tcp::endpoint sender, net::Message& req){
        std::lock_guard guard(mutex);
        node->on_push_req(req);
    }
    void on_pull_resp(asio::ip::tcp::endpoint sender, net::Message& resp){
        std::lock_guard guard(mutex);
        node->on_pull_resp(resp);
    }
    void on_push_resp(asio::ip::tcp::endpoint sender, net::Message& resp){
        resp.set_type(net::SESSION_END);
    }

    void basalt_init(NodeId id, const Array<NodeId>& bs, duration<double> updateDelay, duration<double> resetDelay){
        /* init node here */
        node = new Node(id, bs, (uint32_t)bs.size()>>1, rank);
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
    void basalt_set_logger(HTTPLogger* log){
        logger = log;
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
