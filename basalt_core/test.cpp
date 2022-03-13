#include <iostream>
#include <basalt.hpp>
#include <SHA256Hash.hpp>
#include <JOAATHash.hpp>
#include <SpookyHash.h>

int main(int argc, char const *argv[])
{
    SHA256Hash h("", 0);
    Hash<4>&& h2 = JOAATHash("The quick brown fox jumps over the lazy dog", 43);
    Hash<16>&& h3 = SpookyHash("The quick brown fox jumps over the lazy dog", 43);
    std::cout << "SHA256('') = " << h << '\n';
    std::cout << "joaat('abc') = " << h2 << '\n';
    std::cout << "SpookyHash('The quick brown fox jumps over the lazy dog') = " << h3 << '\n';
}
