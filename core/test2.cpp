#include <basalt.hpp>
#include <semaphore>

std::binary_semaphore sem(0);

void on_logger_resp(const llhttp_t& parser, Basalt::net::HTTPClient::BufferView body){
    std::cout << "Code: " << parser.status_code << '\n';
    for(char c: body)
        std::cout << c;
    sem.release();
}

int main(int argc, char const *argv[])
{
    using namespace Basalt;
    using namespace std::chrono_literals;

    net::HTTPClient cli("example.org");

    cli.GET("/", on_logger_resp);
    sem.acquire();

    return 0;
}
