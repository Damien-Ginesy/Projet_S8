#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <asio/error_code.hpp>
#include <asio/ip/tcp.hpp>

namespace Basalt
{
    using byte = uint8_t;
    /* Dumps an integer of any size in little endian order */
    static inline uint32_t lendian32(uint32_t x){
        byte b[] = {
            (byte)(x&0xff),
            (byte)((x>>8)&0xff),
            (byte)((x>>16)&0xff),
            (byte)((x>>24)&0xff),
        };
        return *(uint32_t*)b;
    }
    static inline uint16_t lendian16(uint16_t x){
        byte b[] = {(byte)(x&0xff), (byte)(x>>8)};
        return *(uint16_t*)b;
    }
    asio::error_code read_n(asio::ip::tcp::socket& sock, size_t n, uint8_t* out);
    asio::error_code write_n(asio::ip::tcp::socket& sock, size_t n, const uint8_t* in);
} // namespace Basalt
