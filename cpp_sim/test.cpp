#include <iostream>
#include "Node.hpp"
#include "joaat_hash.h"

bool R(uint32_t seed, uint32_t i1, uint32_t i2){
    struct input_t{ uint32_t seed, id; };
    input_t x{seed, i1}, y{seed, i2};
    return joaat_hash(&x, sizeof(x)) < joaat_hash(&y, sizeof(y));
}
rps::Node foo() { 
    rps::Node ret(3, R);
    for(int i=0; i<3; i++)
        std::cout << ret.seeds(i) << ' ';
    std::cout << std::endl; 
    return ret;
}

int main(int argc, char const *argv[])
{
    rps::Node n(foo());
    for(int i=0; i<3; i++)
        std::cout << n.seeds(i) << ' ';
    std::cout << std::endl; 
}
