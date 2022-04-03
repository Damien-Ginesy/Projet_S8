#pragma once
#include <asio/ip/tcp.hpp>
#include <exception>
#include <sstream>
#include <llhttp.h>

#define BUFFER_SIZE 1024


namespace Basalt{
    class HTTPLogger
    {
    private:
        asio::ip::tcp::endpoint _remote;
        std::string *_start=nullptr, *_cur,*_end;
        std::string _apiEndpoint = "/";
        asio::ip::tcp::socket _sock;
        size_t _nRead = 0;
        uint8_t _buffer[BUFFER_SIZE] = {};
        llhttp_t _parser;
        llhttp_cb _callback = nullptr;
        llhttp_settings_t _settings;
        
        void makeRequest(std::string& out);
        static void async_read_n(size_t, HTTPLogger*);
    public:
        HTTPLogger(size_t bufferSize, asio::io_context& ctx, const std::string& hostName, uint16_t port = 80,
            const std::string& apiEndpoint = "/");
        const asio::ip::tcp::endpoint& endpoint() const { return _remote; }
        HTTPLogger& operator<<(const std::string& in);
        void setCallback(llhttp_cb callback);
        ~HTTPLogger();
    };
}