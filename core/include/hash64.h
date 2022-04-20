#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>


/* Simplified version of the fast-hash function, adapted for 64-bit inputs.
THe original is available here: https://github.com/ztanml/fast-hash */
uint64_t hash64(uint64_t x, uint64_t seed);

#ifdef __cplusplus
}
#endif
