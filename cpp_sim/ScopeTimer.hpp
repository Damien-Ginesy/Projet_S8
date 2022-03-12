#pragma once
#include <chrono>

template<class duration_t>
class ScopeTimer
{
    using clock_t = std::chrono::high_resolution_clock;
private:
    std::chrono::time_point<clock_t> _start;
    duration_t* _d;
public:
    ScopeTimer(duration_t* output): _d(output){
        _start = clock_t::now();
    }
    ~ScopeTimer(){
        *_d = std::chrono::duration_cast<duration_t>(clock_t::now()-_start);
    }
};