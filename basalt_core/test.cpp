#include <iostream>
#include <basalt.hpp>
#include <SHA256Hash.hpp>

int main(int argc, char const *argv[])
{
    SHA256Hash h("abc", 3);
    std::cout << h << '\n';
}
