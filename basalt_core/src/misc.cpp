#include "misc.h"

namespace Basalt{
    void toLittleEndian(uintmax_t x, unsigned size,uint8_t out[]){
        for (uintmax_t i = 0; i < size; i++)
        {
            out[i] = x & 0xff;
            x >>= 8;
        }
    }
}
