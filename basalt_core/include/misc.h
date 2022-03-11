#pragma once

#include <inttypes.h>

namespace Basalt
{
    using byte = uint8_t;
    void toLittleEndian(uintmax_t x, unsigned size,uint8_t out[]);
} // namespace Basalt
