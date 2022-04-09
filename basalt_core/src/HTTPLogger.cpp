#include <HTTPLogger.hpp>
#include <misc.h>
#include <iostream>

#define CRLF        "\r\n"

namespace Basalt
{
    HTTPLogger::HTTPLogger(size_t bufferSize, const std::string& hostName, uint16_t port,
        const std::string& apiEndpoint): _apiEndpoint(apiEndpoint),
        _cli(hostName, port)
    {
        _start = _cur = new std::string[bufferSize];
        _end = _start + bufferSize;
        using namespace asio::ip;
    }
    void HTTPLogger::setCallback(HTTPLogger::cbk_t callback){
        _on_resp = callback;
    }
    HTTPLogger::~HTTPLogger()
    {
        delete[] _start;
    }
    void HTTPLogger::makeRequest(std::string& out){
        std::stringstream body;
        body << '[';
        for(auto s=_start; s<_cur-1; ++s)
            body << *s << ", ";
        body << *(_cur-1) << "]" CRLF;
        out = body.str();
    }
    HTTPLogger& HTTPLogger::operator<<(const std::string& in){
        *_cur++ = in;
        if(_cur == _end){ // if buffer is full
            std::string req;
            makeRequest(req);
            _cur = _start;
            using namespace asio::ip;
            asio::error_code ec;
            _cli.POST(_apiEndpoint, net::HTTPClient::BufferView(req.data(), req.size()), 
            _on_resp, "application/json");
        }
        return *this;
    }

} // namespace Basalt

