#include <net/Message.hpp>

namespace Basalt
{
    namespace net
    {
        using namespace asio::ip;
        static asio::error_code read_n(tcp::socket& sock, size_t n, uint8_t* out){
            uint32_t nRead=0;
            asio::error_code ec;
            while(nRead<n){
                sock.wait(sock.wait_read);
                nRead += sock.read_some(asio::buffer(out+nRead, n-nRead), ec);
                if(ec) break;
            }
            return ec;
        }
        static asio::error_code write_n(tcp::socket& sock, size_t n, const uint8_t* in){
            uint32_t nWritten=0;
            asio::error_code ec;
            while(nWritten<n){
                sock.wait(sock.wait_write);
                nWritten += sock.write_some(asio::buffer(in+nWritten, n-nWritten), ec);
                if(ec) break;
            }
            return ec;
        }
        Message::Message() { _header.size=0; }
        Message::Message(MessageType t): _header({t, 0}) {}
        Message& Message::operator<<(uint8_t b){
            _payload.push_back(b);
            ++_header.size;
            return *this;
        }
        Message& Message::operator<<(uint16_t x) {
            *this << (uint8_t)(x&0xff) << (uint8_t)(x>>8);
            return *this;
        }
        Message& Message::operator<<(uint32_t x) {
            *this << (uint16_t)(x&0xffff) << (uint16_t)(x>>16);
            return *this;
        }
        Message& Message::operator<<(uint64_t x) {
            *this << (uint32_t)(x&0xffffffff) << (uint32_t)(x>>32);
            return *this;
        }
        Message& Message::operator>>(uint8_t& out) {
            out = _payload[--_header.size];
            _payload.pop_back();
            return *this;
        }
        Message& Message::operator>>(uint16_t& out) {
            uint8_t a, b;
            *this >> a >> b;
            out = ((uint16_t)a << 8) | b;
            return *this;
        }
        Message& Message::operator>>(uint32_t& out) {
            uint16_t a, b;
            *this >> a >> b;
            out = ((uint32_t)a << 16) | b;
            return *this;
        }
        Message& Message::operator>>(uint64_t& out) {
            uint32_t a, b;
            *this >> a >> b;
            out = ((uint64_t)a << 32) | b;
            return *this;
        }
        void Message::operator>>(char* out){
            std::memcpy(out, _payload.data(), _header.size);
            _payload.resize(_header.size = 0);
        }
        void Message::operator>>(std::string& out){
            out.resize(_header.size);
            memcpy(out.data(), _payload.data(), _header.size);
            _payload.resize(_header.size = 0);
        }
        Message& Message::operator<<(tcp::socket& sock){
            asio::error_code ec;
            uint8_t buffer[sizeof(Header)];
            if(ec = read_n(sock, Header::dataSize, buffer)) throw ec;
            _header = Header::fromBytes(buffer);
            _payload.resize(_header.size);
            if(ec = read_n(sock, _header.size, _payload.data()))
                throw ec;
            return *this;
        }
        Message& Message::operator<<(const char* str){
            unsigned len = strlen(str);
            _payload.resize(_header.size + len);
            std::memcpy(_payload.data()+_header.size, str, len);
            _header.size += len;
            return *this;
        }
        Message& Message::operator<<(const std::string& str){
            return *this << str.c_str();
        }
        tcp::socket& operator<<(asio::ip::tcp::socket& sock, const Message& msg){
            asio::error_code ec;
            uint8_t buffer[sizeof(Header)];
            size_t headerSize =  msg._header.toBytes(buffer);
            if(ec = write_n(sock, headerSize, buffer)) throw ec;
            if(ec = write_n(sock, msg._header.size, msg._payload.data()));
            return sock;
        }

    } // namespace net
    
} // namespace Basalt
