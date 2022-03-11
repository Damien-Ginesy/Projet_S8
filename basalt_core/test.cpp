#include <iostream>
#include <basalt.hpp>

int i=0;
int main(int argc, char const *argv[])
{
    using namespace std::chrono_literals;
    Basalt::basalt_init(1s, 1s);
    std::this_thread::sleep_for(3s);
    Basalt::basalt_stop();
    return 0;
}
