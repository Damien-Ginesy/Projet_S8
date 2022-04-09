#pragma once
#include <asio/ip/tcp.hpp>
#include <exception>
#include <sstream>
#include <llhttp.h>
#include <net/HTTPClient.hpp>

#define BUFFER_SIZE 1024

namespace Basalt{
    class HTTPLogger
    {
    public: using cbk_t = void (*)(const llhttp_t&, net::HTTPClient::BufferView);
    private:
        net::HTTPClient _cli;
        std::string *_start=nullptr, *_cur,*_end;
        std::string _apiEndpoint = "/";
        cbk_t _on_resp = nullptr;
        void makeRequest(std::string& out);
    public:
        HTTPLogger(size_t bufferSize, const std::string& hostName, uint16_t port = 80,
            const std::string& apiEndpoint = "/");
        const asio::ip::tcp::endpoint& endpoint() const { return _cli.endpoint(); }
        HTTPLogger& operator<<(const std::string& in);
        void setCallback(cbk_t callback);
        ~HTTPLogger();
    };
}