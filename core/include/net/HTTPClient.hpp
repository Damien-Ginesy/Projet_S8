#pragma once
#include <llhttp.h>
#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>
#include <thread>

#define BUF_SIZE    1024

namespace Basalt{
    namespace net
    {

        class HTTPClient
        {
        public:
            struct BufferView
            {
            private:
                const char* _start, *_end;
            public:
                char operator[](size_t i) const { return _start[i]; }
                const char* begin() const { return _start; }
                const char* end() const { return _end; }
                size_t size() const { return _end-_start; }
                BufferView(const char* s, const char* e): _start(s), _end(e) {}
                BufferView(const char* s, size_t size): _start(s), _end(s+size) 
                {}
            };
            HTTPClient(const std::string& host, uint16_t port=80);
            asio::error_code GET(const std::string& uri, void (*)(const llhttp_t&, BufferView));
            asio::error_code POST(const std::string& uri, BufferView body,
                void (*)(const llhttp_t&, BufferView),
                std::string contentType = "text/plain");
            asio::io_context& get_context() { return _ctx; }
            const asio::ip::tcp::endpoint& endpoint() const { return _remoteEp; }
            ~HTTPClient();
        private:
            asio::io_context _ctx;
            asio::ip::tcp::socket _sock;
            std::string _host;
            asio::ip::tcp::endpoint _remoteEp;
            std::string _req;
            std::vector<char> _resp;
            std::thread _t;
            struct {
                char *data; size_t n;
            } _rwContext = {nullptr, 0};
            llhttp_t _parser;
            llhttp_settings_t _settings;
            void (*_on_response)(const llhttp_t&, BufferView) = nullptr;

            static void async_write_n(HTTPClient* cli, size_t n);
            static void async_read_response(HTTPClient* cli);
            static int on_headers_complete(llhttp_t*);
        };
    } // namespace net
    
}

