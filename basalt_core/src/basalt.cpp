#include "basalt.hpp"
#include "JOAATHash.hpp"

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

    Hash<4> hashFunc(const NodeId& id, uint32_t seed) {
        byte data[sizeof(id)+4] = {0};
        toLittleEndian(seed, 4, data);
        id.deserialize(data+4);
        return (Hash<4>)JOAATHash(data, sizeof(id)+4);
    }
    void update(){
        std::cout << JOAATHash((byte*)&iterCount, 4) << '\n';
        iterCount++;
    }
    void reset(){
        std::cout << "Reset\n";
    }
    void basalt_init(duration<double> updateDelay, duration<double> resetDelay){
        /* init node here 
        
        */
        // init main update loop
        mainLoop = new LoopedFunction<>(update, updateDelay);
        resetLoop = new LoopedFunction<>(reset, resetDelay);
    }
    void basalt_stop(){
        mainLoop->finish();
        resetLoop->finish();
        delete mainLoop;
        delete resetLoop;
        // delete node;
    }
} // namespace Basalt
