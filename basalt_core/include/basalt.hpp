#pragma once
#include "Node.hpp"
#include <chrono>
#include <thread>

namespace Basalt
{
    extern uint32_t iterCount;

    void basalt_init(std::chrono::duration<double> updateDelay, std::chrono::duration<double> resetDelay);
    void basalt_stop();
} // namespace basalt
