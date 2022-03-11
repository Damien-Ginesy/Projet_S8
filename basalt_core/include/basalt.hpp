#pragma once
#include "Node.hpp"
#include <chrono>


namespace basalt
{
    extern uint32_t iterCount;
    void basalt_init(std::chrono::duration<double> updateDelay, std::chrono::duration<double> resetDelay);
} // namespace basalt
