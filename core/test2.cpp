#include <basalt.hpp>
#include <semaphore>


int main(int argc, char const *argv[])
{
    using namespace Basalt;

    std::cout << (Hash_t {1, 1, 1, 1} < Hash_t{1, 1, 1, 0}) << '\n';
    std::cout << (Hash_t {1, 1, 1, 1} < Hash_t{1, 1, 1, 1}) << '\n';
    std::cout << (Hash_t {1, 1, 1, 1} == Hash_t{1, 1, 1, 1}) << '\n';
    std::cout << (Hash_t {1, 1, 1, 1} < Hash_t{2, 1, 1, 1}) << '\n';
    return 0;
}
