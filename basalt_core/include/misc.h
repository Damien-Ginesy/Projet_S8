#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <asio/error_code.hpp>
#include <asio/ip/tcp.hpp>

namespace Basalt
{
    using byte = uint8_t;
    /* Dumps an integer of any size in little endian order */
    void toLittleEndian(uintmax_t x, unsigned size,uint8_t out[]);
    asio::error_code read_n(asio::ip::tcp::socket& sock, size_t n, uint8_t* out);
    asio::error_code write_n(asio::ip::tcp::socket& sock, size_t n, const uint8_t* in);
} // namespace Basalt
