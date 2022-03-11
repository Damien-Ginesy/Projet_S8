#include "basalt.hpp"

namespace Basalt
{
    template<typename... Arg_t>
    class LoopedFunction{
        std::thread _thread;
        std::chrono::duration<double> _period;
        void (*_cbk)(Arg_t... args);
        bool _keepGoing = true;

        public:
        LoopedFunction(void (*f)(Arg_t...), std::chrono::duration<double> period, Arg_t... args):
            _cbk(f), _period(period)
        {
            auto threadFunc = [this](Arg_t... args){
                while(this->_keepGoing){
                    this->_cbk(std::forward<Arg_t>(args)...);
                    std::this_thread::sleep_for(this->_period);
                }
            };
            _thread = std::thread(threadFunc, std::forward<Arg_t>(args)...);
        }
        void finish() { _keepGoing = false; }
        ~LoopedFunction() { _thread.join(); }
    };

    static bool keepGoing = true;
    uint32_t iterCount = 0;
    LoopedFunction<> *loop;


    void basalt_init(std::chrono::duration<double> updateDelay, std::chrono::duration<double> resetDelay){
        void (*foo)() = [](){ std::cout << iterCount++ << std::endl; };
        loop = new LoopedFunction(foo, updateDelay);
    }
    void basalt_stop(){
        loop->finish();
        delete loop;
    }
} // namespace Basalt
