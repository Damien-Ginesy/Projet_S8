#pragma once

#include "Hash.hpp"

struct SHA256Hash: public Hash<32>
{
    SHA256Hash(const void* in, size_t len);
};
