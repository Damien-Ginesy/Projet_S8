#include <iostream>
#include <basalt.hpp>
#include <JOAATHash.hpp>

int i=0;
int main(int argc, char const *argv[])
{
    Hash<4>&& h1 = JOAATHash("a", 1);
    Hash<4>&& h2 = JOAATHash("Hello", 5);

    std::cout << h1 << ' ' << h2 << '\n';
    std::cout << (h1 < h2) << '\n';
    return 0;
}
