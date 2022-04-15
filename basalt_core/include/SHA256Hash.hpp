#pragma once

#include "Hash.hpp"

/* Wrapper class for the SHA256 hash function */
struct SHA256Hash: public Hash<32>
{
    SHA256Hash(const void* in, size_t len);
};
