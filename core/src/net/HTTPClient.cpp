#include <net/HTTPClient.hpp>
#include <exception>
#include <iostream>

using namespace asio::ip;
#define CRLF    "\r\n"
#define BUF_SIZE    1024


namespace Basalt
{
    namespace net
    {
        
        HTTPClient::HTTPClient(const std::string& host, uint16_t port):
            _sock(_ctx), _host(host)
        {
            asio::error_code ec;
            tcp::resolver resolver(_ctx);
            auto res = resolver.resolve(host, std::to_string(port), ec);
            if(ec) throw std::runtime_error(ec.message());
            _remoteEp = res.begin()->endpoint();

            std::memset(&_settings, 0, sizeof(_settings));
            _settings.on_headers_complete = on_headers_complete;
            _t = std::thread([this]() {
                auto guard = asio::make_work_guard(this->_ctx);
                this->_ctx.run();
            });
        }
        static size_t write_n(tcp::socket& sock, const char* buffer, size_t n, asio::error_code& ec){
            size_t nWritten = 0;
            while(!ec && nWritten < n)
                nWritten += sock.write_some(asio::buffer(buffer+nWritten, n-nWritten), ec);
            return nWritten;
        }
        void HTTPClient::async_read_response(HTTPClient* cli){
            auto handler = [=](asio::error_code ec, size_t nRead){
                cli->_rwContext.n += nRead;
                if(ec == asio::error::eof){
                    cli->_sock.close(); 
                    llhttp_init(&cli->_parser, HTTP_RESPONSE, &cli->_settings, cli);
                    llhttp_execute(&cli->_parser, cli->_resp.data(), cli->_rwContext.n);
                    return;
                }
                cli->_rwContext.data += nRead;
                if(!ec) cli->async_read_response(cli);
            };
            if(cli->_rwContext.n == cli->_resp.size())
            {
                cli->_resp.resize(cli->_resp.size()+BUF_SIZE);
                cli->_rwContext.data = cli->_resp.data()+cli->_rwContext.n;
            }
            size_t maxRead = cli->_resp.size()-cli->_rwContext.n;
            cli->_sock.async_read_some(
                asio::buffer(cli->_rwContext.data, maxRead), handler);
        }
        int HTTPClient::on_headers_complete(llhttp_t* parser){
            HTTPClient* cli = (HTTPClient*)parser->arg;
            char *bodyStart = strstr(cli->_resp.data(), (CRLF CRLF)) + 4;
            if(cli->_on_response)
                cli->_on_response(*parser, BufferView(bodyStart, cli->_rwContext.data));
            return 1;
        }

        asio::error_code HTTPClient::GET(const std::string& uri, void (*cbk)(const llhttp_t&, BufferView)){
            _req = "GET " + uri + " HTTP/1.1" CRLF
            "Host: " + _host + CRLF
            "Connection: close" CRLF
            CRLF;
            asio::error_code ec;
            _sock.connect(_remoteEp, ec);
            if(ec) return ec;
            if(ec) return ec;

            _rwContext = {_resp.data(), 0};
            _on_response = cbk;
            write_n(_sock, _req.data(), _req.size(), ec);
            async_read_response(this);
            return ec;
        }
        asio::error_code HTTPClient::POST(const std::string& uri, BufferView body,
        void (*cbk)(const llhttp_t&, BufferView), std::string contentType){
            std::stringstream s;
            s << "POST " << uri << " HTTP/1.1" CRLF
                "Host: " << _host << CRLF
                "Content-Type: " << contentType << CRLF
                "Content-Length: " << body.size() << CRLF
                "Connection: close" CRLF
                CRLF;
            _req = s.str();
            asio::error_code ec;
            _sock.connect(_remoteEp, ec);
            if(ec) return ec;
            if(ec) return ec;

            _rwContext = {_resp.data(), 0};
            _on_response = cbk;
            write_n(_sock, _req.data(), _req.size(), ec);
            write_n(_sock, body.begin(), body.size(), ec);
            async_read_response(this);
            return ec;
        }

        HTTPClient::~HTTPClient() { 
            if(_sock.is_open())
            {
                _sock.cancel();
                _sock.close();
            }
            _ctx.stop();
            _t.join();
        }
    } // namespace net
    
} // namespace Basalt

