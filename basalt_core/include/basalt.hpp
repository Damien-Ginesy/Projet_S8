#pragma once
#include "Node.hpp"
#include <chrono>
#include <thread>

namespace Basalt
{
    extern uint32_t iterCount;

    /* Inits the Basalt RPS service */
    void basalt_init(NodeId id, const Array<NodeId>& bootstrap, 
        std::chrono::duration<double> updateDelay, std::chrono::duration<double> resetDelay);
    /* Stops Basalt and clears the internal state */
    void basalt_stop();
} // namespace basalt
