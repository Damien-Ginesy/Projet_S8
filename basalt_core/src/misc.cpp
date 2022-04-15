#include "misc.h"

namespace Basalt{
    void toLittleEndian(uintmax_t x, unsigned size,uint8_t out[]){
        for (uintmax_t i = 0; i < size; i++)
        {
            out[i] = x & 0xff;
            x >>= 8;
        }
    }
    using namespace asio::ip;
    asio::error_code read_n(tcp::socket& sock, size_t n, uint8_t* out){
        uint32_t nRead=0;
        asio::error_code ec;
        while(nRead<n){
            sock.wait(sock.wait_read);
            nRead += sock.read_some(asio::buffer(out+nRead, n-nRead), ec);
            if(ec) break;
        }
        return ec;
    }
    asio::error_code write_n(tcp::socket& sock, size_t n, const uint8_t* in){
        uint32_t nWritten=0;
        asio::error_code ec;
        while(nWritten<n){
            sock.wait(sock.wait_write);
            nWritten += sock.write_some(asio::buffer(in+nWritten, n-nWritten), ec);
            if(ec) break;
        }
        return ec;
    }

}
