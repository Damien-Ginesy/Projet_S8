#pragma once
#include "Node.hpp"
#include <chrono>
#include <thread>
#include <HTTPLogger.hpp>

namespace Basalt
{
    extern uint32_t iterCount;

    /* Inits the Basalt RPS service */
    #if IS_BYZANTINE==0
    void basalt_init(NodeId id, const Array<NodeId>& bootstrap, unsigned samplesPerReset,
        std::chrono::duration<double> updateDelay, std::chrono::duration<double> resetDelay);
    #else
    void basalt_init(NodeId id, const Array<NodeId>& bs, Array<NodeId>& friends, 
        unsigned k, std::chrono::duration<double> updateDelay, 
        std::chrono::duration<double> resetDelay);
    #endif
    void basalt_set_logger(Basalt::HTTPLogger* logger);
        
    Hash_t rank(uint32_t id, uint32_t seed);
    
    /* Stops Basalt and clears the internal state */
    void basalt_stop();
} // namespace basalt
