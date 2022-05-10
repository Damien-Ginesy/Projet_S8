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
    int delayLogStart;

    Hash_t rank(uint32_t id, uint32_t seed) {
        return Hash_t {
            hash64(id & 0xff000000, seed),
            hash64(id & 0xffff0000, seed),
            hash64(id & 0xffffff00, seed),
            hash64(id, seed),
        };
    }
    void update(){
        std::lock_guard guard(mutex);
        node->update();
        if(logger)
        {
            try
            {
                if(node->iter() > delayLogStart)
                  *logger << node->to_string();
            }
            catch(const asio::error_code& ec)
            {
                std::cerr << "[LOGGER ERROR] " << ec.message() << '\n';
            }
        }
    }
    #if IS_BYZANTINE==0
    void reset(){
        std::lock_guard guard(mutex);
        node->reset();
    }
    #endif
    // message handlers
    void on_pull_req(asio::ip::tcp::endpoint sender, net::Message& req){
        std::cout << "Pull from " << sender.address() << ':' << sender.port() << '\n';
        using namespace asio::ip;
        std::lock_guard guard(mutex);
        node->on_pull_req(req);
    }
    void on_push_resp(asio::ip::tcp::endpoint sender, net::Message& resp){
        resp.set_type(net::SESSION_END);
    }
    #if IS_BYZANTINE==0
    void on_push_req(asio::ip::tcp::endpoint sender, net::Message& req){
        std::lock_guard guard(mutex);
        std::cout << "Push from " << sender.address() << ':' << sender.port() << '\n';

        node->on_push_req(req, sender);
    }
    void on_pull_resp(asio::ip::tcp::endpoint sender, net::Message& resp){
        std::lock_guard guard(mutex);
        node->on_pull_resp(resp, sender);
    }
    #endif

    #if IS_BYZANTINE==0
    void basalt_init(NodeId id, const Array<NodeId>& bs, unsigned k, duration<double> updateDelay, duration<double> resetDelay){
        /* init node here */
        node = new Node(id, bs, k, rank);

        net::CallbackMap callbacks {
            {net::PULL_REQ, on_pull_req},
            {net::PUSH_REQ, on_push_req},
            {net::PULL_RESP, on_pull_resp},
            {net::PUSH_RESP, on_push_resp}
        };
    #else
    void basalt_init(NodeId id, const Array<NodeId>& bs, Array<NodeId>& friends,
            unsigned k, duration<double> updateDelay, duration<double> resetDelay){
        node = new Node(id, bs, friends);
        net::CallbackMap callbacks {
            {net::PULL_REQ, on_pull_req},
            {net::PUSH_RESP, on_push_resp},
            {net::PUSH_REQ, on_push_resp}, // respond with a SESSION_END
        };
    #endif
        using namespace asio::ip;
        tcp::endpoint ep(tcp::v4(), id._port);
        net::net_init(callbacks, ep);
        // init main update loop
        using namespace asio::chrono;

        mainLoop = new LoopedFunction(ctx, duration_cast<milliseconds>(updateDelay), update);
        #if IS_BYZANTINE==0
        resetLoop = new LoopedFunction(ctx, duration_cast<milliseconds>(resetDelay), reset);
        #endif
        runner = std::thread([](){ ctx.run(); });
    }
    void basalt_set_logger(size_t bufferSize, const std::string& hostname, uint16_t port, const std::string& apiEndpoint,
            HTTPLogger::cbk_t callback)
    {

        logger = new HTTPLogger(bufferSize, hostname, port, apiEndpoint);
        logger->setCallback(callback);
        delayLogStart = rand() % bufferSize;
    }
    void basalt_stop(){
        net::net_finish();
        mainLoop->stop();
        resetLoop->stop();
        ctx.stop();
        runner.join();
        delete mainLoop;
        delete resetLoop;
        delete logger;
        delete node;
    }
} // namespace Basalt
