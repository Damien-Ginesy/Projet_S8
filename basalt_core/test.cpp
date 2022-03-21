#include <iostream>
#include <basalt.hpp>
#include <SHA256Hash.hpp>
#include <JOAATHash.hpp>
#include <SpookyHash.h>
#include <random>

void a(){
    throw "error";
}
void b(){ a(); }

int main(int argc, char const *argv[])
{
    try
    {
        b();
    }
    catch(const char* e)
    {
        std::cerr << e << '\n';
    }
    
}
