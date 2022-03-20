#include <asio.hpp>
#include <NodeId.hpp>
#include <net/Message.hpp>
#include <thread>
#include <list>


int main(int argc, char const *argv[])
{
    std::list<int> L;
    L.push_back(0);
    auto i = L.begin();
    L.push_front(1);
    auto j = L.begin();
    L.erase(j);

    printf("%d\n", *i);
    return 0;
}
