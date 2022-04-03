#include <HTTPLogger.hpp>
#include <misc.h>
#include <iostream>

#define CRLF        "\r\n"

namespace Basalt
{
    HTTPLogger::HTTPLogger(size_t bufferSize, asio::io_context& ctx, const std::string& hostName, uint16_t port,
        const std::string& apiEndpoint):
        _sock(asio::ip::tcp::socket(ctx)), _apiEndpoint(apiEndpoint)
    {
        _start = _cur = new std::string[bufferSize];
        _end = _start + bufferSize;
        using namespace asio::ip;
        tcp::resolver resolver(ctx);
        asio::error_code ec;
        char portString[8] = {};
        sprintf(portString, "%u", port);
        tcp::resolver::results_type res = resolver.resolve(hostName, portString, ec);
        if(ec) throw std::runtime_error(ec.message());
        _remote = res.begin()->endpoint();
        llhttp_settings_init(&_settings);
    }
    void HTTPLogger::setCallback(llhttp_cb callback){
        _callback = callback;
    }
    HTTPLogger::~HTTPLogger()
    {
        delete[] _start;
    }
    void HTTPLogger::makeRequest(std::string& out){
        std::stringstream header;
        header << "POST " << _apiEndpoint << " HTTP/1.1" CRLF
        "Content-Type: application/json" CRLF
        "Connection: close" CRLF;
        std::stringstream body;
        body << '[';
        for(auto s=_start; s<_cur-1; ++s)
            body << *s << ", ";
        body << *(_cur-1) << "]" CRLF;
        size_t contentLength = body.str().size();
        header << "Content-Length: " << contentLength << CRLF CRLF;
        header << body.str();
        out = header.str();
    }
    void HTTPLogger::async_read_n(size_t n,HTTPLogger* logger){
        auto handler = [&](asio::error_code ec, size_t nRead){
            if(!ec && logger->_nRead<n){
                logger->async_read_n(n, logger);
            }
            else if(ec.value()==2 || logger->_nRead == n){
                logger->_settings.on_headers_complete = logger->_callback;
                llhttp_init(&logger->_parser, HTTP_RESPONSE, &logger->_settings);
                llhttp_execute(&logger->_parser, (char*)logger->_buffer, logger->_nRead);
                logger->_sock.close();
            }
        };
        logger->_sock.async_read_some(asio::buffer(logger->_buffer+logger->_nRead, n-logger->_nRead), handler);
    }
    HTTPLogger& HTTPLogger::operator<<(const std::string& in){
        *_cur++ = in;
        if(_cur == _end){ // if buffer is full
            std::string req;
            makeRequest(req);
            _cur = _start;
            using namespace asio::ip;
            asio::error_code ec;
            if(_sock.is_open()){
                _sock.cancel();
                _sock.close();
            }
            _settings.on_headers_complete = _callback;
            llhttp_init(&_parser, HTTP_RESPONSE, &_settings);
            _sock.connect(_remote, ec);
            if(ec) throw std::runtime_error(ec.message());
            ec = write_n(_sock, req.size(), (uint8_t*)req.data());
            if(ec) throw std::runtime_error(ec.message());
            _nRead = 0;
            memset(_buffer, 0, BUFFER_SIZE);
            ec = read_n(_sock, BUFFER_SIZE, _buffer);
            llhttp_execute(&_parser, (char*)_buffer, BUFFER_SIZE);
        }
        return *this;
    }

} // namespace Basalt

