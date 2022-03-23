#pragma once

#include <inttypes.h>
#include <stddef.h>

namespace Basalt
{
    using byte = uint8_t;
    /* Dumps an integer of any size in little endian order */
    void toLittleEndian(uintmax_t x, unsigned size,uint8_t out[]);
} // namespace Basalt
