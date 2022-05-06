#include "misc.h"

namespace Basalt{
    using namespace asio::ip;
    asio::error_code read_n(tcp::socket& sock, size_t n, void* out){
        uint32_t nRead=0;
        asio::error_code ec;
        while(nRead<n){
            sock.wait(sock.wait_read);
            nRead += sock.read_some(asio::buffer((uint8_t*)out+nRead, n-nRead), ec);
            if(ec) break;
        }
        return ec;
    }
    asio::error_code write_n(tcp::socket& sock, size_t n, const void* in){
        uint32_t nWritten=0;
        asio::error_code ec;
        while(nWritten<n){
            sock.wait(sock.wait_write);
            nWritten += sock.write_some(asio::buffer((const uint8_t*)in+nWritten, n-nWritten), ec);
            if(ec) break;
        }
        return ec;
    }

}
